#define __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "actionba.h"

std::vector<action::prototype*> protocontainer<action>::ProtoData;
valuemap protocontainer<action>::CodeNameMap;

#include "actionde.h"

#undef __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__

#include "message.h"
#include "save.h"
#include "charba.h"
#include "lsquare.h"
#include "itemba.h"
#include "level.h"
#include "error.h"
#include "game.h"
#include "lterraba.h"
#include "materba.h"

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
  if(!Counter--)
    Terminate(true);
  else
    {
      GetActor()->EditExperience(ARM_STRENGTH, -2);
      GetActor()->EditExperience(LEG_STRENGTH, -2);
    }
}

void faint::Terminate(bool Finished)
{
  if(GetActor()->IsPlayer())
    ADD_MESSAGE("You wake up.");
  else if(GetActor()->CanBeSeenByPlayer())
    ADD_MESSAGE("%s wakes up.", GetActor()->CHAR_NAME(DEFINITE));

  action::Terminate(Finished);
}

void consume::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << WasOnGround;
  SaveFile << Consuming;
}

void consume::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> WasOnGround;
  LoadActionSlot(SaveFile, Consuming);
}

void consume::Handle()
{
  if(!*Consuming)
    {
      Terminate(false);
      return;
    }

  character* Actor = GetActor();

  if(!InDNDMode() && Actor->CheckBulimia())
    if(Actor->IsPlayer())
      {
	ADD_MESSAGE("You have a really hard time getting all this down your throat.");

	if(game::BoolQuestion("Continue " + GetDescription() + "? [y/N]"))
	  SetInDNDMode(true);
	else
	  {
	    Terminate(false);
	    return;
	  }
      }
    else
      {
	Terminate(false);
	return;
      }

  if(!Actor->IsPlayer() && !Consuming->CanBeEatenByAI(Actor)) // item may be spoiled after action was started
    {
      Terminate(false);
      return;
    }

  SetHasEaten(true);

  /* Note: if backupped Actor has died of food effect, Action is deleted automatically, so we mustn't Terminate it */

  if(Consuming->Consume(Actor, 500) && Actor->GetAction() && Actor->IsEnabled())
    Terminate(true);
}

void consume::Terminate(bool Finished)
{
  if(Finished)
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You finish %s %s.", Description.c_str(), Consuming->CHAR_NAME(DEFINITE));
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s finishes %s %s.", GetActor()->CHAR_NAME(DEFINITE), Description.c_str(), Consuming->CHAR_NAME(DEFINITE));

      if(HasEaten())
	Consuming->AddConsumeEndMessage(GetActor());

      Consuming->GenerateLeftOvers(GetActor());
    }
  else if(*Consuming)
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop %s %s.", Description.c_str(), Consuming->CHAR_NAME(DEFINITE));
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops %s %s.", GetActor()->CHAR_NAME(DEFINITE), Description.c_str(), Consuming->CHAR_NAME(DEFINITE));

      if(HasEaten())
	Consuming->AddConsumeEndMessage(GetActor());

      if(GetWasOnGround())
	Consuming->MoveTo(GetActor()->GetStackUnder());
      else
	Consuming->MoveTo(GetActor()->GetStack());
    }
  else
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop %s.", Description.c_str());
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops %s.", GetActor()->CHAR_NAME(DEFINITE), Description.c_str());
    }

  action::Terminate(Finished);
}

item* consume::GetConsuming() const
{
  return *Consuming;
}

void consume::SetConsuming(item* Food)
{
  if(!Food)
    ABORT("Consuming nothing!");

  Food->RemoveFromSlot();
  Food->PlaceToSlot(&Consuming);
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
  if(GetActor()->GetHP() >= GoalHP || GetActor()->GetHP() == GetActor()->GetMaxHP() || !GetActor()->CanHeal())
    Terminate(true);
  else
    {
      GetActor()->EditExperience(DEXTERITY, -1);
      GetActor()->EditExperience(AGILITY, -1);
    }
}

void rest::Terminate(bool Finished)
{
  if(Finished)
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You finish resting.");
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s finishes resting.", GetActor()->CHAR_NAME(DEFINITE));
    }
  else
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop resting.");
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops resting.", GetActor()->CHAR_NAME(DEFINITE));
    }

  action::Terminate(Finished);
}

void dig::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << SquareDug;
  SaveFile << RightBackup;
  SaveFile << LeftBackup;
}

void dig::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> SquareDug;
  LoadActionSlot(SaveFile, RightBackup);
  LoadActionSlot(SaveFile, LeftBackup);
}

void dig::Handle()
{
  if(!GetActor()->GetMainWielded())
    {
      Terminate(false);
      return;
    }

  lsquare* Square = GetActor()->GetNearLSquare(SquareDug);
  ushort Damage = GetActor()->GetAttribute(ARM_STRENGTH) * GetActor()->GetMainWielded()->GetMainMaterial()->GetStrengthValue() / 500;
  Square->GetOLTerrain()->EditHP(-Max<ushort>(Damage, 1));
  GetActor()->EditExperience(ARM_STRENGTH, 10);
  GetActor()->EditAP(-100000 / APBonus(GetActor()->GetAttribute(DEXTERITY)));
  GetActor()->EditNP(-30);

  if(Square->GetOLTerrain()->GetHP() <= 0)
    {
      if(Square->CanBeSeenByPlayer())
	ADD_MESSAGE("%s", Square->GetOLTerrain()->GetDigMessage().c_str());

      character* Actor = GetActor();
      Square->GetOLTerrain()->Break();

      /* If the door was boobytrapped etc. and the character is dead, Action has already been deleted */

      if(!Actor->IsEnabled())
	return;

      if(GetActor()->GetMainWielded())
	GetActor()->GetMainWielded()->MoveTo(GetActor()->GetStack());

      if(GetRightBackup())
	{
	  item* RB = GetRightBackup();
	  RB->RemoveFromSlot();
	  GetActor()->SetRightWielded(RB);
	}

      if(GetLeftBackup())
	{
	  item* LB = GetLeftBackup();
	  LB->RemoveFromSlot();
	  GetActor()->SetLeftWielded(LB);
	}

      Terminate(true);
    }
  else
    game::DrawEverything();
}

void dig::Terminate(bool Finished)
{
  if(!Finished)
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop digging.");
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops digging.", GetActor()->CHAR_NAME(DEFINITE));
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
    {
      Item->RemoveFromSlot();
      Item->PlaceToSlot(&RightBackup);
    }
}

item* dig::GetLeftBackup() const
{
  return *LeftBackup;
}

void dig::SetLeftBackup(item* Item)
{
  if(Item)
    {
      Item->RemoveFromSlot();
      Item->PlaceToSlot(&LeftBackup);
    }
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
  GetActor()->EditAP(GetActor()->GetStateAPGain(100)); // gum solution
  GetActor()->GoOn(this);
}

void consume::DropUsedItems()
{
  if(GetConsuming())
    if(!game::IsInWilderness())
      GetConsuming()->MoveTo(GetActor()->GetStackUnder());
    else
      GetConsuming()->MoveTo(GetActor()->GetStack());
}

void consume::DeleteUsedItems()
{
  if(GetConsuming())
    GetConsuming()->SendToHell();
}

void dig::DropUsedItems()
{
  if(GetRightBackup())
    if(!game::IsInWilderness())
      GetRightBackup()->MoveTo(GetActor()->GetStackUnder());
    else
      GetRightBackup()->MoveTo(GetActor()->GetStack());

  if(GetLeftBackup())
    if(!game::IsInWilderness())
      GetLeftBackup()->MoveTo(GetActor()->GetStackUnder());
    else
      GetLeftBackup()->MoveTo(GetActor()->GetStack());
}

void dig::DeleteUsedItems()
{
  if(GetRightBackup())
    GetRightBackup()->SendToHell();

  if(GetLeftBackup())
    GetLeftBackup()->SendToHell();
}

void consume::VirtualConstructor(bool)
{
  Consuming.Init(this);
  SetHasEaten(false);
}

void dig::VirtualConstructor(bool)
{
  RightBackup.Init(this);
  LeftBackup.Init(this);
}

void read::Handle()
{
  if(!*Literature)
    {
      Terminate(false);
      return;
    }

  if(GetActor()->GetLSquareUnder()->IsDark() && !game::SeeWholeMapCheatIsActive())
    {
      ADD_MESSAGE("It is too dark to read now.");
      Terminate(false);
      return;
    }

  if(game::CompareLightToInt(GetActor()->GetLSquareUnder()->GetLuminance(), 115) < 0)
    GetActor()->EditExperience(PERCEPTION, -1);

  GetActor()->EditExperience(INTELLIGENCE, 5);

  if(!Counter)
    {
      Terminate(true);
      return;
    }

  if(GetActor()->GetAttribute(INTELLIGENCE) >= Counter)
    Counter = 0;
  else
    Counter -= GetActor()->GetAttribute(INTELLIGENCE);
}

void read::Terminate(bool Finished)
{
  if(Finished)
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You finish reading %s.", Literature->CHAR_NAME(DEFINITE));
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s finishes reading %s.", GetActor()->CHAR_NAME(DEFINITE), Literature->CHAR_NAME(DEFINITE));

      character* Actor = GetActor();
      Literature->FinishReading(Actor);

      /* Actor may have died and Action may have been deleted, check needed */

      if(!Actor->IsEnabled())
	return;

      if(GetLiterature())
	Literature->MoveTo(GetActor()->GetStack());
    }
  else if(GetLiterature())
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop reading %s.", Literature->CHAR_NAME(DEFINITE));
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops reading %s.", GetActor()->CHAR_NAME(DEFINITE), Literature->CHAR_NAME(DEFINITE));

      Literature->MoveTo(GetActor()->GetStack());
    }
  else
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop reading.");
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops reading.", GetActor()->CHAR_NAME(DEFINITE));
    }

  action::Terminate(Finished);
}


void read::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << Counter;
  SaveFile << Literature;
}

void read::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Counter;
  LoadActionSlot(SaveFile, Literature);
}

void read::DropUsedItems()
{
  if(GetLiterature())
    if(!game::IsInWilderness())
      GetLiterature()->MoveTo(GetActor()->GetStackUnder());
    else
      GetLiterature()->MoveTo(GetActor()->GetStack());
}

void read::DeleteUsedItems()
{
  if(GetLiterature())
    GetLiterature()->SendToHell();
}

item* read::GetLiterature() const
{
  return *Literature;
}

void read::SetLiterature(item* What)
{
  if(!What)
    ABORT("Reading nothing!");

  What->RemoveFromSlot();
  What->PlaceToSlot(&Literature);
}

void read::VirtualConstructor(bool)
{
  Literature.Init(this);
}

/* These function are really lame */

ulong consume::GetVolume() const
{
  return *Consuming ? Consuming->GetVolume() : 0;
}

ulong consume::GetWeight() const
{
  return *Consuming ? Consuming->GetWeight() : 0;
}

ulong dig::GetVolume() const
{
  ulong Volume = 0;

  if(*RightBackup)
    Volume += RightBackup->GetVolume();

  if(*LeftBackup)
    Volume += LeftBackup->GetVolume();

  return Volume;
}

ulong dig::GetWeight() const
{
  ulong Weight = 0;

  if(*RightBackup)
    Weight += RightBackup->GetWeight();

  if(*LeftBackup)
    Weight += LeftBackup->GetWeight();

  return Weight;
}

ulong read::GetVolume() const
{
  return *Literature ? Literature->GetVolume() : 0;
}

ulong read::GetWeight() const
{
  return *Literature ? Literature->GetWeight() : 0;
}

ulong consume::GetEmitation() const
{
  return *Consuming ? Consuming->GetEmitation() : 0;
}

ulong dig::GetEmitation() const
{
  ulong Emitation = 0;

  if(*RightBackup)
    game::CombineLights(Emitation, RightBackup->GetEmitation());

  if(*LeftBackup)
    game::CombineLights(Emitation, LeftBackup->GetEmitation());

  return Emitation;
}

ulong read::GetEmitation() const
{
  return *Literature ? Literature->GetEmitation() : 0;
}

long consume::GetScore() const
{
  return *Consuming ? Consuming->GetScore() : 0;
}

long dig::GetScore() const
{
  long Score = 0;

  if(*RightBackup)
    Score += RightBackup->GetScore();

  if(*LeftBackup)
    Score += LeftBackup->GetScore();

  return Score;
}

long read::GetScore() const
{
  return *Literature ? Literature->GetScore() : 0;
}
