#ifndef SNAKELOGIC_H_
#define SNAKELOGIC_H_

#include "Gameboard.h"

#include <exception>
#include <random>
#include <queue>
#include <string>

// NOTE: All logic assmues playing field is 80 wide by 60 high, 
// with the (0, 0) co-ordinate being the top left of the field, 
// and the top row being a scoreboard.

namespace Snake {
	class Snake {
	private:
		// help increase readability of apple related topics
		enum BoardState {
			No_Apple = false,
			Apple = true
		};

		// Note to self: Co-ordinate system takes top left as (0, 0).
		const int BOUNDL = 0;
		const int BOUNDR = 81;
		const int BOUNDU = 1;
		const int BOUNDD = 60;
		
		// bool board is a little bigger than it needs to be. This is intention-
		// al. If it's just one row too small in either direction, we start fli-
		// pping random bits of unused memory on and off. Not exactly great.
		bool board[81][60], pause_queue, game, exit;

		bool is_apple; // set to true when appple is put on board

		Game::Gameboard *g; // gameboard handles I/O for game logic

		int score;// score counter!

		// boundaries for game logic, math taken from a 80x60 board, where the
		// top two rows are off-limits (from rows 0-59, where the 0th row is 
		// off limits)

	
		const double TICK_TIME = 100; //in miliseconds, as per SDL

		double time_since_tick = 0.0; // time elapsed since last tick
		double time_count = 0.0; // current time, to compare.
		double time_start = 0.0; // time_count at start of round
		double time_score = 0.0; // time_count - time_start (for scoreboard)
		
		Game::KeyPressed input, input_old;
		// input is the input for the current tick
		// input_old is the input from the last tick, used to check to see
		// 		if next input is compatible with the last
		//		ex) going up, you press down, snake immediately eats itself
		//			and oroborous_check trips, this is an instant game over.
		//			It can also happen by accident, so yeah, making sure to 
		//			handle this automatically!

		std::queue<int> snakex, snakey; // snake coordinates

/**
 * void input_override()
 * 
 * Checks the current input against the input from the last tick to make sure
 * that the snake is not doubling back on itself, as this is an instant game
 * over that is far too easy to trip by accident.
 **/
		void input_override() ;

/**
 * void kill_switch()
 * 
 * Sets game to false and exit to true, if main is structured correctly (
 * a while loop controlled by exit_check()), then this will shut everything down
 **/
		void kill_switch() ;

/**
 * void place_apple()
 * 
 * Generates random co-ordinates, checks against the snake co-ordinate queue
 * to make sure that we do not place the apple on the snake, sets the
 * corresponding board co-ordinate to true, and draws an apple at that location
 **/
		void place_apple() ;

/**
 * void restart_game()
 * 
 * Re-initializes every variable used in the snake game, empties the snake
 * co-ordinate queues and blanks the screen.
 **/
		void restart_game() ;

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
		void update_snake() ;

/**
 * void update_scoreboard()
 * 
 * Draws the current score and time to the screen. "Pauses" the timer if game is
 * false, by simply not updating it. Time is derived from current time - the
 * time since the round has started. Time is casted into an int to get rid of
 * trailing digits.
 **/
		void update_scoreboard() ;

/**
 * bool oroborous_check()
 * 
 * creates copies of the snake co-ordinate queues, and by popping
 * the values in the copied queues, checks against the head co-ordinate from the
 * original queue, to verify if the snake is eating itself
 * 
 * return status, if false game ends, if true game continues
 **/
		bool oroborous_check() ;


	public:

		Snake() ;

/**
 * void update()
 * 
 * This is the main logic function. It handles inputs and modifies the
 * snake's position accordingly, and calls the functions, when appropriate
 * that check to see if the snake's position is valid, and handles a game over
 * status.
 **/
		void update() ;

/**
 * bool exit_check()
 * 
 * returns exit (representing the status of sdl via g->status())
 **/
		bool exit_check() ;

		// Destructor
		~Snake() ;
	};
};

// TODO: Flash Game over at game = false
// TODO: Timer counting up
// TODO: Scoreboard, counting up
#endif