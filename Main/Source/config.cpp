#include "config.h"
#include "save.h"
#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "strover.h"
#include "feio.h"
#include "game.h"
#include "message.h"
#include "area.h"

uchar configuration::Contrast = 100;
bool configuration::BeepOnCritical = false;
std::string configuration::DefaultName;
bool configuration::AutodropLeftOvers = true;
ushort configuration::AutosaveInterval = 100;
bool configuration::OutlineCharacters = false;
bool configuration::OutlineItems = false;

void configuration::Save()
{
	std::ofstream SaveFile("ivan.cfg", std::ios::out);

	if(!SaveFile.is_open())
		return;

	SaveFile << "DefaultName = \"" << DefaultName << "\";\n";
	SaveFile << "AutosaveInterval = " << AutosaveInterval << ";\n";
	SaveFile << "Contrast = " << ulong(Contrast) << ";\n";
	SaveFile << "BeepOnCritical = " << BeepOnCritical << ";\n";
	SaveFile << "AutodropLeftOvers = " << AutodropLeftOvers << ";\n";
	SaveFile << "OutlineCharacters = " << OutlineCharacters << ";\n";
	SaveFile << "OutlineItems = " << OutlineItems << ";";
}

void configuration::Load()
{
	inputfile SaveFile("ivan.cfg", false);

	if(!SaveFile.GetBuffer().is_open())
		return;

	std::map<std::string, long> ValueMap;

	for(std::string Word = SaveFile.ReadWord(false); !SaveFile.GetBuffer().eof(); Word = SaveFile.ReadWord(false))
	{
		if(Word == "DefaultName")
		{
			SaveFile.ReadWord();
			SetDefaultName(SaveFile.ReadWord());
		}

		if(Word == "AutosaveInterval")
			SetAutosaveInterval(SaveFile.ReadNumber(ValueMap));

		if(Word == "Contrast")
			SetContrast(SaveFile.ReadNumber(ValueMap));

		if(Word == "BeepOnCritical")
			SetBeepOnCritical(SaveFile.ReadBool());

		if(Word == "AutodropLeftOvers")
			SetAutodropLeftOvers(SaveFile.ReadBool());

		if(Word == "OutlineCharacters")
			SetOutlineCharacters(SaveFile.ReadBool());

		if(Word == "OutlineItems")
			SetOutlineItems(SaveFile.ReadBool());
	}
}

void configuration::EditContrast(char Change)
{
	short TContrast = Contrast;

	TContrast += Change;

	if(TContrast < 0) TContrast = 0;
	if(TContrast > 200) TContrast = 200;

	Contrast = TContrast;
}

void configuration::ShowConfigScreen()
{
	vector2d QuestionPos = game::GetRunning() ? vector2d(7, 7) : vector2d(30, 46);

	while(true)
	{
		if(!game::GetRunning())
			DOUBLEBUFFER->ClearToColor(0);
		else
		{
			EMPTY_MESSAGES();
			game::DrawEverythingNoBlit();
		}

		felist List("Which setting do you wish to configure?", WHITE, 0);

		List.AddDescription("");
		List.AddDescription("Setting                                    Value");

		List.AddEntry(std::string("Player's default name:                  ") + (DefaultName == "" ? "-" : DefaultName), BLUE);
		List.AddEntry(std::string("Autosave interval:                      ") + AutosaveInterval + " turns", BLUE);
		List.AddEntry(std::string("Contrast:                               ") + Contrast + "/100", BLUE);
		List.AddEntry(std::string("Beep on critical messages:              ") + (BeepOnCritical ? "yes" : "no"), BLUE);
		List.AddEntry(std::string("Drop food leftovers automatically:      ") + (AutodropLeftOvers ? "yes" : "no"), BLUE);
		List.AddEntry(std::string("Outline all characters:                 ") + (OutlineCharacters ? "yes" : "no"), BLUE);
		List.AddEntry(std::string("Outline all items:                      ") + (OutlineItems ? "yes" : "no"), BLUE);

		switch(List.Draw(false))
		{
		case 0:
			SetDefaultName(iosystem::StringQuestion("Set new default name (3-20 letters):", QuestionPos, WHITE, 0, 20));
			continue;
		case 1:
			SetAutosaveInterval(iosystem::NumberQuestion("Set new autosave interval (1-50000 turns, 0 for never):", QuestionPos, WHITE));
			continue;
		case 2:
			SetContrast(iosystem::NumberQuestion("Set new contrast value (0-200):", QuestionPos, WHITE));
			if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
			continue;
		case 3:
			SetBeepOnCritical(!GetBeepOnCritical());
			continue;
		case 4:
			SetAutodropLeftOvers(!GetAutodropLeftOvers());
			continue;
		case 5:
			SetOutlineCharacters(!GetOutlineCharacters());
			if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
			continue;
		case 6:
			SetOutlineItems(!GetOutlineItems());
			if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
			continue;
		}

		break;
	}
}

void configuration::SetDefaultName(std::string What)
{
	DefaultName = What;

	if(DefaultName.length() < 3)
		DefaultName = "";
}

void configuration::SetAutosaveInterval(long What)
{
	if(What < 0) What = 0;
	if(What > 50000) What = 50000;

	AutosaveInterval = What;
}

void configuration::SetContrast(long What)
{
	if(What < 0) What = 0;
	if(What > 200) What = 200;

	Contrast = What;
}
