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

void stackslot::Empty() const
{
  GetMotherStack()->RemoveItem(StackIterator);
}

void stackslot::FastEmpty() const
{
  GetMotherStack()->FastRemoveItem(StackIterator);
}

void characterslot::Empty() const
{
}

void characterslot::FastEmpty() const
{
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
