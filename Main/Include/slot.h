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
  virtual void Empty() = 0;
  virtual void FastEmpty() = 0;
  void Save(outputfile&) const;
  void Load(inputfile&);
  item* GetItem() const { return Item; }
  void SetItem(item*);
  item* operator->() const { return Item; }
  item* operator*() const { return Item; }
  virtual void MoveItemTo(stack*) = 0;
  virtual bool IsStackSlot() const { return false; }
  virtual bool IsCharacterSlot() const { return false; }
  virtual bool IsGearSlot() const { return false; }
  virtual void AddFriendItem(item*) const = 0;
  virtual bool IsOnGround() const { return false; }
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
 protected:
  character* Master;
};

inline outputfile& operator<<(outputfile& SaveFile, characterslot CharacterSlot)
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
  void Init(bodypart*);
 protected:
  bodypart* BodyPart;
};

inline outputfile& operator<<(outputfile& SaveFile, gearslot GearSlot)
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
 protected:
  action* Action;
};

inline outputfile& operator<<(outputfile& SaveFile, actionslot ActionSlot)
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
