#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "area.h"

class wsquare;
class continent;

class worldmap : public area
{
 public:
  worldmap(ushort, ushort);
  worldmap();
  virtual ~worldmap();
  void Generate();
  void Save(outputfile&) const;
  void Load(inputfile&);
  wsquare* GetWSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  wsquare* GetWSquare(ushort x, ushort y) const { return Map[x][y]; }
  void GenerateClimate();
  ushort WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(ushort, ushort);
  void CalculateContinents();
  void SmoothAltitude();
  void SmoothClimate();
  void RandomizeAltitude();
  continent* GetContinentUnder(vector2d) const;
  continent* GetContinent(ushort Index) const { return Continent[Index]; }
  void RemoveEmptyContinents();
  short GetAltitude(vector2d Pos) { return AltitudeBuffer[Pos.X][Pos.Y]; }
  std::vector<character*>& GetPlayerGroup() { return PlayerGroup; }
  character* GetPlayerGroupMember(uchar c) { return PlayerGroup[c]; }
  virtual void Draw(bool) const;
  void CalculateLuminances();
  void CalculateNeighbourBitmapPoses();
  wsquare* GetNeighbourWSquare(vector2d, ushort) const;
  vector2d GetEntryPos(const character*, uchar) const;
  void RevealEnvironment(vector2d, ushort);
  void SafeSmooth(ushort, ushort);
  void FastSmooth(ushort, ushort);
  wsquare*** GetMap() const { return Map; }
 protected:
  wsquare*** Map;
  std::vector<continent*> Continent;
  ushort** TypeBuffer;
  ushort** OldTypeBuffer;
  short** AltitudeBuffer;
  short** OldAltitudeBuffer;
  uchar** ContinentBuffer;
  std::vector<character*> PlayerGroup;
};

outputfile& operator<<(outputfile&, const worldmap*);
inputfile& operator>>(inputfile&, worldmap*&);

#endif
