#include "winsys.h"
#include "cpoint.h"
#include "screen.h"

#include <list>
#include <string>
#include <ctype.h>
#include <stdarg.h>
using namespace std;

void CView::move(const CPoint & delta)//poruszanie prostokatem na ktorym wykonujemy operacje
{
  geom.topleft += delta;
}

void CWindow::paint()//rysowanie i pisanie znaku "c" w danym okreslonym prostokacie
{
  for (int y = geom.topleft.y; y < geom.topleft.y + geom.size.y; y++)
    for (int x = geom.topleft.x; x < geom.topleft.x + geom.size.x; x++)
    {
      gotoyx(y, x);
      printl("%c", c);
    }
}

bool CWindow::handleEvent(int key)//pobiera klawisze do pobierania okna
{
  switch (key)
    {
    case KEY_UP:
      move (CPoint (0, -1));
      return true;
    case KEY_DOWN:
      move (CPoint (0, 1));
      return true;
    case KEY_RIGHT:
      move (CPoint (1, 0));
      return true;
    case KEY_LEFT:
      move (CPoint (-1, 0));
      return true;
    };
  return false;
}

void CFramedWindow::paint()//powoduje ze nasze okienko ma ramke
{
  for(int y = geom.topleft.y; y < geom.topleft.y + geom.size.y; y++)
  {
    if((y == geom.topleft.y) || (y == geom.topleft.y + geom.size.y-1))
    {
      for(int x = geom.topleft.x; x < geom.topleft.x + geom.size.x; x++)
      {
        gotoyx(y, x);
        if((x == geom.topleft.x) || (x == geom.topleft.x + geom.size.x-1))
          printl ("+");
        else
          printl ("-");
      }
    }
    else
    {
      for(int x = geom.topleft.x; x < geom.topleft.x + geom.size.x; x++)
      {
        gotoyx(y, x);
        if((x == geom.topleft.x) || (x == geom.topleft.x + geom.size.x-1))
          printl ("|");
        else
          printl ("%c", c);
      }
    }
  }
}

void CInputLine::paint()//wypisuje linie z tekstem
{
  CFramedWindow::paint();

  unsigned int row = 0;
  for(int y = geom.topleft.y+1; y < geom.topleft.y + geom.size.y-1; y++, row++)
  {
    unsigned int letter = row * (geom.size.x-2);
    for (int x = geom.topleft.x+1; x < geom.topleft.x + geom.size.x-1; x++, letter++)
    {
      gotoyx(y, x);
      printl("%c", letter < text.size() ? text[letter] : c); //c to domyslny symbol
    }
  }
}

bool CInputLine::handleEvent(int c)
{
  if(CFramedWindow::handleEvent(c))//rozpoznaje klawisz
    return true;
  if((c == KEY_DC) || (c == KEY_BACKSPACE))
    {
      if (text.length() > 0)
        {
          text.erase (text.length() - 1);
          return true;
        };
    }
  if((c > 255) || (c < 0))
    return false;
  if(!isalnum (c) && (c != ' '))
    return false;
  text.push_back(c);
  return true;
}

void CGroup::paint()//wypelnia caly ekran kropkami tam gdzie nie ma okien, nastepnie iteruje po swoich cview rysujac oddzielne okna, czyli rysuje caly ekran
{
  for(int y= geom.topleft.y; y < geom.topleft.y + geom.size.y; y++)
  {
    gotoyx(y, geom.topleft.x);
    for(int x = 0; x < geom.size.x; x++)
      printl (".");
  }

  for (list< CView * >::iterator i = children.begin(); i != children.end(); i++)
    (*i)->paint();
}

bool CGroup::handleEvent(int key)//lapie klawisz i jezeli lista cview(dzieci) nei jest pusta sprawdza sprawdza czy ktores z jego dzieci reaguje na jakis klawisz
{
  if (!children.empty() && children.back()->handleEvent (key))
    return true;
  if (key == '\t')//tabulator pozwala przechodzic miedzy okienkami, jezeli jakeis zlapie tabulacje to sie wywali
    {
      if (!children.empty())
        {
          children.push_front(children.back());
          children.pop_back();
        };
      return true;
    }
  return false;
}

void CGroup::insert(CView * v)//tworzymy nowe cview i dodajemy do puli okien
{
  children.push_back(v);
}

CGroup::~CGroup()//wywalamy wszystkie okna
{
  for (list< CView * >::iterator i = children.begin();
       i != children.end(); i++)
    delete(*i);
}

CDesktop::CDesktop() : CGroup(CRect())//caly ekran terminala
{
  init_screen();
  update_screen();
}

CDesktop::~CDesktop()//konczy prace z ncurses
{
  done_screen();
}

void CDesktop::paint()//sprawdza rozmair terminala wypelnia kropkami i wywoluje cgroup painta tez kropkami
{
  getscreensize (geom.size.y, geom.size.x);

  for(int y = geom.topleft.y; y < geom.topleft.y + geom.size.y; y++)
  {
    gotoyx(y, geom.topleft.x);
    for (int x = 0; x < geom.size.x; x++)
      printl (".");
  }

  CGroup::paint();
}

int CDesktop::getEvent()//pobiera przycisk
{
  return ngetch();
}

void CDesktop::run()//zbiera info, q wywala system okienkowy
{
  update_screen();
  paint();
  refresh();

  while(1)
  {
    int c = getEvent();

    if(c == 'q' || c == 'Q')
      break;

    if(c == KEY_RESIZE || handleEvent(c))//skalowanie i reakcja
    {
        update_screen();
        paint();
        refresh();
    }

  }
}
