#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#include "area.h"
#include "wterrain.h"
class worldmapsquare;

class worldmap : public area
{
public:
	worldmap(ushort, ushort);
	worldmap(void) {}
	~worldmap(void);
	virtual void Generate(void);
	virtual void Draw(void) const;
	virtual void Save(std::ofstream*) const;
	virtual worldmapsquare* GetWorldMapSquare(vector Pos) const {return Map[Pos.X][Pos.Y];}
	virtual void GenerateTerrain(void);
	virtual void Load(std::ifstream*);
//	virtual groundworldmapterrain* WhatTerrainIsMostCommonNear(ushort, ushort);
protected:
	worldmapsquare*** Map;
	friend class worldmapterrain;
};

#endif
