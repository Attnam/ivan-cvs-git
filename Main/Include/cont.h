/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

#ifndef __CONT_H__
#define __CONT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "vector2d.h"

class outputfile;
class inputfile;
class festring;

class continent
{
 public:
  friend class worldmap;
  continent();
  continent(int);
  void AttachTo(continent*);
  void Add(vector2d);
  void Save(outputfile&) const;
  void Load(inputfile&);
  long GetSize() const;
  int GetIndex() const { return Index; }
  void GenerateInfo();
  festring GetName() const { return Name; }
  int GetGTerrainAmount(int) const;
  vector2d GetRandomMember(int);
  vector2d GetMember(int) const;
 private:
  static uchar** TypeBuffer;
  static short** AltitudeBuffer;
  static uchar** ContinentBuffer;
  festring Name;
  std::vector<vector2d> Member;
  std::vector<long> GTerrainAmount;
  int Index;
};

outputfile& operator<<(outputfile&, const continent*);
inputfile& operator>>(inputfile&, continent*&);

#endif
