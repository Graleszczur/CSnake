/* screen.cpp */
#include "screen.h"
#include "cpoint.h"

#define LINE_MAX 255

static CPoint TopLeft; //lewa gorna krawedz calego ekranu
static CPoint BottomRight; //prawy dolny rog calego okna
static int cury, curx; //wartosc kursora patrzymy gdzei co napisac

void init_screen() //inicjalizacja okna
{
  initscr();
  cbreak();
  noecho();
  timeout(20);
  nonl();
  leaveok(stdscr,TRUE);
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  curs_set(0);
}

void done_screen()//ma zakonczyc tryb okienkowy
{
  endwin();
}

void update_screen()//skalowanie erkanu przy rozszerzaniu
{
  getscreenanchor(TopLeft.y, TopLeft.x);
  getscreensize(BottomRight.y, BottomRight.x);
  BottomRight += TopLeft;
}

int ngetch()//pobranie klawisza
{
  return wgetch(stdscr);
}

void getscreenanchor(int &y, int &x)//pobranie poczatkowych x i y 
{
  getbegyx(stdscr, y, x);
}

void getscreensize(int &y, int &x)//pobiera max rozmiar okna
{
  getmaxyx(stdscr, y, x);
}

int gotoyx(int y, int x)//ustawia kursor do naszych koordynatow, czyli tam piszemy cos
{
  cury=y;
  curx=x;
  return (cury >= TopLeft.y && cury < BottomRight.y && curx >= TopLeft.x && curx < BottomRight.x);
}//zwraca czy mozna ustawic czy nie, jak damy punkt spoza okna wywali blad

void getcursor(int& y, int& x)//pobieramy wartosci naszej lokalizacji przez referencje i dajemy do
{
  y = cury;
  x = curx;
}

void printc(char c)//jezeli nie robimy nowej linii ani powrotu to printuje znak i przesuwa kursor
{
  if(c != '\n' && c != '\r')
  {
    if(cury >= TopLeft.y && cury < BottomRight.y && curx >= TopLeft.x && curx < BottomRight.x)
    {
      wmove(stdscr, cury, curx);
      addch(c);
    }
    curx++;
  }
}

int printl(const char* fmt, ...)//zmienna liczba argumentow od miejsca gdzie kursor napisze napis
{
  va_list ap;
  va_start(ap, fmt);

  char dest[LINE_MAX];
  vsnprintf(dest, LINE_MAX, fmt, ap);

  int i = 0;
  while(dest[i])
    printc(dest[i++]);

  va_end(ap);
  return !ERR;
}
