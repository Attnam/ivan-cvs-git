#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "colorbit.h"
#include "save.h"

const felist* FelistCurrentlyDrawn = 0;

bool FelistDrawController()
{
  FelistCurrentlyDrawn->DrawPage(DOUBLEBUFFER);
  return true;
}

felistentry::felistentry(const std::vector<bitmap*>& BitmapVector, const std::string& String, ushort Color, bool Selectable) : String(String), Color(Color), Selectable(Selectable)
{
  for(ushort c = 0; c < BitmapVector.size(); ++c)
    Bitmap.push_back(new bitmap(BitmapVector[c]));
}

felist::~felist()
{
  Empty();
}

ushort felist::Draw(vector2d DrawPos, ushort DrawWidth, ushort DrawPageLength, ushort DrawBackColor, bool DrawSelectable, bool BlitAfterwards, bool DrawBackroundAfterwards, bool Fade, bool DrawInverseMode)
{
  if(!Entry.size())
    return 0xFFFF;

  Pos = DrawPos;
  Width = DrawWidth;
  PageLength = DrawPageLength;
  Selectable = DrawSelectable;
  BackColor = DrawBackColor;
  InverseMode = DrawInverseMode;

  FelistCurrentlyDrawn = this;

  if(globalwindowhandler::ControlLoopsInstalled())
    globalwindowhandler::InstallControlLoop(FelistDrawController);

  bitmap BackGround(RES);
  bitmap* Buffer;

  if(Fade)
    {
      Buffer = new bitmap(RES, 0);
      BackGround.Fill(0);
    }
  else
    {
      Buffer = DOUBLEBUFFER;
      DOUBLEBUFFER->Blit(&BackGround);
    }

  if(Selectable)
    PageBegin = Selected - Selected % PageLength;
  else if(InverseMode)
    PageBegin = LastEntryIndex() - LastEntryIndex() % PageLength;
  else
    PageBegin = 0;

  ushort Return, c, Selectables = 0;

  bool JustSelectMove = false;

  for(c = 0; c < Entry.size(); ++c)
    if(Entry[c].Selectable)
      ++Selectables;

  for(;;)
    {
      bool AtTheEnd = DrawPage(Buffer);

      if(Fade)
	{
	  if(JustSelectMove)
	    {
	      Buffer->Blit(DOUBLEBUFFER);
	      graphics::BlitDBToScreen();
	    }
	  else
	    Buffer->FadeToScreen();

	  JustSelectMove = false;
	}
      else
	graphics::BlitDBToScreen();

      int Pressed = GETKEY(false);

      if(Selectable && Pressed > 64 && Pressed < 91)
	{
	  Return = Pressed - 65 + PageBegin < long(Entry.size()) ? Pressed - 65 + PageBegin : 0xFFFF;
	  break;
	}

      if(Selectable && Pressed > 96 && Pressed < 123)
	{
	  Return = Pressed - 97 + PageBegin < long(Entry.size()) ? Pressed - 97 + PageBegin : 0xFFFF;
	  break;
	}

      if(Selectable && Pressed == 0x148)
	{
	  if(Selected)
	    {
	      --Selected;

	      if(Selected < PageBegin)
		{
		  BackGround.Blit(Buffer);
		  PageBegin -= PageLength;
		}
	      else
		JustSelectMove = true;
	    }
	  else
	    {
	      for(c = 0, Selected = 0xFFFF; c < Entry.size(); ++c)
		if(Entry[c].Selectable)
		  ++Selected;

	      if(PageBegin == Selected - Selected % PageLength)
		JustSelectMove = true;
	      else
		{
		  BackGround.Blit(Buffer);
		  PageBegin = Selected - Selected % PageLength;
		}
	    }

	  continue;
	}

      if(Selectable && Pressed == 0x150)
	{
	  if(!AtTheEnd || Selected != Selectables - 1)
	    {
	      ++Selected;

	      if(Selected > PageBegin + PageLength - 1)
		{
		  BackGround.Blit(Buffer);
		  PageBegin += PageLength;
		}
	      else
		JustSelectMove = true;
	    }
	  else
	    {
	      if(!PageBegin)
		JustSelectMove = true;
	      else
		BackGround.Blit(Buffer);

	      Selected = PageBegin = 0;
	    }

	  continue;
	}

      if(Selectable && Pressed == 0x0D)
	{
	  Return = Selected;
	  break;
	}

      if(Pressed == 0x1B || (AtTheEnd && !InverseMode) || (!PageBegin && InverseMode))
	{
	  Return = 0xFFFF;
	  break;
	}
      else
	{
	  BackGround.Blit(Buffer);

	  if(InverseMode)
	    PageBegin -= PageLength;
	  else
	    PageBegin += PageLength;

	  if(Selectable)
	    Selected = PageBegin;
	}

    }

  if(!Fade)
    {
      if(DrawBackroundAfterwards)
	BackGround.Blit(DOUBLEBUFFER);

      if(BlitAfterwards)
	graphics::BlitDBToScreen();
    }
  else
    delete Buffer;

  globalwindowhandler::DeInstallControlLoop(FelistDrawController);
  return Return;
}

bool felist::DrawPage(bitmap* Buffer) const
{
  ushort LastFillBottom = Pos.Y + 23 + Description.size() * 10;
  DrawDescription(Buffer, Pos, Width, BackColor);

  ushort c, i; // c == entry index, i == selectable index

  for(c = 0, i = 0; i != PageBegin; ++c)
    if(Entry[c].Selectable)
      ++i;

  for(;;)
    {
      std::string Str;

      if(Selectable && Entry[c].Selectable)
	{
	  Str += 'A' + (i - PageBegin);
	  Str += ": ";
	}

      Str += Entry[c].String;

      if(Entry[c].Bitmap.size())
	{
          Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 20, BackColor);
	  Entry[c].Bitmap[globalwindowhandler::GetTick() % Entry[c].Bitmap.size()]->MaskedBlit(Buffer, 0, 0, Pos.X + 13, LastFillBottom, 16, 16);

	  if(Selectable && Entry[c].Selectable && Selected == i)
	      FONT->PrintfUnshaded(Buffer, Pos.X + 38, LastFillBottom + 5, MAKE_SHADE_COL(Entry[c].Color), "%s", Str.c_str());
	  else
	      FONT->Printf(Buffer, Pos.X + 37, LastFillBottom + 4, Entry[c].Color, "%s", Str.c_str());

	  LastFillBottom += 20;
	}
      else
	{
	  Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, BackColor);

	  if(Selectable && Entry[c].Selectable && Selected == i)
	      FONT->PrintfUnshaded(Buffer, Pos.X + 14, LastFillBottom + 1, MAKE_SHADE_COL(Entry[c].Color), "%s", Str.c_str());
	  else
	      FONT->Printf(Buffer, Pos.X + 13, LastFillBottom, Entry[c].Color, "%s", Str.c_str());

	  LastFillBottom += 10;
	}

      if((i - PageBegin == PageLength - 1 && Entry[c].Selectable) || c == Entry.size() - 1)
	{
	  if((!InverseMode && c != Entry.size() - 1) || (InverseMode && PageBegin))
	    {
	      Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 30, BackColor);
	      FONT->Printf(Buffer, Pos.X + 13, LastFillBottom + 10, WHITE, "- Press SPACE to continue, ESC to exit -");
	      LastFillBottom += 30;
	    }
	  else
	    {
	      Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, BackColor);
	      LastFillBottom += 10;
	    }

	  Buffer->DrawRectangle(Pos.X + 1, Pos.Y + 1, Pos.X + Width - 2, LastFillBottom + 1, DARKGRAY, true);
	  return c == Entry.size() - 1;
	}

      if(Entry[c++].Selectable)
	++i;
    }
}

void felist::DrawDescription(bitmap* Buffer, vector2d Pos, ushort Width, ushort BackColor) const
{
  Buffer->Fill(Pos.X + 3, Pos.Y + 3, Width - 6, 20, BackColor);

  for(ushort c = 0; c < Description.size(); ++c)
    {
      Buffer->Fill(Pos.X + 3, Pos.Y + 13 + c * 10, Width - 6, 10, BackColor);
      FONT->Printf(Buffer, Pos.X + 13, Pos.Y + 13 + c * 10, Description[c].Color, Description[c].String.c_str());
    }

  Buffer->Fill(Pos.X + 3, Pos.Y + 13 + Description.size() * 10, Width - 6, 10, BackColor);
}

void felist::QuickDraw(vector2d Pos, ushort Width, ushort PageLength) const
{
  DOUBLEBUFFER->Fill(Pos.X + 3, Pos.Y + 3, Width - 6, 20 + PageLength * 10, 0);
  DOUBLEBUFFER->DrawRectangle(Pos.X + 1, Pos.Y + 1, Pos.X + Width - 2, Pos.Y + 24 + PageLength * 10, DARKGRAY, true);
  ushort LastBottom = Pos.Y + 13;

  ushort Index = 0;

  if(Selected + PageLength > Length())
    {
      Index = PageLength - Length() + Selected;
      LastBottom += Index * 10;
    }

  for(ushort c = 0; Index < PageLength; ++c, ++Index, LastBottom += 10)
    {
      ushort Color = Entry[c + Selected].Color;

      if(PageLength > 1)
	Color = MAKE_RGB((GET_RED(Color) + GET_RED(Color) * 3 * Index / (PageLength - 1)) >> 2, (GET_GREEN(Color) + GET_GREEN(Color) * 3 * Index / (PageLength - 1)) >> 2, (GET_BLUE(Color) + GET_BLUE(Color) * 3 * Index / (PageLength - 1)) >> 2);

      FONT->Printf(DOUBLEBUFFER, Pos.X + 13, LastBottom, Color, "%s", Entry[c + Selected].String.c_str());
    }
}

void felist::Empty()
{
  for(ushort c = 0; c < Entry.size(); ++c)
    for(ushort i = 0; i < Entry[c].Bitmap.size(); ++i)
      delete Entry[c].Bitmap[i];

  Entry.clear();
}

void felist::AddEntry(const std::string& Str, ushort Color, bitmap* Bitmap, bool Selectable)
{
  std::vector<bitmap*> BitmapVector;

  if(Bitmap)
    BitmapVector.push_back(Bitmap);

  AddEntry(Str, Color, BitmapVector, Selectable);
}

void felist::AddEntry(const std::string& Str, ushort Color, const std::vector<bitmap*>& Bitmap, bool Selectable)
{
  Entry.push_back(felistentry(Bitmap, Str, Color, Selectable));

  if(Maximum && Entry.size() > Maximum)
    Entry.erase(Entry.begin());
}

void felist::Save(outputfile& SaveFile) const
{
  SaveFile << Entry << Description << Maximum << Selected;
}

void felist::Load(inputfile& SaveFile) 
{
  SaveFile >> Entry >> Description >> Maximum >> Selected;
}

void felist::AddDescription(const std::string& Str, ushort Color)
{
  Description.push_back(felistdescription(Str, Color)); 
}

outputfile& operator<<(outputfile& SaveFile, felistentry Entry)
{
  SaveFile << Entry.Bitmap << Entry.String << Entry.Color << Entry.Selectable;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, felistentry& Entry)
{
  SaveFile >> Entry.Bitmap >> Entry.String >> Entry.Color >> Entry.Selectable;
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, felistdescription Desc)
{
  SaveFile << Desc.String << Desc.Color;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, felistdescription& Desc)
{
  SaveFile >> Desc.String >> Desc.Color;
  return SaveFile;
}

