#define __FILE_OF_STATIC_ACTION_PROTOTYPE_DECLARATIONS__

#include "proto.h"
#include "actionba.h"

std::vector<action::prototype*>		protocontainer<action>::ProtoData;
std::map<std::string, ushort>		protocontainer<action>::CodeNameMap;;

#include "actionde.h"

#undef __FILE_OF_STATIC_ACTION_PROTOTYPE_DECLARATIONS__

#include "message.h"
#include "save.h"
#include "charba.h"
#include "lsquare.h"
#include "itemba.h"
#include "level.h"
#include "error.h"

void faint::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << Counter;
}

void faint::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Counter;
}

void faint::Handle()
{
  if(!(Counter--))
    {
      if(GetActor()->GetIsPlayer())
	ADD_MESSAGE("You wake up.");
      else
	if(GetActor()->GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s wakes up.", GetActor()->CHARNAME(DEFINITE));

      Terminate(true);
    }
  else
    {
      GetActor()->EditStrengthExperience(-3);
    }
}

void faint::Terminate(bool Finished)
{
  if(GetActor()->GetIsPlayer())
    ADD_MESSAGE("You wake up.");
  else if(GetActor()->GetSquareUnder()->CanBeSeen())
    ADD_MESSAGE("%s wakes up.", GetActor()->CHARNAME(DEFINITE));

  action::Terminate(Finished);
}

void consume::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << Consuming << WasOnGround;
}

void consume::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Consuming >> WasOnGround;
}

void consume::Handle()
{
  if(!*Consuming)
    Terminate(false);

  SetHasEaten(true);

  if(Consuming->Consume(GetActor(), 500))
    Terminate(true);
}

void consume::Terminate(bool Finished)
{
  if(Finished)
    {
      if(GetActor()->GetIsPlayer())
	ADD_MESSAGE("You finish %s %s.", Consuming->GetConsumeVerb().c_str(), Consuming->CHARNAME(DEFINITE));
      else if(GetActor()->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s finishes %s %s.", GetActor()->CHARNAME(DEFINITE), Consuming->GetConsumeVerb().c_str(), Consuming->CHARNAME(DEFINITE));

      if(GetHasEaten())
	Consuming->AddConsumeEndMessage(GetActor());

      Consuming->GenerateLeftOvers(GetActor());
    }
  else if(*Consuming)
    {
      if(GetActor()->GetIsPlayer())
	ADD_MESSAGE("You stop %s %s.", Consuming->GetConsumeVerb().c_str(), Consuming->CHARNAME(DEFINITE));
      else if(GetActor()->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s stops %s %s.", GetActor()->CHARNAME(DEFINITE), Consuming->GetConsumeVerb().c_str(), Consuming->CHARNAME(DEFINITE));

      if(GetHasEaten())
	Consuming->AddConsumeEndMessage(GetActor());

      if(GetWasOnGround())
	Consuming->MoveTo(GetActor()->GetLSquareUnder()->GetStack());
      else
	Consuming->MoveTo(GetActor()->GetStack());

      Consuming.SetItem(0);
    }
  else
    {
      if(GetActor()->GetIsPlayer())
	ADD_MESSAGE("You stop %s.", Consuming->GetConsumeVerb().c_str());
      else if(GetActor()->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s stops %s.", GetActor()->CHARNAME(DEFINITE), Consuming->GetConsumeVerb().c_str());
    }

  action::Terminate(Finished);
}

item* consume::GetConsuming() const
{
  return *Consuming;
}

void consume::SetConsuming(item* Food)
{
  if(Food)
    Food->RemoveFromSlot();
  else
    ABORT("Consuming nothing!");

  Consuming.SetItem(Food);
}

void rest::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << GoalHP;
}

void rest::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> GoalHP;
}

void rest::Handle()
{
  if(GetActor()->GetHP() >= GoalHP || GetActor()->GetHP() == GetActor()->GetMaxHP())
    Terminate(true);
  else
    GetActor()->EditAgilityExperience(-1 * GetActor()->GetSquareUnder()->RestModifier());
}

void rest::Terminate(bool Finished)
{
  if(Finished)
    {
      if(GetActor()->GetIsPlayer())
	ADD_MESSAGE("You finish resting.");
      else if(GetActor()->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s finishes resting.", GetActor()->CHARNAME(DEFINITE));
    }
  else
    {
      if(GetActor()->GetIsPlayer())
	ADD_MESSAGE("You stop resting.");
      else if(GetActor()->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s stops resting.", GetActor()->CHARNAME(DEFINITE));
    }

  action::Terminate(Finished);
}

void dig::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << RightBackup << LeftBackup << SquareDug;
}

void dig::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> RightBackup >> LeftBackup >> SquareDug;
}

void dig::Handle()
{
  if(!GetActor()->GetMainWielded())
    Terminate(false);

  lsquare* Square = GetActor()->GetLSquareUnder()->GetLevelUnder()->GetLSquare(SquareDug);
  Square->GetOLTerrain()->EditHP(-long(GetActor()->GetStrength()) * GetActor()->GetMainWielded()->GetMainMaterial()->GetStrengthValue() / Square->GetOLTerrain()->GetMainMaterial()->GetStrengthValue());

  if(Square->GetOLTerrain()->GetHP() <= 0)
    {
      if(Square->CanBeSeen())
	ADD_MESSAGE("%s", Square->GetOLTerrain()->DigMessage().c_str());

      Square->GetOLTerrain()->Break();
      GetActor()->GetMainWielded()->MoveTo(GetActor()->GetStack());
      GetActor()->SetRightWielded(GetRightBackup());
      SetRightBackup(0);
      GetActor()->SetLeftWielded(GetLeftBackup());
      SetLeftBackup(0);
      Terminate(true);
    }
}

void dig::Terminate(bool Finished)
{
  if(!Finished)
    {
      if(GetActor()->GetIsPlayer())
	ADD_MESSAGE("You stop digging.");
      else if(GetActor()->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s stops digging.", GetActor()->CHARNAME(DEFINITE));
    }

  if(GetRightBackup())
    GetRightBackup()->MoveTo(GetActor()->GetStack());

  if(GetLeftBackup())
    GetLeftBackup()->MoveTo(GetActor()->GetStack());

  action::Terminate(Finished);
}

item* dig::GetRightBackup() const
{
  return *RightBackup;
}

void dig::SetRightBackup(item* Item)
{
  if(Item)
    Item->RemoveFromSlot();

  RightBackup.SetItem(Item);
}

item* dig::GetLeftBackup() const
{
  return *LeftBackup;
}

void dig::SetLeftBackup(item* Item)
{
  if(Item)
    Item->RemoveFromSlot();

  LeftBackup.SetItem(Item);
}

void go::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << Direction << WalkingInOpen;
}

void go::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Direction >> WalkingInOpen;
}

void go::Handle()
{
  GetActor()->GoOn(this);
}

ulong consume::GetWeight() const
{
  if(GetConsuming())
    return GetConsuming()->GetWeight();
  else
    return 0;
}

ulong dig::GetWeight() const
{
  ulong Weight = 0;

  if(GetRightBackup())
    Weight += GetRightBackup()->GetWeight();

  if(GetLeftBackup())
    Weight += GetLeftBackup()->GetWeight();

  return Weight;
}

void consume::DropUsedItems()
{
  if(GetConsuming())
    if(!game::GetInWilderness())
      GetConsuming()->MoveTo(GetActor()->GetLSquareUnder()->GetStack());
    else
      GetConsuming()->MoveTo(GetActor()->GetStack());
}

void consume::DeleteUsedItems()
{
  if(GetConsuming())
    GetConsuming()->SetExists(false);
}

void dig::DropUsedItems()
{
  if(GetRightBackup())
    if(!game::GetInWilderness())
      GetRightBackup()->MoveTo(GetActor()->GetLSquareUnder()->GetStack());
    else
      GetRightBackup()->MoveTo(GetActor()->GetStack());

  if(GetLeftBackup())
    if(!game::GetInWilderness())
      GetLeftBackup()->MoveTo(GetActor()->GetLSquareUnder()->GetStack());
    else
      GetLeftBackup()->MoveTo(GetActor()->GetStack());
}

void dig::DeleteUsedItems()
{
  if(GetRightBackup())
    GetRightBackup()->SetExists(false);

  if(GetLeftBackup())
    GetLeftBackup()->SetExists(false);
}
