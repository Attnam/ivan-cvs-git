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
	virtual ~worldmapsquare(void);
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual void DrawToTileBuffer(void) const;
	virtual void UpdateMemorizedAndDraw(void);
	virtual void SetGroundWorldMapTerrain(groundworldmapterrain*);
	virtual void SetOverWorldMapTerrain(overworldmapterrain*);
	virtual groundworldmapterrain* GetGroundWorldMapTerrain(void) const { return GroundWorldMapTerrain; }
	virtual overworldmapterrain* GetOverWorldMapTerrain(void) const { return OverWorldMapTerrain; }
	virtual void ChangeWorldMapTerrain(groundworldmapterrain*, overworldmapterrain*);
	//virtual short GetAltitude(void) { return Altitude; };
	//virtual void SetAltitude(short What) { Altitude = What; }
	virtual worldmap* GetWorldMapUnder(void) const { return (worldmap*)AreaUnder; }
	virtual void SetWorldMapUnder(worldmap* What) { AreaUnder = (area*)What; }
	virtual void UpdateMemorizedDescription(void);
	virtual groundterrain* GetGroundTerrain(void) const;// { return GroundWorldMapTerrain; }
	virtual overterrain* GetOverTerrain(void) const;// { return OverWorldMapTerrain; }
	virtual void DrawCheat(void) const;
protected:
	groundworldmapterrain* GroundWorldMapTerrain;
	overworldmapterrain* OverWorldMapTerrain;
	//worldmap* WorldMapUnder;
	//short Altitude;
	bool TerrainChanged;
};

#endif

