#include <algorithm>

#include "itemba.h"
#include "stack.h"
#include "festring.h"
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
#include "config.h"

ushort stack::Selected;

stack::stack(square* MotherSquare, entity* MotherEntity, uchar SquarePosition, bool IgnoreVisibility) : Bottom(0), Top(0), MotherSquare(MotherSquare), MotherEntity(MotherEntity), SquarePosition(SquarePosition), Volume(0), Weight(0), Emitation(0), Items(0), IgnoreVisibility(IgnoreVisibility)
{
}

stack::~stack()
{
  Clean(true);
}

void stack::Draw(const character* Viewer, bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAlpha, bool AllowAnimate, bool AllowOutline) const
{
  if(!Items)
    return;

  ushort VisibleItems = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->CanBeSeenBy(Viewer) || game::SeeWholeMapCheatIsActive())
      {
	i->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
	++VisibleItems;
      }

  if(VisibleItems && AllowOutline && configuration::GetOutlineItems())
    {
      igraph::GetTileBuffer()->Fill(TRANSPARENT_COLOR);

      for(stackiterator i = GetBottom(); i.HasItem(); ++i)
	if(i->CanBeSeenBy(Viewer) || game::SeeWholeMapCheatIsActive())
	  i->Draw(igraph::GetTileBuffer(), vector2d(0, 0), MakeRGB24(128, 128, 128), false, AllowAnimate);

      igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetItemOutlineColor());
      igraph::GetOutlineBuffer()->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, configuration::GetContrastLuminance());
    }

  if(SquarePosition == CENTER && VisibleItems > 1)
    igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, 0, 16, Pos, 16, 16, configuration::GetContrastLuminance());
}

void stack::AddItem(item* ToBeAdded)
{
  if(!ToBeAdded)
    return;

  AddElement(ToBeAdded);

  if(SquarePosition == HIDDEN)
    return;

  lsquare* SquareUnder = GetLSquareTrulyUnder();

  if(!SquareUnder)
    return;

  if(ToBeAdded->IsAnimated())
    SquareUnder->IncAnimatedEntities();

  if(!game::IsGenerating())
    {
      SquareUnder->SendNewDrawRequest();
      SquareUnder->SendMemorizedUpdateRequest();
    }
}

void stack::RemoveItem(stackslot* Slot)
{
  bool WasAnimated = Slot->GetItem()->IsAnimated();
  ulong Emit = Slot->GetItem()->GetEmitation();
  RemoveElement(Slot);
  SignalVolumeAndWeightChange();
  SignalEmitationDecrease(Emit);

  if(SquarePosition == HIDDEN)
    return;

  lsquare* SquareUnder = GetLSquareTrulyUnder();

  if(!SquareUnder)
    return;

  if(WasAnimated)
    SquareUnder->DecAnimatedEntities();

  if(!game::IsGenerating())
    {
      SquareUnder->SendNewDrawRequest();
      SquareUnder->SendMemorizedUpdateRequest();
    }
}

void stack::Clean(bool LastClean)
{
  ulong Emit = GetEmitation();
  stackslot* Slot = Bottom;
  lsquare* SquareUnder = LastClean ? 0 : GetLSquareTrulyUnder();

  while(Slot)
    {
      if(SquarePosition != HIDDEN && Slot->GetItem()->IsAnimated() && SquareUnder)
	SquareUnder->DecAnimatedEntities();

      if(LastClean)
	delete Slot->GetItem();
      else
	Slot->GetItem()->SendToHell();

      stackslot* Rubbish = Slot;
      Slot = Slot->Next;
      delete Rubbish;
    }

  if(!LastClean)
    {
      Bottom = Top = 0;
      Volume = Weight = Items = 0;
      SignalVolumeAndWeightChange();
      SignalEmitationDecrease(Emit);
    }
}

item* stack::MoveItem(stackslot* Slot, stack* MoveTo)
{
  item* Item = Slot->GetItem();
  Item->RemoveFromSlot();
  MoveTo->AddItem(Item);
  return Item;
}

void stack::Save(outputfile& SaveFile) const
{
  SaveFile << SquarePosition << Items;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    SaveFile << i.GetSlot();
}

void stack::Load(inputfile& SaveFile)
{
  SaveFile >> SquarePosition >> Items;

  for(ushort c = 0; c < Items; ++c)
    {
      if(!c)
	Bottom = Top = new stackslot(this, 0);
      else
	Top = Top->Next = new stackslot(this, Top);

      SaveFile >> *Top;
      Volume += (*Top)->GetVolume();
      Weight += (*Top)->GetWeight();
      game::CombineLights(Emitation, (*Top)->GetEmitation());
    }
}

vector2d stack::GetPos() const
{
  return GetSquareUnder()->GetPos();
}

bool stack::SortedItems(const character* Viewer, bool (*SorterFunction)(item*, const character*)) const
{
  if(SorterFunction == 0)
    {
      for(stackiterator i = GetBottom(); i.HasItem(); ++i)
	if(IgnoreVisibility || i->CanBeSeenBy(Viewer))
	  return true;
    }
  else
    {
      for(stackiterator i = GetBottom(); i.HasItem(); ++i)
	if(SorterFunction(*i, Viewer) && (IgnoreVisibility || i->CanBeSeenBy(Viewer)))
	  return true;
    }

  return false;
}

void stack::BeKicked(character* Kicker, ushort KickDamage)
{
  if(KickDamage)
    {
      ReceiveDamage(Kicker, KickDamage, PHYSICAL_DAMAGE);

      /* Bug: you can kick mines with this. - somebody
	 Feature: you can kick mines with this. - hex */

      for(ushort c = 0; c < 1 + (RAND() & 1); ++c)
	if(GetItems())
	  GetTop()->Fly(Kicker, game::GetDirectionForVector(GetPos() - Kicker->GetPos()), KickDamage);
    }
  else if(GetVisibleItems(Kicker) && Kicker->IsPlayer())
    ADD_MESSAGE("Your weak kick has no effect.");
}

long stack::Score() const
{
  long Score = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    Score += i->GetScore();

  return Score;
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
      ItemVector[c]->StepOnEffect(Stepper);
}

square* stack::GetSquareTrulyUnder() const
{
  switch(SquarePosition)
    {
    case DOWN:
      if(GetAreaUnder()->IsValidPos(GetPos() + vector2d(0, 1)))
	return GetNearSquare(GetPos() + vector2d(0, 1));
      else
	return 0;
    case LEFT:
      if(GetAreaUnder()->IsValidPos(GetPos() + vector2d(-1, 0)))
	return GetNearSquare(GetPos() + vector2d(-1, 0));
      else
	return 0;
    case UP:
      if(GetAreaUnder()->IsValidPos(GetPos() + vector2d(0, -1)))
	return GetNearSquare(GetPos() + vector2d(0, -1));
      else
	return 0;
    case RIGHT:
      if(GetAreaUnder()->IsValidPos(GetPos() + vector2d(1, 0)))
	return GetNearSquare(GetPos() + vector2d(1, 0));
      else
	return 0;
    default:
      return GetSquareUnder();
    }
}

void stack::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
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

void stack::FillItemVector(itemvector& ItemVector) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    ItemVector.push_back(*i);
}

item* stack::GetItem(ushort Index) const
{
  ushort c = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i, ++c)
    if(c == Index)
      return *i;

  return 0;
}

ushort stack::SearchItem(item* ToBeSearched) const
{
  ushort c = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i, ++c)
    if(*i == ToBeSearched)
      return c;

  return 0xFFFF;
}

item* stack::DrawContents(const character* Viewer, const std::string& Topic, uchar Flags, bool (*SorterFunction)(item*, const character*)) const
{
  std::vector<item*> ReturnVector;
  DrawContents(ReturnVector, 0, Viewer, Topic, "", "", Flags|NO_MULTI_SELECT, SorterFunction);
  return ReturnVector.empty() ? 0 : ReturnVector[0];
}

void stack::DrawContents(std::vector<item*>& ReturnVector, const character* Viewer, const std::string& Topic, uchar Flags, bool (*SorterFunction)(item*, const character*)) const
{
  DrawContents(ReturnVector, 0, Viewer, Topic, "", "", Flags, SorterFunction);
}

/* MergeStack is used for showing two stacks together. Like when eating when there are items on the ground and in the character's stack */

void stack::DrawContents(std::vector<item*>& ReturnVector, stack* MergeStack, const character* Viewer, const std::string& Topic, const std::string& ThisDesc, const std::string& ThatDesc, uchar Flags, bool (*SorterFunction)(item*, const character*)) const
{
  felist Contents(Topic);

  if(!(Flags & NO_SPECIAL_INFO))
    {
      Contents.AddDescription("");
      Contents.AddDescription(std::string("Overall weight: ") + (MergeStack ? GetWeight() + MergeStack->GetWeight() : GetWeight()) + " grams");
    }

  if(MergeStack)
    MergeStack->AddContentsToList(Contents, Viewer, ThatDesc, Flags, SorterFunction);

  AddContentsToList(Contents, Viewer, ThisDesc, Flags, SorterFunction);
  game::SetStandardListAttributes(Contents);
  Contents.SetPageLength(12);
  Contents.RemoveFlags(BLIT_AFTERWARDS);

  if(!(Flags & NO_SELECT))
    Contents.AddFlags(SELECTABLE);

  if(Flags & REMEMBER_SELECTED)
    Contents.SetSelected(GetSelected());

  Selected = Contents.Draw();

  if(Selected & 0x8000)
    {
      Selected = 0;
      return;
    }

  ushort Pos = 0;

  if(MergeStack)
    {
      Pos = MergeStack->SearchChosen(ReturnVector, Viewer, Pos, Selected, Flags, SorterFunction);

      if(!ReturnVector.empty())
	return;
    }

  SearchChosen(ReturnVector, Viewer, Pos, Selected, Flags, SorterFunction);
}

/* fix selectitem warning! */

void stack::AddContentsToList(felist& Contents, const character* Viewer, const std::string& Desc, uchar Flags, bool (*SorterFunction)(item*, const character*)) const
{
  std::vector<std::vector<item*> > PileVector;
  Pile(PileVector, Viewer, SorterFunction);

  bool DrawDesc = Desc.length() != 0;
  ulong LastCategory = 0;

  for(ushort p = 0; p < PileVector.size(); ++p)
    {
      if(DrawDesc)
	{
	  if(!Contents.IsEmpty())
	    Contents.AddEntry("", WHITE, 0, 0, false);

	  Contents.AddEntry(Desc, WHITE, 0, 0, false);
	  Contents.AddEntry("", WHITE, 0, 0, false);
	  DrawDesc = false;
	}

      item* Item = PileVector[p].back();

      if(Item->GetCategory() != LastCategory)
	{
	  LastCategory = Item->GetCategory();
	  Contents.AddEntry(item::ItemCategoryName(LastCategory), LIGHT_GRAY, 0, 0, false);
	}

      std::string Entry;
      Item->AddInventoryEntry(Viewer, Entry, PileVector[p].size(), !(Flags & NO_SPECIAL_INFO));
      Contents.AddEntry(Entry, LIGHT_GRAY, 0, Item->GetPicture());
    }
}

ushort stack::SearchChosen(std::vector<item*>& ReturnVector, const character* Viewer, ushort Pos, ushort Chosen, uchar Flags, bool (*SorterFunction)(item*, const character*)) const
{
  /* Not really efficient... :( */

  std::vector<std::vector<item*> > PileVector;
  Pile(PileVector, Viewer, SorterFunction);

  for(ushort p = 0; p < PileVector.size(); ++p)
    if(Pos++ == Chosen)
      if(Flags & NO_MULTI_SELECT)
	{
	  ReturnVector.assign(1, PileVector[p].back());
	  return 0xFFFF;
	}
      else
	{
	  ushort Amount = PileVector[p].size();

	  if(Amount > 1)
	    Amount = game::ScrollBarQuestion("How many " + PileVector[p][0]->GetName(PLURAL) + '?', vector2d(16, 6), Amount, 1, 0, Amount, WHITE, LIGHT_GRAY, DARK_GRAY);

	  ReturnVector.assign(PileVector[p].end() - Amount, PileVector[p].end());
	  return 0xFFFF;
	}

  return Pos;
}

bool stack::RaiseTheDead(character* Summoner)
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->RaiseTheDead(Summoner))
      return true;

  return false;
}

bool stack::TryKey(item* Key, character* Applier)
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if((IgnoreVisibility || i->CanBeSeenBy(Applier)) && i->TryKey(Key, Applier))
      return true;

  return false;
}

bool stack::Open(character* Opener)
{
  if(!Opener->IsPlayer())
    return false;

  item* ToBeOpened = DrawContents(Opener, "What do you wish to open?", 0, &item::OpenableSorter);

  if(ToBeOpened == 0)
    return false;

  return ToBeOpened->Open(Opener);
}

ushort stack::GetVisibleItems(const character* Viewer) const
{
  if(IgnoreVisibility)
    return Items;

  ushort VisibleItems = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->CanBeSeenBy(Viewer))
      ++VisibleItems;

  return VisibleItems;
}

item* stack::GetBottomVisibleItem(const character* Viewer) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(IgnoreVisibility || i->CanBeSeenBy(Viewer))
      return *i;

  return 0;
}

square* stack::GetSquareUnder() const
{
  if(MotherEntity)
    return MotherEntity->GetSquareUnder();
  else
    return MotherSquare;
}

void stack::SignalVolumeAndWeightChange()
{
  CalculateVolumeAndWeight();

  if(MotherEntity)
    MotherEntity->SignalVolumeAndWeightChange();
}

void stack::CalculateVolumeAndWeight()
{
  Volume = Weight = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    {
      Volume += i->GetVolume();
      Weight += i->GetWeight();
    }
}

void stack::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
    {
      game::CombineLights(Emitation, EmitationUpdate);

      if(MotherEntity)
	MotherEntity->SignalEmitationIncrease(EmitationUpdate);
      else
	GetLSquareTrulyUnder()->SignalEmitationIncrease(EmitationUpdate);
    }
}

void stack::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
    {
      ulong Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation)
	{
	  if(MotherEntity)
	    MotherEntity->SignalEmitationDecrease(EmitationUpdate);
	  else
	    GetLSquareTrulyUnder()->SignalEmitationDecrease(EmitationUpdate);
	}
    }
}

void stack::CalculateEmitation()
{
  Emitation = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    game::CombineLights(Emitation, i->GetEmitation());
}

bool stack::CanBeSeenBy(const character* Viewer) const
{
  if(MotherEntity)
    return MotherEntity->ContentsCanBeSeenBy(Viewer);
  else
    return GetSquareTrulyUnder() == Viewer->GetSquareUnder() || GetSquareTrulyUnder()->CanBeSeenBy(Viewer);
}

bool stack::IsDangerousForAIToStepOn(const character* Stepper) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->CanBeSeenBy(Stepper) && i->DangerousToStepOn(Stepper))
      return true;

  return false;
}

/* returns true if something was cloned. Max is the cap of items to be cloned */

bool stack::Clone(ushort Max)
{
  if(!GetItems())
    return false;

  itemvector ItemVector;
  FillItemVector(ItemVector);
  ushort p = 0;

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists() && ItemVector[c]->DuplicateToStack(this) && ++p == Max)
      break;

  return p > 0;
}

void stack::AddElement(item* Item)
{
  ++Items;

  /* "I love writing illegible code." - Guy who wrote this */

  (Top = (Bottom ? Top->Next : Bottom) = new stackslot(this, Top))->PutInItem(Item);
}

void stack::RemoveElement(stackslot* Slot)
{
  --Items;
  (Slot->Last ? Slot->Last->Next : Bottom) = Slot->Next;
  (Slot->Next ? Slot->Next->Last : Top) = Slot->Last;
  delete Slot;
}

void stack::MoveItemsTo(stack* Stack)
{
  while(GetItems())
    GetBottom()->MoveTo(Stack);
}

void stack::MoveItemsTo(slot* Slot)
{
  while(GetItems())
    {
      Slot->AddFriendItem(*GetBottom());
    }
}


ushort stack::GetItems(const character* Char, bool ForceIgnoreVisibility) const
{
  return ForceIgnoreVisibility ? Items : GetVisibleItems(Char);
}

item* stack::GetBottomItem(const character* Char, bool ForceIgnoreVisibility) const
{
  if(IgnoreVisibility || ForceIgnoreVisibility)
    return Bottom ? **Bottom : 0;
  else
    return GetBottomVisibleItem(Char);
}

bool CategorySorter(const std::vector<item*>& V1, const std::vector<item*>& V2)
{
  return (*V1.begin())->GetCategory() < (*V2.begin())->GetCategory();
}

void stack::Pile(std::vector<std::vector<item*> >& PileVector, const character* Viewer, bool (*SorterFunction)(item*, const character*)) const
{
  bool UseSorterFunction = SorterFunction != 0;
  std::list<item*> List;

  for(stackiterator s = GetBottom(); s.HasItem(); ++s)
    if((!UseSorterFunction || SorterFunction(*s, Viewer)) && (IgnoreVisibility || s->CanBeSeenBy(Viewer)))
      List.push_back(*s);

  for(std::list<item*>::iterator i = List.begin(); i != List.end(); ++i)
    {
      PileVector.resize(PileVector.size() + 1);
      std::vector<item*>& Pile = PileVector.back();
      Pile.push_back(*i);

      if((*i)->CanBePiled())
	{
	  std::list<item*>::iterator j = i;

	  for(++j; j != List.end();)
	    if((*i)->CanBePiledWith(*j, Viewer))
	      {
		Pile.push_back(*j);
		std::list<item*>::iterator Dirt = j++;
		List.erase(Dirt);
	      }
	    else
	      ++j;
	}
    }

  std::stable_sort(PileVector.begin(), PileVector.end(), CategorySorter);
}

ulong stack::GetPrice() const
{
  ulong Price = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    Price += i->GetPrice();

  return Price;
}

ulong stack::GetTotalExplosivePower() const
{
  ulong ExplosivePower = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->IsExplosive())
      ExplosivePower += i->GetTotalExplosivePower();

  return ExplosivePower;
}

void stack::ReceiveFluidSpill(material* Liquid)
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->ReceiveFluidSpill(Liquid);
}


