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
  void SetItem(item* What) { Item = What; }
  item* operator->() const { return Item; }
  item* operator*() const { return Item; }
  virtual void MoveItemTo(stack*) = 0;
  virtual bool IsCharacterSlot() const { return false; }
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
 protected:
  character* Master;
};

inline outputfile& operator<<(outputfile& SaveFile, characterslot StackSlot)
{
  StackSlot.Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, characterslot& StackSlot)
{
  StackSlot.Load(SaveFile);
  return SaveFile;
}

#endif
