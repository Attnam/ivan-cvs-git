#include "script.h"
#include "save.h"
#include "error.h"
#include "proto.h"
#include "materba.h"
#include "lterraba.h"
#include "charba.h"
#include "allocate.h"
#include "itemba.h"

template contentscript<character>;
template contentscript<item>;
template contentscript<groundlevelterrain>;
template contentscript<overlevelterrain>;

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

template <class type> void contentscript<type>::ReadFrom(inputfile& SaveFile)
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

	if(Index = protocontainer<type>::SearchCodeName(Word))
	{
		if(!ContentType)
			ContentType = new ushort;

		*ContentType = Index;

		Word = SaveFile.ReadWord();
	}

	if(Word != ";" && Word != ",")
		ABORT("Script error: Odd terminator %s encountered in content script!", Word.c_str());
}

template <class type> type* contentscript<type>::Instantiate() const
{
	if(!ContentType)
		ABORT("Illegal content script instantiation!");

	type* Instance;

	if(MaterialType)
	{
		Instance = protocontainer<type>::GetProto(*ContentType)->Clone(false);
		Instance->InitMaterials(protocontainer<material>::GetProto(*MaterialType)->Clone());
	}
	else
		Instance = protocontainer<type>::GetProto(*ContentType)->Clone();

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
			if(Word == "Character")
			{
				if(!Character)
					Character = new contentscript<character>;

				Character->SetValueMap(ValueMap);
				Character->ReadFrom(SaveFile);

				continue;
			}

			if(Word == "Item")
			{
				if(!Item)
					Item = new contentscript<item>;

				Item->SetValueMap(ValueMap);
				Item->ReadFrom(SaveFile);

				continue;
			}

			if(Word == "GroundTerrain")
			{
				if(!GroundTerrain)
					GroundTerrain = new contentscript<groundlevelterrain>;

				GroundTerrain->SetValueMap(ValueMap);
				GroundTerrain->ReadFrom(SaveFile);

				continue;
			}

			if(Word == "OverTerrain")
			{
				if(!OverTerrain)
					OverTerrain = new contentscript<overlevelterrain>;

				OverTerrain->SetValueMap(ValueMap);
				OverTerrain->ReadFrom(SaveFile);

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

			if(Word == "IsWorldMapEntry")
			{
				if(!IsWorldMapEntry)
					IsWorldMapEntry = new bool;

				*IsWorldMapEntry = SaveFile.ReadBool();

				continue;
			}
		}
	}
	else
	{
		if(!GroundTerrain)
			GroundTerrain = new contentscript<groundlevelterrain>;

		GroundTerrain->SetValueMap(ValueMap);
		GroundTerrain->ReadFrom(SaveFile);

		if(!OverTerrain)
			OverTerrain = new contentscript<overlevelterrain>;

		OverTerrain->SetValueMap(ValueMap);
		OverTerrain->ReadFrom(SaveFile);
	}
}

template <class type> void contentmap<type>::ReadFrom(inputfile& SaveFile)
{
	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in content map script!");

	std::map<char, contentscript<type>*> SymbolMap;

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
				contentscript<type>* ContentScript = new contentscript<type>;

				ContentScript->SetValueMap(ValueMap);
				ContentScript->ReadFrom(SaveFile);

				SymbolMap[Word[0]] = ContentScript;
			}

			continue;
		}
	}

	if(!ContentScriptMap)
		Alloc2D(ContentScriptMap, GetSize()->X, GetSize()->Y);

	if(SaveFile.ReadWord() != "{")
		ABORT("Missing bracket in content map script!");

	for(ushort y = 0; y < GetSize()->Y; ++y)
		for(ushort x = 0; x < GetSize()->X; ++x)
		{
			char Char = SaveFile.ReadLetter();

			std::map<char, contentscript<type>*>::iterator Iterator = SymbolMap.find(Char);

			if(Iterator != SymbolMap.end())
				ContentScriptMap[x][y] = Iterator->second;
			else
				ABORT("Illegal content %c in content map!", Char);
		}

	if(SaveFile.ReadWord() != "}")
		ABORT("Missing bracket in content map script!");
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
				CharacterMap = new contentmap<character>;

			CharacterMap->SetValueMap(ValueMap);
			CharacterMap->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "ItemMap")
		{
			if(!ItemMap)
				ItemMap = new contentmap<item>;

			ItemMap->SetValueMap(ValueMap);
			ItemMap->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "GroundTerrainMap")
		{
			if(!GroundTerrainMap)
				GroundTerrainMap = new contentmap<groundlevelterrain>;

			GroundTerrainMap->SetValueMap(ValueMap);
			GroundTerrainMap->ReadFrom(SaveFile);

			continue;
		}

		if(Word == "OverTerrainMap")
		{
			if(!OverTerrainMap)
				OverTerrainMap = new contentmap<overlevelterrain>;

			OverTerrainMap->SetValueMap(ValueMap);
			OverTerrainMap->ReadFrom(SaveFile);

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

		if(Word == "GenerateTunnel")
		{
			if(!GenerateTunnel)
				GenerateTunnel = new bool;

			*GenerateTunnel = SaveFile.ReadBool();

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

		if(Word == "OnGround")
		{
			if(!OnGround)
				OnGround = new bool;

			*OnGround = SaveFile.ReadBool();

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
