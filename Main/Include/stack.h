#ifndef __STACK_H__
#define __STACK_H__

#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3

#define CENTER 4

#include <fstream>

#include "typedef.h"
#include "vector.h"

class item;
class character;
class levelsquare;
class vector;
class bitmap;
class square;

/* Presentation of the stack class */

class stack
{
public:
	stack(square* = 0);
	stack(std::ifstream*);
	~stack(void);
	void PositionedDrawToTileBuffer(uchar = CENTER) const;
	ushort AddItem(item*);
	ushort FastAddItem(item*);
	item* RemoveItem(ushort);
	void FastRemoveItem(ushort);
	item* GetItem(ushort I) const	{ return Item[I]; }
	ushort GetItems(void) const	{ return Items; }
	void SetSquareUnder(square*);
	ushort DrawContents(const char*) const;
	void DrawPartOfContents(ushort, ushort, bool, const char*) const;
	ushort MoveItem(ushort, stack*);
	ushort GetEmitation(void) const;
	vector GetPos(void) const;
	void Clean(void);
	ulong SumOfMasses(void) const;
	void Save(std::ofstream*) const;
	ushort SearchItem(item*) const;
	void Move(levelsquare*);
	square* GetSquareUnder(void) const { return SquareUnder; }
	levelsquare* GetLevelSquareUnder(void) const { return (levelsquare*)SquareUnder; }
	void SetItem(ushort Where, item* What) { Item[Where] = What; }
	void SetItems(ushort What) { Items = What; }
	ushort CNonExistent(void) const { return NonExistent; }
	void SNonExistent(ushort What) { NonExistent = What; }
	ushort ConsumableItems(character*);
	void DrawItemData(ushort, ushort) const;
	ushort DrawConsumableContents(const char*, character*) const;
	void DeletePointers(void);
	void StackMerge(stack*);
	ushort MultiselectDrawContents(const char*) const;
	void Kick(ushort, bool, uchar);
	long Score(void) const;
private:
	void Optimize(ushort);
	square* SquareUnder;
	item** Item;
	ushort Items, NonExistent;
};

#endif
