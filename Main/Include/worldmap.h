#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#include <deque>
#include <string>

#include "typedef.h"

#include "area.h"
#include "wterraba.h"
#include "cont.h"

class worldmapsquare;
class outputfile;
class inputfile;
class character;

class worldmap : public area
{
public:
	worldmap(ushort, ushort);
	worldmap() {}
	virtual ~worldmap();
	virtual void Generate();
	virtual void Draw() const;
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual worldmapsquare* GetWorldMapSquare(vector2d Pos) const {return Map[Pos.X][Pos.Y];}
	virtual worldmapsquare* GetWorldMapSquare(ushort x, ushort y) const {return Map[x][y];}
	virtual void GenerateClimate();
	virtual ushort WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(ushort, ushort);
	virtual void CalculateContinents();
	virtual void SmoothAltitude();
	virtual void SmoothClimate();
	virtual void RandomizeAltitude();
	virtual continent* GetContinentUnder(vector2d Pos) const { return Continent[ContinentBuffer[Pos.X][Pos.Y]]; }
	virtual continent* GetContinent(ushort Index) const { return Continent[Index]; }
	virtual void RemoveEmptyContinents();
	virtual short GetAltitude(vector2d Pos) { return AltitudeBuffer[Pos.X][Pos.Y]; }
	virtual std::vector<character*>* GetPlayerGroup() { return &PlayerGroup; }
	virtual character* GetPlayerGroupMember(uchar c) { return PlayerGroup[c]; }
protected:
	worldmapsquare*** Map;
	std::vector<continent*> Continent;
	ushort** TypeBuffer, ** OldTypeBuffer;
	short** AltitudeBuffer;
	uchar** ContinentBuffer;
	std::vector<character*> PlayerGroup;
};

inline outputfile& operator<<(outputfile& SaveFile, worldmap* WorldMap)
{
	if(WorldMap)
	{
		SaveFile.GetBuffer().put(1);
		WorldMap->Save(SaveFile);
	}
	else
		SaveFile.GetBuffer().put(0);

	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, worldmap*& WorldMap)
{
	if(SaveFile.GetBuffer().get())
	{
		WorldMap = new worldmap;
		WorldMap->Load(SaveFile);
	}

	return SaveFile;
}

#endif
