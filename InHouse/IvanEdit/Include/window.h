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
  virtual void Draw(void);
  virtual void SetPos(rectangle What) { Pos = What; }
  window(rectangle Pos, unsigned short BackgroundColor, unsigned short BorderColor, unsigned short BarColor) : Pos(Pos), BackgroundColor(BackgroundColor), BorderColor(BorderColor), BarColor(BarColor) {}
  void BringOnTop(void);
  virtual void Click(vector2d);
  virtual rectangle GetPos(void) { return Pos; }
  window(void) {}
  virtual void SetBackgroundColor(unsigned short What) { BackgroundColor = What; }
  virtual void DrawBackground(void);
  virtual void MoveTo(rectangle What) { Pos = What; }
  virtual vector<textbox*> GetTextBoxList(void) { return TextBoxList; }
  virtual std::string GetSaveName(void) { return "jes.txt"; }
  static void Close(window*);
 protected:
  vector<textbox*> TextBoxList;
  rectangle Pos;
  unsigned short BackgroundColor, BorderColor, BarColor;
};

class editor : public window
{
 public:
  virtual void Draw(void);
  editor::editor(rectangle, char*);
  editor::~editor(void);
 protected:
  rectangle BitmapPos;
};

class resultwindow : public window
{
 public:
  virtual void Draw(void);
  resultwindow(rectangle, vector<string>, vector<unsigned int>, vector<unsigned int>);
  resultwindow(void) {}
  ~resultwindow(void);
  virtual void MoveTo(rectangle);
};

class editpersonwindow : public resultwindow
{
 public:
  editpersonwindow(rectangle);
  ~editpersonwindow(void);
  void Click(vector2d Where);
  static void Save(window*);
  void Draw(void);
  void Load(ifstream*);
  std::string GetSaveName(void);
 protected:
  functionbutton* OKButton;
};

class whattoedit : public window
{
 public:
  void Draw(void);
  whattoedit(void);
  void Click(vector2d);
 protected:
  functionbutton* OKButton;
  textbox* TextBox;
  edittextbox* EditTextBox;
};
#endif
  
