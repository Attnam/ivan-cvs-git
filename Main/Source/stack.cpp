#include "bitmap.h"
#include "igraph.h"
#include "item.h"
#include "stack.h"
#include "strover.h"
#include "list.h"
#include "lsquare.h"
#include "game.h"
#include "proto.h"
#include "message.h"

stack::stack(square* SquareUnder) : SquareUnder(SquareUnder), Item(0), Items(0), NonExistent(0)
{
}

stack::~stack(void)
{
	Clean();
}

void stack::PositionedDrawToTileBuffer(uchar LevelSquarePosition) const
{
	for(ushort c = 0; c < GetItems(); c++)
		if(GetItem(c))
			GetItem(c)->PositionedDrawToTileBuffer(LevelSquarePosition);

	if(GetItems() > 1)
		igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetTileBuffer(), 0, 16, 0, 0, 16, 16);
}

ushort stack::AddItem(item* ToBeAdded)
{
	item** TempItem = new item*[GetItems() + 1];

	for(ushort c = 0; c < GetItems(); c++)
		TempItem[c] = GetItem(c);

	delete [] Item;

	Item = TempItem;

	SetItem(GetItems(), ToBeAdded);

	if(!game::GetInWilderness() && ToBeAdded->GetEmitation() > GetEmitation())
	{
		SetItems(GetItems() + 1);

		GetLevelSquareUnder()->SignalEmitationIncrease(ToBeAdded->GetEmitation());
	}
	else
		SetItems(GetItems() + 1);

	//GGG
	//if(GetLevelSquareUnder()->CanBeSeen())
	//	GetLevelSquareUnder()->UpdateItemMemory();

	return GetItems() - 1;
}

ushort stack::FastAddItem(item* ToBeAdded)
{
	item** TempItem = new item*[Items + 1];

	for(ushort c = 0; c < GetItems(); c++)
		TempItem[c] = GetItem(c);

	delete [] Item;

	Item = TempItem;

	SetItem(GetItems(), ToBeAdded);

	SetItems(GetItems() + 1);

	return GetItems() - 1;
}

item* stack::RemoveItem(ushort Index)
{
	if(Item && Index < GetItems())
	{
		item* Removed;

		ushort IEmit = GetEmitation();

		Removed = Item[Index];

		Item[Index] = 0;

		SNonExistent(CNonExistent() + 1);

		Optimize(0);

		if(IEmit > GetEmitation())
			GetLevelSquareUnder()->SignalEmitationDecrease(IEmit);

		if(GetLevelSquareUnder()->CanBeSeen())
			GetLevelSquareUnder()->UpdateItemMemory();

		return Removed;
	}
	else
		return 0;
}

void stack::FastRemoveItem(ushort Index)
{
	if(Item)
	{
		SetItem(Index, 0);

		SNonExistent(CNonExistent() + 1);

		Optimize(0);
	}
}

void stack::Clean(void)
{
	for(ushort c = 0; c < GetItems(); c++)
		if(GetItem(c))
			delete Item[c];

	delete [] Item;

	Item = 0;

	SetItems(0);

	SNonExistent(0);
}

ushort stack::MoveItem(ushort Index, stack* MoveTo) // Moves item #Index to stack MoveTo
{
	ushort ToBeReturned;
	if(this == MoveTo)
		return Index;
	if(Item && GetItems() > Index && GetItem(Index) && MoveTo)
		if(MoveTo->GetLevelSquareUnder() == GetLevelSquareUnder())
		{
			ToBeReturned = MoveTo->FastAddItem(GetItem(Index));
			FastRemoveItem(Index);

			if(GetLevelSquareUnder()->CanBeSeen())
				GetLevelSquareUnder()->UpdateItemMemory();
		}
		else
		{
			ToBeReturned = MoveTo->AddItem(GetItem(Index));
			RemoveItem(Index);

			if(GetLevelSquareUnder()->CanBeSeen())
				GetLevelSquareUnder()->UpdateItemMemory();

			if(MoveTo->GetLevelSquareUnder()->CanBeSeen())
				MoveTo->GetLevelSquareUnder()->UpdateItemMemory();
		}

	return ToBeReturned;
}

void stack::Optimize(ushort OptimizeBoundary) // Optimizes the inventory
{
	if(CNonExistent() > OptimizeBoundary)
	{
		if(GetItems() - CNonExistent())
		{
			item** TempItem = new item*[GetItems() - NonExistent];

			for(ushort c = 0, i = 0; c < GetItems(); c++)
				if(GetItem(c))
					TempItem[i++] = GetItem(c);

			delete [] Item;

			Item = TempItem;

			SetItems(GetItems() - NonExistent);
		}
		else
		{
			delete [] Item;

			Item = 0;

			SetItems(0);
		}

		SNonExistent(0);
	}
}

ushort stack::DrawContents(const char* Topic) const 	// Draws a list of the items in this stack on the screen
{							// Displays Topic on the screen also...
	if(!GetItems()) return 0xFFFF;
	list ItemNames(Topic);
	ItemNames.AddDescription("");
	ItemNames.AddDescription("Name                                                 Weight       Armor  Strength");
	for(ushort c = 0; c < GetItems(); c++)
	{
		std::string Buffer = GetItem(c)->Name(INDEFINITE);
		Buffer.resize(50,' ');
		Buffer += int(GetItem(c)->GetWeight());
		Buffer.resize(63, ' ');
		Buffer += int(GetItem(c)->GetArmorValue());
		Buffer.resize(70, ' ');
		Buffer += int(GetItem(c)->GetWeaponStrength());

		ItemNames.AddString(Buffer);
	}
	return ItemNames.Draw();
}

ushort stack::GetEmitation(void) const // Calculates the biggest light emmision of the levelsquare...
{
	ushort Emitation = 0;

	for(ushort c = 0; c < GetItems(); c++)
		if(GetItem(c))
			if(GetItem(c)->GetEmitation() > Emitation)
				Emitation = GetItem(c)->GetEmitation();

	return Emitation;
}

ulong stack::SumOfMasses(void) const
{
	ulong Sum = 0;

	for(ushort c = 0; c < GetItems(); c++)
		Sum += GetItem(c)->GetWeight();

	return Sum;
}

void stack::Save(std::ofstream* SaveFile) const
{
	SaveFile->write((char*)&Items, sizeof(Items));

	for(ushort c = 0; c < Items; c++)
		Item[c]->Save(SaveFile);
}

void stack::Load(std::ifstream* SaveFile)
{
	SaveFile->read((char*)&Items, sizeof(Items));

	if(Items)
	{
		Item = new item*[Items];

		for(ushort c = 0; c < Items; c++)
			Item[c] = prototypesystem::LoadItem(SaveFile);
	}
	else
		Item = 0;

	NonExistent = 0;
}

ushort stack::SearchItem(item* ToBeSearched) const
{
	for(ushort c = 0; c < GetItems(); c++)
		if(GetItem(c) == ToBeSearched)
			return c;

	return 0xFFFF;
}

void stack::Move(levelsquare* To)
{
	GetLevelSquareUnder()->SignalEmitationDecrease(GetEmitation());
	SetSquareUnder(To);
	GetLevelSquareUnder()->SignalEmitationIncrease(GetEmitation());
}

vector stack::GetPos(void) const
{
	return GetLevelSquareUnder()->GetPos();
}

ushort stack::ConsumableItems(character* Eater)
{
	ushort Counter = 0;

	for(ushort c = 0; c < GetItems(); c++)
	{
		if(GetItem(c)->Consumable(Eater))
			Counter++;
	}

	return Counter;
}

ushort stack::DrawConsumableContents(const char* Topic, character* Eater) const
{
	stack ConsumableStack;
	item* TheItem;
	ushort Key;
	for(ushort c = 0; c < GetItems(); c++)
	{
		if(GetItem(c)->Consumable(Eater))
			ConsumableStack.FastAddItem(GetItem(c));
	}
	Key = ConsumableStack.DrawContents(Topic);

	if(Key == 0xFFFF || Key == 0x1B || Key > ConsumableStack.GetItems())
	{
		ConsumableStack.DeletePointers();
		return 0xFFFF;
	}

	TheItem = ConsumableStack.GetItem(Key);
	ConsumableStack.DeletePointers();
	return SearchItem(TheItem);

}

void stack::DeletePointers(void)
{
	while(GetItems())
		FastRemoveItem(0);
}

void stack::StackMerge(stack* ToBeMerged)
{
	for(ushort c = 0; c < ToBeMerged->GetItems(); c++)
		FastAddItem(ToBeMerged->GetItem(c));

	ToBeMerged->DeletePointers();
}

void stack::Kick(ushort Strength, bool ShowOnScreen, uchar Direction)
{
	if(Strength > 3)
		for(ushort c = 0; c < GetItems(); c++)
		{
			GetItem(c)->ImpactDamage(Strength >> 1, ShowOnScreen, this);
			GetItem(c)->Fly(Direction, Strength, this, true);
		}
	else
		if(ShowOnScreen) ADD_MESSAGE("Your weak kick does no damage.");
}

long stack::Score(void) const
{
	long Score = 0;

	for(ushort c = 0; c < GetItems(); c++)
		Score += GetItem(c)->Score();

	return Score;
}

void stack::SetSquareUnder(square* Square)
{
	SquareUnder = Square;
}
