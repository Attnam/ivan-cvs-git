#include "config.h"
#include "save.h"

uchar configuration::Contrast = 90;
bool configuration::BeepOnCritical = false;
std::string configuration::DefaultName;
bool configuration::AutodropFoodRemains = true;
ushort configuration::AutosaveInterval = 100;

void configuration::Save()
{
	std::ofstream SaveFile("ivan.cfg", std::ios::out);

	if(!SaveFile.is_open())
		return;

	SaveFile << "Contrast = " << ulong(Contrast) << ";\n";
	SaveFile << "BeepOnCritical = " << BeepOnCritical << ";\n";
	SaveFile << "DefaultName = \"" << DefaultName << "\";\n";
	SaveFile << "AutodropFoodRemains = " << AutodropFoodRemains << ";\n";
	SaveFile << "AutosaveInterval = " << AutosaveInterval << ";\n";
}

void configuration::Load()
{
	inputfile SaveFile("ivan.cfg", false);

	if(!SaveFile.GetBuffer().is_open())
		return;

	std::map<std::string, long> ValueMap;

	for(std::string Word = SaveFile.ReadWord(false); !SaveFile.GetBuffer().eof(); Word = SaveFile.ReadWord(false))
	{
		if(Word == "Contrast")
			Contrast = SaveFile.ReadNumber(ValueMap);

		if(Word == "BeepOnCritical")
			BeepOnCritical = SaveFile.ReadBool();

		if(Word == "DefaultName")
		{
			SaveFile.ReadWord();
			DefaultName = SaveFile.ReadWord();
		}

		if(Word == "AutodropFoodRemains")
			AutodropFoodRemains = SaveFile.ReadBool();

		if(Word == "AutosaveInterval")
			AutosaveInterval = SaveFile.ReadNumber(ValueMap);
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
