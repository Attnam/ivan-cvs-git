#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#include <deque>
#include <string>

#include "typedef.h"

#include "area.h"
#include "wterrain.h"
#include "cont.h"

class worldmapsquare;

class worldmap : public area
{
public:
	worldmap(ushort, ushort);
	worldmap(void) {}
	virtual ~worldmap(void);
	virtual void Generate(void);
	virtual void Draw(void) const;
	virtual void Save(std::ofstream&) const;
	virtual worldmapsquare* GetWorldMapSquare(vector Pos) const {return Map[Pos.X][Pos.Y];}
	virtual void GenerateClimate(void);
	virtual void Load(std::ifstream&);
	virtual ushort WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(ushort, ushort);
	virtual void CalculateContinents(void);
	//virtual void GenerateClimate(void);
	virtual void SmoothAltitude(void);
	virtual void SmoothClimate(void);
	virtual void RandomizeAltitude(void);
	virtual uchar GetContinentUnder(vector Pos) const { return ContinentBuffer[Pos.X][Pos.Y]; }
	virtual void RemoveEmptyContinents(void);
protected:
	worldmapsquare*** Map;
	//std::vector<char*> ContinentName;
	//uchar Continents;
	std::vector<continent> Continent;
	ushort** TypeBuffer, ** OldTypeBuffer;
	short** AltitudeBuffer;
	uchar** ContinentBuffer;
	//friend class worldmapterrain;
};

#endif
