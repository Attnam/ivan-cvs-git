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
  continent(ushort);
  void AttachTo(continent*);
  void Add(vector2d);
  void Save(outputfile&) const;
  void Load(inputfile&);
  ulong GetSize() const;
  uchar GetIndex() const { return Index; }
  void GenerateInfo();
  festring GetName() const { return Name; }
  ushort GetGTerrainAmount(ushort) const;
  vector2d GetRandomMember(ushort);
  vector2d GetMember(ushort) const;
 private:
  static ushort** TypeBuffer;
  static short** AltitudeBuffer;
  static uchar** ContinentBuffer;
  festring Name;
  std::vector<vector2d> Member;
  std::vector<long> GTerrainAmount;
  uchar Index;
};

outputfile& operator<<(outputfile&, const continent*);
inputfile& operator>>(inputfile&, continent*&);

#endif
