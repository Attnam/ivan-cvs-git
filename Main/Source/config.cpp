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

#ifdef WIN32
#define CONFIG_FILENAME "ivan.cfg"
#else
#define CONFIG_FILENAME (std::string(getenv("HOME")) + std::string("/.ivan.conf")).c_str()
#endif

std::string configuration::DefaultName;
ushort configuration::AutosaveInterval = 500;
uchar configuration::Contrast = 100;
bool configuration::FullScreenMode = false;
bool configuration::BeepOnCritical = false;
bool configuration::AutodropLeftOvers = true;
bool configuration::OutlineCharacters = false;
bool configuration::OutlineItems = false;
ushort configuration::CharacterOutlineColor = BLUE;
ushort configuration::ItemOutlineColor = GREEN;

void configuration::Save()
{
	std::ofstream SaveFile(CONFIG_FILENAME, std::ios::out);

	if(!SaveFile.is_open())
		return;

	SaveFile << "DefaultName = \"" << DefaultName << "\";\n";
	SaveFile << "AutosaveInterval = " << AutosaveInterval << ";\n";
	SaveFile << "Contrast = " << ulong(Contrast) << ";\n";
	SaveFile << "FullScreenMode = " << FullScreenMode << ";\n";
	SaveFile << "BeepOnCritical = " << BeepOnCritical << ";\n";
	SaveFile << "AutodropLeftOvers = " << AutodropLeftOvers << ";\n";
	SaveFile << "OutlineCharacters = " << OutlineCharacters << ";\n";
	SaveFile << "OutlineItems = " << OutlineItems << ";\n";
	SaveFile << "CharacterOutlineColor = " << GET_RED(CharacterOutlineColor) << ", " << GET_GREEN(CharacterOutlineColor) << ", " << GET_BLUE(CharacterOutlineColor) << ";\n";
	SaveFile << "ItemOutlineColor = " << GET_RED(ItemOutlineColor) << ", " << GET_GREEN(ItemOutlineColor) << ", " << GET_BLUE(ItemOutlineColor) << ";";
}

void configuration::Load()
{
	inputfile SaveFile(CONFIG_FILENAME, false);

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

		if(Word == "FullScreenMode")
			SetFullScreenMode(SaveFile.ReadBool());

		if(Word == "BeepOnCritical")
			SetBeepOnCritical(SaveFile.ReadBool());

		if(Word == "AutodropLeftOvers")
			SetAutodropLeftOvers(SaveFile.ReadBool());

		if(Word == "OutlineCharacters")
			SetOutlineCharacters(SaveFile.ReadBool());

		if(Word == "OutlineItems")
			SetOutlineItems(SaveFile.ReadBool());

		if(Word == "CharacterOutlineColor")
			SetCharacterOutlineColor(MAKE_RGB(SaveFile.ReadNumber(ValueMap), SaveFile.ReadNumber(ValueMap), SaveFile.ReadNumber(ValueMap)));

		if(Word == "ItemOutlineColor")
			SetItemOutlineColor(MAKE_RGB(SaveFile.ReadNumber(ValueMap), SaveFile.ReadNumber(ValueMap), SaveFile.ReadNumber(ValueMap)));
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
	bool BoolChange = false;

	while(true)
	{
		if(game::GetRunning())
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
		List.AddEntry(std::string("Drop food leftovers automatically:      ") + (AutodropLeftOvers ? "yes" : "no"), BLUE);
		List.AddEntry(std::string("Outline all characters:                 ") + (OutlineCharacters ? "yes" : "no"), BLUE);
		List.AddEntry(std::string("Outline all items:                      ") + (OutlineItems ? "yes" : "no"), BLUE);

#ifdef WIN32
		List.AddEntry(std::string("Run the game in full screen mode:       ") + (FullScreenMode ? "yes" : "no"), BLUE);
		List.AddEntry(std::string("Beep on critical messages:              ") + (BeepOnCritical ? "yes" : "no"), BLUE);
#endif

		switch(List.Draw(false, !game::GetRunning() && !BoolChange))
		{
		case 0:
			SetDefaultName(iosystem::StringQuestion("Set new default name (3-20 letters):", QuestionPos, WHITE, 0, 20, !game::GetRunning()));
			BoolChange = false;
			continue;
		case 1:
			SetAutosaveInterval(iosystem::NumberQuestion("Set new autosave interval (1-50000 turns, 0 for never):", QuestionPos, WHITE, !game::GetRunning()));
			BoolChange = false;
			continue;
		case 2:
			SetContrast(iosystem::NumberQuestion("Set new contrast value (0-200):", QuestionPos, WHITE, !game::GetRunning()));
			if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
			BoolChange = false;
			continue;

		case 3:
			SetAutodropLeftOvers(!GetAutodropLeftOvers());
			BoolChange = true;
			continue;
		case 4:
			SetOutlineCharacters(!GetOutlineCharacters());
			if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
			BoolChange = true;
			continue;
		case 5:
			SetOutlineItems(!GetOutlineItems());
			if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
			BoolChange = true;
			continue;
#ifdef WIN32
		case 6:
		        graphics::SwitchMode();
			BoolChange = true;
			continue;
		case 7:
			SetBeepOnCritical(!GetBeepOnCritical());
			BoolChange = true;
			continue;
#endif
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

void configuration::SwitchModeHandler()
{
	SetFullScreenMode(!GetFullScreenMode());
}
