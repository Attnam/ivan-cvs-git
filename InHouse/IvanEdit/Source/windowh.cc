#include <allegro.h>
#include <algorithm>
#include <vector>
#include "window.h"
#include "windowh.h"
#include "cursor.h"
#include "button.h"

vector<window*> windowhandler::WindowList(0);
cursor* windowhandler::Cursor;
int windowhandler::MouseSpeed;
programexitbutton* windowhandler::ProgramExitButton;
void windowhandler::CreateWindow(window* NewWindow)
{
 WindowList.insert(WindowList.begin(), NewWindow);
}

void windowhandler::Draw(void)
{
 for(int x = 0; x < WindowList.size(); x++)
 {
  WindowList[x]->Draw();
 }
 ProgramExitButton->Draw();
 Cursor->Draw();
}

void windowhandler::Init(void)
{
 Cursor = new cursor;
 ProgramExitButton = new programexitbutton(vector2d(0,0));
 MouseSpeed = 1;
 set_mouse_speed(MouseSpeed, MouseSpeed);
}

void windowhandler::Click(vector2d Where)
{
 if(Where.X < 16 && Where.Y < 16)
  ProgramExitButton->Click();
 for(int x = WindowList.size() - 1; x != -1; x--)
 {
  if(WindowList[x]->GetPos().IsIn(Where))
  {
   WindowList[x]->Click(Where);
   break;
  }
 }
}

void windowhandler::BringOnTop(window* What)
{
 window** Temp = find(WindowList.begin(), WindowList.end(), What);
 if(Temp == WindowList.end() - 1)
  return;
 WindowList.insert(WindowList.end(), What);
 WindowList.erase(Temp);
}

void windowhandler::DeInit(void)
{
 delete Cursor;
 delete ProgramExitButton;
 for(int x = 0; x < WindowList.size(); x++)
  delete WindowList[x];
}
