#include <cstdlib>

#include "graphics.h"
#include "bitmap.h"
#include "igraph.h"
#include "item.h"
#include "stack.h"
#include "strover.h"
#include "list.h"
#include "whandler.h"
#include "lsquare.h"

stack::stack(square* SquareUnder) : SquareUnder(SquareUnder), Item(0), Items(0), NonExistent(0), Emitation(0)
{
	for(ushort c = 0; c < CItems(); c++)		// What in dev/null this does? Who wrote this?
		SItem(c, 0);				//   Confess immediately!
}

stack::~stack(void)
{
	Clean();
}

void stack::PositionedDrawToTileBuffer(uchar LevelSquarePosition)
{
	for(ushort c = 0; c < CItems(); c++)
		if(CItem()[c])
			CItem()[c]->PositionedDrawToTileBuffer(LevelSquarePosition);

	if(CItems() > 1)
		igraph::CSymbolGraphic()->MaskedBlit(igraph::CTileBuffer(), 0, 16, 0, 0, 16, 16);
}

ushort stack::AddItem(item* ToBeAdded)
{
	item** TempItem = new item*[CItems() + 1];

	for(ushort c = 0; c < CItems(); c++)
		TempItem[c] = CItem()[c];

	delete [] Item;

	Item = TempItem;

	SItem(CItems(), ToBeAdded);

	if(ToBeAdded->CEmitation() > CEmitation())
	{
		SItems(CItems() + 1);

		CLevelSquareUnder()->SignalEmitationIncrease(ToBeAdded->CEmitation());
	}
	else
		SItems(CItems() + 1);

	if(CLevelSquareUnder()->CanBeSeen())
		CLevelSquareUnder()->UpdateItemMemory();

	return CItems() - 1;
}

ushort stack::FastAddItem(item* ToBeAdded)
{
	item** TempItem = new item*[Items + 1];

	for(ushort c = 0; c < CItems(); c++)
		TempItem[c] = CItem()[c];

	delete [] Item;

	Item = TempItem;

	SItem(CItems(), ToBeAdded);

	SItems(CItems() + 1);

	return CItems() - 1;
}

item* stack::RemoveItem(ushort Index)
{
	if(CItem() && Index < CItems())
	{
		item* Removed;

		ushort IEmit = Item[Index]->CEmitation();

		Removed = Item[Index];

		Item[Index] = 0;

		SNonExistent(CNonExistent() + 1);

		Optimize(0);

		if(IEmit == CEmitation())
		{
			Emitation = CEmitation();

			if(IEmit != CEmitation())
				CLevelSquareUnder()->SignalEmitationDecrease(IEmit);
		}

		if(CLevelSquareUnder()->CanBeSeen())
			CLevelSquareUnder()->UpdateItemMemory();

		return Removed;
	}
	else
		return 0;
}

void stack::FastRemoveItem(ushort Index)
{
	if(CItem())
	{
		SItem(Index, 0);

		SNonExistent(CNonExistent() + 1);

		Optimize(0);
	}
}

void stack::Clean(void)
{
	for(ushort c = 0; c < CItems(); c++)
		if(CItem()[c])
			delete Item[c];

	delete [] Item;

	Item = 0;

	SItems(0);

	SNonExistent(0);
}

ushort stack::MoveItem(ushort Index, stack* MoveTo) // Moves item #Index to stack MoveTo
{
	ushort ToBeReturned;
	if(this == MoveTo)
		return Index;
	if(CItem() && CItems() > Index && CItem()[Index] && MoveTo)
		if(MoveTo->CLevelSquareUnder() == CLevelSquareUnder())
		{
			ToBeReturned = MoveTo->FastAddItem(CItem()[Index]);
			FastRemoveItem(Index);

			if(CLevelSquareUnder()->CanBeSeen())
				CLevelSquareUnder()->UpdateItemMemory();
		}
		else
		{
			ToBeReturned = MoveTo->AddItem(CItem()[Index]);
			RemoveItem(Index);

			if(CLevelSquareUnder()->CanBeSeen())
				CLevelSquareUnder()->UpdateItemMemory();

			if(MoveTo->CLevelSquareUnder()->CanBeSeen())
				MoveTo->CLevelSquareUnder()->UpdateItemMemory();
		}

	return ToBeReturned;
}

void stack::Optimize(ushort OptimizeBoundary) // Optimizes the inventory
{
	if(CNonExistent() > OptimizeBoundary)
	{
		if(CItems() - CNonExistent())
		{
			item** TempItem = new item*[CItems() - NonExistent];

			for(ushort c = 0, i = 0; c < CItems(); c++)
				if(CItem()[c])
					TempItem[i++] = CItem()[c];

			delete [] Item;

			Item = TempItem;

			SItems(CItems() - NonExistent);
		}
		else
		{
			delete [] Item;

			Item = 0;

			SItems(0);
		}

		SNonExistent(0);
	}
}

ushort stack::DrawContents(const char* Topic) 	// Draws a list of the items in this stack on the screen
{							// Displays Topic on the screen also...
	if(!CItems()) return 0xFFFF;
	list ItemNames(Topic);
	ItemNames.AddDescription("");
	ItemNames.AddDescription("Name                                                 Weight       Armor  Strength");
	for(ushort c = 0; c < CItems(); c++)
	{
		std::string Buffer = CItem()[c]->Name(INDEFINITE);
		Buffer.resize(50,' ');
		Buffer += int(CItem()[c]->CWeight());
		Buffer.resize(63, ' ');
		Buffer += int(CItem()[c]->GetArmorValue());
		Buffer.resize(70, ' ');
		Buffer += int(CItem()[c]->GetWeaponStrength());

		ItemNames.AddString(Buffer);
	}
	return ItemNames.Draw();
}

ushort stack::CEmitation(void) // Calculates the biggest light emmision of the levelsquare...
{
	Emitation = 0;

	for(ushort c = 0; c < CItems(); c++)
		if(CItem()[c])
			if(CItem()[c]->CEmitation() > Emitation)
				Emitation = CItem()[c]->CEmitation();

	return Emitation;
}

ulong stack::SumOfMasses(void)
{
	ulong Sum = 0;
	for(ushort c = 0; c < CItems(); c++)
		Sum += CItem()[c]->CWeight();
	return Sum;
}

void stack::Save(std::ofstream* SaveFile)
{
	SaveFile->write((char*)&Items, sizeof(Items));

	for(ushort c = 0; c < Items; c++)
		Item[c]->Save(SaveFile);
}

stack::stack(std::ifstream* SaveFile)
{
	SaveFile->read((char*)&Items, sizeof(Items));

	if(Items)
	{
		Item = new item*[Items];

		for(ushort c = 0; c < Items; c++)
			Item[c] = game::LoadItem(SaveFile);
	}
	else
		Item = 0;

	NonExistent = 0;
}

ushort stack::SearchItem(item* ToBeSearched)
{
	for(ushort c = 0; c < CItems(); c++)
		if(CItem()[c] == ToBeSearched)
			return c;

	return 0xFFFF;
}

void stack::Move(levelsquare* To)
{
	CLevelSquareUnder()->SignalEmitationDecrease(Emitation);
	SSquareUnder(To);
	CLevelSquareUnder()->SignalEmitationIncrease(Emitation);
}

vector stack::CPos(void)
{
	return CLevelSquareUnder()->CPos();
}

ushort stack::ConsumableItems(character* Eater)
{
	ushort Counter = 0;
	for(ushort c = 0; c < CItems(); c++)
	{
		if(CItem()[c]->Consumable(Eater))
			Counter++;
	}
	return Counter;
}

ushort stack::DrawConsumableContents(const char* Topic, character* Eater)
{
	stack ConsumableStack;
	item* TheItem;
	ushort Key;
	for(ushort c = 0; c < CItems(); c++)
	{
		if(CItem()[c]->Consumable(Eater))
			ConsumableStack.FastAddItem(CItem()[c]);
	}
	Key = ConsumableStack.DrawContents(Topic);

	if(Key == 0xFFFF || Key == 0x1B || Key > ConsumableStack.CItems())
	{
		ConsumableStack.DeletePointers();
		return 0xFFFF;
	}

	TheItem = ConsumableStack.CItem()[Key];
	ConsumableStack.DeletePointers();
	return FindItem(TheItem);

}




void stack::DeletePointers(void)
{
	while(CItems())
		FastRemoveItem(0);
}


void stack::StackMerge(stack ToBeMerged)
{
	for(ushort c = 0; c < ToBeMerged.CItems(); c++)
	{
		FastAddItem(ToBeMerged.CItem()[c]);
	}
	ToBeMerged.DeletePointers();
}

ushort stack::FindItem(item* ToBeSearched)
{
	for(ushort c = 0; c < CItems(); c++)
	{
		if(CItem()[c] == ToBeSearched)
			return c;
	}
	return 0xFFFF;
}


void stack::Kick(ushort Strength, bool ShowOnScreen, uchar Direction)
{
	if(Strength > 3)
		for(ushort c = 0; c < CItems(); c++)
		{
			CItem()[c]->ImpactDamage(Strength >> 1, ShowOnScreen, this);
			CItem(c)->Fly(Direction, Strength, this, true);
		}
	else
		if(ShowOnScreen) ADD_MESSAGE("Your weak kick does no damage.");
}

long stack::Score(void)
{
	long Score = 0;

	for(ushort c = 0; c < CItems(); c++)
		Score += CItem(c)->Score();

	return Score;
}
