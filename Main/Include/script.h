#ifndef __SCRIPT_H__
#define __SCRIPT_H__

/*#include <string>

#include "typedef.h"
#include "vector2d.h"

class inputfile;

struct groundterrainscript
{
	groundterrainscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&);
	uchar OverrideMap;
	ushort MaterialType;
	ushort TerrainType;
};

struct overterrainscript
{
	overterrainscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&);
	uchar OverrideMap;
	ushort MaterialType;
	ushort TerrainType;
};

struct characterscript
{
	characterscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&);
	uchar OverrideMap;
	ushort MaterialType;
	ushort CharacterType;
};

struct squarescript
{
	squarescript() : OverrideMap(0) {}
	void ReadFrom(inputfile&);
	uchar OverrideMap;
	groundterrainscript GroundTerrain;
	overterrainscript OverTerrain;
	characterscript Character;
};

struct roomscript
{
	roomscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&);
	uchar OverrideMap;
	std::map<vector2d, roomscript> Square;
	terrainscript WallTerrain;
	terrainscript FloorTerrain;
	terrainscript DoorTerrain;
	vector2d Size;
	bool AltarPossible, GenerateDoor;
};

struct levelscript
{
	levelscript() : OverrideMap(0) {}
	void ReadFrom(inputfile&);
	uchar OverrideMap;
	std::map<uchar, roomscript> Room;
	roomscript DefaultRoom;
	terrainscript FillTerrain;
	std::string LevelMessage;
	vector2d Size;
	ushort Items;
	uchar Rooms;
	bool GenerateMonsters;
};*/

#endif
