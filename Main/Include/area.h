#ifndef __AREA_H__
#define __AREA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <map>

#include "rect.h"

class character;
class square;
class bitmap;
class outputfile;
class inputfile;

class area
{
 public:
  area();
  area(ushort, ushort);
  virtual ~area();
  virtual void Draw(bool) const = 0;
  void Save(outputfile&) const;
  void Load(inputfile&);
  ushort GetFlag(vector2d Pos) const { return FlagMap[Pos.X][Pos.Y]; }
  square* GetSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  square* GetSquare(ushort x, ushort y) const { return Map[x][y]; }
  ushort GetXSize() const { return XSize; }
  ushort GetYSize() const { return YSize; }
  void UpdateLOS();
  void SendNewDrawRequest();
  void Initialize(ushort, ushort);
  square* GetNeighbourSquare(vector2d, ushort) const;
  bool IsValidPos(vector2d Pos) const { return Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize; }
  bool IsValidPos(short X, short Y) const { return X >= 0 && Y >= 0 && X < XSize && Y < YSize; }
  const rect& GetBorder() const { return Border; }
  void SetEntryPos(uchar, vector2d);
 protected:
  square*** Map;
  ushort** FlagMap;
  ushort XSize, YSize;
  ulong XSizeTimesYSize;
  rect Border;
  std::map<uchar, vector2d> EntryMap;
};

#endif
