/* Compiled through itemset.cpp */

void materialcontainer::SetSecondaryMaterial(material* What, int SpecialFlags) { SetMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
void materialcontainer::ChangeSecondaryMaterial(material* What, int SpecialFlags) { ChangeMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
void materialcontainer::InitMaterials(material* M1, material* M2, bool CUP) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), SecondaryMaterial, M2, GetDefaultSecondaryVolume(), CUP); }
void materialcontainer::InitMaterials(const materialscript* M, const materialscript* C, bool CUP) { InitMaterials(M->Instantiate(), C->Instantiate(), CUP); }

int holybanana::GetSpecialFlags() const { return ST_FLAME_1; }

color16 lantern::GetMaterialColorA(int) const { return MakeRGB16(255, 255, 240); }
color16 lantern::GetMaterialColorB(int) const { return MakeRGB16(255, 255, 100); }
color16 lantern::GetMaterialColorC(int) const { return MakeRGB16(255, 255, 100); }
color16 lantern::GetMaterialColorD(int) const { return MakeRGB16(255, 255, 100); }

bool can::AddAdjective(festring& String, bool Articled) const { return AddEmptyAdjective(String, Articled); }
vector2d can::GetBitmapPos(int) const { return vector2d(16, SecondaryMaterial ? 288 : 304); }
bool can::IsDipDestination(const character*) const { return SecondaryMaterial && SecondaryMaterial->IsLiquid(); }

bool potion::IsExplosive() const { return GetSecondaryMaterial() && GetSecondaryMaterial()->IsExplosive(); }
bool potion::AddAdjective(festring& String, bool Articled) const { return AddEmptyAdjective(String, Articled); }
bool potion::EffectIsGood() const { return GetSecondaryMaterial() && GetSecondaryMaterial()->EffectIsGood(); }
bool potion::IsDipDestination(const character*) const { return SecondaryMaterial && SecondaryMaterial->IsLiquid(); }

bool bananapeels::IsDangerousForAI(const character* Stepper) const { return Stepper->HasFeet(); }

bool brokenbottle::IsDangerousForAI(const character* Stepper) const { return Stepper->HasFeet(); }

long wand::GetPrice() const { return Charges > TimesUsed ? item::GetPrice() : 0; }

bool backpack::IsExplosive() const { return GetSecondaryMaterial() && GetSecondaryMaterial()->IsExplosive(); }
long backpack::GetTotalExplosivePower() const { return GetSecondaryMaterial() ? GetSecondaryMaterial()->GetTotalExplosivePower() : 0; }

long stone::GetTruePrice() const { return GetMainMaterial()->GetRawPrice() << 1; }

color16 whistle::GetMaterialColorB(int) const { return MakeRGB16(80, 32, 16); }

color16 itemcontainer::GetMaterialColorB(int) const { return MakeRGB16(80, 80, 80); }

bool mine::AddAdjective(festring& String, bool Articled) const { return IsActive() && AddActiveAdjective(String, Articled); }

bool beartrap::AddAdjective(festring& String, bool Articled) const { return (IsActive() && AddActiveAdjective(String, Articled)) || (!IsActive() && item::AddAdjective(String, Articled)); }

color16 carrot::GetMaterialColorB(int) const { return MakeRGB16(80, 100, 16); }

color16 charmlyre::GetMaterialColorB(int) const { return MakeRGB16(150, 130, 110); }

bool scroll::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::GetSeeWholeMapCheatMode();
}

void scrollofcreatemonster::FinishReading(character* Reader)
{
  vector2d TryToCreate;

  for(int c = 0; c < 100; ++c)
    {
      TryToCreate = Reader->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);
      character* Monster = protosystem::CreateMonster();

      if(GetArea()->IsValidPos(TryToCreate) && Monster->CanMoveOn(GetNearLSquare(TryToCreate)) && Monster->IsFreeForMe(GetNearLSquare(TryToCreate)))
	{
	  Monster->PutTo(TryToCreate);

	  if(Reader->IsPlayer())
	    {
	      if(Monster->CanBeSeenByPlayer())
		ADD_MESSAGE("%s appears.", Monster->CHAR_NAME(INDEFINITE));
	      else
		ADD_MESSAGE("You notice no effect.");
	    }
	  else if(Reader->CanBeSeenByPlayer())
	    {
	      if(Monster->CanBeSeenByPlayer())
		ADD_MESSAGE("%s summons %s!", Reader->CHAR_NAME(DEFINITE), Monster->CHAR_NAME(INDEFINITE));
	    }
	  else if(Monster->CanBeSeenByPlayer())
	    ADD_MESSAGE("Suddenly %s appears.", Monster->CHAR_NAME(INDEFINITE));

	  RemoveFromSlot();
	  SendToHell();
	  return;
	}
      else
	delete Monster;
    }

  ADD_MESSAGE("You feel a lost soul fly by you.");
  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 100, 1 << 12);
}

void scrollofteleportation::FinishReading(character* Reader)
{
  if(Reader->IsPlayer())
    ADD_MESSAGE("Suddenly you realize you have teleported.");
  else if(Reader->CanBeSeenByPlayer())
    ADD_MESSAGE("%s disappears!", Reader->CHAR_NAME(DEFINITE));

  Reader->TeleportRandomly();
  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 100, 1 << 12);
}

bool lump::HitEffect(character* Enemy, character*, vector2d, int, int, bool BlockedByArmour)
{
  if(!BlockedByArmour && RAND() & 1)
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("The %s touches %s!", GetMainMaterial()->GetName(false, false).CStr(), Enemy->CHAR_DESCRIPTION(DEFINITE));

      return GetMainMaterial()->HitEffect(Enemy);
    }
  else
    return false;
}

bool wand::Apply(character* Terrorist)
{
  if(Terrorist->IsPlayer() && !game::BoolQuestion(CONST_S("Are you sure you want to break ") + GetName(DEFINITE) + "? [y/N]")) 
    return false;

  BreakEffect(Terrorist, CONST_S("killed by breaking ") + GetName(INDEFINITE));
  Terrorist->DexterityAction(5);
  return true;
}

void wand::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << TimesUsed << Charges;
}

void wand::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> TimesUsed >> Charges;
}

void scrollofwishing::FinishReading(character* Reader)
{
  game::Wish(Reader,
	     "%s appears from nothing and the scroll burns!",
	     "Two %s appear from nothing and the scroll burns!");

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 400, 1 << 12);
}

void scrollofchangematerial::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->GetItems() && !Reader->EquipsSomething())
    {
      ADD_MESSAGE("You notice you have lost anything you wished to alter.");
      return;
    }

  for(;;)
    {
      itemvector Item;
      Reader->SelectFromPossessions(Item, CONST_S("What item do you wish to change?"), NO_MULTI_SELECT|SELECT_PAIR);

      if(Item.empty())
	if(game::BoolQuestion(CONST_S("Really cancel read? [y/N]")))
	  return;
	else
	  continue;

      if(!Item[0]->IsMaterialChangeable())
	{
	  ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
	  break;
	}

      if(Item[0]->HandleInPairs() && Item.size() == 1)
	{
	  ADD_MESSAGE("Only one %s will be altered.", Item[0]->CHAR_NAME(UNARTICLED));

	  if(!game::BoolQuestion(CONST_S("Still continue? [y/N]")))
	    continue;
	}

      festring Temp = game::DefaultQuestion(CONST_S("What material do you want to wish for?"),
					    game::GetDefaultChangeMaterial());
      material* TempMaterial = protosystem::CreateMaterial(Temp);

      if(!TempMaterial)
	{
	  game::DrawEverythingNoBlit();
	  continue;
	}

      msgsystem::EnterBigMessageMode();

      if(Item.size() == 1)
	ADD_MESSAGE("Suddenly your %s is consumed in roaring magical flames.", Item[0]->CHAR_NAME(UNARTICLED));
      else
	ADD_MESSAGE("Suddenly your %s are consumed in roaring magical flames.", Item[0]->CHAR_NAME(PLURAL));

      if(TempMaterial->GetIntelligenceRequirement() + 10 > Reader->GetAttribute(INTELLIGENCE))
	{
	  ADD_MESSAGE("But your mind is not yet strong enough to summon enough %s for the change.", TempMaterial->GetName(false, false).CStr());
	  delete TempMaterial;
	  msgsystem::LeaveBigMessageMode();
	  continue;
	}

      material* MainMaterial = Item[0]->GetMainMaterial();
      material* SecondaryMaterial = Item[0]->GetSecondaryMaterial();

      if(Item.size() == 1)
	{
	  if(!MainMaterial->IsSameAs(TempMaterial))
	    {
	      ADD_MESSAGE("As you lift it again it looks greatly altered.");

	      if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
		Item[0]->ChangeSecondaryMaterial(TempMaterial->Clone());

	      Item[0]->ChangeMainMaterial(TempMaterial);
	    }
	  else
	    ADD_MESSAGE("As you lift it again it looks unchanged.");
	}
      else
	{
	  if(!MainMaterial->IsSameAs(TempMaterial))
	    {
	      ADD_MESSAGE("As you lift them again they look greatly altered.");

	      if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
		for(uint c = 0; c < Item.size(); ++c)
		  Item[c]->ChangeSecondaryMaterial(TempMaterial->Clone());

	      Item[0]->ChangeMainMaterial(TempMaterial);

	      for(uint c = 1; c < Item.size(); ++c)
		Item[c]->ChangeMainMaterial(TempMaterial->Clone());
	    }
	  else
	    ADD_MESSAGE("As you lift them again they look unchanged.");
	}

      msgsystem::LeaveBigMessageMode();
      break;
    }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 350, 1 << 12);
}

item* brokenbottle::BetterVersion() const
{
  return new potion;
}

void brokenbottle::StepOnEffect(character* Stepper)
{
  if(Stepper->HasFeet() && !(RAND() % 5))
    {
      if(Stepper->IsPlayer())
	ADD_MESSAGE("Auch. You step on sharp glass splinters.");
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s steps on sharp glass splinters.", Stepper->CHAR_NAME(DEFINITE));

      Stepper->ReceiveDamage(0, 1 + RAND() % 3, PHYSICAL_DAMAGE, LEGS);
      Stepper->CheckDeath(CONST_S("stepped on a broken bottle"), 0);
    }
}

liquid* can::CreateDipLiquid()
{
  return static_cast<liquid*>(GetSecondaryMaterial()->Clone(GetSecondaryMaterial()->TakeDipVolumeAway()));
}

liquid* potion::CreateDipLiquid()
{
  return static_cast<liquid*>(GetSecondaryMaterial()->Clone(GetSecondaryMaterial()->TakeDipVolumeAway()));
}

void potion::DipInto(liquid* Liquid, character* Dipper)
{
  /* Add alchemy */

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now filled with %s.", CHAR_NAME(DEFINITE), Liquid->GetName(false, false).CStr());

  ChangeSecondaryMaterial(Liquid);
  Dipper->DexterityAction(10);
}

void lantern::SignalSquarePositionChange(int SquarePosition)
{
  item::SignalSquarePositionChange(SquarePosition);
  UpdatePictures();
}


item* potion::BetterVersion() const
{
  if(!GetSecondaryMaterial())
    return new potion;
  else
    return 0;
}

item* can::BetterVersion() const
{
  if(!GetSecondaryMaterial())
    {
      material* Stuff;

      if(RAND() & 1)
	Stuff = MAKE_MATERIAL(SCHOOL_FOOD);
      else
	Stuff = MAKE_MATERIAL(BANANA_FLESH);

      can* P = new can(0, NO_MATERIALS); 
      P->InitMaterials(MAKE_MATERIAL(IRON), Stuff); 
      return P;
    }
  else
    return 0;
}

bool backpack::Apply(character* Terrorist)
{
  if(IsExplosive())
    {
      if(Terrorist->IsPlayer())
	ADD_MESSAGE("You light your %s. It explodes!", CHAR_NAME(UNARTICLED));
      else if(Terrorist->CanBeSeenByPlayer())
	ADD_MESSAGE("%s lights %s. It explodes!", Terrorist->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
      else if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("Something explodes!");

      RemoveFromSlot();
      SendToHell();

      festring DeathMsg;

      if(Terrorist->IsPlayer())
	DeathMsg = CONST_S("exploded himself with ") + GetName(INDEFINITE);
      else
	DeathMsg = CONST_S("kamikazed by ") + Terrorist->GetKillName();

      Terrorist->DexterityAction(5);
      Terrorist->GetLevel()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
      return true;
    }
  else if(Terrorist->IsPlayer())
    ADD_MESSAGE("You are not able to explode yourself with this crummy %s!", CHAR_NAME(UNARTICLED));

  return false;
}

bool holybook::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::GetSeeWholeMapCheatMode();
}

void holybook::FinishReading(character* Reader)
{
  if(Reader->IsPlayer())
    {
      PLAYER->EditExperience(INTELLIGENCE, 50, 1 << 12);
      PLAYER->EditExperience(WISDOM, 100, 1 << 12);

      if(GetMasterGod()->IsKnown())
	{
	  ADD_MESSAGE("The book reveals many divine secrets of %s to you.", GetMasterGod()->GetName());
	  GetMasterGod()->AdjustRelation(75);
	  game::ApplyDivineAlignmentBonuses(GetMasterGod(), 15, true);

	  if(!(RAND() % 3))
	    {
	      ADD_MESSAGE("But then it disappears.");
	      RemoveFromSlot();
	      SendToHell();
	    }
	}
      else
	{
	  GetMasterGod()->SetIsKnown(true);
	  ADD_MESSAGE("You feel you master the magical rites of %s.", GetMasterGod()->GetName());
	}
    }
}

bool wand::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(Type & (FIRE|ENERGY|PHYSICAL_DAMAGE) && Damage && (Damage > 125 || !(RAND() % (250 / Damage))))
    {
      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      BreakEffect(Damager, CONST_S("killed by an exploding ") + GetName(UNARTICLED));
      return true;
    }

  return false;
}

bool backpack::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(Type & (FIRE|ENERGY) && Damage && IsExplosive() && (Damage > 25 || !(RAND() % (50 / Damage))))
    {
      festring DeathMsg = CONST_S("killed by an explosion of ");
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

bool scroll::ReceiveDamage(character*, int Damage, int Type, int)
{
  if(Type & FIRE && Damage && GetMainMaterial()->IsFlammable() && (Damage > 125 || !(RAND() % (250 / Damage))))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s catches fire!", CHAR_NAME(DEFINITE));

      RemoveFromSlot();
      SendToHell();
      return true;
    }

  return false;
}

bool holybook::ReceiveDamage(character*, int Damage, int Type, int)
{
  if(Type & FIRE && Damage && GetMainMaterial()->IsFlammable() && (Damage > 125 || !(RAND() % (250 / Damage))))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s catches fire!", CHAR_NAME(DEFINITE));

      RemoveFromSlot();
      SendToHell();
      return true;
    }

  return false;
}

bool oillamp::Apply(character* Applier)
{
  if(Applier->IsPlayer())
    ADD_MESSAGE("You rub %s.", CHAR_NAME(DEFINITE));

  if(GetInhabitedByGenie())
    {
      genie* Genie = new genie;
      vector2d TryToCreate;
      bool FoundPlace = false;

      /* First try to create a genie nearby (10 tries - if all of them fail then stop trying) */

      for(int c = 0; c < 10 && !FoundPlace; ++c)
	{	  
	  TryToCreate = Applier->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

	  if(GetArea()->IsValidPos(TryToCreate) && Genie->CanMoveOn(GetNearLSquare(TryToCreate)) && Genie->IsFreeForMe(GetNearLSquare(TryToCreate)))
	    {
	      Genie->PutTo(TryToCreate);
	      FoundPlace = true;
	      SetInhabitedByGenie(false);
	      break;
	    }
	}

      if(FoundPlace)
	{
	  Genie->GetLSquareUnder()->AddSmoke(new gas(SMOKE, 1000));

	  if(!(RAND() % 5))
	    {
	      Genie->SetTeam(game::GetTeam(MONSTER_TEAM));
	      ADD_MESSAGE("You see a puff of smoke, and %s appears. \"For centuries I have been imprisoned in this lamp. But at last you have freed me! As a reward, I will kill you.\"", Genie->CHAR_NAME(INDEFINITE));
	    }
	  else
	    {
	      if(Applier->IsPlayer())
		{
		  Genie->SetTeam(Applier->GetTeam());
		  ADD_MESSAGE("You see a puff of smoke, and %s appears. \"For centuries I have been imprisoned in this lamp. But at last you have freed me! I am deeply grateful. You deserve a generous reward. I may serve you for 1001 nights or grant you a wish. It's your choice.\"", Genie->CHAR_NAME(INDEFINITE));

		  if(game::BoolQuestion(CONST_S("Do you want to wish? [Y/n]"), YES))
		    {
		      ADD_MESSAGE("You may wish for an item.");
		      game::Wish(Applier,
				 "%s appears from nothing and the genie flies happily away!",
				 "Two %s appear from nothing and the genie flies happily away!");

		      Genie->Remove();
		      delete Genie;
		      Applier->EditAP(-1000);
		      return true;
		    }
		}
	    }

	  meleeweapon* Weapon = new meleeweapon(TWO_HANDED_SCIMITAR, NO_MATERIALS);
	  Weapon->InitMaterials(MAKE_MATERIAL(ARCANITE), MAKE_MATERIAL(ARCANITE), true);
	  Genie->SetRightWielded(Weapon);
	  ADD_MESSAGE("%s wishes for a two-handed scimitar. Suddenly %s appears from nothing and %s wields it.", Genie->CHAR_NAME(DEFINITE), Genie->GetMainWielded()->CHAR_NAME(INDEFINITE), Genie->CHAR_NAME(DEFINITE));
	}
      else
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You feel that it is warmer.");

	  delete Genie;
	}
    }
  else
    if(Applier->IsPlayer())
      ADD_MESSAGE("Nothing happens.");

  Applier->EditAP(-1000);
  return true;
}

void oillamp::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << InhabitedByGenie;
}

void oillamp::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> InhabitedByGenie;
}

color16 holybook::GetMaterialColorA(int) const
{
  return GetMasterGod()->GetColor();
}

bool scrollofcharging::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::IsChargeable) && !Reader->EquipsSomething(&item::IsChargeable))
    {
      ADD_MESSAGE("You have nothing to charge.");
      return false;
    }
  else
    {
      Reader->StartReading(this, GetReadDifficulty());
      return true;
    }
}

void scrollofcharging::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::IsChargeable) && !Reader->EquipsSomething(&item::IsChargeable))
    ADD_MESSAGE("You have lost whatever you wished to charge.");
  else
    for(;;)
      {
	itemvector Item;
	Reader->SelectFromPossessions(Item, CONST_S("Which item do you wish to charge?"), NO_MULTI_SELECT|SELECT_PAIR, &item::IsChargeable);

	if(!Item.empty())
	  {
	    if(Item[0]->HandleInPairs() && Item.size() == 1)
	      {
		ADD_MESSAGE("Only one %s will be charged.", Item[0]->CHAR_NAME(UNARTICLED));

		if(!game::BoolQuestion(CONST_S("Still continue? [y/N]")))
		  continue;
	      }

	    for(uint c = 0; c < Item.size(); ++c)
	      Item[c]->ChargeFully(Reader);

	    ADD_MESSAGE("You charge %s and the scroll burns.", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
	    break;
	  }
	else if(game::BoolQuestion(CONST_S("Really cancel read? [y/N]")))
	  return;
      }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
}

void bananapeels::StepOnEffect(character* Stepper)
{
  if(Stepper->HasFeet() && !(RAND() % 5))
    {
      if(Stepper->IsPlayer())
	ADD_MESSAGE("Auch. Your feet slip on %s and you fall down.", CHAR_NAME(INDEFINITE));
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s steps on %s and falls down.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));

      /* Do damage against any random bodypart except legs */

      Stepper->ReceiveDamage(0, 1 + (RAND() & 1), PHYSICAL_DAMAGE, ALL&~LEGS);
      Stepper->CheckDeath(CONST_S("slipped on a banana peel"), 0);
      Stepper->EditAP(-500);
    }
}

void scrolloftaming::FinishReading(character* Reader)
{
  // First find all characters in the squares around Reader

  character* CharacterNear[8];
  int Index = 0;

  for(int c = 0; c < 8; ++c)
    {
      vector2d Test = Reader->GetPos() + game::GetMoveVector(c);

      if(GetArea()->IsValidPos(Test))
	{
	  character* Char = GetNearSquare(Test)->GetCharacter();

	  if(Char && Char->IsCharmable() && Char->GetTeam() != Reader->GetTeam())
	    CharacterNear[Index++] = Char;
	}
    }
  
  // Then pick one of the characters and set it to the same team as Reader

  if(!Index)
    {
      if(Reader->IsPlayer() || Reader->CanBeSeenByPlayer())
	ADD_MESSAGE("The scroll burns, but nothing happens.");
    }
  else
    {
      character* ToBeTamed = CharacterNear[RAND() % Index];
      ToBeTamed->ChangeTeam(Reader->GetTeam());

      if(Reader->IsPlayer())
	{
	  if(ToBeTamed->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s looks much friendlier.", ToBeTamed->CHAR_NAME(INDEFINITE));
	  else
	    ADD_MESSAGE("You notice no effect.");
	}
      else if(Reader->CanBeSeenByPlayer())
	{
	  if(ToBeTamed->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s seems to like %s far more.", ToBeTamed->CHAR_NAME(INDEFINITE), Reader->CHAR_NAME(DEFINITE));
	}
      else if(ToBeTamed->CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks like having fallen in love with something nearby.", ToBeTamed->CHAR_NAME(INDEFINITE));
    }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
}

void mine::Load(inputfile& SaveFile)
{
  materialcontainer::Load(SaveFile);
  SaveFile >> Active >> Team >> DiscoveredByTeam;
}

void mine::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << Active << Team << DiscoveredByTeam;
}

bool mine::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if((Type & (FIRE|ENERGY) && Damage && (Damage > 50 || !(RAND() % (100 / Damage)))) || (Type & (PHYSICAL_DAMAGE|SOUND) && WillExplode(0)))
    {
      festring DeathMsg = CONST_S("killed by an explosion of ");
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

void mine::StepOnEffect(character* Stepper)
{
  if(!WillExplode(Stepper))
    return;

  if(Stepper->IsPlayer())
    {
      if(Stepper->CanHear())
	ADD_MESSAGE("You hear a faint thump. You look down. You see %s.", CHAR_NAME(INDEFINITE));
      else
	ADD_MESSAGE("You sense a faint thump. You look down. You see %s.", CHAR_NAME(INDEFINITE));
    }
  else if(Stepper->CanBeSeenByPlayer())
    ADD_MESSAGE("%s steps on %s.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
  else if(GetSquareUnder()->CanBeSeenByPlayer())
    ADD_MESSAGE("Something explodes!");

  SetIsActive(false);
  SendNewDrawAndMemorizedUpdateRequest();

  if(Stepper->IsPlayer())
    game::AskForKeyPress(CONST_S("Trap activated! [press any key to continue]"));

  lsquare* Square = GetLSquareUnder();
  RemoveFromSlot();
  SendToHell();
  Square->GetLevel()->Explosion(0, "killed by a land mine", Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
}

bool key::Apply(character* User)
{
  if(User->IsPlayer())
    {
      if(!User->CanOpen())
	{
	  ADD_MESSAGE("This monster type cannot use keys.");
	  return false;
	}
      int Key;
      bool OpenableItems = User->GetStack()->SortedItems(User, &item::HasLock);

      if(OpenableItems)
	Key = game::AskForKeyPress(CONST_S("What do you wish to lock or unlock? [press a direction key, space or i]"));
      else
	Key = game::AskForKeyPress(CONST_S("What do you wish to lock or unlock? [press a direction key or space]"));

      if(Key == 'i' && OpenableItems)
	{
	  item* Item = User->GetStack()->DrawContents(User, CONST_S("What do you want to lock or unlock?"), 0, &item::IsOpenable);
	  return Item && Item->TryKey(this, User);
	}

      vector2d DirVect = game::GetDirectionVectorForKey(Key);

      if(DirVect != ERROR_VECTOR && User->GetArea()->IsValidPos(User->GetPos() + DirVect))
	return GetLevel()->GetLSquare(User->GetPos() + DirVect)->TryKey(this, User);
    }
  return true;
}

void materialcontainer::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << SecondaryMaterial;
}

void materialcontainer::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  LoadMaterial(SaveFile, SecondaryMaterial);
}

material* materialcontainer::GetMaterial(int I) const
{
  return !I ? MainMaterial : SecondaryMaterial;
}

color16 materialcontainer::GetMaterialColorB(int Frame) const
{
  if(GetSecondaryMaterial())
    return GetSecondaryMaterial()->GetColor();
  else
    return GetMaterialColorA(Frame);
}

alpha materialcontainer::GetAlphaB(int Frame) const
{
  if(GetSecondaryMaterial() && GetSecondaryMaterial()->GetAlpha() > GetAlphaA(Frame))
    return GetSecondaryMaterial()->GetAlpha();
  else
    return GetAlphaA(Frame);
}

void wand::VirtualConstructor(bool Load)
{
  if(!Load)
    {
      Charges = GetMinCharges() + RAND() % (GetMaxCharges() - GetMinCharges() + 1);
      TimesUsed = 0;
    }
}

void oillamp::VirtualConstructor(bool Load)
{
  if(!Load)
    SetInhabitedByGenie(RAND() & 1);
}

void mine::VirtualConstructor(bool)
{
  Active = false; /* this must be false */
}

bool whistle::Apply(character* Whistler) 
{
  if(!Whistler->HasHead())
    {
      ADD_MESSAGE("You must have a head to do this.");
      return false;
    }
  BlowEffect(Whistler);
  Whistler->EditAP(-1000);
  return true;
} 

void whistle::BlowEffect(character* Whistler)
{
  if(Whistler->IsPlayer())
    {
      if(Whistler->CanHear())
       ADD_MESSAGE("You produce an interesting sound.");
      else
	ADD_MESSAGE("You blow %s", CHAR_NAME(DEFINITE));
    }
  else if(Whistler->CanBeSeenByPlayer())
    {
      if(PLAYER->CanHear())
	ADD_MESSAGE("%s blows %s and produces an interesting sound.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
      else
	ADD_MESSAGE("%s blows %s.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    }
  else if(PLAYER->CanHear())
    ADD_MESSAGE("You hear a whistle playing.");

  game::CallForAttention(GetPos(), 400);
}

struct distancepair
{
  distancepair(long Distance, character* Char) : Distance(Distance), Char(Char) { }
  bool operator<(const distancepair& D) const { return Distance > D.Distance; }
  long Distance;
  character* Char;
};

void magicalwhistle::BlowEffect(character* Whistler)
{
  if(LastUsed && game::GetTick() - LastUsed < 2500)
    {
      whistle::BlowEffect(Whistler);
      return;
    }
  else 
    LastUsed = game::GetTick();

  if(Whistler->IsPlayer())
    {
      if(Whistler->CanHear())
	ADD_MESSAGE("You produce a peculiar sound.");
      else
	ADD_MESSAGE("You blow %s.", CHAR_NAME(DEFINITE));
    }
  else if(Whistler->CanBeSeenByPlayer())
    {
    if(PLAYER->CanHear())
      ADD_MESSAGE("%s blows %s and produces a peculiar sound.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    else
      ADD_MESSAGE("%s blows %s.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    }  
  else if(PLAYER->CanHear())
    ADD_MESSAGE("You hear a strange tune playing.");

  const std::list<character*>& Member = Whistler->GetTeam()->GetMember();
  std::vector<distancepair> ToSort;
  vector2d Pos = Whistler->GetPos();

  for(std::list<character*>::const_iterator i = Member.begin(); i != Member.end(); ++i)
    if((*i)->IsEnabled() && Whistler != *i)
      ToSort.push_back(distancepair((Pos - (*i)->GetPos()).GetLengthSquare(), *i));

  if(ToSort.size() > 10)
    std::sort(ToSort.begin(), ToSort.end());

  for(uint c = 0; c < 10 && c < ToSort.size(); ++c)
    ToSort[c].Char->TeleportNear(Whistler);

  game::CallForAttention(GetPos(), 400);
}

void itemcontainer::VirtualConstructor(bool Load)
{
  Contained = new stack(0, this, HIDDEN);

  if(!Load)
    {
      SetIsLocked(!!RAND_N(3));
      long ItemNumber = RAND() % (GetMaxGeneratedContainedItems() + 1);

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

      for(int c = 0; c < ItemNumber; ++c)
	{
	  item* NewItem = protosystem::BalancedCreateItem();
	  long Volume = NewItem->GetVolume();

	  if(NewItem->HandleInPairs())
	    Volume <<= 1;

	  if(NewItem->CanBeGeneratedInContainer() && (GetStorageVolume() - GetContained()->GetVolume()) >= Volume)
	    {
	      GetContained()->AddItem(NewItem);
	      NewItem->SpecialGenerationHandler();
	    }
	  else
	    delete NewItem;
	}
    }
}

bool itemcontainer::TryKey(item* Key, character* Applier)
{
  if(GetConfig() & BROKEN_LOCK)
    {
      ADD_MESSAGE("The lock is broken.");
      return true;
    }

  if(Key->CanOpenLockType(GetConfig()&LOCK_BITS))
    {
      if(IsLocked())
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You unlock %s.", CHAR_DESCRIPTION(DEFINITE));
	  else if(Applier->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s unlocks %s.", Applier->CHAR_NAME(DEFINITE), CHAR_DESCRIPTION(DEFINITE));
	}
      else
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You lock %s.", CHAR_DESCRIPTION(DEFINITE));
	  else if(Applier->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s locks %s.", Applier->CHAR_NAME(DEFINITE), CHAR_DESCRIPTION(DEFINITE));
	}

      SetIsLocked(!IsLocked());
    }
  else
    {
      if(Applier->IsPlayer())
	ADD_MESSAGE("%s doesn't fit in the lock.", Key->CHAR_NAME(DEFINITE));
      else if(Applier->CanBeSeenByPlayer())
	ADD_MESSAGE("%s tries to fit %s in the lock, but fails.", Applier->CHAR_NAME(DEFINITE), Key->CHAR_NAME(DEFINITE));
    }

  return true;
}

void materialcontainer::GenerateMaterials()
{
  int Chosen = RandomizeMaterialConfiguration();
  const fearray<long>& MMC = GetMainMaterialConfig();
  InitMaterial(MainMaterial,
	       MAKE_MATERIAL(MMC.Data[MMC.Size == 1 ? 0 : Chosen]),
	       GetDefaultMainVolume());
  const fearray<long>& SMC = GetSecondaryMaterialConfig();
  InitMaterial(SecondaryMaterial,
	       MAKE_MATERIAL(SMC.Data[SMC.Size == 1 ? 0 : Chosen]),
	       GetDefaultSecondaryVolume());
}

/* Returns true if container opens fine else false */

bool itemcontainer::Open(character* Opener)
{
  if(IsLocked())
    {
      ADD_MESSAGE("%s seems to be locked.", CHAR_NAME(DEFINITE));
      return false;
    }

  festring Question = CONST_S("Do you want to (t)ake something from or (p)ut something in this container? [t,p]");
  bool Success;

  switch(game::KeyQuestion(Question, KEY_ESC, 3, 't', 'p', KEY_ESC))
    {
    case 't':
      Success = GetContained()->TakeSomethingFrom(Opener, GetName(DEFINITE));
      break;
    case 'p':
      Success = GetContained()->PutSomethingIn(Opener, GetName(DEFINITE), GetStorageVolume(), GetID());
      break;
    default:
      return false;
    }

  if(Success)
    Opener->DexterityAction(Opener->OpenMultiplier() * 5);

  return Success;
}

void itemcontainer::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  Contained->Save(SaveFile);
  SaveFile << Locked;
}

void itemcontainer::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  Contained->Load(SaveFile);
  SaveFile >> Locked;
}

bool itemcontainer::Polymorph(character* Polymorpher, stack* CurrentStack)
{
  GetContained()->MoveItemsTo(CurrentStack);
  item::Polymorph(Polymorpher, CurrentStack);
  return true;
}

itemcontainer::~itemcontainer()
{
  delete Contained;
}

/* Victim is the stuck person, Bodypart is the index of the bodypart that the trap is stuck to and the last vector2d is just a direction vector that may - or may not - be used in the future. This function returns true if the character manages to escape */

bool beartrap::TryToUnstuck(character* Victim, int BodyPart, vector2d)
{
  if(IsBroken())
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONE_INDEX);
      return true;
    }

  if(!(RAND() % (GetBaseDamage() << 1)))
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONE_INDEX);

      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to free yourself from %s.", CHAR_NAME(DEFINITE));
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s manages to free %sself from %s.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN, CHAR_NAME(DEFINITE));

      return true;
    }

  if(!(RAND() % (GetBaseDamage() << 2)))
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONE_INDEX);
      Break(Victim);

      if(Victim->IsPlayer())
	ADD_MESSAGE("You are freed.");
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is freed.", Victim->CHAR_NAME(DEFINITE));

      return true;
    }

  if(!(RAND() % 3))
    {
      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to hurt your %s even more.", Victim->GetBodyPartName(BodyPart).CStr());
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s hurts %s %s more with %s.", Victim->CHAR_NAME(DEFINITE), Victim->GetPossessivePronoun().CStr(), Victim->GetBodyPartName(BodyPart).CStr(), CHAR_NAME(DEFINITE));

      Victim->ReceiveBodyPartDamage(0, GetBaseDamage(), PHYSICAL_DAMAGE, BodyPart, YOURSELF, false, false, false);
      Victim->CheckDeath(CONST_S("died while trying to escape from ") + GetName(INDEFINITE), 0);
      return false;
    }

  if(Victim->IsPlayer())
    ADD_MESSAGE("You are unable to escape from %s.", CHAR_NAME(DEFINITE));

  return false;
}

void beartrap::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Active >> Team >> DiscoveredByTeam;
}

void beartrap::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Active << Team << DiscoveredByTeam;
}

void beartrap::VirtualConstructor(bool)
{
  Active = false; /* this must be false */
}

void beartrap::StepOnEffect(character* Stepper)
{
  if(IsActive() && !Stepper->IsStuck() && !IsBroken())
    {
      int StepperBodyPart = Stepper->GetRandomStepperBodyPart();

      if(StepperBodyPart == NONE_INDEX)
	return;

      Stepper->SetStuckTo(this);
      Stepper->SetStuckToBodyPart(StepperBodyPart);

      if(Stepper->IsPlayer())
	ADD_MESSAGE("You step in %s and it traps your %s.", CHAR_NAME(INDEFINITE), Stepper->GetBodyPartName(StepperBodyPart).CStr());
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is trapped in %s.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));

      SetIsActive(false);
      SendNewDrawAndMemorizedUpdateRequest();

      if(Stepper->IsPlayer())
	game::AskForKeyPress(CONST_S("Trap activated! [press any key to continue]"));

      Stepper->ReceiveBodyPartDamage(0, GetBaseDamage() << 1, PHYSICAL_DAMAGE, Stepper->GetStuckToBodyPart(), YOURSELF, false, false, false);
      Stepper->CheckDeath(CONST_S("died by stepping to ") + GetName(INDEFINITE), 0);
    }
}

bool beartrap::CheckPickUpEffect(character* Picker)
{
  if(Picker->IsStuck() && Picker->GetStuckTo()->GetID() == GetID())
    {
      if(Picker->IsPlayer())
	ADD_MESSAGE("%s is tightly stuck to your %s.", CHAR_NAME(DEFINITE), Picker->GetBodyPartName(Picker->GetStuckToBodyPart()).CStr());

      return false;
    }

  SetIsActive(false);
  return true;
}

int lantern::GetSpecialFlags() const
{
  switch(GetSquarePosition())
    {
    case LEFT: return ROTATE|MIRROR;
    case DOWN: return FLIP;
    case UP: return 0;
    case RIGHT: return ROTATE; 
    }

  return 0;
}

bool stethoscope::Apply(character* Doctor) 
{
  if(!Doctor->CanUseStethoscope(true))
    return false;

  if(!Doctor->IsPlayer())
    ABORT("Doctor is not here, man, but these pills taste just as good anyway.");

  int Dir = game::DirectionQuestion(CONST_S("What do you want to inspect? [press a direction key]"), false,true);  

  if(Dir == DIR_ERROR)
    return false;

  Doctor->DexterityAction(2);
  character* Char = GetNearSquare(GetPos() + game::GetMoveVector(Dir))->GetCharacter();

  if(!Char)
    {
      ADD_MESSAGE("There's no-one here.");
      return false;
    }

  Char->DisplayStethoscopeInfo(Doctor);
  return true;
} 

void itemcontainer::CalculateVolumeAndWeight()
{
  item::CalculateVolumeAndWeight();
  Volume += Contained->GetVolume();
  Weight += Contained->GetWeight();
}

materialcontainer::~materialcontainer()
{
  delete SecondaryMaterial;
}

bool itemcontainer::ContentsCanBeSeenBy(const character* Viewer) const
{
  return GetMainMaterial()->IsTransparent() && CanBeSeenBy(Viewer);
}

bool mine::CanBeSeenBy(const character* Viewer) const
{ 
  int ViewerTeam = Viewer->GetTeam()->GetID();
  return (!IsActive() || ViewerTeam == Team || DiscoveredByTeam.find(ViewerTeam) != DiscoveredByTeam.end()) && materialcontainer::CanBeSeenBy(Viewer);
}

bool beartrap::CanBeSeenBy(const character* Viewer) const
{
  int ViewerTeam = Viewer->GetTeam()->GetID();
  return (!IsActive() || ViewerTeam == Team || DiscoveredByTeam.find(ViewerTeam) != DiscoveredByTeam.end()) && item::CanBeSeenBy(Viewer);
}

bool mine::Apply(character* User)
{
  if(User->IsPlayer() && !game::BoolQuestion(CONST_S("Are you sure you want to plant ") + GetName(DEFINITE) + "? [y/N]")) 
    return false;

  room* Room = GetRoom();

  if(Room)
    Room->HostileAction(User);

  if(User->IsPlayer())
    ADD_MESSAGE("%s is now %sactive.", CHAR_NAME(DEFINITE), IsActive() ? "in" : "");

  SetIsActive(!IsActive());
  User->DexterityAction(10);

  if(IsActive())
    {
      Team = User->GetTeam()->GetID();
      RemoveFromSlot();
      User->GetStackUnder()->AddItem(this);
    }

  return true;
}

bool beartrap::Apply(character* User)
{
  if(User->IsPlayer() && !game::BoolQuestion(CONST_S("Are you sure you want to plant ") + GetName(DEFINITE) + "? [y/N]")) 
    return false;

  room* Room = GetRoom();

  if(Room)
    Room->HostileAction(User);

  if(IsBroken())
    {
      if(User->IsPlayer())
	ADD_MESSAGE("%s is useless.", CHAR_NAME(DEFINITE));

      return false;
    }

  if(User->GetAttribute(DEXTERITY) < RAND() % 15)
    {
      int StepperBodyPart = User->GetRandomApplyBodyPart();

      if(User->IsPlayer())
	ADD_MESSAGE("Somehow you manage to trap your %s in %s.", User->GetBodyPartName(StepperBodyPart).CStr(), CHAR_NAME(DEFINITE));
      else if(User->CanBeSeenByPlayer())
	ADD_MESSAGE("%s somehow traps %sself in %s.", User->CHAR_NAME(DEFINITE), User->CHAR_OBJECT_PRONOUN, CHAR_NAME(DEFINITE));

      User->SetStuckTo(this);
      User->SetStuckToBodyPart(StepperBodyPart);
      SendNewDrawAndMemorizedUpdateRequest();
      RemoveFromSlot();
      User->GetStackUnder()->AddItem(this);

      if(User->IsPlayer())
	game::AskForKeyPress(CONST_S("Trap activated! [press any key to continue]"));

      User->ReceiveBodyPartDamage(0, 1 + (RAND() & 1), PHYSICAL_DAMAGE, User->GetStuckToBodyPart(), YOURSELF, false, false, false);
      User->CheckDeath(CONST_S("died failing to set ") + GetName(INDEFINITE), 0);
    }
  else
    {
      if(User->IsPlayer())
	ADD_MESSAGE("%s is now %sactive.", CHAR_NAME(DEFINITE), IsActive() ? "in" : "");

      SetIsActive(!IsActive());
      User->DexterityAction(10);

      if(IsActive())
	{
	  Team = User->GetTeam()->GetID();
	  RemoveFromSlot();
	  User->GetStackUnder()->AddItem(this);
	}
    }

  return true;
}

vector2d beartrap::GetBitmapPos(int Frame) const
{
  if(!IsBroken())
    return IsActive() ? vector2d(32, 304) : vector2d(32, 320);
  else
    return item::GetBitmapPos(Frame);
}

bool mine::WillExplode(const character* Stepper) const
{
  return IsActive() && GetSecondaryMaterial() && GetSecondaryMaterial()->IsExplosive() && (!Stepper || Stepper->GetWeight() > 5000);
}

materialcontainer::materialcontainer(const materialcontainer& MC) : item(MC)
{
  CopyMaterial(MC.SecondaryMaterial, SecondaryMaterial);
}

itemcontainer::itemcontainer(const itemcontainer& Container) : item(Container), Locked(Container.Locked)
{
  Contained = new stack(0, this, HIDDEN);
}

oillamp::oillamp(const oillamp& Lamp) : item(Lamp), InhabitedByGenie(false)
{
}

bool wand::Zap(character* Zapper, vector2d, int Direction)
{
  if(Charges <= TimesUsed)
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Zapper->EditExperience(PERCEPTION, 100, 1 << 10);

  beamdata Beam
  (
    Zapper,
    CONST_S("killed by ") + GetName(INDEFINITE),
    Zapper->GetPos(),
    GetBeamColor(),
    GetBeamEffect(),
    Direction,
    GetBeamRange(),
    GetSpecialParameters()
  );

  (GetLevel()->*level::GetBeam(GetBeamStyle()))(Beam);
  ++TimesUsed;
  return true;
}

void wand::AddInventoryEntry(const character*, festring& Entry, int, bool ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
    {
      Entry << " [" << GetWeight();

      if(TimesUsed == 1)
	Entry << "g, used 1 time]";
      else if(TimesUsed)
	Entry << "g, used " << TimesUsed << " times]";
      else
	Entry << "g]";
    }
}

void materialcontainer::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == MainMaterial)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s becomes so spoiled that it cannot hold its contents anymore.", CHAR_NAME(DEFINITE));

      RemoveMainMaterial();
    }
  else
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The contents of %s spoil completely.", CHAR_NAME(DEFINITE));

      delete RemoveSecondaryMaterial();
    }
}

bool materialcontainer::CanBePiledWith(const item* Item, const character* Viewer) const
{
  if(!item::CanBePiledWith(Item, Viewer))
    return false;

  const materialcontainer* Weapon = static_cast<const materialcontainer*>(Item);

  if(!SecondaryMaterial && !Weapon->SecondaryMaterial)
    return true;

  return SecondaryMaterial
      && Weapon->SecondaryMaterial
      && SecondaryMaterial->IsSameAs(Weapon->SecondaryMaterial)
      && SecondaryMaterial->GetSpoilLevel() == Weapon->SecondaryMaterial->GetSpoilLevel();
}

long itemcontainer::GetTruePrice() const
{
  return GetContained()->GetTruePrice() + item::GetTruePrice();
}

void potion::Break(character* Breaker, int Dir)
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s shatters to pieces.", CHAR_NAME(DEFINITE));
  else if(PLAYER->CanHear())
    ADD_MESSAGE("You hear something shattering.", CHAR_NAME(DEFINITE));

  if(Breaker && IsOnGround())
    {
      room* Room = GetRoom();

      if(Room)
	Room->HostileAction(Breaker);
    }

  item* Remains = new brokenbottle(0, NO_MATERIALS);
  Remains->InitMaterials(GetMainMaterial()->Clone());
  DonateFluidsTo(Remains);
  DonateIDTo(Remains);
  DonateSlotTo(Remains);
  SendToHell();

  if(GetSecondaryMaterial() && GetSecondaryMaterial()->IsLiquid())
    {
      liquid* Liquid = static_cast<liquid*>(GetSecondaryMaterial());

      if(Dir != YOURSELF)
	{
	  vector2d Pos = Remains->GetPos() + game::GetMoveVector(Dir);

	  if(Remains->GetLevel()->IsValidPos(Pos))
	    {
	      long HalfVolume = GetSecondaryMaterial()->GetVolume() >> 1;
	      Liquid->EditVolume(-HalfVolume);
	      Remains->GetNearLSquare(Pos)->SpillFluid(Breaker, Liquid->CloneLiquid(HalfVolume));
	    }
	}

      if(Remains->Exists())
	Remains->GetLSquareUnder()->SpillFluid(Breaker, Liquid->CloneLiquid(Liquid->GetVolume()));
    }

  if(PLAYER->Equips(Remains))
    game::AskForKeyPress(CONST_S("Equipment broken! [press any key to continue]"));
}

void materialcontainer::Be()
{
  item::Be();

  if(Exists() && SecondaryMaterial)
    SecondaryMaterial->Be();
}

bool materialcontainer::IsSparkling(int I) const
{
  return (I == 0 && MainMaterial->IsSparkling()) || (I == 1 && SecondaryMaterial && SecondaryMaterial->IsSparkling());
}

bool scrollofenchantweapon::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::IsWeapon) && !Reader->EquipsSomething(&item::IsWeapon))
    {
      ADD_MESSAGE("You have nothing to enchant.");
      return false;
    }
  else
    {
      Reader->StartReading(this, GetReadDifficulty());
      return true;
    }
}

void scrollofenchantweapon::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::IsWeapon) && !Reader->EquipsSomething(&item::IsWeapon))
    ADD_MESSAGE("You notice you have lost anything you wished to enchant.");
  else
    {
      for(;;)
	{
	  itemvector Item;
	  Reader->SelectFromPossessions(Item, CONST_S("Choose a weapon to enchant:"), NO_MULTI_SELECT|SELECT_PAIR, &item::IsWeapon);

	  if(!Item.empty())
	    {
	      if(!Item[0]->CanBeEnchanted())
		{
		  ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
		  break;
		}

	      if(Item[0]->HandleInPairs() && Item.size() == 1)
		{
		  ADD_MESSAGE("Only one %s will be enchanted.", Item[0]->CHAR_NAME(UNARTICLED));

		  if(!game::BoolQuestion(CONST_S("Still continue? [y/N]")))
		    continue;
		}

	      if(Item[0]->GetEnchantment() >= 5 && RAND() % (Item[0]->GetEnchantment() - 3))
		{
		  if(Item.size() == 1)
		    ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant it further!", Item[0]->CHAR_NAME(DEFINITE));
		  else
		    ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant them further!", Item[0]->CHAR_NAME(DEFINITE|PLURAL));

		  break;
		}

	      if(Item.size() == 1)
		ADD_MESSAGE("Your %s glows briefly red. It feels very warm now.", Item[0]->CHAR_NAME(UNARTICLED));
	      else
		ADD_MESSAGE("Your %s glow briefly red. They feel very warm now.", Item[0]->CHAR_NAME(PLURAL));

	      for(uint c = 0; c < Item.size(); ++c)
		Item[c]->EditEnchantment(1);

	      break;
	    }
	  else if(game::BoolQuestion(CONST_S("Really cancel read? [y/N]")))
	    return;
	}

      RemoveFromSlot();
      SendToHell();
      Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
    }
}

bool scrollofenchantarmor::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::IsArmor) && !Reader->EquipsSomething(&item::IsArmor))
    {
      ADD_MESSAGE("You have nothing to enchant.");
      return false;
    }
  else
    {
      Reader->StartReading(this, GetReadDifficulty());
      return true;
    }
}

void scrollofenchantarmor::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::IsArmor) && !Reader->EquipsSomething(&item::IsArmor))
    ADD_MESSAGE("You notice you have lost anything you wished to enchant.");
  else
    {
      for(;;)
	{
	  itemvector Item;
	  Reader->SelectFromPossessions(Item, CONST_S("Choose an armor to enchant:"), NO_MULTI_SELECT|SELECT_PAIR, &item::IsArmor);

	  if(!Item.empty())
	    {
	      if(!Item[0]->CanBeEnchanted())
		{
		  ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
		  break;
		}

	      if(Item[0]->HandleInPairs() && Item.size() == 1)
		{
		  ADD_MESSAGE("Only one %s will be enchanted.", Item[0]->CHAR_NAME(UNARTICLED));

		  if(!game::BoolQuestion(CONST_S("Still continue? [y/N]")))
		    continue;
		}

	      if(Item[0]->GetEnchantment() >= 5 && RAND() % (Item[0]->GetEnchantment() - 3))
		{
		  if(Item.size() == 1)
		    ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant it further!", Item[0]->CHAR_NAME(DEFINITE));
		  else
		    ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant them further!", Item[0]->CHAR_NAME(DEFINITE|PLURAL));

		  break;
		}

	      if(Item.size() == 1)
		ADD_MESSAGE("Your %s glows briefly blue. It feels very warm now.", Item[0]->CHAR_NAME(UNARTICLED));
	      else
		ADD_MESSAGE("Your %s glow briefly blue. They feel very warm now.", Item[0]->CHAR_NAME(PLURAL));

	      for(uint c = 0; c < Item.size(); ++c)
		Item[c]->EditEnchantment(1);

	      break;
	    }
	  else if(game::BoolQuestion(CONST_S("Really cancel read? [y/N]")))
	    return;
	}

      RemoveFromSlot();
      SendToHell();
      Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
    }
}

bool itemcontainer::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(Type & (PHYSICAL_DAMAGE|SOUND))
    {
      Contained->ReceiveDamage(Damager, Damage / GetDamageDivider(), Type);
      int SV = Max(GetStrengthValue(), 1);

      if(IsLocked() && Damage > SV && RAND() % (100 * Damage / SV) >= 100)
	{
	  SetIsLocked(false);
	  SetConfig(GetConfig()&~LOCK_BITS|BROKEN_LOCK);

	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The %s's lock shatters to pieces.", GetNameSingular().CStr());

	  return true;
	}
      else if(Damager && Damager->IsPlayer())
	ADD_MESSAGE("THUMP!");
    }

  return false;
}

void itemcontainer::DrawContents(const character* Char)
{
  festring Topic = CONST_S("Contents of your ") + GetName(UNARTICLED);
  GetContained()->DrawContents(Char, Topic, NO_SELECT);

  for(stackiterator i = GetContained()->GetBottom(); i.HasItem(); ++i)
    i->DrawContents(Char);
}

void magicalwhistle::Save(outputfile& SaveFile) const
{
  whistle::Save(SaveFile);
  SaveFile << LastUsed;
}

void magicalwhistle::Load(inputfile& SaveFile)
{
  whistle::Load(SaveFile);
  SaveFile >> LastUsed;
}

void magicalwhistle::VirtualConstructor(bool Load)
{
  whistle::VirtualConstructor(Load);
  LastUsed = 0;
}

int materialcontainer::GetSpoilLevel() const
{
  return Max(MainMaterial->GetSpoilLevel(), SecondaryMaterial ? SecondaryMaterial->GetSpoilLevel() : 0);
}

void itemcontainer::SetItemsInside(const fearray<contentscript<item> >& ItemArray, int SpecialFlags)
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

bool mine::CheckPickUpEffect(character*)
{
  if(WillExplode(0))
    {
      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(0, "tried to pick up an active land mine", Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
      return false;
    }

  return true;
}

bool scrollofrepair::Read(character* Reader)
{
  if(!Reader->HasRepairableBodyParts() && !Reader->GetStack()->SortedItems(Reader, &item::IsRepairable) && !Reader->EquipsSomething(&item::IsRepairable))
    {
      ADD_MESSAGE("You have nothing to repair.");
      return false;
    }

  Reader->StartReading(this, GetReadDifficulty());
  return true;
}

void scrollofrepair::FinishReading(character* Reader)
{
  if(!Reader->HasRepairableBodyParts() && !Reader->GetStack()->SortedItems(Reader, &item::IsRepairable) && !Reader->EquipsSomething(&item::IsRepairable))
    ADD_MESSAGE("You have lost whatever you wished to repair.");
  else
    {
      for(int c = 0; c < Reader->GetBodyParts(); ++c)
	{
	  bodypart* BodyPart = Reader->GetBodyPart(c);

	  if(BodyPart)
	    {
	      if(BodyPart->IsRepairable() && game::BoolQuestion("Would you like to repair your " + BodyPart->GetBodyPartName() + "? [y/N]"))
		{
		  BodyPart->RestoreHP();
		  RemoveFromSlot();
		  SendToHell();  
		  return;
		}
	    }
	}
      
      for(;;)
	{
	  itemvector Item;
	  Reader->SelectFromPossessions(Item, CONST_S("Which item do you wish to repair?"), NO_MULTI_SELECT|SELECT_PAIR, &item::IsRepairable);

	  if(!Item.empty())
	    {
	      if(Item[0]->HandleInPairs() && Item.size() == 1)
		{
		  ADD_MESSAGE("Only one %s will be repaired.", Item[0]->CHAR_NAME(UNARTICLED));

		  if(!game::BoolQuestion(CONST_S("Still continue? [y/N]")))
		    continue;
		}

	      if(Item.size() == 1)
		ADD_MESSAGE("As you read the scroll, %s glows green and %s.", Item[0]->CHAR_NAME(DEFINITE), Item[0]->IsBroken() ? "fixes itself" : "its rust vanishes");
	      else
		ADD_MESSAGE("As you read the scroll, %s glow green and %s.", Item[0]->CHAR_NAME(PLURAL), Item[0]->IsBroken() ? "fix themselves" : "their rust vanishes");

	      for(uint c = 0; c < Item.size(); ++c)
		{
		  Item[c]->RemoveRust();
		  Item[c]->Fix();
		}

	      break;
	    }
	  else if(game::BoolQuestion(CONST_S("Really cancel read? [y/N]")))
	    return;
	}
    }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
}

item* brokenbottle::Fix()
{
  potion* Potion = new potion(0, NO_MATERIALS);
  Potion->InitMaterials(GetMainMaterial(), 0);
  DonateFluidsTo(Potion);
  DonateIDTo(Potion);
  DonateSlotTo(Potion);
  SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  SendToHell();
  return Potion;
}

bool encryptedscroll::Read(character*)
{
  ADD_MESSAGE("You could never hope to decipher this top secret message. It is meant for Petrus's eyes only.");
  return false;
}

bool horn::Apply(character* Blower) 
{
  if(!Blower->HasHead())
    {
      if(Blower->IsPlayer())
	ADD_MESSAGE("You need a head to do this.");
      return false;
    }

  if(!LastUsed || game::GetTick() - LastUsed >= 2500)
    {
      LastUsed = game::GetTick();
      const char* SoundDescription = GetConfig() == BRAVERY ? "loud but calming" : "frightening, almost scream-like";

      if(Blower->IsPlayer())
	{
	  if(Blower->CanHear())
	    ADD_MESSAGE("You produce a %s sound.", SoundDescription);
	  else
	    ADD_MESSAGE("You blow %s.", CHAR_NAME(DEFINITE));
	}
      else if(Blower->CanBeSeenByPlayer())
	{
	  if(PLAYER->CanHear())
	    ADD_MESSAGE("%s blows %s and produces a %s sound.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE), SoundDescription);
	  else
	    ADD_MESSAGE("%s blows %s.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
	}
      else if(PLAYER->CanHear())
	ADD_MESSAGE("You hear a %s sound echoing everywhere.", SoundDescription);

      rect Rect;
      femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), GetPos(), 10);

      for(int x = Rect.X1; x <= Rect.X2; ++x)
	for(int y = Rect.Y1; y <= Rect.Y2; ++y)
	  {
	    character* Audience = GetNearSquare(x, y)->GetCharacter();

	    if(Audience)
	      {
		if(GetConfig() == BRAVERY && Audience->CanHear() && Audience->TemporaryStateIsActivated(PANIC)
		   && Blower->IsAlly(Audience))
		  {
		    if(Audience->IsPlayer())
		      ADD_MESSAGE("You calm down.");
		    else if(CanBeSeenByPlayer())
		      ADD_MESSAGE("%s calms down.", Audience->CHAR_NAME(DEFINITE));

		    Audience->DeActivateTemporaryState(PANIC);
		  }
		else if(GetConfig() == FEAR && !Audience->TemporaryStateIsActivated(PANIC) 
			&& Blower->GetRelation(Audience) == HOSTILE && Audience->HornOfFearWorks())
		  Audience->BeginTemporaryState(PANIC, 500 + RAND() % 500);
	      }
	  }

    }
  else
    {
      if(Blower->IsPlayer())
	{
	  if(Blower->CanHear())
	    ADD_MESSAGE("You produce a mighty sound.");
	  else
	    ADD_MESSAGE("You blow %s.", CHAR_NAME(DEFINITE));
	}
      else if(Blower->CanBeSeenByPlayer())
	{
	  if(PLAYER->CanHear())
	    ADD_MESSAGE("%s blows %s and produces a mighty sound.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s blows %s.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
	}
      else if(PLAYER->CanHear())
	ADD_MESSAGE("You hear a horn being blown.");
    }

  game::CallForAttention(GetPos(), 900);
  Blower->EditAP(-1000);
  return true;
}

void horn::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LastUsed;
}

void horn::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LastUsed;
}

void horn::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  LastUsed = 0;
}

item* bananapeels::BetterVersion() const
{
  return new banana;
}

bool beartrap::IsPickable(character* Picker) const
{
  return !IsActive() && (!Picker->GetStuckTo() || Picker->GetStuckTo()->GetID() != GetID());
}

void banana::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << TimesUsed << Charges;
}

void banana::Load(inputfile& SaveFile)
{
  materialcontainer::Load(SaveFile);
  SaveFile >> TimesUsed >> Charges;
}

bool banana::Zap(character*, vector2d, int)
{
  if(IsBroken())
    {
      ADD_MESSAGE("This banana is disfunctional.");
      return false;
    }

  if(Charges > TimesUsed)
    {
      ADD_MESSAGE("BANG! You zap %s!", CHAR_NAME(DEFINITE));
      ++TimesUsed;
    }
  else
    ADD_MESSAGE("Click!");

  return true;
}

void banana::VirtualConstructor(bool Load)
{
  materialcontainer::VirtualConstructor(Load);

  if(!Load)
    {
      Charges = 6;
      TimesUsed = 0;
    }
}

void banana::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == GetSecondaryMaterial() && !GetMainMaterial()->IsVeryCloseToSpoiling())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The inside of %s spoils completely.", CHAR_NAME(DEFINITE));

      delete RemoveSecondaryMaterial();
    }
  else
    item::SignalSpoil(Material);
}

bool bone::DogWillCatchAndConsume(const character* Doggie) const
{
  return GetConsumeMaterial(Doggie)->GetConfig() == BONE && !GetConsumeMaterial(Doggie)->GetSpoilLevel();
}

bool stone::ShowMaterial() const
{
  return GetMainMaterial()->GetConfig() != STONE; // gum solution
}

int itemcontainer::GetOfferValue(int Receiver) const
{
  int Sum = 0;

  for(int c = 0; c < GetContained()->GetItems(); ++c)
    Sum += GetContained()->GetItem(c)->GetOfferValue(Receiver);

  return item::GetOfferValue(Receiver) + Sum;
}

bool itemcontainer::IsDestroyable(const character* Char) const
{
  for(int c = 0; c < GetContained()->GetItems(); ++c)
    if(!GetContained()->GetItem(c)->IsDestroyable(Char))
      return false;

  return true;
}

alpha lantern::GetAlphaB(int Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 1);
}

alpha lantern::GetAlphaC(int Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 2);
}

alpha lantern::GetAlphaD(int Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 3);
}

void itemcontainer::SortAllItems(itemvector& AllItems, const character* Character, sorter Sorter) const
{
  if(Sorter == 0 || (this->*Sorter)(Character))
    AllItems.push_back(const_cast<itemcontainer*>(this));

  GetContained()->SortAllItems(AllItems, Character, Sorter);
}

int materialcontainer::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : SecondaryMaterial ? SecondaryMaterial->GetAttachedGod() : 0;
}

void beartrap::Search(const character* Char, int Perception)
{
  int ViewerTeam = Char->GetTeam()->GetID();

  if(IsActive() && ViewerTeam != Team && DiscoveredByTeam.find(ViewerTeam) == DiscoveredByTeam.end() && !RAND_N(200 / Perception))
    {
      DiscoveredByTeam.insert(ViewerTeam);
      SendNewDrawAndMemorizedUpdateRequest();

      if(Char->IsPlayer())
	{
	  game::AskForKeyPress(CONST_S("Trap found! [press any key to continue]"));
	  ADD_MESSAGE("You find %s.", CHAR_NAME(INDEFINITE));
	}
    }
}

void mine::Search(const character* Char, int Perception)
{
  int ViewerTeam = Char->GetTeam()->GetID();

  if(IsActive() && ViewerTeam != Team && DiscoveredByTeam.find(ViewerTeam) == DiscoveredByTeam.end() && !RAND_N(200 / Perception))
    {
      DiscoveredByTeam.insert(ViewerTeam);
      SendNewDrawAndMemorizedUpdateRequest();

      if(Char->IsPlayer())
	{
	  game::AskForKeyPress(CONST_S("Trap found! [press any key to continue]"));
	  ADD_MESSAGE("You find %s.", CHAR_NAME(INDEFINITE));
	}
    }
}

void beartrap::SetIsActive(bool What)
{
  Active = What;
  UpdatePictures();
  DiscoveredByTeam.clear();
}

void mine::SetIsActive(bool What)
{
  Active = What;
  DiscoveredByTeam.clear();
}

void wand::BreakEffect(character* Terrorist, const festring& DeathMsg)
{
  vector2d Pos = GetPos();
  level* Level = GetLevel();
  RemoveFromSlot();
  ulong StackSize = Level->AddRadiusToSquareStack(Pos, GetBreakEffectRangeSquare());
  lsquare** SquareStack = Level->GetSquareStack();
  ulong c;

  for(c = 0; c < StackSize; ++c)
    SquareStack[c]->RemoveFlags(IN_SQUARE_STACK);

  fearray<lsquare*> Stack(SquareStack, StackSize);
  (Level->*level::GetBeamEffectVisualizer(GetBeamStyle()))(Stack, GetBeamColor());

  beamdata Beam
  (
    Terrorist,
    DeathMsg,
    YOURSELF,
    GetSpecialParameters()
  );

  for(c = 0; c < Stack.Size; ++c)
    (Stack[c]->*lsquare::GetBeamEffect(GetBeamEffect()))(Beam);

  SendToHell();
}

bool beartrap::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(!IsStuck() && !IsBroken() && Type & PHYSICAL_DAMAGE && Damage)
    {
      if(Damage > 125 || !(RAND() % (250 / Damage)))
	{
	  SetIsActive(false);
	  Break(Damager);	  
	  return true;
	}
      else
	{
	  if(IsActive())
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s snaps shut.", CHAR_NAME(DEFINITE));

	      SetIsActive(false);
	      SendNewDrawAndMemorizedUpdateRequest();  
	      return true;
	    }
	}
    }

  return false;
}

bool potion::ReceiveDamage(character* Damager, int Damage, int Type, int Dir)
{
  if(Type & FIRE && Damage && IsExplosive() && (Damage > 50 || !(RAND() % (100 / Damage))))
    {
      festring DeathMsg = CONST_S("killed by an explosion of ");
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
      return true;
    }

  if(Type & THROW)
    {
      int StrengthValue = GetStrengthValue();

      if(!StrengthValue)
	StrengthValue = 1;

      if(Damage > StrengthValue << 2 && RAND() % (50 * Damage / StrengthValue) >= 100)
	{
	  Break(Damager, Dir);
	  return true;
	}
    }

  return item::ReceiveDamage(Damager, Damage, Type, Dir);
}

bool beartrap::IsStuck() const
{
  character* Char = GetLSquareUnder()->GetCharacter();
  return Char && Char->GetStuckTo() && Char->GetStuckTo()->GetID() == GetID();
}

void beartrap::Fly(character* Thrower, int Direction, int Force)
{
  if(!IsStuck())
    item::Fly(Thrower, Direction, Force);
}

void can::DipInto(liquid* Liquid, character* Dipper)
{
  /* Add alchemy */

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now filled with %s.", CHAR_NAME(DEFINITE), Liquid->GetName(false, false).CStr());

  ChangeSecondaryMaterial(Liquid);
  Dipper->DexterityAction(10);
}

bool holybanana::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, int BodyPartIndex, int Direction, bool BlockedByArmour)
{
  bool BaseSuccess = banana::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(Enemy->IsEnabled() && RAND() & 1)
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s banana burns %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

      return Enemy->ReceiveBodyPartDamage(Hitter, 2 + (RAND() & 3), FIRE, BodyPartIndex, Direction) || BaseSuccess;
    }
  else
    return BaseSuccess;
}

bool holybanana::Zap(character* Zapper, vector2d, int Direction)
{
  if(Charges > TimesUsed)
    {
      ADD_MESSAGE("BANG! You zap %s!", CHAR_NAME(DEFINITE));
      Zapper->EditExperience(PERCEPTION, 10, 1 << 10);

      beamdata Beam
      (
	Zapper,
	CONST_S("killed by ") + GetName(INDEFINITE),
	Zapper->GetPos(),
	YELLOW,
	BEAM_FIRE_BALL,
	Direction,
	50,
	0
      );

      (GetLevel()->*level::GetBeam(PARTICLE_BEAM))(Beam);
      ++TimesUsed;
    }
  else
    ADD_MESSAGE("Click!");

  return true;
}

void holybanana::AddInventoryEntry(const character* Viewer, festring& Entry, int, bool ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
    {
      Entry << " [" << GetWeight() << "g, DAM " << GetBaseMinDamage() << '-' << GetBaseMaxDamage();
      int DamageBonus = int(GetDamageBonus());

      if(DamageBonus)
	{
	  if(DamageBonus > 0)
	    Entry << '+';

	  Entry << DamageBonus;
	}

      Entry << ", " << GetBaseToHitValueDescription();

      if(!IsBroken())
	Entry << ", " << GetStrengthValueDescription();

      int CWeaponSkillLevel = Viewer->GetCWeaponSkillLevel(this);
      int SWeaponSkillLevel = Viewer->GetSWeaponSkillLevel(this);

      if(CWeaponSkillLevel || SWeaponSkillLevel)
	Entry << ", skill " << CWeaponSkillLevel << '/' << SWeaponSkillLevel;

      if(TimesUsed == 1)
	Entry << ", used 1 time";
      else if(TimesUsed)
	Entry << ", used " << TimesUsed << " times";

      Entry << ']';
    }
}

bool holybanana::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(TimesUsed != 6 && Type & (PHYSICAL_DAMAGE|FIRE|ENERGY) && Damage && (Damage > 50 || !(RAND() % (100 / Damage))))
    {
      festring DeathMsg = CONST_S("killed by an explosion of ");
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), (6 - TimesUsed) * 50);
      return true;
    }

  return false;
}

void itemcontainer::PreProcessForBone()
{
  item::PreProcessForBone();
  Contained->PreProcessForBone();
}

void itemcontainer::PostProcessForBone()
{
  item::PostProcessForBone();
  Contained->PostProcessForBone();
}

void itemcontainer::FinalProcessForBone()
{
  item::FinalProcessForBone();
  Contained->FinalProcessForBone();
}

void mine::FinalProcessForBone()
{
  materialcontainer::FinalProcessForBone();

  if(Team == PLAYER_TEAM)
    Team = MONSTER_TEAM;

  std::set<int>::iterator i = DiscoveredByTeam.find(PLAYER_TEAM);

  if(i != DiscoveredByTeam.end())
    DiscoveredByTeam.erase(i);
}

void beartrap::FinalProcessForBone()
{
  item::FinalProcessForBone();

  if(Team == PLAYER_TEAM)
    Team = MONSTER_TEAM;

  std::set<int>::iterator i = DiscoveredByTeam.find(PLAYER_TEAM);

  if(i != DiscoveredByTeam.end())
    DiscoveredByTeam.erase(i);
}

void magicalwhistle::FinalProcessForBone()
{
  whistle::FinalProcessForBone();
  LastUsed = 0;
}

void horn::FinalProcessForBone()
{
  item::FinalProcessForBone();
  LastUsed = 0;
}

bool charmlyre::Apply(character* Charmer)
{
  if(LastUsed && game::GetTick() - LastUsed < 10000)
    {
      if(Charmer->IsPlayer())
	{
	  if(Charmer->CanHear())
	    ADD_MESSAGE("You produce a highly alluring sound.");
	  else
	    ADD_MESSAGE("You try to play something with the %s, but it is hard when you can't hear.", CHAR_NAME(DEFINITE));
	}
      else if(Charmer->CanBeSeenByPlayer())
	{
	  if(PLAYER->CanHear())
	    ADD_MESSAGE("%s plays %s and produces a highly alluring sound.", Charmer->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s plays %s.", Charmer->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
	}
      else if(PLAYER->CanHear())
	ADD_MESSAGE("You hear a lyre playing.");
    }
  else
    {
      LastUsed = game::GetTick();
      if(Charmer->IsPlayer())
	{
	  if(Charmer->CanHear())
	    ADD_MESSAGE("You produce a mesmerizing sound.");
	  else
	    ADD_MESSAGE("You try to play something with the %s, but it is hard when you can't hear.", CHAR_NAME(DEFINITE));
	}
      else if(Charmer->CanBeSeenByPlayer())
	{
	  if(PLAYER->CanHear())
	    ADD_MESSAGE("%s plays %s and produces a mesmerizing sound.", Charmer->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s plays %s.", Charmer->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
	}

      for(int d = 0; d < Charmer->GetNeighbourSquares(); ++d)
	{
	  square* Square = Charmer->GetNeighbourSquare(d);

	  if(Square)
	    {
	      character* Char = Square->GetCharacter();

	      if(Char)
		if(Char->IsCharmable() && Char->CanHear())
		  {
		    if(Charmer->GetRelativeDanger(Char) > 4.0)
		      {
			if(Char->GetTeam() == Charmer->GetTeam())
			  ADD_MESSAGE("%s seems to be very happy.", Char->CHAR_NAME(DEFINITE));
			else if(Char->GetRelation(Charmer) == HOSTILE)
			  ADD_MESSAGE("%s stops fighting.", Char->CHAR_NAME(DEFINITE));
			else
			  ADD_MESSAGE("%s seems to be very friendly towards you.", Char->CHAR_NAME(DEFINITE));

			Char->ChangeTeam(Charmer->GetTeam());
		      }
		    else
		      ADD_MESSAGE("%s resists its charming call.", Char->CHAR_NAME(DEFINITE));
		  }
		else
		  ADD_MESSAGE("%s seems not affected.", Char->CHAR_NAME(DEFINITE));
	    }
	}
    }

  Charmer->EditAP(-1000);
  game::CallForAttention(GetPos(), 100);
  return true;
}

void charmlyre::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LastUsed;
}

void charmlyre::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LastUsed;
}

void charmlyre::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  LastUsed = 0;
}

void charmlyre::FinalProcessForBone()
{
  item::FinalProcessForBone();
  LastUsed = 0;
}

bool carrot::BunnyWillCatchAndConsume(const character* Bunny) const
{
  return GetConsumeMaterial(Bunny)->GetConfig() == CARROT_FLESH && !GetConsumeMaterial(Bunny)->GetSpoilLevel();
}

void beartrap::TeleportRandomly()
{
  Team = NO_TEAM;
  DiscoveredByTeam.clear();
  item::TeleportRandomly();
}

void mine::TeleportRandomly()
{
  Team = NO_TEAM;
  DiscoveredByTeam.clear();
  item::TeleportRandomly();
}

int materialcontainer::GetRustDataB() const
{
  return SecondaryMaterial ? SecondaryMaterial->GetRustData() : GetRustDataA();
}

void backpack::SpillFluid(character* Spiller, liquid* Liquid, int SquareIndex)
{
  if(!Liquid->IsExplosive())
    {
      GetSecondaryMaterial()->AddWetness(Liquid->GetVolume() * 25);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s gets wet.", CHAR_NAME(DEFINITE));
    }

  item::SpillFluid(Spiller, Liquid, SquareIndex);
}

material* materialcontainer::GetConsumeMaterial(const character* Consumer, materialpredicate Predicate) const
{
  if(SecondaryMaterial
  && (SecondaryMaterial->*Predicate)()
  && Consumer->CanConsume(SecondaryMaterial))
    return SecondaryMaterial;
  else
    return item::GetConsumeMaterial(Consumer, Predicate);
}

material* materialcontainer::RemoveMaterial(material* Material)
{
  if(Material == MainMaterial)
    return RemoveMainMaterial();
  else
    return RemoveSecondaryMaterial();
}

material* materialcontainer::RemoveMainMaterial()
{
  bool Equipped = PLAYER->Equips(this);

  if(!SecondaryMaterial)
    RemoveFromSlot();
  else if(SecondaryMaterial->IsLiquid())
    {
      if(!game::IsInWilderness())
	{
	  lsquare* Square = GetLSquareUnder();
	  RemoveFromSlot();
	  Square->SpillFluid(0, static_cast<liquid*>(SecondaryMaterial));
	  SetSecondaryMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
	}
      else
	RemoveFromSlot();
    }
  else
    {
      item* Lump = new lump(0, NO_MATERIALS);
      Lump->InitMaterials(SecondaryMaterial);
      DonateFluidsTo(Lump);
      DonateIDTo(Lump);
      DonateSlotTo(Lump);
      SetSecondaryMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
    }

  SendToHell();

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment destroyed! [press any key to continue]"));

  return 0;
}

material* materialcontainer::RemoveSecondaryMaterial()
{
  material* Material = SecondaryMaterial;
  SetSecondaryMaterial(0);
  SendNewDrawAndMemorizedUpdateRequest();
  return Material;
}

material* banana::RemoveSecondaryMaterial()
{
  item* Peel = new bananapeels(0, NO_MATERIALS);
  Peel->InitMaterials(MainMaterial);
  DonateSlotTo(Peel);
  DonateIDTo(Peel);
  SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  SendToHell();
  return 0;
}

material* itemcontainer::RemoveMaterial(material* Material)
{
  Contained->MoveItemsTo(GetSlot());
  return item::RemoveMaterial(Material);
}

void materialcontainer::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  if(MainMaterial)
    game::CombineLights(Emitation, MainMaterial->GetEmitation());

  if(SecondaryMaterial)
    game::CombineLights(Emitation, SecondaryMaterial->GetEmitation());
}

bool materialcontainer::CalculateHasBe() const
{
  return LifeExpectancy
      || (MainMaterial && MainMaterial->HasBe())
      || (SecondaryMaterial && SecondaryMaterial->HasBe());
}

vector2d lantern::GetBitmapPos(int Frame) const
{
  return GetSquarePosition() == CENTER
       ? item::GetBitmapPos(Frame)
       : item::GetWallBitmapPos(Frame);
}

long materialcontainer::GetMaterialPrice() const
{
  return MainMaterial->GetRawPrice()
       + (SecondaryMaterial ? SecondaryMaterial->GetRawPrice() : 0);
}

void scrollofdetectmaterial::FinishReading(character* Reader)
{
  material* TempMaterial;

  for(;;)
    {
      festring Temp = game::DefaultQuestion(CONST_S("What material do you want to detect?"),
					    game::GetDefaultDetectMaterial());
      TempMaterial = protosystem::CreateMaterial(Temp);

      if(TempMaterial)
	break;
      else
	game::DrawEverythingNoBlit();
    }

  level* Level = GetLevel();
  int Squares = Level->DetectMaterial(TempMaterial);
  RemoveFromSlot();
  SendToHell();

  if(Squares > Reader->GetAttribute(INTELLIGENCE) * (25 + RAND() % 51))
    {
      ADD_MESSAGE("An enormous burst of geographical information overwhelms your consciousness. Your mind cannot cope with it and your memories blur.");
      Level->BlurMemory();
      Reader->BeginTemporaryState(CONFUSED, 1000 + RAND() % 1000);
      Reader->EditExperience(INTELLIGENCE, -100, 1 << 12);
    }
  else if(!Squares)
    {
      ADD_MESSAGE("You feel a sudden urge to imagine the dark void of a starless night sky.");
      Reader->EditExperience(INTELLIGENCE, 150, 1 << 12);
    }
  else
    {
      ADD_MESSAGE("You feel attracted to all things made of %s.", TempMaterial->GetName(false, false).CStr());
      game::PositionQuestion(CONST_S("Detecting material [direction keys move cursor, space exits]"), Reader->GetPos(), 0, 0, false);
      Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
    }

  delete TempMaterial;
  Level->CalculateLuminances();
  game::SendLOSUpdateRequest();
}

int beartrap::GetBaseDamage() const
{
  int Modifier = GetMainMaterial()->GetStrengthValue() / 50;
  Modifier *= Modifier;
  Modifier >>= 1;
  return Modifier ? Modifier + RAND() % Modifier : 1;
}

void scrollofhardenmaterial::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->GetItems() && !Reader->EquipsSomething())
    {
      ADD_MESSAGE("You notice you have lost anything you wished to alter.");
      return;
    }

  for(;;)
    {
      itemvector Item;
      Reader->SelectFromPossessions(Item, CONST_S("What item do you wish to harden?"), NO_MULTI_SELECT|SELECT_PAIR, &item::CanBeHardened);

      if(Item.empty())
	if(game::BoolQuestion(CONST_S("Really cancel read? [y/N]")))
	  return;
	else
	  continue;

      if(!Item[0]->IsMaterialChangeable())
	{
	  ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
	  break;
	}

      if(Item[0]->HandleInPairs() && Item.size() == 1)
	{
	  ADD_MESSAGE("Only one %s will be altered.", Item[0]->CHAR_NAME(UNARTICLED));

	  if(!game::BoolQuestion(CONST_S("Still continue? [y/N]")))
	    continue;
	}

      msgsystem::EnterBigMessageMode();

      if(Item.size() == 1)
	ADD_MESSAGE("Suddenly your %s is consumed in roaring magical flames.", Item[0]->CHAR_NAME(UNARTICLED));
      else
	ADD_MESSAGE("Suddenly your %s are consumed in roaring magical flames.", Item[0]->CHAR_NAME(PLURAL));

      int Config = Item[0]->GetMainMaterial()->GetHardenedMaterial();

      if(!Config)
	{
	  /* Should not be possible */

	  if(Item.size() == 1)
	    ADD_MESSAGE("But it is already as hard as it can get.");
	  else
	    ADD_MESSAGE("But they are already as hard as they can get.");

	  msgsystem::LeaveBigMessageMode();
	  break;
	}

      material* TempMaterial = MAKE_MATERIAL(Config);

      if(TempMaterial->GetIntelligenceRequirement() > Reader->GetAttribute(INTELLIGENCE))
	{
	  delete TempMaterial;
	  ADD_MESSAGE("But your mind is not yet strong enough to harden %s.", Item.size() == 1 ? "it" : "them");
	  msgsystem::LeaveBigMessageMode();
	  continue;
	}

      material* MainMaterial = Item[0]->GetMainMaterial();
      material* SecondaryMaterial = Item[0]->GetSecondaryMaterial();

      if(Item.size() == 1)
	{
	  ADD_MESSAGE("As you lift it again it looks much harder.");

	  if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
	    Item[0]->ChangeSecondaryMaterial(TempMaterial->Clone());

	  Item[0]->ChangeMainMaterial(TempMaterial);
	}
      else
	{
	  ADD_MESSAGE("As you lift them again they look much harder.");

	  if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
	    for(uint c = 0; c < Item.size(); ++c)
	      Item[c]->ChangeSecondaryMaterial(TempMaterial->Clone());

	  Item[0]->ChangeMainMaterial(TempMaterial);

	  for(uint c = 1; c < Item.size(); ++c)
	    Item[c]->ChangeMainMaterial(TempMaterial->Clone());
	}

      msgsystem::LeaveBigMessageMode();
      break;
    }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
}

void itemcontainer::SetLifeExpectancy(int Base, int RandPlus)
{
  LifeExpectancy = RandPlus ? Base + RAND_N(RandPlus) : Base;
  Enable();
  Contained->SetLifeExpectancy(Base, RandPlus);
}

ulong wand::GetSpecialParameters() const
{
  switch(GetConfig())
    {
    case WAND_OF_MIRRORING:
      return MIRROR_IMAGE|(1000 << LE_BASE_SHIFT)|(1000 << LE_RAND_SHIFT);
    }

  return 0;
}

bool scrollofgolemcreation::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::IsDestroyable) && !Reader->EquipsSomething(&item::IsDestroyable))
    {
      ADD_MESSAGE("You have nothing you can use for golem creation.");
      return false;
    }
  else
    {
      Reader->StartReading(this, GetReadDifficulty());
      return true;
    }
}

void scrollofgolemcreation::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::IsDestroyable) && !Reader->EquipsSomething(&item::IsDestroyable))
    ADD_MESSAGE("You have lost whatever you wished to change into a golem.");
  else
    for(;;)
      {
	itemvector Item;
	Reader->SelectFromPossessions(Item, CONST_S("Which item do you wish to use for golem creation?"), NO_MULTI_SELECT, &item::IsDestroyable);

	if(!Item.empty())
	  {
	    material* Main = Item[0]->GetMainMaterial();
	    material* Sec = Item[0]->GetSecondaryMaterial();
	    bool MainPossible = Main->IsGolemMaterial();
	    bool SecPossible = Sec && Sec->GetVolume()
			    && Sec->IsGolemMaterial()
			    && !Sec->IsSameAs(Main);

	    if(!MainPossible && !SecPossible)
	      {
		ADD_MESSAGE("You can't use that for golem creation.");
		continue;
	      }

	    if(MainPossible && SecPossible)
	      if(game::BoolQuestion(CONST_S("Use main material? [Y/n]"), YES))
		SecPossible = false;
	      else
		MainPossible = false;

	    int MaterialConfig = MainPossible ? Main->GetConfig() : Sec->GetConfig();
	    golem* Golem = new golem(MaterialConfig);
	    vector2d Where = GetLevel()->GetNearestFreeSquare(Golem, Reader->GetPos());
	    Item[0]->RemoveFromSlot();
	    Item[0]->SendToHell();

	    if(Where == ERROR_VECTOR)
	      {
		ADD_MESSAGE("You cast the spell and %s is sucked into a rainbow-colored magical vortex, but nothing happens.", Item[0]->CHAR_NAME(DEFINITE));
		delete Golem;
	      }
	    else
	      {
		ADD_MESSAGE("You cast the spell and %s is sucked into a rainbow-colored magical vortex.", Item[0]->CHAR_NAME(DEFINITE));
		Golem->SetTeam(Reader->GetTeam());
		Golem->PutTo(Where);

		if(Golem->CanBeSeenByPlayer())
		  ADD_MESSAGE("Suddenly %s materializes!", Golem->CHAR_NAME(INDEFINITE));

		Golem->GetLSquareUnder()->DrawParticles(RED);
	      }

	    break;
	  }
	else if(game::BoolQuestion(CONST_S("Really cancel read? [y/N]")))
	  return;
      }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
}
