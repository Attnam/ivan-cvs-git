#ifndef __AREA_H__
#define __AREA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define FORBIDDEN		1
#define ON_POSSIBLE_ROUTE	2
#define STILL_ON_POSSIBLE_ROUTE	4
#define PREFERRED		8

#include "typedef.h"
#include "vector2d.h"

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
  virtual void Draw() const;
  virtual void AddCharacter(vector2d, character*);
  virtual void RemoveCharacter(vector2d);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetFlag(vector2d Pos) const { return FlagMap[Pos.X][Pos.Y]; }
  virtual square* operator [] (vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  virtual square* GetSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  virtual square* GetSquare(ushort x, ushort y) const { return Map[x][y]; }
  virtual ushort GetXSize() const { return XSize; }
  virtual ushort GetYSize() const { return YSize; }
  virtual void UpdateLOS();
  virtual void SendNewDrawRequest();
  virtual void Initialize(ushort, ushort);
  virtual void MoveCharacter(vector2d, vector2d);
  virtual vector2d GetNearestFreeSquare(character*, vector2d);
  virtual vector2d FreeSquareSeeker(character*, vector2d, vector2d, uchar);
  virtual ushort GetLOSModifier() const { return 16; }
 protected:
  square*** Map;
  ushort** FlagMap;
  ushort XSize, YSize;
  ulong XSizeTimesYSize;
};

inline outputfile& operator<<(outputfile& SaveFile, area* Area)
{
  Area->Save(SaveFile);
  return SaveFile;
}

#endif


