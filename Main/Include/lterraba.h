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

/* Presentation of the lterrain class & subclasses */

class lterrain : public object
{
 public:
  lterrain(bool AddToPool) : object(AddToPool, false) { }
  virtual void Load(inputfile&);
  virtual bool Open(character* Opener);
  virtual bool Close(character* Closer);
  virtual vector2d GetPos() const;
  virtual bool CanBeOpened() const { return false; }
  virtual bool CanBeOffered() const { return false; }
  virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
  virtual bool CanBeDigged() const { return false; }
  virtual uchar OKVisualEffects() const { return 0; }
  virtual uchar GetVisualFlags() const { return VisualFlags; }
  virtual void SetVisualFlags(uchar What) { VisualFlags = What; }
  virtual void HandleVisualEffects();
  virtual void Save(outputfile&) const;
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 10000000; else return 0; }
  virtual void ReceiveVomit(character*) { }
  virtual bool CanBeOpenedByAI() { return false; }
  virtual bool ReceiveStrike() { return false; }
  virtual bool GetIsLocked() const { return false; }
  virtual bool Polymorph(character*) { return false; }
  virtual bool ReceiveApply(item*,character*) { return false; }
  virtual bool ReceiveDip(item*,character*) { return false; }
  virtual bool HasDipEffect() const { return false; }
 protected:
  virtual uchar GetGraphicsContainerIndex() const { return GRLTERRAIN; }
  uchar VisualFlags;
};

class glterrain : public lterrain, public gterrain
{
 public:
  glterrain(bool = true, bool = true, bool AddToPool = true) : lterrain(AddToPool) { }
  virtual void DrawToTileBuffer() const;
  virtual glterrain* Clone(bool = true, bool = true) const = 0;
  virtual std::string Name(uchar Case = 0) const { return lterrain::Name(Case); }
  virtual bool SitOn(character*);
  virtual ushort GetEntryAPRequirement() const { return 1000; }
};

class olterrain : public lterrain, public oterrain
{
 public:
  olterrain(bool = true, bool = true, bool AddToPool = true) : lterrain(AddToPool) { }
  virtual void DrawToTileBuffer() const;
  virtual bool GoUp(character*) const;
  virtual bool GoDown(character*) const;
  virtual uchar GetOwnerGod() const { return 0; }
  virtual std::string DigMessage() const { return "The ground is too hard to dig."; }
  virtual olterrain* Clone(bool = true, bool = true) const = 0;
  virtual std::string Name(uchar Case = 0) const { return lterrain::Name(Case); }
  virtual void Kick(ushort, bool, uchar) { }
  virtual bool IsDoor() const { return false; }
  virtual bool SitOn(character*) { return false; }
  virtual bool HasConsumeEffect() const { return false; } 
  virtual bool Consume(character*) { return false; }
  virtual void Lock() { }
  virtual void CreateBoobyTrap() { }
  virtual void HasBeenHitBy(item*, float, uchar, bool) {}
};

#ifdef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DECLARATIONS__

#define LTERRAIN_PROTOINSTALLER(name, base, protobase, initmaterials, setstats)\
  \
  static class name##_protoinstaller\
  {\
   public:\
    name##_protoinstaller() : Index(protocontainer<protobase>::Add(new name(false, false, false))) { }\
    ushort GetIndex() const { return Index; }\
   private:\
    ushort Index;\
  } name##_ProtoInstaller;\
  \
  name::name(bool CreateMaterials, bool SetStats, bool AddToPool) : base(false, false, AddToPool) { if(CreateMaterials) initmaterials ; if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
  name::name(material* FirstMaterial, bool SetStats) : base(false, false) { initmaterials ; SetMaterial(0, FirstMaterial); if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
  void name::SetDefaultStats() { setstats }\
  ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
  const protobase* const name::GetPrototype() { return protocontainer<protobase>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

#define LTERRAIN_PROTOINSTALLER(name, base, protobase, initmaterials, setstats)

#endif

#define LTERRAIN(name, base, protobase, initmaterials, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool = true, bool = true, bool = true);\
  name(material*, bool = true);\
  static ushort StaticType();\
  static const protobase* const GetPrototype();\
  virtual std::string ClassName() const { return #name; }\
 protected:\
  virtual void SetDefaultStats();\
  virtual ushort Type() const;\
  data\
}; LTERRAIN_PROTOINSTALLER(name, base, protobase, initmaterials, setstats)

#define GLTERRAIN(name, base, initmaterials, setstats, data)\
\
LTERRAIN(\
  name,\
  base,\
  glterrain,\
  initmaterials,\
  setstats,\
  virtual glterrain* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
  virtual type* CloneAndLoad(inputfile& SaveFile) const { glterrain* G = new name(false, false); G->Load(SaveFile); return G; }\
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
  virtual type* CloneAndLoad(inputfile& SaveFile) const { olterrain* O = new name(false, false); O->Load(SaveFile); return O; }\
  data\
);

#endif



