#include "actionba.h"
#include "charba.h"
#include "save.h"
#include "proto.h"
#include "slot.h"
#include "itemba.h"

void action::Terminate(bool)
{
  GetActor()->SetAction(0);
  delete this;
}

void action::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
}

action* actionprototype::CloneAndLoad(inputfile& SaveFile) const
{
  action* Action = Clone();
  Action->Load(SaveFile);
  return Action;
}

actionprototype::actionprototype()
{
  Index = protocontainer<action>::Add(this);
}

void action::EditVolume(long What)
{
  Volume += What;

  if(GetActor())
    GetActor()->EditVolume(What);
}

void action::EditWeight(long What)
{
  Weight += What;

  if(GetActor())
    GetActor()->EditWeight(What);
}

void action::LoadActionSlot(inputfile& SaveFile, actionslot& ActionSlot)
{
  SaveFile >> ActionSlot;
  ActionSlot.SetAction(this);

  if(*ActionSlot)
    {
      EditVolume(ActionSlot->GetVolume());
      EditWeight(ActionSlot->GetWeight());
    }
}
