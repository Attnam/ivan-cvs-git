#ifndef __ACTIONBA_H__
#define __ACTIONBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "type.h"

class character;

class action : public type
{
 public:
  action(character* Actor) : Actor(Actor) { }
  virtual void Handle() = 0;
  virtual void Terminate(bool);
  virtual character* GetActor() const { return Actor; }
  virtual void SetActor(character* What) { Actor = What; }
  virtual bool IsVoluntary() const { return true; }
  virtual bool AllowFaint() const { return true; }
  virtual bool AllowFoodConsumption() const { return true; }
  virtual bool AllowDisplace() const { return true; }
 protected:
  character* Actor;
};

#ifdef __FILE_OF_STATIC_ACTION_PROTOTYPE_DECLARATIONS__

#define ACTION_PROTOINSTALLER(name, base, setstats)\
  \
  static class name##_protoinstaller\
  {\
   public:\
    name##_protoinstaller() : Index(protocontainer<action>::Add(new name)) { }\
    ushort GetIndex() const { return Index; }\
   private:\
    ushort Index;\
  } name##_ProtoInstaller;\
  \
  void name::Init() { setstats }\
  ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
  const action* const name::GetPrototype() { return protocontainer<action>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

#define ACTION_PROTOINSTALLER(name, base, setstats)

#endif

#define ACTION(name, base, setstats, data)\
\
name : public base\
{\
 public:\
  name() : base(0) { Init(); }\
  name(character* Actor) : base(Actor) { Init(); }\
  virtual action* Clone() const { return new name; }\
  virtual type* CloneAndLoad(inputfile& SaveFile) const { action* Act = new name; Act->Load(SaveFile); return Act; }\
  static ushort StaticType();\
  static const action* const GetPrototype();\
  virtual std::string ClassName() const { return #name; }\
 protected:\
  virtual void Init();\
  virtual ushort Type() const;\
  data\
}; ACTION_PROTOINSTALLER(name, base, setstats)

#endif
