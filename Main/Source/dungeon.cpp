#include "dungeon.h"
#include "allocate.h"
#include "level.h"
#include "lsquare.h"
#include "lterrade.h"
#include "charde.h"
#include "stack.h"
#include "strover.h"
#include "error.h"
#include "save.h"
#include "materde.h"
#include "script.h"

dungeon::dungeon(uchar Index) : Index(Index)
{
	Initialize();

	for(uchar c = 0; c < GetLevels(); ++c)
		Generated[c] = false;
}

dungeon::~dungeon()
{
	for(ushort c = 0; c < GetLevels(); ++c)
		delete Level[c];

	delete [] Level;
}

void dungeon::Initialize()
{
	std::map<uchar, dungeonscript*>::iterator DungeonIterator = game::GetGameScript().GetDungeon().find(Index);

	if(DungeonIterator != game::GetGameScript().GetDungeon().end())
		DungeonScript = DungeonIterator->second;
	else
		DungeonScript = game::GetGameScript().GetDungeonDefault();

	Level = new level*[GetLevels()];
	Generated = new bool[GetLevels()];

	for(uchar c = 0; c < GetLevels(); ++c)
		Level[c] = 0;
}

levelscript* dungeon::GetLevelScript(ushort Index)
{
	std::map<uchar, levelscript*>::iterator LevelIterator = DungeonScript->GetLevel().find(Index);

	levelscript* LevelScript;

	if(LevelIterator != DungeonScript->GetLevel().end())
	{
		LevelScript = LevelIterator->second;

		if(*LevelScript->GetReCalculate())
		{
			inputfile ScriptFile("Script/Dungeon.dat");
			LevelScript->ReadFrom(ScriptFile, true);
		}
	}
	else
	{
		LevelScript = DungeonScript->GetLevelDefault();

		if(*LevelScript->GetReCalculate())
		{
			inputfile ScriptFile("Script/Dungeon.dat");
			LevelScript->ReadFrom(ScriptFile, true);
		}
	}

	return LevelScript;
}

void dungeon::PrepareLevel(ushort Index)
{
	if(Generated[Index])
	{
		LoadLevel(game::SaveName(), Index);
	}
	else
	{
		Level[Index] = new level;
		Level[Index]->Generate(GetLevelScript(Index));

		Generated[Index] = true;
	}
}

void dungeon::SaveLevel(std::string SaveName, ushort Number, bool DeleteAfterwards)
{
	outputfile SaveFile(SaveName + ".d" + Index + "-" + Number);

	SaveFile << Level[Number];

	if(DeleteAfterwards)
	{
		delete Level[Number];
		Level[Number] = 0;
	}
}

void dungeon::LoadLevel(std::string SaveName, ushort Number)
{
	inputfile SaveFile(SaveName + ".d" + Index + "-" + Number);

	Level[Number] = new level;
	Level[Number]->Load(SaveFile);
	Level[Number]->SetLevelScript(GetLevelScript(Number));
}

void dungeon::Save(outputfile& SaveFile) const
{
	SaveFile << Index;

	for(uchar c = 0; c < GetLevels(); ++c)
		SaveFile << Generated[c];
}

void dungeon::Load(inputfile& SaveFile)
{
	SaveFile >> Index;

	Initialize();

	for(uchar c = 0; c < GetLevels(); ++c)
		SaveFile >> Generated[c];
}

uchar dungeon::GetLevels() const
{
	return *DungeonScript->GetLevels();
}
