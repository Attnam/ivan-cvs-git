#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <string>
#include <allegro.h>

#include "graphics.h"
#include "vector.h"
#include "rect.h"
#include "strover.h"
class window;
class button
{
 public:
  virtual void Draw(void);
  virtual void Click(void);
  button(vector2d What) : Pos(What) {}
  button(void) {}
  virtual vector2d GetBitmapPos(void) const { return vector2d(0,0); }
  virtual vector2d GetSize(void) const { return vector2d(16,16); }
  virtual vector2d GetPos(void) const { return Pos; }
  virtual void SetPos(vector2d What) { Pos = What; }
  virtual bool IsIn(vector2d What) { return(What.X > Pos.X && What.X < Pos.X + 16 && What.Y > Pos.Y && What.Y < Pos.Y + 16); }
 protected:
  vector2d Pos;
};

class programexitbutton : public button
{
 public:
  virtual vector2d GetBitmapPos(void) const { return vector2d(16,0); }
  virtual void Click(void);
  programexitbutton(vector2d);
};
  
class textbox : public button
{
 public:
  textbox::textbox(std::string xText, vector2d What) : Text(xText) {Pos = What;}
  textbox::textbox(void) {}
  virtual void Draw(void);
  virtual void Click(void);
  virtual void SetText(std::string What) { Text = What; }
  virtual std::string GetText(void) { return Text; }
  virtual unsigned short GetBorderColor(void) { return BorderColor; }
  virtual bool IsIn(vector2d) { return false; }
  virtual void SetBorderColor(unsigned short What) { BorderColor = What; }
 protected:
  std::string Text;
  unsigned short BorderColor;
};

class edittextbox : public textbox
{
 public:
  edittextbox::edittextbox(rectangle What) { Pos = What; Text = ""; BorderColor = makecol(255,255,255);}
  virtual void Draw(void);
  virtual void Click(void);
  virtual bool IsIn(vector2d What) { return Pos.IsIn(What); }
 protected:
  rectangle Pos;
};

class functionbutton : public button
{
 public:
  functionbutton(void (*xFunction)(window*), window* xCaller, vector2d xPos) {Function = xFunction; Caller = xCaller; Pos = xPos;}
  virtual vector2d GetBitmapPos(void) const { return vector2d(16,0); }
  virtual vector2d GetSize(void) const { return vector2d(16,16); }
  virtual void Click(void);
 protected:
  void (*Function)(window*);
  window* Caller;
};
  

#endif
