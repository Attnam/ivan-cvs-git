/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __TRAP_H__
#define __TRAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "entity.h"

class trap;
class lsquare;
class character;
class bitmap;

struct trapdata
{
  trapdata(ulong TrapID, ulong VictimID, ulong BodyParts) : Next(0), TrapID(TrapID), VictimID(VictimID), BodyParts(BodyParts) { }
  trapdata() : Next(0) { }
  trapdata* Next;
  ulong TrapID;
  ulong VictimID;
  ulong BodyParts;
};

outputfile& operator<<(outputfile&, const trapdata*);
inputfile& operator>>(inputfile&, trapdata*&);
outputfile& operator<<(outputfile&, const trapdata&);
inputfile& operator>>(inputfile&, trapdata&);

class trapprototype
{
 public:
  trapprototype(trap* (*)(bool), const char*);
  trap* Clone() const { return Cloner(false); }
  trap* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  trap* (*Cloner)(bool);
  const char* ClassID;
};

class trap : public entity
{
 public:
  /* Come To The Dark Side */
  trap* Next;
 public:
  typedef trapprototype prototype;
  trap();
  trap(donothing);
  virtual ~trap();
  virtual square* GetSquareUnderEntity(int = 0) const;
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  virtual bool IsOnGround() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual void AddDescription(festring&) const = 0;
  virtual const prototype* GetProtoType() const;
  virtual void StepOnEffect(character*) = 0;
  virtual void Draw(bitmap*, vector2d, color24) const = 0;
 protected:
  virtual void VirtualConstructor(bool) { }
  lsquare* LSquareUnder;
};

#ifdef __FILE_OF_STATIC_TRAP_PROTOTYPE_DEFINITIONS__
#define TRAP_PROTOTYPE(name, base)\
trap* name##_Clone(bool Load) { return new name(Load); }\
trapprototype name##_ProtoType(&name##_Clone, #name);\
name::name(bool Load) : base(donothing()) { VirtualConstructor(Load); }\
name::name(donothing D) : base(D) { }\
const trapprototype* name::GetProtoType() const { return &name##_ProtoType; }
#else
#define TRAP_PROTOTYPE(name, base)
#endif

#define TRAP(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool = false);\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  data\
}; TRAP_PROTOTYPE(name, base);

#endif
