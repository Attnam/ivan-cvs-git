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
  virtual void Be();
  virtual bool HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsVeryCloseToSpoiling() const { return GetSpoilModifier() - SpoilCounter <= 10; }
 protected:
  virtual void VirtualConstructor(bool) { SpoilCounter = 0; }
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
  virtual std::string GetConsumeVerb() const { return "drinking"; }
  virtual bool IsLiquid() const { return true; }
);

class MATERIAL
(
  flesh,
  organicsubstance,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetSkinColor() const { return SkinColor; }
  virtual void SetSkinColor(ushort What) { SkinColor = What; }
  virtual bool IsFlesh() const { return true; }
 protected:
  ushort SkinColor;
);

#endif

