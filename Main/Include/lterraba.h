#ifndef __LTERRABA_H__
#define __LTERRABA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "object.h"
#include "terra.h"
#include "typedef.h"
#include "vector2d.h"

class character;
class material;
class game;
class object;
class lsquare;
class square;
class outputfile;
class inputfile;
class glterrain;
class olterrain;
class item;
template <class type> class database;

struct terraindatabase
{
  void InitDefaults() { IsAbstract = false; }
  vector2d BitmapPos;
  std::string Article;
  std::string Adjective;
  std::string AdjectiveArticle;
  std::string NameSingular;
  std::string NamePlural;
  std::string PostFix;
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
  std::string SitMessage;
  ulong DefaultMainVolume;
  ulong DefaultSecondaryVolume;
  ulong DefaultContainedVolume;
  bool CreateDivineConfigurations;
  bool ShowMaterial;
};

class lterrain : public object
{
 public:
  lterrain() : SquareUnder(0) { }
  virtual void Load(inputfile&);
  virtual bool Open(character*) { return false; }
  virtual bool Close(character*) { return false; }
  virtual vector2d GetPos() const;
  virtual bool CanBeOpened() const { return false; }
  virtual bool AcceptsOffers() const { return false; }
  virtual void ReceiveVomit(character*) { }
  virtual bool CanBeOpenedByAI() { return false; }
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
  virtual bool Polymorph(character*) { return false; }
  virtual bool DipInto(item*, character*) { return false; }
  virtual bool IsDipDestination() const { return false; }
  virtual bool TryKey(item*, character*) { return false; }
  virtual bool CanBeSeenByPlayer() const;
  virtual bool CanBeSeenBy(character*) const;
  virtual const std::string& GetSitMessage() const = 0;
  virtual bool SitOn(character*);
  virtual square* GetSquareUnder() const { return SquareUnder; }
  void SetSquareUnder(square* What) { SquareUnder = What; }
  lsquare* GetLSquareUnder() const;
 protected:
  void Initialize(uchar, bool, bool);
  virtual void VirtualConstructor(bool) { }
  square* SquareUnder;
};

struct glterraindatabase : public terraindatabase
{
};

class glterrainprototype
{
 public:
  friend class database<glterrain>;
  glterrainprototype(glterrainprototype*, glterrain* (*)(ushort, bool, bool), const std::string&);
  glterrain* Clone(ushort Config = 0, bool CallGenerateMaterials = true) const { return Cloner(Config, CallGenerateMaterials, false); }
  glterrain* CloneAndLoad(inputfile&) const;
  const std::string& GetClassId() const { return ClassId; }
  ushort GetIndex() const { return Index; }
  PROTODATABASEBOOL(IsAbstract);
  PROTODATABASEBOOL(CreateDivineConfigurations);
  const glterraindatabase* GetDataBase() const { return &DataBase; }
  const std::map<ushort, glterraindatabase>& GetConfig() const { return Config; }
  const glterrainprototype* GetBase() const { return Base; }
 protected:
  ushort Index;
  glterraindatabase DataBase;
  glterrainprototype* Base;
  std::map<ushort, glterraindatabase> Config;
  glterrain* (*Cloner)(ushort, bool, bool);
  std::string ClassId;
};

class glterrain : public lterrain, public gterrain
{
 public:
  friend class database<glterrain>;
  typedef glterrainprototype prototype;
  typedef glterraindatabase database;
  typedef std::map<ushort, glterraindatabase> databasemap;
  glterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetEntryDifficulty() const { return 1; }
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual const prototype* GetProtoType() const { return &glterrain_ProtoType; }
  const database* GetDataBase() const { return DataBase; }
  DATABASEVALUEWITHPARAMETER(vector2d, BitmapPos, ushort);
  DATABASEVALUE(const std::string&, Article);
  DATABASEVALUE(const std::string&, Adjective);
  DATABASEVALUE(const std::string&, AdjectiveArticle);
  DATABASEVALUE(const std::string&, NameSingular);
  DATABASEVALUE(const std::string&, NamePlural);
  DATABASEVALUE(const std::string&, PostFix);
  DATABASEVALUE(uchar, ArticleMode);
  DATABASEVALUE(const std::vector<long>&, MainMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, SecondaryMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, ContainedMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, MaterialConfigChances);
  DATABASEVALUE(uchar, OKVisualEffects);
  DATABASEVALUEWITHPARAMETER(ushort, MaterialColorB, ushort);
  DATABASEVALUEWITHPARAMETER(ushort, MaterialColorC, ushort);
  DATABASEVALUEWITHPARAMETER(ushort, MaterialColorD, ushort);
  DATABASEVALUE(const std::string&, SitMessage);
  DATABASEVALUE(ulong, DefaultMainVolume);
  DATABASEVALUE(ulong, DefaultSecondaryVolume);
  DATABASEVALUE(ulong, DefaultContainedVolume);
  DATABASEBOOL(ShowMaterial);
  static glterrain* Clone(ushort, bool, bool) { return 0; }
 protected:
  virtual void InstallDataBase();
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRGLTERRAIN; }
  const database* DataBase;
  static prototype glterrain_ProtoType;
};

struct olterraindatabase : public terraindatabase
{
  std::string DigMessage;
  bool CanBeDug;
  bool IsSafeToDestroy;
  uchar RestModifier;
  std::string RestMessage;
};

class olterrainprototype
{
 public:
  friend class database<olterrain>;
  olterrainprototype(olterrainprototype*, olterrain* (*)(ushort, bool, bool), const std::string&);
  olterrain* Clone(ushort Config = 0, bool CallGenerateMaterials = true) const { return Cloner(Config, CallGenerateMaterials, false); }
  olterrain* CloneAndLoad(inputfile&) const;
  const std::string& GetClassId() const { return ClassId; }
  ushort GetIndex() const { return Index; }
  PROTODATABASEBOOL(IsAbstract);
  PROTODATABASEBOOL(CreateDivineConfigurations);
  const olterraindatabase* GetDataBase() const { return &DataBase; }
  const std::map<ushort, olterraindatabase>& GetConfig() const { return Config; }
  const olterrainprototype* GetBase() const { return Base; }
 protected:
  ushort Index;
  olterraindatabase DataBase;
  olterrainprototype* Base;
  std::map<ushort, olterraindatabase> Config;
  olterrain* (*Cloner)(ushort, bool, bool);
  std::string ClassId;
};

class olterrain : public lterrain, public oterrain
{
 public:
  friend class database<olterrain>;
  typedef olterrainprototype prototype;
  typedef olterraindatabase database;
  typedef std::map<ushort, olterraindatabase> databasemap;
  olterrain(donothing) : HP(1000) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GoUp(character*) const;
  virtual bool GoDown(character*) const;
  virtual void BeKicked(character*, float) { }
  virtual bool IsDoor() const { return false; }
  virtual bool HasEatEffect() const { return false; } 
  virtual bool HasDrinkEffect() const { return false; } 
  virtual bool Eat(character*) { return false; }
  virtual bool Drink(character*) { return false; }
  virtual void Lock() { }
  virtual bool IsLocked() const { return false; }
  virtual void CreateBoobyTrap() { }
  virtual void HasBeenHitBy(item*, float, uchar) { }
  virtual void Break();
  virtual short GetHP() const { return HP; }
  virtual void SetHP(short What) { HP = What; }
  virtual void EditHP(short What) { HP += What; }
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual const prototype* GetProtoType() const { return &olterrain_ProtoType; }
  const database* GetDataBase() const { return DataBase; }
  virtual void ShowRestMessage(character*) const;
  DATABASEVALUEWITHPARAMETER(vector2d, BitmapPos, ushort);
  DATABASEVALUE(const std::string&, Article);
  DATABASEVALUE(const std::string&, Adjective);
  DATABASEVALUE(const std::string&, AdjectiveArticle);
  DATABASEVALUE(const std::string&, NameSingular);
  DATABASEVALUE(const std::string&, NamePlural);
  DATABASEVALUE(const std::string&, PostFix);
  DATABASEVALUE(uchar, ArticleMode);
  DATABASEVALUE(const std::vector<long>&, MainMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, SecondaryMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, ContainedMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, MaterialConfigChances);
  DATABASEVALUE(uchar, OKVisualEffects);
  DATABASEVALUEWITHPARAMETER(ushort, MaterialColorB, ushort);
  DATABASEVALUEWITHPARAMETER(ushort, MaterialColorC, ushort);
  DATABASEVALUEWITHPARAMETER(ushort, MaterialColorD, ushort);
  DATABASEVALUE(const std::string&, SitMessage);
  DATABASEVALUE(ulong, DefaultMainVolume);
  DATABASEVALUE(ulong, DefaultSecondaryVolume);
  DATABASEVALUE(ulong, DefaultContainedVolume);
  DATABASEBOOL(ShowMaterial);
  DATABASEVALUE(const std::string&, DigMessage);
  DATABASEBOOL(CanBeDug);
  DATABASEBOOL(IsSafeToDestroy);
  DATABASEVALUE(uchar, RestModifier);
  DATABASEVALUE(const std::string&, RestMessage);
  static olterrain* Clone(ushort, bool, bool) { return 0; }
 protected:
  virtual void InstallDataBase();
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GROLTERRAIN; }
  const database* DataBase;
  short HP;
  static prototype olterrain_ProtoType;
};

#ifdef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__
#define LTERRAIN_PROTOTYPE(name, baseproto, protobase) protobase##prototype name::name##_ProtoType(baseproto, &name::Clone, #name);
#else
#define LTERRAIN_PROTOTYPE(name, baseproto, protobase)
#endif

#define LTERRAIN(name, base, protobase, data)\
\
name : public base\
{\
 public:\
  name(ushort Config = 0, bool CallGenerateMaterials = true, bool Load = false) : base(donothing()) { Initialize(Config, CallGenerateMaterials, Load); }\
  name(donothing D) : base(D) { }\
  static ushort StaticType();\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static protobase* Clone(ushort Config, bool CallGenerateMaterials, bool Load) { return new name(Config, CallGenerateMaterials, Load); }\
 protected:\
  static prototype name##_ProtoType;\
  data\
}; LTERRAIN_PROTOTYPE(name, &base##_ProtoType, protobase);

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

