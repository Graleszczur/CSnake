#include "snake.h"
#include <stdlib.h>
#include <time.h>

CSnake::CSnake(CRect r, char _c /*=' '*/):
  CFramedWindow(r, _c)
{
	body.push_back(CPoint(2, 4));
	body.push_back(CPoint(2, 5));
	status = 0; //0-menu,1-gra, 2-help,3-pauza,4-zdech
	dir_x = 1;
	dir_y = 0;
	score = 0;
	food_position(6, 6);
	
	
}
