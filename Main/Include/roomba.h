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
  room_prototype();
  virtual room* Clone() const = 0;
  room* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class room
{
 public:
  typedef room_prototype prototype;
  room() : Master(0) { }
  virtual ~room() { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Enter(character*) { }
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
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual void SetDivineMaster(uchar What) { DivineMaster = What; }
  virtual void KickSquare(character*, lsquare*) { }
  virtual bool ConsumeItem(character*, item*) { return true; }
  virtual bool AllowDropGifts() const { return true; }
  virtual bool Drink(character*) const { return true; }
  virtual bool HasDrinkHandler() const { return false; }
  virtual bool Dip(character*) const { return true; }
  virtual bool HasDipHandler() const { return false; }
  virtual void TeleportSquare(character*, lsquare*) { }
  virtual const prototype& GetProtoType() const = 0;
  virtual ushort GetType() const { return GetProtoType().GetIndex(); }
 protected:
  virtual void VirtualConstructor() { }
  std::vector<vector2d> Door;
  vector2d Pos, Size;
  character* Master;
  uchar Index, DivineMaster;
};

#ifdef __FILE_OF_STATIC_ROOM_PROTOTYPE_DECLARATIONS__

#define ROOM_PROTOTYPE(name, base)\
  \
  static class name##_prototype : public room::prototype\
  {\
   public:\
    virtual room* Clone() const { return new name; }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const room::prototype& name::GetProtoType() const { return name##_ProtoType; }

#else

#define ROOM_PROTOTYPE(name, base)

#endif

#define ROOM(name, base, data)\
\
name : public base\
{\
 public:\
  name() { VirtualConstructor(); }\
  static ushort StaticType();\
  virtual const room::prototype& GetProtoType() const;\
  data\
}; ROOM_PROTOTYPE(name, base)

#endif



