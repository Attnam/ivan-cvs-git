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
  game::GetPlayer()->Move(game::GetCurrentLevel()->GetRandomSquare(game::GetPlayer()), true);
}

void sophos::PrayBadEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement!");
  game::GetPlayer()->TeleportSomePartsAway(RAND() % 2 + 1);
  game::GetPlayer()->CheckDeath("shattered to pieces by the wrath of " + Name(), 0);
}

void valpurus::PrayGoodEffect()
{
  if(RAND() & 7)
    {
      ADD_MESSAGE("You hear booming voice: \"THIS WILL PROTECT THEE FROM MORTIFER, MY PALADIN!\" A shield glittering with holy might appears from nothing.");
      shield* Shield = new shield;
      Shield->InitMaterials(MAKE_MATERIAL(VALPURIUM));
      game::GetPlayer()->GetGiftStack()->AddItem(Shield);
    }
  else
    {
      ADD_MESSAGE("You hear booming voice: \"DEFEAT MORTIFER WITH THIS, MY PALADIN!\" A sword glittering with holy might appears from nothing.");
      meleeweapon* Weapon = new meleeweapon(TWO_HANDED_SWORD);
      Weapon->InitMaterials(MAKE_MATERIAL(VALPURIUM), MAKE_MATERIAL(VALPURIUM), 0);
      game::GetPlayer()->GetGiftStack()->AddItem(Weapon);
    }
}

void valpurus::PrayBadEffect()
{
  ADD_MESSAGE("Valpurus smites you with a small hammer.");
  game::GetPlayer()->ReceiveDamage(0, 10, PHYSICAL_DAMAGE, HEAD, RAND() % 8);
  game::GetPlayer()->CheckDeath("faced the hammer of Justice from the hand of " + Name(), 0);
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
  game::GetPlayer()->CheckDeath("burned to death by the wrath of " + Name(), 0);
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
		    ADD_MESSAGE("%s seems to be very happy.", Char->CHAR_NAME(DEFINITE));
		  else if(Char->GetRelation(game::GetPlayer()) == HOSTILE)
		    ADD_MESSAGE("%s stops fighting.", Char->CHAR_NAME(DEFINITE));
		  else
		    ADD_MESSAGE("%s seems to be very friendly towards you.", Char->CHAR_NAME(DEFINITE));

		  Char->ChangeTeam(game::GetPlayer()->GetTeam());
		}
	      else
		ADD_MESSAGE("%s resists its charming call.", Char->CHAR_NAME(DEFINITE));
	    else
	      ADD_MESSAGE("%s seems not affected.", Char->CHAR_NAME(DEFINITE));
	}
    }
}

void dulcis::PrayBadEffect()
{
  ADD_MESSAGE("%s plays a horrible tune that rots your brain.", GOD_NAME);
  game::GetPlayer()->ReceiveDamage(0, 1 + RAND() % 9, SOUND, HEAD);
  game::GetPlayer()->CheckDeath("became insane by listening " + Name() + " too much", 0);
}

void seges::PrayGoodEffect()
{
  if(game::GetPlayer()->HasAllBodyParts() || game::GetPlayer()->GetNP() < HUNGER_LEVEL)
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
      item* Reward = new bodyarmor(PLATE_MAIL, NO_MATERIALS);
      Reward->InitMaterials(MAKE_MATERIAL(MITHRIL));
      ADD_MESSAGE("%s materializes before you.", Reward->CHAR_NAME(INDEFINITE));
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
	  ADD_MESSAGE("Your %s disappears.", Disappearing->CHAR_NAME(UNARTICLED));
	  Disappearing->RemoveFromSlot();
	  //if(game::GetPlayer()->GetWielded() == Disappearing) game::GetPlayer()->SetWielded(0);
	  //if(game::GetPlayer()->GetBodyArmor() == Disappearing) game::GetPlayer()->SetBodyArmor(0);
	  Disappearing->SendToHell();
	}
      else
	{
	  ADD_MESSAGE("%s tries to remove your %s, but fails. You feel you are not so gifted anymore.", GOD_NAME, Disappearing->CHAR_NAME(UNARTICLED));
	  game::GetPlayer()->EditAttribute(AGILITY, -1);
	  game::GetPlayer()->EditAttribute(ARM_STRENGTH, -1);
	  game::GetPlayer()->EditAttribute(ENDURANCE, -1);
	}
    }
  else
    {
      ADD_MESSAGE("You feel you are not so gifted anymore.");
      game::GetPlayer()->EditAttribute(AGILITY, -1);
      game::GetPlayer()->EditAttribute(ARM_STRENGTH, -1);
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
	game::GetCurrentLevel()->AttachPos(game::GetCurrentLevel()->GetRandomSquare(0, NOT_WALKABLE|ATTACHABLE));

      uchar ToEmpty = 10 + RAND() % 11;

      for(c = 0; c < ToEmpty; ++c)
	for(ushort i = 0; i < 50; ++i)
	  {
	    vector2d Pos = game::GetCurrentLevel()->GetRandomSquare(0, NOT_WALKABLE);
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
	    vector2d Pos = game::GetCurrentLevel()->GetRandomSquare(0, RAND() & 1 ? 0 : HAS_CHARACTER);

	    character* Char = game::GetCurrentLevel()->GetLSquare(Pos)->GetCharacter();

	    if(!game::GetCurrentLevel()->GetLSquare(Pos)->GetOLTerrain()->IsSafeToDestroy() || (Char && (Char->IsPlayer() || game::GetPlayer()->GetRelation(Char) != HOSTILE)))
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
		      ADD_MESSAGE("%s is hit by a brick of earth falling from the roof!", Char->CHAR_NAME(DEFINITE));

		    Char->ReceiveDamage(0, 20 + RAND() % 21, PHYSICAL_DAMAGE, HEAD|TORSO, 8, true);
		    Char->CheckDeath("killed by an earthquake", 0);
		  }

		game::GetCurrentLevel()->GetLSquare(Pos)->KickAnyoneStandingHereAway();

		ushort p;

		for(p = 0; p < 4; ++p)
		  game::GetCurrentLevel()->GetLSquare(Pos)->GetSideStack(p)->Clean();

		game::GetCurrentLevel()->GetLSquare(Pos)->GetStack()->ReceiveDamage(0, 10 + RAND() % 41, PHYSICAL_DAMAGE);
		break;
	      }
	  }
      // Generate a few boulders in the level
      ushort BoulderNumber = RAND() % 20;
      for(c = 0; c < BoulderNumber; ++c)
	{
	    vector2d Pos = game::GetCurrentLevel()->GetRandomSquare();
	    character* MonsterHere = game::GetCurrentLevel()->GetLSquare(Pos)->GetCharacter();
	    if(!MonsterHere || !MonsterHere->IsPlayer())
	      {
		game::GetCurrentLevel()->GetLSquare(Pos)->ChangeOLTerrainAndUpdateLights(new boulder);
		if(MonsterHere)
		  {
		    MonsterHere->ReceiveDamage(0, 1 + RAND() % 5, PHYSICAL_DAMAGE, HEAD|TORSO, 8, true);
		  }
		game::GetCurrentLevel()->GetLSquare(Pos)->GetStack()->ReceiveDamage(0, 1 + RAND() % 5, PHYSICAL_DAMAGE);
	      }
	}
      // Impact damage for items in the level

      for(ushort x = 0; x < game::GetCurrentLevel()->GetXSize(); ++x)
	for(ushort y = 0; y < game::GetCurrentLevel()->GetYSize(); ++y)
	  {
	    game::GetCurrentLevel()->GetLSquare(x,y)->GetStack()->ReceiveDamage(0, RAND() % 5, PHYSICAL_DAMAGE);

	    for(ushort c = 0; c < 4; ++c)
	      game::GetCurrentLevel()->GetLSquare(x,y)->GetSideStack(c)->ReceiveDamage(0, RAND() % 5, PHYSICAL_DAMAGE);
	  }
    }
  else
    {
      ushort TryToCreate = 1 + RAND() % 7;
      ushort Created = 0;

      for(; Created < TryToCreate; ++Created)
	{
	  wolf* Wolf = new wolf;
	  vector2d Pos = game::GetCurrentLevel()->GetNearestFreeSquare(Wolf, game::GetPlayer()->GetPos());

	  if(Pos == DIR_ERROR_VECTOR)
	    {
	      delete Wolf;
	      break;
	    }

	  Wolf->SetTeam(game::GetPlayer()->GetTeam());
	  game::GetCurrentLevel()->AddCharacter(Pos, Wolf);
	}

      if(!Created)
	ADD_MESSAGE("You hear a sad howling of a wolf imprisoned in the earth.");

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
      game::GetPlayer()->Polymorph(new ass, 1000 + RAND() % 1000);
      break;
    case 2:
      game::GetPlayer()->Polymorph(new jackal, 1000 + RAND() % 1000);
      break;
    }
}

void loricatus::PrayGoodEffect()
{
  for(ushort c = 0; c < game::GetPlayer()->GetEquipmentSlots(); ++c)
    if(game::GetPlayer()->GetEquipment(c) && game::GetPlayer()->GetEquipment(c)->IsBroken())
      {
	ADD_MESSAGE("%s fixes your %s.", GOD_NAME, game::GetPlayer()->GetEquipment(c)->CHAR_NAME(UNARTICLED));
	game::GetPlayer()->GetEquipment(c)->Fix();
	return;
      }

  if(!game::GetPlayer()->HasAllBodyParts())
    {
      bodypart* NewBodyPart = game::GetPlayer()->GenerateRandomBodyPart();
      NewBodyPart->ChangeMainMaterial(MAKE_MATERIAL(STEEL));
      NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
      ADD_MESSAGE("You grow a new %s that is made of steel.", NewBodyPart->GetBodyPartName().c_str());
      return;
    }
	
  if(game::GetPlayer()->GetMainWielded())
    {
      if(game::GetPlayer()->GetMainWielded()->IsMaterialChangeable())
	{
	  std::string OldName;
	  ushort StrengthValue = protocontainer<material>::GetProto(1)->GetConfig().find(STEEL)->second.StrengthValue;

	  if(StrengthValue > game::GetPlayer()->GetMainWielded()->GetMainMaterial()->GetStrengthValue())
	    {
	      game::GetPlayer()->GetMainWielded()->AddName(OldName, UNARTICLED);
	      game::GetPlayer()->GetMainWielded()->ChangeMainMaterial(MAKE_MATERIAL(STEEL));
	      ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::GetPlayer()->GetMainWielded()->CHAR_NAME(INDEFINITE));
	    }
	  else
	    {
	      if(!(RAND() % 10))
		{
		  item* Scroll = new scrollofrepair;
		  ADD_MESSAGE("%s gives you %s.", GOD_NAME, Scroll->CHAR_NAME(INDEFINITE));
		  game::GetPlayer()->GetGiftStack()->AddItem(Scroll);
		}
	      else
		ADD_MESSAGE("\"Mortal, thou art always my valiant knight!\"");  
	    }
	  return;
	}
      else
	ADD_MESSAGE("%s glows in a strange light but remains unchanged.", game::GetPlayer()->GetMainWielded()->CHAR_NAME(DEFINITE));
    }
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
	game::GetPlayer()->GetMainWielded()->ChangeMainMaterial(MAKE_MATERIAL(BANANA_FLESH));
	ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::GetPlayer()->GetMainWielded()->CHAR_NAME(INDEFINITE));
      }
    else
      ADD_MESSAGE("%s glows in a strange light but remain unchanged.", game::GetPlayer()->GetMainWielded()->CHAR_NAME(DEFINITE));
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
  ADD_MESSAGE("The air vibrates violently around you. A terrible undead voice echoes through the caverns: \"SlAvE! ThOu HaSt PlAeSeD mE! lIfT tHy ReWaRd, ChAmPiOn!\" A heavy weapon of pure corruption materializes before you.");
  game::GetPlayer()->GetGiftStack()->AddItem(new neercseulb);
}

void mortifer::PrayBadEffect()
{
  ADD_MESSAGE("A dark, booming voice shakes the area: \"PuNy MoRtAl! ThOu ArT nOt WoRtHy! I sHaLl dEsTrOy ThEe LiKe EvErYoNe ElSe!\" A bolt of black energy hits you.");
  game::GetPlayer()->ReceiveDamage(0, 1 + RAND() % 20, ENERGY, ALL);
  game::GetPlayer()->EditAttribute(AGILITY, -1);
  game::GetPlayer()->EditAttribute(ARM_STRENGTH, -1);
  game::GetPlayer()->EditAttribute(ENDURANCE, -1);
  game::GetPlayer()->CheckDeath("obliterated by the unholy power of " + Name(), 0);
}

void mellis::PrayGoodEffect()
{
  bool Success = false;
  item* NewVersion;

  if(game::GetPlayer()->GetStack()->GetItems())
    {
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
		  ADD_MESSAGE("%s manages to trade %s into %s.", GOD_NAME, ToBeDeleted->CHAR_NAME(DEFINITE), NewVersion->CHAR_NAME(INDEFINITE));
		  ToBeDeleted->SendToHell();
		  Cont = true;
		  break;
		}
	    }
	}
    }

  if((Success && !(RAND() % 5) || (!Success && !(RAND() % 3))))
    {
      uchar GodNumber = game::GetGods();
      uchar* Possible = new uchar[GodNumber]; 
      ushort PossibleSize = 0;

      for(uchar c = 1; c < GodNumber; ++c)
	if(!game::GetGod(c)->GetKnown())
	  Possible[PossibleSize++] = c;

      if(PossibleSize)
	{
	  uchar NewKnownGod = Possible[RAND() % PossibleSize];
	  game::GetGod(NewKnownGod)->SetKnown(true);
	  ADD_MESSAGE("%s shares his knowledge of %s the %s.", GOD_NAME, game::GetGod(NewKnownGod)->Name().c_str(), game::GetGod(NewKnownGod)->Description().c_str());
	  return;
	}
    }

  ADD_MESSAGE("Nothing happens.");
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
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
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
	      ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
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
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
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
	      ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
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
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
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
	      ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
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
  potion* Reward = new potion(0, NO_MATERIALS);
  Reward->InitMaterials(MAKE_MATERIAL(GLASS), MAKE_MATERIAL(OMMEL_URINE));
  game::GetPlayer()->GetGiftStack()->AddItem(Reward);
  ADD_MESSAGE("%s drops on the ground.", Reward->CHAR_NAME(DEFINITE));
}

void nefas::PrayBadEffect()
{
  ADD_MESSAGE("A potion drops on your head and shatters into small bits.");
  game::GetPlayer()->ReceiveDamage(0, 2 + RAND() % 7, PHYSICAL_DAMAGE, HEAD);
  game::GetPlayer()->GetStackUnder()->AddItem(new brokenbottle);
  game::GetPlayer()->CheckDeath("killed while enjoying the company of " + Name(), 0);
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

  if(!(RAND() % 50))
    {
      ADD_MESSAGE("Five cans full of school food drop from somewhere above!");

      for(ushort c = 0; c < 5; ++c)
	{
	  can* Reward = new can(0, NO_MATERIALS);
	  Reward->InitMaterials(MAKE_MATERIAL(IRON), MAKE_MATERIAL(SCHOOL_FOOD));
	  game::GetPlayer()->GetGiftStack()->AddItem(Reward);
	}
    }
  else
    {
      for(ushort d = 0; d < 8; ++d)
	{
	  lsquare* Square = game::GetPlayer()->GetNeighbourLSquare(d);

	  if(Square && Square->GetCharacter() && Square->GetCharacter()->GetRelation(game::GetPlayer()) == HOSTILE)
	    {
	      ADD_MESSAGE("%s throws poison on %s!", GOD_NAME, Square->GetCharacter()->CHAR_NAME(DEFINITE));
	      Square->SpillFluid(game::GetPlayer(), MAKE_MATERIAL(POISON_LIQUID, 500), 100);
	    }
	}
    }

}

void scabies::PrayBadEffect()
{
  if(!(RAND() % 50))
    {
      ADD_MESSAGE("%s makes you eat a LOT of school food.", GOD_NAME);
      material* SchoolFood = MAKE_MATERIAL(SCHOOL_FOOD, 1000);
      SchoolFood->EatEffect(game::GetPlayer(), 1000);
      delete SchoolFood;
      ADD_MESSAGE("You feel your muscles softening terribly...");
      game::GetPlayer()->EditAttribute(ARM_STRENGTH, -1);
      game::GetPlayer()->EditAttribute(DEXTERITY, -1);
    }
  else
    {
      ADD_MESSAGE("%s unleashes all her fury upon you!", GOD_NAME);
      game::GetPlayer()->BeginTemporaryState(POISONED, 600 + RAND() % 400);
    }
}

void infuscor::PrayGoodEffect()
{
  ADD_MESSAGE("%s help you.", GOD_NAME);
  if(!game::GetPlayer()->StateIsActivated(ESP))
    {
      game::GetPlayer()->BeginTemporaryState(ESP, 10000 + RAND() % 10000);
      return;
    }

  if(!game::GetPlayer()->StateIsActivated(TELEPORT_CONTROL))
    {
      game::GetPlayer()->BeginTemporaryState(TELEPORT_CONTROL, 10000 + RAND() % 10000);
      return;
    }

  if(!game::GetPlayer()->StateIsActivated(POLYMORPH_CONTROL))
    {
      game::GetPlayer()->BeginTemporaryState(POLYMORPH_CONTROL, 10000 + RAND() % 10000);
      return;
    }    

  ADD_MESSAGE("Suddenly 5 scrolls appear almost under your feet.");
  for(ushort c = 0; c < 5; ++c)
    game::GetPlayer()->GetGiftStack()->AddItem(new scrollofteleportation);
}

void cruentus::PrayGoodEffect()
{
  if(!game::GetPlayer()->HasAllBodyParts())
    {
      bodypart* NewBodyPart = game::GetPlayer()->GenerateRandomBodyPart();
      NewBodyPart->ChangeMainMaterial(MAKE_MATERIAL(IRON));
      NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
      ADD_MESSAGE("You grow a new %s, which seems to be made of iron.", NewBodyPart->GetBodyPartName().c_str()); 
      return;
    }

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, game::GetCurrentLevel()->GetBorder(), game::GetPlayer()->GetPos(), 10);
  bool AudiencePresent = false;

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    {
      for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
	{
	  character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

	  if(Audience && Audience->CanBeSeenByPlayer() && !Audience->TemporaryStateIsActivated(PANIC) && game::GetPlayer()->GetRelation(Audience) == HOSTILE)
	    {
	      AudiencePresent = true;
	      break;
	    }
	}

      if(AudiencePresent)
	break;
    }

  if(AudiencePresent)
    {
      ADD_MESSAGE("The thundering voice of a godly battle drum shakes everything around you.");

      for(ushort x = Rect.X1; x <= Rect.X2; ++x)
	for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
	  {
	    character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

	    if(Audience && !Audience->TemporaryStateIsActivated(PANIC) && game::GetPlayer()->GetRelation(Audience) == HOSTILE && Audience->GetPanicLevel() >= RAND() % 100)
	      Audience->BeginTemporaryState(PANIC, 500 + RAND() % 500);
	  }

      return;
    }

  item* Weapon = game::GetPlayer()->GetMainWielded();

  if(!Weapon || !Weapon->IsWeapon(game::GetPlayer()))
    Weapon = game::GetPlayer()->GetSecondaryWielded();

  if(Weapon && Weapon->IsWeapon(game::GetPlayer()) && !(RAND() % 10))
    {
      ADD_MESSAGE("Your %s glows briefly red. It feels very warm now.", Weapon->CHAR_NAME(UNARTICLED));
      Weapon->EditEnchantment(1);
    }
  else if(RAND() & 3)
    {
      ADD_MESSAGE("Your inventory feels heavier.");
      potion* Bottle = new potion(0, NO_MATERIALS);
      Bottle->InitMaterials(MAKE_MATERIAL(GLASS), MAKE_MATERIAL(TROLL_BLOOD));
      game::GetPlayer()->GetGiftStack()->AddItem(Bottle);
    }
  else
    {
      ADD_MESSAGE("Cruentus recommends you to his master, Mortifer.");
      game::GetGod(MORTIFER)->AdjustRelation(100);
    }
}

void cruentus::PrayBadEffect()
{
  item* ToBe = game::GetPlayer()->GetMainWielded();

  if(ToBe)
    {
      if(!ToBe->IsDestroyable())
	{
	  ToBe = game::GetPlayer()->GetSecondaryWielded();

	  if(!ToBe || !ToBe->IsDestroyable())
	    ADD_MESSAGE("%s tries to destroy your %s, but fails.", GOD_NAME, game::GetPlayer()->GetMainWielded()->CHAR_NAME(UNARTICLED));
	}
    }
  else
    {
      ToBe = game::GetPlayer()->GetSecondaryWielded();

      if(ToBe && !ToBe->IsDestroyable())
	ADD_MESSAGE("%s tries to destroy your %s, but fails.", GOD_NAME, ToBe->CHAR_NAME(UNARTICLED));
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
      game::GetPlayer()->ReceiveDamage(0, 1 + RAND() % 30, PHYSICAL_DAMAGE, ALL, RAND() % 8);
      game::GetPlayer()->CheckDeath("destroyed by " + Name(), 0);
    }
}

std::string legifer::GetPriestMessage() const
{
  return "\"" + Name() + " is the Great Protector of all Law and Order. Prayed upon, He may burn thy enemies with the Fire of Justice, if thou art worthy.\"";
}

std::string dulcis::GetPriestMessage() const
{
  return "\"" + Name() + " is the Creator of everything we call Art and Beauty. When thou prayest for Her help, She may calm thy worst enemies with Her love. But beware! There are villains that may resist even Her call!\"";
}

std::string seges::GetPriestMessage() const
{
  return "\"" + Name() + " brings Life, Health and Nutrition to all who follow Her. When thou callest upon Her with an empty stomach, a miracle may indeed fill it.";
}

std::string sophos::GetPriestMessage() const
{
  return "\"The Wise bow before " + Name() + ", for He maketh the Universe as rational as it is. Those who follow Him are not bound to space and time, since knowledge controls them. This is why those chosen by Him may escape any danger with their wisdom. Alas, beware! Soon thou mayst find thyself in an even worse situation!\"";
}

void scabies::PlayerVomitedOnAltar()
{
  ADD_MESSAGE("%s feels that you are indeed her follower.", GOD_NAME); 
  AdjustRelation(1);
}

std::string valpurus::GetPriestMessage() const
{
  return "\"" + Name() + " the Great Frog is the highest of all gods. The Wise know that the world is really a square pancake which He carries on His back. This is why this Cathedral and the whole city of Attnam is dedicated to His worship.\" \"In thy prayers thou must understand He is a busy god who knows His importance. He will not help newbies. Thou shouldst only pray Him when He hath called thee a Champion!\"";
}
