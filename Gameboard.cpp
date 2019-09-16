#include "Gameboard.h"

// Constructor
Game::Gameboard::Gameboard() {
    SDL_Status = true; // assume sdl will work, unless it does not

    // default background color (black)
    background.r = 0;
    background.g = 0;
    background.b = 0;

    text.r = 255;
    text.g = 255;
    text.b = 255;

    bloc_width = 10;
    bloc_height = 10;

    mem = k_right; // default key

    // init sdl

    // if sdl fails to initialize, throw the results of SDL_GetError and set
    // sdl error state
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDL_GetError();
        SDL_Status = false;
    } else {
        // create window
        window = SDL_CreateWindow(
                "Snake"
                , SDL_WINDOWPOS_CENTERED
                , SDL_WINDOWPOS_CENTERED
                , screen_width
                , screen_height
                , SDL_WINDOW_SHOWN);
        // if the SDL_CreateWindow fails, then the pointer *window will be
        // null. so, if window is nullptr, then SDL_CreateWindow failed,
        // throw the appropriate answer and set sdl error state.
        if (window == nullptr) {
            throw SDL_GetError();
            SDL_Status = false;
        } else {
            // attempt to create a renderer
            renderer = SDL_CreateRenderer(
                window
                , -1
                , SDL_RENDERER_ACCELERATED);
            // as with SDL_CreateWindow, if SDL_CreateRenderer failes, then
            // *renderer will be a nullptr, and so if *renderer is a nullptr
            // then set sdl state and throw an error
            if (renderer == nullptr) {
                throw SDL_GetError();
                SDL_Status = false;
            } else {
                if (TTF_Init() < 0 ) { // initialize SDL_TTF
                    throw SDL_GetError();
                    SDL_Status = false;
                } else {
                    // load our font
                    font = TTF_OpenFont("Inconsolata.ttf", 14);
                    if (font == nullptr) {
                        throw SDL_GetError();
                        SDL_Status = false;
                        // because of my problems with ttf stuffs, I now know
                        // how to get the try-catch block to work.
                    }
                    // set draw color to bg, and...
                    SDL_SetRenderDrawColor(
                        renderer
                        , background.r
                        , background.g
                        , background.b
                        , 255);

                    // init texture
                    render_texture = SDL_CreateTexture(renderer
                        , SDL_PIXELFORMAT_RGBA8888
                        , SDL_TEXTUREACCESS_TARGET
                        , 800
                        , 600);

                    // set texture as render target
                    SDL_SetRenderTarget(renderer, render_texture);

                    // calls blank_screen() to color the texture to match
                    // background color
                    blank_screen();
                    
                    // present the renderer!
                    SDL_RenderPresent(renderer);
                }
            }
        }
    }
}

/**
 * bool status();
 * 
 * Returns SDL_Status, used to determine whether SDL initialized correctly
 * from outside of the class
 * 
 * return bool
 **/
bool Game::Gameboard::status() {
    return SDL_Status;
}

/**
 * void blank_screen()
 * 
 * re-draws entire texture with background color. This should be called if
 * restarting game logic
 **/
void Game::Gameboard::blank_screen() {
    // create a rect that covers the render texture
    SDL_Rect temp;
    temp.w = 800;
    temp.h = 600;
    temp.x = 0;
    temp.y = 0;
    // set renderer such that it matches the background color
    SDL_SetRenderDrawColor(renderer
        , background.r
        , background.g
        , background.b
        , 255);
    // fill render texture with background color
    SDL_RenderFillRect(renderer, &temp);
    // now whenever SDL_ScreenPresent is called, it will be 'blank'
    return;
}

/**
 * void draw_block()
 * 
 * Takes a co-ordinate pair and a color, and draws a block of that color,
 * at that co-ordinate.
 * 
 * Assumes that the co-ordinates conform to an 80x60 grid.
 **/
void Game::Gameboard::draw_block( int xpos
    , int ypos
    , unsigned char r
    , unsigned char g
    , unsigned char b) {
    // sanity check, Y cannot be greater than 59 and
    // X cannot be greater than 80, neither can be less than 0.
    // Y should be at offset for scoreboard line
    if (xpos > 80) {
        // subtract 80 from xpos until it is in bounds
        // this lets it 'wrap around' in extreme cases
        while (xpos > 80) {
            xpos = xpos - 80;
        }
    } else if (xpos < 0) {
        while (xpos < 0) {
            xpos = xpos + 80;
        }
    } // else do nothing

    if (ypos > 59) {
        while (ypos > 80) {
            ypos = ypos - 59;
        }
    } else if (ypos < 0) {
        while (ypos < 0) {
            ypos = ypos + 59;
        }
    } // else do nothing

    SDL_Rect temp; // temp rect used to draw the block
    // set the values in temp to match those given
    temp.w = bloc_width;
    temp.h = bloc_height;
    // xpos and ypos are modified to scale up from 80x60 grid to
    // 800x600 texture
    temp.x = (xpos * 10) - 10;
    temp.y = (ypos * 10); // skip subtractin 10 so ypos leaves line for
                            // scoreboard
    // set draw color to that given
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    // render rect to texture.
    SDL_RenderFillRect(renderer, &temp);
    // set draw color back to bg (good idea to do this in every case,
    // even if other functions that require it to be set to bg, set it to
    // bg just in case)
    SDL_SetRenderDrawColor(
        renderer
        ,background.r
        ,background.g
        ,background.b
        ,255);
    return;
}

void Game::Gameboard::draw_text ( int xpos
    , int ypos
    , std::string input) {

    SDL_Rect temp; // holds dimensions of text box

    // It's generally really really bad to use cstrings. No choice.
    SDL_Surface *text_surface = TTF_RenderText_Shaded( font
                                                    , input.c_str() 
                                                    , text
                                                    , background);
    if (text_surface == nullptr) {
        throw "Failed to load text to surface";
    } else {
        temp.w = text_surface->w;
        temp.h = text_surface->h;
        temp.x = xpos;
        temp.y = ypos;

        // clear space where text will be rendererd
        SDL_RenderFillRect(renderer, &temp);

        // render text
        SDL_Texture *text_swap = SDL_CreateTextureFromSurface(renderer
                                                            , text_surface);
        SDL_RenderCopy(renderer, text_swap, NULL, &temp);
    }
    return;
}

/**
 * void input_reset()
 * 
 * Simple utility function that resets mem to default value.
 **/
void Game::Gameboard::input_reset() {
    // sets mem value to that specified in initialiazer
    mem = k_right;
    return;
}

/**
 * void set_bg_color()
 * 
 * Simple utility function that updates the background color.
 **/
void Game::Gameboard::set_bg_color(unsigned char r
    , unsigned char g
    , unsigned char b) {
    // set background color to that given
    background.r = static_cast<int>(r);
    background.g = static_cast<int>(g);
    background.b = static_cast<int>(b);
    return;
}

/**
 * void update_screen()
 * 
 * copies the texture to the screen surface and presents that surface
 **/
void Game::Gameboard::update_screen() {
    // unsets the render target, so renderpresent does not invalidate texture?
    SDL_SetRenderTarget(renderer, NULL);
    // copy the texture onto the render surface
    SDL_RenderCopy(renderer, render_texture, 0, 0);
    // present the render surface 
    SDL_RenderPresent(renderer);
    // set render target back to texture, it is now safe
    SDL_SetRenderTarget(renderer, render_texture);
    return;
}

/**
 * KeyPressed update_input();
 * 
 * Handles the input queue from sdl, saves the last given input into mem,
 * and returns that.
 * 
 * If, instead, SDL_QUIT is set, the set the sdl state accordingly, and logic
 * should exit.
 * 
 * return KeyPressed
 **/
Game::KeyPressed Game::Gameboard::update_input() {
    while (SDL_PollEvent(&input)) {
        if (input.type == SDL_QUIT) {
            SDL_Status = false; // SDL will still work
            // however, logic should respect this, and exit out of given program
            // so child logic should respect this and end so this class goes
            // out of scope and destructs.
        } else if (input.type == SDL_KEYDOWN) {
            switch (input.key.keysym.sym) {
                case SDLK_UP:
                    mem = k_up;
                    break;
                case SDLK_RIGHT:
                    mem = k_right;
                    break;
                case SDLK_DOWN:
                    mem = k_down;
                    break;
                case SDLK_LEFT:
                    mem = k_left;
                    break;
                case SDLK_SPACE:
                    mem = k_space;
                    break;
                default:
                    break;
            }
        }
    }
    return mem;
}

// Destructor
Game::Gameboard::~Gameboard() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(render_texture);
    renderer = nullptr;
    window = nullptr;
    render_texture = nullptr;
    SDL_Quit();
}
