#ifndef __MATERBA_H__
#define __MATERBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "ivandef.h"

/* Presentation of material class */

class character;
class item;
class entity;
class outputfile;
class inputfile;
class material;

struct material_database
{
  ushort StrengthValue;
  ushort ConsumeType;
  ushort Density;
  ushort OfferValue;
  ushort Color;
  ulong PriceModifier;
  bool IsSolid;
  ushort Emitation;
  bool CanBeWished;
  uchar Alignment;
  ushort NutritionValue;
  bool IsAlive;
  bool IsBadFoodForAI;
  ushort ExplosivePower;
  bool IsFlammable;
  bool IsFlexible;
  bool IsExplosive;
};

class material_prototype
{
 public:
  virtual material* Clone(ulong) const = 0;
  virtual material* Clone() const = 0;
  virtual material* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
  virtual bool GetIsSolid() const { return GetDataBase().IsSolid; }
  virtual bool GetCanBeWished() const { return GetDataBase().CanBeWished; }
  /*virtual bool CanBeWished() const = 0;
  virtual material* CreateWishedMaterial(ulong) const = 0;
  virtual bool IsSolid() const = 0;*/
  virtual material_database& GetDataBase() const = 0;
 protected:
  ushort Index;
};

class material
{
 public:
  typedef material_prototype prototype;
  material() : Volume(0), MotherEntity(0) { }
  virtual ~material() { }
  virtual std::string Name(bool = false, bool = true) const;
  //virtual ushort GetStrengthValue() const = 0;
  //virtual ushort GetConsumeType() const = 0;
  virtual ulong GetVolume() const { return Volume; }
  virtual ulong GetWeight() const { return ulong(float(Volume) * GetDensity() / 1000); }
  //virtual ushort GetDensity() const = 0;
  //virtual ushort GetDensity() const { return GetDataBaseDensity(); }
  virtual ushort TakeDipVolumeAway();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetVolume(ulong What) { Volume = What; }
  //virtual ushort GetEmitation() const { return 0; }
  //virtual ushort OfferValue() const = 0;
  //virtual uchar Alignment() const { return NEUTRAL; }
  virtual void EatEffect(character*, float, float = 1.0);
  virtual void HitEffect(character*)   { }
  //virtual short NutritionValue() const { return 0; }
  virtual void MinusAmount(float Amount)   { SetVolume(ulong(GetVolume() > Amount ? GetVolume() - Amount : 0)); }
  virtual material* Clone(ulong) const = 0;
  virtual material* Clone() const = 0;
  virtual bool IsType(ushort QType) const { return Type() == QType; }
  //static bool IsSolid() { return false; }
  virtual ushort GetSkinColor(ushort) const { return GetColor(); }
  //virtual ushort GetColor() const { return GetDataBaseColor(); }
  //static bool CanBeWished() { return true; }
  virtual entity* GetMotherEntity() const { return MotherEntity; }
  virtual void SetMotherEntity(entity* What) { MotherEntity = What; }
  virtual ulong RawPrice() const { return 0; }
  //virtual bool GetIsBadFoodForAI() const { return false; }
  virtual bool CanBeDug(material*) const;
  //virtual bool IsFlexible() const { return false; }
  //virtual bool IsExplosive() const { return false; }
  //virtual ushort ExplosivePower() const { return 0; }
  //virtual bool IsFlammable() const { return false; }
  //virtual bool IsAlive() const { return false; }
  virtual bool HasBe() const { return false; }
  virtual bool Be() { return true; }
  static bool SpecialWishedMaterial() { return false; }
  virtual ushort GetType() const { return Type(); }
  virtual void AddConsumeEndMessage(character*) const { }
  virtual long CalculateOfferValue(char GodAlignment) const;

  DATABASEVALUE(ushort, StrengthValue);
  DATABASEVALUE(ushort, ConsumeType);
  DATABASEVALUE(ushort, Density);
  DATABASEVALUE(ushort, OfferValue);
  DATABASEVALUE(ushort, Color);
  DATABASEVALUE(ulong, PriceModifier);
  DATABASEBOOL(IsSolid);
  DATABASEVALUE(ushort, Emitation);
  DATABASEBOOL(CanBeWished);
  DATABASEVALUE(uchar, Alignment);
  DATABASEVALUE(ushort, NutritionValue);
  DATABASEBOOL(IsAlive);
  DATABASEBOOL(IsBadFoodForAI);
  DATABASEVALUE(ushort, ExplosivePower);
  DATABASEBOOL(IsFlammable);
  DATABASEBOOL(IsFlexible);
  DATABASEBOOL(IsExplosive);

  /*virtual ushort GetStrengthValue() const { return GetDataBase().StrengthValue; }
  virtual ushort GetConsumeType() const { return GetDataBase().ConsumeType; }
  virtual ushort GetDensity() const { return GetDataBase().Density; }
  virtual ushort GetOfferValue() const { return GetDataBase().OfferValue; }
  virtual ushort GetColor() const { return GetDataBase().Color; }
  virtual ushort GetPriceModifier() const { return GetDataBase().PriceModifier; }
  virtual bool IsSolid() const { return GetDataBase().IsSolid; }
  virtual ushort GetEmitation() const { return GetDataBase().Emitation; }
  virtual bool CanBeWished() const { return GetDataBase().CanBeWished; }
  virtual uchar GetAlignment() const { return GetDataBase().Alignment; }
  virtual ushort GetNutritionValue() const { return GetDataBase().NutritionValue; }
  virtual bool IsAlive() const { return GetDataBase().IsAlive; }
  virtual bool IsBadFoodForAI() const { return GetDataBase().IsBadFoodForAI; }
  virtual ushort GetExplosivePower() const { return GetDataBase().ExplosivePower; }
  virtual bool IsFlammable() const { return GetDataBase().IsFlammable; }
  virtual bool IsFlexible() const { return GetDataBase().IsFlexible; }
  virtual bool IsExplosive() const { return GetDataBase().IsExplosive; }*/

  /*virtual ushort GetStrengthValue() const { return GetDataBaseStrengthValue(); }
  virtual ushort GetConsumeType() const { return GetDataBaseConsumeType(); }
  virtual ushort GetDensity() const { return GetDataBaseDensity(); }
  virtual ushort GetOfferValue() const { return GetDataBaseOfferValue(); }
  virtual ushort GetColor() const { return GetDataBaseColor(); }
  virtual ushort GetPriceModifier() const { return GetDataBasePriceModifier(); }
  virtual bool GetIsSolid() const { return GetDataBaseIsSolid(); }
  virtual ushort GetEmitation() const { return GetDataBaseEmitation(); }
  virtual bool GetCanBeWished() const { return GetDataBaseCanBeWished(); }
  virtual uchar GetAlignment() const { return GetDataBaseAlignment(); }
  virtual ushort GetNutritionValue() const { return GetDataBaseNutritionValue(); }
  virtual bool GetIsAlive() const { return GetDataBaseIsAlive(); }
  virtual bool GetIsBadFoodForAI() const { return GetDataBaseIsBadFoodForAI(); }
  virtual ushort GetExplosivePower() const { return GetDataBaseExplosivePower(); }
  virtual bool GetIsFlammable() const { return GetDataBaseIsFlammable(); }
  virtual bool GetIsFlexible() const { return GetDataBaseIsFlexible(); }
  virtual bool GetIsExplosive() const { return GetDataBaseIsExplosive(); }*/

  /*virtual ushort GetDataBaseStrengthValue() const = 0;
  virtual ushort GetDataBaseConsumeType() const = 0;
  virtual ushort GetDataBaseDensity() const = 0;
  virtual ushort GetDataBaseOfferValue() const = 0;
  virtual ushort GetDataBaseColor() const = 0;
  virtual ushort GetDataBasePriceModifier() const = 0;
  virtual bool GetDataBaseIsSolid() const = 0;
  virtual ushort GetDataBaseEmitation() const = 0;
  virtual bool GetDataBaseCanBeWished() const = 0;
  virtual uchar GetDataBaseAlignment() const = 0;
  virtual ushort GetDataBaseNutritionValue() const = 0;
  virtual bool GetDataBaseIsAlive() const = 0;
  virtual bool GetDataBaseIsBadFoodForAI() const = 0;
  virtual ushort GetDataBaseExplosivePower() const = 0;
  virtual bool GetDataBaseIsFlammable() const = 0;
  virtual bool GetDataBaseIsFlexible() const = 0;
  virtual bool GetDataBaseIsExplosive() const = 0;*/

  virtual const material_database& GetDataBase() const = 0;

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
  material_database name##_DataBase;\
  \
  static class name##_prototype : public material::prototype\
  {\
   public:\
    name##_prototype() { Index = protocontainer<material>::Add(this); }\
    virtual material* Clone(ulong Volume) const { return new name(Volume); }\
    virtual material* Clone() const { return new name; }\
    virtual material* CloneAndLoad(inputfile& SaveFile) const { material* Mat = new name; Mat->Load(SaveFile); return Mat; }\
    virtual std::string ClassName() const { return #name; }\
    /*virtual bool CanBeWished() const { return name::CanBeWished(); }\
    virtual material* CreateWishedMaterial(ulong) const;\
    virtual bool IsSolid() const { return name::IsSolid(); }*/\
    virtual material_database& GetDataBase() const { return name##_DataBase; }\
  } name##_ProtoType;\
  \
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const material::prototype* const name::GetProtoType() { return &name##_ProtoType; }\
  ushort name::Type() const { return name##_ProtoType.GetIndex(); }\
  const material_database& name::GetDataBase() const { return name##_DataBase; }
  /*material* name##_prototype::CreateWishedMaterial(ulong Volume) const { if(!name::SpecialWishedMaterial()) return Clone(Volume); else return name::CreateWishedMaterial(Volume); }*/
  /*ushort name::GetDataBaseStrengthValue() const { return name##_DataBase.StrengthValue; }\
  ushort name::GetDataBaseConsumeType() const { return name##_DataBase.ConsumeType; }\
  ushort name::GetDataBaseDensity() const { return name##_DataBase.Density; }\
  ushort name::GetDataBaseOfferValue() const { return name##_DataBase.OfferValue; }\
  ushort name::GetDataBaseColor() const { return name##_DataBase.Color; }\
  ushort name::GetDataBasePriceModifier() const { return name##_DataBase.PriceModifier; }\
  bool name::GetDataBaseIsSolid() const { return name##_DataBase.IsSolid; }\
  ushort name::GetDataBaseEmitation() const { return name##_DataBase.Emitation; }\
  bool name::GetDataBaseCanBeWished() const { return name##_DataBase.CanBeWished; }\
  uchar name::GetDataBaseAlignment() const { return name##_DataBase.Alignment; }\
  ushort name::GetDataBaseNutritionValue() const { return name##_DataBase.NutritionValue; }\
  bool name::GetDataBaseIsAlive() const { return name##_DataBase.IsAlive; }\
  bool name::GetDataBaseIsBadFoodForAI() const { return name##_DataBase.IsBadFoodForAI; }\
  ushort name::GetDataBaseExplosivePower() const { return name##_DataBase.ExplosivePower; }\
  bool name::GetDataBaseIsFlammable() const { return name##_DataBase.IsFlammable; }\
  bool name::GetDataBaseIsFlexible() const { return name##_DataBase.IsFlexible; }\
  bool name::GetDataBaseIsExplosive() const { return name##_DataBase.IsExplosive; }*/

  /*ushort name::GetDataBaseDensity() const { return name##_DataBase.Density; }\
  ushort name::GetDataBaseColor() const { return name##_DataBase.Color; }*/

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
  static const material::prototype* const GetProtoType();\
  virtual const material_database& GetDataBase() const;\
  /*virtual ushort GetDataBaseStrengthValue() const;\
  virtual ushort GetDataBaseConsumeType() const;\
  virtual ushort GetDataBaseDensity() const;\
  virtual ushort GetDataBaseOfferValue() const;\
  virtual ushort GetDataBaseColor() const;\
  virtual ushort GetDataBasePriceModifier() const;\
  virtual bool GetDataBaseIsSolid() const;\
  virtual ushort GetDataBaseEmitation() const;\
  virtual bool GetDataBaseCanBeWished() const;\
  virtual uchar GetDataBaseAlignment() const;\
  virtual ushort GetDataBaseNutritionValue() const;\
  virtual bool GetDataBaseIsAlive() const;\
  virtual bool GetDataBaseIsBadFoodForAI() const;\
  virtual ushort GetDataBaseExplosivePower() const;\
  virtual bool GetDataBaseIsFlammable() const;\
  virtual bool GetDataBaseIsFlexible() const;\
  virtual bool GetDataBaseIsExplosive() const;*/\
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

