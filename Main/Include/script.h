#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#pragma warning(disable : 4786)

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
	posscript() : Vector(0), IsWalkable(0) {}
	void ReadFrom(inputfile&);
	vector2d* GetVector(bool AOE = true) const { SCRIPT_RETURN(Vector) }
	bool* GetIsWalkable(bool AOE = true) const { SCRIPT_RETURN(IsWalkable) }
	bool GetRandom() const { return Random; }
protected:
	vector2d* Vector;
	bool* IsWalkable;
	bool Random;
};

class groundterrainscript : public script
{
public:
	groundterrainscript() : MaterialType(0), TerrainType(0) {}
	void ReadFrom(inputfile&);
	ushort* GetMaterialType(bool AOE = true) const { SCRIPT_RETURN(MaterialType) }
	ushort* GetTerrainType(bool AOE = true) const { SCRIPT_RETURN(TerrainType) }
	groundlevelterrain* Instantiate() const;
protected:
	ushort* MaterialType;
	ushort* TerrainType;
};

class overterrainscript : public script
{
public:
	overterrainscript() : MaterialType(0), TerrainType(0) {}
	void ReadFrom(inputfile&);
	ushort* GetMaterialType(bool AOE = true) const { SCRIPT_RETURN(MaterialType) }
	ushort* GetTerrainType(bool AOE = true) const { SCRIPT_RETURN(TerrainType) }
	overlevelterrain* Instantiate() const;
protected:
	ushort* MaterialType;
	ushort* TerrainType;
};

class characterscript : public script
{
public:
	characterscript() : MaterialType(0), CharacterType(0) {}
	void ReadFrom(inputfile&);
	ushort* GetMaterialType(bool AOE = true) const { SCRIPT_RETURN(MaterialType) }
	ushort* GetCharacterType(bool AOE = true) const { SCRIPT_RETURN(CharacterType) }
	character* Instantiate() const;
protected:
	ushort* MaterialType;
	ushort* CharacterType;
};

class squarescript : public script
{
public:
	squarescript() : PosScript(0), GroundTerrain(0), OverTerrain(0), Character(0) {}
	void ReadFrom(inputfile&);
	posscript* GetPosScript(bool AOE = true) const { SCRIPT_RETURN(PosScript) }
	groundterrainscript* GetGroundTerrain(bool AOE = true) const { SCRIPT_RETURN(GroundTerrain) }
	overterrainscript* GetOverTerrain(bool AOE = true) const { SCRIPT_RETURN(OverTerrain) }
	characterscript* GetCharacter(bool AOE = true) const { SCRIPT_RETURN(Character) }
protected:
	posscript* PosScript;
	groundterrainscript* GroundTerrain;
	overterrainscript* OverTerrain;
	characterscript* Character;
};

class roomscript : public script
{
public:
	roomscript() : WallSquare(0), FloorSquare(0), DoorSquare(0), Size(0), Pos(0), AltarPossible(0), GenerateDoor(0), ReCalculate(0), Base(0) {}
	void ReadFrom(inputfile&, bool = false);
	void SetBase(roomscript* What) { Base = What; }
	std::vector<squarescript*>& GetSquare() { return Square; }
	squarescript* GetWallSquare(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(WallSquare) }
	squarescript* GetFloorSquare(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(FloorSquare) }
	squarescript* GetDoorSquare(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(DoorSquare) }
	vector2d* GetSize(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Size) }
	vector2d* GetPos(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(Pos) }
	bool* GetAltarPossible(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(AltarPossible) }
	bool* GetGenerateDoor(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(GenerateDoor) }
	bool* GetReCalculate(bool AOE = true) const { SCRIPT_RETURN_WITH_BASE(ReCalculate) }
protected:
	ulong BufferPos;
	std::vector<squarescript*> Square;
	squarescript* WallSquare;
	squarescript* FloorSquare;
	squarescript* DoorSquare;
	vector2d* Size;
	vector2d* Pos;
	bool* AltarPossible, * GenerateDoor, * ReCalculate;
	roomscript* Base;
};

class levelscript : public script
{
public:
	levelscript() : RoomDefault(0), FillSquare(0), LevelMessage(0), Size(0), Items(0), Rooms(0), GenerateMonsters(0), ReCalculate(0), GenerateUpStairs(0), GenerateDownStairs(0), Base(0) {}
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
	bool* GenerateUpStairs, * GenerateDownStairs;
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

class gamescript : public script
{
public:
	gamescript() : DungeonDefault(0), Dungeons(0) {}
	void ReadFrom(inputfile&);
	std::map<uchar, dungeonscript*>& GetDungeon() { return Dungeon; }
	dungeonscript* GetDungeonDefault(bool AOE = true) const { SCRIPT_RETURN(DungeonDefault) }
	uchar* GetDungeons(bool AOE = true) const { SCRIPT_RETURN(Dungeons) }
protected:
	std::map<uchar, dungeonscript*> Dungeon;
	dungeonscript* DungeonDefault;
	uchar* Dungeons;
};

#endif
