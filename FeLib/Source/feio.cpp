#include <ctime>
#include <cctype>

#ifdef WIN32
#include <io.h>
#endif

#ifdef LINUX
#include <dirent.h>
#include <stddef.h>
#include <cstdio>
#include <sys/types.h>
#include <algorithm>
#endif

#ifdef __DJGPP__
#include <dir.h>
#endif

#include "graphics.h"
#include "feio.h"
#include "whandler.h"
#include "felist.h"
#include "colorbit.h"
#include "festring.h"
#include "bitmap.h"

#define PENT_WIDTH 70

void iosystem::TextScreen(const festring& Text, ushort Color, bool GKey, void (*BitmapEditor)(bitmap*))
{
  bitmap Buffer(RES_X, RES_Y, 0);
  ushort c, LineNumber = 0;

  for(c = 0; c < Text.GetSize(); ++c)
    if(Text[c] == '\n')
      ++LineNumber;

  LineNumber >>= 1;
  char Line[200];
  ushort Lines = 0, LastBeginningOfLine = 0;

  for(c = 0; c < Text.GetSize(); ++c)
    if(Text[c] == '\n')
      {
	Line[c - LastBeginningOfLine] = 0;
	FONT->Printf(&Buffer, (RES_X >> 1) - (strlen(Line) << 2), (RES_Y << 1) / 5 - (LineNumber - Lines) * 15, Color, Line);
	++Lines;
	LastBeginningOfLine = c + 1;
      }
    else
      Line[c - LastBeginningOfLine] = Text[c];

  Line[c - LastBeginningOfLine] = 0;
  FONT->Printf(&Buffer, (RES_X >> 1) - (strlen(Line) << 2), (RES_Y << 1) / 5 - (LineNumber - Lines) * 15, Color, Line);
  Buffer.FadeToScreen(BitmapEditor);

  if(GKey)
      GET_KEY();
}

ulong iosystem::CountChars(char cSF, const festring& sSH)
{
  ulong iReturnCounter = 0;

  for(ulong i = 0; i < sSH.GetSize(); ++i)
    if(sSH[i] == cSF)
      ++iReturnCounter;

  return iReturnCounter;
}

int iosystem::Menu(bitmap* BackGround, vector2d Pos, const festring& Topic, const festring& sMS, ushort Color, const festring& SmallText1, const festring& SmallText2)
{
  if(CountChars('\r',sMS) < 1)
    return (-1);

  bool bReady = false;
  ulong iSelected = 0;
  bitmap Backup(DOUBLE_BUFFER);
  bitmap Buffer(RES_X, RES_Y);
  ushort c = 0;

  if(BackGround)
    BackGround->FastBlit(&Buffer);
  else
    Buffer.ClearToColor(0);

  festring sCopyOfMS;
  festring VeryUnGuruPrintf;

  while(!bReady)
    {
      clock_t StartTime = clock();
      sCopyOfMS = Topic;
      ulong i;

      for(i = 0; i < CountChars('\r', Topic); ++i)
	{
	  festring::sizetype RPos = sCopyOfMS.Find('\r');
	  VeryUnGuruPrintf = sCopyOfMS;
	  VeryUnGuruPrintf.Resize(RPos);
	  sCopyOfMS.Erase(0,RPos+1);
	  FONT->Printf(&Buffer, Pos.X - (VeryUnGuruPrintf.GetSize() << 2), Pos.Y - 30 - (CountChars('\r', Topic) + CountChars('\r', sMS)) * 25 + i * 25, RED, "%s", VeryUnGuruPrintf.CStr());
	}

      sCopyOfMS = sMS;

      for(i = 0; i < CountChars('\r', sMS); ++i)
	{
	  festring::sizetype RPos = sCopyOfMS.Find('\r');
	  VeryUnGuruPrintf = sCopyOfMS;
	  VeryUnGuruPrintf.Resize(RPos);
	  sCopyOfMS.Erase(0,RPos+1);
	  ushort XPos = Pos.X - ((VeryUnGuruPrintf.GetSize() + 3) << 2);
	  ushort YPos = Pos.Y - CountChars('\r', sMS) * 25 + i * 50;

	  if(i == iSelected)
	    {
	      Buffer.Fill(XPos, YPos, ((VeryUnGuruPrintf.GetSize() + 3) << 3), 8, 0);
	      FONT->PrintfShade(&Buffer, XPos, YPos, Color, "%d. %s", i + 1, VeryUnGuruPrintf.CStr());
	    }
	  else
	    FONT->Printf(&Buffer, XPos, YPos, Color, "%d. %s", i + 1, VeryUnGuruPrintf.CStr());
	}

      sCopyOfMS = SmallText1;

      for(i = 0; i < CountChars('\r', SmallText1); ++i)
	{
	  festring::sizetype RPos = sCopyOfMS.Find('\r');
	  VeryUnGuruPrintf = sCopyOfMS;
	  VeryUnGuruPrintf.Resize(RPos);
	  sCopyOfMS.Erase(0,RPos+1);
	  FONT->Printf(&Buffer, 3, RES_Y - CountChars('\r', SmallText1) * 10 + i * 10, Color, "%s", VeryUnGuruPrintf.CStr());
	}

      sCopyOfMS = SmallText2;

      for(i = 0; i < CountChars('\r', SmallText2); ++i)
	{
	  festring::sizetype RPos = sCopyOfMS.Find('\r');
	  VeryUnGuruPrintf = sCopyOfMS;
	  VeryUnGuruPrintf.Resize(RPos);
	  sCopyOfMS.Erase(0,RPos+1);
	  FONT->Printf(&Buffer, RES_X - (VeryUnGuruPrintf.GetSize() << 3) - 2, RES_Y - CountChars('\r', SmallText2) * 10 + i * 10, Color, "%s", VeryUnGuruPrintf.CStr());
	}

      int k;

      if(c < 5)
	{
	  ushort Element = 127 - c * 25;
	  Backup.MaskedBlit(DOUBLE_BUFFER, MakeRGB24(Element, Element, Element), 0);
	  Buffer.SimpleAlphaBlit(DOUBLE_BUFFER, c++ * 50, 0);
	  graphics::BlitDBToScreen();
	  while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
	  k = READ_KEY();
	}
      else
	{
	  Buffer.FastBlit(DOUBLE_BUFFER);
	  graphics::BlitDBToScreen();
	  k = GET_KEY(false);
	}
		
      switch(k)
	{	
	case KEY_UP:
	  if(iSelected > 0)
	    --iSelected;
	  else
	    iSelected = (CountChars('\r',sMS)-1);
	  break;

	case KEY_DOWN:
	  if(iSelected < (CountChars('\r',sMS)-1))
	    ++iSelected;
	  else
	    iSelected = 0;
	  break;

	case 0x00D:
	  bReady = true;
	  break;

	default:
	  if(k > 0x30 && k < int(0x31 + CountChars('\r',sMS)))
	    return signed(k - 0x31);
	}
    }

  return signed(iSelected);
}

ushort iosystem::StringQuestion(festring& Input, const festring& Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters, bool Fade, bool AllowExit)
{
  if(Fade)
    {
      bitmap Buffer(RES_X, RES_Y, 0);
      FONT->Printf(&Buffer, Pos.X, Pos.Y, Color, "%s", Topic.CStr());
      FONT->Printf(&Buffer, Pos.X, Pos.Y + 10, Color, "%s_", Input.CStr());
      Buffer.FadeToScreen();
    }

  bool TooShort = false;
  FONT->Printf(DOUBLE_BUFFER, Pos.X, Pos.Y, Color, "%s", Topic.CStr());

  for(int LastKey = 0;; LastKey = 0)
    {
      DOUBLE_BUFFER->Fill(Pos.X, Pos.Y + 10, (MaxLetters << 3) + 9, 9, 0);
      FONT->Printf(DOUBLE_BUFFER, Pos.X, Pos.Y + 10, Color, "%s_", Input.CStr());

      if(TooShort)
	{
	  FONT->Printf(DOUBLE_BUFFER, Pos.X, Pos.Y + 30, Color, "Too short!");
	  TooShort = false;
	}

      graphics::BlitDBToScreen();

      if(TooShort)
	DOUBLE_BUFFER->Fill(Pos.X, Pos.Y + 30, 81, 9, 0);
		
      while(!(LastKey >= 0x20 || LastKey == KEY_BACK_SPACE || LastKey == KEY_ENTER || LastKey == KEY_ESC))
	LastKey = GET_KEY(false);

      if(LastKey == KEY_ESC && AllowExit)
	return ABORTED;
		
      if(LastKey == KEY_BACK_SPACE)
	{
	  if(!Input.IsEmpty())
	    Input.Resize(Input.GetSize() - 1);

	  continue;
	}

      if(LastKey == KEY_ENTER)
	if(Input.GetSize() >= MinLetters)
	  break;
	else
	  {
	    TooShort = true;
	    continue;
	  }

      if(LastKey >= 0x20 && Input.GetSize() < MaxLetters && (LastKey != ' ' || !Input.IsEmpty()))
	Input << char(LastKey);
    }

  festring::sizetype LastAlpha = festring::NPos;

  for(festring::sizetype c = 0; c < Input.GetSize(); ++c)
    if(Input[c] != ' ')
      LastAlpha = c;

  Input.Resize(LastAlpha + 1);
  return NORMAL_EXIT;
}

long iosystem::NumberQuestion(const festring& Topic, vector2d Pos, ushort Color, bool Fade)
{
  if(Fade)
    {
      bitmap Buffer(RES_X, RES_Y, 0);
      FONT->Printf(&Buffer, Pos.X, Pos.Y, Color, "%s", Topic.CStr());
      FONT->Printf(&Buffer, Pos.X, Pos.Y + 10, Color, "_");
      Buffer.FadeToScreen();
    }

  festring Input;
  FONT->Printf(DOUBLE_BUFFER, Pos.X, Pos.Y, Color, "%s", Topic.CStr());

  for(int LastKey = 0;; LastKey = 0)
    {
      DOUBLE_BUFFER->Fill(Pos.X, Pos.Y + 10, 105, 9, 0);
      FONT->Printf(DOUBLE_BUFFER, Pos.X, Pos.Y + 10, Color, "%s_", Input.CStr());
      graphics::BlitDBToScreen();

      while(!isdigit(LastKey) && LastKey != KEY_BACK_SPACE && LastKey != KEY_ENTER && (LastKey != '-' || !Input.IsEmpty()))
	LastKey = GET_KEY(false);

      if(LastKey == KEY_BACK_SPACE)
	{
	  if(!Input.IsEmpty())
	    Input.Resize(Input.GetSize() - 1);

	  continue;
	}

      if(LastKey == KEY_ENTER)
	break;

      if(Input.GetSize() < 12)
	Input << char(LastKey);
    }

  return atoi(Input.CStr());
}

/* Notice that '<' and '>' are active always. */

long iosystem::ScrollBarQuestion(const festring& Topic, vector2d Pos, long StartValue, long Step, long Min, long Max, long AbortValue, ushort TopicColor, ushort Color1, ushort Color2, ushort LeftKey, ushort RightKey, bool Fade, void (*Handler)(long))
{
  long BarValue = StartValue;
  festring Input;
  bool FirstTime = true;

  if(Fade)
    {
      bitmap Buffer(RES_X, RES_Y, 0);
      FONT->Printf(&Buffer, Pos.X, Pos.Y, TopicColor, "%s %d", Topic.CStr(), StartValue);
      FONT->Printf(&Buffer, Pos.X + (Topic.GetSize() << 3) + 8, Pos.Y + 1, TopicColor, "_");
      Buffer.DrawHorizontalLine(Pos.X + 1, Pos.X + 201, Pos.Y + 15, Color2, false);
      Buffer.DrawVerticalLine(Pos.X + 201, Pos.Y + 12, Pos.Y + 18, Color2, false);
      Buffer.DrawHorizontalLine(Pos.X + 1, Pos.X + 1 + (BarValue - Min) * 200 / (Max - Min), Pos.Y + 15, Color1, true);
      Buffer.DrawVerticalLine(Pos.X + 1, Pos.Y + 12, Pos.Y + 18, Color1, true);
      Buffer.DrawVerticalLine(Pos.X + 1 + (BarValue - Min) * 200 / (Max - Min), Pos.Y + 12, Pos.Y + 18, Color1, true);
      Buffer.FadeToScreen();
    }

  for(int LastKey = 0;; LastKey = 0)
    {
      if(!FirstTime)
	BarValue = Input.IsEmpty() ? Min : atoi(Input.CStr());

      if(BarValue < Min)
	BarValue = Min;

      if(BarValue > Max)
	BarValue = Max;

      if(Handler)
	Handler(BarValue);

      DOUBLE_BUFFER->Fill(Pos.X, Pos.Y, ((Topic.GetSize() + 14) << 3) + 1, 10, 0);
      DOUBLE_BUFFER->Fill(Pos.X, Pos.Y + 10, 203, 10, 0);

      if(FirstTime)
	{
	  FONT->Printf(DOUBLE_BUFFER, Pos.X, Pos.Y, TopicColor, "%s %d", Topic.CStr(), StartValue);
	  FONT->Printf(DOUBLE_BUFFER, Pos.X + (Topic.GetSize() << 3) + 8, Pos.Y + 1, TopicColor, "_");
	  FirstTime = false;
	}
      else
	{
	  FONT->Printf(DOUBLE_BUFFER, Pos.X, Pos.Y, TopicColor, "%s %s_", Topic.CStr(), Input.CStr());
	  FONT->Printf(DOUBLE_BUFFER, Pos.X + ((Topic.GetSize() + Input.GetSize()) << 3) + 8, Pos.Y + 1, TopicColor, "_");
	}
      
      DOUBLE_BUFFER->DrawHorizontalLine(Pos.X + 1, Pos.X + 201, Pos.Y + 15, Color2, false);
      DOUBLE_BUFFER->DrawVerticalLine(Pos.X + 201, Pos.Y + 12, Pos.Y + 18, Color2, false);
      DOUBLE_BUFFER->DrawHorizontalLine(Pos.X + 1, Pos.X + 1 + (BarValue - Min) * 200 / (Max - Min), Pos.Y + 15, Color1, true);
      DOUBLE_BUFFER->DrawVerticalLine(Pos.X + 1, Pos.Y + 12, Pos.Y + 18, Color1, true);
      DOUBLE_BUFFER->DrawVerticalLine(Pos.X + 1 + (BarValue - Min) * 200 / (Max - Min), Pos.Y + 12, Pos.Y + 18, Color1, true);
      graphics::BlitDBToScreen();

      while(!isdigit(LastKey) && LastKey != KEY_ESC && LastKey != KEY_BACK_SPACE && LastKey != KEY_ENTER && LastKey != KEY_SPACE && LastKey != '<' && LastKey != '>' && LastKey != RightKey && LastKey != LeftKey)
	LastKey = GET_KEY(false);

      if(LastKey == KEY_ESC)
	{
	  BarValue = AbortValue;
	  break;
	}

      if(LastKey == KEY_BACK_SPACE)
	{
	  if(!Input.IsEmpty())
	    Input.Resize(Input.GetSize() - 1);

	  continue;
	}

      if(LastKey == KEY_ENTER || LastKey == KEY_SPACE)
	break;

      if(LastKey == '<' || LastKey == LeftKey)
	{
	  BarValue -= Step;

	  if(BarValue < Min)
	    BarValue = Min;

	  Input.Empty();
	  Input << BarValue;
	  continue;
	}

      if(LastKey == '>' || LastKey == RightKey)
	{
	  BarValue += Step;

	  if(BarValue > Max)
	    BarValue = Max;

	  Input.Empty();
	  Input << BarValue;
	  continue;
	}

      if(Input.GetSize() < 12)
	Input << char(LastKey);
    }

  return BarValue;
}

festring iosystem::ContinueMenu(ushort TopicColor, ushort ListColor, const festring& DirectoryName)
{
#ifdef WIN32
  struct _finddata_t Found;
  long hFile;
  int Check = 0;
  festring Buffer;
  felist List(CONST_S("Choose a file and be sorry:"), TopicColor);
  hFile = _findfirst(festring(DirectoryName + "*.sav").CStr(), &Found);

  if(hFile == -1L)
    {
      TextScreen(CONST_S("You don't have any previous saves."), TopicColor);
      return "";
    }

  while(!Check)
    {
      Buffer.Empty();
      Buffer << Found.name;
      List.AddEntry(Buffer, ListColor);
      Check = _findnext(hFile, &Found);
    }

  Check = List.Draw();

  if(Check & FELIST_ERROR_BIT)
    return "";

  return List.GetEntry(Check);
#endif

#ifdef LINUX
  DIR* dp;
  struct dirent* ep;
  festring Buffer;
  felist List(CONST_S("Choose a file and be sorry:"), TopicColor);
  dp = opendir(DirectoryName.CStr());

  if(dp)
    {
      while((ep = readdir(dp)))
	{
	  Buffer.Empty();
	  Buffer << ep->d_name;

	  if(Buffer.Find(".sav") != Buffer.NPos)
	    List.AddEntry(Buffer, ListColor);
	}

      if(List.IsEmpty())
	{
	  TextScreen(CONST_S("You don't have any previous saves."), TopicColor);
	  return "";
	}
      else
	{
	  int Check = List.Draw();

	  if(Check & FELIST_ERROR_BIT)
	    return "";

	  return List.GetEntry(Check);
	}

    }

  return "";
#endif

#ifdef __DJGPP__
  struct ffblk Found;
  int Check = 0;
  festring Buffer;
  felist List(CONST_S("Choose a file and be sorry:"), TopicColor);
  Check = findfirst(festring(DirectoryName + "*.sav").CStr(), &Found, FA_HIDDEN | FA_ARCH);

  if(Check)
    {
      TextScreen(CONST_S("You don't have any previous saves."), TopicColor);
      return "";
    }

  while(!Check)
    {
      Buffer.Empty();
      Buffer << Found.ff_name;
      List.AddEntry(Buffer, ListColor);
      Check = findnext(&Found);
    }

  Check = List.Draw();

  if(Check & FELIST_ERROR_BIT)
    return "";

  return List.GetEntry(Check);
#endif
}
