#include "lterrade.h"
#include "message.h"
#include "god.h"
#include "igraph.h"
#include "save.h"
#include "level.h"
#include "charba.h"
#include "dungeon.h"
#include "feio.h"
#include "hscore.h"
#include "lsquare.h"
#include "worldmap.h"
#include "charba.h"
#include "team.h"
#include "whandler.h"
#include "stack.h"
#include "itemba.h"
#include "femath.h"
#include "config.h"

bool door::Open(character* Opener)
{
  if(!GetIsWalkable())
    {
      if(IsLocked)
	{
	  if(Opener->GetIsPlayer())
	    ADD_MESSAGE("The door is locked.");

	  return false;
	}
      else if(RAND() % 20 < Opener->GetStrength())
	{
	  MakeWalkable();

	  if(Opener->GetIsPlayer())
	    ADD_MESSAGE("You open the door.");
	  else if(GetLevelSquareUnder()->CanBeSeen())
	    {
	      if(Opener->GetLevelSquareUnder()->CanBeSeen())
		ADD_MESSAGE("%s opens the door.", Opener->CNAME(DEFINITE));
	      else
		ADD_MESSAGE("Something opens the door.");
	    }

	  return true;
	}
      else
	{
	  if(Opener->GetIsPlayer())
	    ADD_MESSAGE("The door resists.");
	  else if(GetLevelSquareUnder()->CanBeSeen())
	    if(Opener->GetLevelSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s fails to open the door.", Opener->CNAME(DEFINITE));

	  return true;
	}
    }
  else
    {
      if(Opener->GetIsPlayer()) ADD_MESSAGE("The door is already open, %s.", game::Insult());
      return false;
    }
}

bool door::Close(character* Closer)
{
  if(Closer->GetIsPlayer())
    if(GetIsWalkable())
      if(RAND() % 20 < Closer->GetStrength())
	ADD_MESSAGE("You close the door.");
      else
	{
	  ADD_MESSAGE("The door resists!");
	  return false;
	}
    else
      {
	ADD_MESSAGE("The door is already closed, %s.", game::Insult());
	return false;
      }

  MakeNotWalkable();
  return true;
}

void altar::DrawToTileBuffer() const
{
  Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
  igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetOwnerGod() << 4, 0, 0, 0, 16, 16);
}

void altar::Load(inputfile& SaveFile)
{
  levelterrain::Load(SaveFile);

  SaveFile >> OwnerGod;
}

void altar::Save(outputfile& SaveFile) const
{
  levelterrain::Save(SaveFile);

  SaveFile << OwnerGod;
}

bool stairsup::GoUp(character* Who) const // Try to go up
{
  if(game::GetCurrent())
    {
      if(game::GetCurrent() == 9)
	if(!game::BoolQuestion("Somehow you get the feeling you cannot return. Continue anyway? [y/N]"))
	  return false;

      std::vector<character*> MonsterList;

      if(!GetLevelSquareUnder()->GetLevelUnder()->CollectCreatures(MonsterList, Who, true))
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
      if(configuration::GetAutosaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    {
      if(Who->GetIsPlayer())
	{
	  std::vector<character*> TempPlayerGroup;

	  if(!GetLevelSquareUnder()->GetLevelUnder()->CollectCreatures(TempPlayerGroup, Who, false))
	    return false;

	  game::GetCurrentArea()->RemoveCharacter(Who->GetPos());
	  game::GetCurrentDungeon()->SaveLevel();
	  game::LoadWorldMap();

	  game::GetWorldMap()->GetPlayerGroup().swap(TempPlayerGroup);

	  game::SetInWilderness(true);
	  game::GetCurrentArea()->AddCharacter(game::GetCurrentDungeon()->GetWorldMapPos(), Who);
	  game::SendLOSUpdateRequest();
	  game::UpdateCamera();
	  if(configuration::GetAutosaveInterval())
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

      if(!GetLevelSquareUnder()->GetLevelUnder()->CollectCreatures(MonsterList, Who, true))
	return false;

      if(game::GetCurrent() == 8)
	Who->GetLevelSquareUnder()->ChangeLevelTerrain(new parquet, new empty);

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
      game::UpdateCamera();
      game::SendLOSUpdateRequest();
      if(configuration::GetAutosaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    {
      if(Who->GetIsPlayer())
	ADD_MESSAGE("You are at the bottom.");

      return false;
    }
}

void door::Kick(ushort Strength, bool ShowOnScreen, uchar)
{
  if(!GetIsWalkable()) 
    {
      if(!IsLocked && Strength > RAND() % 20)
	{
	  if(ShowOnScreen) ADD_MESSAGE("The door opens.");
	  MakeWalkable();
	}
      else if(Strength > RAND() % 40)
	{
	  bool NewLockedStatus;

	  if(IsLocked && RAND() % 2) // _can't really think of a good formula for this... 
	    {			//Strength isn't everything
	      if(ShowOnScreen)
		ADD_MESSAGE("The lock breaks and the door is damaged.");

	      NewLockedStatus = false;
	    }
	  else
	    {
	      if(ShowOnScreen)
		ADD_MESSAGE("The door is damaged.");

	      NewLockedStatus = IsLocked;
	    }

	  brokendoor* Temp = new brokendoor(false);
	  Temp->InitMaterials(GetMaterial(0));
	  PreserveMaterial(0);
	  GetLevelSquareUnder()->ChangeOverLevelTerrain(Temp);
	  Temp->SetIsLocked(NewLockedStatus);
	}
      else
	if(ShowOnScreen)
	  ADD_MESSAGE("The door won't budge!");

      // The door may have been destroyed here, so don't do anything!
    }
}

void door::Save(outputfile& SaveFile) const
{
  levelterrain::Save(SaveFile);

  SaveFile << IsOpen;
  SaveFile << IsLocked;
}

void door::Load(inputfile& SaveFile)
{
  levelterrain::Load(SaveFile);

  SaveFile >> IsOpen;
  SaveFile >> IsLocked;
}

void door::MakeWalkable()
{
  IsOpen = true;

  UpdatePicture();

  GetLevelSquareUnder()->SendNewDrawRequest();
  GetLevelSquareUnder()->SendMemorizedUpdateRequest();
  GetLevelSquareUnder()->SetDescriptionChanged(true);
  GetLevelSquareUnder()->ForceEmitterEmitation();

  if(GetLevelSquareUnder()->GetLastSeen() == game::GetLOSTurns())
    game::SendLOSUpdateRequest();
}

void door::MakeNotWalkable()
{
  GetLevelSquareUnder()->ForceEmitterNoxify();

  IsOpen = false;

  UpdatePicture();

  GetLevelSquareUnder()->SendNewDrawRequest();
  GetLevelSquareUnder()->SendMemorizedUpdateRequest();
  GetLevelSquareUnder()->SetDescriptionChanged(true);
  GetLevelSquareUnder()->ForceEmitterEmitation();

  if(GetLevelSquareUnder()->GetLastSeen() == game::GetLOSTurns())
    game::SendLOSUpdateRequest();
}

void altar::StepOn(character* Stepper)
{
  if(Stepper->GetIsPlayer() && OwnerGod && !game::GetGod(OwnerGod)->GetKnown())
    {
      ADD_MESSAGE("The ancient altar is covered with strange markings. You manage to decipher them.");
      ADD_MESSAGE("The altar is dedicated to %s, the %s.", game::GetGod(OwnerGod)->Name().c_str(), game::GetGod(OwnerGod)->Description().c_str());
      ADD_MESSAGE("You now know the sacred rituals that allow you to contact this deity via prayers.");
      game::GetGod(OwnerGod)->SetKnown(true);
    }
}

bool throne::SitOn(character* Sitter)
{
  if(Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() != 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler.");
      ADD_MESSAGE("The daydream fades in a whisper: \"Thou shalt be a My Champion first!\"");
      return true;
    }

  if(Sitter->HasPetrussNut() && !Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler.");
      ADD_MESSAGE("The daydream fades in a whisper: \"Thou shalt wear My shining armor first!\"");
      return true;
    }

  if(!Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler.");
      ADD_MESSAGE("The daydream fades in a whisper: \"Thou shalt surpass thy predecessor first!\"");
      return true;
    }

  if(Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      iosystem::TextScreen("A heavenly choir starts to sing Grandis Rana and a booming voice fills the air:\n\n\"Mortal! Thou hast surpassed Petrus, and pleaseth Me greatly during thine adventures!\nI hereby title thee as My new High Priest!\"\n\nYou are victorious!");
      game::RemoveSaves();

      if(!game::GetWizardMode())
	{
	  game::GetPlayer()->AddScoreEntry("became the new High Priest of the Great Frog", 5, false);
	  highscore HScore;
	  HScore.Draw();
	}

      game::Quit();
      return true;
    }

  ADD_MESSAGE("You feel somehow out of place.");
  return true;
}

void altar::Kick(ushort, bool ShowOnScreen, uchar)
{
  // This function does not currently support AI kicking altars when they are in player's LOS

  if(ShowOnScreen)
    ADD_MESSAGE("You feel like a sinner.");

  game::GetGod(GetLevelSquareUnder()->GetDivineOwner())->PlayerKickedAltar();

  if(GetLevelSquareUnder()->GetDivineOwner() > 1)
    game::GetGod(GetLevelSquareUnder()->GetDivineOwner() - 1)->PlayerKickedFriendsAltar();

  if(GetLevelSquareUnder()->GetDivineOwner() < game::GetGodNumber())
    game::GetGod(GetLevelSquareUnder()->GetDivineOwner() + 1)->PlayerKickedFriendsAltar();
}

void altar::ReceiveVomit(character* Who)
{
  if(Who->GetIsPlayer())
    game::GetGod(GetLevelSquareUnder()->GetDivineOwner())->PlayerVomitedOnAltar();
}

std::string door::Name(uchar Case) const
{
  if(!(Case & PLURAL))
    if(!(Case & DEFINEBIT))
      return std::string(IsOpen ? "open" : "closed") + (IsLocked ? ", locked " : " ") + GetMaterial(0)->Name() + " " + NameSingular();
    else
      if(!(Case & INDEFINEBIT))
	return std::string(IsOpen ? "the open" : "the closed") + (IsLocked ? ", locked " : " ") + GetMaterial(0)->Name() + " " + NameSingular();
      else
	return std::string(IsOpen ? "an open" : "a closed") + (IsLocked ? ", locked " : " ") + GetMaterial(0)->Name() + " " + NameSingular();
  else
    if(!(Case & DEFINEBIT))
      return GetMaterial(0)->Name() + " " + NamePlural();
    else
      if(!(Case & INDEFINEBIT))
	return std::string("the ") + GetMaterial(0)->Name() + " " + NamePlural();
      else
	return GetMaterial(0)->Name() + " " + NamePlural();
}

bool couch::SitOn(character*)
{
  ADD_MESSAGE("The couch is extremely soft and confortable. You relax well.");
  return true;
}

bool pool::SitOn(character*)
{
  ADD_MESSAGE("You sit on the pool. Oddly enough, you sink. You feel stupid.");
  return true;
}

void stairsup::StepOn(character* Stepper)
{
  if(Stepper->GetIsPlayer()) 
    ADD_MESSAGE("There is stairway leading upwards here.");
}

void stairsdown::StepOn(character* Stepper)
{
  if(Stepper->GetIsPlayer()) 
    ADD_MESSAGE("There is stairway leading downwards here.");
}

bool bookcase::SitOn(character*)
{
  ADD_MESSAGE("The bookcase is very unconfortable to sit on.");
  return true;
}

bool fountain::SitOn(character* Char)
{
  if(GetMaterial(1))
    {
      ADD_MESSAGE("You sit on the fountain. Water falls on your head and you get quite wet. You feel like a moron.");
      return true;
    }
  else
    return overlevelterrain::SitOn(Char);
}

bool doublebed::SitOn(character*)
{
  ADD_MESSAGE("The beautiful bed is very soft. You get a feeling it's not meant for your kind of people.");
  return true;
}

bool fountain::Consume(character* Drinker)
{
  if(GetMaterial(1))
    {
      if(GetMaterial(1)->GetType() == water::StaticType()) 
	{
	  if(GetLevelSquareUnder()->GetRoom() && GetLevelSquareUnder()->GetLevelUnder()->GetRoom(GetLevelSquareUnder()->GetRoom())->HasDrinkHandler())
	    {
	      if(!GetLevelSquareUnder()->GetLevelUnder()->GetRoom(GetLevelSquareUnder()->GetRoom())->Drink(Drinker))
		return false;
	    }
	  else
	    {
	      if(!game::BoolQuestion("Do you still want to drink from the fountain? [y/N]"))
		{
		  return false;
		}
	    }

	  switch(RAND() % 5)
	    {
	    case 0:
	      ADD_MESSAGE("The water is contaminated!");
	      Drinker->SetNP(Drinker->GetNP() + 5);

	      if(!(RAND() % 5))
		Drinker->Polymorph(protosystem::CreateMonster(false), 2500 + RAND() % 2500);
	      else
		Drinker->ChangeRandomStat(-1);

	      break;
	
	    case 1:
	      ADD_MESSAGE("The water tasted very good.");
	      Drinker->SetNP(Drinker->GetNP() + 100);
	      Drinker->ChangeRandomStat(1);
	      break;

	    case 2:
	      if(!(RAND() % 10))
		{
		  ADD_MESSAGE("You have freed a spirit that grants you a wish. You may wish for an item. - press any key -");
		  game::DrawEverything();
		  GETKEY();

		  while(true)
		    {
		      std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(7,7), WHITE, 0, 80);
		      item* TempItem = protosystem::CreateItem(Temp, Drinker->GetIsPlayer());

		      if(TempItem)
			{
			  Drinker->GetStack()->AddItem(TempItem);
			  ADD_MESSAGE("%s appears from nothing and the spirit flies happily away!", TempItem->CNAME(INDEFINITE));
			  break;
			}
		      else
			{
			  ADD_MESSAGE("You may try again. - press any key -");
			  DRAW_MESSAGES();
			  game::DrawEverything();
			  GETKEY();
			}
		    }
		}

	    case 3:
	      DryOut();
	      break;

	    default:
	      ADD_MESSAGE("The water tastes good.");
	      Drinker->SetNP(Drinker->GetNP() + 25);
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
  ADD_MESSAGE("%s dries out.", CNAME(DEFINITE));
  delete GetMaterial(1);
  SetMaterial(1, 0);
  UpdatePicture();

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();

      GetSquareUnder()->SendNewDrawRequest();
      GetSquareUnder()->SendMemorizedUpdateRequest();

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorized();
    }
}

void brokendoor::Kick(ushort Strength, bool ShowOnScreen, uchar)
{
  if(!GetIsWalkable())
    if(IsLocked)
      {
	if(Strength > RAND() % 30)
	  {
	    if(ShowOnScreen)
	      ADD_MESSAGE("The doors opens from the force of your kick.");

	    IsLocked = false;
	    MakeWalkable();
	  }
	else if(Strength > RAND() % 20)
	  {
	    if(ShowOnScreen)
	      ADD_MESSAGE("The lock breaks from the force of your kick.");

	    IsLocked = false;
	  }
	else
	  if(ShowOnScreen)
	    ADD_MESSAGE("The door won't budge!");
      }
    else
      if(Strength > RAND() % 20)
	{
	  if(ShowOnScreen)
	    ADD_MESSAGE("The broken door opens.");

	  MakeWalkable();
	}
      else
	if(ShowOnScreen)
	  ADD_MESSAGE("The door resists.");
}

bool door::ReceiveStrike()
{
  if(RAND() % 2)
    {
      bool NewLockedStatus = IsLocked;

      if(GetLevelSquareUnder()->CanBeSeen())
	ADD_MESSAGE("The wand strikes the door and the door breaks.");

      brokendoor* Temp;
      GetLevelSquareUnder()->ChangeOverLevelTerrain(Temp = new brokendoor);
      Temp->SetIsLocked(NewLockedStatus);
    }
  else
    {
      if(GetLevelSquareUnder()->CanBeSeen())
	ADD_MESSAGE("The wand strikes the door and the door opens.");
		
      MakeWalkable();
      IsLocked = false;
    }

  return true;
}

bool brokendoor::ReceiveStrike()
{
  if(RAND() % 2)
    {
      MakeWalkable();
      IsLocked = false;
      if(GetLevelSquareUnder()->CanBeSeen())
	ADD_MESSAGE("The wand strikes the door and the door opens.");
    }
  else
    ADD_MESSAGE("The wand strikes the door, but the door won't budge.");

  return true;
}

bool altar::Polymorph(character*)
{
  if(GetSquareUnder()->CanBeSeen())
    ADD_MESSAGE("%s glows briefly.", CNAME(DEFINITE));
	
  uchar OldGod = OwnerGod;

  while(OwnerGod == OldGod)
    OwnerGod = RAND() % game::GetGodNumber() + 1;

  GetSquareUnder()->SendNewDrawRequest();
  GetSquareUnder()->SendMemorizedUpdateRequest();
  GetSquareUnder()->SetDescriptionChanged(true);

  if(GetSquareUnder()->CanBeSeen())
    {
      GetSquareUnder()->UpdateMemorizedDescription();
      GetSquareUnder()->UpdateMemorized();
    }

  return true;	
}

bool altar::SitOn(character*)
{
  ADD_MESSAGE("You kneel down and worship %s for a moment.", game::GetGod(OwnerGod)->GOD_NAME);

  if(game::GetGod(OwnerGod)->GetRelation() < 500)
    {
      if(!(RAND() % 20))
	{
	  game::GetGod(OwnerGod)->AdjustRelation(2);
	  game::ApplyDivineAlignmentBonuses(game::GetGod(OwnerGod), true, 1);
	}
    }
  else
    if(!(RAND() % 2500))
      {
	character* Angel = game::GetGod(OwnerGod)->CreateAngel();

	if(Angel)
	  {
	    Angel->SetTeam(game::GetPlayer()->GetTeam());
	    ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CNAME(DEFINITE));
	  }

	game::GetGod(OwnerGod)->AdjustRelation(50);
	game::ApplyDivineAlignmentBonuses(game::GetGod(OwnerGod), true);
      }

  return true;
}

bool pool::GetIsWalkable(character* ByWho) const
{
  return ByWho && (ByWho->CanSwim() || ByWho->CanFly());
}

std::string fountain::Name(uchar Case) const
{
  if(GetMaterial(1))
    return NameWithMaterial(Case, 1);
  else
    return NameNormal(Case, "a", "dried out ");
}
