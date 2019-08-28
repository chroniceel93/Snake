#include "snake.h"

const int W = 800;
const int H = 600;

int main() {

    Snake::Snake *s = new Snake::Snake();
    // no idea of the try-catch attempt actually *works*
    try {
        while (!(*s).exit_check()) {
            (*s).update();
        }
    } catch (std::string const &s) {
        std::cout << s;
    }

    delete s; // this is normally assumed, I'm declairing explicitly because you
    // can never know when a compiler will misbehave 
    // and leave a class object dead in-memory.

    return 0;
}

// TODO: ticker class?

// Author: William Tradewell - ChronicEel93