#ifndef __FLUID_H__
#define __FLUID_H__

#include "object.h"

class fluid : public entity
{
 public:
  fluid();
  virtual ~fluid();
  virtual void SpillFluid(uchar, ulong, ushort = 5, ushort = 32);
  virtual void Be();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void DrawToTileBuffer() const;
};

inline outputfile& operator<<(outputfile& SaveFile, fluid* Fluid)
{
  if(Fluid)
    {
      SaveFile.Put(1);
      Fluid->Save(SaveFile);
    }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, fluid*& Fluid)
{
  if(SaveFile.Get())
    {
      Fluid = new fluid;
      Fluid->Load(SaveFile);
    }

  return SaveFile;
}

#endif
