/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __MATERIA_H__
#define __MATERIA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "script.h"
#include "ivandef.h"

#define MAKE_MATERIAL material::MakeMaterial

class entity;
class bodypart;
class materialprototype;
template <class type> class databasecreator;

struct materialdatabase : public databasebase
{
  typedef materialprototype prototype;
  void InitDefaults(const prototype*, int);
  void PostProcess() { }
  const prototype* ProtoType;
  bool IsAbstract;
  int StrengthValue;
  int ConsumeType;
  int Density;
  int Color;
  int RainColor;
  long PriceModifier;
  bool IsGolemMaterial;
  color24 Emitation;
  bool CanBeWished;
  int NutritionValue;
  bool IsAlive;
  bool IsFlammable;
  bool IsExplosive;
  festring NameStem;
  festring AdjectiveStem;
  festring Article;
  int Effect;
  int ConsumeEndMessage;
  int HitMessage;
  long ExplosivePower;
  alpha Alpha;
  int Flexibility;
  int SpoilModifier;
  bool IsSparkling;
  int EffectStrength;
  bool IsMetal;
  bool CanHaveParasite;
  int DigProductMaterial;
  int ConsumeWisdomLimit;
  int AttachedGod;
  festring BreatheMessage;
  bool EffectIsGood;
  bool IsWarm;
  bool UseMaterialAttributes;
  bool CanRegenerate;
  int BreatheWisdomLimit;
  int RustModifier;
  bool IsBlood;
  int Acidicity;
  bool IsImmuneToAcid;
  contentscript<item> NaturalForm;
  int HardenedMaterial;
  int IntelligenceRequirement;
  bool IsScary;
  bool CanBeMirrored;
  bool AffectInside;
  bool IsValuable;
  bool CanBeTailored;
};

class materialprototype
{
 public:
  friend class databasecreator<material>;
  materialprototype(materialprototype*, material* (*)(int, long, bool), const char*);
  material* Clone(int Config, long Volume = 0) const { return Cloner(Config, Volume, false); }
  material* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
  const materialprototype* GetBase() const { return Base; }
  int CreateSpecialConfigurations(materialdatabase**, int Configs) { return Configs; }
  const materialdatabase* ChooseBaseForConfig(materialdatabase** TempConfig, int, int) { return *TempConfig; }
  const materialdatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  materialprototype* Base;
  materialdatabase** ConfigData;
  materialdatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  material* (*Cloner)(int, long, bool);
  const char* ClassID;
};

class material
{
 public:
  friend class databasecreator<material>;
  typedef materialprototype prototype;
  typedef materialdatabase database;
  material(int NewConfig, long InitVolume = 0, bool Load = false) : MotherEntity(0) { Initialize(NewConfig, InitVolume, Load); }
  material(donothing) : MotherEntity(0) { }
  virtual ~material() { }
  virtual void AddName(festring&, bool = false, bool = true) const;
  festring GetName(bool = false, bool = true) const;
  int TakeDipVolumeAway();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  bool Effect(character*, long);
  virtual material* EatEffect(character*, long);
  bool HitEffect(character*, bodypart*);
  virtual color16 GetSkinColor() const { return GetColor(); }
  virtual void SetSkinColor(int) { }
  long GetRawPrice() const;
  bool CanBeDug(material* ShovelMaterial) const;
  virtual bool HasBe() const { return false; }
  virtual void Be() { }
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual void AddConsumeEndMessage(character*) const;
  DATA_BASE_VALUE(int, Config);
  virtual DATA_BASE_VALUE(int, StrengthValue);
  DATA_BASE_VALUE(int, ConsumeType);
  DATA_BASE_VALUE(int, Density);
  DATA_BASE_VALUE(int, Color);
  DATA_BASE_VALUE(int, RainColor);
  DATA_BASE_VALUE(long, PriceModifier);
  DATA_BASE_BOOL(IsGolemMaterial);
  DATA_BASE_VALUE(color24, Emitation);
  DATA_BASE_BOOL(CanBeWished);
  DATA_BASE_VALUE(int, NutritionValue);
  DATA_BASE_BOOL(IsAlive);
  DATA_BASE_BOOL(IsFlammable);
  virtual DATA_BASE_BOOL(IsExplosive);
  DATA_BASE_VALUE(const festring&, NameStem);
  DATA_BASE_VALUE(const festring&, AdjectiveStem);
  DATA_BASE_VALUE(const festring&, Article);
  DATA_BASE_VALUE(int, Effect);
  DATA_BASE_VALUE(int, ConsumeEndMessage);
  DATA_BASE_VALUE(int, HitMessage);
  DATA_BASE_VALUE(long, ExplosivePower);
  DATA_BASE_VALUE(alpha, Alpha);
  DATA_BASE_VALUE(int, Flexibility);
  DATA_BASE_VALUE(int, SpoilModifier);
  virtual DATA_BASE_BOOL(IsSparkling);
  DATA_BASE_BOOL(IsMetal);
  DATA_BASE_BOOL(CanHaveParasite);
  DATA_BASE_VALUE(int, EffectStrength);
  DATA_BASE_VALUE(int, DigProductMaterial);
  DATA_BASE_VALUE(int, ConsumeWisdomLimit);
  DATA_BASE_VALUE(int, AttachedGod);
  DATA_BASE_BOOL(EffectIsGood);
  DATA_BASE_BOOL(UseMaterialAttributes);
  DATA_BASE_BOOL(CanRegenerate);
  DATA_BASE_VALUE(int, RustModifier);
  DATA_BASE_BOOL(IsBlood);
  DATA_BASE_VALUE(int, Acidicity);
  DATA_BASE_BOOL(IsImmuneToAcid);
  DATA_BASE_VALUE(const contentscript<item>&, NaturalForm);
  DATA_BASE_VALUE(int, HardenedMaterial);
  DATA_BASE_VALUE(int, IntelligenceRequirement);
  DATA_BASE_BOOL(IsValuable);
  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  material* Clone() const { return GetProtoType()->Clone(GetConfig(), Volume); }
  material* Clone(long Volume) const { return GetProtoType()->Clone(GetConfig(), Volume); }
  long GetTotalExplosivePower() const;
  static material* MakeMaterial(int, long = 0);
  virtual bool IsFlesh() const { return false; }
  virtual bool IsLiquid() const { return false; }
  virtual const char* GetConsumeVerb() const;
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  bool IsSameAs(const material* What) const { return What->GetConfig() == GetConfig(); }
  bool IsTransparent() const { return GetAlpha() != 255; }
  virtual material* Duplicate() const = 0;
  virtual long GetTotalNutritionValue() const;
  virtual bool IsVeryCloseToSpoiling() const { return false; }
  virtual void AddWetness(long) { }
  virtual int GetSpoilLevel() const { return 0; }
  virtual void ResetSpoiling() { }
  bool CanBeEatenByAI(const character*) const;
  virtual void SetSpoilCounter(int) { }
  DATA_BASE_VALUE(const festring&, BreatheMessage);
  bool BreatheEffect(character*);
  virtual bool SkinColorIsSparkling() const { return IsSparkling(); }
  virtual void SetSkinColorIsSparkling(bool) { }
  DATA_BASE_BOOL(IsWarm);
  DATA_BASE_VALUE(int, BreatheWisdomLimit);
  DATA_BASE_BOOL(IsScary);
  DATA_BASE_BOOL(CanBeMirrored);
  DATA_BASE_BOOL(AffectInside);
  DATA_BASE_BOOL(CanBeTailored);
  virtual void SetRustLevel(int) { }
  virtual int GetRustLevel() const { return NOT_RUSTED; }
  virtual int GetRustData() const { return NOT_RUSTED; }
  virtual bool TryToRust(long, long = 0) { return false; }
  static const database* GetDataBase(int);
  virtual bool CanSpoil() const { return false; }
  bool IsSolid() const { return !IsLiquid(); }
  /* A dummy materialpredicate */
  bool True() const { return true; }
  void FinishConsuming(character*);
  long GetVolume() const { return Volume; }
  long GetWeight() const { return Volume ? Volume * GetDensity() / 1000 : 0; }
  void EditVolume(long What) { SetVolume(Volume + What); }
  void SetVolume(long);
  void SetVolumeNoSignals(long What) { Volume = What; }
  virtual bool IsPowder() const { return false; }
  static item* CreateNaturalForm(int, long);
  item* CreateNaturalForm(long) const;
  virtual bool IsInfectedByLeprosy() const { return false; }
  virtual void SetIsInfectedByLeprosy(bool) { }
  virtual bool AddRustLevelDescription(festring&, bool) const { return false; }
 protected:
  virtual void VirtualConstructor(bool) { }
  void Initialize(int, long, bool);
  const database* DataBase;
  entity* MotherEntity;
  long Volume;
};

#ifdef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__
#define MATERIAL_PROTOTYPE(name, base, baseproto)\
material* name##_Clone(int NewConfig, long Volume, bool Load) { return new name(NewConfig, Volume, Load); }\
materialprototype name##_ProtoType(baseproto, &name##_Clone, #name);\
name::name(int NewConfig, long InitVolume, bool Load) : base(donothing()) { Initialize(NewConfig, InitVolume, Load); }\
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
  name(int, long = 0, bool = false);\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  virtual material* Duplicate() const;\
  data\
}; MATERIAL_PROTOTYPE(name, base, &base##_ProtoType);

#endif
