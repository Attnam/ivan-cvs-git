#ifndef __AREA_H__
#define __AREA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "femath.h"

class character;
class square;
class bitmap;
class outputfile;
class inputfile;

class area
{
 public:
  area(ushort, ushort);
  area() { }
  virtual ~area();
  virtual void Draw() const = 0;
  void AddCharacter(vector2d, character*);
  void RemoveCharacter(vector2d);
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
  virtual void MoveCharacter(vector2d, vector2d);
  vector2d GetNearestFreeSquare(character*, vector2d);
  vector2d FreeSquareSeeker(character*, vector2d, vector2d, uchar);
  virtual ushort GetLOSModifier() const { return 16; }
  square* GetNeighbourSquare(vector2d, ushort) const;
  bool IsValidPos(vector2d Pos) const { return Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize; }
  const rect& GetBorder() const { return Border; }
 protected:
  square*** Map;
  ushort** FlagMap;
  ushort XSize, YSize;
  ulong XSizeTimesYSize;
  rect Border;
};

#endif


