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
	void PositionedDrawToTileBuffer(uchar = CENTER);
	ushort AddItem(item*);
	ushort FastAddItem(item*);
	item* RemoveItem(ushort);
	void FastRemoveItem(ushort);
	item** CItem(void)	{ return Item; }
	item* CItem(ushort I)	{ return Item[I]; }
	ushort CItems(void)	{ return Items; }
	void SSquareUnder(square*);
	ushort DrawContents(const char*);
	void DrawPartOfContents(ushort, ushort, bool, const char*);
	ushort MoveItem(ushort, stack*);
	ushort CEmitation(void);
	vector CPos(void);
	void Clean(void);
	ulong SumOfMasses(void);
	void Save(std::ofstream*);
	ushort SearchItem(item*);
	void Move(levelsquare*);
	square* CSquareUnder(void) { return SquareUnder; }
	levelsquare* CLevelSquareUnder(void) { return (levelsquare*)SquareUnder; }
	void SItem(ushort Where, item* What) { Item[Where] = What; }
	void SEmitation(ushort What) { Emitation = What; }
	void SItems(ushort What) { Items = What; }
	ushort CNonExistent(void) { return NonExistent; }
	void SNonExistent(ushort What) { NonExistent = What; }
	ushort ConsumableItems(character*);
	void DrawItemData(ushort, ushort);
	ushort DrawConsumableContents(const char*, character*);
	void DeletePointers(void);
	void StackMerge(stack);
	ushort FindItem(item* ToBeSearched);
	ushort MultiselectDrawContents(const char*);
	void Kick(ushort, bool, uchar);
	long Score(void);
private:
	void Optimize(ushort);
	square* SquareUnder;
	item** Item;
	ushort Items, NonExistent, Emitation;
};

#endif
