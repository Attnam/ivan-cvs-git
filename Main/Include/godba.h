#ifndef __GODBA_H__
#define __GODBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"
#include "ivandef.h"

#define GOD_NAME Name().c_str()

class item;
class character;
class outputfile;
class inputfile;
class god;

class godprototype
{
 public:
  godprototype(god* (*)(bool), const std::string&);
  god* Clone() const { return Cloner(false); }
  god* CloneAndLoad(inputfile&) const;
  const std::string& GetClassId() const { return ClassId; }
  ushort GetIndex() const { return Index; }
 private:
  ushort Index;
  god* (*Cloner)(bool);
  std::string ClassId;
};

class god
{
 public:
  typedef godprototype prototype;
  god() : Relation(0), Timer(0), Known(false) { }
  virtual void Pray();
  virtual std::string Name() const = 0;
  virtual std::string Description() const = 0;
  virtual uchar Alignment() const = 0;
  std::string CompleteDescription() const;
  void ApplyDivineTick(ushort Turns) { Timer -= Turns; if(Timer < 0) Timer = 0; }
  void AdjustRelation(god*, bool, short = 25);
  void AdjustRelation(short Amount);
  void AdjustTimer(long Amount);
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetRelation(short Value) { Relation = Value; }
  void SetTimer(long Value) { Timer = Value; }
  bool ReceiveOffer(item*);
  virtual uchar BasicAlignment() const { return NEUTRAL; }
  short GetRelation() const { return Relation; }
  void PrintRelation() const;
  void SetKnown(bool What) { Known = What; }
  bool GetKnown() const { return Known; }
  virtual std::string GetPriestMessage() const;
  void PlayerKickedAltar() { AdjustRelation(-100); }
  void PlayerKickedFriendsAltar() { AdjustRelation(-50); }
  virtual void PlayerVomitedOnAltar();
  character* CreateAngel();
  virtual ushort GetColor() const = 0;
  virtual const prototype* GetProtoType() const = 0;
  ushort GetType() const { return GetProtoType()->GetIndex(); }
 protected:
  virtual void PrayGoodEffect() = 0;
  virtual void PrayBadEffect() = 0;
  short Relation;
  long Timer;
  bool Known;
};

#ifdef __FILE_OF_STATIC_GOD_PROTOTYPE_DEFINITIONS__
#define GOD_PROTOTYPE(name) godprototype name::name##_ProtoType(&name::Clone, #name);
#else
#define GOD_PROTOTYPE(name)
#endif

#define GOD(name, base, data)\
\
name : public base\
{\
 public:\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static god* Clone(bool) { return new name; }\
 protected:\
  static prototype name##_ProtoType;\
  data\
}; GOD_PROTOTYPE(name);

#endif
