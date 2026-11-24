#ifndef TICKET_H
#define TICKET_H

#include <string>

using namespace std;

// Holds the details of one active ticket (booking).
class Booking {
private:
    int bookingId;
    int movieId;
    string customerName;
    string seatCode;

public:
    Booking();
    Booking(int bookingId, int movieId, const string& customerName, const string& seatCode);

    int getBookingId() const;
    int getMovieId() const;
    string getCustomerName() const;
    string getSeatCode() const;

    void display() const;
};

#endif