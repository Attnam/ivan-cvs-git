#define __FILE_OF_STATIC_ACTION_PROTOTYPE_DECLARATIONS__

#include "proto.h"

class action;

std::vector<action*>			protocontainer<action>::ProtoData;
std::map<std::string, ushort>		protocontainer<action>::CodeNameMap;

#include "actionde.h"

#undef __FILE_OF_STATIC_ACTION_PROTOTYPE_DECLARATIONS__

#include "save.h"
#include "message.h"
#include "charba.h"
#include "lsquare.h"
#include "itemba.h"
#include "level.h"

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
  SaveFile << Consuming;
}

void consume::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Consuming;
}

void consume::Handle()
{
  /*if(ulong(++StateCounter[CONSUMING]) * 500 >= GetConsumingCurrently()->ConsumeLimit())
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You finish %s %s.", GetConsumingCurrently()->GetConsumeVerb().c_str(), GetConsumingCurrently()->CHARNAME(DEFINITE));
      else
	if(GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s finishes eating %s.", CHARNAME(DEFINITE), GetConsumingCurrently()->CHARNAME(DEFINITE));

      EndConsuming();

      return;
    }*/
}

void consume::Terminate(bool Finished)
{
  /*if(StateIsActivated(CONSUMING))
    {
      if(GetConsumingCurrently()->Consume(this, StateCounter[CONSUMING] * 500))
	{
	  GetConsumingCurrently()->RemoveFromSlot();
	  /item* ToBeDeleted = GetStack()->RemoveItem(GetStack()->SearchItem(GetConsumingCurrently()));

	  if(!ToBeDeleted)
	    ToBeDeleted = GetLSquareUnder()->GetStack()->RemoveItem(GetLSquareUnder()->GetStack()->SearchItem(GetConsumingCurrently()));/

	  GetConsumingCurrently()->SetExists(false);
	}

      ReceiveBulimiaDamage();
      DeActivateState(CONSUMING);
      SetConsumingCurrently(0);
    }*/

  if(Finished)
    {
      if(GetActor()->GetIsPlayer())
	ADD_MESSAGE("You finish %s.", Consuming->GetConsumeVerb().c_str());
      else if(GetActor()->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s finishes %s.", GetActor()->CHARNAME(DEFINITE), Consuming->GetConsumeVerb().c_str());
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

  /*if(StateCounter[DIGGING] > 0)
    {
      StateCounter[DIGGING]--;
      EditStrengthExperience(5);
      EditAgilityExperience(-5);
      EditNP(-5);
    }
  else
    EndDig();*/
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
