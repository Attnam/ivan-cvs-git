/* Compiled through charsset.cpp */

ushort nonhumanoid::GetUnarmedMinDamage() const { return ushort(UnarmedDamage * 0.75f); }
ushort nonhumanoid::GetUnarmedMaxDamage() const { return ushort(UnarmedDamage * 1.25f + 1); }
ushort nonhumanoid::GetKickMinDamage() const { return ushort(KickDamage * 0.75f); }
ushort nonhumanoid::GetKickMaxDamage() const { return ushort(KickDamage * 1.25f + 1); }
ushort nonhumanoid::GetBiteMinDamage() const { return ushort(BiteDamage * 0.75f); }
ushort nonhumanoid::GetBiteMaxDamage() const { return ushort(BiteDamage * 1.25f + 1); }
ushort nonhumanoid::GetCarryingStrength() const { return (Max<ushort>(GetAttribute(LEG_STRENGTH), 1) << 1) + CarryingBonus; }
bool nonhumanoid::UseMaterialAttributes() const { return GetTorso()->UseMaterialAttributes(); }

bool elpuri::SpecialEnemySightedReaction(character*) { return !(Active = true); }

const char* billswill::FirstPersonBiteVerb() const { return "emit psi waves at"; }
const char* billswill::FirstPersonCriticalBiteVerb() const { return "emit powerful psi waves at"; }
const char* billswill::ThirdPersonBiteVerb() const { return "emits psi waves at"; }
const char* billswill::ThirdPersonCriticalBiteVerb() const { return "emits powerful psi waves at"; }

bodypart* mommo::MakeBodyPart(ushort) const { return new mommotorso(0, NO_MATERIALS); }

ushort dolphin::GetSpecialBodyPartFlags(ushort, bool) const { return RAND() & (MIRROR|ROTATE); }

ushort chameleon::GetSkinColor() const { return MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190); }

void floatingeye::SetWayPoints(const std::vector<vector2d>& What) { WayPoints = What; }

bodypart* eddy::MakeBodyPart(ushort) const { return new eddytorso(0, NO_MATERIALS); }

const char* ghost::FirstPersonBiteVerb() const { return "touch"; }
const char* ghost::FirstPersonCriticalBiteVerb() const { return "awfully touch"; }
const char* ghost::ThirdPersonBiteVerb() const { return "touches"; }
const char* ghost::ThirdPersonCriticalBiteVerb() const { return "awfully touches"; }
bool ghost::SpecialEnemySightedReaction(character*) { return !(Active = true); }

const char* magpie::FirstPersonBiteVerb() const { return "peck"; }
const char* magpie::FirstPersonCriticalBiteVerb() const { return "critically peck"; }
const char* magpie::ThirdPersonBiteVerb() const { return "pecks"; }
const char* magpie::ThirdPersonCriticalBiteVerb() const { return "critically peck"; }

bodypart* largecreature::MakeBodyPart(ushort) const { return new largetorso(0, NO_MATERIALS); }
lsquare* largecreature::GetNeighbourLSquare(ushort Index) const { return static_cast<lsquare*>(GetNeighbourSquare(Index)); }
wsquare* largecreature::GetNeighbourWSquare(ushort Index) const { return static_cast<wsquare*>(GetNeighbourSquare(Index)); }

bodypart* hattifattener::MakeBodyPart(ushort) const { return new hattifattenertorso(0, NO_MATERIALS); }

ushort vladimir::GetSkinColor() const { return MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190); }

bool elpuri::Hit(character* Enemy, vector2d, uchar, bool ForceHit)
{
  character* EnemyHit[MAX_NEIGHBOUR_SQUARES];
  ushort EnemiesHit = 0;

  for(ushort d = 0; d < GetExtendedNeighbourSquares(); ++d)
    if(IsEnabled())
      {
	lsquare* Square = GetNeighbourLSquare(d);

	if(Square)
	  {
	    character* ByStander = Square->GetCharacter();

	    if(ByStander && (ByStander == Enemy || GetRelation(ByStander) == HOSTILE))
	      {
		bool Abort = false;

		for(ushort c = 0; c < EnemiesHit; ++c)
		  if(EnemyHit[c] == ByStander)
		    Abort = true;

		if(!Abort)
		  {
		    nonhumanoid::Hit(ByStander, Square->GetPos(), YOURSELF, ForceHit);
		    ByStander->DamageAllItems(this, RAND() % 36 + RAND() % 36, PHYSICAL_DAMAGE);
		    EnemyHit[EnemiesHit++] = ByStander;
		  }
	      }

	    Square->GetStack()->ReceiveDamage(this, RAND() % 36 + RAND() % 36, PHYSICAL_DAMAGE);

	    for(ushort c = 0; c < 4; ++c)
	      {
		square* STUSS = Square->GetSideStack(c)->GetSquareTrulyUnder();

		if(STUSS && IsEnabled() && IsOver(STUSS->GetPos()))
		  Square->GetSideStack(c)->ReceiveDamage(this, RAND() % 36 + RAND() % 36, PHYSICAL_DAMAGE);
	      }
	  }
      }

  EditAP(-500);
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

void nonhumanoid::Bite(character* Enemy, vector2d HitPos, uchar Direction, bool ForceHit)
{
  EditNP(-50);
  EditAP(-GetBiteAPCost());
  EditExperience(ARM_STRENGTH, 20);
  EditExperience(AGILITY, 40);
  Enemy->TakeHit(this, 0, GetTorso(), HitPos, GetBiteDamage(), GetBiteToHitValue(), RAND() % 26 - RAND() % 26, BITE_ATTACK, Direction, !(RAND() % GetCriticalModifier()), ForceHit);
}

void nonhumanoid::Kick(lsquare* Square, uchar Direction, bool ForceHit)
{
  EditNP(-50);
  EditAP(-GetKickAPCost());

  if(Square->BeKicked(this, 0, GetTorso(), GetKickDamage(), GetKickToHitValue(), RAND() % 26 - RAND() % 26, Direction, !(RAND() % GetCriticalModifier()), ForceHit))
    {
      EditExperience(LEG_STRENGTH, 40);
      EditExperience(AGILITY, 20);
    }
}

bool nonhumanoid::Hit(character* Enemy, vector2d HitPos, uchar Direction, bool ForceHit)
{
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
    return false;

  if(!IsPlayer() && GetAttribute(WISDOM) >= Enemy->GetAttackWisdomLimit())
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
      room* Room = GetNearLSquare(HitPos)->GetRoom();

      if(Room && !Room->AllowKick(this, GetNearLSquare(HitPos)))
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
      UnarmedHit(Enemy, HitPos, Direction, ForceHit);
      msgsystem::LeaveBigMessageMode();
      return true;
    case USE_LEGS:
      msgsystem::EnterBigMessageMode();
      Hostility(Enemy);
      Kick(GetNearLSquare(HitPos), Direction, ForceHit);
      msgsystem::LeaveBigMessageMode();
      return true;
    case USE_HEAD:
      msgsystem::EnterBigMessageMode();
      Hostility(Enemy);
      Bite(Enemy, HitPos, Direction, ForceHit);
      msgsystem::LeaveBigMessageMode();
      return true;
    default:
      ABORT("Strange alien attack style requested!");
      return false;
    }
}

void nonhumanoid::UnarmedHit(character* Enemy, vector2d HitPos, uchar Direction, bool ForceHit)
{
  EditNP(-50);
  EditAP(-GetUnarmedAPCost());

  switch(Enemy->TakeHit(this, 0, GetTorso(), HitPos, GetUnarmedDamage(), GetUnarmedToHitValue(), RAND() % 26 - RAND() % 26, UNARMED_ATTACK, Direction, !(RAND() % GetCriticalModifier()), ForceHit))
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case HAS_DIED:
    case DID_NO_DAMAGE:
      EditExperience(ARM_STRENGTH, 40);
    case HAS_DODGED:
      EditExperience(DEXTERITY, 20);
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
  if(GetTorso()->UseMaterialAttributes())
    {
      character::ApplyExperience(false);
      return;
    }

  if(CheckForAttributeIncrease(Strength, StrengthExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel stronger!");
      else if(IsPet() && CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks stronger.", CHAR_NAME(DEFINITE));

      CalculateBurdenState();
      Edited = true;
    }
  else if(CheckForAttributeDecrease(Strength, StrengthExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel weaker.");
      else if(IsPet() && CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks weaker.", CHAR_NAME(DEFINITE));

      CalculateBurdenState();
      Edited = true;
    }

  if(CheckForAttributeIncrease(Agility, AgilityExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel very agile!");
      else if(IsPet() && CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks very agile.", CHAR_NAME(DEFINITE));

      Edited = true;
    }
  else if(CheckForAttributeDecrease(Agility, AgilityExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel slower.");
      else if(IsPet() && CanBeSeenByPlayer())
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
      if(!GetTorso()->UseMaterialAttributes())
	return Strength >> 1;
      else
	return GetTorso()->GetMainMaterial()->GetStrengthValue();
    }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    {
      if(!GetTorso()->UseMaterialAttributes())
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
    return !GetTorso()->UseMaterialAttributes() && RawEditAttribute(Strength, Value, true);
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    return !GetTorso()->UseMaterialAttributes() && RawEditAttribute(Agility, Value, true);
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
      if(!GetTorso()->UseMaterialAttributes())
	{
	  if(!IsPlayer())
	    Value <<= 1;

	  StrengthExperience += Value;
	}
    }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    {
      if(!GetTorso()->UseMaterialAttributes())
	{
	  if(!IsPlayer())
	    Value <<= 1;

	  AgilityExperience += Value;
	}
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
  if(RAND() % 25)
    {
      if(GetRelation(PLAYER) != HOSTILE)
	{
	  static bool Last;
	  const char* Reply;

	  if(GetHP() << 1 > GetMaxHP())
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
  ++TurnsExisted;

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
		    lsquare* LSquare = (*i)->GetNeighbourLSquare(RAND() % GetNeighbourSquares());

		    if(LSquare && (LSquare->GetWalkability() & WALK) && !LSquare->GetCharacter())
		      {
			character* NewPlant;
			ulong RandomValue = RAND() % TurnsExisted;

			if(RandomValue < 250)
			  NewPlant = new carnivorousplant;
			else if(RandomValue < 1500)
			  NewPlant = new carnivorousplant(GREATER);
			else
			  NewPlant = new carnivorousplant(GIANT);

			for(ushort c = 3; c < TurnsExisted / 500; ++c)
			  NewPlant->EditAllAttributes(1);

			NewPlant->SetTeam(GetTeam());
			NewPlant->PutTo(LSquare->GetPos());
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

  AttackAdjacentEnemyAI();
}

ushort carnivorousplant::GetTorsoSpecialColor() const // the flower
{
  if(!GetConfig())
    return MakeRGB16(RAND() % 100, 125 + RAND() % 125, RAND() % 100);
  else if(GetConfig() == GREATER)
    return MakeRGB16(RAND() % 100, RAND() % 100, 125 + RAND() % 125);
  else
    return MakeRGB16(125 + RAND() % 125, 125 + RAND() % 125, RAND() % 100);
}

void ostrich::GetAICommand()
{
  if(game::TweraifIsFree())
    {
      nonhumanoid::GetAICommand();
      return;
    }

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
      SetGoingTo(vector2d(45, 45));

      if(MoveTowardsTarget())
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

      Move(Where, true);
      RestoreHP();
      ResetStates();
      TemporaryState = 0;

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s enters the town.", CHAR_NAME(INDEFINITE));

      HasBeenOnLandingSite = false;
    }
  else
    {
      SetGoingTo(vector2d(45, 54));

      if(MoveTowardsTarget())
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

bool ostrich::HandleCharacterBlockingTheWay(character* Char, vector2d Pos, uchar Dir)
{
  return Char->GetPos() == vector2d(45, 45) && (Displace(Char, true) || Hit(Char, Pos, Dir));
}

void elpuri::Save(outputfile& SaveFile) const
{
  largecreature::Save(SaveFile);
  SaveFile << Active;
}

void elpuri::Load(inputfile& SaveFile)
{
  largecreature::Load(SaveFile);
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

ushort elpuri::ReceiveBodyPartDamage(character* Damager, ushort Damage, ushort Type, uchar BodyPartIndex, uchar Direction, bool PenetrateResistance, bool Critical, bool ShowNoDamageMsg, bool CaptureBodyPart)
{
  Active = true;
  return character::ReceiveBodyPartDamage(Damager, Damage, Type, BodyPartIndex, Direction, PenetrateResistance, Critical, ShowNoDamageMsg, CaptureBodyPart);
}

void elpuri::VirtualConstructor(bool Load)
{
  nonhumanoid::VirtualConstructor(Load);
  Active = false;
}

void mommo::CreateCorpse(lsquare* Square)
{
  for(ushort d = 0; d < GetExtendedNeighbourSquares(); ++d)
    {
      lsquare* NeighbourSquare = Square->GetNeighbourLSquare(d);

      if(NeighbourSquare && NeighbourSquare->IsFlyable())
	NeighbourSquare->SpillFluid(0, new liquid(BROWN_SLIME, 250 + RAND() % 250));
    }

  SendToHell();  
}

void carnivorousplant::CreateCorpse(lsquare* Square)
{
  ushort Amount = !GetConfig() ? (RAND() % 7 ? 0 : 1) : GetConfig() == GREATER ? (RAND() & 1 ? 0 : (RAND() % 5 ? 1 : (RAND() % 5 ? 2 : 3))) : (!(RAND() % 3) ? 0 : (RAND() % 3 ? 1 : (RAND() % 3 ? 2 : 3)));

  for(ushort c = 0; c < Amount; ++c)
    Square->AddItem(new kiwi);

  nonhumanoid::CreateCorpse(Square);
}

void genetrixvesana::CreateCorpse(lsquare* Square)
{
  for(ushort c = 0; c < 3; ++c)
    Square->AddItem(new pineapple);

  largecreature::CreateCorpse(Square);
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
  if(WayPoints.size() && !IsGoingSomeWhere())
    {
      if(GetPos() == WayPoints[NextWayPoint])
	if(NextWayPoint < WayPoints.size() - 1)
	  ++NextWayPoint;
	else
	  NextWayPoint = 0;

      GoingTo = WayPoints[NextWayPoint];
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

bool floatingeye::Hit(character* Enemy, vector2d, uchar, bool)
{
  if(IsPlayer())
    ADD_MESSAGE("You stare at %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
  else if(Enemy->IsPlayer() && CanBeSeenByPlayer())
    ADD_MESSAGE("%s stares at you.", CHAR_NAME(DEFINITE));

  EditAP(-1000);
  return true;
}

ushort floatingeye::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, vector2d HitPos, float Damage, float ToHitValue, short Success, uchar Type, uchar Direction, bool Critical, bool ForceHit)
{
  if(CanBeSeenBy(Enemy) && Enemy->HasEyes() && RAND() % 3 && Enemy->Faint(150 + RAND() % 150)) /* Changes for fainting 2 out of 3 */
    {
      if(!Enemy->IsPlayer())
	Enemy->EditExperience(WISDOM, 100);

      return HAS_FAILED;
    }
  else
    return nonhumanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage, ToHitValue, Success, Type, Direction, Critical, ForceHit);
}

void elpuri::CreateCorpse(lsquare* Square)
{
  largecreature::CreateCorpse(Square);
  Square->AddItem(new headofelpuri);
}

bool snake::SpecialBiteEffect(character* Char, vector2d, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour)
    {
      Char->BeginTemporaryState(POISONED, 400 + RAND() % 200);
      return true;
    }
  else
    return false;
}

bool spider::SpecialBiteEffect(character* Char, vector2d, uchar, uchar, bool BlockedByArmour)
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

ushort chameleon::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, vector2d HitPos, float Damage, float ToHitValue, short Success, uchar Type, uchar Direction, bool Critical, bool ForceHit)
{
  ushort Return = nonhumanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage, ToHitValue, Success, Type, Direction, Critical, ForceHit);

  if(Return != HAS_DIED)
    {
      character* NewForm = PolymorphRandomly(10, 100, 500 + RAND() % 500);
      NewForm->GainIntrinsic(POLYMORPH);
    }

  return Return;
}

bool eddy::Hit(character* Enemy, vector2d, uchar, bool)
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

      Enemy->TeleportRandomly();
    }
  else if(IsPlayer())
    ADD_MESSAGE("You miss %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));

  EditAP(-500);
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
  lsquare* CradleSquare = GetNeighbourLSquare(RAND() % 8);

  if(CradleSquare && !CradleSquare->GetCharacter() && (CradleSquare->GetWalkability() & WALK))
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
	  Child->PutTo(CradleSquare->GetPos());
	  for(ushort c = 0; c < BASE_ATTRIBUTES; ++c)
	    Child->BaseAttribute[c] = RandomizeBabyAttribute(BaseAttribute[c] * 4);

	  if(Child->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s pops out from the ground.", Child->CHAR_NAME(INDEFINITE));
	}
    }

  AttackAdjacentEnemyAI();
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
  if(!(RAND() % 750))
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

bool twoheadedmoose::Hit(character* Enemy, vector2d HitPos, uchar Direction, bool ForceHit)
{
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
    return false;

  if(!IsPlayer() && GetAttribute(WISDOM) >= Enemy->GetAttackWisdomLimit())
    return false;

  if(GetBurdenState() == OVER_LOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return false;
    }

  Hostility(Enemy);
  msgsystem::EnterBigMessageMode();
  Bite(Enemy, HitPos, Direction, ForceHit);
  vector2d Pos[MAX_NEIGHBOUR_SQUARES];
  character* Char[MAX_NEIGHBOUR_SQUARES];
  ushort Index = 0;

  for(ushort d = 0; d < GetNeighbourSquares(); ++d)
    {
      lsquare* LSquare = GetNeighbourLSquare(d);

      if(LSquare) 
	{
	  character* Enemy = LSquare->GetCharacter();

	  if(Enemy && GetRelation(Enemy) == HOSTILE && GetAttribute(WISDOM) < Enemy->GetAttackWisdomLimit())
	    {
	      Pos[Index] = LSquare->GetPos();
	      Char[Index++] = Enemy;
	    }
	}
    }

  if(Index)
    {
      ushort ChosenIndex = RAND() % Index;
      Bite(Char[ChosenIndex], Pos[ChosenIndex], game::GetDirectionForVector(Pos[ChosenIndex] - GetPos()), ForceHit);
    }

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
  if(!GetLSquareUnder()->GetOLTerrain() && !(RAND() % 500))
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

	  if(Char)
	    {
	      ushort Amount = 500 / Char->GetSquaresUnder();
	      bool Success = false;

	      for(ushort c = 0; c < Char->GetSquaresUnder(); ++c)
		if(Char->GetLSquareUnder(c)->IsFlyable())
		  {
		    Success = true;
		    Char->GetLSquareUnder(c)->AddSmoke(new gas(SKUNK_SMELL, Amount));
		  }

	      if(Success)
		{
		  if(CanBeSeenByPlayer())
		    ADD_MESSAGE("%s stinks.", CHAR_NAME(DEFINITE));

		  EditAP(-1000);
		  return;
		}
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

ushort unicorn::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, vector2d HitPos, float Damage, float ToHitValue, short Success, uchar Type, uchar Direction, bool Critical, bool ForceHit)
{
  ushort Return = nonhumanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage, ToHitValue, Success, Type, Direction, Critical, ForceHit);

  if(Return != HAS_DIED)
    {
      if((RAND() % 3 && IsInBadCondition()) || !(RAND() % 10))
	TeleportRandomly();
    }

  return Return;
}

bool elpuri::TryToRiseFromTheDead()
{
  character::TryToRiseFromTheDead();

  for(ushort c = 0; c < GetSquaresUnder(); ++c)
    for(stackiterator i = GetLSquareUnder(c)->GetStack()->GetBottom(); i.HasItem(); ++i)
      if(i->IsHeadOfElpuri())
	{
	  i->SendToHell();
	  i->RemoveFromSlot();
	  return true;
	}

  if(CanBeSeenByPlayer())
    {
      ADD_MESSAGE("The headless body of %s vibrates violently.", CHAR_NAME(DEFINITE));
      ADD_MESSAGE("%s dies.", CHAR_NAME(DEFINITE));
    }

  return false;
}

bool nonhumanoid::EditAllAttributes(short Amount)
{
  Strength += Amount << 1;
  Agility += Amount << 1;

  if(!IsPlayer())
    {
      if(Strength > 200)
	Strength = 200;

      if(Agility > 200)
	Agility = 200;
    }

  return character::EditAllAttributes(Amount) || Strength < 200 || Agility < 200;
}

#ifdef WIZARD

void nonhumanoid::AddAttributeInfo(festring& Entry) const
{
  Entry.Resize(45);
  Entry << GetAttribute(ARM_STRENGTH);
  Entry.Resize(48);
  Entry << "-  -  " << GetAttribute(AGILITY);
  character::AddAttributeInfo(Entry);
}

void nonhumanoid::AddAttackInfo(felist& List) const
{
  festring Entry;

  if(IsUsingArms())
    {
      Entry = CONST_S("   unarmed attack");
      Entry.Resize(50);
      Entry << GetUnarmedMinDamage() << '-' << GetUnarmedMaxDamage();
      Entry.Resize(60);
      Entry << int(GetUnarmedToHitValue());
      Entry.Resize(70);
      Entry << GetUnarmedAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingLegs())
    {
      Entry = CONST_S("   kick attack");
      Entry.Resize(50);
      Entry << GetKickMinDamage() << '-' << GetKickMaxDamage();
      Entry.Resize(60);
      Entry << int(GetKickToHitValue());
      Entry.Resize(70);
      Entry << GetKickAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingHead())
    {
      Entry = CONST_S("   bite attack");
      Entry.Resize(50);
      Entry << GetBiteMinDamage() << '-' << GetBiteMaxDamage();
      Entry.Resize(60);
      Entry << int(GetBiteToHitValue());
      Entry.Resize(70);
      Entry << GetBiteAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

#else

void nonhumanoid::AddAttributeInfo(festring&) const { }
void nonhumanoid::AddAttackInfo(felist&) const { }

#endif

bool elpuri::MustBeRemovedFromBone() const
{
  return !IsEnabled() || GetTeam()->GetID() != MONSTER_TEAM || GetDungeon()->GetIndex() != ELPURI_CAVE || GetLevel()->GetIndex() != DARK_LEVEL;
}

bool genetrixvesana::MustBeRemovedFromBone() const
{
  return !IsEnabled() || GetTeam()->GetID() != MONSTER_TEAM || GetDungeon()->GetIndex() != UNDER_WATER_TUNNEL || GetLevel()->GetIndex() != VESANA_LEVEL;
}

void ghost::AddName(festring& String, uchar Case) const
{
  if(OwnerSoul.IsEmpty() || Case & PLURAL)
    character::AddName(String, Case);
  else
    {
      character::AddName(String, (Case|ARTICLE_BIT)&~INDEFINE_BIT);
      String << " of " << OwnerSoul;
    }
}

void ghost::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << OwnerSoul << Active;
}

void ghost::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> OwnerSoul >> Active;
}

bool ghost::RaiseTheDead(character* Summoner)
{
  itemvector ItemVector;
  GetStackUnder()->FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->SuckSoul(this, Summoner))
      return true;

  if(IsPlayer())
    ADD_MESSAGE("You shudder.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s shudders.", CHAR_NAME(DEFINITE));

  return false;
}

void ghost::VirtualConstructor(bool)
{
  Active = true;
}

ushort ghost::ReceiveBodyPartDamage(character* Damager, ushort Damage, ushort Type, uchar BodyPartIndex, uchar Direction, bool PenetrateResistance, bool Critical, bool ShowNoDamageMsg, bool CaptureBodyPart)
{
  if(Type != SOUND)
    {
      Active = true;
      return character::ReceiveBodyPartDamage(Damager, Damage, Type, BodyPartIndex, Direction, PenetrateResistance, Critical, ShowNoDamageMsg, CaptureBodyPart);
    }
  else
    return 0;
}

void ghost::GetAICommand()
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

ushort largecreature::GetSquareIndex(vector2d Pos) const
{
  vector2d RelativePos = Pos - GetPos();
  return RelativePos.X + (RelativePos.Y << 1);
}

square* largecreature::GetNeighbourSquare(ushort Index) const
{
  square* SquareUnder = GetSquareUnder();
  area* Area = SquareUnder->GetArea();
  vector2d Pos = SquareUnder->GetPos() + game::GetLargeMoveVector(Index);
  return Area->IsValidPos(Pos) ? SquareUnder->GetArea()->GetSquare(Pos) : 0;
}

ushort largecreature::CalculateNewSquaresUnder(lsquare** NewSquare, vector2d Pos) const
{
  level* Level = GetLevel();

  for(ushort c = 0; c < 4; ++c)
    {
      vector2d SquarePos = Pos + game::GetLargeMoveVector(12 + c);

      if(Level->IsValidPos(SquarePos))
	NewSquare[c] = Level->GetLSquare(SquarePos);
      else
	return 0;
    }

  return 4;
}

bool largecreature::IsFreeForMe(square* Square) const
{
  vector2d Pos = Square->GetPos();
  area* Area = Square->GetArea();

  for(ushort c = 0; c < 4; ++c)
    {
      vector2d SquarePos = Pos + game::GetLargeMoveVector(12 + c);

      if(!Area->IsValidPos(SquarePos) || (Area->GetSquare(SquarePos)->GetCharacter() && Area->GetSquare(SquarePos)->GetCharacter() != static_cast<const character*>(this)))
	return false;
    }

  return true;
}

bool largecreature::CanTheoreticallyMoveOn(const lsquare* LSquare) const
{
  vector2d Pos = LSquare->GetPos();
  level* Level = LSquare->GetLevel();

  for(ushort c = 0; c < 4; ++c)
    {
      vector2d SquarePos = Pos + game::GetLargeMoveVector(12 + c);

      if(!Level->IsValidPos(SquarePos) || !(GetMoveType() & Level->GetLSquare(SquarePos)->GetTheoreticalWalkability()))
	return false;
    }

  return true;
}

bool largecreature::CanMoveOn(const lsquare* LSquare) const
{
  vector2d Pos = LSquare->GetPos();
  level* Level = LSquare->GetLevel();

  for(ushort c = 0; c < 4; ++c)
    {
      vector2d SquarePos = Pos + game::GetLargeMoveVector(12 + c);

      if(!Level->IsValidPos(SquarePos) || !(GetMoveType() & Level->GetLSquare(SquarePos)->GetWalkability()))
	return false;
    }

  return true;
}

bool largecreature::CanMoveOn(const square* Square) const
{
  vector2d Pos = Square->GetPos();
  area* Area = Square->GetArea();

  for(ushort c = 0; c < 4; ++c)
    {
      vector2d SquarePos = Pos + game::GetLargeMoveVector(12 + c);

      if(!Area->IsValidPos(SquarePos) || !(GetMoveType() & Area->GetSquare(SquarePos)->GetSquareWalkability()))
	return false;
    }

  return true;
}

void largecreature::PutTo(vector2d Pos)
{
  for(ushort c = 0; c < 4; ++c)
    {
      SquareUnder[c] = game::GetCurrentArea()->GetSquare(Pos + game::GetLargeMoveVector(12 + c));
      SquareUnder[c]->AddCharacter(this);
    }
}

void largecreature::Remove()
{
  for(ushort c = 0; c < 4; ++c)
    GetSquareUnder(c)->RemoveCharacter();
}

void largecreature::CreateCorpse(lsquare* Square)
{
  if(!BodyPartsDisappearWhenSevered())
    {
      corpse* Corpse = new largecorpse(0, NO_MATERIALS);
      Corpse->SetDeceased(this);
      Square->AddItem(Corpse);
      Disable();
    }
  else
    SendToHell();
}

void largecreature::LoadSquaresUnder()
{
  for(ushort c = 0; c < 4; ++c)
    SquareUnder[c] = game::GetSquareInLoad()->GetArea()->GetSquare(game::GetSquareInLoad()->GetPos() + game::GetLargeMoveVector(12 + c));
}

bool vladimir::MustBeRemovedFromBone() const
{
  return !IsEnabled() || GetTeam()->GetID() != IVAN_TEAM || GetDungeon()->GetIndex() != ELPURI_CAVE|| GetLevel()->GetIndex() != IVAN_LEVEL;
}

void hattifattener::GetAICommand()
{
  if(!(RAND() % 7))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s emits a lightning bolt!", CHAR_DESCRIPTION(DEFINITE));

      GetLevel()->LightningBeam(this, "killed by a hattifattener's lightning", GetPos(), WHITE, BEAM_LIGHTNING, RAND() & 7, 1 + (RAND() & 7));
      EditAP(-1000);
      return;
    }

  SeekLeader();

  if(FollowLeader())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void hattifattener::CreateCorpse(lsquare* Square)
{
  level* Level = Square->GetLevel();

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, Level->GetBorder(), Square->GetPos(), 2);
  Level->LightningVisualizer(Rect, WHITE);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      Level->GetLSquare(x, y)->Lightning(this, "killed by electricity released by a dying hattifattener", YOURSELF);

  SendToHell();
}

void hedgehog::SpecialBodyDefenceEffect(character* Enemy, bodypart* BodyPart, uchar Type)
{   
  if(Type != WEAPON_ATTACK && RAND() & 1)
    {
      if(Enemy->IsPlayer())
	ADD_MESSAGE("%s spines jab your %s!", CHAR_POSSESSIVE_PRONOUN, BodyPart->GetBodyPartName().CStr());
      else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("%s spines jab %s!", CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_NAME(DEFINITE));

      Enemy->ReceiveBodyPartDamage(this, 1 + (RAND() & 1), PHYSICAL_DAMAGE, BodyPart->GetBodyPartIndex(), YOURSELF, false, false, true, false);
    }
}

void genetrixvesana::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << TurnsExisted;
}

void genetrixvesana::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> TurnsExisted;
}

void genetrixvesana::VirtualConstructor(bool)
{
  TurnsExisted = 0;
}

bool largecreature::CreateRoute()
{
  Route.clear();

  if(GetAttribute(INTELLIGENCE) >= 10 && !StateIsActivated(CONFUSED))
    {
      node* Node = GetLevel()->FindRoute(GetPos(), GoingTo, Illegal, 0, this);

      if(Node)
	while(Node->Last)
	  {
	    Route.push_back(Node->Pos);
	    Node = Node->Last;
	  }
      else
	TerminateGoingTo();

      IntelligenceAction(5);
      return true;
    }
  else
    return false;
}

void bunny::GetAICommand()
{
  if(GetConfig() < 4 && GetNP() > (SATIATED_LEVEL + BLOATED_LEVEL) >> 1)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s looks more mature.", CHAR_NAME(DEFINITE));

      GetTorso()->SetSize(GetTorso()->GetSize() << 1);
      Strength <<= 1;
      Agility <<= 1;

      for(ushort c = 0; c < BASE_ATTRIBUTES; ++c)
	BaseAttribute[c] <<= 1;

      GetTorso()->GetMainMaterial()->SetVolume(GetTorso()->GetMainMaterial()->GetVolume() << 1);
      SetConfig(GetConfig() + 2);
      RestoreHP();
    }

  SeekLeader();

  if(FollowLeader())
    return;

  if(CheckForEnemies(true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(CheckForDoors())
    return;

  if(CheckForFood(5))
    return;

  if(CheckForMatePartner())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void bunny::SignalNaturalGeneration()
{
  character* Partner = new bunny(GetConfig()^1);
  Partner->SetTeam(GetTeam());
  Partner->PutNear(GetPos());
}

bool bunny::CheckForMatePartner()
{
  if(GetConfig() == ADULT_MALE)
    {
      character* BestPartner = 0;
      float BestPartnerDanger = 0;

      for(ushort c = 0; c < game::GetTeams(); ++c)
	if(GetTeam()->GetRelation(game::GetTeam(c)) != HOSTILE)
	  for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	    if((*i)->IsEnabled() && (*i)->IsBunny() && (*i)->GetConfig() == ADULT_FEMALE && (*i)->GetNP() > SATIATED_LEVEL)
	      {
		float Danger = (*i)->GetRelativeDanger(this, true);

		if(Danger > BestPartnerDanger)
		  {
		    BestPartner = *i;
		    BestPartnerDanger = Danger;
		  }
	      }

      if(BestPartner && !GetPos().IsAdjacent(BestPartner->GetPos()))
	{
	  SetGoingTo(BestPartner->GetPos());
	  MoveTowardsTarget();
	  return true;
	}
    }

  if(GetConfig() == ADULT_FEMALE && GetNP() > SATIATED_LEVEL)
    {
      for(ushort d = 0; d < GetNeighbourSquares(); ++d)
	{
	  lsquare* Square = GetNeighbourLSquare(d);

	  if(Square)
	    {
	      character* Father = Square->GetCharacter();

	      if(Father && Father->IsBunny() && Father->GetConfig() == ADULT_MALE && GetRelation(Father) != HOSTILE)
		{
		  if(CanBeSeenByPlayer())
		    {
		      if(Father->IsPlayer())
			ADD_MESSAGE("You have much fun with %s.", CHAR_NAME(DEFINITE));
		      else if(Father->CanBeSeenByPlayer())
			ADD_MESSAGE("%s and %s seem to have much fun together.", Father->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
		      else
			ADD_MESSAGE("%s seems to have much fun.", CHAR_NAME(DEFINITE));
		    }
		  else
		    {
		      if(Father->IsPlayer())
			ADD_MESSAGE("You have much fun with something.");
		      else if(Father->CanBeSeenByPlayer())
			ADD_MESSAGE("%s seems to have much fun.", Father->CHAR_NAME(DEFINITE));
		    }

		  bunny* Baby = new bunny(BABY_MALE + (RAND() & 1));
		  Baby->Strength = RandomizeBabyAttribute(Strength + static_cast<bunny*>(Father)->Strength);
		  Baby->Agility = RandomizeBabyAttribute(Agility + static_cast<bunny*>(Father)->Agility);

		  if(Baby->GetConfig() == BABY_MALE)
		    {
		      Baby->Strength <<= 2;
		      Baby->Agility <<= 2;
		    }
		  else
		    {
		      Baby->Strength <<= 1;
		      Baby->Agility *= 6;
		    }

		  Baby->Strength /= 3;
		  Baby->Agility /= 5;

		  for(ushort c = 0; c < BASE_ATTRIBUTES; ++c)
		    Baby->BaseAttribute[c] = RandomizeBabyAttribute(BaseAttribute[c] + static_cast<bunny*>(Father)->BaseAttribute[c]);

		  Baby->CalculateAll();
		  Baby->RestoreHP();
		  Baby->SetTeam(GetTeam());
		  Baby->PutNear(GetPos());

		  if(Baby->CanBeSeenByPlayer())
		    ADD_MESSAGE("%s is born.", Baby->CHAR_NAME(INDEFINITE));

		  EditNP(-10000);
		  Father->EditAP(-3000);
		  EditAP(-5000);
		  return true;
		}
	    }
	}
    }

  return false;
}

bool bunny::Catches(item* Thingy)
{
  if(Thingy->BunnyWillCatchAndConsume())
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

bool largecreature::PlaceIsIllegal(vector2d Pos, vector2d Illegal) const
{
  for(ushort c = 0; c < 4; ++c)
    if(Pos + game::GetLargeMoveVector(12 + c) == Illegal)
      return true;

  return false;
}

bool mommo::Hit(character* Enemy, vector2d Pos, uchar, bool)
{
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
    return false;

  Hostility(Enemy);

  if(IsPlayer())
    ADD_MESSAGE("You vomit acidous slime at %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
  else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    ADD_MESSAGE("%s vomits acidous slime at %s.", CHAR_DESCRIPTION(DEFINITE), Enemy->CHAR_DESCRIPTION(DEFINITE));

  Vomit(Pos, 500 + RAND() % 500, false);
  EditAP(-1000);
  return true;  
}

void mommo::GetAICommand()
{
  SeekLeader();

  if(CheckForEnemies(false, false))
    return;

  if(!(RAND() % 10))
    {
      Vomit(GetPos(), 500 + RAND() % 500);
      EditAP(-1000);
      return;
    }

  if(FollowLeader())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}
