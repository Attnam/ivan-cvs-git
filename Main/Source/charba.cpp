#include <cmath>

#include "charba.h"
#include "stack.h"
#include "error.h"
#include "message.h"
#include "itemde.h"
#include "lsquare.h"
#include "lterraba.h"
#include "dungeon.h"
#include "whandler.h"
#include "level.h"
#include "worldmap.h"
#include "hscore.h"
#include "feio.h"
#include "godba.h"
#include "felist.h"
#include "team.h"
#include "colorbit.h"
#include "config.h"
#include "femath.h"
#include "strover.h"

character::character(bool MakeBodyParts, bool SetStats, bool CreateEquipment, bool AllocBodyParts, bool AddToPool) : entity(AddToPool, true), Stack(new stack), Wielded(0), RegenerationCounter(0), NP(25000), AP(0), StrengthExperience(0), EnduranceExperience(0), AgilityExperience(0), PerceptionExperience(0), IsPlayer(false), State(0), Team(0), WayPoint(-1, -1), Money(0), HomeRoom(0)
{
  if(MakeBodyParts || SetStats || CreateEquipment || AllocBodyParts)
    ABORT("BOOO!");

  StateHandler[FAINTED] = &character::FaintHandler;
  StateHandler[CONSUMING] = &character::ConsumeHandler;
  StateHandler[POLYMORPHED] = &character::PolymorphHandler;
  StateHandler[RESTING] = &character::RestHandler;
  StateHandler[DIGGING] = &character::DigHandler;
  StateHandler[GOING] = &character::GoHandler;
  StateHandler[HASTE] = &character::HasteHandler;
  StateHandler[SLOW] = &character::SlowHandler;
}

character::~character()
{
  if(GetTeam())
    GetTeam()->Remove(GetTeamIterator());

  delete Stack;
  GetTorso()->SetExists(false);
}

void character::ReceiveSound(char* Pointer, short Success, float ScreamStrength)
{
  if(GetIsPlayer())
    ADD_MESSAGE(Pointer);

  ushort Damage = ushort(ScreamStrength * (1 + float(Success) / 100) / 50000);

  SetHP(HP - Damage);

  /* Behold! The Ultimate in gum solutions! */

  stack WieldedStack(GetLSquareUnder());

  if(GetWielded())
    {
      WieldedStack.FastAddItem(GetStack()->RemoveItem(GetStack()->SearchItem(GetWielded())));
    }

  stack TorsoStack(GetLSquareUnder());

  if(GetTorsoArmor())
    {
      TorsoStack.FastAddItem(GetStack()->RemoveItem(GetStack()->SearchItem(GetTorsoArmor())));
    }

  GetStack()->ReceiveSound(ScreamStrength);
  if(GetWielded())
    WieldedStack.ReceiveSound(ScreamStrength);
  if(GetTorsoArmor())
    TorsoStack.ReceiveSound(ScreamStrength);

  if(GetWielded())
    SetWielded(GetStack()->GetItem(GetStack()->AddItem(WieldedStack.RemoveItem(0))));
  if(GetTorsoArmor())
    SetTorsoArmor(GetStack()->GetItem(GetStack()->AddItem(TorsoStack.RemoveItem(0))));

  CheckDeath("killed by an Enner Beast's scream");
  CheckGearExistence();
}

void character::Hunger(ushort Turns) 
{
  switch(GetBurdenState())
    {
    case UNBURDENED:
      EditNP(-Turns);
      break;
    case BURDENED:
      EditNP(-2 * Turns);
      EditStrengthExperience(2 * Turns);
      EditAgilityExperience(-2 * Turns);
      break;
    case STRESSED:
    case OVERLOADED:
      EditNP(-4 * Turns);
      EditStrengthExperience(4 * Turns);
      EditAgilityExperience(-4 * Turns);
      break;
    }

  if(GetHungerState() == HUNGRY || GetHungerState() == VERYHUNGRY)
    EditStrengthExperience(-Turns);

  CheckStarvationDeath("starved to death");
}

bool character::Hit(character* Enemy)
{
  if(GetIsPlayer() && GetTeam()->GetRelation(Enemy->GetTeam()) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  Hostility(Enemy);
  if(GetBurdenState() == OVERLOADED)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You try to hit, but the weight of your items is too great for you to cause damage to %s.", Enemy->CNAME(DEFINITE));
      return true;
    }
  short Success = RAND() % 26 - RAND() % 26;

  switch(Enemy->TakeHit(this, Success)) //there's no breaks and there shouldn't be any
    {
    case HAS_HIT:
    case HAS_BLOCKED:
      if(GetWielded())
	GetWielded()->ReceiveHitEffect(Enemy, this);
    case HAS_DIED:
      EditStrengthExperience(50);
      if(GetWielded()) 
	if(GetWielded()->ImpactDamage(GetStrength() / 2, GetLSquareUnder()->CanBeSeen(), GetStack()))
	  SetWielded(GetStack()->GetItem(GetStack()->GetItems() - 1));
    case HAS_DODGED:
      EditAgilityExperience(25);
    }

  EditNP(-50);

  return true;
}

ushort character::CalculateArmorModifier() const
{
  return 100;
}

uchar character::TakeHit(character* Enemy, short Success)
{
  DeActivateVoluntaryStates(Enemy->Name(DEFINITE) + " seems to be hostile");

  if(!(RAND() % Enemy->CriticalModifier()))
    {
      ushort Damage = ushort(Enemy->GetAttackStrength() * Enemy->GetStrength() * (1 + float(Success) / 100) * CalculateArmorModifier() / 2500000) + (RAND() % 3 ? 2 : 1);

      Enemy->AddHitMessage(this,true);

      if(game::GetWizardMode() && GetLSquareUnder()->CanBeSeen(true))
	ADD_MESSAGE("(damage: %d)", Damage);

      SetHP(GetHP() - Damage);

      if(GetTorsoArmor() && RAND() % 2 && GetTorsoArmor()->ImpactDamage(Enemy->GetStrength(), GetLSquareUnder()->CanBeSeen(), GetStack()))
	SetTorsoArmor(GetStack()->GetItem(GetStack()->GetItems() - 1));

      SpillBlood(3 + RAND() % 3);

      DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
      {
	vector2d Where(DoX, DoY);

	if(game::GetCurrentLevel()->GetLSquare(Where)->GetOTerrain()->GetIsWalkable()) 
	  SpillBlood(2 + RAND() % 2, Where);
      });

      if(CheckDeath(std::string("killed by ") + Enemy->Name(INDEFINITE), Enemy->GetIsPlayer()))
	return HAS_DIED;

      return HAS_HIT;
    }

  if(RAND() % ushort(100 + Enemy->GetToHitValue() / GetDodgeValue() * (100 + Success)) >= 100)
    {
      ushort Damage = ushort(Enemy->GetAttackStrength() * Enemy->GetStrength() * (1 + float(Success) / 100) * CalculateArmorModifier() / 5000000) + (RAND() % 3 ? 1 : 0);

      if(!Damage)
	{
	  Enemy->AddBlockMessage(this);

	  EditStrengthExperience(50);
	  EditEnduranceExperience(50);

	  return HAS_BLOCKED;
	}
      else
	{
	  Enemy->AddHitMessage(this);

	  if(game::GetWizardMode() && GetLSquareUnder()->CanBeSeen(true))
	    ADD_MESSAGE("(damage: %d)", Damage);

	  SetHP(GetHP() - Damage);

	  if(CheckDeath(std::string("killed by ") + Enemy->Name(INDEFINITE), Enemy->GetIsPlayer()))
	    return HAS_DIED;

	  return HAS_HIT;
	}
    }
  else
    {
      Enemy->AddDodgeMessage(this);
      EditAgilityExperience(50);
      EditPerceptionExperience(25);
      return HAS_DODGED;
    }
}

void character::Be()
{
  if(game::GetPlayerBackup() != this)
    {		
      if(game::GetIsLoading())
	{
	  if(!GetIsPlayer())
	    return;
	  else
	    game::SetIsLoading(false);
	}
      else
	{
	  ApplyExperience();

	  switch(GetBurdenState())
	    {
	    case UNBURDENED:
	      EditAP(long((100 + (GetAgility() >> 1)) * GetAPStateMultiplier()));
	      break;
	    case BURDENED:
	      EditAP(long((75 + (GetAgility() >> 1) - (GetAgility() >> 2)) * GetAPStateMultiplier()));
	      break;
	    case STRESSED:
	    case OVERLOADED:
	      EditAP(long((50 + (GetAgility() >> 2)) * GetAPStateMultiplier()));
	      break;
	    }

	  for(ushort c = 0; c < STATES; ++c)
	    if(StateIsActivated(c))
	      (this->*StateHandler[c])();

	  if(!GetExists())
	    return;

	  if(GetHP() < GetMaxHP() / 3)
	    SpillBlood(RAND() % 2);

	  if(GetIsPlayer() && GetHungerState() == VERYHUNGRY && !(RAND() % 50) && !StateIsActivated(FAINTED) && !StateIsActivated(CONSUMING))
	    {
	      DeActivateVoluntaryStates();
	      Faint();
	    }
	}

      if(GetAP() >= 0)
	StateAutoDeactivation();

      if(GetAP() >= 0)
	{
	  if(GetIsPlayer())
	    {
	      static ushort Timer = 0;

	      if(configuration::GetAutosaveInterval() && CanMove() && ++Timer >= configuration::GetAutosaveInterval())
		{
		  game::Save(game::GetAutoSaveFileName().c_str());
		  Timer = 0;
		}

	      if(CanMove())
		GetPlayerCommand();
	      else
		{
		  game::DrawEverything(false);

		  if(READKEY())
		    DeActivateVoluntaryStates();
		}
	    }
	  else
	    {
	      if(CanMove() && !game::GetInWilderness())
		GetAICommand();
	    }

	  EditAP(-1000);
	}

      CharacterSpeciality();
      Regenerate();

      if(GetIsPlayer())
	{
	  if(!StateIsActivated(CONSUMING))
	    Hunger();
	}
    }
}

bool character::GoUp()
{
  if(GetSquareUnder()->GetOTerrain()->GoUp(this))
    {
      EditStrengthExperience(50);
      EditNP(-20);
      EditAP(-1000);
      return true;
    }
  else
    return false;
}

bool character::GoDown()
{
  if(GetSquareUnder()->GetOTerrain()->GoDown(this))
    {
      EditAgilityExperience(25);
      EditNP(-10);
      EditAP(-1000);
      return true;
    }
  else
    return false;
}

bool character::Open()
{
  if(CanOpenDoors())
    {
      if(GetStack()->GetItems())
	ADD_MESSAGE("Where is this famous door you wish to open?  [press a direction key, space or i]");
      else
	ADD_MESSAGE("Where is this famous door you wish to open?  [press a direction key or space]");

      DRAW_MESSAGES();
      EMPTY_MESSAGES();
      graphics::BlitDBToScreen();

      while(true)
	{
	  int Key = GETKEY();

	  if(Key == 'i') 
	    return OpenItem();

	  if(Key == 0x1B || Key == ' ')
	    return false;

	  for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	    if(Key == game::GetMoveCommandKey(c))
	      return OpenPos(GetPos() + game::GetMoveVector(c));
	}
    }
  else
    ADD_MESSAGE("This monster type cannot open doors.");

  return false;
}

bool character::Close()
{
  if(CanOpenDoors())
    {
      ADD_MESSAGE("Where is this door you wish to close? [press a direction key or space]");
      DRAW_MESSAGES();
      EMPTY_MESSAGES();
      graphics::BlitDBToScreen();

      while(true)
	{
	  int Key = GETKEY();

	  if(Key == 0x1B || Key == ' ')
	    return false;

	  for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	    if(Key == game::GetMoveCommandKey(c))
	      if(game::GetCurrentLevel()->GetLSquare(GetPos() + game::GetMoveVector(c))->Close(this))
		{
		  EditAgilityExperience(25);
		  EditNP(-10);
		  EditAP(-500);
		  return true;
		}
	      else
		return false;
	}
    }
  else
    ADD_MESSAGE("This monster type cannot close doors.");
	
  return false;
}

bool character::Drop()
{
  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to drop!");
      return false;
    }

  ushort Index = GetStack()->DrawContents(this, "What do you want to drop?");

  if(Index < GetStack()->GetItems() && GetStack()->GetItem(Index))
    if(GetStack()->GetItem(Index) == GetWielded())
      ADD_MESSAGE("You can't drop something you wield!");
    else if(GetStack()->GetItem(Index) == GetTorsoArmor())
      ADD_MESSAGE("You can't drop something you wear!");
    else
      if(!GetLSquareUnder()->GetRoom() || (GetLSquareUnder()->GetRoom() && GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->DropItem(this, GetStack()->GetItem(Index))))
	{
	  GetStack()->MoveItem(Index, GetLSquareUnder()->GetStack());

	  return true;
	}

  return false;
}

bool character::Consume()
{
  if(!game::GetInWilderness() && GetLSquareUnder()->GetOLTerrain()->HasConsumeEffect())
    {
      if(GetLSquareUnder()->GetOLTerrain()->Consume(this))
	return true;
    }

  if(!game::GetInWilderness() && GetLSquareUnder()->GetStack()->ConsumableItems(this) && game::BoolQuestion("Do you wish to consume one of the items lying on the ground? [y/N]"))
    {
      ushort Index = GetLSquareUnder()->GetStack()->DrawConsumableContents(this, "What do you wish to consume?");

      if(Index < GetLSquareUnder()->GetStack()->GetItems())
	{
	  if(GetLSquareUnder()->GetRoom() && !GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->ConsumeItem(this, GetLSquareUnder()->GetStack()->GetItem(Index)))
	    return false;

	  if(CheckBulimia() && !game::BoolQuestion("You think your stomach will burst if you eat anything more. Force it down? [y/N]"))
	    return false;

	  if(ConsumeItem(GetLSquareUnder()->GetStack()->GetItem(Index), GetLSquareUnder()->GetStack()))
	    {
	      ReceiveBulimiaDamage();
	      return true;
	    }
	  else
	    return false;
	}
      else
	return false;
    }

  if(GetStack()->ConsumableItems(this))
    {
      ushort Index = GetStack()->DrawConsumableContents(this, "What do you wish to consume?");

      if(Index < GetStack()->GetItems())
	{
	  if(!CheckIfConsumable(Index))
	    {
	      ADD_MESSAGE("You can't consume items in your gear!");
	      return false;
	    }

	  if(CheckBulimia() && !game::BoolQuestion("You think your stomach will burst if you eat anything more. Force it down? [y/N]"))
	    return false;

	  if(ConsumeItem(GetStack()->GetItem(Index), GetStack()))
	    {
	      ReceiveBulimiaDamage();
	      return true;
	    }
	  else
	    return false;
	}
      else
	return false;
    }

  if(GetIsPlayer() && (game::GetInWilderness() || !GetLSquareUnder()->GetStack()->ConsumableItems(this)) && !GetStack()->ConsumableItems(this))
    ADD_MESSAGE("You have nothing to consume!");

  return false;
}

bool character::CheckBulimia() const
{
  return GetHungerState() == BLOATED;
}

void character::ReceiveBulimiaDamage()
{
  if(GetHungerState() == BLOATED)
    {
      ADD_MESSAGE("Urgh... Your stomach hurts.");

      SetHP(GetHP() - (GetNP() / 10 - (GetSize() << 5)) / 50);

      CheckDeath("died of bulimia");
    }
}

bool character::CheckIfConsumable(ushort Index) const
{
  return (GetTorsoArmor() != GetStack()->GetItem(Index) && GetWielded() != GetStack()->GetItem(Index));
}

bool character::ConsumeItem(item* ToBeEaten, stack* ItemsStack)
{
  if(ConsumeItemType(ToBeEaten->GetConsumeType()))
    {
      if((ToBeEaten = ToBeEaten->PrepareForConsuming(this, ItemsStack)))
	{
	  SetConsumingCurrently(ToBeEaten);
	  ActivateState(CONSUMING);
	  StateCounter[CONSUMING] = 0;
	  return true;
	}
    }
  else
    if(GetIsPlayer())
      ADD_MESSAGE("You can't consume this.");

  return false;
}

bool character::ConsumeItemType(uchar Type) const
{
  switch(Type)
    {
    case LIQUID:
      return true;
      break;	
    case ODD:
      return false;
      break;
    case FRUIT:
      return true;
      break;
    case MEAT:
      return true;
      break;
    case SPOILED:
      return true;
      break;
    case HARD:
      return false;
      break;
    case SCHOOLFOOD:
      return true;
      break;
    case CONTAINER:
      return false;
      break;
    case BONE:
      return false;
      break;
    default:
      ADD_MESSAGE("ERRRRORRRRRR in Consumeitemtype."); //All hail SpykoS! He is the author of this file, and his might is over that of PMGR!!!
      return false;
    }
}

void character::Move(vector2d MoveTo, bool TeleportMove)
{
  if(GetBurdenState() || TeleportMove)
    {
      game::GetCurrentArea()->MoveCharacter(GetPos(), MoveTo);

      if(GetIsPlayer())
	ShowNewPosInfo();

      if(!TeleportMove)
	{
	  EditAP(-GetSquareUnder()->GetEntryAPRequirement() + 1000);
	  EditNP(-10);
	  EditAgilityExperience(5);
	}
    }
  else
    if(GetIsPlayer())
      ADD_MESSAGE("You try to use your claws to crawl forward. But your load is too heavy.");
}

void character::DrawToTileBuffer() const
{
  //Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
  GetTorso()->DrawToTileBuffer();
}

bool character::Wield()
{
  if(!CanWield())
    {
      ADD_MESSAGE("This monster can not wield anything.");
      return false;
    }

  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to wield!");
      return false;
    }

  ushort Index = GetStack()->DrawContents(this, "What do you want to wield? or press '-' for hands");

  if(Index == 0xFFFE)
    SetWielded(0);
  else
    if(Index < GetStack()->GetItems())
      if(GetStack()->GetItem(Index) != GetTorsoArmor())
	SetWielded(GetStack()->GetItem(Index));
      else
	{
	  ADD_MESSAGE("You can't wield something that you wear!");
	  return false;
	}
    else
      return false;

  return true;
}

void character::GetAICommand()	// Freedom is slavery. Love is hate. War is peace.
				// Shouldn't it be "Ignorance is strength", not "Love is hate"?
{
  SeekLeader();

  if(CheckForEnemies(true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(CheckForDoors())
    return;

  MoveRandomly();
}

bool character::MoveTowards(vector2d TPos)
{
  vector2d MoveTo[3];

  if(TPos.X < GetPos().X)
    {
      if(TPos.Y < GetPos().Y)
	{
	  MoveTo[0] = vector2d(-1, -1);
	  MoveTo[1] = vector2d(-1,  0);
	  MoveTo[2] = vector2d( 0, -1);
	}

      if(TPos.Y == GetPos().Y)
	{
	  MoveTo[0] = vector2d(-1,  0);
	  MoveTo[1] = vector2d(-1, -1);
	  MoveTo[2] = vector2d(-1,  1);
	}

      if(TPos.Y > GetPos().Y)
	{
	  MoveTo[0] = vector2d(-1, 1);
	  MoveTo[1] = vector2d(-1, 0);
	  MoveTo[2] = vector2d( 0, 1);
	}
    }

  if(TPos.X == GetPos().X)
    {
      if(TPos.Y < GetPos().Y)
	{
	  MoveTo[0] = vector2d( 0, -1);
	  MoveTo[1] = vector2d(-1, -1);
	  MoveTo[2] = vector2d( 1, -1);
	}

      if(TPos.Y == GetPos().Y)
	return false;

      if(TPos.Y > GetPos().Y)
	{
	  MoveTo[0] = vector2d( 0, 1);
	  MoveTo[1] = vector2d(-1, 1);
	  MoveTo[2] = vector2d( 1, 1);
	}
    }

  if(TPos.X > GetPos().X)
    {
      if(TPos.Y < GetPos().Y)
	{
	  MoveTo[0] = vector2d(1, -1);
	  MoveTo[1] = vector2d(1,  0);
	  MoveTo[2] = vector2d(0, -1);
	}

      if(TPos.Y == GetPos().Y)
	{
	  MoveTo[0] = vector2d(1,  0);
	  MoveTo[1] = vector2d(1, -1);
	  MoveTo[2] = vector2d(1,  1);
	}

      if(TPos.Y > GetPos().Y)
	{
	  MoveTo[0] = vector2d(1, 1);
	  MoveTo[1] = vector2d(1, 0);
	  MoveTo[2] = vector2d(0, 1);
	}
    }

  if(TryMove(GetPos() + MoveTo[0])) return true;

  if(RAND() % 2)
    {
      if(TryMove(GetPos() + MoveTo[1])) return true;
      if(TryMove(GetPos() + MoveTo[2])) return true;
    }
  else
    {
      if(TryMove(GetPos() + MoveTo[2])) return true;
      if(TryMove(GetPos() + MoveTo[1])) return true;
    }

  return false;
}

bool character::TryMove(vector2d MoveTo, bool DisplaceAllowed)
{
  if(!game::GetInWilderness())
    if(MoveTo.X >= 0 && MoveTo.Y >= 0 && MoveTo.X < game::GetCurrentLevel()->GetXSize() && MoveTo.Y < game::GetCurrentLevel()->GetYSize())
      {
	character* Character;

	if((Character = game::GetCurrentLevel()->GetLSquare(MoveTo)->GetCharacter()))
	  if(GetIsPlayer())
	    if(GetTeam() != Character->GetTeam())
	      return Hit(Character);
	    else
	      if(DisplaceAllowed)
		{
		  Displace(Character);
		  return true;
		}
	      else
		return false;
	  else
	    if(GetTeam()->GetRelation(Character->GetTeam()) == HOSTILE)
	      return Hit(Character);
	    else
	      if(GetTeam() == Character->GetTeam() && DisplaceAllowed)
		return Displace(Character);
	      else
		return false;
	else
	  if(game::GetCurrentLevel()->GetLSquare(MoveTo)->GetIsWalkable(this) || (game::GetGoThroughWallsCheat() && GetIsPlayer()))
	    {
	      Move(MoveTo);
	      return true;
	    }
	  else
	    if(GetIsPlayer() && game::GetCurrentLevel()->GetLSquare(MoveTo)->GetOLTerrain()->CanBeOpened())
	      {
		if(CanOpenDoors())
		  {
		    if(game::GetCurrentLevel()->GetLSquare(MoveTo)->GetOLTerrain()->GetIsLocked())
		      {
			ADD_MESSAGE("The door is locked.");
			return false;
		      }
		    else
		      {
			if(game::BoolQuestion("Do you want to open this door? [y/N]", false, game::GetMoveCommandKey(game::GetPlayer()->GetPos(), MoveTo)))
			  {
			    OpenPos(MoveTo);
			    return true;
			  }
			else
			  return false;
		      }
		  }
		else
		  {
		    ADD_MESSAGE("This monster type cannot open doors.");
		    return false;
		  }
	      }
	    else
	      return false;
      }
    else
      {
	if(GetIsPlayer() && game::GetCurrentLevel()->GetOnGround() && game::BoolQuestion("Do you want to leave " + game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrent()) + "? [y/N]"))
	  {
	    if(HasPetrussNut() && !HasGoldenEagleShirt())
	      {
		iosystem::TextScreen("An undead and sinister voice greets you as you leave the city behind:\n\n\"MoRtAl! ThOu HaSt SlAuGtHeReD pErTtU aNd PlEaSeD mE!\nfRoM tHiS dAy On, YoU aRe ThE dEaReSt SeRvAnT oF aLl EvIl!\"\n\nYou are victorious!");
		game::RemoveSaves();

		if(!game::GetWizardMode())
		  {
		    game::GetPlayer()->AddScoreEntry("killed Petrus and became the Avatar of Chaos", 3, false);
		    highscore HScore;
		    HScore.Draw();
		  }

		game::Quit();
		return true;
	      }

	    std::vector<character*> TempPlayerGroup;

	    if(!GetLSquareUnder()->GetLevelUnder()->CollectCreatures(TempPlayerGroup, this, false))
	      return false;

	    game::GetCurrentArea()->RemoveCharacter(GetPos());
	    game::GetCurrentDungeon()->SaveLevel();
	    game::LoadWorldMap();
	    game::GetWorldMap()->GetPlayerGroup().swap(TempPlayerGroup);
	    game::SetInWilderness(true);
	    game::GetCurrentArea()->AddCharacter(game::GetCurrentDungeon()->GetWorldMapPos(), this);
	    game::SendLOSUpdateRequest();
	    game::UpdateCamera();
	    game::GetCurrentArea()->UpdateLOS();
	    if(configuration::GetAutosaveInterval())
	      game::Save(game::GetAutoSaveFileName().c_str());
	    return true;
	  }

	return false;
      }
  else
    if(MoveTo.X >= 0 && MoveTo.Y >= 0 && MoveTo.X < game::GetWorldMap()->GetXSize() && MoveTo.Y < game::GetWorldMap()->GetYSize())
      if(game::GetCurrentArea()->GetSquare(MoveTo)->GetIsWalkable(this) || game::GetGoThroughWallsCheat())
	{
	  Move(MoveTo);
	  return true;
	}
      else
	return false;
    else
      return false;
	
}

bool character::ShowInventory()
{
  GetStack()->DrawContents(this, "Character's Inventory, press ESC to exit");
  return false;
}

bool character::PickUp()
{
  bool ToBeReturned = false;

  if(GetLSquareUnder()->GetStack()->GetItems() > 0)
    if(GetLSquareUnder()->GetStack()->GetItems() > 1)
      {
	ushort Index;

	for(;;)
	  {
	    Index = GetLSquareUnder()->GetStack()->DrawContents(this, "What do you want to pick up?");

	    if(Index < GetLSquareUnder()->GetStack()->GetItems())
	      if(GetLSquareUnder()->GetStack()->GetItem(Index))
		if(!GetLSquareUnder()->GetRoom() || (GetLSquareUnder()->GetRoom() && GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, GetLSquareUnder()->GetStack()->GetItem(Index))))
		  {
		    ADD_MESSAGE("%s picked up.", GetLSquareUnder()->GetStack()->GetItem(Index)->CNAME(INDEFINITE));
		    GetLSquareUnder()->GetStack()->MoveItem(Index, GetStack())->CheckPickUpEffect(this);
		    ToBeReturned = true;
		  }

	    if(Index & 0x8000 || !GetLSquareUnder()->GetStack()->GetItems())
	      break;

	    game::DrawEverythingNoBlit();
	  }
      }
    else
      {
	if(!GetLSquareUnder()->GetRoom() || (GetLSquareUnder()->GetRoom() && GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, GetLSquareUnder()->GetStack()->GetItem(0))))
	  {
	    ADD_MESSAGE("%s picked up.", GetLSquareUnder()->GetStack()->GetItem(0)->CNAME(INDEFINITE));
	    GetLSquareUnder()->GetStack()->MoveItem(0, GetStack())->CheckPickUpEffect(this);
	    return true;
	  }
      }
  else
    ADD_MESSAGE("There is nothing here to pick up, %s!", game::Insult());

  return ToBeReturned;
}

bool character::Quit()
{
  if(game::BoolQuestion("Thine Holy Quest is not yet compeleted! Really quit? [y/N]"))
    {
      game::RemoveSaves();

      if(!game::GetWizardMode())
	{
	  AddScoreEntry("cowardly quit the game", 0.75f);
	  highscore HScore;
	  HScore.Draw();
	}

      game::Quit();
      return true;
    }
  else
    return false;
}

void character::CreateCorpse()
{
  /*corpse* Corpse = new corpse(GetMaterial(0));
    Corpse->SetBloodColor(GetBloodColor());
    GetLSquareUnder()->GetStack()->AddItem(Corpse);
    PreserveMaterial(0);*/
}

void character::Die(bool ForceMsg)
{
  // Note for programmers: This function MUST NOT delete any objects!

  if(!GetExists())
    return;

  if(GetIsPlayer())
    {
      ADD_MESSAGE("You die.");

      if(game::GetWizardMode())
	{
	  game::DrawEverything(false);

	  if(!game::BoolQuestion("Do you want to do this, cheater? [y/n]", 2))
	    {
	      SetHP(GetMaxHP());
	      SetNP(10000);
	      return;
	    }
	}

      game::RemoveSaves();
    }
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE(DeathMessage().c_str());
    else
      if(ForceMsg)
	ADD_MESSAGE("You sense the death of something.");

  SetExists(false);

  if(HomeRoom && GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetMaster() == this)
    GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->SetMaster(0);

  GetSquareUnder()->RemoveCharacter();

  if(!game::GetInWilderness())
    CreateCorpse();

  if(GetIsPlayer())
    {
      if(!game::GetInWilderness())
	GetLSquareUnder()->SetTemporaryEmitation(GetEmitation());

      game::DrawEverything(false);

      if(!game::GetInWilderness())
	GetLSquareUnder()->SetTemporaryEmitation(0);

      if(GetStack()->GetItems())
	if(game::BoolQuestion("Do you want to see your inventory? [y/n]", 2))
	  GetStack()->DrawContents(this, "Your inventory");

      if(game::BoolQuestion("Do you want to see your message history? [y/n]", 2))
	DrawMessageHistory();
    }

  if(!game::GetInWilderness())
    {
      lsquare* Square = GetLSquareUnder();
      SetSquareUnder(0); // prevents light optimization

      while(GetStack()->GetItems())
	GetStack()->MoveItem(0, Square->GetStack());

      SetSquareUnder(Square);
    }
  else
    while(GetStack()->GetItems())
      {
	GetStack()->GetItem(0)->SetExists(false);
	GetStack()->RemoveItem(0);
      }

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(0);

  SetExists(false);

  if(GetIsPlayer())
    {
      iosystem::TextScreen("Unfortunately thee died during thine journey. The High Priest is not happy.");

      if(!game::GetWizardMode())
	{
	  highscore HScore;
	  HScore.Draw();
	}

      game::Quit();
    }
}

bool character::OpenItem()
{
  ushort Index = Stack->DrawContents(this, "What do you want to open?");

  if(Index < GetStack()->GetItems())
    if(GetStack()->GetItem(Index)->TryToOpen(this, Stack))
      {
	EditAgilityExperience(25);
	EditNP(-10);
	EditAP(-500);
	return true;
      }
    else
      {
	ADD_MESSAGE("You can't open %s.", GetStack()->GetItem(Index)->CNAME(DEFINITE));
	return false;
      }

  return false;
}

void character::Regenerate(ushort Turns)
{
  ulong RegenerationBonus = GetEndurance() * Turns;

  if(StateIsActivated(RESTING))
    RegenerationBonus *= GetSquareUnder()->RestModifier();

  EditRegenerationCounter(RegenerationBonus);

  while(GetRegenerationCounter() > 1000)
    {
      if(GetHP() < GetMaxHP())
	{
	  SetHP(GetHP() + 1);
	  EditEnduranceExperience(100);
	}

      EditRegenerationCounter(-1000);
    }
}

bool character::WearArmor()
{
  ADD_MESSAGE("This monster type can't use armor.");
  return false;
}

void character::AddBlockMessage(character* Enemy) const
{
  std::string ThisDescription = GetLSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something";
  std::string EnemyDescription = Enemy->GetLSquareUnder()->CanBeSeen() ? Enemy->CNAME(DEFINITE) : "something";

  if(Enemy->GetIsPlayer())
    ADD_MESSAGE("%s fails to hurt you!", ThisDescription.c_str());
  else
    if(GetIsPlayer())
      ADD_MESSAGE("You fail to hurt %s!", EnemyDescription.c_str());
    else
      if(GetLSquareUnder()->CanBeSeen() || Enemy->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s fails to hurt %s!", ThisDescription.c_str(), EnemyDescription.c_str());
}

void character::AddDodgeMessage(character* Enemy) const
{
  std::string ThisDescription = GetLSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something";
  std::string EnemyDescription = Enemy->GetLSquareUnder()->CanBeSeen() ? Enemy->CNAME(DEFINITE) : "something";

  if(Enemy->GetIsPlayer())
    ADD_MESSAGE("You dodge %s!", ThisDescription.c_str());
  else
    if(GetIsPlayer())
      ADD_MESSAGE("%s dodges you!", EnemyDescription.c_str());
    else
      if(GetLSquareUnder()->CanBeSeen() || Enemy->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s dodges %s!", EnemyDescription.c_str(), ThisDescription.c_str());
}

void character::AddHitMessage(character* Enemy, const bool Critical) const
{
  std::string ThisDescription = GetLSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something";
  std::string EnemyDescription = Enemy->GetLSquareUnder()->CanBeSeen() ? Enemy->CNAME(DEFINITE) : "something";

  if(Enemy->GetIsPlayer())
    if(GetWielded() && GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s %s you with %s %s!", ThisDescription.c_str(), ThirdPersonWeaponHitVerb(Critical).c_str(), game::PossessivePronoun(GetSex()), Wielded->CNAME(0));
    else
      ADD_MESSAGE("%s %s you!", ThisDescription.c_str(), ThirdPersonMeleeHitVerb(Critical).c_str());
  else
    if(GetIsPlayer())
      ADD_MESSAGE("You %s %s!", FirstPersonHitVerb(Enemy, Critical).c_str(), EnemyDescription.c_str());
    else
      if(GetLSquareUnder()->CanBeSeen() || Enemy->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s %s %s!", ThisDescription.c_str(), AICombatHitVerb(Enemy, Critical).c_str(), EnemyDescription.c_str());
}

void character::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s %s.", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something", TalkVerb().c_str());
}

bool character::Talk()
{
  int k;
  ADD_MESSAGE("To whom do you wish to talk to? [press a direction key or space]");
  DRAW_MESSAGES();
  EMPTY_MESSAGES();
  graphics::BlitDBToScreen();
  bool CorrectKey = false;

  while(!CorrectKey)
    {
      k = GETKEY();

      if(k == 0x1B || k == ' ')
	CorrectKey = true;

      for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	if(k == game::GetMoveCommandKey(c))
	  {
	    character* Who = game::GetCurrentLevel()->GetLSquare(GetPos() + game::GetMoveVector(c))->GetCharacter();
	    if(Who)
	      {
		if(Who->GetTeam()->GetRelation(GetTeam()) != HOSTILE)
		  Who->EditAP(-1000);

		Who->BeTalkedTo(this);
		return true;
	      }
	    else
	      {
		ADD_MESSAGE("You get no response.");
		return false;
	      }

	    CorrectKey = true;
	  }
    }

  return false;
}

bool character::NOP()
{
  EditAgilityExperience(-10);
  return true;
}

void character::ApplyExperience()
{
  if(GetStrengthExperience() > pow(1.18, long(GetStrength())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You feel stronger!");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks stronger.", CNAME(DEFINITE));

      SetStrength(GetStrength() + 1);
      SetStrengthExperience(0);
    }

  if(GetStrengthExperience() < -pow(1.18, long(100 - GetStrength())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You collapse under your load.");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks weaker.", CNAME(DEFINITE));

      SetStrength(GetStrength() - 1);
      SetStrengthExperience(0);
    }

  if(GetEnduranceExperience() > pow(1.18, long(GetEndurance())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You feel Valpurus's toughness around you!");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks tougher.", CNAME(DEFINITE));

      SetEndurance(GetEndurance() + 1);
      SetEnduranceExperience(0);
    }

  if(GetEnduranceExperience() < -pow(1.18, long(100 - GetEndurance())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You feel less healthy.");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks less tough.", CNAME(DEFINITE));

      SetEndurance(GetEndurance() - 1);
      SetEnduranceExperience(0);
    }

  if(GetAgilityExperience() > pow(1.18, long(GetAgility())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Your agility challenges even Valpurus's angels!");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks faster.", CNAME(DEFINITE));

      SetAgility(GetAgility() + 1);
      SetAgilityExperience(0);
    }

  if(GetAgilityExperience() < -pow(1.18, long(100 - GetAgility())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You seem as fast as a flat mommo.");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks slower.", CNAME(DEFINITE));

      SetAgility(GetAgility() - 1);
      SetAgilityExperience(0);
    }

  if(GetPerceptionExperience() > pow(1.18, long(GetPerception())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Your don't feel as guru anymore.");

      SetPerception(GetPerception() + 1);
      SetPerceptionExperience(0);
    }

  if(GetPerceptionExperience() < -pow(1.18, long(100 - GetPerception())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You feel very guru.");

      SetPerception(GetPerception() - 1);
      SetPerceptionExperience(0);
      game::GetGod(1)->AdjustRelation(100);
    }
}

bool character::HasHeadOfElpuri() const
{
  for(ushort c = 0; c < GetStack()->GetItems(); ++c)
    if(GetStack()->GetItem(c)->IsHeadOfElpuri())
      return true;

  return false;
}

bool character::HasPetrussNut() const
{
  for(ushort c = 0; c < GetStack()->GetItems(); ++c)
    if(GetStack()->GetItem(c)->IsPetrussNut())
      return true;

  return false;
}

bool character::HasGoldenEagleShirt() const
{
  for(ushort c = 0; c < GetStack()->GetItems(); ++c)
    if(GetStack()->GetItem(c)->IsGoldenEagleShirt())
      return true;

  return false;
}

bool character::Save()
{
  if(game::BoolQuestion("Dost thee truly wish to save and flee? [y/N]"))
    {
      game::Save();
      game::RemoveSaves(false);
      game::Quit();
      return true;
    }
  else
    return false;
}

bool character::Read()
{
  if(!CanRead())
    {
      ADD_MESSAGE("You can't read.");
      return false;
    }

  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to read!");
      return false;
    }

  ushort Index = GetStack()->DrawContents(this, "What do you want to read?");

  if(Index < GetStack()->GetItems())
    return ReadItem(Index, GetStack());
  else
    return false;
}

bool character::ReadItem(int ToBeRead, stack* ItemsStack)
{
  if(ItemsStack->GetItem(ToBeRead) == GetTorsoArmor())
    {
      ADD_MESSAGE("You can't read items that you wear!");
      return false;
    }

  if(ItemsStack->GetItem(ToBeRead)->CanBeRead(this))
    if(GetLSquareUnder()->GetLuminance() >= LIGHT_BORDER || game::GetSeeWholeMapCheat())
      {
	bool LackOfLight = GetLSquareUnder()->GetRawLuminance() < 225 ? true : false;

	if(ItemsStack->GetItem(ToBeRead)->Read(this))
	  {
	    ItemsStack->RemoveItem(ToBeRead)->SetExists(false);

	    if(LackOfLight)
	      SetPerceptionExperience(-25);
	  }

	CheckGearExistence();
	return true;
      }
    else
      {
	ADD_MESSAGE("It's too dark in here to read.");
	return false;
      }
  else
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You can't read this.");

      return false;
    }
}

uchar character::GetBurdenState(ulong Mass) const
{
  ulong SumOfMasses;
  if(!Mass)
    SumOfMasses = GetStack()->SumOfMasses();
  else
    SumOfMasses = Mass;
  if(SumOfMasses > ulong(7000 * GetStrength()))
    return OVERLOADED;
  if(SumOfMasses > ulong(5000 * GetStrength()))
    return STRESSED;
  if(SumOfMasses > ulong(2500 * GetStrength()))
    return BURDENED;
  return UNBURDENED;
}

bool character::Dip()
{
  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to dip!");
      return false;
    }

  ushort What = GetStack()->DrawContents(this, "What do you want to dip?");

  if(What < GetStack()->GetItems())
    {
      if(GetStack()->GetItem(What)->CanBeDipped())
	{
	  game::DrawEverythingNoBlit();
	  if(game::BoolQuestion("Do you wish to dip in a nearby square? [y,n]", 2))
	    {
	      vector2d VectorDir;
	      std::string Question = std::string("Where do you want to dip ") + GetStack()->GetItem(What)->Name(DEFINITE) + std::string("? [press a cursor key or . for the current square]");
	      uchar Dir = game::DirectionQuestion(Question, 0,true,true);

	      if(Dir == '.')
		VectorDir = vector2d(0,0);
	      else
		VectorDir = game::GetMoveVector(Dir);
	      
	      return game::GetCurrentLevel()->GetLSquare(GetPos() + VectorDir)->ReceiveDip(GetStack()->GetItem(What), this);
	    }
	  else
	    {
	      ushort To = GetStack()->DrawContents(this, "Into what do you wish to dip it?");

	      if(To < GetStack()->GetItems() && GetStack()->GetItem(To) && GetStack()->GetItem(What) != GetStack()->GetItem(To))
		if(GetStack()->GetItem(To)->CanBeDippedInto(GetStack()->GetItem(What)))
		  {
		    GetStack()->GetItem(What)->DipInto(GetStack()->GetItem(To));
		    return true;
		  }
	    }

	  ADD_MESSAGE("Invalid selection!");
	}
    }

  return false;
}

void character::Save(outputfile& SaveFile) const
{
  type::Save(SaveFile);
  entity::Save(SaveFile);

  Stack->Save(SaveFile);

  ushort Index = Wielded ? Stack->SearchItem(Wielded) : 0xFFFF;

  SaveFile << Index << Strength << Endurance << Agility << Perception << RegenerationCounter;
  SaveFile << HP << NP << AP;
  SaveFile << StrengthExperience << EnduranceExperience << AgilityExperience << PerceptionExperience;
  SaveFile << State << Money << HomeRoom << WayPoint;

  uchar c;

  for(c = 0; c < BodyParts(); ++c)
    SaveFile << (item*)BodyPart[c];

  if(HomeRoom)
    if(!game::GetInWilderness() && GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetMaster() == this)
      SaveFile << bool(true);
    else
      SaveFile << bool(false);

  if(StateIsActivated(CONSUMING))
    SaveFile << GetLSquareUnder()->GetStack()->SearchItem(GetConsumingCurrently());
  else
    SaveFile << ushort(0);

  if(StateIsActivated(DIGGING))
    {
      SaveFile << ushort(1);
      SaveFile << GetStack()->SearchItem(GetOldWieldedItem());
      SaveFile << StateVariables.Digging.SquareBeingDiggedX << StateVariables.Digging.SquareBeingDiggedY;
    }
  else
    SaveFile << ushort(0);

  for(c = 0; c < STATES; ++c)
    SaveFile << StateCounter[c];

  if(GetTeam())
    {
      SaveFile << uchar(1);
      SaveFile << Team->GetID();
    }
  else
    SaveFile << uchar(0);

  if(StateIsActivated(CONSUMING))
    {
      Index = GetStack()->SearchItem(GetConsumingCurrently());

      if(Index != 0xFFFF)
	SaveFile << uchar(1) << Index;
      else
	{
	  if(!game::GetInWilderness())
	    Index = GetLSquareUnder()->GetStack()->SearchItem(GetConsumingCurrently());

	  if(Index == 0xFFFF)
	    ABORT("Consuming unknown item!");

	  SaveFile << uchar(2) << Index;
	}
    }
  else
    SaveFile << uchar(0);

  if(GetTeam() && GetTeam()->GetLeader() == this)
    SaveFile << uchar(1);
  else
    SaveFile << uchar(0);
  SaveFile << AssignedName;
}

void character::Load(inputfile& SaveFile)
{
  type::Load(SaveFile);
  entity::Load(SaveFile);

  if(Stack)
    delete Stack;

  Stack = new stack;
  Stack->Load(SaveFile);

  ushort Index;

  SaveFile >> Index;

  Wielded = Index != 0xFFFF ? Stack->GetItem(Index) : 0;

  SaveFile >> Strength >> Endurance >> Agility >> Perception >> RegenerationCounter;
  SaveFile >> HP >> NP >> AP;
  SaveFile >> StrengthExperience >> EnduranceExperience >> AgilityExperience >> PerceptionExperience;
  SaveFile >> State >> Money >> HomeRoom >> WayPoint;

  uchar c;

  for(c = 0; c < BodyParts(); ++c)
    {
      /* CHeck ThIs! */
      item* I;
      SaveFile >> I;
      BodyPart[c] = (bodypart*)I;
    }

  if(HomeRoom)
    {
      bool Master;

      SaveFile >> Master;

      if(Master)
	GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->SetMaster(this);
    }

  SaveFile >> Index;

  if(Index)
    SetConsumingCurrently(GetLSquareUnder()->GetStack()->GetItem(Index));

  SaveFile >> Index;

  if(Index)
    {
      SaveFile >> Index;
      SetOldWieldedItem(GetStack()->GetItem(Index));
      SaveFile >> StateVariables.Digging.SquareBeingDiggedX >> StateVariables.Digging.SquareBeingDiggedY;
    }

  for(c = 0; c < STATES; ++c)
    SaveFile >> StateCounter[c];

  uchar TeamNZ;

  SaveFile >> TeamNZ;

  if(TeamNZ)
    {
      ushort TeamID;
      SaveFile >> TeamID;
      SetTeam(game::GetTeam(TeamID));
    }

  uchar Stacky;

  SaveFile >> Stacky;

  if(Stacky)
    {
      SaveFile >> Index;

      if(Stacky == 1)
	SetConsumingCurrently(GetStack()->GetItem(Index));

      if(Stacky == 2)
	SetConsumingCurrently(GetLSquareUnder()->GetStack()->GetItem(Index));
    }

  uchar Leader;

  SaveFile >> Leader;

  if(Leader)
    GetTeam()->SetLeader(this);

  SaveFile >> AssignedName;
}

bool character::WizardMode()
{
  if(!game::GetWizardMode())
    {
      if(game::BoolQuestion("Do you want to cheat, cheater? This action cannot be undone. [y/N]"))
	{
	  game::EnableWizardMode();

	  for(ushort x = 0; x < 5; ++x)
	    GetStack()->AddItem(new scrollofwishing);
	}
    }
  else
    {
      for(ushort x = 0; x < 5; ++x)
	GetStack()->AddItem(new scrollofwishing);
    }

  return false;

}

bool character::RaiseStats()
{
  Strength += 10;   // I won't touch these just in case we'd do something
  Endurance += 10;  // really odd with GetStrength() etc.
  Agility += 10;
  Perception += 10;
  HP = GetMaxHP();
  game::SendLOSUpdateRequest();

  return false;
}

bool character::LowerStats()
{
  Strength -= 10;
  Endurance -= 10;
  Agility -= 10;
  Perception -= 10;
  HP = GetMaxHP();
  game::SendLOSUpdateRequest();

  return false;
}

bool character::GainAllItems()
{
  for(ushort c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
    if(protocontainer<item>::GetProto(c)->AutoInitializable())
      Stack->AddItem(protocontainer<item>::GetProto(c)->Clone());

  return false;
}

bool character::SeeWholeMap()
{
  game::SeeWholeMap();

  return false;
}

bool character::IncreaseContrast()
{
  configuration::EditContrast(5);
  game::GetCurrentArea()->SendNewDrawRequest();
  return false;
}

bool character::DecreaseContrast()
{
  configuration::EditContrast(-5);
  game::GetCurrentArea()->SendNewDrawRequest();
  return false;
}

ushort character::GetEmitation() const
{
  ushort Emitation = 0;

  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c)->GetEmitation() > Emitation)
      Emitation = GetBodyPart(c)->GetEmitation();

  if(GetStack()->GetEmitation() > Emitation)
    Emitation = GetStack()->GetEmitation();

  return Emitation;
}

void character::SetSquareUnder(square* Square)
{
  entity::SetSquareUnder(Square);
  Stack->SetSquareUnder(Square);
}

bool character::WalkThroughWalls()
{
  game::GoThroughWalls();
  return false;
}

bool character::ShowKeyLayout()
{
  felist List("Keyboard Layout", WHITE, 0, false);

  List.AddDescription("");
  List.AddDescription("Key       Description");

  for(ushort c = 1; game::GetCommand(c); ++c)
    if(game::GetWizardMode() || !game::GetCommand(c)->GetWizardModeFunction())
      {
	std::string Buffer;
	Buffer += game::GetCommand(c)->GetKey();
	Buffer.resize(10, ' ');
	List.AddEntry(Buffer + game::GetCommand(c)->GetDescription(), RED);
      }

  List.Draw();

  return false;
}

bool character::Look()
{
  int Key = 0;
  std::string OldMemory;
  vector2d CursorPos = GetPos();
  game::DrawEverythingNoBlit();
  FONT->Printf(DOUBLEBUFFER, 16, 514, WHITE, "Press direction keys to move cursor or esc to exit from the mode.");
  DRAW_MESSAGES();
  EMPTY_MESSAGES();
  graphics::BlitDBToScreen();

  while(Key != ' ' && Key != 0x1B)
    {
      for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	if(Key == game::GetMoveCommandKey(c))
	  {
	    DOUBLEBUFFER->Fill((CursorPos.X - game::GetCamera().X) << 4, (CursorPos.Y - game::GetCamera().Y + 2) << 4, 16, 16, 0);
	    CursorPos += game::GetMoveVector(c);

	    if(short(CursorPos.X) > game::GetCurrentArea()->GetXSize()-1) CursorPos.X = 0;
	    if(short(CursorPos.X) < 0) 			CursorPos.X = game::GetCurrentArea()->GetXSize()-1;
	    if(short(CursorPos.Y) > game::GetCurrentArea()->GetYSize()-1) CursorPos.Y = 0;
	    if(short(CursorPos.Y) < 0) 			CursorPos.Y = game::GetCurrentArea()->GetYSize()-1;
	  }



      if(CursorPos.X < game::GetCamera().X + 2 || CursorPos.X > game::GetCamera().X + 48)
	game::UpdateCameraXWithPos(CursorPos.X);

      if(CursorPos.Y < game::GetCamera().Y + 2 || CursorPos.Y > game::GetCamera().Y + 27)
	game::UpdateCameraYWithPos(CursorPos.Y);



      if(game::GetSeeWholeMapCheat())
	{
	  OldMemory = game::GetCurrentArea()->GetSquare(CursorPos)->GetMemorizedDescription();
	  game::GetCurrentArea()->GetSquare(CursorPos)->UpdateMemorizedDescription(true);
	}

      if(game::GetCurrentArea()->GetSquare(CursorPos)->GetLastSeen() || game::GetSeeWholeMapCheat())
	{
	  std::string Msg;

	  if(game::GetCurrentArea()->GetSquare(CursorPos)->CanBeSeen(true) || game::GetSeeWholeMapCheat())
	    Msg = "You see here ";
	  else
	    Msg = "You remember here ";

	  Msg += game::GetCurrentArea()->GetSquare(CursorPos)->GetMemorizedDescription();

	  ADD_MESSAGE("%s.", Msg.c_str());

	  character* Character;

	  if((Character = game::GetCurrentArea()->GetSquare(CursorPos)->GetCharacter()) && (game::GetCurrentArea()->GetSquare(CursorPos)->CanBeSeen() && (game::GetInWilderness() || game::GetCurrentLevel()->GetLSquare(CursorPos)->GetLuminance() >= LIGHT_BORDER) || game::GetSeeWholeMapCheat()))
	    Character->DisplayInfo();
	}
      else
	ADD_MESSAGE("You have no idea what might lie here.");

      if(game::GetSeeWholeMapCheat())
	{
	  game::GetCurrentArea()->GetSquare(CursorPos)->SetMemorizedDescription(OldMemory);
	  game::GetCurrentArea()->GetSquare(CursorPos)->SetDescriptionChanged(true);
	}

      if(game::GetWizardMode())
	ADD_MESSAGE("(%d, %d)", CursorPos.X, CursorPos.Y);

      game::DrawEverythingNoBlit();
      igraph::DrawCursor((CursorPos - game::GetCamera() + vector2d(0,2)) << 4);
      game::GetCurrentArea()->GetSquare(CursorPos)->SendNewDrawRequest();
      FONT->Printf(DOUBLEBUFFER, 16, 514, WHITE, "Press direction keys to move cursor or esc to exit from the mode.");
      graphics::BlitDBToScreen();
      EMPTY_MESSAGES();
      Key = GETKEY();
    }

  DOUBLEBUFFER->Fill((CursorPos.X - game::GetCamera().X) << 4, (CursorPos.Y - game::GetCamera().Y + 2) << 4, 16, 16, 0);
  EditPerceptionExperience(1);
  EditAP(900);
  return true;
}

float character::GetAttackStrength() const
{
  return GetWielded() ? GetWielded()->GetWeaponStrength() : GetMeleeStrength();
}

bool character::Engrave(std::string What)
{
  game::GetCurrentLevel()->GetLSquare(GetPos())->Engrave(What);
  return true;
}

bool character::WhatToEngrave()
{
  game::GetCurrentLevel()->GetLSquare(GetPos())->Engrave(game::StringQuestion("What do you want to engrave here?", vector2d(7,7), WHITE, 0, 80, true));
  return false;
}

bool character::MoveRandomly()
{
  bool OK = false;

  for(ushort c = 0; c < 10 && !OK; ++c)
    {
      ushort ToTry = RAND() % 8;

      if(game::GetCurrentLevel()->IsValid(GetPos() + game::GetMoveVector(ToTry)))
	OK = TryMove(GetPos() + game::GetMoveVector(ToTry), false);
    }

  return OK;
}

bool character::TestForPickup(item* ToBeTested) const
{
  if(GetBurdenState(ToBeTested->GetWeight() + GetStack()->SumOfMasses()) != UNBURDENED)
    return false;

  return true;
}

bool character::OpenPos(vector2d APos)
{
  if(game::GetCurrentLevel()->GetLSquare(APos)->Open(this))
    {
      EditAgilityExperience(25);
      EditNP(-10);
      EditAP(-500);
      return true;
    }

  return false;
}

bool character::Pray()
{
  felist Panthenon("To Whom shall thee address thine prayers?", WHITE, 0, true);

  std::vector<uchar> KnownIndex;

  if(!GetLSquareUnder()->GetDivineOwner())
    {
      for(ushort c = 1; c < game::GetGods(); ++c)
	if(game::GetGod(c)->GetKnown())
	  {
	    Panthenon.AddEntry(game::GetGod(c)->CompleteDescription(), RED);
	    KnownIndex.push_back(c);
	  }
    }
  else
    if(game::GetGod(GetLSquareUnder()->GetDivineOwner())->GetKnown())
      Panthenon.AddEntry(game::GetGod(GetLSquareUnder()->GetDivineOwner())->CompleteDescription(), RED);
    else
      ADD_MESSAGE("Somehow you feel that no deity you know can hear your prayers from this place.");

  ushort Select = Panthenon.Draw();

  if(Select & 0x8000)
    return false;
  else
    {
      if(GetLSquareUnder()->GetDivineOwner())
	{
	  if(!Select)
	    {
	      if(game::BoolQuestion(std::string("Do you really wish to pray to ") + game::GetGod(GetLSquareUnder()->GetDivineOwner())->Name() + "? [y/N]"))
		game::GetGod(GetLSquareUnder()->GetDivineOwner())->Pray();
	      else
		return false;
	    }
	  else
	    return false;
	}
      else
	{
	  if(game::BoolQuestion(std::string("Do you really wish to pray to ") + game::GetGod(KnownIndex[Select])->Name() + "? [y/N]"))
	    game::GetGod(KnownIndex[Select])->Pray();
	  else
	    return false;
	}

      return true;
    }
}

void character::SpillBlood(uchar HowMuch, vector2d GetPos)
{
  if(!HowMuch)
    return;

  if(!game::GetInWilderness()) 
    game::GetCurrentLevel()->GetLSquare(GetPos)->SpillFluid(HowMuch, GetBloodColor(), 5, 60);
}

void character::SpillBlood(uchar HowMuch)
{
  if(!HowMuch)
    return;

  if(!game::GetInWilderness()) 
    GetLSquareUnder()->SpillFluid(HowMuch, GetBloodColor(), 5, 60);
}

void character::ReceiveSchoolFoodEffect(long SizeOfEffect)
{
  if(GetIsPlayer())
    ADD_MESSAGE("Yuck! This stuff feels like vomit and old mousepads.");

  SetHP(GetHP() - SizeOfEffect / 100);
  Vomit(SizeOfEffect / 250);

  if(CheckDeath("was poisoned by school food"))
    return;

  if(!(RAND() % 3) && SizeOfEffect / 500)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You gain a little bit of toughness for surviving this stuff.");
      else
	if(GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks tougher.", CNAME(DEFINITE));

      SetEndurance(GetEndurance() + SizeOfEffect / 500);
    }
}

void character::ReceiveNutrition(long SizeOfEffect)
{
  EditNP(SizeOfEffect * 20);
}

void character::ReceiveOmleUrineEffect(long)
{
  if(GetIsPlayer())
    ADD_MESSAGE("You feel a primitive Force coursing through your veins.");
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s looks more powerful.", CNAME(DEFINITE));

  SetStrength(GetStrength() + 1 + RAND() % 2);
  SetHP(GetHP() + 2);
}

void character::ReceivePepsiEffect(long SizeOfEffect)
{
  if(GetIsPlayer())
    ADD_MESSAGE("Urgh. You feel your guruism fading away.");

  SetHP(GetHP() - short(sqrt(SizeOfEffect / 20)));
  SetPerception(GetPerception() + short(sqrt(SizeOfEffect / 250)));

  if(CheckDeath("was poisoned by pepsi"))
    return;

  if(GetIsPlayer())
    game::DoEvilDeed(SizeOfEffect / 10);
}

void character::Darkness(long SizeOfEffect)
{
  if(GetIsPlayer())
    ADD_MESSAGE("Arg. You feel the fate of a navastater placed upon you...");
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("Suddenly %s looks like a navastater.", CNAME(DEFINITE));

  long Penalty = 1 + SizeOfEffect * (100 + RAND() % 26 - RAND() % 26) / 100000;

  if(GetStrength() - Penalty > 1)
    SetStrength(GetStrength() - Penalty);
  else
    SetStrength(1);

  if(GetEndurance() - Penalty > 1)
    SetEndurance(GetEndurance() - Penalty);
  else
    SetEndurance(1);

  if(GetAgility() - Penalty > 1)
    SetAgility(GetAgility() - Penalty);
  else
    SetAgility(1);

  if(GetHP() - Penalty / 2 > 1)
    SetHP(GetHP() - Penalty / 2);
  else
    SetHP(1);

  if(GetIsPlayer())
    {
      game::DoEvilDeed(short(SizeOfEffect / 50));

      if(game::GetWizardMode())
	ADD_MESSAGE("Change in relation: %d.", short(SizeOfEffect / 100));
    }
}

bool character::Kick()
{
  uchar Direction = 0;

  if(!CanKick())
    {
      ADD_MESSAGE("This monster type can not kick.");
      return false;
    }

  if(GetBurdenState() == OVERLOADED)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You try to kick, but you collapse under your load.");
      return true;
    }
  ADD_MESSAGE("In what direction do you wish to kick? [press a direction key or space]");
  game::DrawEverything();
  vector2d KickPos = game::AskForDirectionVector();

  if(KickPos != vector2d(0,0))
    {
      for(ushort c = 0; c < 8; ++c)
	if(KickPos == game::GetMoveVector(c))
	  Direction = c;
		
      return game::GetCurrentLevel()->GetLSquare(GetPos() + KickPos)->Kick(GetStrength(), Direction, this);
    }

  return false;
}

bool character::Offer()
{
  if(GetLSquareUnder()->GetOLTerrain()->CanBeOffered())
    {
      if(!GetStack()->GetItems())
	{
	  ADD_MESSAGE("You have nothing to offer!");
	  return false;
	}

      ushort Index = GetStack()->DrawContents(this, "What do you want to offer?");

      if(Index < GetStack()->GetItems())
	{
	  if(GetStack()->GetItem(Index) == GetWielded())
	    {
	      ADD_MESSAGE("You can't offer something that you wield.");
	      return false;
	    }

	  if(GetStack()->GetItem(Index) == GetTorsoArmor())
	    {
	      ADD_MESSAGE("You can't offer something that you wear.");
	      return false;
	    }

	  if(game::GetGod(GetLSquareUnder()->GetOLTerrain()->GetOwnerGod())->ReceiveOffer(GetStack()->GetItem(Index)))
	    {
	      delete GetStack()->RemoveItem(Index);
	      return true;
	    }
	  else
	    return false;
	}
      else
	return false;
    }
  else
    ADD_MESSAGE("There isn't any altar here!");

  return false;
}

long character::StatScore() const
{
  return GetHP() * 5 + GetEndurance() * 30 + (GetStrength() + GetAgility() + GetPerception()) * 40;
}

long character::Score() const
{
  return (game::GetPlayerBackup() ? game::GetPlayerBackup()->StatScore() : StatScore()) + GetMoney() / 5 + Stack->Score() + game::GodScore();
}

void character::AddScoreEntry(std::string Description, float Multiplier, bool AddEndLevel) const
{
  highscore HScore;

  std::string Desc = game::GetPlayerName() + ", " + Description;

  if(AddEndLevel)
    Desc += " in " + (game::GetInWilderness() ? "the World map" : game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrent()));

  HScore.Add(long((Score() - game::GetBaseScore()) * Multiplier), Desc);

  HScore.Save();
}

bool character::CheckDeath(std::string Msg, bool ForceMsg)
{
  if(GetHP() < 1)
    {
      if(GetIsPlayer() && !game::GetWizardMode())
	AddScoreEntry(Msg);

      Die(ForceMsg);

      return true;
    }
  else
    return false;
}

bool character::CheckStarvationDeath(std::string Msg)
{
  if(GetNP() < 1)
    {
      if(GetIsPlayer() && !game::GetWizardMode())
	AddScoreEntry(Msg);

      Die();

      return true;
    }
  else
    return false;
}

bool character::DrawMessageHistory()
{
  globalmessagingsystem::DrawMessageHistory();

  return false;
}

bool character::Throw()
{
  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to throw!");
      return false;
    }

  ushort Index = GetStack()->DrawContents(this, "What do you want to throw?");

  if(Index < GetStack()->GetItems())
    {
      if(GetStack()->GetItem(Index) == GetTorsoArmor())
	{
	  ADD_MESSAGE("You can't throw something that you wear.");
	  return false;
	}

      uchar Answer = game::DirectionQuestion("In what direction do you wish to throw?  [press a direction key or space]", 8, false);

      if(Answer == 0xFF)
	return false;

      if(GetStack()->GetItem(Index) == GetWielded())
	SetWielded(0);

      ThrowItem(Answer, GetStack()->GetItem(Index));

      EditStrengthExperience(25);
      EditAgilityExperience(25);
      EditPerceptionExperience(25);
      EditNP(-50);
      return true;
    }
  else
    return false;
}

bool character::ThrowItem(uchar Direction, item* ToBeThrown)
{
  if(Direction > 7)
    ABORT("Throw in TOO odd direction...");
  if(GetWielded() && ToBeThrown->UseThrowStrengthModifier())
    return ToBeThrown->Fly(Direction, ushort(GetStrength() * GetThrowStrengthModifier()), GetStack(), GetIsPlayer());
  else
    return ToBeThrown->Fly(Direction, GetStrength(), GetStack(), GetIsPlayer());
}

void character::HasBeenHitByItem(item* Thingy, float Speed)
{
  ushort Damage = ushort(Thingy->GetWeaponStrength() * Thingy->GetWeight() * CalculateArmorModifier() * sqrt(Speed) / 5000000000.0f) + (RAND() % 5 ? 1 : 0);

  SetHP(GetHP() - Damage);

  if(GetIsPlayer())
    ADD_MESSAGE("%s hits you.", Thingy->CNAME(DEFINITE));
  else
    if(GetSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s hits %s.", Thingy->CNAME(DEFINITE), CNAME(DEFINITE));

  if(game::GetWizardMode() && GetLSquareUnder()->CanBeSeen(true))
    ADD_MESSAGE("(damage: %d) (speed: %f)", Damage, Speed);

  SpillBlood(1 + RAND() % 1);
  CheckDeath(std::string("died by thrown ") + Thingy->CNAME(INDEFINITE) );
}

bool character::DodgesFlyingItem(item*, float Speed)
{			// Formula requires a little bit of tweaking...
  if(!(RAND() % 20))
    return RAND() % 2 ? true : false;

  if(RAND() % ulong(sqrt(Speed) * GetSize() / GetAgility() * 10 + 1) > 40)
    return false;
  else
    return true;
}

void character::ReceiveFireDamage(character* Burner, std::string DeathMsg, long SizeOfEffect)
{
  if(GetIsPlayer())
    ADD_MESSAGE("You burn.");
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s is hurt by the fire.", CNAME(DEFINITE));

  if(SizeOfEffect > 1)
    SetHP(GetHP() - (SizeOfEffect + RAND() % (SizeOfEffect / 2)));
  else
    SetHP(GetHP() - SizeOfEffect);

  GetStack()->ReceiveFireDamage(Burner, DeathMsg, SizeOfEffect);
}

void character::GetPlayerCommand()
{
  bool HasActed = false;

  while(!HasActed)
    {
      game::DrawEverything();

      game::SetInGetCommand(true);
      int Key = GETKEY();
      game::SetInGetCommand(false);

      bool ValidKeyPressed = false;
      uchar c;

      for(c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	if(Key == game::GetMoveCommandKey(c))
	  {
	    HasActed = TryMove(GetPos() + game::GetMoveVector(c));
	    ValidKeyPressed = true;
	  }

      for(c = 1; game::GetCommand(c); ++c)
	if(Key == game::GetCommand(c)->GetKey())
	  {
	    if(game::GetInWilderness() && !game::GetCommand(c)->GetCanBeUsedInWilderness())
	      ADD_MESSAGE("This function cannot be used while in wilderness.");
	    else
	      if(!game::GetWizardMode() && game::GetCommand(c)->GetWizardModeFunction())
		ADD_MESSAGE("Activate wizardmode to use this function.");
	      else
		HasActed = (this->*game::GetCommand(c)->GetLinkedFunction())();

	    ValidKeyPressed = true;
	  }

      if(!ValidKeyPressed)
	ADD_MESSAGE("Unknown key, you %s. Press '?' for a list of commands.", game::Insult());
    }
}

void character::Vomit(ushort)
{
  if(GetIsPlayer())
    ADD_MESSAGE("You vomit.");
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s vomits.", CNAME(DEFINITE));

  EditStrengthExperience(-50);
  EditEnduranceExperience(50);
  EditNP(-200 - RAND() % 201);

  GetLSquareUnder()->ReceiveVomit(this);
}

bool character::Apply()
{
  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to apply!");
      return false;
    }

  ushort Index = GetStack()->DrawContents(this, "What do you want to apply?");

  if(Index < GetStack()->GetItems())
    {
      if(GetStack()->GetItem(Index) == GetTorsoArmor())
	{
	  ADD_MESSAGE("You can't apply something that you wear.");
	  return false;
	}

      if(!GetStack()->GetItem(Index)->Apply(this, GetStack()))
	return false;

      if(GetWielded() && !GetWielded()->GetExists()) 
	SetWielded(0);

      return true;
    }
  else
    return false;
}

vector2d character::GetPos() const
{
  return SquareUnder->GetPos();
}

bool character::ForceVomit()
{
  ADD_MESSAGE("You push your fingers down to your throat and vomit.");
  Vomit(1 + RAND() % 3);
  return true;
}

bool character::Zap()
{
  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to zap with.");
      return false;
    }

  ushort Index = GetStack()->DrawContents(this, "What do you want to zap with?");

  if(Index < GetStack()->GetItems())
    {
      if(!GetStack()->GetItem(Index)->CanBeZapped())
	{
	  ADD_MESSAGE("You can't zap that!");
	  return false;
	}

      uchar Answer = game::DirectionQuestion("In what direction do you wish to zap?  [press a direction key, space or .]", 8, false, true);

      if(Answer == 0xFF)
	return false;

      return GetStack()->GetItem(Index)->Zap(this, GetPos(), Answer);
    }
  else
    return false;
}

bool character::Polymorph(character* NewForm, ushort Counter)
{
  if(GetIsPlayer())
    {
      if(StateIsActivated(POLYMORPHED))
	{
	  ADD_MESSAGE("You shudder.");
	  delete NewForm;
	  return true;
	}

      GetSquareUnder()->RemoveCharacter();
      GetSquareUnder()->AddCharacter(NewForm);
      SetSquareUnder(0);

      ADD_MESSAGE("Your body glows in a crimson light. You transform into %s!", NewForm->CNAME(INDEFINITE));
      game::SetPlayerBackup(this);
      game::SetPlayer(NewForm);
      NewForm->ActivateState(POLYMORPHED);
      NewForm->SetStateCounter(POLYMORPHED, Counter);
      game::SendLOSUpdateRequest();
    }
  else
    {
      GetSquareUnder()->RemoveCharacter();
      GetSquareUnder()->AddCharacter(NewForm);
      SetSquareUnder(0);

      SetExists(false);
    }

  while(GetStack()->GetItems())
    GetStack()->MoveItem(0, NewForm->GetStack());

  NewForm->SetMoney(GetMoney());

  if(NewForm->CanWield())
    NewForm->SetWielded(GetWielded());

  if(NewForm->CanWear())
    NewForm->SetTorsoArmor(GetTorsoArmor());

  NewForm->ChangeTeam(GetTeam());

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(NewForm);

  ChangeTeam(0);

  return true;
}

wsquare* character::GetWSquareUnder() const
{
  return (wsquare*)SquareUnder;
}

ulong character::GetBloodColor() const
{
  return MAKE_RGB(75,0,0);
}

void character::BeKicked(ushort KickStrength, bool ShowOnScreen, uchar Direction, character* Kicker)
{
  if(RAND() % 10 + RAND() % 3 * KickStrength / 2 > GetAgility())
    {
      if(KickStrength > 8 + RAND() % 4 - RAND() % 4)
	{
	  if(ShowOnScreen)
	    if(GetIsPlayer())
	      ADD_MESSAGE("The kick throws you off balance.");
	    else
	      ADD_MESSAGE("The kick throws %s off balance.", CNAME(DEFINITE));

	  FallTo(GetPos() + game::GetMoveVector(Direction), ShowOnScreen);

	  ushort Damage = RAND() % (KickStrength / 10 + 1);

	  if(ShowOnScreen && game::GetWizardMode())
	    ADD_MESSAGE("(damage: %d)", Damage);

	  SetHP(GetHP() - Damage);			
	  CheckDeath("kicked to death", Kicker->GetIsPlayer());		
	}
      else
	{
	  if(ShowOnScreen)
	    if(GetIsPlayer())
	      ADD_MESSAGE("The kick hits you, but you remain standing.");
	    else
	      ADD_MESSAGE("The kick hits %s.", CNAME(DEFINITE));

	  ushort Damage = RAND() % (KickStrength / 15 + 1);

	  if(ShowOnScreen && game::GetWizardMode())
	    ADD_MESSAGE("(damage: %d)", Damage);

	  SetHP(GetHP() - Damage);
	  CheckDeath("kicked to death", Kicker->GetIsPlayer());
	}

      Kicker->KickHit();
    }
  else
    if(ShowOnScreen)
      if(GetIsPlayer())
	ADD_MESSAGE("The kick misses you.");
      else
	ADD_MESSAGE("The kick misses %s.", CNAME(DEFINITE));
}

void character::FallTo(vector2d Where, bool OnScreen)
{
  EditAP(-500);

  if(game::GetCurrentLevel()->GetLSquare(Where)->GetOLTerrain()->GetIsWalkable() && !game::GetCurrentLevel()->GetLSquare(Where)->GetCharacter())
    Move(Where, true);

  if(!game::GetCurrentLevel()->GetLSquare(Where)->GetOLTerrain()->GetIsWalkable())
    {
      if(OnScreen)
	if(GetIsPlayer()) 
	  ADD_MESSAGE("You hit your head on the wall.");
	else
	  ADD_MESSAGE("%s hits %s head on the wall.", Name(DEFINITE).c_str(), game::PossessivePronoun(GetSex()));

      SetHP(GetHP() - RAND() % 2);
      CheckDeath("killed by hitting a wall");
    }

  // Place code that handles characters bouncing to each other here
}

bool character::CheckCannibalism(ushort What)
{ 
  return GetTorso()->GetMaterial(0)->GetType() == What; 
}

void character::StandIdleAI()
{
  SeekLeader();

  if(CheckForEnemies(true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(CheckForDoors())
    return;
}

bool character::ShowWeaponSkills()
{
  ADD_MESSAGE("This race isn't capable of developing weapon skill experience!");
  return false;
}

void character::Faint()
{
  if(GetIsPlayer())
    ADD_MESSAGE("You faint.");
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s faints.", CNAME(DEFINITE));

  ActivateState(FAINTED);
  StateCounter[FAINTED] = 100 + RAND() % 101;
}

void character::FaintHandler()
{
  if(!(StateCounter[FAINTED]--))
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You wake up.");
      else
	if(GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s wakes up.", CNAME(DEFINITE));

      EndFainted();
    }
  else
    {
      EditStrengthExperience(-3);
    }
}

void character::ConsumeHandler()
{
  if(ulong(++StateCounter[CONSUMING]) * 500 >= GetConsumingCurrently()->ConsumeLimit())
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You finish %s %s.", GetConsumingCurrently()->GetConsumeVerb().c_str(), GetConsumingCurrently()->CNAME(DEFINITE));
      else
	if(GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s finishes eating %s.", CNAME(DEFINITE), GetConsumingCurrently()->CNAME(DEFINITE));

      EndConsuming();

      return;
    }
}

void character::PolymorphHandler()
{
  if(!(StateCounter[POLYMORPHED]--))
    {
      ADD_MESSAGE("You return to your true form.");
      EndPolymorph();
    }
}

void character::EndFainted()
{
  if(StateIsActivated(FAINTED))
    DeActivateState(FAINTED);
}

void character::EndConsuming()
{
  if(StateIsActivated(CONSUMING))
    {
      if(GetConsumingCurrently()->Consume(this, StateCounter[CONSUMING] * 500))
	{
	  item* ToBeDeleted = GetStack()->RemoveItem(GetStack()->SearchItem(GetConsumingCurrently()));

	  if(!ToBeDeleted)
	    ToBeDeleted = GetLSquareUnder()->GetStack()->RemoveItem(GetLSquareUnder()->GetStack()->SearchItem(GetConsumingCurrently()));

	  delete ToBeDeleted;
	}

      DeActivateState(CONSUMING);
      SetConsumingCurrently(0);
    }
}

void character::EndPolymorph()
{
  if(StateIsActivated(POLYMORPHED))
    {
      SetExists(false);

      GetSquareUnder()->RemoveCharacter();
      GetSquareUnder()->AddCharacter(game::GetPlayerBackup());
      SetSquareUnder(0);

      while(GetStack()->GetItems())
	GetStack()->MoveItem(0, game::GetPlayerBackup()->GetStack());

      SetSquareUnder(game::GetPlayerBackup()->GetSquareUnder()); // might be used afterwards

      if(game::GetPlayerBackup()->CanWield())
	game::GetPlayerBackup()->SetWielded(GetWielded());

      if(game::GetPlayerBackup()->CanWear())
	game::GetPlayerBackup()->SetTorsoArmor(GetTorsoArmor());

      game::GetPlayerBackup()->SetMoney(GetMoney());
      game::SetPlayer(game::GetPlayerBackup());
      game::SetPlayerBackup(0);
      game::GetPlayer()->ChangeTeam(GetTeam());

      if(GetTeam()->GetLeader() == this)
	GetTeam()->SetLeader(game::GetPlayer());

      game::GetPlayer()->TestWalkability();

      game::SendLOSUpdateRequest();
    }
}

bool character::CanMove()
{
  if(StateIsActivated(FAINTED) || StateIsActivated(CONSUMING) || StateIsActivated(RESTING) || StateIsActivated(DIGGING) || StateIsActivated(GOING))
    return false;
  else
    return true;
}

void character::DeActivateVoluntaryStates(std::string Reason)
{
  if(GetIsPlayer())
    {
      if((StateIsActivated(CONSUMING) || StateIsActivated(RESTING) || StateIsActivated(DIGGING) || StateIsActivated(GOING)) && Reason != "")
	ADD_MESSAGE("%s.", Reason.c_str());

      if(StateIsActivated(CONSUMING))
	ADD_MESSAGE("You stop eating.");

      if(StateIsActivated(RESTING))
	ADD_MESSAGE("You stop resting.");

      if(StateIsActivated(DIGGING))
	ADD_MESSAGE("You stop digging.");
    }

  EndConsuming();
  EndRest();

  if(StateIsActivated(DIGGING))
    {
      EndDig();
      EditAP(-250);
    }

  EndGoing();
}

void character::StateAutoDeactivation()
{
  if(!StateIsActivated(CONSUMING) && !StateIsActivated(RESTING) && !StateIsActivated(DIGGING) && !StateIsActivated(GOING))
    return;

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->GetExists() && ((GetIsPlayer() && (*i)->GetSquareUnder()->CanBeSeen()) || (!GetIsPlayer() && (*i)->GetSquareUnder()->CanBeSeenFrom(GetPos(), LOSRangeSquare(), HasInfraVision()))))
	  {
	    DeActivateVoluntaryStates((*i)->Name(DEFINITE) + " seems to be hostile");
	    return;
	  }
}

void character::StruckByWandOfStriking(character* Zapper, std::string DeathMsg)
{
  if(GetIsPlayer())
    ADD_MESSAGE("The wand hits you.");
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("The wand hits %s.", CNAME(DEFINITE));

  GetStack()->StruckByWandOfStriking(Zapper, DeathMsg);

  SetHP(GetHP() - 20 - RAND() % 11);

  CheckDeath("killed by a wand of striking");
}

bool character::CheckForEnemies(bool CheckDoors)
{
  bool HostileCharsNear = false;

  character* NearestChar = 0;
  ulong NearestDistance = 0xFFFFFFFF;

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->GetExists())
	  {
	    ulong ThisDistance = GetHypotSquare(long((*i)->GetPos().X) - GetPos().X, long((*i)->GetPos().Y) - GetPos().Y);

	    if(ThisDistance <= LOSRangeSquare())
	      HostileCharsNear = true;

	    if((ThisDistance < NearestDistance || (ThisDistance == NearestDistance && !(RAND() % 3))) && (*i)->GetLSquareUnder()->CanBeSeenFrom(GetPos(), LOSRangeSquare(), HasInfraVision()))
	      {
		NearestChar = *i;
		NearestDistance = ThisDistance;
	      }
	  }

  if(NearestChar)
    {
      if(SpecialEnemySightedReaction(NearestChar))
	return true;

      if(!GetTeam()->GetLeader() && NearestChar->GetIsPlayer())
	WayPoint = NearestChar->GetPos();

      return MoveTowards(NearestChar->GetPos());
    }
  else
    if(!GetTeam()->GetLeader() && WayPoint.X != -1)
      if(!MoveTowards(WayPoint))
	{
	  WayPoint.X = -1;
	  return false;
	}
      else
	return true;
    else
      if((!GetTeam()->GetLeader() || (GetTeam()->GetLeader() && WayPoint.X == -1)) && HostileCharsNear)
	{
	  if(CheckDoors && CheckForDoors())
	    return true;

	  MoveRandomly(); // one has heard that an enemy is near but doesn't know where
	  return true;
	}
      else
	return false;
}

bool character::CheckForDoors()
{
  if(CanOpenDoors())
    {
      DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
			    if(game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetOLTerrain()->CanBeOpenedByAI() && game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->Open(this))
			    return true;);
    }

  return false;
}

bool character::CheckForUsefulItemsOnGround()
{
  for(ushort c = 0; c < GetLSquareUnder()->GetStack()->GetItems(); ++c)
    {
      /*if(CanWear() && GetLSquareUnder()->GetStack()->GetItem(c)->GetArmorValue() < CalculateArmorModifier() && GetBurdenState(GetStack()->SumOfMasses() + GetLSquareUnder()->GetStack()->GetItem(c)->GetWeight() - (GetTorsoArmor() ? GetTorsoArmor()->GetWeight() : 0)) == UNBURDENED)
	if(!GetLSquareUnder()->GetRoom() || GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, GetLSquareUnder()->GetStack()->GetItem(c)))
	{
	item* ToWear = GetLSquareUnder()->GetStack()->MoveItem(c, GetStack());

	if(GetTorsoArmor())
	GetStack()->MoveItem(GetStack()->SearchItem(GetTorsoArmor()), GetLSquareUnder()->GetStack());

	SetTorsoArmor(ToWear);

	if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s picks up and wears %s.", CNAME(DEFINITE), ToWear->CNAME(DEFINITE));

	return true;
	}*/

      if(CanWield() && long(GetLSquareUnder()->GetStack()->GetItem(c)->GetWeaponStrength()) > long(GetAttackStrength()) && GetBurdenState(GetStack()->SumOfMasses() + GetLSquareUnder()->GetStack()->GetItem(c)->GetWeight() - (GetWielded() ? GetWielded()->GetWeight() : 0)) == UNBURDENED)
	if(!GetLSquareUnder()->GetRoom() || GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, GetLSquareUnder()->GetStack()->GetItem(c)))
	  {
	    item* ToWield = GetLSquareUnder()->GetStack()->MoveItem(c, GetStack());

	    if(GetWielded())
	      GetStack()->MoveItem(GetStack()->SearchItem(GetWielded()), GetLSquareUnder()->GetStack());

	    SetWielded(ToWield);

	    if(GetLSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s picks up and wields %s.", CNAME(DEFINITE), ToWield->CNAME(DEFINITE));

	    return true;
	  }

      if(GetLSquareUnder()->GetStack()->GetItem(c)->Consumable(this) && !GetLSquareUnder()->GetStack()->GetItem(c)->IsBadFoodForAI(this))
	if(!GetLSquareUnder()->GetRoom() || GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->ConsumeItem(this, GetLSquareUnder()->GetStack()->GetItem(c)))
	  {
	    if(GetLSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s eats %s.", CNAME(DEFINITE), GetLSquareUnder()->GetStack()->GetItem(c)->CNAME(DEFINITE));

	    ConsumeItem(GetLSquareUnder()->GetStack()->GetItem(c), GetLSquareUnder()->GetStack());

	    return true;
	  }
    }

  return false;
}

bool character::FollowLeader()
{
  if(!GetTeam()->GetLeader())
    return false;

  if(GetTeam()->GetLeader()->GetLSquareUnder()->CanBeSeenFrom(GetPos(), LOSRangeSquare(), HasInfraVision()))
    {
      vector2d Distance = GetPos() - WayPoint;

      if(abs(short(Distance.X)) <= 2 && abs(short(Distance.Y)) <= 2)
	return false;
      else
	return MoveTowards(WayPoint);
    }
  else
    if(WayPoint.X != -1)
      if(!MoveTowards(WayPoint))
	{
	  WayPoint.X = -1;
	  return false;
	}
      else
	return true;
    else
      return false;
}

void character::SeekLeader()
{
  if(GetTeam()->GetLeader() && GetTeam()->GetLeader()->GetLSquareUnder()->CanBeSeenFrom(GetPos(), LOSRangeSquare(), HasInfraVision()))
    WayPoint = GetTeam()->GetLeader()->GetPos();
}

bool character::RestUntilHealed()
{
  long HPToRest = game::NumberQuestion("How many hit points you desire?", vector2d(7,7), WHITE);

  if(HPToRest <= GetHP())
    {
      ADD_MESSAGE("Your HP is already %d.", GetHP());
      return false;
    }

  GetSquareUnder()->GetOTerrain()->ShowRestMessage(this);
  DRAW_MESSAGES();
  StateCounter[RESTING] = HPToRest;
  ActivateState(RESTING);
  return true;
}


void character::RestHandler()
{
  if(GetHP() >= StateCounter[RESTING] || GetHP() == GetMaxHP())
    EndRest();
  else
    {
      EditAgilityExperience(-1 * GetSquareUnder()->RestModifier());
    }
}

void character::EndRest()
{
  DeActivateState(RESTING);
}

void character::DigHandler()
{
  if(StateCounter[DIGGING] > 0)
    {
      StateCounter[DIGGING]--;
      EditStrengthExperience(5);
      EditAgilityExperience(-5);
      EditNP(-5);
    }
  else
    EndDig();
}

void character::EndDig()
{
  if(StateIsActivated(DIGGING))
    {
      if(StateCounter[DIGGING] == 0)
	{
	  game::GetCurrentLevel()->GetLSquare(GetSquareBeingDigged())->Dig(this, GetWielded());
	  SetWielded(GetOldWieldedItem());
	}

      DeActivateState(DIGGING);
    }
}

bool character::OutlineCharacters()
{
  configuration::SetOutlineCharacters(!configuration::GetOutlineCharacters());
  game::GetCurrentArea()->SendNewDrawRequest();
  return false;
}

bool character::OutlineItems()
{
  configuration::SetOutlineItems(!configuration::GetOutlineItems());
  game::GetCurrentArea()->SendNewDrawRequest();
  return false;
}

float character::GetThrowStrengthModifier() const
{
  if(GetWielded())
    return GetWielded()->GetThrowStrengthModifier();
  else
    return 1;
}

ushort character::GetMeleeAttributeModifier() const
{
  ushort Modifier = (GetAgility() << 2) + GetStrength() + GetPerception() / 3;

  switch(GetBurdenState())
    {
    case BURDENED:
      return (Modifier * 3) >> 2;
    case STRESSED:
    case OVERLOADED:
      return Modifier >> 1;
    default:
      return Modifier;
    }
}

float character::GetToHitValue() const
{
  if(GetWielded())
    return GetMeleeAttributeModifier() / sqrt(GetWielded()->GetWeight() > 400 ? GetWielded()->GetWeight() : 400) * 10;
  else
    return GetMeleeAttributeModifier() >> 1;
}

float character::GetDodgeValue() const
{
  return (GetMeleeAttributeModifier() << 1) / sqrt(GetSize());
}

ulong character::CurrentDanger()
{
  return ulong(GetAttackStrength() * GetStrength() * GetHP() * (GetToHitValue() + GetDodgeValue() + GetAgility()) / (float(CalculateArmorModifier()) * 1000));
}

ulong character::MaxDanger()
{
  return ulong(GetAttackStrength() * GetStrength() * GetMaxHP() * (GetToHitValue() + GetDodgeValue() + GetAgility()) / (float(CalculateArmorModifier()) * 1000));
}

bool character::RaiseGodRelations()
{
  for(ushort c = 1; c < game::GetGods(); ++c)
    game::GetGod(c)->AdjustRelation(50);

  return false;
}

bool character::LowerGodRelations()
{
  for(ushort c = 1; c < game::GetGods(); ++c)
    game::GetGod(c)->AdjustRelation(-50);

  return false;
}

ushort character::LOSRange() const
{
  return GetPerception() * game::GetCurrentArea()->GetLOSModifier() / 48;
}

ushort character::LOSRangeSquare() const
{
  ulong LOSModifier = game::GetCurrentArea()->GetLOSModifier();

  return GetPerception() * GetPerception() * LOSModifier * LOSModifier / 2304;
}

bool character::GainDivineKnowledge()
{
  for(ushort c = 1; c < game::GetGods(); ++c)
    game::GetGod(c)->SetKnown(true);

  return false;
}

bool character::Displace(character* Who)
{
  if(!GetBurdenState())
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You try to use your claws to crawl forward. But your load is too heavy.");

      return true;
    }

  if(CurrentDanger() > Who->CurrentDanger() && Who->CanBeDisplaced() && !Who->StateIsActivated(CONSUMING) && !Who->StateIsActivated(RESTING) && !Who->StateIsActivated(DIGGING))
    {
      if(GetIsPlayer())
	if(GetSquareUnder()->CanBeSeen() || Who->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("You displace %s!", Who->CNAME(DEFINITE));
	else
	  ADD_MESSAGE("You displace something!");
      else
	if(Who->GetIsPlayer())
	  if(GetSquareUnder()->CanBeSeen() || Who->GetSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s displaces you!", CNAME(DEFINITE));
	  else
	    ADD_MESSAGE("Something displaces you!");
	else
	  if(GetSquareUnder()->CanBeSeen() || Who->GetSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s displaces %s!", CNAME(DEFINITE), Who->CNAME(DEFINITE));

      GetLSquareUnder()->SwapCharacter(Who->GetLSquareUnder());

      if(GetIsPlayer())
	ShowNewPosInfo();

      if(Who->GetIsPlayer())
	Who->ShowNewPosInfo();

      EditAP(-500);
      return true;
    }
  else
    {
      if(GetIsPlayer())
	if(Who->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s resists!", Who->CNAME(DEFINITE));
	else
	  ADD_MESSAGE("Something resists!");

      return false;
    }
}

bool character::Sit()
{
  if(GetLSquareUnder()->GetOLTerrain()->SitOn(this))
    return true;
  else
    return GetLSquareUnder()->GetGLTerrain()->SitOn(this);
}

void character::SetNP(long What)
{
  long BNP = GetNP();
  uchar OldGetHungerState = GetHungerState();
  NP = What;

  if(GetIsPlayer())
    {
      if(GetHungerState() == VERYHUNGRY && OldGetHungerState != VERYHUNGRY)
	{
	  game::Beep();
	  ADD_MESSAGE("You are getting very hungry.");
	  DeActivateVoluntaryStates();
	}
      else if(GetHungerState() == HUNGRY && OldGetHungerState != HUNGRY && OldGetHungerState != VERYHUNGRY)
	{
	  ADD_MESSAGE("You are getting hungry.");
	  DeActivateVoluntaryStates();
	}
    }
}
void character::ShowNewPosInfo() const
{
  if(GetPos().X < game::GetCamera().X + 2 || GetPos().X > game::GetCamera().X + 48)
    game::UpdateCameraX();

  if(GetPos().Y < game::GetCamera().Y + 2 || GetPos().Y > game::GetCamera().Y + 27)
    game::UpdateCameraY();

  game::SendLOSUpdateRequest();

  if(!game::GetInWilderness())
    {
      if(GetLSquareUnder()->GetLuminance() < LIGHT_BORDER && !game::GetSeeWholeMapCheat())
	ADD_MESSAGE("It's dark in here!");

      if(GetLSquareUnder()->GetStack()->GetItems() > 0)
	{
	  if(GetLSquareUnder()->GetStack()->GetItems() > 1)
	    ADD_MESSAGE("Several items are lying here.");
	  else
	    ADD_MESSAGE("%s is lying here.", GetLSquareUnder()->GetStack()->GetItem(0)->CNAME(INDEFINITE));
	}
		
      if(game::GetCurrentLevel()->GetLSquare(GetPos())->GetEngraved() != "")
	{
	  ADD_MESSAGE("Something has been engraved here:");
	  ADD_MESSAGE("\"%s\"", game::GetCurrentLevel()->GetLSquare(GetPos())->GetEngraved().c_str());
	}
    }
}

void character::Hostility(character* Enemy)
{
  if(GetTeam() != Enemy->GetTeam())
    GetTeam()->Hostility(Enemy->GetTeam());
  else if(Enemy->GetExists())
    {
      if(Enemy->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s becomes enraged.", Enemy->CNAME(DEFINITE));

      Enemy->ChangeTeam(game::GetTeam(1));
    }
}

stack* character::GetGiftStack() const
{
  if(GetLSquareUnder()->GetRoom() && !GetLSquareUnder()->GetRoomClass()->AllowDropGifts())
    return GetStack();
  else
    return GetLSquareUnder()->GetStack();
}

bool character::MoveRandomlyInRoom()
{
  bool OK = false;

  for(ushort c = 0; c < 10 && !OK; ++c)
    {
      ushort ToTry = RAND() % 8;

      if(game::GetCurrentLevel()->IsValid(GetPos() + game::GetMoveVector(ToTry)) && !game::GetCurrentLevel()->GetLSquare(GetPos() + game::GetMoveVector(ToTry))->GetOLTerrain()->IsDoor())
	OK = TryMove(GetPos() + game::GetMoveVector(ToTry), false);
    }

  return OK;
}

void character::SetHP(short What)
{
  long BHP = GetHP();

  HP = What;

  if(GetIsPlayer())
    if(GetHP() == 1 && BHP >= 2)
      {
	game::Beep();
	ADD_MESSAGE("You bleed very badly.");
      }
    else if(GetHP() < GetMaxHP() / 3 && BHP >= GetMaxHP() / 3)
      {
	game::Beep();
	ADD_MESSAGE("You bleed.");
      }
}

void character::EndGoing()
{
  DeActivateState(GOING);
  game::GetCurrentArea()->SendNewDrawRequest();
}

bool character::Go()
{
  vector2d Temp;

  if((Temp = game::AskForDirectionVector("What direction do you want to go?")) != vector2d(0,0))
    {
      ActivateState(GOING);
      StateVariables.Going.Direction = game::GetDirectionForVector(Temp);

      uchar OKDirectionsCounter = 0;

      DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
      {
	if(game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetIsWalkable(this))
	  OKDirectionsCounter++;	
      });

      StateVariables.Going.WalkingInOpen = OKDirectionsCounter > 2 ? true : false;

      square* OldSquare = GetSquareUnder();

      GoHandler();

      return OldSquare != GetSquareUnder();
    }

  return false;
}

void character::GoHandler()
{
  if(GetAP() >= 0)
    {
      StateAutoDeactivation();

      if(!StateIsActivated(GOING))
	return;

      if(!game::IsValidPos(GetPos() + game::GetMoveVector(StateVariables.Going.Direction)))
	{
	  EndGoing();
	  return;
	}

      lsquare* MoveToSquare = game::GetCurrentLevel()->GetLSquare(GetPos() + game::GetMoveVector(StateVariables.Going.Direction));

      if(!MoveToSquare->GetIsWalkable(this) || (MoveToSquare->GetCharacter() && GetTeam() != MoveToSquare->GetCharacter()->GetTeam()))
	{
	  EndGoing();
	  return;
	}

      uchar OKDirectionsCounter = 0;

      DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
      {
	if(game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetIsWalkable(this))
	  OKDirectionsCounter++;	
      });

      if(!StateVariables.Going.WalkingInOpen)
	{
	  if(OKDirectionsCounter > 2)
	    {
	      EndGoing();
	      return;
	    }
	}
      else
	if(OKDirectionsCounter <= 2)
	  StateVariables.Going.WalkingInOpen = false;

      square* BeginSquare = GetSquareUnder();

      if(!TryMove(MoveToSquare->GetPos()) || BeginSquare == GetSquareUnder() || GetLSquareUnder()->GetLuminance() < LIGHT_BORDER || GetLSquareUnder()->GetStack()->GetItems())
	{
	  EndGoing();
	  return;
	}
    }
}

bool character::ShowConfigScreen()
{
  configuration::ShowConfigScreen();
  return false;
}

void character::SetTeam(team* What)
{
  Team = What;
  SetTeamIterator(GetTeam()->Add(this));
}

void character::ChangeTeam(team* What)
{
  if(GetTeam())
    GetTeam()->Remove(GetTeamIterator());

  Team = What;

  if(GetTeam())
    SetTeamIterator(GetTeam()->Add(this));
}

void character::ReceiveKoboldFleshEffect(long)
{
  if(!(RAND() % 3))
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You lose control of your legs and fall down.");

      Faint();
    }
  else
    ADD_MESSAGE("This stuff tastes really funny.");
}

bool character::ChangeRandomStat(short HowMuch)
{
  // Strength, Endurance, Agility, Perception

  for(ushort c = 0; c < 15; c++)
    switch(RAND() % 4)
      {
      case 0:
	if(GetStrength() > -HowMuch + 1)
	  {
	    SetStrength(GetStrength() + HowMuch);
	    return true;
	  }
	break;

      case 1:
	if(GetEndurance() > -HowMuch + 1)
	  {
	    SetEndurance(GetEndurance() + HowMuch);
	    return true;
	  }
	break;

      case 2:
	if(GetAgility() > -HowMuch + 1)
	  {
	    SetAgility(GetAgility() + HowMuch);
	    return true;
	  }
	break;

      case 3:
	if(GetPerception() > -HowMuch + 1)
	  {
	    SetPerception(GetPerception() + HowMuch);
	    return true;
	  }
	break;
      }
	
  return false; // No stat could be lowered (tried randomly 15 times)
}

void character::CheckGearExistence()
{
  if(GetWielded() && !GetWielded()->GetExists())
    SetWielded(0);
}

uchar character::RandomizeReply(uchar Replies, bool* Said)
{
  bool NotSaid = false;

  for(ushort c = 0; c < Replies; ++c)
    if(!Said[c])
      {
	NotSaid = true;
	break;
      }

  if(!NotSaid)
    for(ushort c = 0; c < Replies; ++c)
      Said[c] = false;

  uchar ToSay;

  while(Said[ToSay = RAND() % Replies]);

  Said[ToSay] = true;

  return ToSay;
}

ushort character::DangerLevel()
{
  static ulong DangerPointMinimum[] = { 0, 100, 500, 2500, 10000, 50000, 250000, 1000000, 5000000, 25000000 };

  for(ushort c = 9;; --c)
    if(CurrentDanger() >= DangerPointMinimum[c])
      return c;
}

void character::DisplayInfo()
{
  if(GetIsPlayer())
    ADD_MESSAGE("You are %s here.", StandVerb().c_str());
  else
    {
      ADD_MESSAGE("%s is %s here.", CNAME(INDEFINITE), StandVerb().c_str());

      std::string Msg = game::PersonalPronoun(GetSex());

      if(GetTeam() == game::GetPlayer()->GetTeam())
	Msg += std::string(" is at danger level ") + DangerLevel();

      if(GetWielded())
	{
	  if(GetTorsoArmor())
	    {
	      if(GetTeam() == game::GetPlayer()->GetTeam())
		Msg += " and";

	      Msg += std::string(" is wielding ") + GetWielded()->CNAME(INDEFINITE);

	      ADD_MESSAGE("%s.", Msg.c_str());
	      Msg = std::string(game::PersonalPronoun(GetSex())) + " wears " + GetTorsoArmor()->CNAME(INDEFINITE);
	    }
	  else
	    {
	      if(GetTeam() == game::GetPlayer()->GetTeam())
		Msg += ",";

	      Msg += std::string(" is wielding ") + GetWielded()->CNAME(INDEFINITE);
	    }

	  Msg += " and";
	}
      else if(GetTorsoArmor())
	{
	  if(GetTeam() == game::GetPlayer()->GetTeam())
	    Msg += ",";

	  Msg += std::string(" is wearing ") + GetTorsoArmor()->CNAME(INDEFINITE) + " and";
	}
      else
	if(GetTeam() == game::GetPlayer()->GetTeam())
	  Msg += " and";

      if(GetTeam() == game::GetPlayer()->GetTeam())
	ADD_MESSAGE("%s is tame.", Msg.c_str());
      else
	{
	  uchar Relation = GetTeam()->GetRelation(game::GetPlayer()->GetTeam());

	  if(Relation == HOSTILE)
	    ADD_MESSAGE("%s is hostile.", Msg.c_str());
	  else if(Relation == NEUTRAL)
	    ADD_MESSAGE("%s does not care about you.", Msg.c_str());
	  else
	    ADD_MESSAGE("%s is friendly.", Msg.c_str());
	}
    }

  if(game::GetWizardMode())
    ADD_MESSAGE("(danger: %d)", CurrentDanger());
}

void character::TestWalkability()
{
  if(!GetSquareUnder()->GetIsWalkable(this))
    {
      bool Alive = false;

      while(true)
	{
	  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentArea()->GetXSize(), game::GetCurrentArea()->GetYSize(),
	  {
	    vector2d Where(DoX, DoY);

	    if(GetSquareUnder()->GetAreaUnder()->GetSquare(Where)->GetIsWalkable(this) && !GetSquareUnder()->GetAreaUnder()->GetSquare(Where)->GetCharacter()) 
	      {
		ADD_MESSAGE("%s.", GetSquareUnder()->SurviveMessage(this).c_str());
		Move(Where, true); // actually, this shouldn't be a teleport move
		Alive = true;
		break;
	      }
	  });

	  break;
	}

      if(!Alive)
	{
	  if(GetIsPlayer())
	    {
	      GetSquareUnder()->RemoveCharacter();
	      ADD_MESSAGE("%s.", GetSquareUnder()->DeathMessage(this).c_str());
	      game::DrawEverything();
	      GETKEY();

	      game::RemoveSaves();

	      if(!game::GetWizardMode())
		{
		  game::GetPlayer()->AddScoreEntry(GetSquareUnder()->ScoreEntry(this));
		  highscore HScore;
		  HScore.Draw();
		}

	      game::Quit();
	    }
	  else
	    {
	      ADD_MESSAGE("%s.", GetSquareUnder()->MonsterDeathVerb(this).c_str());
	      Die();
	    }
	}
    }
}

void character::CreateBodyParts()
{
  CreateTorso();
}

material* character::CreateTorsoBone(ulong Volume) const
{
  return new bone(Volume);
}

void character::SetSize(ushort Size)
{
  GetTorso()->SetSize(Size);
}

ushort character::GetSize() const
{
  return GetTorso()->GetSize();
}

void character::CreateTorso()
{
  SetTorso(new normaltorso(false, false));
  GetTorso()->SetBitmapPos(GetBitmapPos());
  GetTorso()->InitMaterials(2, CreateTorsoFlesh(TorsoVolume() * (100 - TorsoBonePercentile()) / 100), CreateTorsoBone(TorsoVolume() * TorsoBonePercentile() / 100));
}

torso* character::GetTorso() const { return (torso*)GetBodyPart(0); }
void character::SetTorso(torso* What) { SetBodyPart(0, What); }

void character::SetMaterial(uchar Index, material* NewMaterial)
{
  /* No error handling here... */

  NewMaterial->SetVolume(GetBodyPart(0)->GetMaterial(Index)->GetVolume());

  GetBodyPart(0)->SetMaterial(Index, NewMaterial);

  for(ushort c = 1; c < BodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone();
      NewMaterial->SetVolume(GetBodyPart(c)->GetMaterial(Index)->GetVolume());
      GetBodyPart(c)->SetMaterial(Index, NewMaterial);
    }
}

void character::Teleport()
{
  Move(game::GetCurrentLevel()->RandomSquare(this, true), true);
}

bool character::SecretKnowledge()
{
  felist List("Knowledge of the ancients:", WHITE, 0, true);

  std::string Buffer = "Name                                                 Weight       SV     Str";
  List.AddDescription(Buffer);

  for(ushort c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
    if(protocontainer<item>::GetProto(c)->AutoInitializable())
      {
	item* Item = protocontainer<item>::GetProto(c)->Clone();
	Buffer = Item->Name(INDEFINITE);
	Buffer.resize(50,' ');
	Buffer += int(Item->GetWeight());
	Buffer.resize(63, ' ');
	Buffer += int(Item->StrengthValue());
	Buffer.resize(70, ' ');
	Buffer += int(Item->GetWeaponStrength() / 100);
	List.AddEntry(Buffer, RED);
	delete Item;
      }

  List.Draw(false);
  return false;
}

bool character::AssignName()
{
  vector2d Where = game::PositionQuestion("What do you want to name? (choose a square and press space bar or press ESC to quit)", GetPos());
  if(Where == vector2d(-1,-1) || Where == GetPos())
    return false;
  character* Character = game::GetCurrentLevel()->GetLSquare(Where)->GetCharacter();
  if(Character && Character->GetSquareUnder()->CanBeSeen())
    {
      if(Character->CanBeAssignedAName())
	{
	  std::string Topic = std::string("What do you want to call this ") + Character->Name(UNARTICLED);
	  std::string Name = game::StringQuestion(Topic, vector2d(7,7), WHITE, 0, 80, true);
	  if(Name != "")
	    Character->ReName(Name);
	}
      else
	ADD_MESSAGE("%s refuses to be called anything else but %s", Character->CNAME(DEFINITE), Character->CNAME(DEFINITE));
    }
  return false;
}


std::string character::Name(uchar Case) const
{
  if(AssignedName.size())
    {
      if(ShowClassName()) 
	return AssignedName + " " + NameNormal(DEFINITE, Article(), Adjective());
      else
	return AssignedName;
    }
  else
    return NameNormal(Case, Article(), Adjective());
} 

void character::ReceiveHeal(long Amount)
{
  if(GetIsPlayer())
    ADD_MESSAGE("You feel better.");
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s looks healthier.", CNAME(DEFINITE));

  if(GetHP() + Amount > GetMaxHP())
    SetHP(GetMaxHP());
  else
    SetHP(GetHP() + Amount);
}

void character::Haste(ushort Counter)
{
  if(StateIsActivated(HASTE))
    return;

  if(StateIsActivated(SLOW))
    {
      EndSlow();
      if(GetIsPlayer())
	ADD_MESSAGE("Time seems to go by at the normal rate now.");
      else
	ADD_MESSAGE("%s slows down to the normal pace.", CNAME(DEFINITE));
    }   
  else
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Time slows down to a crawl.");
      else
	if(GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s looks faster!", CNAME(DEFINITE));
      ActivateState(HASTE);
      SetStateCounter(HASTE, Counter);
    }
}

void character::HasteHandler()
{
  if(!(StateCounter[HASTE]--))
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Time seems to go by at the normal rate now.");
      else if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s seems to move at the normal pace now.", CNAME(DEFINITE));
      EndHaste();
    }
}

void character::EndHaste()
{
  if(StateIsActivated(HASTE))
    DeActivateState(HASTE);
}

void character::Slow(ushort Counter)
{
  if(StateIsActivated(SLOW))
    return;
  if(StateIsActivated(HASTE))
    {
      EndHaste();
      if(GetIsPlayer())
	ADD_MESSAGE("Time seems to go by at the normal rate now.");
      else
	ADD_MESSAGE("%s slows down to the normal pace.", CNAME(DEFINITE));
    }
  else
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Everything seems to move much faster now.");
      else
	if(GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s looks slower!", CNAME(DEFINITE));
      ActivateState(SLOW);
      SetStateCounter(SLOW, Counter);
    }
}

void character::SlowHandler()
{
  if(!(StateCounter[SLOW]--))
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Time seems to go by at the normal rate now.");
      else
	ADD_MESSAGE("%s seems to move at the normal pace now.", CNAME(DEFINITE));
      EndHaste();
    }
}

void character::EndSlow()
{
  if(StateIsActivated(SLOW))
    DeActivateState(SLOW);
}

float character::GetAPStateMultiplier() const
{
  if(StateIsActivated(HASTE))
    return 2;
  if(StateIsActivated(SLOW))
    return 0.5;
  
  return 1;
}

uchar character::GetHungerState() const
{
  if(GetNP() > BLOATEDLEVEL)
    return BLOATED;

  if(GetNP() > SATIATEDLEVEL)
    return SATIATED;
  
  if(GetNP() > NOTHUNGERLEVEL)
    return NOTHUNGRY;

  if(GetNP() > HUNGERLEVEL)
    return HUNGRY;
  else
    return VERYHUNGRY;
}
