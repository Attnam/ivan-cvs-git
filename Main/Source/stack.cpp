#include "itemba.h"
#include "stack.h"
#include "strover.h"
#include "felist.h"
#include "lsquare.h"
#include "message.h"
#include "graphics.h"
#include "charba.h"
#include "area.h"
#include "femath.h"

stack::stack(square* SquareUnder, uchar SquarePosition) : SquareUnder(SquareUnder), Item(0), Items(0), NonExistent(0), SquarePosition(SquarePosition)
{
}

stack::~stack()
{
  Clean();
}

bool stack::DrawToTileBuffer() const
{
  if(!GetItems())
    return false;

  for(ushort c = 0; c < GetItems(); ++c)
    GetItem(c)->PositionedDrawToTileBuffer(SquarePosition);

  return true;
}

ushort stack::AddItem(item* ToBeAdded)
{
  item** TempItem = new item*[GetItems() + 1];

  for(ushort c = 0; c < GetItems(); ++c)
    TempItem[c] = GetItem(c);

  delete [] Item;

  Item = TempItem;

  SetItem(GetItems(), ToBeAdded);

  ToBeAdded->SetSquareUnder(GetSquareUnder());

  SetItems(GetItems() + 1);

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();
    }

  if(GetSquareTrulyUnder())
    {
      if(!game::GetInWilderness())
	GetLevelSquareTrulyUnder()->SignalEmitationIncrease(ToBeAdded->GetEmitation());

      GetSquareTrulyUnder()->SendNewDrawRequest();
      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();

      if(GetSquareTrulyUnder()->CanBeSeen())
	GetSquareTrulyUnder()->UpdateMemorized();
    }

  return GetItems() - 1;
}

ushort stack::FastAddItem(item* ToBeAdded)
{
  item** TempItem = new item*[Items + 1];

  for(ushort c = 0; c < GetItems(); ++c)
    TempItem[c] = GetItem(c);

  delete [] Item;

  Item = TempItem;

  SetItem(GetItems(), ToBeAdded);

  ToBeAdded->SetSquareUnder(GetSquareUnder());

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

      if(GetSquareUnder())
	{
	  GetSquareUnder()->SetDescriptionChanged(true);

	  if(GetSquareUnder()->CanBeSeen())
	    GetSquareUnder()->UpdateMemorizedDescription();
	}

      if(GetSquareTrulyUnder())
	{
	  if(!game::GetInWilderness())
	    GetLevelSquareTrulyUnder()->SignalEmitationDecrease(IEmit);

	  GetSquareTrulyUnder()->SendNewDrawRequest();
	  GetSquareTrulyUnder()->SendMemorizedUpdateRequest();

	  if(GetSquareTrulyUnder()->CanBeSeen())
	    GetSquareTrulyUnder()->UpdateMemorized();
	}

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

void stack::Clean()
{
  for(ushort c = 0; c < GetItems(); ++c)
    if(GetItem(c))
      delete Item[c];

  delete [] Item;

  Item = 0;

  SetItems(0);

  SNonExistent(0);
}

item* stack::MoveItem(ushort Index, stack* MoveTo) // Moves item #Index to stack MoveTo
{
  ushort ToBeReturned = 0;

  if(this == MoveTo)
    return GetItem(Index);

  if(GetSquareTrulyUnder())
    {
      GetSquareTrulyUnder()->SendNewDrawRequest();
      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();
    }

  if(GetSquareUnder())
    GetSquareUnder()->SetDescriptionChanged(true);

  if(Item && GetItems() > Index && GetItem(Index) && MoveTo)
    if(MoveTo->GetLevelSquareTrulyUnder() == GetLevelSquareTrulyUnder())
      {
	ToBeReturned = MoveTo->FastAddItem(GetItem(Index));
	FastRemoveItem(Index);

	if(GetSquareUnder() && GetSquareUnder()->CanBeSeen())
	  GetSquareUnder()->UpdateMemorizedDescription();

	if(GetSquareTrulyUnder() && GetSquareTrulyUnder()->CanBeSeen())
	  GetSquareTrulyUnder()->UpdateMemorized();
      }
    else
      {
	if(MoveTo->GetSquareTrulyUnder())
	  {
	    MoveTo->GetSquareTrulyUnder()->SendNewDrawRequest();
	    MoveTo->GetSquareTrulyUnder()->SendMemorizedUpdateRequest();
	  }

	if(MoveTo->GetSquareUnder())
	  MoveTo->GetSquareUnder()->SetDescriptionChanged(true);

	ToBeReturned = MoveTo->AddItem(GetItem(Index));
	RemoveItem(Index);

	if(GetSquareUnder() && GetSquareUnder()->CanBeSeen())
	  GetSquareUnder()->UpdateMemorizedDescription();

	if(MoveTo->GetSquareUnder() && MoveTo->GetSquareUnder()->CanBeSeen())
	  MoveTo->GetSquareUnder()->UpdateMemorizedDescription();

	if(GetSquareTrulyUnder() && GetSquareTrulyUnder()->CanBeSeen())
	  GetSquareTrulyUnder()->UpdateMemorized();
      }

  return MoveTo->GetItem(ToBeReturned);
}

void stack::Optimize(ushort OptimizeBoundary)
{
  if(CNonExistent() > OptimizeBoundary)
    {
      if(GetItems() - CNonExistent())
	{
	  item** TempItem = new item*[GetItems() - NonExistent];

	  for(ushort c = 0, i = 0; c < GetItems(); ++c)
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

ushort stack::DrawContents(character* Viewer, std::string Topic) const // Draws a list of the items in this stack on the screen
{									// Displays Topic on the screen also...
  if(!GetItems()) return 0xFFFF;
  felist ItemNames(Topic, WHITE, 0, true);

  ItemNames.AddDescription("");
  ItemNames.AddDescription(std::string("Overall weight: ") + SumOfMasses() + " grams");
  ItemNames.AddDescription("");

  std::string Buffer = "Name                                                 Weight       AV     Str";
  Viewer->AddSpecialItemInfoDescription(Buffer);
  ItemNames.AddDescription(Buffer);
	
  for(ushort c = 0; c < GetItems(); ++c)
    {
      Buffer = GetItem(c)->Name(INDEFINITE);
      Buffer.resize(50,' ');
      Buffer += int(GetItem(c)->GetWeight());
      Buffer.resize(63, ' ');
      Buffer += int(GetItem(c)->GetArmorValue());
      Buffer.resize(70, ' ');
      Buffer += int(GetItem(c)->GetWeaponStrength() / 100);
      Viewer->AddSpecialItemInfo(Buffer, GetItem(c));

      ItemNames.AddEntry(Buffer, RED);
    }

  return ItemNames.Draw(false);
}

ushort stack::GetEmitation() const // Calculates the biggest light emmision of the levelsquare...
{
  ushort Emitation = 0;

  for(ushort c = 0; c < GetItems(); ++c)
    if(GetItem(c))
      if(GetItem(c)->GetEmitation() > Emitation)
	Emitation = GetItem(c)->GetEmitation();

  return Emitation;
}

ulong stack::SumOfMasses() const
{
  ulong Sum = 0;

  for(ushort c = 0; c < GetItems(); ++c)
    Sum += GetItem(c)->GetWeight();

  return Sum;
}

void stack::Save(outputfile& SaveFile) const
{
  SaveFile << Items << SquarePosition;

  for(ushort c = 0; c < Items; ++c)
    SaveFile << Item[c];
}

void stack::Load(inputfile& SaveFile)
{
  SaveFile >> Items >> SquarePosition;

  if(Items)
    {
      Item = new item*[Items];

      for(ushort c = 0; c < Items; ++c)
	SaveFile >> Item[c];
    }
  else
    Item = 0;

  NonExistent = 0;

  SquareUnder = game::GetSquareInLoad();
}

ushort stack::SearchItem(item* ToBeSearched) const
{
  for(ushort c = 0; c < GetItems(); ++c)
    if(GetItem(c) == ToBeSearched)
      return c;

  return 0xFFFF;
}

vector2d stack::GetPos() const
{
  return GetLevelSquareUnder()->GetPos();
}

ushort stack::ConsumableItems(character* Eater)
{
  ushort Counter = 0;

  for(ushort c = 0; c < GetItems(); ++c)
    if(GetItem(c)->Consumable(Eater))
      ++Counter;

  return Counter;
}

ushort stack::DrawConsumableContents(character* Eater, std::string Topic) const
{
  stack ConsumableStack(GetSquareUnder());
  item* TheItem;
  ushort Key;

  for(ushort c = 0; c < GetItems(); ++c)
    {
      if(GetItem(c)->Consumable(Eater))
	ConsumableStack.FastAddItem(GetItem(c));
    }

  Key = ConsumableStack.DrawContents(Eater, Topic);

  if(Key == 0xFFFF || Key == 0x1B || Key > ConsumableStack.GetItems())
    {
      ConsumableStack.DeletePointers();
      return 0xFFFF;
    }

  TheItem = ConsumableStack.GetItem(Key);
  ConsumableStack.DeletePointers();
  return SearchItem(TheItem);

}

void stack::DeletePointers()
{
  while(GetItems())
    FastRemoveItem(0);
}

void stack::StackMerge(stack* ToBeMerged)
{
  for(ushort c = 0; c < ToBeMerged->GetItems(); ++c)
    FastAddItem(ToBeMerged->GetItem(c));

  ToBeMerged->DeletePointers();
}

void stack::Kick(ushort Strength, bool ShowOnScreen, uchar Direction)
{
  if(Strength > 3)
    {
      ImpactDamage(Strength, ShowOnScreen);

      if(GetItems())
	GetItem(0)->Fly(Direction, Strength, this, true);

      if(GetItems() && RAND() % 2)
	GetItem(0)->Fly(Direction, Strength, this, true);
    }
  else
    if(GetItems() && ShowOnScreen)
      ADD_MESSAGE("Your weak kick has no effect.");
}

long stack::Score() const
{
  long Score = 0;

  for(ushort c = 0; c < GetItems(); ++c)
    Score += GetItem(c)->Score();

  return Score;
}

void stack::SetSquareUnder(square* Square)
{
  SquareUnder = Square;
}

bool stack::Polymorph()
{
  for(ushort c = 0, i = 0; c < GetItems() && c - i < 5; ++c)
    if(GetItem(i)->Polymorph(this))
      RemoveItem(i);
    else
      ++i;

  return true;
}

void stack::ReceiveSound(float Strength)
{
  for(int x = 0; x < GetItems(); ++x) // PROBLEM!!! This probably has the same problems as kick... So...
    GetItem(x)->ReceiveSound(Strength, GetLevelSquareTrulyUnder()->CanBeSeen(), this);
}

void stack::StruckByWandOfStriking(character* Zapper, std::string DeathMsg)
{
  for(ushort c = 0; c < GetItems();)
    if(!GetItem(c)->StruckByWandOfStriking(Zapper, DeathMsg, this))
      ++c;
}

void stack::CheckForStepOnEffect(character* Stepper)
{
  for(ushort c = 0; c < GetItems();)
    if(!GetItem(c)->GetStepOnEffect(Stepper))
      ++c;
}

square* stack::GetSquareTrulyUnder() const
{
  switch(SquarePosition)
    {
    case DOWN:
      if(game::IsValidPos(GetSquareUnder()->GetPos() + vector2d(0, -1)))
	return GetSquareUnder()->GetAreaUnder()->GetSquare(GetSquareUnder()->GetPos() + vector2d(0, -1));
      else
	return 0;
    case LEFT:
      if(game::IsValidPos(GetSquareUnder()->GetPos() + vector2d(1, 0)))
	return GetSquareUnder()->GetAreaUnder()->GetSquare(GetSquareUnder()->GetPos() + vector2d(1, 0));
      else
	return 0;
    case UP:
      if(game::IsValidPos(GetSquareUnder()->GetPos() + vector2d(0, 1)))
	return GetSquareUnder()->GetAreaUnder()->GetSquare(GetSquareUnder()->GetPos() + vector2d(0, 1));
      else
	return 0;
    case RIGHT:
      if(game::IsValidPos(GetSquareUnder()->GetPos() + vector2d(-1, 0)))
	return GetSquareUnder()->GetAreaUnder()->GetSquare(GetSquareUnder()->GetPos() + vector2d(-1, 0));
      else
	return 0;
    default:
      return GetSquareUnder();
    }
}

void stack::ImpactDamage(ushort Strength, bool ShowOnScreen)
{
  // This may jam if an item is destroyed but doesn't leave anything behind

  for(ushort c = 0; c < GetItems();)
    if(!GetItem(c)->ImpactDamage(Strength, ShowOnScreen, this))
      ++c;
}

void stack::ReceiveFireDamage(character* Burner, std::string DeathMsg, long SizeOfEffect)
{
  for(ushort c = 0; c < GetItems();)
    if(!GetItem(c)->ReceiveFireDamage(Burner, DeathMsg, this, SizeOfEffect))
      ++c;
}

bool stack::Teleport()
{
  bool FoundItems = GetItems() > 0;

  while(GetItems())
    {
      GetItem(0)->Teleport(this);
    }
  return FoundItems;
}
