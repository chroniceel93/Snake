#include "Gameboard.h"

// Constructors
Game::Gameboard::Gameboard() {
    SDL_Status = true; // assume sdl will work, unless it does not

    // default background color (black)
    background.r = 0;
    background.g = 0;
    background.b = 0;

    // init sdl
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
        if (window == nullptr) {
            throw SDL_GetError();
            SDL_Status = false;
        } else {
            renderer = SDL_CreateRenderer(
                window
                , -1
                , SDL_RENDERER_ACCELERATED);
            if (renderer == nullptr) {
                throw SDL_GetError();
                SDL_Status = false;
            } else {
                SDL_SetRenderDrawColor(
                    renderer
                    , background.r
                    , background.g
                    , background.b
                    , 255);
                SDL_RenderPresent(renderer);
            }
        }
    }

    // init texture
    render_texture = SDL_CreateTexture(renderer
        , SDL_PIXELFORMAT_RGBA8888
        , SDL_TEXTUREACCESS_TARGET
        , 800
        , 600);

    // set texture as render target
    SDL_SetRenderTarget(renderer, render_texture);

    bloc_width = 10;
    bloc_height = 10;

    mem = k_right; // default key
}

bool Game::Gameboard::status() {
    return SDL_Status;
}

void Game::Gameboard::blank_screen() {
    SDL_Rect temp;
    temp.w = 800;
    temp.h = 600;
    temp.x = 0;
    temp.y = 0;
    SDL_SetRenderDrawColor(renderer
        , background.r
        , background.g
        , background.b
        , 255);
    SDL_RenderFillRect(renderer, &temp);
    return;
}

void Game::Gameboard::draw_block( int xpos
    , int ypos
    , unsigned char r
    , unsigned char g
    , unsigned char b) {
    // sanity check, Y cannot be greater than 59 and
    // X cannot be greater than 80, neither can be less than 0.
    // Y should be at offset for scoreboard line
    if (xpos > 80) {
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

    SDL_Rect temp;
    temp.w = bloc_width;
    temp.h = bloc_height;
    temp.x = (xpos * 10) - 10;
    temp.y = (ypos * 10); // skip subtractin 10 so ypos leaves line for
                            // scoreboard
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &temp);
    SDL_SetRenderDrawColor(
        renderer
        ,background.r
        ,background.g
        ,background.b
        ,255);
    return;
}

void Game::Gameboard::input_reset() {
    mem = k_right;
    return;
}

void Game::Gameboard::set_bg_color(unsigned char r
    , unsigned char g
    , unsigned char b) {
    background.r = static_cast<int>(r);
    background.g = static_cast<int>(g);
    background.b = static_cast<int>(b);
    return;
}

void Game::Gameboard::update_screen() {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, render_texture, 0, 0);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, render_texture);
    return;
}

Game::KeyPressed Game::Gameboard::update_input() {
    while (SDL_PollEvent(&input)) {
        if (input.type == SDL_QUIT) {
            SDL_Status = false; // SDL will still work
            // however, logic should respect this, and exit out of given program
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

Game::Gameboard::~Gameboard() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;
    SDL_Quit();
}
