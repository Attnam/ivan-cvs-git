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

const char* billswill::FirstPersonBiteHitVerb() const { return "emit psi waves at"; }
const char* billswill::FirstPersonCriticalBiteHitVerb() const { return "emit powerful psi waves at"; }
const char* billswill::ThirdPersonBiteHitVerb() const { return "emits psi waves at"; }
const char* billswill::ThirdPersonCriticalBiteHitVerb() const { return "emits powerful psi waves at"; }

bodypart* mommo::MakeBodyPart(ushort) const { return new mommotorso(0, NO_MATERIALS); }
const char* mommo::FirstPersonBiteVerb() const { return "vomit acidous slime at"; }
const char* mommo::FirstPersonCriticalBiteVerb() const { return "vomit very acidous slime at"; }
const char* mommo::ThirdPersonBiteVerb() const { return "vomits acidous slime at"; }
const char* mommo::ThirdPersonCriticalBiteVerb() const { return "vomits very acidous slime at"; }
const char* mommo::BiteNoun() const { return "slime"; }

ushort dolphin::GetSpecialBodyPartFlags(ushort, bool) const { return RAND() & (MIRROR|ROTATE); }

ushort chameleon::GetSkinColor() const { return MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190); }

void floatingeye::SetWayPoints(const std::vector<vector2d>& What) { WayPoints = What; }

bodypart* eddy::MakeBodyPart(ushort) const { return new eddytorso(0, NO_MATERIALS); }

const char* ghost::FirstPersonBiteHitVerb() const { return "touch"; }
const char* ghost::FirstPersonCriticalBiteHitVerb() const { return "awfully touch"; }
const char* ghost::ThirdPersonBiteHitVerb() const { return "touches"; }
const char* ghost::ThirdPersonCriticalBiteHitVerb() const { return "awfully touches"; }

const char* magpie::FirstPersonBiteVerb() const { return "peck"; }
const char* magpie::FirstPersonCriticalBiteVerb() const { return "critically peck"; }
const char* magpie::ThirdPersonBiteVerb() const { return "pecks"; }
const char* magpie::ThirdPersonCriticalBiteVerb() const { return "critically peck"; }

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

bool unicorn::SpecialEnemySightedReaction(character*)
{
  if((RAND() % 2 && IsInBadCondition()) || !(RAND() % 20))
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
  EditExperience(AGILITY, 50);
  Enemy->TakeHit(this, 0, GetBiteDamage(), GetBiteToHitValue(), RAND() % 26 - RAND() % 26, BITE_ATTACK, !(RAND() % GetCriticalModifier()), ForceHit);
}

void nonhumanoid::Kick(lsquare* Square, bool ForceHit)
{
  EditNP(-50);
  EditAP(-GetKickAPCost());
  EditExperience(LEG_STRENGTH, 25);
  EditExperience(AGILITY, 25);
  Square->BeKicked(this, 0, GetKickDamage(), GetKickToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % GetCriticalModifier()), ForceHit);
}

bool nonhumanoid::Hit(character* Enemy, bool ForceHit)
{
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
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

ushort nonhumanoid::DrawStats(bool AnimationDraw) const
{
  if(AnimationDraw)
    return 3;

  ushort PanelPosX = RES_X - 96, PanelPosY = 3;

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Str %d", GetAttribute(ARM_STRENGTH));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Agi %d", GetAttribute(AGILITY));
  return PanelPosY;
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
    {
      if(GetRelation(PLAYER) != HOSTILE)
	{
	  static bool Last;
	  const char* Reply;

	  if(!IsInBadCondition())
	    Reply = Last ? "barks happily" : "wags its tail happily";
	  else
	    Reply = Last ? "yelps" : "howls";

	  ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), Reply);
	  Last = !Last;
	}
      else
	character::BeTalkedTo();
    }
  else
    ADD_MESSAGE("\"Can't you understand I can't speak?\"");
}

ushort wolf::GetSkinColor() const
{
  ushort Element = 40 + RAND() % 50;
  return MakeRGB16(Element, Element, Element);
}

void genetrixvesana::GetAICommand()
{
  if(!(RAND() % 40))
    {
      ushort NumberOfPlants = RAND() % 3 + RAND() % 3 + RAND() % 3 + RAND() % 3;

      for(ushort c1 = 0; c1 < 50 && NumberOfPlants; ++c1)
	{
	  for(ushort c2 = 0; c2 < game::GetTeams() && NumberOfPlants; ++c2)
	    if(GetTeam()->GetRelation(game::GetTeam(c2)) == HOSTILE)
	      for(std::list<character*>::const_iterator i = game::GetTeam(c2)->GetMember().begin(); i != game::GetTeam(c2)->GetMember().end() && NumberOfPlants; ++i)
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
			      ADD_MESSAGE("%s sprouts from the ground near you.", NewPlant->CHAR_NAME(INDEFINITE));
			    else if((*i)->CanBeSeenByPlayer())
			      ADD_MESSAGE("%s sprouts from the ground near %s.", NewPlant->CHAR_NAME(INDEFINITE), (*i)->CHAR_NAME(DEFINITE));
			    else
			      ADD_MESSAGE("%s sprouts from the ground.", NewPlant->CHAR_NAME(INDEFINITE));
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

      vector2d Where = GetLevel()->GetNearestFreeSquare(this, vector2d(45, 0));

      if(Where == ERROR_VECTOR)
	Where = GetLevel()->GetRandomSquare(this, NOT_IN_ROOM); // this is odd but at least it doesn't crash

      Teleport(Where);
      RestoreHP();
      ResetStates();
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

ushort elpuri::ReceiveBodyPartDamage(character* Damager, ushort Damage, ushort Type, uchar BodyPartIndex, uchar Direction, bool PenetrateResistance, bool Critical, bool ShowNoDamageMsg)
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

      if(NeighbourSquare && NeighbourSquare->IsFlyable())
	NeighbourSquare->SpillFluid(RAND() % 20, GetBloodColor(), 5, 60);
    }

  SendToHell();  
}

void carnivorousplant::CreateCorpse(lsquare* Square)
{
  ushort Amount = !Config ? (RAND() % 7 ? 0 : 1)
			  : (RAND() & 1 ? 0 : (RAND() % 5 ? 1 : (RAND() % 5 ? 2 : 3)));

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
  Info.AddEntry(CONST_S("Strength: ") + GetAttribute(ARM_STRENGTH), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Agility: ") + GetAttribute(AGILITY), LIGHT_GRAY);
}

void floatingeye::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << WayPoints << NextWayPoint;
}

void floatingeye::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> WayPoints >> NextWayPoint;
}

void floatingeye::VirtualConstructor(bool Load)
{
  nonhumanoid::VirtualConstructor(Load);
  NextWayPoint = 0;
}

void floatingeye::GetAICommand()
{
  if(WayPoints.size() && WayPoint.X == -1)
    {
      if(GetPos() == WayPoints[NextWayPoint])
	if(NextWayPoint < WayPoints.size() - 1)
	  ++NextWayPoint;
	else
	  NextWayPoint = 0;

      WayPoint = WayPoints[NextWayPoint];
    }

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
    {
      if(!Enemy->IsPlayer())
	Enemy->EditExperience(WISDOM, 100);
      return HAS_FAILED;
    }
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
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
    return false;

  Hostility(Enemy);

  if(RAND() & 1)
    {
      if(IsPlayer())
	ADD_MESSAGE("You engulf %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
      else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("%s engulfs %s.", CHAR_DESCRIPTION(DEFINITE), Enemy->CHAR_DESCRIPTION(DEFINITE));

      Enemy->GetLSquareUnder()->Teleport(this, "", game::GetDirectionForVector(Enemy->GetPos() - GetPos())); // Teleports also the items on the square randomly
    }
  else if(IsPlayer())
    ADD_MESSAGE("You miss %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));

  EditAP(-1000);
  return true;  
}

void mushroom::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << Species;
}

void mushroom::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> Species;
}

void mushroom::GetAICommand()
{
  character* CharNear[8];
  ushort CharIndex = 0;

  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char)
	    {
	      if(GetRelation(Char) == HOSTILE || StateIsActivated(CONFUSED))
		CharNear[CharIndex++] = Char;
	    }
	}
    }

  if(CharIndex)
    Hit(CharNear[RAND() % CharIndex]);

  lsquare* CradleSquare = GetNeighbourLSquare(RAND() % 8);

  if(CradleSquare && !CradleSquare->GetCharacter() && CradleSquare->IsWalkable())
    {
      ushort SpoiledItems = 0;
      ushort MushroomsNear = 0;

      for(ushort d = 0; d < 8; ++d)
	{
	  lsquare* Square = CradleSquare->GetNeighbourLSquare(d);

	  if(Square)
	    {
	      character* Char = Square->GetCharacter();

	      if(Char && Char->IsMushroom())
		++MushroomsNear;

	      SpoiledItems += Square->GetSpoiledItems();
	    }
	}

      if((SpoiledItems && MushroomsNear < 5 && !RAND_N(50)) || (MushroomsNear < 3 && !RAND_N((1 + MushroomsNear) * 100)))
	{
	  mushroom* Child = static_cast<mushroom*>(GetProtoType()->Clone(GetConfig()));
	  Child->SetSpecies(Species);
	  Child->SetTeam(GetTeam());
	  CradleSquare->AddCharacter(Child);

	  if(Child->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s pops out from the ground.", Child->CHAR_NAME(INDEFINITE));
	}
    }

  EditAP(-1000);
  return;
}

void mushroom::VirtualConstructor(bool Load)
{
  nonhumanoid::VirtualConstructor(Load);

  if(!Load)
    switch(RAND() % 3)
      {
      case 0: SetSpecies(MakeRGB16(125 + RAND() % 125, RAND() % 100, RAND() % 100)); break;
      case 1: SetSpecies(MakeRGB16(RAND() % 100, 125 + RAND() % 125, RAND() % 100)); break;
      case 2: SetSpecies(MakeRGB16(RAND() % 100, RAND() % 100, 125 + RAND() % 125)); break;
      }
}

void magicmushroom::GetAICommand()
{
  if(!(RAND() % 500))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

      TeleportRandomly();
      EditAP(-1000);
    }
  else if(!(RAND() % 50))
    {
      lsquare* Square = GetNeighbourLSquare(RAND() % 8);

      if(Square && Square->IsFlyable())
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s releases odd-looking gas.", CHAR_NAME(DEFINITE));

	  Square->AddSmoke(new gas(MAGIC_VAPOUR, 1000));
	  EditAP(-1000);
	}
    }
  else
    mushroom::GetAICommand();
}

void mushroom::SetSpecies(ushort What)
{
  Species = What;
  UpdatePictures();
}

bool twoheadedmoose::Hit(character* Enemy, bool ForceHit)
{
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
    return false;

  if(GetBurdenState() == OVER_LOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return false;
    }

  Hostility(Enemy);
  msgsystem::EnterBigMessageMode();
  Bite(Enemy, ForceHit);
  character* ToBeHit = GetRandomNeighbour(HOSTILE);

  if(ToBeHit)
    Bite(ToBeHit, ForceHit);

  msgsystem::LeaveBigMessageMode();
  return true;
}

bool magpie::IsRetreating() const
{
  if(StateIsActivated(PANIC))
    return true;

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if((*i)->IsSparkling())
      return true;

  return false;
}

void magpie::GetAICommand()
{
  if(!IsRetreating())
    {
      character* Char = GetRandomNeighbour();

      if(Char)
	{
	  std::vector<item*> Sparkling;

	  for(stackiterator i = Char->GetStack()->GetBottom(); i.HasItem(); ++i)
	    {
	      if((*i)->IsSparkling() && !MakesBurdened((*i)->GetWeight()))
		Sparkling.push_back(*i);
	    }

	  if(!Sparkling.empty())
	    {
	      item* ToSteal = Sparkling[RAND() % Sparkling.size()];
	      ToSteal->RemoveFromSlot();
	      GetStack()->AddItem(ToSteal);

	      if(Char->IsPlayer())
		ADD_MESSAGE("%s steals your %s.", CHAR_NAME(DEFINITE), ToSteal->CHAR_NAME(UNARTICLED));

	      EditAP(-500);
	      return;
	    }
	}
    }

  nonhumanoid::GetAICommand();
}

void eddy::GetAICommand()
{
  if(!(RAND() % 500))
    {
      decoration* Couch = new decoration(COUCH);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s spits out %s.", CHAR_NAME(DEFINITE), Couch->CHAR_NAME(INDEFINITE));

      GetLSquareUnder()->ChangeOLTerrainAndUpdateLights(Couch);
      EditAP(-1000);
      return;
    }

  if(GetStackUnder()->GetItems() && !(RAND() % 10))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engulfs something under it.", CHAR_NAME(DEFINITE));

      GetStackUnder()->TeleportRandomly(3);
      EditAP(-1000);
      return;
    }

  if(!(RAND() % 100))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engulfs itself.", CHAR_NAME(DEFINITE));

      TeleportRandomly();
      EditAP(-1000);
      return;
    }

  nonhumanoid::GetAICommand();
}

void skunk::GetAICommand()
{
  if(!IsRetreating())
    {
      if(!RAND_N(4))
	{
	  character* Char = GetRandomNeighbour(HOSTILE);

	  if(Char && Char->GetLSquareUnder()->IsFlyable())
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s stinks.", CHAR_NAME(DEFINITE));

	      Char->GetLSquareUnder()->AddSmoke(new gas(SKUNK_SMELL, 500));
	      EditAP(-1000);
	      return;
	    }
	}
    }
  else if(RAND_N(2))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s stinks.", CHAR_NAME(DEFINITE));

      GetLSquareUnder()->AddSmoke(new gas(SKUNK_SMELL, 500));
    }

  nonhumanoid::GetAICommand();
}

ushort unicorn::TakeHit(character* Enemy, item* Weapon, float Damage, float ToHitValue, short Success, uchar Type, bool Critical, bool ForceHit)
{
  ushort Return = nonhumanoid::TakeHit(Enemy, Weapon, Damage, ToHitValue, Success, Type, Critical, ForceHit);

  if(Return != HAS_DIED)
    {
      if((RAND() % 3 && IsInBadCondition()) || !(RAND() % 10))
	TeleportRandomly();
    }

  return Return;
}

#ifdef WIZARD

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

void nonhumanoid::AddAttributeInfo(festring& Entry) const
{
  Entry.Resize(45, ' ');
  Entry << GetAttribute(ARM_STRENGTH);
  Entry.Resize(48, ' ');
  Entry << "-  -  " << GetAttribute(AGILITY);
  character::AddAttributeInfo(Entry);
}

void nonhumanoid::AddAttackInfo(felist& List) const
{
  festring Entry;

  if(IsUsingArms())
    {
      Entry = CONST_S("   unarmed attack");
      Entry.Resize(50, ' ');
      Entry << GetUnarmedMinDamage() << '-' << GetUnarmedMaxDamage();
      Entry.Resize(60, ' ');
      Entry << int(GetUnarmedToHitValue());
      Entry.Resize(70, ' ');
      Entry << GetUnarmedAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingLegs())
    {
      Entry = CONST_S("   kick attack");
      Entry.Resize(50, ' ');
      Entry << GetKickMinDamage() << '-' << GetKickMaxDamage();
      Entry.Resize(60, ' ');
      Entry << int(GetKickToHitValue());
      Entry.Resize(70, ' ');
      Entry << GetKickAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingHead())
    {
      Entry = CONST_S("   bite attack");
      Entry.Resize(50, ' ');
      Entry << GetBiteMinDamage() << '-' << GetBiteMaxDamage();
      Entry.Resize(60, ' ');
      Entry << int(GetBiteToHitValue());
      Entry.Resize(70, ' ');
      Entry << GetBiteAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

#else

void nonhumanoid::RaiseStats() { }
void nonhumanoid::LowerStats() { }
void nonhumanoid::AddAttributeInfo(festring&) const { }
void nonhumanoid::AddAttackInfo(felist&) const { }

#endif
