#ifndef __FLUID_H__
#define __FLUID_H__

#include "entity.h"

class outputfile;
class inputfile;
class material;
class bitmap;

class fluid : public entity
{
 public:
  fluid();
  virtual ~fluid();
  void SpillFluid(uchar, ulong, ushort = 5, ushort = 32);
  virtual void Be();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void DrawToTileBuffer(bool) const;
  ushort GetEmitation() const;
  bitmap* GetPicture() const { return Picture; }
  material* GetMaterial() const { return Material; }
 protected:
  bitmap* Picture;
  material* Material;
};

outputfile& operator<<(outputfile&, fluid*);
inputfile& operator>>(inputfile&, fluid*&);

#endif
