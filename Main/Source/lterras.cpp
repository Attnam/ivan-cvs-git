/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

/* Compiled through levelset.cpp */

bool door::CanBeOpenedByAI() { return !IsLocked() && CanBeOpened(); }
void door::HasBeenHitByItem(character* Thrower, item*, int Damage) { ReceiveDamage(Thrower, Damage, PHYSICAL_DAMAGE); }
vector2d door::GetBitmapPos(int Frame) const { return Opened ? GetOpenBitmapPos(Frame) : olterrain::GetBitmapPos(Frame); }
int door::GetTheoreticalWalkability() const { return ANY_MOVE; }

vector2d portal::GetBitmapPos(int Frame) const { return vector2d(16 + (((Frame & 31) << 3)&~8), 0); } // gum solution, should come from script
vector2d monsterportal::GetBitmapPos(int Frame) const { return vector2d(16 + (((Frame & 31) << 3)&~8), 0); } // gum solution, should come from script

void fountain::SetSecondaryMaterial(material* What, int SpecialFlags) { SetMaterial(SecondaryMaterial, What, 0, SpecialFlags); }
void fountain::ChangeSecondaryMaterial(material* What, int SpecialFlags) { ChangeMaterial(SecondaryMaterial, What, 0, SpecialFlags); }
void fountain::InitMaterials(material* M1, material* M2, bool CUP) { ObjectInitMaterials(MainMaterial, M1, 0, SecondaryMaterial, M2, 0, CUP); }
vector2d fountain::GetBitmapPos(int) const { return vector2d(GetSecondaryMaterial() ? 16 : 32, 288); }
void fountain::InitMaterials(const materialscript* M, const materialscript* C, bool CUP) { InitMaterials(M->Instantiate(), C->Instantiate(), CUP); }

void brokendoor::HasBeenHitByItem(character* Thrower, item*, int Damage) { ReceiveDamage(Thrower, Damage, PHYSICAL_DAMAGE); }

const char* liquidterrain::SurviveMessage() const { return "you manage to get out of the pool"; }
const char* liquidterrain::MonsterSurviveMessage() const { return "manages to get out of the pool"; }
const char* liquidterrain::DeathMessage() const { return "you drown"; }
const char* liquidterrain::MonsterDeathVerb() const { return "drowns"; }
const char* liquidterrain::ScoreEntry() const { return "drowned"; }
vector2d liquidterrain::GetBitmapPos(int Frame) const { return vector2d(48 + ((Frame << 3)&~8), 0); } // gum solution, should come from script

festring sign::GetText() const { return Text; }

bool door::Open(character* Opener)
{
  if(!Opened)
    {
      if(IsLocked())
	{
	  if(Opener->IsPlayer())
	    ADD_MESSAGE("The door is locked.");
	  
	  return false;
	}
      else if(RAND() % 20 < Opener->GetAttribute(ARM_STRENGTH))
	{
	  bool WasSeenByPlayer = CanBeSeenByPlayer(); // MakeWalkable() might destroy the door
	  MakeWalkable();

	  if(Opener->IsPlayer())
	    ADD_MESSAGE("You open the door.");
	  else if(WasSeenByPlayer)
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
      if(Opener->IsPlayer())
	ADD_MESSAGE("The door is already open, %s.", game::Insult());

      return false;
    }
}

bool door::Close(character* Closer)
{
  if(Closer->IsPlayer())
    if(Opened)
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

void altar::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance, int SquareIndex, bool AllowAnimate) const
{
  olterrain::Draw(Bitmap, Pos, Luminance, SquareIndex, AllowAnimate);
  igraph::GetSymbolGraphic()->LuminanceMaskedBlit(Bitmap, GetConfig() << 4, 0, Pos, 16, 16, Luminance);
}

void door::BeKicked(character* Kicker, int KickDamage, int)
{
  if(!Opened) 
    {
      if(!IsLocked() && KickDamage > (RAND() & 3))
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The door opens.");

	  MakeWalkable();
	  return;
	}

      if(KickDamage <= GetStrengthValue())
	{
	  if(CanBeSeenByPlayer() && (Kicker->CanBeSeenByPlayer() || Kicker->IsPlayer()))
	    ADD_MESSAGE("%s weak kick has no chance to affect this door.", Kicker->CHAR_POSSESSIVE_PRONOUN);
	  
	  return;
	}

      EditHP(GetStrengthValue() - KickDamage);
      int SV = Max(GetStrengthValue(), 1);
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

	  room* Room = GetRoom();
	  Break();

	  if(Room)
	    Room->DestroyTerrain(Kicker);
	}
      else if(CanBeSeenByPlayer())
	{
	  if(LockBreaks)
	    {
	      ADD_MESSAGE("The lock breaks.");

	      if(GetRoom())
		GetRoom()->DestroyTerrain(Kicker);
	    }
	  else if(Kicker->CanBeSeenByPlayer() || Kicker->IsPlayer())
	    ADD_MESSAGE("The door won't budge!");
	}

      // The door may have been destroyed here, so don't do anything!
    }
}

void door::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << Opened << Locked << BoobyTrap;
}

void door::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  SaveFile >> Opened >> Locked >> BoobyTrap;
}

void door::MakeWalkable()
{
  SetIsOpened(true);
  UpdatePictures();
  GetLSquareUnder()->SendNewDrawRequest();
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  GetLSquareUnder()->CalculateIsTransparent();
  GetLevel()->ForceEmitterEmitation(GetLSquareUnder()->GetEmitter(),
				    GetLSquareUnder()->GetSunEmitter());
  GetLSquareUnder()->CalculateLuminance();

  if(GetLSquareUnder()->GetLastSeen() == game::GetLOSTick())
    game::SendLOSUpdateRequest();

  ActivateBoobyTrap();
}

void door::MakeNotWalkable()
{
  emittervector EmitterBackup = GetLSquareUnder()->GetEmitter();
  GetLevel()->ForceEmitterNoxify(EmitterBackup);
  SetIsOpened(false);
  UpdatePictures();
  GetLSquareUnder()->SendNewDrawRequest();
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  GetLSquareUnder()->CalculateIsTransparent();
  GetLevel()->ForceEmitterEmitation(EmitterBackup,
				    GetLSquareUnder()->GetSunEmitter(),
				    FORCE_ADD);
  GetLSquareUnder()->CalculateLuminance();

  if(GetLSquareUnder()->GetLastSeen() == game::GetLOSTick())
    game::SendLOSUpdateRequest();
}

void altar::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer() && !GetMasterGod()->IsKnown())
    {
      ADD_MESSAGE("The ancient altar is covered with strange markings. You manage to decipher them. The altar is dedicated to %s, the %s. You now know the sacred rituals that allow you to contact this deity via prayers.", GetMasterGod()->GetName(), GetMasterGod()->GetDescription());
      GetMasterGod()->SetIsKnown(true);
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
      game::TextScreen(CONST_S("A heavenly choir starts to sing Grandis Rana and a booming voice fills the air:\n\n\"Mortal! Thou hast surpassed Petrus, and pleased Us greatly during thy adventures!\nWe hereby title thee as Our new high priest!\"\n\nYou are victorious!"));
      game::GetCurrentArea()->SendNewDrawRequest();
      game::DrawEverything();
      PLAYER->ShowAdventureInfo();
      festring Msg = CONST_S("became the new high priest of the Great Frog");
      PLAYER->AddScoreEntry(Msg, 5, false);
      game::End(Msg);
      return true;
    }

  ADD_MESSAGE("You feel somehow out of place.");
  return true;
}

void altar::BeKicked(character* Kicker, int, int)
{
  if(Kicker->IsPlayer())
    ADD_MESSAGE("You feel like a sinner.");
  else if(Kicker->CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks like a sinner.", Kicker->CHAR_NAME(DEFINITE));

  if(GetRoom())
    GetRoom()->DestroyTerrain(Kicker);

  if(Kicker->IsPlayer())
    {
      GetMasterGod()->PlayerKickedAltar();

      if(GetConfig() > 1)
	game::GetGod(GetConfig() - 1)->PlayerKickedFriendsAltar();

      if(GetConfig() < GODS)
	game::GetGod(GetConfig() + 1)->PlayerKickedFriendsAltar();
    }
}

bool altar::ReceiveVomit(character* Who, liquid* Liquid)
{
  if(Who->IsPlayer())
    {
      if(GetRoom())
	GetRoom()->HostileAction(Who);

      return GetMasterGod()->PlayerVomitedOnAltar(Liquid);
    }
  else
    return false;
}

bool door::AddAdjective(festring& String, bool Articled) const
{
  if(olterrain::AddAdjective(String, Articled))
    Articled = false;

  if(Articled)
    String << (Opened ? "an open" : "a closed");
  else
    String << (Opened ? "open" : "closed");

  if(IsLocked())
    String << " locked ";
  else
    String << ' ';

  return true;
}

bool fountain::SitOn(character* Sitter)
{
  if(GetSecondaryMaterial())
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
  if(GetSecondaryMaterial())
    {
      if(GetSecondaryMaterial()->GetConfig() == WATER) 
	{
	  room* Room = GetRoom();

	  if(Room && Room->HasDrinkHandler() && !Room->Drink(Drinker))
	    return false;
	  else
	    {
	      if(!game::BoolQuestion(CONST_S("Do you want to drink from the fountain? [y/N]")))
		return false;
	    }

	  Drinker->EditAP(-1000);

	  switch(RAND() % 10)
	    {
	    case 0:
	      ADD_MESSAGE("The water is contaminated!");
	      Drinker->EditNP(100);

	      if(!(RAND() % 5))
		Drinker->PolymorphRandomly(0, 1000000, 2500 + RAND() % 2500);
	      else
		Drinker->ChangeRandomAttribute(-1);

	      break;
	
	    case 1:
	      ADD_MESSAGE("The water tasted very good.");
	      Drinker->EditNP(2500);
	      Drinker->ChangeRandomAttribute(1);
	      break;

	    case 2:
	      if(!(RAND() % 15))
		{
		  ADD_MESSAGE("You have freed a spirit that grants you a wish. You may wish for an item.");
		  game::Wish(Drinker,
			     "%s appears from nothing and the spirit flies happily away!",
			     "Two %s appear from nothing and the spirit flies happily away!");
		}
	      else
		DryOut();

	      break;

	    case 4:
	      if(RAND() & 7)
		{
		  ADD_MESSAGE("The water tastes normal, but there is an odd after taste.");
		  Drinker->ActivateRandomState(SRC_FOUNTAIN, 10000 + RAND() % 20000);
		}
	      else
		{
		  ADD_MESSAGE("This water tastes very odd.");

		  if(!Drinker->GainRandomIntrinsic(SRC_FOUNTAIN))
		    ADD_MESSAGE("You feel like a penguin."); /* This is rather rare, so no harm done */
		}
	      break;

	    case 5:
	      {
		bool Created = false;
		character* Monster = 0;

		switch(RAND_N(5))
		  {
		  case 0:
		    Monster = new snake;
		    break;
		  case 1:
		    Monster = new mommo(RAND_2 ? CONICAL : FLAT);
		    break;
		  case 2:
		    Monster = new spider;
		    break;
		  case 3:
		    if(!RAND_N(50))
		      Monster = new dolphin;
		    else
		      Monster = new frog(RAND_N(5) ? DARK : RAND_N(5) ? GREATER_DARK : GIANT_DARK);

		    break;
		  case 4:
		    Monster = new largerat;
		    break;
		  }

		for(int p = 0; p < 10; ++p)
		  {
		    vector2d TryToCreate = Drinker->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

		    if(GetLevel()->IsValidPos(TryToCreate)
		    && GetNearLSquare(TryToCreate)->IsFlyable()
		    && !GetNearLSquare(TryToCreate)->GetCharacter())//Monster->CanMoveOn(GetNearLSquare(TryToCreate)) && Monster->IsFreeForMe(GetNearLSquare(TryToCreate)))
		      {
			Created = true;

			if(RAND_N(3))
			  Monster->SetTeam(game::GetTeam(MONSTER_TEAM));
			else
			  Monster->SetTeam(game::GetTeam(PLAYER_TEAM));

			Monster->PutTo(TryToCreate);

			if(Monster->CanBeSeenByPlayer())
			  {
			    if(Monster->IsPet())
			      {
				if(Monster->CanBeSeenByPlayer())
				  ADD_MESSAGE("%s appears from the fountain! %s seems to be friendly.", Monster->CHAR_NAME(DEFINITE), Monster->GetPersonalPronoun().CapitalizeCopy().CStr());
			      }
			    else
			      {
				if(Monster->CanBeSeenByPlayer())
				  ADD_MESSAGE("%s appears from the fountain!", Monster->CHAR_NAME(DEFINITE));
			      }
			  }
			else
			  ADD_MESSAGE("You feel something moving near you.");

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
		  GetLSquareUnder()->AddItem(ToBeCreated);

		  if(ToBeCreated->CanBeSeenByPlayer())
		    ADD_MESSAGE("There's something sparkling in the water.");

		  break;
		}
	    case 7:
	      {
		if(!RAND_N(2))
		  {
		    olterrain* Found = GetLevel()->GetRandomFountainWithWater(this);
		    Drinker->RemoveTraps();

		    if(Found)
		      {
			ADD_MESSAGE("The fountain sucks you in. You are thrown through a network of tunnels and end up coming out from an other fountain.");
			Found->GetLSquareUnder()->KickAnyoneStandingHereAway();
			Drinker->Move(Found->GetPos(), true);
		      }
		    else
		      {
			int To = GetLSquareUnder()->GetDungeon()->GetLevelTeleportDestination(GetLevel()->GetIndex());
			int From = GetLevel()->GetIndex();

			if(To == From)
			  game::TryTravel(game::GetCurrentDungeonIndex(), To, RANDOM, true, false);		   
			else
			  game::TryTravel(game::GetCurrentDungeonIndex(), To, FOUNTAIN, true, false);

			olterrain* OLTerrain = Drinker->GetLSquareUnder()->GetOLTerrain();

			if(OLTerrain && OLTerrain->IsFountainWithWater() && To != From)
			  {
			    ADD_MESSAGE("The fountain sucks you in. You are thrown through a network of tunnels and end up coming out from an other fountain.");
			  }
			else
			  {
			    ADD_MESSAGE("The fountain sucks you in. You are thrown through a network of tunnels. Suddenly the wall of the tunnel bursts open and you fly out with the water.");
			    Drinker->GetLSquareUnder()->SpillFluid(Drinker, new liquid(WATER, 1000 + RAND() % 501), false, false);
			  }
		      }
		    break;
		  }
	      }
	    default:
	      ADD_MESSAGE("The water tastes good.");
	      Drinker->EditNP(500);
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
  ChangeSecondaryMaterial(0);

  if(GetLSquareUnder())
    {
      GetLSquareUnder()->SendNewDrawRequest();
      GetLSquareUnder()->SendMemorizedUpdateRequest();
    }
}

void brokendoor::BeKicked(character* Kicker, int KickDamage, int)
{
  if(!Opened) 
    {
      if(!IsLocked() && KickDamage > (RAND() & 3))
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The broken door opens.");

	  MakeWalkable();
	  return;
	}

      if(IsLocked())
	{
	  int SV = Max(GetStrengthValue(), 1);

	  if(KickDamage > SV && RAND() % (100 * KickDamage / SV) >= 100)
	    {
	      if(RAND() & 1)
		{
		  if(CanBeSeenByPlayer())
		    ADD_MESSAGE("The broken door opens from the force of the kick.");

		  MakeWalkable();
		}
	      else if(CanBeSeenByPlayer())
		{
		  ADD_MESSAGE("The lock breaks from the force of the kick.");

		  if(GetRoom())
		    GetRoom()->DestroyTerrain(Kicker);
		}

	      SetIsLocked(false);
	      return;
	    }
	}

      if(CanBeSeenByPlayer() && (Kicker->CanBeSeenByPlayer() || Kicker->IsPlayer()))
	ADD_MESSAGE("The broken door won't budge!");
    }
}

bool altar::Polymorph(character*)
{
  room* Room = GetRoom();

  if(Room && !Room->AllowAltarPolymorph())
    return false;

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s glows briefly.", CHAR_NAME(DEFINITE));
	
  int OldGod = GetConfig(), NewGod = GetConfig();

  while(NewGod == OldGod)
    NewGod = 1 + RAND() % GODS;

  if(GetRoom())
    GetRoom()->SetDivineMaster(NewGod);

  SetConfig(NewGod);
  GetLSquareUnder()->SendNewDrawRequest();
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  return true;	
}

bool altar::SitOn(character* Sitter)
{
  ADD_MESSAGE("You kneel down and worship %s for a moment.", GetMasterGod()->GetName());

  if(GetMasterGod()->GetRelation() < 500)
    {
      if(!(RAND() % 20))
	{
	  GetMasterGod()->AdjustRelation(2);
	  game::ApplyDivineAlignmentBonuses(GetMasterGod(), 1, true);
	  PLAYER->EditExperience(WISDOM, 75, 1 << 6);
	}
    }
  else
    if(!(RAND() % 2500))
      {
	character* Angel = GetMasterGod()->CreateAngel(PLAYER->GetTeam());

	if(Angel)
	  ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));

	GetMasterGod()->AdjustRelation(50);
	game::ApplyDivineAlignmentBonuses(GetMasterGod(), 10, true);
	PLAYER->EditExperience(WISDOM, 400, 1 << 11);
      }

  Sitter->EditAP(-1000);
  return true;
}

void door::Break()
{
  if(BoobyTrap)
    ActivateBoobyTrap();
  else
    {
      bool Open = Opened;
      brokendoor* Temp = new brokendoor(GetConfig(), NO_MATERIALS);
      Temp->InitMaterials(GetMainMaterial()->Clone());
      Temp->SetIsLocked(IsLocked());
      Temp->SetBoobyTrap(0);
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
      GetLevel()->Explosion(0, "killed by an exploding booby trapped door", GetPos(), 20 + RAND() % 5 - RAND() % 5);
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
  ToBeDipped->DipInto(static_cast<liquid*>(GetSecondaryMaterial()->Clone(100)), Who);
  return true;
}

void fountain::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << SecondaryMaterial;
}

void fountain::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  LoadMaterial(SaveFile, SecondaryMaterial);

}

material* fountain::GetMaterial(int I) const
{
  return !I ? MainMaterial : SecondaryMaterial;
}

color16 fountain::GetMaterialColorB(int) const
{
  if(GetSecondaryMaterial())
    return GetSecondaryMaterial()->GetColor();
  else
    return 0;
}

void door::VirtualConstructor(bool Load)
{
  olterrain::VirtualConstructor(Load);

  if(!Load)
    {
      /* Terrible gum solution! */

      if(!(GetConfig() & LOCK_BITS))
	{
	  int NormalLockTypes = 0;
	  const database*const* ConfigData = GetProtoType()->GetConfigData();
	  int c, ConfigSize = GetProtoType()->GetConfigSize();

	  for(c = 0; c < ConfigSize; ++c)
	    if(ConfigData[c]->Config & LOCK_BITS
	    && (ConfigData[c]->Config & ~LOCK_BITS) == GetConfig()
	    && !(ConfigData[c]->Config & S_LOCK_ID))
	      ++NormalLockTypes;

	  int ChosenLock = RAND() % NormalLockTypes;

	  for(c = 0; c < ConfigSize; ++c)
	    if(ConfigData[c]->Config & LOCK_BITS
	    && (ConfigData[c]->Config & ~LOCK_BITS) == GetConfig()
	    && !(ConfigData[c]->Config & S_LOCK_ID)
	    && !ChosenLock--)
	      {
		SetConfig(ConfigData[c]->Config, NO_PIC_UPDATE);
		break;
	      }
	}

      SetBoobyTrap(0);
      SetIsOpened(false);
      SetIsLocked(false);
    }
}

void door::SetParameters(int Param)
{
  SetIsLocked(Param & LOCKED);
}

bool door::TryKey(item* Thingy, character* Applier)
{
  if(Opened)
    return false;

  if(Thingy->CanOpenLockType(GetConfig()&LOCK_BITS))
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
  int Chosen = RandomizeMaterialConfiguration();
  const fearray<long>& MMC = GetMainMaterialConfig();
  InitMaterial(MainMaterial,
	       MAKE_MATERIAL(MMC.Data[MMC.Size == 1 ? 0 : Chosen]),
	       0);
  const fearray<long>& SMC = GetSecondaryMaterialConfig();
  InitMaterial(SecondaryMaterial,
	       MAKE_MATERIAL(SMC.Data[SMC.Size == 1 ? 0 : Chosen]),
	       0);
}

bool fountain::AddAdjective(festring& String, bool Articled) const
{
  if(!GetSecondaryMaterial())
    {
      String << (Articled ? "a dried out " : "dried out ");
      return true;
    }
  else
    return false;
}

fountain::~fountain()
{
  delete SecondaryMaterial;
}

bool fountain::IsSparkling(int I) const
{
  return (I == 0 && MainMaterial->IsSparkling()) || (I == 1 && SecondaryMaterial && SecondaryMaterial->IsSparkling());
}

void stairs::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << AttachedArea << AttachedEntry;
}

void stairs::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  SaveFile >> AttachedArea >> AttachedEntry;
}

bool stairs::Enter(bool DirectionUp) const
{
  if(DirectionUp != IsUpLink())
    return olterrain::Enter(DirectionUp);

  /* "Temporary" gum solutions */

  if(GetConfig() == OREE_LAIR_ENTRY)
    {
      ADD_MESSAGE("You sense terrible evil trembling very near under your feet. You feel you shouldn't wander any further. On the other hand you have little choice.");

      if(!game::BoolQuestion(CONST_S("Continue? [y/N]")))
	return false;
    }

  if(GetConfig() == OREE_LAIR_EXIT)
    if(PLAYER->HasGoldenEagleShirt())
      {
	ADD_MESSAGE("Somehow you get the feeling you cannot return.");

	if(!game::BoolQuestion(CONST_S("Continue anyway? [y/N]")))
	  return false;
      }
    else
      {
	ADD_MESSAGE("An unknown magical force pushes you back.");
	PLAYER->EditAP(-1000);
	return true;
      }

  if(GetConfig() == DARK_LEVEL)
    {
      ADD_MESSAGE("This dark gate seems to be a one-way portal. You sense something distant but extremely dangerous on the other side. You feel you should think twice before entering.");

      if(!game::BoolQuestion(CONST_S("Continue? [y/N]")))
	return false;
    }

  if(GetConfig() == SUMO_ARENA_ENTRY && !game::TryToEnterSumoArena())
    return false;

  if(GetConfig() == SUMO_ARENA_EXIT && !game::TryToExitSumoArena())
    return false;

  return game::TryTravel(game::GetCurrentDungeonIndex(), GetAttachedArea(), GetAttachedEntry(), GetAttachedArea() != WORLD_MAP);
}

void stairs::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer()) 
    ADD_MESSAGE("There is %s here.", CHAR_NAME(INDEFINITE));
}

void stairs::VirtualConstructor(bool Load)
{
  olterrain::VirtualConstructor(Load);

  if(!Load)
    if(GetConfig() == STAIRS_UP)
      {
	if(game::GetCurrentLevelIndex())
	  {
	    AttachedArea = game::GetCurrentLevelIndex() - 1;
	    AttachedEntry = STAIRS_DOWN;
	  }
	else
	  {
	    AttachedArea = WORLD_MAP;
	    AttachedEntry = game::GetCurrentDungeonIndex();
	  }
      }
    else if(GetConfig() == STAIRS_DOWN)
      {
	AttachedArea = game::GetCurrentLevelIndex() + 1;
	AttachedEntry = STAIRS_UP;
      }
}

void sign::AddPostFix(festring& String) const
{
  String << " with text \"" << Text << '\"';
}

void sign::StepOn(character* Stepper)
{
  if(Stepper->IsPlayer())
    ADD_MESSAGE("There's a sign here saying: \"%s\"", Text.CStr());
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
  Contained = new stack(0, this, HIDDEN);
}

bool olterraincontainer::Open(character* Opener)
{
  if(!Opener->IsPlayer())
    return false;

  bool Success;

  switch(game::KeyQuestion(CONST_S("Do you want to (t)ake something from or (p)ut something in this container? [t,p]"), KEY_ESC, 3, 't', 'p', KEY_ESC))
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

void olterraincontainer::SetItemsInside(const fearray<contentscript<item> >& ItemArray, int SpecialFlags)
{
  GetContained()->Clean();

  for(uint c1 = 0; c1 < ItemArray.Size; ++c1)
    if(ItemArray[c1].IsValid())
      {
	const interval* TimesPtr = ItemArray[c1].GetTimes();
	int Times = TimesPtr ? TimesPtr->Randomize() : 1;

	for(int c2 = 0; c2 < Times; ++c2)
	  {
	    item* Item = ItemArray[c1].Instantiate(SpecialFlags);

	    if(Item)
	      {
		Contained->AddItem(Item);
		Item->SpecialGenerationHandler();
	      }
	  }
      }
}

void door::ReceiveDamage(character* Villain, int Damage, int)
{
  if(!Opened && !IsLocked() && Damage > (RAND() & 3))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The door opens.");
		
      MakeWalkable();
      return;
    }

  if(CanBeDestroyed() && Damage > GetStrengthValue())
    {
      EditHP(GetStrengthValue() - Damage);
      int SV = Max(GetStrengthValue(), 1);
      bool LockBreaks = IsLocked() && RAND() % (100 * Damage / SV) >= 100;

      if(LockBreaks)
	SetIsLocked(false);

      if(HP <= 0)
	{
	  if(CanBeSeenByPlayer())
	    if(LockBreaks)
	      ADD_MESSAGE("The door breaks and its lock is destroyed.");
	    else
	      ADD_MESSAGE("The door breaks.");

	  room* Room = GetRoom();
	  Break();

	  if(Room)
	    Room->DestroyTerrain(Villain);
	}
      else if(LockBreaks && CanBeSeenByPlayer())
	{
	  ADD_MESSAGE("The door's lock is shattered.");

	  if(GetRoom())
	    GetRoom()->DestroyTerrain(Villain);
	}
    }
}

void brokendoor::ReceiveDamage(character* Villain, int Damage, int)
{
  if(!Opened && !IsLocked() && Damage > (RAND() & 3))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The broken door opens.");
		
      MakeWalkable();
      return;
    }

  if(CanBeDestroyed() && Damage > GetStrengthValue())
    {
      EditHP(GetStrengthValue() - Damage);
      int SV = Max(GetStrengthValue(), 1);
      bool LockBreaks = IsLocked() && RAND() % (100 * Damage / SV) >= 100;

      if(LockBreaks)
	SetIsLocked(false);

      if(HP <= 0)
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The broken door is completely destroyed.");

	  room* Room = GetRoom();
	  Break();

	  if(Room)
	    Room->DestroyTerrain(Villain);
	}
      else if(LockBreaks && CanBeSeenByPlayer())
	{
	  ADD_MESSAGE("The broken door's lock is shattered.");

	  if(GetRoom())
	    GetRoom()->DestroyTerrain(Villain);
	}
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

bool fountain::IsDipDestination() const
{
 return SecondaryMaterial && SecondaryMaterial->IsLiquid(); 
}

int door::GetWalkability() const
{
  return Opened ? ANY_MOVE : ANY_MOVE&~(WALK|FLY);
}

bool door::IsTransparent() const
{
  return Opened || MainMaterial->IsTransparent();
}

bool liquidterrain::DipInto(item* ToBeDipped, character* Who)
{
  ToBeDipped->DipInto(static_cast<liquid*>(GetMainMaterial()->Clone(100)), Who);
  return true;
}

void earth::VirtualConstructor(bool Load)
{
  olterrain::VirtualConstructor(Load);
  if(!Load)
    PictureIndex = RAND() & 3;
}

void earth::Save(outputfile& SaveFile) const
{
  olterrain::Save(SaveFile);
  SaveFile << PictureIndex;
}

void earth::Load(inputfile& SaveFile)
{
  olterrain::Load(SaveFile);
  SaveFile >> PictureIndex;
}

vector2d earth::GetBitmapPos(int I) const
{
  return olterrain::GetBitmapPos(I) + vector2d(PictureIndex * 48, 0);
}

void door::BeDestroyed()
{
  olterrain::Break();
}

bool fountain::IsFountainWithWater() const
{
  return !!GetSecondaryMaterial();
}

void liquidterrain::SurviveEffect(character* Survivor)
{
  Survivor->GetLSquareUnder()->SpillFluid(Survivor, static_cast<liquid*>(GetMainMaterial()->Clone(1000 + RAND_N(500))), false, false);  
}

void monsterportal::VirtualConstructor(bool Load)
{
  olterrain::VirtualConstructor(Load);
  game::SetMonsterPortal(this);
}

void olterraincontainer::PreProcessForBone()
{
  olterrain::PreProcessForBone();
  Contained->PreProcessForBone();
}

void olterraincontainer::PostProcessForBone()
{
  olterrain::PostProcessForBone();
  Contained->PostProcessForBone();
}

void olterraincontainer::FinalProcessForBone()
{
  olterrain::FinalProcessForBone();
  Contained->FinalProcessForBone();
}
