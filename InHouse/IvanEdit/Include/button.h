#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <string>
#include <allegro.h>

#include "graphics.h"
#include "vector2d2d.h"
#include "rect.h"
#include "strover.h"
class window;
class button
{
 public:
  virtual void Draw();
  virtual void Click();
  button(vector2d2d2d What) : Pos(What) {}
  button() {}
  virtual vector2d2d2d GetBitmapPos() const { return vector2d2d2d(0,0); }
  virtual vector2d2d2d GetSize() const { return vector2d2d2d(16,16); }
  virtual vector2d2d2d GetPos() const { return Pos; }
  virtual void SetPos(vector2d2d2d What) { Pos = What; }
  virtual bool IsIn(vector2d2d2d What) { return(What.X > Pos.X && What.X < Pos.X + 16 && What.Y > Pos.Y && What.Y < Pos.Y + 16); }
 protected:
  vector2d2d2d Pos;
};

class programexitbutton : public button
{
 public:
  virtual vector2d2d2d GetBitmapPos() const { return vector2d2d2d(16,0); }
  virtual void Click();
  programexitbutton(vector2d2d2d);
};
  
class textbox : public button
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
  functionbutton(void (*xFunction)(window*), window* xCaller, vector2d2d2d xPos) {Function = xFunction; Caller = xCaller; Pos = xPos;}
  virtual vector2d2d2d GetBitmapPos() const { return vector2d2d2d(16,0); }
  virtual vector2d2d2d GetSize() const { return vector2d2d2d(16,16); }
  virtual void Click();
 protected:
  void (*Function)(window*);
  window* Caller;
};
  

#endif

