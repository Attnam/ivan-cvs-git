/* Compiled through actset.cpp */

const char* faint::GetDeathExplanation() const { return " while helpless"; }
const char* faint::GetDescription() const { return "fainted"; }
const char* consume::GetDescription() const { return Description.CStr(); }
void consume::SetDescription(const festring& What) { Description = What; }
const char* rest::GetDescription() const { return "resting"; }
const char* dig::GetDescription() const { return "digging"; }
const char* go::GetDescription() const { return "going"; }
const char* study::GetDescription() const { return "reading"; }

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
  SaveFile << WasOnGround << ConsumingID << Description;
}

void consume::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> WasOnGround >> ConsumingID >> Description;
}

void consume::Handle()
{
  item* Consuming = game::SearchItem(ConsumingID);

  if(!Consuming || !Consuming->Exists() || !Actor->IsOver(Consuming))
    {
      Terminate(false);
      return;
    }

  character* Actor = GetActor();

  if(!InDNDMode() && Actor->GetHungerState() >= BLOATED)
    if(Actor->IsPlayer())
      {
	ADD_MESSAGE("You have a really hard time getting all this down your throat.");

	if(game::BoolQuestion(CONST_S("Continue ") + GetDescription() + "? [y/N]"))
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

  if(Consuming->Consume(Actor, 500) && Actor->GetAction() == this && Actor->IsEnabled())
    Terminate(true);
  else if(Actor->GetHungerState() == OVER_FED)
    {
      Actor->DeActivateVoluntaryAction(CONST_S("You are about to choke on this stuff."));
      Actor->Vomit(2 + RAND() % 3);
    }
}

void consume::Terminate(bool Finished)
{
  item* Consuming = game::SearchItem(ConsumingID);

  if(Finished)
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You finish %s %s.", Description.CStr(), Consuming->CHAR_NAME(DEFINITE));
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s finishes %s %s.", GetActor()->CHAR_NAME(DEFINITE), Description.CStr(), Consuming->CHAR_NAME(DEFINITE));

      if(Eaten)
	Consuming->AddConsumeEndMessage(GetActor());

      Consuming->GenerateLeftOvers(GetActor());
    }
  else if(Consuming && Consuming->Exists())
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop %s %s.", Description.CStr(), Consuming->CHAR_NAME(DEFINITE));
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops %s %s.", GetActor()->CHAR_NAME(DEFINITE), Description.CStr(), Consuming->CHAR_NAME(DEFINITE));

      if(Eaten)
	Consuming->AddConsumeEndMessage(GetActor());
    }
  else
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop %s.", Description.CStr());
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops %s.", GetActor()->CHAR_NAME(DEFINITE), Description.CStr());
    }

  action::Terminate(Finished);
}

void rest::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << GoalHP << TurnToStop;
}

void rest::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> GoalHP >> TurnToStop;
}

void rest::Handle()
{
  if((GoalHP && (GetActor()->GetHP() >= GoalHP || GetActor()->GetHP() == GetActor()->GetMaxHP() || !GetActor()->CanHeal()))
  || (TurnToStop && game::GetTicks() / 10 >= TurnToStop))
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
  SaveFile << SquareDug << MoveDigger << RightBackupID << LeftBackupID;
}

void dig::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> SquareDug >> MoveDigger >> RightBackupID >> LeftBackupID;
}

void dig::Handle()
{
  character* Actor = GetActor();
  item* Digger = Actor->GetMainWielded();

  if(!Digger)
    {
      Terminate(false);
      return;
    }

  lsquare* Square = Actor->GetNearLSquare(SquareDug);
  olterrain* Terrain = Square->GetOLTerrain();

  if(!Terrain || !Terrain->CanBeDestroyed() || !Terrain->GetMainMaterial()->CanBeDug(Digger->GetMainMaterial()))
    {
      Terminate(false);
      return;
    }

  ushort Damage = Actor->GetAttribute(ARM_STRENGTH) * Digger->GetMainMaterial()->GetStrengthValue() / 500;
  Terrain->EditHP(-Max<ushort>(Damage, 1));
  Actor->EditExperience(ARM_STRENGTH, 10);
  Actor->EditAP(-100000 / APBonus(Actor->GetAttribute(DEXTERITY)));
  Actor->EditNP(-30);

  if(Terrain->GetHP() <= 0)
    {
      if(Square->CanBeSeenByPlayer())
	ADD_MESSAGE("%s", Terrain->GetDigMessage().CStr());

      Terrain->Break();

      /* If the door was boobytrapped etc. and the character is dead, Action has already been deleted */

      if(!Actor->IsEnabled())
	return;

      if(MoveDigger && Actor->GetMainWielded())
	Actor->GetMainWielded()->MoveTo(Actor->GetStack());

      item* RightBackup = game::SearchItem(RightBackupID);

      if(RightBackup && RightBackup->Exists() && Actor->IsOver(RightBackup))
	{
	  RightBackup->RemoveFromSlot();
	  Actor->SetRightWielded(RightBackup);
	}

      item* LeftBackup = game::SearchItem(LeftBackupID);

      if(LeftBackup && LeftBackup->Exists() && Actor->IsOver(LeftBackup))
	{
	  LeftBackup->RemoveFromSlot();
	  Actor->SetLeftWielded(LeftBackup);
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

  action::Terminate(Finished);
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

void study::Handle()
{
  item* Literature = game::SearchItem(LiteratureID);

  if(!Literature || !Literature->Exists() || !Actor->IsOver(Literature))
    {
      Terminate(false);
      return;
    }

  if(GetActor()->GetLSquareUnder()->IsDark() && !game::GetSeeWholeMapCheatMode())
    {
      ADD_MESSAGE("It is too dark to read now.");
      Terminate(false);
      return;
    }

  if(game::CompareLightToInt(GetActor()->GetLSquareUnder()->GetLuminance(), 115) < 0)
    GetActor()->EditExperience(PERCEPTION, -1);

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

void study::Terminate(bool Finished)
{
  item* Literature = game::SearchItem(LiteratureID);

  if(Finished)
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You finish reading %s.", Literature->CHAR_NAME(DEFINITE));
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s finishes reading %s.", GetActor()->CHAR_NAME(DEFINITE), Literature->CHAR_NAME(DEFINITE));

      character* Actor = GetActor();
      Literature->FinishReading(Actor);

      if(!Actor->IsEnabled())
	return;
    }
  else if(Literature && Literature->Exists())
    {
      if(GetActor()->IsPlayer())
	ADD_MESSAGE("You stop reading %s.", Literature->CHAR_NAME(DEFINITE));
      else if(GetActor()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s stops reading %s.", GetActor()->CHAR_NAME(DEFINITE), Literature->CHAR_NAME(DEFINITE));
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


void study::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << Counter << LiteratureID;
}

void study::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Counter >> LiteratureID;
}

bool go::TryDisplace()
{
  Terminate(false);
  return true;
}
