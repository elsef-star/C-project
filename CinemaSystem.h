#ifndef CINEMASYSTEM_H
#define CINEMASYSTEM_H

#include <map>
#include <string>
#include <vector>

#include "Movie.h"
#include "Ticket.h"

using namespace std;

// Owns all screenings, active bookings and provides the console menu.
class CinemaSystem {
private:
    vector<Movie> movies;
    map<int, Booking> bookings;
    string moviesFile;
    string bookingsFile;
    int nextBookingId;

    // Input helpers
    void showMenu() const;
    int readInt(const string& prompt) const;
    string readLine(const string& prompt) const;
    int readMenuChoice() const;
    void waitForEnter() const;

    // Menu actions (each one is a small, focused function)
    void handleViewMovies() const;
    void handleSearchMovies() const;
    void handleViewSeatMap() const;
    void handleBookTicket();
    void handleCancelTicket();
    void handleViewBookings() const;
    void handleAddMovie();

    // Lookup helpers
    Movie* findMovieById(int id);
    const Movie* findMovieById(int id) const;
    Booking* findBookingById(int id);
    vector<const Movie*> searchMoviesByTitle(const string& query) const;
    static string toLower(string text);
    static vector<string> split(const string& text, char delimiter);

    // Persistence
    void loadMovies();
    void saveMovies() const;
    void loadBookings();
    void saveBookings() const;
    void saveAll() const;
    void seedSampleData();
    void printMovieSummary(const Movie& movie) const;

public:
    CinemaSystem();
    void run();
};

#endif