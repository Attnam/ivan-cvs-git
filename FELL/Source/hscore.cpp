#include "error.h"
#include "graphics.h"
#include "bitmap.h"
#include "strover.h"
#include "hscore.h"
#include "whandler.h"
#include "save.h"
#include "colorbit.h"

void highscore::Add(long NewScore, std::string NewEntry)
{
	for(uchar c = 0; c < Score.size(); ++c)
		if(Score[c] < NewScore)
		{
			Entry.insert(Entry.begin() + c, NewEntry);
			Score.insert(Score.begin() + c, NewScore);

			if(Score.size() > 100)
			{
				Entry.resize(100, std::string());
				Score.resize(100);
			}

			LastAdd = c;
			return;
		}

	if(Score.size() < 100)
	{
		LastAdd = Score.size();
		Entry.push_back(NewEntry);
		Score.push_back(NewScore);
	}
	else
		LastAdd = 100;
}

void highscore::Draw() const
{
	DOUBLEBUFFER->ClearToColor(0);

	FONT->Printf(DOUBLEBUFFER, 30, 30,  WHITE, "Adventurers' Hall of Fame");

	ushort Min = 0;

	for(uchar c = 0; c < Score.size(); ++c)
	{
		if(c - Min == 50)
		{
			Min += 50;

			FONT->Printf(DOUBLEBUFFER, 30, 560, WHITE, "-- Press ESC to exit, any other key for next page --");

			graphics::BlitDBToScreen();

			if(GETKEY() == 0x1B)
				return;

			DOUBLEBUFFER->ClearToColor(0);

			FONT->Printf(DOUBLEBUFFER, 30, 30, WHITE, "Adventurers' Hall of Fame");
		}

		std::string Desc;

		Desc += int(c + 1);

		Desc.resize(5, ' ');

		Desc += int(Score[c]);

		Desc.resize(13, ' ');

		Desc += Entry[c];

		FONT->Printf(DOUBLEBUFFER, 30, 50 + (c - Min) * 10, c == LastAdd ? RED : BLUE, "%s", Desc.c_str());
	}

	graphics::BlitDBToScreen();

	GETKEY();
}

void highscore::Save(std::string File) const
{
	outputfile HighScore(File);

	HighScore << Score << Entry << LastAdd;
}

void highscore::Load(std::string File)
{
	inputfile HighScore(File, false);

	if(!HighScore.GetBuffer().is_open())
		return;

	HighScore >> Score >> Entry >> LastAdd;
}
