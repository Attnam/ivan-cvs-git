/* Compiled through godset.cpp */

godprototype::godprototype(god* (*Cloner)(bool), const char* ClassId) : Cloner(Cloner), ClassId(ClassId) { Index = protocontainer<god>::Add(this); }

god::god() : Relation(0), Timer(0), Known(false) { }
uchar god::GetBasicAlignment() const { return NEUTRAL; }

void god::Pray()
{
  if(!Timer)
    if(Relation >= -RAND_N(500))
      {
	ADD_MESSAGE("You feel %s is pleased.", GetName());
	PrayGoodEffect();
	AdjustTimer(5000);
	AdjustRelation(50);
	game::ApplyDivineAlignmentBonuses(this, true);
	PLAYER->EditExperience(WISDOM, 250);

	if(Relation > 500 && !(RAND() % 100))
	  {
	    character* Angel = CreateAngel();

	    if(Angel)
	      {
		Angel->SetTeam(PLAYER->GetTeam());
		ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
	      }
	  }
      }
    else
      {
	ADD_MESSAGE("You feel %s is displeased today.", GetName());
	PrayBadEffect();
	AdjustTimer(10000);
	game::ApplyDivineAlignmentBonuses(this, false);
	PLAYER->EditExperience(WISDOM, -250);
      }
  else
    if(Relation > RAND_N(500))
      {
	ADD_MESSAGE("You feel %s is displeased, but tries to help you anyway.", GetName());
	PrayGoodEffect();
	AdjustTimer(25000);
	AdjustRelation(-50);
	game::ApplyDivineAlignmentBonuses(this, false);
	PLAYER->EditExperience(WISDOM, -250);
      }
    else
      {
	ADD_MESSAGE("You feel %s is angry.", GetName());
	PrayBadEffect();
	AdjustTimer(50000);
	AdjustRelation(-100);
	game::ApplyDivineAlignmentBonuses(this, false);
	PLAYER->EditExperience(WISDOM, -500);

	if(Relation < -500 && !(RAND() % 50))
	  {
	    character* Angel = CreateAngel();

	    if(Angel)
	      {
		Angel->SetTeam(game::GetTeam(4));
		ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
	      }
	  }
      }
}

festring god::GetCompleteDescription() const
{
  festring Desc(game::GetAlignment(GetAlignment()));
  Desc.Resize(4, ' ');
  Desc << GetName();
  Desc.Resize(20, ' ');

  if(game::WizardModeIsActive())
    Desc << Timer << " - " << Relation;
  else
    Desc << "the " << GetDescription();

  return Desc;
}

void god::AdjustRelation(god* Competitor, bool Good, short Multiplier)
{
  short Adjustment = (Multiplier << 1) - abs((schar)(GetAlignment()) - Competitor->GetAlignment()) * Multiplier;

  if(!Good && Adjustment > 0)
    Adjustment = -Adjustment;

  AdjustRelation(Adjustment);
}

void god::AdjustRelation(short Amount)
{
  if(Amount < 0)
    Amount = Amount * 100 / (100 + PLAYER->GetAttribute(WISDOM));
  else
    Amount = Amount * (100 + PLAYER->GetAttribute(WISDOM)) / 100;

  Relation += Amount;

  if(Relation < -1000)
    Relation = -1000;

  if(Relation > 1000)
    Relation = 1000;
}

void god::AdjustTimer(long Amount)
{
  Timer += Amount;

  if(Timer < 0)
    Timer = 0;

  if(Timer > 1000000000)
    Timer = 1000000000;
}

void god::PlayerVomitedOnAltar()
{
  ADD_MESSAGE("The vomit drops on the altar, but then suddenly gravity changes its direction. The vomit lands on your face.");
  AdjustRelation(-200);
  PLAYER->ReceiveDamage(0, 1 + (RAND() & 1), ACID, HEAD);
  PLAYER->CheckDeath(CONST_S("choked to death by own vomit"), 0);

  if(!(RAND() % 50))
    {
      character* Angel = CreateAngel();

      if(Angel)
	{
	  Angel->SetTeam(game::GetTeam(4));
	  ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
	}
    }
}

character* god::CreateAngel()
{
  vector2d TryToCreate;

  for(ushort c = 0; c < 100; ++c)
    {
      TryToCreate = PLAYER->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);
      angel* Angel = new angel(GetType());

      if(game::GetCurrentArea()->IsValidPos(TryToCreate) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->IsWalkable(Angel) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetCharacter() == 0)
	{
	  game::GetCurrentLevel()->GetLSquare(TryToCreate)->AddCharacter(Angel);
	  ADD_MESSAGE("Suddenly %s appears!", Angel->CHAR_NAME(INDEFINITE));
	  return Angel;
	}
      else
	Angel->SendToHell();
    }

  return 0;
}

void god::PrintRelation() const
{
  const char* VerbalRelation;

  if(GetRelation() == 1000)
    VerbalRelation = "greets you as a Champion of the Cause!";
  else if(GetRelation() > 750)
    VerbalRelation = "is extremely pleased.";
  else if(GetRelation() > 250)
    VerbalRelation = "is very pleased.";
  else if(GetRelation() > 50)
    VerbalRelation = "is pleased.";
  else if(GetRelation() > -50)
    VerbalRelation = "is content.";
  else if(GetRelation() > -250)
    VerbalRelation = "is angry.";
  else if(GetRelation() > -750)
    VerbalRelation = "is very angry.";
  else if(GetRelation() > -1000)
    VerbalRelation = "is extremely angry.";
  else VerbalRelation = "hates you more than any other mortal.";

  ADD_MESSAGE("%s %s", GetName(), VerbalRelation);
}

bool god::ReceiveOffer(item* Sacrifice)
{
  short OfferValue = Sacrifice->GetOfferValue(GetType());

  if(OfferValue)
    {
      if(!Sacrifice->IsDestroyable())
	{
	  ADD_MESSAGE("%s is too important for you to be sacrificed.", Sacrifice->CHAR_NAME(DEFINITE));
	  return false;
	}

      AdjustRelation(OfferValue);
      game::ApplyDivineAlignmentBonuses(this, OfferValue > 0);

      if(OfferValue > 0)
	PLAYER->EditExperience(WISDOM, 50);
      else
	PLAYER->EditExperience(WISDOM, -50);

      if(OfferValue > 0)
	{
	  if(Sacrifice->GetAttachedGod() == GetType())
	    ADD_MESSAGE("%s appreciates your generous offer truly.", GetName());
	  else
	    ADD_MESSAGE("%s thanks you for your gift.", GetName());
	}
      else
	ADD_MESSAGE("%s seems not to appreciate your gift at all.", GetName());

      PrintRelation();

      if(OfferValue > 0 && Relation > 500 && !(RAND() % 100))
	{
	  character* Angel = CreateAngel();

	  if(Angel)
	    {
	      Angel->SetTeam(PLAYER->GetTeam());
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
	    }
	}

      return true;
    }
  else
    {
      ADD_MESSAGE("Nothing happens.");
      return false;
    }
}

void god::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  SaveFile << Relation << Timer << Known;
}

void god::Load(inputfile& SaveFile)
{
  SaveFile >> Relation >> Timer >> Known;
}

god* godprototype::CloneAndLoad(inputfile& SaveFile) const
{
  god* God = Cloner(true);
  God->Load(SaveFile);
  return God;
}

void god::ApplyDivineTick()
{
  if(Timer)
    --Timer;
}
