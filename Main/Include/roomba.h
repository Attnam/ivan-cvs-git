#ifndef __ROOMBA_H__
#define __ROOMBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "vector2d.h"
#include "typeable.h"

class outputfile;
class inputfile;
class character;
class overlevelterrain;
class item;
class levelsquare;

class room : public typeable
{
 public:
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
  virtual void HandleInstantiatedOverLevelTerrain(overlevelterrain*);
  virtual void SetIndex(uchar What) { Index = What; }
  virtual uchar GetIndex() const { return Index; }
  virtual character* GetMaster() const { return Master; }
  virtual void SetMaster(character* What) { Master = What; }
  virtual bool PickupItem(character*, item*) { return true; }
  virtual bool DropItem(character*, item*) { return true; }
  virtual uchar GetDivineOwner() const { return DivineOwner; }
  virtual void SetDivineOwner(uchar What) { DivineOwner = What; }
  virtual void KickSquare(character*, levelsquare*) { }
  virtual bool ConsumeItem(character*, item*) { return true; }
  virtual bool AllowDropGifts() const { return true; }
  virtual bool Drink(character*) const { return true; }
  virtual bool HasDrinkHandler() const { return false; }
  virtual void TeleportSquare(character*, levelsquare*) { }
 protected:
  virtual void SetDefaultStats() = 0;
  std::vector<vector2d> Door;
  vector2d Pos, Size;
  character* Master;
  uchar Index, DivineOwner;
};

#ifdef __FILE_OF_STATIC_ROOM_PROTOTYPE_DECLARATIONS__

#define ROOM_PROTOINSTALLER(name, base, setstats)\
  \
  static class name##_protoinstaller\
  {\
   public:\
    name##_protoinstaller() : Index(protocontainer<room>::Add(new name)) { }\
    ushort GetIndex() const { return Index; }\
   private:\
    ushort Index;\
  } name##_ProtoInstaller;\
  \
  ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
  void name::SetDefaultStats() { setstats }\
  const room* const name::GetPrototype() { return protocontainer<room>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

#define ROOM_PROTOINSTALLER(name, base, setstats)

#endif

#define ROOM(name, base, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
  virtual room* Clone() const { return new name; }\
  virtual typeable* CloneAndLoad(inputfile& SaveFile) const { room* Room = new name; Room->Load(SaveFile); return Room; }\
  static ushort StaticType();\
  static const room* const GetPrototype();\
  virtual std::string ClassName() const { return #name; }\
 protected:\
  virtual ushort Type() const;\
  virtual void SetDefaultStats();\
  data\
}; ROOM_PROTOINSTALLER(name, base, setstats)

#endif


