#ifndef __WSQUARE_H__
#define __WSQUARE_H__

#include "typedef.h"
#include "vector.h"

#include "square.h"

class area;
class material;
class worldmap;
class groundworldmapterrain;
class overworldmapterrain;

class worldmapsquare : public square
{
public:
	worldmapsquare(worldmap*, vector);
	~worldmapsquare(void);
	//virtual void Save(std::ofstream*) const;
	//virtual void Load(std::ifstream*);
	virtual void DrawToTileBuffer(void) const;
	virtual void UpdateMemorizedAndDraw(void);
	virtual void SetGroundWorldMapTerrain(groundworldmapterrain* What) { GroundWorldMapTerrain = What; }
	virtual void SetOverWorldMapTerrain(overworldmapterrain* What) { OverWorldMapTerrain = What; }
	virtual groundworldmapterrain* GetGroundWorldMapTerrain(void) const { return GroundWorldMapTerrain; }
	virtual overworldmapterrain* GetOverWorldMapTerrain(void) const { return OverWorldMapTerrain; }
	virtual void ChangeWorldMapTerrain(groundworldmapterrain*, overworldmapterrain*);
protected:
	worldmap* MotherWorldMap;
	groundworldmapterrain* GroundWorldMapTerrain;
	overworldmapterrain* OverWorldMapTerrain;
};

#endif
