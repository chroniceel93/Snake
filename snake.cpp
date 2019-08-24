#include "snake.h"

const int W = 800;
const int H = 600;

int main() {

    Snake::Snake *s = new Snake::Snake();

    while (!(*s).exit_check()) {
        (*s).update();
    }

    delete s; // this is normally assumed, I'm declairing explicitly because you
    // can never know when a compiler will misbehave and leave a class object dead in-memory.

    return 0;
}



// TODO: Exit condition in main loop. 
// TODO: General input, up, down, left, right wired in. 

// Author: William Tradewell - ChronicEel93