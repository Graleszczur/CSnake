#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "cpoint.h"
#include "screen.h"
#include <vector>


class CSnake:public CFramedWindow
{
	private:
		std::vector<CPoint> body;
		Cpoint food_position;
		int dir_x;
		int dir_y;
		int score;
		int status;
		
		
	public:
  		CSnake(CRect r, char _c = ' ');
		void print_window();
		void game_start();
		void game_reset();
		int game_end();
		void print_snake();
		void move();
		void place_food();
		int collision(Cpoint, Cpoint);
		int key_res(int key);
		void eat_food();

		
		

		

};

#endif
