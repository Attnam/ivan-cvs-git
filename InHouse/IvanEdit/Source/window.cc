#include <vector>
#include <algorithm>
#include <allegro.h>
#include <fstream>
#include <cstdlib>
#include "stdio.h"
#include "window.h"
#include "graphics.h"
#include "windowh.h"
#include "rect.h"
#include "program.h"

void window::Draw(void)
{
 DrawBackground();
 DrawButtons();
}
void window::DrawBackground(void)
{
 graphics::DrawFilledRectangle(rectangle(Pos.Top, Pos.Left, Pos.Right, Pos.Top + 16), BarColor);
 graphics::DrawFilledRectangle(rectangle(Pos.Top + 16, Pos.Left, Pos.Right, Pos.Bottom), BackgroundColor);
 graphics::DrawRectangle(Pos, BorderColor);
}

void window::BringOnTop(void)
{
 windowhandler::BringOnTop(this);
}



void window::Close(window* Caller)
{
 windowhandler::GetWindowList().erase(find(windowhandler::GetWindowList().begin(), windowhandler::GetWindowList().end(), Caller));
 delete Caller;
}


void window::Click(vector2d Where)
{
 BringOnTop();
 if(Where.Y < Pos.Top + 16)
 {
  vector2d PosInWindow(short(mouse_x) - GetPos().Left, short(mouse_y) - GetPos().Top);
  while(mouse_b & 1)
  {
   MoveTo(vector2d(mouse_x - PosInWindow.X, mouse_y - PosInWindow.Y));
   graphics::Clear();
   windowhandler::Draw();

   graphics::BlitDoubleBuffer();
  }
 }
 for(int x = 0; x < GetWindowObjectList().size(); x++)
 {
  if(GetWindowObjectList()[x]->IsIn(Where))
   GetWindowObjectList()[x]->Click();
 }
 if(Where.X > Pos.Right - 5 && Where.X < Pos.Right + 5 && Where.Y < Pos.Bottom + 5 && Where.Y > Pos.Bottom - 5)
  while(mouse_b & 1)
  {
   TryResize(vector2d(mouse_x, mouse_y));
   windowhandler::Draw();
   graphics::BlitDoubleBuffer();
   graphics::Clear();
  }
}


editorwindow::editorwindow(rectangle xPos, rectangle xEditorPos, toolwindow* xLinkedTools)
{
 Pos = xPos;
 BitmapPos = xEditorPos;
 LinkedTools = xLinkedTools;
 BackgroundColor = makecol(255,255,255);
 BorderColor = makecol(0,255,255);
 BarColor = makecol(0,0,255);
 Data = create_bitmap(16,16);
 UpdateGraphics();
}

void editorwindow::Draw(void)
{
 DrawBackground();
 graphics::DrawStretched(rectangle(Pos.Top + 17, Pos.Left + 1, Pos.Right - 1, Pos.Bottom - 1), Data);
 DrawButtons();
}

void editorwindow::Click(vector2d Where)
{
 if(Where.Y - Pos.Top > 16 && Where.X > Pos.Left)
  switch(LinkedTools->GetActive())
  {
   case 1:
    DrawPixel(Where);
   default:
   break;
  }


 window::Click(Where);


}
void window::DrawButtons(void)
{
 for(int x = 0; x < WindowObjectList.size(); x++)
  WindowObjectList[x]->Draw();
}

void window::TryResize(vector2d ToWhat)
{
 vector2d Largest(24,40);
 long Temp;
 for(int x = 0; x < WindowObjectList.size(); x++)
 {
  if((Temp = WindowObjectList[x]->GetRectangle().Right) > Largest.X) Largest.X = Temp;
  if((Temp = WindowObjectList[x]->GetRectangle().Bottom) > Largest.Y) Largest.Y = Temp;
 }
 if(ToWhat.X > Largest.X + 8 + Pos.Left && ToWhat.Y > Largest.Y + 8 + Pos.Top)
  SetPos(rectangle(Pos.Top, Pos.Left, ToWhat.X, ToWhat.Y));
}

void chooserwindow::Draw(void)
{
 DrawBackground();
 graphics::BlitData(rectangle(Scroll.Y, Scroll.X,Pos.Right - Pos.Left, Pos.Bottom - Pos.Top), rectangle(Pos.Top + 16, Pos.Left + 1, Pos.Right - 16, Pos.Bottom - 1));
 graphics::DrawStretched(rectangle(Pos.Top, Pos.Right - 16, Pos.Right, Pos.Bottom), ScrollBar);
 rectangle Temp(Selected.Top + 16 + Pos.Top - Scroll.Y, Selected.Left + 1 + Pos.Left - Scroll.X, Selected.Right + 1 + Pos.Left - Scroll.X, Selected.Bottom + 16 + Pos.Top - Scroll.Y);
 if(Temp.Left >= Pos.Left && Temp.Right <= Pos.Right - 16 && Temp.Top >= Pos.Top + 16 && Temp.Bottom <= Pos.Bottom)
  graphics::DrawRectangle(Temp, makecol(0,255,0));

}

chooserwindow::chooserwindow(rectangle xPos, editorwindow* xLinked)
{
 SetScroll(vector2d(0,0));
 Pos = xPos;
 BackgroundColor = makecol(255,255,255);
 BarColor = makecol(0,0,255);
 BorderColor = makecol(0,0,255);
 Linked = xLinked;
 ScrollBar = create_bitmap(16,62);
 graphics::BlitToolTo(rectangle(18,0,16,80), ScrollBar);
}

void chooserwindow::Click(vector2d Where)
{
 BringOnTop();
 if(Where.X > Pos.Right - 16 && Where.Y < Pos.Bottom - 10)
 {
  if((Where.Y - Pos.Top) * 2 + Pos.Top > Pos.Bottom)
   ScrollDown();
  else
   ScrollUp();
 }
 else if(Where.X <= Pos.Right - 16 && !(Pos.Top + 16 > Where.Y))
 {
  vector2d RealPictureStart(Pos.Left + 1 - Scroll.X, Pos.Top + 16 - Scroll.Y);
  vector2d Temp = Where;
  Temp =  Temp - RealPictureStart;
  rectangle Temp2 = rectangle(Temp.Y - Temp.Y % 16, Temp.X - Temp.X % 16, Temp.X + 16 - Temp.X % 16, Temp.Y + 16 - Temp.Y % 16);
  Linked->SetBitmapPos(Temp2);
  SetSelected(Temp2);
 }
 window::Click(Where);
}

editorwindow::~editorwindow(void)
{
 destroy_bitmap(Data);
}

void editorwindow::SetBitmapPos(rectangle What)
{
 BitmapPos = What;
 blit(graphics::GetData(), Data, BitmapPos.Left,BitmapPos.Top,0,0,BitmapPos.Right - BitmapPos.Left,BitmapPos.Bottom - BitmapPos.Top);
}

toolwindow::toolwindow(rectangle xPos)
{
 Pos = xPos;
 BackgroundColor = makecol(255,255,255);
 BorderColor = makecol(255,0,0);
 BarColor = makecol(0,0,255);
 WindowObjectList.insert(WindowObjectList.begin(), new toolbutton(WindowObjectList.size()+1, this, vector2d(Pos.Left + 3, Pos.Top + 32), vector2d(16,16)));
 WindowObjectList.insert(WindowObjectList.end(), new toolbutton(WindowObjectList.size()+1, this, vector2d(Pos.Left + 20, Pos.Top + 32), vector2d(16,16)));
 Active = 0;
}

void window::MoveTo(vector2d What)
{
 vector2d Movement = What - vector2d(Pos.Left, Pos.Top);
 Pos = rectangle(Pos.Top + Movement.Y, Pos.Left + Movement.X, Pos.Right + Movement.X, Pos.Bottom + Movement.Y);
 if(Movement != vector2d(0,0))
  for(int x = 0; x < WindowObjectList.size(); x++)
   WindowObjectList[x]->Move(Movement);
}

void toolwindow::SetActive(unsigned char What)
{
 if(!What)
 {
  Active = 0;
  return;
 }
 if(Active)
  WindowObjectList[Active - 1]->DeActivate();
 Active = What;
}

void editorwindow::DrawPixel(vector2d Where)
{
 float MultX = float(Pos.Right - Pos.Left - 2) / 16, MultY = float(Pos.Bottom - Pos.Top - 17) / 16;

 vector2d RealClickPos((1/MultX)*(Where.X - Pos.Left - 2) + BitmapPos.Left, (1/MultY)*(Where.Y - Pos.Top - 17) + BitmapPos.Top);
 graphics::DrawPixelOnData(RealClickPos, 2);
 UpdateGraphics();
}

void editorwindow::UpdateGraphics(void)
{
 blit(graphics::GetData(), Data, BitmapPos.Left,BitmapPos.Top,0,0,BitmapPos.Right - BitmapPos.Left,BitmapPos.Bottom - BitmapPos.Top);
}
