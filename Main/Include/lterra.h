#ifndef __LTERRA_H__
#define __LTERRA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "object.h"
#include "terra.h"

class glterrain;
class olterrain;
class item;
class level;
class lsquare;
class stack;
class room;
template <class type> class contentscript;
template <class type> class databasecreator;

class lterrain : public object
{
 public:
  lterrain() : LSquareUnder(0) { }
  virtual void Load(inputfile&);
  virtual bool Open(character*) { return false; }
  virtual bool Close(character*) { return false; }
  vector2d GetPos() const;
  virtual bool CanBeOpened() const { return false; }
  virtual bool AcceptsOffers() const { return false; }
  virtual void ReceiveVomit(character*) { }
  virtual bool CanBeOpenedByAI() { return false; }
  virtual bool Polymorph(character*) { return false; }
  virtual bool DipInto(item*, character*) { return false; }
  virtual bool IsDipDestination() const { return false; }
  virtual bool TryKey(item*, character*) { return false; }
  bool CanBeSeenByPlayer() const;
  bool CanBeSeenBy(character*) const;
  virtual const festring& GetSitMessage() const = 0;
  virtual bool SitOn(character*);
  virtual square* GetSquareUnderEntity(ushort = 0) const;
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  level* GetLevel() const;
  lsquare* GetNearLSquare(vector2d) const;
  lsquare* GetNearLSquare(ushort, ushort) const;
  virtual void CalculateAll() { CalculateEmitation(); }
  virtual void SignalEmitationIncrease(ulong);
  virtual void SignalEmitationDecrease(ulong);
  virtual bool HasKeyHole() const { return CanBeOpened(); }
  virtual bool IsOnGround() const { return true; }
  room* GetRoom() const;
  void Draw(bitmap*, vector2d, ulong, bool) const;
  void Draw(bitmap*, vector2d, ulong, bool, bool) const;
 protected:
  void Initialize(ushort, ushort);
  virtual void VirtualConstructor(bool) { }
  lsquare* LSquareUnder;
};

struct glterraindatabase
{
  void InitDefaults(ushort NewConfig) { Config = NewConfig; }
  bool AllowRandomInstantiation() const { return true; }
  ushort Config;
  vector2d BitmapPos;
  festring Article;
  festring Adjective;
  festring AdjectiveArticle;
  festring NameSingular;
  festring NamePlural;
  festring PostFix;
  uchar ArticleMode;
  std::vector<long> MainMaterialConfig;
  std::vector<long> SecondaryMaterialConfig;
  std::vector<long> ContainedMaterialConfig;
  std::vector<long> MaterialConfigChances;
  bool IsAbstract;
  uchar OKVisualEffects;
  ushort MaterialColorB;
  ushort MaterialColorC;
  ushort MaterialColorD;
  festring SitMessage;
  ulong DefaultMainVolume;
  ulong DefaultSecondaryVolume;
  ulong DefaultContainedVolume;
  bool CreateDivineConfigurations;
  bool ShowMaterial;
  uchar AttachedGod;
  uchar Walkability;
};

class glterrainprototype
{
 public:
  friend class databasecreator<glterrain>;
  glterrainprototype(glterrainprototype*, glterrain* (*)(ushort, ushort), const char*);
  glterrain* Clone(ushort Config = 0, ushort SpecialFlags = 0) const { return Cloner(Config, SpecialFlags); }
  glterrain* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  ushort GetIndex() const { return Index; }
  const std::map<ushort, glterraindatabase>& GetConfig() const { return Config; }
  const glterrainprototype* GetBase() const { return Base; }
  void CreateSpecialConfigurations() { }
  const glterraindatabase& ChooseBaseForConfig(ushort);
 private:
  ushort Index;
  glterrainprototype* Base;
  std::map<ushort, glterraindatabase> Config;
  glterrain* (*Cloner)(ushort, ushort);
  const char* ClassID;
};

class glterrain : public lterrain, public gterrain
{
 public:
  friend class databasecreator<glterrain>;
  typedef glterrainprototype prototype;
  typedef glterraindatabase database;
  typedef std::map<ushort, glterraindatabase> databasemap;
  glterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetEntryDifficulty() const { return 1; }
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  DATA_BASE_VALUE(ushort, Config);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(vector2d, BitmapPos, ushort);
  DATA_BASE_VALUE(const festring&, Article);
  DATA_BASE_VALUE(const festring&, Adjective);
  DATA_BASE_VALUE(const festring&, AdjectiveArticle);
  DATA_BASE_VALUE(const festring&, NameSingular);
  DATA_BASE_VALUE(const festring&, NamePlural);
  DATA_BASE_VALUE(const festring&, PostFix);
  DATA_BASE_VALUE(uchar, ArticleMode);
  DATA_BASE_VALUE(const std::vector<long>&, MainMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, SecondaryMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, ContainedMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, MaterialConfigChances);
  DATA_BASE_VALUE(uchar, OKVisualEffects);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(ushort, MaterialColorB, ushort);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(ushort, MaterialColorC, ushort);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(ushort, MaterialColorD, ushort);
  virtual DATA_BASE_VALUE(const festring&, SitMessage);
  DATA_BASE_VALUE(ulong, DefaultMainVolume);
  DATA_BASE_VALUE(ulong, DefaultSecondaryVolume);
  DATA_BASE_VALUE(ulong, DefaultContainedVolume);
  DATA_BASE_BOOL(ShowMaterial);
  DATA_BASE_VALUE(uchar, Walkability);
  virtual uchar GetAttachedGod() const;
  virtual uchar GetTheoreticalWalkability() const { return DataBase->Walkability; }
 protected:
  virtual void InstallDataBase(ushort);
  virtual uchar GetGraphicsContainerIndex() const;
  const database* DataBase;
};

struct olterraindatabase
{
  void InitDefaults(ushort);
  bool AllowRandomInstantiation() const;
  ushort Config;
  vector2d BitmapPos;
  festring Article;
  festring Adjective;
  festring AdjectiveArticle;
  festring NameSingular;
  festring NamePlural;
  festring PostFix;
  uchar ArticleMode;
  std::vector<long> MainMaterialConfig;
  std::vector<long> SecondaryMaterialConfig;
  std::vector<long> ContainedMaterialConfig;
  std::vector<long> MaterialConfigChances;
  bool IsAbstract;
  uchar OKVisualEffects;
  ushort MaterialColorB;
  ushort MaterialColorC;
  ushort MaterialColorD;
  festring SitMessage;
  ulong DefaultMainVolume;
  ulong DefaultSecondaryVolume;
  ulong DefaultContainedVolume;
  bool CreateDivineConfigurations;
  bool ShowMaterial;
  uchar AttachedGod;
  festring DigMessage;
  bool CanBeDestroyed;
  uchar RestModifier;
  festring RestMessage;
  bool IsUpLink;
  ulong StorageVolume;
  uchar HPModifier;
  bool IsSafeToCreateDoor;
  vector2d OpenBitmapPos;
  bool CreateLockConfigurations;
  uchar Walkability;
  bool IsAlwaysTransparent;
  bool UseBorderTiles;
  ushort BorderTilePriority;
};

class olterrainprototype
{
 public:
  friend class databasecreator<olterrain>;
  olterrainprototype(olterrainprototype*, olterrain* (*)(ushort, ushort), const char*);
  olterrain* Clone(ushort Config = 0, ushort SpecialFlags = 0) const { return Cloner(Config, SpecialFlags); }
  olterrain* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  ushort GetIndex() const { return Index; }
  const std::map<ushort, olterraindatabase>& GetConfig() const { return Config; }
  const olterrainprototype* GetBase() const { return Base; }
  void CreateSpecialConfigurations();
  const olterraindatabase& ChooseBaseForConfig(ushort);
 private:
  ushort Index;
  olterrainprototype* Base;
  std::map<ushort, olterraindatabase> Config;
  olterrain* (*Cloner)(ushort, ushort);
  const char* ClassID;
};

class olterrain : public lterrain, public oterrain
{
 public:
  friend class databasecreator<olterrain>;
  typedef olterrainprototype prototype;
  typedef olterraindatabase database;
  typedef std::map<ushort, olterraindatabase> databasemap;
  olterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool Enter(bool) const;
  virtual void BeKicked(character*, ushort, uchar);
  virtual bool IsDoor() const { return false; }
  virtual bool HasEatEffect() const { return false; } 
  virtual bool HasDrinkEffect() const { return false; } 
  virtual bool Eat(character*) { return false; }
  virtual bool Drink(character*) { return false; }
  virtual void Lock() { }
  virtual bool IsLocked() const { return false; }
  virtual void CreateBoobyTrap() { }
  virtual void HasBeenHitByItem(character*, item*, ushort) { }
  virtual void Break();
  short GetHP() const { return HP; }
  void EditHP(short What) { HP += What; }
  virtual void ReceiveDamage(character*, ushort, ushort);
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  void ShowRestMessage(character*) const;
  DATA_BASE_VALUE(ushort, Config);
  DATA_BASE_VALUE(const festring&, Article);
  DATA_BASE_VALUE(const festring&, Adjective);
  DATA_BASE_VALUE(const festring&, AdjectiveArticle);
  DATA_BASE_VALUE(const festring&, NameSingular);
  DATA_BASE_VALUE(const festring&, NamePlural);
  DATA_BASE_VALUE(const festring&, PostFix);
  DATA_BASE_VALUE(uchar, ArticleMode);
  DATA_BASE_VALUE(const std::vector<long>&, MainMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, SecondaryMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, ContainedMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, MaterialConfigChances);
  DATA_BASE_VALUE(uchar, OKVisualEffects);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(ushort, MaterialColorB, ushort);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(ushort, MaterialColorC, ushort);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(ushort, MaterialColorD, ushort);
  virtual DATA_BASE_VALUE(const festring&, SitMessage);
  DATA_BASE_VALUE(ulong, DefaultMainVolume);
  DATA_BASE_VALUE(ulong, DefaultSecondaryVolume);
  DATA_BASE_VALUE(ulong, DefaultContainedVolume);
  DATA_BASE_BOOL(ShowMaterial);
  DATA_BASE_VALUE(const festring&, DigMessage);
  bool CanBeDestroyed() const;
  DATA_BASE_VALUE(uchar, RestModifier);
  DATA_BASE_VALUE(const festring&, RestMessage);
  DATA_BASE_BOOL(IsUpLink);
  DATA_BASE_VALUE(ulong, StorageVolume);
  DATA_BASE_VALUE(uchar, HPModifier);
  DATA_BASE_BOOL(IsSafeToCreateDoor);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, OpenBitmapPos, ushort);
  DATA_BASE_BOOL(UseBorderTiles);
  DATA_BASE_VALUE(ushort, BorderTilePriority);
  virtual DATA_BASE_VALUE(uchar, Walkability);
  DATA_BASE_BOOL(IsAlwaysTransparent);
  virtual void SetAttachedArea(uchar) { }
  virtual void SetAttachedEntry(uchar) { }
  virtual void SetText(const festring&) { }
  virtual festring GetText() const;
  virtual void SetItemsInside(const std::list<contentscript<item> >&, ushort) { }
  ushort GetStrengthValue() const;
  virtual void SignalVolumeAndWeightChange() { CalculateHP(); }
  void CalculateHP();
  virtual uchar GetAttachedGod() const;
  void SetConfig(ushort, ushort = 0);
  god* GetMasterGod() const;
  virtual bool IsTransparent() const;
  virtual void Draw(bitmap*, vector2d, ulong, ushort, bool) const;
  virtual void Draw(bitmap*, vector2d, ulong, ushort, bool, bool) const;
  virtual uchar GetTheoreticalWalkability() const { return DataBase->Walkability; }
 protected:
  virtual vector2d GetBitmapPos(ushort) const;
  virtual void ModifyAnimationFrames(ushort&) const;
  virtual void VirtualConstructor(bool);
  virtual void InstallDataBase(ushort);
  virtual uchar GetGraphicsContainerIndex() const;
  const database* DataBase;
  short HP;
};

#ifdef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__
#define LTERRAIN_PROTOTYPE(name, base, baseproto, protobase)\
protobase* name##_Clone(ushort Config, ushort SpecialFlags) { return new name(Config, SpecialFlags); }\
protobase##prototype name##_ProtoType(baseproto, &name##_Clone, #name);\
name::name(ushort Config, ushort SpecialFlags) : base(donothing()) { Initialize(Config, SpecialFlags); }\
name::name(donothing D) : base(D) { }\
const protobase##prototype* name::GetProtoType() const { return &name##_ProtoType; }
#else
#define LTERRAIN_PROTOTYPE(name, base, baseproto, protobase)
#endif

#define LTERRAIN(name, base, protobase, data)\
\
name : public base\
{\
 public:\
  name(ushort = 0, ushort = 0);\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  data\
}; LTERRAIN_PROTOTYPE(name, base, &base##_ProtoType, protobase);

#define GLTERRAIN(name, base, data)\
\
LTERRAIN(\
  name,\
  base,\
  glterrain,\
  data\
);

#define OLTERRAIN(name, base, data)\
\
LTERRAIN(\
  name,\
  base,\
  olterrain,\
  data\
);

#endif
