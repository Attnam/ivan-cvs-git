#include <queue>
#include <cmath>

#include "charde.h"
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
#include "festring.h"
#include "slot.h"
#include "actionde.h"
#include "command.h"
#include "proto.h"
#include "save.h"
#include "database.h"
#include "wsquare.h"

/*
 * These arrays contain functions and values used for handling states. Remember to update them.
 * All normal states must have PrintBeginMessage and PrintEndMessage functions, a StateDescription string
 * and a StateIsSecret boolean (if true, StateDescription is not shown in the panel without magical means).
 * BeginHandler, EndHandler and StateHandler (called each tick) are optional, enter zero if the state
 * doesn't need one.
 */

void (character::*character::PrintBeginStateMessage[])() const = { 0, &character::PrintBeginHasteMessage, &character::PrintBeginSlowMessage, &character::PrintBeginPolymorphControlMessage, &character::PrintBeginLifeSaveMessage, &character::PrintBeginLycanthropyMessage, &character::PrintBeginInvisibilityMessage, &character::PrintBeginInfraVisionMessage, &character::PrintBeginESPMessage, &character::PrintBeginPoisonedMessage, &character::PrintBeginTeleportMessage, &character::PrintBeginPolymorphMessage, &character::PrintBeginTeleportControlMessage, &character::PrintBeginPanicMessage };
void (character::*character::PrintEndStateMessage[])() const = { 0, &character::PrintEndHasteMessage, &character::PrintEndSlowMessage, &character::PrintEndPolymorphControlMessage, &character::PrintEndLifeSaveMessage, &character::PrintEndLycanthropyMessage, &character::PrintEndInvisibilityMessage, &character::PrintEndInfraVisionMessage, &character::PrintEndESPMessage, &character::PrintEndPoisonedMessage, &character::PrintEndTeleportMessage, &character::PrintEndPolymorphMessage, &character::PrintEndTeleportControlMessage, &character::PrintEndPanicMessage };
void (character::*character::BeginStateHandler[])() = { 0, 0, 0, 0, 0, 0, &character::BeginInvisibility, &character::BeginInfraVision, &character::BeginESP, 0, 0, 0, 0, 0, 0 };
void (character::*character::EndStateHandler[])() = { &character::EndPolymorph, 0, 0, 0, 0, 0, &character::EndInvisibility, &character::EndInfraVision, &character::EndESP, 0, 0, 0, 0, 0 };
void (character::*character::StateHandler[])() = { 0, 0, 0, 0, 0, &character::LycanthropyHandler, 0, 0, 0, &character::PoisonedHandler, &character::TeleportHandler, &character::PolymorphHandler, 0, 0 };
std::string character::StateDescription[] = { "Polymorphed", "Hasted", "Slowed", "PolyControl", "Life Saved", "Lycanthropy", "Invisible", "Infravision", "ESP", "Poisoned", "Teleporting", "Polymorphing", "TeleControl", "Panic" };
bool character::StateIsSecret[] = { false, false, false, false, true, true, false, false, false, false, true, true, false, false };
bool character::StateCanBeRandomlyActivated[] = { false, true, true, true, false, true, true, true, true, false, true, true, true, false };

character::character(const character& Char) : entity(Char), id(Char), NP(Char.NP), AP(Char.AP), Player(false), TemporaryState(Char.TemporaryState&~POLYMORPHED), Team(Char.Team), WayPoint(-1, -1), Money(0), HomeRoom(Char.HomeRoom), AssignedName(Char.AssignedName), Action(0), Config(Char.Config), DataBase(Char.DataBase), StuckToBodyPart(NONE_INDEX), StuckTo(0), MotherEntity(0), PolymorphBackup(0), EquipmentState(0), SquareUnder(0), Initializing(true), AllowedWeaponSkillCategories(Char.AllowedWeaponSkillCategories), BodyParts(Char.BodyParts), Polymorphed(false), InNoMsgMode(true), RegenerationCounter(Char.RegenerationCounter)
{
  Stack = new stack(0, this, HIDDEN, true);

  ushort c;

  for(c = 0; c < STATES; ++c)
    TemporaryStateCounter[c] = Char.TemporaryStateCounter[c];

  if(Team)
    TeamIterator = Team->Add(this);

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    {
      BaseAttribute[c] = Char.BaseAttribute[c];
      BaseExperience[c] = Char.BaseExperience[c];
    }

  BodyPartSlot = new characterslot[BodyParts];
  OriginalBodyPartID = new std::list<ulong>[BodyParts];
  CWeaponSkill = new cweaponskill*[AllowedWeaponSkillCategories];

  for(c = 0; c < BodyParts; ++c)
    {
      BodyPartSlot[c].SetMaster(this);

      if(Char.GetBodyPart(c))
	SetBodyPart(c, static_cast<bodypart*>(Char.GetBodyPart(c)->Duplicate()));
      else
	OriginalBodyPartID[c] = Char.OriginalBodyPartID[c];
    }

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    CWeaponSkill[c] = new cweaponskill(*Char.CWeaponSkill[c]);

  Initializing = InNoMsgMode = false;
}

character::character(donothing) : entity(true), NP(50000), AP(0), Player(false), TemporaryState(0), Team(0), WayPoint(-1, -1), Money(0), HomeRoom(0), Action(0), StuckToBodyPart(NONE_INDEX), StuckTo(0), MotherEntity(0), PolymorphBackup(0), EquipmentState(0), SquareUnder(0), Polymorphed(false), RegenerationCounter(0)
{
  Stack = new stack(0, this, HIDDEN, true);
}

character::~character()
{
  if(Action)
    {
      Action->DeleteUsedItems();
      delete Action;
    }

  if(Team)
    Team->Remove(GetTeamIterator());

  delete Stack;
  ushort c;

  for(c = 0; c < BodyParts; ++c)
    delete GetBodyPart(c);

  delete [] BodyPartSlot;
  delete [] OriginalBodyPartID;
  delete PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    delete CWeaponSkill[c];

  delete [] CWeaponSkill;
}

void character::Hunger() 
{
  switch(GetBurdenState())
    {
    case UNBURDENED:
      EditNP(-1);
      break;
    case BURDENED:
      EditNP(-2);
      EditExperience(LEG_STRENGTH, 2);
      EditExperience(AGILITY, -2);
      break;
    case STRESSED:
    case OVER_LOADED:
      EditNP(-4);
      EditExperience(LEG_STRENGTH, 4);
      EditExperience(AGILITY, -4);
      break;
    }

  if(GetHungerState() == HUNGRY || GetHungerState() == STARVING)
    {
      EditExperience(ARM_STRENGTH, -1);
      EditExperience(LEG_STRENGTH, -1);
    }

  if(GetHungerState() == SATIATED || GetHungerState() == BLOATED)
    EditExperience(AGILITY, -1);

  CheckStarvationDeath("starved to death");
}

uchar character::TakeHit(character* Enemy, item* Weapon, float Damage, float ToHitValue, short Success, uchar Type, bool Critical)
{
  DeActivateVoluntaryAction("The attack of " + Enemy->GetName(DEFINITE) + " interupts you.");
  uchar Dir = Type == BITE_ATTACK ? YOURSELF : game::GetDirectionForVector(GetPos() - Enemy->GetPos());
  float DodgeValue = GetDodgeValue();

  if(!Enemy->CanBeSeenBy(this))
    ToHitValue *= 2;

  if(!CanBeSeenBy(Enemy))
    DodgeValue *= 2;

  WayPoint = Enemy->GetPos();

  /* Effectively, the average chance to hit is 100% / (DV/THV + 1). */

  if(!Critical && RAND() % ushort(100 + ToHitValue / DodgeValue * (100 + Success)) < 100)
    {
      Enemy->AddMissMessage(this);
      EditExperience(AGILITY, 50);
      EditExperience(PERCEPTION, 25);
      return HAS_DODGED;
    }

  ushort TrueDamage = ushort(Damage * (100 + Success) / 100) + (RAND() % 3 ? 1 : 0);

  if(Critical)
    {
      TrueDamage <<= 1;
      ++TrueDamage;
    }

  uchar BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, DodgeValue);

  if(Critical)
    {
      switch(Type)
	{
	case UNARMED_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonCriticalUnarmedHitVerb(), Enemy->ThirdPersonCriticalUnarmedHitVerb(), BodyPart);
	  break;
	case WEAPON_ATTACK:
          Enemy->AddWeaponHitMessage(this, Weapon, BodyPart, true);
	  break;
	case KICK_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonCriticalKickVerb(), Enemy->ThirdPersonCriticalKickVerb(), BodyPart);
	  break;
	case BITE_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonCriticalBiteVerb(), Enemy->ThirdPersonCriticalBiteVerb(), BodyPart);
	  break;
	}
    }
  else
    {
      switch(Type)
	{
	case UNARMED_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonUnarmedHitVerb(), Enemy->ThirdPersonUnarmedHitVerb(), BodyPart);
	  break;
	case WEAPON_ATTACK:
	  Enemy->AddWeaponHitMessage(this, Weapon, BodyPart, false);
	  break;
	case KICK_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonKickVerb(), Enemy->ThirdPersonKickVerb(), BodyPart);
	  break;
	case BITE_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonBiteVerb(), Enemy->ThirdPersonBiteVerb(), BodyPart);
	  break;
	}
    }

  if(!Critical && TrueDamage && Enemy->AttackIsBlockable(Type))
    {
      TrueDamage = CheckForBlock(Enemy, Weapon, ToHitValue, TrueDamage, Success, Type);

      if(!TrueDamage || (Weapon && !Weapon->Exists()))
	return HAS_BLOCKED;
    }

  ushort DoneDamage = ReceiveBodyPartDamage(Enemy, TrueDamage, PHYSICAL_DAMAGE, BodyPart, Dir, false, Critical);
  bool Succeeded = (GetBodyPart(BodyPart) && HitEffect(Enemy, Weapon, Type, BodyPart, Dir, !DoneDamage)) || DoneDamage;

  if(Succeeded)
    Enemy->WeaponSkillHit(Weapon, Type);

  if(Weapon && DoneDamage < TrueDamage)
    Weapon->ReceiveDamage(Enemy, TrueDamage - DoneDamage, PHYSICAL_DAMAGE);

  if(!Succeeded)
    return DID_NO_DAMAGE;

  if(CheckDeath("killed by " + Enemy->GetName(INDEFINITE), Enemy->IsPlayer()))
    return HAS_DIED;

  return HAS_HIT;
}

struct svpriorityelement
{
  svpriorityelement(uchar BodyPart, uchar StrengthValue) : BodyPart(BodyPart), StrengthValue(StrengthValue) { }
  bool operator<(const svpriorityelement& AnotherPair) const { return StrengthValue > AnotherPair.StrengthValue; }
  uchar BodyPart;
  ushort StrengthValue;
};

uchar character::ChooseBodyPartToReceiveHit(float ToHitValue, float DodgeValue)
{
  if(GetBodyParts() == 1)
    return 0;

  std::priority_queue<svpriorityelement> SVQueue;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      SVQueue.push(svpriorityelement(c, GetBodyPart(c)->GetStrengthValue() + GetBodyPart(c)->GetHP()));

  while(SVQueue.size())
    {
      ushort ToHitPercentage = ushort(GLOBAL_WEAK_BODYPART_HIT_MODIFIER * ToHitValue * GetBodyPart(SVQueue.top().BodyPart)->GetBodyPartVolume() / (DodgeValue * GetBodyVolume()));

      if(ToHitPercentage < 1)
	ToHitPercentage = 1;
      else if(ToHitPercentage > 95)
	ToHitPercentage = 95;

      if(ToHitPercentage > RAND() % 100)
	return SVQueue.top().BodyPart;

      SVQueue.pop();
    }

  return 0;
}

void character::Be()
{
  if(game::IsLoading())
    {
      if(!IsPlayer())
	return;
      else
	game::SetIsLoading(false);
    }
  else
    {
      HandleStates();

      if(!IsEnabled())
	return;

      if(GetTeam() == game::GetPlayer()->GetTeam())
	{
	  for(ushort c = 0; c < AllowedWeaponSkillCategories; ++c)
	    if(CWeaponSkill[c]->Tick() && IsPlayer())
	      CWeaponSkill[c]->AddLevelDownMessage();

	  SWeaponSkillTick();
	}

      if(IsPlayer())
	{
	  if(GetHungerState() == STARVING && !(RAND() % 50))
	    Faint();

	  if(!Action || Action->AllowFoodConsumption())
	    Hunger();
	}

      if(HP != MaxHP)
	Regenerate();

      if(Action && AP >= 1000)
	ActionAutoTermination();

      if(Action && AP >= 1000)
	{
	  Action->Handle();

	  if(!IsEnabled())
	    return;
	}
      else
	EditAP(GetStateAPGain(100));
    }

  if(AP >= 1000)
    {
      if(!GetAction())
	ApplyExperience();

      SpecialTurnHandler();

      if(IsPlayer())
	{
	  static ushort Timer = 0;

	  if(configuration::GetAutoSaveInterval() && !GetAction() && ++Timer >= configuration::GetAutoSaveInterval())
	    {
	      game::Save(game::GetAutoSaveFileName());
	      Timer = 0;
	    }

	  game::CalculateNextDanger();

	  if(!GetAction())
	    GetPlayerCommand();
	  else
	    {
	      game::DrawEverything();

	      if(READ_KEY() && GetAction()->IsVoluntary())
		GetAction()->Terminate(false);
	    }
	}
      else
	{
	  if(!Action && !game::IsInWilderness())
	    GetAICommand();
	}
    }
}

bool character::GoUp()
{
  if(GetSquareUnder()->GetOTerrain()->Enter(true))
    {
      EditExperience(LEG_STRENGTH, 50);
      EditNP(-20);
      EditAP(-100000 / APBonus(GetAttribute(AGILITY)));
      return true;
    }
  else
    return false;
}

bool character::GoDown()
{
  if(GetSquareUnder()->GetOTerrain()->Enter(false))
    {
      EditExperience(AGILITY, 25);
      EditNP(-10);
      EditAP(-100000 / APBonus(GetAttribute(AGILITY)));
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
	Key = game::AskForKeyPress("What do you wish to open?  [press a direction key, space or i]");
      else
	Key = game::AskForKeyPress("What do you wish to open?  [press a direction key or space]");

      if(Key == 'i' && OpenableItems)
	return OpenItem();

      vector2d DirVect = game::GetDirectionVectorForKey(Key);

      if(DirVect != DIR_ERROR_VECTOR && GetAreaUnder()->IsValidPos(GetPos() + DirVect))
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
      uchar Dir = game::DirectionQuestion("What do you wish to close?  [press a direction key]", false);

      if(Dir != DIR_ERROR && GetAreaUnder()->IsValidPos(GetPos() + game::GetMoveVector(Dir)))
	return ClosePos(GetPos() + game::GetMoveVector(Dir));
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

  std::vector<item*> ToDrop;
  GetStack()->DrawContents(ToDrop, this, "What do you want to drop?");

  if(ToDrop.empty())
    return false;

  if(!GetRoomUnder() || GetRoomUnder()->DropItem(this, ToDrop[0], ToDrop.size()))
    {
      for(ushort c = 0; c < ToDrop.size(); ++c)
	ToDrop[c]->MoveTo(GetStackUnder());

      ADD_MESSAGE("%s dropped.", ToDrop[0]->GetName(INDEFINITE, ToDrop.size()).c_str());
      DexterityAction(1);
      return true;
    }

  return false;
}

bool character::Eat()
{
  if(!game::IsInWilderness() && GetLSquareUnder()->GetOLTerrain()->HasEatEffect())
    {
      if(GetLSquareUnder()->GetOLTerrain()->Eat(this))
	return true;
    }

  if((game::IsInWilderness() || !GetStackUnder()->SortedItems(this, &item::EatableSorter)) && !GetStack()->SortedItems(this, &item::EatableSorter))
    {
      ADD_MESSAGE("You have nothing to eat!");
      return false;
    }

  std::vector<item*> Item;

  if(!game::IsInWilderness() && GetStackUnder()->SortedItems(this, &item::EatableSorter))
    GetStack()->DrawContents(Item, GetStackUnder(), this, "What do you wish to eat?", "Items in your inventory", "Items on the ground", 0, &item::EatableSorter);
  else
    GetStack()->DrawContents(Item, this, "What do you wish to eat?", 0, &item::EatableSorter);

  return !Item.empty() ? ConsumeItem(Item[0]) : false;
}

bool character::Drink()
{
  if(!game::IsInWilderness() && GetLSquareUnder()->GetOLTerrain()->HasDrinkEffect())
    {
      if(GetLSquareUnder()->GetOLTerrain()->Drink(this))
	return true;
    }

  if((game::IsInWilderness() || !GetStackUnder()->SortedItems(this, &item::DrinkableSorter)) && !GetStack()->SortedItems(this, &item::DrinkableSorter))
    {
      ADD_MESSAGE("You have nothing to drink!");
      return false;
    }

  std::vector<item*> Item;

  if(!game::IsInWilderness() && GetStackUnder()->SortedItems(this, &item::DrinkableSorter))
    GetStack()->DrawContents(Item, GetStackUnder(), this, "What do you wish to drink?", "Items in your inventory", "Items on the ground", 0, &item::DrinkableSorter);
  else
    GetStack()->DrawContents(Item, this, "What do you wish to drink?", 0, &item::DrinkableSorter);

  return !Item.empty() ? ConsumeItem(Item[0]) : false;
}

bool character::ConsumeItem(item* Item)
{
  if(HasHadBodyPart(Item) && !game::BoolQuestion("Are you sure? You may be able to put it back... [y/N]"))
    return false;

  Item = Item->PrepareForConsuming(this);

  if(!Item)
    return false;

  if(IsPlayer())
    ADD_MESSAGE("You begin %s %s.", Item->GetConsumeVerb().c_str(), Item->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s begins %s %s.", CHAR_NAME(DEFINITE), Item->GetConsumeVerb().c_str(), Item->CHAR_NAME(DEFINITE));

  consume* Consume = new consume(this);
  Consume->SetDescription(Item->GetConsumeVerb());
  Consume->SetWasOnGround(Item->IsOnGround());
  Consume->SetConsuming(Item);
  SetAction(Consume);
  DexterityAction(5);
  return true;
}

void character::Move(vector2d MoveTo, bool TeleportMove)
{
  /* Test whether the player is stuck to something */

  if(!TeleportMove && IsStuck() && !TryToUnstuck(MoveTo - GetPos()))
    return;

  if(GetBurdenState() != OVER_LOADED || TeleportMove)
    {
      GetAreaUnder()->MoveCharacter(GetPos(), MoveTo);

      if(IsPlayer())
	ShowNewPosInfo();

      if(!TeleportMove)
	{
	  EditAP(-GetMoveAPRequirement(GetSquareUnder()->GetEntryDifficulty()));
	  EditNP(-10 * GetSquareUnder()->GetEntryDifficulty());
	  EditExperience(AGILITY, 5 * GetSquareUnder()->GetEntryDifficulty());
	}
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("You try very hard to crawl forward. But your load is too heavy.");

      EditAP(-1000);
    }
}

void character::GetAICommand()
{
  SeekLeader();

  if(CheckForEnemies(true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(CheckForDoors())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

/* TPos = Target Pos */

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

  if(GetPos().IsAdjacent(TPos))
    return false;

  if(RAND() & 1)
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
  if(!game::IsInWilderness())
    {
      if(GetLevelUnder()->IsValidPos(MoveTo))
	{
	  lsquare* MoveToSquare = GetNearLSquare(MoveTo);
	  character* Character = MoveToSquare->GetCharacter();

	  if(Character)
	    {
	      if(IsPlayer())
		{
		  if(GetTeam() != Character->GetTeam())
		    return Hit(Character);
		  else
		    if(DisplaceAllowed && (MoveToSquare->IsWalkable(this) || game::GetGoThroughWallsCheat()))
		      {
			if(!Displace(Character))
			  EditAP(-1000);

			return true;
		      }
		    else
		      return false;
		}
	      else
		{
		  if(GetRelation(Character) == HOSTILE)
		    return Hit(Character);
		  else
		    if(GetTeam() == Character->GetTeam() && DisplaceAllowed && MoveToSquare->IsWalkable(this))
		      return Displace(Character);
		    else
		      return false;
		}
	    }
	  else
	    {
	      if(MoveToSquare->IsWalkable(this) || (game::GetGoThroughWallsCheat() && IsPlayer()))
		{
		  Move(MoveTo);
		  return true;
		}
	      else
		{
		  olterrain* Terrain = MoveToSquare->GetOLTerrain();

		  if(IsPlayer() && Terrain->CanBeOpened())
		    {
		      if(CanOpen())
			{
			  if(Terrain->IsLocked())
			    {
			      ADD_MESSAGE("%s is locked.", Terrain->CHAR_NAME(DEFINITE));
			      return false;
			    }
			  else
			    {
			      if(game::BoolQuestion("Do you want to open " + Terrain->GetName(UNARTICLED) + "? [y/N]", false, game::GetMoveCommandKeyBetweenPoints(game::GetPlayer()->GetPos(), MoveTo)))
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
	    }
	}
      else
	{
	  if(IsPlayer() && GetLevelUnder()->GetOnGround() && game::BoolQuestion("Do you want to leave " + game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrent()) + "? [y/N]"))
	    {
	      if(HasPetrussNut() && !HasGoldenEagleShirt())
		{
		  game::TextScreen("An undead and sinister voice greets you as you leave the city behind:\n\n\"MoRtAl! ThOu HaSt SlAuGtHeReD pEtRuS aNd PlEaSeD mE!\nfRoM tHiS dAy On, YoU aRe ThE dEaReSt SeRvAnT oF aLl EvIl!\"\n\nYou are victorious!");
		  game::GetPlayer()->AddScoreEntry("killed Petrus and became the Avatar of Chaos", 3, false);
		  game::End();
		  return true;
		}

	      std::vector<character*> Group;

	      if(game::LeaveLevel(Group, false))
		{
		  game::EnterArea(Group, WORLD_MAP, game::GetCurrentDungeon()->GetIndex());
		  return true;
		}
	    }

	  return false;
	}
    }
  else
    {
      if(GetAreaUnder()->IsValidPos(MoveTo) && (GetNearSquare(MoveTo)->IsWalkable(this) || game::GetGoThroughWallsCheat()))
	{
	  Move(MoveTo);
	  return true;
	}
      else
	return false;
    }
}

bool character::ShowInventory()
{
  GetStack()->DrawContents(this, "Your inventory", NO_SELECT);
  return false;
}

bool character::PickUp()
{
  ushort VisibleItemsOnGround = GetStackUnder()->GetVisibleItems(this);

  if(!VisibleItemsOnGround)
    {
      ADD_MESSAGE("There is nothing here to pick up, %s!", game::Insult());
      return false;
    }

  std::vector<std::vector<item*> > PileVector;
  GetStackUnder()->Pile(PileVector, this);

  if(PileVector.size() == 1)
    {
      ushort Amount = PileVector[0].size();

      if(Amount > 1)
	Amount = game::ScrollBarQuestion("How many " + PileVector[0][0]->GetName(PLURAL) + '?', vector2d(16, 6), 1, 1, 0, Amount, WHITE, LIGHT_GRAY, DARK_GRAY);

      if((!GetRoomUnder() || GetRoomUnder()->PickupItem(this, PileVector[0][0], Amount)) && PileVector[0][0]->CheckPickUpEffect(this))
	{
	  for(ushort c = 0; c < Amount; ++c)
	    PileVector[0][c]->MoveTo(GetStack());

	  ADD_MESSAGE("%s picked up.", PileVector[0][0]->GetName(INDEFINITE, Amount).c_str());
	  DexterityAction(2);
	  return true;
	}
      else
	return false;
    }

  bool Success = false;
  stack::SetSelected(0);

  for(;;)
    {
      std::vector<item*> ToPickup;
      game::DrawEverythingNoBlit();
      GetStackUnder()->DrawContents(ToPickup, this, "What do you want to pick up?", REMEMBER_SELECTED);

      if(ToPickup.empty())
	break;

      if((!GetRoomUnder() || GetRoomUnder()->PickupItem(this, ToPickup[0], ToPickup.size())) && ToPickup[0]->CheckPickUpEffect(this))
	{
	  for(ushort c = 0; c < ToPickup.size(); ++c)
	    ToPickup[c]->MoveTo(GetStack());

	  ADD_MESSAGE("%s picked up.", ToPickup[0]->GetName(INDEFINITE, ToPickup.size()).c_str());
	  Success = true;
	}
    }

  if(Success)
    {
      DexterityAction(2);
      return true;
    }
  else
    return false;
}

bool character::Quit()
{
  if(game::BoolQuestion("Thine Holy Quest is not yet compeleted! Really quit? [y/N]"))
    {
      AddScoreEntry("cowardly quit the game", 0.75f);
      game::End(!game::WizardModeActivated() || game::BoolQuestion("Remove saves? [y/N]"));
      return true;
    }
  else
    return false;
}

void character::CreateCorpse()
{
  corpse* Corpse = new corpse(0, NO_MATERIALS);
  Corpse->SetDeceased(this);
  GetStackUnder()->AddItem(Corpse);
  SetHasBe(false);
}

void character::Die(bool ForceMsg)
{
  // Note for programmers: This function MUST NOT delete any objects in any case! 

  if(!IsEnabled())
    return;

  if(IsPlayer())
    {
      ADD_MESSAGE("You die.");

      if(game::WizardModeActivated())
	{
	  game::DrawEverything();

	  if(!game::BoolQuestion("Do you want to do this, cheater? [y/n]", REQUIRES_ANSWER))
	    {
	      RestoreBodyParts();
	      RestoreHP();
	      SetNP(50000);
	      GetSquareUnder()->SendNewDrawRequest();
	      return;
	    }
	}
    }
  else
    if(CanBeSeenByPlayer())
      ADD_MESSAGE(GetDeathMessage().c_str());
    else if(ForceMsg)
      ADD_MESSAGE("You sense the death of something.");

  if(StateIsActivated(LIFE_SAVED))
    {
      SaveLife();
      return;
    }

  InNoMsgMode = true;

  if(IsPlayer())
    game::RemoveSaves();

  if(HomeRoom && GetLevelUnder()->GetRoom(HomeRoom)->GetMaster() == this)
    GetLevelUnder()->GetRoom(HomeRoom)->SetMaster(0);

  GetSquareUnder()->RemoveCharacter();

  if(!game::IsInWilderness())
    CreateCorpse();

  if(IsPlayer())
    {
      if(!game::IsInWilderness())
	GetLSquareUnder()->SetTemporaryEmitation(GetEmitation());

      if(GetStack()->GetItems())
	if(game::BoolQuestion("Do you want to see your inventory? [y/n]", REQUIRES_ANSWER))
	  {
	    GetStack()->DrawContents(this, "Your inventory", NO_SELECT);

	    for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
	      i->DrawContents(this);

	    for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	      if(GetEquipment(c))
		GetEquipment(c)->DrawContents(this);
	  }

      if(game::BoolQuestion("Do you want to see your message history? [y/n]", REQUIRES_ANSWER))
	DrawMessageHistory();

      if(!game::IsInWilderness())
	GetLSquareUnder()->SetTemporaryEmitation(0);
    }

  if(!game::IsInWilderness())
    {
      GetStack()->MoveItemsTo(GetLSquareUnder()->GetStack());

      for(ushort c = 0; c < GetBodyParts(); ++c)
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

      for(ushort c = 0; c < GetBodyParts(); ++c)
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
	  GetStack()->GetBottom()->SendToHell();
	  GetStack()->GetBottom()->RemoveFromSlot();
	}
    }

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(0);

  InNoMsgMode = false;

  if(IsPlayer())
    {
      game::TextScreen("Unfortunately thee died during thine journey. The High Priest is not happy.");
      game::End();
    }
}

bool character::OpenItem()
{
  item* Item = Stack->DrawContents(this, "What do you want to open?", 0, &item::OpenableSorter);
  return Item && Item->Open(this);
}

void character::AddMissMessage(const character* Enemy) const
{
  std::string Msg;

  if(Enemy->IsPlayer())
    Msg = GetDescription(DEFINITE) + " misses you!";
  else if(IsPlayer())
    Msg = "You miss " + Enemy->GetDescription(DEFINITE) + "!";
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg = GetDescription(DEFINITE) + " misses " + Enemy->GetDescription(DEFINITE) + "!";
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::AddBlockMessage(const character* Enemy, const item* Blocker, const std::string& HitNoun, bool Partial) const
{
  std::string Msg;
  std::string BlockVerb = (Partial ? " to partially block the " : " to block the ") + HitNoun;

  if(IsPlayer())
    Msg << "You manage" << BlockVerb << " with your " << Blocker->GetName(UNARTICLED) << "!";
  else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg << GetDescription(DEFINITE) << " manages" << BlockVerb << " with " << GetPossessivePronoun() << " " << Blocker->GetName(UNARTICLED) << "!";
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::AddPrimitiveHitMessage(const character* Enemy, const std::string& FirstPersonHitVerb, const std::string& ThirdPersonHitVerb, uchar BodyPart) const
{
  std::string Msg;
  std::string BodyPartDescription = BodyPart && (Enemy->CanBeSeenByPlayer() || Enemy->IsPlayer()) ? " in the " + Enemy->GetBodyPartName(BodyPart) : "";

  if(Enemy->IsPlayer())
    Msg << GetDescription(DEFINITE) << " " << ThirdPersonHitVerb << " you" << BodyPartDescription << "!";
  else if(IsPlayer())
    Msg << "You " << FirstPersonHitVerb << " " << Enemy->GetDescription(DEFINITE) << BodyPartDescription << "!";
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg << GetDescription(DEFINITE) << " " << ThirdPersonHitVerb << " " << Enemy->GetDescription(DEFINITE) + BodyPartDescription << "!";
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::AddWeaponHitMessage(const character* Enemy, const item* Weapon, uchar BodyPart, bool Critical) const
{
  std::string Msg;
  std::string BodyPartDescription = BodyPart && (Enemy->CanBeSeenByPlayer() || Enemy->IsPlayer()) ? " in the " + Enemy->GetBodyPartName(BodyPart) : "";

  if(Enemy->IsPlayer())
    {
      Msg << GetDescription(DEFINITE) << (Critical ? " critically hits you" : " hits you") << BodyPartDescription;

      if(CanBeSeenByPlayer())
	Msg << " with " << GetPossessivePronoun() << " " << Weapon->GetName(UNARTICLED);

      Msg << "!";
    }
  else if(IsPlayer())
    Msg << "You " << (Critical ? "critically hit " : "hit ") << Enemy->GetDescription(DEFINITE) << BodyPartDescription << "!";
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    {
      Msg << GetDescription(DEFINITE) << (Critical ? " critically hits " : " hits ") << Enemy->GetDescription(DEFINITE) << BodyPartDescription;

      if(CanBeSeenByPlayer())
	Msg << " with " << GetPossessivePronoun() << " " << Weapon->GetName(UNARTICLED);

      Msg << "!";
    }
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

bool character::Talk()
{
  if(!CanTalk())
    {
      ADD_MESSAGE("This race does not know the art of talking.");
      return false;
    }

  character* ToTalk = 0;
  ushort Characters = 0;


  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char)
	    {
	      ToTalk = Char;
	      ++Characters;
	    }
	}
    }

  if(!Characters)
    {
      ADD_MESSAGE("Find yourself someone to talk to first!");
      return false;
    }
  else if(Characters == 1)
    {
      ToTalk->BeTalkedTo();
      EditAP(-1000);
      return true;
    }
  else
    {
      uchar Dir = game::DirectionQuestion("To whom do you wish to talk to? [press a direction key]", false, true);

      if(Dir == DIR_ERROR || !GetAreaUnder()->IsValidPos(GetPos() + game::GetMoveVector(Dir)))
	return false;

      if(Dir == YOURSELF)
	{
	  ADD_MESSAGE("You talk to yourself for some time.");
	  EditAP(-1000);
	  return true;
	}

      character* Char = GetNearSquare(GetPos() + game::GetMoveVector(Dir))->GetCharacter();

      if(Char)
	{
	  Char->BeTalkedTo();
	  EditAP(-1000);
	  return true;
	}
      else
	ADD_MESSAGE("You get no response.");
    }

  return false;
}

bool character::NOP()
{
  EditExperience(AGILITY, -10);
  EditAP(-1000);
  return true;
}

void character::ApplyExperience(bool Edited)
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->ApplyExperience())
      Edited = true;

  if(IsAlive())
    if(CheckForAttributeIncrease(BaseAttribute[ENDURANCE], BaseExperience[ENDURANCE]))
      {
	if(IsPlayer())
	  ADD_MESSAGE("You feel tougher than anything!");
	else if(CanBeSeenByPlayer())
	  ADD_MESSAGE("Suddenly %s looks tougher.", CHAR_NAME(DEFINITE));

	CalculateBodyPartMaxHPs();
	Edited = true;
      }
    else if(CheckForAttributeDecrease(BaseAttribute[ENDURANCE], BaseExperience[ENDURANCE]))
      {
	if(IsPlayer())
	  ADD_MESSAGE("You feel less healthy.");
	else if(CanBeSeenByPlayer())
	  ADD_MESSAGE("Suddenly %s looks less healthy.", CHAR_NAME(DEFINITE));

	CalculateBodyPartMaxHPs();
	Edited = true;
      }

  if(CheckForAttributeIncrease(BaseAttribute[PERCEPTION], BaseExperience[PERCEPTION]))
    {
      if(IsPlayer())
	{
	  ADD_MESSAGE("Your don't feel as guru anymore.");
	  game::SendLOSUpdateRequest();
	}

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[PERCEPTION], BaseExperience[PERCEPTION]))
    {
      if(IsPlayer())
	{
	  ADD_MESSAGE("You feel very guru.");
	  game::GetGod(1)->AdjustRelation(100);
	  game::SendLOSUpdateRequest();
	}

      Edited = true;
    }

  if(CheckForAttributeIncrease(BaseAttribute[INTELLIGENCE], BaseExperience[INTELLIGENCE]))
    {
      if(IsPlayer())
	ADD_MESSAGE("Suddenly the inner structure of the Multiverse around you looks quite simple.");

      if(IsPlayer())
	UpdateESPLOS();

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[INTELLIGENCE], BaseExperience[INTELLIGENCE]))
    {
      if(IsPlayer())
	ADD_MESSAGE("It surely is hard to think today.");

      if(IsPlayer())
	UpdateESPLOS();

      Edited = true;
    }

  if(CheckForAttributeIncrease(BaseAttribute[WISDOM], BaseExperience[WISDOM]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel your life experience increasing all the time.");

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[WISDOM], BaseExperience[WISDOM]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel like having done something unwise.");

      Edited = true;
    }

  if(CheckForAttributeIncrease(BaseAttribute[CHARISMA], BaseExperience[CHARISMA]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel very confident of your appearance.");
      else if(CanBeSeenByPlayer())
	{
	  if(GetAttribute(CHARISMA) <= 15)
	    ADD_MESSAGE("%s looks less ugly.", CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s looks more attractive.", CHAR_NAME(DEFINITE));
	}

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[CHARISMA], BaseExperience[CHARISMA]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel somehow disliked.");
      else if(CanBeSeenByPlayer())
	{
	  if(GetAttribute(CHARISMA) < 15)
	    ADD_MESSAGE("%s looks more ugly.", CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s looks less attractive.", CHAR_NAME(DEFINITE));
	}

      Edited = true;
    }

  if(CheckForAttributeIncrease(BaseAttribute[MANA], BaseExperience[MANA]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel magical forces coursing through your body!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("You notice an odd glow around %s.", CHAR_NAME(DEFINITE));

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[MANA], BaseExperience[MANA]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel your magical abilities withering slowly.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("You notice strange vibrations in the air around %s. But they disappear rapidly.", CHAR_NAME(DEFINITE));

      Edited = true;
    }

  if(Edited)
    CalculateBattleInfo();
}

bool character::HasHeadOfElpuri() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsHeadOfElpuri())
      return true;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsHeadOfElpuri())
      return true;

  return false;
}

bool character::HasPetrussNut() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsPetrussNut())
      return true;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsPetrussNut())
      return true;

  return false;
}

bool character::HasGoldenEagleShirt() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsGoldenEagleShirt())
      return true;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsGoldenEagleShirt())
      return true;

  return false;
}

bool character::Save()
{
  if(game::BoolQuestion("Dost thee truly wish to save and flee? [y/N]"))
    {
      game::Save();
      game::End(false);
      return true;
    }
  else
    return false;
}

bool character::Read()
{
  if(!CanRead() && !game::GetSeeWholeMapCheat())
    {
      ADD_MESSAGE("You can't read.");
      return false;
    }

  if(!GetStack()->SortedItems(this, &item::ReadableSorter))
    {
      ADD_MESSAGE("You have nothing to read!");
      return false;
    }

  item* Item = GetStack()->DrawContents(this, "What do you want to read?", 0, &item::ReadableSorter);
  return Item && ReadItem(Item);
}

bool character::ReadItem(item* ToBeRead)
{
  if(ToBeRead->CanBeRead(this))
    {
      if(!GetLSquareUnder()->IsDark() || game::GetSeeWholeMapCheat())
	{
	  if(ToBeRead->Read(this))
	    {
	      /* This AP is used to take the stuff out of backpack */
	      DexterityAction(5);
	      return true;
	    }
	  else
	    return false;
	}
      else
	{
	  if(IsPlayer())
	    ADD_MESSAGE("It's too dark here to read.");

	  return false;
	}
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("You can't read this.");

      return false;
    }
}

void character::CalculateBurdenState()
{
  uchar OldBurdenState = BurdenState;
  ulong SumOfMasses = GetCarriedWeight();

  if(SumOfMasses > ulong(GetCarryingStrength()) * 7500)
    BurdenState = OVER_LOADED;
  else if(SumOfMasses > ulong(GetCarryingStrength()) * 5000)
    BurdenState = STRESSED;
  else if(SumOfMasses > ulong(GetCarryingStrength()) * 2500)
    BurdenState = BURDENED;
  else
    BurdenState = UNBURDENED;

  if(!Initializing && BurdenState != OldBurdenState)
    CalculateBattleInfo();
}

bool character::Dip()
{
  if(!GetStack()->SortedItems(this, &item::DippableSorter))
    {
      ADD_MESSAGE("You have nothing to dip!");
      return false;
    }

  item* Item = GetStack()->DrawContents(this, "What do you want to dip?", 0, &item::DippableSorter);

  if(Item)
    {
      bool HasDipDestination = GetStack()->SortedItems(this, &item::DipDestinationSorter);

      if(!HasDipDestination || game::BoolQuestion("Do you wish to dip in a nearby square? [y/N]"))
	{
	  uchar Dir = game::DirectionQuestion("Where do you want to dip " + Item->GetName(DEFINITE) + "? [press a direction key or '.']", false, true);

	  if(Dir == DIR_ERROR || !GetAreaUnder()->IsValidPos(GetPos() + game::GetMoveVector(Dir)))
	    return false;
	  
	  return GetNearLSquare(GetPos() + game::GetMoveVector(Dir))->DipInto(Item, this);
	}
      else
	{
	  item* DipTo = GetStack()->DrawContents(this, "Into what do you wish to dip it?", 0, &item::DipDestinationSorter);

	  if(DipTo)
	    {
	      if(Item == DipTo)
		{
		  ADD_MESSAGE("Very funny...");
		  return false;
		}

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
  Stack->Save(SaveFile);

  ushort c;

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    SaveFile << BaseAttribute[c] << BaseExperience[c];

  SaveFile << NP << AP;
  SaveFile << TemporaryState << EquipmentState << Money << HomeRoom << WayPoint << Config << RegenerationCounter;
  SaveFile << HasBe() << Polymorphed;

  for(c = 0; c < BodyParts; ++c)
    SaveFile << BodyPartSlot[c] << OriginalBodyPartID[c];

  if(HomeRoom)
    if(!game::IsInWilderness() && GetSquareUnder() && GetLevelUnder()->GetRoom(HomeRoom)->GetMaster() == this)
      SaveFile << bool(true);
    else
      SaveFile << bool(false);

  SaveFile << Action;

  for(c = 0; c < STATES; ++c)
    SaveFile << TemporaryStateCounter[c];

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

  SaveFile << AssignedName << PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    SaveFile << GetCWeaponSkill(c);

  if(IsStuck())
    SaveFile << bool(true) << GetStackUnder()->SearchItem(StuckTo) << StuckToBodyPart;
  else
    SaveFile << bool(false);
}

void character::Load(inputfile& SaveFile)
{
  SquareUnder = game::GetSquareInLoad();
  Stack->Load(SaveFile);

  ushort c;

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    SaveFile >> BaseAttribute[c] >> BaseExperience[c];

  SaveFile >> NP >> AP;
  SaveFile >> TemporaryState >> EquipmentState >> Money >> HomeRoom >> WayPoint >> Config >> RegenerationCounter;
  SetHasBe(ReadType<bool>(SaveFile));
  SaveFile >> Polymorphed;

  for(c = 0; c < BodyParts; ++c)
    SaveFile >> BodyPartSlot[c] >> OriginalBodyPartID[c];

  if(HomeRoom)
    if(ReadType<bool>(SaveFile))
      GetLevelUnder()->GetRoom(HomeRoom)->SetMaster(this);

  SaveFile >> Action;

  if(Action)
    Action->SetActor(this);

  for(c = 0; c < STATES; ++c)
    SaveFile >> TemporaryStateCounter[c];

  if(ReadType<bool>(SaveFile))
    SetTeam(game::GetTeam(ReadType<ushort>(SaveFile)));

  if(ReadType<bool>(SaveFile))
    GetTeam()->SetLeader(this);

  SaveFile >> AssignedName >> PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    SaveFile >> GetCWeaponSkill(c);

  if(ReadType<bool>(SaveFile))
    {
      StuckTo = GetStackUnder()->GetItem(ReadType<ushort>(SaveFile));
      SaveFile >> StuckToBodyPart;
    }

  InstallDataBase();
}

bool character::WizardMode()
{
  if(!game::WizardModeActivated())
    {
      if(game::BoolQuestion("Do you want to cheat, cheater? This action cannot be undone. [y/N]"))
	{
	  game::ActivateWizardMode();
	  ADD_MESSAGE("Wizard mode activated.");

	  for(ushort x = 0; x < 5; ++x)
	    GetStack()->AddItem(new scrollofwishing);

	  game::Save();
	  game::Save(game::GetAutoSaveFileName());
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
  ushort c = 0;

  for(c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->RaiseStats();

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    BaseAttribute[c] += 10;

  CalculateAll();
  RestoreHP();
  game::SendLOSUpdateRequest();
  UpdateESPLOS();
  return false;
}

bool character::LowerStats()
{
  ushort c = 0;

  for(c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->LowerStats();

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    BaseAttribute[c] -= 10;

  CalculateAll();
  RestoreHP();
  game::SendLOSUpdateRequest();
  UpdateESPLOS();
  return false;
}

bool character::GainAllItems()
{
  std::vector<item*> AllItems;
  protosystem::CreateEveryItem(AllItems);

  for(ushort c = 0; c < AllItems.size(); ++c)
    GetStack()->AddItem(AllItems[c]);

  return false;
}

bool character::SeeWholeMap()
{
  game::SeeWholeMap();
  return false;
}

bool character::WalkThroughWalls()
{
  game::GoThroughWalls();
  return false;
}

bool character::ShowKeyLayout()
{
  felist List("Keyboard Layout");

  List.AddDescription("");
  List.AddDescription("Key       Description");

  for(ushort c = 1; game::GetCommand(c); ++c)
    if(!game::GetCommand(c)->IsWizardModeFunction())
      {
	std::string Buffer;
	Buffer += game::GetCommand(c)->GetKey();
	Buffer.resize(10, ' ');
	List.AddEntry(Buffer + game::GetCommand(c)->GetDescription(), LIGHT_GRAY);
      }

  if(game::WizardModeActivated())
    {
      List.AddEntry("", WHITE);
      List.AddEntry("Wizard mode functions:", WHITE);
      List.AddEntry("", WHITE);

      for(ushort c = 1; game::GetCommand(c); ++c)
	if(game::GetCommand(c)->IsWizardModeFunction())
	  {
	    std::string Buffer;
	    Buffer += game::GetCommand(c)->GetKey();
	    Buffer.resize(10, ' ');
	    List.AddEntry(Buffer + game::GetCommand(c)->GetDescription(), LIGHT_GRAY);
	  }
    }

  game::SetStandardListAttributes(List);
  List.Draw();
  return false;
}

bool character::Look()
{
  std::string Msg;

  if(!game::IsInWilderness())
    Msg = "Direction keys move cursor, ESC exits, 'i' examines items, 'c' examines a character.";
  else
    Msg = "Direction keys move cursor, ESC exits, 'c' examines a character.";

  game::PositionQuestion(Msg, GetPos(), &game::LookHandler, &game::LookKeyHandler, configuration::GetLookZoom());
  EditExperience(PERCEPTION, 1);
  EditAP(-100);
  return true;
}

bool character::Engrave(const std::string& What)
{
  GetNearLSquare(GetPos())->Engrave(What);
  return true;
}

bool character::WhatToEngrave()
{
  if(!CanRead())
    {
      ADD_MESSAGE("You can't even read.");
      return false;
    }

  GetNearLSquare(GetPos())->Engrave(game::StringQuestion("What do you want to engrave here?", vector2d(16, 6), WHITE, 0, 80, true));
  return false;
}

bool character::MoveRandomly()
{
  for(ushort c = 0; c < 10; ++c)
    {
      vector2d ToTry = GetPos() + game::GetMoveVector(RAND() % 8);

      if(GetLevelUnder()->IsValidPos(ToTry) && !GetNearLSquare(ToTry)->IsDangerousForAIToStepOn(this) && TryMove(ToTry, false))
	return true;
    }

  return false;
}

bool character::TestForPickup(item* ToBeTested) const
{
  if(MakesBurdened(ToBeTested->GetWeight() + GetCarriedWeight()))
    return false;

  return true;
}

bool character::OpenPos(vector2d APos)
{
  return GetNearLSquare(APos)->Open(this);
}

bool character::ClosePos(vector2d APos)
{
  return GetNearLSquare(APos)->Close(this);
}

bool character::Pray()
{
  felist Panthenon("To Whom shall thee address thine prayers?");
  std::vector<uchar> KnownIndex;

  if(!GetLSquareUnder()->GetDivineMaster())
    {
      for(ushort c = 1; c < game::GetGods(); ++c)
	if(game::GetGod(c)->GetKnown())
	  {
	    bitmap Icon(igraph::GetSymbolGraphic(), c << 4, 0, 16, 16);
	    Panthenon.AddEntry(game::GetGod(c)->CompleteDescription(), LIGHT_GRAY, 20, &Icon);
	    KnownIndex.push_back(c);
	  }
    }
  else
    if(game::GetGod(GetLSquareUnder()->GetDivineMaster())->GetKnown())
      {
	bitmap Icon(igraph::GetSymbolGraphic(), GetLSquareUnder()->GetDivineMaster() << 4, 0, 16, 16);
	Panthenon.AddEntry(game::GetGod(GetLSquareUnder()->GetDivineMaster())->CompleteDescription(), LIGHT_GRAY, 20, &Icon);
      }
    else
      ADD_MESSAGE("Somehow you feel that no deity you know can hear your prayers from this place.");

  game::SetStandardListAttributes(Panthenon);
  Panthenon.AddFlags(SELECTABLE);
  ushort Select = Panthenon.Draw();

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

      EditAP(-1000);
      return true;
    }
}

bool character::Kick()
{
  if(!CheckKick())
    return false;

  if(GetBurdenState() == OVER_LOADED)
    {
      ADD_MESSAGE("You try to kick, but you collapse under your load.");
      EditAP(-100000 / APBonus(GetAttribute(AGILITY)));
      return true;
    }

  uchar Dir = game::DirectionQuestion("In what direction do you wish to kick? [press a direction key]", false);

  if(Dir == DIR_ERROR || !GetAreaUnder()->IsValidPos(GetPos() + game::GetMoveVector(Dir)))
    return false;

  lsquare* Square = GetNearLSquare(GetPos() + game::GetMoveVector(Dir));

  if(Square->GetCharacter() && GetRelation(Square->GetCharacter()) != HOSTILE)
    if(!game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
      return false;
    else
      Hostility(Square->GetCharacter());

  Kick(Square);
  return true;
}

bool character::Offer()
{
  if(!CheckOffer())
    return false;

  if(GetLSquareUnder()->GetOLTerrain()->AcceptsOffers())
    {
      if(!GetStack()->GetItems())
	{
	  ADD_MESSAGE("You have nothing to offer!");
	  return false;
	}

      item* Item = GetStack()->DrawContents(this, "What do you want to offer?");

      if(Item)
	{
	  if(game::GetGod(GetLSquareUnder()->GetDivineMaster())->ReceiveOffer(Item))
	    {
	      Item->RemoveFromSlot();
	      Item->SendToHell();
	      DexterityAction(5);
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
  long SkillScore = 0;

  for(ushort c = 0; c < GetAllowedWeaponSkillCategories(); ++c)
    SkillScore += GetCWeaponSkill(c)->GetHits();

  return (SkillScore >> 2) + (GetAttribute(ARM_STRENGTH) + GetAttribute(LEG_STRENGTH) + GetAttribute(DEXTERITY) + GetAttribute(AGILITY) + GetAttribute(ENDURANCE) + GetAttribute(PERCEPTION) + GetAttribute(INTELLIGENCE) + GetAttribute(WISDOM) + GetAttribute(CHARISMA) + GetAttribute(MANA)) * 40;
}

long character::GetScore() const
{
  return (GetPolymorphBackup() ? GetPolymorphBackup()->GetStatScore() : GetStatScore()) + GetMoney() / 5 + Stack->Score() + game::GodScore();
}

void character::AddScoreEntry(const std::string& Description, float Multiplier, bool AddEndLevel) const
{
  if(!game::WizardModeActivated())
    {
      highscore HScore;
      std::string Desc = game::GetPlayer()->GetAssignedName() + ", " + Description;

      if(AddEndLevel)
	Desc += " in " + (game::IsInWilderness() ? "the World map" : game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrent()));

      HScore.Add(long((GetScore() - game::GetBaseScore()) * Multiplier), Desc);
      HScore.Save();
    }
  else
    ADD_MESSAGE("Death message: %s.", Description.c_str());
}

bool character::CheckDeath(const std::string& Msg, bool ForceMsg)
{
  if(!IsEnabled())
    return true;

  if(IsDead())
    {
      if(IsPlayer())
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
      if(IsPlayer())
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
      uchar Answer = game::DirectionQuestion("In what direction do you wish to throw?  [press a direction key]", false);

      if(Answer == DIR_ERROR)
	return false;

      ThrowItem(Answer, Item);
      EditExperience(ARM_STRENGTH, 25);
      EditExperience(PERCEPTION, 25);
      EditNP(-50);
      DexterityAction(5);
      return true;
    }
  else
    return false;
}

bool character::ThrowItem(uchar Direction, item* ToBeThrown)
{
  if(Direction > 7)
    ABORT("Throw in TOO odd direction...");

  return ToBeThrown->Fly(this, Direction, GetAttribute(ARM_STRENGTH));
}

void character::HasBeenHitByItem(character* Thrower, item* Thingy, float Speed)
{
  ushort Damage = ushort(Thingy->GetWeaponStrength() * Thingy->GetWeight() * sqrt(Speed) / 5000000000.0f) + (RAND() % 5 ? 1 : 0);

  if(IsPlayer())
    ADD_MESSAGE("%s hits you.", Thingy->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s hits %s.", Thingy->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));

  ReceiveDamage(Thrower, Damage, PHYSICAL_DAMAGE, ALL);
  CheckDeath("killed by a flying " + Thingy->GetName(UNARTICLED));
}

bool character::DodgesFlyingItem(item*, float Speed)
{			// Formula requires a little bit of tweaking...
  if(!(RAND() % 10))
    return RAND() & 1;

  if(!GetAttribute(AGILITY) || RAND() % ulong(sqrt(Speed) * GetSize() / GetAttribute(AGILITY) * 10 + 1) > 40)
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

      game::SetIsInGetCommand(true);
      int Key = GET_KEY();
      game::SetIsInGetCommand(false);

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
	    if(game::IsInWilderness() && !game::GetCommand(c)->IsUsableInWilderness())
	      ADD_MESSAGE("This function cannot be used while in wilderness.");
	    else
	      if(!game::WizardModeActivated() && game::GetCommand(c)->IsWizardModeFunction())
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

void character::Vomit(ushort Amount)
{
  if(IsPlayer())
    ADD_MESSAGE("You vomit.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s vomits.", CHAR_NAME(DEFINITE));

  EditExperience(ARM_STRENGTH, -50);
  EditExperience(LEG_STRENGTH, -50);
  EditNP(-200 - RAND() % 201);

  if(!game::IsInWilderness())
    GetLSquareUnder()->ReceiveVomit(this, Amount);
}

bool character::Apply()
{
  if(!GetStack()->SortedItems(this, &item::AppliableSorter))
    {
      ADD_MESSAGE("You have nothing to apply!");
      return false;
    }

  item* Item = GetStack()->DrawContents(this, "What do you want to apply?", 0, &item::AppliableSorter);
  return Item && Item->Apply(this);
}

bool character::ForceVomit()
{
  ADD_MESSAGE("You push your fingers down to your throat and vomit.");
  Vomit(2 + RAND() % 3);
  EditAP(-1000);
  return true;
}

bool character::Zap()
{
  if(!CheckZap())
    return false;

  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to zap with.");
      return false;
    }

  item* Item = GetStack()->DrawContents(this, "What do you want to zap with?", 0, &item::ZappableSorter);

  if(Item)
    {
      uchar Answer = game::DirectionQuestion("In what direction do you wish to zap?  [press a direction key or '.']", false, true);

      if(Answer == DIR_ERROR)
	return false;

      if(Item->Zap(this, GetPos(), Answer))
	{
	  EditExperience(PERCEPTION, 50);
	  EditAP(-100000 / APBonus(GetAttribute(PERCEPTION)));
	  return true;
	}
      else
	return false;
    }
  else
    return false;
}

bool character::Polymorph(character* NewForm, ushort Counter)
{
  if(!IsPolymorphable() || (!IsPlayer() && game::IsInWilderness()))
    {
      delete NewForm;
      return false;
    }

  if(GetAction())
    GetAction()->Terminate(false);

  NewForm->SetAssignedName("");

  if(IsPlayer())
    ADD_MESSAGE("Your body glows in a crimson light. You transform into %s!", NewForm->CHAR_NAME(INDEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s glows in a crimson light and %s transforms into %s!", CHAR_NAME(DEFINITE), GetPersonalPronoun().c_str(), NewForm->CHAR_NAME(INDEFINITE));

  InNoMsgMode = NewForm->InNoMsgMode = true;
  GetSquareUnder()->RemoveCharacter();
  GetSquareUnder()->AddCharacter(NewForm);
  SetSquareUnder(0);
  NewForm->SetAssignedName(GetAssignedName());
  NewForm->ActivateTemporaryState(POLYMORPHED);
  NewForm->SetTemporaryStateCounter(POLYMORPHED, Counter);

  if(TemporaryStateIsActivated(POLYMORPHED))
    {
      NewForm->SetPolymorphBackup(GetPolymorphBackup());
      SetPolymorphBackup(0);
      SendToHell();
    }
  else
    {
      NewForm->SetPolymorphBackup(this);
      SetPolymorphed(true);
      SetHasBe(false);
    }

  GetStack()->MoveItemsTo(NewForm->GetStack());
  NewForm->SetMoney(GetMoney());

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
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
	    Item->MoveTo(IsPlayer() ? NewForm->GetStack() : NewForm->GetStackUnder());
	}
    }

  NewForm->ChangeTeam(GetTeam());

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(NewForm);

  InNoMsgMode = NewForm->InNoMsgMode = false;
  NewForm->CalculateAll();

  if(IsPlayer())
    {
      game::SetPlayer(NewForm);
      game::SendLOSUpdateRequest();
      UpdateESPLOS();
    }

  NewForm->TestWalkability();
  return true;
}

void character::BeKicked(character* Kicker, item* Boot, float KickDamage, float ToHitValue, short Success, bool Critical)
{
  switch(TakeHit(Kicker, Boot, KickDamage, ToHitValue, Success, KICK_ATTACK, Critical))
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case DID_NO_DAMAGE:
      if(!CheckBalance(KickDamage))
	{
	  if(IsPlayer())
	    ADD_MESSAGE("The kick throws you off balance.");
	  else if(Kicker->IsPlayer())
	    ADD_MESSAGE("The kick throws %s off balance.", CHAR_DESCRIPTION(DEFINITE));

	  if(GetAreaUnder()->IsValidPos((GetPos() << 1) - Kicker->GetPos()))
	    FallTo(Kicker, (GetPos() << 1) - Kicker->GetPos());
	}
    }
}
/* return true if still in balance */
bool character::CheckBalance(float KickDamage)
{
  if(KickDamage == 0)
    return true;
  return KickDamage * 25 < RAND() % GetSize();
}

void character::FallTo(character* GuiltyGuy, vector2d Where)
{
  EditAP(-500);

  if(GetNearLSquare(Where)->GetOLTerrain()->IsWalkable() && !GetNearSquare(Where)->GetCharacter())
    Move(Where, true);

  if(!GetNearLSquare(Where)->GetOLTerrain()->IsWalkable())
    {
      if(IsPlayer()) 
	ADD_MESSAGE("You hit your head on the wall.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s hits %s head on the wall.", CHAR_NAME(DEFINITE), GetPossessivePronoun().c_str());

      ReceiveDamage(GuiltyGuy, 1 + RAND() % 5, PHYSICAL_DAMAGE, HEAD);
      CheckDeath("killed by hitting a wall");
    }

  // Place code that handles characters bouncing to each other here
}

bool character::CheckCannibalism(const material* What) const
{ 
  return GetTorso()->GetConsumeMaterial()->IsSameAs(What); 
}

void character::StandIdleAI()
{
  SeekLeader();

  if(CheckForEnemies(true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(CheckForDoors())
    return;

  EditAP(-1000);
}

void character::Faint()
{
  if(GetAction())
    {
      if(!GetAction()->AllowFaint())
	return;
      else
	GetAction()->Terminate(false);
    }

  if(IsPlayer())
    ADD_MESSAGE("You faint.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s faints.", CHAR_NAME(DEFINITE));

  faint* Faint = new faint(this);
  Faint->SetCounter(100 + RAND() % 101);
  SetAction(Faint);
}

void character::DeActivateVoluntaryAction(const std::string& Reason)
{
  if(GetAction() && GetAction()->IsVoluntary())
    {
      if(IsPlayer())
	{
	  if(Reason.length())
	    ADD_MESSAGE("%s", Reason.c_str());

	  if(game::BoolQuestion("Continue " + GetAction()->GetDescription() + "? [y/N]"))
	    GetAction()->SetInDNDMode(true);
	  else
	    GetAction()->Terminate(false);
	}
      else
	GetAction()->Terminate(false);
    }
}

void character::ActionAutoTermination()
{
  if(!GetAction() || !GetAction()->IsVoluntary() || GetAction()->InDNDMode())
    return;

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled() && (*i)->CanBeSeenBy(this))
	  {
	    if(IsPlayer())
	      {
		ADD_MESSAGE("%s seems to be hostile.", (*i)->CHAR_NAME(DEFINITE));

		if(game::BoolQuestion("Continue " + GetAction()->GetDescription() + "? [y/N]"))
		  GetAction()->SetInDNDMode(true);
		else
		  GetAction()->Terminate(false);
	      }
	    else
	      GetAction()->Terminate(false);

	    return;
	  }
}

bool character::CheckForEnemies(bool CheckDoors, bool CheckGround)
{
  bool HostileCharsNear = false;
  character* NearestChar = 0;
  ulong NearestDistance = 0xFFFFFFFF;

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
	  {
	    ulong ThisDistance = HypotSquare(long((*i)->GetPos().X) - GetPos().X, long((*i)->GetPos().Y) - GetPos().Y);

	    if(ThisDistance <= LOSRangeSquare())
	      HostileCharsNear = true;

	    if((ThisDistance < NearestDistance || (ThisDistance == NearestDistance && !(RAND() % 3))) && (*i)->CanBeSeenBy(this))
	      {
		NearestChar = *i;
		NearestDistance = ThisDistance;
	      }
	  }

  if(NearestChar)
    {
      if(SpecialEnemySightedReaction(NearestChar))
	return true;

      if(StateIsActivated(PANIC))
	{
	  if(!MoveTowards((GetPos() << 1) - NearestChar->GetPos()))
	    return MoveRandomly();

	  return true;
	}

      if(!GetTeam()->GetLeader() && NearestChar->IsPlayer())
	WayPoint = NearestChar->GetPos();

      return MoveTowards(NearestChar->GetPos());
    }
  else
    {
      if(!GetTeam()->GetLeader() && WayPoint.X != -1)
	{
	  if(!MoveTowards(WayPoint))
	    {
	      WayPoint.X = -1;
	      return false;
	    }
	  else
	    return true;
	}
      else
	{
	  if((!GetTeam()->GetLeader() || (GetTeam()->GetLeader() && WayPoint.X == -1)) && HostileCharsNear)
	    {
	      if(CheckDoors && CheckForDoors())
		return true;

	      if(CheckGround && CheckForUsefulItemsOnGround())
		return true;

	      return MoveRandomly(); // one has heard that an enemy is near but doesn't know where
	    }
	  else
	    return false;
	}
    }
}

bool character::CheckForDoors()
{
  if(!CanOpen())
    return false;

  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square && Square->GetOLTerrain()->CanBeOpenedByAI() && OpenPos(Square->GetPos()))
	return true;
    }

  return false;
}

bool character::CheckForUsefulItemsOnGround()
{
  itemvector ItemVector;
  GetStackUnder()->FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->CanBeSeenBy(this) && ItemVector[c]->IsPickable(this))
      {
	if(TryToEquip(ItemVector[c]))
	  return true;

	if(TryToConsume(ItemVector[c]))
	  return true;
      }

  return false;
}

bool character::FollowLeader()
{
  if(!GetTeam()->GetLeader())
    return false;

  if(GetTeam()->GetLeader()->CanBeSeenBy(this))
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
  if(GetTeam()->GetLeader() && GetTeam()->GetLeader()->CanBeSeenBy(this))
    WayPoint = GetTeam()->GetLeader()->GetPos();
}

bool character::RestUntilHealed()
{
  if(GetHP() == GetMaxHP())
    {
      ADD_MESSAGE("You HP is already at its maximum.");
      return false;
    }

  if(!CanHeal())
    {
      ADD_MESSAGE("You cannot heal.");
      return false;
    }

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
  configuration::Save();
  GetAreaUnder()->SendNewDrawRequest();
  return false;
}

bool character::OutlineItems()
{
  configuration::SetOutlineItems(!configuration::GetOutlineItems());
  configuration::Save();
  GetAreaUnder()->SendNewDrawRequest();
  return false;
}

ushort character::GetMoveEase() const
{
  switch(GetBurdenState())
    {
    case BURDENED:
      return 75;
    case STRESSED:
    case OVER_LOADED:
      return 50;
    default:
      return 100;
    }
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
  if(!game::IsInWilderness())
    return GetAttribute(PERCEPTION) * GetLevelUnder()->GetLOSModifier() / 48;
  else
    return 3;
}

ushort character::LOSRangeSquare() const
{
  if(!game::IsInWilderness())
    {
      ulong LOSModifier = GetLevelUnder()->GetLOSModifier();
      return GetAttribute(PERCEPTION) * GetAttribute(PERCEPTION) * LOSModifier * LOSModifier / 2304;
    }
  else
    return 9;
}

ushort character::ESPRange() const
{
  return GetAttribute(INTELLIGENCE) / 3;
}

ushort character::ESPRangeSquare() const
{
  return GetAttribute(INTELLIGENCE) * GetAttribute(INTELLIGENCE) / 9;
}

bool character::GainDivineKnowledge()
{
  for(ushort c = 1; c < game::GetGods(); ++c)
    game::GetGod(c)->SetKnown(true);

  return false;
}

bool character::Displace(character* Who)
{
  if(GetBurdenState() == OVER_LOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You try to use your claws to crawl forward. But your load is too heavy.");

      return true;
    }

  if(GetRelativeDanger(Who) > 1.0f && Who->CanBeDisplaced() && !IsStuck() && !Who->IsStuck() && (!Who->GetAction() || Who->GetAction()->AllowDisplace()) && GetSquareUnder()->IsWalkable(Who))
    {
      if(IsPlayer())
	ADD_MESSAGE("You displace %s!", Who->CHAR_DESCRIPTION(DEFINITE));
      else if(Who->IsPlayer())
	ADD_MESSAGE("%s displaces you!", CHAR_DESCRIPTION(DEFINITE));
      else if(CanBeSeenByPlayer() || Who->CanBeSeenByPlayer())
	ADD_MESSAGE("%s displaces %s!", CHAR_DESCRIPTION(DEFINITE), Who->CHAR_DESCRIPTION(DEFINITE));

      GetLSquareUnder()->SwapCharacter(Who->GetLSquareUnder());

      if(IsPlayer())
	ShowNewPosInfo();

      if(Who->IsPlayer())
	Who->ShowNewPosInfo();

      EditAP(-GetMoveAPRequirement(GetSquareUnder()->GetEntryDifficulty()) - 500);
      EditNP(-10 * GetSquareUnder()->GetEntryDifficulty());
      EditExperience(AGILITY, 5 * GetSquareUnder()->GetEntryDifficulty());
      return true;
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("%s resists!", Who->CHAR_DESCRIPTION(DEFINITE));

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

  if(IsPlayer())
    {
      if(GetHungerState() == STARVING && OldGetHungerState != STARVING)
	{
	  game::Beep();
	  ADD_MESSAGE("You are getting very hungry.");
	  DeActivateVoluntaryAction();
	}
      else if(GetHungerState() == HUNGRY && OldGetHungerState != HUNGRY && OldGetHungerState != STARVING)
	{
	  ADD_MESSAGE("You are getting hungry.");
	  DeActivateVoluntaryAction();
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
  UpdateESPLOS();

  if(!game::IsInWilderness())
    {
      if(GetLSquareUnder()->IsDark() && !game::GetSeeWholeMapCheat())
	ADD_MESSAGE("It's dark in here!");

      ushort VisibleItemsOnGround = GetStackUnder()->GetVisibleItems(this);

      if(VisibleItemsOnGround > 0)
	{
	  if(VisibleItemsOnGround > 1)
	    ADD_MESSAGE("Several items are lying here.");
	  else
	    ADD_MESSAGE("%s is lying here.", GetStackUnder()->GetBottomVisibleItem(this)->CHAR_NAME(INDEFINITE));
	}
		
      if(GetNearLSquare(GetPos())->GetEngraved().length())
	ADD_MESSAGE("Something has been engraved here: \"%s\"", GetNearLSquare(GetPos())->GetEngraved().c_str());
    }
}

void character::Hostility(character* Enemy)
{
  if(Enemy == this)
    return;

  if(GetTeam() != Enemy->GetTeam())
    GetTeam()->Hostility(Enemy->GetTeam());
  else if(Enemy->IsEnabled())
    {
      if(Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("%s becomes enraged.", Enemy->CHAR_NAME(DEFINITE));

      Enemy->ChangeTeam(game::GetTeam(MONSTER_TEAM));
    }
}

stack* character::GetGiftStack() const
{
  if(GetLSquareUnder()->GetRoom() && !GetLSquareUnder()->GetRoomClass()->AllowDropGifts())
    return GetStack();
  else
    return GetStackUnder();
}

bool character::MoveRandomlyInRoom()
{
  for(ushort c = 0; c < 10; ++c)
    {
      vector2d ToTry = GetPos() + game::GetMoveVector(RAND() % 8);

      if(GetLevelUnder()->IsValidPos(ToTry) && !GetNearLSquare(ToTry)->IsDangerousForAIToStepOn(this) && !GetNearLSquare(ToTry)->GetOLTerrain()->IsDoor() && TryMove(ToTry, false))
	return true;
    }

  return false;
}

bool character::Go()
{
  uchar Dir = game::DirectionQuestion("In what direction do you want to go? [press a direction key]", false);

  if(Dir == DIR_ERROR)
    return false;

  go* Go = new go(this);
  Go->SetDirection(Dir);
  uchar OKDirectionsCounter = 0;

  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square && Square->IsWalkable(this))
	++OKDirectionsCounter;
    }

  Go->SetWalkingInOpen(OKDirectionsCounter > 2);
  SetAction(Go);
  Go->Handle();
  return GetAction() != 0;
}

void character::GoOn(go* Go)
{
  if(!GetAreaUnder()->IsValidPos(GetPos() + game::GetMoveVector(Go->GetDirection())))
    {
      Go->Terminate(false);
      return;
    }

  lsquare* MoveToSquare = GetNearLSquare(GetPos() + game::GetMoveVector(Go->GetDirection()));

  if(!MoveToSquare->IsWalkable(this) || (MoveToSquare->GetCharacter() && GetTeam() != MoveToSquare->GetCharacter()->GetTeam()))
    {
      Go->Terminate(false);
      return;
    }

  uchar OKDirectionsCounter = 0;

  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square && Square->IsWalkable(this))
	++OKDirectionsCounter;
    }

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

  if(!TryMove(MoveToSquare->GetPos()) || BeginSquare == GetSquareUnder() || GetLSquareUnder()->IsDark() || GetStackUnder()->GetVisibleItems(this))
    {
      Go->Terminate(false);
      return;
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

  if(GetSquareUnder())
    GetSquareUnder()->SendNewDrawRequest();

  SetTeamIterator(GetTeam()->Add(this));
}

void character::ChangeTeam(team* What)
{
  if(GetTeam())
    GetTeam()->Remove(GetTeamIterator());

  Team = What;
  GetSquareUnder()->SendNewDrawRequest();

  if(GetTeam())
    SetTeamIterator(GetTeam()->Add(this));
}

bool character::ChangeRandomStat(short HowMuch)
{
  for(ushort c = 0; c < 50; ++c)
    {
      ushort AttribID = RAND() % ATTRIBUTES;

      if(EditAttribute(AttribID, HowMuch))
	return true;
    }

  return false;
}

ushort character::RandomizeReply(std::vector<bool>& Said)
{
  bool NotSaid = false;

  for(ushort c = 0; c < Said.size(); ++c)
    if(!Said[c])
      {
	NotSaid = true;
	break;
      }

  if(!NotSaid)
    for(ushort c = 0; c < Said.size(); ++c)
      Said[c] = false;

  uchar ToSay;
  while(Said[ToSay = RAND() % Said.size()]);
  Said[ToSay] = true;
  return ToSay;
}

void character::DisplayInfo(std::string& Msg)
{
  if(IsPlayer())
    Msg << " You are " << GetStandVerb() << " here.";
  else
    {
      Msg << " " << festring::CapitalizeCopy(GetName(INDEFINITE)) << " is " << GetStandVerb() << " here. " << festring::CapitalizeCopy(GetPersonalPronoun());

      /*if(GetTeam() == game::GetPlayer()->GetTeam())
	Msg << " is at danger level " << DangerLevel() << " and";*/

      if(GetTeam() == game::GetPlayer()->GetTeam())
	Msg << " is tame.";
      else
	{
	  uchar Relation = GetRelation(game::GetPlayer());

	  if(Relation == HOSTILE)
	    Msg << " is hostile.";
	  else if(Relation == UNCARING)
	    Msg << " does not care about you.";
	  else
	    Msg << " is friendly.";
	}
    }
}

void character::TestWalkability()
{
  if(GetSquareUnder()->IsFatalToStay() && !GetSquareUnder()->IsWalkable(this))
    {
      bool Alive = false;

      for(ushort d = 0; d < 8; ++d)
	{
	  square* Square = GetNeighbourSquare(d);

	  if(Square && Square->IsWalkable(this) && !Square->GetCharacter())
	    {
	      if(IsPlayer())
		ADD_MESSAGE("%s.", GetSquareUnder()->SurviveMessage(this).c_str());
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s.", GetSquareUnder()->MonsterSurviveMessage(this).c_str());

	      Move(Square->GetPos(), true); // actually, this shouldn't be a teleport move
	      Alive = true;
	      break;
	    }
	}

      if(!Alive)
	{
	  if(IsPlayer())
	    {
	      GetSquareUnder()->RemoveCharacter();
	      SendToHell();
	      game::AskForKeyPress(GetSquareUnder()->DeathMessage(this) + ".");
	      game::GetPlayer()->AddScoreEntry(GetSquareUnder()->ScoreEntry(this));
	      game::End();
	    }
	  else
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), GetSquareUnder()->MonsterDeathVerb(this).c_str());

	      Die();
	    }
	}
    }
}

ushort character::GetSize() const
{
  if(GetTorso())
    return GetTorso()->GetSize();
  else
    return 0;
}

void character::SetMainMaterial(material* NewMaterial, ushort SpecialFlags)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->SetMainMaterial(NewMaterial, SpecialFlags);

  for(ushort c = 1; c < GetBodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetMainMaterial()->GetVolume());
      GetBodyPart(c)->SetMainMaterial(NewMaterial, SpecialFlags);
    }
}

void character::ChangeMainMaterial(material* NewMaterial, ushort SpecialFlags)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->ChangeMainMaterial(NewMaterial, SpecialFlags);

  for(ushort c = 1; c < GetBodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetMainMaterial()->GetVolume());
      GetBodyPart(c)->ChangeMainMaterial(NewMaterial, SpecialFlags);
    }
}

void character::SetSecondaryMaterial(material*, ushort)
{
  ABORT("Illegal character::SetSecondaryMaterial call!");
}

void character::ChangeSecondaryMaterial(material*, ushort)
{
  ABORT("Illegal character::ChangeSecondaryMaterial call!");
}

void character::SetContainedMaterial(material*, ushort)
{
  ABORT("Illegal character::SetContainedMaterial call!");
}

void character::ChangeContainedMaterial(material*, ushort)
{
  ABORT("Illegal character::ChangeContainedMaterial call!");
}

void character::TeleportRandomly()
{
  StuckTo = 0;
  SetStuckToBodyPart(NONE_INDEX);

  if(StateIsActivated(TELEPORT_CONTROL) && IsPlayer())
    {
      while(true)
	{
	  vector2d PlayersInput = game::PositionQuestion("Where do you wish to teleport? [direction keys]", GetPos(), 0,0,false);

	  if(GetNearLSquare(PlayersInput)->IsWalkable(this) || game::GetGoThroughWallsCheat())
	    {
	      if(GetNearLSquare(PlayersInput)->GetCharacter())
		{
		  ADD_MESSAGE("You feel that something wierd has happened, but can't really tell what exactly.");
		  break;
		  /* break this loop and teleport randomly */
		}

	      Move(PlayersInput, true);
	      return;
	    }
	}
    }

  Move(GetLevelUnder()->GetRandomSquare(this), true);
}

bool character::SecretKnowledge()
{
  felist List("Knowledge of the ancients");
  List.AddEntry("Character attributes", LIGHT_GRAY);
  List.AddEntry("Character attack info", LIGHT_GRAY);
  List.AddEntry("Character defence info", LIGHT_GRAY);
  List.AddEntry("Character danger values", LIGHT_GRAY);
  List.AddEntry("Item attack info", LIGHT_GRAY);
  List.AddEntry("Miscellaneous item info", LIGHT_GRAY);
  game::SetStandardListAttributes(List);
  List.AddFlags(SELECTABLE);
  ushort Chosen = List.Draw();
  ushort c, PageLength = 20;

  if(Chosen & 0x8000)
    return false;

  List.Empty();

  if(Chosen < 4)
    {
      std::vector<character*> Character;
      protosystem::CreateEveryCharacter(Character);
      bitmap Pic(16, 16);

      switch(Chosen)
	{
	case 0:
	  List.AddDescription("                                                AS LS DX AG EN PE IN WI CH MA");

	  for(c = 0; c < Character.size(); ++c)
	    {
	      std::string Entry;
	      Character[c]->AddName(Entry, UNARTICLED);
	      Pic.Fill(TRANSPARENT_COLOR);
	      Character[c]->DrawBodyParts(&Pic, vector2d(0, 0), MakeRGB24(128, 128, 128), false, false);
	      Character[c]->AddAttributeInfo(Entry);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, &Pic);
	    }

	  PageLength = 15;
	  break;
	case 1:
	  List.AddDescription("                                                  BD        THV       APC");

	  for(c = 0; c < Character.size(); ++c)
	    {
	      std::string Entry;
	      Character[c]->AddName(Entry, UNARTICLED);
	      Pic.Fill(TRANSPARENT_COLOR);
	      Character[c]->DrawBodyParts(&Pic, vector2d(0, 0), MakeRGB24(128, 128, 128), false, false);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, &Pic);
	      Character[c]->AddAttackInfo(List);
	    }

	  PageLength = 20;
	  break;
	case 2:
	  List.AddDescription("                                                  DV/BV     HP        AV/BS");

	  for(c = 0; c < Character.size(); ++c)
	    {
	      std::string Entry;
	      Character[c]->AddName(Entry, UNARTICLED);
	      Entry.resize(47, ' ');
	      Entry << int(Character[c]->GetDodgeValue());
	      Entry.resize(57, ' ');
	      Entry << Character[c]->GetMaxHP();
	      Pic.Fill(TRANSPARENT_COLOR);
	      Character[c]->DrawBodyParts(&Pic, vector2d(0, 0), MakeRGB24(128, 128, 128), false, false);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, &Pic);
	      Character[c]->AddDefenceInfo(List);
	    }

	  PageLength = 25;
	  break;
	case 3:
	  List.AddDescription("                                                  Danger    GModifier");

	  for(c = 0; c < Character.size(); ++c)
	    {
	      std::string Entry;
	      Character[c]->AddName(Entry, UNARTICLED);
	      Entry.resize(47, ' ');
	      Entry << int(Character[c]->GetRelativeDanger(this, true) * 100);
	      Entry.resize(57, ' ');

	      if(Character[c]->CanBeGenerated())
		Entry << int(Character[c]->GetDangerModifier() * 100);
	      else
		Entry << '-';

	      Pic.Fill(TRANSPARENT_COLOR);
	      Character[c]->DrawBodyParts(&Pic, vector2d(0, 0), MakeRGB24(128, 128, 128), false, false);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, &Pic);
	    }

	  PageLength = 15;
	  break;
	}

      for(c = 0; c < Character.size(); ++c)
	delete Character[c];
    }
  else if(Chosen < 6)
    {
      std::vector<item*> Item;
      protosystem::CreateEveryItem(Item);
      PageLength = 20;

      switch(Chosen)
	{
	case 4:
	  List.AddDescription("                                        Weight    Size      SR        DAM");

	  for(c = 0; c < Item.size(); ++c)
	    {
	      std::string Entry;
	      Item[c]->AddName(Entry, UNARTICLED);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, Item[c]->GetPicture());
	      Item[c]->AddAttackInfo(List);
	    }

	  break;
	case 5:
	  List.AddDescription("                                        \177              OV             NV");

	  for(c = 0; c < Item.size(); ++c)
	    {
	      std::string Entry;
	      Item[c]->AddName(Entry, UNARTICLED);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, Item[c]->GetPicture());
	      Item[c]->AddMiscellaneousInfo(List);
	    }

	  break;
	}

      for(c = 0; c < Item.size(); ++c)
	delete Item[c];
    }

  game::SetStandardListAttributes(List);
  List.SetPageLength(PageLength);
  List.Draw();
  List.PrintToFile(HOME_DIR + "secret" + Chosen + ".txt");
  ADD_MESSAGE("Info written also to %ssecret%d.txt.", HOME_DIR.c_str(), Chosen);
  return false;
}

void character::RestoreHP()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->RestoreHP();

  HP = MaxHP;
}

bool character::AllowDamageTypeBloodSpill(uchar Type) const
{
  switch(Type)
    {
    case PHYSICAL_DAMAGE:
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

/* Returns truly done damage */

ushort character::ReceiveBodyPartDamage(character* Damager, ushort Damage, uchar Type, uchar BodyPartIndex, uchar Direction, bool PenetrateResistance, bool Critical, bool ShowNoDamageMsg)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);
  BodyPart->DamageArmor(Damager, Damage, Type);

  if(!PenetrateResistance)
    Damage -= (BodyPart->GetTotalResistance(Type) >> 1) + RAND() % ((BodyPart->GetTotalResistance(Type) >> 1) + 1);

  if(short(Damage) < 1)
    if(Critical)
      Damage = 1;
    else
      {
	if(ShowNoDamageMsg)
	  {
	    if(IsPlayer())
	      ADD_MESSAGE("You are not hurt.");
	    else if(CanBeSeenByPlayer())
	      ADD_MESSAGE("%s is not hurt.", GetPersonalPronoun().c_str());
	  }

	return 0;
      }

  if(Critical && AllowDamageTypeBloodSpill(Type) && !game::IsInWilderness())
    {
      BodyPart->SpillBlood(3 + RAND() % 3);

      for(ushort d = 0; d < 8; ++d)
	{
	  lsquare* Square = GetNeighbourLSquare(d);

	  if(Square && Square->GetOLTerrain()->IsWalkable())
	    BodyPart->SpillBlood(2 + (RAND() & 1), Square->GetPos());
	}
    }

  if(BodyPart->ReceiveDamage(Damager, Damage, Type) && BodyPartCanBeSevered(BodyPartIndex))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your %s is severed off!", BodyPart->GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s %s is severed off!", GetPossessivePronoun().c_str(), BodyPart->GetBodyPartName().c_str());

      item* Severed = SevereBodyPart(BodyPartIndex);
      GetSquareUnder()->SendNewDrawRequest();

      if(!game::IsInWilderness())
	{
	  GetStackUnder()->AddItem(Severed);

	  if(Direction != YOURSELF)
	    Severed->Fly(0, Direction, Damage);
	}
      else
	  GetStack()->AddItem(Severed);

      Severed->DropEquipment();

      if(IsPlayer())
	game::AskForKeyPress("Bodypart severed! [press any key to continue]");
      else if(RAND() % 100 < GetPanicLevel() && !IsDead())
	BeginTemporaryState(PANIC, 1000 + RAND() % 1001);
    }

  if(!IsPlayer() && !IsDead())
    CheckPanic(500);

  return Damage;
}

item* character::SevereBodyPart(ushort BodyPartIndex)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);
  BodyPart->SetOwnerDescription("of " + GetName(INDEFINITE));
  BodyPart->SetUnique(GetArticleMode() != NORMAL_ARTICLE || AssignedName.length());
  BodyPart->RemoveFromSlot();
  BodyPart->RandomizePosition();
  CalculateAttributeBonuses();
  CalculateBattleInfo();

  if(StuckToBodyPart == BodyPartIndex)
    {
      StuckToBodyPart = NONE_INDEX;
      StuckTo = 0;
    }

  return BodyPart;
}

/* The second uchar is actually TargetFlags, which is not used here, but seems to be used in humanoid::ReceiveDamage. Returns true if the character really receives damage */

bool character::ReceiveDamage(character* Damager, ushort Damage, uchar Type, uchar, uchar Direction, bool, bool PenetrateArmor, bool Critical, bool ShowMsg)
{
  bool Affected = ReceiveBodyPartDamage(Damager, Damage, Type, 0, Direction, PenetrateArmor, Critical, ShowMsg) != 0;

  if(DamageTypeAffectsInventory(Type))
    {
      for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	if(GetEquipment(c))
	  GetEquipment(c)->ReceiveDamage(Damager, Damage, Type);

      GetStack()->ReceiveDamage(Damager, Damage, Type);
    }

  return Affected;
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
  game::PositionQuestion("What do you want to name? [press 'n' over a tame creature or ESC to exit]", GetPos(), 0, &game::NameKeyHandler);
  return false;
}

std::string character::GetDescription(uchar Case) const
{
  if(IsPlayer())
    return "you";
  else if(CanBeSeenByPlayer())
    return GetName(Case);
  else
    return "something";
}

std::string character::GetPersonalPronoun() const
{
  if(IsPlayer())
    return "you";
  else if(GetSex() == UNDEFINED || (!CanBeSeenByPlayer() && !game::GetSeeWholeMapCheat()))
    return "it";
  else if(GetSex() == MALE)
    return "he";
  else
    return "she";
}

std::string character::GetPossessivePronoun() const
{
  if(IsPlayer())
    return "your";
  else if(GetSex() == UNDEFINED || (!CanBeSeenByPlayer() && !game::GetSeeWholeMapCheat()))
    return "its";
  else if(GetSex() == MALE)
    return "his";
  else
    return "her";
}

std::string character::GetObjectPronoun() const
{
  if(IsPlayer())
    return "you";
  else if(GetSex() == UNDEFINED || (!CanBeSeenByPlayer() && !game::GetSeeWholeMapCheat()))
    return "it";
  else if(GetSex() == MALE)
    return "him";
  else
    return "her";
}

bool character::AddMaterialDescription(std::string& String, bool Articled) const
{
  GetTorso()->GetMainMaterial()->AddName(String, Articled);
  String << " ";
  return true;
}

void character::AddName(std::string& String, uchar Case) const
{
  if(!(Case & PLURAL) && AssignedName.length())
    {
      if(!ShowClassDescription())
	String << AssignedName;
      else
	{
	  String << AssignedName << " ";
	  id::AddName(String, (Case|ARTICLE_BIT)&~INDEFINE_BIT);
	}
    }
  else
    id::AddName(String, Case);
}

uchar character::GetHungerState() const
{
  if(GetNP() > BLOATED_LEVEL)
    return BLOATED;
  else if(GetNP() > SATIATED_LEVEL)
    return SATIATED;
  else if(GetNP() > NOT_HUNGER_LEVEL)
    return NOT_HUNGRY;
  else if(GetNP() > HUNGER_LEVEL)
    return HUNGRY;
  else
    return STARVING;
}

bool character::EquipmentScreen()
{
  if(!CanUseEquipment())
    {
      ADD_MESSAGE("You cannot use equipment.");
      return false;
    }

  ushort Chosen = 0;
  bool EquipmentChanged = false;
  felist List("Equipment menu");

  while(true)
    {
      List.Empty();

      for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	{
	  std::string Entry = EquipmentName(c) + ":";
	  Entry.resize(20, ' ');

	  if(GetEquipment(c))
	    {
	      GetEquipment(c)->AddInventoryEntry(this, Entry, 1, true);
	      AddSpecialEquipmentInfo(Entry, c);
	      List.AddEntry(Entry, LIGHT_GRAY, 20, GetEquipment(c)->GetPicture());
	    }
	  else
	    {
	      Entry += GetBodyPartOfEquipment(c) ? "-" : "can't use";
	      bitmap Black(16, 16, TRANSPARENT_COLOR);
	      List.AddEntry(Entry, LIGHT_GRAY, 20, &Black);
	    }
	}

      game::DrawEverythingNoBlit();
      game::SetStandardListAttributes(List);
      List.SetFlags(SELECTABLE|DRAW_BACKGROUND_AFTERWARDS);
      Chosen = List.Draw();

      if(Chosen >= GetEquipmentSlots())
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
	  game::DrawEverythingNoBlit();
	  item* Item = GetStack()->DrawContents(this, "Choose " + EquipmentName(Chosen) + ":", 0, EquipmentSorter(Chosen));

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

void character::SetBodyPart(ushort Index, bodypart* What)
{
  BodyPartSlot[Index].PutInItem(What);

  if(What)
    AddOriginalBodyPartID(Index, What->GetID());
}

bool character::CanConsume(material* Material) const
{
  return (GetConsumeFlags() & Material->GetConsumeType()) != 0;
}

void character::SetTemporaryStateCounter(ushort State, ushort What)
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      TemporaryStateCounter[c] = What;
}

void character::EditTemporaryStateCounter(ushort State, short What)
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      TemporaryStateCounter[c] += What;
}

ushort character::GetTemporaryStateCounter(ushort State) const
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      return TemporaryStateCounter[c];

  ABORT("Illegal GetTemporaryStateCounter request!");
  return 0;
}

bool character::CheckKick() const
{
  if(!CanKick())
    {
      ADD_MESSAGE("This race can't kick.");
      return false;
    }
  else
    return true;
}

ushort character::GetResistance(uchar Type) const
{
  switch(Type)
    {
    case PHYSICAL_DAMAGE: return GetPhysicalDamageResistance();
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

void character::Regenerate()
{
  if(HP == MaxHP)
    return;

  ulong RegenerationBonus = 0;
  bool NoHealableBodyParts = true;

  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->IsAlive())
	{
	  RegenerationBonus += BodyPart->GetMaxHP();

	  if(NoHealableBodyParts && BodyPart->GetHP() < BodyPart->GetMaxHP())
	    NoHealableBodyParts = false;
	}
    }

  if(!RegenerationBonus || NoHealableBodyParts)
    return;

  RegenerationBonus *= (50 + GetAttribute(ENDURANCE));

  if(Action && Action->GetRestRegenerationBonus())
    RegenerationBonus *= GetSquareUnder()->GetRestModifier();

  RegenerationCounter += RegenerationBonus;

  while(RegenerationCounter > 600000)
    {
      uchar NeedHeal = 0, NeedHealIndex[MAX_BODYPARTS];

      for(ushort c = 0; c < BodyParts; ++c)
	{
	  bodypart* BodyPart = GetBodyPart(c);

	  if(BodyPart && BodyPart->IsAlive() && BodyPart->GetHP() < BodyPart->GetMaxHP())
	    NeedHealIndex[NeedHeal++] = c;
	}

      if(!NeedHeal)
	break;

      RegenerationCounter -= 600000;
      GetBodyPart(NeedHealIndex[RAND() % NeedHeal])->IncreaseHP();
      EditExperience(ENDURANCE, Max(10000 / MaxHP, 1));
      ++HP;
    }
}

void character::PrintInfo() const
{
  felist Info("Information about " + GetName(DEFINITE));

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if((EquipmentEasilyRecognized(c) || game::WizardModeActivated()) && GetEquipment(c))
      Info.AddEntry(EquipmentName(c) + ": " + GetEquipment(c)->GetName(INDEFINITE), LIGHT_GRAY, 0, GetEquipment(c)->GetPicture());

  if(Info.IsEmpty())
    ADD_MESSAGE("There's nothing special to tell about %s.", CHAR_NAME(DEFINITE));
  else
    {
      game::SetStandardListAttributes(Info);
      Info.Draw();
    }
}

void character::CompleteRiseFromTheDead()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      {
	GetBodyPart(c)->ResetSpoiling();
	GetBodyPart(c)->SetHP(1);
      }
}

bool character::RaiseTheDead(character*)
{
  bool Useful = false;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c))
      {
	CreateBodyPart(c)->SetHP(1);

	if(IsPlayer())
	  ADD_MESSAGE("Suddenly you grow a new %s.", GetBodyPartName(c).c_str());
	else if(CanBeSeenByPlayer())
	  ADD_MESSAGE("%s grows a new %s.", CHAR_NAME(DEFINITE), GetBodyPartName(c).c_str());

	Useful = true;
      }

  if(!Useful)
    {
      if(IsPlayer())
	ADD_MESSAGE("You shudder.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s shudders.", CHAR_NAME(DEFINITE));
    }

  return Useful;
}

void character::SetSize(ushort Size)
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->SetSize(GetBodyPartSize(c, Size));
}

ulong character::GetBodyPartSize(ushort Index, ushort TotalSize)
{
  if(Index == TORSO_INDEX)
    return TotalSize;
  else
    {
      ABORT("Weird bodypart size request for a character!");
      return 0;
    }
}

ulong character::GetBodyPartVolume(ushort Index) const
{
  if(Index == TORSO_INDEX)
    return GetTotalVolume();
  else
    {
      ABORT("Weird bodypart volume request for a character!");
      return 0;
    }
}

void character::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < GetBodyParts(); ++c) 
    CreateBodyPart(c, SpecialFlags);
}

void character::RestoreBodyParts()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c))
      CreateBodyPart(c);
}

void character::UpdatePictures()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    UpdateBodyPartPicture(c);
}

bodypart* character::MakeBodyPart(ushort Index)
{
  if(Index == TORSO_INDEX)
    return new normaltorso(0, NO_MATERIALS);
  else
    {
      ABORT("Weird bodypart to make for a character!");
      return 0;
    }
}

bodypart* character::CreateBodyPart(ushort Index, ushort SpecialFlags)
{
  bodypart* BodyPart = MakeBodyPart(Index);
  BodyPart->InitMaterials(CreateBodyPartMaterial(Index, GetBodyPartVolume(Index)), false);
  BodyPart->SetSize(GetBodyPartSize(Index, GetTotalSize()));
  SetBodyPart(Index, BodyPart);
  BodyPart->InitSpecialAttributes();

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdateBodyPartPicture(Index);

  if(!Initializing)
    {
      CalculateBattleInfo();

      if(GetSquareUnder())
	GetSquareUnder()->SendNewDrawRequest();
    }

  return BodyPart;
}

vector2d character::GetBodyPartBitmapPos(ushort Index)
{
  if(Index == TORSO_INDEX)
    return GetTorsoBitmapPos();
  else
    {
      ABORT("Weird bodypart BitmapPos request for a character!");
      return vector2d();
    }
}

ushort character::GetBodyPartColorA(ushort Index)
{
  if(Index < GetBodyParts())
    return GetSkinColor();
  else
    {
      ABORT("Weird bodypart color A request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColorB(ushort Index)
{
  if(Index == TORSO_INDEX)
    return GetTorsoMainColor();
  else
    {
      ABORT("Weird bodypart color B request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColorC(ushort Index)
{
  if(Index == TORSO_INDEX)
    return 0; // reserved for future use
  else
    {
      ABORT("Weird bodypart color C request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColorD(ushort Index)
{
  if(Index == TORSO_INDEX)
    return GetTorsoSpecialColor();
  else
    {
      ABORT("Weird bodypart color D request for a character!");
      return 0;
    }
}

void character::UpdateBodyPartPicture(ushort Index)
{
  bodypart* BodyPart = GetBodyPart(Index);

  if(BodyPart)
    {
      BodyPart->SetBitmapPos(GetBodyPartBitmapPos(Index));
      BodyPart->GetMainMaterial()->SetSkinColor(GetBodyPartColorA(Index));
      BodyPart->SetMaterialColorB(GetBodyPartColorB(Index));
      BodyPart->SetMaterialColorC(GetBodyPartColorC(Index));
      BodyPart->SetMaterialColorD(GetBodyPartColorD(Index));
      BodyPart->SetSpecialFlags(GetSpecialBodyPartFlags(Index));
      BodyPart->UpdatePictures();
    }
}

void character::LoadDataBaseStats()
{
  BaseAttribute[ENDURANCE] = GetDefaultEndurance() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[PERCEPTION] = GetDefaultPerception() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[INTELLIGENCE] = GetDefaultIntelligence() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[WISDOM] = GetDefaultWisdom() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[CHARISMA] = GetDefaultCharisma() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[MANA] = GetDefaultMana() * (100 + GetAttributeBonus()) / 100;
  SetMoney(GetDefaultMoney());

  const std::vector<long>& Skills = GetKnownCWeaponSkills();

  if(Skills.size())
    {
      const std::vector<long>& Hits = GetCWeaponSkillHits();

      if(Hits.size() == 1)
	{
	  for(ushort c = 0; c < Skills.size(); ++c)
	    CWeaponSkill[Skills[c]]->AddHit(Hits[0]);
	}
      else if(Hits.size() == Skills.size())
	{
	  for(ushort c = 0; c < Skills.size(); ++c)
	    CWeaponSkill[Skills[c]]->AddHit(Hits[c]);
	}
      else
	ABORT("Illegal weapon skill hit array size detected!");
    }
}

character* characterprototype::CloneAndLoad(inputfile& SaveFile) const
{
  character* Char = Cloner(0, LOAD);
  Char->Load(SaveFile);
  Char->CalculateAll();
  return Char;
}

void character::Initialize(ushort NewConfig, ushort SpecialFlags)
{
  Initializing = InNoMsgMode = true;
  CalculateBodyParts();
  CalculateAllowedWeaponSkillCategories();
  BodyPartSlot = new characterslot[BodyParts];
  OriginalBodyPartID = new std::list<ulong>[BodyParts];
  CWeaponSkill = new cweaponskill*[AllowedWeaponSkillCategories];

  ushort c;

  for(c = 0; c < BodyParts; ++c)
    BodyPartSlot[c].SetMaster(this);

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    CWeaponSkill[c] = new cweaponskill(c);

  if(!(SpecialFlags & LOAD))
    {
      Config = NewConfig;
      InstallDataBase();
      LoadDataBaseStats();
      TemporaryState |= GetClassStates();

      if(TemporaryState)
	for(c = 0; c < STATES; ++c)
	  if(TemporaryState & (1 << c))
	    TemporaryStateCounter[c] = 0;

      CreateBodyParts(SpecialFlags & NO_PIC_UPDATE);

      for(c = 0; c < BASE_ATTRIBUTES; ++c)
	BaseExperience[c] = 0;

      InitSpecialAttributes();

      if(!GetDefaultName().empty())
	SetAssignedName(GetDefaultName());
    }

  VirtualConstructor(SpecialFlags & LOAD);

  if(!(SpecialFlags & LOAD))
    {
      if(!(SpecialFlags & NO_EQUIPMENT))
	CreateInitialEquipment((SpecialFlags & NO_EQUIPMENT_PIC_UPDATE) >> 1);

      CalculateAll();
      RestoreHP();
    }

  Initializing = InNoMsgMode = false;
}

characterprototype::characterprototype(characterprototype* Base, character* (*Cloner)(ushort, ushort), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<character>::Add(this);
}

bool character::TeleportNear(character* Caller)
{
  vector2d Where = GetAreaUnder()->GetNearestFreeSquare(Caller, Caller->GetPos());

  if(Where == DIR_ERROR_VECTOR)
    return false;

  Teleport(Where);
  return true;
}

void character::Teleport(vector2d Pos)
{
  Move(Pos, true);
}

void character::ReceiveHeal(long Amount)
{
  ushort c;

  for(c = 0; c < Amount / 50; ++c)
    {
      uchar NeedHeal = 0, NeedHealIndex[MAX_BODYPARTS];

      for(ushort b = 0; b < BodyParts; ++b)
	{
	  bodypart* BodyPart = GetBodyPart(b);

	  if(BodyPart && BodyPart->IsAlive() && BodyPart->GetHP() < BodyPart->GetMaxHP())
	    NeedHealIndex[NeedHeal++] = b;
	}

      if(!NeedHeal)
	break;

      GetBodyPart(NeedHealIndex[RAND() % NeedHeal])->IncreaseHP();
      ++HP;
    }

  Amount -= c * 50;

  for(c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c) && RAND() & 1 && Amount >= 500)
      {
	if(IsPlayer())
	  ADD_MESSAGE("You grow a new %s!", GetBodyPartName(c).c_str());
	else if(CanBeSeenByPlayer())
	  ADD_MESSAGE("%s grows a new %s!", CHAR_NAME(DEFINITE), GetBodyPartName(c).c_str());

	GenerateRandomBodyPart()->SetHP(1);
	Amount -= 500;
      }
}

void character::AddHealingLiquidConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel better.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks healthier.", CHAR_NAME(DEFINITE));
}

void character::ReceiveSchoolFood(long SizeOfEffect)
{
  SizeOfEffect += RAND() % SizeOfEffect;

  if(SizeOfEffect >= 250)
    Vomit(SizeOfEffect / 250);

  if(!(RAND() % 3) && SizeOfEffect >= 500 && EditAttribute(ENDURANCE, SizeOfEffect / 500))
    {
      if(IsPlayer())
	ADD_MESSAGE("You gain a little bit of toughness for surviving this stuff.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks tougher.", CHAR_NAME(DEFINITE));
    }

  BeginTemporaryState(POISONED, (SizeOfEffect >> 1));
}

void character::AddSchoolFoodConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Yuck! This stuff tasted like vomit and old mousepads.");
}

void character::AddSchoolFoodHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Yuck! This stuff feels like vomit and old mousepads.");
}

void character::ReceiveNutrition(long SizeOfEffect)
{
  if(GetHungerState() == BLOATED)
    {
      ReceiveDamage(0, SizeOfEffect / 1000 ? SizeOfEffect / 1000 : 1, BULIMIA, TORSO);
      CheckDeath("choked on his food");
    }

  EditNP(SizeOfEffect);
}

void character::ReceiveOmmelUrine(long Amount)
{
  EditExperience(ARM_STRENGTH, Amount << 5);
  EditExperience(LEG_STRENGTH, Amount << 5);
  RestoreHP();
}

void character::AddOmmelUrineConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel a primitive Force coursing through your veins.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s looks more powerful.", CHAR_NAME(DEFINITE));
}

void character::ReceivePepsi(long SizeOfEffect)
{
  ReceiveDamage(0, SizeOfEffect / 100, POISON, TORSO);
  EditExperience(PERCEPTION, SizeOfEffect << 5);

  if(CheckDeath("was poisoned by pepsi"))
    return;

  if(IsPlayer())
    game::DoEvilDeed(SizeOfEffect / 10);
}

void character::AddPepsiConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Urgh. You feel your guruism fading away.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks very lame.", CHAR_NAME(DEFINITE));
}

void character::ReceiveDarkness(long SizeOfEffect)
{
  EditAttribute(INTELLIGENCE, -1);
  EditAttribute(WISDOM, -1);
  EditAttribute(CHARISMA, -1);
  EditAttribute(MANA, -1);

  if(IsPlayer())
    game::DoEvilDeed(short(SizeOfEffect / 50));
}

void character::AddFrogFleshConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Arg. You feel the fate of a navastater placed upon you...");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s looks like a navastater.", CHAR_NAME(DEFINITE));
}

void character::ReceiveKoboldFlesh(long)
{
  /* As it is commonly known, the possibility of fainting per 500 cubic centimeters of kobold flesh is exactly 5%. */

  if(!(RAND() % 20))
    {
      if(IsPlayer())
	ADD_MESSAGE("You lose control of your legs and fall down.");

      Faint();
    }
}

void character::AddKoboldFleshConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("This stuff tasted really funny.");
}

void character::AddKoboldFleshHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel very funny.");
}

void character::AddBoneConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel like a hippie.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s barks happily.", CHAR_NAME(DEFINITE)); // this suspects that nobody except dogs can eat bones
}

/* returns true if character manages to unstuck himself (from all traps...). vector2d is the direction which the character has tried to escape to */

bool character::TryToUnstuck(vector2d Direction)
{
  return StuckTo->TryToUnstuck(this, StuckToBodyPart, Direction);
}

bool character::IsStuck() const
{
  return GetStuckToBodyPart() != NONE_INDEX;
}

bool character::CheckForAttributeIncrease(ushort& Attribute, long& Experience, bool DoubleAttribute)
{
  /* Check if attribute is disabled for the creature */

  if(!Attribute)
    return false;

  bool Effect = false;

  while(true)
    {
      if(!DoubleAttribute)
	{
	  if(Experience > long(Attribute) << 10)
	    {
	      if(Attribute < 100)
		{
		  Attribute += 1;
		  Experience -= Attribute << 10;
		  Effect = true;
		  continue;
		}
	    }
	}
      else
	{
	  if(Experience > long(Attribute) << 9)
	    {
	      if(Attribute < 200)
		{
		  Attribute += 1;
		  Experience -= Attribute << 9;
		  Effect = true;
		  continue;
		}
	    }
	}

      break;
    }

  return Effect;
}

bool character::CheckForAttributeDecrease(ushort& Attribute, long& Experience, bool DoubleAttribute)
{
  /* Check if attribute is disabled for the creature */

  if(!Attribute)
    return false;

  bool Effect = false;

  while(true)
    {
      if(!DoubleAttribute)
	{
	  if(Experience < (long(Attribute) - 100) << 10)
	    {
	      if(Attribute > 1)
		{
		  Attribute -= 1;
		  Experience += Max(long(100 - Attribute) << 10, 0L);
		  Effect = true;
		  continue;
		}
	    }
	}
      else
	{
	  if(Experience < (long(Attribute) - 200) << 9)
	    {
	      if(Attribute > 2)
		{
		  Attribute -= 1;
		  Experience += Max(long(200 - Attribute) << 9, 0L);
		  Effect = true;
		  continue;
		}
	    }
	}

      break;
    }

  return Effect;
}

bool character::RawEditAttribute(ushort& Attribute, short& Amount, bool DoubleAttribute)
{
  /* Check if attribute is disabled for creature */

  if(!Attribute)
    return false;

  if(Amount < 0)
    {
      ushort Limit = DoubleAttribute ? 2 : 1;

      if(Attribute > 1)
	{
	  Attribute = Attribute > Limit - Amount ? Attribute + Amount : Limit;
	  return true;
	}
      else
	return false;
    }
  else
    {
      ushort Limit = DoubleAttribute ? 200 : 100;

      if(Attribute < Limit)
	{
	  Attribute = Attribute + Amount < Limit ? Attribute + Amount : Limit;
	  return true;
	}
      else
	return false;
    }
}

void character::DrawPanel(bool AnimationDraw) const
{
  if(AnimationDraw)
    {
      DrawStats(true);
      return;
    }

  DOUBLE_BUFFER->Fill(19 + (game::GetScreenSize().X << 4), 0, RES.X - 19 - (game::GetScreenSize().X << 4), RES.Y, 0);
  DOUBLE_BUFFER->Fill(16, 45 + game::GetScreenSize().Y * 16, game::GetScreenSize().X << 4, 9, 0);
  FONT->Printf(DOUBLE_BUFFER, 16, 45 + game::GetScreenSize().Y * 16, WHITE, "%s", CHAR_NAME(INDEFINITE));//, GetVerbalPlayerAlignment().c_str());

  ushort PanelPosX = RES.X - 96;
  ushort PanelPosY = DrawStats(false);

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "End %d", GetAttribute(ENDURANCE));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Per %d", GetAttribute(PERCEPTION));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Int %d", GetAttribute(INTELLIGENCE));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Wis %d", GetAttribute(WISDOM));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Cha %d", GetAttribute(CHARISMA));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Siz %d", GetSize());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, IsInBadCondition() ? RED : WHITE, "HP %d/%d", GetHP(), GetMaxHP());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Gold: %d", GetMoney());
  ++PanelPosY;

  if(game::IsInWilderness())
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Worldmap");
  else
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "%s", festring::CapitalizeCopy(game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrent())).c_str());

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Time: %d", game::GetTicks() / 10);

  ++PanelPosY;

  if(GetAction())
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "%s", festring::CapitalizeCopy(GetAction()->GetDescription()).c_str());

  for(ushort c = 0; c < STATES; ++c)
    if(!StateIsSecret[c] && StateIsActivated(1 << c) && (1 << c != HASTE || !StateIsActivated(SLOW)) && (1 << c != SLOW || !StateIsActivated(HASTE)))
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, (1 << c) & EquipmentState || !TemporaryStateCounter[c] ? BLUE : WHITE, "%s", StateDescription[c].c_str());

  if(GetHungerState() == STARVING)
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, RED, "Starving");
  else
    if(GetHungerState() == HUNGRY)
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, BLUE, "Hungry");
  else 
    if(GetHungerState() == SATIATED)
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Satiated");
  else
    if(GetHungerState() == BLOATED)
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Bloated");

  switch(GetBurdenState())
    {
    case OVER_LOADED:
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, RED, "Overload!");
      break;
    case STRESSED:
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, BLUE, "Stressed");
      break;
    case BURDENED:
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, BLUE, "Burdened");
    case UNBURDENED:
      break;
    }
}

void character::CalculateDodgeValue()
{
  DodgeValue = 0.1f * GetMoveEase() * GetAttribute(AGILITY) / sqrt(GetSize());

  if(CanFly())
    DodgeValue *= 2;

  if(DodgeValue < 1)
    DodgeValue = 1;
}

bool character::DamageTypeAffectsInventory(uchar Type) const
{
  switch(Type)
    {
    case SOUND:
    case ENERGY:
    case ACID:
    case FIRE:
      return true;
    case PHYSICAL_DAMAGE:
    case POISON:
    case BULIMIA:
      return false;
    default:
      ABORT("Unknown reaping effect destroyed dungeon!");
      return false;
    }
}

ushort character::CheckForBlockWithArm(character* Enemy, item* Weapon, arm* Arm, float WeaponToHitValue, ushort Damage, short Success, uchar Type)
{
  ushort BlockStrength = Arm->GetBlockCapability();
  float BlockValue = Arm->GetBlockValue();

  if(BlockStrength && BlockValue)
    {
      item* Blocker = Arm->GetWielded();

      if(RAND() % ushort(100 + WeaponToHitValue / Arm->GetBlockValue() * (100 + Success)) < 100)
	{
	  ushort NewDamage = BlockStrength < Damage ? Damage - BlockStrength : 0;

	  switch(Type)
	    {
	    case UNARMED_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->UnarmedHitNoun(), NewDamage != 0);
	      break;
	    case WEAPON_ATTACK:
	      AddBlockMessage(Enemy, Blocker, "hit", NewDamage != 0);
	      break;
	    case KICK_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->KickNoun(), NewDamage != 0);
	      break;
	    case BITE_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->BiteNoun(), NewDamage != 0);
	      break;
	    }

	  Arm->EditExperience(ARM_STRENGTH, 25);
	  Arm->EditExperience(DEXTERITY, 50);
	  Blocker->WeaponSkillHit();
	  Blocker->ReceiveDamage(this, Damage, PHYSICAL_DAMAGE);

	  if(Weapon)
	    Weapon->ReceiveDamage(Enemy, Damage - NewDamage, PHYSICAL_DAMAGE);

	  return NewDamage;
	}
    }

  return Damage;
}

bool character::ShowWeaponSkills()
{
  felist List("Your experience in weapon categories");

  List.AddDescription("");
  List.AddDescription("Category name                 Level     Points    Needed    Battle bonus");

  bool Something = false;

  for(ushort c = 0; c < GetAllowedWeaponSkillCategories(); ++c)
    if(GetCWeaponSkill(c)->GetHits())
      {
	std::string Buffer;
	Buffer << GetCWeaponSkill(c)->Name();
	Buffer.resize(30, ' ');
	Buffer << GetCWeaponSkill(c)->GetLevel();
	Buffer.resize(40, ' ');
	Buffer << GetCWeaponSkill(c)->GetHits();
	Buffer.resize(50, ' ');

	if(GetCWeaponSkill(c)->GetLevel() != 10)
	  Buffer << (GetCWeaponSkill(c)->GetLevelMap(GetCWeaponSkill(c)->GetLevel() + 1) - GetCWeaponSkill(c)->GetHits());
	else
	  Buffer << '-';

	Buffer.resize(60, ' ');
	Buffer << '+' << int(GetCWeaponSkill(c)->GetBonus() - 100) << '%';
	List.AddEntry(Buffer, LIGHT_GRAY);
	Something = true;
      }

  if(AddSpecialSkillInfo(List))
    Something = true;

  if(Something)
    {
      game::SetStandardListAttributes(List);
      List.Draw();
    }
  else
    ADD_MESSAGE("You are not experienced in any weapon skill yet!");

  return false;
}

long character::GetStateAPGain(long BaseAPGain) const
{
  if(StateIsActivated(HASTE))
    BaseAPGain <<= 1;

  if(StateIsActivated(SLOW))
    BaseAPGain >>= 1;

  return BaseAPGain;
}

void character::SignalEquipmentAdd(ushort EquipmentIndex)
{
  item* Equipment = GetEquipment(EquipmentIndex);

  if(EquipmentHasNoPairProblems(EquipmentIndex))
    {
      ushort AddedStates = Equipment->GetGearStates();

      if(AddedStates)
	for(ushort c = 0; c < STATES; ++c)
	  if(AddedStates & (1 << c))
	    {
	      if(!StateIsActivated(1 << c))
		{
		  if(!InNoMsgMode)
		    (this->*PrintBeginStateMessage[c])();

		  EquipmentState |= 1 << c;

		  if(BeginStateHandler[c])
		    (this->*BeginStateHandler[c])();
		}
	      else
		EquipmentState |= 1 << c;
	    }
    }

  if(!Initializing)
    ApplyEquipmentAttributeBonuses(Equipment);
}

void character::SignalEquipmentRemoval(ushort)
{
  CalculateEquipmentState();
  CalculateAttributeBonuses();
}

void character::CalculateEquipmentState()
{
  ushort c, Back = EquipmentState;
  EquipmentState = 0;

  for(c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && EquipmentHasNoPairProblems(c))
      EquipmentState |= GetEquipment(c)->GetGearStates();

  for(c = 0; c < STATES; ++c)
    if(Back & (1 << c) && !StateIsActivated(1 << c))
      {
	if(EndStateHandler[c])
	  {
	    (this->*EndStateHandler[c])();

	    if(!IsEnabled())
	      return;
	  }

	if(!InNoMsgMode)
	  (this->*PrintEndStateMessage[c])();
      }
}

/* Counter = duration in ticks */

void character::BeginTemporaryState(ushort State, ushort Counter)
{
  ushort Index;

  if(State == POLYMORPHED)
    ABORT("No Polymorphing with BeginTemporaryState!");

  for(Index = 0; Index < STATES; ++Index)
    if(1 << Index == State)
      break;

  if(Index == STATES)
    ABORT("BeginTemporaryState works only when State == 2 ^ n");

  if(TemporaryStateIsActivated(State))
    {
      if(GetTemporaryStateCounter(State))
	EditTemporaryStateCounter(State, Counter);
    }
  else
    {
      if(!EquipmentStateIsActivated(State))
	{
	  (this->*PrintBeginStateMessage[Index])();

	  if(BeginStateHandler[Index])
	    (this->*BeginStateHandler[Index])();
	}

      ActivateTemporaryState(State);
      SetTemporaryStateCounter(State, Counter);
    }
}

void character::HandleStates()
{
  if(!TemporaryState && !EquipmentState)
    return;

  for(ushort c = 0; c < STATES; ++c)
    {
      if(TemporaryState & (1 << c) && TemporaryStateCounter[c])
	{
	  if(!--TemporaryStateCounter[c])
	    {
	      TemporaryState &= ~(1 << c);

	      if(!(EquipmentState & (1 << c)))
		{
		  if(EndStateHandler[c])
		    {
		      (this->*EndStateHandler[c])();

		      if(!IsEnabled())
			return;
		    }

		  (this->*PrintEndStateMessage[c])();
		}
	    }
	}

      if(StateIsActivated(1 << c))
	if(StateHandler[c])
	  (this->*StateHandler[c])();

      if(!IsEnabled())
	return;
    }
}

void character::LoseIntrinsic(ushort What)
{
  ushort Index;

  for(Index = 0; Index < STATES; ++Index)
    if(1 << Index == What)
      break;

  TemporaryState &= ~What;

  if(!(EquipmentState & What))
    {
      if(EndStateHandler[Index])
	{
	  (this->*EndStateHandler[Index])();

	  if(!IsEnabled())
	    return;
	}

      (this->*PrintEndStateMessage[Index])();
    }
}

void character::PrintBeginPolymorphControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your mind has total control over your body.");
}

void character::PrintEndPolymorphControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You are somehow uncertain of your willpower.");
}

void character::PrintBeginLifeSaveMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You hear Hell's gates being locked just now.");
}

void character::PrintEndLifeSaveMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel the Afterlife is welcoming you once again.");
}

void character::PrintBeginLycanthropyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly notice you've always loved full moons.");
}

void character::PrintEndLycanthropyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel the wolf inside you has had enough of your bad habits.");
}

void character::PrintBeginInvisibilityMessage() const
{
  if((game::GetPlayer()->StateIsActivated(INFRA_VISION) && IsWarm()) || (game::GetPlayer()->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
    {
      if(IsPlayer())
	ADD_MESSAGE("You seem somehow transparent.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s seems somehow transparent.", CHAR_NAME(DEFINITE));
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("You fade away.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s disappears!", CHAR_NAME(DEFINITE));
    }
}

void character::PrintEndInvisibilityMessage() const
{
  if((game::GetPlayer()->StateIsActivated(INFRA_VISION) && IsWarm()) || (game::GetPlayer()->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your notice your transparency has ended.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("The appearance of %s seems far more solid now.", CHAR_NAME(INDEFINITE));
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("You reappear.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s appears from nowhere!", CHAR_NAME(INDEFINITE));
    }
}

void character::PrintBeginInfraVisionMessage() const
{
  if(IsPlayer())
    if(StateIsActivated(INVISIBLE) && IsWarm() && !(StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
      ADD_MESSAGE("You reappear.");
    else
      ADD_MESSAGE("You feel your perception being magically altered.");
}

void character::PrintEndInfraVisionMessage() const
{
  if(IsPlayer())
    if(StateIsActivated(INVISIBLE) && IsWarm() && !(StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
      ADD_MESSAGE("You disappear.");
    else
      ADD_MESSAGE("You feel your perception returning to normal.");
}

void character::PrintBeginESPMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly feel like being only a tiny part of a great network of intelligent minds.");
}

void character::PrintEndESPMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You are filled with desire to be just yourself from now on.");
}

void character::PrintBeginHasteMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Time slows down to a crawl.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks faster!", CHAR_NAME(DEFINITE));
}

void character::PrintEndHasteMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Everything seems to move much faster now.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks slower!", CHAR_NAME(DEFINITE));
}

void character::PrintBeginSlowMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Everything seems to move much faster now.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks slower!", CHAR_NAME(DEFINITE));
}

void character::PrintEndSlowMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Time slows down to a crawl.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks faster!", CHAR_NAME(DEFINITE));
}

void character::EndPolymorph()
{
  if(IsPlayer())
    ADD_MESSAGE("You return to your true form.");
  else if(game::IsInWilderness())
    return; // fast gum solution, state ends when the player enters a dungeon
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s returns to %s true form.", CHAR_NAME(DEFINITE), GetPossessivePronoun().c_str());

  if(GetAction())
    GetAction()->Terminate(false);

  SendToHell();
  GetSquareUnder()->RemoveCharacter();
  character* Char = GetPolymorphBackup();
  InNoMsgMode = Char->InNoMsgMode = true;
  SetPolymorphBackup(0);
  GetSquareUnder()->AddCharacter(Char);
  Char->SetHasBe(true);
  Char->SetPolymorphed(false);
  SetSquareUnder(0);
  GetStack()->MoveItemsTo(Char->GetStack());

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Item = GetEquipment(c);

      if(Item)
	{
	  if(Char->CanUseEquipment(c))
	    {
	      Item->RemoveFromSlot();
	      Char->SetEquipment(c, Item);
	    }
	  else
	    Item->MoveTo(IsPlayer() ? Char->GetStack() : Char->GetStackUnder());
	}
    }

  Char->SetMoney(GetMoney());
  Char->ChangeTeam(GetTeam());

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(Char);

  InNoMsgMode = Char->InNoMsgMode = false;
  Char->CalculateAll();

  if(IsPlayer())
    {
      game::SetPlayer(Char);
      game::SendLOSUpdateRequest();
      UpdateESPLOS();
    }

  Char->TestWalkability();
}

void character::LycanthropyHandler()
{
  if(!(RAND() % 2500))
    Polymorph(new werewolfwolf, RAND() % 2500);
}

void character::SaveLife()
{
  if(TemporaryStateIsActivated(LIFE_SAVED))
    {
      if(IsPlayer())
	ADD_MESSAGE("But wait! You glow briefly red and seem to be in a better shape!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("But wait, suddenly %s glows briefly red and seems to be in a better shape!", GetPersonalPronoun().c_str());

      DeActivateTemporaryState(LIFE_SAVED);
    }
  else
    {
      item* LifeSaver = 0;

      for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	if(GetEquipment(c) && EquipmentHasNoPairProblems(c) && GetEquipment(c)->GetGearStates() & LIFE_SAVED)
	  LifeSaver = GetEquipment(c);

      if(!LifeSaver)
	ABORT("The Universe can only kill one once!");

      if(IsPlayer())
	ADD_MESSAGE("But wait! Your %s glows briefly red and disappears and you seem to be in a better shape!", LifeSaver->CHAR_NAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("But wait, suddenly %s %s glows briefly red and disappears and %s seems to be in a better shape!", GetPossessivePronoun().c_str(), LifeSaver->CHAR_NAME(UNARTICLED), GetPersonalPronoun().c_str());

      LifeSaver->RemoveFromSlot();
      LifeSaver->SendToHell();
    }

  if(IsPlayer())
    game::AskForKeyPress("Life saved! [press any key to continue]");

  RestoreBodyParts();
  RestoreHP();

  if(GetNP() < 10000)
    SetNP(10000);

  GetSquareUnder()->SendNewDrawRequest();
}

void character::PolymorphRandomly(ushort Time)
{
  character* NewForm = 0;

  if(StateIsActivated(POLYMORPH_CONTROL))
    {
      if(IsPlayer())
	while(!NewForm)
	  {
	    std::string Temp = game::StringQuestion("What do you want to become?", vector2d(16, 6), WHITE, 0, 80, false);
	    NewForm = protosystem::CreateMonster(Temp, NO_EQUIPMENT);
	  }
      else
	{
	  switch(GetSex())
	    {
	    case UNDEFINED: NewForm = new frog(GREATER_DARK, NO_EQUIPMENT); break;
	    case MALE: NewForm = new communist(0, NO_EQUIPMENT); break;
	    case FEMALE: NewForm = new mistress(WAR_LADY, NO_EQUIPMENT); break;
	    }
	}
    }
  else
    NewForm = protosystem::CreateMonster(NO_EQUIPMENT);

  Polymorph(NewForm, Time);
}

/* In reality, the reading takes Time / (Intelligence * 10) turns */

void character::StartReading(item* Item, ulong Time)
{
  read* Read = new read(this);
  Read->SetLiterature(Item); // slot cleared automatically

  if(game::WizardModeActivated() && Time > 100)
    Time = 100;

  Read->SetCounter(Time);
  SetAction(Read);

  if(IsPlayer())
    ADD_MESSAGE("You start reading %s.", Item->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s starts reading %s.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(DEFINITE));
}

/*
 * Call when one makes something with his/her/its hands.
 * Difficulty of 5 takes about one turn, so it's the most common to use.
 */

void character::DexterityAction(ushort Difficulty)
{
  EditAP(-20000 * Difficulty / APBonus(GetAttribute(DEXTERITY)));
  EditExperience(DEXTERITY, 5 * Difficulty);
}

/* if Theretically true then Range is not a factor */
bool character::CanBeSeenByPlayer(bool Theoretically) const
{
  bool Visible = !StateIsActivated(INVISIBLE) || (game::GetPlayer()->StateIsActivated(INFRA_VISION) && IsWarm());

  if((game::IsInWilderness() && Visible) || (game::GetPlayer()->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5 && (Theoretically || (GetPos() - game::GetPlayer()->GetPos()).Length() <= game::GetPlayer()->ESPRangeSquare())))
    return true;
  else if(!Visible)
    return false;
  else
    return Theoretically || GetSquareUnder()->CanBeSeenByPlayer(game::GetPlayer()->StateIsActivated(INFRA_VISION) && IsWarm());
}

bool character::CanBeSeenBy(const character* Who, bool Theoretically) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer(Theoretically);
  else
    {
      bool Visible = !StateIsActivated(INVISIBLE) || (Who->StateIsActivated(INFRA_VISION) && IsWarm());

      if((game::IsInWilderness() && Visible) || (Who->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5 && (Theoretically || (GetPos() - Who->GetPos()).Length() <= Who->ESPRangeSquare())))
	return true;
      else if(!Visible)
	return false;
      else
	return Theoretically || GetSquareUnder()->CanBeSeenFrom(Who->GetPos(), Who->LOSRangeSquare(), Who->StateIsActivated(INFRA_VISION) && IsWarm());
    }
}

bool character::DetachBodyPart()
{
  ADD_MESSAGE("You haven't got any extra bodyparts.");
  return false;
}

void character::AttachBodyPart(bodypart* BodyPart)
{
  SetBodyPart(BodyPart->GetBodyPartIndex(), BodyPart);
  BodyPart->ResetSpoiling();
  BodyPart->ResetPosition();
  BodyPart->UpdatePictures();
  CalculateAttributeBonuses();
  CalculateBattleInfo();
  GetSquareUnder()->SendNewDrawRequest();
}

/* Returns true if the character has all bodyparts, false if not. */ 

bool character::HasAllBodyParts() const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c))
      return false;

  return true;
}

bodypart* character::GenerateRandomBodyPart()
{
  std::vector<ushort> NeededBodyParts;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c))
      NeededBodyParts.push_back(c);

  if(NeededBodyParts.empty())
    return 0;

  return CreateBodyPart(NeededBodyParts[RAND() % NeededBodyParts.size()]);
}

/* searched for character's Stack and if it find some bodyparts there that are the character's old bodyparts returns a stackiterator to one of them (choocen in random). If no fitting bodyparts are found the function returns 0 */

bodypart* character::FindRandomOwnBodyPart() const
{
  std::vector<bodypart*> LostAndFound;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c))
      for(std::list<ulong>::iterator i = OriginalBodyPartID[c].begin(); i != OriginalBodyPartID[c].end(); ++i)
	{
	  item* Found = SearchForItemWithID(*i);

	  if(Found)
	    LostAndFound.push_back(static_cast<bodypart*>(Found));
	}

  if(LostAndFound.empty())
    return 0;
  else
    return LostAndFound[RAND() % LostAndFound.size()];
}

void character::PrintBeginPoisonedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You seem to be very ill.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks very ill.", CHAR_NAME(DEFINITE));
}

void character::PrintEndPoisonedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel better again.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks better.", CHAR_NAME(DEFINITE)); 
}

void character::PoisonedHandler()
{
  if(!(RAND() % 100))
    Vomit(2 + RAND() % 2);

  ushort Damage = 0;

  for(ushort Used = 0; Used < GetTemporaryStateCounter(POISONED); Used += 100)
    if(!(RAND() % 50))
      ++Damage;

  if(Damage)
    {
      ReceiveDamage(0, Damage, POISON, ALL, 8, false, false, false, false);
      CheckDeath("died of acute poisoning");
    }
}

bool character::IsWarm() const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->IsAlive())
      return true;

  return false;
}

void character::BeginInvisibility()
{
  UpdatePictures();

  if(GetSquareUnder())
    GetSquareUnder()->SendNewDrawRequest();
}

void character::BeginInfraVision()
{
  if(IsPlayer())
    GetAreaUnder()->SendNewDrawRequest();
}

void character::BeginESP()
{
  if(IsPlayer())
    GetAreaUnder()->SendNewDrawRequest();
}

void character::EndInvisibility()
{
  UpdatePictures();
  GetSquareUnder()->SendNewDrawRequest();
}

void character::EndInfraVision()
{
  if(IsPlayer())
    GetAreaUnder()->SendNewDrawRequest();
}

void character::EndESP()
{
  if(IsPlayer())
    GetAreaUnder()->SendNewDrawRequest();
}

void character::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  if((game::GetPlayer()->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5 && (game::GetPlayer()->GetPos() - GetPos()).Length() <= game::GetPlayer()->ESPRangeSquare()) || (game::GetPlayer()->StateIsActivated(INFRA_VISION) && IsWarm()))
    Luminance = configuration::GetContrastLuminance();

  DrawBodyParts(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(configuration::GetOutlineCharacters())
    {
      igraph::GetTileBuffer()->Fill(TRANSPARENT_COLOR);
      DrawBodyParts(igraph::GetTileBuffer(), vector2d(0, 0), MakeRGB24(128, 128, 128), false, AllowAnimate);
      igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetCharacterOutlineColor());
      igraph::GetOutlineBuffer()->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, configuration::GetContrastLuminance());
    }
  
  if(GetTeam() == game::GetPlayer()->GetTeam() && !IsPlayer())
    igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, 32, 16, Pos, 16, 16, configuration::GetContrastLuminance());
}

void character::DrawBodyParts(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  GetTorso()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
}

void character::SetConfig(ushort NewConfig)
{
  Config = NewConfig;
  InstallDataBase();
  CalculateAll();
}

god* character::GetMasterGod() const
{
  return game::GetGod(GetConfig());
}

void character::InstallDataBase()
{
  /*
   * Plain database without preceding colons would mean
   * characterdatabase and cause a syntax error. Sorry for bad naming.
   */

  ::database<character>::InstallDataBase(this);
}

void character::PrintBeginTeleportMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel jumpy.");
}

void character::PrintEndTeleportMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly realize you've always preferred walking to jumping.");
}

void character::TeleportHandler()
{
  if(!(RAND() % 1500))
    TeleportRandomly();
}

void character::PrintBeginPolymorphMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("An unconfortable uncertainty of who you really are overwhelms you.");
}

void character::PrintEndPolymorphMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel you are you and no one else.");
}

void character::PolymorphHandler()
{
  if(!(RAND() % 400))
    PolymorphRandomly(200 + RAND() % 800);
}

void character::PrintBeginTeleportControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel very controlled.");
}

void character::PrintEndTeleportControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your control slipping.");
}

void character::DisplayStethoscopeInfo(character*) const
{
  felist Info("Information about " + GetName(DEFINITE));
  Info.AddEntry(std::string("Endurance: ") + GetAttribute(ENDURANCE), LIGHT_GRAY);
  Info.AddEntry(std::string("Perception: ") + GetAttribute(PERCEPTION), LIGHT_GRAY);
  Info.AddEntry(std::string("Intelligence: ") + GetAttribute(INTELLIGENCE), LIGHT_GRAY);
  Info.AddEntry(std::string("Wisdom: ") + GetAttribute(WISDOM), LIGHT_GRAY);
  Info.AddEntry(std::string("Charisma: ") + GetAttribute(CHARISMA), LIGHT_GRAY);
  Info.AddEntry(std::string("Carried weight: ") + GetCarriedWeight() + "g", LIGHT_GRAY);
  Info.AddEntry(std::string("Total weight: ") + GetWeight() + "g", LIGHT_GRAY);
  Info.AddEntry(std::string("HP: ") + GetHP() + "/" + GetMaxHP(), LIGHT_GRAY);
  game::SetStandardListAttributes(Info);
  Info.Draw();
}

bool character::CanUseStethoscope(bool PrintReason) const
{
  if(PrintReason)
    ADD_MESSAGE("This type of monster can't use a stethoscope.");

  return false;
}

/* 
 * Effect used by atleast Sophos. 
 * NOTICE: Doesn't check for death! 
 */

void character::TeleportSomePartsAway(ushort NumberToTeleport)
{
  for(ushort c = 0; c < NumberToTeleport; ++c)
    {
      uchar RandomBodyPart = GetRandomNonVitalBodyPart();
      if(RandomBodyPart == NONE_INDEX)
	{
	  for(;c < NumberToTeleport; ++c)
	    {
	      GetTorso()->SetHP(GetTorso()->GetHP() - RAND() % 5 - 1);
	      ulong TorsosVolume = GetTorso()->GetMainMaterial()->GetVolume() / 10;

	      if(TorsosVolume == 0)
		break;
	      
	      ulong Amount = (RAND() % TorsosVolume) + 1;
	      lump* Lump = new lump(0, NO_MATERIALS); 
	      Lump->InitMaterials(GetTorso()->GetMainMaterial()->Clone(Amount));
	      GetTorso()->GetMainMaterial()->SetVolume(GetTorso()->GetMainMaterial()->GetVolume() - Amount);
	      Lump->MoveTo(GetNearLSquare(GetLevelUnder()->GetRandomSquare())->GetStack());

	      if(IsPlayer())
		ADD_MESSAGE("Parts of you teleport away.");
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("Parts of %s teleport away.", CHAR_NAME(DEFINITE));
	    }
	  return;
	}
      else
	{
	  item* SeveredBodyPart = SevereBodyPart(RandomBodyPart);
	  GetNearLSquare(GetLevelUnder()->GetRandomSquare())->GetStack()->AddItem(SeveredBodyPart);
	  SeveredBodyPart->DropEquipment();

	  if(IsPlayer())
	    ADD_MESSAGE("Your %s teleports away.", SeveredBodyPart->CHAR_NAME(UNARTICLED));
	  else
	    ADD_MESSAGE("%s %s teleports away.", GetPossessivePronoun().c_str(), SeveredBodyPart->CHAR_NAME(UNARTICLED));
	}	
    }
}

/* Returns an index of a random bodypart that is not vital. If no non-vital bodypart is found returns NONE_INDEX */

uchar character::GetRandomNonVitalBodyPart()
{
  ushort OKBodyPart[MAX_BODYPARTS];
  ushort OKBodyParts = 0;
  ushort c;
  for(c = 0; c < GetBodyParts(); ++c)
    {
      if(GetBodyPart(c) && !BodyPartVital(c))
	{
	  OKBodyPart[OKBodyParts] = c;
	  ++OKBodyParts;
	}
    }
  if(OKBodyParts)
    return OKBodyPart[RAND() % OKBodyParts];
  else
    return NONE_INDEX;
}

void character::CalculateVolumeAndWeight()
{
  Volume = Stack->GetVolume();
  Weight = Stack->GetWeight();

  if(Action)
    {
      Volume += Action->GetVolume();
      Weight += Action->GetWeight();
    }

  BodyVolume = 0;
  CarriedWeight = Weight;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      {
	BodyVolume += GetBodyPart(c)->GetBodyPartVolume();
	Volume += GetBodyPart(c)->GetVolume();
	CarriedWeight += GetBodyPart(c)->GetCarriedWeight();
	Weight += GetBodyPart(c)->GetWeight();
      }
}

void character::SignalVolumeAndWeightChange()
{
  if(!Initializing)
    {
      CalculateVolumeAndWeight();

      if(IsEnabled())
	CalculateBurdenState();

      if(MotherEntity)
	MotherEntity->SignalVolumeAndWeightChange();
    }
}

void character::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
    {
      game::CombineLights(Emitation, EmitationUpdate);

      if(MotherEntity)
	MotherEntity->SignalEmitationIncrease(EmitationUpdate);
      else if(SquareUnder && !game::IsInWilderness())
	GetLSquareUnder()->SignalEmitationIncrease(EmitationUpdate);
    }
}

void character::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
    {
      ulong Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation)
	{
	  if(MotherEntity)
	    MotherEntity->SignalEmitationDecrease(EmitationUpdate);
	  else if(SquareUnder && !game::IsInWilderness())
	    GetLSquareUnder()->SignalEmitationDecrease(EmitationUpdate);
	}
    }
}

void character::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      game::CombineLights(Emitation, GetBodyPart(c)->GetEmitation());

  game::CombineLights(Emitation, Stack->GetEmitation());

  if(Action)
    game::CombineLights(Emitation, Action->GetEmitation());
}

void character::CalculateAll()
{
  Initializing = true;
  CalculateAttributeBonuses();
  CalculateVolumeAndWeight();
  CalculateEmitation();
  CalculateBodyPartMaxHPs();
  CalculateBurdenState();
  CalculateBattleInfo();
  Initializing = false;
}

void character::CalculateHP()
{
  HP = 0;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      HP += GetBodyPart(c)->GetHP();
}

void character::CalculateMaxHP()
{
  MaxHP = 0;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      MaxHP += GetBodyPart(c)->GetMaxHP();
}

void character::CalculateBodyPartMaxHPs()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->CalculateMaxHP();

  CalculateMaxHP();
  CalculateHP();
}

bool character::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier == ENDURANCE && !IsAlive())
    return false;

  if(RawEditAttribute(BaseAttribute[Identifier], Value))
    {
      if(!Initializing)
	{
	  if(Identifier == LEG_STRENGTH)
	    CalculateBurdenState();
	  else if(Identifier == ENDURANCE)
	    CalculateBodyPartMaxHPs();
	  else if(Identifier == PERCEPTION && IsPlayer())
	    game::SendLOSUpdateRequest();

	  CalculateBattleInfo();
	}

      return true;
    }
  else
    return false;
}

void character::EditExperience(ushort Identifier, long Value)
{
  if(Identifier == ENDURANCE && !IsAlive())
    return;

  BaseExperience[Identifier] += Value;
}

bool character::ActivateRandomState(ushort Time)
{
  ushort ToBeActivated = GetRandomNotActivatedState();

  if(ToBeActivated == 0)
    return false;

  BeginTemporaryState(ToBeActivated, Time);
  return true;
}

bool character::GainRandomInstric()
{
  ushort ToBeActivated = GetRandomNotActivatedState();

  if(ToBeActivated == 0)
    return false;

  GainIntrinsic(ToBeActivated);
  return true;
}

/*
 * Returns a state that is 
 * 1) Not active
 * 2) ActivatableRandomly
 * if no such state is found return 0
 */

ushort character::GetRandomNotActivatedState()
{
  ushort OKStates[STATES];
  ushort NumberOfOKStates = 0;

  for(ushort c = 0; c < STATES; ++c)
    if(StateCanBeRandomlyActivated[c])
      OKStates[NumberOfOKStates++] = 1 << c;

  if(NumberOfOKStates == 0)
    return 0;
  
  return OKStates[RAND() % NumberOfOKStates];
}

void characterprototype::CreateSpecialConfigurations()
{
  if(Config.begin()->second.CreateGolemMaterialConfigurations)
    for(ushort c = 1; c < protocontainer<material>::GetProtoAmount(); ++c)
      {
	const material::databasemap& MaterialConfig = protocontainer<material>::GetProto(c)->GetConfig();

	for(material::databasemap::const_iterator i = MaterialConfig.begin(); i != MaterialConfig.end(); ++i)
	  if(i->first && i->second.IsGolemMaterial && Config.find(i->first) == Config.end())
	    {
	      character::database TempDataBase(Config.begin()->second);
	      TempDataBase.InitDefaults(i->first);
	      Config[i->first] = TempDataBase;
	    }
      }
}

float character::GetTimeToDie(const character* Enemy, ushort Damage, float ToHitValue, bool AttackIsBlockable, bool UseMaxHP) const
{
  float DodgeValue = GetDodgeValue();

  if(!Enemy->CanBeSeenBy(this, true))
    ToHitValue *= 2;

  if(!CanBeSeenBy(Enemy, true))
    DodgeValue *= 2;

  float MinHits = 1000;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(BodyPartVital(c) && GetBodyPart(c))
      {
	float Hits = GetBodyPart(c)->GetTimeToDie(Damage, ToHitValue, DodgeValue, AttackIsBlockable, UseMaxHP);

	if(Hits < MinHits)
	  MinHits = Hits;
      }

  return MinHits;
}

float character::GetDangerModifier() const
{
  return game::GetDangerMap().find(configid(GetType(), GetConfig()))->second.Danger;
}

float character::GetRelativeDanger(const character* Enemy, bool UseMaxHP) const
{
  float Danger = Enemy->GetTimeToKill(this, UseMaxHP) / GetTimeToKill(Enemy, UseMaxHP);
  return Limit(Danger, -100.0f, 100.0f);
}

bool character::ReloadDatafiles()
{
  databasesystem::Initialize();
  return false;
}

std::string character::GetBodyPartName(ushort Index, bool Articled) const
{
  if(Index == TORSO_INDEX)
    return Articled ? "a torso" : "torso";
  else
    {
      ABORT("Illegal character bodypart name request!");
      return 0;
    }
}

item* character::SearchForItemWithID(ulong ID) const
{
  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->GetID() == ID)
      return GetEquipment(c);

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->GetID() == ID)
      return *i;

  return 0;
}

bool character::ContentsCanBeSeenBy(const character* Viewer) const
{
  return Viewer == this;
}

bool character::HitEffect(character* Enemy, item* Weapon, uchar Type, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  if(Weapon)
    return Weapon->HitEffect(this, Enemy, BodyPartIndex, Direction, BlockedByArmour);

  switch(Type)
    {
    case UNARMED_ATTACK:
      return Enemy->SpecialUnarmedEffect(this, BodyPartIndex, Direction, BlockedByArmour);
    case KICK_ATTACK:
      return Enemy->SpecialKickEffect(this, BodyPartIndex, Direction, BlockedByArmour);
    case BITE_ATTACK:
      return Enemy->SpecialBiteEffect(this, BodyPartIndex, Direction, BlockedByArmour);
    }

  return false;
}

void character::WeaponSkillHit(item* Weapon, uchar Type)
{
  ushort Category;

  switch(Type)
    {
    case UNARMED_ATTACK:
      Category = UNARMED;
      break;
    case WEAPON_ATTACK:
      Weapon->WeaponSkillHit();
      return;
    case KICK_ATTACK:
      Category = KICK;
      break;
    case BITE_ATTACK:
      Category = BITE;
      break;
    default:
      ABORT("Illegal Type %d passed to character::WeaponSkillHit()!", Type);
      return;
    }

  if(GetCWeaponSkill(Category)->AddHit())
    {
      CalculateBattleInfo();

      if(IsPlayer())
	GetCWeaponSkill(Category)->AddLevelUpMessage();
    }
}

void character::AddDefenceInfo(felist& List) const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      {
	std::string Entry = "   ";
	Entry << GetBodyPart(c)->GetBodyPartName();
	Entry.resize(60, ' ');
	Entry << GetBodyPart(c)->GetMaxHP();
	Entry.resize(70, ' ');
	Entry << GetBodyPart(c)->GetTotalResistance(PHYSICAL_DAMAGE);
	List.AddEntry(Entry, LIGHT_GRAY);
      }
}

/* returns 0 if character cannot be cloned */

character* character::Duplicate() const
{
  if(!CanBeCloned())
    return 0;

  character* Char = RawDuplicate();
  Char->CalculateAll();
  return Char;
}

bool character::TryToEquip(item* Item)
{
  if(!CanUseEquipment())
    return false;

  for(ushort e = 0; e < GetEquipmentSlots(); ++e)
    if(GetBodyPartOfEquipment(e) && (!EquipmentSorter(e) || EquipmentSorter(e)(Item, this)))
      {
	msgsystem::DisableMessages();
	float Danger = GetRelativeDanger(game::GetPlayer());
	item* OldEquipment = GetEquipment(e);

	if(OldEquipment)
	  OldEquipment->RemoveFromSlot();

	Item->RemoveFromSlot();
	SetEquipment(e, Item);
	float NewDanger = GetRelativeDanger(game::GetPlayer());
	Item->RemoveFromSlot();
	GetStackUnder()->AddItem(Item);

	if(OldEquipment)
	  SetEquipment(e, OldEquipment);

	msgsystem::EnableMessages();

	if(OldEquipment)
	  {
	    if(NewDanger > Danger)
	      {
		if(!GetRoomUnder() || GetRoomUnder()->PickupItem(this, Item, 1))
		  {
		    if(CanBeSeenByPlayer())
		      ADD_MESSAGE("%s drops %s %s and equips %s instead.", CHAR_NAME(DEFINITE), CHAR_POSSESSIVE_PRONOUN, OldEquipment->CHAR_NAME(UNARTICLED), Item->CHAR_NAME(INDEFINITE));

		    if(GetRoomUnder())
		      GetRoomUnder()->DropItem(this, OldEquipment, 1);

		    OldEquipment->MoveTo(GetStackUnder());
		    Item->RemoveFromSlot();
		    SetEquipment(e, Item);
		    DexterityAction(5);
		    return true;
		  }
	      }
	  }
	else
	  {
	    if(NewDanger > Danger || (NewDanger == Danger && e != RIGHT_WIELDED_INDEX && e != LEFT_WIELDED_INDEX))
	      {
		if(!GetRoomUnder() || GetRoomUnder()->PickupItem(this, Item, 1))
		  {
		    if(CanBeSeenByPlayer())
		      ADD_MESSAGE("%s picks up and equips %s.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(INDEFINITE));

		    Item->RemoveFromSlot();
		    SetEquipment(e, Item);
		    DexterityAction(5);
		    return true;
		  }
	      }
	  }
      }

  return false;
}

bool character::TryToConsume(item* Item)
{
  if(Item->IsConsumable(this) && !Item->IsBadFoodForAI(this) && (!GetRoomUnder() || GetRoomUnder()->ConsumeItem(this, Item, 1)))
    {
      ConsumeItem(Item);
      return true;
    }
  else
    return false;
}

void character::UpdateESPLOS() const
{
  if(StateIsActivated(ESP) && !game::IsInWilderness())
    for(ushort c = 0; c < game::GetTeams(); ++c)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
	  (*i)->GetSquareUnder()->SendNewDrawRequest();
}

uchar character::GetCWeaponSkillLevel(const item* Item) const
{
  if(Item->GetWeaponCategory() < GetAllowedWeaponSkillCategories())
    return GetCWeaponSkill(Item->GetWeaponCategory())->GetLevel();
  else
    return 0;
}

void character::PrintBeginPanicMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You panic!");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s panics.", CHAR_NAME(DEFINITE));  
}

void character::PrintEndPanicMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You finally calm down.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s calms down.", CHAR_NAME(DEFINITE));
}

void character::CheckPanic(ushort Ticks)
{
  if(GetHP() * 100 < GetPanicLevel() * GetMaxHP())
    BeginTemporaryState(PANIC, ((Ticks * 3) >> 2) + RAND() % ((Ticks >> 1) + 1)); // 25% randomness to ticks...
}

/* returns 0 if fails else the newly created character */

character* character::CloneToNearestSquare(character*) const
{
  vector2d Where = GetAreaUnder()->GetNearestFreeSquare(this, GetPos());

  if(Where == DIR_ERROR_VECTOR)
    {
      ADD_MESSAGE("You sense a disturbance in the Force.");
      return 0;
    }

  character* NewlyCreated = Duplicate();

  if(!NewlyCreated)
    return 0;
  
  GetLevelUnder()->GetLSquare(Where)->AddCharacter(NewlyCreated);
  return NewlyCreated;
}

void character::SignalSpoil()
{
  /* Add support for spoiling zombies! */

  if(GetMotherEntity())
    GetMotherEntity()->SignalSpoil(0);
}

square* character::GetSquareUnder() const
{
  return !MotherEntity ? SquareUnder : MotherEntity->GetSquareUnder();
}

bool character::CanHeal() const
{
  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->IsAlive() && BodyPart->GetHP() < BodyPart->GetMaxHP())
	return true;
    }

  return false;
}

void character::ReceiveFluidSpill(material* Liquid, ushort HitPercent)
{
  Liquid->Effect(this, Liquid->GetVolume() * HitPercent / 100);
}

uchar character::GetRelation(const character* Who) const
{
  return GetTeam()->GetRelation(Who->GetTeam());
}

void character::CalculateAttributeBonuses()
{
  ushort c;

  for(c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->CalculateAttributeBonuses();

  short BackupBonus[BASE_ATTRIBUTES];
  short BackupCarryingBonus = CarryingBonus;
  CarryingBonus = 0;
  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    {
      BackupBonus[c] = AttributeBonus[c];
      AttributeBonus[c] = 0;
    }

  for(c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(!Equipment)
	continue;

      if(Equipment->AffectsEndurance())
	AttributeBonus[ENDURANCE] += Equipment->GetEnchantment();

      if(Equipment->AffectsPerception())
	AttributeBonus[PERCEPTION] += Equipment->GetEnchantment();

      if(Equipment->AffectsIntelligence())
	AttributeBonus[INTELLIGENCE] += Equipment->GetEnchantment();

      if(Equipment->AffectsWisdom())
	AttributeBonus[WISDOM] += Equipment->GetEnchantment();

      if(Equipment->AffectsCharisma())
	AttributeBonus[CHARISMA] += Equipment->GetEnchantment();

      if(Equipment->AffectsMana())
	AttributeBonus[MANA] += Equipment->GetEnchantment();
      
      if(Equipment->AffectsCarryingCapacity())
	CarryingBonus += Equipment->GetCarryingBonus();
    }

    if(!Initializing && AttributeBonus[ENDURANCE] != BackupBonus[ENDURANCE])
      CalculateBodyPartMaxHPs();

    if(IsPlayer() && !Initializing && AttributeBonus[PERCEPTION] != BackupBonus[PERCEPTION])
      game::SendLOSUpdateRequest();

    if(IsPlayer() && !Initializing && AttributeBonus[INTELLIGENCE] != BackupBonus[INTELLIGENCE])
      UpdateESPLOS();

    if(!Initializing && CarryingBonus != BackupCarryingBonus)
      CalculateBurdenState();
}

void character::ApplyEquipmentAttributeBonuses(item* Equipment)
{
  if(Equipment->AffectsEndurance())
    {
      AttributeBonus[ENDURANCE] += Equipment->GetEnchantment();
      CalculateBodyPartMaxHPs();
    }

  if(Equipment->AffectsPerception())
    {
      AttributeBonus[PERCEPTION] += Equipment->GetEnchantment();

      if(IsPlayer())
	game::SendLOSUpdateRequest();
    }

  if(Equipment->AffectsIntelligence())
    {
      AttributeBonus[INTELLIGENCE] += Equipment->GetEnchantment();

      if(IsPlayer())
	UpdateESPLOS();
    }

  if(Equipment->AffectsWisdom())
    AttributeBonus[WISDOM] += Equipment->GetEnchantment();

  if(Equipment->AffectsCharisma())
    AttributeBonus[CHARISMA] += Equipment->GetEnchantment();

  if(Equipment->AffectsMana())
    AttributeBonus[MANA] += Equipment->GetEnchantment();

  if(Equipment->AffectsCarryingCapacity())
    {
      CarryingBonus += Equipment->GetCarryingBonus();
      CalculateBurdenState();
    }
}

void character::ReceiveAntidote(long Amount)
{
  if(StateIsActivated(POISONED))
    {
      long Left = GetTemporaryStateCounter(POISONED);
      if(Left > Amount)
	{
	  EditTemporaryStateCounter(POISONED, -Amount);
	}
      else
	{
	  if(IsPlayer())
	    ADD_MESSAGE("Aaaah... You feel MUCH better.");

	  DeActivateTemporaryState(POISONED);
	}
    }
}

void character::AddAntidoteConsumeEndMessage() const
{
  if(StateIsActivated(POISONED)) /* true only if the poison didn't cure the poison completely */
				 /* Comment: Whaaat? */
    {
      if(IsPlayer())
	ADD_MESSAGE("Your body processes the poison in your veins with rapid speed.");
    }
}

bool character::IsDead() const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(BodyPartVital(c) && (!GetBodyPart(c) || GetBodyPart(c)->GetHP() < 1))
      return true;

  return false;
}

void character::SignalSpoilLevelChange()
{
  /* Add support for spoiling zombies! */

  if(GetMotherEntity())
    GetMotherEntity()->SignalSpoilLevelChange(0);
}

void character::AddOriginalBodyPartID(ushort Index, ulong What)
{
  OriginalBodyPartID[Index].push_back(What);

  if(OriginalBodyPartID[Index].size() > 100)
    OriginalBodyPartID[Index].erase(OriginalBodyPartID[Index].begin());
}

void character::AddToInventory(const std::vector<contentscript<item> >& ItemVector, ushort SpecialFlags)
{
  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c].IsValid())
      GetStack()->AddItem(ItemVector[c].Instantiate(SpecialFlags));
}

void character::ShowDodgeAndMoveInfo() const
{
  felist Info("Dodge and move info:");
  game::SetStandardListAttributes(Info);
  short Bonus;

  /* Dodge value */

  Info.AddEntry("Dodge value: how easily you may avoid enemy attacks", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry(std::string("Base: ") + int(100 / sqrt(GetSize())), LIGHT_GRAY);
  AddSpecialMovePenaltyInfo(Info);
  Bonus = GetAttribute(AGILITY) * 10;

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real dodge value: ") + int(GetDodgeValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Move speed */

  Info.AddEntry("Move speed: how often you may move", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 10", LIGHT_GRAY);
  AddSpecialMovePenaltyInfo(Info);
  Bonus = APBonus(GetAttribute(AGILITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real move speed: ") + 10000 / GetMoveAPRequirement(1), MakeRGB16(220, 220, 220));
  Info.Draw();
}

bool character::HasHadBodyPart(const item* Item) const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(std::find(OriginalBodyPartID[c].begin(), OriginalBodyPartID[c].end(), Item->GetID()) != OriginalBodyPartID[c].end())
      return true;

  return GetPolymorphBackup() && GetPolymorphBackup()->HasHadBodyPart(Item);
}

bool character::SummonMonster()
{
  character* Summoned = 0;

  while(!Summoned)
    Summoned = protosystem::CreateMonster(game::StringQuestion("Summon which monster?", vector2d(16, 6), WHITE, 0, 80, false));

  Summoned->SetTeam(game::GetTeam(MONSTER_TEAM));
  vector2d Where = GetLevelUnder()->GetNearestFreeSquare(Summoned, GetPos());

  if(Where == DIR_ERROR_VECTOR)
    Where = GetLevelUnder()->GetRandomSquare(Summoned);

  GetNearLSquare(Where)->AddCharacter(Summoned);
  return false;
}

/* this is evil */

#define SEARCH_N_REPLACE(what, with)\
for(Pos = Reply.find(what, 0); Pos != std::string::npos; Pos = Reply.find(what, Pos))\
{\
  Reply.erase(Pos, 3);\
  Reply.insert(Pos, with);\
}

void character::MakeReply(std::string Reply) const
{
  strsize Pos;
  SEARCH_N_REPLACE("@nu", GetName(UNARTICLED));
  SEARCH_N_REPLACE("@ni", GetName(INDEFINITE));
  SEARCH_N_REPLACE("@nd", GetName(DEFINITE));
  SEARCH_N_REPLACE("@du", GetDescription(UNARTICLED));
  SEARCH_N_REPLACE("@di", GetDescription(INDEFINITE));
  SEARCH_N_REPLACE("@dd", GetDescription(DEFINITE));
  SEARCH_N_REPLACE("@Nu", festring::CapitalizeCopy(GetName(UNARTICLED)));
  SEARCH_N_REPLACE("@Ni", festring::CapitalizeCopy(GetName(INDEFINITE)));
  SEARCH_N_REPLACE("@Nd", festring::CapitalizeCopy(GetName(DEFINITE)));
  SEARCH_N_REPLACE("@Du", festring::CapitalizeCopy(GetDescription(UNARTICLED)));
  SEARCH_N_REPLACE("@Di", festring::CapitalizeCopy(GetDescription(INDEFINITE)));
  SEARCH_N_REPLACE("@Dd", festring::CapitalizeCopy(GetDescription(DEFINITE)));
  SEARCH_N_REPLACE("@Gd", GetMasterGod()->Name().c_str());
  ADD_MESSAGE("%s", Reply.c_str());
}

void character::BeTalkedTo()
{
  static std::vector<bool> Said;

  if(GetRelation(game::GetPlayer()) == HOSTILE)
    {
      if(Said.size() != GetHostileReplies().size())
	Said.resize(GetHostileReplies().size());

      MakeReply(GetHostileReplies()[RandomizeReply(Said)]);
    }
  else
    {
      if(Said.size() != GetFriendlyReplies().size())
	Said.resize(GetFriendlyReplies().size());

      MakeReply(GetFriendlyReplies()[RandomizeReply(Said)]);
    }
}

bool character::CheckZap()
{
  if(!CanZap())
    {
      ADD_MESSAGE("This monster type can't zap.");
      return false;
    }

  return true;
}

void character::DamageAllItems(character* Damager, ushort Damage, uchar Type)
{
  GetStack()->ReceiveDamage(Damager, Damage, Type);

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c))
      GetEquipment(c)->ReceiveDamage(Damager, Damage, Type);
}

bool character::Equips(const item* Item) const
{
  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->GetID() == Item->GetID())
      return true;

  return false;
}

void character::AddAttributeInfo(std::string& Entry) const
{
  Entry.resize(57, ' ');
  Entry << GetAttribute(ENDURANCE);
  Entry.resize(60, ' ');
  Entry << GetAttribute(PERCEPTION);
  Entry.resize(63, ' ');
  Entry << GetAttribute(INTELLIGENCE);
  Entry.resize(66, ' ');
  Entry << GetAttribute(WISDOM);
  Entry.resize(69, ' ');
  Entry << GetAttribute(CHARISMA);
  Entry.resize(72, ' ');
  Entry << GetAttribute(MANA);
}
