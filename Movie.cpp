#include "Movie.h"

#include <cctype>
#include <iostream>
#include <stdexcept>

using namespace std;

Movie::Movie() : id(0), title(""), genre(""), showtime("") {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            seats[r][c] = 'O';
        }
    }
}

Movie::Movie(int id, const string& title, const string& genre, const string& showtime)
    : id(id), title(title), genre(genre), showtime(showtime) {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            seats[r][c] = 'O';
        }
    }
}

int Movie::getId() const { return id; }
string Movie::getTitle() const { return title; }
string Movie::getGenre() const { return genre; }
string Movie::getShowtime() const { return showtime; }

const queue<string>& Movie::getWaitingList() const { return waitingList; }

void Movie::addToWaitingList(const string& customerName) {
    waitingList.push(customerName);
}

string Movie::popWaitingList() {
    if (waitingList.empty()) {
        throw out_of_range("Waiting list is empty.");
    }
    string next = waitingList.front();
    waitingList.pop();
    return next;
}

int Movie::waitingCount() const {
    return static_cast<int>(waitingList.size());
}

bool Movie::isValidSeat(int row, int col) const {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

char Movie::getSeat(int row, int col) const {
    if (!isValidSeat(row, col)) {
        throw out_of_range("Seat coordinates are outside the theater layout.");
    }
    return seats[row][col];
}

void Movie::setSeat(int row, int col, char value) {
    if (!isValidSeat(row, col)) {
        throw out_of_range("Seat coordinates are outside the theater layout.");
    }
    seats[row][col] = value;
}

bool Movie::isSeatAvailable(int row, int col) const {
    return getSeat(row, col) == 'O';
}

void Movie::bookSeat(int row, int col) {
    if (!isSeatAvailable(row, col)) {
        throw runtime_error("That seat is already booked.");
    }
    seats[row][col] = 'X';
}

void Movie::freeSeat(int row, int col) {
    if (!isValidSeat(row, col)) {
        throw out_of_range("Seat coordinates are outside the theater layout.");
    }
    seats[row][col] = 'O';
}

int Movie::availableSeats() const {
    int count = 0;
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (seats[r][c] == 'O') ++count;
        }
    }
    return count;
}

bool Movie::isSoldOut() const {
    return availableSeats() == 0;
}

void Movie::displaySeatMap() const {
    cout << "\n      ";
    for (int c = 0; c < COLS; ++c) {
        cout << "   " << (c + 1);
    }
    cout << "\n      ";
    for (int c = 0; c < COLS; ++c) {
        cout << " ---";
    }
    cout << "\n";

    for (int r = 0; r < ROWS; ++r) {
        cout << "   " << static_cast<char>('A' + r) << " |";
        for (int c = 0; c < COLS; ++c) {
            cout << "   " << seats[r][c];
        }
        cout << "\n";
    }
    cout << "\n   Legend: O = available, X = booked\n";
}

string Movie::serializeSeats() const {
    string data;
    data.reserve(ROWS * COLS);
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            data += seats[r][c];
        }
    }
    return data;
}

bool Movie::loadSeatsFromString(const string& data) {
    if (static_cast<int>(data.size()) != ROWS * COLS) {
        throw invalid_argument("Saved seat map has the wrong size.");
    }
    int index = 0;
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            char value = data[index++];
            if (value != 'O' && value != 'X') {
                throw invalid_argument("Saved seat map contains invalid characters.");
            }
            seats[r][c] = value;
        }
    }
    return true;
}

string Movie::serializeWaitingList() const {
    string data;
    queue<string> copy = waitingList;
    while (!copy.empty()) {
        if (!data.empty()) data += ";";
        data += copy.front();
        copy.pop();
    }
    return data;
}

bool Movie::loadWaitingListFromString(const string& data) {
    size_t start = 0;
    size_t pos = data.find(';', start);
    while (pos != string::npos) {
        string name = data.substr(start, pos - start);
        if (!name.empty()) waitingList.push(name);
        start = pos + 1;
        pos = data.find(';', start);
    }
    string last = data.substr(start);
    if (!last.empty()) waitingList.push(last);
    return true;
}

bool Movie::parseSeatCode(const string& code, int& row, int& col) {
    if (code.size() < 2) {
        throw invalid_argument("Seat code must be a letter followed by a number, e.g. B4.");
    }
    char letter = static_cast<char>(toupper(static_cast<unsigned char>(code[0])));
    if (letter < 'A' || letter > 'A' + ROWS - 1) {
        throw out_of_range("Seat row is out of range.");
    }
    int colNum = 0;
    size_t processed = 0;
    try {
        colNum = stoi(code.substr(1), &processed);
    } catch (...) {
        throw invalid_argument("Invalid seat column number.");
    }
    if (processed != code.substr(1).size() || colNum < 1 || colNum > COLS) {
        throw out_of_range("Seat column is out of range.");
    }
    row = letter - 'A';
    col = colNum - 1;
    return true;
}

string Movie::makeSeatCode(int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        throw out_of_range("Cannot build a seat code from invalid coordinates.");
    }
    return string(1, static_cast<char>('A' + row)) + to_string(col + 1);
}