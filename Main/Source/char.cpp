/* Compiled through charset.cpp */

/*
 * These arrays contain functions and values used for handling states. Remember to update them.
 * All normal states must have PrintBeginMessage and PrintEndMessage functions, a StateDescription string
 * and a StateIsSecret boolean (if true, StateDescription is not shown in the panel without magical means).
 * BeginHandler, EndHandler and StateHandler (called each tick) are optional, enter zero if the state
 * doesn't need one.
 */

struct statedata
{
  void (character::*PrintBeginMessage)() const;
  void (character::*PrintEndMessage)() const;
  void (character::*BeginHandler)();
  void (character::*EndHandler)();
  void (character::*Handler)();
  bool (character::*IsAllowed)() const;
  char* Description;
  ushort Flags;
};

statedata StateData[STATES] =
{
  /* PrintBeginMessage */			    /* PrintEndMessage */			  /* BeginHandler */		  /* EndHandler */		/* Handler */			/* IsAllowed */		      /* Desc */	      /* Flags */
  { 0,						    0,						  0,				  &character::EndPolymorph,	0,				0,			      "Polymorphed",  0 },
  { &character::PrintBeginHasteMessage,		    &character::PrintEndHasteMessage,		  0,				  0,				0,				0,			      "Hasted",	      RANDOMIZABLE },
  { &character::PrintBeginSlowMessage,		    &character::PrintEndSlowMessage,		  0,				  0,				0,				0,			      "Slowed",	      RANDOMIZABLE },
  { &character::PrintBeginPolymorphControlMessage,  &character::PrintEndPolymorphControlMessage,  0,				  0,				0,				0,			      "PolyControl",  RANDOMIZABLE },
  { &character::PrintBeginLifeSaveMessage,	    &character::PrintEndLifeSaveMessage,	  0,				  0,				0,				0,			      "LifeSaved",    SECRET },
  { &character::PrintBeginLycanthropyMessage,	    &character::PrintEndLycanthropyMessage,	  0,				  0,				&character::LycanthropyHandler,	0,			      "Lycanthropy",  SECRET|SRC_FOUNTAIN|SRC_CONFUSE_READ|DUR_TEMPORARY },
  { &character::PrintBeginInvisibilityMessage,	    &character::PrintEndInvisibilityMessage,	  &character::BeginInvisibility,  &character::EndInvisibility,	0,				0,			      "Invisible",    RANDOMIZABLE },
  { &character::PrintBeginInfraVisionMessage,	    &character::PrintEndInfraVisionMessage,	  &character::BeginInfraVision,	  &character::EndInfraVision,	0,				0,			      "Infravision",  RANDOMIZABLE },
  { &character::PrintBeginESPMessage,		    &character::PrintEndESPMessage,		  &character::BeginESP,		  &character::EndESP,		0,				0,			      "ESP",	      RANDOMIZABLE },
  { &character::PrintBeginPoisonedMessage,	    &character::PrintEndPoisonedMessage,	  0,				  0,				&character::PoisonedHandler,	&character::CanBePoisoned,    "Poisoned",     0 },
  { &character::PrintBeginTeleportMessage,	    &character::PrintEndTeleportMessage,	  0,				  0,				&character::TeleportHandler,	0,			      "Teleporting",  SECRET|RANDOMIZABLE },
  { &character::PrintBeginPolymorphMessage,	    &character::PrintEndPolymorphMessage,	  0,				  0,				&character::PolymorphHandler,	0,			      "Polymorphing", SECRET|RANDOMIZABLE },
  { &character::PrintBeginTeleportControlMessage,   &character::PrintEndTeleportControlMessage,	  0,				  0,				0,				0,			      "TeleControl",  RANDOMIZABLE },
  { &character::PrintBeginPanicMessage,		    &character::PrintEndPanicMessage,		  0,				  0,				0,				0,			      "Panicked",     0 },
  { &character::PrintBeginConfuseMessage,	    &character::PrintEndConfuseMessage,		  0,				  0,				0,				&character::CanBeConfused,    "Confused",     RANDOMIZABLE&~DUR_PERMANENT },
  { &character::PrintBeginParasitizedMessage,	    &character::PrintEndParasitizedMessage,	  0,				  0,				&character::ParasitizedHandler,	&character::CanBeParasitized, "Parasitized",  RANDOMIZABLE&~DUR_TEMPORARY },
  { &character::PrintBeginSearchingMessage,	    &character::PrintEndSearchingMessage,	  0,				  0,				&character::SearchingHandler,	0,			      "Searching",    0 },
  { &character::PrintBeginGasImmunityMessage,	    &character::PrintEndGasImmunityMessage,	  0,				  0,				0,				0,			      "GasImmunity",  SECRET|RANDOMIZABLE }
};

characterprototype::characterprototype(characterprototype* Base, character* (*Cloner)(ushort, ushort), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId) { Index = protocontainer<character>::Add(this); }
const characterdatabase& characterprototype::ChooseBaseForConfig(ushort) { return Config.begin()->second; }

/*
 * Plain database without preceding colons would mean
 * characterdatabase and cause a syntax error. Sorry for bad naming.
 */

void character::InstallDataBase() { ::database<character>::InstallDataBase(this); }
std::list<character*>::iterator character::GetTeamIterator() { return TeamIterator; }
void character::SetTeamIterator(std::list<character*>::iterator What) { TeamIterator = What; }
void character::CreateInitialEquipment(ushort SpecialFlags) { AddToInventory(DataBase->Inventory, SpecialFlags); }
void character::EditAP(long What) { AP = Limit<long>(AP + What, -12000, 1200); }
bool character::CanUseEquipment(ushort Index) const { return CanUseEquipment() && Index < GetEquipmentSlots() && GetBodyPartOfEquipment(Index); }
ushort character::GetAttribute(ushort Identifier) const { return Max(BaseAttribute[Identifier] + AttributeBonus[Identifier], 1); }
ushort character::GetRandomStepperBodyPart() const { return TORSO_INDEX; }
void character::GainIntrinsic(ulong What) { BeginTemporaryState(What, PERMANENT); }
bool character::IsUsingArms() const { return (GetAttackStyle() & USE_ARMS) != 0; }
bool character::IsUsingLegs() const { return (GetAttackStyle() & USE_LEGS) != 0; }
bool character::IsUsingHead() const { return (GetAttackStyle() & USE_HEAD) != 0; }
void character::CalculateAllowedWeaponSkillCategories() { AllowedWeaponSkillCategories = MARTIAL_SKILL_CATEGORIES; }
std::string character::GetBeVerb() const { return IsPlayer() ? "are" : "is"; }
void character::SetEndurance(ushort What) { BaseAttribute[ENDURANCE] = What; }
void character::SetPerception(ushort What) { BaseAttribute[PERCEPTION] = What; }
void character::SetIntelligence(ushort What) { BaseAttribute[INTELLIGENCE] = What; }
void character::SetWisdom(ushort What) { BaseAttribute[WISDOM] = What; }
void character::SetCharisma(ushort What) { BaseAttribute[CHARISMA] = What; }
void character::SetMana(ushort What) { BaseAttribute[MANA] = What; }
bool character::IsOnGround() const { return MotherEntity && MotherEntity->IsOnGround(); }
bool character::LeftOversAreUnique() const { return GetArticleMode() != NORMAL_ARTICLE || AssignedName.length(); }
bool character::HomeDataIsValid() const { return HomeData && HomeData->Level == GetLSquareUnder()->GetLevelIndex() && HomeData->Dungeon == GetLSquareUnder()->GetDungeonIndex(); }
void character::SetHomePos(vector2d Pos) { HomeData->Pos = Pos; }
std::string character::FirstPersonUnarmedHitVerb() const { return "hit"; }
std::string character::FirstPersonCriticalUnarmedHitVerb() const { return "critically hit"; }
std::string character::ThirdPersonUnarmedHitVerb() const { return "hits"; }
std::string character::ThirdPersonCriticalUnarmedHitVerb() const { return "critically hits"; }
std::string character::FirstPersonKickVerb() const { return "kick"; }
std::string character::FirstPersonCriticalKickVerb() const { return "critically kick"; }
std::string character::ThirdPersonKickVerb() const { return "kicks"; }
std::string character::ThirdPersonCriticalKickVerb() const { return "critically kicks"; }
std::string character::FirstPersonBiteVerb() const { return "bite"; }
std::string character::FirstPersonCriticalBiteVerb() const { return "critically bite"; }
std::string character::ThirdPersonBiteVerb() const { return "bites"; }
std::string character::ThirdPersonCriticalBiteVerb() const { return "critically bites"; }
std::string character::UnarmedHitNoun() const { return "attack"; }
std::string character::KickNoun() const { return "kick"; }
std::string character::BiteNoun() const { return "attack"; }
uchar character::GetSpecialBodyPartFlags(ushort, bool) const { return ST_NORMAL; }
std::string character::GetEquipmentName(ushort) const { return ""; }
const std::list<ulong>& character::GetOriginalBodyPartID(ushort Index) const { return OriginalBodyPartID[Index]; }
wsquare* character::GetNeighbourWSquare(ushort Index) const { return static_cast<wsquare*>(GetSquareUnder())->GetNeighbourWSquare(Index); }
wsquare* character::GetWSquareUnder() const { return static_cast<wsquare*>(GetSquareUnder()); }
god* character::GetMasterGod() const { return game::GetGod(GetConfig()); }
wsquare* character::GetNearWSquare(vector2d Pos) const { return static_cast<wsquare*>(GetSquareUnder()->GetArea()->GetSquare(Pos)); }
wsquare* character::GetNearWSquare(ushort x, ushort y) const { return static_cast<wsquare*>(GetSquareUnder()->GetArea()->GetSquare(x, y)); }

character::character(const character& Char) : entity(Char), id(Char), NP(Char.NP), AP(Char.AP), Player(false), TemporaryState(Char.TemporaryState&~POLYMORPHED), Team(Char.Team), WayPoint(-1, -1), Money(0), AssignedName(Char.AssignedName), Action(0), Config(Char.Config), DataBase(Char.DataBase), StuckToBodyPart(NONE_INDEX), StuckTo(0), MotherEntity(0), PolymorphBackup(0), EquipmentState(0), SquareUnder(0), Initializing(true), AllowedWeaponSkillCategories(Char.AllowedWeaponSkillCategories), BodyParts(Char.BodyParts), Polymorphed(false), InNoMsgMode(true), RegenerationCounter(Char.RegenerationCounter)
{
  Stack = new stack(0, this, HIDDEN, true);

  ushort c;

  for(c = 0; c < STATES; ++c)
    TemporaryStateCounter[c] = Char.TemporaryStateCounter[c];

  if(Team)
    TeamIterator = Team->Add(this);

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    {
      BaseAttribute[c] = Char.BaseAttribute[c];
      BaseExperience[c] = Char.BaseExperience[c];
    }

  BodyPartSlot = new characterslot[BodyParts];
  OriginalBodyPartID = new std::list<ulong>[BodyParts];
  CWeaponSkill = new cweaponskill*[AllowedWeaponSkillCategories];

  for(c = 0; c < BodyParts; ++c)
    {
      BodyPartSlot[c].SetMaster(this);

      if(Char.GetBodyPart(c))
	SetBodyPart(c, static_cast<bodypart*>(Char.GetBodyPart(c)->Duplicate()));
      else
	OriginalBodyPartID[c] = Char.OriginalBodyPartID[c];
    }

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    CWeaponSkill[c] = new cweaponskill(*Char.CWeaponSkill[c]);

  HomeData = Char.HomeData ? new homedata(*Char.HomeData) : 0;
  ID = game::CreateNewCharacterID(this);
  Initializing = InNoMsgMode = false;
}

character::character(donothing) : entity(HAS_BE), NP(50000), AP(0), Player(false), TemporaryState(0), Team(0), WayPoint(-1, -1), Money(0), Action(0), StuckToBodyPart(NONE_INDEX), StuckTo(0), MotherEntity(0), PolymorphBackup(0), EquipmentState(0), SquareUnder(0), Polymorphed(false), RegenerationCounter(0), HomeData(0)
{
  Stack = new stack(0, this, HIDDEN, true);
}

character::~character()
{
  if(Action)
    {
      Action->DeleteUsedItems();
      delete Action;
    }

  if(Team)
    Team->Remove(GetTeamIterator());

  delete Stack;
  ushort c;

  for(c = 0; c < BodyParts; ++c)
    delete GetBodyPart(c);

  delete [] BodyPartSlot;
  delete [] OriginalBodyPartID;
  delete PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    delete CWeaponSkill[c];

  delete [] CWeaponSkill;
  delete HomeData;
  game::RemoveCharacterID(ID);
}

void character::Hunger() 
{
  uchar OldState = GetHungerState();

  switch(GetBurdenState())
    {
    case UNBURDENED:
      EditNP(-1);
      break;
    case BURDENED:
      EditNP(-2);
      EditExperience(LEG_STRENGTH, 1);
      EditExperience(AGILITY, -1);
      break;
    case STRESSED:
    case OVER_LOADED:
      EditNP(-4);
      EditExperience(LEG_STRENGTH, 2);
      EditExperience(AGILITY, -2);
      break;
    }

  if(OldState != STARVING && GetHungerState() == STARVING)
    DeActivateVoluntaryAction("You are getting extremely hungry.");

  if(GetHungerState() == HUNGRY || GetHungerState() == STARVING)
    {
      EditExperience(ARM_STRENGTH, -1);
      EditExperience(LEG_STRENGTH, -1);
    }

  if(GetHungerState() >= BLOATED)
    EditExperience(AGILITY, -1);

  CheckStarvationDeath("starved to death");
}

ushort character::TakeHit(character* Enemy, item* Weapon, float Damage, float ToHitValue, short Success, uchar Type, bool Critical, bool ForceHit)
{
  uchar Dir = Type == BITE_ATTACK ? YOURSELF : game::GetDirectionForVector(GetPos() - Enemy->GetPos());
  float DodgeValue = GetDodgeValue();

  if(!Enemy->IsPlayer() && GetAttackWisdomLimit() != NO_LIMIT)
    Enemy->EditExperience(WISDOM, 200);

  if(!Enemy->CanBeSeenBy(this))
    ToHitValue *= 2;

  if(!CanBeSeenBy(Enemy))
    DodgeValue *= 2;

  WayPoint = Enemy->GetPos();
  Enemy->WayPoint = GetPos();

  /* Effectively, the average chance to hit is 100% / (DV/THV + 1). */

  if(RAND() % ushort(100 + ToHitValue / DodgeValue * (100 + Success)) < 100 && !Critical && !ForceHit)
    {
      Enemy->AddMissMessage(this);
      EditExperience(AGILITY, 50);
      EditExperience(PERCEPTION, 25);

      if(Enemy->CanBeSeenByPlayer())
	DeActivateVoluntaryAction("The attack of " + Enemy->GetName(DEFINITE) + " interupts you.");
      else
	DeActivateVoluntaryAction("The attack interupts you.");

      return HAS_DODGED;
    }

  ushort TrueDamage = ushort(Damage * (100 + Success) / 100) + (RAND() % 3 ? 1 : 0);

  if(Critical)
    {
      TrueDamage <<= 1;
      ++TrueDamage;
    }

  ushort BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, DodgeValue);

  if(Critical)
    {
      switch(Type)
	{
	case UNARMED_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonCriticalUnarmedHitVerb(), Enemy->ThirdPersonCriticalUnarmedHitVerb(), BodyPart);
	  break;
	case WEAPON_ATTACK:
          Enemy->AddWeaponHitMessage(this, Weapon, BodyPart, true);
	  break;
	case KICK_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonCriticalKickVerb(), Enemy->ThirdPersonCriticalKickVerb(), BodyPart);
	  break;
	case BITE_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonCriticalBiteVerb(), Enemy->ThirdPersonCriticalBiteVerb(), BodyPart);
	  break;
	}
    }
  else
    {
      switch(Type)
	{
	case UNARMED_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonUnarmedHitVerb(), Enemy->ThirdPersonUnarmedHitVerb(), BodyPart);
	  break;
	case WEAPON_ATTACK:
	  Enemy->AddWeaponHitMessage(this, Weapon, BodyPart, false);
	  break;
	case KICK_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonKickVerb(), Enemy->ThirdPersonKickVerb(), BodyPart);
	  break;
	case BITE_ATTACK:
	  Enemy->AddPrimitiveHitMessage(this, Enemy->FirstPersonBiteVerb(), Enemy->ThirdPersonBiteVerb(), BodyPart);
	  break;
	}
    }

  if(!Critical && TrueDamage && Enemy->AttackIsBlockable(Type))
    {
      TrueDamage = CheckForBlock(Enemy, Weapon, ToHitValue, TrueDamage, Success, Type);

      if(!TrueDamage || (Weapon && !Weapon->Exists()))
	{
	  if(Enemy->CanBeSeenByPlayer())
	    DeActivateVoluntaryAction("The attack of " + Enemy->GetName(DEFINITE) + " interupts you.");
	  else
	    DeActivateVoluntaryAction("The attack interupts you.");

	  return HAS_BLOCKED;
	}
    }

  ushort DoneDamage = ReceiveBodyPartDamage(Enemy, TrueDamage, PHYSICAL_DAMAGE, BodyPart, Dir, false, Critical);
  bool Succeeded = (GetBodyPart(BodyPart) && HitEffect(Enemy, Weapon, Type, BodyPart, Dir, !DoneDamage)) || DoneDamage;

  if(Succeeded)
    Enemy->WeaponSkillHit(Weapon, Type);

  if(Weapon && DoneDamage < TrueDamage)
    Weapon->ReceiveDamage(Enemy, TrueDamage - DoneDamage, PHYSICAL_DAMAGE);

  if(!Succeeded)
    {
      if(Enemy->CanBeSeenByPlayer())
	DeActivateVoluntaryAction("The attack of " + Enemy->GetName(DEFINITE) + " interupts you.");
      else
	DeActivateVoluntaryAction("The attack interupts you.");

      return DID_NO_DAMAGE;
    }

  std::string DeathMsg = "killed by " + Enemy->GetKillName();

  if(CheckDeath(DeathMsg, Enemy, Enemy->IsPlayer()))
    return HAS_DIED;

  if(Enemy->CanBeSeenByPlayer())
    DeActivateVoluntaryAction("The attack of " + Enemy->GetName(DEFINITE) + " interupts you.");
  else
    DeActivateVoluntaryAction("The attack interupts you.");

  return HAS_HIT;
}

struct svpriorityelement
{
  svpriorityelement(ushort BodyPart, ushort StrengthValue) : BodyPart(BodyPart), StrengthValue(StrengthValue) { }
  bool operator<(const svpriorityelement& AnotherPair) const { return StrengthValue > AnotherPair.StrengthValue; }
  ushort BodyPart;
  ushort StrengthValue;
};

ushort character::ChooseBodyPartToReceiveHit(float ToHitValue, float DodgeValue)
{
  if(GetBodyParts() == 1)
    return 0;

  std::priority_queue<svpriorityelement> SVQueue;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && !BodyPart->CannotBeSevered(PHYSICAL_DAMAGE))
	SVQueue.push(svpriorityelement(c, BodyPart->GetStrengthValue() + BodyPart->GetHP()));
    }

  while(SVQueue.size())
    {
      ushort ToHitPercentage = ushort(GLOBAL_WEAK_BODYPART_HIT_MODIFIER * ToHitValue * GetBodyPart(SVQueue.top().BodyPart)->GetBodyPartVolume() / (DodgeValue * GetBodyVolume()));

      if(ToHitPercentage < 1)
	ToHitPercentage = 1;
      else if(ToHitPercentage > 95)
	ToHitPercentage = 95;

      if(ToHitPercentage > RAND() % 100)
	return SVQueue.top().BodyPart;

      SVQueue.pop();
    }

  return 0;
}

void character::Be()
{
  if(game::IsLoading())
    {
      if(!IsPlayer())
	return;
      else
	game::SetIsLoading(false);
    }
  else
    {
      HandleStates();

      if(!IsEnabled())
	return;

      if(GetTeam() == PLAYER->GetTeam())
	{
	  for(ushort c = 0; c < AllowedWeaponSkillCategories; ++c)
	    if(CWeaponSkill[c]->Tick() && IsPlayer())
	      CWeaponSkill[c]->AddLevelDownMessage();

	  SWeaponSkillTick();
	}

      if(IsPlayer())
	{
	  if(GetHungerState() == STARVING && !(RAND() % 50))
	    Faint(500 + RAND() % 500, true);

	  if(!Action || Action->AllowFoodConsumption())
	    Hunger();
	}

      if(HP != MaxHP)
	Regenerate();

      if(Action && AP >= 1000)
	ActionAutoTermination();

      if(Action && AP >= 1000)
	{
	  Action->Handle();

	  if(!IsEnabled())
	    return;
	}
      else
	EditAP(GetStateAPGain(100));
    }

  if(AP >= 1000)
    {
      ApplyExperience();
      SpecialTurnHandler();

      if(IsPlayer())
	{
	  static ushort Timer = 0;

	  if(configuration::GetAutoSaveInterval() && !GetAction() && ++Timer >= configuration::GetAutoSaveInterval())
	    {
	      game::Save(game::GetAutoSaveFileName());
	      Timer = 0;
	    }

	  game::CalculateNextDanger();

	  if(!StateIsActivated(POLYMORPHED))
	    game::UpdatePlayerAttributeAverage();

	  if(!game::IsInWilderness())
	    Search(GetAttribute(PERCEPTION));

	  if(!Action)
	    GetPlayerCommand();
	  else
	    {
	      if(Action->ShowEnvironment())
		{
		  static ushort Counter = 0;

		  if(++Counter == 10)
		    {
		      game::DrawEverything();
		      Counter = 0;
		    }
		}

	      if(Action->IsVoluntary() && READ_KEY())
		Action->Terminate(false);
	    }
	}
      else
	{
	  if(!Action && !game::IsInWilderness())
	    GetAICommand();
	}
    }
}

void character::Move(vector2d MoveTo, bool TeleportMove)
{
  /* Test whether the player is stuck to something */

  if(!TeleportMove && IsStuck() && !TryToUnstuck(MoveTo - GetPos()))
    return;

  if(GetBurdenState() != OVER_LOADED || TeleportMove)
    {
      GetArea()->MoveCharacter(GetPos(), MoveTo);

      if(IsPlayer())
	ShowNewPosInfo();

      if(!TeleportMove)
	{
	  EditAP(-GetMoveAPRequirement(GetSquareUnder()->GetEntryDifficulty()));
	  EditNP(-10 * GetSquareUnder()->GetEntryDifficulty());
	  EditExperience(AGILITY, 5 * GetSquareUnder()->GetEntryDifficulty());
	}
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("You try very hard to crawl forward. But your load is too heavy.");

      EditAP(-1000);
    }
}

void character::GetAICommand()
{
  SeekLeader();

  if(FollowLeader())
    return;

  if(CheckForEnemies(true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(CheckForDoors())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

/* TPos = Target Pos */

bool character::MoveTowards(vector2d TPos)
{
  vector2d MoveTo[3];

  if(TPos.X < GetPos().X)
    {
      if(TPos.Y < GetPos().Y)
	{
	  MoveTo[0] = vector2d(-1, -1);
	  MoveTo[1] = vector2d(-1,  0);
	  MoveTo[2] = vector2d( 0, -1);
	}

      if(TPos.Y == GetPos().Y)
	{
	  MoveTo[0] = vector2d(-1,  0);
	  MoveTo[1] = vector2d(-1, -1);
	  MoveTo[2] = vector2d(-1,  1);
	}

      if(TPos.Y > GetPos().Y)
	{
	  MoveTo[0] = vector2d(-1, 1);
	  MoveTo[1] = vector2d(-1, 0);
	  MoveTo[2] = vector2d( 0, 1);
	}
    }

  if(TPos.X == GetPos().X)
    {
      if(TPos.Y < GetPos().Y)
	{
	  MoveTo[0] = vector2d( 0, -1);
	  MoveTo[1] = vector2d(-1, -1);
	  MoveTo[2] = vector2d( 1, -1);
	}

      if(TPos.Y == GetPos().Y)
	return false;

      if(TPos.Y > GetPos().Y)
	{
	  MoveTo[0] = vector2d( 0, 1);
	  MoveTo[1] = vector2d(-1, 1);
	  MoveTo[2] = vector2d( 1, 1);
	}
    }

  if(TPos.X > GetPos().X)
    {
      if(TPos.Y < GetPos().Y)
	{
	  MoveTo[0] = vector2d(1, -1);
	  MoveTo[1] = vector2d(1,  0);
	  MoveTo[2] = vector2d(0, -1);
	}

      if(TPos.Y == GetPos().Y)
	{
	  MoveTo[0] = vector2d(1,  0);
	  MoveTo[1] = vector2d(1, -1);
	  MoveTo[2] = vector2d(1,  1);
	}

      if(TPos.Y > GetPos().Y)
	{
	  MoveTo[0] = vector2d(1, 1);
	  MoveTo[1] = vector2d(1, 0);
	  MoveTo[2] = vector2d(0, 1);
	}
    }

  if(TryMove(GetPos() + ApplyStateModification(MoveTo[0]))) return true;

  if(GetPos().IsAdjacent(TPos))
    return false;

  if(RAND() & 1)
    {
      if(TryMove(GetPos() + ApplyStateModification(MoveTo[1]))) return true;
      if(TryMove(GetPos() + ApplyStateModification(MoveTo[2]))) return true;
    }
  else
    {
      if(TryMove(GetPos() + ApplyStateModification(MoveTo[2]))) return true;
      if(TryMove(GetPos() + ApplyStateModification(MoveTo[1]))) return true;
    }

  return false;
}

bool character::TryMove(vector2d MoveTo, bool DisplaceAllowed)
{
  if(!game::IsInWilderness())
    {
      if(GetLevel()->IsValidPos(MoveTo))
	{
	  lsquare* MoveToSquare = GetNearLSquare(MoveTo);
	  character* Character = MoveToSquare->GetCharacter();

	  if(Character)
	    {
	      if(IsPlayer())
		{
		  if(GetTeam() != Character->GetTeam())
		    return Hit(Character);
		  else
		    if(DisplaceAllowed && (MoveToSquare->IsWalkable(this) || game::GoThroughWallsCheatIsActive()))
		      {
			if(!Displace(Character))
			  EditAP(-1000);

			return true;
		      }
		    else
		      return false;
		}
	      else
		{
		  if(GetRelation(Character) == HOSTILE)
		    return Hit(Character);
		  else
		    if(GetTeam() == Character->GetTeam() && DisplaceAllowed && MoveToSquare->IsWalkable(this))
		      return Displace(Character);
		    else
		      return HandleCharacterBlockingTheWay(Character);
		}
	    }
	  else
	    {
	      if(MoveToSquare->IsWalkable(this) || (game::GoThroughWallsCheatIsActive() && IsPlayer()))
		{
		  Move(MoveTo);
		  return true;
		}
	      else
		{
		  olterrain* Terrain = MoveToSquare->GetOLTerrain();

		  if(IsPlayer() && Terrain && Terrain->CanBeOpened())
		    {
		      if(CanOpen())
			{
			  if(Terrain->IsLocked())
			    {
			      ADD_MESSAGE("The %s is locked.", Terrain->GetNameSingular().c_str()); /* not sure if this is better than "the door is locked", but I guess it _might_ be slighltly better */
			      return false;
			    }
			  else
			    {
			      if(game::BoolQuestion("Do you want to open " + Terrain->GetName(UNARTICLED) + "? [y/N]", false, game::GetMoveCommandKeyBetweenPoints(PLAYER->GetPos(), MoveTo)))
				{
				  OpenPos(MoveTo);
				  return true;
				}
			      else
				return false;
			    }
			}
		      else
			{
			  ADD_MESSAGE("This monster type cannot open doors.");
			  return false;
			}
		    }
		  else
		    return false;
		}
	    }
	}
      else
	{
	  if(IsPlayer() && !IsStuck() && GetLevel()->IsOnGround() && game::BoolQuestion("Do you want to leave " + game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrentLevelIndex()) + "? [y/N]"))
	    {
	      if(HasPetrussNut() && !HasGoldenEagleShirt())
		{
		  game::TextScreen("An undead and sinister voice greets you as you leave the city behind:\n\n\"MoRtAl! ThOu HaSt SlAuGtHeReD pEtRuS aNd PlEaSeD mE!\nfRoM tHiS dAy On, ThOu ArT tHe DeArEsT sErVaNt Of AlL eViL!\"\n\nYou are victorious!");
		  game::GetCurrentArea()->SendNewDrawRequest();
		  game::DrawEverything();
		  ShowAdventureInfo();
		  PLAYER->AddScoreEntry("killed Petrus and became the Avatar of Chaos", 3, false);
		  game::End();
		  return true;
		}

	      if(game::TryTravel(WORLD_MAP, WORLD_MAP, game::GetCurrentDungeonIndex()))
		return true;
	    }

	  return false;
	}
    }
  else
    {
      if(GetArea()->IsValidPos(MoveTo) && (GetNearWSquare(MoveTo)->IsWalkable(this) || game::GoThroughWallsCheatIsActive()))
	{
	  if(!game::GoThroughWallsCheatIsActive())
	    for(ushort c = 0; c < game::GetWorldMap()->GetPlayerGroup().size(); ++c)
	      if(!GetNearWSquare(MoveTo)->IsWalkable(game::GetWorldMap()->GetPlayerGroup()[c]))
		{
		  ADD_MESSAGE("One or more of your team members cannot cross this terrain.");
		  return false;
		}

	  Move(MoveTo);
	  return true;
	}
      else
	return false;
    }
}

void character::CreateCorpse(lsquare* Square)
{
  if(!BodyPartsDisappearWhenSevered())
    {
      corpse* Corpse = new corpse(0, NO_MATERIALS);
      Corpse->SetDeceased(this);
      Square->AddItem(Corpse);
      Disable();
    }
  else
    SendToHell();
}

void character::Die(const character* Killer, const std::string& Msg, bool ForceMsg)
{
  // Note: This function MUST NOT delete any objects in any case! 

  if(!IsEnabled())
    return;

  if(StuckToBodyPart != NONE_INDEX)
    {
      StuckToBodyPart = NONE_INDEX;
      StuckTo = 0;
    }

  if(IsPlayer())
    {
      ADD_MESSAGE("You die.");

      if(game::WizardModeIsActive())
	{
	  game::DrawEverything();

	  if(!game::BoolQuestion("Do you want to do this, cheater? [y/n]", REQUIRES_ANSWER))
	    {
	      RestoreBodyParts();
	      RestoreHP();
	      ResetStates();
	      SetNP(SATIATED_LEVEL);
	      GetSquareUnder()->SendNewDrawRequest();
	      return;
	    }
	}
    }
  else if(CanBeSeenByPlayer())
    ProcessAndAddMessage(GetDeathMessage());
  else if(ForceMsg)
    ADD_MESSAGE("You sense the death of something.");

  if(StateIsActivated(LIFE_SAVED))
    {
      SaveLife();
      return;
    }

  InNoMsgMode = true;

  if(IsPlayer())
    game::RemoveSaves();

  GetSquareUnder()->RemoveCharacter();

  if(!game::IsInWilderness())
    {
      if(!StateIsActivated(POLYMORPHED))
	{
	  if(!IsPlayer())
	    game::SignalDeath(this, Killer);

	  CreateCorpse(GetLSquareUnder());
	}
      else
	{
	  if(!IsPlayer())
	    game::SignalDeath(GetPolymorphBackup(), Killer);

	  GetPolymorphBackup()->CreateCorpse(GetLSquareUnder());
	  GetPolymorphBackup()->SetPolymorphed(false);
	  SetPolymorphBackup(0);
	  SendToHell();
	}
    }

  if(IsPlayer())
    {
      if(!game::IsInWilderness())
	GetLSquareUnder()->SetTemporaryEmitation(GetEmitation());

      ShowAdventureInfo();

      if(!game::IsInWilderness())
	GetLSquareUnder()->SetTemporaryEmitation(0);
    }

  if(!game::IsInWilderness())
    {
      GetStack()->MoveItemsTo(GetLSquareUnder()->GetStack());

      for(ushort c = 0; c < GetBodyParts(); ++c)
	if(GetBodyPart(c))
	  GetBodyPart(c)->DropEquipment();

      if(GetAction())
	{
	  GetAction()->DropUsedItems();
	  delete GetAction();
	  SetAction(0);
	}
    }
  else
    {
      /* Drops the equipment to the character's stack */

      for(ushort c = 0; c < GetBodyParts(); ++c)
	if(GetBodyPart(c))
	  GetBodyPart(c)->DropEquipment();

      if(GetAction())
	{
	  GetAction()->DeleteUsedItems();
	  delete GetAction();
	  SetAction(0);
	}

      while(GetStack()->GetItems())
	{
	  GetStack()->GetBottom()->SendToHell();
	  GetStack()->GetBottom()->RemoveFromSlot();
	}
    }

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(0);

  InNoMsgMode = false;

  if(IsPlayer())
    {
      AddScoreEntry(Msg);
      game::TextScreen("Unfortunately you died during your journey. The high priest is not happy.");
      game::End();
    }
}

void character::AddMissMessage(const character* Enemy) const
{
  std::string Msg;

  if(Enemy->IsPlayer())
    Msg = GetDescription(DEFINITE) + " misses you!";
  else if(IsPlayer())
    Msg = "You miss " + Enemy->GetDescription(DEFINITE) + "!";
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg = GetDescription(DEFINITE) + " misses " + Enemy->GetDescription(DEFINITE) + "!";
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::AddBlockMessage(const character* Enemy, const item* Blocker, const std::string& HitNoun, bool Partial) const
{
  std::string Msg;
  std::string BlockVerb = (Partial ? " to partially block the " : " to block the ") + HitNoun;

  if(IsPlayer())
    Msg << "You manage" << BlockVerb << " with your " << Blocker->GetName(UNARTICLED) << "!";
  else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg << GetDescription(DEFINITE) << " manages" << BlockVerb << " with " << GetPossessivePronoun() << " " << Blocker->GetName(UNARTICLED) << "!";
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::AddPrimitiveHitMessage(const character* Enemy, const std::string& FirstPersonHitVerb, const std::string& ThirdPersonHitVerb, ushort BodyPart) const
{
  std::string Msg;
  std::string BodyPartDescription = BodyPart && (Enemy->CanBeSeenByPlayer() || Enemy->IsPlayer()) ? " in the " + Enemy->GetBodyPartName(BodyPart) : "";

  if(Enemy->IsPlayer())
    Msg << GetDescription(DEFINITE) << " " << ThirdPersonHitVerb << " you" << BodyPartDescription << "!";
  else if(IsPlayer())
    Msg << "You " << FirstPersonHitVerb << " " << Enemy->GetDescription(DEFINITE) << BodyPartDescription << "!";
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg << GetDescription(DEFINITE) << " " << ThirdPersonHitVerb << " " << Enemy->GetDescription(DEFINITE) + BodyPartDescription << "!";
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::AddWeaponHitMessage(const character* Enemy, const item* Weapon, ushort BodyPart, bool Critical) const
{
  std::string Msg;
  std::string BodyPartDescription = BodyPart && (Enemy->CanBeSeenByPlayer() || Enemy->IsPlayer()) ? " in the " + Enemy->GetBodyPartName(BodyPart) : "";

  if(Enemy->IsPlayer())
    {
      Msg << GetDescription(DEFINITE) << (Critical ? " critically hits you" : " hits you") << BodyPartDescription;

      if(CanBeSeenByPlayer())
	Msg << " with " << GetPossessivePronoun() << " " << Weapon->GetName(UNARTICLED);

      Msg << "!";
    }
  else if(IsPlayer())
    Msg << "You " << (Critical ? "critically hit " : "hit ") << Enemy->GetDescription(DEFINITE) << BodyPartDescription << "!";
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    {
      Msg << GetDescription(DEFINITE) << (Critical ? " critically hits " : " hits ") << Enemy->GetDescription(DEFINITE) << BodyPartDescription;

      if(CanBeSeenByPlayer())
	Msg << " with " << GetPossessivePronoun() << " " << Weapon->GetName(UNARTICLED);

      Msg << "!";
    }
  else
    return;

  ADD_MESSAGE("%s", Msg.c_str());
}

void character::ApplyExperience(bool Edited)
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->ApplyExperience())
      Edited = true;

  if(IsAlive())
    if(CheckForAttributeIncrease(BaseAttribute[ENDURANCE], BaseExperience[ENDURANCE]))
      {
	if(IsPlayer())
	  ADD_MESSAGE("You feel tougher than anything!");
	else if(CanBeSeenByPlayer())
	  ADD_MESSAGE("Suddenly %s looks tougher.", CHAR_NAME(DEFINITE));

	CalculateBodyPartMaxHPs();
	Edited = true;
      }
    else if(CheckForAttributeDecrease(BaseAttribute[ENDURANCE], BaseExperience[ENDURANCE]))
      {
	if(IsPlayer())
	  ADD_MESSAGE("You feel less healthy.");
	else if(CanBeSeenByPlayer())
	  ADD_MESSAGE("Suddenly %s looks less healthy.", CHAR_NAME(DEFINITE));

	CalculateBodyPartMaxHPs();
	Edited = true;
      }

  if(CheckForAttributeIncrease(BaseAttribute[PERCEPTION], BaseExperience[PERCEPTION]))
    {
      if(IsPlayer())
	{
	  ADD_MESSAGE("You now see the world in much better detail than before.");
	  game::SendLOSUpdateRequest();
	}

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[PERCEPTION], BaseExperience[PERCEPTION]))
    {
      if(IsPlayer())
	{
	  ADD_MESSAGE("You feel very guru.");
	  game::GetGod(1)->AdjustRelation(100);
	  game::SendLOSUpdateRequest();
	}

      Edited = true;
    }

  if(CheckForAttributeIncrease(BaseAttribute[INTELLIGENCE], BaseExperience[INTELLIGENCE]))
    {
      if(IsPlayer())
	ADD_MESSAGE("Suddenly the inner structure of the Multiverse around you looks quite simple.");

      if(IsPlayer())
	UpdateESPLOS();

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[INTELLIGENCE], BaseExperience[INTELLIGENCE]))
    {
      if(IsPlayer())
	ADD_MESSAGE("It surely is hard to think today.");

      if(IsPlayer())
	UpdateESPLOS();

      Edited = true;
    }

  if(CheckForAttributeIncrease(BaseAttribute[WISDOM], BaseExperience[WISDOM]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel your life experience increasing all the time.");

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[WISDOM], BaseExperience[WISDOM]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel like having done something unwise.");

      Edited = true;
    }

  if(CheckForAttributeIncrease(BaseAttribute[CHARISMA], BaseExperience[CHARISMA]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel very confident of your social skills.");
      else if(CanBeSeenByPlayer())
	{
	  if(GetAttribute(CHARISMA) <= 15)
	    ADD_MESSAGE("%s looks less ugly.", CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s looks more attractive.", CHAR_NAME(DEFINITE));
	}

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[CHARISMA], BaseExperience[CHARISMA]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel somehow disliked.");
      else if(CanBeSeenByPlayer())
	{
	  if(GetAttribute(CHARISMA) < 15)
	    ADD_MESSAGE("%s looks more ugly.", CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("%s looks less attractive.", CHAR_NAME(DEFINITE));
	}

      Edited = true;
    }

  if(CheckForAttributeIncrease(BaseAttribute[MANA], BaseExperience[MANA]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel magical forces coursing through your body!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("You notice an odd glow around %s.", CHAR_NAME(DEFINITE));

      Edited = true;
    }
  else if(CheckForAttributeDecrease(BaseAttribute[MANA], BaseExperience[MANA]))
    {
      if(IsPlayer())
	ADD_MESSAGE("You feel your magical abilities withering slowly.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("You notice strange vibrations in the air around %s. But they disappear rapidly.", CHAR_NAME(DEFINITE));

      Edited = true;
    }

  if(Edited)
    CalculateBattleInfo();
}

bool character::HasHeadOfElpuri() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsHeadOfElpuri())
      return true;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsHeadOfElpuri())
      return true;

  return false;
}

bool character::HasPetrussNut() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsPetrussNut())
      return true;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsPetrussNut())
      return true;

  return false;
}

bool character::HasGoldenEagleShirt() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsGoldenEagleShirt())
      return true;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsGoldenEagleShirt())
      return true;

  return false;
}

bool character::RemoveEncryptedScroll()
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsEncryptedScroll())
      {
	item* Item = *i;
	Item->RemoveFromSlot();
	Item->SendToHell();
	return true;
      }

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Item = GetEquipment(c);

      if(Item && Item->IsEncryptedScroll())
	{
	  Item->RemoveFromSlot();
	  Item->SendToHell();
	  return true;
	}
    }

  return false;
}

bool character::ReadItem(item* ToBeRead)
{
  if(ToBeRead->CanBeRead(this))
    {
      if(!GetLSquareUnder()->IsDark() || game::SeeWholeMapCheatIsActive())
	{
	  if(StateIsActivated(CONFUSED))
	    {
	      if(!ToBeRead->IsDestroyable())
		ADD_MESSAGE("You read some words of %s and understand exactly nothing.", ToBeRead->CHAR_NAME(DEFINITE));
	      else
		{
		  ADD_MESSAGE("%s is very confusing. Or perhaps you are just too confused?", ToBeRead->CHAR_NAME(DEFINITE));
		  ActivateRandomState(SRC_CONFUSE_READ, 1000 + RAND() % 1500);
		  ToBeRead->RemoveFromSlot();
		  ToBeRead->SendToHell();
		}

	      EditAP(-1000);
	      return true;
	    }

	  if(ToBeRead->Read(this))
	    {
	      if(!game::WizardModeIsActive())
		{
		  /* This AP is used to take the stuff out of backpack */
		  DexterityAction(5);
		}

	      return true;
	    }
	  else
	    return false;
	}
      else
	{
	  if(IsPlayer())
	    ADD_MESSAGE("It's too dark here to read.");

	  return false;
	}
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("You can't read this.");

      return false;
    }
}

void character::CalculateBurdenState()
{
  uchar OldBurdenState = BurdenState;
  ulong SumOfMasses = GetCarriedWeight();

  if(SumOfMasses > ulong(GetCarryingStrength()) * 7500)
    BurdenState = OVER_LOADED;
  else if(SumOfMasses > ulong(GetCarryingStrength()) * 5000)
    BurdenState = STRESSED;
  else if(SumOfMasses > ulong(GetCarryingStrength()) * 2500)
    BurdenState = BURDENED;
  else
    BurdenState = UNBURDENED;

  if(!Initializing && BurdenState != OldBurdenState)
    CalculateBattleInfo();
}

void character::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  Stack->Save(SaveFile);
  SaveFile << ID;
  ushort c;

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    SaveFile << BaseAttribute[c] << BaseExperience[c];

  SaveFile << NP << AP;
  SaveFile << TemporaryState << EquipmentState << Money << WayPoint << Config << RegenerationCounter;
  SaveFile << IsEnabled() << Polymorphed << HomeData;

  for(c = 0; c < BodyParts; ++c)
    SaveFile << BodyPartSlot[c] << OriginalBodyPartID[c];

  SaveFile << Action;

  for(c = 0; c < STATES; ++c)
    SaveFile << TemporaryStateCounter[c];

  if(GetTeam())
    {
      SaveFile << bool(true);
      SaveFile << Team->GetID();
    }
  else
    SaveFile << bool(false);

  if(GetTeam() && GetTeam()->GetLeader() == this)
    SaveFile << bool(true);
  else
    SaveFile << bool(false);

  SaveFile << AssignedName << PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    SaveFile << GetCWeaponSkill(c);

  if(IsStuck())
    SaveFile << bool(true) << GetStackUnder()->SearchItem(StuckTo) << StuckToBodyPart;
  else
    SaveFile << bool(false);
}

void character::Load(inputfile& SaveFile)
{
  SquareUnder = game::GetSquareInLoad();
  Stack->Load(SaveFile);
  SaveFile >> ID;
  game::AddCharacterID(this, ID);
  ushort c;

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    SaveFile >> BaseAttribute[c] >> BaseExperience[c];

  SaveFile >> NP >> AP;
  SaveFile >> TemporaryState >> EquipmentState >> Money >> WayPoint >> Config >> RegenerationCounter;

  if(!ReadType<bool>(SaveFile))
    Disable();

  SaveFile >> Polymorphed >> HomeData;

  for(c = 0; c < BodyParts; ++c)
    SaveFile >> BodyPartSlot[c] >> OriginalBodyPartID[c];

  SaveFile >> Action;

  if(Action)
    Action->SetActor(this);

  for(c = 0; c < STATES; ++c)
    SaveFile >> TemporaryStateCounter[c];

  if(ReadType<bool>(SaveFile))
    SetTeam(game::GetTeam(ReadType<ushort>(SaveFile)));

  if(ReadType<bool>(SaveFile))
    GetTeam()->SetLeader(this);

  SaveFile >> AssignedName >> PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    SaveFile >> GetCWeaponSkill(c);

  if(ReadType<bool>(SaveFile))
    {
      StuckTo = GetStackUnder()->GetItem(ReadType<ushort>(SaveFile));
      SaveFile >> StuckToBodyPart;
    }

  InstallDataBase();
}

bool character::Engrave(const std::string& What)
{
  GetNearLSquare(GetPos())->Engrave(What);
  return true;
}

bool character::MoveRandomly()
{
  for(ushort c = 0; c < 10; ++c)
    {
      vector2d ToTry = GetPos() + game::GetMoveVector(RAND() & 7);

      if(GetLevel()->IsValidPos(ToTry) && !GetNearLSquare(ToTry)->IsDangerousForAIToStepOn(this) && TryMove(ToTry, false))
	return true;
    }

  return false;
}

bool character::TestForPickup(item* ToBeTested) const
{
  if(MakesBurdened(ToBeTested->GetWeight() + GetCarriedWeight()))
    return false;

  return true;
}

bool character::OpenPos(vector2d APos)
{
  return GetNearLSquare(APos)->Open(this);
}

bool character::ClosePos(vector2d APos)
{
  return GetNearLSquare(APos)->Close(this);
}

long character::GetStatScore() const
{
  long SkillScore = 0;

  for(ushort c = 0; c < GetAllowedWeaponSkillCategories(); ++c)
    SkillScore += GetCWeaponSkill(c)->GetHits();

  return (SkillScore << 1) + (long(game::GetAveragePlayerArmStrength() + game::GetAveragePlayerLegStrength() + game::GetAveragePlayerDexterity() + game::GetAveragePlayerAgility()) + GetAttribute(ENDURANCE) + GetAttribute(PERCEPTION) + GetAttribute(INTELLIGENCE) + GetAttribute(WISDOM) + GetAttribute(CHARISMA) + GetAttribute(MANA)) * 50;
}

long character::GetScore() const
{
  return (GetPolymorphBackup() ? GetPolymorphBackup()->GetStatScore() : GetStatScore()) + GetMoney() / 5 + GetStuffScore() + game::GodScore();
}

void character::AddScoreEntry(const std::string& Description, float Multiplier, bool AddEndLevel) const
{
  if(!game::WizardModeIsActive())
    {
      highscore HScore;
      std::string Desc = PLAYER->GetAssignedName();
      Desc << ", " << Description;

      if(AddEndLevel)
	Desc << " in " + (game::IsInWilderness() ? "the World map" : game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrentLevelIndex()));

      HScore.Add(long((GetScore() - game::GetBaseScore()) * Multiplier), Desc);
      HScore.Save();
    }
}

bool character::CheckDeath(const std::string& Msg, const character* Murderer, bool ForceMsg)
{
  if(!IsEnabled())
    return true;

  if(IsDead())
    {
      if(Murderer && Murderer->IsPlayer() && GetTeam()->GetKillEvilness())
	game::DoEvilDeed(GetTeam()->GetKillEvilness());

      std::string NewMsg = Msg;

      if(GetAction())
	NewMsg += GetAction()->GetDeathExplanation();

      if(IsPlayer() && game::WizardModeIsActive())
	ADD_MESSAGE("Death message: %s.", NewMsg.c_str());

      Die(Murderer, NewMsg, ForceMsg);
      return true;
    }
  else
    return false;
}

bool character::CheckStarvationDeath(const std::string& Msg)
{
  if(GetNP() < 1 && UsesNutrition())
    {
      Die(0, Msg);
      return true;
    }
  else
    return false;
}

void character::ThrowItem(uchar Direction, item* ToBeThrown)
{
  if(Direction > 7)
    ABORT("Throw in TOO odd direction...");

  ToBeThrown->Fly(this, Direction, GetAttribute(ARM_STRENGTH));
}

void character::HasBeenHitByItem(character* Thrower, item* Thingy, ushort Damage, float ToHitValue, uchar Direction)
{
  if(IsPlayer())
    ADD_MESSAGE("%s hits you.", Thingy->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s hits %s.", Thingy->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));

  ushort BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, DodgeValue);
  ushort DoneDamage = ReceiveBodyPartDamage(Thrower, Damage, PHYSICAL_DAMAGE, BodyPart, Direction);
  bool Succeeded = (GetBodyPart(BodyPart) && HitEffect(Thrower, Thingy, THROW_ATTACK, BodyPart, Direction, !DoneDamage)) || DoneDamage;

  if(Succeeded)
    Thrower->WeaponSkillHit(Thingy, THROW_ATTACK);

  std::string DeathMsg = "killed by a flying " + Thingy->GetName(UNARTICLED);

  if(CheckDeath(DeathMsg, Thrower))
    return;

  if(Thrower->CanBeSeenByPlayer())
    DeActivateVoluntaryAction("The attack of " + Thrower->GetName(DEFINITE) + " interupts you.");
  else
    DeActivateVoluntaryAction("The attack interupts you.");
}

bool character::DodgesFlyingItem(item* Item, float ToHitValue)
{
  return !Item->EffectIsGood() && RAND() % ushort(100 + ToHitValue / DodgeValue * 100) < 100;
}

void character::GetPlayerCommand()
{
  bool HasActed = false;

  while(!HasActed)
    {
      game::DrawEverything();
      game::SetIsInGetCommand(true);
      int Key = GET_KEY();
      game::SetIsInGetCommand(false);
      bool ValidKeyPressed = false;
      ushort c;

      for(c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	if(Key == game::GetMoveCommandKey(c))
	  {
	    HasActed = TryMove(GetPos() + ApplyStateModification(game::GetMoveVector(c)));
	    ValidKeyPressed = true;
	  }

      for(c = 1; commandsystem::GetCommand(c); ++c)
	if(Key == commandsystem::GetCommand(c)->GetKey())
	  {
	    if(game::IsInWilderness() && !commandsystem::GetCommand(c)->IsUsableInWilderness())
	      ADD_MESSAGE("This function cannot be used while in wilderness.");
	    else
	      if(!game::WizardModeIsActive() && commandsystem::GetCommand(c)->IsWizardModeFunction())
		ADD_MESSAGE("Activate wizardmode to use this function.");
	      else
		HasActed = commandsystem::GetCommand(c)->GetLinkedFunction()(this);

	    ValidKeyPressed = true;
	    break;
	  }

      if(!ValidKeyPressed)
	ADD_MESSAGE("Unknown key, you %s. Press '?' for a list of commands.", game::Insult());
    }
}

void character::Vomit(ushort Amount)
{
  if(!IsAlive())
    return;
    
  if(IsPlayer())
    ADD_MESSAGE("You vomit.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s vomits.", CHAR_NAME(DEFINITE));

  EditExperience(ARM_STRENGTH, -50);
  EditExperience(LEG_STRENGTH, -50);
  EditNP(-2000 - RAND() % 2001);

  if(StateIsActivated(PARASITIZED) && !(RAND() & 3))
    {
      if(IsPlayer())
	ADD_MESSAGE("You notice a dead broad tapeworm among your former stomach contents.");

      DeActivateTemporaryState(PARASITIZED);
    }

  if(!game::IsInWilderness())
    GetLSquareUnder()->ReceiveVomit(this, Amount);
}

bool character::Polymorph(character* NewForm, ushort Counter)
{
  if(!IsPolymorphable() || (!IsPlayer() && game::IsInWilderness()))
    {
      delete NewForm;
      return false;
    }

  if(StuckToBodyPart != NONE_INDEX)
    {
      StuckToBodyPart = NONE_INDEX;
      StuckTo = 0;
    }

  if(GetAction())
    GetAction()->Terminate(false);

  NewForm->SetAssignedName("");

  if(IsPlayer())
    ADD_MESSAGE("Your body glows in a crimson light. You transform into %s!", NewForm->CHAR_NAME(INDEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s glows in a crimson light and %s transforms into %s!", CHAR_NAME(DEFINITE), GetPersonalPronoun().c_str(), NewForm->CHAR_NAME(INDEFINITE));

  InNoMsgMode = NewForm->InNoMsgMode = true;
  GetSquareUnder()->RemoveCharacter();
  GetSquareUnder()->AddCharacter(NewForm);
  NewForm->SetAssignedName(GetAssignedName());
  NewForm->ActivateTemporaryState(POLYMORPHED);
  NewForm->SetTemporaryStateCounter(POLYMORPHED, Counter);

  if(TemporaryStateIsActivated(POLYMORPHED))
    {
      NewForm->SetPolymorphBackup(GetPolymorphBackup());
      SetPolymorphBackup(0);
      SendToHell();
    }
  else
    {
      NewForm->SetPolymorphBackup(this);
      SetPolymorphed(true);
      Disable();
    }

  GetStack()->MoveItemsTo(NewForm->GetStack());
  NewForm->SetMoney(GetMoney());

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Item = GetEquipment(c);

      if(Item)
	{
	  if(NewForm->CanUseEquipment(c))
	    {
	      Item->RemoveFromSlot();
	      NewForm->SetEquipment(c, Item);
	    }
	  else
	    Item->MoveTo(IsPlayer() ? NewForm->GetStack() : NewForm->GetStackUnder());
	}
    }

  NewForm->ChangeTeam(GetTeam());

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(NewForm);

  SetSquareUnder(0);
  InNoMsgMode = NewForm->InNoMsgMode = false;
  NewForm->CalculateAll();

  if(IsPlayer())
    {
      game::SetPlayer(NewForm);
      game::SendLOSUpdateRequest();
      UpdateESPLOS();
    }

  NewForm->TestWalkability();
  return true;
}

void character::BeKicked(character* Kicker, item* Boot, float KickDamage, float ToHitValue, short Success, bool Critical, bool ForceHit)
{
  switch(TakeHit(Kicker, Boot, KickDamage, ToHitValue, Success, KICK_ATTACK, Critical, ForceHit))
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case DID_NO_DAMAGE:
      if(!CheckBalance(KickDamage))
	{
	  if(IsPlayer())
	    ADD_MESSAGE("The kick throws you off balance.");
	  else if(Kicker->IsPlayer())
	    ADD_MESSAGE("The kick throws %s off balance.", CHAR_DESCRIPTION(DEFINITE));

	  if(GetArea()->IsValidPos((GetPos() << 1) - Kicker->GetPos()))
	    FallTo(Kicker, (GetPos() << 1) - Kicker->GetPos());
	}
    }
}

/* return true if still in balance */

bool character::CheckBalance(float KickDamage)
{
  return !CanWalk() || !KickDamage || KickDamage * 25 < RAND() % GetSize();
}

void character::FallTo(character* GuiltyGuy, vector2d Where)
{
  EditAP(-500);
  olterrain* Terrain = GetNearLSquare(Where)->GetOLTerrain();

  if(!Terrain || Terrain->IsWalkable(this))
    {
      if(!GetNearSquare(Where)->GetCharacter())
	Move(Where, true);
    }
  else
    {
      if(HasHead())
	{
	  if(IsPlayer()) 
	    ADD_MESSAGE("You hit your head on the wall.");
	  else if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s hits %s head on the wall.", CHAR_NAME(DEFINITE), GetPossessivePronoun().c_str());
	}

      ReceiveDamage(GuiltyGuy, 1 + RAND() % 5, PHYSICAL_DAMAGE, HEAD);
      CheckDeath("killed by hitting a wall", GuiltyGuy);
    }

  // Place code that handles characters bouncing to each other here
}

bool character::CheckCannibalism(const material* What) const
{ 
  return GetTorso()->GetConsumeMaterial()->IsSameAs(What); 
}

void character::StandIdleAI()
{
  SeekLeader();

  if(CheckForEnemies(true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(CheckForDoors())
    return;

  if(MoveTowardsHomePos())
    return;

  EditAP(-1000);
}

bool character::Faint(ushort Counter, bool HungerFaint)
{
  if(GetAction())
    {
      if(HungerFaint && !GetAction()->AllowFaint())
	return false;
      else
	GetAction()->Terminate(false);
    }

  if(IsPlayer())
    ADD_MESSAGE("You faint.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s faints.", CHAR_NAME(DEFINITE));

  faint* Faint = new faint(this);
  Faint->SetCounter(Counter);
  SetAction(Faint);
  return true;
}

void character::DeActivateVoluntaryAction(const std::string& Reason)
{
  if(GetAction() && GetAction()->IsVoluntary())
    {
      if(IsPlayer())
	{
	  if(Reason.length())
	    ADD_MESSAGE("%s", Reason.c_str());

	  if(game::BoolQuestion("Continue " + GetAction()->GetDescription() + "? [y/N]"))
	    GetAction()->SetInDNDMode(true);
	  else
	    GetAction()->Terminate(false);
	}
      else
	GetAction()->Terminate(false);
    }
}

void character::ActionAutoTermination()
{
  if(!GetAction() || !GetAction()->IsVoluntary() || GetAction()->InDNDMode())
    return;

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled() && (*i)->CanBeSeenBy(this, false, true) && ((*i)->CanMove() || (*i)->GetPos().IsAdjacent(GetPos())) && (*i)->CanAttack())
	  {
	    if(IsPlayer())
	      {
		ADD_MESSAGE("%s seems to be hostile.", (*i)->CHAR_NAME(DEFINITE));

		if(game::BoolQuestion("Continue " + GetAction()->GetDescription() + "? [y/N]"))
		  GetAction()->SetInDNDMode(true);
		else
		  GetAction()->Terminate(false);
	      }
	    else
	      GetAction()->Terminate(false);

	    return;
	  }
}

bool character::CheckForEnemies(bool CheckDoors, bool CheckGround, bool MayMoveRandomly)
{
  bool HostileCharsNear = false;
  character* NearestChar = 0;
  ulong NearestDistance = 0xFFFFFFFF;

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
	  {
	    ulong ThisDistance = Max<ulong>(abs((*i)->GetPos().X - GetPos().X), abs((*i)->GetPos().Y - GetPos().Y));

	    if(ThisDistance <= GetLOSRangeSquare())
	      HostileCharsNear = true;

	    if((ThisDistance < NearestDistance || (ThisDistance == NearestDistance && !(RAND() % 3))) && (*i)->CanBeSeenBy(this, false, !CanWalkThroughWalls()) && GetAttribute(WISDOM) < (*i)->GetAttackWisdomLimit())
	      {
		NearestChar = *i;
		NearestDistance = ThisDistance;
	      }
	  }

  if(NearestChar)
    {
      if(SpecialEnemySightedReaction(NearestChar))
	return true;

      if(IsExtraCoward() && !StateIsActivated(PANIC) && NearestChar->GetRelativeDanger(this) >= 0.5f)
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s sees %s.", CHAR_NAME(DEFINITE), NearestChar->CHAR_DESCRIPTION(DEFINITE));

	  BeginTemporaryState(PANIC, 500 + RAND() % 500);
	}

      if(!IsRetreating())
	WayPoint = NearestChar->GetPos();
      else
	WayPoint = (GetPos() << 1) - NearestChar->GetPos();

      return MoveTowards(WayPoint);
    }
  else
    {
      if(!GetTeam()->GetLeader() && WayPoint.X != -1)
	{
	  if(!MoveTowards(WayPoint))
	    {
	      WayPoint.X = -1;
	      return false;
	    }
	  else
	    {
	      if(GetPos() == WayPoint)
		WayPoint.X = -1;

	      return true;
	    }
	}
      else
	{
	  if((!GetTeam()->GetLeader() || (GetTeam()->GetLeader() && WayPoint.X == -1)) && HostileCharsNear)
	    {
	      if(CheckDoors && CheckForDoors())
		return true;

	      if(CheckGround && CheckForUsefulItemsOnGround())
		return true;

	      if(MayMoveRandomly && MoveRandomly()) // one has heard that an enemy is near but doesn't know where
		return true;; 
	    }

	  return false;
	}
    }
}

bool character::CheckForDoors()
{
  if(!CanOpen())
    return false;

  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square && Square->GetOLTerrain() && Square->GetOLTerrain()->Open(this))
	return true;
    }

  return false;
}

bool character::CheckForUsefulItemsOnGround()
{
  itemvector ItemVector;
  GetStackUnder()->FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->CanBeSeenBy(this) && ItemVector[c]->IsPickable(this))
      {
	if(TryToEquip(ItemVector[c]))
	  return true;

	if(UsesNutrition() && GetHungerState() < SATIATED && TryToConsume(ItemVector[c]))
	  return true;
      }

  return false;
}

bool character::FollowLeader()
{
  if(!GetTeam()->GetLeader())
    return false;

  if(GetTeam()->GetLeader()->CanBeSeenBy(this))
    {
      vector2d Distance = GetPos() - WayPoint;

      if(abs(short(Distance.X)) <= 2 && abs(short(Distance.Y)) <= 2)
	return false;
      else
	return MoveTowards(WayPoint);
    }
  else
    if(WayPoint.X != -1)
      if(!MoveTowards(WayPoint))
	{
	  WayPoint.X = -1;
	  return false;
	}
      else
	return true;
    else
      return false;
}

void character::SeekLeader()
{
  if(GetTeam()->GetLeader() && GetTeam()->GetLeader()->CanBeSeenBy(this))
    WayPoint = GetTeam()->GetLeader()->GetPos();
}

ushort character::GetMoveEase() const
{
  switch(GetBurdenState())
    {
    case BURDENED:
      return 75;
    case STRESSED:
    case OVER_LOADED:
      return 50;
    default:
      return 100;
    }
}

ushort character::GetLOSRange() const
{
  if(!game::IsInWilderness())
    return GetAttribute(PERCEPTION) * GetLevel()->GetLOSModifier() / 48;
  else
    return 3;
}

bool character::Displace(character* Who, bool Forced)
{
  if(GetBurdenState() == OVER_LOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You try very hard to crawl forward. But your load is too heavy.");

      return true;
    }

  if((Forced || (GetRelativeDanger(Who) > 1.0f && Who->CanBeDisplaced())) && !IsStuck() && !Who->IsStuck() && (!Who->GetAction() || Who->GetAction()->AllowDisplace()) && GetLSquareUnder()->IsWalkable(Who))
    {
      if(IsPlayer())
	ADD_MESSAGE("You displace %s!", Who->CHAR_DESCRIPTION(DEFINITE));
      else if(Who->IsPlayer())
	ADD_MESSAGE("%s displaces you!", CHAR_DESCRIPTION(DEFINITE));
      else if(CanBeSeenByPlayer() || Who->CanBeSeenByPlayer())
	ADD_MESSAGE("%s displaces %s!", CHAR_DESCRIPTION(DEFINITE), Who->CHAR_DESCRIPTION(DEFINITE));

      GetLSquareUnder()->SwapCharacter(Who->GetLSquareUnder());

      if(IsPlayer())
	ShowNewPosInfo();

      if(Who->IsPlayer())
	Who->ShowNewPosInfo();

      EditAP(-GetMoveAPRequirement(GetSquareUnder()->GetEntryDifficulty()) - 500);
      EditNP(-10 * GetSquareUnder()->GetEntryDifficulty());
      EditExperience(AGILITY, 5 * GetSquareUnder()->GetEntryDifficulty());
      return true;
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("%s resists!", Who->CHAR_DESCRIPTION(DEFINITE));

      return false;
    }
}

void character::SetNP(long What)
{
  uchar OldGetHungerState = GetHungerState();
  NP = What;

  if(IsPlayer())
    {
      if(GetHungerState() == STARVING && OldGetHungerState != STARVING)
	DeActivateVoluntaryAction("You are getting very hungry.");
      else if(GetHungerState() == HUNGRY && OldGetHungerState != HUNGRY && OldGetHungerState != STARVING)
	DeActivateVoluntaryAction("You are getting hungry.");
    }
}

void character::ShowNewPosInfo() const
{
  if(GetPos().X < game::GetCamera().X + 3 || GetPos().X >= game::GetCamera().X + game::GetScreenSize().X - 3)
    game::UpdateCameraX();

  if(GetPos().Y < game::GetCamera().Y + 3 || GetPos().Y >= game::GetCamera().Y + game::GetScreenSize().Y - 3)
    game::UpdateCameraY();

  game::SendLOSUpdateRequest();
  UpdateESPLOS();

  if(!game::IsInWilderness())
    {
      if(GetLSquareUnder()->IsDark() && !game::SeeWholeMapCheatIsActive())
	ADD_MESSAGE("It's dark in here!");

      GetLSquareUnder()->ShowSmokeMessage();
      std::vector<itemvector> PileVector;
      GetStackUnder()->Pile(PileVector, this);

      if(PileVector.size() == 1)
	ADD_MESSAGE("%s %s lying here.", PileVector[0][0]->GetName(INDEFINITE, PileVector[0].size()).c_str(), PileVector[0].size() == 1 ? "is" : "are");
      else if(!PileVector.empty())
	ADD_MESSAGE("Several items are lying here.");
		
      if(GetNearLSquare(GetPos())->GetEngraved().length())
	ADD_MESSAGE("Something has been engraved here: \"%s\"", GetNearLSquare(GetPos())->GetEngraved().c_str());
    }
}

void character::Hostility(character* Enemy)
{
  if(Enemy == this || !Enemy || !Team || !Enemy->Team)
    return;

  if(GetTeam()->GetID() != Enemy->GetTeam()->GetID())
    GetTeam()->Hostility(Enemy->GetTeam());
  else if(Enemy->IsEnabled() && IsPlayer())
    {
      if(Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("%s becomes enraged.", Enemy->CHAR_NAME(DEFINITE));

      Enemy->ChangeTeam(game::GetTeam(MONSTER_TEAM));
    }
}

stack* character::GetGiftStack() const
{
  if(GetLSquareUnder()->GetRoomIndex() && !GetLSquareUnder()->GetRoom()->AllowDropGifts())
    return GetStack();
  else
    return GetStackUnder();
}

bool character::MoveRandomlyInRoom()
{
  for(ushort c = 0; c < 10; ++c)
    {
      vector2d ToTry = GetPos() + game::GetMoveVector(RAND() & 7);

      if(GetLevel()->IsValidPos(ToTry))
	{
	  lsquare* Square = GetNearLSquare(ToTry);

	  if(!Square->IsDangerousForAIToStepOn(this) && (!Square->GetOLTerrain() || !Square->GetOLTerrain()->IsDoor()) && TryMove(ToTry, false))
	    return true;
	}
    }

  return false;
}

void character::GoOn(go* Go)
{
  if(!GetArea()->IsValidPos(GetPos() + game::GetMoveVector(Go->GetDirection())))
    {
      Go->Terminate(false);
      return;
    }

  lsquare* MoveToSquare = GetNearLSquare(GetPos() + game::GetMoveVector(Go->GetDirection()));

  if(!MoveToSquare->IsWalkable(this)
  || (MoveToSquare->GetCharacter() && GetTeam() != MoveToSquare->GetCharacter()->GetTeam())
  ||  MoveToSquare->IsDangerous(this))
    {
      Go->Terminate(false);
      return;
    }

  uchar OKDirectionsCounter = 0;

  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square && Square->IsWalkable(this))
	++OKDirectionsCounter;
    }

  if(!Go->GetWalkingInOpen())
    {
      if(OKDirectionsCounter > 2)
	{
	  Go->Terminate(false);
	  return;
	}
    }
  else
    if(OKDirectionsCounter <= 2)
      Go->SetWalkingInOpen(false);

  square* BeginSquare = GetSquareUnder();

  if(!TryMove(MoveToSquare->GetPos()) || BeginSquare == GetSquareUnder() || GetLSquareUnder()->IsDark() || GetStackUnder()->GetVisibleItems(this))
    {
      Go->Terminate(false);
      return;
    }

  game::DrawEverything();
}

void character::SetTeam(team* What)
{
  Team = What;

  if(GetSquareUnder())
    GetSquareUnder()->SendNewDrawRequest();

  SetTeamIterator(What->Add(this));
}

void character::ChangeTeam(team* What)
{
  if(Team)
    Team->Remove(GetTeamIterator());

  Team = What;
  GetSquareUnder()->SendNewDrawRequest();

  if(Team)
    SetTeamIterator(Team->Add(this));
}

bool character::ChangeRandomStat(short HowMuch)
{
  for(ushort c = 0; c < 50; ++c)
    {
      ushort AttribID = RAND() % ATTRIBUTES;

      if(EditAttribute(AttribID, HowMuch))
	return true;
    }

  return false;
}

ushort character::RandomizeReply(ulong& Said, ushort Replies)
{
  bool NotSaid = false;

  for(ushort c = 0; c < Replies; ++c)
    if(!(Said & (1 << c)))
      {
	NotSaid = true;
	break;
      }

  if(!NotSaid)
    Said = 0;

  ulong ToSay;
  while(Said & 1 << (ToSay = RAND() % Replies));
  Said |= 1 << ToSay;
  return ToSay;
}

void character::DisplayInfo(std::string& Msg)
{
  if(IsPlayer())
    Msg << " You are " << GetStandVerb() << " here.";
  else
    {
      Msg << " " << festring::CapitalizeCopy(GetName(INDEFINITE)) << " is " << GetStandVerb() << " here. " << festring::CapitalizeCopy(GetPersonalPronoun());

      if(GetTeam() == PLAYER->GetTeam())
	Msg << " is tame";
      else
	{
	  uchar Relation = GetRelation(PLAYER);

	  if(Relation == HOSTILE)
	    Msg << " is hostile";
	  else if(Relation == UNCARING)
	    Msg << " does not care about you";
	  else
	    Msg << " is friendly";
	}
      if(StateIsActivated(PANIC))
	Msg << " and panicked.";
      else
	Msg << '.';
    }
}

void character::TestWalkability()
{
  if(GetSquareUnder()->IsFatalToStay() && !GetSquareUnder()->SquareIsWalkable(this))
    {
      bool Alive = false;

      for(ushort d = 0; d < 8; ++d)
	{
	  square* Square = GetNeighbourSquare(d);

	  if(Square && Square->SquareIsWalkable(this) && !Square->GetCharacter())
	    {
	      if(IsPlayer())
		ADD_MESSAGE("%s.", GetSquareUnder()->SurviveMessage(this).c_str());
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s.", GetSquareUnder()->MonsterSurviveMessage(this).c_str());

	      Move(Square->GetPos(), true); // actually, this shouldn't be a teleport move
	      Alive = true;
	      break;
	    }
	}

      if(!Alive)
	{
	  if(IsPlayer())
	    {
	      GetSquareUnder()->RemoveCharacter();
	      SendToHell();
	      game::AskForKeyPress(GetSquareUnder()->DeathMessage(this) + ".");
	      PLAYER->AddScoreEntry(GetSquareUnder()->ScoreEntry(this));
	      game::End();
	    }
	  else
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), GetSquareUnder()->MonsterDeathVerb(this).c_str());

	      Die();
	    }
	}
    }
}

ushort character::GetSize() const
{
  if(GetTorso())
    return GetTorso()->GetSize();
  else
    return 0;
}

void character::SetMainMaterial(material* NewMaterial, ushort SpecialFlags)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->SetMainMaterial(NewMaterial, SpecialFlags);

  for(ushort c = 1; c < GetBodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetMainMaterial()->GetVolume());
      GetBodyPart(c)->SetMainMaterial(NewMaterial, SpecialFlags);
    }
}

void character::ChangeMainMaterial(material* NewMaterial, ushort SpecialFlags)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->ChangeMainMaterial(NewMaterial, SpecialFlags);

  for(ushort c = 1; c < GetBodyParts(); ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetMainMaterial()->GetVolume());
      GetBodyPart(c)->ChangeMainMaterial(NewMaterial, SpecialFlags);
    }
}

void character::SetSecondaryMaterial(material*, ushort)
{
  ABORT("Illegal character::SetSecondaryMaterial call!");
}

void character::ChangeSecondaryMaterial(material*, ushort)
{
  ABORT("Illegal character::ChangeSecondaryMaterial call!");
}

void character::SetContainedMaterial(material*, ushort)
{
  ABORT("Illegal character::SetContainedMaterial call!");
}

void character::ChangeContainedMaterial(material*, ushort)
{
  ABORT("Illegal character::ChangeContainedMaterial call!");
}

void character::TeleportRandomly()
{
  StuckTo = 0;
  SetStuckToBodyPart(NONE_INDEX);

  if(StateIsActivated(TELEPORT_CONTROL) && IsPlayer())
    {
      while(true)
	{
	  vector2d PlayersInput = game::PositionQuestion("Where do you wish to teleport? [direction keys move cursor, space accepts]", GetPos(), 0, 0, false);

	  if(GetNearLSquare(PlayersInput)->IsWalkable(this) || game::GoThroughWallsCheatIsActive())
	    {
	      if(GetNearLSquare(PlayersInput)->GetPos() == GetPos())
		{
		  ADD_MESSAGE("You disappear and reappear.");
		  return;
		}

	      if(GetNearLSquare(PlayersInput)->GetCharacter())
		{
		  ADD_MESSAGE("You feel that something weird has happened, but can't really tell what exactly.");
		  break;
		  /* break this loop and teleport randomly */
		}

	      Move(PlayersInput, true);
	      return;
	    }
	}
    }

  Move(GetLevel()->GetRandomSquare(this), true);

  if(!IsPlayer() && CanBeSeenByPlayer())
    ADD_MESSAGE("%s appears.", CHAR_NAME(INDEFINITE));
}

void character::RestoreHP()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->RestoreHP();
    }

  HP = MaxHP;
}

void character::RestoreLivingHP()
{
  HP = 0;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->IsAlive())
	{
	  BodyPart->RestoreHP();
	  HP += BodyPart->GetHP();
	}
    }
}

bool character::AllowDamageTypeBloodSpill(ushort Type)
{
  switch(Type&0xFFF)
    {
    case PHYSICAL_DAMAGE:
    case SOUND:
    case ENERGY:
      return true;
    case ACID:
    case FIRE:
    case DRAIN:
    case POISON:
    case ELECTRICITY:
      return false;
    default:
      ABORT("Unknown blood effect destroyed the dungeon!");
      return false;
    }
}

/* Returns truly done damage */

ushort character::ReceiveBodyPartDamage(character* Damager, ushort Damage, ushort Type, uchar BodyPartIndex, uchar Direction, bool PenetrateResistance, bool Critical, bool ShowNoDamageMsg)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);
  BodyPart->DamageArmor(Damager, Damage, Type);

  if(!PenetrateResistance)
    Damage -= (BodyPart->GetTotalResistance(Type) >> 1) + RAND() % ((BodyPart->GetTotalResistance(Type) >> 1) + 1);

  if(short(Damage) < 1)
    if(Critical)
      Damage = 1;
    else
      {
	if(ShowNoDamageMsg)
	  {
	    if(IsPlayer())
	      ADD_MESSAGE("You are not hurt.");
	    else if(CanBeSeenByPlayer())
	      ADD_MESSAGE("%s is not hurt.", GetPersonalPronoun().c_str());
	  }

	return 0;
      }

  if(Critical && AllowDamageTypeBloodSpill(Type) && !game::IsInWilderness())
    {
      BodyPart->SpillBlood(3 + RAND() % 3);

      for(ushort d = 0; d < 8; ++d)
	{
	  lsquare* Square = GetNeighbourLSquare(d);

	  if(Square && Square->IsFlyable())
	    BodyPart->SpillBlood(2 + (RAND() & 1), Square->GetPos());
	}
    }

  if(BodyPart->ReceiveDamage(Damager, Damage, Type) && BodyPartCanBeSevered(BodyPartIndex))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your %s is severed off!", BodyPart->GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s %s is severed off!", GetPossessivePronoun().c_str(), BodyPart->GetBodyPartName().c_str());

      item* Severed = SevereBodyPart(BodyPartIndex);
      GetSquareUnder()->SendNewDrawRequest();

      if(Severed)
	{
	  if(!game::IsInWilderness())
	    {
	      GetStackUnder()->AddItem(Severed);
	      
	      if(Direction != YOURSELF)
		Severed->Fly(0, Direction, Damage);
	    }
	  else
	    GetStack()->AddItem(Severed);

	  Severed->DropEquipment();
	}
      else if(IsPlayer() || CanBeSeenByPlayer())
	ADD_MESSAGE("It vanishes.");

      if(IsPlayer())
	game::AskForKeyPress("Bodypart severed! [press any key to continue]");
      else if(RAND() % 100 < GetPanicLevel() && !IsDead())
	BeginTemporaryState(PANIC, 1000 + RAND() % 1001);
    }

  if(!IsPlayer() && !IsDead())
    CheckPanic(500);

  return Damage;
}

/* Returns 0 if bodypart dissappears */

item* character::SevereBodyPart(ushort BodyPartIndex)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);

  if(BodyPartsDisappearWhenSevered() || StateIsActivated(POLYMORPHED))
    {
      BodyPart->RemoveFromSlot();
      GetStackUnder()->AddItem(BodyPart);
      BodyPart->DropEquipment();
      BodyPart->RemoveFromSlot();
      CalculateAttributeBonuses();
      CalculateBattleInfo();
      BodyPart->SendToHell();
      return 0;
    }
  else
    {
      BodyPart->SetOwnerDescription("of " + GetName(INDEFINITE));
      BodyPart->SetIsUnique(LeftOversAreUnique());
      UpdateBodyPartPicture(BodyPartIndex, true);
      BodyPart->RemoveFromSlot();
      BodyPart->RandomizePosition();
      CalculateAttributeBonuses();
      CalculateBattleInfo();

      if(StuckToBodyPart == BodyPartIndex)
	{
	  StuckToBodyPart = NONE_INDEX;
	  StuckTo = 0;
	}

      return BodyPart;
    }
}

/* The second uchar is actually TargetFlags, which is not used here, but seems to be used in humanoid::ReceiveDamage. Returns true if the character really receives damage */

bool character::ReceiveDamage(character* Damager, ushort Damage, ushort Type, uchar, uchar Direction, bool, bool PenetrateArmor, bool Critical, bool ShowMsg)
{
  bool Affected = ReceiveBodyPartDamage(Damager, Damage, Type, 0, Direction, PenetrateArmor, Critical, ShowMsg) != 0;

  if(DamageTypeAffectsInventory(Type))
    {
      for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	if(GetEquipment(c))
	  GetEquipment(c)->ReceiveDamage(Damager, Damage, Type);

      GetStack()->ReceiveDamage(Damager, Damage, Type);
    }

  return Affected;
}

bool character::BodyPartCanBeSevered(ushort Index) const
{
  if(Index == 0)
    return false;
  else
    return true;
}

std::string character::GetDescription(uchar Case) const
{
  if(IsPlayer())
    return "you";
  else if(CanBeSeenByPlayer())
    return GetName(Case);
  else
    return "something";
}

std::string character::GetPersonalPronoun(bool PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return "you";
  else if(GetSex() == UNDEFINED || (!CanBeSeenByPlayer() && !game::SeeWholeMapCheatIsActive()))
    return "it";
  else if(GetSex() == MALE)
    return "he";
  else
    return "she";
}

std::string character::GetPossessivePronoun(bool PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return "your";
  else if(GetSex() == UNDEFINED || (!CanBeSeenByPlayer() && !game::SeeWholeMapCheatIsActive()))
    return "its";
  else if(GetSex() == MALE)
    return "his";
  else
    return "her";
}

std::string character::GetObjectPronoun(bool PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return "you";
  else if(GetSex() == UNDEFINED || (!CanBeSeenByPlayer() && !game::SeeWholeMapCheatIsActive()))
    return "it";
  else if(GetSex() == MALE)
    return "him";
  else
    return "her";
}

void character::AddName(std::string& String, uchar Case) const
{
  if(!(Case & PLURAL) && AssignedName.length())
    {
      if(!ShowClassDescription())
	String << AssignedName;
      else
	{
	  String << AssignedName << " ";
	  id::AddName(String, (Case|ARTICLE_BIT)&~INDEFINE_BIT);
	}
    }
  else
    id::AddName(String, Case);
}

uchar character::GetHungerState() const
{
  if(!UsesNutrition())
    return NOT_HUNGRY;
  else if(GetNP() > OVER_FED_LEVEL)
    return OVER_FED;
  else if(GetNP() > BLOATED_LEVEL)
    return BLOATED;
  else if(GetNP() > SATIATED_LEVEL)
    return SATIATED;
  else if(GetNP() > NOT_HUNGER_LEVEL)
    return NOT_HUNGRY;
  else if(GetNP() > HUNGER_LEVEL)
    return HUNGRY;
  else
    return STARVING;
}

bool character::CanConsume(material* Material) const
{
  return (GetConsumeFlags() & Material->GetConsumeType()) != 0;
}

void character::SetTemporaryStateCounter(ulong State, ushort What)
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      TemporaryStateCounter[c] = What;
}

void character::EditTemporaryStateCounter(ulong State, short What)
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      TemporaryStateCounter[c] += What;
}

ushort character::GetTemporaryStateCounter(ulong State) const
{
  for(ushort c = 0; c < STATES; ++c)
    if((1 << c) & State)
      return TemporaryStateCounter[c];

  ABORT("Illegal GetTemporaryStateCounter request!");
  return 0;
}

bool character::CheckKick() const
{
  if(!CanKick())
    {
      ADD_MESSAGE("This race can't kick.");
      return false;
    }
  else
    return true;
}

ushort character::GetResistance(ushort Type) const
{
  switch(Type&0xFFF)
    {
    case PHYSICAL_DAMAGE:
    case SOUND:
    case ENERGY:
    case ACID:
    case DRAIN:
      return 0;
    case FIRE: return GetFireResistance();
    case POISON: return GetPoisonResistance();
    case ELECTRICITY: return GetElectricityResistance();
    default:
      ABORT("Resistance lack detected!");
      return 0;
    }
}

void character::Regenerate()
{
  if(HP == MaxHP)
    return;

  ulong RegenerationBonus = 0;
  bool NoHealableBodyParts = true;

  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->IsAlive())
	{
	  RegenerationBonus += BodyPart->GetMaxHP();

	  if(NoHealableBodyParts && BodyPart->GetHP() < BodyPart->GetMaxHP())
	    NoHealableBodyParts = false;
	}
    }

  if(!RegenerationBonus || NoHealableBodyParts)
    return;

  RegenerationBonus *= (50 + GetAttribute(ENDURANCE));

  if(Action && Action->GetRestRegenerationBonus())
    RegenerationBonus *= GetSquareUnder()->GetRestModifier();

  RegenerationCounter += RegenerationBonus;

  while(RegenerationCounter > 1000000)
    {
      if(!HealHitPoint())
	break;

      RegenerationCounter -= 1000000;
      EditExperience(ENDURANCE, Max(5000 / MaxHP, 1));
    }
}

void character::PrintInfo() const
{
  felist Info("Information about " + GetName(DEFINITE));

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if((EquipmentEasilyRecognized(c) || game::WizardModeIsActive()) && GetEquipment(c))
      Info.AddEntry(GetEquipmentName(c) + ": " + GetEquipment(c)->GetName(INDEFINITE), LIGHT_GRAY, 0, GetEquipment(c)->GetPicture(), true, GetEquipment(c)->AllowAlphaEverywhere());

  if(Info.IsEmpty())
    ADD_MESSAGE("There's nothing special to tell about %s.", CHAR_NAME(DEFINITE));
  else
    {
      game::SetStandardListAttributes(Info);
      Info.Draw();
    }
}

void character::CompleteRiseFromTheDead()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      {
	GetBodyPart(c)->ResetSpoiling();
	GetBodyPart(c)->SetHP(1);
      }
}

bool character::RaiseTheDead(character*)
{
  bool Useful = false;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(!BodyPart && CanCreateBodyPart(c))
	{
	  CreateBodyPart(c)->SetHP(1);

	  if(IsPlayer())
	    ADD_MESSAGE("Suddenly you grow a new %s.", GetBodyPartName(c).c_str());
	  else if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s grows a new %s.", CHAR_NAME(DEFINITE), GetBodyPartName(c).c_str());

	  Useful = true;
	}
      else if(BodyPart && BodyPart->IsAlive() && BodyPart->GetHP() < 1)
	BodyPart->SetHP(1);
    }

  if(!Useful)
    {
      if(IsPlayer())
	ADD_MESSAGE("You shudder.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s shudders.", CHAR_NAME(DEFINITE));
    }

  return Useful;
}

void character::SetSize(ushort Size)
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->SetSize(GetBodyPartSize(c, Size));
}

ulong character::GetBodyPartSize(ushort Index, ushort TotalSize) const
{
  if(Index == TORSO_INDEX)
    return TotalSize;
  else
    {
      ABORT("Weird bodypart size request for a character!");
      return 0;
    }
}

ulong character::GetBodyPartVolume(ushort Index) const
{
  if(Index == TORSO_INDEX)
    return GetTotalVolume();
  else
    {
      ABORT("Weird bodypart volume request for a character!");
      return 0;
    }
}

void character::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < GetBodyParts(); ++c) 
    if(CanCreateBodyPart(c))
      CreateBodyPart(c, SpecialFlags);
}

void character::RestoreBodyParts()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      CreateBodyPart(c);
}

void character::UpdatePictures()
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    UpdateBodyPartPicture(c);
}

bodypart* character::MakeBodyPart(ushort Index) const
{
  if(Index == TORSO_INDEX)
    return new normaltorso(0, NO_MATERIALS);
  else
    {
      ABORT("Weird bodypart to make for a character!");
      return 0;
    }
}

bodypart* character::CreateBodyPart(ushort Index, ushort SpecialFlags)
{
  bodypart* BodyPart = MakeBodyPart(Index);
  BodyPart->InitMaterials(CreateBodyPartMaterial(Index, GetBodyPartVolume(Index)), false);
  BodyPart->SetSize(GetBodyPartSize(Index, GetTotalSize()));
  SetBodyPart(Index, BodyPart);
  BodyPart->InitSpecialAttributes();

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdateBodyPartPicture(Index);

  if(!Initializing)
    {
      CalculateBattleInfo();

      if(GetSquareUnder())
	GetSquareUnder()->SendNewDrawRequest();
    }

  return BodyPart;
}

vector2d character::GetBodyPartBitmapPos(ushort Index, bool) const
{
  if(Index == TORSO_INDEX)
    return GetTorsoBitmapPos();
  else
    {
      ABORT("Weird bodypart BitmapPos request for a character!");
      return vector2d();
    }
}

ushort character::GetBodyPartColorA(ushort Index, bool) const
{
  if(Index < GetBodyParts())
    return GetSkinColor();
  else
    {
      ABORT("Weird bodypart color A request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColorB(ushort Index, bool) const
{
  if(Index == TORSO_INDEX)
    return GetTorsoMainColor();
  else
    {
      ABORT("Weird bodypart color B request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColorC(ushort Index, bool) const
{
  if(Index == TORSO_INDEX)
    return 0; // reserved for future use
  else
    {
      ABORT("Weird bodypart color C request for a character!");
      return 0;
    }
}

ushort character::GetBodyPartColorD(ushort Index, bool) const
{
  if(Index == TORSO_INDEX)
    return GetTorsoSpecialColor();
  else
    {
      ABORT("Weird bodypart color D request for a character!");
      return 0;
    }
}

void character::UpdateBodyPartPicture(ushort Index, bool Severed)
{
  bodypart* BodyPart = GetBodyPart(Index);

  if(BodyPart)
    {
      BodyPart->SetBitmapPos(GetBodyPartBitmapPos(Index, Severed));
      BodyPart->GetMainMaterial()->SetSkinColor(GetBodyPartColorA(Index, Severed));
      BodyPart->SetMaterialColorB(GetBodyPartColorB(Index, Severed));
      BodyPart->SetMaterialColorC(GetBodyPartColorC(Index, Severed));
      BodyPart->SetMaterialColorD(GetBodyPartColorD(Index, Severed));
      BodyPart->SetSpecialFlags(GetSpecialBodyPartFlags(Index, Severed));
      BodyPart->UpdatePictures();
    }
}

void character::LoadDataBaseStats()
{
  BaseAttribute[ENDURANCE] = GetDefaultEndurance() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[PERCEPTION] = GetDefaultPerception() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[INTELLIGENCE] = GetDefaultIntelligence() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[WISDOM] = GetDefaultWisdom() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[CHARISMA] = GetDefaultCharisma() * (100 + GetAttributeBonus()) / 100;
  BaseAttribute[MANA] = GetDefaultMana() * (100 + GetAttributeBonus()) / 100;
  SetMoney(GetDefaultMoney());

  const std::vector<long>& Skills = GetKnownCWeaponSkills();

  if(Skills.size())
    {
      const std::vector<long>& Hits = GetCWeaponSkillHits();

      if(Hits.size() == 1)
	{
	  for(ushort c = 0; c < Skills.size(); ++c)
	    CWeaponSkill[Skills[c]]->AddHit(Hits[0]);
	}
      else if(Hits.size() == Skills.size())
	{
	  for(ushort c = 0; c < Skills.size(); ++c)
	    CWeaponSkill[Skills[c]]->AddHit(Hits[c]);
	}
      else
	ABORT("Illegal weapon skill hit array size detected!");
    }
}

character* characterprototype::CloneAndLoad(inputfile& SaveFile) const
{
  character* Char = Cloner(0, LOAD);
  Char->Load(SaveFile);
  Char->CalculateAll();
  return Char;
}

void character::Initialize(ushort NewConfig, ushort SpecialFlags)
{
  Initializing = InNoMsgMode = true;
  CalculateBodyParts();
  CalculateAllowedWeaponSkillCategories();
  BodyPartSlot = new characterslot[BodyParts];
  OriginalBodyPartID = new std::list<ulong>[BodyParts];
  CWeaponSkill = new cweaponskill*[AllowedWeaponSkillCategories];

  ushort c;

  for(c = 0; c < BodyParts; ++c)
    BodyPartSlot[c].SetMaster(this);

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    CWeaponSkill[c] = new cweaponskill(c);

  if(!(SpecialFlags & LOAD))
    {
      ID = game::CreateNewCharacterID(this);
      Config = NewConfig;
      InstallDataBase();
      LoadDataBaseStats();
      TemporaryState |= GetClassStates();

      if(TemporaryState)
	for(c = 0; c < STATES; ++c)
	  if(TemporaryState & (1 << c))
	    TemporaryStateCounter[c] = PERMANENT;

      CreateBodyParts(SpecialFlags & NO_PIC_UPDATE);

      for(c = 0; c < BASE_ATTRIBUTES; ++c)
	BaseExperience[c] = 0;

      InitSpecialAttributes();

      if(!GetDefaultName().empty())
	SetAssignedName(GetDefaultName());
    }

  VirtualConstructor(SpecialFlags & LOAD);

  if(!(SpecialFlags & LOAD))
    {
      if(!(SpecialFlags & NO_EQUIPMENT))
	CreateInitialEquipment((SpecialFlags & NO_EQUIPMENT_PIC_UPDATE) >> 1);

      CalculateAll();
      RestoreHP();
    }

  Initializing = InNoMsgMode = false;
}

bool character::TeleportNear(character* Caller)
{
  vector2d Where = GetLevel()->GetNearestFreeSquare(this, Caller->GetPos());

  if(Where == ERROR_VECTOR)
    return false;

  Teleport(Where);
  return true;
}

void character::Teleport(vector2d Pos)
{
  Move(Pos, true);
}

void character::ReceiveHeal(long Amount)
{
  if(Amount >= 100 && StateIsActivated(PARASITIZED))
    {
      if(IsPlayer())
	ADD_MESSAGE("Something in your belly didn't seem to like this stuff.");

      DeActivateTemporaryState(PARASITIZED);
      Amount -= 100;
    }

  ushort c;

  for(c = 0; c < Amount / 15; ++c)
    if(!HealHitPoint())
      break;

  Amount -= c * 15;

  if(Amount >= 250)
    {
      bodypart* NewBodyPart = GenerateRandomBodyPart();
      if(!NewBodyPart)
	return;
      NewBodyPart->SetHP(1);
      if(IsPlayer())
	ADD_MESSAGE("You grow a new %s.", NewBodyPart->GetBodyPartName().c_str()); 
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s grows a new %s.", CHAR_NAME(DEFINITE), NewBodyPart->GetBodyPartName().c_str());
    }
}

void character::AddHealingLiquidConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel better.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks healthier.", CHAR_NAME(DEFINITE));
}

void character::ReceiveSchoolFood(long SizeOfEffect)
{
  SizeOfEffect += RAND() % SizeOfEffect;

  if(SizeOfEffect >= 250)
    Vomit(SizeOfEffect / 250);

  if(!(RAND() % 3) && SizeOfEffect >= 500 && EditAttribute(ENDURANCE, SizeOfEffect / 500))
    {
      if(IsPlayer())
	ADD_MESSAGE("You gain a little bit of toughness for surviving this stuff.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks tougher.", CHAR_NAME(DEFINITE));
    }

  BeginTemporaryState(POISONED, (SizeOfEffect >> 1));
}

void character::AddSchoolFoodConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Yuck! This stuff tasted like vomit and old mousepads.");
}

void character::AddSchoolFoodHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Yuck! This stuff feels like vomit and old mousepads.");
}

void character::ReceiveNutrition(long SizeOfEffect)
{
  EditNP(SizeOfEffect);

  if(GetHungerState() == OVER_FED)
    {
      DeActivateVoluntaryAction("You are about to choke on this stuff.");
      Vomit(2 + RAND() % 3);
    }
}

void character::ReceiveOmmelUrine(long Amount)
{
  EditExperience(ARM_STRENGTH, Amount * 5);
  EditExperience(LEG_STRENGTH, Amount * 5);
  RestoreLivingHP();

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::AddOmmelUrineConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel a primitive force coursing through your veins.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s looks more powerful.", CHAR_NAME(DEFINITE));
}

void character::ReceivePepsi(long SizeOfEffect)
{
  ReceiveDamage(0, SizeOfEffect / 100, POISON, TORSO);
  EditExperience(PERCEPTION, SizeOfEffect << 3);

  if(CheckDeath("was poisoned by pepsi", 0))
    return;

  if(IsPlayer())
    game::DoEvilDeed(SizeOfEffect / 10);
}

void character::AddPepsiConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Urgh. You feel your guruism fading away.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks very lame.", CHAR_NAME(DEFINITE));
}

void character::ReceiveDarkness(long SizeOfEffect)
{
  EditExperience(INTELLIGENCE, -(SizeOfEffect << 3));
  EditExperience(WISDOM, -(SizeOfEffect << 3));
  EditExperience(CHARISMA, -(SizeOfEffect << 3));

  if(IsPlayer())
    game::DoEvilDeed(short(SizeOfEffect / 50));
}

void character::AddFrogFleshConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Arg. You feel the fate of a navastater placed upon you...");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s looks like a navastater.", CHAR_NAME(DEFINITE));
}

void character::ReceiveKoboldFlesh(long)
{
  /* As it is commonly known, the possibility of fainting per 500 cubic centimeters of kobold flesh is exactly 5%. */

  if(!(RAND() % 20))
    {
      if(IsPlayer())
	ADD_MESSAGE("You lose control of your legs and fall down.");

      Faint(250 + RAND() % 250);
    }
}

void character::AddKoboldFleshConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("This stuff tasted really funny.");
}

void character::AddKoboldFleshHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel very funny.");
}

void character::AddBoneConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel like a hippie.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s barks happily.", CHAR_NAME(DEFINITE)); // this suspects that nobody except dogs can eat bones
}

/* Returns true if character manages to unstuck himself (from all traps). vector2d is the direction which the character has tried to escape to. */

bool character::TryToUnstuck(vector2d Direction)
{
  return StuckTo->TryToUnstuck(this, StuckToBodyPart, Direction);
}

bool character::IsStuck() const
{
  return GetStuckToBodyPart() != NONE_INDEX;
}

bool character::CheckForAttributeIncrease(ushort& Attribute, long& Experience, bool DoubleAttribute)
{
  /* Check if attribute is disabled for the creature */

  if(!Attribute)
    return false;

  bool Effect = false;

  while(true)
    {
      if(!DoubleAttribute)
	{
	  if(Experience > long(Attribute) << 8)
	    {
	      if(Attribute < 100)
		{
		  Experience -= Attribute << 8;
		  Attribute += 1;
		  Effect = true;
		  continue;
		}
	    }
	}
      else
	{
	  if(Experience > long(Attribute) << 7)
	    {
	      if(Attribute < 200)
		{
		  Experience -= Attribute << 7;
		  Attribute += 1;
		  Effect = true;
		  continue;
		}
	    }
	}

      break;
    }

  return Effect;
}

bool character::CheckForAttributeDecrease(ushort& Attribute, long& Experience, bool DoubleAttribute)
{
  /* Check if attribute is disabled for the creature */

  if(!Attribute)
    return false;

  bool Effect = false;

  while(true)
    {
      if(!DoubleAttribute)
	{
	  if(Experience < (long(Attribute) - 100) << 10)
	    {
	      if(Attribute > 1)
		{
		  Experience += Max(long(100 - Attribute) << 10, 0L);
		  Attribute -= 1;
		  Effect = true;
		  continue;
		}
	    }
	}
      else
	{
	  if(Experience < (long(Attribute) - 200) << 9)
	    {
	      if(Attribute > 2)
		{
		  Experience += Max(long(200 - Attribute) << 9, 0L);
		  Attribute -= 1;
		  Effect = true;
		  continue;
		}
	    }
	}

      break;
    }

  return Effect;
}

bool character::RawEditAttribute(ushort& Attribute, short& Amount, bool DoubleAttribute) const
{
  /* Check if attribute is disabled for creature */

  if(!Attribute)
    return false;

  if(Amount < 0)
    {
      ushort Limit = DoubleAttribute ? 2 : 1;

      if(Attribute > 1)
	{
	  Attribute = Attribute > Limit - Amount ? Attribute + Amount : Limit;
	  return true;
	}
      else
	return false;
    }
  else
    {
      ushort Limit = DoubleAttribute ? 200 : 100;

      if(Attribute < Limit)
	{
	  Attribute = Attribute + Amount < Limit ? Attribute + Amount : Limit;
	  return true;
	}
      else
	return false;
    }
}

void character::DrawPanel(bool AnimationDraw) const
{
  if(AnimationDraw)
    {
      DrawStats(true);
      return;
    }

  DOUBLE_BUFFER->Fill(19 + (game::GetScreenSize().X << 4), 0, RES_X - 19 - (game::GetScreenSize().X << 4), RES_Y, 0);
  DOUBLE_BUFFER->Fill(16, 45 + (game::GetScreenSize().Y << 4), game::GetScreenSize().X << 4, 9, 0);
  FONT->Printf(DOUBLE_BUFFER, 16, 45 + (game::GetScreenSize().Y << 4), WHITE, "%s", GetPanelName().c_str());

  ushort PanelPosX = RES_X - 96;
  ushort PanelPosY = DrawStats(false);

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "End %d", GetAttribute(ENDURANCE));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Per %d", GetAttribute(PERCEPTION));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Int %d", GetAttribute(INTELLIGENCE));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Wis %d", GetAttribute(WISDOM));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Cha %d", GetAttribute(CHARISMA));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Siz %d", GetSize());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, IsInBadCondition() ? RED : WHITE, "HP %d/%d", GetHP(), GetMaxHP());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Gold: %d", GetMoney());
  ++PanelPosY;

  if(game::IsInWilderness())
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Worldmap");
  else
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "%s", festring::CapitalizeCopy(game::GetCurrentDungeon()->GetShortLevelDescription(game::GetCurrentLevelIndex())).c_str());

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Time: %d", game::GetTicks() / 10);

  ++PanelPosY;

  if(GetAction())
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "%s", festring::CapitalizeCopy(GetAction()->GetDescription()).c_str());

  for(ushort c = 0; c < STATES; ++c)
    if(!(StateData[c].Flags & SECRET) && StateIsActivated(1 << c) && (1 << c != HASTE || !StateIsActivated(SLOW)) && (1 << c != SLOW || !StateIsActivated(HASTE)))
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, (1 << c) & EquipmentState || TemporaryStateCounter[c] == PERMANENT ? BLUE : WHITE, "%s", StateData[c].Description);

  /* Make this more elegant!!! */

  if(GetHungerState() == STARVING)
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, RED, "Starving");
  else if(GetHungerState() == HUNGRY)
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, BLUE, "Hungry");
  else  if(GetHungerState() == SATIATED)
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Satiated");
  else if(GetHungerState() == BLOATED)
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Bloated");
  else if(GetHungerState() == OVER_FED)
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Overfed!");

  switch(GetBurdenState())
    {
    case OVER_LOADED:
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, RED, "Overload!");
      break;
    case STRESSED:
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, BLUE, "Stressed");
      break;
    case BURDENED:
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, BLUE, "Burdened");
    case UNBURDENED:
      break;
    }
}

void character::CalculateDodgeValue()
{
  DodgeValue = 0.05f * GetMoveEase() * GetAttribute(AGILITY) / sqrt(GetSize());

  if(CanFly())
    DodgeValue *= 2;

  if(DodgeValue < 1)
    DodgeValue = 1;
}

bool character::DamageTypeAffectsInventory(ushort Type)
{
  switch(Type&0xFFF)
    {
    case SOUND:
    case ENERGY:
    case ACID:
    case FIRE:
    case ELECTRICITY:
      return true;
    case PHYSICAL_DAMAGE:
    case POISON:
    case DRAIN:
      return false;
    default:
      ABORT("Unknown reaping effect destroyed dungeon!");
      return false;
    }
}

ushort character::CheckForBlockWithArm(character* Enemy, item* Weapon, arm* Arm, float WeaponToHitValue, ushort Damage, short Success, uchar Type)
{
  ushort BlockStrength = Arm->GetBlockCapability();
  float BlockValue = Arm->GetBlockValue();

  if(BlockStrength && BlockValue)
    {
      item* Blocker = Arm->GetWielded();

      if(RAND() % ushort(100 + WeaponToHitValue / BlockValue * (100 + Success)) < 100)
	{
	  ushort NewDamage = BlockStrength < Damage ? Damage - BlockStrength : 0;

	  switch(Type)
	    {
	    case UNARMED_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->UnarmedHitNoun(), NewDamage != 0);
	      break;
	    case WEAPON_ATTACK:
	      AddBlockMessage(Enemy, Blocker, "attack", NewDamage != 0);
	      break;
	    case KICK_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->KickNoun(), NewDamage != 0);
	      break;
	    case BITE_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->BiteNoun(), NewDamage != 0);
	      break;
	    }

	  Arm->EditExperience(ARM_STRENGTH, 25);
	  Arm->EditExperience(DEXTERITY, 50);
	  Blocker->WeaponSkillHit();
	  Blocker->ReceiveDamage(this, Damage, PHYSICAL_DAMAGE);

	  if(Weapon)
	    Weapon->ReceiveDamage(Enemy, Damage - NewDamage, PHYSICAL_DAMAGE);

	  return NewDamage;
	}
    }

  return Damage;
}

long character::GetStateAPGain(long BaseAPGain) const
{
  if(StateIsActivated(HASTE) == StateIsActivated(SLOW))
    return BaseAPGain;
  else if(StateIsActivated(HASTE))
    return (BaseAPGain * 5) >> 2;
  else
    return (BaseAPGain << 2) / 5;
}

void character::SignalEquipmentAdd(ushort EquipmentIndex)
{
  item* Equipment = GetEquipment(EquipmentIndex);

  if(Equipment->IsInCorrectSlot(EquipmentIndex))
    {
      ulong AddedStates = Equipment->GetGearStates();

      if(AddedStates)
	for(ushort c = 0; c < STATES; ++c)
	  if(AddedStates & (1 << c))
	    {
	      if(!StateIsActivated(1 << c))
		{
		  if(!InNoMsgMode)
		    (this->*StateData[c].PrintBeginMessage)();

		  EquipmentState |= 1 << c;

		  if(StateData[c].BeginHandler)
		    (this->*StateData[c].BeginHandler)();
		}
	      else
		EquipmentState |= 1 << c;
	    }
    }

  if(!Initializing && Equipment->IsInCorrectSlot(EquipmentIndex))
    ApplyEquipmentAttributeBonuses(Equipment);
}

void character::SignalEquipmentRemoval(ushort)
{
  CalculateEquipmentState();
  CalculateAttributeBonuses();
}

void character::CalculateEquipmentState()
{
  ushort c;
  ulong Back = EquipmentState;
  EquipmentState = 0;

  for(c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsInCorrectSlot(c))
      EquipmentState |= GetEquipment(c)->GetGearStates();

  for(c = 0; c < STATES; ++c)
    if(Back & (1 << c) && !StateIsActivated(1 << c))
      {
	if(StateData[c].EndHandler)
	  {
	    (this->*StateData[c].EndHandler)();

	    if(!IsEnabled())
	      return;
	  }

	if(!InNoMsgMode)
	  (this->*StateData[c].PrintEndMessage)();
      }
}

/* Counter = duration in ticks */

void character::BeginTemporaryState(ulong State, ushort Counter)
{
  if(!Counter)
    return;

  ushort  Index;

  if(State == POLYMORPHED)
    ABORT("No Polymorphing with BeginTemporaryState!");

  for(Index = 0; Index < STATES; ++Index)
    if(1UL << Index == State)
      break;

  if(Index == STATES)
    ABORT("BeginTemporaryState works only when State == 2 ^ n!");

  if(TemporaryStateIsActivated(State))
    {
      if(GetTemporaryStateCounter(State) != PERMANENT)
	EditTemporaryStateCounter(State, Counter);
    }
  else if(StateData[Index].IsAllowed == 0 || (this->*StateData[Index].IsAllowed)())
    {
      ActivateTemporaryState(State);
      SetTemporaryStateCounter(State, Counter);

      if(!EquipmentStateIsActivated(State))
	{
	  (this->*StateData[Index].PrintBeginMessage)();

	  if(StateData[Index].BeginHandler)
	    (this->*StateData[Index].BeginHandler)();
	}
    }
}

void character::HandleStates()
{
  if(!TemporaryState && !EquipmentState)
    return;

  for(ushort c = 0; c < STATES; ++c)
    {
      if(TemporaryState & (1 << c) && TemporaryStateCounter[c] != PERMANENT)
	{
	  if(!--TemporaryStateCounter[c])
	    {
	      TemporaryState &= ~(1 << c);

	      if(!(EquipmentState & (1 << c)))
		{
		  if(StateData[c].EndHandler)
		    {
		      (this->*StateData[c].EndHandler)();

		      if(!IsEnabled())
			return;
		    }

		  (this->*StateData[c].PrintEndMessage)();
		}
	    }
	}

      if(StateIsActivated(1 << c))
	if(StateData[c].Handler)
	  (this->*StateData[c].Handler)();

      if(!IsEnabled())
	return;
    }
}

void character::PrintBeginPolymorphControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your mind has total control over your body.");
}

void character::PrintEndPolymorphControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You are somehow uncertain of your willpower.");
}

void character::PrintBeginLifeSaveMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You hear Hell's gates being locked just now.");
}

void character::PrintEndLifeSaveMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel the Afterlife is welcoming you once again.");
}

void character::PrintBeginLycanthropyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly notice you've always loved full moons.");
}

void character::PrintEndLycanthropyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel the wolf inside you has had enough of your bad habits.");
}

void character::PrintBeginInvisibilityMessage() const
{
  if((PLAYER->StateIsActivated(INFRA_VISION) && IsWarm()) || (PLAYER->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
    {
      if(IsPlayer())
	ADD_MESSAGE("You seem somehow transparent.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s seems somehow transparent.", CHAR_NAME(DEFINITE));
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("You fade away.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s disappears!", CHAR_NAME(DEFINITE));
    }
}

void character::PrintEndInvisibilityMessage() const
{
  if((PLAYER->StateIsActivated(INFRA_VISION) && IsWarm()) || (PLAYER->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your notice your transparency has ended.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("The appearance of %s seems far more solid now.", CHAR_NAME(INDEFINITE));
    }
  else
    {
      if(IsPlayer())
	ADD_MESSAGE("You reappear.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s appears from nowhere!", CHAR_NAME(INDEFINITE));
    }
}

void character::PrintBeginInfraVisionMessage() const
{
  if(IsPlayer())
    if(StateIsActivated(INVISIBLE) && IsWarm() && !(StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
      ADD_MESSAGE("You reappear.");
    else
      ADD_MESSAGE("You feel your perception being magically altered.");
}

void character::PrintEndInfraVisionMessage() const
{
  if(IsPlayer())
    if(StateIsActivated(INVISIBLE) && IsWarm() && !(StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
      ADD_MESSAGE("You disappear.");
    else
      ADD_MESSAGE("You feel your perception returning to normal.");
}

void character::PrintBeginESPMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly feel like being only a tiny part of a great network of intelligent minds.");
}

void character::PrintEndESPMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You are filled with desire to be just yourself from now on.");
}

void character::PrintBeginHasteMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Time slows down to a crawl.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks faster!", CHAR_NAME(DEFINITE));
}

void character::PrintEndHasteMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Everything seems to move much faster now.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks slower!", CHAR_NAME(DEFINITE));
}

void character::PrintBeginSlowMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Everything seems to move much faster now.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks slower!", CHAR_NAME(DEFINITE));
}

void character::PrintEndSlowMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Time slows down to a crawl.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks faster!", CHAR_NAME(DEFINITE));
}

void character::EndPolymorph()
{
  if(IsPlayer())
    ADD_MESSAGE("You return to your true form.");
  else if(game::IsInWilderness())
    return; // fast gum solution, state ends when the player enters a dungeon
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s returns to %s true form.", CHAR_NAME(DEFINITE), GetPossessivePronoun().c_str());

  if(StuckToBodyPart != NONE_INDEX)
    {
      StuckToBodyPart = NONE_INDEX;
      StuckTo = 0;
    }

  if(GetAction())
    GetAction()->Terminate(false);

  SendToHell();
  GetSquareUnder()->RemoveCharacter();
  character* Char = GetPolymorphBackup();
  InNoMsgMode = Char->InNoMsgMode = true;
  SetPolymorphBackup(0);
  GetSquareUnder()->AddCharacter(Char);
  Char->Enable();
  Char->SetPolymorphed(false);
  GetStack()->MoveItemsTo(Char->GetStack());

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Item = GetEquipment(c);

      if(Item)
	{
	  if(Char->CanUseEquipment(c))
	    {
	      Item->RemoveFromSlot();
	      Char->SetEquipment(c, Item);
	    }
	  else
	    Item->MoveTo(IsPlayer() ? Char->GetStack() : Char->GetStackUnder());
	}
    }

  Char->SetMoney(GetMoney());
  Char->ChangeTeam(GetTeam());

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(Char);

  SetSquareUnder(0);
  InNoMsgMode = Char->InNoMsgMode = false;
  Char->CalculateAll();
  Char->SetAssignedName(GetAssignedName());

  if(IsPlayer())
    {
      game::SetPlayer(Char);
      game::SendLOSUpdateRequest();
      UpdateESPLOS();
    }

  Char->TestWalkability();
}

void character::LycanthropyHandler()
{
  if(!(RAND() % 2000))
    Polymorph(new werewolfwolf, 1000 + RAND() % 2000);
}

void character::SaveLife()
{
  if(TemporaryStateIsActivated(LIFE_SAVED))
    {
      if(IsPlayer())
	ADD_MESSAGE("But wait! You glow briefly red and seem to be in a better shape!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("But wait, suddenly %s glows briefly red and seems to be in a better shape!", GetPersonalPronoun().c_str());

      DeActivateTemporaryState(LIFE_SAVED);
    }
  else
    {
      item* LifeSaver = 0;

      for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	if(GetEquipment(c) && GetEquipment(c)->IsInCorrectSlot(c) && GetEquipment(c)->GetGearStates() & LIFE_SAVED)
	  LifeSaver = GetEquipment(c);

      if(!LifeSaver)
	ABORT("The Universe can only kill you once!");

      if(IsPlayer())
	ADD_MESSAGE("But wait! Your %s glows briefly red and disappears and you seem to be in a better shape!", LifeSaver->CHAR_NAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("But wait, suddenly %s %s glows briefly red and disappears and %s seems to be in a better shape!", GetPossessivePronoun().c_str(), LifeSaver->CHAR_NAME(UNARTICLED), GetPersonalPronoun().c_str());

      LifeSaver->RemoveFromSlot();
      LifeSaver->SendToHell();
    }

  if(IsPlayer())
    game::AskForKeyPress("Life saved! [press any key to continue]");

  RestoreBodyParts();
  RestoreHP();
  ResetStates();

  if(GetNP() < SATIATED_LEVEL)
    SetNP(SATIATED_LEVEL);

  GetSquareUnder()->SendNewDrawRequest();
}

character* character::PolymorphRandomly(ushort MinDanger, ushort MaxDanger, ushort Time)
{
  character* NewForm = 0;

  if(StateIsActivated(POLYMORPH_CONTROL))
    {
      std::string Topic, Temp;

      if(IsPlayer())
	{
	  while(!NewForm)
	    {
	      Topic = "What do you want to become?";

	      if(game::GetDefaultPolymorphTo() != "")
		Topic << " [" << game::GetDefaultPolymorphTo() << ']';

	      Temp = game::StringQuestion(Topic, vector2d(16, 6), WHITE, 0, 80, false);

	      if(Temp == "")
		Temp = game::GetDefaultPolymorphTo();

	      NewForm = protosystem::CreateMonster(Temp, NO_EQUIPMENT);
	    }

	  game::SetDefaultPolymorphTo(Temp);
	}
      else
	NewForm = protosystem::CreateMonster(MinDanger * 10, MaxDanger * 10, NO_EQUIPMENT);
    }
  else
    NewForm = protosystem::CreateMonster(MinDanger, MaxDanger, NO_EQUIPMENT);

  Polymorph(NewForm, Time);
  return NewForm;
}

/* In reality, the reading takes Time / (Intelligence * 10) turns */

void character::StartReading(item* Item, ulong Time)
{
  read* Read = new read(this);
  Read->SetLiterature(Item); // slot cleared automatically

  if(game::WizardModeIsActive())
    Time = 1;

  Read->SetCounter(Time);
  SetAction(Read);

  if(IsPlayer())
    ADD_MESSAGE("You start reading %s.", Item->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s starts reading %s.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(DEFINITE));
}

/*
 * Call when one makes something with his/her/its hands.
 * Difficulty of 5 takes about one turn, so it's the most common to use.
 */

void character::DexterityAction(ushort Difficulty)
{
  EditAP(-20000 * Difficulty / APBonus(GetAttribute(DEXTERITY)));
  EditExperience(DEXTERITY, 5 * Difficulty);
}

/* If Theoretically == true, range is not a factor. */

bool character::CanBeSeenByPlayer(bool Theoretically, bool IgnoreESP) const
{
  bool MayBeESPSeen = PLAYER->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5;
  bool MayBeInfraSeen = PLAYER->StateIsActivated(INFRA_VISION) && IsWarm();
  bool Visible = !StateIsActivated(INVISIBLE) || MayBeESPSeen || MayBeInfraSeen;

  if((game::IsInWilderness() && Visible) || (!IgnoreESP && MayBeESPSeen && (Theoretically || (GetPos() - PLAYER->GetPos()).GetLengthSquare() <= PLAYER->GetESPRangeSquare())))
    return true;
  else if(!Visible)
    return false;
  else
    return Theoretically || GetSquareUnder()->CanBeSeenByPlayer(MayBeInfraSeen || MayBeESPSeen) || ((MayBeInfraSeen || MayBeESPSeen) && (GetPos() - PLAYER->GetPos()).GetLengthSquare() <= PLAYER->GetLOSRangeSquare() && femath::DoLine(PLAYER->GetPos().X, PLAYER->GetPos().Y, GetPos().X, GetPos().Y, game::EyeHandler));
}

bool character::CanBeSeenBy(const character* Who, bool Theoretically, bool IgnoreESP) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer(Theoretically, IgnoreESP);
  else
    {
      bool MayBeESPSeen = Who->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5;
      bool MayBeInfraSeen = Who->StateIsActivated(INFRA_VISION) && IsWarm();
      bool Visible = !StateIsActivated(INVISIBLE) || MayBeESPSeen || MayBeInfraSeen;

      if((game::IsInWilderness() && Visible) || (!IgnoreESP && MayBeESPSeen && (Theoretically || (GetPos() - Who->GetPos()).GetLengthSquare() <= Who->GetESPRangeSquare())))
	return true;
      else if(!Visible)
	return false;
      else
	return Theoretically || GetSquareUnder()->CanBeSeenFrom(Who->GetPos(), Who->GetLOSRangeSquare(), MayBeESPSeen || MayBeInfraSeen);
    }
}

void character::AttachBodyPart(bodypart* BodyPart)
{
  SetBodyPart(BodyPart->GetBodyPartIndex(), BodyPart);
  BodyPart->ResetSpoiling();
  BodyPart->ResetPosition();
  BodyPart->UpdatePictures();
  CalculateAttributeBonuses();
  CalculateBattleInfo();
  GetSquareUnder()->SendNewDrawRequest();
}

/* Returns true if the character has all bodyparts, false if not. */ 

bool character::HasAllBodyParts() const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      return false;

  return true;
}

bodypart* character::GenerateRandomBodyPart()
{
  std::vector<ushort> NeededBodyParts;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      NeededBodyParts.push_back(c);

  if(NeededBodyParts.empty())
    return 0;

  return CreateBodyPart(NeededBodyParts[RAND() % NeededBodyParts.size()]);
}

/* searched for character's Stack and if it find some bodyparts there that are the character's old bodyparts returns a stackiterator to one of them (choocen in random). If no fitting bodyparts are found the function returns 0 */

bodypart* character::FindRandomOwnBodyPart(bool AllowNonLiving) const
{
  itemvector LostAndFound;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c))
      for(std::list<ulong>::iterator i = OriginalBodyPartID[c].begin(); i != OriginalBodyPartID[c].end(); ++i)
	{
	  bodypart* Found = static_cast<bodypart*>(SearchForItemWithID(*i));

	  if(Found && (AllowNonLiving || Found->IsAlive()))
	    LostAndFound.push_back(Found);
	}

  if(LostAndFound.empty())
    return 0;
  else
    return static_cast<bodypart*>(LostAndFound[RAND() % LostAndFound.size()]);
}

void character::PrintBeginPoisonedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You seem to be very ill.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks very ill.", CHAR_NAME(DEFINITE));
}

void character::PrintEndPoisonedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel better again.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks better.", CHAR_NAME(DEFINITE)); 
}

void character::PoisonedHandler()
{
  if(!(RAND() % 100))
    Vomit(2 + (RAND() & 1));

  ushort Damage = 0;

  for(ushort Used = 0; Used < GetTemporaryStateCounter(POISONED); Used += 100)
    if(!(RAND() % 100))
      ++Damage;

  if(Damage)
    {
      ReceiveDamage(0, Damage, POISON, ALL, 8, false, false, false, false);
      CheckDeath("died of acute poisoning", 0);
    }
}

bool character::IsWarm() const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->IsAlive())
      return true;

  return false;
}

void character::BeginInvisibility()
{
  UpdatePictures();

  if(GetSquareUnder())
    GetSquareUnder()->SendNewDrawRequest();
}

void character::BeginInfraVision()
{
  if(IsPlayer())
    GetArea()->SendNewDrawRequest();
}

void character::BeginESP()
{
  if(IsPlayer())
    GetArea()->SendNewDrawRequest();
}

void character::EndInvisibility()
{
  UpdatePictures();
  GetSquareUnder()->SendNewDrawRequest();
}

void character::EndInfraVision()
{
  if(IsPlayer())
    GetArea()->SendNewDrawRequest();
}

void character::EndESP()
{
  if(IsPlayer())
    GetArea()->SendNewDrawRequest();
}

void character::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAnimate) const
{
  if((PLAYER->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5 && (PLAYER->GetPos() - GetPos()).GetLengthSquare() <= PLAYER->GetESPRangeSquare()) || (PLAYER->StateIsActivated(INFRA_VISION) && IsWarm()))
    Luminance = configuration::GetContrastLuminance();

  DrawBodyParts(Bitmap, Pos, Luminance, AllowAnimate);

  if(configuration::GetOutlineCharacters())
    {
      igraph::GetTileBuffer()->ClearToColor(TRANSPARENT_COLOR);
      DrawBodyParts(igraph::GetTileBuffer(), vector2d(0, 0), NORMAL_LUMINANCE, AllowAnimate);
      igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetCharacterOutlineColor());
      igraph::GetOutlineBuffer()->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, configuration::GetContrastLuminance());
    }
  
  if(GetTeam() == PLAYER->GetTeam() && !IsPlayer())
    igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, 32, 16, Pos, 16, 16, configuration::GetContrastLuminance());

  if(CanFly())
    igraph::GetSymbolGraphic()->MaskedBlit(Bitmap, 128, 16, Pos, 16, 16, configuration::GetContrastLuminance());
}

void character::DrawBodyParts(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAnimate, bool AllowAlpha) const
{
  GetTorso()->Draw(Bitmap, Pos, Luminance, AllowAnimate, AllowAlpha);
}

void character::PrintBeginTeleportMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel jumpy.");
}

void character::PrintEndTeleportMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly realize you've always preferred walking to jumping.");
}

void character::TeleportHandler()
{
  if(!(RAND() % 1500) && !game::IsInWilderness())
    TeleportRandomly();
}

void character::PrintBeginPolymorphMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("An unconfortable uncertainty of who you really are overwhelms you.");
}

void character::PrintEndPolymorphMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel you are you and no one else.");
}

void character::PolymorphHandler()
{
  if(!(RAND() % 1500))
    PolymorphRandomly(1, 9999, 200 + RAND() % 800);
}

void character::PrintBeginTeleportControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel very controlled.");
}

void character::PrintEndTeleportControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your control slipping.");
}

void character::DisplayStethoscopeInfo(character*) const
{
  felist Info("Information about " + GetName(DEFINITE));
  AddSpecialStethoscopeInfo(Info);
  Info.AddEntry(std::string("Endurance: ") + GetAttribute(ENDURANCE), LIGHT_GRAY);
  Info.AddEntry(std::string("Perception: ") + GetAttribute(PERCEPTION), LIGHT_GRAY);
  Info.AddEntry(std::string("Intelligence: ") + GetAttribute(INTELLIGENCE), LIGHT_GRAY);
  Info.AddEntry(std::string("Wisdom: ") + GetAttribute(WISDOM), LIGHT_GRAY);
  Info.AddEntry(std::string("Charisma: ") + GetAttribute(CHARISMA), LIGHT_GRAY);
  Info.AddEntry(std::string("HP: ") + GetHP() + "/" + GetMaxHP(), IsInBadCondition() ? RED : LIGHT_GRAY);
  game::SetStandardListAttributes(Info);
  Info.Draw();
}

bool character::CanUseStethoscope(bool PrintReason) const
{
  if(PrintReason)
    ADD_MESSAGE("This type of monster can't use a stethoscope.");

  return false;
}

/* 
 * Effect used by atleast Sophos. 
 * NOTICE: Doesn't check for death! 
 */

void character::TeleportSomePartsAway(ushort NumberToTeleport)
{
  for(ushort c = 0; c < NumberToTeleport; ++c)
    {
      ushort RandomBodyPart = GetRandomNonVitalBodyPart();

      if(RandomBodyPart == NONE_INDEX)
	{
	  for(; c < NumberToTeleport; ++c)
	    {
	      GetTorso()->SetHP((GetTorso()->GetHP() << 2) / 5);
	      ulong TorsosVolume = GetTorso()->GetMainMaterial()->GetVolume() / 10;

	      if(TorsosVolume == 0)
		break;
	      
	      ulong Amount = (RAND() % TorsosVolume) + 1;
	      lump* Lump = new lump(0, NO_MATERIALS); 
	      Lump->InitMaterials(GetTorso()->GetMainMaterial()->Clone(Amount));
	      GetTorso()->GetMainMaterial()->SetVolume(GetTorso()->GetMainMaterial()->GetVolume() - Amount);
	      Lump->MoveTo(GetNearLSquare(GetLevel()->GetRandomSquare())->GetStack());

	      if(IsPlayer())
		ADD_MESSAGE("Parts of you teleport away.");
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("Parts of %s teleport away.", CHAR_NAME(DEFINITE));
	    }
	}
      else
	{
	  item* SeveredBodyPart = SevereBodyPart(RandomBodyPart);

	  if(SeveredBodyPart)
	    {
	      GetNearLSquare(GetLevel()->GetRandomSquare())->AddItem(SeveredBodyPart);
	      SeveredBodyPart->DropEquipment();

	      if(IsPlayer())
		ADD_MESSAGE("Your %s teleports away.", GetBodyPartName(RandomBodyPart).c_str());
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s %s teleports away.", GetPossessivePronoun().c_str(), GetBodyPartName(RandomBodyPart).c_str());
	    }
	  else
	    {
	      if(IsPlayer())
		ADD_MESSAGE("Your %s disappears.", GetBodyPartName(RandomBodyPart).c_str());
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s %s dissappears.", GetPossessivePronoun().c_str(), GetBodyPartName(RandomBodyPart).c_str());
	    }
	}	
    }
}

/* Returns an index of a random bodypart that is not vital. If no non-vital bodypart is found returns NONE_INDEX */

ushort character::GetRandomNonVitalBodyPart()
{
  ushort OKBodyPart[MAX_BODYPARTS];
  ushort OKBodyParts = 0;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && !BodyPartIsVital(c))
      OKBodyPart[OKBodyParts++] = c;

  return OKBodyParts ? OKBodyPart[RAND() % OKBodyParts] : NONE_INDEX;
}

void character::CalculateVolumeAndWeight()
{
  Volume = Stack->GetVolume();
  Weight = Stack->GetWeight();

  if(Action)
    {
      Volume += Action->GetVolume();
      Weight += Action->GetWeight();
    }

  BodyVolume = 0;
  CarriedWeight = Weight;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      {
	BodyVolume += GetBodyPart(c)->GetBodyPartVolume();
	Volume += GetBodyPart(c)->GetVolume();
	CarriedWeight += GetBodyPart(c)->GetCarriedWeight();
	Weight += GetBodyPart(c)->GetWeight();
      }
}

void character::SignalVolumeAndWeightChange()
{
  if(!Initializing)
    {
      CalculateVolumeAndWeight();

      if(IsEnabled())
	CalculateBurdenState();

      if(MotherEntity)
	MotherEntity->SignalVolumeAndWeightChange();
    }
}

void character::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
    {
      game::CombineLights(Emitation, EmitationUpdate);

      if(MotherEntity)
	MotherEntity->SignalEmitationIncrease(EmitationUpdate);
      else if(SquareUnder && !game::IsInWilderness())
	GetLSquareUnder()->SignalEmitationIncrease(EmitationUpdate);
    }
}

void character::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
    {
      ulong Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation)
	{
	  if(MotherEntity)
	    MotherEntity->SignalEmitationDecrease(EmitationUpdate);
	  else if(SquareUnder && !game::IsInWilderness())
	    GetLSquareUnder()->SignalEmitationDecrease(EmitationUpdate);
	}
    }
}

void character::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      game::CombineLights(Emitation, GetBodyPart(c)->GetEmitation());

  game::CombineLights(Emitation, Stack->GetEmitation());

  if(Action)
    game::CombineLights(Emitation, Action->GetEmitation());
}

void character::CalculateAll()
{
  Initializing = true;
  CalculateAttributeBonuses();
  CalculateVolumeAndWeight();
  CalculateEmitation();
  CalculateBodyPartMaxHPs(false);
  CalculateBurdenState();
  CalculateBattleInfo();
  Initializing = false;
}

void character::CalculateHP()
{
  HP = 0;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      HP += GetBodyPart(c)->GetHP();
}

void character::CalculateMaxHP()
{
  MaxHP = 0;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      MaxHP += GetBodyPart(c)->GetMaxHP();
}

void character::CalculateBodyPartMaxHPs(bool MayChangeHPs)
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->CalculateMaxHP(MayChangeHPs);

  CalculateMaxHP();
  CalculateHP();
}

bool character::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier == ENDURANCE && !IsAlive())
    return false;

  if(RawEditAttribute(BaseAttribute[Identifier], Value))
    {
      if(!Initializing)
	{
	  if(Identifier == LEG_STRENGTH)
	    CalculateBurdenState();
	  else if(Identifier == ENDURANCE)
	    CalculateBodyPartMaxHPs();
	  else if(Identifier == PERCEPTION && IsPlayer())
	    game::SendLOSUpdateRequest();

	  CalculateBattleInfo();
	}

      return true;
    }
  else
    return false;
}

void character::EditExperience(ushort Identifier, long Value)
{
  if(Identifier == ENDURANCE && !IsAlive())
    return;

  BaseExperience[Identifier] += Value;
}

bool character::ActivateRandomState(ushort Flags, ushort Time, ulong Seed)
{
  femath::SaveSeed();

  if(Seed)
    femath::SetSeed(Seed);

  ulong ToBeActivated = GetRandomState(Flags|DUR_TEMPORARY);
  femath::LoadSeed();

  if(!ToBeActivated)
    return false;

  BeginTemporaryState(ToBeActivated, Time);
  return true;
}

bool character::GainRandomIntrinsic(ushort Flags)
{
  ulong ToBeActivated = GetRandomState(Flags|DUR_PERMANENT);

  if(!ToBeActivated)
    return false;

  GainIntrinsic(ToBeActivated);
  return true;
}

/*
 * Returns 0 if state not found
 */

ulong character::GetRandomState(ushort Flags) const
{
  ulong OKStates[STATES];
  ushort NumberOfOKStates = 0;

  for(ushort c = 0; c < STATES; ++c)
    if(StateData[c].Flags & Flags & DUR_FLAGS && StateData[c].Flags & Flags & SRC_FLAGS)
      OKStates[NumberOfOKStates++] = 1 << c;
  
  return NumberOfOKStates ? OKStates[RAND() % NumberOfOKStates] : 0;
}

void characterprototype::CreateSpecialConfigurations()
{
  if(Config.begin()->second.CreateGolemMaterialConfigurations)
    for(ushort c = 1; c < protocontainer<material>::GetProtoAmount(); ++c)
      {
	const material::databasemap& MaterialConfig = protocontainer<material>::GetProto(c)->GetConfig();

	for(material::databasemap::const_iterator i = MaterialConfig.begin(); i != MaterialConfig.end(); ++i)
	  if(i->first && i->second.IsGolemMaterial && Config.find(i->first) == Config.end())
	    {
	      character::database TempDataBase(Config.begin()->second);
	      TempDataBase.InitDefaults(i->first);
	      TempDataBase.Adjective = i->second.NameStem;
	      TempDataBase.AdjectiveArticle = i->second.Article;
	      TempDataBase.AttachedGod = i->second.AttachedGod;
	      Config.insert(std::pair<ushort, character::database>(i->first, TempDataBase));
	    }
      }
}

float character::GetTimeToDie(const character* Enemy, ushort Damage, float ToHitValue, bool AttackIsBlockable, bool UseMaxHP) const
{
  float DodgeValue = GetDodgeValue();

  if(!Enemy->CanBeSeenBy(this, true))
    ToHitValue *= 2;

  if(!CanBeSeenBy(Enemy, true))
    DodgeValue *= 2;

  float MinHits = 1000;
  bool First = true;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(BodyPartIsVital(c) && GetBodyPart(c))
      {
	float Hits = GetBodyPart(c)->GetTimeToDie(Damage, ToHitValue, DodgeValue, AttackIsBlockable, UseMaxHP);

	if(First)
	  {
	    MinHits = Hits;
	    First = false;
	  }
	else
	  MinHits = 1 / (1 / MinHits + 1 / Hits);
      }

  return MinHits;
}

float character::GetRelativeDanger(const character* Enemy, bool UseMaxHP) const
{
  float Danger = Enemy->GetTimeToKill(this, UseMaxHP) / GetTimeToKill(Enemy, UseMaxHP);

  if(!Enemy->CanBeSeenBy(this, true))
    Danger *= 0.25f;

  if(!CanBeSeenBy(Enemy, true))
    Danger *= 4.0f;

  return Limit(Danger, -100.0f, 100.0f);
}

std::string character::GetBodyPartName(ushort Index, bool Articled) const
{
  if(Index == TORSO_INDEX)
    return Articled ? "a torso" : "torso";
  else
    {
      ABORT("Illegal character bodypart name request!");
      return 0;
    }
}

item* character::SearchForItemWithID(ulong ID) const
{
  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->GetID() == ID)
      return GetEquipment(c);

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->GetID() == ID)
      return *i;

  return 0;
}

bool character::ContentsCanBeSeenBy(const character* Viewer) const
{
  return Viewer == this;
}

bool character::HitEffect(character* Enemy, item* Weapon, uchar Type, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  if(Weapon)
    return Weapon->HitEffect(this, Enemy, BodyPartIndex, Direction, BlockedByArmour);

  switch(Type)
    {
    case UNARMED_ATTACK:
      return Enemy->SpecialUnarmedEffect(this, BodyPartIndex, Direction, BlockedByArmour);
    case KICK_ATTACK:
      return Enemy->SpecialKickEffect(this, BodyPartIndex, Direction, BlockedByArmour);
    case BITE_ATTACK:
      return Enemy->SpecialBiteEffect(this, BodyPartIndex, Direction, BlockedByArmour);
    }

  return false;
}

void character::WeaponSkillHit(item* Weapon, uchar Type)
{
  ushort Category;

  switch(Type)
    {
    case UNARMED_ATTACK:
      Category = UNARMED;
      break;
    case WEAPON_ATTACK:
      Weapon->WeaponSkillHit();
      return;
    case KICK_ATTACK:
      Category = KICK;
      break;
    case BITE_ATTACK:
      Category = BITE;
      break;
    case THROW_ATTACK:
      if(!IsHumanoid()) return;
      Category = Weapon->GetWeaponCategory();
      break;
    default:
      ABORT("Illegal Type %d passed to character::WeaponSkillHit()!", Type);
      return;
    }

  if(GetCWeaponSkill(Category)->AddHit())
    {
      CalculateBattleInfo();

      if(IsPlayer())
	GetCWeaponSkill(Category)->AddLevelUpMessage();
    }
}

void character::AddDefenceInfo(felist& List) const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      {
	std::string Entry = "   ";
	Entry << GetBodyPart(c)->GetBodyPartName();
	Entry.resize(60, ' ');
	Entry << GetBodyPart(c)->GetMaxHP();
	Entry.resize(70, ' ');
	Entry << GetBodyPart(c)->GetTotalResistance(PHYSICAL_DAMAGE);
	List.AddEntry(Entry, LIGHT_GRAY);
      }
}

/* returns 0 if character cannot be cloned */

character* character::Duplicate() const
{
  if(!CanBeCloned())
    return 0;

  character* Char = RawDuplicate();
  Char->CalculateAll();
  Char->UpdatePictures();
  return Char;
}

bool character::TryToEquip(item* Item)
{
  if(!CanUseEquipment() || GetAttribute(WISDOM) >= Item->GetWearWisdomLimit())
    return false;

  for(ushort e = 0; e < GetEquipmentSlots(); ++e)
    if(GetBodyPartOfEquipment(e) && (!EquipmentSorter(e) || EquipmentSorter(e)(Item, this)))
      {
	item* OldEquipment = GetEquipment(e);

	if(BoundToUse(OldEquipment, e))
	  continue;

	msgsystem::DisableMessages();
	float Danger = GetRelativeDanger(PLAYER);

	if(OldEquipment)
	  OldEquipment->RemoveFromSlot();

	Item->RemoveFromSlot();
	SetEquipment(e, Item);
	float NewDanger = GetRelativeDanger(PLAYER);
	Item->RemoveFromSlot();
	GetStackUnder()->AddItem(Item);

	if(OldEquipment)
	  SetEquipment(e, OldEquipment);

	msgsystem::EnableMessages();

	if(OldEquipment)
	  {
	    if(NewDanger > Danger || BoundToUse(Item, e))
	      {
		if(!GetRoom() || GetRoom()->PickupItem(this, Item, 1))
		  {
		    if(CanBeSeenByPlayer())
		      ADD_MESSAGE("%s drops %s %s and equips %s instead.", CHAR_NAME(DEFINITE), CHAR_POSSESSIVE_PRONOUN, OldEquipment->CHAR_NAME(UNARTICLED), Item->CHAR_NAME(INDEFINITE));

		    if(GetRoom())
		      GetRoom()->DropItem(this, OldEquipment, 1);

		    OldEquipment->MoveTo(GetStackUnder());
		    Item->RemoveFromSlot();
		    SetEquipment(e, Item);
		    DexterityAction(5);
		    return true;
		  }
	      }
	  }
	else
	  {
	    if(NewDanger > Danger || (NewDanger == Danger && e != RIGHT_WIELDED_INDEX && e != LEFT_WIELDED_INDEX) || BoundToUse(Item, e))
	      {
		if(!GetRoom() || GetRoom()->PickupItem(this, Item, 1))
		  {
		    if(CanBeSeenByPlayer())
		      ADD_MESSAGE("%s picks up and equips %s.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(INDEFINITE));

		    Item->RemoveFromSlot();
		    SetEquipment(e, Item);
		    DexterityAction(5);
		    return true;
		  }
	      }
	  }
      }

  return false;
}

bool character::TryToConsume(item* Item)
{
  if(Item->IsConsumable(this) && Item->CanBeEatenByAI(this) && (!GetRoom() || GetRoom()->ConsumeItem(this, Item, 1)))
    {
      if(Item->IsStupidToConsume())
	EditExperience(WISDOM, 500);

      ConsumeItem(Item);
      return true;
    }
  else
    return false;
}

void character::UpdateESPLOS() const
{
  if(StateIsActivated(ESP) && !game::IsInWilderness())
    for(ushort c = 0; c < game::GetTeams(); ++c)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
	  (*i)->GetSquareUnder()->SendNewDrawRequest();
}

uchar character::GetCWeaponSkillLevel(const item* Item) const
{
  if(Item->GetWeaponCategory() < GetAllowedWeaponSkillCategories())
    return GetCWeaponSkill(Item->GetWeaponCategory())->GetLevel();
  else
    return 0;
}

void character::PrintBeginPanicMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You panic!");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s panics.", CHAR_NAME(DEFINITE));  
}

void character::PrintEndPanicMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You finally calm down.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s calms down.", CHAR_NAME(DEFINITE));
}

void character::CheckPanic(ushort Ticks)
{
  if(GetHP() * 100 < GetPanicLevel() * GetMaxHP())
    BeginTemporaryState(PANIC, ((Ticks * 3) >> 2) + RAND() % ((Ticks >> 1) + 1)); // 25% randomness to ticks...
}

/* returns 0 if fails else the newly created character */

character* character::CloneToNearestSquare(character*) const
{
  vector2d Where = GetLevel()->GetNearestFreeSquare(this, GetPos());

  if(Where == ERROR_VECTOR)
    {
      ADD_MESSAGE("You sense a disturbance in the Force.");
      return 0;
    }

  character* NewlyCreated = Duplicate();

  if(!NewlyCreated)
    return 0;
  
  GetLevel()->GetLSquare(Where)->AddCharacter(NewlyCreated);
  return NewlyCreated;
}

void character::SignalSpoil()
{
  /* Add support for spoiling zombies! */

  if(GetMotherEntity())
    GetMotherEntity()->SignalSpoil(0);
}

bool character::CanHeal() const
{
  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->IsAlive() && BodyPart->GetHP() < BodyPart->GetMaxHP())
	return true;
    }

  return false;
}

void character::ReceiveFluidSpill(material* Liquid, ushort HitPercent)
{
  Liquid->Effect(this, Liquid->GetVolume() * HitPercent / 100);
  GetStack()->ReceiveFluidSpill(Liquid);
}

uchar character::GetRelation(const character* Who) const
{
  return GetTeam()->GetRelation(Who->GetTeam());
}

void character::CalculateAttributeBonuses()
{
  ushort c;

  for(c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->CalculateAttributeBonuses();

  short BackupBonus[BASE_ATTRIBUTES];
  short BackupCarryingBonus = CarryingBonus;
  CarryingBonus = 0;
  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    {
      BackupBonus[c] = AttributeBonus[c];
      AttributeBonus[c] = 0;
    }

  for(c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(!Equipment || !Equipment->IsInCorrectSlot(c))
	continue;

      if(Equipment->AffectsEndurance())
	AttributeBonus[ENDURANCE] += Equipment->GetEnchantment();

      if(Equipment->AffectsPerception())
	AttributeBonus[PERCEPTION] += Equipment->GetEnchantment();

      if(Equipment->AffectsIntelligence())
	AttributeBonus[INTELLIGENCE] += Equipment->GetEnchantment();

      if(Equipment->AffectsWisdom())
	AttributeBonus[WISDOM] += Equipment->GetEnchantment();

      if(Equipment->AffectsCharisma())
	AttributeBonus[CHARISMA] += Equipment->GetEnchantment();

      if(Equipment->AffectsMana())
	AttributeBonus[MANA] += Equipment->GetEnchantment();
      
      if(Equipment->AffectsCarryingCapacity())
	CarryingBonus += Equipment->GetCarryingBonus();
    }

    if(!Initializing && AttributeBonus[ENDURANCE] != BackupBonus[ENDURANCE])
      CalculateBodyPartMaxHPs();

    if(IsPlayer() && !Initializing && AttributeBonus[PERCEPTION] != BackupBonus[PERCEPTION])
      game::SendLOSUpdateRequest();

    if(IsPlayer() && !Initializing && AttributeBonus[INTELLIGENCE] != BackupBonus[INTELLIGENCE])
      UpdateESPLOS();

    if(!Initializing && CarryingBonus != BackupCarryingBonus)
      CalculateBurdenState();
}

void character::ApplyEquipmentAttributeBonuses(item* Equipment)
{
  if(Equipment->AffectsEndurance())
    {
      AttributeBonus[ENDURANCE] += Equipment->GetEnchantment();
      CalculateBodyPartMaxHPs();
    }

  if(Equipment->AffectsPerception())
    {
      AttributeBonus[PERCEPTION] += Equipment->GetEnchantment();

      if(IsPlayer())
	game::SendLOSUpdateRequest();
    }

  if(Equipment->AffectsIntelligence())
    {
      AttributeBonus[INTELLIGENCE] += Equipment->GetEnchantment();

      if(IsPlayer())
	UpdateESPLOS();
    }

  if(Equipment->AffectsWisdom())
    AttributeBonus[WISDOM] += Equipment->GetEnchantment();

  if(Equipment->AffectsCharisma())
    AttributeBonus[CHARISMA] += Equipment->GetEnchantment();

  if(Equipment->AffectsMana())
    AttributeBonus[MANA] += Equipment->GetEnchantment();

  if(Equipment->AffectsCarryingCapacity())
    {
      CarryingBonus += Equipment->GetCarryingBonus();
      CalculateBurdenState();
    }
}

void character::ReceiveAntidote(long Amount)
{
  if(StateIsActivated(POISONED))
    {
      if(GetTemporaryStateCounter(POISONED) > Amount)
	{
	  EditTemporaryStateCounter(POISONED, -Amount);
	  Amount = 0;
	}
      else
	{
	  if(IsPlayer())
	    ADD_MESSAGE("Aaaah... You feel much better.");

	  Amount -= GetTemporaryStateCounter(POISONED);
	  DeActivateTemporaryState(POISONED);
	}
    }

  if(Amount >= 100 && StateIsActivated(PARASITIZED))
    {
      if(IsPlayer())
	ADD_MESSAGE("Something in your belly didn't seem to like this stuff.");

      DeActivateTemporaryState(PARASITIZED);
      Amount -= 100;
    }
}

void character::AddAntidoteConsumeEndMessage() const
{
  if(StateIsActivated(POISONED)) /* true only if the poison didn't cure the poison completely */
				 /* Comment: Whaaat? */
    {
      if(IsPlayer())
	ADD_MESSAGE("Your body processes the poison in your veins with rapid speed.");
    }
}

bool character::IsDead() const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(BodyPartIsVital(c) && (!GetBodyPart(c) || GetBodyPart(c)->GetHP() < 1))
      return true;

  return false;
}

void character::SignalSpoilLevelChange()
{
  /* Add support for spoiling zombies! */

  if(GetMotherEntity())
    GetMotherEntity()->SignalSpoilLevelChange(0);
}

void character::AddOriginalBodyPartID(ushort Index, ulong What)
{
  OriginalBodyPartID[Index].push_back(What);

  if(OriginalBodyPartID[Index].size() > 100)
    OriginalBodyPartID[Index].erase(OriginalBodyPartID[Index].begin());
}

void character::AddToInventory(const std::list<contentscript<item> >& ItemList, ushort SpecialFlags)
{
  for(std::list<contentscript<item> >::const_iterator i = ItemList.begin(); i != ItemList.end(); ++i)
    if(i->IsValid())
      {
	item* Item = i->Instantiate(SpecialFlags);

	if(Item)
	  {
	    Stack->AddItem(Item);
	    Item->SpecialGenerationHandler();
	  }
      }
}

void character::ShowDodgeAndMoveInfo() const
{
  felist Info("Dodge and move info:");
  game::SetStandardListAttributes(Info);
  short Bonus;

  /* Dodge value */

  Info.AddEntry("Dodge value: how easily you may avoid enemy attacks", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry(std::string("Base: ") + int(100 / sqrt(GetSize())), LIGHT_GRAY);
  AddSpecialMovePenaltyInfo(Info);
  Bonus = GetAttribute(AGILITY) * 10;

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real dodge value: ") + int(GetDodgeValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Move speed */

  Info.AddEntry("Move speed: how often you may move", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 10", LIGHT_GRAY);
  AddSpecialMovePenaltyInfo(Info);
  Bonus = APBonus(GetAttribute(AGILITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real move speed: ") + 10000 / GetMoveAPRequirement(1), MakeRGB16(220, 220, 220));
  Info.Draw();
}

bool character::HasHadBodyPart(const item* Item) const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(std::find(OriginalBodyPartID[c].begin(), OriginalBodyPartID[c].end(), Item->GetID()) != OriginalBodyPartID[c].end())
      return true;

  return GetPolymorphBackup() && GetPolymorphBackup()->HasHadBodyPart(Item);
}

std::string& character::ProcessMessage(std::string& Msg) const
{
  SEARCH_N_REPLACE("@nu", GetName(UNARTICLED));
  SEARCH_N_REPLACE("@ni", GetName(INDEFINITE));
  SEARCH_N_REPLACE("@nd", GetName(DEFINITE));
  SEARCH_N_REPLACE("@du", GetDescription(UNARTICLED));
  SEARCH_N_REPLACE("@di", GetDescription(INDEFINITE));
  SEARCH_N_REPLACE("@dd", GetDescription(DEFINITE));
  SEARCH_N_REPLACE("@pp", GetPersonalPronoun());
  SEARCH_N_REPLACE("@sp", GetPossessivePronoun());
  SEARCH_N_REPLACE("@op", GetObjectPronoun());
  SEARCH_N_REPLACE("@Nu", festring::CapitalizeCopy(GetName(UNARTICLED)));
  SEARCH_N_REPLACE("@Ni", festring::CapitalizeCopy(GetName(INDEFINITE)));
  SEARCH_N_REPLACE("@Nd", festring::CapitalizeCopy(GetName(DEFINITE)));
  SEARCH_N_REPLACE("@Du", festring::CapitalizeCopy(GetDescription(UNARTICLED)));
  SEARCH_N_REPLACE("@Di", festring::CapitalizeCopy(GetDescription(INDEFINITE)));
  SEARCH_N_REPLACE("@Dd", festring::CapitalizeCopy(GetDescription(DEFINITE)));
  SEARCH_N_REPLACE("@Pp", festring::CapitalizeCopy(GetPersonalPronoun()));
  SEARCH_N_REPLACE("@Sp", festring::CapitalizeCopy(GetPossessivePronoun()));
  SEARCH_N_REPLACE("@Op", festring::CapitalizeCopy(GetObjectPronoun()));
  SEARCH_N_REPLACE("@Gd", GetMasterGod()->GetName());
  return Msg;
}

void character::ProcessAndAddMessage(std::string Msg) const
{
  ADD_MESSAGE("%s", ProcessMessage(Msg).c_str());
}

void character::BeTalkedTo()
{
  static ulong Said;

  if(GetRelation(PLAYER) == HOSTILE)
    ProcessAndAddMessage(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().size())]);
  else
    ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().size())]);
}

bool character::CheckZap()
{
  if(!CanZap())
    {
      ADD_MESSAGE("This monster type can't zap.");
      return false;
    }

  return true;
}

void character::DamageAllItems(character* Damager, ushort Damage, ushort Type)
{
  GetStack()->ReceiveDamage(Damager, Damage, Type);

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c))
      GetEquipment(c)->ReceiveDamage(Damager, Damage, Type);
}

bool character::Equips(const item* Item) const
{
  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->GetID() == Item->GetID())
      return true;

  return false;
}

void character::AddAttributeInfo(std::string& Entry) const
{
  Entry.resize(57, ' ');
  Entry << GetAttribute(ENDURANCE);
  Entry.resize(60, ' ');
  Entry << GetAttribute(PERCEPTION);
  Entry.resize(63, ' ');
  Entry << GetAttribute(INTELLIGENCE);
  Entry.resize(66, ' ');
  Entry << GetAttribute(WISDOM);
  Entry.resize(69, ' ');
  Entry << GetAttribute(CHARISMA);
  Entry.resize(72, ' ');
  Entry << GetAttribute(MANA);
}

void character::PrintBeginConfuseMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Whoh! The ground refuses to settle.");
}

void character::PrintEndConfuseMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("The world is boring again.");
}

vector2d character::ApplyStateModification(vector2d TryDirection) const
{
  if(!StateIsActivated(CONFUSED) || RAND() & 1 || game::IsInWilderness())
    return TryDirection;
  else
    {
      vector2d To = GetLevel()->GetFreeAdjacentSquare(this, GetPos(), true);

      if(To == ERROR_VECTOR)
	return TryDirection;
      else
	return To - GetPos();
    }
}

void character::AddConfuseHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("This stuff is confusing.");
}

item* character::SelectFromPossessions(const std::string& Topic, bool (*SorterFunction)(const item*, const character*))
{
  itemvector ReturnVector;
  SelectFromPossessions(ReturnVector, Topic, NO_MULTI_SELECT, SorterFunction);
  return !ReturnVector.empty() ? ReturnVector[0] : 0;
}

void character::SelectFromPossessions(itemvector& ReturnVector, const std::string& Topic, uchar Flags, bool (*SorterFunction)(const item*, const character*))
{
  if(!CanUseEquipment())
    {
      GetStack()->DrawContents(ReturnVector, this, Topic, Flags, SorterFunction);
      return;
    }

  felist List(Topic);
  bool InventoryPossible = GetStack()->SortedItems(this, SorterFunction);

  if(InventoryPossible)
    List.AddEntry("choose from inventory", LIGHT_GRAY, 20, igraph::GetTransparentTile());

  bool Any = false, UseSorterFunction = (SorterFunction != 0);
  itemvector Item;
  std::string Entry;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && (!UseSorterFunction || SorterFunction(GetEquipment(c), this)))
      {
	Item.push_back(GetEquipment(c));
	Entry = GetEquipmentName(c);
	Entry << ':';
	Entry.resize(20, ' ');
	GetEquipment(c)->AddInventoryEntry(this, Entry, 1, true);
	AddSpecialEquipmentInfo(Entry, c);
	List.AddEntry(Entry, LIGHT_GRAY, 20, GetEquipment(c)->GetPicture(), true, GetEquipment(c)->AllowAlphaEverywhere());
	Any = true;
      }

  if(Any)
    {
      game::SetStandardListAttributes(List);
      List.SetFlags(SELECTABLE|DRAW_BACKGROUND_AFTERWARDS);
      ushort Chosen = List.Draw();

      if(Chosen != ESCAPED)
	{
	  if((InventoryPossible && !Chosen) || Chosen & FELIST_ERROR_BIT)
	    GetStack()->DrawContents(ReturnVector, this, Topic, Flags, SorterFunction);
	  else
	    {
	      ReturnVector.push_back(Item[InventoryPossible ? Chosen - 1 : Chosen]);

	      if(Flags & SELECT_PAIR && ReturnVector[0]->HandleInPairs())
		{
		  item* PairEquipment = GetPairEquipment(ReturnVector[0]->GetEquipmentIndex());

		  if(PairEquipment && PairEquipment->CanBePiledWith(ReturnVector[0], this))
		    ReturnVector.push_back(PairEquipment);
		}
	    }
	}
    }
  else
    GetStack()->DrawContents(ReturnVector, this, Topic, Flags, SorterFunction);
}

bool character::EquipsSomething(bool (*SorterFunction)(const item*, const character*))
{
  bool UseSorterFunction = SorterFunction != 0;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && (!UseSorterFunction || SorterFunction(GetEquipment(c), this)))
      return true;

  return false;
}

material* character::CreateBodyPartMaterial(ushort, ulong Volume) const
{
  return MAKE_MATERIAL(GetFleshMaterial(), Volume);
}

bool character::CheckTalk()
{
  if(!CanTalk())
    {
      ADD_MESSAGE("This monster does not know the art of talking."); 
      return false;
    }

  return true;
}

long character::GetStuffScore() const
{
  long Score = GetStack()->GetScore();

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c))
      Score += GetEquipment(c)->GetScore();

  if(GetAction())
    Score += GetAction()->GetScore();

  return Score;
}

bool character::MoveTowardsHomePos()
{
  return HomeDataIsValid() && (MoveTowards(HomeData->Pos) || (!GetPos().IsAdjacent(HomeData->Pos) && MoveRandomly()));
}

bool character::TryToChangeEquipment(ushort Chosen)
{
  if(!GetBodyPartOfEquipment(Chosen))
    {
      ADD_MESSAGE("Bodypart missing!");
      return false;
    }

  item* OldEquipment = GetEquipment(Chosen);

  if(OldEquipment)
    GetEquipment(Chosen)->MoveTo(GetStack());

  if(!GetStack()->SortedItems(this, EquipmentSorter(Chosen)))
    {
      ADD_MESSAGE("You haven't got any item that could be used for this purpose.");
      return false;
    }
  else
    {
      game::DrawEverythingNoBlit();
      itemvector ItemVector;
      ushort Return = GetStack()->DrawContents(ItemVector, this, "Choose " + GetEquipmentName(Chosen) + ":", NONE_AS_CHOICE|NO_MULTI_SELECT, EquipmentSorter(Chosen));

      if(Return == ESCAPED)
	{
	  if(OldEquipment)
	    {
	      OldEquipment->RemoveFromSlot();
	      SetEquipment(Chosen, OldEquipment);
	    }

	  return false;
	}

      item* Item = ItemVector.empty() ? 0 : ItemVector[0];

      if(Item)
	{
	  Item->RemoveFromSlot();
	  SetEquipment(Chosen, Item);

	  if(CheckIfEquipmentIsNotUsable(Chosen))
	    Item->MoveTo(GetStack());
	}

      return Item != OldEquipment;
    }
}

void character::PrintBeginParasitizedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel you are no longer alone.");
}

void character::PrintEndParasitizedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("A feeling of long welcome emptiness overwhelms you.");
}

void character::ParasitizedHandler()
{
  EditNP(-1);

  if(!(RAND() % 500))
    {
      if(IsPlayer())
	ADD_MESSAGE("Ugh. You feel something violently carving its way through your intestines.");

      ReceiveDamage(0, 1, POISON, TORSO, 8, false, false, false, false);
      CheckDeath("killed by a vile parasite", 0);
    }
}

bool character::CanFollow() const
{
  return CanWalk() && !IsRetreating();
}

std::string character::GetKillName() const
{
  if(!GetPolymorphBackup())
    return GetName(INDEFINITE);
  else
    return GetPolymorphBackup()->GetName(INDEFINITE) + " polymorphed into " + GetName(INDEFINITE);
}

std::string character::GetPanelName() const
{
  std::string Name;
  Name << AssignedName << " the " << game::GetVerbalPlayerAlignment() << ' ';
  id::AddName(Name, UNARTICLED);
  return Name;
}

long character::GetMoveAPRequirement(uchar Difficulty) const
{
  return (!StateIsActivated(PANIC) ? 10000000 : 8000000) * Difficulty / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
}

bool character::HealHitPoint()
{
  uchar NeedHeal = 0, NeedHealIndex[MAX_BODYPARTS];

  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->IsAlive() && BodyPart->GetHP() < BodyPart->GetMaxHP())
	NeedHealIndex[NeedHeal++] = c;
    }

  if(NeedHeal)
    {
      GetBodyPart(NeedHealIndex[RAND() % NeedHeal])->IncreaseHP();
      ++HP;
      return true;
    }
  else
    return false;
}

void character::CreateHomeData()
{
  HomeData = new homedata;
  lsquare* Square = GetLSquareUnder();
  HomeData->Pos = Square->GetPos();
  HomeData->Dungeon = Square->GetDungeonIndex();
  HomeData->Level = Square->GetLevelIndex();
  HomeData->Room = Square->GetRoomIndex();
}

room* character::GetHomeRoom() const
{
  if(HomeDataIsValid() && HomeData->Room)
    return GetLevel()->GetRoom(HomeData->Room);
  else
    return 0;
}

void character::RemoveHomeData()
{ 
  delete HomeData;
  HomeData = 0;
}

void character::AddESPConsumeMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel a strange mental activity.");
}

void character::SetBodyPart(ushort Index, bodypart* What)
{
  BodyPartSlot[Index].PutInItem(What);

  if(What)
    AddOriginalBodyPartID(Index, What->GetID());
}

void character::DetachBodyPart()
{
  ADD_MESSAGE("You haven't got any extra bodyparts.");
}

bool character::ConsumeItem(item* Item)
{
  if(IsPlayer() && HasHadBodyPart(Item) && !game::BoolQuestion("Are you sure? You may be able to put it back... [y/N]"))
    return false;

  if(Item->IsOnGround() && GetRoom() && !GetRoom()->ConsumeItem(this,Item, 1))
    return false;
     
  if(IsPlayer())
    ADD_MESSAGE("You begin %s %s.", Item->GetConsumeVerb().c_str(), Item->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s begins %s %s.", CHAR_NAME(DEFINITE), Item->GetConsumeVerb().c_str(), Item->CHAR_NAME(DEFINITE));

  consume* Consume = new consume(this);
  Consume->SetDescription(Item->GetConsumeVerb());
  Consume->SetWasOnGround(Item->IsOnGround());
  Consume->SetConsuming(Item);
  SetAction(Consume);
  DexterityAction(5);
  return true;
}

void character::RaiseStats()
{
  ushort c = 0;

  for(c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->RaiseStats();

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    BaseAttribute[c] += 10;

  CalculateAll();
  RestoreHP();
  game::SendLOSUpdateRequest();
  UpdateESPLOS();
}

void character::LowerStats()
{
  ushort c = 0;

  for(c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->LowerStats();

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    BaseAttribute[c] -= 10;

  CalculateAll();
  RestoreHP();
  game::SendLOSUpdateRequest();
  UpdateESPLOS();
}

bool character::CheckThrow() const
{
  if(!CanThrow())
    {
      ADD_MESSAGE("This monster type cannot throw.");
      return false;
    }

  return true;
}

void character::GetHitByExplosion(const explosion* Explosion, ushort Damage)
{
  uchar DamageDirection = GetPos() == Explosion->Pos ? RANDOM_DIR : game::CalculateRoughDirection(GetPos() - Explosion->Pos);

  if(GetTeam()->GetID() != PLAYER->GetTeam()->GetID() && Explosion->Terrorist && Explosion->Terrorist->GetTeam()->GetID() == PLAYER->GetTeam()->GetID())
    Explosion->Terrorist->Hostility(this);

  GetTorso()->SpillBlood((8 - Explosion->Size + RAND() % (8 - Explosion->Size)) >> 1);
  vector2d SpillPos = GetPos() + game::GetMoveVector(DamageDirection);

  if(GetArea()->IsValidPos(SpillPos))
    GetTorso()->SpillBlood((8 - Explosion->Size + RAND() % (8 - Explosion->Size)) >> 1, SpillPos);

  if(IsPlayer())
    ADD_MESSAGE("You are hit by the explosion!");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s is hit by the explosion.", CHAR_NAME(DEFINITE));
  
  ReceiveDamage(Explosion->Terrorist, Damage >> 1, FIRE, ALL, DamageDirection, true, false, false, false);
  ReceiveDamage(Explosion->Terrorist, Damage >> 1, PHYSICAL_DAMAGE, ALL, DamageDirection, true, false, false, false);
  CheckDeath(Explosion->DeathMsg, Explosion->Terrorist);
}

void character::SortAllItems(itemvector& AllItems, const character* Character, bool (*Sorter)(const item*, const character*))
{
  GetStack()->SortAllItems(AllItems, Character, Sorter);

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c))
      GetEquipment(c)->SortAllItems(AllItems, Character, Sorter);
}

void character::PrintBeginSearchingMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel you can now notice even the very smallest details around you.");
}

void character::PrintEndSearchingMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel less perceptive.");
}

void character::SearchingHandler()
{
  if(!game::IsInWilderness())
    Search(15);
}

void character::Search(ushort Perception)
{
  for(ushort d = 0; d < 9; ++d)
    {
      lsquare* LSquare = GetNeighbourLSquare(d);

      if(LSquare) 
	LSquare->GetStack()->Search(this, Min<ushort>(Perception, 200));
    }
}

// surprisingly returns 0 if fails

character* character::GetRandomNeighbour(uchar RelationFlags) const
{
  character* Chars[8];
  ushort Index = 0;

  for(ushort c = 0; c < 8; ++c)
    {
      lsquare* LSquare = GetNeighbourLSquare(c);

      if(LSquare) 
	{
	  character* Char = LSquare->GetCharacter();

	  if(Char && (GetRelation(Char) & RelationFlags))
	    Chars[Index++] = Char;
	}
    }

  return Index ? Chars[RAND() % Index] : 0;
}

void character::ResetStates()
{
  for(ushort c = 0; c < STATES; ++c)
    if(1 << c != POLYMORPHED && TemporaryStateIsActivated(1 << c) && TemporaryStateCounter[c] != PERMANENT)
      {
	TemporaryState &= ~(1 << c);

	if(StateData[c].EndHandler)
	  {
	    (this->*StateData[c].EndHandler)();

	    if(!IsEnabled())
	      return;
	  }
      }
}

void characterdatabase::InitDefaults(ushort Config)
{
  IsAbstract = false;

  /* TERRIBLE gum solution! */

  if(Config & DEVOUT)
    PostFix.append("of " + festring::CapitalizeCopy(protocontainer<god>::GetProto(Config&0xFF)->GetClassId()));
}

void character::PrintBeginGasImmunityMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("All smells fade away.");
}

void character::PrintEndGasImmunityMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Yuck! The world smells bad again.");
}

void character::ShowAdventureInfo() const
{
  if(GetStack()->GetItems() && game::BoolQuestion("Do you want to see your inventory? [y/n]", REQUIRES_ANSWER))
    {
      GetStack()->DrawContents(this, "Your inventory", NO_SELECT);

      for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
	i->DrawContents(this);

      for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	if(GetEquipment(c))
	  GetEquipment(c)->DrawContents(this);
    }

  if(game::BoolQuestion("Do you want to see your message history? [y/n]", REQUIRES_ANSWER))
    msgsystem::DrawMessageHistory();

  if(!game::MassacreListsEmpty() && game::BoolQuestion("Do you want to see your massacre history? [y/n]", REQUIRES_ANSWER))
    game::DisplayMassacreLists();
}

void character::DrawBodyPartVector(std::vector<bitmap*>& Bitmap) const
{
  ushort c, AnimationFrames = 1;

  for(c = 0; c < BodyParts; ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->GetAnimationFrames() > AnimationFrames)
      AnimationFrames = GetBodyPart(c)->GetAnimationFrames();

  Bitmap.resize(AnimationFrames);

  for(c = 0; c < AnimationFrames; ++c)
    {
      Bitmap[c] = new bitmap(16, 16, TRANSPARENT_COLOR);
      globalwindowhandler::SetTick(c);
      DrawBodyParts(Bitmap[c], vector2d(0, 0), NORMAL_LUMINANCE, true, false);
    }
}
