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

/* Presentation of the lterrain class & subclasses */

class lterrain : public object
{
 public:
  virtual bool Open(character*) { return false; }
  virtual bool Close(character*) { return false; }
  virtual vector2d GetPos() const;
  virtual bool CanBeOpened() const { return false; }
  virtual bool CanBeOffered() const { return false; }
  virtual bool CanBeDug() const { return false; }
  virtual void ReceiveVomit(character*) { }
  virtual bool CanBeOpenedByAI() { return false; }
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
  virtual bool Polymorph(character*) { return false; }
  virtual bool DipInto(item*, character*) { return false; }
  virtual bool IsDipDestination() const { return false; }
  virtual void SetDivineMaster(uchar) { }
  virtual bool TryKey(item*, character*) { return false; }
  virtual bool CanBeSeenByPlayer() const;
  virtual bool CanBeSeenBy(character*) const;
 protected:
  virtual void GenerateMaterials() = 0;
  virtual void Initialize(uchar, bool, bool);
  virtual void VirtualConstructor(bool) { }
  virtual ulong GetDefaultMainVolume() const { return 10000000; }
  virtual bool ShowMaterial() const { return true; }
};

class glterrainprototype
{
 public:
  glterrainprototype();
  virtual glterrain* Clone(ushort = 0, bool = true, bool = false) const = 0;
  glterrain* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class glterrain : public lterrain, public gterrain
{
 public:
  typedef glterrainprototype prototype;
  glterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual bool SitOn(character*);
  virtual uchar GetEntryDifficulty() const { return 1; }
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  //virtual void DrawToTileBuffer(bool) const;
 protected:
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRGLTERRAIN; }
};

class olterrainprototype
{
 public:
  olterrainprototype();
  virtual olterrain* Clone(ushort = 0, bool = true, bool = false) const = 0;
  olterrain* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class olterrain : public lterrain, public oterrain
{
 public:
  typedef olterrainprototype prototype;
  olterrain(donothing) : HP(1000) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GoUp(character*) const;
  virtual bool GoDown(character*) const;
  virtual uchar GetDivineMaster() const { return 0; }
  virtual std::string DigMessage() const { return "The ground is too hard to dig."; }
  virtual void BeKicked(character*, float) { }
  virtual bool IsDoor() const { return false; }
  virtual bool SitOn(character*) { return false; }
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
  virtual bool IsSafeToDestroy() const { return false; }
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual bool IsEmpty() const { return false; }
  //virtual void DrawToTileBuffer(bool) const;
 protected:
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GROLTERRAIN; }
  short HP;
};

#ifdef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DECLARATIONS__

#define LTERRAIN_PROTOTYPE(name, protobase)\
  \
  class name##_prototype : public protobase::prototype\
  {\
   public:\
    virtual protobase* Clone(ushort Config, bool CallGenerateMaterials, bool Load) const { return new name(Config, CallGenerateMaterials, Load); }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  const protobase::prototype* name::GetProtoType() const { return &name##_ProtoType; }

#else

#define LTERRAIN_PROTOTYPE(name, protobase)

#endif

#define LTERRAIN(name, base, protobase, data)\
\
name : public base\
{\
 public:\
  name(ushort Config = 0, bool CallGenerateMaterials = true, bool Load = false) : base(donothing()) { Initialize(Config, CallGenerateMaterials, Load); }\
  name(donothing D) : base(D) { }\
  static ushort StaticType();\
  virtual const prototype* GetProtoType() const;\
  data\
}; LTERRAIN_PROTOTYPE(name, protobase);

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

