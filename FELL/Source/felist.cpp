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

ushort felist::Draw(bool BlitBackroundAfterwards, bool DrawBackroundAfterwards, bool Fade) const
{
  if(!Entry.size())
    return 0xFFFF;

  bitmap Buffer(XRES, YRES);

  if(Fade)
    Buffer.Fill(0);
  else
    DOUBLEBUFFER->Blit(&Buffer, 0, 0, 0, 0, XRES, YRES);

  bitmap* Bitmap = Fade ? &Buffer : DOUBLEBUFFER;

  DrawDescription(Bitmap);

  ushort Return;

  ushort LastFillBottom = 66 + Description.size() * 10;

  ushort Selected = this->Selected, Min, c, i;

  Min = Selected - Selected % 20;

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
          Bitmap->Fill(20, LastFillBottom, 758, 20, 128);
	  Entry[c].Bitmap->MaskedBlit(Bitmap, 0, 0, 30, LastFillBottom, 16, 16);

	  if(Selectable && Entry[c].Selectable && Selected == i)
	      FONT->PrintfUnshaded(Bitmap, 55, LastFillBottom + 5, MAKE_SHADE_COL(Entry[c].Color), "%s", Str.c_str());
	  else
	      FONT->Printf(Bitmap, 54, LastFillBottom + 4, Entry[c].Color, "%s", Str.c_str());

	  LastFillBottom += 20;
	}
      else
	{
	  Bitmap->Fill(20, LastFillBottom, 758, 10, 128);

	  if(Selectable && Entry[c].Selectable && Selected == i)
	      FONT->PrintfUnshaded(Bitmap, 31, LastFillBottom + 1, MAKE_SHADE_COL(Entry[c].Color), "%s", Str.c_str());
	  else
	      FONT->Printf(Bitmap, 30, LastFillBottom, Entry[c].Color, "%s", Str.c_str());

	  LastFillBottom += 10;
	}

      bool p = Entry[c].Selectable;
      std::string q = Entry[c].String;

      if(c != Entry.size() - 1 && Entry[c].Selectable && i - Min == PageLength - 1)
	{
	  Bitmap->Fill(20, LastFillBottom, 758, 30, 128);
	  FONT->Printf(Bitmap, 30, LastFillBottom + 10, WHITE, "- Press SPACE to continue, ESC to exit -");
	  LastFillBottom += 30;
	}

      if(c == Entry.size() - 1 && (!Entry[c].Selectable || i - Min != PageLength - 1))
	{
	  Bitmap->Fill(20, LastFillBottom, 758, 10, 128);
	  LastFillBottom += 10;
	}

      if(c == Entry.size() - 1 || (Entry[c].Selectable && i - Min == PageLength - 1))
	{
	  Bitmap->DrawLine(18, 44, 18, LastFillBottom + 1, DARKGRAY, true);
	  Bitmap->DrawLine(779, 44, 779, LastFillBottom + 1, DARKGRAY, true);
	  Bitmap->DrawLine(18, LastFillBottom + 1, 779, LastFillBottom + 1, DARKGRAY, true);

	  if(Fade)
	    {
	      if(JustSelectMove)
		{
		  Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
		  graphics::BlitDBToScreen();
		}
	      else
		Buffer.FadeToScreen();

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
		      if(!Fade)
			Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

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
		      if(!Fade)
			Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

		      Min = Selected - Selected % PageLength;
		    }
		}

	      for(c = 0, i = 0; i != Min; ++c)
		if(Entry[c].Selectable)
		  ++i;

	      DrawDescription(Bitmap);
	      LastFillBottom = 66 + Description.size() * 10;
	      continue;
	    }

	  if(Selectable && Pressed == 0x150)
	    {
	      if(i != Selected || c != Entry.size() - 1)
		{
		  ++Selected;

		  if(Selected > Min + PageLength - 1)
		    {
		      if(!Fade)
			Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

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
		  else if(!Fade)
		    Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

		  Selected = Min = i = c = 0;
		}

	      DrawDescription(Bitmap);
	      LastFillBottom = 66 + Description.size() * 10;
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
	      if(!Fade)
		Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

	      DrawDescription(Bitmap);
	      Min += PageLength;
	      Selected = Min;
	      LastFillBottom = 66 + Description.size() * 10;
	    }
	}

	if(Entry[c++].Selectable)
	  ++i;
    }

  if(!Fade)
    {
      if(DrawBackroundAfterwards)
	Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

      if(BlitBackroundAfterwards)
	graphics::BlitDBToScreen();
    }

  return Return;
}

void felist::DrawDescription(bitmap* Buffer) const
{
  Buffer->DrawLine(18, 44, 779, 44, DARKGRAY, true);
  Buffer->Fill(20, 46, 758, 20, 128);

  for(ushort c = 0; c < Description.size(); ++c)
    {
      Buffer->Fill(20, 56 + c * 10, 758, 10, 128);
      FONT->Printf(Buffer, 30, 56 + c * 10, Description[c].Color, Description[c].String.c_str());
    }

  Buffer->Fill(20, 56 + Description.size() * 10, 758, 10, 128);
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
  SaveFile << Entry << Description << Maximum << InverseMode << Selectable << Selected;
}

void felist::Load(inputfile& SaveFile) 
{
  SaveFile >> Entry >> Description >> Maximum >> InverseMode >> Selectable >> Selected;
}

void felist::AddDescription(std::string Str, ushort Color)
{
  Description.push_back(felistdescription(Str, Color)); 
}

