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

class item;
class character;
class lsquare;
class bitmap;
class square;
class outputfile;
class inputfile;
class stackslot;
class felist;
class entity;

typedef std::list<stackslot*> stacklist;
typedef std::list<stackslot*>::iterator stackiterator;
typedef std::vector<item*> itemvector;

/* Presentation of the stack class */

class stack
{
 public:
  stack(square* = 0, entity* = 0, uchar = CENTER);
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
  void SetSquareUnder(square*);
  item* DrawContents(character*, const std::string&, bool (*)(item*, character*) = 0) const;
  item* DrawContents(character*, const std::string&, bool, bool (*)(item*, character*) = 0) const;
  item* DrawContents(stack*, character*, const std::string&, const std::string&, const std::string&, bool (*)(item*, character*) = 0) const;
  item* DrawContents(stack*, character*, const std::string&, const std::string&, const std::string&, bool, bool (*)(item*, character*) = 0) const;
  item* MoveItem(stackiterator, stack*);
  ushort GetEmitation() const;
  vector2d GetPos() const;
  void Clean(bool = true);
  //ulong GetTotalWeight() const;
  void Save(outputfile&) const;
  ushort SearchItem(item*) const;
  square* GetSquareUnder() const { return SquareUnder; }
  lsquare* GetLSquareUnder() const;
  bool SortedItems(character*, bool (*)(item*, character*)) const;
  void DeletePointers();
  void BeKicked(character*, float);
  long Score() const;
  void Polymorph();
  void CheckForStepOnEffect(character*);
  square* GetSquareTrulyUnder() const;
  lsquare* GetLSquareTrulyUnder() const;
  void ReceiveDamage(character*, short, uchar);
  void TeleportRandomly();
  void FillItemVector(itemvector&) const;
  void AddContentsToList(felist&, character*, const std::string&, bool, bool (*)(item*, character*)) const;
  item* SearchChosen(ushort&, ushort, character*, bool (*)(item*, character*)) const;
  bool IsOnGround() const { return SquarePosition != HIDDEN; }
  bool RaiseTheDead(character*);
  bool TryKey(item*, character*);
  bool Open(character*);
  //ulong GetTotalVolume() const;
  void MoveAll(stack*);

  ulong GetVolume() const { return Volume; }
  void SetVolume(ulong What) { Volume = What; }
  ulong GetWeight() const { return Weight; }
  void SetWeight(ulong What) { Weight = What; }

  void EditVolume(long);
  void EditWeight(long);

  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }

 private:
  stacklist* Item;
  square* SquareUnder;
  uchar SquarePosition;

  ulong Volume;
  ulong Weight;
  entity* MotherEntity;
};

#endif
