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
//#include "feio.h"
//#include "igraph.h"
#include "script.h"

dungeon::dungeon(uchar Index) : Index(Index)
{
	//if(Initialized)
	//	return;

	//iosystem::TextScreen(FONTW, "Generating dungeon...\n\nThis may take some time, please wait.", false);

	/*std::map<uchar, dungeonscript*>::iterator DungeonIterator = game::GetGameScript().GetDungeon().find(Index);

	//dungeonscript* DungeonScript;

	if(DungeonIterator != game::GetGameScript().GetDungeon().end())
		DungeonScript = DungeonIterator->second;
	else
		DungeonScript = game::GetGameScript().GetDungeonDefault();

	//Levels = *DungeonScript->GetLevels();

	Level = new level*[GetLevels()];
	Generated = new bool[GetLevels()];

	for(uchar c = 0; c < GetLevels(); ++c)
	{
		Level[c] = 0;
		Generated[c] = false;
	}*/

	Initialize();

	for(uchar c = 0; c < GetLevels(); ++c)
		Generated[c] = false;

	//Initialized = true;
}

dungeon::~dungeon()
{
	//if(Initialized)
	//{
		//delete [] BlockMap;

		for(ushort c = 0; c < GetLevels(); ++c)
			delete Level[c];

		delete [] Level;
	//}
}

void dungeon::Initialize()
{
	std::map<uchar, dungeonscript*>::iterator DungeonIterator = game::GetGameScript().GetDungeon().find(Index);

	//dungeonscript* DungeonScript;

	if(DungeonIterator != game::GetGameScript().GetDungeon().end())
		DungeonScript = DungeonIterator->second;
	else
		DungeonScript = game::GetGameScript().GetDungeonDefault();

	//Levels = *DungeonScript->GetLevels();

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
	//SaveLevel(game::SaveName(), c);

	/*switch(Index)
	{
	case 0:
	{

	Levels = 10;

	Alloc3D(BlockMap, Levels, 36, 36, uchar(0));

	for(uchar c = 0; c < Levels; ++c)
		Fill2D(BlockMap[c], 0, 0, 36, 36, c);

	Level = new level*[Levels];

	{
	for(uchar c = 0; c < Levels; ++c)
		Level[c] = new level(36, 36, c);
	}

	Level[3]->SetLevelMessage("You hear a wailing scream in the distance. An Enner Beast must dwell in the level!");
	Level[6]->SetLevelMessage("You shudder as you sense a being of pure darkness nearby. Your goal is near.");
	Level[9]->SetLevelMessage("You are welcomed by an evil laughter: \"Welcome to my private lair, mortal! There's no escape now! Prepare to be pepsified!\" Suddenly the stairs behind you are gone.");

	vector2d PerttuPos = vector2d(5 + rand() % (Level[0]->GetXSize() - 10), 5 + rand() % (Level[0]->GetYSize() - 10));

	{
	for(ushort c = 0; !Level[0]->MakeRoom(PerttuPos + vector2d(-2, -2), vector2d(5, 5), false); ++c)
		PerttuPos = vector2d(5 + rand() % (Level[0]->GetXSize() - 10), 5 + rand() % (Level[0]->GetYSize() - 10));
	}

	Level[0]->GetLevelSquare(PerttuPos)->ChangeLevelTerrain(new parquet, new stairsup);
	Level[0]->SetUpStairs(PerttuPos);

	Level[0]->GetLevelSquare(PerttuPos)->FastAddCharacter(new perttu);

	vector2d Pos = vector2d(6 + rand() % (Level[8]->GetXSize() - 12), 6 + rand() % (Level[8]->GetYSize() - 12));

	{
	for(ushort c = 0; !Level[8]->MakeRoom(Pos + vector2d(-3, -3), vector2d(7, 7), false); ++c)
		Pos = vector2d(6 + rand() % (Level[8]->GetXSize() - 12), 6 + rand() % (Level[8]->GetYSize() - 12));
	}

	Level[9]->MakeRoom(Pos + vector2d(-3, -3), vector2d(7, 7), false, 16);

	Level[8]->GetLevelSquare(Pos)->ChangeLevelTerrain(new parquet, new stairsdown(new pepsi(1)));
	Level[8]->SetDownStairs(Pos);
	Level[8]->GetLevelSquare(Pos + vector2d(0, -2))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
	Level[8]->GetLevelSquare(Pos + vector2d(0,  2))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
	Level[8]->GetLevelSquare(Pos + vector2d(-2, 0))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
	Level[8]->GetLevelSquare(Pos + vector2d( 2, 0))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
	Level[9]->GetLevelSquare(Pos + vector2d(0,  2))->ChangeLevelTerrain(new parquet, new stairsup(new pepsi(1)));
	Level[9]->SetUpStairs(Pos + vector2d(0,  2));
	altar* Altar = new altar(new pepsi(1));
	Altar->SetOwnerGod(16);
	Level[9]->GetLevelSquare(Pos + vector2d(0, -2))->ChangeLevelTerrain(new parquet, Altar); //GGG

	DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, Level[9]->GetXSize() - 1, Level[9]->GetYSize() - 1, Level[9]->GetLevelSquare(vector2d(DoX, DoY))->FastAddCharacter(new swatcommando);)
	Level[9]->GetLevelSquare(Pos + vector2d(0, -2))->FastAddCharacter(new oree);

	Level[9]->GetLevelSquare(Pos + vector2d(0,  2))->FastAddCharacter(new golem(new pepsi(100000)));
	Level[9]->GetLevelSquare(Pos + vector2d(-2, 0))->FastAddCharacter(new golem(new pepsi(100000)));
	Level[9]->GetLevelSquare(Pos + vector2d( 2, 0))->FastAddCharacter(new golem(new pepsi(100000)));

	{
	for(ushort c = 0; c < game::GetLevels() - 1; ++c)
	{
		Level[c]->Generate();

		if(c != 6 && c != 8)
			Level[c]->CreateDownStairs();
	}
	}

	Level[0]->GetLevelSquare(PerttuPos)->GetStack()->Clean();

	Pos = Level[3]->RandomSquare(true);
	Level[3]->GetLevelSquare(Pos)->FastAddCharacter(new ennerbeast);

	Pos = Level[6]->RandomSquare(true);
	Level[6]->GetLevelSquare(Pos)->FastAddCharacter(new elpuri);

	{
		for(ushort c = 0; c < Levels; ++c)
			SaveLevel(game::SaveName(), c);
	}

	Initialized = true;

	break;
	}

	case 1:
	{

	Levels = 1;

	Alloc3D(BlockMap, Levels, 36, 36, uchar(0));

	{
	for(uchar c = 0; c < Levels; ++c)
		Fill2D(BlockMap[c], 0, 0, 36, 36, c);
	}

	Level = new level*[Levels];

	{
	for(uchar c = 0; c < Levels; ++c)
		Level[c] = new level(36, 36, c);
	}

	Level[0]->MakeRoom(vector2d(2, 2), vector2d(32, 32), false);
	Level[0]->SetUpStairs(vector2d(18, 18));

	for(uchar c = 0; c < 30; ++c)
	{
		const character* const Prototype[3] = { farmer::GetPrototype(), cityguard::GetPrototype(), shopkeeper::GetPrototype() };

		vector2d Pos = Level[0]->RandomSquare(true);

		if(!Level[0]->GetSquare(Pos)->GetCharacter())
			Level[0]->GetSquare(Pos)->AddCharacter(Prototype[rand() % 3]->Clone());
	}

	{
		for(ushort c = 0; c < Levels; ++c)
			SaveLevel(game::SaveName(), c);
	}

	Initialized = true;

	break;
	}
	default:
		ABORT("Digging prohibited");
	}*/
}

void dungeon::SaveLevel(std::string SaveName, ushort Number, bool DeleteAfterwards)
{
	outputfile SaveFile(SaveName + ".d" + Index + "-" + Number);

	if(!SaveFile.GetBuffer().is_open())
		ABORT("Level lost!");

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

	if(!SaveFile.GetBuffer().is_open())
		ABORT("Level gone!");

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

	//if(Initialized)
	//{
		/*Alloc3D(BlockMap, Levels, 36, 36, uchar(0));

		for(uchar c = 0; c < Levels; ++c)
			Fill2D(BlockMap[c], 0, 0, 36, 36, c);*/

		//Level = new level*[GetLevels()];
		//Generated = new bool[GetLevels()];

		for(uchar c = 0; c < GetLevels(); ++c)
		{
			//Level[c] = 0;
			SaveFile >> Generated[c];
		}
	//}
}

uchar dungeon::GetLevels() const
{
	return *DungeonScript->GetLevels();
}
