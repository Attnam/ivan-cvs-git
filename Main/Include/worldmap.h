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

class worldmap : public area
{
public:
	worldmap(ushort, ushort);
	worldmap() {}
	virtual ~worldmap();
	virtual void Generate();
	virtual void Draw() const;
	virtual void Save(outputfile&) const;
	virtual worldmapsquare* GetWorldMapSquare(vector2d Pos) const {return Map[Pos.X][Pos.Y];}
	virtual worldmapsquare* GetWorldMapSquare(ushort x, ushort y) const {return Map[x][y];}
	virtual void GenerateClimate();
	virtual void Load(inputfile&);
	virtual ushort WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(ushort, ushort);
	virtual void CalculateContinents();
	virtual void SmoothAltitude();
	virtual void SmoothClimate();
	virtual void RandomizeAltitude();
	virtual continent* GetContinentUnder(vector2d Pos) const { return Continent[ContinentBuffer[Pos.X][Pos.Y]]; }
	virtual void RemoveEmptyContinents();
	virtual short GetAltitude(vector2d Pos) { return AltitudeBuffer[Pos.X][Pos.Y]; }
protected:
	worldmapsquare*** Map;
	std::vector<continent*> Continent;
	ushort** TypeBuffer, ** OldTypeBuffer;
	short** AltitudeBuffer;
	uchar** ContinentBuffer;
};

#endif
