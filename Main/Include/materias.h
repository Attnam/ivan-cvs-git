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
  virtual uchar GetSpoilLevel() const { return SpoilLevel; }
  virtual void ResetSpoiling();
  virtual material* EatEffect(character*, ulong);
  virtual void AddConsumeEndMessage(character*) const;
  virtual void SetSpoilCounter(ushort);
  virtual bool CanSpoil() const { return true; }
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
  void TouchEffect(item*);
  void TouchEffect(character*, ushort);
  void TouchEffect(lterrain*);
  //liquid* CloneLiquid() const { return static_cast<liquid*>(Clone(Volume)); }
  liquid* CloneLiquid(ulong Volume) const { return static_cast<liquid*>(Clone(Volume)); }
);

class MATERIAL
(
  flesh,
  organic,
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
  liquid,
 public:
  virtual bool IsPowder() const { return true; }
  virtual bool IsExplosive() const;
  virtual void AddWetness(ulong What) { Wetness += What; }
  virtual void Be();
  virtual bool HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void VirtualConstructor(bool) { Wetness = 0; }
  ulong Wetness;
);

/* Materials that can rust */

class MATERIAL
(
  ironalloy,
  solid,
 public:
  virtual void AddName(festring&, bool = false, bool = true) const;
  virtual void SetRustLevel(uchar);
  virtual ushort GetStrengthValue() const;
  virtual uchar GetRustLevel() const { return RustData & 3; }
  virtual bool IsSparkling() const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetRustData() const { return RustData; }
  virtual bool TryToRust(ulong);
 protected:
  virtual void VirtualConstructor(bool) { RustData = NOT_RUSTED; }
  uchar RustData;
);

#endif
