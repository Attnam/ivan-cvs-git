#ifndef __MATERDE_H__
#define __MATERDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "materba.h"
#include "felibdef.h"

class MATERIAL
(
  organicsubstance,
  material,
 public:
  virtual bool Be();
  //virtual bool HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  ushort SpoilCounter;
);

class MATERIAL
(
  gas,
  material,
  ;
);

class MATERIAL
(
  liquid,
  material,
  ;
);

class MATERIAL
(
  flesh,
  organicsubstance,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual std::vector<ushort>& GetSkinColorVector() { return SkinColor; }
  virtual ushort GetSkinColor(ushort Frame) const { return SkinColor[Frame]; }
  virtual bool IsFlesh() const { return true; }
 protected:
  std::vector<ushort> SkinColor;
);

#endif
