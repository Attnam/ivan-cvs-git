#include "slot.h"
#include "stack.h"
#include "save.h"
#include "itemde.h"
#include "charba.h"
#include "lsquare.h"
#include "actionba.h"
#include "proto.h"
#include "game.h"

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
  GetMotherStack()->RemoveItem(StackIterator);
}

void stackslot::FastEmpty()
{
  GetMotherStack()->FastRemoveItem(StackIterator);
}

void characterslot::Empty()
{
  /* add light update */

  EditVolume(-Item->GetVolume());
  EditWeight(-Item->GetWeight());
  Item = 0;
  GetMaster()->CalculateEquipmentStates();
}

void characterslot::FastEmpty()
{
  EditVolume(-Item->GetVolume());
  EditWeight(-Item->GetWeight());
  Item = 0;
  GetMaster()->CalculateEquipmentStates();
}

void stackslot::MoveItemTo(stack* Stack)
{
  GetMotherStack()->MoveItem(GetStackIterator(), Stack);
}

void characterslot::MoveItemTo(stack* Stack)
{
  Stack->AddItem(Item);
  Empty();
}

void gearslot::Empty()
{
  /* add light update? */

  EditVolume(-Item->GetVolume());
  EditWeight(-Item->GetWeight());
  Item = 0;
  GetBodyPart()->SignalEquipmentRemoval(this);
}

void gearslot::FastEmpty()
{
  EditVolume(-Item->GetVolume());
  EditWeight(-Item->GetWeight());
  Item = 0;
  GetBodyPart()->SignalEquipmentRemoval(this);
}

void gearslot::MoveItemTo(stack* Stack)
{
  Stack->AddItem(Item);
  Empty();
}

void gearslot::Init(bodypart* BodyPart, uchar Index)
{
  SetBodyPart(BodyPart);
  SetEquipmentIndex(Index);
}

void actionslot::Empty()
{
  /* add light update? */

  EditVolume(-Item->GetVolume());
  EditWeight(-Item->GetWeight());
  Item = 0;
}

void actionslot::FastEmpty()
{
  EditVolume(-Item->GetVolume());
  EditWeight(-Item->GetWeight());
  Item = 0;
}

void actionslot::MoveItemTo(stack* Stack)
{
  Stack->AddItem(Item);
  Empty();
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
    GetMaster()->GetLSquareUnder()->GetStack()->AddItem(Item);
  else
    GetMaster()->GetStack()->AddItem(Item);
}

void gearslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();

  if(!game::IsInWilderness())
    GetBodyPart()->GetLSquareUnder()->GetStack()->AddItem(Item);
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

void stackslot::EditVolume(long What)
{
  GetMotherStack()->EditVolume(What);
}

void stackslot::EditWeight(long What)
{
  GetMotherStack()->EditWeight(What);
}

void characterslot::EditVolume(long What)
{
  GetMaster()->EditVolume(What);
}

void characterslot::EditCarriedWeight(long What)
{
  GetMaster()->EditWeight(What);
}

void gearslot::EditVolume(long What)
{
  GetBodyPart()->EditVolume(What);
}

void gearslot::EditWeight(long What)
{
  GetBodyPart()->EditWeight(What);
  GetBodyPart()->EditCarriedWeight(What);
}

void actionslot::EditVolume(long What)
{
  GetAction()->EditVolume(What);
}

void actionslot::EditWeight(long What)
{
  GetAction()->EditWeight(What);
}

void slot::PutInItem(item* What)
{
  Item = What;

  if(Item)
    {
      Item->SetSlot(this);
      EditVolume(Item->GetVolume());
      EditWeight(Item->GetWeight());
    }
}

void gearslot::PutInItem(item* What)
{
  Item = What;

  if(Item)
    {
      Item->SetSlot(this);
      EditVolume(Item->GetVolume());
      EditWeight(Item->GetWeight());
      GetBodyPart()->SignalEquipmentAdd(this);
    }
}
