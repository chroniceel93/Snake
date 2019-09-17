#include "Snakelogic.h"

Snake::Snake::Snake() {
    g = new Game::Gameboard();

    input = Game::KeyPressed::k_right;
    score = 0;
    pause_queue = true; //start paused so snake is two long
    snakex.push(40);
    snakey.push(29);
    if (g->status()) {
        game = true;
        exit = false;
    } else {
        game = false;
        exit = true;
    }
    for (auto &x : board) {
        for (auto &y : x) {
            y = No_Apple;
        }
    }
    is_apple = false;
}

/**
 * void input_override()
 * 
 * Checks the current input against the input from the last tick to make sure
 * that the snake is not doubling back on itself, as this is an instant game
 * over that is far too easy to trip by accident.
 **/
void Snake::Snake::input_override() {
    // if current input is up, and last input was down,
    // ignore current input (set it to last input) to save us from
    // an instant game over... logic repeats for all combinations
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
// it is essentail to make sure that you cannot do a 180
// turn in a single tick, otherwise you will find the
// oroborous function returning true, so I am
// creating a copy of the last ticks input state,
// and checking that against the current input state.
// scattering theis notice here and there bc without this rationale
// this bit of logic seems somewhat arbitrary

/**
 * void kill_switch()
 * 
 * Sets game to false and exit to true, if main is structured correctly (
 * a while loop controlled by exit_check()), then this will shut everything down
 **/
void Snake::Snake::kill_switch() {
    game = false;
    exit = true;
    return;
}

/**
 * void place_apple()
 * 
 * Generates random co-ordinates, checks against the snake co-ordinate queue
 * to make sure that we do not place the apple on the snake, sets the
 * corresponding board co-ordinate to true, and draws an apple at that location
 **/
void Snake::Snake::place_apple() {
    // generate random co-ordinates that fit on 80x59 grid
    int x = std::rand() % (BOUNDR - BOUNDL);
    // plus one to stop apples in scoreboard
    int y = (std::rand() % (BOUNDD - BOUNDU)) + 2; 

    // check coordinates to see if snake is on the board position
    std::queue<int> tempx, tempy;
    // make copy of snakex and snakey
    tempx = snakex;
    tempy = snakey;

    // while temp queues are not empty, check the co-ordinates from the 
    // queues against the generated x and y values. If there is a match, 
    // reset the temp snake queue, and regenerate the random coordinates,
    // do this until we have a co-ordinate pair that does not match any
    // occurring in the snake co-ordinate queues.
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
        // This loop should never exit if an Apple is 
        // going to be placed on the snake
    }
    // at the generated co-ordinates to true (there is an apple), so
    // the snake can find the apple
    board[x][y] = Apple;
    // draw an apple at the co-ordinates so the player can find the apple
    g->draw_block(x, y, 255, 0, 0);
    return;
}

/**
 * void restart_game()
 * 
 * Re-initializes every variable used in the snake game, empties the snake
 * co-ordinate queues and blanks the screen.
 **/
void Snake::Snake::restart_game() { // copy of constructor stuffs
    // re-init variables
    score = 0;

    game = true;
    is_apple = false;
    pause_queue = true; //start paused so snake is two long
    
    input = Game::KeyPressed::k_right;

    // empty the co-ordinate queues
    while (!snakey.empty()) {
        snakey.pop();
    }
    while (!snakex.empty()) {
        snakex.pop();
    }
    // add default values to co-ordinate queues
    snakex.push(40);
    snakey.push(29);

    // re-initialize the board
    for (auto &x : board) {
        for (auto &y : x) {
            y = No_Apple;
       
        }
    }
    
    // redraw background over old board state
    g->blank_screen();
    g->input_reset();
    return;
}

/**
 * void update_snake()
 * 
 * using the current state of the class:
 * 
 * First, run oroborous_check() to see if we are eating ourselves, if we
 * are skip all other logic and set state to false, otherwise
 * 
 * Second, check to see if we are eating an apple, if we are, grow the snake,
 * and place a new apple
 * 
 * Third, if the queue is unpaused, draw the updated snake head co-ordinates,
 * draw the bg color over the tbr tail and pop the tail co-ords
 *         else, if the queue is paused, only update the head co-ords.
 **/
void Snake::Snake::update_snake() {
    // check to see if we are eating ourselves this tick
    // if we are then skip the main update logic and set game state to false
    if (oroborous_check()) {
        // if there is an apple on the board at the head co-ordinates, 
        // then delete the apple, pause the co-ordinate queues (so snake will
        // grow), and reset the apple.
        if (board[snakex.back()][snakey.back()] == Apple) {
            board[snakex.back()][snakey.back()] = No_Apple;
            place_apple();
            score = score + 100;
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

void Snake::Snake::update_scoreboard() {
    std::string temp;
    temp = "Score: ";
    temp.append(std::to_string(score));
    g->draw_text(0, 0, temp);
    temp = "Timer: ";
    temp.append(std::to_string(time / 1000));
    g->draw_text(700, 0, temp);
    return;
}

/**
 * bool oroborous_check()
 * 
 * creates copies of the snake co-ordinate queues, and by popping
 * the values in the copied queues, checks against the head co-ordinate from the
 * original queue, to verify if the snake is eating itself
 * 
 * return status, if false game ends, if true game continues
 **/
bool Snake::Snake::oroborous_check() {
    std::queue<int> tempx, tempy; // will hold copies of snake queues
    int tempvarx, tempvary; // will hold copies of the tail of the snake
    bool send = true; // send is the status that we are returning

    tempx = snakex;
    tempy = snakey;
    // this works but I forgot why :(
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

/**
 * void update()
 * 
 * This is the main logic function. It handles inputs and modifies the
 * snake's position accordingly, and calls the functions, when appropriate
 * that check to see if the snake's position is valid, and handles a game over
 * status.
 **/
// TODO: restructure input handler such that we only call g->status() once
void Snake::Snake::update() {
    input = g->update_input();
    input_override();
    time = SDL_GetTicks();
    if ((time - time_since_tick) >= TICK_TIME) {
        time_since_tick = time;
        if (game) {
            // handle case where no apples are currently on board
            if (!is_apple) {
                place_apple();
                is_apple = true;
                update_scoreboard();
            } else {
                if (g->status()) { // we only want this to happen every 100 ms.
                    switch (input) {
                        case Game::KeyPressed::k_up:
                            snakex.push(snakex.back());
                            if ((snakey.back() -1) == BOUNDU) {
                                game = false; // oob, game over
                            } else {
                                snakey.push(snakey.back() - 1);
                            }
                            break;
                        case Game::KeyPressed::k_right:
                            if ((snakex.back() + 1) == BOUNDR) {
                                game = false; // oob, game over
                            } else {
                                snakex.push(snakex.back() + 1);
                            }
                            snakey.push(snakey.back());
                            break;
                        case Game::KeyPressed::k_down:
                            snakex.push(snakex.back());
                            if ((snakey.back() + 1) == BOUNDD) {
                                game = false; // oob, game over
                            } else {
                                snakey.push(snakey.back() + 1);
                            }
                            break;
                        case Game::KeyPressed::k_left:
                            if ((snakex.back() - 1) == BOUNDL) {
                                game = false; // oob, game over
                            } else {
                                snakex.push(snakex.back() - 1);
                            }
                            snakey.push(snakey.back());
                            break;
                        case Game::KeyPressed::k_space: // turns out, this can crash
                        // the damn thing if I don't handle it. Might as well make
                        // it a feature.
                            game = false; // exit button
                            break;
                        default:
                            break;
                    }
                    update_snake();
                } else {
                    kill_switch();
                }
            }   
            input_old = input;
        } else {
            if (g->status()) {
                if (input == Game::KeyPressed::k_space) {
                    restart_game();
                } else {} // do nothing
            } else {
                kill_switch();
            }
        }
    }
    update_scoreboard();
    g->update_screen(); //we want to re-draw screen ever tick regardless of
    // game state
    // TODO: Make each tick last exactly 100ms (unless tick goes over)
    return;
}

/**
 * bool exit_check()
 * 
 * returns exit (representing the status of sdl via g->status())
 **/
bool Snake::Snake::exit_check() {
    return exit;
}

// Destructor
Snake::Snake::~Snake() {
    delete g;
}
