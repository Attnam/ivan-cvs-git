#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "save.h"

ushort felist::Draw(bitmap* TopicFont, bitmap* ListFont, bool WillDrawNumbers) const
{
	if(!String.Length())
		return 0xFFFF;

	bitmap BackGround(XRES, YRES);

	DOUBLEBUFFER->Blit(&BackGround, 0, 0, 0, 0, XRES, YRES);

	DrawDescription(TopicFont);

	for(ushort Min = 0, c = 0;; ++c)
	{
		DOUBLEBUFFER->ClearToColor(20, 56 + (c - Min + Description.Length()) * 10, 758, 20, 128);

		if(WillDrawNumbers)
			ListFont->Printf(DOUBLEBUFFER, 30, 56 + (c - Min + Description.Length()) * 10, "%c: %s", 'A' + c - Min, String.Access(c).c_str());
		else
			ListFont->Printf(DOUBLEBUFFER, 30, 56 + (c - Min + Description.Length()) * 10, "%s", String.Access(c).c_str());

		if(c - Min == 19 && c != String.Length() - 1)
		{
			DOUBLEBUFFER->ClearToColor(20, 266 + Description.Length() * 10, 758, 20, 128);
			TopicFont->Printf(DOUBLEBUFFER, 30, 266 + Description.Length() * 10, "- Press SPACE to continue, ESC to exit -");
		}

		if(c - Min == 19 || c == String.Length() - 1)
		{
			graphics::BlitDBToScreen();

			int Pressed = GETKEY();

			if(Pressed > 64 && Pressed < 91)
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				graphics::BlitDBToScreen();
				return Pressed - 65 + Min < String.Length() ? Pressed - 65 + Min : 0xFFFF;
			}

			if(Pressed > 96 && Pressed < 123)
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				graphics::BlitDBToScreen();
				return Pressed - 97 + Min < String.Length() ? Pressed - 97 + Min : 0xFFFF;
			}

			if(Pressed == '-')
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				graphics::BlitDBToScreen();
				return 0xFFFE;
			}

			if(Pressed == 0x1B || (Pressed == 0x20 && c == String.Length() - 1))
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				graphics::BlitDBToScreen();
				return 0xFFFD;
			}

			if(c == String.Length() - 1)
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				graphics::BlitDBToScreen();
				return 0xFFFF;
			}
			else
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				graphics::BlitDBToScreen();
				DrawDescription(TopicFont);
				Min += 20;
			}
		}
	}
}

void felist::DrawDescription(bitmap* TopicFont) const
{
	DOUBLEBUFFER->ClearToColor(20, 36, 758, 20, 128);

	for(ushort c = 0; c < Description.Length(); ++c)
	{
		DOUBLEBUFFER->ClearToColor(20, 46 + c * 10, 758, 10, 128);
		TopicFont->Printf(DOUBLEBUFFER, 30, 46 + c * 10, Description.Access(c).c_str());
	}

	DOUBLEBUFFER->ClearToColor(20, 46 + Description.Length() * 10, 758, 10, 128);
}

void felist::Empty()
{
	while(String.Length())
		String.Remove(0);
}

void felist::AddString(std::string S, bool WriteToEnd)
{
	if(WriteToEnd)
		String.Add(S);
	else
		String.Put(S, 0);
	if(Maximum && String.Length() > Maximum)
		String.Remove(String.Length() - 1);
}

void felist::Save(outputfile& Temp) const
{
	ushort c;
	Temp << String.Length();

	for(c = 0; c < String.Length(); ++c)
	{
		Temp << String.Access(c);
	}
	
	Temp << Description.Length();

	for(c = 0; c < Description.Length(); ++c)
		Temp << Description.Access(c);
}

void felist::Load(inputfile& Temp) 
{
	ushort TempLength, c;

	Temp >> TempLength;
	

	for(c = 0; c < TempLength; ++c)
	{
		std::string LoadedEntry;
		Temp >> LoadedEntry;
		String.Add(LoadedEntry);
	}
	Temp >> TempLength;
	for(c = 0; c < TempLength; ++c)
	{
		std::string LoadedEntry;
		Temp >> LoadedEntry;
		String.Add(LoadedEntry);
	}
}
