#include "script.h"
#include "save.h"
#include "error.h"
#include "proto.h"
#include "materba.h"
#include "lterraba.h"
#include "charba.h"
#include "allocate.h"

void posscript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(Word != ",")
		ABORT("Colon missing in position script!");

	Word = SaveFile.ReadWord();

	if(Word == "Pos")
	{
		Random = false;

		if(!Vector)
			Vector = new vector2d;

		*Vector = SaveFile.ReadVector2d(ValueMap);
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
				if(!IsWalkable)
					IsWalkable = new bool;

				*IsWalkable = SaveFile.ReadBool();

				continue;
			}
		}
	}
}

void groundterrainscript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(Word == "=")
		Word = SaveFile.ReadWord();

	ushort Index;

	if(Index = protocontainer<material>::SearchCodeName(Word))
	{
		if(!MaterialType)
			MaterialType = new ushort;

		*MaterialType = Index;

		Word = SaveFile.ReadWord();
	}

	if(Index = protocontainer<groundlevelterrain>::SearchCodeName(Word))
	{
		if(!TerrainType)
			TerrainType = new ushort;

		*TerrainType = Index;

		Word = SaveFile.ReadWord();
	}

	if(Word != ";" && Word != ",")
		ABORT("Script error in groundterrain script!");
}

groundlevelterrain* groundterrainscript::Instantiate() const
{
	if(!TerrainType)
		ABORT("Illegal groundterrainscript instantiation!");

	groundlevelterrain* Instance;

	if(MaterialType)
	{
		Instance = protocontainer<groundlevelterrain>::GetProto(*TerrainType)->Clone(false);
		Instance->InitMaterials(protocontainer<material>::GetProto(*MaterialType)->Clone());
	}
	else
		Instance = protocontainer<groundlevelterrain>::GetProto(*TerrainType)->Clone();

	return Instance;
}

void overterrainscript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(Word == "=")
		Word = SaveFile.ReadWord();

	ushort Index;

	if(Index = protocontainer<material>::SearchCodeName(Word))
	{
		if(!MaterialType)
			MaterialType = new ushort;

		*MaterialType = Index;

		Word = SaveFile.ReadWord();
	}

	if(Index = protocontainer<overlevelterrain>::SearchCodeName(Word))
	{
		if(!TerrainType)
			TerrainType = new ushort;

		*TerrainType = Index;

		Word = SaveFile.ReadWord();
	}

	if(Word != ";" && Word != ",")
		ABORT("Script error in overterrain script!");
}

overlevelterrain* overterrainscript::Instantiate() const
{
	if(!TerrainType)
		ABORT("Illegal overterrainscript instantiation!");

	overlevelterrain* Instance;

	if(MaterialType)
	{
		Instance = protocontainer<overlevelterrain>::GetProto(*TerrainType)->Clone(false);
		Instance->InitMaterials(protocontainer<material>::GetProto(*MaterialType)->Clone());
	}
	else
		Instance = protocontainer<overlevelterrain>::GetProto(*TerrainType)->Clone();

	return Instance;
}

void characterscript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(Word == "=")
		Word = SaveFile.ReadWord();

	ushort Index;

	if(Index = protocontainer<material>::SearchCodeName(Word))
	{
		if(!MaterialType)
			MaterialType = new ushort;

		*MaterialType = Index;

		Word = SaveFile.ReadWord();
	}

	if(Index = protocontainer<character>::SearchCodeName(Word))
	{
		if(!CharacterType)
			CharacterType = new ushort;

		*CharacterType = Index;

		Word = SaveFile.ReadWord();
	}

	if(Word != ";" && Word != ",")
		ABORT("Script error in character script!");
}

character* characterscript::Instantiate() const
{
	if(!CharacterType)
		ABORT("Illegal characterscript instantiation!");

	character* Instance;

	if(MaterialType)
	{
		Instance = protocontainer<character>::GetProto(*CharacterType)->Clone(false);
		Instance->InitMaterials(protocontainer<material>::GetProto(*MaterialType)->Clone());
	}
	else
		Instance = protocontainer<character>::GetProto(*CharacterType)->Clone();

	return Instance;
}

void squarescript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(Word != "=")
	{
		SaveFile.GetBuffer().seekg(-Word.length(), std::ios::cur);

		if(!PosScript)
			PosScript = new posscript;

		PosScript->SetValueMap(ValueMap);
		PosScript->ReadFrom(SaveFile);

		if(SaveFile.ReadWord() != "{")
			ABORT("Bracket missing in square script!");

		for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
		{
			if(Word == "GroundTerrain")
			{
				if(!GroundTerrain)
					GroundTerrain = new groundterrainscript;

				GroundTerrain->SetValueMap(ValueMap);
				GroundTerrain->ReadFrom(SaveFile);

				continue;
			}

			if(Word == "OverTerrain")
			{
				if(!OverTerrain)
					OverTerrain = new overterrainscript;

				OverTerrain->SetValueMap(ValueMap);
				OverTerrain->ReadFrom(SaveFile);

				continue;
			}

			if(Word == "Character")
			{
				if(!Character)
					Character = new characterscript;

				Character->SetValueMap(ValueMap);
				Character->ReadFrom(SaveFile);

				continue;
			}

			if(Word == "IsUpStairs")
			{
				if(!IsUpStairs)
					IsUpStairs = new bool;

				*IsUpStairs = SaveFile.ReadBool();

				continue;
			}

			if(Word == "IsDownStairs")
			{
				if(!IsDownStairs)
					IsDownStairs = new bool;

				*IsDownStairs = SaveFile.ReadBool();

				continue;
			}
		}
	}
	else
	{
		if(!GroundTerrain)
			GroundTerrain = new groundterrainscript;

		GroundTerrain->SetValueMap(ValueMap);
		GroundTerrain->ReadFrom(SaveFile);

		if(!OverTerrain)
			OverTerrain = new overterrainscript;

		OverTerrain->SetValueMap(ValueMap);
		OverTerrain->ReadFrom(SaveFile);
	}
}

void charactermap::ReadFrom(inputfile& SaveFile)
{
	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in room script!");

	std::map<char, characterscript*> SymbolMap;

	SymbolMap['.'] = 0;

	for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	{
		if(Word == "Pos")
		{
			if(!Pos)
				Pos = new vector2d;

			*Pos = SaveFile.ReadVector2d(ValueMap);

			continue;
		}

		if(Word == "Size")
		{
			if(!Size)
				Size = new vector2d;

			*Size = SaveFile.ReadVector2d(ValueMap);

			continue;
		}

		if(Word == "Types")
		{
			if(SaveFile.ReadWord() != "{")
				ABORT("Bracket missing in room script!");

			for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
			{
				characterscript* CharacterScript = new characterscript;

				CharacterScript->SetValueMap(ValueMap);
				CharacterScript->ReadFrom(SaveFile);

				SymbolMap[Word[0]] = CharacterScript;
			}

			continue;
		}
	}

	if(!CharacterScriptMap)
		Alloc2D(CharacterScriptMap, GetSize()->X, GetSize()->Y);

	if(SaveFile.ReadWord() != "{")
		ABORT("Missing bracket in charactermap script!");

	for(ushort y = 0; y < GetSize()->Y; ++y)
		for(ushort x = 0; x < GetSize()->X; ++x)
		{
			char Char = SaveFile.ReadLetter();

			std::map<char, characterscript*>::iterator Iterator = SymbolMap.find(Char);

			if(Iterator != SymbolMap.end())
				CharacterScriptMap[x][y] = Iterator->second;
			else
				ABORT("Illegal character %c in charactermap!", Char);
		}

	if(SaveFile.ReadWord() != "}")
		ABORT("Missing bracket in charactermap script!");
}

void roomscript::ReadFrom(inputfile& SaveFile, bool ReRead)
{
	if(ReRead)
	{
		if(Base)
			if(Base->GetReCalculate(false) && *Base->GetReCalculate(false))
				Base->ReadFrom(SaveFile, true);

		if(ReCalculate && *ReCalculate)
		{
			SaveFile.GetBuffer().clear();
			SaveFile.GetBuffer().seekg(BufferPos, std::ios::beg);
		}
		else
			return;
	}
	else
		BufferPos = SaveFile.GetBuffer().tellg();

	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in room script!");

	for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	{
		if(Word == "Square")
		{
			squarescript* SS = new squarescript;

			SS->SetValueMap(ValueMap);
			SS->ReadFrom(SaveFile);

			if(!ReRead)
				Square.push_back(SS);
			else
				delete SS;

			continue;
		}

		if(Word == "CharacterMap")
		{
			if(!CharacterMap)
				CharacterMap = new charactermap;

			CharacterMap->SetValueMap(ValueMap);
			CharacterMap->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "WallSquare")
		{
			if(!WallSquare)
				WallSquare = new squarescript;

			WallSquare->SetValueMap(ValueMap);
			WallSquare->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "FloorSquare")
		{
			if(!FloorSquare)
				FloorSquare = new squarescript;

			FloorSquare->SetValueMap(ValueMap);
			FloorSquare->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "DoorSquare")
		{
			if(!DoorSquare)
				DoorSquare = new squarescript;

			DoorSquare->SetValueMap(ValueMap);
			DoorSquare->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "Size")
		{
			if(!Size)
				Size = new vector2d;

			*Size = SaveFile.ReadVector2d(ValueMap);

			continue;
		}

		if(Word == "AltarPossible")
		{
			if(!AltarPossible)
				AltarPossible = new bool;

			*AltarPossible = SaveFile.ReadBool();

			continue;
		}

		if(Word == "GenerateDoor")
		{
			if(!GenerateDoor)
				GenerateDoor = new bool;

			*GenerateDoor = SaveFile.ReadBool();

			continue;
		}

		if(Word == "Pos")
		{
			if(!Pos)
				Pos = new vector2d;

			*Pos = SaveFile.ReadVector2d(ValueMap);

			continue;
		}

		if(Word == "ReCalculate")
		{
			if(!ReCalculate)
				ReCalculate = new bool;

			*ReCalculate = SaveFile.ReadBool();

			continue;
		}

		if(Word == "DivineOwner")
		{
			if(!DivineOwner)
				DivineOwner = new uchar;

			*DivineOwner = SaveFile.ReadNumber(ValueMap);

			continue;
		}
	}
}

void levelscript::ReadFrom(inputfile& SaveFile, bool ReRead)
{
	if(ReRead)
	{
		if(Base)
			if(Base->GetReCalculate(false) && *Base->GetReCalculate(false))
				Base->ReadFrom(SaveFile, true);

		if(ReCalculate && *ReCalculate)
		{
			SaveFile.GetBuffer().clear();
			SaveFile.GetBuffer().seekg(BufferPos, std::ios::beg);
		}
		else
			return;
	}
	else
		BufferPos = SaveFile.GetBuffer().tellg();

	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in level script!");

	for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	{
		if(Word == "Square")
		{
			squarescript* SS = new squarescript;

			SS->SetValueMap(ValueMap);
			SS->ReadFrom(SaveFile);

			if(!ReRead)
				Square.push_back(SS);
			else
				delete SS;

			continue;
		}

		if(Word == "Room")
		{
			uchar Index = SaveFile.ReadNumber(ValueMap);

			std::map<uchar, roomscript*>::iterator Iterator = Room.find(Index);

			roomscript* RS = Iterator == Room.end() ? new roomscript : Iterator->second;

			RS->SetValueMap(ValueMap);

			if(RoomDefault)
				RS->SetBase(RoomDefault);
			else
				if(Base)
					RS->SetBase(Base->GetRoomDefault(false));

			RS->ReadFrom(SaveFile, ReRead);

			Room[Index] = RS;

			continue;
		}

		if(Word == "RoomDefault")
		{
			if(!RoomDefault)
				RoomDefault = new roomscript;

			RoomDefault->SetValueMap(ValueMap);

			if(Base)
				RoomDefault->SetBase(Base->GetRoomDefault(false));

			RoomDefault->ReadFrom(SaveFile, ReRead);

			continue;
		}

		if(Word == "FillSquare")
		{
			if(!FillSquare)
				FillSquare = new squarescript;

			FillSquare->SetValueMap(ValueMap);
			FillSquare->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "LevelMessage")
		{
			SaveFile.ReadWord();

			if(!LevelMessage)
				LevelMessage = new std::string;

			*LevelMessage = SaveFile.ReadWord();

			SaveFile.ReadWord();

			continue;
		}

		if(Word == "Size")
		{
			SaveFile.ReadWord();

			if(!Size)
				Size = new vector2d;

			*Size = SaveFile.ReadVector2d(ValueMap);

			ValueMap["XSize"] = Size->X;
			ValueMap["YSize"] = Size->Y;

			continue;
		}

		if(Word == "Items")
		{
			if(!Items)
				Items = new ushort;

			*Items = SaveFile.ReadNumber(ValueMap);

			continue;
		}

		if(Word == "Rooms")
		{
			if(!Rooms)
				Rooms = new uchar;

			*Rooms = SaveFile.ReadNumber(ValueMap);

			continue;
		}

		if(Word == "GenerateMonsters")
		{
			if(!GenerateMonsters)
				GenerateMonsters = new bool;

			*GenerateMonsters = SaveFile.ReadBool();

			continue;
		}

		if(Word == "ReCalculate")
		{
			if(!ReCalculate)
				ReCalculate = new bool;

			*ReCalculate = SaveFile.ReadBool();

			continue;
		}

		if(Word == "GenerateUpStairs")
		{
			if(!GenerateUpStairs)
				GenerateUpStairs = new bool;

			*GenerateUpStairs = SaveFile.ReadBool();

			continue;
		}

		if(Word == "GenerateDownStairs")
		{
			if(!GenerateDownStairs)
				GenerateDownStairs = new bool;

			*GenerateDownStairs = SaveFile.ReadBool();

			continue;
		}
	}
}

void dungeonscript::ReadFrom(inputfile& SaveFile)
{
	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in dungeon script!");

	for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	{
		if(Word == "Level")
		{
			uchar Index = SaveFile.ReadNumber(ValueMap);

			std::map<uchar, levelscript*>::iterator Iterator = Level.find(Index);

			levelscript* LS = Iterator == Level.end() ? new levelscript : Iterator->second;

			LS->SetValueMap(ValueMap);

			if(LevelDefault)
				LS->SetBase(LevelDefault);
			else
				if(Base)
					LS->SetBase(Base->GetLevelDefault(false));

			LS->ReadFrom(SaveFile);

			Level[Index] = LS;

			continue;
		}

		if(Word == "LevelDefault")
		{
			if(!LevelDefault)
				LevelDefault = new levelscript;

			LevelDefault->SetValueMap(ValueMap);

			if(Base)
				LevelDefault->SetBase(Base->GetLevelDefault(false));

			LevelDefault->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "Levels")
		{
			if(!Levels)
				Levels = new uchar;

			*Levels = SaveFile.ReadNumber(ValueMap);

			continue;
		}
	}
}

void gamescript::ReadFrom(inputfile& SaveFile)
{
	for(std::string Word = SaveFile.ReadWord(false); !SaveFile.GetBuffer().eof(); Word = SaveFile.ReadWord(false))
	{
		if(Word == "Dungeon")
		{
			uchar Index = SaveFile.ReadNumber(ValueMap);

			std::map<uchar, dungeonscript*>::iterator Iterator = Dungeon.find(Index);

			dungeonscript* DS = Iterator == Dungeon.end() ? new dungeonscript : Iterator->second;

			DS->SetValueMap(ValueMap);
			DS->SetBase(DungeonDefault);
			DS->ReadFrom(SaveFile);

			Dungeon[Index] = DS;

			continue;
		}

		if(Word == "DungeonDefault")
		{
			if(!DungeonDefault)
				DungeonDefault = new dungeonscript;

			DungeonDefault->SetValueMap(ValueMap);
			DungeonDefault->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "Dungeons")
		{
			if(!Dungeons)
				Dungeons = new uchar;

			*Dungeons = SaveFile.ReadNumber(ValueMap);

			continue;
		}
	}
}
