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
  virtual void SpillFluid(uchar, ulong, ushort = 5, ushort = 32);
  virtual void Be();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void DrawToTileBuffer(bool) const;
  virtual ushort GetEmitation() const;
  virtual bitmap* GetPicture() const { return Picture; }
  virtual material* GetMaterial() const { return Material; }
 protected:
  bitmap* Picture;
  material* Material;
};

outputfile& operator<<(outputfile&, fluid*);
inputfile& operator>>(inputfile&, fluid*&);

#endif
