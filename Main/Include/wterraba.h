#ifndef __WTERRABA_H__
#define __WTERRABA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "vector2d.h"
#include "terra.h"

class wsquare;
class worldmap;
class gwterrain;
class owterrain;
class bitmap;

class wterrain
{
 public:
  wterrain() : WSquareUnder(0), AnimationFrames(1) { }
  virtual void Load(inputfile&);
  virtual vector2d GetPos() const;
  virtual wsquare* GetWSquareUnder() const { return WSquareUnder; }
  virtual void SetWSquareUnder(wsquare* What) { WSquareUnder = What; }
  virtual worldmap* GetWorldMapUnder() const;
  virtual std::string Name(uchar) const;
  bool IsAnimated() const { return AnimationFrames > 1; }
  void SetAnimationFrames(ushort What) { AnimationFrames = What; }
 protected:
  virtual void VirtualConstructor(bool) { }
  virtual std::string GetNameStem() const = 0; // should be const std::string&
  virtual bool LongerArticle() const { return false; }
  virtual vector2d GetBitmapPos(ushort) const = 0;
  wsquare* WSquareUnder;
  ushort AnimationFrames;
};

class gwterrainprototype
{
 public:
  gwterrainprototype(gwterrain* (*)(bool), const std::string&);
  gwterrain* Clone() const { return Cloner(false); }
  gwterrain* CloneAndLoad(inputfile&) const;
  const std::string& GetClassId() const { return ClassId; }
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
  gwterrain* (*Cloner)(bool);
  std::string ClassId;
};

class gwterrain : public wterrain, public gterrain
{
 public:
  typedef gwterrainprototype prototype;
  gwterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual void Draw(bitmap*, vector2d, ushort, bool, bool) const;
  virtual uchar Priority() const = 0;
  virtual uchar GetEntryDifficulty() const { return 10; }
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
};

class owterrainprototype
{
 public:
  owterrainprototype(owterrain* (*)(bool), const std::string&);
  owterrain* Clone() const { return Cloner(false); }
  owterrain* CloneAndLoad(inputfile&) const;
  const std::string& GetClassId() const { return ClassId; }
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
  owterrain* (*Cloner)(bool);
  std::string ClassId;
};

class owterrain : public wterrain, public oterrain
{
 public:
  typedef owterrainprototype prototype;
  owterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual void Draw(bitmap*, vector2d, ushort, bool, bool) const;
  virtual bool GoUp(character*) const;
  virtual bool GoDown(character*) const;
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
};

#ifdef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__
#define WTERRAIN_PROTOTYPE(name, protobase) protobase##prototype name::name##_ProtoType(&name::Clone, #name);
#else
#define WTERRAIN_PROTOTYPE(name, protobase)
#endif

#define WTERRAIN(name, base, protobase, data)\
\
name : public base\
{\
 public:\
  name(bool Load = false) : base(donothing()) { VirtualConstructor(Load); }\
  name(donothing D) : base(D) { }\
  static ushort StaticType() { return name##_ProtoType.GetIndex(); }\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static protobase* Clone(bool Load) { return new name(Load); }\
 protected:\
  static prototype name##_ProtoType;\
  data\
}; WTERRAIN_PROTOTYPE(name, protobase);

#define GWTERRAIN(name, base, data)\
\
WTERRAIN(\
  name,\
  base,\
  gwterrain,\
  data\
);

#define OWTERRAIN(name, base, data)\
\
WTERRAIN(\
  name,\
  base,\
  owterrain,\
  data\
);

#endif
