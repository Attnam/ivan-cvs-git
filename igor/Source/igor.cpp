#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "feio.h"
#include "colorbit.h"
#include "felist.h"

#define STRETCH 5

int Main(int, char**)
{
  std::string OldDirectory;
  std::ifstream IConfigFile("igor.cfg");
  if(IConfigFile)
    {
      char ch;
      while(IConfigFile.get(ch))
	OldDirectory += ch;
    }
  IConfigFile.close();

  std::cout << std::endl << std::endl << "Where is the Graphics/ directory. [" + OldDirectory + "] ";
  char ch;
  std::string Directory;
  while((ch = getchar()) != '\n')
    Directory += ch;


  if(Directory == "")
    Directory = OldDirectory;
  if(Directory.empty() || Directory[Directory.size() - 1] != '/')
    Directory += '/';

  std::ofstream OConfigFile("igor.cfg");
  OConfigFile << Directory;
  OConfigFile.close();

  graphics::Init();
  graphics::SetMode("IGOR 1.203", 0, vector2d(800, 600), true);
  graphics::LoadDefaultFont(Directory + "Font.pcx");
  DOUBLE_BUFFER->Fill(0);
  colorizablebitmap* CBitmap;
  felist List("Choose file to edit:");
  List.AddEntry("Char.pcx", LIGHT_GRAY);
  List.AddEntry("Humanoid.pcx", LIGHT_GRAY);
  List.AddEntry("Item.pcx", LIGHT_GRAY);
  List.AddEntry("GLTerra.pcx", LIGHT_GRAY);
  List.AddEntry("OLTerra.pcx", LIGHT_GRAY);
  ushort Selected;
  std::string FileName;
  List.SetPos(vector2d(300, 250));
  List.SetWidth(200);

  while((Selected = List.Draw()) & FELIST_ERROR_BIT);

  switch(Selected)
    {
    case 0: FileName = Directory + "Char.pcx"; break;
    case 1: FileName = Directory + "Humanoid.pcx"; break;
    case 2: FileName = Directory + "Item.pcx"; break;
    case 3: FileName = Directory + "GLTerra.pcx"; break;
    case 4: FileName = Directory + "OLTerra.pcx"; break;
    }

  CBitmap = new colorizablebitmap(FileName);
  bitmap CursorBitmap(Directory + "Cursor.pcx");
  vector2d Cursor(0, 0);
  int k = 0;
  Selected = 0;
  ushort Color[4] = { MakeRGB16(47, 131, 95), MakeRGB16(123, 0, 127), MakeRGB16(0, 131, 131), MakeRGB16(175, 131, 0) };
  std::vector<vector2d> DrawQueue;

  while(true)
    {
      static vector2d MoveVector[] = { vector2d(0, -16), vector2d(-16, 0), vector2d(0, 16), vector2d(16, 0) };
      static int Key[] = { 'w', 'a', 's', 'd' };

      ushort c;

      for(c = 0; c < 4; ++c)
	{
	  if(Key[c] == k)
	    {
	      vector2d NewPos = Cursor + MoveVector[c];

	      if(NewPos.X >= 0 && NewPos.X <= CBitmap->GetXSize() - 16 && NewPos.Y >= 0 && NewPos.Y <= CBitmap->GetYSize() - 16)
		Cursor = NewPos;

	      break;
	    }

	  if((Key[c]&~0x20) == k)
	    {
	      vector2d NewPos = Cursor + (MoveVector[c] << 2);

	      if(NewPos.X >= 0 && NewPos.X <= CBitmap->GetXSize() - 16 && NewPos.Y >= 0 && NewPos.Y <= CBitmap->GetYSize() - 16)
		Cursor = NewPos;

	      break;
	    }
	}

      if(k >= 0x31 && k <= 0x34)
	Selected = k - 0x31;
      else if(k == '+')
	CBitmap->AlterGradient(Cursor, 16, 16, Selected, 1, false);
      else if(k == '-')
	CBitmap->AlterGradient(Cursor, 16, 16, Selected, -1, false);
      else if(k == '>')
	CBitmap->AlterGradient(Cursor, 16, 16, Selected, 1, true);
      else if(k == '<')
	CBitmap->AlterGradient(Cursor, 16, 16, Selected, -1, true);
      else if(k == KEY_UP) 
	CBitmap->Roll(Cursor, 16, 16, 0, -1);
      else if(k == KEY_DOWN)
	CBitmap->Roll(Cursor, 16, 16, 0, 1);
      else if(k == KEY_RIGHT)
	CBitmap->Roll(Cursor, 16, 16, 1, 0);
      else if(k == KEY_LEFT)
	CBitmap->Roll(Cursor, 16, 16, -1, 0);
      else if(k == '=')
	{
	  FONT->Printf(DOUBLE_BUFFER, 10, 460, RED, "Select color to swap with [1-4/ESC]");
	  graphics::BlitDBToScreen();

	  for(k = GET_KEY(); k != 0x1B; k = GET_KEY())
	    if(k >= 0x31 && k <= 0x34)
	      {
		CBitmap->SwapColors(Cursor, 16, 16, Selected, k - 0x31);
		break;
	      }
	}
      else if(k == 0x1B)
	{
	  FONT->Printf(DOUBLE_BUFFER, 10, 460, RED, "Save? [y/n/c]");
	  graphics::BlitDBToScreen();

	  while(true)
	    {
	      k = GET_KEY();

	      if(k == 'y' || k == 'Y')
		{
		  CBitmap->Save(FileName);
		  delete CBitmap;
		  return 0;
		}

	      if(k == 'n' || k == 'N')
		{
		  delete CBitmap;
		  return 0;
		}

	      if(k == 'c' || k == 'C')
		break;
	    }
	}
      else if(k == 'p')
	{
	  std::vector<vector2d>::iterator i = std::find(DrawQueue.begin(), DrawQueue.end(), Cursor);

	  if(i == DrawQueue.end())
	    DrawQueue.push_back(Cursor);
	  else
	    DrawQueue.erase(i);
	}
      else if(k == 'c')
	DrawQueue.clear();

      DOUBLE_BUFFER->Fill(0);
      DOUBLE_BUFFER->Fill(0, 0, CBitmap->GetSize(), 0xF81F);
      CBitmap->MaskedBlit(DOUBLE_BUFFER, 0, 0, 0, 0, CBitmap->GetSize(), Color);
      DOUBLE_BUFFER->DrawRectangle(RES.X - STRETCH * 16 - 12, RES.Y - STRETCH * 16 - 12, RES.X - 9, RES.Y - 9, DARK_GRAY, true);
      DOUBLE_BUFFER->DrawRectangle(RES.X - STRETCH * 32 - 22, RES.Y - STRETCH * 16 - 12, RES.X - STRETCH * 16 - 19, RES.Y - 9, DARK_GRAY, true);
      DOUBLE_BUFFER->StretchBlit(DOUBLE_BUFFER, Cursor, RES.X - STRETCH * 16 - 10, RES.Y - STRETCH * 16 - 10, 16, 16, STRETCH);
      FONT->Printf(DOUBLE_BUFFER, 10, 480, WHITE, "Control cursor: wasd and WASD");
      FONT->Printf(DOUBLE_BUFFER, 10, 490, WHITE, "Select m-color: 1-4");
      FONT->Printf(DOUBLE_BUFFER, 10, 500, WHITE, "Safely alter gradient: ±");
      FONT->Printf(DOUBLE_BUFFER, 10, 510, WHITE, "Power alter gradient: <>");
      FONT->Printf(DOUBLE_BUFFER, 10, 520, WHITE, "Swap m-colors: =");
      FONT->Printf(DOUBLE_BUFFER, 10, 530, WHITE, "Push to / pop from draw queue: p");
      FONT->Printf(DOUBLE_BUFFER, 10, 540, WHITE, "Clear draw queue: c");
      FONT->Printf(DOUBLE_BUFFER, 10, 550, WHITE, "Roll picture: arrow keys");
      FONT->Printf(DOUBLE_BUFFER, 10, 570, WHITE, "MColor selected: %d", Selected + 1);
      FONT->Printf(DOUBLE_BUFFER, 10, 580, WHITE, "Current position: (%d, %d)", Cursor.X, Cursor.Y);

      for(c = 0; c < DrawQueue.size(); ++c)
	{
	  DOUBLE_BUFFER->StretchBlit(DOUBLE_BUFFER, DrawQueue[c], RES.X - STRETCH * 32 - 20, RES.Y - STRETCH * 16 - 10, 16, 16, STRETCH);
	  CursorBitmap.MaskedBlit(DOUBLE_BUFFER, 0, 0, DrawQueue[c], 16, 16);
	}

      CursorBitmap.MaskedBlit(DOUBLE_BUFFER, 0, 0, Cursor, 16, 16);
      graphics::BlitDBToScreen();
      k = GET_KEY();
    }

  return 1;
}
