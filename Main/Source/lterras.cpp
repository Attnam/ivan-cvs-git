/* Compiled through levelset.cpp */

bool door::CanBeOpenedByAI() { return !IsLocked() && CanBeOpened(); }
void door::HasBeenHitByItem(character* Thrower, item*, ushort Damage) { ReceiveDamage(Thrower, Damage, PHYSICAL_DAMAGE); }
void door::AddPostFix(festring& String) const { AddLockPostFix(String, LockType); }
vector2d door::GetBitmapPos(ushort Frame) const { return Opened ? GetOpenBitmapPos(Frame) : olterrain::GetBitmapPos(Frame); }

vector2d portal::GetBitmapPos(ushort Frame) const { return vector2d(16 + (((Frame & 31) << 3)&~8), 0); } // gum solution, should come from script

void fountain::SetContainedMaterial(material* What, ushort SpecialFlags) { SetMaterial(ContainedMaterial, What, GetDefaultContainedVolume(), SpecialFlags); }
void fountain::ChangeContainedMaterial(material* What, ushort SpecialFlags) { ChangeMaterial(ContainedMaterial, What, GetDefaultContainedVolume(), SpecialFlags); }
void fountain::InitMaterials(material* M1, material* M2, bool CUP) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), ContainedMaterial, M2, GetDefaultContainedVolume(), CUP); }
vector2d fountain::GetBitmapPos(ushort) const { return vector2d(GetContainedMaterial() ? 16 : 32, 288); }

void brokendoor::HasBeenHitByItem(character* Thrower, item*, ushort Damage) { ReceiveDamage(Thrower, Damage, PHYSICAL_DAMAGE); }

const char* liquidterrain::SurviveMessage() const { return "you manage to get out of the pool"; }
const char* liquidterrain::MonsterSurviveMessage() const { return "manages to get out of the pool"; }
const char* liquidterrain::DeathMessage() const { return "you drown"; }
const char* liquidterrain::MonsterDeathVerb() const { return "drowns"; }
const char* liquidterrain::ScoreEntry() const { return "drowned"; }
vector2d liquidterrain::GetBitmapPos(ushort Frame) const { return vector2d(48 + ((Frame << 3)&~8), 0); } // gum solution, should come from script

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
      if(Opener->IsPlayer()) ADD_MESSAGE("The door is already open, %s.", game::Insult());
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

void altar::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAnimate) const
{
  olterrain::Draw(Bitmap, Pos, Luminance, AllowAnimate);
  igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, GetConfig() << 4, 0, Pos, 16, 16, Luminance);
}

void door::BeKicked(character* Kicker, ushort KickDamage)
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
      PLAYER->AddScoreEntry(CONST_S("became the new high priest of the Great Frog"), 5, false);
      game::End();
      return true;
    }

  ADD_MESSAGE("You feel somehow out of place.");
  return true;
}

void altar::BeKicked(character* Kicker, ushort)
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

void altar::ReceiveVomit(character* Who)
{
  if(Who->IsPlayer())
    {
      GetMasterGod()->PlayerVomitedOnAltar();

      if(GetRoom())
	GetRoom()->HostileAction(Who);
    }
}

bool door::AddAdjective(festring& String, bool Articled) const
{
  if(Articled)
    String << (Opened ? "an open" : "a closed");
  else
    String << (Opened ? "open" : "closed");

  if(IsLocked())
    String << ", locked ";
  else
    String << ' ';

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
	  room* Room = GetRoom();

	  if(Room && Room->HasDrinkHandler() && !Room->Drink(Drinker))
	    return false;
	  else
	    {
	      if(!game::BoolQuestion(CONST_S("Do you want to drink from the fountain? [y/N]")))
		return false;
	    }

	  Drinker->EditAP(-1000);

	  switch(RAND() % 9)
	    {
	    case 0:
	      ADD_MESSAGE("The water is contaminated!");
	      Drinker->EditNP(100);

	      if(!(RAND() % 5))
		Drinker->PolymorphRandomly(0, 10000, 2500 + RAND() % 2500);
	      else
		Drinker->ChangeRandomStat(-1);

	      break;
	
	    case 1:
	      ADD_MESSAGE("The water tasted very good.");
	      Drinker->EditNP(2500);
	      Drinker->ChangeRandomStat(1);
	      break;

	    case 2:
	      if(!(RAND() % 12))
		{
		  ADD_MESSAGE("You have freed a spirit that grants you a wish. You may wish for an item.");

		  while(true)
		    {
		      festring Temp = game::StringQuestion(CONST_S("What do you want to wish for?"), vector2d(16, 6), WHITE, 0, 80, false);
		      item* TempItem = protosystem::CreateItem(Temp, Drinker->IsPlayer());

		      if(TempItem)
			{
			  Drinker->GetStack()->AddItem(TempItem);
			  TempItem->SpecialGenerationHandler();

			  if(TempItem->HandleInPairs())
			    ADD_MESSAGE("Two %s appear from nothing and the spirit flies happily away!", TempItem->CHAR_NAME(PLURAL));
			  else
			    ADD_MESSAGE("%s appears from nothing and the spirit flies happily away!", TempItem->CHAR_NAME(INDEFINITE));

			  break;
			}
		    }
		}
	      else
		DryOut();

	      break;

	    case 4:
	      if(RAND() & 7)
		{
		  ADD_MESSAGE("The water tastes normal, but there is an odd after taste.");
		  Drinker->ActivateRandomState(SRC_FOUNTAIN, 2000 + RAND() % 3000);
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

		switch(RAND() & 3)
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

		    if(GetLevel()->IsValidPos(TryToCreate) && GetNearLSquare(TryToCreate)->IsWalkable(Monster) && !GetNearLSquare(TryToCreate)->GetCharacter())
		      {
			Created = true;
			GetNearLSquare(TryToCreate)->AddCharacter(Monster);

			if(RAND() % 5)
			  {
			    Monster->SetTeam(game::GetTeam(MONSTER_TEAM));

			    if(Monster->CanBeSeenByPlayer())
			      ADD_MESSAGE("%s appears from the fountain!", Monster->CHAR_NAME(DEFINITE));
			  }
			else
			  {
			    Monster->SetTeam(game::GetTeam(PLAYER_TEAM));

			    if(Monster->CanBeSeenByPlayer())
			      ADD_MESSAGE("%s appears from the fountain! %s seems to be friendly.", Monster->CHAR_NAME(DEFINITE), Monster->GetPersonalPronoun().CapitalizeCopy().CStr());
			  }

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
  ChangeContainedMaterial(0);

  if(GetLSquareUnder())
    {
      GetLSquareUnder()->SendNewDrawRequest();
      GetLSquareUnder()->SendMemorizedUpdateRequest();
    }
}

void brokendoor::BeKicked(character* Kicker, ushort KickDamage)
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
		{
		  ADD_MESSAGE("The lock breaks from the force of your kick.");

		  if(GetRoom())
		    GetRoom()->DestroyTerrain(Kicker);
		}

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
  room* Room = GetRoom();

  if(Room && !Room->AllowAltarPolymorph())
    return false;

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s glows briefly.", CHAR_NAME(DEFINITE));
	
  uchar OldGod = GetConfig(), NewGod = GetConfig();

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
	  game::ApplyDivineAlignmentBonuses(GetMasterGod(), true, 1);
	  PLAYER->EditExperience(WISDOM, 10);
	}
    }
  else
    if(!(RAND() % 2500))
      {
	character* Angel = GetMasterGod()->CreateAngel();

	if(Angel)
	  {
	    Angel->SetTeam(PLAYER->GetTeam());
	    ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
	  }

	GetMasterGod()->AdjustRelation(50);
	game::ApplyDivineAlignmentBonuses(GetMasterGod(), true);
	PLAYER->EditExperience(WISDOM, 250);
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
      Temp->InitMaterials(GetMainMaterial()->Clone());
      Temp->SetIsLocked(IsLocked());
      Temp->SetBoobyTrap(0);
      Temp->SetLockType(GetLockType());
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
  ToBeDipped->DipInto(GetContainedMaterial()->Clone(GetContainedMaterial()->TakeDipVolumeAway()), Who);
  return true;
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
  return !Index ? MainMaterial : ContainedMaterial;
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

bool fountain::AddAdjective(festring& String, bool Articled) const
{
  if(!GetContainedMaterial())
    {
      String << (Articled ? "a dried out " : "dried out ");
      return true;
    }
  else
    return false;
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

  return game::TryTravel(game::GetCurrentDungeonIndex(), GetAttachedArea(), GetAttachedEntry(), GetAttachedArea() != WORLD_MAP);
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
      GetLSquareUnder()->AddItem(Stone);
      
    }
  olterrain::Break();
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
  Contained = new stack(0, this, HIDDEN, true);
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

void olterraincontainer::SetItemsInside(const std::list<contentscript<item> >& ItemList, ushort SpecialFlags)
{
  GetContained()->Clean();

  for(std::list<contentscript<item> >::const_iterator i = ItemList.begin(); i != ItemList.end(); ++i)
    if(i->IsValid())
      {
	item* Item = i->Instantiate(SpecialFlags);

	if(Item)
	  {
	    Contained->AddItem(Item);
	    Item->SpecialGenerationHandler();
	  }
      }
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
	  GetLSquareUnder()->AddItem(Stone);
	}
    }

  olterrain::Break();
}

void door::ReceiveDamage(character* Villain, ushort Damage, ushort)
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
      ushort SV = Max<ushort>(GetStrengthValue(), 1);
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

void brokendoor::ReceiveDamage(character* Villain, ushort Damage, ushort)
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
      ushort SV = Max<ushort>(GetStrengthValue(), 1);
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
 return ContainedMaterial != 0 && ContainedMaterial->IsLiquid(); 
}

bool wall::IsWalkable(const character* Char) const
{
  return Char && Char->CanWalkThroughWalls();
}

bool wall::IsTransparent() const
{
  return MainMaterial->IsTransparent();
}

bool door::IsWalkable(const character* Char) const
{
  return Opened || (Char && Char->CanWalkThroughWalls());
}

bool door::IsTransparent() const
{
  return Opened || MainMaterial->IsTransparent() || GetConfig() == DOOR_PRISON;
}

bool liquidterrain::DipInto(item* ToBeDipped, character* Who)
{
  ToBeDipped->DipInto(GetMainMaterial()->Clone(GetMainMaterial()->TakeDipVolumeAway()), Who);
  return true;
}
