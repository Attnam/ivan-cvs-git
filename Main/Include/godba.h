#ifndef __GODBA_H__
#define __GODBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "ivandef.h"

#define GOD_NAME Name().c_str()

class item;
class character;
class outputfile;
class inputfile;
class god;

class god_prototype
{
 public:
  virtual god* Clone() const = 0;
  virtual god* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
 protected:
  ushort Index;
};

class god
{
 public:
  typedef god_prototype prototype;
  god() : Relation(0), Timer(0), Known(false) { }
  virtual void Pray();
  virtual std::string Name() const = 0;
  virtual std::string Description() const = 0;
  virtual uchar Alignment() const = 0;
  virtual std::string CompleteDescription() const;
  virtual void ApplyDivineTick(ushort Turns) { Timer -= Turns; if(Timer < 0) Timer = 0; }
  virtual void AdjustRelation(god*, bool, short = 25);
  virtual void AdjustRelation(short Amount);
  virtual void AdjustTimer(long Amount);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetRelation(short Value) { Relation = Value; }
  virtual void SetTimer(long Value) { Timer = Value; }
  virtual bool ReceiveOffer(item*);
  virtual uchar BasicAlignment() const { return NEUTRAL; }
  virtual short GetRelation() const { return Relation; }
  virtual void PrintRelation() const;
  virtual void SetKnown(bool What) { Known = What; }
  virtual bool GetKnown() const { return Known; }
  virtual std::string GetPriestMessage() const;
  virtual void PlayerKickedAltar() { AdjustRelation(-100); }
  virtual void PlayerKickedFriendsAltar() { AdjustRelation(-50); }
  virtual void PlayerVomitedOnAltar();
  virtual character* CreateAngel();
  virtual god* Clone() const = 0;
  virtual ushort GetColor() const = 0;
 protected:
  virtual void PrayGoodEffect() = 0;
  virtual void PrayBadEffect() = 0;
  virtual ushort Type() const = 0;
  short Relation;
  long Timer;
  bool Known;
};

#ifdef __FILE_OF_STATIC_GOD_PROTOTYPE_DECLARATIONS__

#define GOD_PROTOTYPE(name, base)\
  \
  static class name##_prototype : public god::prototype\
  {\
   public:\
    name##_prototype() { Index = protocontainer<god>::Add(this); }\
    virtual god* Clone() const { return new name; }\
    virtual god* CloneAndLoad(inputfile& SaveFile) const { god* God = new name; God->Load(SaveFile); return God; }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType;\
  \
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const god::prototype* const name::GetPrototype() { return &name##_ProtoType; }\
  ushort name::Type() const { return name##_ProtoType.GetIndex(); }

#else

#define GOD_PROTOTYPE(name, base)

#endif

#define GOD(name, base, data)\
\
name : public base\
{\
 public:\
  virtual god* Clone() const { return new name; }\
  static ushort StaticType();\
  static const god::prototype* const GetPrototype();\
 protected:\
  virtual ushort Type() const;\
  data\
}; GOD_PROTOTYPE(name, base)

#endif

