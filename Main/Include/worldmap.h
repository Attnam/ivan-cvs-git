#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "area.h"

class wsquare;
class continent;

typedef std::vector<character*> charactervector;

class worldmap : public area
{
 public:
  worldmap(int, int);
  worldmap();
  virtual ~worldmap();
  void Generate();
  void Save(outputfile&) const;
  void Load(inputfile&);
  wsquare* GetWSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  wsquare* GetWSquare(int x, int y) const { return Map[x][y]; }
  void GenerateClimate();
  int WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(int, int);
  void CalculateContinents();
  void SmoothAltitude();
  void SmoothClimate();
  void RandomizeAltitude();
  continent* GetContinentUnder(vector2d) const;
  continent* GetContinent(int) const;
  void RemoveEmptyContinents();
  int GetAltitude(vector2d);
  charactervector& GetPlayerGroup();
  character* GetPlayerGroupMember(int);
  virtual void Draw(bool) const;
  void CalculateLuminances();
  void CalculateNeighbourBitmapPoses();
  wsquare* GetNeighbourWSquare(vector2d, int) const;
  vector2d GetEntryPos(const character*, int) const;
  void RevealEnvironment(vector2d, int);
  void SafeSmooth(int, int);
  void FastSmooth(int, int);
  wsquare*** GetMap() const { return Map; }
  void UpdateLOS();
 protected:
  wsquare*** Map;
  std::vector<continent*> Continent;
  uchar** TypeBuffer;
  uchar** OldTypeBuffer;
  short** AltitudeBuffer;
  short** OldAltitudeBuffer;
  uchar** ContinentBuffer;
  charactervector PlayerGroup;
};

outputfile& operator<<(outputfile&, const worldmap*);
inputfile& operator>>(inputfile&, worldmap*&);

#endif
