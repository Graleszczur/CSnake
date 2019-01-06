#ifndef __CPOINT_H__
#define __CPOINT_H__

#include <list>

using namespace std;

struct CPoint //rysowany punkt reprezentacja na erkanie
{
  int x; //koordynaty
  int y;
  CPoint(int _x=0, int _y=0): x(_x), y(_y) {}; //lista inicjalizacyjna
  CPoint& operator+=(const CPoint& delta)
  {
    x+=delta.x;
    y+=delta.y;
    return *this;
  }
};

struct CRect //prostokat
{
  CPoint topleft; //poczatek
  CPoint size;//ile ma sie rozwinac topleft+size = prawy dolny rog
  CRect(CPoint t1=CPoint(), CPoint s=CPoint()):
    topleft(t1), size(s) {}; //lista inicjalizacyjna
};

#endif
