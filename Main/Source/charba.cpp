#include <queue>
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
#include "slot.h"
#include "actionde.h"
#include "command.h"
#include "proto.h"
#include "save.h"

void (character::*character::StateHandler[STATES])() = { &character::PolymorphHandler, &character::HasteHandler, &character::SlowHandler };

character::character() : Stack(new stack(0, HIDDEN)), NP(25000), AP(0), StrengthExperience(0), EnduranceExperience(0), AgilityExperience(0), PerceptionExperience(0), IsPlayer(false), State(0), Team(0), WayPoint(-1, -1), Money(0), HomeRoom(0), Action(0)
{
  SetHasBe(true);
}

/*character::character(bool MakeBodyParts, bool SetStats, bool CreateEquipment, bool AllocBodyParts) : Stack(new stack(0, HIDDEN)), NP(25000), AP(0), StrengthExperience(0), EnduranceExperience(0), AgilityExperience(0), PerceptionExperience(0), IsPlayer(false), State(0), Team(0), WayPoint(-1, -1), Money(0), HomeRoom(0), Action(0)
{
  SetHasBe(true);

  if(MakeBodyParts || SetStats || CreateEquipment || AllocBodyParts)
    ABORT("BOOO!");
}*/

character::~character()
{
  if(GetAction())
    {
      GetAction()->DeleteUsedItems();
      delete GetAction();
    }

  if(GetTeam())
    GetTeam()->Remove(GetTeamIterator());

  delete Stack;

  if(GetTorso())
    GetTorso()->SetExists(false);

  delete [] BodyPartSlot;
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
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return true;
    }

  switch(Enemy->TakeHit(this, 0, GetAttackStrength(), GetToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % Enemy->GetCriticalModifier()))) //there's no breaks and there shouldn't be any
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case HAS_DIED:
      EditStrengthExperience(50);
    case HAS_DODGED:
      EditAgilityExperience(25);
    }

  EditNP(-50);
  return true;
}

uchar character::TakeHit(character* Enemy, item* Weapon, float AttackStrength, float ToHitValue, short Success, bool Critical)
{
  DeActivateVoluntaryStates(Enemy->Name(DEFINITE) + " seems to be hostile.");
  uchar Dir = game::GetDirectionForVector(GetPos() - Enemy->GetPos());

  if(Critical)
    {
      ushort Damage = ushort(AttackStrength * Enemy->GetStrength() * (1 + float(Success) / 100) / 25000) + (RAND() % 3 ? 2 : 1);
      uchar BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, GetDodgeValue());
      Enemy->AddHitMessage(this, Weapon, BodyPart, true);
      ReceiveBodyPartDamage(this, Damage, PHYSICALDAMAGE, BodyPart, Dir, false, true);

      if(game::GetWizardMode() && GetLSquareUnder()->CanBeSeen(true))
      	ADD_MESSAGE("(damage: %d)", Damage);

      if(CheckDeath("killed by " + Enemy->Name(INDEFINITE), Enemy->GetIsPlayer()))
	return HAS_DIED;

      return HAS_HIT;
    }

  if(RAND() % ushort(100 + Enemy->GetToHitValue() / GetDodgeValue() * (100 + Success)) >= 100)
    {
      ushort Damage = ushort(AttackStrength * Enemy->GetStrength() * (1 + float(Success) / 100) / 50000) + (RAND() % 3 ? 1 : 0);
      uchar BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, GetDodgeValue());
      Enemy->AddHitMessage(this, Weapon, BodyPart);

      if(!ReceiveBodyPartDamage(this, Damage, PHYSICALDAMAGE, BodyPart, Dir))
	return HAS_BLOCKED;

      if(CheckDeath("killed by " + Enemy->Name(INDEFINITE), Enemy->GetIsPlayer()))
	return HAS_DIED;

      return HAS_HIT;
    }
  else
    {
      Enemy->AddMissMessage(this);
      EditAgilityExperience(50);
      EditPerceptionExperience(25);
      return HAS_DODGED;
    }
}

struct svpriorityelement
{
  svpriorityelement(uchar BodyPart, uchar StrengthValue) : BodyPart(BodyPart), StrengthValue(StrengthValue) { }
  bool operator < (const svpriorityelement& AnotherPair) const { return StrengthValue > AnotherPair.StrengthValue; }
  uchar BodyPart;
  ushort StrengthValue;
};

uchar character::ChooseBodyPartToReceiveHit(float ToHitValue, float DodgeValue)
{
  if(BodyParts() == 1)
    return 0;

  std::priority_queue<svpriorityelement> SVQueue;

  ulong TotalVolume = 0;

  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      {
	SVQueue.push(svpriorityelement(c, GetBodyPart(c)->GetStrengthValue() + GetBodyPart(c)->GetHP()));
	TotalVolume += GetBodyPart(c)->GetVolume();
      }

  while(SVQueue.size())
    {
      ushort ToHitPercentage = ushort(ToHitValue * GetBodyPart(SVQueue.top().BodyPart)->GetVolume() * 100 / (DodgeValue * TotalVolume));

      if(ToHitPercentage < 1)
	ToHitPercentage = 1;

      if(ToHitPercentage > RAND() % 100)
	return SVQueue.top().BodyPart;

      SVQueue.pop();
    }

  return 0;
}

void character::Be()
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
	(this->*StateHandler[c])();

      if(GetAction())
	GetAction()->Handle();

      if(!IsEnabled())
	return;

      if(GetHP() < GetMaxHP() / 3)
	SpillBlood(RAND() % 2);

      if(GetIsPlayer() && GetHungerState() == VERYHUNGRY && !(RAND() % 50) && (!GetAction() || GetAction()->AllowFaint()))
	{
	  if(GetAction())
	    GetAction()->Terminate(false);

	  Faint();
	}
    }

  if(GetAP() >= 0)
    ActionAutoTermination();

  if(GetAP() >= 0)
    {
      if(GetIsPlayer())
	{
	  static ushort Timer = 0;

	  if(configuration::GetAutoSaveInterval() && !GetAction() && ++Timer >= configuration::GetAutoSaveInterval())
	    {
	      game::Save(game::GetAutoSaveFileName().c_str());
	      Timer = 0;
	    }

	  if(!GetAction())
	    GetPlayerCommand();
	  else
	    {
	      game::DrawEverything();

	      if(READKEY() && GetAction()->IsVoluntary())
		GetAction()->Terminate(false);
	    }
	}
      else
	{
	  if(!GetAction() && !game::GetInWilderness())
	    GetAICommand();
	}

      EditAP(-1000);
    }

  CharacterSpeciality();
  Regenerate();

  if(GetIsPlayer())
    {
      if(!GetAction() || GetAction()->AllowFoodConsumption())
	Hunger();
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
  if(CanOpen())
    {
      int Key;

      bool OpenableItems = GetStack()->SortedItems(this, &item::OpenableSorter);

      if(OpenableItems)
	Key = game::AskForKeyPress("Which door do you wish to open?  [press a direction key, space or i]");
      else
	Key = game::AskForKeyPress("Which door do you wish to open?  [press a direction key or space]");

      if(Key == 'i' && OpenableItems)
	return OpenItem();

      vector2d DirVect = game::GetDirectionVectorForKey(Key);

      if(DirVect != vector2d(0, 0))
	return OpenPos(GetPos() + DirVect);
    }
  else
    ADD_MESSAGE("This monster type cannot open anything.");

  return false;
}

bool character::Close()
{
  if(CanOpen())
    {
      uchar Dir = game::DirectionQuestion("Which door do you wish to close?  [press a direction key or space]", 0xFF, false);

      if(Dir != 0xFF)
	{
	  vector2d DirVect = game::GetMoveVector(Dir);
	  return ClosePos(GetPos() + DirVect);
	}
    }
  else
    ADD_MESSAGE("This monster type cannot close anything.");

  return false;
}

bool character::Drop()
{
  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to drop!");
      return false;
    }

  item* Item = GetStack()->DrawContents(this, "What do you want to drop?");

  if(Item)
    if(!GetLSquareUnder()->GetRoom() || (GetLSquareUnder()->GetRoom() && GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->DropItem(this, Item)))
      {
	Item->MoveTo(GetLSquareUnder()->GetStack());
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

  if((game::GetInWilderness() || !GetLSquareUnder()->GetStack()->SortedItems(this, &item::ConsumableSorter)) && !GetStack()->SortedItems(this, &item::ConsumableSorter))
    {
      ADD_MESSAGE("You have nothing to consume!");
      return false;
    }

  item* Item;

  if(!game::GetInWilderness() && GetLSquareUnder()->GetStack()->SortedItems(this, &item::ConsumableSorter))
      Item = GetStack()->DrawContents(GetLSquareUnder()->GetStack(), this, "What do you wish to consume?", "Items in your inventory", "Items on ground", &item::ConsumableSorter);
  else
      Item = GetStack()->DrawContents(this, "What do you wish to consume?", &item::ConsumableSorter);

  if(Item)
    {
      if(CheckBulimia() && !game::BoolQuestion("You think your stomach will burst if you eat anything more. Force it down? [y/N]"))
	return false;

      ConsumeItem(Item);
      return true;
    }

  return false;
}

bool character::ConsumeItem(item* Item)
{
  Item = Item->PrepareForConsuming(this);

  if(!Item)
    return false;

  consume* Consume = new consume(this);
  Consume->SetWasOnGround(Item->IsOnGround());
  Consume->SetConsuming(Item);
  SetAction(Consume);
  return true;
}

bool character::CheckBulimia() const
{
  return GetHungerState() == BLOATED;
}

void character::Move(vector2d MoveTo, bool TeleportMove)
{
  if(GetBurdenState() != OVERLOADED || TeleportMove)
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

void character::DrawToTileBuffer(bool Animate) const
{
  if(GetTorso())
    GetTorso()->DrawToTileBuffer(Animate);
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
		if(CanOpen())
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
	    if(configuration::GetAutoSaveInterval())
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
  GetStack()->DrawContents(this, "Your inventory", false);
  return true;
}

bool character::PickUp()
{
  bool ToBeReturned = false;

  if(GetLSquareUnder()->GetStack()->GetItems() > 0)
    if(GetLSquareUnder()->GetStack()->GetItems() > 1)
      {
	for(;;)
	  {
	    item* Item = GetLSquareUnder()->GetStack()->DrawContents(this, "What do you want to pick up?");

	    if(Item)
		if(!GetLSquareUnder()->GetRoom() || (GetLSquareUnder()->GetRoom() && GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, Item)))
		  {
		    ADD_MESSAGE("%s picked up.", Item->CHARNAME(INDEFINITE));
		    Item->MoveTo(GetStack());
		    Item->CheckPickUpEffect(this);
		    ToBeReturned = true;
		  }

	    if(!Item || !GetLSquareUnder()->GetStack()->GetItems())
	      break;

	    game::DrawEverythingNoBlit();
	  }
      }
    else
      {
	item* Item = GetLSquareUnder()->GetStack()->GetBottomItem();

	if(!GetLSquareUnder()->GetRoom() || (GetLSquareUnder()->GetRoom() && GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, Item)))
	  {
	    ADD_MESSAGE("%s picked up.", Item->CHARNAME(INDEFINITE));
	    Item->MoveTo(GetStack());
	    Item->CheckPickUpEffect(this);
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
  corpse* Corpse = new corpse;
  Corpse->SetDeceased(this);
  GetLSquareUnder()->GetStack()->AddItem(Corpse);
  SetHasBe(false);
}

void character::Die(bool ForceMsg)
{
  // Note for programmers: This function MUST NOT delete any objects!

  if(!IsEnabled())
    return;

  if(GetIsPlayer())
    {
      ADD_MESSAGE("You die.");

      if(game::GetWizardMode())
	{
	  game::DrawEverything();

	  if(!game::BoolQuestion("Do you want to do this, cheater? [y/n]", 2))
	    {
	      RestoreBodyParts();
	      RestoreHP();
	      SetNP(10000);
	      GetSquareUnder()->SendNewDrawRequest();
	      return;
	    }
	}

      game::RemoveSaves();
    }
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE(GetDeathMessage().c_str());
    else
      if(ForceMsg)
	ADD_MESSAGE("You sense the death of something.");

  if(HomeRoom && GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetMaster() == this)
    GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->SetMaster(0);

  GetSquareUnder()->RemoveCharacter();

  if(!game::GetInWilderness())
    CreateCorpse();

  if(GetIsPlayer())
    {
      if(!game::GetInWilderness())
	GetLSquareUnder()->SetTemporaryEmitation(GetEmitation());

      if(GetStack()->GetItems())
	if(game::BoolQuestion("Do you want to see your inventory? [y/n]", 2))
	  GetStack()->DrawContents(this, "Your inventory", false);

      if(game::BoolQuestion("Do you want to see your message history? [y/n]", 2))
	DrawMessageHistory();

      if(!game::GetInWilderness())
	GetLSquareUnder()->SetTemporaryEmitation(0);
    }

  if(!game::GetInWilderness())
    {
      lsquare* Square = GetLSquareUnder();

      while(GetStack()->GetItems())
	GetStack()->MoveItem(GetStack()->GetBottomSlot(), Square->GetStack());

      for(ushort c = 0; c < BodyParts(); ++c)
	if(GetBodyPart(c))
	  GetBodyPart(c)->DropEquipment();

      if(GetAction())
	{
	  GetAction()->DropUsedItems();
	  delete GetAction();
	  SetAction(0);
	}
    }
  else
    {
      /* Drops the equipment to the character's stack */

      for(ushort c = 0; c < BodyParts(); ++c)
	if(GetBodyPart(c))
	  GetBodyPart(c)->DropEquipment();

      if(GetAction())
	{
	  GetAction()->DeleteUsedItems();
	  delete GetAction();
	  SetAction(0);
	}

      while(GetStack()->GetItems())
	{
	  GetStack()->GetBottomItem()->SetExists(false);
	  GetStack()->RemoveItem(GetStack()->GetBottomSlot());
	}
    }

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(0);

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
  item* Item = Stack->DrawContents(this, "What do you want to open?", &item::OpenableSorter);

  if(Item)
    if(Item->TryToOpen(this))
      {
	EditAgilityExperience(25);
	EditNP(-10);
	EditAP(-500);
	return true;
      }
    else
      return false;

  return false;
}

void character::AddMissMessage(character* Enemy) const
{
  std::string Msg;

  if(Enemy->GetIsPlayer())
    Msg = Description(DEFINITE) + " misses you!";
  else if(GetIsPlayer())
    Msg = "You miss " + Enemy->Description(DEFINITE) + "!";
  else if(GetSquareUnder()->CanBeSeen() || Enemy->GetSquareUnder()->CanBeSeen())
    Msg = Description(DEFINITE) + " misses " + Enemy->Description(DEFINITE) + "!";
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::AddHitMessage(character* Enemy, item* Weapon, uchar BodyPart, bool Critical) const
{
  std::string Msg;
  std::string BodyPartDescription = BodyPart && Enemy->GetSquareUnder()->CanBeSeen() ? " in " + Enemy->GetBodyPart(BodyPart)->Name(DEFINITE) : "";

  if(Enemy->GetIsPlayer())
    {
      if(Weapon && GetSquareUnder()->CanBeSeen())
	Msg = Description(DEFINITE) + " " + ThirdPersonWeaponHitVerb(Critical) + " you" + BodyPartDescription + " with " + PossessivePronoun() + " " + Weapon->Name(UNARTICLED) + "!";
      else
	Msg = Description(DEFINITE) + " " + ThirdPersonMeleeHitVerb(Critical) + " you" + BodyPartDescription + "!";
    }
  else if(GetIsPlayer())
    Msg = "You " + FirstPersonHitVerb(Enemy, Critical) + " " + Enemy->Description(DEFINITE) + BodyPartDescription + "!";
  else if(GetSquareUnder()->CanBeSeen() || Enemy->GetSquareUnder()->CanBeSeen())
    Msg = Description(DEFINITE) + " " + AICombatHitVerb(Enemy, Critical) + " " + Enemy->Description(DEFINITE) + BodyPartDescription + "!";
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s %s.", CHARDESCRIPTION(DEFINITE), GetTalkVerb().c_str());
}

bool character::Talk()
{
  character* ToTalk = 0;
  ushort Characters = 0;

  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentArea()->GetXSize(), game::GetCurrentArea()->GetYSize(),
    {
      character* Char = game::GetCurrentArea()->GetSquare(DoX, DoY)->GetCharacter();

      if(Char)
	{
	  ToTalk = Char;
	  ++Characters;
	}
    });

  if(!Characters)
    {
      ADD_MESSAGE("Find yourself someone to talk to first!");
      return false;
    }
  else if(Characters == 1)
    {
      TalkTo(ToTalk);
      return true;
    }
  else
    {
      uchar Dir = game::DirectionQuestion("To whom do you wish to talk to? [press a direction key]", 0xFF, false, true);

      if(Dir == 0xFF)
	return false;

      if(Dir == YOURSELF)
	{
	  ADD_MESSAGE("You talk to yourself for some time.");
	  return true;
	}

      character* Char = game::GetCurrentArea()->GetSquare(GetPos() + game::GetMoveVector(Dir))->GetCharacter();

      if(Char)
	{
	  TalkTo(Char);
	  return true;
	}
      else
	ADD_MESSAGE("You get no response.");
    }

  return false;
}

void character::TalkTo(character* Who)
{
  if(Who->GetTeam()->GetRelation(GetTeam()) != HOSTILE)
    Who->EditAP(-1000);

  Who->BeTalkedTo(this);
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
	  ADD_MESSAGE("Suddenly %s looks stronger.", CHARNAME(DEFINITE));

      SetStrength(GetStrength() + 1);
      SetStrengthExperience(0);
    }

  if(GetStrengthExperience() < -pow(1.18, long(100 - GetStrength())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You collapse under your load.");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks weaker.", CHARNAME(DEFINITE));

      SetStrength(GetStrength() - 1);
      SetStrengthExperience(0);
    }

  if(GetEnduranceExperience() > pow(1.18, long(GetEndurance())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You feel Valpurus's toughness around you!");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks tougher.", CHARNAME(DEFINITE));

      SetEndurance(GetEndurance() + 1);
      SetEnduranceExperience(0);
    }

  if(GetEnduranceExperience() < -pow(1.18, long(100 - GetEndurance())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You feel less healthy.");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks less tough.", CHARNAME(DEFINITE));

      SetEndurance(GetEndurance() - 1);
      SetEnduranceExperience(0);
    }

  if(GetAgilityExperience() > pow(1.18, long(GetAgility())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Your agility challenges even Valpurus's angels!");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks faster.", CHARNAME(DEFINITE));

      SetAgility(GetAgility() + 1);
      SetAgilityExperience(0);
    }

  if(GetAgilityExperience() < -pow(1.18, long(100 - GetAgility())) * 193)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You seem as fast as a flat mommo.");
      else
	if(game::GetInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks slower.", CHARNAME(DEFINITE));

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
  for(stackiterator i = GetStack()->GetBottomSlot(); i != GetStack()->GetSlotAboveTop(); ++i)
    if((**i)->IsHeadOfElpuri())
      return true;

  return false;
}

bool character::HasPetrussNut() const
{
  for(stackiterator i = GetStack()->GetBottomSlot(); i != GetStack()->GetSlotAboveTop(); ++i)
    if((**i)->IsPetrussNut())
      return true;

  return false;
}

bool character::HasGoldenEagleShirt() const
{
  for(stackiterator i = GetStack()->GetBottomSlot(); i != GetStack()->GetSlotAboveTop(); ++i)
    if((**i)->IsGoldenEagleShirt())
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

  item* Item = GetStack()->DrawContents(this, "What do you want to read?", &item::ReadableSorter);

  if(Item)
    return ReadItem(Item);
  else
    return false;
}

bool character::ReadItem(item* ToBeRead)
{
  if(ToBeRead->CanBeRead(this))
    if(GetLSquareUnder()->GetLuminance() >= LIGHT_BORDER || game::GetSeeWholeMapCheat())
      {
	bool LackOfLight = GetLSquareUnder()->GetRawLuminance() < 225 ? true : false;

	if(ToBeRead->Read(this))
	  {
	    ToBeRead->RemoveFromSlot();
	    ToBeRead->SetExists(false);

	    if(LackOfLight)
	      SetPerceptionExperience(-25);
	  }

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
    SumOfMasses = GetTotalWeight();
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

ulong character::GetTotalWeight() const
{
  ulong Weight = GetStack()->GetTotalWeight();

  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      Weight += GetBodyPart(c)->GetTotalWeight();

  if(GetAction())
    Weight += GetAction()->GetWeight();

  return Weight;
}

bool character::Dip()
{
  if(!GetStack()->SortedItems(this, &item::DippableSorter))
    {
      ADD_MESSAGE("You have nothing to dip!");
      return false;
    }

  item* Item = GetStack()->DrawContents(this, "What do you want to dip?", &item::DippableSorter);

  if(Item)
    {
      bool HasDipDestination = GetStack()->SortedItems(this, &item::DipDestinationSorter);

      if(!HasDipDestination || game::BoolQuestion("Do you wish to dip in a nearby square? [y/N]"))
	{
	  vector2d VectorDir;
	  std::string Question = "Where do you want to dip " + Item->Name(DEFINITE) + std::string("? [press a cursor key or . for the current square]");
	  uchar Dir = game::DirectionQuestion(Question, 0xFF, false, true);

	  if(Dir == 0xFF)
	    return false;

	  if(Dir == YOURSELF)
	    VectorDir = vector2d(0, 0);
	  else
	    VectorDir = game::GetMoveVector(Dir);

	  if(!game::IsValidPos(GetPos() + VectorDir))
	    {
	      ADD_MESSAGE("Invalid square!");
	      return false;
	    }
	  
	  return game::GetCurrentLevel()->GetLSquare(GetPos() + VectorDir)->DipInto(Item, this);
	}
      else
	{
	  if(!HasDipDestination)
	    {
	      ADD_MESSAGE("You have nothing to dip into!");
	      return false;
	    }

	  item* DipTo = GetStack()->DrawContents(this, "Into what do you wish to dip it?", &item::DipDestinationSorter);

	  if(DipTo && Item != DipTo)
	    {
	      Item->DipInto(DipTo->CreateDipMaterial(), this);
	      return true;
	    }
	}
    }

  return false;
}

void character::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  entity::Save(SaveFile);

  Stack->Save(SaveFile);

  SaveFile << Strength << Endurance << Agility << Perception;
  SaveFile << NP << AP;
  SaveFile << StrengthExperience << EnduranceExperience << AgilityExperience << PerceptionExperience;
  SaveFile << State << Money << HomeRoom << WayPoint;
  SaveFile << GetHasBe();

  ushort c;

  for(c = 0; c < BodyParts(); ++c)
    SaveFile << BodyPartSlot[c];

  if(HomeRoom)
    if(!game::GetInWilderness() && GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetMaster() == this)
      SaveFile << bool(true);
    else
      SaveFile << bool(false);

  SaveFile << Action;

  for(c = 0; c < STATES; ++c)
    SaveFile << StateCounter[c];

  if(GetTeam())
    {
      SaveFile << bool(true);
      SaveFile << Team->GetID();
    }
  else
    SaveFile << bool(false);

  if(GetTeam() && GetTeam()->GetLeader() == this)
    SaveFile << bool(true);
  else
    SaveFile << bool(false);

  SaveFile << AssignedName;
}

void character::Load(inputfile& SaveFile)
{
  entity::Load(SaveFile);

  if(Stack)
    delete Stack;

  Stack = new stack;
  Stack->Load(SaveFile);

  SaveFile >> Strength >> Endurance >> Agility >> Perception;
  SaveFile >> NP >> AP;
  SaveFile >> StrengthExperience >> EnduranceExperience >> AgilityExperience >> PerceptionExperience;
  SaveFile >> State >> Money >> HomeRoom >> WayPoint;
  SetHasBe(ReadType<bool>(SaveFile));

  ushort c;

  for(c = 0; c < BodyParts(); ++c)
      SaveFile >> BodyPartSlot[c];

  if(HomeRoom)
    if(ReadType<bool>(SaveFile))
      GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->SetMaster(this);

  SaveFile >> Action;

  for(c = 0; c < STATES; ++c)
    SaveFile >> StateCounter[c];

  if(ReadType<bool>(SaveFile))
    SetTeam(game::GetTeam(ReadType<ushort>(SaveFile)));

  if(ReadType<bool>(SaveFile))
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
	  ADD_MESSAGE("Wizard mode activated.");

	  for(ushort x = 0; x < 5; ++x)
	    GetStack()->AddItem(new scrollofwishing);
	}
    }
  else
    {
      ADD_MESSAGE("Got some scrolls of wishing.");

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
  RestoreHP();
  game::SendLOSUpdateRequest();
  return false;
}

bool character::LowerStats()
{
  Strength -= 10;
  Endurance -= 10;
  Agility -= 10;
  Perception -= 10;
  RestoreHP();
  game::SendLOSUpdateRequest();
  return false;
}

bool character::GainAllItems()
{
  for(ushort c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
    if(protocontainer<item>::GetProto(c)->IsAutoInitializable())
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
  ushort Emitation = GetBaseEmitation();

  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->GetEmitation() > Emitation)
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
  felist List("Keyboard Layout", WHITE, 0);

  List.AddDescription("");
  List.AddDescription("Key       Description");

  for(ushort c = 1; game::GetCommand(c); ++c)
    if(game::GetWizardMode() || !game::GetCommand(c)->GetWizardModeFunction())
      {
	std::string Buffer;
	Buffer += game::GetCommand(c)->GetKey();
	Buffer.resize(10, ' ');
	List.AddEntry(Buffer + game::GetCommand(c)->GetDescription(), LIGHTGRAY);
      }

  List.Draw(vector2d(26, 42), 652, 30, false);

  return false;
}

bool character::Look()
{
  vector2d Pos = game::PositionQuestion("Press direction keys to move cursor or esc to exit from the mode.", GetPos(), &game::LookHandler);

  if(game::GetWizardMode() && Pos != vector2d(-1, -1))
    {
      character* Char = game::GetCurrentArea()->GetSquare(Pos)->GetCharacter();

      if(Char)
	Char->PrintInfo();
    }

  EditPerceptionExperience(1);
  EditAP(900);
  return true;
}

float character::GetAttackStrength() const
{
  return GetMeleeStrength();
}

bool character::Engrave(const std::string& What)
{
  game::GetCurrentLevel()->GetLSquare(GetPos())->Engrave(What);
  return true;
}

bool character::WhatToEngrave()
{
  game::GetCurrentLevel()->GetLSquare(GetPos())->Engrave(game::StringQuestion("What do you want to engrave here?", vector2d(16, 6), WHITE, 0, 80, true));
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
  if(GetBurdenState(ToBeTested->GetWeight() + GetTotalWeight()) != UNBURDENED)
    return false;

  return true;
}

bool character::OpenPos(vector2d APos)
{
  if(game::IsValidPos(APos) && game::GetCurrentLevel()->GetLSquare(APos)->Open(this))
    {
      EditAgilityExperience(25);
      EditNP(-10);
      EditAP(GetLengthOfOpen(APos));
      return true;
    }
  else
    return false;
}

bool character::ClosePos(vector2d APos)
{
  if(game::IsValidPos(APos) && game::GetCurrentLevel()->GetLSquare(APos)->Close(this))
    {
      EditAgilityExperience(25);
      EditNP(-10);
      EditAP(GetLengthOfClose(APos));
      return true;
    }
  else
    return false;
}

bool character::Pray()
{
  felist Panthenon("To Whom shall thee address thine prayers?", WHITE, 0);

  std::vector<uchar> KnownIndex;

  if(!GetLSquareUnder()->GetDivineMaster())
    {
      for(ushort c = 1; c < game::GetGods(); ++c)
	if(game::GetGod(c)->GetKnown())
	  {
	    bitmap Icon(igraph::GetSymbolGraphic(), c << 4, 0, 16, 16);
	    Panthenon.AddEntry(game::GetGod(c)->CompleteDescription(), LIGHTGRAY, &Icon);
	    KnownIndex.push_back(c);
	  }
    }
  else
    if(game::GetGod(GetLSquareUnder()->GetDivineMaster())->GetKnown())
      {
	bitmap Icon(igraph::GetSymbolGraphic(), GetLSquareUnder()->GetDivineMaster() << 4, 0, 16, 16);
	Panthenon.AddEntry(game::GetGod(GetLSquareUnder()->GetDivineMaster())->CompleteDescription(), LIGHTGRAY, &Icon);
      }
    else
      ADD_MESSAGE("Somehow you feel that no deity you know can hear your prayers from this place.");

  ushort Select = Panthenon.Draw(vector2d(26, 42), 652);

  if(Select & 0x8000)
    return false;
  else
    {
      if(GetLSquareUnder()->GetDivineMaster())
	{
	  if(!Select)
	    {
	      if(game::BoolQuestion("Do you really wish to pray to " + game::GetGod(GetLSquareUnder()->GetDivineMaster())->Name() + "? [y/N]"))
		game::GetGod(GetLSquareUnder()->GetDivineMaster())->Pray();
	      else
		return false;
	    }
	  else
	    return false;
	}
      else
	{
	  if(game::BoolQuestion("Do you really wish to pray to " + game::GetGod(KnownIndex[Select])->Name() + "? [y/N]"))
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

bool character::Kick()
{
  uchar Direction = 0;

  if(!CheckKick())
    return false;

  if(GetBurdenState() == OVERLOADED)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You try to kick, but you collapse under your load.");
      return true;
    }

  uchar Dir = game::DirectionQuestion("In what direction do you wish to kick? [press a direction key or space]", 0xFF, false);

  if(Dir == 0xFF)
    return false;

  return game::GetCurrentLevel()->GetLSquare(GetPos() + game::GetMoveVector(Dir))->Kick(this, GetStrength(), Direction);
}

bool character::Offer()
{
  if(!CheckOffer())
    return false;

  if(GetLSquareUnder()->GetOLTerrain()->CanBeOffered())
    {
      if(!GetStack()->GetItems())
	{
	  ADD_MESSAGE("You have nothing to offer!");
	  return false;
	}

      item* Item = GetStack()->DrawContents(this, "What do you want to offer?");

      if(Item)
	{
	  if(game::GetGod(GetLSquareUnder()->GetOLTerrain()->GetDivineMaster())->ReceiveOffer(Item))
	    {
	      Item->RemoveFromSlot();
	      Item->SetExists(false);
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

long character::GetStatScore() const
{
  return GetHP() * 5 + GetEndurance() * 30 + (GetStrength() + GetAgility() + GetPerception()) * 40;
}

long character::GetScore() const
{
  return (game::GetPlayerBackup() ? game::GetPlayerBackup()->GetStatScore() : GetStatScore()) + GetMoney() / 5 + Stack->Score() + game::GodScore();
}

void character::AddScoreEntry(const std::string& Description, float Multiplier, bool AddEndLevel) const
{
  highscore HScore;

  std::string Desc = game::GetPlayer()->GetAssignedName() + ", " + Description;

  if(AddEndLevel)
    Desc += " in " + (game::GetInWilderness() ? "the World map" : game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrent()));

  HScore.Add(long((GetScore() - game::GetBaseScore()) * Multiplier), Desc);

  HScore.Save();
}

bool character::CheckDeath(const std::string& Msg, bool ForceMsg)
{
  bool Dead = false;

  for(ushort c = 0; c < BodyParts(); ++c)
    if(BodyPartVital(c) && (!GetBodyPart(c) || GetBodyPart(c)->GetHP() < 1))
      {
	Dead = true;
	break;
      }

  if(Dead)
    {
      if(GetIsPlayer() && !game::GetWizardMode())
	AddScoreEntry(Msg);

      Die(ForceMsg);

      return true;
    }
  else
    return false;
}

bool character::CheckStarvationDeath(const std::string& Msg)
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
  msgsystem::DrawMessageHistory();
  return false;
}

bool character::Throw()
{
  if(!CheckThrow())
    return false;

  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to throw!");
      return false;
    }

  item* Item = GetStack()->DrawContents(this, "What do you want to throw?");

  if(Item)
    {
      uchar Answer = game::DirectionQuestion("In what direction do you wish to throw?  [press a direction key or space]", 0xFF, false);

      if(Answer == 0xFF)
	return false;

      ThrowItem(Answer, Item);

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

  return ToBeThrown->Fly(this, Direction, GetStrength());
}

void character::HasBeenHitByItem(character* Thrower, item* Thingy, float Speed)
{
  ushort Damage = ushort(Thingy->GetWeaponStrength() * Thingy->GetWeight() * sqrt(Speed) / 5000000000.0f) + (RAND() % 5 ? 1 : 0);
  ReceiveDamage(Thrower, Damage, PHYSICALDAMAGE, ALL);

  if(GetIsPlayer())
    ADD_MESSAGE("%s hits you.", Thingy->CHARNAME(DEFINITE));
  else
    if(GetSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s hits %s.", Thingy->CHARNAME(DEFINITE), CHARNAME(DEFINITE));

  if(game::GetWizardMode() && GetLSquareUnder()->CanBeSeen(true))
    ADD_MESSAGE("(damage: %d) (speed: %f)", Damage, Speed);

  SpillBlood(1 + RAND() % 1);
  CheckDeath("died by a flying " + Thingy->Name(UNARTICLED));
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
      ushort c;

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
	    break;
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
      ADD_MESSAGE("%s vomits.", CHARNAME(DEFINITE));

  EditStrengthExperience(-50);
  EditEnduranceExperience(50);
  EditNP(-200 - RAND() % 201);

  GetLSquareUnder()->ReceiveVomit(this);
}

bool character::Apply()
{
  if(!CheckApply())
    return false;

  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to apply!");
      return false;
    }

  item* Item = GetStack()->DrawContents(this, "What do you want to apply?", &item::AppliableSorter);

  if(Item)
    {
      if(!Item->Apply(this))
	return false;

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

  item* Item = GetStack()->DrawContents(this, "What do you want to zap with?", &item::ZappableSorter);

  if(Item)
    {
      uchar Answer = game::DirectionQuestion("In what direction do you wish to zap?  [press a direction key, space or .]", 0xFF, false, true);

      if(Answer == 0xFF)
	return false;

      return Item->Zap(this, GetPos(), Answer);
    }
  else
    return false;
}

bool character::Polymorph(character* NewForm, ushort Counter)
{
  if(GetAction())
    GetAction()->Terminate(false);

  GetSquareUnder()->RemoveCharacter();
  GetSquareUnder()->AddCharacter(NewForm);
  SetSquareUnder(0);

  if(GetIsPlayer())
    {
      ADD_MESSAGE("Your body glows in a crimson light. You transform into %s!", NewForm->CHARNAME(INDEFINITE));
      game::SetPlayer(NewForm);
      NewForm->SetAssignedName(GetAssignedName());
      NewForm->ActivateState(POLYMORPHED);
      NewForm->SetStateCounter(POLYMORPHED, Counter);
      game::SendLOSUpdateRequest();
      game::GetCurrentArea()->UpdateLOS();

      if(StateIsActivated(POLYMORPHED))
	{
	  SetExists(false);
	}
      else
	{
	  game::SetPlayerBackup(this);
	  SetHasBe(false);
	}
    }
  else
    SetExists(false);

  while(GetStack()->GetItems())
    GetStack()->MoveItem(GetStack()->GetBottomSlot(), NewForm->GetStack());

  NewForm->SetMoney(GetMoney());

  for(ushort c = 0; c < EquipmentSlots(); ++c)
    {
      item* Item = GetEquipment(c);

      if(Item)
	{
	  if(NewForm->CanUseEquipment(c))
	    {
	      Item->RemoveFromSlot();
	      NewForm->SetEquipment(c, Item);
	    }
	  else
	    Item->MoveTo(NewForm->GetStack());
	}
    }

  /*if(NewForm->CanWieldInMainHand())
    NewForm->SetMainWielded(GetMainWielded());

  if(NewForm->CanWieldInSecondaryHand())
    NewForm->SetSecondaryWielded(GetSecondaryWielded());

  if(NewForm->CanWear())
    NewForm->SetBodyArmor(GetBodyArmor());*/

  NewForm->ChangeTeam(GetTeam());

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(NewForm);

  //ChangeTeam(0);

  return true;
}

wsquare* character::GetWSquareUnder() const
{
  return (wsquare*)SquareUnder;
}

/*ulong character::GetBloodColor() const
{
  return MAKE_RGB(75,0,0);
}*/

void character::BeKicked(character* Kicker, ushort KickStrength, uchar Direction)
{
  /*if(RAND() % 10 + RAND() % 3 * KickStrength / 2 > GetAgility())
    {
      if(KickStrength > 8 + RAND() % 4 - RAND() % 4)
	{
	  if(ShowOnScreen)
	    if(GetIsPlayer())
	      ADD_MESSAGE("The kick throws you off balance.");
	    else
	      ADD_MESSAGE("The kick throws %s off balance.", CHARNAME(DEFINITE));

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
	      ADD_MESSAGE("The kick hits %s.", CHARNAME(DEFINITE));

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
	ADD_MESSAGE("The kick misses %s.", CHARNAME(DEFINITE));*/
}

void character::FallTo(vector2d Where, bool OnScreen)
{
  /*EditAP(-500);

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
    }*/

  // Place code that handles characters bouncing to each other here
}

bool character::CheckCannibalism(ushort What) const
{ 
  return GetTorso()->GetConsumeMaterial()->GetType() == What; 
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
      ADD_MESSAGE("%s faints.", CHARNAME(DEFINITE));

  faint* Faint = new faint(this);
  Faint->SetCounter(100 + RAND() % 101);
  SetAction(Faint);
}

void character::PolymorphHandler()
{
  if(StateIsActivated(POLYMORPHED))
    {
      if(!GetStateCounter(POLYMORPHED))
	{
	  ADD_MESSAGE("You return to your true form.");
	  EndPolymorph();
	}

      EditStateCounter(POLYMORPHED, -1);
    }
}

void character::EndPolymorph()
{
  if(StateIsActivated(POLYMORPHED))
    {
      if(GetAction())
	GetAction()->Terminate(false);

      SetExists(false);
      GetSquareUnder()->RemoveCharacter();
      character* Player = game::GetPlayerBackup();
      GetSquareUnder()->AddCharacter(Player);
      Player->SetHasBe(true);
      SetSquareUnder(0);

      while(GetStack()->GetItems())
	GetStack()->MoveItem(GetStack()->GetBottomSlot(), Player->GetStack());

      SetSquareUnder(Player->GetSquareUnder()); // might be used afterwards

      for(ushort c = 0; c < EquipmentSlots(); ++c)
	{
	  item* Item = GetEquipment(c);

	  if(Item)
	    {
	      if(Player->CanUseEquipment(c))
		{
		  Item->RemoveFromSlot();
		  Player->SetEquipment(c, Item);
		}
	      else
		Item->MoveTo(Player->GetStack());
	    }
	}

      /*if(Player->CanWieldInMainHand())
	Player->SetMainWielded(GetMainWielded());

      if(Player->CanWieldInSecondaryHand())
	Player->SetSecondaryWielded(GetSecondaryWielded());

      if(Player->CanWear())
	Player->SetBodyArmor(GetBodyArmor());*/

      Player->SetMoney(GetMoney());
      game::SetPlayer(Player);
      game::SetPlayerBackup(0);
      Player->ChangeTeam(GetTeam());

      if(GetTeam()->GetLeader() == this)
	GetTeam()->SetLeader(Player);

      Player->TestWalkability();

      game::SendLOSUpdateRequest();
      game::GetCurrentArea()->UpdateLOS();
    }
}

void character::DeActivateVoluntaryStates(const std::string& Reason)
{
  if(GetAction() && GetAction()->IsVoluntary())
    {
      if(GetIsPlayer() && Reason != "")
	ADD_MESSAGE("%s.", Reason.c_str());

      GetAction()->Terminate(false);
    }
}

void character::ActionAutoTermination()
{
  if(!GetAction() || !GetAction()->IsVoluntary())
    return;

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled() && ((GetIsPlayer() && (*i)->GetSquareUnder()->CanBeSeen()) || (!GetIsPlayer() && (*i)->GetSquareUnder()->CanBeSeenFrom(GetPos(), LOSRangeSquare(), HasInfraVision()))))
	  {
	    ADD_MESSAGE("%s seems to be hostile.", (*i)->CHARNAME(DEFINITE));
	    GetAction()->Terminate(false);
	    return;
	  }
}

bool character::CheckForEnemies(bool CheckDoors)
{
  bool HostileCharsNear = false;

  character* NearestChar = 0;
  ulong NearestDistance = 0xFFFFFFFF;

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
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
  if(CanOpen())
    {
      DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
			    if(game::GetCurrentLevel()->GetLSquare(DoX, DoY)->GetOLTerrain()->CanBeOpenedByAI() && game::GetCurrentLevel()->GetLSquare(DoX, DoY)->Open(this))
			    return true;);
    }

  return false;
}

bool character::CheckForUsefulItemsOnGround()
{
  for(stackiterator i = GetLSquareUnder()->GetStack()->GetBottomSlot(); i != GetLSquareUnder()->GetStack()->GetSlotAboveTop(); ++i)
    {
//<<<charba.cpp
      /*if(CanWear() && (**i)->GetArmorValue() < CalculateArmorModifier() && GetBurdenState(GetStack()->SumOfMasses() + (**i)->GetWeight() - (GetBodyArmor() ? GetBodyArmor()->GetWeight() : 0)) == UNBURDENED)
	if(!GetLSquareUnder()->GetRoom() || GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, ***i))
	  {
	    item* ToWear = GetLSquareUnder()->GetStack()->MoveItem(c, GetStack());

      /if(CanWear() && GetLSquareUnder()->GetStack()->GetItem(c)->GetArmorValue() < CalculateArmorModifier() && GetBurdenState(GetStack()->SumOfMasses() + GetLSquareUnder()->GetStack()->GetItem(c)->GetWeight() - (GetTorsoArmor() ? GetTorsoArmor()->GetWeight() : 0)) == UNBURDENED)
	if(!GetLSquareUnder()->GetRoom() || GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, GetLSquareUnder()->GetStack()->GetItem(c)))
	{
	item* ToWear = GetLSquareUnder()->GetStack()->MoveItem(c, GetStack());
>>1.202

	    if(GetBodyArmor())
	      GetStack()->MoveItem(GetStack()->SearchItem(GetBodyArmor()), GetLSquareUnder()->GetStack());
==
	if(GetTorsoArmor())
	GetStack()->MoveItem(GetStack()->SearchItem(GetTorsoArmor()), GetLSquareUnder()->GetStack());
>> 1.202

	    SetBodyArmor(ToWear);
=
	SetTorsoArmor(ToWear);

<< charba.cpp
	    if(GetLSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s picks up and wears %s.", CHARNAME(DEFINITE), ToWear->CHARNAME(DEFINITE));
=
	if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s picks up and wears %s.", CNAME(DEFINITE), ToWear->CNAME(DEFINITE));

	return true;
	}*/

      /*if(CanWield() && long((**i)->GetWeaponStrength()) > long(GetAttackStrength()) && GetBurdenState(GetStack()->SumOfMasses() + (**i)->GetWeight() - (GetWielded() ? GetWielded()->GetWeight() : 0)) == UNBURDENED)
	if(!GetLSquareUnder()->GetRoom() || GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->PickupItem(this, ***i))
	  {
	    //item* ToWield = GetLSquareUnder()->GetStack()->MoveItem(c, GetStack());
	    item* ToWield = ***i;
	    ToWield->MoveTo(GetStack());

	    if(GetWielded())
	      GetWielded()->MoveTo(GetLSquareUnder()->GetStack());

	    SetWielded(ToWield);

	    if(GetLSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s picks up and wields %s.", CHARNAME(DEFINITE), ToWield->CHARNAME(DEFINITE));

	    return true;
	  }*/

      if((**i)->IsConsumable(this) && !(**i)->IsBadFoodForAI(this))
	if(!GetLSquareUnder()->GetRoom() || GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->ConsumeItem(this, ***i))
	  {
	    if(GetLSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s begins %s %s.", CHARNAME(DEFINITE), (**i)->GetConsumeVerb().c_str(), (**i)->CHARNAME(DEFINITE));

	    ConsumeItem(***i);
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
  long HPToRest = game::NumberQuestion("How many hit points you desire?", vector2d(16, 6), WHITE);

  if(HPToRest <= GetHP())
    {
      ADD_MESSAGE("Your HP is already %d.", GetHP());
      return false;
    }

  GetSquareUnder()->GetOTerrain()->ShowRestMessage(this);
  rest* Rest = new rest(this);
  Rest->SetGoalHP(HPToRest);
  SetAction(Rest);
  return true;
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
  return GetMeleeAttributeModifier() >> 1;
}

float character::GetDodgeValue() const
{
  return (GetMeleeAttributeModifier() << 1) / sqrt(GetSize());
}

ulong character::CurrentDanger() const
{
  ulong AttrDanger = ulong(GetAttackStrength() * GetStrength() * (GetToHitValue() + GetDodgeValue() + GetAgility()) / 100000);
  ulong TotalDanger = 0;
  ushort TotalWeight = 0;

  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      {
	TotalWeight += GetBodyPart(c)->DangerWeight();
	TotalDanger += GetBodyPart(c)->Danger(AttrDanger, false);
      }

  return AttrDanger * TotalDanger / TotalWeight;
}

ulong character::MaxDanger() const
{
  ulong AttrDanger = ulong(GetAttackStrength() * GetStrength() * (GetToHitValue() + GetDodgeValue() + GetAgility()) / 100000);
  ulong TotalDanger = 0;
  ushort TotalWeight = 0;

  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      {
	TotalWeight += GetBodyPart(c)->DangerWeight();
	TotalDanger += GetBodyPart(c)->Danger(AttrDanger, true);
      }

  return AttrDanger * TotalDanger / TotalWeight;
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
  if(GetBurdenState() == OVERLOADED)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You try to use your claws to crawl forward. But your load is too heavy.");

      return true;
    }

  if(CurrentDanger() > Who->CurrentDanger() && Who->CanBeDisplaced() && (!Who->GetAction() || Who->GetAction()->AllowDisplace()))
    {
      if(GetIsPlayer())
	if(GetSquareUnder()->CanBeSeen() || Who->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("You displace %s!", Who->CHARNAME(DEFINITE));
	else
	  ADD_MESSAGE("You displace something!");
      else
	if(Who->GetIsPlayer())
	  if(GetSquareUnder()->CanBeSeen() || Who->GetSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s displaces you!", CHARNAME(DEFINITE));
	  else
	    ADD_MESSAGE("Something displaces you!");
	else
	  if(GetSquareUnder()->CanBeSeen() || Who->GetSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s displaces %s!", CHARNAME(DEFINITE), Who->CHARNAME(DEFINITE));

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
	  ADD_MESSAGE("%s resists!", Who->CHARNAME(DEFINITE));
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
  if(GetPos().X < game::GetCamera().X + 2 || GetPos().X > game::GetCamera().X + game::GetScreenSize().X - 2)
    game::UpdateCameraX();

  if(GetPos().Y < game::GetCamera().Y + 2 || GetPos().Y > game::GetCamera().Y + game::GetScreenSize().Y - 2)
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
	    ADD_MESSAGE("%s is lying here.", GetLSquareUnder()->GetStack()->GetBottomItem()->CHARNAME(INDEFINITE));
	}
		
      if(game::GetCurrentLevel()->GetLSquare(GetPos())->GetEngraved() != "")
	ADD_MESSAGE("Something has been engraved here: \"%s\"", game::GetCurrentLevel()->GetLSquare(GetPos())->GetEngraved().c_str());
    }
}

void character::Hostility(character* Enemy)
{
  if(GetTeam() != Enemy->GetTeam())
    GetTeam()->Hostility(Enemy->GetTeam());
  else if(Enemy->IsEnabled())
    {
      if(Enemy->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s becomes enraged.", Enemy->CHARNAME(DEFINITE));

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

bool character::Go()
{
  uchar Dir = game::DirectionQuestion("In what direction do you want to go?", 0xFF, false);

  if(Dir == 0xFF)
    return false;

  vector2d Temp = game::GetMoveVector(Dir);

  go* Go = new go(this);
  Go->SetDirection(game::GetDirectionForVector(Temp));
  uchar OKDirectionsCounter = 0;

  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
  {
    if(game::GetCurrentLevel()->GetLSquare(DoX, DoY)->GetIsWalkable(this))
      OKDirectionsCounter++;	
  });

  Go->SetWalkingInOpen(OKDirectionsCounter > 2 ? true : false);
  SetAction(Go);
  Go->Handle();
  return GetAction() ? true : false;
}

void character::GoOn(go* Go)
{
  if(GetAP() >= 0)
    {
      ActionAutoTermination();

      if(!GetAction())
	return;

      if(!game::IsValidPos(GetPos() + game::GetMoveVector(Go->GetDirection())))
	{
	  Go->Terminate(false);
	  return;
	}

      lsquare* MoveToSquare = game::GetCurrentLevel()->GetLSquare(GetPos() + game::GetMoveVector(Go->GetDirection()));

      if(!MoveToSquare->GetIsWalkable(this) || (MoveToSquare->GetCharacter() && GetTeam() != MoveToSquare->GetCharacter()->GetTeam()))
	{
	  Go->Terminate(false);
	  return;
	}

      uchar OKDirectionsCounter = 0;

      DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
      {
	if(game::GetCurrentLevel()->GetLSquare(DoX, DoY)->GetIsWalkable(this))
	  OKDirectionsCounter++;	
      });

      if(!Go->GetWalkingInOpen())
	{
	  if(OKDirectionsCounter > 2)
	    {
	      Go->Terminate(false);
	      return;
	    }
	}
      else
	if(OKDirectionsCounter <= 2)
	  Go->SetWalkingInOpen(false);

      square* BeginSquare = GetSquareUnder();

      if(!TryMove(MoveToSquare->GetPos()) || BeginSquare == GetSquareUnder() || GetLSquareUnder()->GetLuminance() < LIGHT_BORDER || GetLSquareUnder()->GetStack()->GetItems())
	{
	  Go->Terminate(false);
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

bool character::ChangeRandomStat(short HowMuch)
{
  // Strength, Endurance, Agility, Perception

  for(ushort c = 0; c < 15; ++c)
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

ushort character::DangerLevel() const
{
  static ulong DangerPointMinimum[] = { 0, 100, 500, 2500, 10000, 50000, 250000, 1000000, 5000000, 25000000 };

  for(ushort c = 9;; --c)
    if(CurrentDanger() >= DangerPointMinimum[c])
      return c;
}

void character::DisplayInfo()
{
  if(GetIsPlayer())
    ADD_MESSAGE("You are %s here.", GetStandVerb().c_str());
  else
    {
      ADD_MESSAGE("%s is %s here.", CHARNAME(INDEFINITE), GetStandVerb().c_str());

      std::string Msg = PersonalPronoun();

      if(GetTeam() == game::GetPlayer()->GetTeam())
	Msg += std::string(" is at danger level ") + DangerLevel();

      /*if(GetWielded())
	{
	  if(GetBodyArmor())
	    {
	      if(GetTeam() == game::GetPlayer()->GetTeam())
		Msg += " and";

	      Msg += " is wielding " + GetWielded()->Name(INDEFINITE);

	      ADD_MESSAGE("%s.", Msg.c_str());
	      Msg = PersonalPronoun() + " wears " + GetBodyArmor()->Name(INDEFINITE);
	    }
	  else
	    {
	      if(GetTeam() == game::GetPlayer()->GetTeam())
		Msg += ",";

	      Msg += " is wielding " + GetWielded()->Name(INDEFINITE);
	    }

	  Msg += " and";
	}
      else if(GetBodyArmor())
	{
	  if(GetTeam() == game::GetPlayer()->GetTeam())
	    Msg += ",";

	  Msg += " is wearing " + GetBodyArmor()->Name(INDEFINITE) + " and";
	}
      else*/
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

/*void character::CreateBodyParts()
{
  CreateTorso();
}

void character::RestoreBodyParts()
{
  if(!GetTorso())
    CreateTorso();
}*/

material* character::CreateBodyPartBone(ushort, ulong Volume) const
{
  return new bone(Volume);
}

/*void character::SetSize(ushort Size)
{
  if(GetTorso())
    GetTorso()->SetSize(Size);
}*/

ushort character::GetSize() const
{
  if(GetTorso())
    return GetTorso()->GetSize();
  else
    return 0;
}

/*ushort character::TorsoSize() const
{
  return GetTotalSize();
}*/

/*void character::CreateTorso()
{
  SetTorso(new normaltorso(false));
  GetTorso()->SetBitmapPos(GetBitmapPos(0)); // fix this!
  GetTorso()->InitMaterials(CreateTorsoFlesh(GetTorsoVolume() * (100 - GetTorsoBonePercentile()) / 100), CreateTorsoBone(GetTorsoVolume() * GetTorsoBonePercentile() / 100));
  GetTorso()->PlaceToSlot(GetTorsoSlot());
  GetTorso()->SetSize(TorsoSize());
}*/

torso* character::GetTorso() const { return (torso*)GetBodyPart(0); }
humanoidtorso* character::GetHumanoidTorso() const { return (humanoidtorso*)GetBodyPart(0); }
void character::SetTorso(torso* What) { SetBodyPart(0, What); }

void character::SetMainMaterial(material* NewMaterial)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->SetMainMaterial(NewMaterial);

  for(ushort c = 1; c < BodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetMainMaterial()->GetVolume());
      GetBodyPart(c)->SetMainMaterial(NewMaterial);
    }
}

void character::ChangeMainMaterial(material* NewMaterial)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->ChangeMainMaterial(NewMaterial);

  for(ushort c = 1; c < BodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetMainMaterial()->GetVolume());
      GetBodyPart(c)->ChangeMainMaterial(NewMaterial);
    }
}

void character::SetSecondaryMaterial(material*)
{
  ABORT("Illegal character::SetSecondaryMaterial call!");
}

void character::ChangeSecondaryMaterial(material*)
{
  ABORT("Illegal character::ChangeSecondaryMaterial call!");
}

void character::SetContainedMaterial(material* NewMaterial)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetContainedMaterial()->GetVolume());
  GetBodyPart(0)->SetContainedMaterial(NewMaterial);

  for(ushort c = 1; c < BodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetContainedMaterial()->GetVolume());
      GetBodyPart(c)->SetContainedMaterial(NewMaterial);
    }
}

void character::ChangeContainedMaterial(material* NewMaterial)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetContainedMaterial()->GetVolume());
  GetBodyPart(0)->SetContainedMaterial(NewMaterial);

  for(ushort c = 1; c < BodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetContainedMaterial()->GetVolume());
      GetBodyPart(c)->ChangeContainedMaterial(NewMaterial);
    }
}

void character::Teleport()
{
  Move(game::GetCurrentLevel()->RandomSquare(this, true), true);
}

bool character::SecretKnowledge()
{
  felist List("Knowledge of the ancients:", WHITE, 0);

  std::string Buffer = "Name                                                 Weight       SV     Str";
  List.AddDescription(Buffer);

  for(ushort c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
    if(protocontainer<item>::GetProto(c)->IsAutoInitializable())
      {
	item* Item = protocontainer<item>::GetProto(c)->Clone();
	Buffer = Item->Name(INDEFINITE);
	Buffer.resize(50,' ');
	Buffer += Item->GetWeight();
	Buffer.resize(63, ' ');
	Buffer += Item->GetStrengthValue();
	Buffer.resize(70, ' ');
	Buffer += ulong(Item->GetWeaponStrength() / 100);
	List.AddEntry(Buffer, LIGHTGRAY);
	delete Item;
      }

  List.Draw(vector2d(26, 42), 652, 10);
  return false;
}

short character::GetHP() const
{
  short HP = 0;

  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      HP += GetBodyPart(c)->GetHP();

  return HP;
}

short character::GetMaxHP() const
{
  short HP = 0;

  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      HP += GetBodyPart(c)->GetMaxHP();

  return HP;
}

void character::RestoreHP()
{
  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->SetHP(GetBodyPart(c)->GetMaxHP());
}

void character::BlockDamageType(uchar Type)
{
  switch(Type)
    {
    case PHYSICALDAMAGE:
      EditStrengthExperience(50);
      EditEnduranceExperience(50);
    }
}

bool character::AllowDamageTypeBloodSpill(uchar Type) const
{
  switch(Type)
    {
    case PHYSICALDAMAGE:
    case SOUND:
    case ENERGY:
      return true;
    case ACID:
    case FIRE:
    case POISON:
    case BULIMIA:
      return false;
    default:
      ABORT("Unknown blood effect destroyed dungeon!");
      return false;
    }
}

bool character::DamageTypeCanSeverBodyPart(uchar Type) const
{
  switch(Type)
    {
    case PHYSICALDAMAGE:
      return true;
    case SOUND:
    case ENERGY:
    case ACID:
    case FIRE:
    case POISON:
    case BULIMIA:
      return false;
    default:
      ABORT("Unknown reaping effect destroyed dungeon!");
      return false;
    }
}

bool character::ReceiveBodyPartDamage(character* Damager, short Damage, uchar Type, uchar BodyPartIndex, uchar Direction, bool PenetrateResistance, bool Critical)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);

  if(!PenetrateResistance)
    Damage -= BodyPart->GetTotalResistance(Type);

  if(Damage < 1)
    if(Critical)
      Damage = 1;
    else
      {
	if(GetIsPlayer())
	  ADD_MESSAGE("You are not hurt.");
	else if(GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s is not hurt.", PersonalPronoun().c_str());

	BlockDamageType(Type);
	return false;
      }

  if(Critical && AllowDamageTypeBloodSpill(Type))
    {
      SpillBlood(3 + RAND() % 3);

      DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
      {
	vector2d Where(DoX, DoY);

	if(game::GetCurrentLevel()->GetLSquare(Where)->GetOTerrain()->GetIsWalkable()) 
	  SpillBlood(2 + RAND() % 2, Where);
      });
    }

  if(BodyPart->ReceiveDamage(Damager, Damage, Type) && DamageTypeCanSeverBodyPart(Type) && BodyPartCanBeSevered(BodyPartIndex))
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Your %s is severed off!", BodyPart->CHARNAME(UNARTICLED));
      else if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s %s is severed off!", PossessivePronoun().c_str(), BodyPart->CHARNAME(UNARTICLED));

      SevereBodyPart(BodyPartIndex);
      GetSquareUnder()->SendNewDrawRequest();

      if(!game::GetInWilderness())
	{
	  GetLSquareUnder()->GetStack()->AddItem(BodyPart);

	  if(Direction != 8)
	    BodyPart->Fly(0, Direction, Damage);
	}
      else
	  GetStack()->AddItem(BodyPart);

      BodyPart->DropEquipment();
    }

  return true;
}

bodypart* character::SevereBodyPart(ushort BodyPartIndex)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);
  BodyPart->SetOwnerDescription("of " + Name(INDEFINITE));
  BodyPart->SetUnique(ForceDefiniteArticle() || AssignedName != "");
  BodyPart->RemoveFromSlot();
  return BodyPart;
}

bool character::ReceiveDamage(character* Damager, short Damage, uchar Type, uchar, uchar Direction, bool, bool PenetrateArmor, bool Critical)
{
  return ReceiveBodyPartDamage(Damager, Damage, Type, 0, Direction, PenetrateArmor, Critical);
}

bool character::BodyPartVital(ushort Index)
{
  if(Index == 0)
    return true;
  else
    return false;
}

bool character::BodyPartCanBeSevered(ushort Index) const
{
  if(Index == 0)
    return false;
  else
    return true;
}

bool character::AssignName()
{
  vector2d Where = game::PositionQuestion("What do you want to name? [choose a square and press space bar or press ESC to quit]", GetPos());

  if(Where == vector2d(-1,-1) || Where == GetPos())
    return false;

  character* Character = game::GetCurrentLevel()->GetLSquare(Where)->GetCharacter();

  if(Character && Character->GetSquareUnder()->CanBeSeen())
    {
      if(Character->IsNameable())
	{
	  std::string Topic = "What do you want to call this " + Character->Name(UNARTICLED) + "?";
	  std::string Name = game::StringQuestion(Topic, vector2d(16, 6), WHITE, 0, 80, true);
	  if(Name != "")
	    Character->SetAssignedName(Name);
	}
      else
	ADD_MESSAGE("%s refuses to be called anything else but %s.", Character->CHARNAME(DEFINITE), Character->CHARNAME(DEFINITE));
    }

  return false;
}

std::string character::Description(uchar Case) const
{
  if(GetSquareUnder()->CanBeSeen())
    return Name(Case);
  else
    return "something";
}

std::string character::PersonalPronoun() const
{
  if(GetSex() == UNDEFINED || !GetSquareUnder()->CanBeSeen())
    return "it";
  else if(GetSex() == MALE)
    return "he";
  else
    return "she";
}

std::string character::PossessivePronoun() const
{
  if(GetSex() == UNDEFINED || !GetSquareUnder()->CanBeSeen())
    return "its";
  else if(GetSex() == MALE)
    return "his";
  else
    return "her";
}

std::string character::ObjectPronoun() const
{
  if(GetSex() == UNDEFINED || !GetSquareUnder()->CanBeSeen())
    return "it";
  else if(GetSex() == MALE)
    return "him";
  else
    return "her";
}

std::string character::MaterialDescription(bool Articled) const
{
  return GetTorso()->GetMainMaterial()->Name(Articled);
}

std::string character::Name(uchar Case) const
{
  if(!(Case & PLURAL) && AssignedName != "")
    {
      if(!ShowClassDescription())
	return AssignedName;
      else
	return AssignedName + " " + id::Name((Case | ARTICLEBIT)&~INDEFINEBIT);
    }
  else
    return id::Name(Case);
}

void character::Haste(ushort Counter)
{
  if(StateIsActivated(HASTE))
    return;

  if(StateIsActivated(SLOW))
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Time seems to go by at the normal rate now.");
      else
	ADD_MESSAGE("%s slows down to the normal pace.", CHARNAME(DEFINITE));

      EndSlow();
    }   
  else
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Time slows down to a crawl.");
      else
	if(GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s looks faster!", CHARNAME(DEFINITE));

      ActivateState(HASTE);
      SetStateCounter(HASTE, Counter);
    }
}

void character::HasteHandler()
{
  if(StateIsActivated(HASTE))
    {
      if(!GetStateCounter(HASTE))
      {
	if(GetIsPlayer())
	  ADD_MESSAGE("Time seems to go by at the normal rate now.");
	else if(GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s seems to move at the normal pace now.", CHARNAME(DEFINITE));

	EndHaste();
      }

      EditStateCounter(HASTE, -1);
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
      if(GetIsPlayer())
	ADD_MESSAGE("Time seems to go by at the normal rate now.");
      else
	ADD_MESSAGE("%s slows down to the normal pace.", CHARNAME(DEFINITE));

      EndHaste();
    }
  else
    {
      if(GetIsPlayer())
	ADD_MESSAGE("Everything seems to move much faster now.");
      else
	if(GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s looks slower!", CHARNAME(DEFINITE));

      ActivateState(SLOW);
      SetStateCounter(SLOW, Counter);
    }
}

void character::SlowHandler()
{
  if(StateIsActivated(SLOW))
    {
      if(!GetStateCounter(SLOW))
	{
	  if(GetIsPlayer())
	    ADD_MESSAGE("Time seems to go by at the normal rate now.");
	  else
	    ADD_MESSAGE("%s seems to move at the normal pace now.", CHARNAME(DEFINITE));

	  EndHaste();
	}

      EditStateCounter(SLOW, -1);
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
  else if(GetNP() > SATIATEDLEVEL)
    return SATIATED;
  else if(GetNP() > NOTHUNGERLEVEL)
    return NOTHUNGRY;
  else if(GetNP() > HUNGERLEVEL)
    return HUNGRY;
  else
    return VERYHUNGRY;
}

bool character::EqupmentScreen()
{
  if(!CheckWearEquipment())
    return false;

  ushort Chosen = 0;
  bool EquipmentChanged = false;

  while(true)
    {
      game::DrawEverythingNoBlit();

      felist List("Equipment menu", WHITE);

      for(ushort c = 0; c < EquipmentSlots(); ++c)
	{
	  std::string Entry = EquipmentName(c) + ":";
	  Entry.resize(20, ' ');

	  if(!GetBodyPartOfEquipment(c))
	    Entry += "can't use";
	  else if(!GetEquipment(c))
	    Entry += "-";
	  else
	    Entry += GetEquipment(c)->Name(INDEFINITE);

	  List.AddEntry(Entry, LIGHTGRAY);
	}

      List.SetSelected(Chosen);
      Chosen = List.Draw(vector2d(26, 42), 652, 20, true, false);

      if(Chosen >= EquipmentSlots())
	break;

      if(!GetBodyPartOfEquipment(Chosen))
	{
	  ADD_MESSAGE("Bodypart missing!");
	  continue;
	}

      item* OldEquipment = GetEquipment(Chosen);

      if(OldEquipment)
	GetEquipment(Chosen)->MoveTo(GetStack());

      if(!GetStack()->SortedItems(this, EquipmentSorter(Chosen)))
	{
	  ADD_MESSAGE("You haven't got any item that could be used for this purpose.");
	  continue;
	}
      else
	{
	  item* Item = GetStack()->DrawContents(this, "Choose " + EquipmentName(Chosen) + ":", EquipmentSorter(Chosen));

	  if(Item != OldEquipment)
	    EquipmentChanged = true;

	  if(Item)
	    {
	      Item->RemoveFromSlot();
	      SetEquipment(Chosen, Item);
	    }
	}
    }

  return EquipmentChanged;
}

bodypart* character::GetBodyPart(ushort Index) const
{
  return (bodypart*)GetBodyPartSlot(Index)->GetItem();
}

void character::SetBodyPart(ushort Index, bodypart* What)
{
  GetBodyPartSlot(Index)->SetItem(What);
}

characterslot* character::GetBodyPartSlot(ushort Index) const
{
  return &BodyPartSlot[Index];
}

bool character::CanEat(material* Material)
{
  return GetEatFlags() & Material->GetConsumeType() ? true : false;
}

void character::SetStateCounter(uchar State, ushort What)
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      StateCounter[c] = What;
}

void character::EditStateCounter(uchar State, short What)
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      StateCounter[c] += What;
}

ushort character::GetStateCounter(uchar State) const
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      return StateCounter[c];

  ABORT("Illegal GetStateCounter request!");
  return 0;
}

bool character::CheckKick() const
{
  ADD_MESSAGE("This monster type can not kick.");
  return false;
}

bool character::CheckWearEquipment() const
{
  ADD_MESSAGE("This monster type can not use equipment.");
  return false;
}

ushort character::GetResistance(uchar Type) const
{
  switch(Type)
    {
    case PHYSICALDAMAGE: return GetPhysicalDamageResistance();
    case SOUND: return GetSoundResistance();
    case ENERGY: return GetEnergyResistance();
    case ACID: return GetAcidResistance();
    case FIRE: return GetFireResistance();
    case POISON: return GetPoisonResistance();
    case BULIMIA: return GetBulimiaResistance();
    default:
      ABORT("Resistance lack detected!");
      return 0;
    }
}

void character::SetDivineMaster(uchar Master)
{
  for(stackiterator i = GetStack()->GetBottomSlot(); i != GetStack()->GetSlotAboveTop(); ++i)
    (**i)->SetDivineMaster(Master);
}

bool character::ScrollMessagesDown()
{
  msgsystem::ScrollDown();
  return false;
}

bool character::ScrollMessagesUp()
{
  msgsystem::ScrollUp();
  return false;
}

void character::Regenerate(ushort Turns)
{
  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->Regenerate(Turns);
}

void character::ReceiveHeal(long Amount)
{
  for(ushort c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      {
	if(GetBodyPart(c)->GetHP() + Amount / BodyParts() > GetBodyPart(c)->GetMaxHP())
	  GetBodyPart(c)->SetHP(GetBodyPart(c)->GetMaxHP());
	else
	  GetBodyPart(c)->EditHP(Amount / BodyParts());
      }
}

void character::AddHealingLiquidConsumeEndMessage() const
{
  if(GetIsPlayer())
    ADD_MESSAGE("You feel better.");
  else
    if(GetSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s looks healthier.", CHARNAME(DEFINITE));
}

void character::ReceiveSchoolFoodEffect(long SizeOfEffect)
{
  SizeOfEffect += RAND() % SizeOfEffect;
  Vomit(SizeOfEffect / 250);
  ReceiveDamage(0, SizeOfEffect / 100, POISON, ALL, true);

  if(CheckDeath("was poisoned by school food"))
    return;

  if(!(RAND() % 3) && SizeOfEffect / 500)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You gain a little bit of toughness for surviving this stuff.");
      else
	if(GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks tougher.", CHARNAME(DEFINITE));

      SetEndurance(GetEndurance() + SizeOfEffect / 500);
    }
}

void character::AddSchoolFoodConsumeEndMessage() const
{
  if(GetIsPlayer())
    ADD_MESSAGE("Yuck! This stuff feels like vomit and old mousepads.");
}

void character::ReceiveNutrition(long SizeOfEffect)
{
  if(GetHungerState() == BLOATED)
    {
      game::GetPlayer()->ReceiveDamage(0, SizeOfEffect / 1000, BULIMIA, TORSO);
      CheckDeath("choked on his food");
    }

  EditNP(SizeOfEffect);
}

void character::ReceiveOmleUrineEffect(long Amount)
{
  EditStrengthExperience(Amount * 4);
  RestoreHP();
}

void character::AddOmleUrineConsumeEndMessage() const
{
  if(GetIsPlayer())
    ADD_MESSAGE("You feel a primitive Force coursing through your veins.");
  else
    if(GetSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s looks more powerful.", CHARNAME(DEFINITE));
}

void character::ReceivePepsiEffect(long SizeOfEffect)
{
  ReceiveDamage(0, SizeOfEffect / 100, POISON, TORSO);
  EditPerceptionExperience(SizeOfEffect * 4);

  if(CheckDeath("was poisoned by pepsi"))
    return;

  if(GetIsPlayer())
    game::DoEvilDeed(SizeOfEffect / 10);
}

void character::AddPepsiConsumeEndMessage() const
{
  if(GetIsPlayer())
    ADD_MESSAGE("Urgh. You feel your guruism fading away.");
  else
    if(GetSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s looks very lame.", CHARNAME(DEFINITE));
}

void character::Darkness(long SizeOfEffect)
{
  long Penalty = 1 + SizeOfEffect * (100 + RAND() % 26 - RAND() % 26) / 100000;

  /*if(GetStrength() - Penalty > 1)
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
    SetAgility(1);*/

  /*ReceiveDamage(0, SizeOfEffect / 100, POISON, TORSO);

  if(CheckDeath("was poisoned by pepsi"))
    return;*/

  /*if(GetHP() - Penalty / 2 > 1)
    SetHP(GetHP() - Penalty / 2);
  else
    SetHP(1);*/

  if(GetIsPlayer())
    {
      game::DoEvilDeed(short(SizeOfEffect / 50));

      if(game::GetWizardMode())
	ADD_MESSAGE("Change in relation: %d.", short(SizeOfEffect / 100));
    }
}

void character::AddDarknessConsumeEndMessage() const
{
  if(GetIsPlayer())
    ADD_MESSAGE("Arg. You feel the fate of a navastater placed upon you...");
  else
    if(GetSquareUnder()->CanBeSeen())
      ADD_MESSAGE("Suddenly %s looks like a navastater.", CHARNAME(DEFINITE));
}

/*void character::ReceiveFireDamage(character* Burner, std::string DeathMsg, long SizeOfEffect)
{
  if(GetIsPlayer())
    ADD_MESSAGE("You burn.");
  else
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s is hurt by the fire.", CHARNAME(DEFINITE));

  if(SizeOfEffect > 1)
    SetHP(GetHP() - (SizeOfEffect + RAND() % (SizeOfEffect / 2)));
  else
    SetHP(GetHP() - SizeOfEffect);

  GetStack()->ReceiveFireDamage(Burner, DeathMsg, SizeOfEffect);
}*/

void character::ReceiveKoboldFleshEffect(long)
{
  /* As it is commonly known, the possibility of fainting per 500 cubic centimeters of kobold flesh is exactly 1%. */

  if(!(RAND() % 100))
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You lose control of your legs and fall down.");

      Faint();
    }
}

void character::AddKoboldFleshConsumeEndMessage() const
{
  if(GetIsPlayer())
    ADD_MESSAGE("This stuff tastes really funny.");
}

void character::AddBoneConsumeEndMessage() const
{
  if(GetIsPlayer())
    ADD_MESSAGE("You feel like a hippie.");
  else if(GetSquareUnder()->CanBeSeen())
    ADD_MESSAGE("%s barks happily.", CHARNAME(DEFINITE));
}

void character::PrintInfo() const
{
  felist Info("All information about " + Name(INDEFINITE), WHITE, 0);
  Info.AddEntry(std::string("Strength: ") + GetStrength(), LIGHTGRAY);
  Info.AddEntry(std::string("Endurance: ") + GetEndurance(), LIGHTGRAY);
  Info.AddEntry(std::string("Agility: ") + GetAgility(), LIGHTGRAY);
  Info.AddEntry(std::string("Perception: ") + GetPerception(), LIGHTGRAY);
  Info.AddEntry(std::string("Total weight: ") + GetTotalWeight(), LIGHTGRAY);

  ushort c;

  for(c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      Info.AddEntry(GetBodyPart(c)->Name(UNARTICLED) + " armor value: " + GetBodyPart(c)->GetTotalResistance(PHYSICALDAMAGE), LIGHTGRAY);

  for(c = 0; c < EquipmentSlots(); ++c)
    {
      std::string Entry = EquipmentName(c) + ": ";

      if(!GetBodyPartOfEquipment(c))
	Entry += "can't use";
      else if(!GetEquipment(c))
	Entry += "-";
      else
	Entry += GetEquipment(c)->Name(INDEFINITE);

      Info.AddEntry(Entry, LIGHTGRAY);
    }

  AddInfo(Info);
  Info.Draw(vector2d(26, 42), 652, 30, false);
}

void character::AddInfo(felist& Info) const
{
  Info.AddEntry(std::string("Attack strength: ") + ulong(GetAttackStrength()), LIGHTGRAY);
  Info.AddEntry(std::string("To hit value: ") + ulong(GetToHitValue()), LIGHTGRAY);
}

void character::CompleteRiseFromTheDead()
{
  for(ushort c = 0; c < BodyParts(); ++c)
    {
      if(GetBodyPart(c))
	GetBodyPart(c)->SetHP(1);
    }
}

bool character::RaiseTheDead(character*)
{
  bool Useful = false;
  for(ushort c = 0; c < BodyParts(); ++c)
    {
      if(!GetBodyPart(c))
	{
	  CreateBodyPart(c);
	  if(GetIsPlayer())
	    ADD_MESSAGE("Suddenly you grow a new %s.", GetBodyPart(c)->CHARNAME(UNARTICLED));
	  else if(GetLSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s grows a new %s.", CHARNAME(DEFINITE), GetBodyPart(c)->CHARNAME(UNARTICLED));
	  Useful = true;
	}
    }
  if(!Useful)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You shudder.");
      else if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s shudders.", CHARNAME(DEFINITE));
    }
  return true;
}

/*void character::CreateBodyPart(ushort Index)
{
  switch(Index)
    {
    case TORSOINDEX:
      CreateTorso();
      break;
    default:
      ABORT("Wierd bodypart case to create for a character.");
    }
}*/

void character::SetSize(ushort Size)
{
  for(uchar c = 0; c < BodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->SetSize(GetBodyPartSize(c, Size));
}

ulong character::GetBodyPartSize(ushort Index, ushort TotalSize)
{
  if(Index == TORSOINDEX)
    return TotalSize;
  else
    {
      ABORT("Wierd bodypart size request for a character!");
      return 0;
    }
}

ulong character::GetBodyPartVolume(ushort Index)
{
  if(Index == TORSOINDEX)
    return GetTotalVolume();
  else
    {
      ABORT("Wierd bodypart volume request for a character!");
      return 0;
    }
}

uchar character::GetBodyPartBonePercentile(ushort Index)
{
  if(Index == TORSOINDEX)
    return GetTorsoBonePercentile();
  else
    {
      ABORT("Wierd bodypart bone percentile request for a character!");
      return 0;
    }
}

void character::CreateBodyParts()
{
  /* Create all body parts */

  for(uchar c = 0; c < BodyParts(); ++c) 
    CreateBodyPart(c);
}

void character::RestoreBodyParts()
{
  for(uchar c = 0; c < BodyParts(); ++c)
    if(!GetBodyPart(c))
      CreateBodyPart(c);
}

void character::UpdateBodyPartPictures()
{
  for(ushort c = 0; c < BodyParts(); ++c)
    UpdateBodyPartPicture(c);

  /*UpdateHeadPicture(CallUpdatePictures);
  UpdateTorsoPicture(CallUpdatePictures);
  UpdateRightArmPicture(CallUpdatePictures);
  UpdateLeftArmPicture(CallUpdatePictures);
  UpdateGroinPicture(CallUpdatePictures);
  UpdateRightLegPicture(CallUpdatePictures);
  UpdateLeftLegPicture(CallUpdatePictures);*/
}

bodypart* character::MakeBodyPart(ushort Index)
{
  if(Index == TORSOINDEX)
    return new normaltorso(false);
  else
    {
      ABORT("Wierd bodypart to make for a character!");
      return 0;
    }
}

void character::CreateBodyPart(ushort Index)
{
  SetBodyPart(Index, MakeBodyPart(Index));
  GetBodyPart(Index)->InitMaterials(CreateBodyPartFlesh(Index, GetBodyPartVolume(Index) * (100 - GetBodyPartBonePercentile(Index)) / 100), CreateBodyPartBone(Index, GetBodyPartVolume(Index) * GetBodyPartBonePercentile(Index) / 100), false);
  UpdateBodyPartPicture(Index);
  GetBodyPart(Index)->PlaceToSlot(GetBodyPartSlot(Index));
  GetBodyPart(Index)->SetSize(GetBodyPartSize(Index, GetTotalSize()));
}

vector2d character::GetBodyPartBitmapPos(ushort Index, ushort Frame)
{
  if(Index == TORSOINDEX)
    return GetTorsoBitmapPos(Frame);
  else
    {
      ABORT("Wierd bodypart BitmapPos request for a character!");
      return vector2d();
    }
}

ushort character::GetBodyPartColor0(ushort Index, ushort Frame)
{
  if(Index < BodyParts())
    return GetSkinColor(Frame);
  else
    {
      ABORT("Wierd bodypart color 0 request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColor1(ushort Index, ushort Frame)
{
  if(Index == TORSOINDEX)
    return GetTorsoMainColor(Frame);
  else
    {
      ABORT("Wierd bodypart color 1 request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColor2(ushort Index, ushort)
{
  if(Index == TORSOINDEX)
    return 0; // reserved for future use
  else
    {
      ABORT("Wierd bodypart color 2 request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColor3(ushort Index, ushort Frame)
{
  if(Index == TORSOINDEX)
    return GetTorsoSpecialColor(Frame);
  else
    {
      ABORT("Wierd bodypart color 3 request for a character!");
      return 0;
    }
}

void character::UpdateBodyPartPicture(ushort Index)
{
  if(GetBodyPart(Index))
    {
      std::vector<vector2d>& BitmapPos = GetBodyPart(Index)->GetBitmapPosVector();
      std::vector<ushort>& Color1 = GetBodyPart(Index)->GetColor1Vector();
      std::vector<ushort>& Color2 = GetBodyPart(Index)->GetColor2Vector();
      std::vector<ushort>& Color3 = GetBodyPart(Index)->GetColor3Vector();

      BitmapPos.clear();
      Color1.clear();
      Color2.clear();
      Color3.clear();

      for(ushort c = 0; c < GetBodyPartAnimationFrames(Index); ++c)
	{
	  BitmapPos.push_back(GetBodyPartBitmapPos(Index, c));
	  Color1.push_back(GetBodyPartColor1(Index, c));
	  Color2.push_back(GetBodyPartColor2(Index, c));
	  Color3.push_back(GetBodyPartColor3(Index, c));
	}

      material* Material = GetBodyPart(Index)->GetMainMaterial();

      if(Material->GetType() == humanflesh::StaticType())
	{
          std::vector<ushort>& SkinColor = ((humanflesh*)Material)->GetSkinColorVector();
	  SkinColor.clear();

	  for(ushort c = 0; c < GetBodyPartAnimationFrames(Index); ++c)
	    SkinColor.push_back(GetBodyPartColor0(Index, c));
	}

      GetBodyPart(Index)->SetAnimationFrames(GetBodyPartAnimationFrames(Index));

      /*GetBodyPart(Index)->SetBitmapPos(GetHeadBitmapPos());
      //GetHead()->SetColor(0, SkinColor());
      GetBodyPart(Index)->SetColor(1, GetCapColor());
      GetBodyPart(Index)->SetColor(2, GetHairColor());
      GetBodyPart(Index)->SetColor(3, GetEyeColor());*/

      GetBodyPart(Index)->UpdatePictures();
    }
}

void character::LoadDataBaseStats()
{
  SetAgility(GetDefaultAgility());
  SetStrength(GetDefaultStrength());
  SetEndurance(GetDefaultEndurance());
  SetPerception(GetDefaultPerception());
  SetMoney(GetDefaultMoney());
}

character* character::Clone(bool MakeBodyParts, bool CreateEquipment) const
{
  return GetProtoType().Clone(MakeBodyParts, CreateEquipment);
}

character* character_prototype::CloneAndLoad(inputfile& SaveFile) const
{
  character* Char = Clone(false, false);
  Char->Load(SaveFile);
  return Char;
}

void character::Initialize(bool MakeBodyParts, bool CreateEquipment)
{
  LoadDataBaseStats();
  VirtualConstructor();

  BodyPartSlot = new characterslot[BodyParts()];

  for(ushort c = 0; c < BodyParts(); ++c)
    GetBodyPartSlot(c)->SetMaster(this);

  if(MakeBodyParts)
    {
      CreateBodyParts();

      if(CreateEquipment)
	CreateInitialEquipment();

      RestoreHP();
    }
}

character_prototype::character_prototype()
{
  Index = protocontainer<character>::Add(this);
}
