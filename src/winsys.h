#ifndef __WINSYS_H__
#define __WINSYS_H__

#include "cpoint.h"
#include <list>

#include <string>
//#include <ctype.h>
//#include <stdarg.h>
using namespace std;

class CView//prostokat cos do obudowania
{
protected:
  CRect geom;

public:
  CView(CRect g) : geom(g) {}
  virtual ~CView() {}		//virtual, klasa nie ma tej metody, ale te metody powinny byc u kogos kto dziedziczy, nie ma destruktora ale pozwala go nadpisac po dziedziczeniu

  virtual void paint () = 0;
  virtual bool handleEvent (int key) = 0;
  virtual void move (const CPoint & delta);
};

class CWindow:public CView// obszar roboczy
{
protected:
  char c;
public:
  CWindow(CRect r, char _c = '*') : CView(r), c(_c) {}

  void paint();
  bool handleEvent(int key);
};

class CFramedWindow:public CWindow//obszar roboczy z ramka
{
public:
  CFramedWindow(CRect r, char _c = '\'') : CWindow(r, _c) {}

  void paint();
};

class CInputLine:public CFramedWindow//linia z tekstem
{
  string text;
public:
  CInputLine(CRect r, char _c = ',') : CFramedWindow(r, _c) {}

  void paint();
  bool handleEvent(int c);
};

class CGroup:public CView //cgroup dziedziczy po cview, w tej liscie sa wskazniki na wszystkie klasy ktore dziedzicza po cview, lista zgodnych komponentow zestaw okien
{
  list< CView * > children;
public:
  CGroup(CRect g) : CView(g) {} //przyjmuje w konstruktorze crect
  ~CGroup();

  void paint();
  bool handleEvent(int key);
  void insert(CView * v);
};

class CDesktop:public CGroup//opisuje caly terminal
{
public:
  CDesktop();
  ~CDesktop();

  void paint();
  int getEvent();
  void run();
};

#endif
