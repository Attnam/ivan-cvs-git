#ifndef __ACTION_H__
#define __ACTION_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

class character;
class action;
class outputfile;
class inputfile;
class actionslot;

class actionprototype
{
 public:
  actionprototype(action* (*)(bool), const char*);
  action* Clone() const { return Cloner(false); }
  action* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  ushort GetIndex() const { return Index; }
 private:
  ushort Index;
  action* (*Cloner)(bool);
  const char* ClassID;
};

class action
{
 public:
  typedef actionprototype prototype;
  action(donothing);
  virtual ~action();
  virtual void Handle() = 0;
  virtual void Terminate(bool);
  character* GetActor() const { return Actor; }
  void SetActor(character* What) { Actor = What; }
  virtual bool IsVoluntary() const { return true; }
  virtual bool AllowFaint() const { return true; }
  virtual bool AllowFoodConsumption() const { return true; }
  virtual bool TryDisplace() { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsRest() const { return false; }
  virtual void DropUsedItems() { }
  virtual void DeleteUsedItems() { }
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual const char* GetDescription() const = 0;
  void LoadActionSlot(inputfile&, actionslot&);
  bool InDNDMode() const { return DNDMode; }
  void SetInDNDMode(bool What) { DNDMode = What; }
  virtual ulong GetVolume() const { return 0; }
  virtual ulong GetWeight() const { return 0; }
  virtual ulong GetEmitation() const { return 0; }
  virtual long GetScore() const { return 0; }
  virtual bool ShowEnvironment() const { return true; }
  virtual const char* GetDeathExplanation() const { return ""; }
  virtual bool CanBeTalkedTo() const { return true; }
 protected:
  virtual void VirtualConstructor(bool) { }
  character* Actor;
  bool DNDMode;
};

#ifdef __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__
#define ACTION_PROTOTYPE(name, base)\
action* name##_Clone(bool Load) { return new name(Load); }\
actionprototype name##_ProtoType(&name##_Clone, #name);\
name::name(bool Load) : base(donothing()) { VirtualConstructor(Load); }\
name::name(character* NewActor) : base(donothing()) { Actor = NewActor; VirtualConstructor(false); }\
name::name(donothing D) : base(D) { }\
const actionprototype* name::GetProtoType() const { return &name##_ProtoType; }
#else
#define ACTION_PROTOTYPE(name, base)
#endif

#define ACTION(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool);\
  name(character*);\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  data\
}; ACTION_PROTOTYPE(name, base);

#endif

