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
  window(rectangle Pos, unsigned short BackgroundColor, unsigned short BorderColor, unsigned short BarColor) : Pos(Pos), BackgroundColor(BackgroundColor), BorderColor(BorderColor), BarColor(BarColor) {}
  void BringOnTop();
  virtual void Click(vector2d2d2d);
  virtual rectangle GetPos() { return Pos; }
  window() {}
  virtual void SetBackgroundColor(unsigned short What) { BackgroundColor = What; }
<<<<<<< window.h
  virtual void DrawBackground(void);
  virtual vector<windowobject*>& GetWindowObjectList(void) { return WindowObjectList; }
=======
  virtual void DrawBackground();
  virtual void MoveTo(rectangle What) { Pos = What; }
  virtual vector2d2d<textbox*> GetTextBoxList() { return TextBoxList; }
  virtual std::string GetSaveName() { return "jes.txt"; }
>>>>>>> 1.2
  static void Close(window*);
  virtual void MoveTo(vector2d);
  virtual void DrawButtons(void);
  virtual void TryResize(vector2d);
 protected:
<<<<<<< window.h
  vector<windowobject*> WindowObjectList;
=======
  vector2d2d<textbox*> TextBoxList;
>>>>>>> 1.2
  rectangle Pos;
  unsigned short BackgroundColor, BorderColor, BarColor;
};

class editorwindow : public window
{
 public:
<<<<<<< window.h
  editorwindow(rectangle, rectangle, toolwindow*);
  ~editorwindow(void);
  virtual void Draw(void);
  void Click(vector2d);
  virtual void SetBitmapPos(rectangle);
  virtual void DrawPixel(vector2d);
  virtual rectangle GetBitmapPos(void) { return BitmapPos; }
  virtual void UpdateGraphics(void);
=======
  virtual void Draw();
  editor::editor(rectangle, char*);
  editor::~editor();
>>>>>>> 1.2
 protected:
  BITMAP* Data;
  rectangle BitmapPos;
  toolwindow* LinkedTools;
};

class chooserwindow : public window
{
 public:
<<<<<<< window.h
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
=======
  virtual void Draw();
  resultwindow(rectangle, vector2d2d<string>, vector2d2d<unsigned int>, vector2d2d<unsigned int>);
  resultwindow() {}
  ~resultwindow();
  virtual void MoveTo(rectangle);
>>>>>>> 1.2
};

class toolwindow : public window
{
 public:
<<<<<<< window.h
 // void Draw(void);
  toolwindow(rectangle);
  virtual void SetActive(unsigned char);
  virtual unsigned char GetActive(void) const { return Active; }
=======
  editpersonwindow(rectangle);
  ~editpersonwindow();
  void Click(vector2d2d2d Where);
  static void Save(window*);
  void Draw();
  void Load(ifstream*);
  std::string GetSaveName();
>>>>>>> 1.2
 protected:
  unsigned char Active;
};

class palettechooser : public window
{
 public:
<<<<<<< window.h
  void Draw(void);
  

=======
  void Draw();
  whattoedit();
  void Click(vector2d2d2d);
>>>>>>> 1.2
 protected:


};

#endif
  

