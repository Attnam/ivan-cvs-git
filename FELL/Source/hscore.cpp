#include "error.h"
#include "graphics.h"
#include "bitmap.h"
#include "strover.h"
#include "hscore.h"
#include "igraph.h"
#include "whandler.h"

void highscore::Add(long NewScore, std::string NewEntry)
{
	for(ushort c = 0; c < Score.Length(); c++)
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

void highscore::Draw(void) const
{
	graphics::ClearDBToColor(0);

	FONTW->PrintfToDB(30, 30, "Adventurers' Hall of Fame");

	ushort Min = 0;

	for(ushort c = 0; c < Score.Length(); c++)
	{
		if(c - Min == 50)
		{
			Min += 50;

			FONTW->PrintfToDB(30, 560, "-- Press ESC to exit, any other key for next page --");

			graphics::BlitDBToScreen();

			if(GETKEY() == 0x1B)
				return;

			graphics::ClearDBToColor(0);

			FONTW->PrintfToDB(30, 30, "Adventurers' Hall of Fame");
		}

		std::string Desc;

		Desc += int(c + 1);

		Desc.resize(5, ' ');

		Desc += int(Score.Access(c));

		Desc.resize(13, ' ');

		Desc += Entry.Access(c);

		FONTB->PrintfToDB(30, 50 + (c - Min) * 10, "%s", Desc.c_str());
	}

	graphics::BlitDBToScreen();

	GETKEY();
}

void highscore::Save(std::string File) const
{
	std::ofstream HighScore(File.c_str(), std::ios::out | std::ios::binary);

	if(!HighScore.is_open())
		ABORT("Can't save highscores! Disk write-protected?");

	ushort EntryAmount = Score.Length();

	HighScore.write((char*)&EntryAmount, sizeof(EntryAmount));

	for(ushort c = 0; c < EntryAmount; c++)
	{
		HighScore.write((char*)&Score.Access(c), sizeof(Score.Access(c)));

		HighScore += Entry.Access(c);
	}

	HighScore.close();
}

void highscore::Load(std::string File)
{
	std::ifstream HighScore(File.c_str(), std::ios::in | std::ios::binary);

	if(!HighScore.is_open())
		return;

	ushort EntryAmount;

	HighScore.read((char*)&EntryAmount, sizeof(EntryAmount));

	for(ushort c = 0; c < EntryAmount; c++)
	{
		long LoadedScore;
		HighScore.read((char*)&LoadedScore, sizeof(LoadedScore));
		Score.Add(LoadedScore);

		std::string LoadedEntry;
		HighScore -= LoadedEntry;
		Entry.Add(LoadedEntry);
	}

	HighScore.close();
}
