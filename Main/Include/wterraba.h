#ifndef __WTERRABA_H__
#define __WTERRABA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"
#include "terra.h"
#include "proto.h"

class wsquare;
class worldmap;
class gwterrain;
class owterrain;

class wterrain
{
 public:
  wterrain() : WSquareUnder(0) { }
  virtual vector2d GetPos() const;
  virtual wsquare* GetWSquareUnder() const { return WSquareUnder; }
  virtual void SetWSquareUnder(wsquare* What) { WSquareUnder = What; }
  virtual worldmap* GetWorldMapUnder() const;
  virtual std::string Name(uchar) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual std::string NameStem() const = 0;
  virtual std::string Article() const { return "a"; }
  virtual vector2d GetBitmapPos() const = 0;
  virtual ushort Type() const = 0;
  wsquare* WSquareUnder;
};

class gwterrain_prototype
{
 public:
  virtual gwterrain* Clone(bool = true) const = 0;
  virtual gwterrain* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class gwterrain : public wterrain, public gterrain
{
 public:
  typedef gwterrain_prototype prototype;
  gwterrain(bool = true) { }
  virtual void DrawToTileBuffer() const;
  virtual gwterrain* Clone(bool = true) const = 0;
  virtual uchar Priority() const = 0;
  virtual ushort GetEntryAPRequirement() const { return 10000; }
};

class owterrain_prototype
{
 public:
  virtual owterrain* Clone(bool = true) const = 0;
  virtual owterrain* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class owterrain : public wterrain, public oterrain
{
 public:
  typedef owterrain_prototype prototype;
  owterrain(bool = true) { }
  virtual void DrawToTileBuffer() const;
  virtual owterrain* Clone(bool = true) const = 0;
  virtual bool GoUp(character*) const;
  virtual bool GoDown(character*) const;
};

#ifdef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DECLARATIONS__

#define WTERRAIN_PROTOTYPE(name, base, protobase, setstats)\
  \
  static class name##_prototype : public protobase::prototype\
  {\
   public:\
    name##_prototype() { Index = protocontainer<protobase>::Add(this); }\
    virtual protobase* Clone(bool SetStats = true) const { return new name(SetStats); }\
    virtual protobase* CloneAndLoad(inputfile& SaveFile) const { protobase* P = new name(false); P->Load(SaveFile); return P; }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  void name::SetDefaultStats() { setstats }\
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const protobase::prototype* const name::GetPrototype() { return &name##_ProtoType; }\
  ushort name::Type() const { return name##_ProtoType.GetIndex(); }

#else

#define WTERRAIN_PROTOTYPE(name, base, protobase, setstats)

#endif

#define WTERRAIN(name, base, protobase, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
  static ushort StaticType();\
  static const protobase::prototype* const GetPrototype();\
 protected:\
  virtual void SetDefaultStats();\
  virtual ushort Type() const;\
  data\
}; WTERRAIN_PROTOTYPE(name, base, protobase, setstats)

#define GWTERRAIN(name, base, setstats, data)\
\
WTERRAIN(\
  name,\
  base,\
  gwterrain,\
  setstats,\
  virtual gwterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
  data\
);

#define OWTERRAIN(name, base, setstats, data)\
\
WTERRAIN(\
  name,\
  base,\
  owterrain,\
  setstats,\
  virtual owterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
  data\
);

#endif



