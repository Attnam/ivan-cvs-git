/* Compiled through slotset.cpp */

void slot::Save(outputfile& SaveFile) const
{
  SaveFile << Item;
}

void slot::Load(inputfile& SaveFile)
{
  SaveFile >> Item;

  if(Item)
    Item->SetMainSlot(this);
}

void stackslot::Load(inputfile& SaveFile)
{
  SaveFile >> Item;

  if(Item)
    Item->SignalStackAdd(this, &stack::AddElement);
}

void stackslot::Empty()
{
  GetMotherStack()->RemoveItem(this);
}

void bodypartslot::Empty()
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

void stackslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();
  GetMotherStack()->AddItem(Item);
}

void bodypartslot::AddFriendItem(item* Item) const
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

bool stackslot::IsOnGround() const
{
  return GetMotherStack()->IsOnGround();
}

void stackslot::SignalVolumeAndWeightChange()
{
  GetMotherStack()->SignalVolumeAndWeightChange();
}

void bodypartslot::SignalVolumeAndWeightChange()
{
  GetMaster()->SignalVolumeAndWeightChange();
}

void gearslot::SignalVolumeAndWeightChange()
{
  GetBodyPart()->SignalVolumeAndWeightChange();
}

void slot::PutInItem(item* What)
{
  Item = What;

  if(Item)
    {
      Item->SetMainSlot(this);
      SignalVolumeAndWeightChange();
      SignalEmitationIncrease(Item->GetEmitation());
    }
}

void stackslot::PutInItem(item* What)
{
  Item = What;

  if(Item)
    {
      Item->SignalStackAdd(this, &stack::AddItem);
      SignalVolumeAndWeightChange();
      SignalEmitationIncrease(Item->GetEmitation());
    }
}

void bodypartslot::PutInItem(item* What)
{
  Item = What;

  if(Item)
    {
      Item->SetMainSlot(this);
      static_cast<bodypart*>(Item)->SetMaster(GetMaster());

      if(!GetMaster()->IsInitializing())
	{
	  SignalVolumeAndWeightChange();
	  SignalEmitationIncrease(Item->GetEmitation());
	  static_cast<bodypart*>(Item)->CalculateMaxHP(false);
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
      Item->SetMainSlot(this);
      GetBodyPart()->SignalEquipmentAdd(this);
      SignalVolumeAndWeightChange();
      SignalEmitationIncrease(Item->GetEmitation());
    }
}

square* stackslot::GetSquareUnder(ushort) const
{
  return GetMotherStack()->GetSquareUnder();
}

square* bodypartslot::GetSquareUnder(ushort Index) const
{
  return GetMaster()->GetSquareUnder(Index);
}

square* gearslot::GetSquareUnder(ushort) const
{
  return GetBodyPart()->GetSquareUnder();
}

void stackslot::SignalEmitationIncrease(ulong Emitation)
{
  GetMotherStack()->SignalEmitationIncrease(Emitation);
}

void bodypartslot::SignalEmitationIncrease(ulong Emitation)
{
  GetMaster()->SignalEmitationIncrease(Emitation);
}

void gearslot::SignalEmitationIncrease(ulong Emitation)
{
  GetBodyPart()->SignalEmitationIncrease(Emitation);
}

void stackslot::SignalEmitationDecrease(ulong Emitation)
{
  GetMotherStack()->SignalEmitationDecrease(Emitation);
}

void bodypartslot::SignalEmitationDecrease(ulong Emitation)
{
  GetMaster()->SignalEmitationDecrease(Emitation);
}

void gearslot::SignalEmitationDecrease(ulong Emitation)
{
  GetBodyPart()->SignalEmitationDecrease(Emitation);
}

void bodypartslot::Load(inputfile& SaveFile)
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

bool bodypartslot::CanBeSeenBy(const character* Viewer) const
{
  return GetMaster()->CanBeSeenBy(Viewer);
}

bool gearslot::CanBeSeenBy(const character* Viewer) const
{
  return GetBodyPart()->CanBeSeenBy(Viewer);
}

void gearslot::SignalEnchantmentChange()
{
  GetBodyPart()->SignalEnchantmentChange();
}

bool stackslot::IsVisible() const
{
  return GetMotherStack()->IsVisible();
}
