#define __FILE_OF_STATIC_GOD_PROTOTYPE_DECLARATIONS__

#include "proto.h"
#include "godba.h"

std::vector<god::prototype*> protocontainer<god>::ProtoData;
valuemap protocontainer<god>::CodeNameMap;

#include "godde.h"

#undef __FILE_OF_STATIC_GOD_PROTOTYPE_DECLARATIONS__

#include "message.h"
#include "charde.h"
#include "level.h"
#include "femath.h"
#include "stack.h"
#include "itemde.h"
#include "lsquare.h"
#include "team.h"
#include "script.h"
#include "lterrade.h"
#include "slot.h"
#include "strover.h"

void consummo::PrayGoodEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement! You teleport away!");
  game::GetPlayer()->Move(game::GetCurrentLevel()->RandomSquare(game::GetPlayer(), true), true);
}

void consummo::PrayBadEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement! Some parts of you teleport away!");
  //game::GetPlayer()->ReceivePhysicalDamage(5, HEAD, false, RAND() % 8);
  //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - RAND() % game::GetPlayer()->GetMaxHP());
  game::GetPlayer()->CheckDeath("shattered to pieces by the wrath of " + Name());
}

void valpurus::PrayGoodEffect()
{
  ADD_MESSAGE("You hear booming voice: \"DEFEAT ERADO WITH THIS, MY PALADIN!\" A sword glittering with holy might appears from nothing.");
  game::GetPlayer()->GetGiftStack()->AddItem(new curvedtwohandedsword(new valpurium));
}

void valpurus::PrayBadEffect()
{
  ADD_MESSAGE("Valpurus smites you with a small hammer.");
  game::GetPlayer()->ReceiveDamage(0, 10, PHYSICALDAMAGE, HEAD, RAND() % 8);
  //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - 5);
  game::GetPlayer()->CheckDeath("faced the hammer of Justice from the hand of " + Name());
}

void venius::PrayGoodEffect()
{
  ADD_MESSAGE("A booming voice echoes: \"Xunil! Xunil! Save us!\" A huge firestorm engulfs everything around you.");
  game::GetCurrentLevel()->Explosion(game::GetPlayer(), "killed accidentally by " + Name(), game::GetPlayer()->GetPos(), 40, false);
}

void venius::PrayBadEffect()
{
  ADD_MESSAGE("%s casts a beam of horrible, yet righteous, fire on you.", GOD_NAME);
  game::GetPlayer()->ReceiveDamage(0, 20 + RAND() % 20, FIRE, ALL);
  //game::GetPlayer()->ReceiveFireDamage(game::GetPlayer(), "killed accidentally by " + Name(), 20);
  game::GetPlayer()->CheckDeath("burned to death by the wrath of " + Name());
}

void dulcis::PrayGoodEffect()
{
  ADD_MESSAGE("A beautiful melody echoes around you.");

  DO_FOR_SQUARES_AROUND(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
  {
    character* Char = game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetCharacter();

    if(Char)
      if(Char->Charmable())
	if(Char->CurrentDanger() * 2 < game::GetPlayer()->MaxDanger())
	  {
	    if(Char->GetTeam() == game::GetPlayer()->GetTeam())
	      ADD_MESSAGE("%s seems to be very happy.", Char->CHARNAME(DEFINITE));
	    else if(Char->GetTeam()->GetRelation(game::GetPlayer()->GetTeam()) == HOSTILE)
	      ADD_MESSAGE("%s stops fighting.", Char->CHARNAME(DEFINITE));
	    else
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Char->CHARNAME(DEFINITE));

	    Char->ChangeTeam(game::GetPlayer()->GetTeam());
	  }
	else
	  ADD_MESSAGE("%s resists its charming call.", Char->CHARNAME(DEFINITE));
      else
	ADD_MESSAGE("%s seems not affected.", Char->CHARNAME(DEFINITE));
  });
}

void dulcis::PrayBadEffect()
{
  ADD_MESSAGE("%s plays a horrible tune that rots your brain.", GOD_NAME);
  game::GetPlayer()->ReceiveDamage(0, 1 + RAND() % 9, SOUND, HEAD);
  //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - RAND() % 9 - 1);
  game::GetPlayer()->CheckDeath("became insane by listening " + Name() + " too much");
}

void seges::PrayGoodEffect()
{
  ADD_MESSAGE("Your stomach feels full again.");

  if(game::GetPlayer()->GetNP() < 10000)
    game::GetPlayer()->SetNP(10000);
}

void seges::PrayBadEffect()
{
  ADD_MESSAGE("You feel Seges altering the contents of your stomach in an eerie way.");
  game::GetPlayer()->EditNP(-1000);

  if(game::GetPlayer()->GetNP() < 1)
    game::GetPlayer()->CheckStarvationDeath("starved by " + Name());
}

void atavus::PrayGoodEffect()
{
  item* Reward = new platemail(false);
  Reward->InitMaterials(new mithril);
  ADD_MESSAGE("%s materializes before you.", Reward->CHARNAME(INDEFINITE));
  game::GetPlayer()->GetGiftStack()->AddItem(Reward);
}

void atavus::PrayBadEffect()
{
  ADD_MESSAGE("You have not been good the whole year.");

  if(game::GetPlayer()->GetStack()->GetItems())
    {
      ushort ToBeDeleted = RAND() % game::GetPlayer()->GetStack()->GetItems();
      item* Disappearing = game::GetPlayer()->GetStack()->GetItem(ToBeDeleted);

      if(Disappearing->IsDestroyable())
	{
	  ADD_MESSAGE("Your %s disappears.", Disappearing->CHARNAME(UNARTICLED));
	  Disappearing->RemoveFromSlot();
	  //if(game::GetPlayer()->GetWielded() == Disappearing) game::GetPlayer()->SetWielded(0);
	  //if(game::GetPlayer()->GetBodyArmor() == Disappearing) game::GetPlayer()->SetBodyArmor(0);
	  Disappearing->SetExists(false);
	}
      else
	{
	  ADD_MESSAGE("%s tries to remove your %s, but fails. You feel you are not so gifted anymore.", GOD_NAME, Disappearing->CHARNAME(UNARTICLED));
	  game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
	  game::GetPlayer()->SetStrength(game::GetPlayer()->GetStrength() - 1);
	  game::GetPlayer()->SetEndurance(game::GetPlayer()->GetEndurance() - 1);
	}
    }
  else
    {
      ADD_MESSAGE("You feel you are not so gifted anymore.");
      game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
      game::GetPlayer()->SetStrength(game::GetPlayer()->GetStrength() - 1);
      game::GetPlayer()->SetEndurance(game::GetPlayer()->GetEndurance() - 1);
    }
}

void silva::PrayGoodEffect()
{
  if(!*game::GetCurrentLevel()->GetLevelScript()->GetOnGround())
    {
      ADD_MESSAGE("Suddenly a horrible earthquake shakes the level.");
      ushort c, Tunnels = 2 + RAND() % 3;

      for(c = 0; c < Tunnels; ++c)
	game::GetCurrentLevel()->AttachPos(game::GetCurrentLevel()->RandomSquare(0, false));

      uchar ToEmpty = 10 + RAND() % 11;

      for(c = 0; c < ToEmpty; ++c)
	for(ushort i = 0; i < 50; ++i)
	  {
	    vector2d Pos = game::GetCurrentLevel()->RandomSquare(0, false);
	    bool Correct = false;

	    DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	    {
	      if(game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetOTerrain()->GetIsWalkable())
		{
		  Correct = true;
		  break;
		}
	    });

	    if(Correct)
	      {
		game::GetCurrentLevel()->GetLSquare(Pos)->ChangeOLTerrainAndUpdateLights(new empty);

		for(ushort p = 0; p < 4; ++p)
		  game::GetCurrentLevel()->GetLSquare(Pos)->GetSideStack(p)->Clean();

		break;
	      }
	  }

      uchar ToGround = 20 + RAND() % 21;

      for(c = 0; c < ToGround; ++c)
	for(ushort i = 0; i < 50; ++i)
	  {
	    vector2d Pos = game::GetCurrentLevel()->RandomSquare(0, true, RAND() % 2 ? true : false);

	    character* Char = game::GetCurrentLevel()->GetLSquare(Pos)->GetCharacter();

	    if(!game::GetCurrentLevel()->GetLSquare(Pos)->GetOLTerrain()->IsSafeToDestroy() || (Char && (Char->GetIsPlayer() || Char->GetTeam()->GetRelation(game::GetPlayer()->GetTeam()) != HOSTILE)))
	      continue;

	    uchar Walkables = 0;

	    DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	    {
	      if(game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetOTerrain()->GetIsWalkable())
		++Walkables;
	    });

	    if(Walkables > 6)
	      {
		game::GetCurrentLevel()->GetLSquare(Pos)->ChangeOLTerrainAndUpdateLights(new earth);

		if(Char)
		  {
		    if(Char->GetSquareUnder()->CanBeSeen())
		      ADD_MESSAGE("%s is hit by a brick of earth falling from the roof!", Char->CHARNAME(DEFINITE));

		    Char->ReceiveDamage(0, 20 + RAND() % 21, PHYSICALDAMAGE, HEAD|TORSO, 8, true);
		    //Char->SetHP(Char->GetHP() - 20 - RAND() % 21);
		    Char->CheckDeath("killed by an earthquake");
		  }

		game::GetCurrentLevel()->GetLSquare(Pos)->KickAnyoneStandingHereAway();

		ushort p;

		for(p = 0; p < 4; ++p)
		  game::GetCurrentLevel()->GetLSquare(Pos)->GetSideStack(p)->Clean();

		/*for(p = 0; p < game::GetCurrentLevel()->GetLSquare(Pos)->GetStack()->GetItems();)
		  if(!game::GetCurrentLevel()->GetLSquare(Pos)->GetStack()->GetItem(p)->ImpactDamage(0xFFFF, game::GetCurrentLevel()->GetLSquare(Pos)->CanBeSeen(), game::GetCurrentLevel()->GetLSquare(Pos)->GetStack()))
		    ++p;*/

		game::GetCurrentLevel()->GetLSquare(Pos)->GetStack()->ReceiveDamage(0, 10 + RAND() % 41, PHYSICALDAMAGE);

		break;
	      }
	  }

      // Impact damage for items in the level

      for(ushort x = 0; x < game::GetCurrentLevel()->GetXSize(); ++x)
	for(ushort y = 0; y < game::GetCurrentLevel()->GetYSize(); ++y)
	  {
	    game::GetCurrentLevel()->GetLSquare(x,y)->GetStack()->ReceiveDamage(0, RAND() % 5, PHYSICALDAMAGE);

	    for(ushort c = 0; c < 4; ++c)
	      game::GetCurrentLevel()->GetLSquare(x,y)->GetSideStack(c)->ReceiveDamage(0, RAND() % 5, PHYSICALDAMAGE);
	  }
    }
  else
    {
      ushort Created = 0;

      DO_FOR_SQUARES_AROUND(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
      {
	wolf* Wolf = new wolf;

	if(game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetIsWalkable(Wolf) && !game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetCharacter())
	  {
	    Wolf->SetTeam(game::GetPlayer()->GetTeam());
	    game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->AddCharacter(Wolf);
	    ++Created;
	  }
	else
	  delete Wolf;
      });

      if(!Created)
	ADD_MESSAGE("You hear a sad howling of a wolf inprisoned in the earth.");

      if(Created == 1)
	ADD_MESSAGE("Suddenly a tame wolf materializes beside you.");

      if(Created > 1)
	ADD_MESSAGE("Suddenly some tame wolves materialize around you.");
    }
}

void silva::PrayBadEffect()
{
  switch(RAND() % 300 / 100)
    {
    case 0:
      game::GetPlayer()->Polymorph(new spider(true, true, false), 1000 + RAND() % 1000);
      break;
    case 1:
      game::GetPlayer()->Polymorph(new donkey(true, true, false), 1000 + RAND() % 1000);
      break;
    case 2:
      game::GetPlayer()->Polymorph(new jackal(true, true, false), 1000 + RAND() % 1000);
      break;
    }
}

void loricatus::PrayGoodEffect()
{
  /*for(stackiterator i = game::GetPlayer()->GetStack()->GetBottomSlot(); i != game::GetPlayer()->GetStack()->GetSlotAboveTop(); ++i)
    {
      item* Old = ***i;

      if(Old->GetType() == brokenplatemail::StaticType())
	{
	  bool Worn = false, Wielded = false;

	  if(Old == game::GetPlayer()->GetBodyArmor())
	    Worn = true;
	  else
	    if(Old == game::GetPlayer()->GetWielded())
	      Wielded = true;

	  Old->RemoveFromSlot();
	  //game::GetPlayer()->GetStack()->RemoveItem(c);
	  ADD_MESSAGE("Loricatus fixes your %s.", Old->CHARNAME(UNARTICLED));
	  Old->SetExists(false);
	  item* Plate = new platemail(false);
	  Plate->InitMaterials(Old->GetMainMaterial());
	  Old->PreserveMaterial(0);
	  game::GetPlayer()->GetStack()->AddItem(Plate);

	  if(Worn)
	    game::GetPlayer()->SetBodyArmor(Plate);
	  else
	    if(Wielded)
	      game::GetPlayer()->SetWielded(Plate);

	  return;
	}
    }*/

  item* Old = game::GetPlayer()->GetBodyArmor();

  if(Old && Old->GetType() == brokenplatemail::StaticType())
    {
      ADD_MESSAGE("Loricatus fixes your %s.", Old->CHARNAME(UNARTICLED));
      Old->RemoveFromSlot();
      Old->SetExists(false);
      item* Plate = new platemail(false);
      Plate->InitMaterials(Old->GetMainMaterial());
      game::GetPlayer()->GetStack()->AddItem(Plate);
    }
	
  if(game::GetPlayer()->GetMainWielded())
    if(game::GetPlayer()->GetMainWielded()->IsMaterialChangeable())
      {
	std::string OldName = game::GetPlayer()->GetMainWielded()->Name(UNARTICLED);
	game::GetPlayer()->GetMainWielded()->ChangeMainMaterial(new mithril);
	ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::GetPlayer()->GetMainWielded()->CHARNAME(INDEFINITE));
	return;
      }
    else
      ADD_MESSAGE("%s glows in a strange light but remains unchanged.", game::GetPlayer()->GetMainWielded()->CHARNAME(DEFINITE));
  else
    ADD_MESSAGE("You feel a slight tingling in your hands.");
}

void loricatus::PrayBadEffect()
{
  std::string OldName;

  if(game::GetPlayer()->GetMainWielded())
    if(game::GetPlayer()->GetMainWielded()->IsMaterialChangeable())
      {
	OldName = game::GetPlayer()->GetMainWielded()->Name(UNARTICLED);
	game::GetPlayer()->GetMainWielded()->ChangeMainMaterial(new bananaflesh);
	ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::GetPlayer()->GetMainWielded()->CHARNAME(INDEFINITE));
      }
    else
      ADD_MESSAGE("%s glows in a strange light but remain unchanged.", game::GetPlayer()->GetMainWielded()->CHARNAME(DEFINITE));
  else
    ADD_MESSAGE("You feel a slight tingling in your hands.");
}

void calamus::PrayGoodEffect()
{
  ADD_MESSAGE("%s gives you the talent for speed.", GOD_NAME);
  game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() + 1);
}

void calamus::PrayBadEffect()
{
  if(game::GetPlayer()->GetAgility() > 5)
    {
      ADD_MESSAGE("%s slows you down.", GOD_NAME);
      game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
    }
  else
    ADD_MESSAGE("Suprisingly you feel nothing.");
}

void erado::PrayGoodEffect()
{
  ADD_MESSAGE("The air vibrates violently around you. A terrible undead voice echoes through the caverns: \"SlAvE! ThOu HaSt PlAeSeD mE! lIfT tHiNe ReWaRd, ChAmPiOn!\" A heavy weapon of pure corruption materializes before you.");
  game::GetPlayer()->GetGiftStack()->AddItem(new neercseulb);
}

void erado::PrayBadEffect()
{
  ADD_MESSAGE("A dark, booming voice shakes the area: \"PuNy MoRtAl! YoU aRe NoT wOrThY! i ShAlL DeStRoY yOu LiKe EvErYoNe ElSe!\" A bolt of black energy hits you.");

  //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - game::GetPlayer()->GetMaxHP() + 1);
  game::GetPlayer()->ReceiveDamage(0, 1 + RAND() % 20, ENERGY, ALL);

  game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
  game::GetPlayer()->SetStrength(game::GetPlayer()->GetStrength() - 1);
  game::GetPlayer()->SetEndurance(game::GetPlayer()->GetEndurance() - 1);
  game::GetPlayer()->CheckDeath("obliterated by the unholy power of " + Name());
}

void mellis::PrayGoodEffect()
{
  bool Success = false;
  item* NewVersion;

  if(game::GetPlayer()->GetStack()->GetItems())
    {
      ADD_MESSAGE("%s tries to trade some of your items into better ones.", GOD_NAME);
      bool Cont = true;

      while(Cont)
	{
	  Cont = false;

	  for(stackiterator i = game::GetPlayer()->GetStack()->GetBottomSlot(); i != game::GetPlayer()->GetStack()->GetSlotAboveTop(); ++i)
	    {
	      NewVersion = (**i)->BetterVersion();

	      if(NewVersion)
		{
		  item* ToBeDeleted = ***i;
		  ToBeDeleted->RemoveFromSlot();
		  game::GetPlayer()->GetStack()->AddItem(NewVersion);
		  Success = true;
		  ADD_MESSAGE("%s manages to trade %s into %s.", GOD_NAME, ToBeDeleted->CHARNAME(DEFINITE), NewVersion->CHARNAME(INDEFINITE));
		  ToBeDeleted->SetExists(false);
		  Cont = true;
		  break;
		}
	    }
	}
    }

  if(!Success)
    ADD_MESSAGE("You have no good items for trade.");
}

void mellis::PrayBadEffect()
{
  for(ushort c = 1; c < game::GetGods(); ++c)
    if(game::GetGod(c) != this)
      game::GetGod(c)->AdjustRelation(-100);

  ADD_MESSAGE("%s spreads bad rumours about you to other gods.", GOD_NAME);
}

void valpurus::Pray()
{
  if(!Timer && Relation == 1000)
    {
      ADD_MESSAGE("You feel %s is very pleased.", GOD_NAME);
      PrayGoodEffect();
      AdjustTimer(100000);
      AdjustRelation(-500);
      game::ApplyDivineAlignmentBonuses(this, true);

      if(Relation > 500 && !(RAND() % 5))
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
      ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GOD_NAME);
      PrayBadEffect();
      AdjustTimer(50000);
      AdjustRelation(-100);
      game::ApplyDivineAlignmentBonuses(this, false);

      if(Relation < -500 && !(RAND() % 25))
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

void atavus::Pray()
{
  if(!Timer && Relation > 500 + RAND() % 500)
    {
      ADD_MESSAGE("You feel %s is pleased.", GOD_NAME);
      PrayGoodEffect();
      AdjustTimer(50000);
      AdjustRelation(-250);
      game::ApplyDivineAlignmentBonuses(this, true);

      if(Relation > 500 && !(RAND() % 5))
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
      ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GOD_NAME);
      PrayBadEffect();
      AdjustTimer(50000);
      AdjustRelation(-100);
      game::ApplyDivineAlignmentBonuses(this, false);

      if(Relation < -500 && !(RAND() % 25))
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

void erado::Pray()
{
  if(!Timer && Relation == 1000)
    {
      ADD_MESSAGE("You feel %s is very pleased.", GOD_NAME);
      PrayGoodEffect();
      AdjustTimer(100000);
      AdjustRelation(-500);
      game::ApplyDivineAlignmentBonuses(this, true);

      if(Relation > 500 && !(RAND() % 5))
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
      ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GOD_NAME);
      PrayBadEffect();
      AdjustTimer(50000);
      AdjustRelation(-100);
      game::ApplyDivineAlignmentBonuses(this, false);

      if(Relation < -500 && !(RAND() % 5))
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

void infuscor::PrayBadEffect()
{
  ADD_MESSAGE("Vile and evil knowledge pulps into your brain. It's too much for it to handle; you faint.");
  game::GetPlayer()->Faint();
}

void macellarius::PrayGoodEffect()
{
  ADD_MESSAGE("%s wishes you to have fun with this potion.", GOD_NAME);
  potion* Reward = new potion(false);
  Reward->InitMaterials(new glass, new omleurine);
  game::GetPlayer()->GetGiftStack()->AddItem(Reward);
  ADD_MESSAGE("%s drops on the ground.", Reward->CHARNAME(DEFINITE));
}

void macellarius::PrayBadEffect()
{
  ADD_MESSAGE("A potion drops on your head and shatters into small bits.");
  game::GetPlayer()->ReceiveDamage(0, RAND() % 7, PHYSICALDAMAGE, HEAD);
  //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - RAND() % 7);
  game::GetPlayer()->GetLSquareUnder()->GetStack()->AddItem(new brokenbottle);
  game::GetPlayer()->CheckDeath("killed while enjoying the company of " + Name());
}

void scabies::PrayGoodEffect()
{
  ADD_MESSAGE("Five cans full of school food drop from somewhere above!");

  for(ushort c = 0; c < 5; ++c)
    {
      can* Reward = new can(false);
      Reward->InitMaterials(new iron, new schoolfood);
      game::GetPlayer()->GetGiftStack()->AddItem(Reward);
    }
}

void scabies::PrayBadEffect()
{
  ADD_MESSAGE("%s makes you eat a LOT of school food.", GOD_NAME);

  for(ushort c = 0; c < 5; ++c)
    {
      material* SchoolFood = new schoolfood(600);
      SchoolFood->EatEffect(game::GetPlayer(), 600);
      delete SchoolFood;
    }

  ADD_MESSAGE("You feel your muscles softening terribly...");

  game::GetPlayer()->SetStrength(game::GetPlayer()->GetStrength() - 1);
  game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
}

void infuscor::PrayGoodEffect()
{
  ADD_MESSAGE("Suddenly five scrolls appear from nothing!");

  for(ushort c = 0; c < 5; ++c)
    game::GetPlayer()->GetGiftStack()->AddItem(new scrollofteleport);
}

void cruentus::PrayGoodEffect()
{
  ADD_MESSAGE("Cruentus recommends you to its master, Erado.");
  game::GetGod(16)->AdjustRelation(100);
}

void cruentus::PrayBadEffect()
{
  item* ToBe = game::GetPlayer()->GetMainWielded();

  if(ToBe)
    {
      if(!ToBe->IsDestroyable())
      {
	ToBe = game::GetPlayer()->GetSecondaryWielded();

	if(!ToBe)
	  ADD_MESSAGE("%s tries to destroy your %s, but fails.", GOD_NAME, game::GetPlayer()->GetMainWielded()->CHARNAME(UNARTICLED));
	else
	  if(!ToBe->IsDestroyable())
	    ADD_MESSAGE("%s tries to destroy your %s, but fails.", GOD_NAME, ToBe->CHARNAME(UNARTICLED));
      }
    }
  else
    {
	ToBe = game::GetPlayer()->GetSecondaryWielded();

	if(ToBe && !ToBe->IsDestroyable())
	  ADD_MESSAGE("%s tries to destroy your %s, but fails.", GOD_NAME, ToBe->CHARNAME(UNARTICLED));
    }

  if(ToBe && ToBe->IsDestroyable())
    {
      ADD_MESSAGE("%s destroys your weapon.", GOD_NAME);

      ToBe->RemoveFromSlot();
      ToBe->SetExists(false);
    }
  else
    {
      ADD_MESSAGE("%s gets mad and hits you!", GOD_NAME);
      //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - RAND() % 20);
      game::GetPlayer()->ReceiveDamage(0, 1 + RAND() % 20, PHYSICALDAMAGE, ALL, RAND() % 8);
      game::GetPlayer()->CheckDeath("destroyed by " + Name());
    }
}

void cruentus::Pray()
{
  if(!Timer && Relation > 500 + RAND() % 500)
    {
      ADD_MESSAGE("You feel %s is pleased.", GOD_NAME);
      PrayGoodEffect();
      AdjustTimer(50000);
      AdjustRelation(-250);
      game::ApplyDivineAlignmentBonuses(this, true);

      if(Relation > 500 && !(RAND() % 10))
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
      ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GOD_NAME);
      PrayBadEffect();
      AdjustTimer(50000);
      AdjustRelation(-100);
      game::ApplyDivineAlignmentBonuses(this, false);

      if(Relation < -500 && !(RAND() % 10))
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

std::string venius::GetPriestMessage() const
{
  return "\"" + Name() + " is the Great Protector of all Law and Order. Prayeth upon, He may burn thy enemies with the Fire of Justice, if thou areth worthy.\"";
}

std::string dulcis::GetPriestMessage() const
{
  return "\"" + Name() + " is the Creator of everything that we call Art and Beauty. When thou pray for Her help, She may calm thine worst enemies with Her love. But beware! There areth some villains that may resist even Her call!\"";
}

std::string seges::GetPriestMessage() const
{
  return "\"" + Name() + " brings Life, Health and Nutrition to all who follow Her. When thou call upon Her with an empty stomach, a miracle may indeed fill it.";
}

std::string consummo::GetPriestMessage() const
{
  return "\"The Wise bow before " + Name() + ", for He maketh the Universe as rational as it is. Those who follow Him are not bound to space and time, since knowledge controls them. This is why those chosen by Him may escape any danger with their wisdom. Alas, beware! Soon thou may find thyself in an even worse situation!\"";
}

void scabies::PlayerVomitedOnAltar()
{
  ADD_MESSAGE("%s feels that you are indeed her follower.", GOD_NAME); 
  AdjustRelation(1);
}

std::string valpurus::GetPriestMessage() const
{
  return "\"" + Name() + " the Great Frog is the highest of all gods. The Wise know that the world is really a square pancake which He carries on His back. This is why this Cathedral and the whole city of Attnam is dedicated to His worship.\" \"In thine prayers thou must understand that He is a busy god who knows His importance. He will not help newbies. Pray Him only when He calls thee a Champion!\"";
}


