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
  SaveFile << GetType() << DNDMode;
}

void action::Load(inputfile& SaveFile)
{
  SaveFile >> DNDMode;
}

action* actionprototype::CloneAndLoad(inputfile& SaveFile) const
{
  action* Action = Cloner(true);
  Action->Load(SaveFile);
  return Action;
}

actionprototype::actionprototype(action* (*Cloner)(bool), const std::string& ClassId) : Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<action>::Add(this);
}

void action::LoadActionSlot(inputfile& SaveFile, actionslot& ActionSlot)
{
  SaveFile >> ActionSlot;
  ActionSlot.SetAction(this);
}
