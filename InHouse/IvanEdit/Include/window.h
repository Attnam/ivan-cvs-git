#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "rect.h"
#include "button.h"
class rectangle;
class textbox;
class functionbutton;
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
  virtual void DrawBackground();
  virtual void MoveTo(rectangle What) { Pos = What; }
  virtual vector2d2d<textbox*> GetTextBoxList() { return TextBoxList; }
  virtual std::string GetSaveName() { return "jes.txt"; }
  static void Close(window*);
 protected:
  vector2d2d<textbox*> TextBoxList;
  rectangle Pos;
  unsigned short BackgroundColor, BorderColor, BarColor;
};

class editor : public window
{
 public:
  virtual void Draw();
  editor::editor(rectangle, char*);
  editor::~editor();
 protected:
  rectangle BitmapPos;
};

class resultwindow : public window
{
 public:
  virtual void Draw();
  resultwindow(rectangle, vector2d2d<string>, vector2d2d<unsigned int>, vector2d2d<unsigned int>);
  resultwindow() {}
  ~resultwindow();
  virtual void MoveTo(rectangle);
};

class editpersonwindow : public resultwindow
{
 public:
  editpersonwindow(rectangle);
  ~editpersonwindow();
  void Click(vector2d2d2d Where);
  static void Save(window*);
  void Draw();
  void Load(ifstream*);
  std::string GetSaveName();
 protected:
  functionbutton* OKButton;
};

class whattoedit : public window
{
 public:
  void Draw();
  whattoedit();
  void Click(vector2d2d2d);
 protected:
  functionbutton* OKButton;
  textbox* TextBox;
  edittextbox* EditTextBox;
};
#endif
  

