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
  static void Draw(void);
  static void Init(void);
  static void DeInit(void);
  static void Click(vector2d);
  static vector<window*>& GetWindowList(void) { return WindowList; }
  static void BringOnTop(window*);
  static cursor* GetCursor(void) { return Cursor; }
 protected:
  static vector<window*> WindowList;
  static cursor* Cursor;
  static programexitbutton* ProgramExitButton;
  static int MouseSpeed;
};

#endif
