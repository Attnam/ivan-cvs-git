/* Compiled through slotset.cpp */

void slot::Save(outputfile& SaveFile) const
{
  SaveFile << Item;
}

void slot::Load(inputfile& SaveFile)
{
  SaveFile >> Item;

  if(Item)
    Item->SetSlot(this);
}

void stackslot::Empty()
{
  GetMotherStack()->RemoveItem(this);
}

void characterslot::Empty()
{
  ulong Emitation = Item->GetEmitation();
  static_cast<bodypart*>(Item)->SetMaster(0);
  Item = 0;
  GetMaster()->CalculateEquipmentState();
  SignalVolumeAndWeightChange();
  SignalEmitationDecrease(Emitation);

  if(!GetMaster()->IsInitializing())
    {
      GetMaster()->CalculateHP();
      GetMaster()->CalculateMaxHP();
    }
}

void gearslot::Empty()
{
  ulong Emitation = Item->GetEmitation();
  Item = 0;
  GetBodyPart()->SignalEquipmentRemoval(this);
  SignalVolumeAndWeightChange();
  SignalEmitationDecrease(Emitation);
}

void gearslot::Init(bodypart* BodyPart, uchar Index)
{
  SetBodyPart(BodyPart);
  SetEquipmentIndex(Index);
}

void actionslot::Empty()
{
  ulong Emitation = Item->GetEmitation();
  Item = 0;
  SignalVolumeAndWeightChange();
  SignalEmitationDecrease(Emitation);
}

void actionslot::Init(action* Action)
{
  SetAction(Action);
}

void stackslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();
  GetMotherStack()->AddItem(Item);
}

void characterslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();

  if(!game::IsInWilderness())
    GetMaster()->GetStackUnder()->AddItem(Item);
  else
    GetMaster()->GetStack()->AddItem(Item);
}

void gearslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();

  if(!game::IsInWilderness())
    GetBodyPart()->GetLSquareUnder()->AddItem(Item);
  else
    GetBodyPart()->GetMaster()->GetStack()->AddItem(Item);
}

void actionslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();
  GetAction()->GetActor()->GetStack()->AddItem(Item);
}

bool stackslot::IsOnGround() const
{
  return GetMotherStack()->IsOnGround();
}

void stackslot::SignalVolumeAndWeightChange()
{
  GetMotherStack()->SignalVolumeAndWeightChange();
}

void characterslot::SignalVolumeAndWeightChange()
{
  GetMaster()->SignalVolumeAndWeightChange();
}

void gearslot::SignalVolumeAndWeightChange()
{
  GetBodyPart()->SignalVolumeAndWeightChange();
}

void actionslot::SignalVolumeAndWeightChange()
{
  GetAction()->GetActor()->SignalVolumeAndWeightChange();
}

void slot::PutInItem(item* What)
{
  Item = What;

  if(Item)
    {
      Item->SetSlot(this);
      SignalVolumeAndWeightChange();
      SignalEmitationIncrease(Item->GetEmitation());
    }
}

void characterslot::PutInItem(item* What)
{
  Item = What;

  if(Item)
    {
      Item->SetSlot(this);
      static_cast<bodypart*>(Item)->SetMaster(GetMaster());

      if(!GetMaster()->IsInitializing())
	{
	  SignalVolumeAndWeightChange();
	  SignalEmitationIncrease(Item->GetEmitation());
	  static_cast<bodypart*>(Item)->CalculateMaxHP();
	  GetMaster()->CalculateHP();
	  GetMaster()->CalculateMaxHP();
	}
    }
}

void gearslot::PutInItem(item* What)
{
  Item = What;

  if(Item)
    {
      Item->SetSlot(this);
      GetBodyPart()->SignalEquipmentAdd(this);
      SignalVolumeAndWeightChange();
      SignalEmitationIncrease(Item->GetEmitation());
    }
}

square* stackslot::GetSquareUnder() const
{
  return GetMotherStack()->GetSquareUnder();
}

square* characterslot::GetSquareUnder() const
{
  return GetMaster()->GetSquareUnder();
}

square* gearslot::GetSquareUnder() const
{
  return GetBodyPart()->GetSquareUnder();
}

square* actionslot::GetSquareUnder() const
{
  return GetAction()->GetActor()->GetSquareUnder();
}

void stackslot::SignalEmitationIncrease(ulong Emitation)
{
  GetMotherStack()->SignalEmitationIncrease(Emitation);
}

void characterslot::SignalEmitationIncrease(ulong Emitation)
{
  GetMaster()->SignalEmitationIncrease(Emitation);
}

void gearslot::SignalEmitationIncrease(ulong Emitation)
{
  GetBodyPart()->SignalEmitationIncrease(Emitation);
}

void actionslot::SignalEmitationIncrease(ulong Emitation)
{
  GetAction()->GetActor()->SignalEmitationIncrease(Emitation);
}

void stackslot::SignalEmitationDecrease(ulong Emitation)
{
  GetMotherStack()->SignalEmitationDecrease(Emitation);
}

void characterslot::SignalEmitationDecrease(ulong Emitation)
{
  GetMaster()->SignalEmitationDecrease(Emitation);
}

void gearslot::SignalEmitationDecrease(ulong Emitation)
{
  GetBodyPart()->SignalEmitationDecrease(Emitation);
}

void actionslot::SignalEmitationDecrease(ulong Emitation)
{
  GetAction()->GetActor()->SignalEmitationDecrease(Emitation);
}

void characterslot::Load(inputfile& SaveFile)
{
  slot::Load(SaveFile);

  if(Item)
    static_cast<bodypart*>(Item)->SetMaster(GetMaster());
}

void slot::DonateTo(item* Item)
{
  Empty();
  PutInItem(Item);
}

void stackslot::DonateTo(item* Item) // could be optimized
{
  AddFriendItem(Item);
  Empty();
}

bool stackslot::CanBeSeenBy(const character* Viewer) const
{
  return GetMotherStack()->CanBeSeenBy(Viewer);
}

bool characterslot::CanBeSeenBy(const character* Viewer) const
{
  return GetMaster()->CanBeSeenBy(Viewer);
}

bool gearslot::CanBeSeenBy(const character* Viewer) const
{
  return GetBodyPart()->CanBeSeenBy(Viewer);
}

bool actionslot::CanBeSeenBy(const character* Viewer) const
{
  return GetAction()->GetActor()->CanBeSeenBy(Viewer);
}

void gearslot::SignalEnchantmentChange()
{
  GetBodyPart()->SignalEnchantmentChange();
}

bool stackslot::IsVisible() const
{
  return GetMotherStack()->IsVisible();
}
