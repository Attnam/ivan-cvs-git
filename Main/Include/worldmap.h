#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "typedef.h"
#include "area.h"
#include "cont.h"

class wsquare;
class outputfile;
class inputfile;
class character;
class continent;

class worldmap : public area
{
 public:
  worldmap(ushort, ushort);
  worldmap() { }
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
  continent* GetContinentUnder(vector2d Pos) const { return Continent[ContinentBuffer[Pos.X][Pos.Y]]; }
  continent* GetContinent(ushort Index) const { return Continent[Index]; }
  void RemoveEmptyContinents();
  short GetAltitude(vector2d Pos) { return AltitudeBuffer[Pos.X][Pos.Y]; }
  std::vector<character*>& GetPlayerGroup() { return PlayerGroup; }
  character* GetPlayerGroupMember(uchar c) { return PlayerGroup[c]; }
  virtual void Draw() const;
  void CalculateLuminances();
  void CalculateNeighbourBitmapPoses();
  wsquare* GetNeighbourWSquare(vector2d, ushort) const;
  vector2d GetEntryPos(const character*, uchar Index) const { return EntryMap.find(Index)->second; }
 protected:
  wsquare*** Map;
  std::vector<continent*> Continent;
  ushort** TypeBuffer, ** OldTypeBuffer;
  short** AltitudeBuffer, ** OldAltitudeBuffer;
  uchar** ContinentBuffer;
  std::vector<character*> PlayerGroup;
};

inline outputfile& operator<<(outputfile& SaveFile, worldmap* WorldMap)
{
  WorldMap->Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, worldmap*& WorldMap)
{
  WorldMap = new worldmap;
  WorldMap->Load(SaveFile);
  return SaveFile;
}

#endif
