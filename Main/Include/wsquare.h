#ifndef __WSQUARE_H__
#define __WSQUARE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "vector2d.h"

#include "square.h"

class area;
class material;
class worldmap;
class gwterrain;
class owterrain;
class outputfile;
class inputfile;

class wsquare : public square
{
 public:
  wsquare(worldmap*, vector2d);
  virtual ~wsquare();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool DrawTerrain() const;
  virtual bool DrawCharacters() const;
  virtual void Draw();
  virtual void UpdateMemorized();
  virtual void SetGWTerrain(gwterrain*);
  virtual void SetOWTerrain(owterrain*);
  virtual gwterrain* GetGWTerrain() const { return GWTerrain; }
  virtual owterrain* GetOWTerrain() const { return OWTerrain; }
  virtual void ChangeWTerrain(gwterrain*, owterrain*);
  virtual worldmap* GetWorldMapUnder() const { return (worldmap*)AreaUnder; }
  virtual void SetWorldMapUnder(worldmap* What) { AreaUnder = (area*)What; }
  virtual void UpdateMemorizedDescription(bool = false);
  virtual gterrain* GetGTerrain() const;
  virtual oterrain* GetOTerrain() const;
  virtual void ChangeGWTerrain(gwterrain*);
  virtual void ChangeOWTerrain(owterrain*);
 protected:
  gwterrain* GWTerrain;
  owterrain* OWTerrain;
};

#endif

