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

class action_prototype
{
 public:
  virtual action* Clone() const = 0;
  virtual action* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class action
{
 public:
  typedef action_prototype prototype;
  action(character* Actor) : Actor(Actor) { }
  virtual void Handle() = 0;
  virtual void Terminate(bool);
  virtual character* GetActor() const { return Actor; }
  virtual void SetActor(character* What) { Actor = What; }
  virtual bool IsVoluntary() const { return true; }
  virtual bool AllowFaint() const { return true; }
  virtual bool AllowFoodConsumption() const { return true; }
  virtual bool AllowDisplace() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&) { }
  virtual bool GetRestRegenerationBonus() const { return false; }
  virtual ulong GetWeight() const { return 0; }
 protected:
  virtual ushort Type() const = 0;
  character* Actor;
};

#ifdef __FILE_OF_STATIC_ACTION_PROTOTYPE_DECLARATIONS__

#define ACTION_PROTOTYPE(name, base, setstats)\
  \
  static class name##_prototype : public action::prototype\
  {\
   public:\
    name##_prototype() { Index = protocontainer<action>::Add(this); }\
    virtual action* Clone() const { return new name; }\
    virtual action* CloneAndLoad(inputfile& SaveFile) const { action* Act = new name; Act->Load(SaveFile); return Act; }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  void name::Init() { setstats }\
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const action::prototype* const name::GetPrototype() { return &name##_ProtoType; }\
  ushort name::Type() const { return name##_ProtoType.GetIndex(); }

#else

#define ACTION_PROTOTYPE(name, base, setstats)

#endif

#define ACTION(name, base, setstats, data)\
\
name : public base\
{\
 public:\
  name() : base(0) { Init(); }\
  name(character* Actor) : base(Actor) { Init(); }\
  virtual action* Clone() const { return new name; }\
  static ushort StaticType();\
  static const action::prototype* const GetPrototype();\
 protected:\
  virtual void Init();\
  virtual ushort Type() const;\
  data\
}; ACTION_PROTOTYPE(name, base, setstats)

#endif


