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
  virtual void Generate();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual wsquare* GetWSquare(vector2d Pos) const {return Map[Pos.X][Pos.Y];}
  virtual wsquare* GetWSquare(ushort x, ushort y) const {return Map[x][y];}
  virtual void GenerateClimate();
  virtual ushort WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(ushort, ushort);
  virtual void CalculateContinents();
  virtual void SmoothAltitude();
  virtual void SmoothClimate();
  virtual void RandomizeAltitude();
  virtual continent* GetContinentUnder(vector2d Pos) const { return Continent[ContinentBuffer[Pos.X][Pos.Y]]; }
  virtual continent* GetContinent(ushort Index) const { return Continent[Index]; }
  virtual void RemoveEmptyContinents();
  virtual short GetAltitude(vector2d Pos) { return AltitudeBuffer[Pos.X][Pos.Y]; }
  virtual std::vector<character*>& GetPlayerGroup() { return PlayerGroup; }
  virtual character* GetPlayerGroupMember(uchar c) { return PlayerGroup[c]; }
 protected:
  wsquare*** Map;
  std::vector<continent*> Continent;
  ushort** TypeBuffer, ** OldTypeBuffer;
  short** AltitudeBuffer;
  uchar** ContinentBuffer;
  std::vector<character*> PlayerGroup;
};

outputfile& operator<<(outputfile&, worldmap*);
inputfile& operator>>(inputfile&, worldmap*&);

#endif


