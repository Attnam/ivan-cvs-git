/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

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
  area(int, int);
  virtual ~area();
  virtual void Draw(bool) const = 0;
  void Save(outputfile&) const;
  void Load(inputfile&);
  int GetFlag(vector2d Pos) const { return FlagMap[Pos.X][Pos.Y]; }
  square* GetSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  square* GetSquare(int x, int y) const { return Map[x][y]; }
  int GetXSize() const { return XSize; }
  int GetYSize() const { return YSize; }
  void SendNewDrawRequest();
  void Initialize(int, int);
  square* GetNeighbourSquare(vector2d, int) const;
  bool IsValidPos(vector2d Pos) const { return Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize; }
  bool IsValidPos(int X, int Y) const { return X >= 0 && Y >= 0 && X < XSize && Y < YSize; }
  const rect& GetBorder() const { return Border; }
  void SetEntryPos(int, vector2d);
 protected:
  square*** Map;
  uchar** FlagMap;
  int XSize, YSize;
  ulong XSizeTimesYSize;
  rect Border;
  std::map<int, vector2d> EntryMap;
};

#endif
