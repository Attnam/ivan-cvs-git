#ifndef __MATERIA_H__
#define __MATERIA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <map>

#include "typedef.h"
#include "ivandef.h"
#include "festring.h"

#define MAKE_MATERIAL material::MakeMaterial

class character;
class item;
class entity;
class outputfile;
class inputfile;
class material;
class festring;
template <class type> class databasecreator;

struct materialdatabase
{
  void InitDefaults(ushort NewConfig) { DigProductMaterial = Config = NewConfig; }
  ushort Config;
  ushort StrengthValue;
  ushort ConsumeType;
  ushort Density;
  ushort Color;
  ulong PriceModifier;
  bool IsGolemMaterial;
  ulong Emitation;
  bool CanBeWished;
  ushort NutritionValue;
  bool IsAlive;
  bool IsFlammable;
  bool IsExplosive;
  festring NameStem;
  festring AdjectiveStem;
  festring Article;
  uchar Effect;
  uchar ConsumeEndMessage;
  uchar HitMessage;
  ulong ExplosivePower;
  uchar Alpha;
  bool CreateDivineConfigurations;
  ushort Flexibility;
  ushort SpoilModifier;
  bool IsSparkling;
  ushort EffectStrength;
  bool IsMetal;
  bool CanHaveParasite;
  ushort DigProductMaterial;
  ushort ConsumeWisdomLimit;
  uchar AttachedGod;
  festring BreatheMessage;
  bool EffectIsGood;
  bool IsWarm;
  bool UseMaterialAttributes;
  bool CanRegenerate;
  ushort BreatheWisdomLimit;
  ushort RustModifier;
  bool IsBlood;
  ushort Acidicity;
  bool IsImmuneToAcid;
};

class materialprototype
{
 public:
  friend class databasecreator<material>;
  materialprototype(materialprototype*, material* (*)(ushort, ulong, bool), const char*);
  material* Clone(ushort Config, ulong Volume = 0) const { return Cloner(Config, Volume, false); }
  material* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  ushort GetIndex() const { return Index; }
  const materialprototype* GetBase() const { return Base; }
  const std::map<ushort, materialdatabase>& GetConfig() const { return Config; }
  void CreateSpecialConfigurations() { }
  const materialdatabase& ChooseBaseForConfig(ushort);
 private:
  ushort Index;
  materialprototype* Base;
  std::map<ushort, materialdatabase> Config;
  material* (*Cloner)(ushort, ulong, bool);
  const char* ClassID;
};

class material
{
 public:
  friend class databasecreator<material>;
  typedef materialprototype prototype;
  typedef materialdatabase database;
  typedef std::map<ushort, materialdatabase> databasemap;
  material(ushort NewConfig, ulong InitVolume = 0, bool Load = false) : MotherEntity(0) { Initialize(NewConfig, InitVolume, Load); }
  material(donothing) : MotherEntity(0) { }
  virtual ~material() { }
  virtual void AddName(festring&, bool = false, bool = true) const;
  festring GetName(bool = false, bool = true) const;
  ushort TakeDipVolumeAway();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  bool Effect(character*, ulong);
  virtual material* EatEffect(character*, ulong);
  bool HitEffect(character*);
  virtual ushort GetSkinColor() const { return GetColor(); }
  virtual void SetSkinColor(ushort) { }
  ulong GetRawPrice() const;
  bool CanBeDug(material* ShovelMaterial) const;
  virtual bool HasBe() const { return false; }
  virtual void Be() { }
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual void AddConsumeEndMessage(character*) const;
  DATA_BASE_VALUE(ushort, Config);
  virtual DATA_BASE_VALUE(ushort, StrengthValue);
  DATA_BASE_VALUE(ushort, ConsumeType);
  DATA_BASE_VALUE(ushort, Density);
  DATA_BASE_VALUE(ushort, Color);
  DATA_BASE_VALUE(ulong, PriceModifier);
  DATA_BASE_BOOL(IsGolemMaterial);
  DATA_BASE_VALUE(ulong, Emitation);
  DATA_BASE_BOOL(CanBeWished);
  DATA_BASE_VALUE(ushort, NutritionValue);
  DATA_BASE_BOOL(IsAlive);
  DATA_BASE_BOOL(IsFlammable);
  virtual DATA_BASE_BOOL(IsExplosive);
  DATA_BASE_VALUE(const festring&, NameStem);
  DATA_BASE_VALUE(const festring&, AdjectiveStem);
  DATA_BASE_VALUE(const festring&, Article);
  DATA_BASE_VALUE(uchar, Effect);
  DATA_BASE_VALUE(uchar, ConsumeEndMessage);
  DATA_BASE_VALUE(uchar, HitMessage);
  DATA_BASE_VALUE(ulong, ExplosivePower);
  DATA_BASE_VALUE(uchar, Alpha);
  DATA_BASE_VALUE(ushort, Flexibility);
  DATA_BASE_VALUE(ushort, SpoilModifier);
  virtual DATA_BASE_BOOL(IsSparkling);
  DATA_BASE_BOOL(IsMetal);
  DATA_BASE_BOOL(CanHaveParasite);
  DATA_BASE_VALUE(ushort, EffectStrength);
  DATA_BASE_VALUE(ushort, DigProductMaterial);
  DATA_BASE_VALUE(ushort, ConsumeWisdomLimit);
  DATA_BASE_VALUE(uchar, AttachedGod);
  DATA_BASE_BOOL(EffectIsGood);
  DATA_BASE_BOOL(UseMaterialAttributes);
  DATA_BASE_BOOL(CanRegenerate);
  DATA_BASE_VALUE(ushort, RustModifier);
  DATA_BASE_BOOL(IsBlood);
  DATA_BASE_VALUE(ushort, Acidicity);
  DATA_BASE_BOOL(IsImmuneToAcid);
  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  material* Clone() const { return GetProtoType()->Clone(GetConfig(), Volume); }
  material* Clone(ulong Volume) const { return GetProtoType()->Clone(GetConfig(), Volume); }
  ulong GetTotalExplosivePower() const;
  static material* MakeMaterial(ushort);
  static material* MakeMaterial(ushort, ulong);
  virtual bool IsFlesh() const { return false; }
  virtual bool IsLiquid() const { return false; }
  virtual const char* GetConsumeVerb() const;
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  bool IsSameAs(const material* What) const { return What->GetConfig() == GetConfig(); }
  bool IsTransparent() const { return GetAlpha() != 255; }
  virtual material* Duplicate() const = 0;
  virtual ulong GetTotalNutritionValue() const;
  virtual bool IsVeryCloseToSpoiling() const { return false; }
  virtual void AddWetness(ulong) { }
  virtual uchar GetSpoilLevel() const { return 0; }
  virtual void ResetSpoiling() { }
  bool CanBeEatenByAI(const character*) const;
  virtual void SetSpoilCounter(ushort) { }
  DATA_BASE_VALUE(const festring&, BreatheMessage);
  bool BreatheEffect(character*);
  virtual bool SkinColorIsSparkling() const { return IsSparkling(); }
  virtual void SetSkinColorIsSparkling(bool) { }
  DATA_BASE_BOOL(IsWarm);
  DATA_BASE_VALUE(ushort, BreatheWisdomLimit);
  virtual void SetRustLevel(uchar) { }
  virtual uchar GetRustLevel() const { return NOT_RUSTED; }
  virtual uchar GetRustData() const { return NOT_RUSTED; }
  virtual bool TryToRust(ulong) { return false; }
  static const database* GetDataBase(ushort);
  virtual bool CanSpoil() const { return false; }
  bool IsSolid() const { return !IsLiquid(); }
  /* A dummy materialpredicate */
  bool True() const { return true; }
  void FinishConsuming(character*);
  ulong GetVolume() const { return Volume; }
  ulong GetWeight() const { return Weight; }
  void EditVolume(long What) { SetVolume(Volume + What); }
  void SetVolume(ulong);
  void SetVolumeNoSignals(ulong);
  void CalculateWeight() { Weight = Volume ? Volume * GetDensity() / 1000 : 0; }
 protected:
  virtual void VirtualConstructor(bool) { }
  void Initialize(ushort, ulong, bool);
  void InstallDataBase(ushort);
  const database* DataBase;
  entity* MotherEntity;
  ulong Volume;
  ulong Weight;
};

#ifdef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__
#define MATERIAL_PROTOTYPE(name, base, baseproto)\
material* name##_Clone(ushort NewConfig, ulong Volume, bool Load) { return new name(NewConfig, Volume, Load); }\
materialprototype name##_ProtoType(baseproto, &name##_Clone, #name);\
name::name(ushort NewConfig, ulong InitVolume, bool Load) : base(donothing()) { Initialize(NewConfig, InitVolume, Load); }\
name::name(donothing D) : base(D) { }\
const materialprototype* name::GetProtoType() const { return &name##_ProtoType; }\
material* name::Duplicate() const { return new name(*this); }
#else
#define MATERIAL_PROTOTYPE(name, base, baseproto)
#endif

#define MATERIAL(name, base, data)\
name : public base\
{\
 public:\
  name(ushort, ulong = 0, bool = false);\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  virtual material* Duplicate() const;\
  data\
}; MATERIAL_PROTOTYPE(name, base, &base##_ProtoType);

#endif
