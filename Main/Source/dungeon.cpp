#include "dungeon.h"
#include "allocate.h"
#include "level.h"
#include "lsquare.h"
#include "lterrain.h"
#include "char.h"
#include "stack.h"
#include "strover.h"
#include "error.h"

dungeon::dungeon(uchar Levels) : Levels(Levels)
{
	Alloc3D(BlockMap, Levels, 36, 36, uchar(0));

	for(uchar c = 0; c < Levels; c++)
		Fill2D(BlockMap[c], 0, 0, 36, 36, c);
}

dungeon::~dungeon(void)
{
	delete [] BlockMap;

	for(ushort c = 0; c < GetLevels(); c++)
		delete Level[c];

	delete [] Level;
}

void dungeon::Generate(void)
{
	Level = new level*[Levels];

	{
	for(uchar c = 0; c < Levels; c++)
		Level[c] = new level(36, 36, c);
	}

	vector PerttuPos = vector(5 + rand() % (Level[0]->GetXSize() - 10), 5 + rand() % (Level[0]->GetYSize() - 10));

	{
	for(ushort c = 0; !Level[0]->MakeRoom(PerttuPos + vector(-2, -2), vector(5, 5), false); c++)
		PerttuPos = vector(5 + rand() % (Level[0]->GetXSize() - 10), 5 + rand() % (Level[0]->GetYSize() - 10));
	}

	Level[0]->GetLevelSquare(PerttuPos)->ChangeLevelTerrain(new parquet, new stairsup);
	Level[0]->SetUpStairs(PerttuPos);

	Level[0]->GetLevelSquare(PerttuPos)->FastAddCharacter(new perttu);

	vector Pos = vector(6 + rand() % (Level[8]->GetXSize() - 12), 6 + rand() % (Level[8]->GetYSize() - 12));

	{
	for(ushort c = 0; !Level[8]->MakeRoom(Pos + vector(-3, -3), vector(7, 7), false); c++)
		Pos = vector(6 + rand() % (Level[8]->GetXSize() - 12), 6 + rand() % (Level[8]->GetYSize() - 12));
	}

	Level[9]->MakeRoom(Pos + vector(-3, -3), vector(7, 7), false, 16);

	Level[8]->GetLevelSquare(Pos)->ChangeLevelTerrain(new parquet, new stairsdown(new pepsi(1)));
	Level[8]->SetDownStairs(Pos);
	Level[8]->GetLevelSquare(Pos + vector(0, -2))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
	Level[8]->GetLevelSquare(Pos + vector(0,  2))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
	Level[8]->GetLevelSquare(Pos + vector(-2, 0))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
	Level[8]->GetLevelSquare(Pos + vector( 2, 0))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
	Level[9]->GetLevelSquare(Pos + vector(0,  2))->ChangeLevelTerrain(new parquet, new stairsup(new pepsi(1)));
	Level[9]->SetUpStairs(Pos + vector(0,  2));
	altar* Altar = new altar(new pepsi(1));
	Altar->SetOwnerGod(16);
	Level[9]->GetLevelSquare(Pos + vector(0, -2))->ChangeLevelTerrain(new parquet, Altar); //GGG

	DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, Level[9]->GetXSize() - 1, Level[9]->GetYSize() - 1, Level[9]->GetLevelSquare(vector(DoX, DoY))->FastAddCharacter(new swatcommando);)
	Level[9]->GetLevelSquare(Pos + vector(0, -2))->FastAddCharacter(new oree);

	Level[9]->GetLevelSquare(Pos + vector(0,  2))->FastAddCharacter(new golem(new pepsi(100000)));
	Level[9]->GetLevelSquare(Pos + vector(-2, 0))->FastAddCharacter(new golem(new pepsi(100000)));
	Level[9]->GetLevelSquare(Pos + vector( 2, 0))->FastAddCharacter(new golem(new pepsi(100000)));

	{
	for(ushort c = 0; c < game::GetLevels() - 1; c++)
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
		for(ushort c = 0; c < Levels; c++)
			SaveLevel(game::SaveName(), c);
	}
}

void dungeon::SaveLevel(std::string SaveName, ushort Index, bool DeleteAfterwards)
{
	std::ofstream SaveFile((SaveName + ".l" + Index).c_str(), std::ios::out | std::ios::binary);

	if(!SaveFile.is_open())
		ABORT("Level lost!");

	Level[Index]->Save(SaveFile);

	if(DeleteAfterwards)
	{
		delete Level[Index];
		Level[Index] = 0;
	}
}

void dungeon::LoadLevel(std::string SaveName, ushort Index)
{
	std::ifstream SaveFile((SaveName + ".l" + Index).c_str(), std::ios::in | std::ios::binary);

	if(!SaveFile.is_open())
		ABORT("Level gone!");

	Level[Index] = new level;
	Level[Index]->Load(SaveFile);
}

void dungeon::Save(std::ofstream& SaveFile) const
{
	SaveFile.write((char*)&Levels, sizeof(Levels));
}

void dungeon::Load(std::ifstream& SaveFile)
{
	SaveFile.read((char*)&Levels, sizeof(Levels));

	Alloc3D(BlockMap, Levels, 36, 36, uchar(0));

	for(uchar c = 0; c < Levels; c++)
		Fill2D(BlockMap[c], 0, 0, 36, 36, c);

	Level = new level*[Levels];

	for(c = 0; c < Levels; c++)
		Level[c] = 0;
}
