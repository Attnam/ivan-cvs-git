#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "colorbit.h"

felist::~felist()
{
  for(ushort c = 0; c < Entry.size(); ++c)
    delete Entry[c].Bitmap;
}

ushort felist::Draw(vector2d Pos, ushort Width, ushort PageLength, bool Selectable, bool BlitAfterwards, bool DrawBackroundAfterwards, bool Fade) const
{
  if(!Entry.size())
    return 0xFFFF;

  bitmap BackGround(XRES, YRES);
  bitmap* Buffer;

  if(Fade)
    {
      Buffer = new bitmap(XRES, YRES, 0);
      BackGround.Fill(0);
    }
  else
    {
      Buffer = DOUBLEBUFFER;
      DOUBLEBUFFER->Blit(&BackGround, 0, 0, 0, 0, XRES, YRES);
    }

  DrawDescription(Buffer, Pos, Width);

  ushort LastFillBottom = Pos.Y + 23 + Description.size() * 10;
  ushort Selected = this->Selected;
  ushort Min = Selected - Selected % PageLength;
  ushort Return, c, i;

  for(c = 0, i = 0; i != Min; ++c)
    if(Entry[c].Selectable)
      ++i;

  bool JustSelectMove = false;

  for(;;)
    {
      std::string Str;

      if(Selectable && Entry[c].Selectable)
	{
	  Str += 'A' + (i - Min);
	  Str += ": ";
	}

      Str += Entry[c].String;

      if(Entry[c].Bitmap)
	{
          Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 20, 128);
	  Entry[c].Bitmap->MaskedBlit(Buffer, 0, 0, Pos.X + 13, LastFillBottom, 16, 16);

	  if(Selectable && Entry[c].Selectable && Selected == i)
	      FONT->PrintfUnshaded(Buffer, Pos.X + 38, LastFillBottom + 5, MAKE_SHADE_COL(Entry[c].Color), "%s", Str.c_str());
	  else
	      FONT->Printf(Buffer, Pos.X + 37, LastFillBottom + 4, Entry[c].Color, "%s", Str.c_str());

	  LastFillBottom += 20;
	}
      else
	{
	  Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, 128);

	  if(Selectable && Entry[c].Selectable && Selected == i)
	      FONT->PrintfUnshaded(Buffer, Pos.X + 14, LastFillBottom + 1, MAKE_SHADE_COL(Entry[c].Color), "%s", Str.c_str());
	  else
	      FONT->Printf(Buffer, Pos.X + 13, LastFillBottom, Entry[c].Color, "%s", Str.c_str());

	  LastFillBottom += 10;
	}

      if(c != Entry.size() - 1 && Entry[c].Selectable && i - Min == PageLength - 1)
	{
	  Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 30, 128);
	  FONT->Printf(Buffer, Pos.X + 13, LastFillBottom + 10, WHITE, "- Press SPACE to continue, ESC to exit -");
	  LastFillBottom += 30;
	}

      if(c == Entry.size() - 1)// && (!Entry[c].Selectable || i - Min != PageLength - 1))
	{
	  Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, 128);
	  LastFillBottom += 10;
	}

      if(c == Entry.size() - 1 || (Entry[c].Selectable && i - Min == PageLength - 1))
	{
	  Buffer->DrawLine(Pos.X + 1, Pos.Y + 1, Pos.X + 1, LastFillBottom + 1, DARKGRAY, true);
	  Buffer->DrawLine(Pos.X + Width - 2, Pos.Y + 1, Pos.X + Width - 2, LastFillBottom + 1, DARKGRAY, true);
	  Buffer->DrawLine(Pos.X + 1, LastFillBottom + 1, Pos.X + Width - 2, LastFillBottom + 1, DARKGRAY, true);

	  if(Fade)
	    {
	      if(JustSelectMove)
		{
		  Buffer->Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
		  graphics::BlitDBToScreen();
		}
	      else
		Buffer->FadeToScreen();

	      JustSelectMove = false;
	    }
	  else
	    graphics::BlitDBToScreen();

	  int Pressed = GETKEY();

	  if(Selectable && Pressed > 64 && Pressed < 91)
	    {
	      Return = Pressed - 65 + Min < long(Entry.size()) ? Pressed - 65 + Min : 0xFFFF;
	      break;
	    }

	  if(Selectable && Pressed > 96 && Pressed < 123)
	    {
	      Return = Pressed - 97 + Min < long(Entry.size()) ? Pressed - 97 + Min : 0xFFFF;
	      break;
	    }

	  if(Selectable && Pressed == 0x148)
	    {
	      if(Selected)
		{
		  --Selected;

		  if(Selected < Min)
		    {
		      BackGround.Blit(Buffer, 0, 0, 0, 0, XRES, YRES);
		      Min -= PageLength;
		    }
		  else
		    JustSelectMove = true;
		}
	      else
		{
		  for(c = 0, Selected = 0xFFFF; c < Entry.size(); ++c)
		    if(Entry[c].Selectable)
		      ++Selected;

		  if(Min == Selected - Selected % PageLength)
		    JustSelectMove = true;
		  else
		    {
		      BackGround.Blit(Buffer, 0, 0, 0, 0, XRES, YRES);
		      Min = Selected - Selected % PageLength;
		    }
		}

	      for(c = 0, i = 0; i != Min; ++c)
		if(Entry[c].Selectable)
		  ++i;

	      DrawDescription(Buffer, Pos, Width);
	      LastFillBottom = Pos.Y + 23 + Description.size() * 10;
	      continue;
	    }

	  if(Selectable && Pressed == 0x150)
	    {
	      if(i != Selected || c != Entry.size() - 1)
		{
		  ++Selected;

		  if(Selected > Min + PageLength - 1)
		    {
		      BackGround.Blit(Buffer, 0, 0, 0, 0, XRES, YRES);
		      Min += PageLength;
		    }
		  else
		    JustSelectMove = true;

		  for(c = 0, i = 0; i != Min; ++c)
		    if(Entry[c].Selectable)
		      ++i;
		}
	      else
		{
		  if(!Min)
		    JustSelectMove = true;
		  else
		    BackGround.Blit(Buffer, 0, 0, 0, 0, XRES, YRES);

		  Selected = Min = i = c = 0;
		}

	      DrawDescription(Buffer, Pos, Width);
	      LastFillBottom = Pos.Y + 23 + Description.size() * 10;
	      continue;
	    }

	  if(Selectable && Pressed == 0x0D)
	    {
	      Return = Selected;
	      break;
	    }

	  if(Pressed == 0x1B || c == Entry.size() - 1)
	    {
	      Return = 0xFFFF;
	      break;
	    }
	  else
	    {
	      BackGround.Blit(Buffer, 0, 0, 0, 0, XRES, YRES);
	      DrawDescription(Buffer, Pos, Width);
	      Min += PageLength;
	      Selected = Min;
	      LastFillBottom = Pos.Y + 23 + Description.size() * 10;
	    }
	}

	if(Entry[c++].Selectable)
	  ++i;
    }

  if(!Fade)
    {
      if(DrawBackroundAfterwards)
	BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

      if(BlitAfterwards)
	graphics::BlitDBToScreen();
    }
  else
    delete Buffer;

  return Return;
}

void felist::DrawDescription(bitmap* Buffer, vector2d Pos, ushort Width) const
{
  Buffer->DrawLine(Pos.X + 1, Pos.Y + 1, Pos.X + Width - 2, Pos.Y + 1, DARKGRAY, true);
  Buffer->Fill(Pos.X + 3, Pos.Y + 3, Width - 6, 20, 128);

  for(ushort c = 0; c < Description.size(); ++c)
    {
      Buffer->Fill(Pos.X + 3, Pos.Y + 13 + c * 10, Width - 6, 10, 128);
      FONT->Printf(Buffer, Pos.X + 13, Pos.Y + 13 + c * 10, Description[c].Color, Description[c].String.c_str());
    }

  Buffer->Fill(Pos.X + 3, Pos.Y + 13 + Description.size() * 10, Width - 6, 10, 128);
}

void felist::Empty()
{
  Entry.clear();
}

void felist::AddEntry(std::string Str, ushort Color, bitmap* Bitmap, bool Selectable)
{
  bitmap* NewBitmap = Bitmap ? new bitmap(Bitmap) : 0;

  if(InverseMode)
    Entry.insert(Entry.begin(), felistentry(NewBitmap, Str, Color, Selectable));
  else
    Entry.push_back(felistentry(NewBitmap, Str, Color, Selectable));

  if(Maximum && Entry.size() > Maximum)
    if(InverseMode)
      Entry.pop_back();
    else
      Entry.erase(Entry.begin());
}

void felist::Save(outputfile& SaveFile) const
{
  SaveFile << Entry << Description << Maximum << InverseMode << Selected;
}

void felist::Load(inputfile& SaveFile) 
{
  SaveFile >> Entry >> Description >> Maximum >> InverseMode >> Selected;
}

void felist::AddDescription(std::string Str, ushort Color)
{
  Description.push_back(felistdescription(Str, Color)); 
}
