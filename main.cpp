#include <iostream>
#include "CinemaSystem.h"

using namespace std;

int main() {
    try {
        CinemaSystem cinema;
        cinema.run();
    } catch (const exception& error) {
        cerr << "Fatal error: " << error.what() << "\n";
        return 1;
    }
    return 0;
}