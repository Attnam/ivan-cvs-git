#ifndef __MATERIAS_H__
#define __MATERIAS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "materia.h"

class MATERIAL
(
  organicsubstance,
  material,
 public:
  virtual void Be();
  virtual bool HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsVeryCloseToSpoiling() const { return SpoilLevel == 8; }
  virtual uchar GetSpoilLevel() const { return SpoilLevel; }
  virtual void ResetSpoiling();
  virtual void EatEffect(character*, ulong);
  virtual void AddConsumeEndMessage(character*) const;
  virtual void SetSpoilCounter(ushort);
 protected:
  virtual void VirtualConstructor(bool);
  ushort SpoilCounter;
  uchar SpoilLevel;
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
 public:
  virtual const char* GetConsumeVerb() const;
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
  virtual bool SkinColorIsSparkling() const { return SkinColorSparkling; }
  virtual void SetSkinColorIsSparkling(bool What) { SkinColorSparkling = What; }
  virtual bool IsFlesh() const { return true; }
 protected:
  ushort SkinColor;
  bool SkinColorSparkling;
);

class MATERIAL
(
  powder,
  material,
 public:
  virtual bool IsPowder() const { return true; }
  virtual bool IsWet() const { return Wetness != 0; }
  virtual bool IsExplosive() const;
  virtual void SetWetness(ulong What) { Wetness = What; }
  virtual void Be();
  virtual bool HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void VirtualConstructor(bool) { Wetness = 0; }
  ulong Wetness;
);

#endif
