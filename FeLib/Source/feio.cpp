#include <ctime>
#include <ctype.h>

#ifdef WIN32
#include <io.h>
#endif

#ifdef USE_SDL
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#endif

#ifdef __DJGPP__
#include <dir.h>
#endif

#include "graphics.h"
#include "bitmap.h"
#include "feio.h"
#include "whandler.h"
#include "felist.h"
#include "colorbit.h"
#include "femath.h"
#include "stdover.h"

#define PENT_WIDTH 70

void iosystem::TextScreen(const std::string& Text, ushort Color, bool GKey, void (*BitmapEditor)(bitmap*))
{
  bitmap Buffer(RES);
  Buffer.Fill(0);
  ushort c, LineNumber = 0;

  for(c = 0; c < Text.length(); ++c)
    if(Text[c] == '\n')
      ++LineNumber;

  LineNumber >>= 1;
  char Line[200];
  ushort Lines = 0, LastBeginningOfLine = 0;

  for(c = 0; c < Text.length(); ++c)
    if(Text[c] == '\n')
      {
	Line[c - LastBeginningOfLine] = 0;
	FONT->Printf(&Buffer, RES.X / 2 - strlen(Line) * 4, RES.Y * 3 / 8 - (LineNumber - Lines) * 15, Color, Line);
	++Lines;
	LastBeginningOfLine = c + 1;
      }
    else
      Line[c - LastBeginningOfLine] = Text[c];

  Line[c - LastBeginningOfLine] = 0;
  FONT->Printf(&Buffer, RES.X / 2 - strlen(Line) * 4, RES.Y * 3 / 8 - (LineNumber - Lines) * 15, Color, Line);
  Buffer.FadeToScreen(BitmapEditor);

  if(GKey)
      GETKEY(false);
}

ulong iosystem::CountChars(char cSF, const std::string& sSH)
{
  ulong iReturnCounter = 0;

  for(ulong i = 0; i < sSH.length(); ++i)
    if(sSH[i] == cSF)
      ++iReturnCounter;

  return iReturnCounter;
}

int iosystem::Menu(bitmap* BackGround, vector2d Pos, const std::string& Topic, const std::string& sMS, ushort ColorSelected, ushort ColorNotSelected, const std::string& SmallText)
{
  if(CountChars('\r',sMS) < 1)
    return (-1);

  bool bReady = false;
  ulong iSelected = 0;
  bitmap Backup(RES);
  DOUBLEBUFFER->Blit(&Backup);
  bitmap Buffer(RES);
  ushort c = 0;

  if(BackGround)
    BackGround->Blit(&Buffer);
  else
    Buffer.Fill(0);

  while(!bReady)
    {
      clock_t StartTime = clock();
      std::string sCopyOfMS = Topic;
      ulong i;

      for(i = 0; i < CountChars('\r', Topic); ++i)
	{
	  std::string HYVINEPAGURUPRINTF = sCopyOfMS.substr(0,sCopyOfMS.find_first_of('\r'));
	  sCopyOfMS.erase(0,sCopyOfMS.find_first_of('\r')+1);
	  FONT->Printf(&Buffer, Pos.X - (HYVINEPAGURUPRINTF.length() << 2), Pos.Y - 30 - (CountChars('\r', Topic) + CountChars('\r', sMS)) * 25 + i * 25, MAKE_RGB(200, 0, 0), "%s", HYVINEPAGURUPRINTF.c_str());
	}

      sCopyOfMS = sMS;

      for(i = 0; i < CountChars('\r', sMS); ++i)
	{
	  std::string HYVINEPAGURUPRINTF = sCopyOfMS.substr(0,sCopyOfMS.find_first_of('\r'));
	  sCopyOfMS.erase(0,sCopyOfMS.find_first_of('\r')+1);

	  if(i == iSelected)
	    {
	      FONT->PrintfUnshaded(&Buffer, Pos.X - ((HYVINEPAGURUPRINTF.length() + 3) << 2), Pos.Y - CountChars('\r', sMS) * 25 + i * 50, BLACK, "%d. %s", i + 1, HYVINEPAGURUPRINTF.c_str());
	      FONT->PrintfUnshaded(&Buffer, Pos.X + 1 - ((HYVINEPAGURUPRINTF.length() + 3) << 2), Pos.Y + 1 - CountChars('\r', sMS) * 25 + i * 50, ColorSelected, "%d. %s", i + 1, HYVINEPAGURUPRINTF.c_str());
	    }
	  else
	    FONT->Printf(&Buffer, Pos.X - ((HYVINEPAGURUPRINTF.length() + 3) << 2), Pos.Y - CountChars('\r', sMS) * 25 + i * 50, ColorNotSelected, "%d. %s", i + 1, HYVINEPAGURUPRINTF.c_str());
	}

      sCopyOfMS = SmallText;

      for(i = 0; i < CountChars('\r', SmallText); ++i)
	{
	  std::string HYVINEPAGURUPRINTF = sCopyOfMS.substr(0,sCopyOfMS.find_first_of('\r'));
	  sCopyOfMS.erase(0,sCopyOfMS.find_first_of('\r')+1);
	  FONT->Printf(&Buffer, 2, RES.Y - CountChars('\r', SmallText) * 10 + i * 10, ColorNotSelected, "%s", HYVINEPAGURUPRINTF.c_str());
	}

      int k;

      if(c < 5)
	{
	  Backup.MaskedBlit(DOUBLEBUFFER, ushort(255 - c * 50), 0);
	  Buffer.SimpleAlphaBlit(DOUBLEBUFFER, c++ * 50, 0);
	  graphics::BlitDBToScreen();
	  while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
	  k = READKEY();
	}
      else
	{
	  Buffer.Blit(DOUBLEBUFFER);
	  graphics::BlitDBToScreen();
	  k = GETKEY();
	}
		
      switch(k)
	{	
	case 0x148:
	  if(iSelected > 0)
	    --iSelected;
	  else
	    iSelected = (CountChars('\r',sMS)-1);
	  break;

	case 0x150:
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

std::string iosystem::StringQuestion(const std::string& Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters, bool Fade, bool AllowExit)
{
  if(Fade)
    {
      bitmap Buffer(RES);
      Buffer.Fill(0);
      FONT->Printf(&Buffer, Pos.X, Pos.Y, Color, "%s", Topic.c_str());
      FONT->Printf(&Buffer, Pos.X, Pos.Y + 10, Color, "_");
      Buffer.FadeToScreen();
    }

  std::string Input;
  bitmap Backup(RES);
  DOUBLEBUFFER->Blit(&Backup);
  Backup.Fill(Pos.X, Pos.Y + 10, 9, 9, 0);
  bool TooShort = false;

  for(int LastKey = 0;; LastKey = 0)
    {
      Backup.Blit(DOUBLEBUFFER);
      FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y, Color, "%s", Topic.c_str());
      FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y + 10, Color, "%s_", Input.c_str());

      if(TooShort)
	{
	  FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y + 50, Color, "Too short!");
	  TooShort = false;
	}

      graphics::BlitDBToScreen();
		
      while(!(LastKey >= 0x20 || LastKey == 0x08 || LastKey == 0x0D || LastKey == 0x1B))
	LastKey = GETKEY();

      if(LastKey == 0x1B && AllowExit)
	return "";
		
      if(LastKey == 0x08)
	{
	  if(Input.length())
	    Input.resize(Input.length() - 1);

	  continue;
	}

      if(LastKey == 0x0D)
	if(Input.length() >= MinLetters)
	  break;
	else
	  {
	    TooShort = true;
	    continue;
	  }

      if(LastKey >= 0x20 && Input.length() < MaxLetters)
	Input += char(LastKey);
    }

  return Input;
}

long iosystem::NumberQuestion(const std::string& Topic, vector2d Pos, ushort Color, bool Fade)
{
  if(Fade)
    {
      bitmap Buffer(RES);
      Buffer.Fill(0);
      FONT->Printf(&Buffer, Pos.X, Pos.Y, Color, "%s", Topic.c_str());
      FONT->Printf(&Buffer, Pos.X, Pos.Y + 10, Color, "_");
      Buffer.FadeToScreen();
    }

  std::string Input;
  bitmap Backup(RES);
  DOUBLEBUFFER->Blit(&Backup);
  Backup.Fill(Pos.X, Pos.Y + 10, 9, 9, 0);

  for(int LastKey = 0;; LastKey = 0)
    {
      Backup.Blit(DOUBLEBUFFER);
      FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y, Color, "%s", Topic.c_str());
      FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y + 10, Color, "%s_", Input.c_str());
      graphics::BlitDBToScreen();

      while(!(isdigit(LastKey) || LastKey == 8 || LastKey == 13))
	{
	  if(LastKey == '-' && !Input.length())
	    break;

	  LastKey = GETKEY();
	}

      if(LastKey == 8)
	{
	  if(Input.length())
	    Input.resize(Input.length() - 1);

	  continue;
	}

      if(LastKey == 13)
	break;

      if(Input.length() < 12)
	Input += char(LastKey);
    }

  return atoi(Input.c_str());
}

std::string iosystem::WhatToLoadMenu(ushort TopicColor, ushort ListColor, const std::string& DirectoryName) // for some _very_ strange reason "LoadMenu" occasionaly generates an error!
{
#ifdef WIN32
  struct _finddata_t Found;
  long hFile;
  int Check = 0;
  felist Buffer("Choose a file and be sorry:", TopicColor);
  hFile = _findfirst((DirectoryName + "*.sav").c_str(), &Found);

  if(hFile == -1L)
    {
      TextScreen("You don't have any previous saves.", TopicColor);
      return "";
    }

  while(!Check)
    {
      Buffer.AddEntry(Found.name, ListColor);
      Check = _findnext(hFile, &Found);
    }

  Check = Buffer.Draw(vector2d(10, 10), 780, 30, BLACK, true, false, false, true);

  if(Check == 0xFFFF)
    return "";

  return Buffer.GetEntry(Check);
#endif

#ifdef USE_SDL
  DIR* dp;
  struct dirent* ep;
  std::string Buffer;
  felist List("Choose a file and be sorry:", TopicColor);
  dp = opendir(DirectoryName.c_str());
  if(dp)
    {
      while(ep = readdir(dp))
	{
	  Buffer = std::string(ep->d_name);
	  if(Buffer.find(".sav") != Buffer.npos)
	    {
	      List.AddEntry(Buffer, ListColor);
	    }
	}
      if(List.IsEmpty())
	{
	  TextScreen("You don't have any previous saves.", TopicColor);
	  return "";
	}
      else
	{
	  int Check = List.Draw(vector2d(10, 10), 780, 30, BLACK, true, false, false, true);

	  if(Check == 0xFFFF)
	    return "";

	  return List.GetEntry(Check);
	}

    }
#endif

#ifdef __DJGPP__
  struct ffblk Found;
  int Check = 0;
  felist Buffer("Choose a file and be sorry:", TopicColor);
  Check = findfirst((DirectoryName + "*.sav").c_str(), &Found, FA_HIDDEN | FA_ARCH);

  if(Check)
    {
      TextScreen("You don't have any previous saves.", TopicColor);
      return "";
    }

  while(!Check)
    {
      Buffer.AddEntry(Found.ff_name, ListColor);
      Check = findnext(&Found);
    }

  Check = Buffer.Draw(vector2d(10, 10), 780, 30, BLACK, true, false, false, true);

  if(Check == 0xFFFF)
    return "";

  return Buffer.GetEntry(Check);
#endif
}

