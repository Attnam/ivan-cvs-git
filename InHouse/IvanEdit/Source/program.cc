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
/* vector<std::string> Names;

 Names.insert(Names.end(), "Heikki Joonas Valter Sairanen");
 Names.insert(Names.end(), "Min„");
 Names.insert(Names.end(), "Sin„");

 vector<unsigned int> Numbers;

 Numbers.insert(Numbers.end(), 2);
 Numbers.insert(Numbers.end(), 3);
 Numbers.insert(Numbers.end(), 120);
 windowhandler::CreateWindow(new window(rectangle(0,0,300,200), makecol(255,0,0), makecol(0,0,255), makecol(25,25,25)));
 windowhandler::CreateWindow(new resultwindow(rectangle(200,20,400,600), Names, Numbers, Numbers));
// windowhandler::CreateWindow(new editor(rectangle(300, 200, 700, 500), "item.pcx"));
*/
// windowhandler::CreateWindow(new editpersonwindow(rectangle(16, 10, 700, 500)));
 windowhandler::CreateWindow(new whattoedit);
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
 std::string Buffer;

 while(ch != ',')
 {
  From->get(ch);
  if(ch != ',') Buffer += ch;
 }
 return Buffer;
}
