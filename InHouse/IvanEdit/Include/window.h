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
  virtual void Draw(void);
  virtual void SetPos(rectangle What) { Pos = What; }
  window(rectangle Pos, unsigned short BackgroundColor, unsigned short BorderColor, unsigned short BarColor) : Pos(Pos), BackgroundColor(BackgroundColor), BorderColor(BorderColor), BarColor(BarColor) {}
  void BringOnTop(void);
  virtual void Click(vector2d);
  virtual rectangle GetPos(void) { return Pos; }
  window(void) {}
  virtual void SetBackgroundColor(unsigned short What) { BackgroundColor = What; }
  virtual void DrawBackground(void);
  virtual vector<windowobject*>& GetWindowObjectList(void) { return WindowObjectList; }
  static void Close(window*);
  virtual void MoveTo(vector2d);
  virtual void DrawButtons(void);
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
  ~editorwindow(void);
  virtual void Draw(void);
  void Click(vector2d);
  virtual void SetBitmapPos(rectangle);
  virtual void DrawPixel(vector2d);
  virtual rectangle GetBitmapPos(void) { return BitmapPos; }
  virtual void UpdateGraphics(void);
 protected:
  BITMAP* Data;
  rectangle BitmapPos;
  toolwindow* LinkedTools;
};

class chooserwindow : public window
{
 public:
  virtual void Draw(void);
  chooserwindow(rectangle, editorwindow*);
  void Click(vector2d);
  void ScrollUp(void) { if(Scroll.Y > 15) Scroll.Y -= 16; }
  void ScrollDown(void) { if(Scroll.Y + Pos.Bottom - Pos.Top <  graphics::GetData()->w + 16) Scroll.Y += 16; }
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
 // void Draw(void);
  toolwindow(rectangle);
  virtual void SetActive(unsigned char);
  virtual unsigned char GetActive(void) const { return Active; }
 protected:
  unsigned char Active;
};

class palettechooser : public window
{
 public:
  void Draw(void);
  

 protected:


};

#endif
  
