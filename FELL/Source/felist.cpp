#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "colorbit.h"

ushort felist::Draw(bool BlitBackroundAfterwards, bool Fade) const
{
	if(!Entry.size())
		return 0xFFFF;

	bitmap Buffer(XRES, YRES);

	if(Fade)
		Buffer.Fill(0);
	else
		DOUBLEBUFFER->Blit(&Buffer, 0, 0, 0, 0, XRES, YRES);

	DrawDescription(Fade ? &Buffer : DOUBLEBUFFER);

	ushort Return;

	for(ushort Min = 0, c = 0;; ++c)
	{
		(Fade ? &Buffer : DOUBLEBUFFER)->Fill(20, 56 + (c - Min + Description.size()) * 10, 758, 20, 128);

		if(DrawLetters)
			FONT->Printf(Fade ? &Buffer : DOUBLEBUFFER, 30, 56 + (c - Min + Description.size()) * 10, Entry[c].Color, "%c: %s", 'A' + c - Min, Entry[c].String.c_str());
		else
			FONT->Printf(Fade ? &Buffer : DOUBLEBUFFER, 30, 56 + (c - Min + Description.size()) * 10, Entry[c].Color, "%s", Entry[c].String.c_str());

		if(c - Min == 19 && c != Entry.size() - 1)
		{
			(Fade ? &Buffer : DOUBLEBUFFER)->Fill(20, 266 + Description.size() * 10, 758, 20, 128);
			FONT->Printf(Fade ? &Buffer : DOUBLEBUFFER, 30, 266 + Description.size() * 10, WHITE, "- Press SPACE to continue, ESC to exit -");
		}

		if(c - Min == 19 || c == Entry.size() - 1)
		{
			if(Fade)
				Buffer.FadeToScreen();
			else
				graphics::BlitDBToScreen();

			int Pressed = GETKEY();

			if(Pressed > 64 && Pressed < 91)
			{
				Return = Pressed - 65 + Min < long(Entry.size()) ? Pressed - 65 + Min : 0xFFFF;
				break;
			}

			if(Pressed > 96 && Pressed < 123)
			{
				Return = Pressed - 97 + Min < long(Entry.size()) ? Pressed - 97 + Min : 0xFFFF;
				break;
			}

			if(Pressed == '-')
			{
				Return = 0xFFFE;
				break;
			}

			if(Pressed == 0x1B || (Pressed == 0x20 && c == Entry.size() - 1))
			{
				Return = 0xFFFD;
				break;
			}

			if(c == Entry.size() - 1)
			{
				Return = 0xFFFF;
				break;
			}
			else
			{
				if(!Fade)
					Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

				DrawDescription(Fade ? &Buffer : DOUBLEBUFFER);
				Min += 20;
			}
		}
	}

	if(!Fade)
	{
		Buffer.Blit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES);

		if(BlitBackroundAfterwards)
			graphics::BlitDBToScreen();
	}

	return Return;
}

void felist::DrawDescription(bitmap* Buffer) const
{
	Buffer->Fill(20, 36, 758, 20, 128);

	for(ushort c = 0; c < Description.size(); ++c)
	{
		Buffer->Fill(20, 46 + c * 10, 758, 10, 128);
		FONT->Printf(Buffer, 30, 46 + c * 10, Description[c].Color, Description[c].String.c_str());
	}

	Buffer->Fill(20, 46 + Description.size() * 10, 758, 10, 128);
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
