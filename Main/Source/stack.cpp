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
#include "slot.h"

stack::stack(square* SquareUnder, uchar SquarePosition) : SquareUnder(SquareUnder), SquarePosition(SquarePosition)
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

  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    (**i)->PositionedDrawToTileBuffer(SquarePosition);

  return true;
}

void stack::AddItem(item* ToBeAdded)
{
  /*item** TempItem = new item*[GetItems() + 1];

  for(ushort c = 0; c < GetItems(); ++c)
    TempItem[c] = GetItem(c);

  delete [] Item;

  Item = TempItem;*/

  stackslot* StackSlot = new stackslot;

  StackSlot->SetMotherStack(this);
  StackSlot->SetStackIterator(Item.insert(Item.end(), StackSlot));
  ToBeAdded->PlaceToSlot(StackSlot);

  //SetItem(GetItems(), ToBeAdded);

  ToBeAdded->SetSquareUnder(GetSquareUnder());

  //SetItems(GetItems() + 1);

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();
    }

  if(GetSquareTrulyUnder())
    {
      if(!game::GetInWilderness())
	GetLSquareTrulyUnder()->SignalEmitationIncrease(ToBeAdded->GetEmitation());

      GetSquareTrulyUnder()->SendNewDrawRequest();
      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();

      if(GetSquareTrulyUnder()->CanBeSeen())
	GetSquareTrulyUnder()->UpdateMemorized();
    }

  //return GetItems() - 1;
}

void stack::FastAddItem(item* ToBeAdded)
{
  stackslot* StackSlot = new stackslot;

  StackSlot->SetMotherStack(this);
  StackSlot->SetStackIterator(Item.insert(Item.end(), StackSlot));
  ToBeAdded->PlaceToSlot(StackSlot);

  //SetItem(GetItems(), ToBeAdded);

  ToBeAdded->SetSquareUnder(GetSquareUnder());
  /*item** TempItem = new item*[Items + 1];

  for(ushort c = 0; c < GetItems(); ++c)
    TempItem[c] = GetItem(c);

  delete [] Item;

  Item = TempItem;

  SetItem(GetItems(), ToBeAdded);

  ToBeAdded->SetSquareUnder(GetSquareUnder());

  SetItems(GetItems() + 1);

  return GetItems() - 1;*/
}

void stack::RemoveItem(stackiterator Iterator)
{
  //if(Item && Index < GetItems())
  //  {
  

  ushort IEmit = GetEmitation();

  //item* Removed = Iterator->Item;
  delete *Iterator;
  Item.erase(Iterator);

  /*Removed = Item[Index];

  Item[Index] = 0;

  SNonExistent(CNonExistent() + 1);

  Optimize(0);*/

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();
    }

  if(GetSquareTrulyUnder())
    {
      if(!game::GetInWilderness())
	GetLSquareTrulyUnder()->SignalEmitationDecrease(IEmit);

      GetSquareTrulyUnder()->SendNewDrawRequest();
      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();

      if(GetSquareTrulyUnder()->CanBeSeen())
	GetSquareTrulyUnder()->UpdateMemorized();
    }

  //return Removed;
  /*  }
  else
    return 0;*/
}

void stack::FastRemoveItem(stackiterator Iterator)
{
  delete *Iterator;
  Item.erase(Iterator);
  /*if(Item)
    {
      SetItem(Index, 0);

      SNonExistent(CNonExistent() + 1);

      Optimize(0);
    }*/
}

void stack::Clean()
{
  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    {
      (**i)->SetExists(false);
      delete *i;
    }
  /*for(ushort c = 0; c < GetItems(); ++c)
    if(GetItem(c))
      Item[c]->SetExists(false);*/

  //delete [] Item;

  /*Item = 0;

  SetItems(0);

  SNonExistent(0);*/

  Item.clear();
}

item* stack::MoveItem(stackiterator Iterator, stack* MoveTo)
{
  //ushort ToBeReturned = 0;

  //if(this == MoveTo)
  //  return ***Iterator;// GetItem(Index);

  if(GetSquareTrulyUnder())
    {
      GetSquareTrulyUnder()->SendNewDrawRequest();
      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();
    }

  if(GetSquareUnder())
    GetSquareUnder()->SetDescriptionChanged(true);

  item* Item = ***Iterator;

  //if(Item && GetItems() > Index && GetItem(Index) && MoveTo)
    if(MoveTo->GetLSquareTrulyUnder() == GetLSquareTrulyUnder())
      {
	/*ToBeReturned = */MoveTo->FastAddItem(***Iterator);
	FastRemoveItem(Iterator);

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

	/*ToBeReturned = */MoveTo->AddItem(***Iterator);
	RemoveItem(Iterator);

	if(GetSquareUnder() && GetSquareUnder()->CanBeSeen())
	  GetSquareUnder()->UpdateMemorizedDescription();

	if(MoveTo->GetSquareUnder() && MoveTo->GetSquareUnder()->CanBeSeen())
	  MoveTo->GetSquareUnder()->UpdateMemorizedDescription();

	if(GetSquareTrulyUnder() && GetSquareTrulyUnder()->CanBeSeen())
	  GetSquareTrulyUnder()->UpdateMemorized();
      }

  return Item;

  //return MoveTo->GetItem(ToBeReturned);
}

/*void stack::Optimize(ushort OptimizeBoundary)
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
}*/

item* stack::DrawContents(character* Viewer, std::string Topic) const // Draws a list of the items in this stack on the screen
{									// Displays Topic on the screen also...
  if(!GetItems()) return 0;
  felist ItemNames(Topic, WHITE, 0, true);

  ItemNames.AddDescription("");
  ItemNames.AddDescription(std::string("Overall weight: ") + SumOfMasses() + " grams");
  ItemNames.AddDescription("");

  std::string Buffer = "Name                                                 Weight       AV     Str";
  Viewer->AddSpecialItemInfoDescription(Buffer);
  ItemNames.AddDescription(Buffer);

  stackiterator i;
	
  //for(ushort c = 0; c < GetItems(); ++c)
  for(i = Item.begin(); i != Item.end(); ++i)
    {
      Buffer = (**i)->Name(INDEFINITE);
      Buffer.resize(50,' ');
      Buffer += int((**i)->GetWeight());
      Buffer.resize(63, ' ');
      Buffer += int((**i)->GetStrengthValue());
      Buffer.resize(70, ' ');
      Buffer += int((**i)->GetWeaponStrength() / 100);
      Viewer->AddSpecialItemInfo(Buffer, ***i);

      ItemNames.AddEntry(Buffer, RED);
    }

  ushort Chosen = ItemNames.Draw(false);

  if(Chosen & 0x8000)
    return 0;

  ushort c;

  for(i = Item.begin(); i != Item.end(); ++i, ++c)
    if(c == Chosen)
      return ***i;

  return 0;
}

ushort stack::GetEmitation() const // Calculates the biggest light emmision of the lsquare...
{
  ushort Emitation = 0;

  //for(ushort c = 0; c < GetItems(); ++c)
  //  if(GetItem(c))
  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    if((**i)->GetEmitation() > Emitation)
      Emitation = (**i)->GetEmitation();

  return Emitation;
}

ulong stack::SumOfMasses() const
{
  ulong Sum = 0;

  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    Sum += (**i)->GetWeight();

  return Sum;
}

void stack::Save(outputfile& SaveFile) const
{
  SaveFile << Item << SquarePosition;

  /*for(ushort c = 0; c < Items; ++c)
    SaveFile << Item[c];*/
}

void stack::Load(inputfile& SaveFile)
{
  SaveFile >> Item >> SquarePosition;

  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    {
      (*i)->SetStackIterator(i);
      (*i)->SetMotherStack(this);
    }

  /*if(Items)
    {
      Item = new item*[Items];

      for(ushort c = 0; c < Items; ++c)
	SaveFile >> Item[c];
    }
  else
    Item = 0;

  NonExistent = 0;*/

  SquareUnder = game::GetSquareInLoad();
}

/*ushort stack::SearchItem(item* ToBeSearched) const
{
  for(ushort c = 0; c < GetItems(); ++c)
    if(GetItem(c) == ToBeSearched)
      return c;

  return 0xFFFF;
}*/

vector2d stack::GetPos() const
{
  return GetLSquareUnder()->GetPos();
}

bool stack::ConsumableItems(character* Eater)
{
  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    if((**i)->Consumable(Eater))
      return true;

  return false;
}

item* stack::DrawConsumableContents(character* Eater, std::string Topic) const
{
  return 0;
  /*stack ConsumableStack(GetSquareUnder());
  item* TheItem;
  ushort Key;

  //for(ushort c = 0; c < GetItems(); ++c)
  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    {
      if((**i)->Consumable(Eater))
	ConsumableStack.FastAddItem((**i));
    }

  Key = ConsumableStack.DrawContents(Eater, Topic);

  if(Key == 0xFFFF || Key == 0x1B || Key > ConsumableStack.GetItems())
    {
      ConsumableStack.DeletePointers();
      return 0xFFFF;
    }

  TheItem = ConsumableStack.GetItem(Key);
  ConsumableStack.DeletePointers();
  return SearchItem(TheItem);*/

}

void stack::DeletePointers()
{
  while(GetItems())
    FastRemoveItem(0);
}

/*void stack::StackMerge(stack* ToBeMerged)
{
  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    FastAddItem(ToBeMerged->GetItem(c));

  ToBeMerged->DeletePointers();
}*/

void stack::Kick(ushort Strength, bool ShowOnScreen, uchar Direction)
{
  if(Strength > 3)
    {
      ImpactDamage(Strength);

      if(GetItems())
	(***Item.begin())->Fly(Direction, Strength, this, true);

      if(GetItems() && RAND() % 2)
	(***Item.begin())->Fly(Direction, Strength, this, true);
    }
  else
    if(GetItems() && ShowOnScreen)
      ADD_MESSAGE("Your weak kick has no effect.");
}

long stack::Score() const
{
  long Score = 0;

  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    Score += (**i)->Score();

  return Score;
}

void stack::SetSquareUnder(square* Square)
{
  SquareUnder = Square;
}

void stack::Polymorph()
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  ushort p = 0;

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists() && ItemVector[c]->Polymorph(this) && ++p == 5)
      break;

  /*std::list<item*> TempItem = Item;



  for(stackiterator i = TempItem.begin(); i != TempItem.end(); ++i)
    if((**i)->GetExists() && (**i)->Polymorph(this) && ++p == 5)
      break;*/

  /*for(ushort c = 0, i = 0; c < GetItems() && c - i < 5; ++c)
    if(GetItem(i)->Polymorph(this))
      RemoveItem(i);
    else
      ++i;*/
}

void stack::ReceiveSound(float Strength)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->ReceiveSound(Strength);

  //for(int x = 0; x < GetItems(); ++x) // PROBLEM!!! This probably has the same problems as kick... So...
    //GetItem(x)->ReceiveSound(Strength, GetLSquareTrulyUnder()->CanBeSeen(), this);
}

void stack::StruckByWandOfStriking(character* Zapper, std::string DeathMsg)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->StruckByWandOfStriking(Zapper, DeathMsg);
}

void stack::CheckForStepOnEffect(character* Stepper)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->GetStepOnEffect(Stepper);
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

void stack::ImpactDamage(ushort Strength)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->ImpactDamage(Strength);
}

void stack::ReceiveFireDamage(character* Burner, std::string DeathMsg, long SizeOfEffect)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->ReceiveFireDamage(Burner, DeathMsg, this, SizeOfEffect);
}

void stack::Teleport()
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->Teleport(this);
}

lsquare* stack::GetLSquareTrulyUnder() const
{
  return (lsquare*)GetSquareTrulyUnder();
}

lsquare* stack::GetLSquareUnder() const
{
  return (lsquare*)SquareUnder;
}

void stack::FillItemVector(itemvector& ItemVector) const
{
  for(stackiterator i = Item.begin(); i != Item.end(); ++i)
    ItemVector.push_back(***i);
}

item* stack::GetBottomItem() const
{
  return ***GetBottomSlot();
}

/*item* stack::MoveItem(item* Item, stack* To)
{
  return MoveItem(((stackslot*)Item->GetSlot())->GetStackIterator(), To);
}*/

item* stack::GetItem(ushort Index) const
{
  ushort c = 0;

  for(stackiterator i = Item.begin(); i != Item.end(); ++i, ++c)
    if(c == Index)
      return ***i;

  return 0;
}

ushort stack::SearchItem(item* ToBeSearched) const
{
  ushort c = 0;

  for(stackiterator i = Item.begin(); i != Item.end(); ++i, ++c)
    if(***i == ToBeSearched)
      return c;

  return 0xFFFF;

  /*for(ushort c = 0; c < GetItems(); ++c)
    if(GetItem(c) == ToBeSearched)
      return c;

  return 0xFFFF;*/
}
