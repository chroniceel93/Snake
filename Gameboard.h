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
 * The 1st vertical line is reserved for scoreboard functionality,
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

        SDL_Window *window; // SDL window object
        SDL_Texture *render_texture; // SDL texture object (this holds gameboard
                                    // state)
        SDL_Renderer *renderer; // SDL renderer object (this draws to things)
        SDL_Color background;  // background color
        SDL_Event input; // input queue

        KeyPressed mem; 

    public:

        // Constructor
        Gameboard() ;

/**
 * bool status();
 * 
 * Returns SDL_Status, used to determine whether SDL initialized correctly
 * from outside of the class
 * 
 * return bool
 **/
        bool status();

/**
 * void blank_screen()
 * 
 * re-draws entire texture with background color to reset apparant game state
 **/
        void blank_screen();

/**
 * void draw_block()
 * 
 * Takes a co-ordinate pair and a color, and draws a block of that color,
 * at that co-ordinate.
 * 
 * Assumes that the co-ordinates conform to an 80x60 grid.
 **/
        void draw_block( int xpos
            , int ypos
            , unsigned char r
            , unsigned char g
            , unsigned char b);

/**
 * void input_reset()
 * 
 * Simple utility function that resets mem to default value.
 **/
        void input_reset();

/**
 * void set_bg_color()
 * 
 * Simple utility function that updates the background color.
 **/
        void set_bg_color(unsigned char r, unsigned char g, unsigned char b) ;

/**
 * void update_screen()
 * 
 * copies the texture to the screen surface and presents that surface
 **/
        void update_screen();
        
/**
 * KeyPressed update_input();
 * 
 * Runs through the SDL_Event queue, and returns the inputs for me.
 * 
 * return mem (the valid input)
 **/
        KeyPressed update_input();

        ~Gameboard() ;
    };
};

#endif