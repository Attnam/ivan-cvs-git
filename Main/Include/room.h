#ifndef __ROOM_H__
#define __ROOM_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"

class room;
class olterrain;
class lsquare;

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
  room(donothing);
  virtual ~room();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Enter(character*) { }
  vector2d GetPos() const { return Pos; }
  void SetPos(vector2d What) { Pos = What; }
  vector2d GetSize() const { return Size; }
  void SetSize(vector2d What) { Size = What; }
  void SetIndex(uchar What) { Index = What; }
  uchar GetIndex() const { return Index; }
  character* GetMaster() const;
  void SetMasterID(ulong What) { MasterID = What; }
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
  void DestroyTerrain(character*);
  virtual bool AllowSpoil(const item*) const { return true; }
  bool CheckDestroyTerrain(character*);
  virtual short GetGodRelationAdjustment() const { return -50; }
  virtual bool AllowKick(const character*, const lsquare*) const { return true; }
  bool MasterIsActive() const;
  bool CheckKickSquare(const character*, const lsquare*) const;
  virtual void HostileAction(character*) const { }
  virtual bool AllowAltarPolymorph() const { return true; }
 protected:
  virtual void VirtualConstructor(bool) { }
  vector2d Pos;
  vector2d Size;
  ulong MasterID;
  uchar Index;
  uchar DivineMaster;
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
