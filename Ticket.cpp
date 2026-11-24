#include "Ticket.h"
#include <iostream>

using namespace std;

Booking::Booking()
    : bookingId(0), movieId(0), customerName(""), seatCode("") {}

Booking::Booking(int bookingId, int movieId, const string& customerName, const string& seatCode)
    : bookingId(bookingId), movieId(movieId), customerName(customerName), seatCode(seatCode) {}

int Booking::getBookingId() const { return bookingId; }
int Booking::getMovieId() const { return movieId; }
string Booking::getCustomerName() const { return customerName; }
string Booking::getSeatCode() const { return seatCode; }

void Booking::display() const {
    cout << "  Booking #" << bookingId
         << " | Movie ID: " << movieId
         << " | Customer: " << customerName
         << " | Seat: " << seatCode << "\n";
}