/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

#ifndef __SLOT_H__
#define __SLOT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

class item;
class character;
class outputfile;
class inputfile;
class bodypart;
class square;

class slot
{
 public:
  slot() : Item(0) { }
  virtual void Empty() = 0;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  item* GetItem() const { return Item; }
  item* operator->() const { return Item; }
  item* operator*() const { return Item; }
  virtual void AddFriendItem(item*) const = 0;
  virtual bool IsOnGround() const { return false; }
  virtual void PutInItem(item*) = 0;
  virtual square* GetSquareUnder(int = 0) const = 0;
  virtual void SignalVolumeAndWeightChange() = 0;
  virtual void SignalEmitationIncrease(color24) = 0;
  virtual void SignalEmitationDecrease(color24) = 0;
  virtual void DonateTo(item*);
  virtual bool CanBeSeenBy(const character*) const = 0;
  virtual void SignalEnchantmentChange() { }
  virtual bool IsVisible() const = 0;
  virtual bool IsGearSlot() const { return false; }
 protected:
  item* Item;
};

class stackslot : public slot
{
 public:
  friend class stack;
  friend class stackiterator;
  stackslot(stack* MotherStack, stackslot* Last) : MotherStack(MotherStack), Last(Last), Next(0) { }
  virtual void Empty();
  virtual void AddFriendItem(item*) const;
  virtual bool IsOnGround() const;
  virtual square* GetSquareUnder(int = 0) const;
  virtual void SignalVolumeAndWeightChange();
  virtual void SignalEmitationIncrease(color24);
  virtual void SignalEmitationDecrease(color24);
  virtual void DonateTo(item*);
  virtual bool CanBeSeenBy(const character*) const;
  stack* GetMotherStack() const { return MotherStack; }
  void SetMotherStack(stack* What) { MotherStack = What; }
  virtual bool IsVisible() const;
  virtual void PutInItem(item*);
  virtual void Load(inputfile&);
 protected:
  stack* MotherStack;
  stackslot* Last;
  stackslot* Next;
};

class bodypartslot : public slot
{
 public:
  virtual void Empty();
  character* GetMaster() const { return Master; }
  void SetMaster(character* What) { Master = What; }
  virtual void AddFriendItem(item*) const;
  virtual square* GetSquareUnder(int = 0) const;
  virtual void SignalVolumeAndWeightChange();
  virtual void SignalEmitationIncrease(color24);
  virtual void SignalEmitationDecrease(color24);
  virtual void PutInItem(item*);
  virtual void Load(inputfile&);
  virtual bool CanBeSeenBy(const character*) const;
  virtual bool IsVisible() const { return false; }
 protected:
  character* Master;
};

class gearslot : public slot
{
 public:
  virtual void Empty();
  bodypart* GetBodyPart() const { return BodyPart; }
  void SetBodyPart(bodypart* What) { BodyPart = What; }
  virtual void AddFriendItem(item*) const;
  void Init(bodypart*, int);
  int GetEquipmentIndex() const { return EquipmentIndex; }
  void SetEquipmentIndex(int What) { EquipmentIndex = What; }
  virtual void PutInItem(item*);
  virtual square* GetSquareUnder(int = 0) const;
  virtual void SignalVolumeAndWeightChange();
  virtual void SignalEmitationIncrease(color24);
  virtual void SignalEmitationDecrease(color24);
  virtual bool CanBeSeenBy(const character*) const;
  virtual void SignalEnchantmentChange();
  virtual bool IsVisible() const { return false; }
  virtual bool IsGearSlot() const { return true; }
 protected:
  bodypart* BodyPart;
  int EquipmentIndex;
};

inline outputfile& operator<<(outputfile& SaveFile, const slot& Slot)
{
  Slot.Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, slot& Slot)
{
  Slot.Load(SaveFile);
  return SaveFile;
}

#endif
