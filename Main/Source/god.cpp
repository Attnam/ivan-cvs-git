/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

/* Compiled through godset.cpp */

godprototype::godprototype(god* (*Cloner)(bool), const char* ClassID) : Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<god>::Add(this); }

god::god() : Relation(0), Timer(0), Known(false) { }
int god::GetBasicAlignment() const { return NEUTRAL; }

void god::Pray()
{
  if(!Timer)
    if(Relation >= -RAND_N(500))
      {
	ADD_MESSAGE("You feel %s is pleased.", GetName());
	PrayGoodEffect();
	AdjustTimer(5000);
	AdjustRelation(50);
	game::ApplyDivineAlignmentBonuses(this, 10, true);
	PLAYER->EditExperience(WISDOM, 200, 1 << 10);

	if(Relation > 250 && !(RAND() % 20))
	  {
	    character* Angel = CreateAngel(PLAYER->GetTeam(), 10000);

	    if(Angel)
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
	  }
	else if(Relation > 100 && !(RAND() % 20))
	  {
	    long Category = RAND() & ANY_CATEGORY;

	    if(!Category)
	      Category = ANY_CATEGORY;

	    item* Gift = protosystem::BalancedCreateItem(Relation / 2, Relation * 2, Category, 0, 0, GetType());

	    if(Gift)
	      {
		Gift->CalculateEnchantment();
		PLAYER->GetStack()->AddItem(Gift);

		if(Gift->IsBroken())
		  Gift->Fix();

		if(Relation == 1000)
		  Gift->EditEnchantment(3);
		else if(Relation >= 500)
		  Gift->EditEnchantment(2);
		else
		  Gift->EditEnchantment(1);

		ADD_MESSAGE("You notice %s in your inventory which you don't recall picking up anywhere.", Gift->CHAR_NAME(INDEFINITE));
	      }
	  }
      }
    else
      {
	ADD_MESSAGE("You feel %s is displeased today.", GetName());
	PrayBadEffect();
	AdjustTimer(10000);
	AdjustRelation(-50);
	game::ApplyDivineAlignmentBonuses(this, 10, false);
	PLAYER->EditExperience(WISDOM, -50, 1 << 10);
      }
  else
    if(Relation > RAND_N(500) && Timer < RAND_N(500000))
      {
	ADD_MESSAGE("You feel %s is displeased, but tries to help you anyway.", GetName());
	PrayGoodEffect();
	AdjustTimer(25000);
	AdjustRelation(-75);
	game::ApplyDivineAlignmentBonuses(this, 15, false);
	PLAYER->EditExperience(WISDOM, -50, 1 << 10);
      }
    else
      {
	ADD_MESSAGE("You feel %s is angry.", GetName());
	PrayBadEffect();
	AdjustTimer(50000);
	AdjustRelation(-100);
	game::ApplyDivineAlignmentBonuses(this, 20, false);
	PLAYER->EditExperience(WISDOM, -100, 1 << 11);

	if(Relation < -250 && !(RAND() % 10))
	  {
	    character* Angel = CreateAngel(game::GetTeam(4), 10000);

	    if(Angel)
	      ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
	  }
      }
}

festring god::GetCompleteDescription() const
{
  festring Desc(game::GetAlignment(GetAlignment()));
  Desc.Resize(4);
  Desc << GetName();
  Desc.Resize(20);

  if(game::WizardModeIsActive())
    Desc << Timer << " - " << Relation;
  else
    Desc << "the " << GetDescription();

  return Desc;
}

void god::AdjustRelation(god* Competitor, int Multiplier, bool Good)
{
  int Adjustment = (Multiplier << 1) - abs(GetAlignment() - Competitor->GetAlignment()) * Multiplier;

  if(!Good && Adjustment > 0)
    Adjustment = -Adjustment;

  AdjustRelation(Adjustment);
}

void god::AdjustRelation(int Amount)
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

bool god::PlayerVomitedOnAltar(liquid* Liquid)
{
  if(PLAYER->GetVirtualHead())
    {
      ADD_MESSAGE("The vomit drops on the altar, but then suddenly gravity changes its direction. The vomit lands on your face.");
      PLAYER->GetVirtualHead()->SpillFluid(0, Liquid);
      PLAYER->ReceiveDamage(0, 1 + (RAND() & 1), ACID, HEAD);
    }
  else
    {
      ADD_MESSAGE("The vomit drops on the altar, but then suddenly gravity changes its direction. The vomit lands all over you.");
      PLAYER->SpillFluid(0, Liquid);
      PLAYER->ReceiveDamage(0, 1 + (RAND() & 1), ACID);
    }

  AdjustRelation(-200);
  PLAYER->CheckDeath(CONST_S("killed by a flying lump of vomit"), 0);

  if(!(RAND() % 10))
    {
      character* Angel = CreateAngel(game::GetTeam(4), 10000);

      if(Angel)
	ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
    }

  return true;
}

character* god::CreateAngel(team* Team, int LifeBase)
{
  vector2d TryToCreate;

  for(int c = 0; c < 100; ++c)
    {
      TryToCreate = PLAYER->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

      if(game::GetCurrentArea()->IsValidPos(TryToCreate))
	{
	  angel* Angel;

	  if(LifeBase && (!(RAND() % 5) || abs(Relation) == 1000))
	    Angel = new archangel(GetType());
	  else
	    Angel = new angel(GetType());

	  if(LifeBase)
	    Angel->SetLifeExpectancy(LifeBase, 0);

	  lsquare* Square = game::GetCurrentLevel()->GetLSquare(TryToCreate);

	  if(Angel->CanMoveOn(Square) && Angel->IsFreeForMe(Square))
	    {
	      Angel->SetTeam(Team);
	      Angel->SetGenerationDanger(ANGEL_GENERATION_DANGER);
	      Angel->PutTo(TryToCreate);
	      ADD_MESSAGE("Suddenly %s appears!", Angel->CHAR_NAME(INDEFINITE));
	      return Angel;
	    }
	  else
	    Angel->SendToHell();
	}
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
  int OfferValue = Sacrifice->GetOfferValue(GetType());

  if(OfferValue)
    {
      if(!Sacrifice->IsDestroyable(PLAYER))
	{
	  ADD_MESSAGE("%s is too important for you to be sacrificed.", Sacrifice->CHAR_NAME(DEFINITE));
	  return false;
	}

      AdjustRelation(OfferValue);

      if(OfferValue > 0)
	game::ApplyDivineAlignmentBonuses(this, OfferValue / 5, true);
      else
	game::ApplyDivineAlignmentBonuses(this, -OfferValue / 5, false);

      if(OfferValue > 0)
	PLAYER->EditExperience(WISDOM, 150, 1 << 7);
      else
	PLAYER->EditExperience(WISDOM, -100, 1 << 7);

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
      int RandModifier = Sacrifice->GetAttachedGod() == GetType() ? 50 : 100;

      if(OfferValue > 0 && Relation > 250 && !(RAND() % RandModifier))
	{
	  character* Angel = CreateAngel(PLAYER->GetTeam());

	  if(Angel)
	    ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
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
  SaveFile << (ushort)GetType();
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
