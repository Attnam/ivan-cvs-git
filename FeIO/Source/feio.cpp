#include <io.h>

#include "graphics.h"
#include "bitmap.h"
#include "feio.h"
#include "whandler.h"
#include "list.h"

void iosystem::TextScreen(bitmap* Font, std::string Text, bool GKey)
{
	char Line[200];
        ushort LastBeginningOfLine = 0;
        ushort c;
        ushort Lines = 0;
	ushort LineNumber = 1;

	DOUBLEBUFFER->ClearToColor(0);

	{
        for(ushort cc = 0; cc < 200; ++cc)
		Line[cc] = 0;
	}

	for(ushort cc = 0; cc < Text.length(); ++cc)
		if(Text[cc] == '\n') ++LineNumber;

	for(c = 0; Text[c] != 0; ++c)
		if(Text[c] == '\n')
		{
			Font->Printf(DOUBLEBUFFER, 400 - strlen(Line) * 4, 275 - (LineNumber - Lines) * 15, "%s", Line);
                        LastBeginningOfLine = c + 1;
                        Lines++;
                        for(ushort cc = 0; cc < 200; ++cc)
                        	Line[cc] = 0;
                }
		else
			Line[c - LastBeginningOfLine] = Text[c];

	Font->Printf(DOUBLEBUFFER, 400 - strlen(Line) * 4, 275 - (LineNumber - Lines) * 15, "%s", Line);

	graphics::BlitDBToScreen();

	if(GKey)
		GETKEY();
}

unsigned int iosystem::CountChars(char cSF,std::string sSH) // (MENU)
{
	unsigned int iReturnCounter = 0;

	for(unsigned int i = 0; i < sSH.length(); ++i)
		if(sSH[i] == cSF)
			++iReturnCounter;

	return iReturnCounter;
}

int iosystem::Menu(bitmap* FontSelected, bitmap* FontNotSelected, std::string sMS) // (MENU)
{
	if(CountChars('\r',sMS) < 1)
		return (-1);

	bool bReady = false;
	unsigned int iSelected = 0;

	while(!bReady)
	{
		std::string sCopyOfMS = sMS;
		DOUBLEBUFFER->ClearToColor(0);

		for(unsigned int i = 0; i < CountChars('\r',sMS); ++i)
		{
			std::string HYVINEPAGURUPRINTF = sCopyOfMS.substr(0,sCopyOfMS.find_first_of('\r'));
			sCopyOfMS.erase(0,sCopyOfMS.find_first_of('\r')+1);
			(i == iSelected ? FontSelected : FontNotSelected)->Printf(DOUBLEBUFFER, 400 - ((HYVINEPAGURUPRINTF.length() + 4) << 2),200+(i*50),"%d. %s",i + 1,HYVINEPAGURUPRINTF.c_str());
		}

		graphics::BlitDBToScreen();
		int k;
		switch(k = GETKEY())
		{
			case 0x148:
				if (iSelected > 0)
					--iSelected;
				else
					iSelected = (CountChars('\r',sMS)-1);
				break;

			case 0x150:
				if (iSelected < (CountChars('\r',sMS)-1))
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

std::string iosystem::StringQuestion(bitmap* Font, std::string Topic, vector2d Pos, ushort MinLetters, ushort MaxLetters)
{
	std::string Input;

	bitmap Backup(XRES, YRES);
	DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, XRES, YRES);

	for(int LastKey = 0;; LastKey = 0)
	{
		Backup.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
		Font->Printf(DOUBLEBUFFER, Pos.X, Pos.Y, "%s", Topic.c_str());
		Font->Printf(DOUBLEBUFFER, Pos.X, Pos.Y + 10, "%s_", Input.c_str());
		graphics::BlitDBToScreen();

		while(!(isalpha(LastKey) || LastKey == ' ' || LastKey == '-' || LastKey == 8 || LastKey == 13))
			LastKey = GETKEY();

		if(LastKey == 8)
		{
			if(Input.length())
				Input.resize(Input.length() - 1);

			continue;
		}

		if(LastKey == 13)
			if(Input.length() >= MinLetters)
				break;
			else
				continue;

		if(Input.length() <= MaxLetters)
			Input += LastKey;
	}

	return Input;
}

std::string iosystem::WhatToLoadMenu(bitmap* TopicFont, bitmap* ListFont) // for some _very_ strange reason "LoadMenu" occasionaly generates an error!
{
	struct _finddata_t Found;
	long hFile;
	int Check = 0;
	list Buffer("Chooseth a file and be sorry");
	hFile = _findfirst("Save/*.sav", &Found);

	if(hFile == -1L)
	{
		DOUBLEBUFFER->ClearToColor(0);
		TopicFont->Printf(DOUBLEBUFFER, 260, 200, "You don't have any previous saves.");
		graphics::BlitDBToScreen();
		GETKEY();
		return "";
	}

	while(!Check)
	{
		Buffer.AddString(Found.name);
		Check = _findnext(hFile, &Found);
	}

	Check = 0xFFFF;

	while(Check > 0xFFFD)
	{
		DOUBLEBUFFER->ClearToColor(0);
		Check = Buffer.Draw(TopicFont, ListFont);
	}

	if(Check == 0xFFFD)
		return "";

	return Buffer.GetString(Check);
}
