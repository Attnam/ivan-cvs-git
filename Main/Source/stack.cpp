/* Compiled through slotset.cpp */

ushort stack::Selected;

stack::stack(square* MotherSquare, entity* MotherEntity, uchar SquarePosition, bool IgnoreVisibility) : Bottom(0), Top(0), MotherSquare(MotherSquare), MotherEntity(MotherEntity), SquarePosition(SquarePosition), Volume(0), Weight(0), Emitation(0), Items(0), IgnoreVisibility(IgnoreVisibility), Freezed(false) { }
stack::~stack() { Clean(true); }
square* stack::GetSquareUnder() const { return !MotherEntity ? MotherSquare : MotherEntity->GetSquareUnderEntity(); }

void stack::Draw(const character* Viewer, bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAnimate, bool AllowOutline) const
{
  if(!Items)
    return;

  ushort VisibleItems = 0;
  vector2d StackPos = GetPos();

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->CanBeSeenBy(Viewer) || game::GetSeeWholeMapCheatMode())
      {
	i->Draw(Bitmap, Pos, Luminance, i->GetSquareIndex(StackPos), AllowAnimate);
	++VisibleItems;
      }

  if(VisibleItems && AllowOutline && configuration::GetOutlineItems())
    {
      igraph::GetTileBuffer()->ClearToColor(TRANSPARENT_COLOR);

      for(stackiterator i = GetBottom(); i.HasItem(); ++i)
	if(i->CanBeSeenBy(Viewer) || game::GetSeeWholeMapCheatMode())
	  i->Draw(igraph::GetTileBuffer(), vector2d(0, 0), NORMAL_LUMINANCE, i->GetSquareIndex(StackPos), AllowAnimate);

      igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetItemOutlineColor());
      igraph::GetOutlineBuffer()->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, configuration::GetContrastLuminance());
    }

  if(SquarePosition == CENTER)
    {
      if(VisibleItems > 1)
	igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, 0, 16, Pos, 16, 16, configuration::GetContrastLuminance());

      if(IsDangerous(Viewer))
	igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, 160, 16, Pos, 16, 16, configuration::GetContrastLuminance());
    }
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
      item* Item = Slot->GetItem();

      if(SquarePosition != HIDDEN && Item->IsAnimated() && SquareUnder)
	SquareUnder->DecAnimatedEntities();

      if(LastClean && Item->GetSquaresUnder() == 1)
	delete Item;
      else
	Item->SendToHell();

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
  SaveFile << SquarePosition;
  ushort SavedItems = 0;

  for(stackiterator i1 = GetBottom(); i1.HasItem(); ++i1)
    if(i1->IsMainSlot(&i1.GetSlot()))
      ++SavedItems;

  SaveFile << SavedItems;

  for(stackiterator i2 = GetBottom(); i2.HasItem(); ++i2)
    if(i2->IsMainSlot(&i2.GetSlot()))
      SaveFile << i2.GetSlot();
}

void stack::Load(inputfile& SaveFile)
{
  ushort SavedItems;
  SaveFile >> SquarePosition >> SavedItems;

  for(ushort c = 0; c < SavedItems; ++c)
    {
      if(!c && !Items)
	Bottom = Top = new stackslot(this, 0);
      else
	Top = Top->Next = new stackslot(this, Top);

      SaveFile >> *Top;
      Volume += (*Top)->GetVolume();
      Weight += (*Top)->GetWeight();
      game::CombineLights(Emitation, (*Top)->GetEmitation());
    }

  Items += SavedItems;
}

vector2d stack::GetPos() const
{
  return GetSquareUnder()->GetPos();
}

bool stack::SortedItems(const character* Viewer, bool (*SorterFunction)(const item*, const character*)) const
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

void stack::BeKicked(character* Kicker, ushort KickDamage, uchar Direction)
{
  if(KickDamage)
    {
      ReceiveDamage(Kicker, KickDamage, PHYSICAL_DAMAGE);

      if(GetItems() && SquarePosition == CENTER)
	{
	  item* Item1 = *GetTop();
	  item* Item2 = RAND() & 1 && GetItems() > 1 ? *--GetTop() : 0;
	  Item1->Fly(Kicker, Direction, KickDamage * 3);

	  if(Item2)
	    Item2->Fly(Kicker, Direction, KickDamage * 3);
	}
    }
  else if(GetVisibleItems(Kicker) && Kicker->IsPlayer())
    ADD_MESSAGE("Your weak kick has no effect.");
}

void stack::Polymorph(character* Polymorpher)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);
  ushort p = 0;

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists() && ItemVector[c]->Polymorph(Polymorpher, this) && ++p == 5)
      break;
}

void stack::CheckForStepOnEffect(character* Stepper)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists())
      {
	ItemVector[c]->StepOnEffect(Stepper);

	if(!Stepper->IsEnabled())
	  return;
      }
}

square* stack::GetSquareTrulyUnder() const
{
  switch(SquarePosition)
    {
    case DOWN:
      if(GetArea()->IsValidPos(GetPos() + vector2d(0, 1)))
	return GetNearSquare(GetPos() + vector2d(0, 1));
      else
	return 0;
    case LEFT:
      if(GetArea()->IsValidPos(GetPos() + vector2d(-1, 0)))
	return GetNearSquare(GetPos() + vector2d(-1, 0));
      else
	return 0;
    case UP:
      if(GetArea()->IsValidPos(GetPos() + vector2d(0, -1)))
	return GetNearSquare(GetPos() + vector2d(0, -1));
      else
	return 0;
    case RIGHT:
      if(GetArea()->IsValidPos(GetPos() + vector2d(1, 0)))
	return GetNearSquare(GetPos() + vector2d(1, 0));
      else
	return 0;
    default:
      return GetSquareUnder();
    }
}

void stack::ReceiveDamage(character* Damager, ushort Damage, ushort Type)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists())
      ItemVector[c]->ReceiveDamage(Damager, Damage, Type);
}

void stack::TeleportRandomly(ushort Amount)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size() && c < Amount; ++c)
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

item* stack::DrawContents(const character* Viewer, const festring& Topic, uchar Flags, bool (*SorterFunction)(const item*, const character*)) const
{
  itemvector ReturnVector;
  DrawContents(ReturnVector, 0, Viewer, Topic, CONST_S(""), CONST_S(""), Flags|NO_MULTI_SELECT, SorterFunction);
  return ReturnVector.empty() ? 0 : ReturnVector[0];
}

ushort stack::DrawContents(itemvector& ReturnVector, const character* Viewer, const festring& Topic, uchar Flags, bool (*SorterFunction)(const item*, const character*)) const
{
  return DrawContents(ReturnVector, 0, Viewer, Topic, CONST_S(""), CONST_S(""), Flags, SorterFunction);
}

/* MergeStack is used for showing two stacks together. Like when eating when there are items on the ground and in the character's stack */

ushort stack::DrawContents(itemvector& ReturnVector, stack* MergeStack, const character* Viewer, const festring& Topic, const festring& ThisDesc, const festring& ThatDesc, uchar Flags, bool (*SorterFunction)(const item*, const character*)) const
{
  felist Contents(Topic);

  if(!(Flags & NO_SPECIAL_INFO))
    {
      Contents.AddDescription(CONST_S(""));
      Contents.AddDescription(CONST_S("Overall weight: ") + (MergeStack ? GetWeight() + MergeStack->GetWeight() : GetWeight()) + " grams");
    }

  if(Flags & NONE_AS_CHOICE)
    Contents.AddEntry(CONST_S("none"), LIGHT_GRAY, 0, igraph::GetTransparentTile());

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

  game::DrawEverythingNoBlit(); //doesn't prevent mirage puppies
  ushort Chosen = Contents.Draw();

  if(Chosen & FELIST_ERROR_BIT)
    {
      Selected = 0;
      return Chosen;
    }
  else
    Selected = Chosen;

  ushort Pos = 0;

  if(Flags & NONE_AS_CHOICE)
    if(!Selected)
      return 0;
    else
      ++Pos;

  if(MergeStack)
    {
      Pos = MergeStack->SearchChosen(ReturnVector, Viewer, Pos, Selected, Flags, SorterFunction);

      if(!ReturnVector.empty())
	return 0;
    }

  SearchChosen(ReturnVector, Viewer, Pos, Selected, Flags, SorterFunction);
  return 0;
}

void stack::AddContentsToList(felist& Contents, const character* Viewer, const festring& Desc, uchar Flags, bool (*SorterFunction)(const item*, const character*)) const
{
  std::vector<itemvector> PileVector;
  Pile(PileVector, Viewer, SorterFunction);

  bool DrawDesc = Desc.GetSize() != 0;
  ulong LastCategory = 0;
  festring Entry;

  for(ushort p = 0; p < PileVector.size(); ++p)
    {
      if(DrawDesc)
	{
	  if(!Contents.IsEmpty())
	    Contents.AddEntry(CONST_S(""), WHITE, 0, 0, false);

	  Contents.AddEntry(Desc, WHITE, 0, 0, false);
	  Contents.AddEntry(CONST_S(""), WHITE, 0, 0, false);
	  DrawDesc = false;
	}

      item* Item = PileVector[p].back();

      if(Item->GetCategory() != LastCategory)
	{
	  LastCategory = Item->GetCategory();
	  Contents.AddEntry(item::GetItemCategoryName(LastCategory), LIGHT_GRAY, 0, 0, false);
	}

      Entry.Empty();
      Item->AddInventoryEntry(Viewer, Entry, PileVector[p].size(), !(Flags & NO_SPECIAL_INFO));
      Contents.AddEntry(Entry, LIGHT_GRAY, 0, Item->GetPicture(), Item->GetStackAnimationFrames(), true, Item->AllowAlphaEverywhere());
    }
}

ushort stack::SearchChosen(itemvector& ReturnVector, const character* Viewer, ushort Pos, ushort Chosen, uchar Flags, bool (*SorterFunction)(const item*, const character*)) const
{
  /* Not really efficient... :( */

  std::vector<itemvector> PileVector;
  Pile(PileVector, Viewer, SorterFunction);

  for(ushort p = 0; p < PileVector.size(); ++p)
    if(Pos++ == Chosen)
      if(Flags & NO_MULTI_SELECT)
	{
	  ushort Amount = Flags & SELECT_PAIR && PileVector[p][0]->HandleInPairs() && PileVector[p].size() >= 2 ? 2 : 1;
	  ReturnVector.assign(PileVector[p].end() - Amount, PileVector[p].end());
	  return 0xFFFF;
	}
      else
	{
	  ushort Amount = PileVector[p].size();

	  if(Amount > 1)
	    Amount = game::ScrollBarQuestion(CONST_S("How many ") + PileVector[p][0]->GetName(PLURAL) + '?', vector2d(16, 6), Amount, 1, 0, Amount, 0, WHITE, LIGHT_GRAY, DARK_GRAY);

	  ReturnVector.assign(PileVector[p].end() - Amount, PileVector[p].end());
	  return 0xFFFF;
	}

  return Pos;
}

bool stack::RaiseTheDead(character* Summoner)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->RaiseTheDead(Summoner))
      return true;

  return false;
}

bool stack::TryKey(item* Key, character* Applier)
{
  if(!Applier->IsPlayer())
    return false;

  item* ToBeOpened = DrawContents(Applier, CONST_S("Where do you wish to use the key?"), 0, &item::HasLockSorter);

  if(ToBeOpened == 0)
    return false;

  return ToBeOpened->TryKey(Key, Applier);
}

bool stack::Open(character* Opener)
{
  if(!Opener->IsPlayer())
    return false;

  item* ToBeOpened = DrawContents(Opener, CONST_S("What do you wish to open?"), 0, &item::OpenableSorter);

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

void stack::SignalVolumeAndWeightChange()
{
  if(!Freezed)
    {
      CalculateVolumeAndWeight();

      if(MotherEntity)
	MotherEntity->SignalVolumeAndWeightChange();
    }
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
  if(!Freezed)
    {
      if(game::CompareLights(EmitationUpdate, Emitation) > 0)
	{
	  game::CombineLights(Emitation, EmitationUpdate);

	  if(MotherEntity)
	    {
	      if(MotherEntity->AllowContentEmitation())
		MotherEntity->SignalEmitationIncrease(EmitationUpdate);
	    }
	  else
	    GetLSquareTrulyUnder()->SignalEmitationIncrease(EmitationUpdate);
	}
    }
}

void stack::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(!Freezed)
    {
      if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
	{
	  ulong Backup = Emitation;
	  CalculateEmitation();

	  if(Backup != Emitation)
	    {
	      if(MotherEntity)
		{
		  if(MotherEntity->AllowContentEmitation())
		    MotherEntity->SignalEmitationDecrease(EmitationUpdate);
		}
	      else
		GetLSquareTrulyUnder()->SignalEmitationDecrease(EmitationUpdate);
	    }
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
    return Viewer->IsOver(GetSquareTrulyUnder()->GetPos()) || GetSquareTrulyUnder()->CanBeSeenBy(Viewer);
}

bool stack::IsDangerousForAIToStepOn(const character* Stepper) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->IsDangerousForAI(Stepper) && i->CanBeSeenBy(Stepper))
      return true;

  return false;
}

/* Returns true if something was cloned. Max is the cap of items to be cloned */

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
    Slot->AddFriendItem(*GetBottom());
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

bool CategorySorter(const itemvector& V1, const itemvector& V2)
{
  return (*V1.begin())->GetCategory() < (*V2.begin())->GetCategory();
}

void stack::Pile(std::vector<itemvector>& PileVector, const character* Viewer, bool (*SorterFunction)(const item*, const character*)) const
{
  bool UseSorterFunction = SorterFunction != 0;
  std::list<item*> List;

  for(stackiterator s = GetBottom(); s.HasItem(); ++s)
    if((!UseSorterFunction || SorterFunction(*s, Viewer)) && (IgnoreVisibility || s->CanBeSeenBy(Viewer)))
      List.push_back(*s);

  for(std::list<item*>::iterator i = List.begin(); i != List.end(); ++i)
    {
      PileVector.resize(PileVector.size() + 1);
      itemvector& Pile = PileVector.back();
      Pile.push_back(*i);

      if((*i)->CanBePiled())
	{
	  std::list<item*>::iterator j = i;

	  for(++j; j != List.end();)
	    if((*j)->CanBePiled() && (*i)->CanBePiledWith(*j, Viewer))
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

ulong stack::GetTruePrice() const
{
  ulong Price = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    Price += i->GetTruePrice();

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

bool stack::TakeSomethingFrom(character* Opener, const festring& ContainerName)
{
  if(!GetItems())
    {
      ADD_MESSAGE("There is nothing in %s.", ContainerName.CStr());
      return false;
    }

  bool Success = false;
  room* Room = GetLSquareUnder()->GetRoom();
  SetSelected(0);

  for(;;)
    {
      itemvector ToTake;
      game::DrawEverythingNoBlit();
      DrawContents(ToTake, Opener, CONST_S("What do you want to take from ") + ContainerName + '?', REMEMBER_SELECTED);

      if(ToTake.empty())
	break;

      if(!IsOnGround() || !Room || Room->PickupItem(Opener, ToTake[0], ToTake.size()))
	{
	  for(ushort c = 0; c < ToTake.size(); ++c)
	    ToTake[c]->MoveTo(Opener->GetStack());

	  ADD_MESSAGE("You take %s from %s.", ToTake[0]->GetName(DEFINITE, ToTake.size()).CStr(), ContainerName.CStr());
	  Success = true;
	}
    }

  return Success;
}

bool stack::PutSomethingIn(character* Opener, const festring& ContainerName, ulong StorageVolume, ulong ContainerID)
{
  if(!Opener->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to put in %s.", ContainerName.CStr());
      return false;
    }

  bool Success = false;
  room* Room = GetLSquareUnder()->GetRoom();
  stack::SetSelected(0);

  for(;;)
    {
      itemvector ToPut;
      game::DrawEverythingNoBlit();
      Opener->GetStack()->DrawContents(ToPut, Opener, CONST_S("What do you want to put in ") + ContainerName + '?', REMEMBER_SELECTED);

      if(ToPut.empty())
	break;

      if(ToPut[0]->GetID() == ContainerID)
	{
	  ADD_MESSAGE("You can't put %s inside itself!", ContainerName.CStr());
	  continue;
	}

      ushort Amount = Min<ushort>((StorageVolume - GetVolume()) / ToPut[0]->GetVolume(), ToPut.size());

      if(!Amount)
	{
	  if(ToPut.size() == 1)
	    ADD_MESSAGE("%s doesn't fit in %s.", ToPut[0]->CHAR_NAME(DEFINITE), ContainerName.CStr());
	  else
	    ADD_MESSAGE("None of the %d %s fits in %s.", ToPut.size(), ToPut[0]->CHAR_NAME(PLURAL), ContainerName.CStr());

	  continue;
	}

      if(Amount != ToPut.size())
	ADD_MESSAGE("Only %d of the %d %s fit%s in %s.", Amount, ToPut.size(), ToPut[0]->CHAR_NAME(PLURAL), Amount == 1 ? "s" : "", ContainerName.CStr());

      if(!IsOnGround() || !Room || Room->DropItem(Opener, ToPut[0], Amount))
	{
	  for(ushort c = 0; c < Amount; ++c)
	    ToPut[c]->MoveTo(this);

	  ADD_MESSAGE("You put %s in %s.", ToPut[0]->GetName(DEFINITE, Amount).CStr(), ContainerName.CStr());
	  Success = true;
	}
    }

  return Success;
}

bool stack::IsOnGround() const
{
  return !MotherEntity || MotherEntity->IsOnGround();
}

ushort stack::GetSpoiledItems() const
{
  ushort Counter = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    Counter += (i->GetSpoilLevel() > 0);

  return Counter;
}

void stack::SortAllItems(itemvector& ItemVector, const character* Character, bool (*Sorter)(const item*, const character*)) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->SortAllItems(ItemVector, Character, Sorter);
}

void stack::Search(const character* Char, ushort Perception)
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->Search(Char, Perception);
}

bool stack::IsDangerous(const character* Viewer) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->IsDangerous() && i->CanBeSeenBy(Viewer))
      return true;

  return false;
}

void stack::PreProcessForBone()
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    ItemVector[c]->PreProcessForBone();
}

void stack::PostProcessForBone()
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    ItemVector[c]->PostProcessForBone();
}

void stack::FinalProcessForBone()
{
  /* Items can't be removed during the final processing stage */

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->FinalProcessForBone();
}
