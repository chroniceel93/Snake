#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

// TODO: Breaking gameboard class while porting
// SDL Handling from main into gameboard class.
// Gameboard class is being upgraded from a partial
// SDL interface class, to a full SDL interface class.



namespace Game {
// translates SDL keycodes into something my code understands...
// will keep, likely makes it more portable... if I want to gut sdl code...
// input and most logic in other games will still be usable.
    enum KeyPressed {
        k_up,
        k_right,
        k_down,
        k_left,
        k_space
    };

/**
 * class Gameboard
 *
 * Used to draw a 10x10 pixel of a given color at a given position (trans-
 * lated into a 80x56 co-ordinate grid)
 *
 * We are assuming that the screen size is 800x600, will work on decoupling
 * resolution later
 *
 * There is a 60th vertical line that is reserved for scoreboard functionality,
 * will be implemented later
 */
    class Gameboard {
    private: 
        // these variables will be used once I implement logic to determine
        // pixel size given arbitrary screen size.
        int bloc_height; // how high the block is
        int bloc_width; // how wide a block is

        const int screen_width = 800;
        const int screen_height = 600;

        bool SDL_Status; // keep track of sdl status.

        SDL_Window *window;
        SDL_Surface *surface;
        SDL_Renderer *renderer;
        SDL_Color background;
        SDL_Event input;

        KeyPressed mem;

    public:

        // Constructors
        Gameboard() ;

        bool status();

        void blank_screen();
        void draw_block( int xpos
            , int ypos
            , unsigned char r
            , unsigned char g
            , unsigned char b);
        void set_bg_color(unsigned char r, unsigned char g, unsigned char b) ;
        void updateScreen();

        KeyPressed update_input();

        ~Gameboard() ;
    };
};

#endif

/**
 * Gameboard function list:
 * 
 * bool update_input()
 *     handles input, sets KeyPressed with input from SDL_Event
 * 
 * void blank_screen()
 *     draws entire screen with background color
 * 
 * void draw_block()
 *     draws a block (10x10) at the given coordinates with
 *     the given colors
 * 
 * void set_bg_color()
 *     change the background color
 * 
 * void throwSDLError()
 * 
 * void updateScreen();
 *     present sdl renderer
 *
 */