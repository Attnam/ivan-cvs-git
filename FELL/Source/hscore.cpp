#include "error.h"
#include "graphics.h"
#include "bitmap.h"
#include "strover.h"
#include "hscore.h"
#include "whandler.h"
#include "save.h"

void highscore::Add(long NewScore, std::string NewEntry)
{
	for(ushort c = 0; c < Score.Length(); ++c)
		if(Score.Access(c) < NewScore)
		{
			Entry.Put(NewEntry, c);
			Score.Put(NewScore, c);
			Entry.Resize(100);
			Score.Resize(100);
			return;
		}

	if(Score.Length() < 100)
	{
		Entry.Add(NewEntry);
		Score.Add(NewScore);
	}
}

void highscore::Draw(bitmap* TopicFont, bitmap* ListFont) const
{
	DOUBLEBUFFER->ClearToColor(0);

	TopicFont->Printf(DOUBLEBUFFER, 30, 30, "Adventurers' Hall of Fame");

	ushort Min = 0;

	for(ushort c = 0; c < Score.Length(); ++c)
	{
		if(c - Min == 50)
		{
			Min += 50;

			TopicFont->Printf(DOUBLEBUFFER, 30, 560, "-- Press ESC to exit, any other key for next page --");

			graphics::BlitDBToScreen();

			if(GETKEY() == 0x1B)
				return;

			DOUBLEBUFFER->ClearToColor(0);

			TopicFont->Printf(DOUBLEBUFFER, 30, 30, "Adventurers' Hall of Fame");
		}

		std::string Desc;

		Desc += int(c + 1);

		Desc.resize(5, ' ');

		Desc += int(Score.Access(c));

		Desc.resize(13, ' ');

		Desc += Entry.Access(c);

		ListFont->Printf(DOUBLEBUFFER, 30, 50 + (c - Min) * 10, "%s", Desc.c_str());
	}

	graphics::BlitDBToScreen();

	GETKEY();
}

void highscore::Save(std::string File) const
{
	outputfile HighScore(File);

	HighScore << Score.Length();

	for(ushort c = 0; c < Score.Length(); ++c)
	{
		HighScore << Score.Access(c);
		HighScore << Entry.Access(c);
	}
}

void highscore::Load(std::string File)
{
	inputfile HighScore(File, false);

	if(!HighScore.GetBuffer().is_open())
		return;

	ushort EntryAmount;

	HighScore >> EntryAmount;

	for(ushort c = 0; c < EntryAmount; ++c)
	{
		long LoadedScore;
		HighScore >> LoadedScore;
		Score.Add(LoadedScore);

		std::string LoadedEntry;
		HighScore >> LoadedEntry;
		Entry.Add(LoadedEntry);
	}
}
