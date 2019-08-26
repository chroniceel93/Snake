#include "Snakelogic.h"

Snake::Snake::Snake() {
    g = new Game::Gameboard();
    input = Game::KeyPressed::k_right;
    score = 0;
    apple_counter = 0;
    pause_queue = true; //start paused so snake is two long
    snakex.push(40);
    snakey.push(29);
    game = true;
    for (auto &x : board) {
        for (auto &y : x) {
            y = No_Apple;
        }
    }
    is_apple = false;
}

void Snake::Snake::input_override() {
    if (input == Game::KeyPressed::k_up 
        && input_old == Game::KeyPressed::k_down) {
        input = Game::KeyPressed::k_down;
    } else {}

    if (input == Game::KeyPressed::k_down
        && input_old == Game::KeyPressed::k_up) {
        input = Game::KeyPressed::k_up;
    } else {}

    if (input == Game::KeyPressed::k_left
        && input_old == Game::KeyPressed::k_right) {
        input = Game::KeyPressed::k_right;
    }

    if (input == Game::KeyPressed::k_right
        && input_old == Game::KeyPressed::k_left) {
        input = Game::KeyPressed::k_left;
    }

    return;
}

void Snake::Snake::kill_switch() {
    game = false;
    exit = true;
    return;
}
// it is essentail to make sure that you cannot do a 180
// turn in a single tick, otherwise you will find the
// oroborous function returning true, so I am
// creating a copy of the last ticks input state,
// and checking that against the current input state.
// scattering theis notice here and there bc without this rationale
// this bit of logic seems somewhat arbitrary

void Snake::Snake::place_apple() {
    int x = std::rand() % 80;
    int y = (std::rand() % 59) + 1; // plus one to stop apples in scoreboard

    // check coordinates to see if snake is on the board position
    std::queue<int> tempx, tempy;
    tempx = snakex;
    tempy = snakey;

    // This loop should never exit if an Apple is 
    // going to be placed on the snake
    while (!tempx.empty() && !tempy.empty()) {
        if (x == tempx.front() && y == tempy.front()) {
            x = std::rand() % 80;
            y = (std::rand() % 59) + 1; 
            tempx = snakex;
            tempy = snakey;
        } else {
            tempx.pop();
            tempy.pop();
        }
    }

    board[x][y] = Apple;
    g->draw_block(x, y, 255, 0, 0);
    return;
}

void Snake::Snake::restart_game() { // copy of constructor stuffs
    score = 0;
    apple_counter = 0;
    pause_queue = true; //start paused so snake is two long
    while (!snakey.empty()) {
        snakey.pop();
    }
    while (!snakex.empty()) {
        snakex.pop();
    }
    snakex.push(40);
    snakey.push(29);
    game = true;
    for (auto &x : board) {
        for (auto &y : x) {
            y = No_Apple;
        }
    }
    g->blank_screen();
    is_apple = false;
    input = Game::KeyPressed::k_right;
    g->input_reset();
    return;
}

void Snake::Snake::update_snake() {
    if (oroborous_check()) {
        if (board[snakex.back()][snakey.back()] == 1) {
            board[snakex.back()][snakey.back()] = 0;
            place_apple();
            score = score + 100;
            apple_counter++;
            pause_queue = true;
        } // else do_nothing

        if (pause_queue) {
            g->draw_block(snakex.back(), snakey.back(), 0, 255, 0);
            pause_queue = false;
        } else {
            g->draw_block(snakex.back(), snakey.back(), 0 ,255, 0);
            g->draw_block(snakex.front(), snakey.front(), 0, 0, 0);
            snakex.pop();
            snakey.pop();
        }
    } else {
        game = false;
    }
    return;
}

void Snake::Snake::update() {
    input = g->update_input();
    input_override();
    if (game) {
        // handle case where no apples are currently on board
        if (!is_apple) {
            place_apple();
            is_apple = true;
        } else {
            if (g->status()) {
                switch (input) {
                    case Game::KeyPressed::k_up:
                        snakex.push(snakex.back());
                        if ((snakey.back() -1) == 0) {
                            game = false; // oob, game over
                        } else {
                            snakey.push(snakey.back() - 1);
                        }
                        break;
                    case Game::KeyPressed::k_right:
                        if ((snakex.back() + 1) == 81) {
                            game = false; // oob, game over
                        } else {
                            snakex.push(snakex.back() + 1);
                        }
                        snakey.push(snakey.back());
                        break;
                    case Game::KeyPressed::k_down:
                        snakex.push(snakex.back());
                        if ((snakey.back() + 1) == 60) {
                            game = false; // oob, game over
                        } else {
                            snakey.push(snakey.back() + 1);
                        }
                        break;
                    case Game::KeyPressed::k_left:
                        if ((snakex.back() - 1) == 0) {
                            game = false; // oob, game over
                        } else {
                            snakex.push(snakex.back() - 1);
                        }
                        snakey.push(snakey.back());
                        break;
                    default:
                        break;
                }
                update_snake();
                g->update_screen();
            } else {
                kill_switch();
            }
        }   
        input_old = input;
    } else {
        if (input == Game::KeyPressed::k_space) {
            restart_game();
        }
    }
    SDL_Delay(100); // delay 100ms after every tick
    // TODO: Make each tick last exactly 100ms (unless tick goes over)
    return;
}

bool Snake::Snake::exit_check() {
    return exit;
}

bool Snake::Snake::oroborous_check() {
    std::queue<int> tempx, tempy;
    int tempvarx, tempvary;
    bool send = true;

    tempx = snakex;
    tempy = snakey;

    while (!tempx.empty() && !tempy.empty()) {
        tempvarx = tempx.front();
        tempx.pop();
        tempvary = tempy.front(); 
        tempy.pop();
        if (!tempx.empty() && !tempy.empty()) {
            if (tempvarx == snakex.back() && tempvary == snakey.back()) {
                send = false;
            } else {} // do_nothing
        } else {} // do_nothing
    }
    return send;
}

Snake::Snake::~Snake() {
    delete g;
}
