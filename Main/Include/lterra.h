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
class liquid;
class materialscript;
class glterrainprototype;
class olterrainprototype;
template <class type> class contentscript;
template <class type> class databasecreator;

struct lterraindatabase : public databasebase
{
  void PostProcess() { }
  vector2d BitmapPos;
  festring Article;
  festring Adjective;
  festring AdjectiveArticle;
  festring NameSingular;
  festring NamePlural;
  festring PostFix;
  int ArticleMode;
  fearray<long> MainMaterialConfig;
  fearray<long> SecondaryMaterialConfig;
  fearray<long> MaterialConfigChances;
  long MaterialConfigChanceSum;
  bool IsAbstract;
  int OKVisualEffects;
  color16 MaterialColorB;
  color16 MaterialColorC;
  color16 MaterialColorD;
  festring SitMessage;
  bool ShowMaterial;
  int AttachedGod;
  int Walkability;
  bool HasSecondaryMaterial;
};

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
  virtual bool ReceiveVomit(character*, liquid*) { return false; }
  virtual bool CanBeOpenedByAI() { return false; }
  virtual bool Polymorph(character*) { return false; }
  virtual bool DipInto(item*, character*) { return false; }
  virtual bool IsDipDestination() const { return false; }
  virtual bool TryKey(item*, character*) { return false; }
  bool CanBeSeenByPlayer() const;
  bool CanBeSeenBy(character*) const;
  virtual const festring& GetSitMessage() const = 0;
  virtual bool SitOn(character*);
  virtual square* GetSquareUnderEntity(int = 0) const;
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  level* GetLevel() const;
  lsquare* GetNearLSquare(vector2d) const;
  lsquare* GetNearLSquare(int, int) const;
  virtual void CalculateAll() { CalculateEmitation(); }
  virtual void SignalEmitationIncrease(color24);
  virtual void SignalEmitationDecrease(color24);
  virtual bool HasKeyHole() const { return CanBeOpened(); }
  virtual bool IsOnGround() const { return true; }
  room* GetRoom() const;
  virtual void SignalRustLevelChange();
  virtual void TryToRust(long);
  virtual void ReceiveAcid(material*, long) { }
  void InitMaterials(material*, bool = true);
  void SetMainMaterial(material*, int = 0);
  void ChangeMainMaterial(material*, int = 0);
  virtual void GenerateMaterials();
  virtual void InitMaterials(const materialscript*, const materialscript*, bool);
  virtual const fearray<long>& GetMainMaterialConfig() const = 0;
 protected:
  void Initialize(int, int);
  virtual void VirtualConstructor(bool) { }
  virtual void InstallDataBase(int) = 0;
  lsquare* LSquareUnder;
};

struct glterraindatabase : public lterraindatabase
{
  typedef glterrainprototype prototype;
  void InitDefaults(const prototype*, int);
  bool AllowRandomInstantiation() const { return true; }
  const prototype* ProtoType;
};

class glterrainprototype
{
 public:
  friend class databasecreator<glterrain>;
  glterrainprototype(glterrainprototype*, glterrain* (*)(int, int), const char*);
  glterrain* Clone(int Config = 0, int SpecialFlags = 0) const { return Cloner(Config, SpecialFlags); }
  glterrain* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
  const glterrainprototype* GetBase() const { return Base; }
  int CreateSpecialConfigurations(glterraindatabase**, int Configs) { return Configs; }
  const glterraindatabase* ChooseBaseForConfig(glterraindatabase** TempConfig, int, int) { return *TempConfig; }
  const glterraindatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  glterrainprototype* Base;
  glterraindatabase** ConfigData;
  glterraindatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  glterrain* (*Cloner)(int, int);
  const char* ClassID;
};

class glterrain : public lterrain, public gterrain
{
 public:
  friend class databasecreator<glterrain>;
  typedef glterrainprototype prototype;
  typedef glterraindatabase database;
  glterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetEntryDifficulty() const { return 1; }
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  DATA_BASE_VALUE(int, Config);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(vector2d, BitmapPos, int);
  DATA_BASE_VALUE(const festring&, Article);
  DATA_BASE_VALUE(const festring&, Adjective);
  DATA_BASE_VALUE(const festring&, AdjectiveArticle);
  DATA_BASE_VALUE(const festring&, NameSingular);
  DATA_BASE_VALUE(const festring&, NamePlural);
  DATA_BASE_VALUE(const festring&, PostFix);
  DATA_BASE_VALUE(int, ArticleMode);
  virtual DATA_BASE_VALUE(const fearray<long>&, MainMaterialConfig);
  DATA_BASE_VALUE(const fearray<long>&, SecondaryMaterialConfig);
  virtual DATA_BASE_VALUE(const fearray<long>&, MaterialConfigChances);
  virtual DATA_BASE_VALUE(long, MaterialConfigChanceSum);
  DATA_BASE_VALUE(int, OKVisualEffects);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(color16, MaterialColorB, int);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(color16, MaterialColorC, int);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(color16, MaterialColorD, int);
  virtual DATA_BASE_VALUE(const festring&, SitMessage);
  DATA_BASE_BOOL(ShowMaterial);
  DATA_BASE_VALUE(int, Walkability);
  virtual int GetAttachedGod() const;
  virtual int GetTheoreticalWalkability() const { return DataBase->Walkability; }
  void Draw(bitmap*, vector2d, color24, bool) const;
 protected:
  virtual void InstallDataBase(int);
  virtual int GetGraphicsContainerIndex() const;
  const database* DataBase;
};

struct olterraindatabase : public lterraindatabase
{
  typedef olterrainprototype prototype;
  void InitDefaults(const prototype*, int);
  bool AllowRandomInstantiation() const;
  const prototype* ProtoType;
  bool CreateDivineConfigurations;
  festring DigMessage;
  bool CanBeDestroyed;
  int RestModifier;
  festring RestMessage;
  bool IsUpLink;
  long StorageVolume;
  int HPModifier;
  bool IsSafeToCreateDoor;
  vector2d OpenBitmapPos;
  bool CreateLockConfigurations;  
  bool IsAlwaysTransparent;
  bool UseBorderTiles;
  int BorderTilePriority;
  bool CreateWindowConfigurations;
  vector2d WindowBitmapPos;
  bool ShowThingsUnder;
  fearray<contentscript<item> > LeftOverItems;
};

class olterrainprototype
{
 public:
  friend class databasecreator<olterrain>;
  olterrainprototype(olterrainprototype*, olterrain* (*)(int, int), const char*);
  olterrain* Clone(int Config = 0, int SpecialFlags = 0) const { return Cloner(Config, SpecialFlags); }
  olterrain* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
  const olterrainprototype* GetBase() const { return Base; }
  int CreateSpecialConfigurations(olterraindatabase**, int);
  const olterraindatabase* ChooseBaseForConfig(olterraindatabase** TempConfig, int, int) { return *TempConfig; }
  const olterraindatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  olterrainprototype* Base;
  olterraindatabase** ConfigData;
  olterraindatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  olterrain* (*Cloner)(int, int);
  const char* ClassID;
};

class olterrain : public lterrain, public oterrain
{
 public:
  friend class databasecreator<olterrain>;
  typedef olterrainprototype prototype;
  typedef olterraindatabase database;
  olterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool Enter(bool) const;
  virtual void BeKicked(character*, int, int);
  virtual bool IsDoor() const { return false; }
  virtual bool HasEatEffect() const { return false; } 
  virtual bool HasDrinkEffect() const { return false; } 
  virtual bool Eat(character*) { return false; }
  virtual bool Drink(character*) { return false; }
  virtual void Lock() { }
  virtual bool IsLocked() const { return false; }
  virtual void CreateBoobyTrap() { }
  virtual void HasBeenHitByItem(character*, item*, int) { }
  virtual void Break();
  int GetHP() const { return HP; }
  void EditHP(int What) { HP += What; }
  virtual void ReceiveDamage(character*, int, int);
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  void ShowRestMessage(character*) const;
  DATA_BASE_VALUE(int, Config);
  DATA_BASE_VALUE(const festring&, Article);
  DATA_BASE_VALUE(const festring&, Adjective);
  DATA_BASE_VALUE(const festring&, AdjectiveArticle);
  DATA_BASE_VALUE(const festring&, NameSingular);
  DATA_BASE_VALUE(const festring&, NamePlural);
  DATA_BASE_VALUE(const festring&, PostFix);
  DATA_BASE_VALUE(int, ArticleMode);
  virtual DATA_BASE_VALUE(const fearray<long>&, MainMaterialConfig);
  DATA_BASE_VALUE(const fearray<long>&, SecondaryMaterialConfig);
  virtual DATA_BASE_VALUE(const fearray<long>&, MaterialConfigChances);
  virtual DATA_BASE_VALUE(long, MaterialConfigChanceSum);
  DATA_BASE_VALUE(int, OKVisualEffects);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(color16, MaterialColorB, int);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(color16, MaterialColorC, int);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(color16, MaterialColorD, int);
  virtual DATA_BASE_VALUE(const festring&, SitMessage);
  DATA_BASE_BOOL(ShowMaterial);
  DATA_BASE_VALUE(const festring&, DigMessage);
  bool CanBeDestroyed() const;
  DATA_BASE_VALUE(int, RestModifier);
  DATA_BASE_VALUE(const festring&, RestMessage);
  DATA_BASE_BOOL(IsUpLink);
  DATA_BASE_VALUE(long, StorageVolume);
  DATA_BASE_VALUE(int, HPModifier);
  DATA_BASE_BOOL(IsSafeToCreateDoor);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, OpenBitmapPos, int);
  DATA_BASE_BOOL(UseBorderTiles);
  DATA_BASE_VALUE(int, BorderTilePriority);
  virtual DATA_BASE_VALUE(int, Walkability);
  DATA_BASE_BOOL(IsAlwaysTransparent);
  DATA_BASE_BOOL(CreateWindowConfigurations);
  DATA_BASE_VALUE(vector2d, WindowBitmapPos);
  DATA_BASE_BOOL(ShowThingsUnder);
  DATA_BASE_VALUE(const fearray<contentscript<item> >&, LeftOverItems);
  virtual void SetAttachedArea(int) { }
  virtual void SetAttachedEntry(int) { }
  virtual void SetText(const festring&) { }
  virtual festring GetText() const;
  virtual void SetItemsInside(const fearray<contentscript<item> >&, int) { }
  int GetStrengthValue() const;
  virtual void SignalVolumeAndWeightChange() { HP = CalculateMaxHP(); }
  int CalculateMaxHP();
  virtual int GetAttachedGod() const;
  void SetConfig(int, int = 0);
  god* GetMasterGod() const;
  virtual bool IsTransparent() const;
  virtual void Draw(bitmap*, vector2d, color24, int, bool) const;
  virtual int GetTheoreticalWalkability() const { return DataBase->Walkability; }
  virtual void BeDestroyed() { Break(); }
  virtual void ReceiveAcid(material*, long);
  virtual void SignalRustLevelChange();
  virtual bool IsFountainWithWater() const { return false; }
 protected:
  virtual vector2d GetBitmapPos(int) const;
  virtual void ModifyAnimationFrames(int&) const;
  virtual void VirtualConstructor(bool);
  virtual void InstallDataBase(int);
  virtual int GetGraphicsContainerIndex() const;
  const database* DataBase;
  int HP;
};

#ifdef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__
#define LTERRAIN_PROTOTYPE(name, base, baseproto, protobase)\
protobase* name##_Clone(int Config, int SpecialFlags) { return new name(Config, SpecialFlags); }\
protobase##prototype name##_ProtoType(baseproto, &name##_Clone, #name);\
name::name(int Config, int SpecialFlags) : base(donothing()) { Initialize(Config, SpecialFlags); }\
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
  name(int = 0, int = 0);\
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
