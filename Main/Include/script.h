#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#define SCRIPT_RETURN(member)\
{\
	if(member)\
		return member;\
	else\
	{\
		if(AOE)\
			ABORT("Undefined script member " #member " sought!");\
		\
		return 0;\
	}\
}

#define SCRIPT_RETURN_WITH_BASE(member)\
{\
	if(member)\
		return member;\
	else\
		if(Base)\
			return Base->Get##member##(AOE);\
		else\
		{\
			if(AOE)\
				ABORT("Undefined script member " #member " sought!");\
			\
			return 0;\
		}\
}

#include <string>
#include <vector>
#include <map>

#include "typedef.h"
#include "vector2d.h"
#include "error.h"

class inputfile;
class groundlevelterrain;
class overlevelterrain;
class character;
class item;

class script
{
public:
	std::map<std::string, long>& GetValueMap() { return ValueMap; }
	void SetValueMap(const std::map<std::string, long>& What) { ValueMap = What; }
protected:
	std::map<std::string, long> ValueMap;
};

class posscript : public script
{
public:
	posscript() : Vector(0), IsWalkable(0), IsInRoom(0) {}
	void ReadFrom(inputfile&);
	vector2d* GetVector(bool AOE = true) const { SCRIPT_RETURN(Vector) }
	bool* GetIsWalkable(bool AOE = true) const { SCRIPT_RETURN(IsWalkable) }
	bool* GetIsInRoom(bool AOE = true) const { SCRIPT_RETURN(IsInRoom) }
	bool GetRandom() const { return Random; }
protected:
	vector2d* Vector;
	bool* IsWalkable;
	bool* IsInRoom;
	bool Random;
};

template <class type> class contentscript : public script
{
public:
	contentscript() : ContentType(0) {}
	void ReadFrom(inputfile&);
	ushort* GetMaterialType(ushort, bool = true) const;
	ulong* GetMaterialVolume(ushort, bool = true) const;
	ushort* GetContentType(bool AOE = true) const { SCRIPT_RETURN(ContentType) }
	type* Instantiate() const;
protected:
	std::vector<std::pair<ushort*, ulong*> > MaterialData;
	ushort* ContentType;
};

template <class type> ushort* contentscript<type>::GetMaterialType(ushort Index, bool AOE) const
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

template <class type> ulong* contentscript<type>::GetMaterialVolume(ushort Index, bool AOE) const
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

class squarescript : public script
{
public:
	squarescript() : PosScript(0), Character(0), Item(0), GroundTerrain(0), OverTerrain(0), IsUpStairs(0), IsDownStairs(0), IsWorldMapEntry(0), Times(0) {}
	void ReadFrom(inputfile&);
	posscript* GetPosScript(bool AOE = true) const { SCRIPT_RETURN(PosScript) }
	contentscript<character>* GetCharacter(bool AOE = true) const { SCRIPT_RETURN(Character) }
	contentscript<item>* GetItem(bool AOE = true) const { SCRIPT_RETURN(Item) }
	contentscript<groundlevelterrain>* GetGroundTerrain(bool AOE = true) const { SCRIPT_RETURN(GroundTerrain) }
	contentscript<overlevelterrain>* GetOverTerrain(bool AOE = true) const { SCRIPT_RETURN(OverTerrain) }
	bool* GetIsUpStairs(bool AOE = true) const { SCRIPT_RETURN(IsUpStairs) }
	bool* GetIsDownStairs(bool AOE = true) const { SCRIPT_RETURN(IsDownStairs) }
	bool* GetIsWorldMapEntry(bool AOE = true) const { SCRIPT_RETURN(IsWorldMapEntry) }
	uchar* GetTimes(bool AOE = true) const { SCRIPT_RETURN(Times) }
protected:
	posscript* PosScript;
	contentscript<character>* Character;
	contentscript<item>* Item;
	contentscript<groundlevelterrain>* GroundTerrain;
	contentscript<overlevelterrain>* OverTerrain;
	bool* IsUpStairs;
	bool* IsDownStairs;
	bool* IsWorldMapEntry;
	uchar* Times;
};

template <class type> class contentmap : public script
{
public:
	contentmap() : ContentScriptMap(0), Size(0), Pos(0) {}
	void ReadFrom(inputfile&);
	contentscript<type>* GetContentScript(ushort X, ushort Y) { return ContentScriptMap[X][Y]; }
	vector2d* GetSize(bool AOE = true) const { SCRIPT_RETURN(Size) }
	vector2d* GetPos(bool AOE = true) const { SCRIPT_RETURN(Pos) }
protected:
	contentscript<type>*** ContentScriptMap;
	vector2d* Size;
	vector2d* Pos;
};

class roomscript : public script
{
public:
	roomscript() : CharacterMap(0), ItemMap(0), GroundTerrainMap(0), OverTerrainMap(0), WallSquare(0), FloorSquare(0), DoorSquare(0), Size(0), Pos(0), AltarPossible(0), GenerateDoor(0), ReCalculate(0), GenerateTunnel(0), DivineOwner(0), GenerateLamps(0), Type(0), Base(0) {}
	void ReadFrom(inputfile&, bool = false);
	void SetBase(roomscript* What) { Base = What; }
	std::vector<squarescript*>& GetSquare() { return Square; }
	contentmap<character>* GetCharacterMap(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(CharacterMap) }
	contentmap<item>* GetItemMap(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(ItemMap) }
	contentmap<groundlevelterrain>* GetGroundTerrainMap(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(GroundTerrainMap) }
	contentmap<overlevelterrain>* GetOverTerrainMap(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(OverTerrainMap) }
	squarescript* GetWallSquare(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(WallSquare) }
	squarescript* GetFloorSquare(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(FloorSquare) }
	squarescript* GetDoorSquare(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(DoorSquare) }
	vector2d* GetSize(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Size) }
	vector2d* GetPos(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Pos) }
	bool* GetAltarPossible(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(AltarPossible) }
	bool* GetGenerateDoor(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(GenerateDoor) }
	bool* GetReCalculate(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(ReCalculate) }
	bool* GetGenerateTunnel(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(GenerateTunnel) }
	uchar* GetDivineOwner(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(DivineOwner) }
	bool* GetGenerateLamps(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(GenerateLamps) }
	ushort* GetType(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Type) }
protected:
	ulong BufferPos;
	std::vector<squarescript*> Square;
	contentmap<character>* CharacterMap;
	contentmap<item>* ItemMap;
	contentmap<groundlevelterrain>* GroundTerrainMap;
	contentmap<overlevelterrain>* OverTerrainMap;
	squarescript* WallSquare;
	squarescript* FloorSquare;
	squarescript* DoorSquare;
	vector2d* Size;
	vector2d* Pos;
	bool* AltarPossible, * GenerateDoor, * ReCalculate, * GenerateTunnel;
	uchar* DivineOwner;
	roomscript* Base;
	bool* GenerateLamps;
	ushort* Type;
};

class levelscript : public script
{
public:
	levelscript() : RoomDefault(0), FillSquare(0), LevelMessage(0), Size(0), Items(0), Rooms(0), OnGround(0), GenerateMonsters(0), ReCalculate(0), GenerateUpStairs(0), GenerateDownStairs(0), TeamDefault(0), AmbientLight(0), Description(0), LOSModifier(0), Base(0) {}
	void ReadFrom(inputfile&, bool = false);
	void SetBase(levelscript* What) { Base = What; }
	std::vector<squarescript*>& GetSquare() { return Square; }
	std::map<uchar, roomscript*>& GetRoom() { return Room; }
	roomscript* GetRoomDefault(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(RoomDefault) }
	squarescript* GetFillSquare(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(FillSquare) }
	std::string* GetLevelMessage(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(LevelMessage) }
	vector2d* GetSize(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Size) }
	ushort* GetItems(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Items) }
	uchar* GetRooms(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Rooms) }
	bool* GetGenerateMonsters(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(GenerateMonsters) }
	bool* GetReCalculate(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(ReCalculate) }
	bool* GetGenerateUpStairs(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(GenerateUpStairs) }
	bool* GetGenerateDownStairs(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(GenerateDownStairs) }
	bool* GetOnGround(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(OnGround) }
	uchar* GetTeamDefault(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(TeamDefault) }
	ushort* GetAmbientLight(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(AmbientLight) }
	std::string* GetDescription(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Description) }
	uchar* GetLOSModifier(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(LOSModifier) }
protected:
	ulong BufferPos;
	std::vector<squarescript*> Square;
	std::map<uchar, roomscript*> Room;
	roomscript* RoomDefault;
	squarescript* FillSquare;
	std::string* LevelMessage;
	vector2d* Size;
	ushort* Items;
	uchar* Rooms;
	bool* GenerateMonsters, * ReCalculate;
	bool* GenerateUpStairs, * GenerateDownStairs, * OnGround;
	uchar* TeamDefault;
	ushort* AmbientLight;
	std::string* Description;
	uchar* LOSModifier;
	levelscript* Base;
};

class dungeonscript : public script
{
public:
	dungeonscript() : LevelDefault(0), Levels(0), Base(0) {}
	void ReadFrom(inputfile&);
	void SetBase(dungeonscript* What) { Base = What; }
	std::map<uchar, levelscript*>& GetLevel() { return Level; }
	levelscript* GetLevelDefault(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(LevelDefault) }
	uchar* GetLevels(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Levels) }
protected:
	std::map<uchar, levelscript*> Level;
	levelscript* LevelDefault;
	uchar* Levels;
	dungeonscript* Base;
};

class teamscript : public script
{
public:
	void ReadFrom(inputfile&);
	std::vector<std::pair<uchar, uchar> >& GetRelation() { return Relation; }
protected:
	std::vector<std::pair<uchar, uchar> > Relation;
};

class gamescript : public script
{
public:
	gamescript() : DungeonDefault(0), Dungeons(0), Teams(0) {}
	void ReadFrom(inputfile&);
	std::map<uchar, dungeonscript*>& GetDungeon() { return Dungeon; }
	dungeonscript* GetDungeonDefault(bool AOE = true) const { SCRIPT_RETURN(DungeonDefault) }
	uchar* GetDungeons(bool AOE = true) const { SCRIPT_RETURN(Dungeons) }
	uchar* GetTeams(bool AOE = true) const { SCRIPT_RETURN(Teams) }
	std::vector<std::pair<uchar, teamscript*> >& GetTeam() { return Team; }
protected:
	std::map<uchar, dungeonscript*> Dungeon;
	dungeonscript* DungeonDefault;
	uchar* Dungeons;
	uchar* Teams;
	std::vector<std::pair<uchar, teamscript*> > Team;
};

#endif
