#ifndef __WTERRA_H__
#define __WTERRA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "terra.h"
#include "wsquare.h"

class wterrain
{
 public:
  wterrain() : WSquareUnder(0), AnimationFrames(1) { }
  virtual ~wterrain() { }
  virtual void Load(inputfile&);
  vector2d GetPos() const { return WSquareUnder->GetPos(); }
  void SetWSquareUnder(wsquare* What) { WSquareUnder = What; }
  worldmap* GetWorldMap() const { return WSquareUnder->GetWorldMap(); }
  void AddName(festring&, int) const;
  festring GetName(int) const;
  bool IsAnimated() const { return AnimationFrames > 1; }
  void SetAnimationFrames(int What) { AnimationFrames = What; }
  virtual const char* GetNameStem() const = 0;
 protected:
  virtual void VirtualConstructor(bool) { }
  virtual bool LongerArticle() const { return false; }
  virtual vector2d GetBitmapPos(int) const = 0;
  wsquare* WSquareUnder;
  int AnimationFrames;
};

class gwterrainprototype
{
 public:
  gwterrainprototype(gwterrain* (*)(bool), const char*);
  gwterrain* Clone() const { return Cloner(false); }
  gwterrain* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  gwterrain* (*Cloner)(bool);
  const char* ClassID;
};

class gwterrain : public wterrain, public gterrain
{
 public:
  typedef gwterrainprototype prototype;
  gwterrain(donothing) { }
  virtual void Save(outputfile&) const;
  void Draw(bitmap*, vector2d, color24, bool) const;
  virtual int GetPriority() const = 0;
  virtual int GetEntryDifficulty() const { return 10; }
  virtual const prototype* GetProtoType() const = 0;
  int GetType() const { return GetProtoType()->GetIndex(); }
  void CalculateNeighbourBitmapPoses();
  virtual int GetWalkability() const;
 protected:
  std::pair<vector2d, int> Neighbour[8];
};

class owterrainprototype
{
 public:
  owterrainprototype(owterrain* (*)(bool), const char*);
  owterrain* Clone() const { return Cloner(false); }
  owterrain* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  owterrain* (*Cloner)(bool);
  const char* ClassID;
};

class owterrain : public wterrain, public oterrain
{
 public:
  typedef owterrainprototype prototype;
  owterrain(donothing) { }
  virtual void Save(outputfile&) const;
  virtual void Draw(bitmap*, vector2d, color24, bool) const;
  virtual const prototype* GetProtoType() const = 0;
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual int GetAttachedDungeon() const { return 0; }
  virtual int GetAttachedArea() const { return 0; }
  virtual int GetAttachedEntry() const;
  virtual bool Enter(bool) const;
  virtual int GetWalkability() const;
};

#ifdef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__
#define WTERRAIN_PROTOTYPE(name, base, protobase)\
protobase* name##_Clone(bool Load) { return new name(Load); }\
protobase##prototype name##_ProtoType(&name##_Clone, #name);\
name::name(bool Load) : base(donothing()) { VirtualConstructor(Load); }\
name::name(donothing D) : base(D) { }\
int name::StaticType() { return name##_ProtoType.GetIndex(); }\
const protobase##prototype* name::GetProtoType() const { return &name##_ProtoType; }
#else
#define WTERRAIN_PROTOTYPE(name, base, protobase)
#endif

#define WTERRAIN(name, base, protobase, data)\
\
name : public base\
{\
 public:\
  name(bool = false);\
  name(donothing);\
  static int StaticType();\
  virtual const prototype* GetProtoType() const;\
  data\
}; WTERRAIN_PROTOTYPE(name, base, protobase);

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
