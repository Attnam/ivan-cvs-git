#ifndef __LTERRABA_H__
#define __LTERRABA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "bitmap.h"
#include "object.h"
#include "terra.h"
#include "typedef.h"
#include "vector2d.h"

#include "proto.h"
#include "materde.h"
#include "game.h"
#include "igraph.h"

class bitmap;
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 10000000; else return 0; }
  virtual void ReceiveVomit(character*) { }
  virtual bool CanBeOpenedByAI() { return false; }
  //virtual bool ReceiveStrike() { return false; }
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
  virtual bool GetIsLocked() const { return false; }
  virtual bool Polymorph(character*) { return false; }
  virtual bool ReceiveApply(item*, character*) { return false; }
  virtual bool DipInto(item*, character*) { return false; }
  virtual bool IsDipDestination() const { return false; }
  virtual void SetDivineMaster(uchar) { }
 protected:
  virtual uchar GetGraphicsContainerIndex() const { return GRLTERRAIN; }
  virtual bool ShowMaterial() const { return true; }
  uchar VisualFlags;
};

class glterrain_prototype
{
 public:
  virtual glterrain* Clone(bool = true, bool = true) const = 0;
  virtual glterrain* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class glterrain : public lterrain, public gterrain
{
 public:
  typedef glterrain_prototype prototype;
  glterrain(bool = true, bool = true) { }
  virtual void DrawToTileBuffer() const;
  virtual glterrain* Clone(bool = true, bool = true) const = 0;
  virtual bool SitOn(character*);
  virtual ushort GetEntryAPRequirement() const { return 1000; }
};

class olterrain_prototype
{
 public:
  virtual olterrain* Clone(bool = true, bool = true) const = 0;
  virtual olterrain* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class olterrain : public lterrain, public oterrain
{
 public:
  typedef olterrain_prototype prototype;
  olterrain(bool = true, bool = true) : HP(1000) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void DrawToTileBuffer() const;
  virtual bool GoUp(character*) const;
  virtual bool GoDown(character*) const;
  virtual uchar GetDivineMaster() const { return 0; }
  virtual std::string DigMessage() const { return "The ground is too hard to dig."; }
  virtual olterrain* Clone(bool = true, bool = true) const = 0;
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
 protected:
  short HP;
};

#ifdef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DECLARATIONS__

#define LTERRAIN_PROTOTYPE(name, base, protobase, initmaterials, setstats)\
  \
  static class name##_prototype : public protobase::prototype\
  {\
   public:\
    name##_prototype() { Index = protocontainer<protobase>::Add(this); }\
    virtual protobase* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
    virtual protobase* CloneAndLoad(inputfile& SaveFile) const { protobase* P = new name(false, false); P->Load(SaveFile); return P; }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  name::name(bool CreateMaterials, bool SetStats) : base(false, false) { if(SetStats) SetDefaultStats(); if(CreateMaterials) initmaterials ; HandleVisualEffects(); }\
  name::name(material* FirstMaterial, bool SetStats) : base(false, false) { if(SetStats) SetDefaultStats(); initmaterials ; SetMaterial(0, FirstMaterial); HandleVisualEffects(); }\
  void name::SetDefaultStats() { setstats }\
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const protobase::prototype* const name::GetPrototype() { return protocontainer<protobase>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoType.GetIndex(); }

#else

#define LTERRAIN_PROTOTYPE(name, base, protobase, initmaterials, setstats)

#endif

#define LTERRAIN(name, base, protobase, initmaterials, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool = true, bool = true);\
  name(material*, bool = true);\
  static ushort StaticType();\
  static const protobase::prototype* const GetPrototype();\
 protected:\
  virtual void SetDefaultStats();\
  virtual ushort Type() const;\
  data\
}; LTERRAIN_PROTOTYPE(name, base, protobase, initmaterials, setstats)

#define GLTERRAIN(name, base, initmaterials, setstats, data)\
\
LTERRAIN(\
  name,\
  base,\
  glterrain,\
  initmaterials,\
  setstats,\
  virtual glterrain* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
  data\
);

#define OLTERRAIN(name, base, initmaterials, setstats, data)\
\
LTERRAIN(\
  name,\
  base,\
  olterrain,\
  initmaterials,\
  setstats,\
  virtual olterrain* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
  data\
);

#endif

