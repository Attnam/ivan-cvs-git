#include <typeinfo>

#include "script.h"
#include "save.h"
#include "error.h"
#include "proto.h"
#include "materba.h"
#include "lterraba.h"
#include "charba.h"
#include "allocate.h"
#include "itemba.h"
#include "team.h"

void posscript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

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

			if(Word == "InRoom")
			{
				if(!IsInRoom)
					IsInRoom = new bool;

				*IsInRoom = SaveFile.ReadBool();

				continue;
			}
			ABORT("Odd script term %s encountered in position script!", Word.c_str());
		}
	}
}

template <class type> void basecontentscript<type>::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(Word == "=")
		Word = SaveFile.ReadWord();

	ushort Index;

	for(ushort c = 0;; ++c)
		if(Index = protocontainer<material>::SearchCodeName(Word))
		{
			if(c < MaterialData.size())
			{
				*MaterialData[c].first = Index;

				Word = SaveFile.ReadWord();

				if(Word == "(")
				{
					*MaterialData[c].second = SaveFile.ReadNumber(ValueMap);
					Word = SaveFile.ReadWord();
				}
			}
			else
			{
				ushort* NewIndex = new ushort;
				ulong* NewVolume = 0;
				*NewIndex = Index;

				Word = SaveFile.ReadWord();

				if(Word == "(")
				{
					NewVolume = new ulong;
					*NewVolume = SaveFile.ReadNumber(ValueMap);
					Word = SaveFile.ReadWord();
				}

				MaterialData.push_back(std::pair<ushort*, ulong*>(NewIndex, NewVolume));
			}
		}
		else
			break;

	if((Index = protocontainer<type>::SearchCodeName(Word)) || Word == "0")
	{
		if(!ContentType)
			ContentType = new ushort;

		*ContentType = Index;

		Word = SaveFile.ReadWord();
	}

	if(Word == "{")
		int esko = 2;

	ReadParameters(SaveFile, Word);
}

template <class type> type* basecontentscript<type>::Instantiate() const
{
	if(!ContentType)
		ABORT("Illegal content script instantiation of %s!", typeid(type).name());

	type* Instance = protocontainer<type>::GetProto(*ContentType)->Clone();

	for(ushort c = 0; c < MaterialData.size(); ++c)
		Instance->SetMaterial(c, protocontainer<material>::GetProto(*MaterialData[c].first)->Clone(MaterialData[c].second ? *MaterialData[c].second : 0));

	return Instance;
}

template <class type> void basecontentscript<type>::ReadParameters(inputfile&, std::string LastWord)
{
	if(LastWord != ";" && LastWord != ",")
		ABORT("Script error: Odd terminator %s encountered in content script of %s!", LastWord.c_str(), typeid(type).name());
}

template <class type> ushort* basecontentscript<type>::GetMaterialType(ushort Index, bool AOE) const
{
	if(Index < MaterialData.size() && MaterialData[Index].first)
		return MaterialData[Index].first;
	else
	{
		if(AOE)
			ABORT("Undefined script member MaterialData[%d] sought!", Index);

		return 0;
	}
}

template <class type> ulong* basecontentscript<type>::GetMaterialVolume(ushort Index, bool AOE) const
{
	if(Index < MaterialData.size() && MaterialData[Index].second)
		return MaterialData[Index].second;
	else
	{
		if(AOE)
			ABORT("Undefined script member MaterialData[%d] sought!", Index);

		return 0;
	}
}

void contentscript<character>::ReadParameters(inputfile& SaveFile, std::string LastWord)
{
	if(LastWord == "{")
		for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
		{
			if(Word == "Team")
			{
				if(!Team)
					Team = new ushort;

				*Team = SaveFile.ReadNumber(ValueMap);

				continue;
			}

			ABORT("Odd script term %s encountered in character script!", Word.c_str());
		}
	else
		basecontentscript<character>::ReadParameters(SaveFile, LastWord);
}

character* contentscript<character>::Instantiate() const
{
	character* Instance = basecontentscript<character>::Instantiate();

	if(GetTeam(false))
		Instance->SetTeam(game::GetTeam(*GetTeam()));

	return Instance;
}

void contentscript<overlevelterrain>::ReadParameters(inputfile& SaveFile, std::string LastWord)
{
	if(LastWord == "{")
		for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
		{
			if(Word == "Locked")
			{
				if(!Locked)
					Locked = new bool;

				*Locked = SaveFile.ReadBool();

				continue;
			}

			ABORT("Odd script term %s encountered in overlevelterrain script!", Word.c_str());
		}
	else
		basecontentscript<overlevelterrain>::ReadParameters(SaveFile, LastWord);
}

overlevelterrain* contentscript<overlevelterrain>::Instantiate() const
{
	overlevelterrain* Instance = basecontentscript<overlevelterrain>::Instantiate();

	if(GetLocked(false) && *GetLocked())
		Instance->Lock();

	return Instance;
}

void squarescript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(Word != "=")
	{
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

			if(Word == "Times")
			{
				if(!Times)
					Times = new uchar;

				*Times = SaveFile.ReadNumber(ValueMap);

				continue;
			}

			ABORT("Odd script term %s encountered in square script!", Word.c_str());
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
		ABORT("Bracket missing in content map script of %s!", typeid(type).name());

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
				ABORT("Missing bracket in content map script of %s!", typeid(type).name());

			for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
			{
				/* This may cause a memory leak if room's ReCalculate == true!!! */

				contentscript<type>* ContentScript = new contentscript<type>;

				ContentScript->SetValueMap(ValueMap);
				ContentScript->ReadFrom(SaveFile);

				if(*ContentScript->GetContentType())
					SymbolMap[Word[0]] = ContentScript;
				else
				{
					SymbolMap[Word[0]] = 0;
					delete ContentScript;
				}
			}

			continue;
		}

		ABORT("Odd script term %s encountered in content script of %s!", Word.c_str(), typeid(type).name());
	}

	if(!ContentScriptMap)
		Alloc2D(ContentScriptMap, GetSize()->X, GetSize()->Y);

	if(SaveFile.ReadWord() != "{")
		ABORT("Missing bracket in content map script of %s!", typeid(type).name());

	for(ushort y = 0; y < GetSize()->Y; ++y)
		for(ushort x = 0; x < GetSize()->X; ++x)
		{
			char Char = SaveFile.ReadLetter();

			std::map<char, contentscript<type>*>::iterator Iterator = SymbolMap.find(Char);

			if(Iterator != SymbolMap.end())
				ContentScriptMap[x][y] = Iterator->second;
			else
				ABORT("Illegal content %c in content map of %s!", Char, typeid(type).name());
		}

	if(SaveFile.ReadWord() != "}")
		ABORT("Missing bracket in content map script of %s!", typeid(type).name());
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

		if(Word == "GenerateLamps")
		{
			if(!GenerateLamps)
				GenerateLamps = new bool;

			*GenerateLamps = SaveFile.ReadBool();

			continue;
		}

		if(Word == "Type")
		{
			if(!Type)
				Type = new ushort;
 
			SaveFile.ReadWord();
			Word = SaveFile.ReadWord();
			SaveFile.ReadWord();

			*Type = protocontainer<room>::SearchCodeName(Word);

			if(!*Type)
				ABORT("Odd room type %s encountered in room script!", Word.c_str());

			continue;
		}

		if(Word == "GenerateFountains")
		{
			if(!GenerateFountains)
				GenerateFountains = new bool;

			*GenerateFountains = SaveFile.ReadBool();

			continue;
		}

		if(Word == "AllowLockedDoors")
		{
			if(!AllowLockedDoors)
				AllowLockedDoors = new bool;

			*AllowLockedDoors = SaveFile.ReadBool();

			continue;
		}

		ABORT("Odd script term %s encountered in room script!", Word.c_str());
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

		if(Word == "TeamDefault")
		{
			if(!TeamDefault)
				TeamDefault = new uchar;

			*TeamDefault = SaveFile.ReadNumber(ValueMap);

			continue;
		}

		if(Word == "AmbientLight")
		{
			if(!AmbientLight)
				AmbientLight = new ushort;

			*AmbientLight  = SaveFile.ReadNumber(ValueMap);

			continue;
		}

		if(Word == "Description")
		{
			SaveFile.ReadWord();

			if(!Description)
				Description = new std::string;

			*Description = SaveFile.ReadWord();

			SaveFile.ReadWord();

			continue;
		}

		if(Word == "LOSModifier")
		{
			if(!LOSModifier)
				LOSModifier = new uchar;

			*LOSModifier = SaveFile.ReadNumber(ValueMap);

			continue;
		}

		ABORT("Odd script term %s encountered in level script!", Word.c_str());
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

		ABORT("Odd script term %s encountered in dungeon script!", Word.c_str());
	}
}

void teamscript::ReadFrom(inputfile& SaveFile)
{
	ValueMap["HOSTILE"] = HOSTILE;
	ValueMap["NEUTRAL"] = NEUTRAL;
	ValueMap["FRIEND"] = FRIEND;

	if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in team script!");

	for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	{
		if(Word == "Relation")
		{
			std::pair<uchar, uchar> Rel;

			Rel.first = SaveFile.ReadNumber(ValueMap);
			Rel.second = SaveFile.ReadNumber(ValueMap);

			Relation.push_back(Rel);

			continue;
		}

		ABORT("Odd script term %s encountered in team script!", Word.c_str());
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

		if(Word == "Teams")
		{
			if(!Teams)
				Teams = new uchar;

			*Teams = SaveFile.ReadNumber(ValueMap);

			continue;
		}

		if(Word == "Team")
		{
			ushort Index = SaveFile.ReadNumber(ValueMap);
			teamscript* TS = new teamscript;

			TS->SetValueMap(ValueMap);
			TS->ReadFrom(SaveFile);

			Team.push_back(std::pair<uchar, teamscript*>(Index, TS));

			continue;
		}

		ABORT("Odd script term %s encountered in game script!", Word.c_str());
	}
}
