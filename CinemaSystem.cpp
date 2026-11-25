#include "CinemaSystem.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

static string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

CinemaSystem::CinemaSystem()
    : moviesFile("movies.txt"), bookingsFile("bookings.txt"), nextBookingId(1) {
    loadMovies();
    loadBookings();
    if (movies.empty()) {
        seedSampleData();
    }
}

// ---------------------------------------------------------------------------
// Input helpers
// ---------------------------------------------------------------------------

string CinemaSystem::readLine(const string& prompt) const {
    cout << prompt;
    string line;
    getline(cin, line);
    return line;
}

int CinemaSystem::readInt(const string& prompt) const {
    string line = readLine(prompt);
    size_t processed = 0;
    int value = stoi(line, &processed);
    if (processed != line.size()) {
        throw invalid_argument("Please enter a whole number only.");
    }
    return value;
}

int CinemaSystem::readMenuChoice() const {
    try {
        return readInt("Enter your choice (1-8): ");
    } catch (const exception& e) {
        cout << "  Invalid input: " << e.what() << "\n";
        return -1;
    }
}

void CinemaSystem::waitForEnter() const {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void CinemaSystem::showMenu() const {
    cout << "\n========== CINEMA TICKET BOOKING SYSTEM ==========\n"
         << "  1. View all movies & showtimes\n"
         << "  2. Search movie by title / genre\n"
         << "  3. View seat map for a movie\n"
         << "  4. Book a ticket\n"
         << "  5. Cancel a ticket\n"
         << "  6. View all bookings & waiting lists\n"
         << "  7. Add a new movie screening (admin)\n"
         << "  8. Save and exit\n"
         << "==================================================\n";
}

// ---------------------------------------------------------------------------
// Lookup & string helpers
// ---------------------------------------------------------------------------

Movie* CinemaSystem::findMovieById(int id) {
    for (Movie& movie : movies) {
        if (movie.getId() == id) return &movie;
    }
    return nullptr;
}

const Movie* CinemaSystem::findMovieById(int id) const {
    for (const Movie& movie : movies) {
        if (movie.getId() == id) return &movie;
    }
    return nullptr;
}

Booking* CinemaSystem::findBookingById(int id) {
    auto it = bookings.find(id);
    return (it == bookings.end()) ? nullptr : &(it->second);
}

string CinemaSystem::toLower(string text) {
    for (char& ch : text) {
        ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    }
    return text;
}

vector<const Movie*> CinemaSystem::searchMoviesByTitle(const string& query) const {
    vector<const Movie*> results;
    string needle = toLower(trim(query));
    if (needle.empty()) return results;

    for (const Movie& movie : movies) {
        if (toLower(movie.getTitle()).find(needle) != string::npos ||
            toLower(movie.getGenre()).find(needle) != string::npos) {
            results.push_back(&movie);
        }
    }
    return results;
}

vector<string> CinemaSystem::split(const string& text, char delimiter) {
    vector<string> fields;
    size_t start = 0, pos = text.find(delimiter);
    while (pos != string::npos) {
        fields.push_back(trim(text.substr(start, pos - start)));
        start = pos + 1;
        pos = text.find(delimiter, start);
    }
    fields.push_back(trim(text.substr(start)));
    return fields;
}

void CinemaSystem::printMovieSummary(const Movie& movie) const {
    cout << "  Movie #" << movie.getId()
         << " | " << movie.getTitle()
         << " | " << movie.getGenre()
         << " | " << movie.getShowtime()
         << " | Seats left: " << movie.availableSeats()
         << "/" << Movie::ROWS * Movie::COLS
         << " | Waiting: " << movie.waitingCount() << "\n";
}

// ---------------------------------------------------------------------------
// Menu actions
// ---------------------------------------------------------------------------

void CinemaSystem::handleViewMovies() const {
    cout << "\n--- All Movies & Showtimes ---\n";
    if (movies.empty()) {
        cout << "  No screenings available.\n";
        return;
    }
    for (const Movie& movie : movies) {
        printMovieSummary(movie);
    }
}

void CinemaSystem::handleSearchMovies() const {
    string query = readLine("Enter title or genre keyword: ");
    vector<const Movie*> results = searchMoviesByTitle(query);
    cout << "\n--- Search Results for \"" << query << "\" ---\n";
    if (results.empty()) {
        cout << "  No movies matched your search.\n";
        return;
    }
    for (const Movie* movie : results) {
        printMovieSummary(*movie);
    }
}

void CinemaSystem::handleViewSeatMap() const {
    int id = readInt("Enter movie ID to view its seat map: ");
    const Movie* movie = findMovieById(id);
    if (movie == nullptr) {
        throw runtime_error("No movie found with ID " + to_string(id) + ".");
    }
    cout << "\nSeat map for \"" << movie->getTitle() << "\" (Movie #" << movie->getId() << ")\n"
         << "Showtime: " << movie->getShowtime() << "\n";
    movie->displaySeatMap();
}

void CinemaSystem::handleBookTicket() {
    int movieId = readInt("Enter movie ID to book: ");
    Movie* movie = findMovieById(movieId);
    if (movie == nullptr) {
        throw runtime_error("No movie found with ID " + to_string(movieId) + ".");
    }

    string customerName = trim(readLine("Enter customer name: "));
    if (customerName.empty()) {
        throw invalid_argument("Customer name cannot be empty.");
    }

    if (movie->isSoldOut()) {
        cout << "Sorry, \"" << movie->getTitle() << "\" is sold out.\n";
        string answer = toLower(trim(readLine("Join the waiting list? (y/n): ")));
        if (answer == "y" || answer == "yes") {
            movie->addToWaitingList(customerName);
            saveAll();
            cout << "Added to the waiting list at position " << movie->waitingCount() << ".\n";
        } else {
            cout << "No waiting-list entry created.\n";
        }
        return;
    }

    // Shows the seating map of the room before selecting a ticket
    movie->displaySeatMap();

    string seatCode = trim(readLine("Enter seat code (e.g. B4): "));
    int row = 0, col = 0;
    Movie::parseSeatCode(seatCode, row, col);

    if (!movie->isSeatAvailable(row, col)) {
        cout << "Seat " << seatCode << " is already booked. Please choose another.\n";
        return;
    }

    movie->bookSeat(row, col);
    int bookingId = nextBookingId++;
    bookings[bookingId] = Booking(bookingId, movieId, customerName, Movie::makeSeatCode(row, col));
    saveAll();
    cout << "Booking confirmed! Booking #" << bookingId
         << " for " << customerName << " at seat " << seatCode << ".\n";
}

void CinemaSystem::handleCancelTicket() {
    int bookingId = readInt("Enter booking ID to cancel: ");
    Booking* booking = findBookingById(bookingId);
    if (booking == nullptr) {
        throw runtime_error("No active booking found with ID " + to_string(bookingId) + ".");
    }

    int movieId = booking->getMovieId();
    string seatCode = booking->getSeatCode();
    Movie* movie = findMovieById(movieId);
    if (movie == nullptr) {
        throw runtime_error("The movie for this booking no longer exists.");
    }

    int row = 0, col = 0;
    Movie::parseSeatCode(seatCode, row, col);
    movie->freeSeat(row, col);
    bookings.erase(bookingId);

    if (movie->waitingCount() > 0) {
        string nextCustomer = movie->popWaitingList();
        movie->bookSeat(row, col);
        int newBookingId = nextBookingId++;
        bookings[newBookingId] = Booking(newBookingId, movieId, nextCustomer, seatCode);
        cout << "Booking #" << bookingId << " cancelled. Seat " << seatCode
             << " was auto-assigned to " << nextCustomer
             << " from the waiting list (new booking #" << newBookingId << ").\n";
    } else {
        cout << "Booking #" << bookingId << " cancelled. Seat " << seatCode << " is now available.\n";
    }
    saveAll();
}

void CinemaSystem::handleViewBookings() const {
    cout << "\n--- Active Bookings ---\n";
    if (bookings.empty()) {
        cout << "  No active bookings.\n";
    } else {
        for (const auto& entry : bookings) {
            entry.second.display();
        }
    }

    cout << "\n--- Waiting Lists ---\n";
    for (const Movie& movie : movies) {
        if (movie.waitingCount() == 0) continue;
        cout << "  Movie #" << movie.getId() << " (" << movie.getTitle() << "):\n";
        queue<string> copy = movie.getWaitingList();
        int position = 1;
        while (!copy.empty()) {
            cout << "    " << position++ << ". " << copy.front() << "\n";
            copy.pop();
        }
    }
}

void CinemaSystem::handleAddMovie() {
    int id = readInt("Enter new movie ID: ");
    if (findMovieById(id) != nullptr) {
        throw runtime_error("A movie with ID " + to_string(id) + " already exists.");
    }
    string title = trim(readLine("Enter title: "));
    string genre = trim(readLine("Enter genre: "));
    string showtime = trim(readLine("Enter showtime (e.g. 19:00-22:00): "));
    if (title.empty() || genre.empty() || showtime.empty()) {
        throw invalid_argument("Title, genre and showtime cannot be empty.");
    }
    movies.push_back(Movie(id, title, genre, showtime));
    saveAll();
    cout << "Movie \"" << title << "\" added successfully.\n";
}

// ---------------------------------------------------------------------------
// File I/O
// ---------------------------------------------------------------------------

void CinemaSystem::loadMovies() {
    try {
        ifstream file(moviesFile);
        if (!file.is_open()) throw runtime_error("Could not open " + moviesFile);

        string line;
        while (getline(file, line)) {
            line = trim(line);
            if (line.empty()) continue;

            // Format: id|title|genre|showtime|waitingList
            vector<string> fields = split(line, '|');
            if (fields.size() < 5) continue;

            Movie movie(stoi(fields[0]), fields[1], fields[2], fields[3]);
            movie.loadWaitingListFromString(fields[4]);

            // Read seat map row by row (ROWS x COLS)
            for (int r = 0; r < Movie::ROWS; ++r) {
                string seatRow;
                while (getline(file, seatRow)) {
                    seatRow = trim(seatRow);
                    if (!seatRow.empty()) break;
                }
                if (static_cast<int>(seatRow.size()) != Movie::COLS) {
                    throw invalid_argument("Seat row length does not match column count.");
                }
                for (int c = 0; c < Movie::COLS; ++c) {
                    if (seatRow[c] != 'O' && seatRow[c] != 'X') {
                        throw invalid_argument("Invalid character in seat map.");
                    }
                    movie.setSeat(r, c, seatRow[c]);
                }
            }

            movies.push_back(movie);
        }
    } catch (const exception& e) {
        cerr << "Warning loading movies: " << e.what() << " (starting with default data).\n";
    }
}

void CinemaSystem::saveMovies() const {
    try {
        ofstream file(moviesFile);
        if (!file.is_open()) throw runtime_error("Could not open " + moviesFile);

        for (const Movie& movie : movies) {
            file << movie.getId() << '|' << movie.getTitle() << '|' << movie.getGenre()
                 << '|' << movie.getShowtime() << '|' << movie.serializeWaitingList() << '\n';

            // Write seat map row by row
            for (int r = 0; r < Movie::ROWS; ++r) {
                for (int c = 0; c < Movie::COLS; ++c) {
                    file << movie.getSeat(r, c);
                }
                file << '\n';
            }
            file << '\n';
        }
    } catch (const exception& e) {
        cerr << "Warning saving movies: " << e.what() << "\n";
    }
}

void CinemaSystem::loadBookings() {
    try {
        ifstream file(bookingsFile);
        if (!file.is_open()) throw runtime_error("Could not open " + bookingsFile);

        string line;
        while (getline(file, line)) {
            if (trim(line).empty()) continue;
            vector<string> fields = split(line, '|');
            if (fields.size() < 4) continue;

            int bookingId = stoi(fields[0]);
            int movieId = stoi(fields[1]);
            bookings[bookingId] = Booking(bookingId, movieId, fields[2], fields[3]);
            nextBookingId = max(nextBookingId, bookingId + 1);

            Movie* movie = findMovieById(movieId);
            if (movie != nullptr) {
                int row = 0, col = 0;
                Movie::parseSeatCode(fields[3], row, col);
                movie->setSeat(row, col, 'X');
            }
        }
    } catch (const exception& e) {
        cerr << "Warning loading bookings: " << e.what() << "\n";
    }
}

void CinemaSystem::saveBookings() const {
    try {
        ofstream file(bookingsFile);
        if (!file.is_open()) throw runtime_error("Could not open " + bookingsFile);

        for (const auto& entry : bookings) {
            const Booking& b = entry.second;
            file << b.getBookingId() << '|' << b.getMovieId() << '|'
                 << b.getCustomerName() << '|' << b.getSeatCode() << '\n';
        }
    } catch (const exception& e) {
        cerr << "Warning saving bookings: " << e.what() << "\n";
    }
}

void CinemaSystem::saveAll() const {
    saveMovies();
    saveBookings();
}

void CinemaSystem::seedSampleData() {
    movies.push_back(Movie(1, "The Prestiege", "Sci-Fi", "18:00-21:00"));
    movies.push_back(Movie(2, "Your name", "Romance", "15:30-17:30"));
    movies.push_back(Movie(3, "The Matrix", "Action", "20:15-23:00"));
    movies.push_back(Movie(4, "The Gladiator", "Historical", "20:15-23:00"));

    
    movies[0].bookSeat(0, 0); // A1
    bookings[nextBookingId] = Booking(nextBookingId, 1, "Ali Mammadov", "A1");
    nextBookingId++;

    movies[0].bookSeat(1, 3); // B4
    bookings[nextBookingId] = Booking(nextBookingId, 1, "Nigar Karimova", "B4");
    nextBookingId++;

    movies[1].bookSeat(2, 4); // C5
    bookings[nextBookingId] = Booking(nextBookingId, 2, "Tural Aliyev", "C5");
    nextBookingId++;
    movies[2].bookSeat(6, 4); // G5
    bookings[nextBookingId] = Booking(nextBookingId, 3, "Nusret Behbudlu", "G5");
    nextBookingId++;

    
    movies[1].addToWaitingList("Leyla Hasanova");
    movies[1].addToWaitingList("Kamran Guliyev");
    movies[1].addToWaitingList("Ahmad Hasanli");

    
    saveAll();
}



// ---------------------------------------------------------------------------
// Main loop
// ---------------------------------------------------------------------------

void CinemaSystem::run() {
    bool running = true;
    while (running) {
        showMenu();
        int choice = readMenuChoice();
        try {
            switch (choice) {
                case 1: handleViewMovies(); break;
                case 2: handleSearchMovies(); break;
                case 3: handleViewSeatMap(); break;
                case 4: handleBookTicket(); break;
                case 5: handleCancelTicket(); break;
                case 6: handleViewBookings(); break;
                case 7: handleAddMovie(); break;
                case 8:
                    saveAll();
                    cout << "All data saved. Goodbye!\n";
                    running = false;
                    break;
                default:
                    cout << "Please choose an option from 1 to 8.\n";
                    break;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << "\n";
        }
        if (running) waitForEnter();
    }
}