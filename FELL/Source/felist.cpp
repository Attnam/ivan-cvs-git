#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "colorbit.h"

ushort felist::Draw(bool BlitBackroundAfterwards) const
{
	if(!Entry.size())
		return 0xFFFF;

	bitmap BackGround(XRES, YRES);

	DOUBLEBUFFER->Blit(&BackGround, 0, 0, 0, 0, XRES, YRES);

	DrawDescription();

	for(ushort Min = 0, c = 0;; ++c)
	{
		DOUBLEBUFFER->ClearToColor(20, 56 + (c - Min + Description.size()) * 10, 758, 20, 128);

		if(DrawLetters)
			FONT->Printf(DOUBLEBUFFER, 30, 56 + (c - Min + Description.size()) * 10, Entry[c].Color, "%c: %s", 'A' + c - Min, Entry[c].String.c_str());
		else
			FONT->Printf(DOUBLEBUFFER, 30, 56 + (c - Min + Description.size()) * 10, Entry[c].Color, "%s", Entry[c].String.c_str());

		if(c - Min == 19 && c != Entry.size() - 1)
		{
			DOUBLEBUFFER->ClearToColor(20, 266 + Description.size() * 10, 758, 20, 128);
			FONT->Printf(DOUBLEBUFFER, 30, 266 + Description.size() * 10, WHITE, "- Press SPACE to continue, ESC to exit -");
		}

		if(c - Min == 19 || c == Entry.size() - 1)
		{
			graphics::BlitDBToScreen();

			int Pressed = GETKEY();

			if(Pressed > 64 && Pressed < 91)
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				if(BlitBackroundAfterwards)
					graphics::BlitDBToScreen();
				return Pressed - 65 + Min < Entry.size() ? Pressed - 65 + Min : 0xFFFF;
			}

			if(Pressed > 96 && Pressed < 123)
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				if(BlitBackroundAfterwards)
					graphics::BlitDBToScreen();
				return Pressed - 97 + Min < Entry.size() ? Pressed - 97 + Min : 0xFFFF;
			}

			if(Pressed == '-')
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				if(BlitBackroundAfterwards)
					graphics::BlitDBToScreen();
				return 0xFFFE;
			}

			if(Pressed == 0x1B || (Pressed == 0x20 && c == Entry.size() - 1))
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				if(BlitBackroundAfterwards)
					graphics::BlitDBToScreen();
				return 0xFFFD;
			}

			if(c == Entry.size() - 1)
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				if(BlitBackroundAfterwards)
					graphics::BlitDBToScreen();
				return 0xFFFF;
			}
			else
			{
				BackGround.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);
				graphics::BlitDBToScreen();
				DrawDescription();
				Min += 20;
			}
		}
	}
}

void felist::DrawDescription() const
{
	DOUBLEBUFFER->ClearToColor(20, 36, 758, 20, 128);

	for(ushort c = 0; c < Description.size(); ++c)
	{
		DOUBLEBUFFER->ClearToColor(20, 46 + c * 10, 758, 10, 128);
		FONT->Printf(DOUBLEBUFFER, 30, 46 + c * 10, Description[c].Color, Description[c].String.c_str());
	}

	DOUBLEBUFFER->ClearToColor(20, 46 + Description.size() * 10, 758, 10, 128);
}

void felist::Empty()
{
	Entry.clear();
}

void felist::AddEntry(std::string Str, ushort Color)
{
	if(InverseMode)
		Entry.insert(Entry.begin(), felistentry(Str, Color));
	else
		Entry.push_back(felistentry(Str, Color));

	if(Maximum && Entry.size() > Maximum)
		if(InverseMode)
			Entry.pop_back();
		else
			Entry.erase(Entry.begin());
}

void felist::Save(outputfile& SaveFile) const
{
	SaveFile << Entry << Description << Maximum << InverseMode << DrawLetters;
}

void felist::Load(inputfile& SaveFile) 
{
	SaveFile >> Entry >> Description >> Maximum >> InverseMode >> DrawLetters;
}
