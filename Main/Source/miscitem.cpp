/* Compiled through itemset.cpp */

void materialcontainer::SetContainedMaterial(material* What, ushort SpecialFlags) { SetMaterial(ContainedMaterial, What, GetDefaultContainedVolume(), SpecialFlags); }
void materialcontainer::ChangeContainedMaterial(material* What, ushort SpecialFlags) { ChangeMaterial(ContainedMaterial, What, GetDefaultContainedVolume(), SpecialFlags); }
void materialcontainer::InitMaterials(material* M1, material* M2, bool CUP) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), ContainedMaterial, M2, GetDefaultContainedVolume(), CUP); }
void materialcontainer::SetConsumeMaterial(material* NewMaterial, ushort SpecialFlags) { SetContainedMaterial(NewMaterial, SpecialFlags); }
void materialcontainer::ChangeConsumeMaterial(material* NewMaterial, ushort SpecialFlags) { ChangeContainedMaterial(NewMaterial, SpecialFlags); }

uchar holybanana::GetSpecialFlags() const { return ST_FLAME; }

vector2d lantern::GetBitmapPos(ushort Frame) const { return SquarePosition == CENTER ? item::GetBitmapPos(Frame) : item::GetWallBitmapPos(Frame); }
ushort lantern::GetMaterialColorA(ushort) const { return MakeRGB16(255, 255, 240); }
ushort lantern::GetMaterialColorB(ushort) const { return MakeRGB16(255, 255, 100); }
ushort lantern::GetMaterialColorC(ushort) const { return MakeRGB16(255, 255, 100); }
ushort lantern::GetMaterialColorD(ushort) const { return MakeRGB16(255, 255, 100); }

bool can::AddAdjective(std::string& String, bool Articled) const { return AddEmptyAdjective(String, Articled); }
vector2d can::GetBitmapPos(ushort) const { return vector2d(16, GetContainedMaterial() ? 288 : 304); }

bool potion::IsExplosive() const { return GetContainedMaterial() && GetContainedMaterial()->IsExplosive(); }
bool potion::AddAdjective(std::string& String, bool Articled) const { return AddEmptyAdjective(String, Articled); }
bool potion::EffectIsGood() const { return GetContainedMaterial() && GetContainedMaterial()->EffectIsGood(); }

ulong wand::GetPrice() const { return Charges > TimesUsed ? item::GetPrice() : 0; }

bool backpack::IsExplosive() const { return GetContainedMaterial() && GetContainedMaterial()->IsExplosive(); }
ulong backpack::GetTotalExplosivePower() const { return GetContainedMaterial() ? GetContainedMaterial()->GetTotalExplosivePower() : 0; }

ulong stone::GetTruePrice() const { return GetMainMaterial()->GetRawPrice() << 1; }

ushort whistle::GetMaterialColorB(ushort) const { return MakeRGB16(80, 32, 16); }

ushort itemcontainer::GetMaterialColorB(ushort) const { return MakeRGB16(80, 80, 80); }
void itemcontainer::AddPostFix(std::string& String) const { AddLockPostFix(String, LockType); }

bool mine::AddAdjective(std::string& String, bool Articled) const { return IsActive() && AddActiveAdjective(String, Articled); }

bool beartrap::AddAdjective(std::string& String, bool Articled) const { return (IsActive() && AddActiveAdjective(String, Articled)) || (!IsActive() && item::AddAdjective(String, Articled)); }

void potion::GenerateLeftOvers(character* Eater)
{
  ChangeConsumeMaterial(0);

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    {
      MoveTo(Eater->GetStackUnder());
      Eater->DexterityAction(1);
    }
  else
    MoveTo(Eater->GetStack());
}

bool scroll::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::SeeWholeMapCheatIsActive();
}

bool scrollofcreatemonster::Read(character* Reader)
{
  Reader->StartReading(this, 200);
  return true;
}

void scrollofcreatemonster::FinishReading(character* Reader)
{
  vector2d TryToCreate;

  for(ushort c = 0; c < 100; ++c)
    {
      TryToCreate = Reader->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);
      character* Monster = protosystem::CreateMonster();

      if(GetArea()->IsValidPos(TryToCreate) && GetNearLSquare(TryToCreate)->IsWalkable(Monster) && GetNearLSquare(TryToCreate)->GetCharacter() == 0)
	{
	  GetNearLSquare(TryToCreate)->AddCharacter(Monster);

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
}

bool scrollofteleportation::Read(character* Reader)
{
  Reader->StartReading(this, 200);
  return true;
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
}

bool lump::HitEffect(character* Enemy, character*, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour && RAND() & 1)
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("The %s touches %s!", GetMainMaterial()->GetName(false, false).c_str(), Enemy->CHAR_DESCRIPTION(DEFINITE));

      bool Success = GetMainMaterial()->HitEffect(Enemy);

      if(!GetMainMaterial()->GetVolume())
	{
	  RemoveFromSlot();
	  SendToHell();
	}

      return Success;
    }
  else
    return false;
}

material* lump::CreateDipMaterial()
{
  material* Material = GetMainMaterial()->Clone(GetMainMaterial()->TakeDipVolumeAway());

  if(!GetMainMaterial()->GetVolume())
    {
      RemoveFromSlot();
      SendToHell();
    }

  return Material;
}

bool wand::Apply(character* Terrorist)
{
  if(Terrorist->IsPlayer() && !game::BoolQuestion("Are you sure you want to break " + GetName(DEFINITE) + "? [y/N]")) 
    return false;

  BreakEffect(Terrorist, "killed by breaking " + GetName(INDEFINITE));
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

bool scrollofwishing::Read(character* Reader)
{
  Reader->StartReading(this, 2500);
  return true;
}

void scrollofwishing::FinishReading(character* Reader)
{
  std::string Temp;

  while(true)
    {
      Temp = game::StringQuestion("What do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
      item* TempItem = protosystem::CreateItem(Temp, Reader->IsPlayer());

      if(TempItem)
	{
	  Reader->GetStack()->AddItem(TempItem);
	  TempItem->SpecialGenerationHandler();

	  if(TempItem->HandleInPairs())
	    ADD_MESSAGE("Two %s appear from nothing and the scroll burns!", TempItem->CHAR_NAME(PLURAL));
	  else
	    ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CHAR_NAME(INDEFINITE));

	  RemoveFromSlot();
	  SendToHell();
	  return;
	}
    }
}

bool scrollofchangematerial::Read(character* Reader)
{
  Reader->StartReading(this, 1000);
  return true;
}

void scrollofchangematerial::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->GetItems() && !Reader->EquipsSomething())
    ADD_MESSAGE("You notice you have lost anything you wished to alter.");
  else
    {
      while(true)
	{
	  itemvector Item;
	  Reader->SelectFromPossessions(Item, "What item do you wish to change?", NO_MULTI_SELECT|SELECT_PAIR);

	  if(!Item.empty())
	    {
	      if(!Item[0]->IsMaterialChangeable())
		{
		  ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
		  break;
		}

	      if(Item[0]->HandleInPairs() && Item.size() == 1)
		{
		  ADD_MESSAGE("Only one %s will be altered.", Item[0]->CHAR_NAME(UNARTICLED));

		  if(!game::BoolQuestion("Still continue? [y/N]"))
		    continue;
		}

	      std::string Temp = game::StringQuestion("What material do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
	      material* TempMaterial = protosystem::CreateMaterial(Temp);

	      if(TempMaterial)
		{
		  material* MainMaterial = Item[0]->GetMainMaterial();
		  material* SecondaryMaterial = Item[0]->GetSecondaryMaterial();

		  if(Item.size() == 1)
		    {
		      if(!MainMaterial->IsSameAs(TempMaterial))
			{
			  ADD_MESSAGE("Suddenly your %s is consumed in roaring magical flames. As you lift it again it looks greatly altered.", Item[0]->CHAR_NAME(UNARTICLED));

			  if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
			    Item[0]->ChangeSecondaryMaterial(TempMaterial->Clone());

			  Item[0]->ChangeMainMaterial(TempMaterial);
			}
		      else
			ADD_MESSAGE("Suddenly your %s is consumed in roaring magical flames. As you lift it again it looks unchanged.", Item[0]->CHAR_NAME(UNARTICLED));
		    }
		  else
		    {
		      if(!MainMaterial->IsSameAs(TempMaterial))
			{
			  ADD_MESSAGE("Suddenly your %s are consumed in roaring magical flames. As you lift them again they look greatly altered.", Item[0]->CHAR_NAME(PLURAL));

			  if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
			    for(ushort c = 0; c < Item.size(); ++c)
			      Item[c]->ChangeSecondaryMaterial(TempMaterial->Clone());

			  Item[0]->ChangeMainMaterial(TempMaterial);

			  for(ushort c = 1; c < Item.size(); ++c)
			    Item[c]->ChangeMainMaterial(TempMaterial->Clone());
			}
		      else
			ADD_MESSAGE("Suddenly your %s are consumed in roaring magical flames. As you lift them again they look unchanged.", Item[0]->CHAR_NAME(PLURAL));
		    }

		  break;
		}
	      else
		game::DrawEverythingNoBlit();
	    }
	  else if(game::BoolQuestion("Really cancel read? [y/N]"))
	    return;
	}

      RemoveFromSlot();
      SendToHell();
    }
}

item* brokenbottle::BetterVersion() const
{
  material* Stuff;

  if(RAND() % 5)
    Stuff = MAKE_MATERIAL(BANANA_FLESH);
  else
    Stuff = MAKE_MATERIAL(OMMEL_URINE);

  potion* P = new potion(0, NO_MATERIALS);
  P->InitMaterials(MAKE_MATERIAL(GLASS), Stuff);
  return P;
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
      Stepper->CheckDeath("stepped on a broken bottle", 0);
    }
}

material* can::CreateDipMaterial()
{
  material* Material = GetContainedMaterial()->Clone(GetContainedMaterial()->TakeDipVolumeAway());

  if(!GetContainedMaterial()->GetVolume())
    Empty();

  return Material;
}

material* potion::CreateDipMaterial()
{
  material* Material = GetContainedMaterial()->Clone(GetContainedMaterial()->TakeDipVolumeAway());

  if(!GetContainedMaterial()->GetVolume())
    Empty();

  return Material;
}

void potion::DipInto(material* Material, character* Dipper)
{
  /* Add alchemy */

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now filled with %s.", CHAR_NAME(DEFINITE), Material->GetName(false, false).c_str());

  ChangeContainedMaterial(Material);
  Dipper->DexterityAction(10);
}

void lantern::SignalSquarePositionChange(uchar NewSquarePosition)
{
  if(SquarePosition != NewSquarePosition)
    {
      SquarePosition = NewSquarePosition;
      UpdatePictures();
    }
}

void lantern::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << SquarePosition;
}

void lantern::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> SquarePosition;
}

item* potion::BetterVersion() const
{
  if(!GetContainedMaterial())
    {
      material* Stuff;

      if(RAND() % 5)
	Stuff = MAKE_MATERIAL(BANANA_FLESH);
      else
	Stuff = MAKE_MATERIAL(OMMEL_URINE);

      potion* P = new potion(0, NO_MATERIALS); 
      P->InitMaterials(MAKE_MATERIAL(GLASS), Stuff);
      return P;
    }
  else
    return 0;
}

item* can::BetterVersion() const
{
  if(!GetContainedMaterial())
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

      std::string DeathMsg;

      if(Terrorist->IsPlayer())
	DeathMsg = "exploded himself with " + GetName(INDEFINITE);
      else
	DeathMsg = "kamikazed by " + Terrorist->GetKillName();

      Terrorist->DexterityAction(5);
      Terrorist->GetLevel()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }
  else if(Terrorist->IsPlayer())
    ADD_MESSAGE("You are not able to explode yourself with this crummy %s!", CHAR_NAME(UNARTICLED));

  return false;
}

bool holybook::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::SeeWholeMapCheatIsActive();
}

bool holybook::Read(character* Reader)
{
  Reader->StartReading(this, 1000);
  return true;
}

void holybook::FinishReading(character* Reader)
{
  if(Reader->IsPlayer())
    {
      if(GetMasterGod()->IsKnown())
	{
	  ADD_MESSAGE("The book reveals many divine secrets of %s to you.", GetMasterGod()->GOD_NAME);
	  GetMasterGod()->AdjustRelation(75);
	  game::ApplyDivineAlignmentBonuses(GetMasterGod(), true);
	  PLAYER->EditExperience(WISDOM, 250);

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
	  ADD_MESSAGE("You feel you master the magical rites of %s.", GetMasterGod()->GOD_NAME);
	}
    }
}

bool wand::ReceiveDamage(character* Damager, ushort Damage, ushort Type)
{
  if(Type & (FIRE|ENERGY|PHYSICAL_DAMAGE) && Damage && (Damage > 125 || !(RAND() % (250 / Damage))))
    {
      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      BreakEffect(Damager, "killed by an exploding " + GetName(UNARTICLED));
      return true;
    }

  return false;
}

bool backpack::ReceiveDamage(character* Damager, ushort Damage, ushort Type)
{
  if(Type & (FIRE|ENERGY) && Damage && IsExplosive() && (Damage > 25 || !(RAND() % (50 / Damage))))
    {
      std::string DeathMsg = "killed by an explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

bool scroll::ReceiveDamage(character*, ushort Damage, ushort Type)
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

bool holybook::ReceiveDamage(character*, ushort Damage, ushort Type)
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

      for(ushort c = 0; c < 10 && !FoundPlace; ++c)
	{	  
	  TryToCreate = Applier->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

	  if(GetArea()->IsValidPos(TryToCreate) && GetNearLSquare(TryToCreate)->IsWalkable(Genie) && GetNearLSquare(TryToCreate)->GetCharacter() == 0)
	    {
	      GetNearSquare(TryToCreate)->AddCharacter(Genie);
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

		  if(game::BoolQuestion("Do you want to wish? [Y/n]", YES))
		    {
		      ADD_MESSAGE("You may wish for an item.");

		      while(true)
			{
			  std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
			  item* TempItem = protosystem::CreateItem(Temp, Applier->IsPlayer());

			  if(TempItem)
			    {
			      Applier->GetStack()->AddItem(TempItem);
			      TempItem->SpecialGenerationHandler();

			      if(TempItem->HandleInPairs())
				ADD_MESSAGE("Two %s appear from nothing and the genie flies happily away!", TempItem->CHAR_NAME(PLURAL));
			      else
				ADD_MESSAGE("%s appears from nothing and the genie flies happily away!", TempItem->CHAR_NAME(INDEFINITE));

			      break;
			    }
			}

		      GetLevel()->RemoveCharacter(TryToCreate);
		      delete Genie;
		      Applier->EditAP(-1000);
		      return true;
		    }
		}
	    }

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

ushort holybook::GetMaterialColorA(ushort) const
{
  return GetMasterGod()->GetColor();
}

bool scrollofcharging::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::ChargeableSorter) && !Reader->EquipsSomething(&item::ChargeableSorter))
    {
      ADD_MESSAGE("You have nothing to charge.");
      return false;
    }
  else
    {
      Reader->StartReading(this, 500);
      return true;
    }
}

void scrollofcharging::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::ChargeableSorter) && !Reader->EquipsSomething(&item::ChargeableSorter))
    ADD_MESSAGE("You have lost whatever you wished to charge.");
  else
    while(true)
      {
	itemvector Item;
	Reader->SelectFromPossessions(Item, "Which item do you wish to charge?", NO_MULTI_SELECT|SELECT_PAIR, &item::ChargeableSorter);

	if(!Item.empty())
	  {
	    if(Item[0]->HandleInPairs() && Item.size() == 1)
	      {
		ADD_MESSAGE("Only one %s will be charged.", Item[0]->CHAR_NAME(UNARTICLED));

		if(!game::BoolQuestion("Still continue? [y/N]"))
		  continue;
	      }

	    for(ushort c = 0; c < Item.size(); ++c)
	      Item[c]->ChargeFully(Reader);

	    ADD_MESSAGE("You charge %s and the scroll burns.", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
	    break;
	  }
	else if(game::BoolQuestion("Really cancel read? [y/N]"))
	  return;
      }

  RemoveFromSlot();
  SendToHell();
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

      Stepper->ReceiveDamage(0, 1 + (RAND() & 3), PHYSICAL_DAMAGE, ALL&~LEGS);
      Stepper->CheckDeath("slipped on a banana peel", 0);
      Stepper->EditAP(-500);
    }
}

bool scrolloftaming::Read(character* Reader)
{
  Reader->StartReading(this, 100);
  return true;
}

void scrolloftaming::FinishReading(character* Reader)
{
  // First find all characters in the squares around Reader

  std::vector<character*> CharactersNearBy;

  for(ushort c = 0; c < 8; ++c)
    {
      vector2d Test = Reader->GetPos() + game::GetMoveVector(c);

      if(GetArea()->IsValidPos(Test))
	{
	  character* CharacterInSquare = GetNearSquare(Test)->GetCharacter();

	  if(CharacterInSquare && CharacterInSquare->IsCharmable() && CharacterInSquare->GetTeam() != Reader->GetTeam())
	    CharactersNearBy.push_back(CharacterInSquare);
	}
    }
  
  // Then pick one of the characters and set it to the same team as Reader

  if(CharactersNearBy.empty())
    {
      if(Reader->IsPlayer() || Reader->CanBeSeenByPlayer())
	ADD_MESSAGE("The scroll burns, but nothing happens.");
    }
  else
    {
      character* ToBeTamed = CharactersNearBy[RAND() % CharactersNearBy.size()];
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

bool mine::ReceiveDamage(character* Damager, ushort Damage, ushort Type)
{
  if((Type & (FIRE|ENERGY) && Damage && (Damage > 50 || !(RAND() % (100 / Damage)))) || (Type & (PHYSICAL_DAMAGE|SOUND) && WillExplode(0)))
    {
      std::string DeathMsg = "killed by an explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

void mine::StepOnEffect(character* Stepper)
{
  if(!WillExplode(Stepper))
    return;

  if(Stepper->IsPlayer())
    ADD_MESSAGE("You hear a faint thump. You look down. You see %s.", CHAR_NAME(INDEFINITE));
  else if(Stepper->CanBeSeenByPlayer())
    ADD_MESSAGE("%s steps on %s.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
  else if(GetSquareUnder()->CanBeSeenByPlayer())
    ADD_MESSAGE("Something explodes!");

  SetIsActive(false);
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  GetLSquareUnder()->SendNewDrawRequest();

  if(Stepper->IsPlayer())
    game::AskForKeyPress("Trap activated! [press any key to continue]");

  lsquare* Square = GetLSquareUnder();
  RemoveFromSlot();
  SendToHell();
  Square->GetLevel()->Explosion(0, "killed by a land mine", Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
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
      uchar Dir = game::DirectionQuestion("What door do you wish to lock or unlock? [press a direction key]", false, true);

      if(Dir == DIR_ERROR)
	return false;

      vector2d ApplyPos = User->GetPos() + game::GetMoveVector(Dir);

      if(GetArea()->IsValidPos(ApplyPos))
	{
	  GetNearLSquare(ApplyPos)->TryKey(this, User);
	  User->DexterityAction(5);
	}
      else
	return false;
    }

  return true;
}

void key::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LockType;
}

void key::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LockType;
}

void materialcontainer::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << ContainedMaterial;
}

void materialcontainer::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  LoadMaterial(SaveFile, ContainedMaterial);
}

material* materialcontainer::GetMaterial(ushort Index) const
{
  return !Index ? MainMaterial : ContainedMaterial;
}

ushort materialcontainer::GetMaterialColorB(ushort Frame) const
{
  if(GetContainedMaterial())
    return GetContainedMaterial()->GetColor();
  else
    return GetMaterialColorA(Frame);
}

uchar materialcontainer::GetAlphaB(ushort Frame) const
{
  if(GetContainedMaterial() && GetContainedMaterial()->GetAlpha() > GetAlphaA(Frame))
    return GetContainedMaterial()->GetAlpha();
  else
    return GetAlphaA(Frame);
}

void lantern::VirtualConstructor(bool)
{
  SetSquarePosition(CENTER);
}

void wand::VirtualConstructor(bool Load)
{
  if(!Load)
    {
      SetCharges(GetMinCharges() + RAND() % (GetMaxCharges() - GetMinCharges() + 1));
      SetTimesUsed(0);
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

void key::VirtualConstructor(bool Load)
{ 
  if(!Load)
    SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
}

bool whistle::Apply(character* Whistler) 
{
  BlowEffect(Whistler);
  Whistler->EditAP(-1000);
  return true;
} 

void whistle::BlowEffect(character* Whistler)
{
  if(Whistler->IsPlayer())
    ADD_MESSAGE("You produce an interesting sound.");
  else if(Whistler->CanBeSeenByPlayer())
    ADD_MESSAGE("%s blows %s and produces an interesting sound.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
  else 
    ADD_MESSAGE("You hear a whistle playing.");

  game::CallForAttention(GetPos(), GetRange());
}

void magicalwhistle::BlowEffect(character* Whistler)
{
  if(LastUsed != 0 && game::GetTicks() - LastUsed < 2500)
    {
      whistle::BlowEffect(Whistler);
      return;
    }
  else 
    LastUsed = game::GetTicks();

  if(Whistler->IsPlayer())
    ADD_MESSAGE("You produce a peculiar sound.");
  else if(Whistler->CanBeSeenByPlayer())
    ADD_MESSAGE("%s blows %s and produces a peculiar sound.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
  else 
    ADD_MESSAGE("You hear a strange tune playing.");

  const std::list<character*>& Member = Whistler->GetTeam()->GetMember();

  for(std::list<character*>::const_iterator i = Member.begin(); i != Member.end(); ++i)
    if((*i)->IsEnabled() && Whistler != *i)
      (*i)->TeleportNear(Whistler);

  game::CallForAttention(GetPos(), GetRange());
}

void itemcontainer::VirtualConstructor(bool Load)
{
  Contained = new stack(0, this, HIDDEN, true);

  if(!Load)
    {
      SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
      SetIsLocked(RAND() % 3 != 0);
      ulong ItemNumber = RAND() % (GetMaxGeneratedContainedItems() + 1);

      for(ushort c = 0; c < ItemNumber; ++c)
	{
	  item* NewItem = protosystem::BalancedCreateItem();
	  ulong Volume = NewItem->GetVolume();

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
  if(GetLockType() == DAMAGED)
    {
      ADD_MESSAGE("The lock is broken.");
      return true;
    }

  if(Key->CanOpenLockType(GetLockType()))
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
  ushort Chosen = RandomizeMaterialConfiguration();
  InitChosenMaterial(MainMaterial, GetMainMaterialConfig(), GetDefaultMainVolume(), Chosen);
  InitChosenMaterial(ContainedMaterial, GetContainedMaterialConfig(), GetDefaultContainedVolume(), Chosen);
}

/* Returns true if container opens fine else false */

bool itemcontainer::Open(character* Opener)
{
  if(IsLocked())
    {
      ADD_MESSAGE("%s seems to be locked.", CHAR_NAME(DEFINITE));
      return false;
    }

  std::string Question = "Do you want to (t)ake something from or (p)ut something in this container? [t,p]";
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
  SaveFile << LockType << Locked;
}

void itemcontainer::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  Contained->Load(SaveFile);
  SaveFile >> LockType >> Locked;
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

const std::string& key::GetAdjective() const
{
  return game::GetLockDescription(LockType);
}

/* Victim is the stuck person, Bodypart is the index of the bodypart that the trap is stuck to and the last vector2d is just a direction vector that may - or may not - be used in the future. This function returns true if the character manages to escape */

bool beartrap::TryToUnstuck(character* Victim, ushort BodyPart, vector2d)
{
  if(IsBroken())
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONE_INDEX);
      return true;
    }

  if(!(RAND() % 3))
    {
      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to hurt your %s even more.", Victim->GetBodyPartName(BodyPart).c_str());
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s hurts %s %s more with %s.", Victim->CHAR_NAME(DEFINITE), Victim->GetPossessivePronoun().c_str(), Victim->GetBodyPartName(BodyPart).c_str(), CHAR_NAME(DEFINITE));

      Victim->ReceiveBodyPartDamage(0, 2 + (RAND() & 1), PHYSICAL_DAMAGE, BodyPart, YOURSELF, false, false, false);
      Victim->CheckDeath("died while trying to escape from " + GetName(DEFINITE), 0);
      return false;
    }

  if(!(RAND() % 5))
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONE_INDEX);

      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to free yourself from %s.", CHAR_NAME(DEFINITE));
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s manages to free %sself from %s.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN, CHAR_NAME(DEFINITE));

      return true;
    }

  if(!(RAND() % 10))
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
      ushort StepperBodyPart = Stepper->GetRandomStepperBodyPart();

      if(StepperBodyPart == NONE_INDEX)
	return;

      Stepper->SetStuckTo(this);
      Stepper->SetStuckToBodyPart(StepperBodyPart);

      if(Stepper->IsPlayer())
	ADD_MESSAGE("You step in %s and it traps your %s.", CHAR_NAME(INDEFINITE), Stepper->GetBodyPartName(StepperBodyPart).c_str());
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is trapped in %s.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));

      SetIsActive(false);
      GetLSquareUnder()->SendMemorizedUpdateRequest();
      GetLSquareUnder()->SendNewDrawRequest();

      if(Stepper->IsPlayer())
	game::AskForKeyPress("Trap activated! [press any key to continue]");

      Stepper->ReceiveBodyPartDamage(0, 3 + RAND() % 3, PHYSICAL_DAMAGE, Stepper->GetStuckToBodyPart(), YOURSELF, false, false, false);
      Stepper->CheckDeath("died by stepping to " + GetName(INDEFINITE), 0);
    }
}

bool beartrap::CheckPickUpEffect(character* Picker)
{
  if(Picker->IsStuck() && Picker->GetStuckTo()->GetID() == GetID())
    {
      if(Picker->IsPlayer())
	ADD_MESSAGE("%s is tightly stuck to your %s.", CHAR_NAME(DEFINITE), Picker->GetBodyPartName(Picker->GetStuckToBodyPart()).c_str());

      return false;
    }

  SetIsActive(false);
  return true;
}

void can::GenerateLeftOvers(character* Eater)
{
  ChangeConsumeMaterial(0);

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    {
      MoveTo(Eater->GetStackUnder());
      Eater->DexterityAction(1);
    }
  else
    MoveTo(Eater->GetStack());
}

uchar lantern::GetSpecialFlags() const
{
  switch(SquarePosition)
    {
    case RIGHT:
      return ROTATE;
    case DOWN:
      return FLIP;
    case LEFT:
      return ROTATE|MIRROR;
    default:
      return 0;
    }
}

bool stethoscope::Apply(character* Doctor) 
{
  if(!Doctor->CanUseStethoscope(true))
    return false;

  if(!Doctor->IsPlayer())
    ABORT("Doctor is not here, man, but these pills taste just as good anyway.");

  uchar Dir = game::DirectionQuestion("What do you want to inspect? [press a direction key]", false,true);  

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
  delete ContainedMaterial;
}

bool itemcontainer::ContentsCanBeSeenBy(const character* Viewer) const
{
  return GetMainMaterial()->IsTransparent() && CanBeSeenBy(Viewer);
}

bool mine::CanBeSeenBy(const character* Viewer) const
{ 
  ushort ViewerTeam = Viewer->GetTeam()->GetID();
  return (!IsActive() || ViewerTeam == Team || DiscoveredByTeam.find(ViewerTeam) != DiscoveredByTeam.end()) && materialcontainer::CanBeSeenBy(Viewer);
}

bool beartrap::CanBeSeenBy(const character* Viewer) const
{
  ushort ViewerTeam = Viewer->GetTeam()->GetID();
  return (!IsActive() || ViewerTeam == Team || DiscoveredByTeam.find(ViewerTeam) != DiscoveredByTeam.end()) && item::CanBeSeenBy(Viewer);
}

bool mine::Apply(character* User)
{
  if(User->IsPlayer() && !game::BoolQuestion("Are you sure you want to plant " + GetName(DEFINITE) + "? [y/N]")) 
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
  if(User->IsPlayer() && !game::BoolQuestion("Are you sure you want to plant " + GetName(DEFINITE) + "? [y/N]")) 
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

vector2d beartrap::GetBitmapPos(ushort Frame) const
{
  if(!IsBroken())
    return IsActive() ? vector2d(32, 304) : vector2d(32, 320);
  else
    return item::GetBitmapPos(Frame);
}

bool mine::WillExplode(const character* Stepper) const
{
  return IsActive() && GetContainedMaterial() && GetContainedMaterial()->IsExplosive() && (!Stepper || Stepper->GetWeight() > 5000);
}

materialcontainer::materialcontainer(const materialcontainer& MC) : item(MC)
{
  CopyMaterial(MC.ContainedMaterial, ContainedMaterial);
}

itemcontainer::itemcontainer(const itemcontainer& Container) : item(Container), LockType(Container.LockType), Locked(Container.Locked)
{
  Contained = new stack(0, this, HIDDEN, true);
}

oillamp::oillamp(const oillamp& Lamp) : item(Lamp), InhabitedByGenie(false)
{
}

bool wand::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Zapper->EditExperience(PERCEPTION, 250);
  std::string DeathMSG = "killed by " + GetName(INDEFINITE);
  (GetLevel()->*level::GetBeam(GetBeamStyle()))(Zapper, DeathMSG, Zapper->GetPos(), GetBeamColor(), GetBeamEffect(), Direction, GetBeamRange());
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

void wand::AddInventoryEntry(const character*, std::string& Entry, ushort, bool ShowSpecialInfo) const // never piled
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

  if(Material == GetMainMaterial())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s becomes so spoiled that it cannot hold its contents anymore.", CHAR_NAME(DEFINITE));

      if(GetContainedMaterial()->IsLiquid())
	{
	  if(!game::IsInWilderness())
	    GetLSquareUnder()->SpillFluid(5, GetContainedMaterial()->GetColor());

	  RemoveFromSlot();
	}
      else
	{
	  lump* Lump = new lump(0, NO_MATERIALS);
	  Lump->InitMaterials(GetContainedMaterial());
	  DonateSlotTo(Lump);
	  SetContainedMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
	}

      SendToHell();
    }
  else if(Material == GetContainedMaterial())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The contents of %s spoil completely.", CHAR_NAME(DEFINITE));

      Empty();
    }
}

bool materialcontainer::CanBePiledWith(const item* Item, const character* Viewer) const
{
  if(!item::CanBePiledWith(Item, Viewer))
    return false;

  const materialcontainer* Weapon = static_cast<const materialcontainer*>(Item);

  if(ContainedMaterial == 0 && Weapon->ContainedMaterial == 0)
    return true;

  return ContainedMaterial != 0
      && Weapon->ContainedMaterial != 0
      && ContainedMaterial->IsSameAs(Weapon->ContainedMaterial)
      && ContainedMaterial->GetSpoilLevel() == Weapon->ContainedMaterial->GetSpoilLevel();
}

bool key::CanBePiledWith(const item* Item, const character* Viewer) const
{
  return item::CanBePiledWith(Item, Viewer) && LockType == static_cast<const key*>(Item)->LockType;
}

ulong itemcontainer::GetTruePrice() const
{
  return GetContained()->GetTruePrice() + item::GetTruePrice();
}

void potion::Break(character* Breaker)
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s shatters to pieces.", CHAR_NAME(DEFINITE));

  if(Breaker && IsOnGround())
    {
      room* Room = GetRoom();

      if(Room)
	Room->HostileAction(Breaker);
    }

  item* Remains = new brokenbottle(0, NO_MATERIALS);
  Remains->InitMaterials(GetMainMaterial()->Clone());
  Remains->SetID(BackupID);
  Remains->SetBackupID(ID);
  DonateSlotTo(Remains);
  SendToHell();

  if(GetContainedMaterial()) 
    Remains->GetLSquareUnder()->SpillFluid(0, GetContainedMaterial()->Clone(), 70);

  if(PLAYER->Equips(Remains))
    game::AskForKeyPress("Equipment broken! [press any key to continue]");
}

void materialcontainer::Be()
{
  MainMaterial->Be();

  if(Exists() && ContainedMaterial)
    ContainedMaterial->Be();
}

bool materialcontainer::IsSparkling(ushort ColorIndex) const
{
  return (ColorIndex == 0 && MainMaterial->IsSparkling()) || (ColorIndex == 1 && ContainedMaterial && ContainedMaterial->IsSparkling());
}

bool scrollofenchantweapon::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::WeaponSorter) && !Reader->EquipsSomething(&item::WeaponSorter))
    {
      ADD_MESSAGE("You have nothing to enchant.");
      return false;
    }
  else
    {
      Reader->StartReading(this, 1000);
      return true;
    }
}

void scrollofenchantweapon::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::WeaponSorter) && !Reader->EquipsSomething(&item::WeaponSorter))
    ADD_MESSAGE("You notice you have lost anything you wished to enchant.");
  else
    {
      while(true)
	{
	  itemvector Item;
	  Reader->SelectFromPossessions(Item, "Choose a weapon to enchant:", NO_MULTI_SELECT|SELECT_PAIR, &item::WeaponSorter);

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

		  if(!game::BoolQuestion("Still continue? [y/N]"))
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

	      for(ushort c = 0; c < Item.size(); ++c)
		Item[c]->EditEnchantment(1);

	      break;
	    }
	  else if(game::BoolQuestion("Really cancel read? [y/N]"))
	    return;
	}

      RemoveFromSlot();
      SendToHell();
    }
}

bool scrollofenchantarmor::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::ArmorSorter) && !Reader->EquipsSomething(&item::ArmorSorter))
    {
      ADD_MESSAGE("You have nothing to enchant.");
      return false;
    }
  else
    {
      Reader->StartReading(this, 1000);
      return true;
    }
}

void scrollofenchantarmor::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::ArmorSorter) && !Reader->EquipsSomething(&item::ArmorSorter))
    ADD_MESSAGE("You notice you have lost anything you wished to enchant.");
  else
    {
      while(true)
	{
	  itemvector Item;
	  Reader->SelectFromPossessions(Item, "Choose an armor to enchant:", NO_MULTI_SELECT|SELECT_PAIR, &item::ArmorSorter);

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

		  if(!game::BoolQuestion("Still continue? [y/N]"))
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

	      for(ushort c = 0; c < Item.size(); ++c)
		Item[c]->EditEnchantment(1);

	      break;
	    }
	  else if(game::BoolQuestion("Really cancel read? [y/N]"))
	    return;
	}

      RemoveFromSlot();
      SendToHell();
    }
}

bool itemcontainer::ReceiveDamage(character* Damager, ushort Damage, ushort Type)
{
  if(Type & (PHYSICAL_DAMAGE|SOUND))
    {
      Contained->ReceiveDamage(Damager, Damage / GetDamageDivider(), Type);
      ushort SV = Max<ushort>(GetStrengthValue(), 1);

      if(IsLocked() && Damage > SV && RAND() % (100 * Damage / SV) >= 100)
	{
	  SetIsLocked(false);
	  SetLockType(DAMAGED);

	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The %s's lock shatters to pieces.", GetNameSingular().c_str());

	  return true;
	}
      else if(Damager && Damager->IsPlayer())
	ADD_MESSAGE("THUMP!");
    }

  return false;
}

void itemcontainer::DrawContents(const character* Char)
{
  std::string Topic = "Contents of your " + GetName(UNARTICLED);
  GetContained()->DrawContents(Char, Topic, NO_SELECT);

  for(stackiterator i = GetContained()->GetBottom(); i.HasItem(); ++i)
    i->DrawContents(Char);
}

void backpack::ReceiveFluidSpill(material* Liquid)
{
  if(!Liquid->IsExplosive())
    {
      ulong Amount = Liquid->GetVolume() * (30 + (RAND() % 71)) / 100;
      GetContainedMaterial()->SetWetness(Amount);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s gets wet.", CHAR_NAME(DEFINITE));
    }
}

void magicalwhistle::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LastUsed;
}

void magicalwhistle::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LastUsed;
}

void magicalwhistle::VirtualConstructor(bool Load)
{
  whistle::VirtualConstructor(Load);
  LastUsed = 0;
}

uchar materialcontainer::GetSpoilLevel() const
{
  return Max<uchar>(MainMaterial->GetSpoilLevel(), ContainedMaterial ? ContainedMaterial->GetSpoilLevel() : 0);
}

void itemcontainer::SetItemsInside(const std::list<contentscript<item> >& ItemList, ushort SpecialFlags)
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

bool mine::CheckPickUpEffect(character*)
{
  if(WillExplode(0))
    {
      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(0, "tried to pick up an active land mine", Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return false;
    }

  return true;
}

void itemcontainer::GenerateLeftOvers(character*)
{
  Contained->MoveItemsTo(GetSlot());
}

bool scrollofrepair::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::BrokenSorter) && !Reader->EquipsSomething(&item::BrokenSorter))
    {
      ADD_MESSAGE("You have nothing to repair.");
      return false;
    }

  Reader->StartReading(this, 1000);
  return true;
}

void scrollofrepair::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::BrokenSorter) && !Reader->EquipsSomething(&item::BrokenSorter))
    ADD_MESSAGE("You have lost whatever you wished to repair.");
  else
    while(true)
      {
	itemvector Item;
	Reader->SelectFromPossessions(Item, "Which item do you wish to repair?", NO_MULTI_SELECT|SELECT_PAIR, &item::BrokenSorter);

	if(!Item.empty())
	  {
	    if(Item[0]->HandleInPairs() && Item.size() == 1)
	      {
		ADD_MESSAGE("Only one %s will be enchanted.", Item[0]->CHAR_NAME(UNARTICLED));

		if(!game::BoolQuestion("Still continue? [y/N]"))
		  continue;
	      }

	    if(Item.size() == 1)
	      ADD_MESSAGE("As you read the scroll, %s glows green and fixes itself.", Item[0]->CHAR_NAME(DEFINITE));
	    else
	      ADD_MESSAGE("As you read the scroll, %s glow green and fix themselves.", Item[0]->CHAR_NAME(PLURAL));

	    for(ushort c = 0; c < Item.size(); ++c)
	      Item[c]->Fix();

	    break;
	  }
	else if(game::BoolQuestion("Really cancel read? [y/N]"))
	  return;
      }

  RemoveFromSlot();
  SendToHell();  
}

item* brokenbottle::Fix()
{
  potion* Potion = new potion(0, NO_MATERIALS);
  Potion->InitMaterials(GetMainMaterial(), 0);
  Potion->SetID(BackupID);
  Potion->SetBackupID(ID);
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

long itemcontainer::GetScore() const
{
  return item::GetScore() + GetContained()->GetScore();
}

bool horn::Apply(character* Blower) 
{
  if(LastUsed == 0 || game::GetTicks() - LastUsed >= 2500)
    {
      LastUsed = game::GetTicks();
      const char* SoundDescription = Config == BRAVERY ? "loud but calming" : "frightening, almost scream-like";

      if(Blower->IsPlayer())
	ADD_MESSAGE("You produce a %s sound.", SoundDescription);
      else if(Blower->CanBeSeenByPlayer())
	ADD_MESSAGE("%s blows %s and produces a %s sound.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE), SoundDescription);
      else 
	ADD_MESSAGE("You hear a %s sound echoing everywhere.", SoundDescription);

      rect Rect;
      femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), GetPos(), 10);

      for(ushort x = Rect.X1; x <= Rect.X2; ++x)
	for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
	  {
	    character* Audience = GetNearSquare(x, y)->GetCharacter();

	    if(Audience)
	      {
		if(Config == BRAVERY && Audience->TemporaryStateIsActivated(PANIC) && Blower->GetTeam()->GetID() == Audience->GetTeam()->GetID())
		  {
		    if(Audience->IsPlayer())
		      ADD_MESSAGE("You calm down.");
		    else if(CanBeSeenByPlayer())
		      ADD_MESSAGE("%s calms down.", Audience->CHAR_NAME(DEFINITE));

		    Audience->DeActivateTemporaryState(PANIC);
		  }
		else if(Config == FEAR && !Audience->TemporaryStateIsActivated(PANIC) && Blower->GetRelation(Audience) == HOSTILE && Audience->GetPanicLevel() >= RAND() % 33)
		  Audience->BeginTemporaryState(PANIC, 500 + RAND() % 500);
	      }
	  }

    }
  else
    {
      if(Blower->IsPlayer())
	ADD_MESSAGE("You produce a mighty sound.");
      else if(Blower->CanBeSeenByPlayer())
	ADD_MESSAGE("%s blows %s and produces a mighty sound.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
      else 
	ADD_MESSAGE("You hear a horn being blown.");
    }

  game::CallForAttention(GetPos(), 30 * 30);
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

void banana::GenerateLeftOvers(character* Eater)
{
  item* Peel = new bananapeels(0, NO_MATERIALS);
  Peel->InitMaterials(GetMainMaterial());

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    {
      Eater->GetStackUnder()->AddItem(Peel);
      Eater->DexterityAction(1);
    }
  else
    Eater->GetStack()->AddItem(Peel);

  RemoveFromSlot();
  SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  SendToHell();
}

void banana::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << Charges;
}

void banana::Load(inputfile& SaveFile)
{
  materialcontainer::Load(SaveFile);
  SaveFile >> Charges;
}

bool banana::Zap(character*, vector2d, uchar)
{
  if(Charges)
    {
      ADD_MESSAGE("BANG! You zap %s!", CHAR_NAME(DEFINITE));
      --Charges;
    }
  else
    ADD_MESSAGE("Click!");

  return true;
}

void banana::VirtualConstructor(bool Load)
{
  materialcontainer::VirtualConstructor(Load);

  if(!Load)
    SetCharges(GetMinCharges() + RAND() % (GetMaxCharges() - GetMinCharges() + 1));
}

void banana::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == GetContainedMaterial() && !GetMainMaterial()->IsVeryCloseToSpoiling())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The inside of %s spoils completely.", CHAR_NAME(DEFINITE));

      item* Peel = new bananapeels(0, NO_MATERIALS);
      Peel->InitMaterials(GetMainMaterial());
      DonateSlotTo(Peel);
      SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
      SendToHell();
    }
  else
    item::SignalSpoil(Material);
}

bool bone::DogWillCatchAndConsume() const
{
  return GetConsumeMaterial()->GetConfig() == BONE && GetConsumeMaterial()->GetSpoilLevel() == 0;
}

bool stone::ShowMaterial() const
{
  return GetMainMaterial()->GetConfig() != STONE; // gum solution
}

short itemcontainer::GetOfferValue(uchar Receiver) const
{
  short Sum = 0;

  for(ushort c = 0; c < GetContained()->GetItems(); ++c)
    Sum += GetContained()->GetItem(c)->GetOfferValue(Receiver);

  return item::GetOfferValue(Receiver) + Sum;
}

bool itemcontainer::IsDestroyable() const
{
  for(ushort c = 0; c < GetContained()->GetItems(); ++c)
    if(!GetContained()->GetItem(c)->IsDestroyable())
      return false;

  return true;
}

uchar lantern::GetAlphaB(ushort Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 1);
}

uchar lantern::GetAlphaC(ushort Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 2);
}

uchar lantern::GetAlphaD(ushort Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 3);
}

void itemcontainer::SortAllItems(itemvector& AllItems, const character* Character, bool (*Sorter)(const item*, const character*)) const
{
  if(Sorter == 0 || Sorter(this, Character))
    AllItems.push_back(const_cast<itemcontainer*>(this));

  GetContained()->SortAllItems(AllItems, Character, Sorter);
}

uchar materialcontainer::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : ContainedMaterial->GetAttachedGod();
}

void beartrap::Search(const character* Char, ushort Perception)
{
  ushort ViewerTeam = Char->GetTeam()->GetID();

  if(IsActive() && ViewerTeam != Team && DiscoveredByTeam.find(ViewerTeam) == DiscoveredByTeam.end() && !RAND_N(200 / Perception))
    {
      DiscoveredByTeam.insert(ViewerTeam);
      GetLSquareUnder()->SendMemorizedUpdateRequest();
      GetLSquareUnder()->SendNewDrawRequest();

      if(Char->IsPlayer())
	{
	  game::AskForKeyPress("Trap found! [press any key to continue]");
	  ADD_MESSAGE("You find %s.", CHAR_NAME(INDEFINITE));
	}
    }
}

void mine::Search(const character* Char, ushort Perception)
{
  ushort ViewerTeam = Char->GetTeam()->GetID();

  if(IsActive() && ViewerTeam != Team && DiscoveredByTeam.find(ViewerTeam) == DiscoveredByTeam.end() && !RAND_N(200 / Perception))
    {
      DiscoveredByTeam.insert(ViewerTeam);
      GetLSquareUnder()->SendMemorizedUpdateRequest();
      GetLSquareUnder()->SendNewDrawRequest();

      if(Char->IsPlayer())
	{
	  game::AskForKeyPress("Trap found! [press any key to continue]");
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

void wand::BreakEffect(character* Terrorist, const std::string& DeathMsg)
{
  vector2d Pos = GetPos();
  level* Level = GetLevel();
  RemoveFromSlot();

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, Level->GetBorder(), Pos, GetBreakEffectRange());
  (Level->*level::GetBeamEffectVisualizer(GetBeamStyle()))(Rect, GetBeamColor());

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      (Level->GetLSquare(x, y)->*lsquare::GetBeamEffect(GetBeamEffect()))(Terrorist, DeathMsg, YOURSELF);

  SendToHell();
}

bool beartrap::ReceiveDamage(character* Damager, ushort Damage, ushort Type)
{
  if(!IsBroken() && Type & PHYSICAL_DAMAGE && Damage)
    {
      if(Damage > 125 || !(RAND() % (250 / Damage)))
	{
	  Break(Damager);	  
	  return true;
	}
      else
	{
	  if(IsActive())
	    {
	      ADD_MESSAGE("%s snaps shut.", CHAR_NAME(DEFINITE));
	      SetIsActive(false);
	      GetLSquareUnder()->SendMemorizedUpdateRequest();
	      GetLSquareUnder()->SendNewDrawRequest();	  
	      return true;
	    }
	}
    }

  return false;
}

bool potion::ReceiveDamage(character* Damager, ushort Damage, ushort Type)
{
  if(Type & FIRE && Damage && IsExplosive() && (Damage > 50 || !(RAND() % (100 / Damage))))
    {
      std::string DeathMsg = "killed by an explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }

  if(Type & THROW)
    {
      ushort StrengthValue = GetStrengthValue();

      if(!StrengthValue)
	StrengthValue = 1;

      if(Damage > StrengthValue << 2 && RAND() % (25 * Damage / StrengthValue) >= 100)
	{
	  Break(Damager);
	  return true;
	}
    }

  return item::ReceiveDamage(Damager, Damage, Type);
}
