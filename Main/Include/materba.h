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

struct materialdatabase
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

class materialprototype
{
 public:
  materialprototype();
  material* Clone(ulong) const;
  virtual material* Clone() const = 0;
  material* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
  DATABASEBOOL(IsSolid);
  DATABASEBOOL(CanBeWished);
  virtual materialdatabase* GetDataBase() const = 0;
  virtual materialprototype* GetBase() const = 0;
  virtual bool IsConcrete() const = 0;
 protected:
  ushort Index;
};

class material
{
 public:
  typedef materialprototype prototype;
  typedef materialdatabase database;
  material() : Volume(0), MotherEntity(0) { }
  virtual ~material() { }
  virtual std::string Name(bool = false, bool = true) const;
  virtual ulong GetVolume() const { return Volume; }
  virtual ulong GetWeight() const { return ulong(float(Volume) * GetDensity() / 1000); }
  virtual ushort TakeDipVolumeAway();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetVolume(ulong What) { Volume = What; }
  virtual void EatEffect(character*, float, float = 1.0);
  virtual void HitEffect(character*)   { }
  virtual void MinusAmount(float Amount)   { SetVolume(ulong(GetVolume() > Amount ? GetVolume() - Amount : 0)); }
  //virtual bool IsType(ushort QType) const { return Type() == QType; }
  virtual ushort GetSkinColor(ushort) const { return GetColor(); }
  virtual entity* GetMotherEntity() const { return MotherEntity; }
  virtual void SetMotherEntity(entity* What) { MotherEntity = What; }
  virtual ulong RawPrice() const { return 0; }
  virtual bool CanBeDug(material*) const;
  virtual bool HasBe() const { return false; }
  virtual bool Be() { return true; }
  virtual ushort GetType() const { return GetProtoType()->GetIndex(); }
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

  static ushort StaticType();
  virtual prototype* GetProtoType() const;
  virtual database* GetDataBase() const;
  virtual material* Clone(ulong Volume) const { return GetProtoType()->Clone(Volume); }
  virtual material* Clone() const { return GetProtoType()->Clone(); }

 protected:
  virtual std::string NameStem() const = 0;
  virtual std::string AdjectiveStem() const { return NameStem(); }
  virtual std::string Article() const { return "a"; }
  virtual void NormalFoodEffect(character*, float, float);
  ulong Volume;
  entity* MotherEntity;
};

#ifdef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DECLARATIONS__

#define MATERIAL_PROTOTYPE(name, cloner, baseproto, concrete)\
  \
  material::database name##_DataBase;\
  \
  static class name##_prototype : public material::prototype\
  {\
   public:\
    virtual material* Clone() const { return cloner; }\
    virtual std::string ClassName() const { return #name; }\
    virtual material::database* GetDataBase() const { return &name##_DataBase; }\
    virtual material::prototype* GetBase() const { return baseproto; }\
    virtual bool IsConcrete() const { return concrete; }\
  } name##_ProtoType;\
  \
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  material::prototype* name::GetProtoType() const { return &name##_ProtoType; }\
  material::database* name::GetDataBase() const { return &name##_DataBase; }

#else

#define MATERIAL_PROTOTYPE(name, cloner, baseproto, concrete)

#endif

#define MATERIAL(name, base, data)\
\
name : public base\
{\
 public:\
  name(ulong InitVolume) { Volume = InitVolume; }\
  name() { }\
  static ushort StaticType();\
  virtual prototype* GetProtoType() const;\
  virtual database* GetDataBase() const;\
  data\
}; MATERIAL_PROTOTYPE(name, new name, &base##_ProtoType, true);

#define ABSTRACT_MATERIAL(name, base, data)\
\
name : public base\
{\
 public:\
  static ushort StaticType();\
  virtual prototype* GetProtoType() const;\
  virtual database* GetDataBase() const;\
  data\
}; MATERIAL_PROTOTYPE(name, 0, &base##_ProtoType, false);

#endif

