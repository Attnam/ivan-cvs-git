#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "rect.h"
#include "button.h"
class rectangle;
class textbox;
class functionbutton;
class windowhandler;

class window
{
 public:
  virtual void Draw();
  virtual void SetPos(rectangle What) { Pos = What; }
  window(rectangle Pos, unsigned short BackgroundColor, unsigned short BorderColor, unsigned short BarColor) : Pos(Pos), BackgroundColor(BackgroundColor), BorderColor(BorderColor), BarColor(BarColor) { }
  void BringOnTop();
  virtual void Click(vector2d);
  virtual rectangle GetPos() { return Pos; }
  window() { }
  virtual void SetBackgroundColor(unsigned short What) { BackgroundColor = What; }
  virtual void DrawBackground();
  virtual vector<windowobject*>& GetWindowObjectList() { return WindowObjectList; }
  static void Close(window*);
  virtual void MoveTo(vector2d);
  virtual void DrawButtons();
  virtual void TryResize(vector2d);
 protected:
  vector<windowobject*> WindowObjectList;
  rectangle Pos;
  unsigned short BackgroundColor, BorderColor, BarColor;
};

class editorwindow : public window
{
 public:
  editorwindow(rectangle, rectangle, toolwindow*);
  ~editorwindow();
  virtual void Draw();
  void Click(vector2d);
  virtual void SetBitmapPos(rectangle);
  virtual void DrawPixel(vector2d);
  virtual rectangle GetBitmapPos() { return BitmapPos; }
  virtual void UpdateGraphics();
 protected:
  BITMAP* Data;
  rectangle BitmapPos;
  toolwindow* LinkedTools;
};

class chooserwindow : public window
{
 public:
  virtual void Draw();
  chooserwindow(rectangle, editorwindow*);
  void Click(vector2d);
  void ScrollUp() { if(Scroll.Y > 15) Scroll.Y -= 16; }
  void ScrollDown() { if(Scroll.Y + Pos.Bottom - Pos.Top <  graphics::GetData()->w + 16) Scroll.Y += 16; }
  void SetSelected(rectangle What) { Selected = What; }
  void SetScroll(vector2d What) { Scroll = What; }
 protected:
  rectangle Selected;
  vector2d Scroll;
  editorwindow* Linked;
  BITMAP* ScrollBar;
};

class toolwindow : public window
{
 public:
  toolwindow(rectangle);
  virtual void SetActive(unsigned char);
  virtual unsigned char GetActive() const { return Active; }
 protected:
  unsigned char Active;
};

class palettechooser : public window
{
 public:
  void Draw();
  

 protected:


};

#endif
  


