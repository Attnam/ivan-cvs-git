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
}
void window::DrawBackground(void)
{
 graphics::DrawFilledRectangle(rectangle(Pos.Top, Pos.Left, Pos.Right, Pos.Top + 20), BarColor);
 graphics::DrawFilledRectangle(rectangle(Pos.Top + 20, Pos.Left, Pos.Right, Pos.Bottom), BackgroundColor);
 graphics::DrawRectangle(Pos, BorderColor);
}
void window::Click(vector2d ClickPos)
{
 BringOnTop();
 if(rectangle(Pos.Top, Pos.Left, Pos.Right, Pos.Top + 20).IsIn(ClickPos))
 {
  vector2d LastPos = vector2d(mouse_x, mouse_y);
  vector2d PositionOnWindow = vector2d(ClickPos.X - Pos.Left, ClickPos.Y - Pos.Top);
  while(mouse_b & 1)
  {
   rectangle Temp(mouse_y - PositionOnWindow.Y, mouse_x - PositionOnWindow.X, mouse_x + Pos.Right - Pos.Left - PositionOnWindow.X, mouse_y + Pos.Bottom - Pos.Top - PositionOnWindow.Y);
   MoveTo(Temp);
   graphics::Clear();
   windowhandler::Draw();
   graphics::BlitDoubleBuffer();
  }
 }
}

void window::BringOnTop(void)
{
 windowhandler::BringOnTop(this);
}


editor::editor(rectangle Rect,char* FileName)
{
 BackgroundColor = makecol(255,255,255);
 BorderColor = makecol(150,150,150);
 BarColor = makecol(20,20,200);
 Pos = Rect;
 rectangle Temp(0,0,16,16);
 BitmapPos = Temp;
}

editor::~editor(void)
{

}

void editor::Draw(void)
{
 DrawBackground();
 graphics::DrawStretchData(BitmapPos, rectangle(Pos.Top + 20, Pos.Left, Pos.Right, Pos.Bottom));
// graphics::DrawRectangle(Pos, BackgroundColor);
}

resultwindow::resultwindow(rectangle What, vector<string> Names, vector<unsigned int> Strokes, vector<unsigned int> Bogi)
{
 Pos = What;
 BackgroundColor = makecol(10,10,10);
 BorderColor = makecol(0,0,200);
 BarColor = makecol(0,0,200);
 for(int x = 0; x < Names.size(); x++)
  TextBoxList.insert(TextBoxList.end(), new textbox(Names[x], vector2d(What.Left + 10, What.Top + x * 10 + 30)));
}

void resultwindow::Draw(void)
{
 DrawBackground();
 for(int x = 0; x < TextBoxList.size(); x++)
  TextBoxList[x]->Draw();
}
 

void resultwindow::MoveTo(rectangle Where)
{
 for(int x = 0; x < TextBoxList.size(); x++)
  TextBoxList[x]->SetPos(vector2d(Where.Left + (TextBoxList[x]->GetPos().X - Pos.Left), Where.Top + (TextBoxList[x]->GetPos().Y - Pos.Top)));
 Pos = Where;
}

resultwindow::~resultwindow(void)
{
 while(TextBoxList.size())
 {
  delete TextBoxList[0];
  TextBoxList.erase(0);
 }
}

void editpersonwindow::Load(ifstream* From)
{
 program::ReadNextWord(From);
 for(int x = 0; x < 16; x++)
  TextBoxList[x]->SetText(program::ReadNextWord(From));
}
editpersonwindow::editpersonwindow(rectangle What)
{
 Pos = What;
 TextBoxList.insert(TextBoxList.end(), new textbox("Nimi", vector2d(Pos.Left + 5, Pos.Top + 30)));
 TextBoxList.insert(TextBoxList.end(), new edittextbox(rectangle(Pos.Top + 30, Pos.Left + 110, Pos.Left + 500, Pos.Top + 39)));
 TextBoxList.insert(TextBoxList.end(), new textbox("Tasoitus", vector2d(Pos.Left + 5, Pos.Top + 45)));
 TextBoxList.insert(TextBoxList.end(), new edittextbox(rectangle(Pos.Top + 45, Pos.Left + 110, Pos.Left + 500, Pos.Top + 54)));
 TextBoxList.insert(TextBoxList.end(), new textbox("Jyv„skyl„ sc", vector2d(Pos.Left + 5, Pos.Top + 60)));
 TextBoxList.insert(TextBoxList.end(), new edittextbox(rectangle(Pos.Top + 60, Pos.Left + 110, Pos.Left + 500, Pos.Top + 69)));
 TextBoxList.insert(TextBoxList.end(), new textbox("Jyv„skyl„ pb", vector2d(Pos.Left + 5, Pos.Top + 75)));
 TextBoxList.insert(TextBoxList.end(), new edittextbox(rectangle(Pos.Top + 75, Pos.Left + 110, Pos.Left + 500, Pos.Top + 84)));
 TextBoxList.insert(TextBoxList.end(), new textbox("Virrat sc", vector2d(Pos.Left + 5, Pos.Top + 90)));
 TextBoxList.insert(TextBoxList.end(), new edittextbox(rectangle(Pos.Top + 90, Pos.Left + 110, Pos.Left + 500, Pos.Top + 99)));
 TextBoxList.insert(TextBoxList.end(), new textbox("Virrat pb", vector2d(Pos.Left + 5, Pos.Top + 105)));
 TextBoxList.insert(TextBoxList.end(), new edittextbox(rectangle(Pos.Top + 105, Pos.Left + 110, Pos.Left + 500, Pos.Top + 114)));
 TextBoxList.insert(TextBoxList.end(), new textbox("Vammala sc", vector2d(Pos.Left + 5, Pos.Top + 120)));
 TextBoxList.insert(TextBoxList.end(), new edittextbox(rectangle(Pos.Top + 120, Pos.Left + 110, Pos.Left + 500, Pos.Top + 129)));
 TextBoxList.insert(TextBoxList.end(), new textbox("Vammala pb", vector2d(Pos.Left + 5, Pos.Top + 135)));
 TextBoxList.insert(TextBoxList.end(), new edittextbox(rectangle(Pos.Top + 135, Pos.Left + 110, Pos.Left + 500, Pos.Top + 144)));

 OKButton = new functionbutton(&Save, this, vector2d(Pos.Left + 100, Pos.Bottom - 100));
 BackgroundColor = makecol(255,255,255);
 BarColor = makecol(0,0,200);
 BorderColor = makecol(100,100,100);
 std::ifstream Temp("jes.txt");
 Load(&Temp);
 Temp.close();
}

void editpersonwindow::Click(vector2d Where)
{
 for(int x = 0; x < TextBoxList.size(); x++)
 {
  if(TextBoxList[x]->IsIn(Where))
   TextBoxList[x]->Click();
 }
 if(OKButton->IsIn(Where))
  OKButton->Click();
}

void editpersonwindow::Save(window* Window)
{
 std::ofstream to(Window->GetSaveName().c_str());
 std::string Buffer;
 for(int x = 0; x < Window->GetTextBoxList().size(); x++)
  Buffer += std::string(",") + Window->GetTextBoxList()[x]->GetText();
 Buffer += std::string(",");
 for(int x = 0; x < Buffer.length(); x++)
  to.put(Buffer[x]);

}

editpersonwindow::~editpersonwindow(void)
{
 delete OKButton;
}

void editpersonwindow::Draw(void)
{
 DrawBackground();
 for(int x = 0; x < TextBoxList.size(); x++)
  TextBoxList[x]->Draw();
 OKButton->Draw();

}

std::string editpersonwindow::GetSaveName(void)
{
 std::string Buffer = TextBoxList[1]->GetText();
 Buffer.resize(8);

 for(int x = 0; x < 8; x++)
  if(Buffer[x] == ' ') Buffer[x] = 'a';
 Buffer += ".gol";
 for(int x = 0; x < 10; x++)
 {
  Buffer[7] = char(x + 48);
  ifstream Temp(Buffer.c_str());
  if(!Temp)
  {
   Temp.close();
   break;
  }
  else
  {
   program::ReadNextWord(&Temp);
   program::ReadNextWord(&Temp);
   if(program::ReadNextWord(&Temp) == this->GetTextBoxList()[1]->GetText())
    break;
   Temp.close();
  }
 }

 return Buffer;
}

whattoedit::whattoedit(void)
{
 BackgroundColor = makecol(0,0,0);
 BarColor = makecol(0,0,255);
 BorderColor = makecol(255,255,255);
 rectangle Temp = rectangle(100, 100, 600, 600);
 Pos = Temp;
 TextBox = new textbox("Kenen tiedot haluat n„hd„?", vector2d(Pos.Top + 50, Pos.Left + 100));
 OKButton = new functionbutton(&Close, this, vector2d(Pos.Top + 150, Pos.Left + 130));
 EditTextBox = new edittextbox(rectangle(Pos.Top + 110, Pos.Left + 100, Pos.Left + 400, Pos.Top + 120));
}

void whattoedit::Draw(void)
{
 DrawBackground();
 TextBox->Draw();
 OKButton->Draw();
 EditTextBox->Draw();
}

void window::Close(window* Caller)
{

 //windowhandler::GetWindowList().clear();
 find(windowhandler::GetWindowList().begin(), windowhandler::GetWindowList().end(), Caller);
 delete Caller;
}

void whattoedit::Click(vector2d Where)
{
  if(EditTextBox->IsIn(Where))
   EditTextBox->Click();
  if(OKButton->IsIn(Where))
   OKButton->Click();
}
