#ifndef __ROOMBA_H__
#define __ROOMBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "vector2d.h"

class outputfile;
class inputfile;
class character;
class olterrain;
class item;
class lsquare;
class room;

class room_prototype
{
 public:
  virtual room* Clone() const = 0;
  virtual room* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class room
{
 public:
  typedef room_prototype prototype;
  room(bool);
  virtual ~room() { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Enter(character*) { }
  virtual room* Clone() const = 0;
  virtual vector2d GetPos() const { return Pos; }
  virtual void SetPos(vector2d What) { Pos = What; }
  virtual vector2d GetSize() const { return Size; }
  virtual void SetSize(vector2d What) { Size = What; }
  virtual void HandleInstantiatedCharacter(character*);
  virtual void HandleInstantiatedOLTerrain(olterrain*);
  virtual void SetIndex(uchar What) { Index = What; }
  virtual uchar GetIndex() const { return Index; }
  virtual character* GetMaster() const { return Master; }
  virtual void SetMaster(character* What) { Master = What; }
  virtual bool PickupItem(character*, item*) { return true; }
  virtual bool DropItem(character*, item*) { return true; }
  virtual uchar GetDivineOwner() const { return DivineOwner; }
  virtual void SetDivineOwner(uchar What) { DivineOwner = What; }
  virtual void KickSquare(character*, lsquare*) { }
  virtual bool ConsumeItem(character*, item*) { return true; }
  virtual bool AllowDropGifts() const { return true; }
  virtual bool Drink(character*) const { return true; }
  virtual bool HasDrinkHandler() const { return false; }
  virtual bool Dip(character*) const { return true; }
  virtual bool HasDipHandler() const { return false; }
  virtual void TeleportSquare(character*, lsquare*) { }
 protected:
  virtual void SetDefaultStats() = 0;
  virtual ushort Type() const = 0;
  std::vector<vector2d> Door;
  vector2d Pos, Size;
  character* Master;
  uchar Index, DivineOwner;
};

#ifdef __FILE_OF_STATIC_ROOM_PROTOTYPE_DECLARATIONS__

#define ROOM_PROTOTYPE(name, base, setstats)\
  \
  static class name##_prototype : public room::prototype\
  {\
   public:\
    name##_prototype() { Index = protocontainer<room>::Add(this); }\
    virtual room* Clone() const { return new name; }\
    virtual room* CloneAndLoad(inputfile& SaveFile) const { room* Room = new name; Room->Load(SaveFile); return Room; }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  void name::SetDefaultStats() { setstats }\
  const room::prototype* const name::GetPrototype() { return protocontainer<room>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoType.GetIndex(); }

#else

#define ROOM_PROTOTYPE(name, base, setstats)

#endif

#define ROOM(name, base, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
  virtual room* Clone() const { return new name; }\
  static ushort StaticType();\
  static const room::prototype* const GetPrototype();\
 protected:\
  virtual ushort Type() const;\
  virtual void SetDefaultStats();\
  data\
}; ROOM_PROTOTYPE(name, base, setstats)

#endif



