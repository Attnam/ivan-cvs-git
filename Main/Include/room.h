#ifndef __ROOM_H__
#define __ROOM_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"

class room;
class olterrain;
class lsquare;
class festring;

class roomprototype
{
 public:
  roomprototype(room* (*)(bool), const char*);
  room* Clone() const { return Cloner(false); }
  room* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  room* (*Cloner)(bool);
  const char* ClassID;
};

class room
{
 public:
  typedef roomprototype prototype;
  room(donothing);
  virtual ~room();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Enter(character*) { }
  vector2d GetPos() const { return Pos; }
  void SetPos(vector2d What) { Pos = What; }
  vector2d GetSize() const { return Size; }
  void SetSize(vector2d What) { Size = What; }
  void SetIndex(int What) { Index = What; }
  int GetIndex() const { return Index; }
  character* GetMaster() const;
  void SetMasterID(ulong What) { MasterID = What; }
  virtual bool PickupItem(character*, item*, int) { return true; }
  virtual bool DropItem(character*, item*, int) { return true; }
  int GetDivineMaster() const { return DivineMaster; }
  void SetDivineMaster(int What) { DivineMaster = What; }
  virtual void KickSquare(character*, lsquare*) { }
  virtual bool ConsumeItem(character*, item*, int) { return true; }
  virtual bool AllowDropGifts() const { return true; }
  virtual bool Drink(character*) const { return true; }
  virtual bool HasDrinkHandler() const { return false; }
  virtual bool Dip(character*) const { return true; }
  virtual bool HasDipHandler() const { return false; }
  virtual void TeleportSquare(character*, lsquare*) { }
  virtual const prototype* GetProtoType() const = 0;
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual void DestroyTerrain(character*);
  virtual bool AllowSpoil(const item*) const { return true; }
  virtual bool CheckDestroyTerrain(character*);
  virtual int GetGodRelationAdjustment() const { return -50; }
  virtual bool AllowKick(const character*, const lsquare*) const { return true; }
  bool MasterIsActive() const;
  bool CheckKickSquare(const character*, const lsquare*) const;
  virtual void HostileAction(character*) const { }
  virtual bool AllowAltarPolymorph() const { return true; }
  virtual bool AllowFoodSearch() const { return true; }
  virtual void ReceiveVomit(character*) { }
 protected:
  virtual void VirtualConstructor(bool) { }
  mutable character* Master;
  mutable ulong LastMasterSearchTick;
  vector2d Pos;
  vector2d Size;
  ulong MasterID;
  int Index;
  int DivineMaster;
};

#ifdef __FILE_OF_STATIC_ROOM_PROTOTYPE_DEFINITIONS__
#define ROOM_PROTOTYPE(name, base)\
room* name##_Clone(bool Load) { return new name(Load); }\
roomprototype name##_ProtoType(&name##_Clone, #name);\
name::name(bool Load) : base(donothing()) { VirtualConstructor(Load); }\
name::name(donothing D) : base(D) { }\
const roomprototype* name::GetProtoType() const { return &name##_ProtoType; }
#else
#define ROOM_PROTOTYPE(name, base)
#endif

#define ROOM(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool = false);\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  data\
}; ROOM_PROTOTYPE(name, base);

#endif
