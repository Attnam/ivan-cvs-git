#ifndef __GODBA_H__
#define __GODBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define GOD_NAME Name().c_str()

#define ALPP	0   // A = Alignment
#define ALP	1   // L = Lawful
#define AL	2   // N = Neutral
#define ALM	3   // C = Chaotic
#define ANP	4   // P = Plus
#define AN	5   // M = Minus
#define ANM	6
#define ACP	7
#define AC	8
#define ACM	9
#define ACMM	10

#include <string>

#include "typedef.h"
#include "type.h"
#include "materba.h"

class item;
class character;
class outputfile;
class inputfile;

class god : public type
{
 public:
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
  virtual void AddPriestMessage() const;
  virtual void PlayerKickedAltar() { AdjustRelation(-100); }
  virtual void PlayerKickedFriendsAltar() { AdjustRelation(-50); }
  virtual void PlayerVomitedOnAltar();
  virtual character* CreateAngel();
  virtual god* Clone() const = 0;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  short Relation;
  long Timer;
  bool Known;
};

#ifdef __FILE_OF_STATIC_GOD_PROTOTYPE_DECLARATIONS__

#define GOD_PROTOINSTALLER(name, base)\
  \
  static class name##_protoinstaller\
  {\
   public:\
    name##_protoinstaller() : Index(protocontainer<god>::Add(new name)) { }\
    ushort GetIndex() const { return Index; }\
   private:\
    ushort Index;\
  } name##_ProtoInstaller;\
  \
  ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
  const god* const name::GetPrototype() { return protocontainer<god>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

#define GOD_PROTOINSTALLER(name, base)

#endif

#define GOD(name, base, data)\
\
name : public base\
{\
 public:\
  virtual god* Clone() const { return new name; }\
  virtual type* CloneAndLoad(inputfile& SaveFile) const { god* God = new name; God->Load(SaveFile); return God; }\
  static ushort StaticType();\
  static const god* const GetPrototype();\
  virtual std::string ClassName() const { return #name; }\
 protected:\
  virtual ushort Type() const;\
  data\
}; GOD_PROTOINSTALLER(name, base)

#endif
