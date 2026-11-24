#ifndef MOVIE_H
#define MOVIE_H

#include <queue>
#include <string>

using namespace std;

// Represents one movie screening with a fixed theater layout and a waiting queue.
class Movie {
public:
    static constexpr int ROWS = 9;  // rows A-I
    static constexpr int COLS = 9;  // columns 1-9

private:
    int id;
    string title;
    string genre;
    string showtime;
    char seats[ROWS][COLS];
    queue<string> waitingList;

public:
    Movie();
    Movie(int id, const string& title, const string& genre, const string& showtime);

    // Getters
    int getId() const;
    string getTitle() const;
    string getGenre() const;
    string getShowtime() const;

    // Waiting-list operations (queue front is the next customer to be served)
    const queue<string>& getWaitingList() const;
    void addToWaitingList(const string& customerName);
    string popWaitingList();
    int waitingCount() const;

    // Seat operations on the 2D char array
    char getSeat(int row, int col) const;
    void setSeat(int row, int col, char value);
    bool isValidSeat(int row, int col) const;
    bool isSeatAvailable(int row, int col) const;
    void bookSeat(int row, int col);
    void freeSeat(int row, int col);
    int availableSeats() const;
    bool isSoldOut() const;
    void displaySeatMap() const;

    // Persistence helpers: flatten the 2D array / queue into single strings.
    string serializeSeats() const;
    bool loadSeatsFromString(const string& data);
    string serializeWaitingList() const;
    bool loadWaitingListFromString(const string& data);

    // Convert "B4" <-> zero-based (row, col). Throws on bad input.
    static bool parseSeatCode(const string& code, int& row, int& col);
    static string makeSeatCode(int row, int col);
};

#endif