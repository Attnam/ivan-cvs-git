#define __FILE_OF_STATIC_GOD_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "godba.h"

std::vector<god::prototype*> protocontainer<god>::ProtoData;
valuemap protocontainer<god>::CodeNameMap;

#include "godde.h"

#undef __FILE_OF_STATIC_GOD_PROTOTYPE_DEFINITIONS__

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
#include "festring.h"

void sophos::PrayGoodEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement! You teleport away!");
  game::GetPlayer()->Move(game::GetCurrentLevel()->RandomSquare(game::GetPlayer(), true), true);
}

void sophos::PrayBadEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement!");
  game::GetPlayer()->TeleportSomePartsAway(RAND() % 2 + 1);
  game::GetPlayer()->CheckDeath("shattered to pieces by the wrath of " + Name());
}

void valpurus::PrayGoodEffect()
{
  ADD_MESSAGE("You hear booming voice: \"DEFEAT MORTIFER WITH THIS, MY PALADIN!\" A sword glittering with holy might appears from nothing.");
  meleeweapon* Weapon = new meleeweapon(TWOHANDEDSWORD);
  Weapon->InitMaterials(MAKE_MATERIAL(VALPURIUM), MAKE_MATERIAL(VALPURIUM), 0);
  game::GetPlayer()->GetGiftStack()->AddItem(Weapon);
}

void valpurus::PrayBadEffect()
{
  ADD_MESSAGE("Valpurus smites you with a small hammer.");
  game::GetPlayer()->ReceiveDamage(0, 10, PHYSICALDAMAGE, HEAD, RAND() % 8);
  //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - 5);
  game::GetPlayer()->CheckDeath("faced the hammer of Justice from the hand of " + Name());
}

void legifer::PrayGoodEffect()
{
  ADD_MESSAGE("A booming voice echoes: \"Xunil! Xunil! Save us!\" A huge firestorm engulfs everything around you.");
  game::GetCurrentLevel()->Explosion(game::GetPlayer(), "killed accidentally by " + Name(), game::GetPlayer()->GetPos(), 40, false);
}

void legifer::PrayBadEffect()
{
  ADD_MESSAGE("%s casts a beam of horrible, yet righteous, fire on you.", GOD_NAME);
  game::GetPlayer()->ReceiveDamage(0, 20 + RAND() % 20, FIRE, ALL);
  //game::GetPlayer()->ReceiveFireDamage(game::GetPlayer(), "killed accidentally by " + Name(), 20);
  game::GetPlayer()->CheckDeath("burned to death by the wrath of " + Name());
}

void dulcis::PrayGoodEffect()
{
  ADD_MESSAGE("A beautiful melody echoes around you.");

  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = game::GetPlayer()->GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char)
	    if(Char->IsCharmable())
	      if(game::GetPlayer()->GetRelativeDanger(Char) > 2.0f)
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
	}
    }
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
  if(game::GetPlayer()->HasAllBodyParts() || game::GetPlayer()->GetNP() < HUNGERLEVEL)
    {
      ADD_MESSAGE("Your stomach feels full again.");

      if(game::GetPlayer()->GetNP() < 10000)
	game::GetPlayer()->SetNP(10000);
    }
  else
    {
      bodypart* OldBodyPart = game::GetPlayer()->FindRandomOwnBodyPart();

      if(OldBodyPart != 0)
	{
	  OldBodyPart->RemoveFromSlot();
	  game::GetPlayer()->AttachBodyPart(OldBodyPart);
	  OldBodyPart->SetHP(OldBodyPart->GetMaxHP());
	  ADD_MESSAGE("%s attaches your old %s back and heals it.", GOD_NAME, OldBodyPart->GetBodyPartName().c_str());
	}
      else
	{
	  bodypart* NewBodyPart = game::GetPlayer()->GenerateRandomBodyPart();
	  NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
	  ADD_MESSAGE("You grow a new %s.", NewBodyPart->GetBodyPartName().c_str()); 
	}
    }
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
  if(game::GetPlayer()->HasAllBodyParts())
    {
      item* Reward = new bodyarmor(PLATEMAIL, NOMATERIALS);
      Reward->InitMaterials(MAKE_MATERIAL(MITHRIL));
      ADD_MESSAGE("%s materializes before you.", Reward->CHARNAME(INDEFINITE));
      game::GetPlayer()->GetGiftStack()->AddItem(Reward);
    }
  else
    {
      bodypart* NewBodyPart = game::GetPlayer()->GenerateRandomBodyPart();
      NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
      ADD_MESSAGE("You gives you a new %s as a gift between friends.", NewBodyPart->GetBodyPartName().c_str());
    }
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
	  Disappearing->SendToHell();
	}
      else
	{
	  ADD_MESSAGE("%s tries to remove your %s, but fails. You feel you are not so gifted anymore.", GOD_NAME, Disappearing->CHARNAME(UNARTICLED));
	  game::GetPlayer()->EditAttribute(AGILITY, -1);
	  game::GetPlayer()->EditAttribute(ARMSTRENGTH, -1);
	  game::GetPlayer()->EditAttribute(ENDURANCE, -1);
	}
    }
  else
    {
      ADD_MESSAGE("You feel you are not so gifted anymore.");
      game::GetPlayer()->EditAttribute(AGILITY, -1);
      game::GetPlayer()->EditAttribute(ARMSTRENGTH, -1);
      game::GetPlayer()->EditAttribute(ENDURANCE, -1);
    }
}

void silva::PrayGoodEffect()
{
  if(!game::GetPlayer()->HasAllBodyParts())
    {
      bodypart* OldBodyPart = game::GetPlayer()->FindRandomOwnBodyPart();

      if(OldBodyPart != 0)
	{
	  OldBodyPart->RemoveFromSlot();
	  game::GetPlayer()->AttachBodyPart(OldBodyPart);
	  OldBodyPart->SetHP(1);
	  ADD_MESSAGE("%s attaches your old %s back.", GOD_NAME, OldBodyPart->GetBodyPartName().c_str());
	}
      else
	{
	  bodypart* NewBodyPart = game::GetPlayer()->GenerateRandomBodyPart();
	  NewBodyPart->SetHP(1);
	  ADD_MESSAGE("You grow a new %s.", NewBodyPart->GetBodyPartName().c_str()); 
	}

      return;
    }

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

	    for(ushort d = 0; d < 8; ++d)
	      {
		lsquare* Square = game::GetCurrentLevel()->GetNeighbourLSquare(Pos, d);

		if(Square && Square->GetOLTerrain()->IsWalkable())
		  {
		    Correct = true;
		    break;
		  }
	      }

	    if(Correct)
	      {
		game::GetCurrentLevel()->GetLSquare(Pos)->ChangeOLTerrainAndUpdateLights(new empty);

		for(ushort p = 0; p < 4; ++p)
		  game::GetCurrentLevel()->GetLSquare(Pos)->GetSideStack(p)->Clean();

		break;
	      }
	  }

      ushort ToGround = 20 + RAND() % 21;

      for(c = 0; c < ToGround; ++c)
	for(ushort i = 0; i < 50; ++i)
	  {
	    vector2d Pos = game::GetCurrentLevel()->RandomSquare(0, true, RAND() & 1);

	    character* Char = game::GetCurrentLevel()->GetLSquare(Pos)->GetCharacter();

	    if(!game::GetCurrentLevel()->GetLSquare(Pos)->GetOLTerrain()->IsSafeToDestroy() || (Char && (Char->IsPlayer() || Char->GetTeam()->GetRelation(game::GetPlayer()->GetTeam()) != HOSTILE)))
	      continue;

	    ushort Walkables = 0;

	    for(ushort d = 0; d < 8; ++d)
	      {
		lsquare* Square = game::GetCurrentLevel()->GetNeighbourLSquare(Pos, d);

		if(Square && Square->GetOLTerrain()->IsWalkable())
		  ++Walkables;
	      }

	    if(Walkables > 6)
	      {
		game::GetCurrentLevel()->GetLSquare(Pos)->ChangeOLTerrainAndUpdateLights(new wall(EARTH));

		if(Char)
		  {
		    if(Char->CanBeSeenByPlayer())
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

      for(ushort d = 0; d < 8; ++d)
	{
	  square* Square = game::GetPlayer()->GetNeighbourSquare(d);

	  if(Square)
	    {
	      wolf* Wolf = new wolf;

	      if(Square->IsWalkable(Wolf) && !Square->GetCharacter())
		{
		  Wolf->SetTeam(game::GetPlayer()->GetTeam());
		  Square->AddCharacter(Wolf);
		  ++Created;
		}
	      else
		delete Wolf;
	    }
	}

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
      game::GetPlayer()->Polymorph(new spider, 1000 + RAND() % 1000);
      break;
    case 1:
      game::GetPlayer()->Polymorph(new donkey, 1000 + RAND() % 1000);
      break;
    case 2:
      game::GetPlayer()->Polymorph(new jackal, 1000 + RAND() % 1000);
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
	  Old->SendToHell();
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

  /*item* Old = game::GetPlayer()->GetBodyArmor();

  if(Old && Old->GetType() == brokenplatemail::StaticType())
    {
      ADD_MESSAGE("Loricatus fixes your %s.", Old->CHARNAME(UNARTICLED));
      Old->RemoveFromSlot();
      Old->SendToHell();
      item* Plate = new platemail(0, false);
      Plate->InitMaterials(Old->GetMainMaterial());
      game::GetPlayer()->GetStack()->AddItem(Plate);
    }*/

  if(!game::GetPlayer()->HasAllBodyParts())
    {
      bodypart* NewBodyPart = game::GetPlayer()->GenerateRandomBodyPart();
      NewBodyPart->ChangeMainMaterial(MAKE_MATERIAL(MITHRIL));
      NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
      ADD_MESSAGE("You grow a new %s that is made of mithril.", NewBodyPart->GetBodyPartName().c_str());
      return;
    }
	
  if(game::GetPlayer()->GetMainWielded())
    if(game::GetPlayer()->GetMainWielded()->IsMaterialChangeable())
      {
	std::string OldName;
	game::GetPlayer()->GetMainWielded()->AddName(OldName, UNARTICLED);
	game::GetPlayer()->GetMainWielded()->ChangeMainMaterial(MAKE_MATERIAL(STEEL));
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
	game::GetPlayer()->GetMainWielded()->AddName(OldName, UNARTICLED);
	game::GetPlayer()->GetMainWielded()->ChangeMainMaterial(MAKE_MATERIAL(BANANAFLESH));
	ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::GetPlayer()->GetMainWielded()->CHARNAME(INDEFINITE));
      }
    else
      ADD_MESSAGE("%s glows in a strange light but remain unchanged.", game::GetPlayer()->GetMainWielded()->CHARNAME(DEFINITE));
  else
    ADD_MESSAGE("You feel a slight tingling in your hands.");
}

void cleptia::PrayGoodEffect()
{
  ADD_MESSAGE("%s gives you the talent for speed.", GOD_NAME);
  game::GetPlayer()->BeginTemporaryState(HASTE, 250);
}

void cleptia::PrayBadEffect()
{
  ADD_MESSAGE("%s slows you down.", GOD_NAME);
  game::GetPlayer()->BeginTemporaryState(SLOW, 250);
}

void mortifer::PrayGoodEffect()
{
  ADD_MESSAGE("The air vibrates violently around you. A terrible undead voice echoes through the caverns: \"SlAvE! ThOu HaSt PlAeSeD mE! lIfT tHiNe ReWaRd, ChAmPiOn!\" A heavy weapon of pure corruption materializes before you.");
  game::GetPlayer()->GetGiftStack()->AddItem(new neercseulb);
}

void mortifer::PrayBadEffect()
{
  ADD_MESSAGE("A dark, booming voice shakes the area: \"PuNy MoRtAl! YoU aRe NoT wOrThY! i ShAlL DeStRoY yOu LiKe EvErYoNe ElSe!\" A bolt of black energy hits you.");

  //game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - game::GetPlayer()->GetMaxHP() + 1);
  game::GetPlayer()->ReceiveDamage(0, 1 + RAND() % 20, ENERGY, ALL);

  game::GetPlayer()->EditAttribute(AGILITY, -1);
  game::GetPlayer()->EditAttribute(ARMSTRENGTH, -1);
  game::GetPlayer()->EditAttribute(ENDURANCE, -1);
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

	  for(stackiterator i = game::GetPlayer()->GetStack()->GetBottom(); i.HasItem(); ++i)
	    {
	      NewVersion = i->BetterVersion();

	      if(NewVersion)
		{
		  item* ToBeDeleted = *i;
		  ToBeDeleted->RemoveFromSlot();
		  game::GetPlayer()->GetStack()->AddItem(NewVersion);
		  Success = true;
		  ADD_MESSAGE("%s manages to trade %s into %s.", GOD_NAME, ToBeDeleted->CHARNAME(DEFINITE), NewVersion->CHARNAME(INDEFINITE));
		  ToBeDeleted->SendToHell();
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

void mortifer::Pray()
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

void nefas::PrayGoodEffect()
{
  ADD_MESSAGE("%s wishes you to have fun with this potion.", GOD_NAME);
  potion* Reward = new potion(0, NOMATERIALS);
  Reward->InitMaterials(MAKE_MATERIAL(GLASS), MAKE_MATERIAL(OMLEURINE));
  game::GetPlayer()->GetGiftStack()->AddItem(Reward);
  ADD_MESSAGE("%s drops on the ground.", Reward->CHARNAME(DEFINITE));
}

void nefas::PrayBadEffect()
{
  ADD_MESSAGE("A potion drops on your head and shatters into small bits.");
  game::GetPlayer()->ReceiveDamage(0, 2 + RAND() % 7, PHYSICALDAMAGE, HEAD);
  game::GetPlayer()->GetStackUnder()->AddItem(new brokenbottle);
  game::GetPlayer()->CheckDeath("killed while enjoying the company of " + Name());
}

void scabies::PrayGoodEffect()
{
  if(!game::GetPlayer()->HasAllBodyParts())
    {
      bodypart* OldBodyPart = game::GetPlayer()->FindRandomOwnBodyPart();

      if(OldBodyPart != 0)
	{
	  OldBodyPart->RemoveFromSlot();
	  game::GetPlayer()->AttachBodyPart(OldBodyPart);
	  OldBodyPart->SetHP(1);
	  OldBodyPart->Mutate();
	  ADD_MESSAGE("%s attaches your old %s back. But it seems somehow different...", GOD_NAME, OldBodyPart->GetBodyPartName().c_str());
	}
      else
	{
	  bodypart* NewBodyPart = game::GetPlayer()->GenerateRandomBodyPart();
	  NewBodyPart->SetHP(1);
	  NewBodyPart->Mutate();
	  ADD_MESSAGE("You grow a new %s, which seems to be a bit strange.", NewBodyPart->GetBodyPartName().c_str()); 
	}

      return;
    }

  ADD_MESSAGE("Five cans full of school food drop from somewhere above!");

  for(ushort c = 0; c < 5; ++c)
    {
      can* Reward = new can(0, NOMATERIALS);
      Reward->InitMaterials(MAKE_MATERIAL(IRON), MAKE_MATERIAL(SCHOOLFOOD));
      game::GetPlayer()->GetGiftStack()->AddItem(Reward);
    }
}

void scabies::PrayBadEffect()
{
  ADD_MESSAGE("%s makes you eat a LOT of school food.", GOD_NAME);
  material* SchoolFood = MAKE_MATERIAL(SCHOOLFOOD, 1000);
  SchoolFood->EatEffect(game::GetPlayer(), 1000);
  delete SchoolFood;
  ADD_MESSAGE("You feel your muscles softening terribly...");
  game::GetPlayer()->EditAttribute(ARMSTRENGTH, -1);
  game::GetPlayer()->EditAttribute(DEXTERITY, -1);
}

void infuscor::PrayGoodEffect()
{
  ADD_MESSAGE("Suddenly five scrolls appear from nothing!");

  for(ushort c = 0; c < 5; ++c)
    game::GetPlayer()->GetGiftStack()->AddItem(new scrollofteleportation);
}

void cruentus::PrayGoodEffect()
{
  if(!game::GetPlayer()->HasAllBodyParts())
    {
      bodypart* NewBodyPart = game::GetPlayer()->GenerateRandomBodyPart();
      NewBodyPart->ChangeMainMaterial(MAKE_MATERIAL(STEEL));
      NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
      ADD_MESSAGE("You grow a new %s, which seems to be made of steel.", NewBodyPart->GetBodyPartName().c_str()); 
      return;
    }

  ADD_MESSAGE("Cruentus recommends you to its master, Mortifer.");
  game::GetGod(15)->AdjustRelation(100);

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
      ToBe->SendToHell();
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

std::string legifer::GetPriestMessage() const
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

std::string sophos::GetPriestMessage() const
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
