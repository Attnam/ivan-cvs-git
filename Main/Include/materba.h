#ifndef __MATERBA_H__
#define __MATERBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <map>

#include "typedef.h"
#include "ivandef.h"

#define MAKE_MATERIAL material::MakeMaterial

/* Presentation of material class */

class character;
class item;
class entity;
class outputfile;
class inputfile;
class material;
template <class type> class database;

struct materialdatabase
{
  void InitDefaults() { }
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
  bool IsFlammable;
  bool IsFlexible;
  bool IsExplosive;
  std::string NameStem;
  std::string AdjectiveStem;
  std::string Article;
  uchar Effect;
  uchar ConsumeEndMessage;
  uchar HitMessage;
  ulong ExplosivePower;
  uchar Alpha;
};

class materialprototype
{
 public:
  friend class database<material>;
  materialprototype(materialprototype*);
  virtual ~materialprototype() { }
  material* Clone(ushort, ulong) const;
  virtual material* Clone(ushort) const = 0;
  material* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
  const materialdatabase* GetDataBase() const { return &DataBase; }
  const materialprototype* GetBase() const { return Base; }
  const std::map<ushort, materialdatabase>& GetConfig() const { return Config; }
 protected:
  ushort Index;
  materialdatabase DataBase;
  materialprototype* Base;
  std::map<ushort, materialdatabase> Config;
};

class material
{
 public:
  typedef materialprototype prototype;
  typedef materialdatabase database;
  typedef std::map<ushort, materialdatabase> databasemap;
  material(ushort Config, ulong Volume) : MotherEntity(0), Volume(Volume), Config(Config) { InstallDataBase(); CalculateWeight(); }
  material(ushort Config) : MotherEntity(0), Volume(0), Weight(0), Config(Config) { InstallDataBase(); }
  material(donothing) : MotherEntity(0), Volume(0), Weight(0), Config(0) { }
  virtual ~material() { }
  virtual std::string GetName(bool = false, bool = true) const;
  ulong GetVolume() const { return Volume; }
  virtual ushort TakeDipVolumeAway();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetVolume(ulong);
  virtual void Effect(character*, long);
  virtual void EatEffect(character*, ulong, float = 1.0);
  virtual void HitEffect(character*);
  virtual ushort GetSkinColor(ushort) const { return GetColor(); }
  virtual ulong RawPrice() const { return 0; }
  virtual bool CanBeDug(material*) const;
  virtual bool HasBe() const { return false; }
  virtual bool Be() { return true; }
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual void AddConsumeEndMessage(character*) const;
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
  DATABASEBOOL(IsFlammable);
  DATABASEBOOL(IsFlexible);
  DATABASEBOOL(IsExplosive);
  DATABASEVALUE(const std::string&, NameStem);
  DATABASEVALUE(const std::string&, AdjectiveStem);
  DATABASEVALUE(const std::string&, Article);
  DATABASEVALUE(uchar, Effect);
  DATABASEVALUE(uchar, ConsumeEndMessage);
  DATABASEVALUE(uchar, HitMessage);
  DATABASEVALUE(ulong, ExplosivePower);
  DATABASEVALUE(uchar, Alpha);

  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  material* Clone(ulong Volume) const { return GetProtoType()->Clone(Config, Volume); }
  material* Clone() const { return GetProtoType()->Clone(Config); }
  virtual ulong GetTotalExplosivePower() const { return ulong(float(Volume) * GetExplosivePower() / 1000000); }
  ushort GetConfig() const { return Config; }
  static material* MakeMaterial(ushort);
  static material* MakeMaterial(ushort, ulong);
  virtual bool IsFlesh() const { return false; }
  virtual bool IsLiquid() const { return false; }
  virtual std::string GetConsumeVerb() const { return "eating"; }
  ulong GetWeight() const { return Weight; }
  void CalculateWeight() { Weight = ulong(float(Volume) * GetDensity() / 1000); }
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  bool IsSameAs(material* What) const { return What->GetConfig() == GetConfig(); }
 protected:
  virtual void InstallDataBase();
  const database* DataBase;
  entity* MotherEntity;
  ulong Volume;
  ulong Weight;
  ushort Config;
};

#ifdef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DECLARATIONS__

#define MATERIAL_PROTOTYPE(name, baseproto)\
  \
  class name##_prototype : public materialprototype\
  {\
   public:\
    name##_prototype(materialprototype* Base) : materialprototype(Base) { }\
    virtual material* Clone(ushort Config) const { return new name(Config); }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType(baseproto);\
  \
  const material::prototype* name::GetProtoType() const { return &name##_ProtoType; }

#else

#define MATERIAL_PROTOTYPE(name, baseproto)

#endif

#define MATERIAL(name, base, data)\
\
name : public base\
{\
 public:\
  name(ushort NewConfig, ulong InitVolume) : base(donothing()) { Config = NewConfig; InstallDataBase(); Volume = InitVolume; CalculateWeight(); }\
  name(ushort NewConfig) : base(donothing()) { Config = NewConfig; InstallDataBase(); }\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const;\
  data\
}; MATERIAL_PROTOTYPE(name, &base##_ProtoType);

#endif

