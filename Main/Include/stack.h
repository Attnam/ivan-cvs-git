#ifndef __STACK_H__
#define __STACK_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3

#define CENTER 4

#include "typedef.h"
#include "vector2d.h"

class item;
class character;
class levelsquare;
class bitmap;
class square;
class outputfile;
class inputfile;

/* Presentation of the stack class */

class stack
{
 public:
  stack(square* = 0, uchar = CENTER);
  ~stack();
  void Load(inputfile&);
  bool DrawToTileBuffer() const;
  ushort AddItem(item*);
  ushort FastAddItem(item*);
  item* RemoveItem(ushort);
  void FastRemoveItem(ushort);
  item* GetItem(ushort I) const { return Item[I]; }
  ushort GetItems() const { return Items; }
  void SetSquareUnder(square*);
  ushort DrawContents(character*, std::string) const;
  void DrawPartOfContents(ushort, ushort, bool, const char*) const;
  item* MoveItem(ushort, stack*);
  ushort GetEmitation() const;
  vector2d GetPos() const;
  void Clean();
  ulong SumOfMasses() const;
  void Save(outputfile&) const;
  ushort SearchItem(item*) const;
  square* GetSquareUnder() const { return SquareUnder; }
  levelsquare* GetLevelSquareUnder() const { return (levelsquare*)SquareUnder; }
  void SetItem(ushort Where, item* What) { Item[Where] = What; }
  void SetItems(ushort What) { Items = What; }
  ushort CNonExistent() const { return NonExistent; }
  void SNonExistent(ushort What) { NonExistent = What; }
  ushort ConsumableItems(character*);
  void DrawItemData(ushort, ushort) const;
  ushort DrawConsumableContents(character*, std::string) const;
  void DeletePointers();
  void StackMerge(stack*);
  ushort MultiselectDrawContents(const char*) const;
  void Kick(ushort, bool, uchar);
  long Score() const;
  bool Polymorph();
  void ReceiveSound(float);
  void StruckByWandOfStriking(character*, std::string);
  void CheckForStepOnEffect(character*);
  square* GetSquareTrulyUnder() const;
  levelsquare* GetLevelSquareTrulyUnder() const { return (levelsquare*)GetSquareTrulyUnder(); }
  void ImpactDamage(ushort, bool);
  void ReceiveFireDamage(character*, std::string, long);
 private:
  void Optimize(ushort);
  square* SquareUnder;
  item** Item;
  ushort Items, NonExistent;
  uchar SquarePosition;
};

#endif



