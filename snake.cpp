#include "snake.h"

const int W = 800;
const int H = 600;

int main() {
    Snake::Snake *s = new Snake::Snake();
    
    try {
        while (!(*s).exit_check()) {
            (*s).update();
        }
    } catch (const char *ted) { // because I like ted. no other reason
        std::cout << ted;
    }

    delete s; // this is normally assumed, I'm declairing explicitly because you
    // can never know when things will misbehave

    return 0; 
}

// Author: William Tradewell - ChronicEel93