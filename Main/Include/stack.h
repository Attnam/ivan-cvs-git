#ifndef __STACK_H__
#define __STACK_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>
#include <string>

#include "typedef.h"
#include "vector2d.h"
#include "ivandef.h"
#include "lsquare.h"
#include "slot.h"

class item;
class character;
class bitmap;
class outputfile;
class inputfile;
class felist;
class entity;

typedef std::vector<item*> itemvector;

class stackiterator
{
public:
  stackiterator(stackslot* Slot) : Slot(Slot) { }
  stackiterator& operator++() { Slot = Slot->Next; return *this; }
  bool HasItem() const { return Slot != 0; }
  item* operator->() const { return Slot->Item; }
  item* operator*() const { return Slot->Item; }
  const stackslot& GetSlot() const { return *Slot; }
private:
  stackslot* Slot;
};

class stack
{
 public:
  stack(square*, entity*, uchar, bool);
  ~stack();
  void Load(inputfile&);
  void Draw(const character*, bitmap*, vector2d, ulong, bool, bool, bool) const;
  void AddItem(item*);
  void RemoveItem(stackslot*);
  item* GetItem(ushort) const;
  stackiterator GetBottom() const { return stackiterator(Bottom); }
  stackiterator GetTop() const { return stackiterator(Top); }
  ushort GetItems() const { return Items; }
  ushort GetVisibleItems(const character*) const;
  ushort GetItems(const character*, bool) const;
  void SetMotherSquare(square* What) { MotherSquare = What; }
  item* DrawContents(const character*, const std::string&, bool = true, bool = true, bool (*)(item*, const character*) = 0) const;
  item* DrawContents(stack*, const character*, const std::string&, const std::string&, const std::string&, bool = true, bool = true, bool (*)(item*, const character*) = 0) const;
  item* MoveItem(stackslot*, stack*);
  vector2d GetPos() const;
  void Clean(bool = false);
  void Save(outputfile&) const;
  ushort SearchItem(item*) const;
  square* GetSquareUnder() const;
  lsquare* GetLSquareUnder() const { return static_cast<lsquare*>(GetSquareUnder()); }
  bool SortedItems(const character*, bool (*)(item*, const character*)) const;
  void BeKicked(character*, ushort);
  long Score() const;
  void Polymorph();
  void CheckForStepOnEffect(character*);
  square* GetSquareTrulyUnder() const;
  lsquare* GetLSquareTrulyUnder() const { return static_cast<lsquare*>(GetSquareTrulyUnder()); }
  void ReceiveDamage(character*, ushort, uchar);
  void TeleportRandomly();
  void FillItemVector(itemvector&) const;
  void AddContentsToList(felist&, const character*, const std::string&, bool, bool (*)(item*, const character*)) const;
  item* SearchChosen(ushort&, ushort, const character*, bool (*)(item*, const character*)) const;
  bool IsOnGround() const { return SquarePosition != HIDDEN; }
  bool RaiseTheDead(character*);
  bool TryKey(item*, character*);
  bool Open(character*);
  void SignalVolumeAndWeightChange();
  void CalculateVolumeAndWeight();
  ulong GetVolume() const { return Volume; }
  ulong GetWeight() const { return Weight; }
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  area* GetAreaUnder() const { return GetSquareUnder()->GetAreaUnder(); }
  square* GetNearSquare(vector2d Pos) const { return GetSquareUnder()->GetAreaUnder()->GetSquare(Pos); }
  ulong GetEmitation() const { return Emitation; }
  void SignalEmitationIncrease(ulong);
  void SignalEmitationDecrease(ulong);
  void CalculateEmitation();
  bool CanBeSeenBy(const character*) const;
  bool IsDangerousForAIToStepOn(const character*) const;
  bool Clone(ushort);
  void MoveItemsTo(stack*);
  item* GetBottomVisibleItem(const character*) const;
  item* GetBottomItem(const character*, bool) const;
  void Pile(std::vector<std::vector<item*> >&, const character*, bool (*)(item*, const character*)) const;
  ulong GetPrice() const;
  ulong GetTotalExplosivePower() const;
 private:
  void AddElement(item*);
  void RemoveElement(stackslot*);
  stackslot* Bottom;
  stackslot* Top;
  square* MotherSquare;
  entity* MotherEntity;
  uchar SquarePosition;
  ulong Volume;
  ulong Weight;
  ulong Emitation;
  ushort Items;
  bool IgnoreVisibility;
};

#endif

