#include <allegro.h>
#include "program.h"
#include "graphics.h"
#include "windowh.h"
#include "rect.h"
#include "window.h"
bool program::Running = true;

void program::Init(void)
{
 graphics::Init();
 windowhandler::Init();
 install_keyboard();
 install_mouse();
 toolwindow* JustSomething = new toolwindow(rectangle(100,500, 564, 200));
 windowhandler::CreateWindow(JustSomething);
 editorwindow* JustCreated = new editorwindow(rectangle(20,20,400,400), rectangle(0,16,32,16), JustSomething);
 windowhandler::CreateWindow(JustCreated);
 windowhandler::CreateWindow(new chooserwindow(rectangle(420,20,420,600), JustCreated));
}

void program::DeInit(void)
{
 graphics::DeInit();
 windowhandler::DeInit();
}

void program::Run(void)
{
 while(Running)
 {
  graphics::Clear();
  windowhandler::Draw();
  graphics::BlitDoubleBuffer();
  if(mouse_b & 1) windowhandler::Click(vector2d(mouse_x, mouse_y));
 }

}

std::string program::ReadNextWord(ifstream* From)
{
 char ch = 0;
 std::string Buffer = "";

 while(ch != ',')
 {
  From->get(ch);
  if(ch != ',') Buffer += ch;
 }
 return Buffer;
}


