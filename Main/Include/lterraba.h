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
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool Open(character* Opener);
  virtual bool Close(character* Closer);
  virtual vector2d GetPos() const;
  virtual bool CanBeOpened() const { return false; }
  virtual bool CanBeOffered() const { return false; }
  virtual bool CanBeDug() const { return false; }
  virtual uchar OKVisualEffects() const { return 0; }
  virtual uchar GetVisualFlags() const { return VisualFlags; }
  virtual void SetVisualFlags(uchar What) { VisualFlags = What; }
  virtual void HandleVisualEffects();
  virtual void ReceiveVomit(character*) { }
  virtual bool CanBeOpenedByAI() { return false; }
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
  virtual bool GetIsLocked() const { return false; }
  virtual bool Polymorph(character*) { return false; }
  virtual bool ReceiveApply(item*, character*) { return false; }
  virtual bool DipInto(item*, character*) { return false; }
  virtual bool IsDipDestination() const { return false; }
  virtual void SetDivineMaster(uchar) { }
  virtual void DrawToTileBuffer(bool) const;
 protected:
  virtual void GenerateMaterials() = 0;
  virtual void Initialize(bool);
  virtual void VirtualConstructor() { }
  virtual ulong GetDefaultMainVolume() const { return 10000000; }
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRLTERRAIN; }
  virtual bool ShowMaterial() const { return true; }
  uchar VisualFlags;
};

class glterrain_prototype
{
 public:
  glterrain_prototype();
  virtual glterrain* Clone(bool = true) const = 0;
  glterrain* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class glterrain : public lterrain, public gterrain
{
 public:
  typedef glterrain_prototype prototype;
  virtual bool SitOn(character*);
  virtual ushort GetEntryAPRequirement() const { return 1000; }
  virtual const prototype& GetProtoType() const = 0;
  virtual ushort GetType() const { return GetProtoType().GetIndex(); }
};

class olterrain_prototype
{
 public:
  olterrain_prototype();
  virtual olterrain* Clone(bool = true) const = 0;
  olterrain* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class olterrain : public lterrain, public oterrain
{
 public:
  typedef olterrain_prototype prototype;
  olterrain() : HP(1000) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GoUp(character*) const;
  virtual bool GoDown(character*) const;
  virtual uchar GetDivineMaster() const { return 0; }
  virtual std::string DigMessage() const { return "The ground is too hard to dig."; }
  virtual void Kick(ushort, bool, uchar) { }
  virtual bool IsDoor() const { return false; }
  virtual bool SitOn(character*) { return false; }
  virtual bool HasConsumeEffect() const { return false; } 
  virtual bool Consume(character*) { return false; }
  virtual void Lock() { }
  virtual void CreateBoobyTrap() { }
  virtual void HasBeenHitBy(item*, float, uchar, bool) { }
  virtual void Break();
  virtual short GetHP() const { return HP; }
  virtual void SetHP(short What) { HP = What; }
  virtual void EditHP(short What) { HP += What; }
  virtual bool IsSafeToDestroy() const { return false; }
  virtual const prototype& GetProtoType() const = 0;
  virtual ushort GetType() const { return GetProtoType().GetIndex(); }
 protected:
  short HP;
};

#ifdef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DECLARATIONS__

#define LTERRAIN_PROTOTYPE(name, base, protobase)\
  \
  static class name##_prototype : public protobase::prototype\
  {\
   public:\
    virtual protobase* Clone(bool CallGenerateMaterials = true) const { return new name(CallGenerateMaterials); }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const protobase::prototype& name::GetProtoType() const { return name##_ProtoType; }

#else

#define LTERRAIN_PROTOTYPE(name, base, protobase)

#endif

#define LTERRAIN(name, base, protobase, data)\
\
name : public base\
{\
 public:\
  name(bool CallGenerateMaterials = true) { Initialize(CallGenerateMaterials); }\
  static ushort StaticType();\
  virtual const protobase::prototype& GetProtoType() const;\
  data\
}; LTERRAIN_PROTOTYPE(name, base, protobase)

#define GLTERRAIN(name, base,  data)\
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

