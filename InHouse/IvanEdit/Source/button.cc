#include "button.h"
#include "graphics.h"
#include "program.h"
#include "rect.h"
#include "windowh.h"
#include "strover.h"
#include "cursor.h"
#include "window.h"
void button::Draw(void)
{
 graphics::DrawTool(Pos, GetBitmapPos(), GetSize());
}

void button::Click(void)
{

}

void programexitbutton::Click(void)
{
 program::SetRunning(false);
}

programexitbutton::programexitbutton(vector2d What)
{
 Pos = What;
}

void textbox::Draw(void)
{
 graphics::DrawRectangle(rectangle(Pos.Y, Pos.X, Pos.X + Text.size() * 8 + 1, Pos.Y + 9), GetBorderColor());
 graphics::DrawText(Text, vector2d(Pos.X + 1, Pos.Y + 1));
}

void textbox::Click(void)
{

}

void edittextbox::Click(void)
{
 char LastKey = 0;
 clear_keybuf();
 windowhandler::GetCursor()->SetShow(false);
 while(LastKey != 13)
 {
  if(LastKey == 8)
  {
   if(Text.size() > 0)
    Text.resize(Text.size() - 1);
  }
  else if(LastKey)
   Text = Text + LastKey;
  Text = Text + '_';
  windowhandler::Draw();
  graphics::BlitDoubleBuffer();
  Text.resize(Text.size() - 1);
  LastKey = readkey();
 }
 windowhandler::GetCursor()->SetShow(true);
}

void edittextbox::Draw(void)
{
 graphics::DrawRectangle(Pos, GetBorderColor());
 graphics::DrawText(Text, vector2d(Pos.Left+1, Pos.Top+1));
}

void functionbutton::Click(void)
{
 Function(Caller);
}

void toolbutton::Click(void)
{
 Activate();
}

void toolbutton::Activate(void)
{
 if(MainWindow->GetActive() != ToolNumber)
 {
  MainWindow->SetActive(ToolNumber);
  BitmapPos.Y += 16;
 }
}

void toolbutton::DeActivate(void)
{
 BitmapPos.Y -= 16;
 MainWindow->SetActive(0);
}
