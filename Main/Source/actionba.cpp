#include "actionba.h"
#include "charba.h"
#include "save.h"
#include "proto.h"

void action::Terminate(bool)
{
  GetActor()->SetAction(0);
  delete this;
}

void action::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
}

action* action_prototype::CloneAndLoad(inputfile& SaveFile) const
{
  action* Action = Clone();
  Action->Load(SaveFile);
  return Action;
}

action_prototype::action_prototype()
{
  Index = protocontainer<action>::Add(this);
}
