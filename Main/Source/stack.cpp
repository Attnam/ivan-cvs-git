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
#include "game.h"
#include "save.h"

stack::stack(square* SquareUnder, uchar SquarePosition) : SquareUnder(SquareUnder), SquarePosition(SquarePosition)
{
  Item = new stacklist;
}

stack::~stack()
{
  Clean();
  delete Item;
}

bool stack::DrawToTileBuffer(bool Animate) const
{
  if(!GetItems())
    return false;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    (**i)->PositionedDrawToTileBuffer(SquarePosition, Animate);

  return true;
}

void stack::AddItem(item* ToBeAdded)
{
  stackslot* StackSlot = new stackslot;

  StackSlot->SetMotherStack(this);
  StackSlot->SetStackIterator(Item->insert(Item->end(), StackSlot));
  ToBeAdded->PlaceToSlot(StackSlot);

  ToBeAdded->SetSquareUnder(GetSquareUnder());

  if(GetSquareUnder() && SquarePosition != HIDDEN)
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();
    }

  if(GetSquareTrulyUnder())
    {
      if(!game::IsInWilderness())
	GetLSquareTrulyUnder()->SignalEmitationIncrease(ToBeAdded->GetEmitation());

      if(SquarePosition != HIDDEN)
	{
	  GetSquareTrulyUnder()->SendNewDrawRequest();
	  GetSquareTrulyUnder()->SendMemorizedUpdateRequest();

	  if(GetSquareTrulyUnder()->CanBeSeen())
	    GetSquareTrulyUnder()->UpdateMemorized();

	  if(ToBeAdded->IsAnimated())
	    GetSquareTrulyUnder()->IncAnimatedEntities();
	}
    }
}

void stack::FastAddItem(item* ToBeAdded)
{
  stackslot* StackSlot = new stackslot;
  StackSlot->SetMotherStack(this);
  StackSlot->SetStackIterator(Item->insert(Item->end(), StackSlot));
  ToBeAdded->PlaceToSlot(StackSlot);
  ToBeAdded->SetSquareUnder(GetSquareUnder());

  if(SquarePosition != HIDDEN && ToBeAdded->IsAnimated() && GetSquareTrulyUnder())
    GetSquareTrulyUnder()->IncAnimatedEntities();
}

void stack::RemoveItem(stackiterator Iterator)
{
  ushort IEmit = GetEmitation();
  bool WasAnimated = (**Iterator)->IsAnimated();
  delete *Iterator;
  Item->erase(Iterator);

  if(GetSquareUnder() && SquarePosition != HIDDEN)
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();
    }

  if(GetSquareTrulyUnder())
    {
      if(!game::IsInWilderness())
	GetLSquareTrulyUnder()->SignalEmitationDecrease(IEmit);

      if(SquarePosition != HIDDEN)
	{
	  GetSquareTrulyUnder()->SendNewDrawRequest();
	  GetSquareTrulyUnder()->SendMemorizedUpdateRequest();

	  if(GetSquareTrulyUnder()->CanBeSeen())
	    GetSquareTrulyUnder()->UpdateMemorized();

	  if(WasAnimated)
	    GetSquareTrulyUnder()->DecAnimatedEntities();
	}
    }
}

void stack::FastRemoveItem(stackiterator Iterator)
{
  if(SquarePosition != HIDDEN && (**Iterator)->IsAnimated() && GetSquareTrulyUnder())
    GetSquareTrulyUnder()->DecAnimatedEntities();

  delete *Iterator;
  Item->erase(Iterator);
}

void stack::Clean()
{
  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    {
      if(SquarePosition != HIDDEN && (**i)->IsAnimated() && GetSquareTrulyUnder())
	GetSquareTrulyUnder()->DecAnimatedEntities();

      (**i)->SetExists(false);
      delete *i;
    }

  Item->clear();
}

item* stack::MoveItem(stackiterator Iterator, stack* MoveTo)
{
  item* Item = ***Iterator;

  if(MoveTo->GetLSquareTrulyUnder() == GetLSquareTrulyUnder())
    {
      MoveTo->FastAddItem(***Iterator);
      FastRemoveItem(Iterator);

      if(SquarePosition != HIDDEN)
	{
	  if(GetSquareTrulyUnder())
	    {
	      GetSquareTrulyUnder()->SendNewDrawRequest();
	      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();
	    }

	  if(GetSquareUnder())
	    GetSquareUnder()->SetDescriptionChanged(true);
	}

      if(SquarePosition != HIDDEN || MoveTo->SquarePosition != HIDDEN)
	{
	  if(GetSquareUnder() && GetSquareUnder()->CanBeSeen())
	    GetSquareUnder()->UpdateMemorizedDescription();

	  if(GetSquareTrulyUnder() && GetSquareTrulyUnder()->CanBeSeen())
	    GetSquareTrulyUnder()->UpdateMemorized();
	}
    }
  else
    {
      MoveTo->AddItem(***Iterator);
      RemoveItem(Iterator);
    }

  return Item;
}

ushort stack::GetEmitation() const // Calculates the biggest light emmision of the lsquare...
{
  ushort Emitation = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    if((**i)->GetEmitation() > Emitation)
      Emitation = (**i)->GetEmitation();

  return Emitation;
}

ulong stack::GetTotalWeight() const
{
  ulong Sum = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    Sum += (**i)->GetWeight();

  return Sum;
}

void stack::Save(outputfile& SaveFile) const
{
  SaveFile << *Item << SquarePosition;
}

void stack::Load(inputfile& SaveFile)
{
  SaveFile >> *Item >> SquarePosition;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    {
      (*i)->SetStackIterator(i);
      (*i)->SetMotherStack(this);
    }

  SquareUnder = game::GetSquareInLoad();
}

vector2d stack::GetPos() const
{
  return GetLSquareUnder()->GetPos();
}

bool stack::SortedItems(character* Viewer, bool (*SorterFunction)(item*, character*)) const
{
  if(SorterFunction == 0)
    return GetItems() ? true : false;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    if(SorterFunction(***i, Viewer))
      return true;

  return false;
}

void stack::DeletePointers()
{
  while(GetItems())
    FastRemoveItem(GetBottomSlot());
}

void stack::Kick(character* Kicker, ushort Strength, uchar Direction)
{
  if(Strength > 3)
    {
      ReceiveDamage(Kicker, Strength, PHYSICALDAMAGE);

      if(GetItems())
	(***Item->begin())->Fly(Kicker, Direction, Strength);

      if(GetItems() && RAND() % 2)
	(***Item->begin())->Fly(Kicker, Direction, Strength);
    }
  else
    if(GetItems() && Kicker->IsPlayer())
      ADD_MESSAGE("Your weak kick has no effect.");
}

long stack::Score() const
{
  long Score = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    Score += (**i)->GetScore();

  return Score;
}

void stack::SetSquareUnder(square* Square)
{
  SquareUnder = Square;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    (**i)->SetSquareUnder(Square);
}

void stack::Polymorph()
{
  itemvector ItemVector;
  FillItemVector(ItemVector);
  ushort p = 0;

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists() && ItemVector[c]->Polymorph(this) && ++p == 5)
      break;
}

void stack::CheckForStepOnEffect(character* Stepper)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists())
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

void stack::ReceiveDamage(character* Damager, short Damage, uchar Type)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists())
      ItemVector[c]->ReceiveDamage(Damager, Damage, Type);
}

void stack::TeleportRandomly()
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists())
      ItemVector[c]->TeleportRandomly();
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
  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    ItemVector.push_back(***i);
}

item* stack::GetBottomItem() const
{
  return ***GetBottomSlot();
}

item* stack::GetItem(ushort Index) const
{
  ushort c = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i, ++c)
    if(c == Index)
      return ***i;

  return 0;
}

ushort stack::SearchItem(item* ToBeSearched) const
{
  ushort c = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i, ++c)
    if(***i == ToBeSearched)
      return c;

  return 0xFFFF;
}

stackiterator stack::GetBottomSlot() const
{
  return Item->begin();
}

stackiterator stack::GetSlotAboveTop() const
{
  return Item->end();
}

item* stack::DrawContents(character* Viewer, const std::string& Topic, bool (*SorterFunction)(item*, character*)) const
{
  return DrawContents(0, Viewer, Topic, "", "", true, SorterFunction);
}

item* stack::DrawContents(stack* MergeStack, character* Viewer, const std::string& Topic, const std::string& ThisDesc, const std::string& ThatDesc, bool (*SorterFunction)(item*, character*)) const
{
  return DrawContents(MergeStack, Viewer, Topic, ThisDesc, ThatDesc, true, SorterFunction);
}

item* stack::DrawContents(character* Viewer, const std::string& Topic, bool SelectItem, bool (*SorterFunction)(item*, character*)) const
{
  return DrawContents(0, Viewer, Topic, "", "", SelectItem, SorterFunction);
}

item* stack::DrawContents(stack* MergeStack, character* Viewer, const std::string& Topic, const std::string& ThisDesc, const std::string& ThatDesc, bool SelectItem, bool (*SorterFunction)(item*, character*)) const
{
  felist ItemNames(Topic, WHITE, 0);

  ItemNames.AddDescription("");
  ItemNames.AddDescription(std::string("Overall weight: ") + GetTotalWeight() + (MergeStack ? MergeStack->GetTotalWeight() : 0) + " grams");
  ItemNames.AddDescription("");

  std::string Buffer = "Icon  Name                                         Weight SV   Str";
  Viewer->AddSpecialItemInfoDescription(Buffer);
  ItemNames.AddDescription(Buffer);

  if(MergeStack)
    MergeStack->AddContentsToList(ItemNames, Viewer, ThatDesc, SelectItem, SorterFunction);

  AddContentsToList(ItemNames, Viewer, ThisDesc, SelectItem, SorterFunction);

  ushort Chosen = ItemNames.Draw(vector2d(26, 42), 652, 12, MAKE_RGB(0, 0, 16), SelectItem, false);

  if(Chosen & 0x8000)
    return 0;

  ushort Pos = 0;

  item* Item;

  if(MergeStack)
    {
      Item = MergeStack->SearchChosen(Pos, Chosen, Viewer, SorterFunction);

      if(Item)
	return Item;
    }

  Item = SearchChosen(Pos, Chosen, Viewer, SorterFunction);
  return Item;
}

void stack::AddContentsToList(felist& ItemNames, character* Viewer, const std::string& Desc, bool SelectItem, bool (*SorterFunction)(item*, character*)) const
{
  bool UseSorterFunction = SorterFunction ? true : false;
  bool DescDrawn = false;

  for(ushort c = 0; c < ITEM_CATEGORIES; ++c)
    {
      bool CatDescDrawn = false;

      for(stackiterator i = Item->begin(); i != Item->end(); ++i)
	if((**i)->GetCategory() == c && (!UseSorterFunction || SorterFunction(***i, Viewer)))
	  {
	    if(!DescDrawn && Desc != "")
	      {
		ItemNames.AddEntry("", LIGHTGRAY, 0, false);
		ItemNames.AddEntry(Desc, LIGHTGRAY, 0, false);
		ItemNames.AddEntry("", LIGHTGRAY, 0, false);
		DescDrawn = true;
	      }

	    if(!CatDescDrawn)
	      {
		ItemNames.AddEntry(item::ItemCategoryName(c), LIGHTGRAY, 0, false);
		CatDescDrawn = true;
	      }

	    std::string Buffer = (**i)->GetName(INDEFINITE);

	    if(Buffer.length() > 44)
	      {
		Buffer[41] = Buffer[42] = Buffer[43] = '.';
		Buffer[44] = ' ';
	      }

	    Buffer.resize(45,' ');
	    Buffer += (**i)->GetWeight();
	    Buffer.resize(52, ' ');
	    Buffer += (**i)->GetStrengthValue();
	    Buffer.resize(57, ' ');
	    Buffer += ulong((**i)->GetWeaponStrength() / 100);
	    Viewer->AddSpecialItemInfo(Buffer, ***i);

	    if(!SelectItem)
	      Buffer = "   " + Buffer;

	    ItemNames.AddEntry(Buffer, LIGHTGRAY, (**i)->GetPicture());
	  }
    }
}

item* stack::SearchChosen(ushort& Pos, ushort Chosen, character* Viewer, bool (*SorterFunction)(item*, character*)) const
{
  bool UseSorterFunction = SorterFunction ? true : false;

  for(ushort c = 0; c < ITEM_CATEGORIES; ++c)
    for(stackiterator i = Item->begin(); i != Item->end(); ++i)
      if((**i)->GetCategory() == c && (!UseSorterFunction || SorterFunction(***i, Viewer)) && Pos++ == Chosen)
	return ***i;

  return 0;
}

bool stack::RaiseTheDead(character* Summoner)
{
  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    {
      if((**i)->RaiseTheDead(Summoner))
	return true;
    }
  return false;
}

bool stack::TryKey(item* Key, character* Applier)
{
  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    {
      if((**i)->TryKey(Key, Applier))
	return true;
    }

  return false;
}

bool stack::Open(character* Opener)
{
  item* ToBeOpened = DrawContents(Opener, "What do you wish to open?", &item::OpenableSorter);

  if(ToBeOpened == 0)
    return false;

  //  item* ItemInside = ToBeOpened->TryToOpen(Opener);

  return ToBeOpened->Open(Opener);
}

ulong stack::GetTotalVolume() const
{
  ulong Sum = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    Sum += (**i)->GetVolume();

  return Sum;
}

void stack::MoveAll(stack* ToStack)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);
  ushort p = 0;

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists())
      ItemVector[c]->MoveTo(ToStack);
}
