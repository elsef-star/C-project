========================================================================
CINEMA TICKET BOOKING SYSTEM
Programming Language (C++) - Final Project
========================================================================

STUDENT INFORMATION
-------------------
Name & Surname : Elsəf Bağırov
Student ID     : 250108032
Course         : Programming Language (C++)
Instructor     : Samir Badiyev
Major          : Computer Science

PROJECT DESCRIPTION
-------------------
The Cinema Ticket Booking System is an object-oriented, menu-driven C++
console application that manages movie screenings, real-time 9x9 seat
reservations, ticket cancellations, and customer waiting lists for
sold-out sessions. Movie records, 9x9 seat matrices, active bookings, and
waiting queues are automatically loaded from 'movies.txt' and
'bookings.txt' at startup and saved back to disk upon changes and exit.


FILES INCLUDED
--------------
- main.cpp                        : Program entry point  with
                                    top-level exception handling.
- Movie.h / Movie.cpp             : Defines the Movie entity class,
                                    storing movie metadata, the 9x9 2D
                                    seat array (rows A-I, columns 1-9),
                                    seat code parsing, and the
                                    std::queue waiting list.
- Booking.h / Booking.cpp         : Defines the Booking entity class,
                                    representing individual ticket
                                    reservations (booking ID, movie ID,
                                    customer name, and seat code).
- CinemaSystem.h / CinemaSystem.cpp : Defines the main controller class
                                    managing std::vector<Movie>,
                                    std::map<int, Booking>, menu flow,
                                    pointer lookups, automatic queue
                                    reassignment, and File I/O.
- movies.txt                      : Data file storing movie details,
                                    waiting lists, and 9x9 seat matrices
                                    ('O' = available, 'X' = booked).
- bookings.txt                    : Data file storing active ticket
                                    reservations.
- README.txt                      : Project documentation, build guide,
                                    and AI use acknowledgment.


HOW TO COMPILE AND RUN
----------------------
Ensure all .cpp, .h, and .txt files are in the same directory.

1. Compile using g++ (C++17 standard):
   Windows:
     g++ -std=c++17 *.cpp -o cinema.exe

   Mac / Linux:
     g++ -std=c++17 *.cpp -o cinema

2. Run the executable:
   Windows:
     .\cinema.exe

   Mac / Linux:
     ./cinema


LIST OF FEATURES
----------------
1. Display All Movies         : Lists all screenings with ID, title,
                                genre, showtime, available seat count
                                out of 81, and waiting list size.
2. View Seat Map              : Displays the 9x9 visual theater seating
                                grid (rows A-I, columns 1-9; 'O' =
                                available, 'X' = booked).
3. Search Movie by Title      : Performs case-insensitive partial title
                                matching using std::string::find().
4. Book a Ticket              : Displays the current 9x9 seat map,
                                validates the chosen seat code (e.g. B4),
                                and confirms booking. If sold out, offers
                                to add the customer to the FIFO waiting
                                queue.
5. Cancel a Ticket            : Cancels an existing booking by ID. If
                                customers are in the waiting queue,
                                automatically assigns the freed seat to
                                the first person in line.
6. View All Bookings & Queues : Displays all active ticket reservations
                                and current customer waiting lists.
7. Add New Movie Screening    : Allows adding a new movie screening with
                                duplicate ID validation.
8. Save and Exit              : Saves all updated movies, seat maps,
                                bookings, and queues to 'movies.txt' and
                                'bookings.txt' before exiting safely.


COURSE REQUIREMENTS MAPPING
---------------------------
- Control Flow (Week 4)          : while loop and switch menu in
                                   CinemaSystem::run(); if/else and
                                   nested for loops across Movie and
                                   CinemaSystem.
- Functions (Week 5)             : Modular member functions and input
                                   helpers keeping main() concise.
- Pointers & References (Week 6) : Pass-by-const-reference (const Movie&,
                                   const Booking&, const string&) and
                                   pointer lookups findMovieById() and
                                   findBookingById() returning pointers
                                   or nullptr.
- Arrays & Strings (Week 7)      : 2D array char seats[9][9] in Movie and
                                   case-insensitive string::find() in
                                   searchMoviesByTitle().
- STL Containers (Week 8)        : std::vector<Movie> for movie screenings
                                   and std::map<int, Booking> for active
                                   reservations in CinemaSystem.
- Stack or Queue (Week 9)        : std::queue<std::string> managing FIFO
                                   customer waiting lists in Movie and
                                   auto-assignment upon cancellation.
- Exception Handling (Week 10)   : try/catch blocks in CinemaSystem and
                                   main.cpp, throwing invalid_argument,
                                   out_of_range, and runtime_error on
                                   invalid input, seat bounds, or file
                                   errors.
- Classes (Week 11)              : Three encapsulated classes (Movie,
                                   Booking, CinemaSystem) with private
                                   members, constructors, getters/setters,
                                   and methods.
- File I/O (Week 12)             : ifstream loading in loadMovies() and
                                   loadBookings(); ofstream saving in
                                   saveMovies() and saveBookings().


ACADEMIC INTEGRITY & AI USE STATEMENT
-------------------------------------
1. AI Tools Used:
   I used Google Gemini while developing this project.

2. Purpose of Use:
   - Google Gemini was used to align the architecture with course
     requirements, customize the 9x9 matrix formatting in both console
     output and 'movies.txt', debug input/file parsing edge cases, and
     review code logic.

3. Code Originating from AI & Student Verification:
   The source files (Movie.h, Movie.cpp, Booking.h, Booking.cpp,
   CinemaSystem.h, CinemaSystem.cpp, and main.cpp) were created with AI
   assistance and subsequently customized, compiled, and tested by me
   across all menu features, file persistence, and exception cases.

4. External Websites / Tutorials Used:
   - C++ Standard Library reference (cppreference.com) for std::queue,
     std::vector, std::map, std::string::find, and <fstream> usage.
========================================================================