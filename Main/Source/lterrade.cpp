#define __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "lterraba.h"

std::vector<glterrain::prototype*> protocontainer<glterrain>::ProtoData;
std::vector<olterrain::prototype*> protocontainer<olterrain>::ProtoData;
valuemap protocontainer<glterrain>::CodeNameMap;
valuemap protocontainer<olterrain>::CodeNameMap;

LTERRAIN_PROTOTYPE(glterrain, 0, glterrain);
LTERRAIN_PROTOTYPE(olterrain, 0, olterrain);

#include "femath.h"
#include "lterrade.h"

#undef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__

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
#include "festring.h"
#include "charde.h"

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
      else if(RAND() % 20 < Opener->GetAttribute(ARM_STRENGTH))
	{
	  MakeWalkable();

	  if(Opener->IsPlayer())
	    ADD_MESSAGE("You open the door.");
	  else if(CanBeSeenByPlayer())
	    {
	      if(Opener->CanBeSeenByPlayer())
		ADD_MESSAGE("%s opens the door.", Opener->CHAR_NAME(DEFINITE));
	      else
		ADD_MESSAGE("Something opens the door.");
	    }
	}
      else
	{
	  if(Opener->IsPlayer())
	    ADD_MESSAGE("The door resists.");
	  else if(CanBeSeenByPlayer() && Opener->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s fails to open the door.", Opener->CHAR_NAME(DEFINITE));

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
	if(RAND() % 20 < Closer->GetAttribute(ARM_STRENGTH))
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

void altar::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  olterrain::Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
  igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, GetConfig() << 4, 0, Pos, 16, 16, Luminance);
}

void door::BeKicked(character* Kicker, ushort KickDamage)
{
  if(!IsWalkable()) 
    {
      room* Room = GetLSquareUnder()->GetRoomClass();

      if(Room)
	Room->DestroyTerrain(Kicker, this);

      if(!IsLocked() && KickDamage > (RAND() & 3))
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The door opens.");

	  MakeWalkable();
	  return;
	}

      if(KickDamage <= GetStrengthValue())
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("Your weak kick has no chance to affect this door.");
	  
	  return;
	}

      EditHP(GetStrengthValue() - KickDamage);
      ushort SV = Max<ushort>(GetStrengthValue(), 1);
      bool LockBreaks = IsLocked() && RAND() % (100 * KickDamage / SV) >= 100;

      if(LockBreaks)
	SetIsLocked(false);

      if(GetHP() <= 0)
	{
	  if(CanBeSeenByPlayer())
	    {
	      if(LockBreaks)
		ADD_MESSAGE("The lock breaks and the door is damaged.");
	      else
		ADD_MESSAGE("The door is damaged.");
	    }

	  Break();
	}
      else if(CanBeSeenByPlayer())
	{
	  if(LockBreaks)
	    ADD_MESSAGE("The lock breaks.");
	  else
	    ADD_MESSAGE("The door won't budge!");
	}

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
  GetLSquareUnder()->ForceEmitterEmitation();
  GetLSquareUnder()->CalculateLuminance();

  if(GetLSquareUnder()->GetLastSeen() == game::GetLOSTurns())
    game::SendLOSUpdateRequest();

  ActivateBoobyTrap();
}

void door::MakeNotWalkable()
{
  GetLSquareUnder()->ForceEmitterNoxify();
  SetIsOpened(false);
  UpdatePictures();
  GetLSquareUnder()->SendNewDrawRequest();
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  GetLSquareUnder()->ForceEmitterEmitation();
  GetLSquareUnder()->CalculateLuminance();

  if(GetLSquareUnder()->GetLastSeen() == game::GetLOSTurns())
    game::SendLOSUpdateRequest();
}

void altar::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer() && !GetMasterGod()->GetKnown())
    {
      ADD_MESSAGE("The ancient altar is covered with strange markings. You manage to decipher them. The altar is dedicated to %s, the %s. You now know the sacred rituals that allow you to contact this deity via prayers.", GetMasterGod()->Name().c_str(), GetMasterGod()->Description().c_str());
      GetMasterGod()->SetKnown(true);
    }
}

bool throne::SitOn(character* Sitter)
{
  Sitter->EditAP(-1000);

  if(Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() != 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler. The daydream fades in a whisper: \"Thou shalt be a Our Champion first!\"");
      return true;
    }

  if(Sitter->HasPetrussNut() && !Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler. The daydream fades in a whisper: \"Thou shalt wear Our shining armor first!\"");
      return true;
    }

  if(!Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      ADD_MESSAGE("You have a strange vision of yourself becoming great ruler. The daydream fades in a whisper: \"Thou shalt surpass thy predecessor first!\"");
      return true;
    }

  if(Sitter->HasPetrussNut() && Sitter->HasGoldenEagleShirt() && game::GetGod(1)->GetRelation() == 1000)
    {
      game::TextScreen("A heavenly choir starts to sing Grandis Rana and a booming voice fills the air:\n\n\"Mortal! Thou hast surpassed Petrus, and pleased Us greatly during thy adventures!\nWe hereby title thee as Our new high priest!\"\n\nYou are victorious!");
      game::GetPlayer()->AddScoreEntry("became the new high priest of the Great Frog", 5, false);
      game::End();
      return true;
    }

  ADD_MESSAGE("You feel somehow out of place.");
  return true;
}

void altar::BeKicked(character* Kicker, ushort)
{
  room* Room = GetLSquareUnder()->GetRoomClass();

  if(Room)
    Room->DestroyTerrain(Kicker, this);

  if(Kicker->IsPlayer())
    ADD_MESSAGE("You feel like a sinner.");
  else if(Kicker->CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks like a sinner.", Kicker->CHAR_NAME(DEFINITE));

  if(Kicker->IsPlayer())
    {
      GetMasterGod()->PlayerKickedAltar();

      if(GetConfig() > 1)
	game::GetGod(GetConfig() - 1)->PlayerKickedFriendsAltar();

      if(GetConfig() < game::GetGods() - 1)
	game::GetGod(GetConfig() + 1)->PlayerKickedFriendsAltar();
    }
}

void altar::ReceiveVomit(character* Who)
{
  if(Who->IsPlayer())
    GetMasterGod()->PlayerVomitedOnAltar();
}

bool door::AddAdjective(std::string& String, bool Articled) const
{
  if(Articled)
    String << (Opened ? "an open" : "a closed");
  else
    String << (Opened ? "open" : "closed");

  if(IsLocked())
    String << ", locked ";
  else
    String << " ";

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

bool fountain::Drink(character* Drinker)
{
  if(GetContainedMaterial())
    {
      if(GetContainedMaterial()->GetConfig() == WATER) 
	{
	  if(GetLSquareUnder()->GetRoom() && GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->HasDrinkHandler())
	    {
	      if(!GetLevelUnder()->GetRoom(GetLSquareUnder()->GetRoom())->Drink(Drinker))
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

	  switch(RAND() % 9)
	    {
	    case 0:
	      ADD_MESSAGE("The water is contaminated!");
	      Drinker->EditNP(50);

	      if(!(RAND() % 5))
		Drinker->PolymorphRandomly(0, 10000, 2500 + RAND() % 2500);
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
			  ADD_MESSAGE("%s appears from nothing and the spirit flies happily away!", TempItem->CHAR_NAME(INDEFINITE));
			  break;
			}
		    }
		}
	      else
		DryOut();

	      break;

	    case 4:
	      if(RAND() % 10)
		{
		  ADD_MESSAGE("The water tastes normal, but there is an odd after taste.");
		  Drinker->ActivateRandomState(2000 + RAND() % 3000);
		}
	      else
		{
		  ADD_MESSAGE("This water tastes very odd.");

		  if(!Drinker->GainRandomInstric())
		    ADD_MESSAGE("You feel like a penguin."); /* This is rather rare, so no harm done */
		}
	      break;

	    case 5:
	      {
		bool Created = false;
		character* Monster = 0;

		switch(RAND() % 4)
		  {
		  case 0:
		    Monster = new snake;
		    break;
		  case 1:
		    Monster = new mommo(RAND() & 1 ? CONICAL : FLAT);
		    break;
		  case 2:
		    Monster = new spider;
		    break;
		  case 3:
		    Monster = new frog(RAND() % 5 ? DARK : GREATER_DARK);
		    break;
		  }

		for(ushort p = 0; p < 10; ++p)
		  {
		    vector2d TryToCreate = Drinker->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

		    if(GetLevelUnder()->IsValidPos(TryToCreate) && GetNearLSquare(TryToCreate)->IsWalkable(Monster) && !GetNearLSquare(TryToCreate)->GetCharacter())
		      {
			Created = true;
			GetNearLSquare(TryToCreate)->AddCharacter(Monster);
			Monster->SetTeam(game::GetTeam(MONSTER_TEAM));

			if(Monster->CanBeSeenByPlayer())
			  ADD_MESSAGE("%s appears from the fountain!", Monster->CHAR_NAME(DEFINITE));

			break;
		      }
		  }

		if(!Created)
		  {
		    ADD_MESSAGE("Weird water...");
		    delete Monster;
		  }

		break;
	      }
	    case 6:
	      if(!(RAND() % 5))
		{
		  item* ToBeCreated = protosystem::BalancedCreateItem(0, MAX_PRICE, RING);
		  GetLSquareUnder()->GetStack()->AddItem(ToBeCreated);

		  if(ToBeCreated->CanBeSeenByPlayer())
		    ADD_MESSAGE("There's something sparkling in the water.");

		  break;
		}

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
  ADD_MESSAGE("%s dries out.", CHAR_NAME(DEFINITE));
  ChangeContainedMaterial(0);

  if(GetLSquareUnder())
    {
      GetLSquareUnder()->SendNewDrawRequest();
      GetLSquareUnder()->SendMemorizedUpdateRequest();
    }
}

void brokendoor::BeKicked(character* Kicker, ushort KickDamage)
{
  if(!IsWalkable()) 
    {
      room* Room = GetLSquareUnder()->GetRoomClass();

      if(Room)
	Room->DestroyTerrain(Kicker, this);

      if(!IsLocked() && KickDamage > (RAND() & 3))
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The broken door opens.");

	  MakeWalkable();
	  return;
	}

      if(IsLocked())
	{
	  ushort SV = Max<ushort>(GetStrengthValue(), 1);

	  if(KickDamage > SV && RAND() % (100 * KickDamage / SV) >= 100)
	    {
	      if(RAND() & 1)
		{
		  if(CanBeSeenByPlayer())
		    ADD_MESSAGE("The broken door opens from the force of your kick.");

		  MakeWalkable();
		}
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("The lock breaks from the force of your kick.");

	      SetIsLocked(false);
	      return;
	    }
	}

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The broken door won't budge!");
    }
}

bool altar::Polymorph(character*)
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s glows briefly.", CHAR_NAME(DEFINITE));
	
  uchar OldGod = GetConfig(), NewGod = GetConfig();

  while(NewGod == OldGod)
    NewGod = 1 + RAND() % (game::GetGods() - 1);

  SetConfig(NewGod);

  GetLSquareUnder()->SendNewDrawRequest();
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  return true;	
}

bool altar::SitOn(character* Sitter)
{
  ADD_MESSAGE("You kneel down and worship %s for a moment.", GetMasterGod()->GOD_NAME);

  if(GetMasterGod()->GetRelation() < 500)
    {
      if(!(RAND() % 20))
	{
	  GetMasterGod()->AdjustRelation(2);
	  game::ApplyDivineAlignmentBonuses(GetMasterGod(), true, 1);
	}
    }
  else
    if(!(RAND() % 2500))
      {
	character* Angel = GetMasterGod()->CreateAngel();

	if(Angel)
	  {
	    Angel->SetTeam(game::GetPlayer()->GetTeam());
	    ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
	  }

	GetMasterGod()->AdjustRelation(50);
	game::ApplyDivineAlignmentBonuses(GetMasterGod(), true);
      }

  Sitter->EditAP(-1000);
  return true;
}

bool liquidterrain::IsWalkable(const character* ByWho) const
{
  return ByWho && (ByWho->CanSwim() || ByWho->CanFly());
}

void door::Break()
{
  if(BoobyTrap)
    ActivateBoobyTrap();
  else
    {
      bool Open = Opened;
      brokendoor* Temp = new brokendoor(0, NO_MATERIALS);
      Temp->InitMaterials(GetMainMaterial());
      Temp->SetIsLocked(IsLocked());
      Temp->SetBoobyTrap(0);
      Temp->SetLockType(GetLockType());
      SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
      GetLSquareUnder()->ChangeOLTerrainAndUpdateLights(Temp);

      if(Open)
	Temp->MakeWalkable();
      else 
	Temp->MakeNotWalkable();
    }
}

void door::ActivateBoobyTrap()
{
  switch(BoobyTrap)
    {
    case 1:
      // Explosion
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s is booby trapped!", CHAR_NAME(DEFINITE));

      BoobyTrap = 0;
      GetLevelUnder()->Explosion(0, "killed by an exploding booby trapped door", GetPos(), 20 + RAND() % 5 - RAND() % 5);
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

material*& fountain::GetMaterialReference(ushort Index)
{
  switch(Index)
    {
    case 0: return MainMaterial;
    default: return ContainedMaterial;
    }
}

ushort fountain::GetMaterialColorB(ushort) const
{
  if(GetContainedMaterial())
    return GetContainedMaterial()->GetColor();
  else
    return 0;
}

uchar fountain::GetAlphaB(ushort) const
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
      SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
    }
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
	    ADD_MESSAGE("%s unlocks the door.", Applier->CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s locks the door.", Applier->CHAR_NAME(DEFINITE));
	}

      SetIsLocked(!IsLocked());
      return true;
    }
  else
    {
      if(Applier->IsPlayer())
	ADD_MESSAGE("%s doesn't fit into the lock.", Thingy->CHAR_NAME(DEFINITE));

      return false;
    }
}

void fountain::GenerateMaterials()
{
  ushort Chosen = RandomizeMaterialConfiguration();
  InitChosenMaterial(MainMaterial, GetMainMaterialConfig(), GetDefaultMainVolume(), Chosen);
  InitChosenMaterial(ContainedMaterial, GetContainedMaterialConfig(), GetDefaultContainedVolume(), Chosen);
}

bool fountain::AddAdjective(std::string& String, bool Articled) const
{
  if(!GetContainedMaterial())
    {
      String += Articled ? "a dried out" : "dried out";
      return true;
    }
  else
    return false;
}

void liquidterrain::VirtualConstructor(bool Load) // gum solution
{
  glterrain::VirtualConstructor(Load);
  SetAnimationFrames(16);
}

fountain::~fountain()
{
  delete ContainedMaterial;
}

bool fountain::IsSparkling(ushort ColorIndex) const
{
  return (ColorIndex == 0 && MainMaterial->IsSparkling()) || (ColorIndex == 1 && ContainedMaterial && ContainedMaterial->IsSparkling());
}

void link::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << AttachedArea << AttachedEntry;
}

void link::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  SaveFile >> AttachedArea >> AttachedEntry;
}

bool link::Enter(bool DirectionUp) const
{
  if(DirectionUp != IsUpLink())
    return olterrain::Enter(DirectionUp);

  /* "Temporary" gum solutions */

  if(Config == OREE_LAIR_ENTRY)
    {
      ADD_MESSAGE("You sense terrible evil trembling very near under your feet. You feel you shouldn't wander any further. On the other hand you have little choice.");

      if(!game::BoolQuestion("Continue? [y/N]"))
	return false;
    }

  if(Config == OREE_LAIR_EXIT)
    if(game::GetPlayer()->HasGoldenEagleShirt())
      {
	ADD_MESSAGE("Somehow you get the feeling you cannot return.");

	if(!game::BoolQuestion("Continue anyway? [y/N]"))
	  return false;
      }
    else
      {
	ADD_MESSAGE("An unknown magical force pushes you back.");
	return true;
      }

  if(Config == DARK_LEVEL)
    {
      ADD_MESSAGE("This dark gate seems to be a one-way portal. You sense something distant but extremely dangerous on the other side. You feel you should think twice before entering.");

      if(!game::BoolQuestion("Continue? [y/N]"))
	return false;
    }

  /* These must be backupped, since LeaveLevel destroys the link */

  uchar AttachedArea = GetAttachedArea();
  uchar AttachedEntry = GetAttachedEntry();

  std::vector<character*> Group;

  if(game::LeaveLevel(Group, AttachedArea != WORLD_MAP))
    {
      game::EnterArea(Group, AttachedArea, AttachedEntry);
      return true;
    }
  else
    return false;
}

void link::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer()) 
    ADD_MESSAGE("There is %s here.", CHAR_NAME(INDEFINITE));
}

void link::VirtualConstructor(bool Load)
{
  olterrain::VirtualConstructor(Load);

  if(!Load)
    if(Config == STAIRS_UP)
      {
	if(game::GetCurrent())
	  {
	    AttachedArea = game::GetCurrent() - 1;
	    AttachedEntry = STAIRS_DOWN;
	  }
	else
	  {
	    AttachedArea = WORLD_MAP;
	    AttachedEntry = game::GetCurrentDungeon()->GetIndex();
	  }
      }
    else if(Config == STAIRS_DOWN)
      {
	AttachedArea = game::GetCurrent() + 1;
	AttachedEntry = STAIRS_UP;
      }
}

void boulder::Break()
{
  /* If this used the volume of the boulder the pieces would be huge or there would be just hundreds of them so I jsut use normal sized stones and just a few of them too... */
  ushort HowManyParts = RAND() % 5 + 1;

  for(ushort c = 0; c < HowManyParts; ++c)
    {
      material* StonesMaterial = GetMainMaterial()->Clone();
      StonesMaterial->SetVolume(1000);
      item* Stone = new stone(0, NO_MATERIALS);
      Stone->InitMaterials(StonesMaterial);
      GetLSquareUnder()->GetStack()->AddItem(Stone);
      
    }
  olterrain::Break();
}

void sign::AddPostFix(std::string& String) const
{
  String << " with text \"" << Text << "\"";
}

void sign::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer())
    ADD_MESSAGE("There's a sign here saying: \"%s\"", Text.c_str());
}

void sign::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << Text;
}

void sign::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  SaveFile >> Text;
}

void olterraincontainer::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  Contained->Save(SaveFile);
}

void olterraincontainer::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  Contained->Load(SaveFile);
}

void olterraincontainer::VirtualConstructor(bool Load)
{
  olterrain::VirtualConstructor(Load);
  Contained = new stack(0, this, HIDDEN, true);
}

bool olterraincontainer::Open(character* Opener)
{
  std::string Question = "Do you want to (t)ake something from or (p)ut something in this container? [t,p]";
  bool Success;

  switch(game::KeyQuestion(Question, KEY_ESC, 3, 't', 'p', KEY_ESC))
    {
    case 't':
      Success = GetContained()->TakeSomethingFrom(Opener, GetName(DEFINITE));
      break;
    case 'p':
      Success = GetContained()->PutSomethingIn(Opener, GetName(DEFINITE), GetStorageVolume(), 0);
      break;
    default:
      return false;
    }

  if(Success)
    Opener->DexterityAction(Opener->OpenMultiplier() * 5);

  return Success;
}

void olterraincontainer::SetItemsInside(const std::vector<contentscript<item> >& ItemVector, ushort SpecialFlags)
{
  GetContained()->Clean();

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c].IsValid())
      GetContained()->AddItem(ItemVector[c].Instantiate(SpecialFlags));
}

void wall::Break()
{
  ushort DigProduct = GetMainMaterial()->GetDigProductMaterial();

  if(DigProduct)
    {
      ushort HowManyParts = 1 + (RAND() & 3);

      for(ushort c = 0; c < HowManyParts; ++c)
	{
	  item* Stone = new stone(0, NO_MATERIALS);
	  Stone->InitMaterials(MAKE_MATERIAL(DigProduct, 1000));
	  GetLSquareUnder()->GetStack()->AddItem(Stone);
	}
    }

  olterrain::Break();
}

void door::ReceiveDamage(character* Villain, ushort Damage, uchar)
{
  if(!IsWalkable() && !IsLocked() && Damage > (RAND() & 3))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The door opens.");
		
      MakeWalkable();
      return;
    }

  if(CanBeDestroyed() && Damage > GetStrengthValue())
    {
      EditHP(GetStrengthValue() - Damage);
      ushort SV = Max<ushort>(GetStrengthValue(), 1);
      bool LockBreaks = IsLocked() && RAND() % (100 * Damage / SV) >= 100;

      if(LockBreaks)
	SetIsLocked(false);

      if(LockBreaks || HP <= 0)
	{
	  room* Room = GetLSquareUnder()->GetRoomClass();

	  if(Room)
	    Room->DestroyTerrain(Villain, this);
	}

      if(HP <= 0)
	{
	  if(CanBeSeenByPlayer())
	    if(LockBreaks)
	      ADD_MESSAGE("The door breaks and its lock is destroyed.");
	    else
	      ADD_MESSAGE("The door breaks.");

	  Break();
	}
      else if(LockBreaks && CanBeSeenByPlayer())
	ADD_MESSAGE("The door's lock is shattered.");
    }
}

void brokendoor::ReceiveDamage(character* Villain, ushort Damage, uchar)
{
  if(!IsWalkable() && !IsLocked() && Damage > (RAND() & 3))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The broken door opens.");
		
      MakeWalkable();
      return;
    }

  if(CanBeDestroyed() && Damage > GetStrengthValue())
    {
      EditHP(GetStrengthValue() - Damage);
      ushort SV = Max<ushort>(GetStrengthValue(), 1);
      bool LockBreaks = IsLocked() && RAND() % (100 * Damage / SV) >= 100;

      if(LockBreaks)
	SetIsLocked(false);

      if(LockBreaks || HP <= 0)
	{
	  room* Room = GetLSquareUnder()->GetRoomClass();

	  if(Room)
	    Room->DestroyTerrain(Villain, this);
	}

      if(HP <= 0)
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The broken door is completely destroyed.");

	  Break();
	}
      else if(LockBreaks && CanBeSeenByPlayer())
	ADD_MESSAGE("The broken door's lock is shattered.");
    }
}

olterraincontainer::~olterraincontainer()
{
  delete Contained;
}

void olterraincontainer::Break()
{
  GetContained()->MoveItemsTo(GetLSquareUnder()->GetStack());
  olterrain::Break();
}
