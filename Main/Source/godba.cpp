#include "godba.h"
#include "femath.h"
#include "message.h"
#include "charde.h"
#include "team.h"
#include "level.h"
#include "lsquare.h"
#include "itemba.h"
#include "strover.h"
#include "game.h"
#include "save.h"

void god::Pray()
{
  if(!Timer)
    if(Relation >= -(RAND() % 500))
      {
	ADD_MESSAGE("You feel %s is pleased.", GOD_NAME);
	PrayGoodEffect();
	AdjustTimer(5000);
	AdjustRelation(50);
	game::ApplyDivineAlignmentBonuses(this, true);

	if(Relation > 500 && !(RAND() % 100))
	  {
	    character* Angel = CreateAngel();

	    if(Angel)
	      {
		Angel->SetTeam(game::GetPlayer()->GetTeam());
		ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHARNAME(DEFINITE));
	      }
	  }
      }
    else
      {
	ADD_MESSAGE("You feel %s is displeased today.", GOD_NAME);
	PrayBadEffect();
	AdjustTimer(10000);
	game::ApplyDivineAlignmentBonuses(this, false);
      }
  else
    if(Relation > RAND() % 500)
      {
	ADD_MESSAGE("You feel %s is displeased, but helps you anyway.", GOD_NAME);
	PrayGoodEffect();
	AdjustTimer(25000);
	AdjustRelation(-50);
	game::ApplyDivineAlignmentBonuses(this, false);
      }
    else
      {
	ADD_MESSAGE("You feel %s is angry.", GOD_NAME);
	PrayBadEffect();
	AdjustTimer(50000);
	AdjustRelation(-100);
	game::ApplyDivineAlignmentBonuses(this, false);

	if(Relation < -500 && !(RAND() % 50))
	  {
	    character* Angel = CreateAngel();

	    if(Angel)
	      {
		Angel->SetTeam(game::GetTeam(5));
		ADD_MESSAGE("%s seems to be hostile.", Angel->CHARNAME(DEFINITE));
	      }
	  }
      }
}

std::string god::CompleteDescription() const
{
  std::string Desc = game::CAlignment(Alignment());

  Desc.resize(4, ' ');
  Desc += Name();
  Desc.resize(17, ' ');

  if(game::GetWizardMode())
    {
      Desc += Timer;
      Desc += " - ";
      Desc += Relation;
    }

  Desc.resize(32, ' ');

  return Desc + "the " + Description();
}

void god::AdjustRelation(god* Competitor, bool Good, short Multiplier)
{
  short Adjustment = 2 * Multiplier - abs((schar)(Alignment()) - Competitor->Alignment()) * Multiplier;

  if(!Good && Adjustment > 0)
    Adjustment = -Adjustment;

  AdjustRelation(Adjustment);
}

void god::AdjustRelation(short Amount)
{
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
  game::GetPlayer()->ReceiveDamage(0, 1 + RAND() % 2, ACID, HEAD);//SetHP(game::GetPlayer()->GetHP() - 1 - RAND() % 2);
  //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - 1 - RAND() % 2);
  game::GetPlayer()->CheckDeath("chocked to death by own vomit");

  if(!(RAND() % 50))
    {
      character* Angel = CreateAngel();

      if(Angel)
	{
	  Angel->SetTeam(game::GetTeam(5));
	  ADD_MESSAGE("%s seems to be hostile.", Angel->CHARNAME(DEFINITE));
	}
    }
}

character* god::CreateAngel()
{
  vector2d TryToCreate;

  for(ushort c = 0; c < 100; ++c)
    {
      TryToCreate = game::GetPlayer()->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

      angel* Angel = new angel;

      if(game::IsValidPos(TryToCreate) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetIsWalkable(Angel) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetCharacter() == 0)
	{
	  Angel->SetDivineMaster(Type());

	  game::GetCurrentLevel()->GetLSquare(TryToCreate)->AddCharacter(Angel);
	  ADD_MESSAGE("Suddenly %s appears!", Angel->CHARNAME(INDEFINITE));
	  return Angel;
	}
      else
	delete Angel;
    }

  return 0;
}

std::string god::GetPriestMessage() const
{
  return "\"Not currently implemented.\"";
}

void god::PrintRelation() const
{
  std::string VerbalRelation;

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

  ADD_MESSAGE("%s %s", GOD_NAME, VerbalRelation.c_str());
}

bool god::ReceiveOffer(item* Sacrifice)
{
  short OfferValue = Sacrifice->CalculateOfferValue(BasicAlignment());

  if(abs(OfferValue) > 5)
    {
      if(!Sacrifice->Destroyable())
	{
	  ADD_MESSAGE("%s is too important for you to be sacrificed.", Sacrifice->CHARNAME(DEFINITE));
	  return false;
	}

      AdjustRelation(OfferValue);
      game::ApplyDivineAlignmentBonuses(this, OfferValue > 0 ? true : false);

      if(OfferValue > 0)
	ADD_MESSAGE("%s thanks you for your gift.", GOD_NAME);
      else
	ADD_MESSAGE("%s seems not to appreciate your gift at all.", GOD_NAME);

      PrintRelation();

      if(OfferValue > 0 && Relation > 500 && !(RAND() % 100))
	{
	  character* Angel = CreateAngel();

	  if(Angel)
	    {
	      Angel->SetTeam(game::GetPlayer()->GetTeam());
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHARNAME(DEFINITE));
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
  SaveFile << Type();
  SaveFile << Relation << Timer << Known;
}

void god::Load(inputfile& SaveFile)
{
  SaveFile >> Relation >> Timer >> Known;
}

