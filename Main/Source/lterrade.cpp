#define __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DECLARATIONS__

#include "proto.h"
#include "lterraba.h"

std::vector<glterrain::prototype*> protocontainer<glterrain>::ProtoData;
std::vector<olterrain::prototype*> protocontainer<olterrain>::ProtoData;
valuemap protocontainer<glterrain>::CodeNameMap;
valuemap protocontainer<olterrain>::CodeNameMap;

#include "femath.h"
#include "lterrade.h"

#undef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DECLARATIONS__

#include "message.h"
#include "godba.h"
#include "level.h"
#include "dungeon.h"
#include "feio.h"
#include "hscore.h"
#include "lsquare.h"
#include "worldmap.h"
#include "charba.h"
#include "whandler.h"
#include "stack.h"
#include "itemba.h"
#include "config.h"
#include "itemba.h"
#include "save.h"

bool door::Open(character* Opener)
{
  if(!IsWalkable())
    {
      if(IsLocked())
	{
	  if(Opener->IsPlayer())
	    ADD_MESSAGE("The door is locked.");
	  
	  return false;
	}
      else if(RAND() % 20 < Opener->GetAttribute(ARMSTRENGTH))
	{
	  MakeWalkable();

	  if(Opener->IsPlayer())
	    ADD_MESSAGE("You open the door.");
	  else if(CanBeSeenByPlayer())
	    {
	      if(Opener->CanBeSeenByPlayer())
		ADD_MESSAGE("%s opens the door.", Opener->CHARNAME(DEFINITE));
	      else
		ADD_MESSAGE("Something opens the door.");
	    }
	}
      else
	{
	  if(Opener->IsPlayer())
	    ADD_MESSAGE("The door resists.");
	  else if(CanBeSeenByPlayer() && Opener->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s fails to open the door.", Opener->CHARNAME(DEFINITE));

	  ActivateBoobyTrap();
	}

      Opener->DexterityAction(Opener->OpenMultiplier() * 5);
      return true;
    }
  else
    {
      if(Opener->IsPlayer()) ADD_MESSAGE("The door is already open, %s.", game::Insult());
      return false;
    }
}

bool door::Close(character* Closer)
{
  if(Closer->IsPlayer())
    if(IsWalkable())
      {
	if(RAND() % 20 < Closer->GetAttribute(ARMSTRENGTH))
	  {
	    ADD_MESSAGE("You close the door.");
	    MakeNotWalkable();
	  }
	else
	  ADD_MESSAGE("The door resists!");
      }
    else
      {
	ADD_MESSAGE("The door is already closed, %s.", game::Insult());
	return false;
      }

  Closer->DexterityAction(Closer->OpenMultiplier() * 5);
  return true;
}

void altar::Draw(bitmap* Bitmap, vector2d Pos, ushort Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  olterrain::Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
  igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, GetDivineMaster() << 4, 0, Pos, 16, 16, Luminance);
}

void altar::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  SaveFile >> DivineMaster;
}

void altar::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << DivineMaster;
}

bool stairsup::GoUp(character* Who) const // Try to go up
{
  if(game::GetCurrent())
    {
      if(game::GetCurrent() == 9)
	if(!game::BoolQuestion("Somehow you get the feeling you cannot return. Continue anyway? [y/N]"))
	  return false;

      std::vector<character*> MonsterList;

      if(!GetLSquareUnder()->GetLevelUnder()->CollectCreatures(MonsterList, Who, true))
	return false;

      game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
      game::GetCurrentDungeon()->SaveLevel();
      game::SetCurrent(game::GetCurrent() - 1);
      game::GetCurrentDungeon()->PrepareLevel();
      game::GetCurrentLevel()->GetSquare(game::GetCurrentLevel()->GetDownStairs())->KickAnyoneStandingHereAway();
      game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetDownStairs(), Who);

      for(std::vector<character*>::iterator c = MonsterList.begin(); c != MonsterList.end(); ++c)
	game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(*c, game::GetCurrentLevel()->GetDownStairs()), *c);

      game::GetCurrentLevel()->Luxify();
      game::SendLOSUpdateRequest();
      game::UpdateCamera();
      game::GetCurrentArea()->UpdateLOS();
      if(configuration::GetAutoSaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    {
      if(Who->IsPlayer())
	{
	  std::vector<character*> TempPlayerGroup;

	  if(!GetLSquareUnder()->GetLevelUnder()->CollectCreatures(TempPlayerGroup, Who, false))
	    return false;

	  game::GetCurrentArea()->RemoveCharacter(Who->GetPos());
	  game::GetCurrentDungeon()->SaveLevel();
	  game::LoadWorldMap();

	  game::GetWorldMap()->GetPlayerGroup().swap(TempPlayerGroup);

	  game::SetIsInWilderness(true);
	  game::GetCurrentArea()->AddCharacter(game::GetCurrentDungeon()->GetWorldMapPos(), Who);
	  game::SendLOSUpdateRequest();
	  game::UpdateCamera();
	  game::GetCurrentArea()->UpdateLOS();
	  if(configuration::GetAutoSaveInterval())
	    game::Save(game::GetAutoSaveFileName().c_str());
	  return true;
	}

      return false;
    }
}

bool stairsdown::GoDown(character* Who) const // Try to go down
{
  if(game::GetCurrent() != game::GetLevels() - 1)
    {
      if(game::GetCurrent() == 8)
	{
	  ADD_MESSAGE("A great evil power seems to tremble under your feet.");
	  ADD_MESSAGE("You feel you shouldn't wander any further.");

	  if(!game::BoolQuestion("Continue anyway? [y/N]"))
	    return false;
	}

      std::vector<character*> MonsterList;

      if(!GetLSquareUnder()->GetLevelUnder()->CollectCreatures(MonsterList, Who, true))
	return false;

      if(game::GetCurrent() == 8)
	Who->GetLSquareUnder()->ChangeLTerrain(new parquet, new empty);

      game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
      game::GetCurrentDungeon()->SaveLevel();
      game::SetCurrent(game::GetCurrent() + 1);
      game::GetCurrentDungeon()->PrepareLevel();
      game::GetCurrentLevel()->GetSquare(game::GetCurrentLevel()->GetUpStairs())->KickAnyoneStandingHereAway();
      game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetUpStairs(), Who);

      for(std::vector<character*>::iterator c = MonsterList.begin(); c != MonsterList.end(); ++c)
	game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(*c, game::GetCurrentLevel()->GetUpStairs()), *c);

      game::GetCurrentLevel()->Luxify();
      game::ShowLevelMessage();
      game::SendLOSUpdateRequest();
      game::UpdateCamera();
      game::GetCurrentArea()->UpdateLOS();
      if(configuration::GetAutoSaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    {
      if(Who->IsPlayer())
	ADD_MESSAGE("You are at the bottom.");

      return false;
    }
}

void door::BeKicked(character*, float KickStrength)
{
  if(!IsWalkable()) 
    {
      if(!IsLocked() && KickStrength > RAND() % 100000)
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The door opens.");

	  MakeWalkable();
	}
      else if(KickStrength > RAND() % 200000)
	{
	  if(IsLocked() && RAND() % 2)	// _can't really think of a good formula for this... 
	    {				//Strength isn't everything
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("The lock breaks and the door is damaged.");

	      SetIsLocked(false);
	    }
	  else
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("The door is damaged.");
	    }

	  Break();
	}
      else
	if(CanBeSeenByPlayer())
	  ADD_MESSAGE("The door won't budge!");

      // The door may have been destroyed here, so don't do anything!
    }
}

void door::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << Opened << Locked << LockType << BoobyTrap;
}

void door::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  SaveFile >> Opened >> Locked >> LockType >> BoobyTrap;
}

void door::MakeWalkable()
{
  SetIsOpened(true);
  UpdatePictures();
  GetLSquareUnder()->SendNewDrawRequest();
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  GetLSquareUnder()->SetDescriptionChanged(true);
  GetLSquareUnder()->ForceEmitterEmitation();
  GetLSquareUnder()->CalculateLuminance();

  if(GetLSquareUnder()->GetLastSeen() == game::GetLOSTurns())
    {
      game::SendLOSUpdateRequest();
      //game::GetCurrentArea()->UpdateLOS();
    }

  ActivateBoobyTrap();
}

void door::MakeNotWalkable()
{
  GetLSquareUnder()->ForceEmitterNoxify();
  SetIsOpened(false);
  UpdatePictures();
  GetLSquareUnder()->SendNewDrawRequest();
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  GetLSquareUnder()->SetDescriptionChanged(true);
  GetLSquareUnder()->ForceEmitterEmitation();
  GetLSquareUnder()->CalculateLuminance();

  if(GetLSquareUnder()->GetLastSeen() == game::GetLOSTurns())
    {
      game::SendLOSUpdateRequest();
      //game::GetCurrentArea()->UpdateLOS();
    }
}

void altar::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer() && DivineMaster && !game::GetGod(DivineMaster)->GetKnown())
    {
      ADD_MESSAGE("The ancient altar is covered with strange markings. You manage to decipher them. The altar is dedicated to %s, the %s. You now know the sacred rituals that allow you to contact this deity via prayers.", game::GetGod(DivineMaster)->Name().c_str(), game::GetGod(DivineMaster)->Description().c_str());
      game::GetGod(DivineMaster)->SetKnown(true);
    }
}

bool throne::SitOn(character* Sitter)
{
  Sitter->EditAP(-1000);

  if(Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() != 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler. The daydream fades in a whisper: \"Thou shalt be a My Champion first!\"");
      return true;
    }

  if(Sitter->HasPetrussNut() && !Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler. The daydream fades in a whisper: \"Thou shalt wear My shining armor first!\"");
      return true;
    }

  if(!Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler. The daydream fades in a whisper: \"Thou shalt surpass thy predecessor first!\"");
      return true;
    }

  if(Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      game::TextScreen("A heavenly choir starts to sing Grandis Rana and a booming voice fills the air:\n\n\"Mortal! Thou hast surpassed Petrus, and pleaseth Me greatly during thine adventures!\nI hereby title thee as My new High Priest!\"\n\nYou are victorious!");
      game::GetPlayer()->AddScoreEntry("became the new High Priest of the Great Frog", 5, false);
      game::End();
      return true;
    }

  ADD_MESSAGE("You feel somehow out of place.");
  return true;
}

void altar::BeKicked(character* Kicker, float)
{
  if(Kicker->IsPlayer())
    ADD_MESSAGE("You feel like a sinner.");
  else if(Kicker->CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks like a sinner.", Kicker->CHARNAME(DEFINITE));

  if(Kicker->IsPlayer())
    {
      game::GetGod(GetLSquareUnder()->GetDivineMaster())->PlayerKickedAltar();

      if(GetLSquareUnder()->GetDivineMaster() > 1)
	game::GetGod(GetLSquareUnder()->GetDivineMaster() - 1)->PlayerKickedFriendsAltar();

      if(GetLSquareUnder()->GetDivineMaster() < game::GetGods() - 1)
	game::GetGod(GetLSquareUnder()->GetDivineMaster() + 1)->PlayerKickedFriendsAltar();
    }
}

void altar::ReceiveVomit(character* Who)
{
  if(Who->IsPlayer())
    game::GetGod(GetLSquareUnder()->GetDivineMaster())->PlayerVomitedOnAltar();
}

std::string door::GetAdjective(bool Articled) const
{
  std::string Adj;

  if(Articled)
    Adj = std::string(Opened ? "an open" : "a closed");
  else
    Adj = std::string(Opened ? "open" : "closed");

  if(IsLocked())
    Adj += ", locked";

  return Adj;
}

bool couch::SitOn(character* Sitter)
{
  ADD_MESSAGE("The couch is extremely soft and confortable. You relax well.");
  Sitter->EditAP(-1000);
  return true;
}

bool poolterrain::SitOn(character* Sitter)
{
  ADD_MESSAGE("You sit on the pool. Oddly enough, you sink. You feel stupid.");
  Sitter->EditAP(-1000);
  return true;
}

void stairsup::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer()) 
    ADD_MESSAGE("There is stairway leading upwards here.");
}

void stairsdown::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer()) 
    ADD_MESSAGE("There is stairway leading downwards here.");
}

bool bookcase::SitOn(character* Sitter)
{
  ADD_MESSAGE("The bookcase is very unconfortable to sit on.");
  Sitter->EditAP(-1000);
  return true;
}

bool fountain::SitOn(character* Sitter)
{
  if(GetContainedMaterial())
    {
      ADD_MESSAGE("You sit on the fountain. Water falls on your head and you get quite wet. You feel like a moron.");
      Sitter->EditAP(-1000);
      return true;
    }
  else
    return olterrain::SitOn(Sitter);
}

bool doublebed::SitOn(character* Sitter)
{
  ADD_MESSAGE("The beautiful bed is very soft. You get a feeling it's not meant for your kind of people.");
  Sitter->EditAP(-1000);
  return true;
}

bool fountain::Drink(character* Drinker)
{
  if(GetContainedMaterial())
    {
      if(GetContainedMaterial()->GetConfig() == WATER) 
	{
	  if(GetLSquareUnder()->GetRoom() && GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->HasDrinkHandler())
	    {
	      if(!GetLSquareUnder()->GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->Drink(Drinker))
		return false;
	    }
	  else
	    {
	      if(!game::BoolQuestion("Do you want to drink from the fountain? [y/N]"))
		{
		  return false;
		}
	    }

	  Drinker->EditAP(-1000);

	  switch(RAND() % 5)
	    {
	    case 0:
	      ADD_MESSAGE("The water is contaminated!");
	      Drinker->EditNP(50);

	      if(!(RAND() % 5))
		Drinker->PolymorphRandomly(2500 + RAND() % 2500);
	      else
		Drinker->ChangeRandomStat(-1);

	      break;
	
	    case 1:
	      ADD_MESSAGE("The water tasted very good.");
	      Drinker->EditNP(1000);
	      Drinker->ChangeRandomStat(1);
	      break;

	    case 2:
	      if(!(RAND() % 10))
		{
		  ADD_MESSAGE("You have freed a spirit that grants you a wish. You may wish for an item.");

		  while(true)
		    {
		      std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
		      item* TempItem = protosystem::CreateItem(Temp, Drinker->IsPlayer());

		      if(TempItem)
			{
			  Drinker->GetStack()->AddItem(TempItem);
			  ADD_MESSAGE("%s appears from nothing and the spirit flies happily away!", TempItem->CHARNAME(INDEFINITE));
			  break;
			}
		    }
		}
	      else
		DryOut();

	      break;

	    default:
	      ADD_MESSAGE("The water tastes good.");
	      Drinker->EditNP(250);
	      break;
	    }

	  // fountain might have dried out: don't do anything here.

	  return true;
	}
      else
	{
	  ADD_MESSAGE("You don't dare to drink from this fountain.");
	  return false;
	}
    }
  else
    {
      ADD_MESSAGE("The fountain has dried out.");
      return false;
    }
}

void fountain::DryOut()
{
  ADD_MESSAGE("%s dries out.", CHARNAME(DEFINITE));
  ChangeContainedMaterial(0);
  UpdatePictures();

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(CanBeSeenByPlayer())
	GetLSquareUnder()->UpdateMemorizedDescription();

      GetSquareUnder()->SendNewDrawRequest();
      GetSquareUnder()->SendMemorizedUpdateRequest();

      if(CanBeSeenByPlayer())
	GetLSquareUnder()->UpdateMemorized();
    }
}

void brokendoor::BeKicked(character*, float KickStrength)
{
  if(!IsWalkable())
    if(IsLocked())
      {
	if(KickStrength > RAND() % 150000)
	  {
	    if(CanBeSeenByPlayer())
	      ADD_MESSAGE("The door opens from the force of your kick.");

	    SetIsLocked(false);
	    MakeWalkable();
	  }
	else if(KickStrength > RAND() % 100000)
	  {
	    if(CanBeSeenByPlayer())
	      ADD_MESSAGE("The lock breaks from the force of your kick.");

	    SetIsLocked(false);
	  }
	else
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The door won't budge!");
      }
    else
      if(KickStrength > RAND() % 100000)
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The broken door opens.");

	  MakeWalkable();
	}
      else
	if(CanBeSeenByPlayer())
	  ADD_MESSAGE("The door resists.");
}

bool door::ReceiveDamage(character*, short, uchar)
{
  if(RAND() % 2)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s breaks.", CHARNAME(DEFINITE));

      Break();
    }
  else
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s opens.", CHARNAME(DEFINITE));
		
      MakeWalkable();
      SetIsLocked(false);
    }

  return true;
}

bool brokendoor::ReceiveDamage(character*, short, uchar)
{
  if(RAND() % 2)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s opens.", CHARNAME(DEFINITE));

      MakeWalkable();
      SetIsLocked(false);
    }

  return true;
}

bool altar::Polymorph(character*)
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s glows briefly.", CHARNAME(DEFINITE));
	
  uchar OldGod = DivineMaster;

  while(DivineMaster == OldGod)
    DivineMaster = 1 + RAND() % (game::GetGods() - 1);

  GetSquareUnder()->SendNewDrawRequest();
  GetSquareUnder()->SendMemorizedUpdateRequest();
  GetSquareUnder()->SetDescriptionChanged(true);

  if(CanBeSeenByPlayer())
    {
      GetLSquareUnder()->UpdateMemorizedDescription();
      GetLSquareUnder()->UpdateMemorized();
    }

  return true;	
}

bool altar::SitOn(character* Sitter)
{
  ADD_MESSAGE("You kneel down and worship %s for a moment.", game::GetGod(DivineMaster)->GOD_NAME);

  if(game::GetGod(DivineMaster)->GetRelation() < 500)
    {
      if(!(RAND() % 20))
	{
	  game::GetGod(DivineMaster)->AdjustRelation(2);
	  game::ApplyDivineAlignmentBonuses(game::GetGod(DivineMaster), true, 1);
	}
    }
  else
    if(!(RAND() % 2500))
      {
	character* Angel = game::GetGod(DivineMaster)->CreateAngel();

	if(Angel)
	  {
	    Angel->SetTeam(game::GetPlayer()->GetTeam());
	    ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHARNAME(DEFINITE));
	  }

	game::GetGod(DivineMaster)->AdjustRelation(50);
	game::ApplyDivineAlignmentBonuses(game::GetGod(DivineMaster), true);
      }

  Sitter->EditAP(-1000);
  return true;
}

bool poolterrain::IsWalkable(character* ByWho) const
{
  return ByWho && (ByWho->CanSwim() || ByWho->CanFly());
}

void couch::ShowRestMessage(character*) const
{
  ADD_MESSAGE("You rest well on the soft sofa.");
}

void doublebed::ShowRestMessage(character*) const
{
  ADD_MESSAGE("You lay yourself on the confortable bed.");
}

void door::HasBeenHitBy(item* Hitter, float Speed, uchar)
{
  if(!IsWalkable())
    {
      float Energy = Speed * Hitter->GetWeight() / 100;  
      // Newton is rolling in his grave. 
      if(CanBeSeenByPlayer() && game::WizardModeActivated())
	{
	  ADD_MESSAGE("Energy hitting the door: %f.", Energy);
	}

      if(Energy > 1000)
	{
	  // The door opens
	  MakeWalkable();

	  if(CanBeSeenByPlayer())
	    {
	      ADD_MESSAGE("%s hits %s and %s opens.", Hitter->CHARNAME(DEFINITE), CHARNAME(DEFINITE), CHARNAME(DEFINITE));
	    }
	}
      else if(Energy > 500)
	{
	  // The door breaks
	  if(IsLocked())
	    SetIsLocked(RAND() % 2 ? true : false);

	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s hits %s and %s breaks.", Hitter->CHARNAME(DEFINITE), CHARNAME(DEFINITE), CHARNAME(DEFINITE));

	  Break();
	} 
      else
	{
	  // Nothing happens
	  if(CanBeSeenByPlayer())
	    {
	      ADD_MESSAGE("%s hits %s. ", Hitter->CHARNAME(DEFINITE), CHARNAME(DEFINITE), CHARNAME(DEFINITE));
	    }
	}
    }
}

void brokendoor::HasBeenHitBy(item* Hitter, float Speed, uchar)
{
  if(!IsWalkable())
    {
      float Energy = Speed * Hitter->GetWeight() / 100;  
      // I hear Newton screaming in his grave.
      if(CanBeSeenByPlayer() && game::WizardModeActivated())
	{
	  ADD_MESSAGE("Energy hitting the broken door: %f.", Energy);
	}
      if(Energy > 400)
	{
	  // The door opens
	  MakeWalkable();

	  if(CanBeSeenByPlayer())
	    {
	      ADD_MESSAGE("%s hits %s and %s opens.", Hitter->CHARNAME(DEFINITE), CHARNAME(DEFINITE), CHARNAME(DEFINITE));
	    }
	}
      else
	{
	  // Nothing happens
	  if(CanBeSeenByPlayer())
	    {
	      ADD_MESSAGE("%s hits %s. ", Hitter->CHARNAME(DEFINITE), CHARNAME(DEFINITE), CHARNAME(DEFINITE));
	    }
	}
    }
}

void door::Break()
{
  if(BoobyTrap)
    ActivateBoobyTrap();
  else
    {
      brokendoor* Temp = new brokendoor(false);
      Temp->InitMaterials(GetMainMaterial());
      Temp->SetIsLocked(IsLocked());
      Temp->SetBoobyTrap(0);
      Temp->SetLockType(GetLockType());
      GetLSquareUnder()->ChangeOLTerrainAndUpdateLights(Temp);
    }
}

void door::ActivateBoobyTrap()
{
  switch(BoobyTrap)
    {
    case 1:
      // Explosion
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s is booby trapped!", CHARNAME(DEFINITE));

      BoobyTrap = 0;
      GetLSquareUnder()->GetLevelUnder()->Explosion(0, "killed by an exploding booby trapped door", GetPos(), 20 + RAND() % 10 - RAND() % 10);
      break;
    case 0:
      break;
    }
}

void door::CreateBoobyTrap()
{
  SetBoobyTrap(1); 
}

bool fountain::DipInto(item* ToBeDipped, character* Who)
{
  if(GetContainedMaterial())
    {
      ToBeDipped->DipInto(GetContainedMaterial()->Clone(GetContainedMaterial()->TakeDipVolumeAway()), Who);
      return true;
    }
  else
    return false;
}

void fountain::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << ContainedMaterial;
}

void fountain::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  LoadMaterial(SaveFile, ContainedMaterial);

}

material* fountain::GetMaterial(ushort Index) const
{
  switch(Index)
    {
    case 0: return MainMaterial;
    case 1: return ContainedMaterial;
    default: return 0;
    }
}

ushort fountain::GetMaterialColor1(ushort) const
{
  if(GetContainedMaterial())
    return GetContainedMaterial()->GetColor();
  else
    return 0;
}

uchar fountain::GetAlpha1(ushort) const
{
  if(GetContainedMaterial())
    return GetContainedMaterial()->GetAlpha();
  else
    return 0;
}

void door::VirtualConstructor(bool Load)
{
  olterrain::VirtualConstructor(Load);

  if(!Load)
    {
      SetBoobyTrap(0);
      SetIsOpened(false);
      SetIsLocked(false);
      SetBoobyTrap(0);
      SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
      SetHP(500);
    }
}

void altar::VirtualConstructor(bool Load)
{
  olterrain::VirtualConstructor(Load);

  if(!Load)
    SetDivineMaster(1 + RAND() % (game::GetGods() - 1));
}

void door::SetParameters(uchar Param)
{
  SetIsLocked(Param & LOCKED);
}

bool door::TryKey(item* Thingy, character* Applier)
{
  if(Opened)
    return false;

  if(Thingy->CanOpenLockType(GetLockType()))
    {
      if(Applier->IsPlayer())
	{
	  if(IsLocked())
	    ADD_MESSAGE("You unlock the door.");
	  else
	    ADD_MESSAGE("You lock the door.");
	}
      else if(Applier->CanBeSeenByPlayer())
	{
	  if(IsLocked())
	    ADD_MESSAGE("%s unlocks the door.", Applier->CHARNAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s locks the door.", Applier->CHARNAME(DEFINITE));
	}

      SetIsLocked(!IsLocked());
      return true;
    }
  else
    {
      if(Applier->IsPlayer())
	ADD_MESSAGE("%s doesn't fit into the lock.", Thingy->CHARNAME(DEFINITE));

      return false;
    }
}

