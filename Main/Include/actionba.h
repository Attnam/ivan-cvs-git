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
  actionprototype(action* (*)(bool), const std::string&);
  action* Clone() const { return Cloner(false); }
  action* CloneAndLoad(inputfile&) const;
  const std::string& GetClassId() const { return ClassId; }
  ushort GetIndex() const { return Index; }
 private:
  ushort Index;
  action* (*Cloner)(bool);
  std::string ClassId;
};

class action
{
 public:
  typedef actionprototype prototype;
  action(donothing) : Actor(0), DNDMode(false) { }
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
  virtual void DropUsedItems() { }
  virtual void DeleteUsedItems() { }
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual std::string GetDescription() const = 0;
  void LoadActionSlot(inputfile&, actionslot&);
  bool InDNDMode() const { return DNDMode; }
  void SetInDNDMode(bool What) { DNDMode = What; }
  virtual ulong GetVolume() const { return 0; }
  virtual ulong GetWeight() const { return 0; }
  virtual ulong GetEmitation() const { return 0; }
  virtual long GetScore() const { return 0; }
  virtual bool ShowEnvironment() const { return true; }
 protected:
  virtual void VirtualConstructor(bool) { }
  character* Actor;
  bool DNDMode;
};

#ifdef __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__
#define ACTION_PROTOTYPE(name) actionprototype name::name##_ProtoType(&name::Clone, #name);
#else
#define ACTION_PROTOTYPE(name)
#endif

#define ACTION(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool Load) : base(donothing()) { VirtualConstructor(Load); }\
  name(character* NewActor) : base(donothing()) { Actor = NewActor; VirtualConstructor(false); }\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static action* Clone(bool Load) { return new name(Load); }\
 protected:\
  static prototype name##_ProtoType;\
  data\
}; ACTION_PROTOTYPE(name);

#endif
