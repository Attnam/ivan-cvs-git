#ifndef __MATERIAS_H__
#define __MATERIAS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "materia.h"

class lterrain;

class MATERIAL
(
  solid,
  material,
  ;
);

class MATERIAL
(
  organic,
  solid,
 public:
  virtual void Be();
  virtual bool HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsVeryCloseToSpoiling() const { return SpoilLevel == 8; }
  virtual int GetSpoilLevel() const { return SpoilLevel; }
  virtual void ResetSpoiling();
  virtual material* EatEffect(character*, long);
  virtual void AddConsumeEndMessage(character*) const;
  virtual void SetSpoilCounter(int);
  virtual bool CanSpoil() const { return true; }
 protected:
  virtual void VirtualConstructor(bool);
  ushort SpoilCounter;
  uchar SpoilCheckCounter;
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
  void TouchEffect(item*, const festring&);
  void TouchEffect(character*, int);
  void TouchEffect(lterrain*);
  liquid* CloneLiquid(long Volume) const { return static_cast<liquid*>(Clone(Volume)); }
);

class MATERIAL
(
  flesh,
  organic,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual color16 GetSkinColor() const { return SkinColor; }
  virtual void SetSkinColor(int What) { SkinColor = What; }
  virtual bool SkinColorIsSparkling() const { return SkinColorSparkling; }
  virtual void SetSkinColorIsSparkling(bool What) { SkinColorSparkling = What; }
  virtual bool IsFlesh() const { return true; }
  virtual void SetIsInfectedByLeprosy(bool What) { InfectedByLeprosy = What; }
  virtual bool IsInfectedByLeprosy() const { return InfectedByLeprosy; }
 protected:
  virtual void VirtualConstructor(bool);
  color16 SkinColor;
  bool SkinColorSparkling;
  bool InfectedByLeprosy;
);

class MATERIAL
(
  powder,
  liquid,
 public:
  virtual bool IsPowder() const { return true; }
  virtual bool IsExplosive() const;
  virtual void AddWetness(long What) { Wetness += What; }
  virtual void Be();
  virtual bool HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void VirtualConstructor(bool) { Wetness = 0; }
  long Wetness;
);

/* Materials that can rust */

class MATERIAL
(
  ironalloy,
  solid,
 public:
  //virtual void AddName(festring&, bool = false, bool = true) const;
  virtual void SetRustLevel(int);
  virtual int GetStrengthValue() const;
  virtual int GetRustLevel() const { return RustData & 3; }
  virtual bool IsSparkling() const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetRustData() const { return RustData; }
  virtual bool TryToRust(long);
  virtual bool AddRustLevelDescription(festring&, bool) const;
 protected:
  virtual void VirtualConstructor(bool) { RustData = NOT_RUSTED; }
  int RustData;
);

#endif
