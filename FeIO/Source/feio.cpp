#include <ctime>
#include <io.h>

#include "graphics.h"
#include "bitmap.h"
#include "feio.h"
#include "whandler.h"
#include "felist.h"
#include "colorbit.h"
#include "femath.h"

#define PENT_WIDTH 70

void iosystem::TextScreen(std::string Text, ushort Color, bool GKey)
{
	bitmap Buffer(XRES, YRES);
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
			FONT->Printf(&Buffer, 400 - strlen(Line) * 4, 225 - (LineNumber - Lines) * 15, Color, Line);
			++Lines;
			LastBeginningOfLine = c + 1;
		}
		else
			Line[c - LastBeginningOfLine] = Text[c];

	Line[c - LastBeginningOfLine] = 0;
	FONT->Printf(&Buffer, 400 - strlen(Line) * 4, 225 - (LineNumber - Lines) * 15, Color, Line);

	Buffer.FadeToScreen();

	if(GKey)
		GETKEY(false);
}

ulong iosystem::CountChars(char cSF,std::string sSH) // (MENU)
{
	ulong iReturnCounter = 0;

	for(ulong i = 0; i < sSH.length(); ++i)
		if(sSH[i] == cSF)
			++iReturnCounter;

	return iReturnCounter;
}

int iosystem::Menu(bitmap* PentaPicture, std::string sMS, ushort ColorSelected, ushort ColorNotSelected) // (MENU)
{
	if(CountChars('\r',sMS) < 1)
		return (-1);

	bool bReady = false;
	ulong iSelected = 0;
	double Rotation = 0;

	bitmap Backup(XRES, YRES);
	DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, XRES, YRES);
	bitmap Buffer(XRES, YRES);

	uchar c = 0;

	while(!bReady)
	{
		clock_t StartTime = clock();

		Rotation += 0.01;

		if(Rotation > 2 * PI)
			Rotation -= 2 * PI;

		Buffer.Fill(0);

		for(int x = 0; x < 10; ++x)
			Buffer.DrawPolygon(vector2d(150,150), 100, 5, MAKE_RGB(int(255 - 25 * (10 - x)),0,0), true, Rotation + double(x) / 50);
		
		std::string sCopyOfMS = sMS;

		for(x = 0; x < 4; ++x)
			Buffer.DrawPolygon(vector2d(150,150), 100 + x, 50, MAKE_RGB(int(255 - 12 * x),0,0));

		PentaPicture->MaskedBlit(&Buffer, 0, 0, 143, 141, 16, 16);

		for(ulong i = 0; i < CountChars('\r',sMS); ++i)
		{
			std::string HYVINEPAGURUPRINTF = sCopyOfMS.substr(0,sCopyOfMS.find_first_of('\r'));
			sCopyOfMS.erase(0,sCopyOfMS.find_first_of('\r')+1);
			FONT->Printf(&Buffer, 400 - ((HYVINEPAGURUPRINTF.length() + 4) << 2), 175+(i*50), (i == iSelected ? ColorSelected : ColorNotSelected), "%d. %s", i + 1, HYVINEPAGURUPRINTF.c_str());
		}

		if(c < 5)
		{
			Backup.MaskedBlit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES, ushort(255 - c * 50), 0);
			Buffer.AlphaBlit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES, ushort(c * 50), 0);
			++c;
		}
		else
			Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

		graphics::BlitDBToScreen();

		while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);

		int k;
		
		switch(k = READKEY())
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

std::string iosystem::StringQuestion(std::string Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters, bool Fade)
{
	if(Fade)
	{
		bitmap Buffer(XRES, YRES);
		Buffer.Fill(0);
		FONT->Printf(&Buffer, Pos.X, Pos.Y, Color, "%s", Topic.c_str());
		FONT->Printf(&Buffer, Pos.X, Pos.Y + 10, Color, "_");
		Buffer.FadeToScreen();
	}

	std::string Input;

	bitmap Backup(XRES, YRES);
	DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, XRES, YRES);
	Backup.Fill(Pos.X, Pos.Y + 10, 8, 8, 0);

	bool TooShort = false;

	for(int LastKey = 0;; LastKey = 0)
	{
		Backup.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
		FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y, Color, "%s", Topic.c_str());
		FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y + 10, Color, "%s_", Input.c_str());

		if(TooShort)
		{
			FONT->Printf(DOUBLEBUFFER, Pos.X, Pos.Y + 50, Color, "Too short!");
			TooShort = false;
		}

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
				TooShort = true;
				continue;
			}

		if(Input.length() < MaxLetters)
			Input += LastKey;
	}


	return Input;
}

long iosystem::NumberQuestion(std::string Topic, vector2d Pos, ushort Color, bool Fade)
{
	if(Fade)
	{
		bitmap Buffer(XRES, YRES);
		Buffer.Fill(0);
		FONT->Printf(&Buffer, Pos.X, Pos.Y, Color, "%s", Topic.c_str());
		FONT->Printf(&Buffer, Pos.X, Pos.Y + 10, Color, "_");
		Buffer.FadeToScreen();
	}

	std::string Input;

	bitmap Backup(XRES, YRES);
	DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, XRES, YRES);
	Backup.Fill(Pos.X, Pos.Y + 10, 8, 8, 0);

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
	felist Buffer("Choose a file and be sorry:", TopicColor);
	hFile = _findfirst("Save/*.sav", &Found);

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

	Check = Buffer.Draw(false, true);

	while(Check > 0xFFFD)
		Check = Buffer.Draw(false);

	if(Check == 0xFFFD)
		return "";

	return Buffer.GetEntry(Check);
}
