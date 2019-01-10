#include "snake.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

CSnake::CSnake(CRect r, char _c /*=' '*/):
  CFramedWindow(r, _c)
{
	body.push_back(CPoint(5, 10));
	body.push_back(CPoint(5, 11));
	body.push_back(CPoint(5, 12));
	status = 0; //0-menu,1-gra, 2-help,3-pauza,4-zdech
	dir_x = 1;
	dir_y = 0;
	score = 0;
	food_position.x = 10;
	food_position.y= 10;
	
	
}

void CSnake::paint()
{
	gotoyx(geom.topleft.y -1, geom.topleft.x);
	printl("|SCORE: %d|", score);
	if(status == 0)
	{
		CFramedWindow::paint();
		gotoyx(geom.topleft.y + 4, geom.topleft.x + 2);
		printl("h - toggle help information");
		gotoyx(geom.topleft.y + 5, geom.topleft.x + 2);
		printl("p - toggle pause/play mode");
		gotoyx(geom.topleft.y + 6, geom.topleft.x + 2);
		printl("r - restart game");
		gotoyx(geom.topleft.y + 7, geom.topleft.x + 2);
		printl("arrows - move window (in pause mode)");
		gotoyx(geom.topleft.y + 8, geom.topleft.x + 2);
		printl("         move snake (in play mode)");
		gotoyx(geom.topleft.y + 9, geom.topleft.x + 2);
		printl("q - toggle end game");
	}
	else if(status == 1)
	{
		CFramedWindow::paint();
		int speed = score * 5000;
		if(speed >= 100000)
		{
			speed = 100000;
		}
		usleep(120000 - (speed));
		move();
		print_snake();
		print_food();
		eat_food();
		if(game_end())
		{
			status = 4;
		}
		
	}
	else if(status == 2)
	{
		CFramedWindow::paint();
		print_snake();
		print_food();
		gotoyx(geom.topleft.y + 4, geom.topleft.x + 2);
		printl("h - toggle help information");
		gotoyx(geom.topleft.y + 5, geom.topleft.x + 2);
		printl("p - toggle pause/play mode");
		gotoyx(geom.topleft.y + 6, geom.topleft.x + 2);
		printl("r - restart game");
		gotoyx(geom.topleft.y + 7, geom.topleft.x + 2);
		printl("arrows - move window (in pause mode)");
		gotoyx(geom.topleft.y + 8, geom.topleft.x + 2);
		printl("         move snake (in play mode)");
		gotoyx(geom.topleft.y + 9, geom.topleft.x + 2);
		printl("q - toggle end game");
		gotoyx(geom.topleft.y + 11, geom.topleft.x + 2);
		printl("Press p to continue...");
	}
	else if(status == 3)
	{
		CFramedWindow::paint();
		print_snake();
		print_food();
		gotoyx(geom.topleft.y + 6, geom.topleft.x + 2);
		printl("PAUSED");
		gotoyx(geom.topleft.y + 10, geom.topleft.x + 2);
		printl("Press p to continue...");
		
	}
	else if(status == 4)
	{
		CFramedWindow::paint();
		print_snake();
		print_food();
		gotoyx(geom.topleft.y + 6, geom.topleft.x + 2);
		printl("GAME OVER");
		gotoyx(geom.topleft.y + 10, geom.topleft.x + 2);
		printl("Press r to exit to menu...");

	}
}

void CSnake::print_snake()
{
	unsigned int i = 0;
	unsigned int size = body.size();
	for(i = 0; i < size; i++)
	{
		if(i == 0)
		{
			gotoyx(body[i].y + geom.topleft.y, body[i].x + geom.topleft.x);
			printc('*');
			continue;
		}
		gotoyx(body[i].y + geom.topleft.y, body[i].x + geom.topleft.x);
		printc('#');
		
	}

}

void CSnake::print_food()
{
	gotoyx(food_position.y + geom.topleft.y, food_position.x + geom.topleft.x);
	printc('0');
}

void CSnake::game_reset()
{
	
	body.clear();
	body.push_back(CPoint(5, 10));
	body.push_back(CPoint(5, 11));
	body.push_back(CPoint(5, 12));
	status = 0; //0-menu,1-gra, 2-help,3-pauza,4-zdech
	dir_x = 1;
	dir_y = 0;
	score = 0;
	food_position.x = 10;
	food_position.y= 10;
	
	
}

bool CSnake::collision(CPoint a, CPoint b)
{
	if(a.x == b.x && a.y == b.y)
	{
		return true;
	}
	return false;
}

bool CSnake::handleEvent(int key)
{
	if(status != 1 && CFramedWindow::handleEvent(key))
	{
		return false;
	}
	else
	{
		if(status == 1)
		{
			if(key == KEY_UP)
			{
				if(reversion_denied(1))
				{
					dir_x = 0;
					dir_y = -1;
				}
			}
			else if(key == KEY_DOWN)
			{
				if(reversion_denied(2))
				{
					dir_x = 0;
					dir_y = 1;
				}
			}
			else if(key == KEY_LEFT)
			{
				if(reversion_denied(3))
				{
					dir_x = -1;
					dir_y = 0;
				}
			}
			else if(key == KEY_RIGHT)
			{
				if(reversion_denied(4))
				{
					dir_x = 1;
					dir_y = 0;
				}
			}
		}
		if(key == 'r')
		{
			game_reset();
			
		}
		else if(key == 'p')
		{
			if(status == 0 || status == 2 || status == 3)//jak menu, help lub pauza to odpal
			{
				status = 1;
			}
			else if(status == 1)//pauza
			{
				status = 3;
			}
		}
		else if(key == 'h')
		{
			status = 2;
		}	
	}
	return true;

}
bool CSnake::reversion_denied(int key)
{
	if(key == 1)
	{
		if(dir_y == 1)
		{
			return false;
		}	
		return true;
		
	}
	else if(key == 2)
	{
		if(dir_y == -1)
		{
			return false;
		}	
		return true;
		
	}
	else if(key == 3)
	{
		if(dir_x == 1)
		{
			return false;
		}	
		return true;
		
	}
	else if(key == 4)
	{
		if(dir_x == -1)
		{
			return false;
		}	
		return true;
		
	}	
	return false;
	
}
void CSnake::move()
{
	unsigned int i = body.size() - 1;
	for(; i >= 1; i--)
	{
		body[i] = body [i - 1];
	}
	go_through_wall();
	
}
void CSnake::go_through_wall()
{
	int x = body[0].x + dir_x; 
	int y = body[0].y + dir_y;
	
	if(x <= 0)
	{
		body[0] = CPoint(geom.size.x - 2, y);
		return;
		
	}
	else if(x >= geom.size.x - 1)
	{
		body[0] = CPoint(1, y);
		return;
		
	}
	else if(y <= 0)
	{
		body[0] = CPoint(x, geom.size.y - 2);
		return;
		
	}
	else if(y >= geom.size.y - 1)
	{
		body[0] = CPoint(x, 1);
		return;
		
	}
	body[0] = CPoint(x, y);
	
	
	
}
void CSnake::relocate_food()
{
	srand(time(NULL));
	food_position.x = rand()%(geom.size.x - 2) + 1;
	food_position.y = rand()%(geom.size.y - 2) + 1;
	unsigned int i = 0;
	for(i = 0; i < body.size(); i++)
	{
		if(collision(body[i], food_position))
		{
			relocate_food();
		}
		
	}
}
bool CSnake::game_end()
{
	unsigned int i = 1;
	for (i = 1; i < body.size(); i++)
	{
		if(collision(body[0], body[i]))
		{
			return true;
		}
	}
	return false;
}

void CSnake::eat_food()
{
	if(collision(body[0], food_position))
	{
		score++;
		relocate_food();
		body.push_back(body[body.size()-1]);
	}
}
