#ifndef __CONT_H__
#define __CONT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "vector2d.h"

class outputfile;
class inputfile;

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
  ulong GetSize() const { return Member.size(); }
  uchar GetIndex() const { return Index; }
  void GenerateInfo();
  std::string GetName() const { return Name; }
  ushort GetGTerrainAmount(ushort Type) const { return GTerrainAmount[Type]; }
  vector2d GetRandomMember(ushort);
  vector2d GetMember(ushort Index) const { return Member[Index]; }
 private:
  static ushort** TypeBuffer;
  static short** AltitudeBuffer;
  static uchar** ContinentBuffer;
  std::string Name;
  std::vector<vector2d> Member;
  std::vector<ushort> GTerrainAmount;
  uchar Index;
};

outputfile& operator<<(outputfile&, const continent*);
inputfile& operator>>(inputfile&, continent*&);

#endif
