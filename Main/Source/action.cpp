/* Compiled through actset.cpp */

actionprototype::actionprototype(action* (*Cloner)(bool), const char* ClassID) : Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<action>::Add(this); }

action::action(donothing) : Actor(0), DNDMode(false) { }
action::~action() { }

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
