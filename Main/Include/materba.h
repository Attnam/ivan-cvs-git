#ifndef __MATERBA_H__
#define __MATERBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define GOOD 0
#define NEUTRAL 1
#define EVIL 2

/* ConsumeTypes */

#define ODD 0
#define FRUIT 1
#define MEAT 2
#define METAL 4
#define MINERAL 8
#define LIQUID 16
#define BONE 32
#define PROCESSED 64
#define MISC_ORGANIC 128
#define GAS 256

#include "typedef.h"

/* Presentation of material class */

class character;
class item;
class entity;
class outputfile;
class inputfile;
class material;

class material_prototype
{
 public:
  virtual material* Clone(ulong) const = 0;
  virtual material* Clone() const = 0;
  virtual material* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
  virtual bool CanBeWished() const = 0;
  virtual material* CreateWishedMaterial(ulong) const = 0;
  virtual bool IsSolid() const = 0;
 protected:
  ushort Index;
};

struct materialdatabase
{
  ushort StrengthValue;
  ushort Density;
  ushort OfferValue;
  ushort Color;
  ushort PriceModifier;
};

class material
{
 public:
  typedef material_prototype prototype;
  material() : Volume(0), MotherEntity(0) { }
  virtual ~material() { }
  virtual std::string Name(bool = false, bool = true) const;
  virtual ushort GetStrengthValue() const = 0;
  virtual ushort GetConsumeType() const = 0;
  virtual ulong GetVolume() const { return Volume; }
  virtual ulong GetWeight() const { return ulong(float(Volume) * GetDensity() / 1000); }
  virtual ushort GetDensity() const = 0;
  virtual ushort TakeDipVolumeAway();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetVolume(ulong What) { Volume = What; }
  virtual ushort GetEmitation() const { return 0; }
  virtual ushort OfferValue() const = 0;
  virtual uchar Alignment() const { return NEUTRAL; }
  virtual void EatEffect(character*, float, float = 1.0);
  virtual void HitEffect(character*)   { }
  virtual short NutritionValue() const { return 0; }
  virtual void MinusAmount(float Amount)   { SetVolume(ulong(GetVolume() > Amount ? GetVolume() - Amount : 0)); }
  virtual material* Clone(ulong) const = 0;
  virtual material* Clone() const = 0;
  virtual bool IsType(ushort QType) const { return Type() == QType; }
  static bool IsSolid() { return false; }
  virtual ushort GetSkinColor() const { return GetColor(); }
  virtual ushort GetColor() const = 0;
  static bool CanBeWished() { return true; }
  static material* CreateWishedMaterial(ulong) { return 0; } // never called
  virtual entity* GetMotherEntity() const { return MotherEntity; }
  virtual void SetMotherEntity(entity* What) { MotherEntity = What; }
  virtual ulong RawPrice() const { return 0; }
  virtual bool GetIsBadFoodForAI() const { return false; }
  virtual bool CanBeDug(material*) const;
  virtual bool IsFlexible() const { return false; }
  virtual bool IsExplosive() const { return false; }
  virtual ushort ExplosivePower() const { return 0; }
  virtual bool IsFlammable() const { return false; }
  virtual bool IsAlive() const { return false; }
  virtual bool HasBe() const { return false; }
  virtual bool Be() { return true; }
  static bool SpecialWishedMaterial() { return false; }
  virtual ushort GetType() const { return Type(); }
  virtual void AddConsumeEndMessage(character*) const { }
  virtual long CalculateOfferValue(char GodAlignment) const;
 protected:
  virtual std::string NameStem() const = 0;
  virtual std::string AdjectiveStem() const { return NameStem(); }
  virtual std::string Article() const { return "a"; }
  virtual void NormalFoodEffect(character*, float, float);
  virtual ushort Type() const = 0;
  ulong Volume;
  entity* MotherEntity;
};

#ifdef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DECLARATIONS__

#define MATERIAL_PROTOTYPE(name, base)\
  \
  /*materialdatabase name##_MaterialDataBase;*/\
  \
  static class name##_prototype : public material::prototype\
  {\
   public:\
    name##_prototype() { Index = protocontainer<material>::Add(this); }\
    virtual material* Clone(ulong Volume) const { return new name(Volume); }\
    virtual material* Clone() const { return new name; }\
    virtual material* CloneAndLoad(inputfile& SaveFile) const { material* Mat = new name; Mat->Load(SaveFile); return Mat; }\
    virtual std::string ClassName() const { return #name; }\
    virtual bool CanBeWished() const { return name::CanBeWished(); }\
    virtual material* CreateWishedMaterial(ulong) const;\
    virtual bool IsSolid() const { return name::IsSolid(); }\
  } name##_ProtoType;\
  \
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const material::prototype* const name::GetPrototype() { return &name##_ProtoType; }\
  ushort name::Type() const { return name##_ProtoType.GetIndex(); }\
  material* name##_prototype::CreateWishedMaterial(ulong Volume) const { if(!name::SpecialWishedMaterial()) return Clone(Volume); else return name::CreateWishedMaterial(Volume); }

#else

#define MATERIAL_PROTOTYPE(name, base)

#endif

#define MATERIAL(name, base, data)\
\
name : public base\
{\
 public:\
  name(ulong IVolume) { Volume = IVolume; }\
  name() { }\
  virtual material* Clone(ulong Volume) const { return new name(Volume); }\
  virtual material* Clone() const { return new name; }\
  static ushort StaticType();\
  static const material::prototype* const GetPrototype();\
 protected:\
  virtual ushort Type() const;\
  data\
}; MATERIAL_PROTOTYPE(name, base)

#define ABSTRACT_MATERIAL(name, base, data)\
\
name : public base\
{\
 public:\
  data\
};

#endif

