#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <string>
#include <allegro.h>

#include "graphics.h"
#include "vector2d2d.h"
#include "rect.h"
#include "strover.h"
class window;
class toolwindow;
class windowobject
{
 public:
<<<<<<< button.h
  virtual vector2d GetPos(void) { return Pos; }
  virtual bool IsIn(vector2d What) { return(What.X > Pos.X && What.X < Pos.X + 16 && What.Y > Pos.Y && What.Y < Pos.Y + 16); }
  virtual void Click(void) {}
  virtual void Draw(void) {}
  virtual void DeActivate(void) {}
  virtual void Move(vector2d What) { Pos = Pos + What;  }
  virtual rectangle GetRectangle(void) { return rectangle(Pos.Y, Pos.X, Pos.X + 16, Pos.Y + 16); }
=======
  virtual void Draw();
  virtual void Click();
  button(vector2d2d2d What) : Pos(What) {}
  button() {}
  virtual vector2d2d2d GetBitmapPos() const { return vector2d2d2d(0,0); }
  virtual vector2d2d2d GetSize() const { return vector2d2d2d(16,16); }
  virtual vector2d2d2d GetPos() const { return Pos; }
  virtual void SetPos(vector2d2d2d What) { Pos = What; }
  virtual bool IsIn(vector2d2d2d What) { return(What.X > Pos.X && What.X < Pos.X + 16 && What.Y > Pos.Y && What.Y < Pos.Y + 16); }
>>>>>>> 1.2
 protected:
  vector2d2d2d Pos;
};

class button : public windowobject
{
 public:
  virtual void Draw(void);
  virtual void Click(void);
  button(vector2d What) : Pos(What) {}
  button(void) {}
  virtual vector2d GetBitmapPos(void) const { return BitmapPos; }
  virtual vector2d GetSize(void) const { return vector2d(16,16); }
  virtual vector2d GetPos(void) const { return Pos; }
  virtual void SetPos(vector2d What) { Pos = What; }
  virtual void SetBitmapPos(vector2d What) { BitmapPos = What; }
  virtual bool IsIn(vector2d Where) { return(Where.X > GetPos().X && Where.X < GetPos().X + GetSize().X && Where.Y > GetPos().Y && Where.Y < GetPos().Y + GetSize().Y); }
  virtual void Move(vector2d What) { Pos = Pos + What;  }
  virtual rectangle GetRectangle(void) { return rectangle(Pos.Y, Pos.X, Pos.X + GetSize().X, Pos.Y + GetSize().Y); }
 protected:
  vector2d Pos, BitmapPos;
};

class programexitbutton : public button
{
 public:
  virtual vector2d2d2d GetBitmapPos() const { return vector2d2d2d(16,0); }
  virtual void Click();
  programexitbutton(vector2d2d2d);
};
  
class textbox : public windowobject
{
 public:
  textbox::textbox(std::string xText, vector2d2d2d What) : Text(xText) {Pos = What;}
  textbox::textbox() {}
  virtual void Draw();
  virtual void Click();
  virtual void SetText(std::string What) { Text = What; }
  virtual std::string GetText() { return Text; }
  virtual unsigned short GetBorderColor() { return BorderColor; }
  virtual bool IsIn(vector2d2d2d) { return false; }
  virtual void SetBorderColor(unsigned short What) { BorderColor = What; }
 protected:
  std::string Text;
  unsigned short BorderColor;
};

class edittextbox : public textbox
{
 public:
  edittextbox::edittextbox(rectangle What) { Pos = What; Text = ""; BorderColor = makecol(255,255,255);}
  virtual void Draw();
  virtual void Click();
  virtual bool IsIn(vector2d2d2d What) { return Pos.IsIn(What); }
 protected:
  rectangle Pos;
};

class functionbutton : public button
{
 public:
<<<<<<< button.h
  functionbutton(void (*xFunction)(window*), window* xCaller, vector2d xPos) {Function = xFunction; Caller = xCaller; Pos = xPos;}
  virtual vector2d GetSize(void) const { return vector2d(16,16); }
  virtual void Click(void);
=======
  functionbutton(void (*xFunction)(window*), window* xCaller, vector2d2d2d xPos) {Function = xFunction; Caller = xCaller; Pos = xPos;}
  virtual vector2d2d2d GetBitmapPos() const { return vector2d2d2d(16,0); }
  virtual vector2d2d2d GetSize() const { return vector2d2d2d(16,16); }
  virtual void Click();
>>>>>>> 1.2
 protected:
  window* Caller;
  void (*Function)(window*);
};

class toolbutton : public button
{
 public:
  toolbutton(unsigned char xToolNumber, toolwindow* xMainWindow, vector2d xPos, vector2d xBitmapPos) { ToolNumber = xToolNumber; MainWindow = xMainWindow; Pos = xPos; BitmapPos = xBitmapPos; }
  virtual void Click(void);
  virtual void Activate(void);
  virtual void DeActivate(void);
 protected:
  unsigned char ToolNumber;
  toolwindow* MainWindow;
};
#endif
