#ifndef __FLUID_H__
#define __FLUID_H__

#include "vector2d.h"
#include "entity.h"
#include "lsquare.h"

class outputfile;
class inputfile;
class material;
class bitmap;

class fluid : public entity
{
 public:
  fluid() : entity(true) { }
  fluid(square*);
  virtual ~fluid();
  void SpillFluid(uchar, ulong, ushort = 5, ushort = 32);
  virtual void Be();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Draw(bitmap*, vector2d, ulong, bool, bool) const;
  bitmap* GetPicture() const { return Picture; }
  material* GetMaterial() const { return Material; }
  virtual square* GetSquareUnder() const { return SquareUnder; }
  void SetSquareUnder(square* What) { SquareUnder = What; }
  lsquare* GetLSquareUnder() const { return static_cast<lsquare*>(SquareUnder); }
  virtual bool IsOnGround() const { return true; }
 protected:
  bitmap* Picture;
  material* Material;
  square* SquareUnder;
};

outputfile& operator<<(outputfile&, fluid*);
inputfile& operator>>(inputfile&, fluid*&);

#endif
