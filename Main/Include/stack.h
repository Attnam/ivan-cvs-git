#ifndef __STACK_H__
#define __STACK_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "lsquare.h"
#include "slot.h"

typedef std::vector<item*> itemvector;

class felist;
class entity;

class stackiterator
{
public:
  stackiterator(stackslot* Slot) : Slot(Slot) { }
  stackiterator& operator++() { Slot = Slot->Next; return *this; }
  stackiterator& operator--() { Slot = Slot->Last; return *this; }
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
  item* DrawContents(const character*, const std::string&, uchar = 0, bool (*)(const item*, const character*) = 0) const;
  ushort DrawContents(std::vector<item*>&, const character*, const std::string&, uchar = 0, bool (*)(const item*, const character*) = 0) const;
  ushort DrawContents(std::vector<item*>&, stack*, const character*, const std::string&, const std::string&, const std::string&, uchar = 0, bool (*)(const item*, const character*) = 0) const;
  item* MoveItem(stackslot*, stack*);
  vector2d GetPos() const;
  void Clean(bool = false);
  void Save(outputfile&) const;
  ushort SearchItem(item*) const;
  square* GetSquareUnder() const;
  lsquare* GetLSquareUnder() const { return static_cast<lsquare*>(GetSquareUnder()); }
  bool SortedItems(const character*, bool (*)(const item*, const character*)) const;
  void BeKicked(character*, ushort);
  long GetScore() const;
  void Polymorph();
  void CheckForStepOnEffect(character*);
  square* GetSquareTrulyUnder() const;
  lsquare* GetLSquareTrulyUnder() const { return static_cast<lsquare*>(GetSquareTrulyUnder()); }
  void ReceiveDamage(character*, ushort, uchar);
  void TeleportRandomly();
  void FillItemVector(itemvector&) const;
  void AddContentsToList(felist&, const character*, const std::string&, uchar, bool (*)(const item*, const character*)) const;
  ushort SearchChosen(std::vector<item*>&, const character*, ushort, ushort, uchar, bool (*)(const item*, const character*) = 0) const;
  bool IsOnGround() const;
  bool RaiseTheDead(character*);
  bool TryKey(item*, character*);
  bool Open(character*);
  void SignalVolumeAndWeightChange();
  void CalculateVolumeAndWeight();
  ulong GetVolume() const { return Volume; }
  ulong GetWeight() const { return Weight; }
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  area* GetArea() const { return GetSquareUnder()->GetArea(); }
  square* GetNearSquare(vector2d Pos) const { return GetSquareUnder()->GetArea()->GetSquare(Pos); }
  ulong GetEmitation() const { return Emitation; }
  void SignalEmitationIncrease(ulong);
  void SignalEmitationDecrease(ulong);
  void CalculateEmitation();
  bool CanBeSeenBy(const character*) const;
  bool IsDangerousForAIToStepOn(const character*) const;
  bool Clone(ushort);
  void MoveItemsTo(stack*);
  void MoveItemsTo(slot*);
  item* GetBottomVisibleItem(const character*) const;
  item* GetBottomItem(const character*, bool) const;
  void Pile(std::vector<std::vector<item*> >&, const character*, bool (*)(const item*, const character*) = 0) const;
  ulong GetPrice() const;
  ulong GetTotalExplosivePower() const;
  void ReceiveFluidSpill(material*);
  static ushort GetSelected() { return Selected; }
  static void SetSelected(ushort What) { Selected = What; }
  bool TakeSomethingFrom(character*, const std::string);
  bool PutSomethingIn(character*, const std::string, ulong, ulong);
  bool IsVisible() const { return !MotherEntity; }
 private:
  void AddElement(item*);
  void RemoveElement(stackslot*);
  static ushort Selected;
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
