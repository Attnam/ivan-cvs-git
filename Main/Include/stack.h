#ifndef __STACK_H__
#define __STACK_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>
#include <vector>
#include <string>

#include "typedef.h"
#include "vector2d.h"
#include "ivandef.h"
#include "lsquare.h"

class item;
class character;
class bitmap;
class outputfile;
class inputfile;
class stackslot;
class felist;
class entity;

typedef std::list<stackslot*> stacklist;
typedef std::list<stackslot*>::iterator stackiterator;
typedef std::vector<item*> itemvector;

class stack
{
 public:
  stack(square*, entity*, uchar);
  ~stack();
  void Load(inputfile&);
  void Draw(bitmap*, vector2d, ushort, bool, bool, bool) const;
  void AddItem(item*);
  void FastAddItem(item*);
  void RemoveItem(stackiterator);
  void FastRemoveItem(stackiterator);
  item* GetItem(ushort) const;
  stackiterator GetBottomSlot() const;
  stackiterator GetSlotAboveTop() const;
  item* GetBottomItem() const;
  item* GetBottomVisibleItem() const;
  ushort GetItems() const { return Item->size(); }
  ushort GetVisibleItems() const;
  void SetMotherSquare(square* What) { MotherSquare = What; }
  item* DrawContents(character*, const std::string&, bool (*)(item*, character*) = 0) const;
  item* DrawContents(character*, const std::string&, bool, bool (*)(item*, character*) = 0) const;
  item* DrawContents(stack*, character*, const std::string&, const std::string&, const std::string&, bool (*)(item*, character*) = 0) const;
  item* DrawContents(stack*, character*, const std::string&, const std::string&, const std::string&, bool, bool (*)(item*, character*) = 0) const;
  item* MoveItem(stackiterator, stack*);
  vector2d GetPos() const;
  void Clean(bool = false);
  void Save(outputfile&) const;
  ushort SearchItem(item*) const;
  square* GetSquareUnder() const;
  lsquare* GetLSquareUnder() const { return static_cast<lsquare*>(GetSquareUnder()); }
  bool SortedItems(character*, bool (*)(item*, character*)) const;
  void DeletePointers();
  void BeKicked(character*, ushort);
  long Score() const;
  void Polymorph();
  void CheckForStepOnEffect(character*);
  square* GetSquareTrulyUnder() const;
  lsquare* GetLSquareTrulyUnder() const { return static_cast<lsquare*>(GetSquareTrulyUnder()); }
  void ReceiveDamage(character*, ushort, uchar);
  void TeleportRandomly();
  void FillItemVector(itemvector&) const;
  void AddContentsToList(felist&, character*, const std::string&, bool, bool (*)(item*, character*)) const;
  item* SearchChosen(ushort&, ushort, character*, bool (*)(item*, character*)) const;
  bool IsOnGround() const { return SquarePosition != HIDDEN; }
  bool RaiseTheDead(character*);
  bool TryKey(item*, character*);
  bool Open(character*);
  void MoveAll(stack*);
  void SignalVolumeAndWeightChange();
  void CalculateVolumeAndWeight();
  ulong GetVolume() const { return Volume; }
  ulong GetWeight() const { return Weight; }
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  area* GetAreaUnder() const { return GetSquareUnder()->GetAreaUnder(); }
  square* GetNearSquare(vector2d Pos) const { return GetSquareUnder()->GetAreaUnder()->GetSquare(Pos); }
  ushort GetEmitation() const { return Emitation; }
  void SignalEmitationIncrease(ushort);
  void SignalEmitationDecrease(ushort);
  void CalculateEmitation();
 private:
  stacklist* Item;
  square* MotherSquare;
  uchar SquarePosition;
  entity* MotherEntity;
  ulong Volume;
  ulong Weight;
  ushort Emitation;
};

#endif
