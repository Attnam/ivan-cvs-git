#ifndef __ACTIONBA_H__
#define __ACTIONBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

class character;
class action;
class outputfile;
class inputfile;
class actionslot;
class square;

class actionprototype
{
 public:
  actionprototype();
  virtual action* Clone() const = 0;
  action* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class action
{
 public:
  typedef actionprototype prototype;
  action() : Actor(0), Volume(0), Weight(0), DNDMode(false) { }
  action(character* Actor) : Actor(Actor), Volume(0), Weight(0), DNDMode(false) { }
  virtual ~action() { }
  virtual void Handle() = 0;
  virtual void Terminate(bool);
  virtual character* GetActor() const { return Actor; }
  virtual void SetActor(character* What) { Actor = What; }
  virtual bool IsVoluntary() const { return true; }
  virtual bool AllowFaint() const { return true; }
  virtual bool AllowFoodConsumption() const { return true; }
  virtual bool AllowDisplace() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GetRestRegenerationBonus() const { return false; }
  //virtual ulong GetWeight() const { return 0; }
  virtual void DropUsedItems() { }
  virtual void DeleteUsedItems() { }
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual std::string GetDescription() const = 0;
  void EditVolume(long);
  void EditWeight(long);
  void LoadActionSlot(inputfile&, actionslot&);

  ulong GetVolume() const { return Volume; }
  void SetVolume(ulong What) { Volume = What; }
  ulong GetWeight() const { return Weight; }
  void SetWeight(ulong What) { Weight = What; }
  virtual void SetSquareUnder(square*) {}
  bool InDNDMode() const { return DNDMode; }
  void SetInDNDMode(bool What) { DNDMode = What; }
 protected:
  virtual void VirtualConstructor() { }
  character* Actor;
  ulong Volume;
  ulong Weight;
  bool DNDMode;
};

#ifdef __FILE_OF_STATIC_ACTION_PROTOTYPE_DECLARATIONS__

#define ACTION_PROTOTYPE(name)\
  \
  class name##_prototype : public actionprototype\
  {\
   public:\
    virtual action* Clone() const { return new name; }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  const action::prototype* name::GetProtoType() const { return &name##_ProtoType; }

#else

#define ACTION_PROTOTYPE(name)

#endif

#define ACTION(name, base, data)\
\
name : public base\
{\
 public:\
  name(character* Actor = 0) : base(Actor) { VirtualConstructor(); }\
  virtual const prototype* GetProtoType() const;\
  data\
}; ACTION_PROTOTYPE(name);

#endif


