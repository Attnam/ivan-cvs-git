/* Compiled through charsset.cpp */

ushort nonhumanoid::GetUnarmedMinDamage() const { return ushort(UnarmedDamage * 0.75f); }
ushort nonhumanoid::GetUnarmedMaxDamage() const { return ushort(UnarmedDamage * 1.25f + 1); }
ushort nonhumanoid::GetKickMinDamage() const { return ushort(KickDamage * 0.75f); }
ushort nonhumanoid::GetKickMaxDamage() const { return ushort(KickDamage * 1.25f + 1); }
ushort nonhumanoid::GetBiteMinDamage() const { return ushort(BiteDamage * 0.75f); }
ushort nonhumanoid::GetBiteMaxDamage() const { return ushort(BiteDamage * 1.25f + 1); }
ushort nonhumanoid::GetCarryingStrength() const { return (Max<ushort>(GetAttribute(LEG_STRENGTH), 1) << 1) + CarryingBonus; }
bool nonhumanoid::IsAlive() const { return GetTorso()->IsAlive(); }

bool elpuri::SpecialEnemySightedReaction(character*) { return !(Active = true); }

std::string billswill::FirstPersonUnarmedHitVerb() const { return "emit psi waves at"; }
std::string billswill::FirstPersonCriticalUnarmedHitVerb() const { return "emit powerful psi waves at"; }
std::string billswill::ThirdPersonUnarmedHitVerb() const { return "emits psi waves at"; }
std::string billswill::ThirdPersonCriticalUnarmedHitVerb() const { return "emits powerful psi waves at"; }

std::string mommo::FirstPersonBiteVerb() const { return "vomit acidous slime at"; }
std::string mommo::FirstPersonCriticalBiteVerb() const { return "vomit very acidous slime at"; }
std::string mommo::ThirdPersonBiteVerb() const { return "vomits acidous slime at"; }
std::string mommo::ThirdPersonCriticalBiteVerb() const { return "vomits very acidous slime at"; }
std::string mommo::BiteNoun() const { return "slime"; }

uchar dolphin::GetSpecialBodyPartFlags(ushort) const { return RAND() & (MIRROR|ROTATE); }

ushort chameleon::GetSkinColor() const { return MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190); }

bodypart* eddy::MakeBodyPart(ushort) const { return new eddytorso(0, NO_MATERIALS); }

bool elpuri::Hit(character* Enemy, bool ForceHit)
{
  for(ushort d = 0; d < 9; ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square)
	{
	  if(d != YOURSELF)
	    {
	      character* ByStander = Square->GetCharacter();

	      if(ByStander && (ByStander == Enemy || GetRelation(ByStander) == HOSTILE))
		{
		  nonhumanoid::Hit(ByStander, ForceHit);
		  ByStander->DamageAllItems(this, RAND() % 36 + RAND() % 36, PHYSICAL_DAMAGE);
		}
	    }

	  Square->GetStack()->ReceiveDamage(this, RAND() % 36 + RAND() % 36, PHYSICAL_DAMAGE);

	  for(ushort c = 0; c < 4; ++c)
	    if(Square->GetSideStack(c)->GetSquareTrulyUnder() == GetSquareUnder())
	      Square->GetSideStack(c)->ReceiveDamage(this, RAND() % 36 + RAND() % 36, PHYSICAL_DAMAGE);
	}
    }

  EditAP(-1000);
  return true;
}

bool dog::Catches(item* Thingy)
{
  if(Thingy->DogWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy))
	{
	  if(IsPlayer())
	    ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHAR_NAME(DEFINITE));
	  else
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s catches %s and eats it.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

	      ChangeTeam(PLAYER->GetTeam());
	    }
	}
      else if(IsPlayer())
	ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHAR_NAME(DEFINITE));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s catches %s.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

      return true;
    }
  else
    return false;
}

bool largecat::Catches(item* Thingy)
{
  if(Thingy->CatWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy))
	{
	  if(IsPlayer())
	    ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHAR_NAME(DEFINITE));
	  else
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s catches %s and eats it.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

	      ChangeTeam(PLAYER->GetTeam());
	    }
	}
      else if(IsPlayer())
	ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHAR_NAME(DEFINITE));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s catches %s.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

      return true;
    }
  else
    return false;
}

bool unicorn::SpecialEnemySightedReaction(character*)
{
  if((RAND() % 3 && IsInBadCondition()) || !(RAND() % 10))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears!", CHAR_NAME(DEFINITE));

    Move(GetLevel()->GetRandomSquare(this), true);

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s appears from nothing!", CHAR_NAME(INDEFINITE));

    return true;
  }

  if(!(RAND() % 3) && MoveRandomly())
    return true;

  return false;
}

void carnivorousplant::GetAICommand()
{
  character* CharNear[8];
  ushort CharIndex = 0;

  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char && (GetRelation(Char) == HOSTILE || StateIsActivated(CONFUSED)))
	    CharNear[CharIndex++] = Char;
	}
    }

  if(CharIndex)
    Hit(CharNear[RAND() % CharIndex]);

  EditAP(-1000);
}

void nonhumanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << Strength << Agility << StrengthExperience << AgilityExperience;
}

void nonhumanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> Strength >> Agility >> StrengthExperience >> AgilityExperience;
}

void nonhumanoid::CalculateUnarmedDamage()
{
  UnarmedDamage = sqrt(5e-10f * GetAttribute(ARM_STRENGTH)) * GetBaseUnarmedStrength() * GetCWeaponSkill(UNARMED)->GetBonus();
}

void nonhumanoid::CalculateUnarmedToHitValue()
{
  UnarmedToHitValue = GetAttribute(DEXTERITY) * sqrt(2.5f * GetAttribute(PERCEPTION)) * GetCWeaponSkill(UNARMED)->GetBonus() * GetMoveEase() / 50000;
}

void nonhumanoid::CalculateUnarmedAPCost()
{
  UnarmedAPCost = Max<long>(1000000000 / (APBonus(GetAttribute(DEXTERITY)) * GetMoveEase() * GetCWeaponSkill(UNARMED)->GetBonus()), 100);
}

void nonhumanoid::CalculateKickDamage()
{
  KickDamage = sqrt(5e-10f * GetAttribute(LEG_STRENGTH)) * GetBaseKickStrength() * GetCWeaponSkill(KICK)->GetBonus();
}

void nonhumanoid::CalculateKickToHitValue()
{
  KickToHitValue = GetAttribute(AGILITY) * sqrt(2.5f * GetAttribute(PERCEPTION)) * GetCWeaponSkill(KICK)->GetBonus() * GetMoveEase() / 100000;
}

void nonhumanoid::CalculateKickAPCost()
{
  KickAPCost = Max<long>(2000000000 / (APBonus(GetAttribute(AGILITY)) * GetMoveEase() * GetCWeaponSkill(KICK)->GetBonus()), 100);
}

void nonhumanoid::CalculateBiteDamage()
{
  BiteDamage = sqrt(5e-10f * GetAttribute(ARM_STRENGTH)) * GetBaseBiteStrength() * GetCWeaponSkill(BITE)->GetBonus();
}

void nonhumanoid::CalculateBiteToHitValue()
{
  BiteToHitValue = GetAttribute(AGILITY) * sqrt(2.5f * GetAttribute(PERCEPTION)) * GetCWeaponSkill(BITE)->GetBonus() * GetMoveEase() / 100000;
}

void nonhumanoid::CalculateBiteAPCost()
{
  BiteAPCost = Max<long>(1000000000 / (APBonus(GetAttribute(DEXTERITY)) * GetMoveEase() * GetCWeaponSkill(BITE)->GetBonus()), 100);
}

void nonhumanoid::InitSpecialAttributes()
{
  Strength = (GetDefaultArmStrength() * (100 + GetAttributeBonus()) / 100) << 1;
  Agility = (GetDefaultAgility() * (100 + GetAttributeBonus()) / 100) << 1;
  StrengthExperience = AgilityExperience = 0;
}

void nonhumanoid::Bite(character* Enemy, bool ForceHit)
{
  EditNP(-50);
  EditAP(-GetBiteAPCost());
  EditExperience(AGILITY, 75);
  Enemy->TakeHit(this, 0, GetBiteDamage(), GetBiteToHitValue(), RAND() % 26 - RAND() % 26, BITE_ATTACK, !(RAND() % GetCriticalModifier()), ForceHit);
}

void nonhumanoid::Kick(lsquare* Square, bool ForceHit)
{
  EditNP(-50);
  EditAP(-GetKickAPCost());
  EditExperience(LEG_STRENGTH, 25);
  EditExperience(AGILITY, 50);
  Square->BeKicked(this, 0, GetKickDamage(), GetKickToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % GetCriticalModifier()), ForceHit);
}

bool nonhumanoid::Hit(character* Enemy, bool ForceHit)
{
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  if(GetBurdenState() == OVER_LOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return false;
    }

  /* Behold this Terrible Father of Gum Solutions! */

  uchar AttackStyle = GetAttackStyle();

  if(AttackStyle & USE_LEGS)
    {
      room* Room = Enemy->GetLSquareUnder()->GetRoom();

      if(Room && !Room->AllowKick(this, Enemy->GetLSquareUnder()))
	AttackStyle &= ~USE_LEGS;
    }

  ushort c, AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(AttackStyle & (1 << c))
      ++AttackStyles;

  ushort Chosen = RAND() % AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(AttackStyle & (1 << c) && AttackStyles++ == Chosen)
      {
	Chosen = 1 << c;
	break;
      }

  switch(Chosen)
    {
    case USE_ARMS:
      msgsystem::EnterBigMessageMode();
      Hostility(Enemy);
      UnarmedHit(Enemy, ForceHit);
      msgsystem::LeaveBigMessageMode();
      return true;
    case USE_LEGS:
      msgsystem::EnterBigMessageMode();
      Hostility(Enemy);
      Kick(Enemy->GetLSquareUnder(), ForceHit);
      msgsystem::LeaveBigMessageMode();
      return true;
    case USE_HEAD:
      msgsystem::EnterBigMessageMode();
      Hostility(Enemy);
      Bite(Enemy, ForceHit);
      msgsystem::LeaveBigMessageMode();
      return true;
    default:
      ABORT("Strange alien attack style requested!");
      return false;
    }
}

void nonhumanoid::UnarmedHit(character* Enemy, bool ForceHit)
{
  EditNP(-50);
  EditAP(-GetUnarmedAPCost());

  switch(Enemy->TakeHit(this, 0, GetUnarmedDamage(), GetUnarmedToHitValue(), RAND() % 26 - RAND() % 26, UNARMED_ATTACK, !(RAND() % GetCriticalModifier()), ForceHit))
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case HAS_DIED:
    case DID_NO_DAMAGE:
      EditExperience(ARM_STRENGTH, 50);
    case HAS_DODGED:
      EditExperience(DEXTERITY, 25);
    }
}

/* Returns the average number of APs required to kill Enemy */

float nonhumanoid::GetTimeToKill(const character* Enemy, bool UseMaxHP) const
{
  float Effectivity = 0;
  ushort AttackStyles = 0;

  if(IsUsingArms())
    {
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetUnarmedDamage()) + 1, GetUnarmedToHitValue(), AttackIsBlockable(UNARMED_ATTACK), UseMaxHP) * GetUnarmedAPCost());
      ++AttackStyles;
    }

  if(IsUsingLegs())
    {
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetKickDamage()) + 1, GetKickToHitValue(), AttackIsBlockable(KICK_ATTACK), UseMaxHP) * GetKickAPCost());
      ++AttackStyles;
    }

  if(IsUsingHead())
    {
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetBiteDamage()) + 1, GetBiteToHitValue(), AttackIsBlockable(BITE_ATTACK), UseMaxHP) * GetBiteAPCost());
      ++AttackStyles;
    }

  if(StateIsActivated(HASTE))
    Effectivity *= 2;

  if(StateIsActivated(SLOW))
    Effectivity /= 2;

  return AttackStyles / Effectivity;
}

void nonhumanoid::ApplyExperience(bool Edited)
{
  if(!IsAlive())
    {
      character::ApplyExperience(false);
      return;
    }

  if(CheckForAttributeIncrease(Strength, StrengthExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel stronger!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks stronger.", CHAR_NAME(DEFINITE));

      CalculateBurdenState();
      Edited = true;
    }
  else if(CheckForAttributeDecrease(Strength, StrengthExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel weaker.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks weaker.", CHAR_NAME(DEFINITE));

      CalculateBurdenState();
      Edited = true;
    }

  if(CheckForAttributeIncrease(Agility, AgilityExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel very agile!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks very agile.", CHAR_NAME(DEFINITE));

      Edited = true;
    }
  else if(CheckForAttributeDecrease(Agility, AgilityExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel slower.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks sluggish.", CHAR_NAME(DEFINITE));

      Edited = true;
    }

  character::ApplyExperience(Edited);
}

ushort nonhumanoid::GetAttribute(ushort Identifier) const
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::GetAttribute(Identifier);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
    {
      if(IsAlive())
	return Strength >> 1;
      else
	return GetTorso()->GetMainMaterial()->GetStrengthValue();
    }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    {
      if(IsAlive())
	return Agility >> 1;
      else
	return (GetTorso()->GetMainMaterial()->GetFlexibility() << 2);
    }
  else
    {
      ABORT("Illegal nonhumanoid attribute %d request!", Identifier);
      return 0xABBE;
    }
}

bool nonhumanoid::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::EditAttribute(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
    return IsAlive() && RawEditAttribute(Strength, Value, true);
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    return IsAlive() && RawEditAttribute(Agility, Value, true);
  else
    {
      ABORT("Illegal nonhumanoid attribute %d edit request!", Identifier);
      return false;
    }
}

void nonhumanoid::EditExperience(ushort Identifier, long Value)
{
  if(Identifier < BASE_ATTRIBUTES)
    character::EditExperience(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
    {
      if(IsAlive())
	StrengthExperience += Value;
    }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    {
      if(IsAlive())
	AgilityExperience += Value;
    }
  else
    ABORT("Illegal nonhumanoid attribute %d experience edit request!", Identifier);
}

void nonhumanoid::RaiseStats()
{
  Strength += 20;
  Agility += 20;
  character::RaiseStats();
}

void nonhumanoid::LowerStats()
{
  Strength -= 20;
  Agility -= 20;
  character::LowerStats();
}

ushort nonhumanoid::DrawStats(bool AnimationDraw) const
{
  if(AnimationDraw)
    return 3;

  ushort PanelPosX = RES.X - 96, PanelPosY = 3;

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Str %d", GetAttribute(ARM_STRENGTH));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Agi %d", GetAttribute(AGILITY));
  return PanelPosY;
}

void nonhumanoid::AddAttackInfo(felist& List) const
{
  if(IsUsingArms())
    {
      std::string Entry = "   unarmed attack";
      Entry.resize(50, ' ');
      Entry << GetUnarmedMinDamage() << '-' << GetUnarmedMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetUnarmedToHitValue());
      Entry.resize(70, ' ');
      Entry << GetUnarmedAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingLegs())
    {
      std::string Entry = "   kick attack";
      Entry.resize(50, ' ');
      Entry << GetKickMinDamage() << '-' << GetKickMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetKickToHitValue());
      Entry.resize(70, ' ');
      Entry << GetKickAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingHead())
    {
      std::string Entry = "   bite attack";
      Entry.resize(50, ' ');
      Entry << GetBiteMinDamage() << '-' << GetBiteMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetBiteToHitValue());
      Entry.resize(70, ' ');
      Entry << GetBiteAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

void nonhumanoid::CalculateBattleInfo()
{
  CalculateDodgeValue();
  CalculateUnarmedAttackInfo();
  CalculateKickAttackInfo();
  CalculateBiteAttackInfo();
}

void nonhumanoid::CalculateUnarmedAttackInfo()
{
  CalculateUnarmedDamage();
  CalculateUnarmedToHitValue();
  CalculateUnarmedAPCost();
}

void nonhumanoid::CalculateKickAttackInfo()
{
  CalculateKickDamage();
  CalculateKickToHitValue();
  CalculateKickAPCost();
}

void nonhumanoid::CalculateBiteAttackInfo()
{
  CalculateBiteDamage();
  CalculateBiteToHitValue();
  CalculateBiteAPCost();
}

void dog::BeTalkedTo()
{
  if(RAND() % 50)
    character::BeTalkedTo();
  else
    ADD_MESSAGE("\"Can't you understand I can't speak?\"");
}

ushort wolf::GetSkinColor() const
{
  ushort Element = 40 + RAND() % 50;
  return MakeRGB16(Element, Element, Element);
}

void nonhumanoid::ShowBattleInfo()
{
  felist CategoryList("Choose battle info category");
  game::SetStandardListAttributes(CategoryList);
  CategoryList.SetFlags(SELECTABLE|DRAW_BACKGROUND_AFTERWARDS);
  ushort Index = 1;
  ushort InfoToDraw[4];
  InfoToDraw[0] = 0;
  CategoryList.AddEntry("Dodge and move info", LIGHT_GRAY);

  if(IsUsingArms())
    {
      InfoToDraw[Index++] = 1;
      CategoryList.AddEntry("Unarmed info", LIGHT_GRAY);
    }

  if(IsUsingLegs())
    {
      InfoToDraw[Index++] = 2;
      CategoryList.AddEntry("Kick info", LIGHT_GRAY);
    }

  if(IsUsingHead())
    {
      InfoToDraw[Index++] = 3;
      CategoryList.AddEntry("Bite info", LIGHT_GRAY);
    }

  while(true)
    {
      Index = CategoryList.Draw();

      if(Index & FELIST_ERROR_BIT)
	return;

      switch(InfoToDraw[Index])
	{
	case 0: ShowDodgeAndMoveInfo(); break;
	case 1: ShowUnarmedInfo(); break;
	case 2: ShowKickInfo(); break;
	case 3: ShowBiteInfo(); break;
	}
    }
}

void nonhumanoid::ShowUnarmedInfo() const
{
  felist Info("Unarmed attack info:");
  game::SetStandardListAttributes(Info);
  Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
  short Bonus = GetCWeaponSkill(UNARMED)->GetBonus();

  if(Bonus > 100)
    Info.AddEntry(std::string("Category weapon skill bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);

  Info.AddEntry("", LIGHT_GRAY);

  /* Damage */

  Info.AddEntry("Damage: how much you can hurt an enemy", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  float Damage = 7.07e-3f * GetBaseUnarmedStrength();
  Info.AddEntry(std::string("Base: ") + int(Damage * 0.75f) + '-' + int(Damage * 1.25f + 1), LIGHT_GRAY);

  if(GetAttribute(ARM_STRENGTH) > 10)
    Info.AddEntry(std::string("Strength bonus: ") + '+' + int(sqrt(1000 * GetAttribute(ARM_STRENGTH)) - 100) + '%', LIGHT_GRAY);
  else if(GetAttribute(ARM_STRENGTH) < 10)
    Info.AddEntry(std::string("Strength penalty: ") + int(sqrt(1000 * GetAttribute(ARM_STRENGTH)) - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real damage: ") + GetUnarmedMinDamage() + '-' + GetUnarmedMaxDamage(), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* To hit value */

  Info.AddEntry("To hit value: determines your chance to hit", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 10", LIGHT_GRAY);
  Bonus = GetAttribute(DEXTERITY) * 10;

  if(Bonus > 100)
    Info.AddEntry(std::string("Dexterity bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Dexterity penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  Bonus = short(sqrt(GetAttribute(PERCEPTION) * 1000));

  if(Bonus > 100)
    Info.AddEntry(std::string("Perception bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Perception penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real to hit value: ") + int(GetUnarmedToHitValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Speed */

  Info.AddEntry("Speed: how often you are able to strike", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 10", LIGHT_GRAY);
  Bonus = APBonus(GetAttribute(DEXTERITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Dexterity bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Dexterity penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real speed: ") + 10000 / GetUnarmedAPCost(), MakeRGB16(220, 220, 220));
  Info.Draw();
}

void nonhumanoid::ShowKickInfo() const
{
  felist Info("Kick attack info:");
  game::SetStandardListAttributes(Info);
  Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
  short Bonus = GetCWeaponSkill(KICK)->GetBonus();

  if(Bonus > 100)
    Info.AddEntry(std::string("Category weapon skill bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);

  Info.AddEntry("", LIGHT_GRAY);

  /* Damage */

  Info.AddEntry("Damage: how much you can hurt an enemy", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  float Damage = 7.07e-3f * GetBaseKickStrength();
  Info.AddEntry(std::string("Base: ") + int(Damage * 0.75f) + '-' + int(Damage * 1.25f + 1), LIGHT_GRAY);

  if(GetAttribute(LEG_STRENGTH) > 10)
    Info.AddEntry(std::string("Strength bonus: ") + '+' + int(sqrt(1000 * GetAttribute(LEG_STRENGTH)) - 100) + '%', LIGHT_GRAY);
  else if(GetAttribute(LEG_STRENGTH) < 10)
    Info.AddEntry(std::string("Strength penalty: ") + int(sqrt(1000 * GetAttribute(LEG_STRENGTH)) - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real damage: ") + GetKickMinDamage() + '-' + GetKickMaxDamage(), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* To hit value */

  Info.AddEntry("To hit value: determines your chance to hit", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 5", LIGHT_GRAY);
  Bonus = GetAttribute(AGILITY) * 10;

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  Bonus = short(sqrt(GetAttribute(PERCEPTION) * 1000));

  if(Bonus > 100)
    Info.AddEntry(std::string("Perception bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Perception penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real to hit value: ") + int(GetKickToHitValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Speed */

  Info.AddEntry("Speed: how often you are able to strike", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 5", LIGHT_GRAY);
  Bonus = APBonus(GetAttribute(AGILITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real speed: ") + 10000 / GetKickAPCost(), MakeRGB16(220, 220, 220));
  Info.Draw();
}

void nonhumanoid::ShowBiteInfo() const
{
  felist Info("Bite attack info:");
  game::SetStandardListAttributes(Info);
  Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
  short Bonus = GetCWeaponSkill(BITE)->GetBonus();

  if(Bonus > 100)
    Info.AddEntry(std::string("Category weapon skill bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);

  Info.AddEntry("", LIGHT_GRAY);

  /* Damage */

  Info.AddEntry("Damage: how much you can hurt an enemy", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  float Damage = 7.07e-3f * GetBaseBiteStrength();
  Info.AddEntry(std::string("Base: ") + int(Damage * 0.75f) + '-' + int(Damage * 1.25f + 1), LIGHT_GRAY);

  if(GetAttribute(ARM_STRENGTH) > 10)
    Info.AddEntry(std::string("Strength bonus: ") + '+' + int(sqrt(1000 * GetAttribute(ARM_STRENGTH)) - 100) + '%', LIGHT_GRAY);
  else if(GetAttribute(ARM_STRENGTH) < 10)
    Info.AddEntry(std::string("Strength penalty: ") + int(sqrt(1000 * GetAttribute(ARM_STRENGTH)) - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real damage: ") + GetBiteMinDamage() + '-' + GetBiteMaxDamage(), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* To hit value */

  Info.AddEntry("To hit value: determines your chance to hit", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 5", LIGHT_GRAY);
  Bonus = GetAttribute(AGILITY) * 10;

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  Bonus = short(sqrt(GetAttribute(PERCEPTION) * 1000));

  if(Bonus > 100)
    Info.AddEntry(std::string("Perception bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Perception penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real to hit value: ") + int(GetBiteToHitValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Speed */

  Info.AddEntry("Speed: how often you are able to strike", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 10", LIGHT_GRAY);
  Bonus = APBonus(GetAttribute(AGILITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real speed: ") + 10000 / GetBiteAPCost(), MakeRGB16(220, 220, 220));
  Info.Draw();
}

void genetrixvesana::GetAICommand()
{
  if(!(RAND() % 40))
    {
      ushort NumberOfPlants = RAND() % 3 + RAND() % 3 + RAND() % 3 + RAND() % 3;

      for(ushort c = 0; c < 50 && NumberOfPlants; ++c)
	{
	  for(std::list<character*>::const_iterator i = game::GetTeam(PLAYER_TEAM)->GetMember().begin(); i != game::GetTeam(PLAYER_TEAM)->GetMember().end(); ++i)
	    if((*i)->IsEnabled())
	      {
		lsquare* LSquare = (*i)->GetNeighbourLSquare(RAND() & 7);

		if(LSquare && LSquare->IsWalkable(0) && !LSquare->GetCharacter())
		  {
		    character* NewPlant = new carnivorousplant(RAND() & 3 ? 0 : GREATER);
		    NewPlant->SetTeam(GetTeam());
		    LSquare->AddCharacter(NewPlant);
		    --NumberOfPlants;

		    if(NewPlant->CanBeSeenByPlayer())
		      {
			if((*i)->IsPlayer())
			  ADD_MESSAGE("%s sprouts from the ground near you.", NewPlant->CHAR_NAME(DEFINITE));
			else if((*i)->CanBeSeenByPlayer())
			  ADD_MESSAGE("%s sprouts from the ground near %s.", NewPlant->CHAR_NAME(DEFINITE), (*i)->CHAR_NAME(DEFINITE));
			else
			  ADD_MESSAGE("%s sprouts from the ground.", NewPlant->CHAR_NAME(DEFINITE));
		      }
		  }
	      }
	}
    }

  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char && GetRelation(Char) == HOSTILE && Hit(Char))
	    return;
	}
    }

  EditAP(-1000);
}

void nonhumanoid::AddAttributeInfo(std::string& Entry) const
{
  Entry.resize(45, ' ');
  Entry << GetAttribute(ARM_STRENGTH);
  Entry.resize(48, ' ');
  Entry << "-  -  " << GetAttribute(AGILITY);
  character::AddAttributeInfo(Entry);
}

ushort carnivorousplant::GetTorsoSpecialColor() const // the flower
{
  if(!Config)
    return MakeRGB16(RAND() % 100, 125 + RAND() % 125, RAND() % 100);
  else
    return MakeRGB16(RAND() % 100, RAND() % 100, 125 + RAND() % 125);
}

ushort genetrixvesana::GetTorsoSpecialColor() const // the flower
{
  return MakeRGB16(160, 0, 0);
}

void ostrich::GetAICommand()
{
  if(CheckForEnemies(false, false))
    return;

  if(GetPos() == vector2d(45, 45))
    HasBeenOnLandingSite = true;

  itemvector ItemVector;
  GetStackUnder()->FillItemVector(ItemVector);
  ushort BananasPicked = 0;  

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->IsBanana() && ItemVector[c]->CanBeSeenBy(this)
    && ItemVector[c]->IsPickable(this) && !MakesBurdened(GetCarriedWeight() + ItemVector[c]->GetWeight()))
      {
	ItemVector[c]->MoveTo(GetStack());
	++BananasPicked;
      }

  if(BananasPicked)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s picks up %s.", CHAR_NAME(DEFINITE), BananasPicked == 1 ? "the banana" : "some bananas");

      return;
    }

  if(!HasBeenOnLandingSite)
    {
      if(MoveTowards(vector2d(45, 45)))
	return;
    }
  else if(GetPos().Y == 54)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s leaves the town.", CHAR_NAME(DEFINITE));

      itemvector ItemVector;
      GetStack()->FillItemVector(ItemVector);

      for(ushort c = 0; c < ItemVector.size(); ++c)
	{
	  ItemVector[c]->RemoveFromSlot();
	  ItemVector[c]->SendToHell();
	}

      vector2d Where = GetArea()->GetNearestFreeSquare(this, vector2d(45, 0));

      if(Where == ERROR_VECTOR)
	Where = GetLevel()->GetRandomSquare(this, NOT_IN_ROOM); // this is odd but at least it doesn't crash

      Teleport(Where);
      RestoreHP();
      TemporaryState = 0;

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s enters the town.", CHAR_NAME(INDEFINITE));

      HasBeenOnLandingSite = false;
    }
  else
    {
      if(MoveTowards(vector2d(45, 54)))
	return;
    }

  EditAP(-1000);
}

void ostrich::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << HasBeenOnLandingSite;
}

void ostrich::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> HasBeenOnLandingSite;
}

void ostrich::VirtualConstructor(bool)
{
  HasBeenOnLandingSite = false;
}

bool ostrich::HandleCharacterBlockingTheWay(character* Char)
{
  return Char->GetPos() == vector2d(45, 45) && (Displace(Char, true) || Hit(Char));
}

void elpuri::Save(outputfile& SaveFile) const
{
  frog::Save(SaveFile);
  SaveFile << Active;
}

void elpuri::Load(inputfile& SaveFile)
{
  frog::Load(SaveFile);
  SaveFile >> Active;
}

void elpuri::GetAICommand()
{
  if(Active)
    character::GetAICommand();
  else
    {
      if(CheckForEnemies(false, false, false))
	return;

      EditAP(-1000);  
    }
}

ushort elpuri::ReceiveBodyPartDamage(character* Damager, ushort Damage, uchar Type, uchar BodyPartIndex, uchar Direction, bool PenetrateResistance, bool Critical, bool ShowNoDamageMsg)
{
  Active = true;
  return character::ReceiveBodyPartDamage(Damager, Damage, Type, BodyPartIndex, Direction, PenetrateResistance, Critical, ShowNoDamageMsg);
}

void elpuri::VirtualConstructor(bool Load)
{
  nonhumanoid::VirtualConstructor(Load);
  Active = false;
}

void mommo::CreateCorpse(lsquare* Square)
{
  for(ushort d = 0; d < 9; ++d)
    {
      lsquare* NeighbourSquare = Square->GetNeighbourLSquare(d);

      if(NeighbourSquare && NeighbourSquare->GetOLTerrain()->IsWalkable())
	NeighbourSquare->SpillFluid(RAND() % 20, GetBloodColor(), 5, 60);
    }

  SendToHell();  
}

void carnivorousplant::CreateCorpse(lsquare* Square)
{
  ushort Amount = !Config ? (RAND() & 1 ? 0 : (RAND() % 3 ? 1 : 2))
			  : (RAND() & 1 ? 1 : (RAND() & 1 ? 2 : (RAND() & 1 ? 3 : 4)));

  for(ushort c = 0; c < Amount; ++c)
    Square->AddItem(new kiwi);

  nonhumanoid::CreateCorpse(Square);
}

void genetrixvesana::CreateCorpse(lsquare* Square)
{
  for(ushort c = 0; c < 3; ++c)
    Square->AddItem(new pineapple);

  nonhumanoid::CreateCorpse(Square);
}

void nonhumanoid::AddSpecialStethoscopeInfo(felist& Info) const
{
  Info.AddEntry(std::string("Strength: ") + GetAttribute(ARM_STRENGTH), LIGHT_GRAY);
  Info.AddEntry(std::string("Agility: ") + GetAttribute(AGILITY), LIGHT_GRAY);
}

void floatingeye::GetAICommand()
{
  SeekLeader();

  if(CheckForEnemies(false, false))
    return;

  if(FollowLeader())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

bool floatingeye::Hit(character* Enemy, bool)
{
  if(IsPlayer())
    ADD_MESSAGE("You stare at %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
  else if(Enemy->IsPlayer() && CanBeSeenByPlayer())
    ADD_MESSAGE("%s stares at you.", CHAR_NAME(DEFINITE));

  EditAP(-1000);
  return true;
}

ushort floatingeye::TakeHit(character* Enemy, item* Weapon, float Damage, float ToHitValue, short Success, uchar Type, bool Critical, bool ForceHit)
{
  if(CanBeSeenBy(Enemy) && Enemy->HasEyes() && RAND() % 3 && Enemy->Faint(150 + RAND() % 150)) /* Changes for fainting 2 out of 3 */
    return HAS_FAILED;
  else
    return nonhumanoid::TakeHit(Enemy, Weapon, Damage, ToHitValue, Success, Type, Critical, ForceHit);
}

void elpuri::CreateCorpse(lsquare* Square)
{
  character::CreateCorpse(Square);
  Square->AddItem(new headofelpuri);
}

bool snake::SpecialBiteEffect(character* Char, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour)
    {
      Char->BeginTemporaryState(POISONED, 400 + RAND() % 200);
      return true;
    }
  else
    return false;
}

bool spider::SpecialBiteEffect(character* Char, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour)
    {
      Char->BeginTemporaryState(POISONED, 80 + RAND() % 40);
      return true;
    }
  else
    return false;
}

bool chameleon::SpecialEnemySightedReaction(character*)
{
  if(HP != MaxHP || !(RAND() % 3))
    {
      character* NewForm = PolymorphRandomly(10, 100, 500 + RAND() % 500);
      NewForm->GainIntrinsic(POLYMORPH);
      return true;
    }

  return false;
}

ushort chameleon::TakeHit(character* Enemy, item* Weapon, float Damage, float ToHitValue, short Success, uchar Type, bool Critical, bool ForceHit)
{
  ushort Return = nonhumanoid::TakeHit(Enemy, Weapon, Damage, ToHitValue, Success, Type, Critical, ForceHit);

  if(Return != HAS_DIED)
    {
      character* NewForm = PolymorphRandomly(10, 100, 500 + RAND() % 500);
      NewForm->GainIntrinsic(POLYMORPH);
    }

  return Return;
}

bool eddy::Hit(character* Enemy, bool)
{
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  if(RAND() % 2)
    {
      if(IsPlayer())
	ADD_MESSAGE("You engulf %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
      else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("%s engulfs %s.", CHAR_DESCRIPTION(DEFINITE), Enemy->CHAR_DESCRIPTION(DEFINITE));

      Enemy->GetLSquareUnder()->Teleport(this, "", game::GetDirectionForVector(Enemy->GetPos() - GetPos())); // Teleports also the on the square randomly
													     // Um, what?
    }
  else if(IsPlayer())
    ADD_MESSAGE("You miss %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));

  EditAP(-1000);
  return true;  
}
