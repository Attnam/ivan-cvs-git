#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#pragma warning(disable : 4786)

#include <string>
#include <vector>
#include <map>

#include "typedef.h"
#include "vector2d.h"

class inputfile;

struct posscript
{
	posscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	vector2d Vector;
	bool IsWalkable;
	bool Random;
};

struct groundterrainscript
{
	groundterrainscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	ushort MaterialType;
	ushort TerrainType;
};

struct overterrainscript
{
	overterrainscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	ushort MaterialType;
	ushort TerrainType;
};

struct characterscript
{
	characterscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	ushort MaterialType;
	ushort CharacterType;
};

struct squarescript
{
	squarescript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	posscript PosScript;
	groundterrainscript GroundTerrain;
	overterrainscript OverTerrain;
	characterscript Character;
};

struct roomscript
{
	roomscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	std::vector<squarescript> Square;
	squarescript WallSquare;
	squarescript FloorSquare;
	squarescript DoorSquare;
	vector2d Size;
	bool AltarPossible, GenerateDoor, ReCalculate;
};

struct levelscript
{
	levelscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	std::vector<squarescript> Square;
	std::map<uchar, roomscript> Room;
	roomscript RoomDefault;
	squarescript FillSquare;
	std::string LevelMessage;
	vector2d Size;
	ushort Items;
	uchar Rooms;
	bool GenerateMonsters, ReCalculate;
};

struct dungeonscript
{
	dungeonscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	std::map<uchar, levelscript> Level;
	levelscript LevelDefault;
	uchar Levels;
};

struct gamescript
{
	gamescript() : OverrideMap(0) {}
	void ReadFrom(inputfile&, std::map<std::string, long>);
	uchar OverrideMap;
	std::map<uchar, dungeonscript> Dungeon;
	dungeonscript DefaultDungeon;
	uchar Dungeons;
};

#endif
