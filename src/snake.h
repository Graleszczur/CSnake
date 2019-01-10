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
		CPoint food_position;
		int dir_x;
		int dir_y;
		int score;
		int status;
		int level;
		
		
		
		
	public:
  		CSnake(CRect r, char _c = ' ');
		void print_snake();//draws snake
		void game_reset();
		bool game_end();//self collision detect
		void paint();//draws window
		void print_food();//draws food
		void move();//snake movement
		void relocate_food();//generates food position
		bool collision(CPoint, CPoint);//detects collision between 2 points
		bool handleEvent(int key);//key detector
		void eat_food();//food eaten and elongate snake
		bool reversion_denied(int key);//1-UP,2-DOWN,3-LEFT,4-RIGHT,-anti 180* turn
		void go_through_wall();//relocates snake to the opposite site of the window true if he went

		
		

		

};

#endif
