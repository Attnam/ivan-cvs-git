#include <io.h>

#include "graphics.h"
#include "bitmap.h"
#include "feio.h"
#include "whandler.h"
#include "felist.h"
#include "colorbit.h"

#define PENT_WIDTH 70

void iosystem::TextScreen(std::string Text, ushort Color, bool GKey)
{
	DOUBLEBUFFER->ClearToColor(0);

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
			FONT->Printf(DOUBLEBUFFER, 400 - strlen(Line) * 4, 225 - (LineNumber - Lines) * 15, Color, Line);
			++Lines;
			LastBeginningOfLine = c + 1;
		}
		else
			Line[c - LastBeginningOfLine] = Text[c];

	Line[c - LastBeginningOfLine] = 0;
	FONT->Printf(DOUBLEBUFFER, 400 - strlen(Line) * 4, 225 - (LineNumber - Lines) * 15, Color, Line);

	graphics::BlitDBToScreen();

	if(GKey)
		GETKEY();
}

ulong iosystem::CountChars(char cSF,std::string sSH) // (MENU)
{
	ulong iReturnCounter = 0;

	for(ulong i = 0; i < sSH.length(); ++i)
		if(sSH[i] == cSF)
			++iReturnCounter;

	return iReturnCounter;
}

int iosystem::Menu(std::string sMS, ushort ColorSelected, ushort ColorNotSelected) // (MENU)
{
	if(CountChars('\r',sMS) < 1)
		return (-1);

	bool bReady = false;
	ulong iSelected = 0;
	double Rotation = 0;
	while(!bReady)
	{
		if(Rotation == 2 * 3.141)
			Rotation = 0;
		else
			Rotation += 0.003;

		DOUBLEBUFFER->ClearToColor(0);

		for(int x = 0; x < 10; ++x)
			DOUBLEBUFFER->DrawPolygon(vector2d(150,150), 100, 5, MAKE_RGB(int(255 - 25 * (10 - x)),0,0), true, Rotation + double(x) / 50);
		
		std::string sCopyOfMS = sMS;

		for(x = 0; x < 4; ++x)
			DOUBLEBUFFER->DrawPolygon(vector2d(150,150), 100 + x, 50, MAKE_RGB(int(255 - 12 * x),0,0));

		for(ulong i = 0; i < CountChars('\r',sMS); ++i)
		{
			std::string HYVINEPAGURUPRINTF = sCopyOfMS.substr(0,sCopyOfMS.find_first_of('\r'));
			sCopyOfMS.erase(0,sCopyOfMS.find_first_of('\r')+1);
			FONT->Printf(DOUBLEBUFFER, 400 - ((HYVINEPAGURUPRINTF.length() + 4) << 2), 200+(i*50), (i == iSelected ? ColorSelected : ColorNotSelected), "%d. %s", i + 1, HYVINEPAGURUPRINTF.c_str());
		}

		graphics::BlitDBToScreen();
		int k;
		
		switch(k = globalwindowhandler::ReadKey())
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
			case 0:
			
			break;
			default:
				if(k > 0x30 && k < int(0x31 + CountChars('\r',sMS)))
					return signed(k - 0x31);
		}
	}

	return signed(iSelected);
}

std::string iosystem::StringQuestion(std::string Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters)
{
	std::string Input;

	bitmap Backup(XRES, YRES);
	DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, XRES, YRES);

	for(int LastKey = 0;; LastKey = 0)
	{
		Backup.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
		FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y, Color, "%s", Topic.c_str());
		FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y + 10, Color, "%s_", Input.c_str());
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
			{
				FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y + 50, Color, "Too short!");
				graphics::BlitDBToScreen();
				GETKEY();
				continue;
			}

		if(Input.length() <= MaxLetters)
			Input += LastKey;
	}


	return Input;
}

long iosystem::NumberQuestion(std::string Topic, vector2d Pos, ushort Color)
{
	std::string Input;

	bitmap Backup(XRES, YRES);
	DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, XRES, YRES);

	for(int LastKey = 0;; LastKey = 0)
	{
		Backup.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
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
			Input += LastKey;
	}

	return atoi(Input.c_str());
}

std::string iosystem::WhatToLoadMenu(ushort TopicColor, ushort ListColor) // for some _very_ strange reason "LoadMenu" occasionaly generates an error!
{
	struct _finddata_t Found;
	long hFile;
	int Check = 0;
	felist Buffer("Chooseth a file and be sorry", TopicColor);
	hFile = _findfirst("Save/*.sav", &Found);

	if(hFile == -1L)
	{
		DOUBLEBUFFER->ClearToColor(0);
		FONT->Printf(DOUBLEBUFFER, 260, 200, TopicColor, "You don't have any previous saves.");
		graphics::BlitDBToScreen();
		GETKEY();
		return "";
	}

	while(!Check)
	{
		Buffer.AddEntry(Found.name, ListColor);
		Check = _findnext(hFile, &Found);
	}

	Check = 0xFFFF;

	while(Check > 0xFFFD)
	{
		DOUBLEBUFFER->ClearToColor(0);
		Check = Buffer.Draw(false);
	}

	if(Check == 0xFFFD)
		return "";

	return Buffer.GetEntry(Check);
}
