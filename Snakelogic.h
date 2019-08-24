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

// NOTE: Main function is assumed to handle game over conditions at the moment.
// will fix this later?

namespace Snake {
	class Snake {
	private:
		enum BoardState {
			No_Apple = false,
			Apple = true
		};

		// Note to self: Co-ordinate system takes top left as (0, 0).
		bool board[80][59], pause_queue, game, exit;
		bool is_apple; // set to true when appple is put on board

		Game::Gameboard *g;

		int score, apple_counter;

		Game::KeyPressed direction;

		std::queue<int> snakex, snakey; // snake coordinates

	public:

		Snake() ;

		void kill_switch() ;
		void place_apple() ;
		void restart_game() ;
		void update_snake() ;
		void update() ;

		bool exit_check() ;
		bool oroborous_check() ;

		~Snake() ;
	};
};

// TODO: Flash Game over at game = false
// TODO: Timer counting up
// TODO: Scoreboard, counting up
#endif