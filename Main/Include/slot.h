#ifndef __SLOT_H__
#define __SLOT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

#include "typedef.h"

class item;
class stack;
class character;
class stackslot;
class outputfile;
class inputfile;
class stack;
class bodypart;
class action;

typedef std::list<stackslot*> stacklist;
typedef std::list<stackslot*>::iterator stackiterator;

class slot
{
 public:
  slot() : Item(0) { }
  virtual void Empty() = 0;
  virtual void FastEmpty() = 0;
  void Save(outputfile&) const;
  void Load(inputfile&);
  item* GetItem() const { return Item; }
  item* operator->() const { return Item; }
  item* operator*() const { return Item; }
  virtual void MoveItemTo(stack*) = 0;
  virtual bool IsStackSlot() const { return false; }
  virtual bool IsCharacterSlot() const { return false; }
  virtual bool IsGearSlot() const { return false; }
  virtual void AddFriendItem(item*) const = 0;
  virtual bool IsOnGround() const { return false; }
  virtual void EditVolume(long) = 0;
  virtual void EditWeight(long) = 0;
  virtual void PutInItem(item*);
  virtual void EditCarriedWeight(long) { }
 protected:
  item* Item;
};

class stackslot : public slot
{
 public:
  virtual void Empty();
  virtual void FastEmpty();
  stack* GetMotherStack() const { return MotherStack; }
  void SetMotherStack(stack* What) { MotherStack = What; }
  std::list<stackslot*>::iterator GetStackIterator() const { return StackIterator; }
  void SetStackIterator(std::list<stackslot*>::iterator What) { StackIterator = What; }
  virtual void MoveItemTo(stack*);
  virtual bool IsStackSlot() const { return true; }
  virtual void AddFriendItem(item*) const;
  virtual bool IsOnGround() const;
  virtual void EditVolume(long);
  virtual void EditWeight(long);
 protected:
  std::list<stackslot*>::iterator StackIterator;
  stack* MotherStack;
};

inline outputfile& operator<<(outputfile& SaveFile, stackslot* StackSlot)
{
  StackSlot->Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, stackslot*& StackSlot)
{
  StackSlot = new stackslot;
  StackSlot->Load(SaveFile);
  return SaveFile;
}

class characterslot : public slot
{
 public:
  virtual void Empty();
  virtual void FastEmpty();
  character* GetMaster() const { return Master; }
  void SetMaster(character* What) { Master = What; }
  virtual void MoveItemTo(stack*);
  virtual bool IsCharacterSlot() const { return true; }
  virtual void AddFriendItem(item*) const;
  virtual void EditVolume(long);
  virtual void EditWeight(long) { }
  virtual void EditCarriedWeight(long);
 protected:
  character* Master;
};

inline outputfile& operator<<(outputfile& SaveFile, const characterslot& CharacterSlot)
{
  CharacterSlot.Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, characterslot& CharacterSlot)
{
  CharacterSlot.Load(SaveFile);
  return SaveFile;
}

class gearslot : public slot
{
 public:
  virtual void Empty();
  virtual void FastEmpty();
  bodypart* GetBodyPart() const { return BodyPart; }
  void SetBodyPart(bodypart* What) { BodyPart = What; }
  virtual void MoveItemTo(stack*);
  virtual bool IsGearSlot() const { return true; }
  virtual void AddFriendItem(item*) const;
  void Init(bodypart*, uchar);
  virtual void EditVolume(long);
  virtual void EditWeight(long);
  virtual uchar GetEquipmentIndex() const { return EquipmentIndex; }
  virtual void SetEquipmentIndex(uchar What) { EquipmentIndex = What; }
  virtual void PutInItem(item*);
 protected:
  bodypart* BodyPart;
  uchar EquipmentIndex;
};

inline outputfile& operator<<(outputfile& SaveFile, const gearslot& GearSlot)
{
  GearSlot.Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, gearslot& GearSlot)
{
  GearSlot.Load(SaveFile);
  return SaveFile;
}

class actionslot : public slot
{
 public:
  virtual void Empty();
  virtual void FastEmpty();
  action* GetAction() const { return Action; }
  void SetAction(action* What) { Action = What; }
  virtual void MoveItemTo(stack*);
  virtual bool IsActionSlot() const { return true; }
  virtual void AddFriendItem(item*) const;
  void Init(action*);
  virtual void EditVolume(long);
  virtual void EditWeight(long);
 protected:
  action* Action;
};

inline outputfile& operator<<(outputfile& SaveFile, const actionslot& ActionSlot)
{
  ActionSlot.Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, actionslot& ActionSlot)
{
  ActionSlot.Load(SaveFile);
  return SaveFile;
}

#endif
