#ifndef __WSQUARE_H__
#define __WSQUARE_H__

#include "typedef.h"
#include "vector2d.h"

#include "square.h"

class area;
class material;
class worldmap;
class groundworldmapterrain;
class overworldmapterrain;
class outputfile;
class inputfile;

class worldmapsquare : public square
{
public:
	worldmapsquare(worldmap*, vector2d);
	virtual ~worldmapsquare();
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void DrawToTileBuffer() const;
	virtual void UpdateMemorizedAndDraw();
	virtual void SetGroundWorldMapTerrain(groundworldmapterrain*);
	virtual void SetOverWorldMapTerrain(overworldmapterrain*);
	virtual groundworldmapterrain* GetGroundWorldMapTerrain() const { return GroundWorldMapTerrain; }
	virtual overworldmapterrain* GetOverWorldMapTerrain() const { return OverWorldMapTerrain; }
	virtual void ChangeWorldMapTerrain(groundworldmapterrain*, overworldmapterrain*);
	virtual worldmap* GetWorldMapUnder() const { return (worldmap*)AreaUnder; }
	virtual void SetWorldMapUnder(worldmap* What) { AreaUnder = (area*)What; }
	virtual void UpdateMemorizedDescription();
	virtual groundterrain* GetGroundTerrain() const;
	virtual overterrain* GetOverTerrain() const;
	virtual void DrawCheat();
	virtual void ChangeGroundWorldMapTerrain(groundworldmapterrain*);
	virtual void ChangeOverWorldMapTerrain(overworldmapterrain*);
	virtual void SetDescriptionChanged(bool What) { DescriptionChanged = What; }
protected:
	groundworldmapterrain* GroundWorldMapTerrain;
	overworldmapterrain* OverWorldMapTerrain;
	bool DescriptionChanged;
};

#endif
