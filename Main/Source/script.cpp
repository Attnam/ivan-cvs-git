#include "script.h"
#include "save.h"
#include "error.h"
#include "proto.h"
#include "materba.h"
#include "lterraba.h"
#include "charba.h"

void posscript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	std::string Word = SaveFile.ReadWord();

	if(Word != ",")
		ABORT("Colon missing in position script!");

	Word = SaveFile.ReadWord();

	if(Word == "Pos")
	{
		Random = false;
		Vector = SaveFile.ReadVector2d(ValueMap);
	}

	if(Word == "Random")
	{
		Random = true;

		if(SaveFile.ReadWord() != "{")
			ABORT("Script error in level script!");

		for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
		{
			if(Word == "Walkable")
			{
				IsWalkable = SaveFile.ReadBool();
				OverrideMap |= 1;
			}
		}
	}
}

void groundterrainscript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	std::string Word = SaveFile.ReadWord();

	if(Word == "=")
		Word = SaveFile.ReadWord();

	for(ushort c = 1; protocontainer<material>::GetProto(c); ++c)
		if(protocontainer<material>::GetProto(c)->ClassName() == Word)
		{
			MaterialType = c;
			OverrideMap |= 1;
			Word = SaveFile.ReadWord();
			break;
		}

	for(c = 1; protocontainer<groundlevelterrain>::GetProto(c); ++c)
		if(protocontainer<groundlevelterrain>::GetProto(c)->ClassName() == Word)
		{
			TerrainType = c;
			OverrideMap |= 2;
			Word = SaveFile.ReadWord();
			break;
		}

	if(Word != ";" && Word != ",")
		ABORT("Script error in groundterrain script!");
}

void overterrainscript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	std::string Word = SaveFile.ReadWord();

	if(Word == "=")
		Word = SaveFile.ReadWord();

	for(ushort c = 1; protocontainer<material>::GetProto(c); ++c)
		if(protocontainer<material>::GetProto(c)->ClassName() == Word)
		{
			MaterialType = c;
			OverrideMap |= 1;
			Word = SaveFile.ReadWord();
			break;
		}

	for(c = 1; protocontainer<overlevelterrain>::GetProto(c); ++c)
		if(protocontainer<overlevelterrain>::GetProto(c)->ClassName() == Word)
		{
			TerrainType = c;
			OverrideMap |= 2;
			Word = SaveFile.ReadWord();
			break;
		}

	if(Word != ";" && Word != ",")
		ABORT("Script error in overterrain script!");
}

void characterscript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	std::string Word = SaveFile.ReadWord();

	if(Word == "=")
		Word = SaveFile.ReadWord();

	for(ushort c = 1; protocontainer<material>::GetProto(c); ++c)
		if(protocontainer<material>::GetProto(c)->ClassName() == Word)
		{
			MaterialType = c;
			OverrideMap |= 1;
			Word = SaveFile.ReadWord();
			break;
		}

	for(c = 1; protocontainer<character>::GetProto(c); ++c)
		if(protocontainer<character>::GetProto(c)->ClassName() == Word)
		{
			CharacterType = c;
			OverrideMap |= 2;
			Word = SaveFile.ReadWord();
			break;
		}

	if(Word != ";" && Word != ",")
		ABORT("Script error in character script!");
}

void squarescript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	std::string Word = SaveFile.ReadWord();

	if(Word != "=")
	{
		SaveFile.GetBuffer().seekg(-Word.length(), std::ios::cur);

		PosScript.ReadFrom(SaveFile, ValueMap);

		if(SaveFile.ReadWord() != "{")
			ABORT("Bracket missing in square script!");

		for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
		{
			if(Word == "GroundTerrain")
			{
				GroundTerrain.ReadFrom(SaveFile, ValueMap);

				if(GroundTerrain.OverrideMap)
					OverrideMap |= 1;
			}

			if(Word == "OverTerrain")
			{
				OverTerrain.ReadFrom(SaveFile, ValueMap);

				if(OverTerrain.OverrideMap)
					OverrideMap |= 2;
			}

			if(Word == "Character")
			{
				Character.ReadFrom(SaveFile, ValueMap);

				if(Character.OverrideMap)
					OverrideMap |= 4;
			}
		}
	}
	else
	{
		GroundTerrain.ReadFrom(SaveFile, ValueMap);

		if(GroundTerrain.OverrideMap)
			OverrideMap |= 1;

		OverTerrain.ReadFrom(SaveFile, ValueMap);

		if(OverTerrain.OverrideMap)
			OverrideMap |= 2;
	}
}

void roomscript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in room script!");

	for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	{
		if(Word == "Square")
		{
			squarescript SS;

			SS.ReadFrom(SaveFile, ValueMap);

			Square.push_back(SS);
		}

		if(Word == "WallSquare")
		{
			WallSquare.ReadFrom(SaveFile, ValueMap);

			if(WallSquare.OverrideMap)
				OverrideMap |= 1;
		}

		if(Word == "FloorSquare")
		{
			FloorSquare.ReadFrom(SaveFile, ValueMap);

			if(FloorSquare.OverrideMap)
				OverrideMap |= 2;
		}

		if(Word == "DoorSquare")
		{
			DoorSquare.ReadFrom(SaveFile, ValueMap);

			if(DoorSquare.OverrideMap)
				OverrideMap |= 4;
		}

		if(Word == "Size")
		{
			Size = SaveFile.ReadVector2d(ValueMap);

			OverrideMap |= 8;
		}

		if(Word == "AltarPossible")
		{
			AltarPossible = SaveFile.ReadBool();

			OverrideMap |= 16;
		}

		if(Word == "GenerateDoor")
		{
			GenerateDoor = SaveFile.ReadBool();

			OverrideMap |= 32;
		}

		if(Word == "ReCalculate")
		{
			ReCalculate = SaveFile.ReadBool();

			OverrideMap |= 64;
		}
	}
}

void levelscript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in level script!");

	for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	{
		if(Word == "Square")
		{
			squarescript SS;

			SS.ReadFrom(SaveFile, ValueMap);

			Square.push_back(SS);
		}

		if(Word == "Room")
		{
			uchar Index = SaveFile.ReadNumber(ValueMap);

			roomscript RS;

			RS.ReadFrom(SaveFile, ValueMap);

			Room[Index] = RS;
		}

		if(Word == "RoomDefault")
		{
			RoomDefault.ReadFrom(SaveFile, ValueMap);

			if(RoomDefault.OverrideMap)
				OverrideMap |= 1;
		}

		if(Word == "FillSquare")
		{
			FillSquare.ReadFrom(SaveFile, ValueMap);

			if(FillSquare.OverrideMap)
				OverrideMap |= 2;
		}

		if(Word == "LevelMessage")
		{
			SaveFile.ReadWord();

			LevelMessage = SaveFile.ReadWord();

			SaveFile.ReadWord();

			OverrideMap |= 4;
		}

		if(Word == "Size")
		{
			SaveFile.ReadWord();

			Size = SaveFile.ReadVector2d(ValueMap);

			ValueMap["SizeX"] = Size.X;
			ValueMap["SizeY"] = Size.Y;

			OverrideMap |= 8;
		}

		if(Word == "Items")
		{
			Items = SaveFile.ReadNumber(ValueMap);

			OverrideMap |= 16;
		}

		if(Word == "Rooms")
		{
			Rooms = SaveFile.ReadNumber(ValueMap);

			OverrideMap |= 32;
		}

		if(Word == "GenerateMonsters")
		{
			GenerateMonsters = SaveFile.ReadBool();

			OverrideMap |= 64;
		}

		if(Word == "ReCalculate")
		{
			ReCalculate = SaveFile.ReadBool();

			OverrideMap |= 128;
		}
	}
}

void dungeonscript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in dungeon script!");

	for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	{
		if(Word == "Level")
		{
			uchar Index = SaveFile.ReadNumber(ValueMap);

			levelscript LS;

			LS.ReadFrom(SaveFile, ValueMap);

			Level[Index] = LS;
		}

		if(Word == "LevelDefault")
		{
			LevelDefault.ReadFrom(SaveFile, ValueMap);

			if(LevelDefault.OverrideMap)
				OverrideMap |= 1;
		}

		if(Word == "Levels")
		{
			Levels = SaveFile.ReadNumber(ValueMap);

			OverrideMap |= 2;
		}
	}
}

void gamescript::ReadFrom(inputfile& SaveFile, std::map<std::string, long> ValueMap)
{
	for(std::string Word = SaveFile.ReadWord(false); !SaveFile.GetBuffer().eof(); Word = SaveFile.ReadWord(false))
	{
		if(Word == "Dungeon")
		{
			uchar Index = SaveFile.ReadNumber(ValueMap);

			dungeonscript DS;

			DS.ReadFrom(SaveFile, ValueMap);

			Dungeon[Index] = DS;
		}

		if(Word == "DefaultDungeon")
		{
			DefaultDungeon.ReadFrom(SaveFile, ValueMap);

			if(DefaultDungeon.OverrideMap)
				OverrideMap |= 1;
		}

		if(Word == "Dungeons")
		{
			Dungeons = SaveFile.ReadNumber(ValueMap);

			OverrideMap |= 2;
		}
	}
}