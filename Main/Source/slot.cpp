#include "slot.h"
#include "stack.h"
#include "save.h"
#include "itemba.h"

void slot::Save(outputfile& SaveFile) const
{
  SaveFile << Item;
}

void slot::Load(inputfile& SaveFile)
{
  SaveFile >> Item;
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
  Empty();
  Stack->AddItem(Item);
}

void slot::PutInItem(item* Item)
{
  SetItem(Item);

  if(Item)
    Item->SetSlot(this);
}

void gearslot::Empty()
{
  /* add light update? */

  SetItem(0);
}

void gearslot::FastEmpty()
{
  SetItem(0);
}

void gearslot::MoveItemTo(stack* Stack)
{
  Empty();
  Stack->AddItem(Item);
}

void gearslot::Init(bodypart* BodyPart)
{
  SetBodyPart(BodyPart);
  SetItem(0);
}
