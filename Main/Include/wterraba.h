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

class wterrain : public type
{
 public:
  wterrain() : WSquareUnder(0) { }
  virtual vector2d GetPos() const;
  virtual wsquare* GetWSquareUnder() const { return WSquareUnder; }
  virtual void SetWSquareUnder(wsquare* What) { WSquareUnder = What; }
  virtual worldmap* GetWorldMapUnder() const;
  virtual std::string Name(uchar) const;
  virtual void Load(inputfile&);
 protected:
  virtual std::string NameStem() const = 0;
  virtual std::string Article() const { return "a"; }
  virtual vector2d GetBitmapPos() const = 0;
  wsquare* WSquareUnder;
};

class gwterrain : public wterrain, public gterrain
{
 public:
  gwterrain(bool = true) { }
  virtual void DrawToTileBuffer() const;
  virtual gwterrain* Clone(bool = true) const = 0;
  virtual uchar Priority() const = 0;
  virtual ushort GetEntryAPRequirement() const { return 10000; }
};

class owterrain : public wterrain, public oterrain
{
 public:
  owterrain(bool = true) { }
  virtual void DrawToTileBuffer() const;
  virtual owterrain* Clone(bool = true) const = 0;
  virtual bool GoUp(character*) const;
  virtual bool GoDown(character*) const;
};

#ifdef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DECLARATIONS__

#define WTERRAIN_PROTOINSTALLER(name, base, protobase, setstats)\
  \
  static class name##_protoinstaller\
  {\
   public:\
    name##_protoinstaller() : Index(protocontainer<protobase>::Add(new name(false))) { }\
    ushort GetIndex() const { return Index; }\
   private:\
    ushort Index;\
  } name##_ProtoInstaller;\
  \
  void name::SetDefaultStats() { setstats }\
  ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
  const protobase* const name::GetPrototype() { return protocontainer<protobase>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

#define WTERRAIN_PROTOINSTALLER(name, base, protobase, setstats)

#endif

#define WTERRAIN(name, base, protobase, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
  static ushort StaticType();\
  static const protobase* const GetPrototype();\
  virtual std::string ClassName() const { return #name; }\
 protected:\
  virtual void SetDefaultStats();\
  virtual ushort Type() const;\
  data\
}; WTERRAIN_PROTOINSTALLER(name, base, protobase, setstats)

#define GWTERRAIN(name, base, setstats, data)\
\
WTERRAIN(\
  name,\
  base,\
  gwterrain,\
  setstats,\
  virtual gwterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
  virtual type* CloneAndLoad(inputfile& SaveFile) const { gwterrain* G = new name(false); G->Load(SaveFile); return G; }\
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
  virtual type* CloneAndLoad(inputfile& SaveFile) const { owterrain* O = new name(false); O->Load(SaveFile); return O; }\
  data\
);

#endif



