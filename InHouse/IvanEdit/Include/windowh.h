#ifndef __WINDOWH_H__
#define __WINDOWH_H__
#include <vector>

class window;
class cursor;
class programexitbutton;
class windowhandler
{
 public:
  static void CreateWindow(window*);
  static void Draw();
  static void Init();
  static void DeInit();
  static void Click(vector2d);
  static vector<window*>& GetWindowList() { return WindowList; }
  static void BringOnTop(window*);
  static cursor* GetCursor() { return Cursor; }
  static void LoadPerson(window*);
 protected:
  static vector<window*> WindowList;
  static cursor* Cursor;
  static programexitbutton* ProgramExitButton;
  static int MouseSpeed;
};

#endif

