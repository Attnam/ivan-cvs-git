#ifndef __ROOMBA_H__
#define __ROOMBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>
#include <string>

#include "vector2d.h"

class outputfile;
class inputfile;
class character;
class olterrain;
class item;
class lsquare;
class room;

class roomprototype
{
 public:
  roomprototype(room* (*)(bool), const std::string&);
  room* Clone() const { return Cloner(false); }
  room* CloneAndLoad(inputfile&) const;
  const std::string& GetClassId() const { return ClassId; }
  ushort GetIndex() const { return Index; }
 private:
  ushort Index;
  room* (*Cloner)(bool);
  std::string ClassId;
};

class room
{
 public:
  typedef roomprototype prototype;
  room(donothing) : Master(0) { }
  virtual ~room() { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Enter(character*) { }
  vector2d GetPos() const { return Pos; }
  void SetPos(vector2d What) { Pos = What; }
  vector2d GetSize() const { return Size; }
  void SetSize(vector2d What) { Size = What; }
  void HandleInstantiatedCharacter(character*);
  void HandleInstantiatedOLTerrain(olterrain*);
  void SetIndex(uchar What) { Index = What; }
  uchar GetIndex() const { return Index; }
  character* GetMaster() const { return Master; }
  void SetMaster(character* What) { Master = What; }
  virtual bool PickupItem(character*, item*, ushort) { return true; }
  virtual bool DropItem(character*, item*, ushort) { return true; }
  uchar GetDivineMaster() const { return DivineMaster; }
  void SetDivineMaster(uchar What) { DivineMaster = What; }
  virtual void KickSquare(character*, lsquare*) { }
  virtual bool ConsumeItem(character*, item*, ushort) { return true; }
  virtual bool AllowDropGifts() const { return true; }
  virtual bool Drink(character*) const { return true; }
  virtual bool HasDrinkHandler() const { return false; }
  virtual bool Dip(character*) const { return true; }
  virtual bool HasDipHandler() const { return false; }
  virtual void TeleportSquare(character*, lsquare*) { }
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  void DestroyTerrain(character*, olterrain*);
  virtual bool AllowSpoil(const item*) const { return true; }
  virtual bool CheckDestroyTerrain(character*, olterrain*);
  virtual short GetGodRelationAdjustment() const { return -50; }
  virtual bool AllowKick(const character*) const { return true; }
 protected:
  virtual void VirtualConstructor(bool) { }
  std::vector<vector2d> Door;
  vector2d Pos, Size;
  character* Master;
  uchar Index, DivineMaster;
};

#ifdef __FILE_OF_STATIC_ROOM_PROTOTYPE_DEFINITIONS__
#define ROOM_PROTOTYPE(name) roomprototype name::name##_ProtoType(&name::Clone, #name);
#else
#define ROOM_PROTOTYPE(name)
#endif

#define ROOM(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool Load = false) : base(donothing()) { VirtualConstructor(Load); }\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static room* Clone(bool Load) { return new name(Load); }\
 protected:\
  static prototype name##_ProtoType;\
  data\
}; ROOM_PROTOTYPE(name);

#endif
