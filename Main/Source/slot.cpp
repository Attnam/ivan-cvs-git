#include "slot.h"
#include "stack.h"
#include "save.h"
#include "itemde.h"
#include "charba.h"
#include "lsquare.h"
#include "actionba.h"
#include "proto.h"

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

  SetItem(0);
}

void characterslot::FastEmpty()
{
  SetItem(0);
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

void slot::SetItem(item* Item)
{
  this->Item = Item;

  if(Item)
    Item->SetSlot(this);
}

void gearslot::Empty()
{
  /* add light update? */

  SetItem(0);
  GetBodyPart()->SignalGearUpdate();
}

void gearslot::FastEmpty()
{
  SetItem(0);
  GetBodyPart()->SignalGearUpdate();
}

void gearslot::MoveItemTo(stack* Stack)
{
  Stack->AddItem(Item);
  Empty();
}

void gearslot::Init(bodypart* BodyPart)
{
  SetBodyPart(BodyPart);
  SetItem(0);
}

void actionslot::Empty()
{
  /* add light update? */

  SetItem(0);
}

void actionslot::FastEmpty()
{
  SetItem(0);
}

void actionslot::MoveItemTo(stack* Stack)
{
  Stack->AddItem(Item);
  Empty();
}

void actionslot::Init(action* Action)
{
  SetAction(Action);
  SetItem(0);
}

void stackslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();
  GetMotherStack()->AddItem(Item);
}

void characterslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();

  if(!game::GetInWilderness())
    GetMaster()->GetLSquareUnder()->GetStack()->AddItem(Item);
  else
    GetMaster()->GetStack()->AddItem(Item);
}

void gearslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();

  if(!game::GetInWilderness())
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
