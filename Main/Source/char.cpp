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

/* Compiled through charset.cpp */

/* These statedata structs contain functions and values used for handling states. Remember to update them.
 * All normal states must have PrintBeginMessage and PrintEndMessage functions and a Description string.
 * BeginHandler, EndHandler, Handler (called each tick) and IsAllowed are optional, enter zero if the state
 * doesn't need one. If the SECRET flag is set, Description is not shown in the panel without magical means.
 * You can also set some source (SRC_*) and duration (DUR_*) flags, which control whether the state can be
 * randomly activated in certain situations. These flags can be found in ivandef.h. RANDOMIZABLE sets all
 * source & duration flags at once. */

struct statedata
{
  char* Description;
  int Flags;
  void (character::*PrintBeginMessage)() const;
  void (character::*PrintEndMessage)() const;
  void (character::*BeginHandler)();
  void (character::*EndHandler)();
  void (character::*Handler)();
  bool (character::*IsAllowed)() const;
  void (character::*SituationDangerModifier)(double&) const;
};

statedata StateData[STATES] =
{
  {
    "Polymorphed",
    NO_FLAGS,
    0,
    0,
    0,
    &character::EndPolymorph,
    0,
    0,
    0
  }, {
    "Hasted",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL),
    &character::PrintBeginHasteMessage,
    &character::PrintEndHasteMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Slowed",
    RANDOMIZABLE&~SRC_GOOD,
    &character::PrintBeginSlowMessage,
    &character::PrintEndSlowMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "PolyControl",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL|SRC_GOOD),
    &character::PrintBeginPolymorphControlMessage,
    &character::PrintEndPolymorphControlMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "LifeSaved",
    SECRET,
    &character::PrintBeginLifeSaveMessage,
    &character::PrintEndLifeSaveMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Lycanthropy",
    SECRET|SRC_FOUNTAIN|SRC_CONFUSE_READ|DUR_FLAGS,
    &character::PrintBeginLycanthropyMessage,
    &character::PrintEndLycanthropyMessage,
    0,
    0,
    &character::LycanthropyHandler,
    0,
    &character::LycanthropySituationDangerModifier
  }, {
    "Invisible",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL),
    &character::PrintBeginInvisibilityMessage,
    &character::PrintEndInvisibilityMessage,
    &character::BeginInvisibility,  &character::EndInvisibility,
    0,
    0,
    0
  }, {
    "Infravision",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL),
    &character::PrintBeginInfraVisionMessage,
    &character::PrintEndInfraVisionMessage,
    &character::BeginInfraVision,
    &character::EndInfraVision,
    0,
    0,
    0
  }, {
    "ESP",
    RANDOMIZABLE&~SRC_EVIL,
    &character::PrintBeginESPMessage,
    &character::PrintEndESPMessage,
    &character::BeginESP,
    &character::EndESP,
    0,
    0,
    0
  }, {
    "Poisoned",
    DUR_TEMPORARY,
    &character::PrintBeginPoisonedMessage,
    &character::PrintEndPoisonedMessage,
    0,
    0,
    &character::PoisonedHandler,
    &character::CanBePoisoned,
    &character::PoisonedSituationDangerModifier
  }, {
    "Teleporting",
    SECRET|RANDOMIZABLE&~(SRC_MUSHROOM|SRC_GOOD),
    &character::PrintBeginTeleportMessage,
    &character::PrintEndTeleportMessage,
    0,
    0,
    &character::TeleportHandler,
    0,
    0
  }, {
    "Polymorphing",
    SECRET|RANDOMIZABLE&~(SRC_MUSHROOM|SRC_GOOD),
    &character::PrintBeginPolymorphMessage,
    &character::PrintEndPolymorphMessage,
    0,
    0,
    &character::PolymorphHandler,
    0,
    &character::PolymorphingSituationDangerModifier
  }, {
    "TeleControl",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL),
    &character::PrintBeginTeleportControlMessage,
    &character::PrintEndTeleportControlMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Panicked",
    NO_FLAGS,
    &character::PrintBeginPanicMessage,
    &character::PrintEndPanicMessage,
    &character::BeginPanic,
    &character::EndPanic,
    0,
    &character::CanPanic,
    &character::PanicSituationDangerModifier
  }, {
    "Confused",
    SECRET|RANDOMIZABLE&~(DUR_PERMANENT|SRC_GOOD),
    &character::PrintBeginConfuseMessage,
    &character::PrintEndConfuseMessage,
    0,
    0,
    0,
    &character::CanBeConfused,
    &character::ConfusedSituationDangerModifier
  }, {
    "Parasitized",
    RANDOMIZABLE&~DUR_TEMPORARY,
    &character::PrintBeginParasitizedMessage,
    &character::PrintEndParasitizedMessage,
    0,
    0,
    &character::ParasitizedHandler,
    &character::CanBeParasitized,
    &character::ParasitizedSituationDangerModifier
  }, {
    "Searching",
    NO_FLAGS,
    &character::PrintBeginSearchingMessage,
    &character::PrintEndSearchingMessage,
    0,
    0,
    &character::SearchingHandler,
    0,
    0
  }, {
    "GasImmunity",
    SECRET|RANDOMIZABLE&~(SRC_GOOD|SRC_EVIL),
    &character::PrintBeginGasImmunityMessage,
    &character::PrintEndGasImmunityMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Levitating",
    RANDOMIZABLE&~SRC_EVIL,
    &character::PrintBeginLevitationMessage,
    &character::PrintEndLevitationMessage,
    0,
    &character::EndLevitation,
    0,
    0,
    0
  }, {
    "Leprosy",
    SECRET|RANDOMIZABLE&~DUR_TEMPORARY,
    &character::PrintBeginLeprosyMessage,
    &character::PrintEndLeprosyMessage,
    &character::BeginLeprosy,
    &character::EndLeprosy,
    &character::LeprosyHandler,
    0,
    &character::LeprosySituationDangerModifier
  }
};

characterprototype::characterprototype(characterprototype* Base, character* (*Cloner)(int, int), const char* ClassID) : Base(Base), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<character>::Add(this); }
std::list<character*>::iterator character::GetTeamIterator() { return TeamIterator; }
void character::SetTeamIterator(std::list<character*>::iterator What) { TeamIterator = What; }
void character::CreateInitialEquipment(int SpecialFlags) { AddToInventory(DataBase->Inventory, SpecialFlags); }
void character::EditAP(long What) { AP = Limit<long>(AP + What, -12000, 1200); }
bool character::CanUseEquipment(int I) const { return CanUseEquipment() && I < GetEquipmentSlots() && GetBodyPartOfEquipment(I); }
int character::GetRandomStepperBodyPart() const { return TORSO_INDEX; }
void character::GainIntrinsic(long What) { BeginTemporaryState(What, PERMANENT); }
bool character::IsUsingArms() const { return !!(GetAttackStyle() & USE_ARMS); }
bool character::IsUsingLegs() const { return !!(GetAttackStyle() & USE_LEGS); }
bool character::IsUsingHead() const { return !!(GetAttackStyle() & USE_HEAD); }
void character::CalculateAllowedWeaponSkillCategories() { AllowedWeaponSkillCategories = MARTIAL_SKILL_CATEGORIES; }
festring character::GetBeVerb() const { return IsPlayer() ? CONST_S("are") : CONST_S("is"); }
void character::SetEndurance(int What) { BaseExperience[ENDURANCE] = What * EXP_MULTIPLIER; }
void character::SetPerception(int What) { BaseExperience[PERCEPTION] = What * EXP_MULTIPLIER; }
void character::SetIntelligence(int What) { BaseExperience[INTELLIGENCE] = What * EXP_MULTIPLIER; }
void character::SetWisdom(int What) { BaseExperience[WISDOM] = What * EXP_MULTIPLIER; }
void character::SetCharisma(int What) { BaseExperience[CHARISMA] = What * EXP_MULTIPLIER; }
void character::SetMana(int What) { BaseExperience[MANA] = What * EXP_MULTIPLIER; }
bool character::IsOnGround() const { return MotherEntity && MotherEntity->IsOnGround(); }
bool character::LeftOversAreUnique() const { return GetArticleMode() != NORMAL_ARTICLE || AssignedName.GetSize(); }
bool character::HomeDataIsValid() const { return HomeData && HomeData->Level == GetLSquareUnder()->GetLevelIndex() && HomeData->Dungeon == GetLSquareUnder()->GetDungeonIndex(); }
void character::SetHomePos(vector2d Pos) { HomeData->Pos = Pos; }
const char* character::FirstPersonUnarmedHitVerb() const { return "hit"; }
const char* character::FirstPersonCriticalUnarmedHitVerb() const { return "critically hit"; }
const char* character::ThirdPersonUnarmedHitVerb() const { return "hits"; }
const char* character::ThirdPersonCriticalUnarmedHitVerb() const { return "critically hits"; }
const char* character::FirstPersonKickVerb() const { return "kick"; }
const char* character::FirstPersonCriticalKickVerb() const { return "critically kick"; }
const char* character::ThirdPersonKickVerb() const { return "kicks"; }
const char* character::ThirdPersonCriticalKickVerb() const { return "critically kicks"; }
const char* character::FirstPersonBiteVerb() const { return "bite"; }
const char* character::FirstPersonCriticalBiteVerb() const { return "critically bite"; }
const char* character::ThirdPersonBiteVerb() const { return "bites"; }
const char* character::ThirdPersonCriticalBiteVerb() const { return "critically bites"; }
const char* character::UnarmedHitNoun() const { return "attack"; }
const char* character::KickNoun() const { return "kick"; }
const char* character::BiteNoun() const { return "attack"; }
//int character::GetSpecialBodyPartFlags(int) const { return ST_NORMAL; }
const char* character::GetEquipmentName(int) const { return ""; }
const std::list<ulong>& character::GetOriginalBodyPartID(int I) const { return OriginalBodyPartID[I]; }
square* character::GetNeighbourSquare(int I) const { return GetSquareUnder()->GetNeighbourSquare(I); }
lsquare* character::GetNeighbourLSquare(int I) const { return static_cast<lsquare*>(GetSquareUnder())->GetNeighbourLSquare(I); }
wsquare* character::GetNeighbourWSquare(int I) const { return static_cast<wsquare*>(GetSquareUnder())->GetNeighbourWSquare(I); }
god* character::GetMasterGod() const { return game::GetGod(GetConfig()); }
wsquare* character::GetNearWSquare(vector2d Pos) const { return static_cast<wsquare*>(GetSquareUnder()->GetArea()->GetSquare(Pos)); }
wsquare* character::GetNearWSquare(int x, int y) const { return static_cast<wsquare*>(GetSquareUnder()->GetArea()->GetSquare(x, y)); }
color16 character::GetBodyPartColorA(int, bool) const { return GetSkinColor(); }
color16 character::GetBodyPartColorB(int, bool) const { return GetTorsoMainColor(); }
color16 character::GetBodyPartColorC(int, bool) const { return 0; } // reserved for future use
color16 character::GetBodyPartColorD(int, bool) const { return GetTorsoSpecialColor(); }
bool character::BodyPartColorAIsSparkling(int, bool) const { return !!(GetSparklingFlags() & SKIN_COLOR); }
bool character::BodyPartColorBIsSparkling(int, bool) const { return !!(GetSparklingFlags() & TORSO_MAIN_COLOR); }
bool character::BodyPartColorCIsSparkling(int, bool) const { return false; } // reserved for future use
bool character::BodyPartColorDIsSparkling(int, bool) const { return !!(GetSparklingFlags() & TORSO_SPECIAL_COLOR); }
int character::GetRandomApplyBodyPart() const { return TORSO_INDEX; }
bool character::MustBeRemovedFromBone() const { return IsUnique() && !CanBeGenerated(); }
bool character::IsPet() const { return GetTeam()->GetID() == PLAYER_TEAM; }
character* character::GetLeader() const { return GetTeam()->GetLeader(); }
int character::GetMoveType() const { return !StateIsActivated(LEVITATION) ? DataBase->MoveType : DataBase->MoveType | FLY; }
festring character::GetZombieDescription() const { return " of " + GetName(INDEFINITE); }
bool character::BodyPartCanBeSevered(int I) const { return !!I; }
bool character::HasBeenSeen() const { return !!(DataBase->Flags & HAS_BEEN_SEEN); }
bool character::IsTemporary() const { return !!GetTorso()->GetLifeExpectancy(); }

character::character(const character& Char) : entity(Char), id(Char), NP(Char.NP), AP(Char.AP), Player(false), TemporaryState(Char.TemporaryState&~POLYMORPHED), Team(Char.Team), GoingTo(ERROR_VECTOR), Money(0), AssignedName(Char.AssignedName), Action(0), DataBase(Char.DataBase), MotherEntity(0), PolymorphBackup(0), EquipmentState(0), SquareUnder(0), Initializing(true), AllowedWeaponSkillCategories(Char.AllowedWeaponSkillCategories), BodyParts(Char.BodyParts), Polymorphed(false), InNoMsgMode(true), RegenerationCounter(Char.RegenerationCounter), PictureUpdatesForbidden(false), SquaresUnder(Char.SquaresUnder), LastAcidMsgMin(0), Stamina(Char.Stamina), MaxStamina(Char.MaxStamina), BlocksSinceLastTurn(0), GenerationDanger(Char.GenerationDanger), CommandFlags(Char.CommandFlags), WarnFlags(0), ScienceTalks(Char.ScienceTalks), TrapData(0)
{
  Stack = new stack(0, this, HIDDEN);

  int c;

  for(c = 0; c < STATES; ++c)
    TemporaryStateCounter[c] = Char.TemporaryStateCounter[c];

  if(Team)
    TeamIterator = Team->Add(this);

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    BaseExperience[c] = Char.BaseExperience[c];

  BodyPartSlot = new bodypartslot[BodyParts];
  OriginalBodyPartID = new std::list<ulong>[BodyParts];
  CWeaponSkill = new cweaponskill[AllowedWeaponSkillCategories];
  SquareUnder = new square*[SquaresUnder];

  if(SquaresUnder == 1)
    *SquareUnder = 0;
  else
    memset(SquareUnder, 0, SquaresUnder * sizeof(square*));

  for(c = 0; c < BodyParts; ++c)
    {
      BodyPartSlot[c].SetMaster(this);
      bodypart* CharBodyPart = Char.GetBodyPart(c);
      OriginalBodyPartID[c] = Char.OriginalBodyPartID[c];

      if(CharBodyPart)
	{
	  bodypart* BodyPart = static_cast<bodypart*>(CharBodyPart->Duplicate());
	  SetBodyPart(c, BodyPart);
	  BodyPart->CalculateEmitation();
	}
    }

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    CWeaponSkill[c] = Char.CWeaponSkill[c];

  HomeData = Char.HomeData ? new homedata(*Char.HomeData) : 0;
  ID = game::CreateNewCharacterID(this);
}

character::character(donothing) : entity(HAS_BE), NP(50000), AP(0), Player(false), TemporaryState(0), Team(0), GoingTo(ERROR_VECTOR), Money(0), Action(0), MotherEntity(0), PolymorphBackup(0), EquipmentState(0), SquareUnder(0), Polymorphed(false), RegenerationCounter(0), HomeData(0), PictureUpdatesForbidden(false), LastAcidMsgMin(0), BlocksSinceLastTurn(0), GenerationDanger(DEFAULT_GENERATION_DANGER), WarnFlags(0), ScienceTalks(0), TrapData(0)
{
  Stack = new stack(0, this, HIDDEN);
}

character::~character()
{
  if(Action)
    delete Action;

  if(Team)
    Team->Remove(GetTeamIterator());

  delete Stack;
  int c;

  for(c = 0; c < BodyParts; ++c)
    delete GetBodyPart(c);

  delete [] BodyPartSlot;
  delete [] OriginalBodyPartID;
  delete PolymorphBackup;
  delete [] SquareUnder;
  delete [] CWeaponSkill;
  delete HomeData;

  for(trapdata* T = TrapData; T;)
    {
      trapdata* ToDel = T;
      T = T->Next;
      delete ToDel;
    }

  game::RemoveCharacterID(ID);
}

void character::Hunger() 
{
  int OldState = GetHungerState();

  switch(GetBurdenState())
    {
    case OVER_LOADED:
    case STRESSED:
      EditNP(-8);
      EditExperience(LEG_STRENGTH, 150, 1 << 2);
      EditExperience(AGILITY, -50, 1 << 2);
      break;
    case BURDENED:
      EditNP(-2);
      EditExperience(LEG_STRENGTH, 75, 1 << 1);
      EditExperience(AGILITY, -25, 1 << 1);
      break;
    case UNBURDENED:
      EditNP(-1);
      break;
    }

  if(OldState != STARVING && GetHungerState() == STARVING)
    DeActivateVoluntaryAction(CONST_S("You are getting extremely hungry."));

  switch(GetHungerState())
    {
    case STARVING:
      EditExperience(ARM_STRENGTH, -75, 1 << 3);
      EditExperience(LEG_STRENGTH, -75, 1 << 3);
      break;
    case VERY_HUNGRY:
      EditExperience(ARM_STRENGTH, -50, 1 << 2);
      EditExperience(LEG_STRENGTH, -50, 1 << 2);
      break;
    case HUNGRY:
      EditExperience(ARM_STRENGTH, -25, 1 << 1);
      EditExperience(LEG_STRENGTH, -25, 1 << 1);
      break;
    case SATIATED:
      EditExperience(AGILITY, -25, 1 << 1);
      break;
    case BLOATED:
      EditExperience(AGILITY, -50, 1 << 2);
      break;
    case OVER_FED:
      EditExperience(AGILITY, -75, 1 << 3);
      break;
    }

  CheckStarvationDeath(CONST_S("starved to death"));
}

int character::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, vector2d HitPos, double Damage, double ToHitValue, int Success, int Type, int GivenDir, bool Critical, bool ForceHit)
{
  int Dir = Type == BITE_ATTACK ? YOURSELF : GivenDir;
  double DodgeValue = GetDodgeValue();

  if(!Enemy->IsPlayer() && GetAttackWisdomLimit() != NO_LIMIT)
    Enemy->EditExperience(WISDOM, 75, 1 << 13);

  if(!Enemy->CanBeSeenBy(this))
    ToHitValue *= 2;

  if(!CanBeSeenBy(Enemy))
    DodgeValue *= 2;

  if(Enemy->StateIsActivated(CONFUSED))
    ToHitValue *= 0.75;

  switch(Enemy->GetTirednessState())
    {
    case FAINTING:
      ToHitValue *= 0.50;
    case EXHAUSTED:
      ToHitValue *= 0.75;
    }

  switch(GetTirednessState())
    {
    case FAINTING:
      DodgeValue *= 0.50;
    case EXHAUSTED:
      DodgeValue *= 0.75;
    }

  if(!ForceHit)
    {
      SetGoingTo(Enemy->GetPos());
      Enemy->SetGoingTo(GetPos());
    }

  /* Effectively, the average chance to hit is 100% / (DV/THV + 1). */

  if(RAND() % int(100 + ToHitValue / DodgeValue * (100 + Success)) < 100 && !Critical && !ForceHit)
    {
      Enemy->AddMissMessage(this);
      EditExperience(AGILITY, 150, 1 << 7);
      EditExperience(PERCEPTION, 75, 1 << 7);

      if(Enemy->CanBeSeenByPlayer())
	DeActivateVoluntaryAction(CONST_S("The attack of ") + Enemy->GetName(DEFINITE) + CONST_S(" interrupts you."));
      else
	DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));

      return HAS_DODGED;
    }

  int TrueDamage = int(Damage * (100 + Success) / 100) + (RAND() % 3 ? 1 : 0);

  if(Critical)
    {
      TrueDamage += TrueDamage >> 1;
      ++TrueDamage;
    }

  int BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, DodgeValue);

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
	    DeActivateVoluntaryAction(CONST_S("The attack of ") + Enemy->GetName(DEFINITE) + CONST_S(" interrupts you."));
	  else
	    DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));

	  return HAS_BLOCKED;
	}
    }

  int WeaponSkillHits = CalculateWeaponSkillHits(Enemy);
  int DoneDamage = ReceiveBodyPartDamage(Enemy, TrueDamage, PHYSICAL_DAMAGE, BodyPart, Dir, false, Critical, true, Type == BITE_ATTACK && Enemy->BiteCapturesBodyPart());
  bool Succeeded = (GetBodyPart(BodyPart) && HitEffect(Enemy, Weapon, HitPos, Type, BodyPart, Dir, !DoneDamage)) || DoneDamage;

  if(Succeeded)
    Enemy->WeaponSkillHit(Weapon, Type, WeaponSkillHits);

  if(Weapon)
    {
      if(Weapon->Exists() && DoneDamage < TrueDamage)
	Weapon->ReceiveDamage(Enemy, TrueDamage - DoneDamage, PHYSICAL_DAMAGE);

      if(Weapon->Exists() && DoneDamage && SpillsBlood() && GetBodyPart(BodyPart) && (GetBodyPart(BodyPart)->IsAlive() || GetBodyPart(BodyPart)->GetMainMaterial()->IsLiquid()))
	Weapon->SpillFluid(0, CreateBlood(15 + RAND() % 15));
    }

  if(Enemy->AttackIsBlockable(Type))
    SpecialBodyDefenceEffect(Enemy, EnemyBodyPart, Type);

  if(!Succeeded)
    {
      if(Enemy->CanBeSeenByPlayer())
	DeActivateVoluntaryAction(CONST_S("The attack of ") + Enemy->GetName(DEFINITE) + CONST_S(" interrupts you."));
      else
	DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));

      return DID_NO_DAMAGE;
    }

  festring DeathMsg = CONST_S("killed by ") + Enemy->GetKillName();

  if(CheckDeath(DeathMsg, Enemy, Enemy->IsPlayer()))
    return HAS_DIED;

  if(Enemy->CanBeSeenByPlayer())
    DeActivateVoluntaryAction(CONST_S("The attack of ") + Enemy->GetName(DEFINITE) + CONST_S(" interrupts you."));
  else
    DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));

  return HAS_HIT;
}

struct svpriorityelement
{
  svpriorityelement(int BodyPart, int StrengthValue) : BodyPart(BodyPart), StrengthValue(StrengthValue) { }
  bool operator<(const svpriorityelement& AnotherPair) const { return StrengthValue > AnotherPair.StrengthValue; }
  int BodyPart;
  int StrengthValue;
};

int character::ChooseBodyPartToReceiveHit(double ToHitValue, double DodgeValue)
{
  if(BodyParts == 1)
    return 0;

  std::priority_queue<svpriorityelement> SVQueue;

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart
      && (BodyPart->GetHP() != 1 || BodyPart->CanBeSevered(PHYSICAL_DAMAGE)))
	SVQueue.push(svpriorityelement(c, ModifyBodyPartHitPreference(c, BodyPart->GetStrengthValue() + BodyPart->GetHP())));
    }

  while(SVQueue.size())
    {
      svpriorityelement E = SVQueue.top();
      int ToHitPercentage = int(GLOBAL_WEAK_BODYPART_HIT_MODIFIER * ToHitValue * GetBodyPart(E.BodyPart)->GetBodyPartVolume() / (DodgeValue * GetBodyVolume()));
      ToHitPercentage = ModifyBodyPartToHitChance(E.BodyPart, ToHitPercentage);

      if(ToHitPercentage < 1)
	ToHitPercentage = 1;
      else if(ToHitPercentage > 95)
	ToHitPercentage = 95;

      if(ToHitPercentage > RAND() % 100)
	return E.BodyPart;

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
      bool ForceBe = HP != MaxHP || AllowSpoil();

      for(int c = 0; c < BodyParts; ++c)
	{
	  bodypart* BodyPart = GetBodyPart(c);

	  if(BodyPart && (ForceBe || BodyPart->NeedsBe()))
	    BodyPart->Be();
	}

      HandleStates();

      if(!IsEnabled())
	return;

      if(GetTeam() == PLAYER->GetTeam())
	{
	  for(int c = 0; c < AllowedWeaponSkillCategories; ++c)
	    if(CWeaponSkill[c].Tick() && IsPlayer())
	      CWeaponSkill[c].AddLevelDownMessage(c);

	  SWeaponSkillTick();
	}

      if(IsPlayer())
	{
	  if(GetHungerState() == STARVING && !(RAND() % 50))
	    LoseConsciousness(250 + RAND_N(250), true);

	  if(!Action || Action->AllowFoodConsumption())
	    Hunger();
	}

      if(Stamina != MaxStamina)
	RegenerateStamina();

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
      //ApplyExperience();
      SpecialTurnHandler();
      BlocksSinceLastTurn = 0;

      if(IsPlayer())
	{
	  static int Timer = 0;

	  if(ivanconfig::GetAutoSaveInterval() && !GetAction() && ++Timer >= ivanconfig::GetAutoSaveInterval())
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
		  static int Counter = 0;

		  if(++Counter == 10)
		    {
		      game::DrawEverything();
		      Counter = 0;
		    }
		}

	      msgsystem::ThyMessagesAreNowOld();

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

void character::Move(vector2d MoveTo, bool TeleportMove, bool Run)
{
  if(!IsEnabled())
    return;

  /* Test whether the player is stuck to something */

  if(!TeleportMove && !TryToUnStickTraps(MoveTo - GetPos()))
    return;

  if(Run && !IsPlayer() && TorsoIsAlive()
  && (Stamina <= 10000 / Max(GetAttribute(LEG_STRENGTH), 1)
   || (!StateIsActivated(PANIC) && Stamina < MaxStamina >> 2)))
    Run = false;

  RemoveTraps();

  if(GetBurdenState() != OVER_LOADED || TeleportMove)
    {
      lsquare* OldSquareUnder[MAX_SQUARES_UNDER];

      if(!game::IsInWilderness())
	for(int c = 0; c < GetSquaresUnder(); ++c)
	  OldSquareUnder[c] = GetLSquareUnder(c);

      Remove();
      PutTo(MoveTo);

      if(!TeleportMove)
	{
	  /* Multitiled creatures should behave differently, maybe? */

	  if(Run)
	    {
	      EditAP(-GetMoveAPRequirement(GetSquareUnder()->GetEntryDifficulty()) >> 1);
	      EditNP(-24 * GetSquareUnder()->GetEntryDifficulty());
	      EditExperience(AGILITY, 125, GetSquareUnder()->GetEntryDifficulty() << 7);
	      int Base = 10000;

	      if(IsPlayer())
		switch(GetHungerState())
		  {
		    case SATIATED:
		      Base = 11000;
		      break;
		    case BLOATED:
		      Base = 12500;
		      break;
		    case OVER_FED:
		      Base = 15000;
		      break;
		  }

	      EditStamina(-Base / Max(GetAttribute(LEG_STRENGTH), 1), true);
	    }
	  else
	    {
	      EditAP(-GetMoveAPRequirement(GetSquareUnder()->GetEntryDifficulty()));
	      EditNP(-12 * GetSquareUnder()->GetEntryDifficulty());
	      EditExperience(AGILITY, 75, GetSquareUnder()->GetEntryDifficulty() << 7);
	    }
	}

      if(IsPlayer())
	ShowNewPosInfo();

      if(!game::IsInWilderness())
	SignalStepFrom(OldSquareUnder);
    }
  else
    {
      if(IsPlayer())
	{
	  const char* CrawlVerb = StateIsActivated(LEVITATION) ? "float" : "crawl";
	  ADD_MESSAGE("You try very hard to %s forward. But your load is too heavy.", CrawlVerb);
	}

      EditAP(-1000);
    }
}

void character::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  if(CheckForEnemies(true, true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(CheckForDoors())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

bool character::MoveTowardsTarget(bool Run)
{
  vector2d Pos = GetPos();
  vector2d TPos;

  if(!Route.empty())
    {
      TPos = Route.back();
      Route.pop_back();
    }
  else
    TPos = GoingTo;

  vector2d MoveTo[3];

  if(TPos.X < Pos.X)
    {
      if(TPos.Y < Pos.Y)
	{
	  MoveTo[0] = vector2d(-1, -1);
	  MoveTo[1] = vector2d(-1,  0);
	  MoveTo[2] = vector2d( 0, -1);
	}

      if(TPos.Y == Pos.Y)
	{
	  MoveTo[0] = vector2d(-1,  0);
	  MoveTo[1] = vector2d(-1, -1);
	  MoveTo[2] = vector2d(-1,  1);
	}

      if(TPos.Y > Pos.Y)
	{
	  MoveTo[0] = vector2d(-1, 1);
	  MoveTo[1] = vector2d(-1, 0);
	  MoveTo[2] = vector2d( 0, 1);
	}
    }

  if(TPos.X == Pos.X)
    {
      if(TPos.Y < Pos.Y)
	{
	  MoveTo[0] = vector2d( 0, -1);
	  MoveTo[1] = vector2d(-1, -1);
	  MoveTo[2] = vector2d( 1, -1);
	}

      if(TPos.Y == Pos.Y)
	{
	  TerminateGoingTo();
	  return false;
	}

      if(TPos.Y > Pos.Y)
	{
	  MoveTo[0] = vector2d( 0, 1);
	  MoveTo[1] = vector2d(-1, 1);
	  MoveTo[2] = vector2d( 1, 1);
	}
    }

  if(TPos.X > Pos.X)
    {
      if(TPos.Y < Pos.Y)
	{
	  MoveTo[0] = vector2d(1, -1);
	  MoveTo[1] = vector2d(1,  0);
	  MoveTo[2] = vector2d(0, -1);
	}

      if(TPos.Y == Pos.Y)
	{
	  MoveTo[0] = vector2d(1,  0);
	  MoveTo[1] = vector2d(1, -1);
	  MoveTo[2] = vector2d(1,  1);
	}

      if(TPos.Y > Pos.Y)
	{
	  MoveTo[0] = vector2d(1, 1);
	  MoveTo[1] = vector2d(1, 0);
	  MoveTo[2] = vector2d(0, 1);
	}
    }

  vector2d ModifiedMoveTo = ApplyStateModification(MoveTo[0]);

  if(TryMove(ModifiedMoveTo, true, Run)) return true;

  int L = (Pos - TPos).GetManhattanLength();

  if(RAND() & 1)
    Swap(MoveTo[1], MoveTo[2]);

  if((Pos + MoveTo[1] - TPos).GetManhattanLength() < L
  && TryMove(ApplyStateModification(MoveTo[1]), true, Run))
    return true;

  if((Pos + MoveTo[2] - TPos).GetManhattanLength() < L
  && TryMove(ApplyStateModification(MoveTo[2]), true, Run))
    return true;

  Illegal.insert(Pos + ModifiedMoveTo);

  if(CreateRoute())
    return true;

  return false;
}

int character::CalculateNewSquaresUnder(lsquare** NewSquare, vector2d Pos) const
{
  if(GetLevel()->IsValidPos(Pos))
    {
      *NewSquare = GetNearLSquare(Pos);
      return 1;
    }
  else
    return 0;
}

bool character::TryMove(vector2d MoveVector, bool Important, bool Run)
{
  lsquare* MoveToSquare[MAX_SQUARES_UNDER];
  character* Pet[MAX_SQUARES_UNDER];
  character* Neutral[MAX_SQUARES_UNDER];
  character* Hostile[MAX_SQUARES_UNDER];
  vector2d PetPos[MAX_SQUARES_UNDER];
  vector2d NeutralPos[MAX_SQUARES_UNDER];
  vector2d HostilePos[MAX_SQUARES_UNDER];
  vector2d MoveTo = GetPos() + MoveVector;
  int Direction = game::GetDirectionForVector(MoveVector);

  if(Direction == DIR_ERROR)
    ABORT("Direction fault.");

  if(!game::IsInWilderness())
    {
      int Squares = CalculateNewSquaresUnder(MoveToSquare, MoveTo);

      if(Squares)
	{
	  int Pets = 0;
	  int Neutrals = 0;
	  int Hostiles = 0;

	  for(int c = 0; c < Squares; ++c)
	    {
	      character* Char = MoveToSquare[c]->GetCharacter();

	      if(Char && Char != this)
		{
		  vector2d Pos = MoveToSquare[c]->GetPos();

		  if(IsAlly(Char))
		    {
		      Pet[Pets] = Char;
		      PetPos[Pets++] = Pos;
		    }
		  else if(Char->GetRelation(this) != HOSTILE)
		    {
		      Neutral[Neutrals] = Char;
		      NeutralPos[Neutrals++] = Pos;
		    }
		  else
		    {
		      Hostile[Hostiles] = Char;
		      HostilePos[Hostiles++] = Pos;
		    }
		}
	    }

	  if(Hostiles == 1)
	    return Hit(Hostile[0], HostilePos[0], Direction);
	  else if(Hostiles)
	    {
	      int Index = RAND() % Hostiles;
	      return Hit(Hostile[Index], HostilePos[Index], Direction);
	    }

	  if(Neutrals == 1)
	    {
	      if(!IsPlayer() && !Pets && Important && CanMoveOn(MoveToSquare[0]))
		return HandleCharacterBlockingTheWay(Neutral[0], NeutralPos[0], Direction);
	      else
		return IsPlayer() && Hit(Neutral[0], NeutralPos[0], Direction);
	    }
	  else if(Neutrals)
	    if(IsPlayer())
	      {
		int Index = RAND() % Neutrals;
		return Hit(Neutral[Index], NeutralPos[Index], Direction);
	      }
	    else
	      return false;

	  if(!IsPlayer())
	    for(int c = 0; c < Squares; ++c)
	      if(MoveToSquare[c]->IsScaryForAIToStepOn(this))
		return false;

	  if(Pets == 1)
	    return Important
		&& (CanMoveOn(MoveToSquare[0])
		 || (IsPlayer()
		  && game::GoThroughWallsCheatIsActive()))
		&& Displace(Pet[0]);
	  else if(Pets)
	    return false;

	  if(CanMove()
	  && CanMoveOn(MoveToSquare[0])
	  || (game::GoThroughWallsCheatIsActive() && IsPlayer()))
	    {
	      Move(MoveTo, false, Run);

	      if(IsEnabled() && GetPos() == GoingTo)
		TerminateGoingTo();

	      return true;
	    }
	  else
	    for(int c = 0; c < Squares; ++c)
	      {
		olterrain* Terrain = MoveToSquare[c]->GetOLTerrain();

		if(Terrain && Terrain->CanBeOpened())
		  {
		    if(CanOpen())
		      {
			if(Terrain->IsLocked())
			  {
			    if(IsPlayer())
			      {
				ADD_MESSAGE("The %s is locked.", Terrain->GetNameSingular().CStr()); /* not sure if this is better than "the door is locked", but I guess it _might_ be slighltly better */
				return false;
			      }
			    else if(Important && CheckKick())
			      {
				room* Room = MoveToSquare[c]->GetRoom();

				if(!Room || Room->AllowKick(this, MoveToSquare[c]))
				  {
				    int HP = Terrain->GetHP();

				    if(CanBeSeenByPlayer())
				      ADD_MESSAGE("%s kicks %s.", CHAR_NAME(DEFINITE), Terrain->CHAR_NAME(DEFINITE));

				    Kick(MoveToSquare[c], Direction);
				    olterrain* NewTerrain = MoveToSquare[c]->GetOLTerrain();

				    if(NewTerrain == Terrain && Terrain->GetHP() == HP) // BUG!
				      {
					Illegal.insert(MoveTo);
					CreateRoute();
				      }

				    return true;
				  }
			      }
			  }
			else
			  {
			    if(!IsPlayer() || game::BoolQuestion(CONST_S("Do you want to open ") + Terrain->GetName(DEFINITE) + "? [y/N]", false, game::GetMoveCommandKeyBetweenPoints(PLAYER->GetPos(), MoveToSquare[0]->GetPos())))
			      {
				OpenPos(MoveToSquare[c]->GetPos());
				return true;
			      }
			    else
			      return false;
			  }
		      }
		    else
		      {
			if(IsPlayer())
			  {
			    ADD_MESSAGE("This monster type cannot open doors.");
			    return false;
			  }
			else if(Important)
			  {
			    Illegal.insert(MoveTo);
			    return CreateRoute();
			  }
		      }
		  }
	      }

	  return false;
	}
      else
	{
	  if(IsPlayer() && !IsStuck() && GetLevel()->IsOnGround() && game::BoolQuestion(CONST_S("Do you want to leave ") + game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrentLevelIndex()) + "? [y/N]"))
	    {
	      if(HasPetrussNut() && !HasGoldenEagleShirt())
		{
		  game::TextScreen(CONST_S("An undead and sinister voice greets you as you leave the city behind:\n\n\"MoRtAl! ThOu HaSt SlAuGtHeReD pEtRuS aNd PlEaSeD mE!\nfRoM tHiS dAy On, ThOu ArT tHe DeArEsT sErVaNt Of AlL eViL!\"\n\nYou are victorious!"));
		  game::GetCurrentArea()->SendNewDrawRequest();
		  game::DrawEverything();
		  ShowAdventureInfo();
		  festring Msg = CONST_S("killed Petrus and became the Avatar of Chaos");
		  PLAYER->AddScoreEntry(Msg, 3, false);
		  game::End(Msg);
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
      /** No multitile support */

      if(CanMove()
      && GetArea()->IsValidPos(MoveTo)
      && (CanMoveOn(GetNearWSquare(MoveTo))
       || game::GoThroughWallsCheatIsActive()))
	{
	  if(!game::GoThroughWallsCheatIsActive())
	    {
	      charactervector& V = game::GetWorldMap()->GetPlayerGroup();
	      bool Discard = false;

	      for(uint c = 0; c < V.size(); ++c)
		if(!V[c]->CanMoveOn(GetNearWSquare(MoveTo)))
		  {
		    if(!Discard)
		      {
			ADD_MESSAGE("One or more of your team members cannot cross this terrain.");

			if(!game::BoolQuestion("Discard them? [y/N]"))
			  return false;

			Discard = true;
		      }

		    if(Discard)
		      delete V[c];

		    V.erase(V.begin() + c--);
		  }
	    }

	  Move(MoveTo, false);
	  return true;
	}
      else
	return false;
    }
}

void character::CreateCorpse(lsquare* Square)
{
  if(!BodyPartsDisappearWhenSevered() && !game::AllBodyPartsVanish())
    {
      corpse* Corpse = new corpse(0, NO_MATERIALS);
      Corpse->SetDeceased(this);
      Square->AddItem(Corpse);
      Disable();
    }
  else
    SendToHell();
}

void character::Die(const character* Killer, const festring& Msg, bool ForceMsg, bool AllowCorpse, bool AllowMsg)
{
  /* Note: This function musn't delete any objects, since one of these may be
     the one currently processed by pool::Be()! */

  if(!IsEnabled())
    return;

  RemoveTraps();

  if(IsPlayer())
    {
      ADD_MESSAGE("You die.");

      if(game::WizardModeIsActive())
	{
	  game::DrawEverything();

	  if(!game::BoolQuestion(CONST_S("Do you want to do this, cheater? [y/n]"), REQUIRES_ANSWER))
	    {
	      RestoreBodyParts();
	      ResetSpoiling();
	      RestoreHP();
	      RestoreStamina();
	      ResetStates();
	      SetNP(SATIATED_LEVEL);
	      SendNewDrawRequest();
	      return;
	    }
	}
    }
  else if(CanBeSeenByPlayer() && AllowMsg)
    ProcessAndAddMessage(GetDeathMessage());
  else if(ForceMsg)
    ADD_MESSAGE("You sense the death of something.");

  if(StateIsActivated(LIFE_SAVED)
  && CanMoveOn(!game::IsInWilderness() ? GetSquareUnder() : PLAYER->GetSquareUnder()))
    {
      SaveLife();
      return;
    }

  InNoMsgMode = true;
  character* Ghost = 0;

  if(IsPlayer())
    {
      game::RemoveSaves();

      if(!game::IsInWilderness())
	{
	  Ghost = game::CreateGhost();
	  Ghost->Disable();
	}
    }

  square* SquareUnder[MAX_SQUARES_UNDER];
  lsquare** LSquareUnder = reinterpret_cast<lsquare**>(SquareUnder);
  memset(SquareUnder, 0, sizeof(SquareUnder));

  if(IsPlayer() || !game::IsInWilderness())
    {
      for(int c = 0; c < SquaresUnder; ++c)
	SquareUnder[c] = GetSquareUnder(c);

      Remove();
    }
  else
    {
      charactervector& V = game::GetWorldMap()->GetPlayerGroup();
      V.erase(std::find(V.begin(), V.end(), this));
    }

  if(!game::IsInWilderness())
    {
      if(!StateIsActivated(POLYMORPHED))
	{
	  if(!IsPlayer() && !IsTemporary())
	    game::SignalDeath(this, Killer, Msg);

	  if(AllowCorpse)
	    CreateCorpse(LSquareUnder[0]);
	  else
	    SendToHell();
	}
      else
	{
	  if(!IsPlayer() && !IsTemporary())
	    game::SignalDeath(GetPolymorphBackup(), Killer, Msg);

	  GetPolymorphBackup()->CreateCorpse(LSquareUnder[0]);
	  GetPolymorphBackup()->SetPolymorphed(false);
	  SetPolymorphBackup(0);
	  SendToHell();
	}
    }
  else
    SendToHell();

  if(IsPlayer())
    {
      if(!game::IsInWilderness())
	for(int c = 0; c < GetSquaresUnder(); ++c)
	  LSquareUnder[c]->SetTemporaryEmitation(GetEmitation());

      ShowAdventureInfo();

      if(!game::IsInWilderness())
	for(int c = 0; c < GetSquaresUnder(); ++c)
	  LSquareUnder[c]->SetTemporaryEmitation(0);
    }

  if(!game::IsInWilderness())
    {
      if(GetSquaresUnder() == 1)
	{
	  stack* StackUnder = LSquareUnder[0]->GetStack();
	  GetStack()->MoveItemsTo(StackUnder);
	  DoForBodyParts(this, &bodypart::DropEquipment, StackUnder);

	  /*for(int c = 0; c < BodyParts; ++c)
	    {
	      bodypart* BodyPart = GetBodyPart(c);

	      if(BodyPart)
		BodyPart->DropEquipment(StackUnder);
	    }*/
	}
      else
	{
	  while(GetStack()->GetItems())
	    GetStack()->GetBottom()->MoveTo(LSquareUnder[RAND_N(GetSquaresUnder())]->GetStack());

	  for(int c = 0; c < BodyParts; ++c)
	    {
	      bodypart* BodyPart = GetBodyPart(c);

	      if(BodyPart)
		BodyPart->DropEquipment(LSquareUnder[RAND_N(GetSquaresUnder())]->GetStack());
	    }
	}
    }

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(0);

  InNoMsgMode = false;

  if(IsPlayer())
    {
      AddScoreEntry(Msg);

      if(!game::IsInWilderness())
	{
	  Ghost->PutTo(LSquareUnder[0]->GetPos());
	  Ghost->Enable();
	  game::CreateBone();
	}

      game::TextScreen(CONST_S("Unfortunately you died during your journey. The high priest is not happy."));
      game::End(Msg);
    }
}

void character::AddMissMessage(const character* Enemy) const
{
  festring Msg;

  if(Enemy->IsPlayer())
    Msg = GetDescription(DEFINITE) + " misses you!";
  else if(IsPlayer())
    Msg = CONST_S("You miss ") + Enemy->GetDescription(DEFINITE) + '!';
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg = GetDescription(DEFINITE) + " misses " + Enemy->GetDescription(DEFINITE) + '!';
  else
    return;

  ADD_MESSAGE("%s", Msg.CStr());
}

void character::AddBlockMessage(const character* Enemy, const item* Blocker, const festring& HitNoun, bool Partial) const
{
  festring Msg;
  festring BlockVerb = (Partial ? " to partially block the " : " to block the ") + HitNoun;

  if(IsPlayer())
    Msg << "You manage" << BlockVerb << " with your " << Blocker->GetName(UNARTICLED) << '!';
  else if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
    {
      if(CanBeSeenByPlayer())
	Msg << GetName(DEFINITE) << " manages" << BlockVerb << " with " << GetPossessivePronoun() << ' ' << Blocker->GetName(UNARTICLED) << '!';
      else
	Msg << "Something manages" << BlockVerb << " with something!";
    }
  else
    return;

  ADD_MESSAGE("%s", Msg.CStr());
}

void character::AddPrimitiveHitMessage(const character* Enemy, const festring& FirstPersonHitVerb, const festring& ThirdPersonHitVerb, int BodyPart) const
{
  festring Msg;
  festring BodyPartDescription;

  if(BodyPart && (Enemy->CanBeSeenByPlayer() || Enemy->IsPlayer()))
    BodyPartDescription << " in the " << Enemy->GetBodyPartName(BodyPart);

  if(Enemy->IsPlayer())
    Msg << GetDescription(DEFINITE) << ' ' << ThirdPersonHitVerb << " you" << BodyPartDescription << '!';
  else if(IsPlayer())
    Msg << "You " << FirstPersonHitVerb << ' ' << Enemy->GetDescription(DEFINITE) << BodyPartDescription << '!';
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg << GetDescription(DEFINITE) << ' ' << ThirdPersonHitVerb << ' ' << Enemy->GetDescription(DEFINITE) + BodyPartDescription << '!';
  else
    return;

  ADD_MESSAGE("%s", Msg.CStr());
}

void character::AddWeaponHitMessage(const character* Enemy, const item* Weapon, int BodyPart, bool Critical) const
{
  festring Msg;
  festring BodyPartDescription;

  if(BodyPart && (Enemy->CanBeSeenByPlayer() || Enemy->IsPlayer()))
    BodyPartDescription << " in the " << Enemy->GetBodyPartName(BodyPart);

  if(Enemy->IsPlayer())
    {
      Msg << GetDescription(DEFINITE) << (Critical ? " critically hits you" : " hits you") << BodyPartDescription;

      if(CanBeSeenByPlayer())
	Msg << " with " << GetPossessivePronoun() << ' ' << Weapon->GetName(UNARTICLED);

      Msg << '!';
    }
  else if(IsPlayer())
    Msg << "You " << (Critical ? "critically hit " : "hit ") << Enemy->GetDescription(DEFINITE) << BodyPartDescription << '!';
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    {
      Msg << GetDescription(DEFINITE) << (Critical ? " critically hits " : " hits ") << Enemy->GetDescription(DEFINITE) << BodyPartDescription;

      if(CanBeSeenByPlayer())
	Msg << " with " << GetPossessivePronoun() << ' ' << Weapon->GetName(UNARTICLED);

      Msg << '!';
    }
  else
    return;

  ADD_MESSAGE("%s", Msg.CStr());
}

bool character::HasHeadOfElpuri() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsHeadOfElpuri())
      return true;

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsHeadOfElpuri())
      return true;

  return false;
}

bool character::HasPetrussNut() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsPetrussNut())
      return true;

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->IsPetrussNut())
      return true;

  return false;
}

bool character::HasGoldenEagleShirt() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsGoldenEagleShirt())
      return true;

  for(int c = 0; c < GetEquipmentSlots(); ++c)
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

  for(int c = 0; c < GetEquipmentSlots(); ++c)
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
      if(!GetLSquareUnder()->IsDark() || game::GetSeeWholeMapCheatMode())
	{
	  if(StateIsActivated(CONFUSED) && !(RAND() & 7))
	    {
	      if(!ToBeRead->IsDestroyable(this))
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
  int OldBurdenState = BurdenState;
  long SumOfMasses = GetCarriedWeight();
  long CarryingStrengthUnits = long(GetCarryingStrength()) * 2500;

  if(SumOfMasses > (CarryingStrengthUnits << 1) + CarryingStrengthUnits)
    BurdenState = OVER_LOADED;
  else if(SumOfMasses > CarryingStrengthUnits << 1)
    BurdenState = STRESSED;
  else if(SumOfMasses > CarryingStrengthUnits)
    BurdenState = BURDENED;
  else
    BurdenState = UNBURDENED;

  if(!Initializing && BurdenState != OldBurdenState)
    CalculateBattleInfo();
}

void character::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
  Stack->Save(SaveFile);
  SaveFile << ID;
  int c;

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    SaveFile << BaseExperience[c];

  SaveFile << NP << AP << Stamina << GenerationDanger << ScienceTalks;
  SaveFile << TemporaryState << EquipmentState << Money << GoingTo << RegenerationCounter << Route << Illegal;
  SaveFile << IsEnabled() << Polymorphed << HomeData << BlocksSinceLastTurn << CommandFlags;
  SaveFile << WarnFlags;

  for(c = 0; c < BodyParts; ++c)
    SaveFile << BodyPartSlot[c] << OriginalBodyPartID[c];

  SaveLinkedList(SaveFile, TrapData);
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
    SaveFile << CWeaponSkill[c];

  /*if(IsStuck())
    SaveFile << bool(true) << GetStackUnder()->SearchItem(StuckTo) << (int)StuckToBodyPart;
  else
    SaveFile << bool(false);*/

  SaveFile << (ushort)GetConfig();
}

void character::Load(inputfile& SaveFile)
{
  LoadSquaresUnder();
  int c;
  Stack->Load(SaveFile);
  SaveFile >> ID;
  game::AddCharacterID(this, ID);

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    SaveFile >> BaseExperience[c];

  SaveFile >> NP >> AP >> Stamina >> GenerationDanger >> ScienceTalks;;
  SaveFile >> TemporaryState >> EquipmentState >> Money >> GoingTo >> RegenerationCounter >> Route >> Illegal;

  if(!ReadType<bool>(SaveFile))
    Disable();

  SaveFile >> Polymorphed >> HomeData >> BlocksSinceLastTurn >> CommandFlags;
  SaveFile >> WarnFlags;
  WarnFlags &= ~WARNED;

  for(c = 0; c < BodyParts; ++c)
    {
      SaveFile >> BodyPartSlot[c] >> OriginalBodyPartID[c];

      item* BodyPart = *BodyPartSlot[c];

      if(BodyPart)
	BodyPart->Disable();
    }

  LoadLinkedList(SaveFile, TrapData);
  SaveFile >> Action;

  if(Action)
    Action->SetActor(this);

  for(c = 0; c < STATES; ++c)
    SaveFile >> TemporaryStateCounter[c];

  if(ReadType<bool>(SaveFile))
    SetTeam(game::GetTeam(ReadType<int>(SaveFile)));

  if(ReadType<bool>(SaveFile))
    GetTeam()->SetLeader(this);

  SaveFile >> AssignedName >> PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    SaveFile >> CWeaponSkill[c];

  /*if(ReadType<bool>(SaveFile))
    {
      StuckTo = GetStackUnder()->GetItem(ReadType<int>(SaveFile));
      SaveFile >> (int&)StuckToBodyPart;
    }*/

  databasecreator<character>::InstallDataBase(this, ReadType<ushort>(SaveFile));

  if(IsEnabled() && !game::IsInWilderness())
    for(c = 1; c < GetSquaresUnder(); ++c)
      GetSquareUnder(c)->SetCharacter(this);
}

bool character::Engrave(const festring& What)
{
  GetLSquareUnder()->Engrave(What);
  return true;
}

bool character::MoveRandomly()
{
  if(!IsEnabled())
    return false;

  for(int c = 0; c < 10; ++c)
    {
      vector2d ToTry = game::GetMoveVector(RAND() & 7);

      if(GetLevel()->IsValidPos(GetPos() + ToTry))
	{
	  lsquare* Square = GetNearLSquare(GetPos() + ToTry);

	  if(!Square->IsDangerousForAIToStepOn(this)
	  && !Square->IsScaryForAIToStepOn(this)
	  && TryMove(ToTry, false, false))
	    return true;
	}
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

void character::AddScoreEntry(const festring& Description, double Multiplier, bool AddEndLevel) const
{
  if(!game::WizardModeIsReallyActive())
    {
      highscore HScore;
      festring Desc = game::GetPlayerName();
      Desc << ", " << Description;

      if(AddEndLevel)
	Desc << " in " + (game::IsInWilderness() ? "the World map" : game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrentLevelIndex()));

      HScore.Add(long(game::GetScore() * Multiplier), Desc);
      HScore.Save();
    }
}

bool character::CheckDeath(const festring& Msg, const character* Murderer, bool ForceMsg, bool ForceDeath, bool AllowCorpse, bool AllowMsg)
{
  if(!IsEnabled())
    return true;

  if(game::IsSumoWrestling() && IsDead())
    {
      game::EndSumoWrestling(IsPlayer());
      return true;
    }

  if(ForceDeath || IsDead())
    {
      if(Murderer && Murderer->IsPlayer() && GetTeam()->GetKillEvilness())
	game::DoEvilDeed(GetTeam()->GetKillEvilness());

      festring SpecifierMsg;
      int SpecifierParts = 0;

      if(GetPolymorphBackup())
	{
	  SpecifierMsg << " polymorphed into ";
	  id::AddName(SpecifierMsg, INDEFINITE);
	  ++SpecifierParts;
	}

      if(IsStuck())
	{
	  if(SpecifierParts++)
	    SpecifierMsg << " and";

	  SpecifierMsg << " caught in " << GetTrapDescription();
	}

      if(GetAction())
	{
	  festring ActionMsg = GetAction()->GetDeathExplanation();

	  if(!ActionMsg.IsEmpty())
	    {
	      if(SpecifierParts > 1)
		SpecifierMsg = ActionMsg << ',' << SpecifierMsg;
	      else
		{
		  if(SpecifierParts)
		    SpecifierMsg << " and";

		  SpecifierMsg << ActionMsg;
		}

	      ++SpecifierParts;
	    }
	}

      festring NewMsg = Msg;

      if(SpecifierParts)
	NewMsg << " while" << SpecifierMsg;

      if(IsPlayer() && game::WizardModeIsActive())
	ADD_MESSAGE("Death message: %s. Score: %d.", NewMsg.CStr(), game::GetScore());

      Die(Murderer, NewMsg, ForceMsg, AllowCorpse, AllowMsg);
      return true;
    }
  else
    return false;
}

bool character::CheckStarvationDeath(const festring& Msg)
{
  if(GetNP() < 1 && UsesNutrition())
    return CheckDeath(Msg, 0, false, true);
  else
    return false;
}

void character::ThrowItem(int Direction, item* ToBeThrown)
{
  if(Direction > 7)
    ABORT("Throw in TOO odd direction...");

  ToBeThrown->Fly(this, Direction, GetAttribute(ARM_STRENGTH));
}

void character::HasBeenHitByItem(character* Thrower, item* Thingy, int Damage, double ToHitValue, int Direction)
{
  if(IsPlayer())
    ADD_MESSAGE("%s hits you.", Thingy->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s hits %s.", Thingy->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));

  int BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, DodgeValue);
  int WeaponSkillHits = CalculateWeaponSkillHits(Thrower);
  int DoneDamage = ReceiveBodyPartDamage(Thrower, Damage, PHYSICAL_DAMAGE, BodyPart, Direction);
  bool Succeeded = (GetBodyPart(BodyPart) && HitEffect(Thrower, Thingy, Thingy->GetPos(), THROW_ATTACK, BodyPart, Direction, !DoneDamage)) || DoneDamage;

  if(Succeeded)
    Thrower->WeaponSkillHit(Thingy, THROW_ATTACK, WeaponSkillHits);

  festring DeathMsg = CONST_S("killed by a flying ") + Thingy->GetName(UNARTICLED);

  if(CheckDeath(DeathMsg, Thrower))
    return;

  if(Thrower->CanBeSeenByPlayer())
    DeActivateVoluntaryAction(CONST_S("The attack of ") + Thrower->GetName(DEFINITE) + CONST_S(" interrupts you."));
  else
    DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));
}

bool character::DodgesFlyingItem(item* Item, double ToHitValue)
{
  return !Item->EffectIsGood() && RAND() % int(100 + ToHitValue / DodgeValue * 100) < 100;
}

void character::GetPlayerCommand()
{
  bool HasActed = false;

  while(!HasActed)
    {
      game::DrawEverything();

      if(game::GetDangerFound())
	{
	  if(game::GetDangerFound() > 500.)
	    {
	      if(game::GetCausePanicFlag())
		{
		  game::SetCausePanicFlag(false);
		  BeginTemporaryState(PANIC, 500 + RAND_N(500));
		}

	      game::AskForKeyPress(CONST_S("You are horrified by your situation! [press any key to continue]"));
	    }
	  else if(ivanconfig::GetWarnAboutDanger())
	    if(game::GetDangerFound() > 50.)
	      game::AskForKeyPress(CONST_S("You sense great danger! [press any key to continue]"));
	    else
	      game::AskForKeyPress(CONST_S("You sense danger! [press any key to continue]"));

	  game::SetDangerFound(0);
	}

      game::SetIsInGetCommand(true);
      int Key = GET_KEY();
      game::SetIsInGetCommand(false);

      if(Key != '+' && Key != '-' && Key != 'M') // gum
	msgsystem::ThyMessagesAreNowOld();

      bool ValidKeyPressed = false;
      int c;

      for(c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	if(Key == game::GetMoveCommandKey(c))
	  {
	    HasActed = TryMove(ApplyStateModification(game::GetMoveVector(c)), true, game::PlayerIsRunning());
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
	ADD_MESSAGE("Unknown key. Press '?' for a list of commands.", game::Insult());
    }

  game::IncreaseTurn();
}

void character::Vomit(vector2d Pos, int Amount, bool ShowMsg)
{
  if(!CanVomit())
    return;

  if(ShowMsg)
    {
      if(IsPlayer())
	ADD_MESSAGE("You vomit.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s vomits.", CHAR_NAME(DEFINITE));
    }

  if(IsPlayer())
    {
      EditExperience(ARM_STRENGTH, -75, 1 << 9);
      EditExperience(LEG_STRENGTH, -75, 1 << 9);
      EditNP(-2500 - RAND() % 2501);
      CheckStarvationDeath(CONST_S("vomited himself to death"));
    }

  if(StateIsActivated(PARASITIZED) && !(RAND() & 7))
    {
      if(IsPlayer())
	ADD_MESSAGE("You notice a dead broad tapeworm among your former stomach contents.");

      DeActivateTemporaryState(PARASITIZED);
    }

  if(!game::IsInWilderness())
    GetNearLSquare(Pos)->ReceiveVomit(this, new liquid(GetVomitMaterial(), long(sqrt(GetBodyVolume()) * Amount / 1000)));
}

bool character::Polymorph(character* NewForm, int Counter)
{
  if(!IsPolymorphable() || (!IsPlayer() && game::IsInWilderness()))
    {
      delete NewForm;
      return false;
    }

  RemoveTraps();

  if(GetAction())
    GetAction()->Terminate(false);

  NewForm->SetAssignedName("");

  if(IsPlayer())
    ADD_MESSAGE("Your body glows in a crimson light. You transform into %s!", NewForm->CHAR_NAME(INDEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s glows in a crimson light and %s transforms into %s!", CHAR_NAME(DEFINITE), GetPersonalPronoun().CStr(), NewForm->CHAR_NAME(INDEFINITE));

  InNoMsgMode = NewForm->InNoMsgMode = true;
  NewForm->ChangeTeam(GetTeam());
  NewForm->GenerationDanger = GenerationDanger;

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(NewForm);

  vector2d Pos = GetPos();
  Remove();
  NewForm->PutToOrNear(Pos);
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
  DonateEquipmentTo(NewForm);
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

void character::BeKicked(character* Kicker, item* Boot, bodypart* Leg, vector2d HitPos, double KickDamage, double ToHitValue, int Success, int Direction, bool Critical, bool ForceHit)
{
  switch(TakeHit(Kicker, Boot, Leg, HitPos, KickDamage, ToHitValue, Success, KICK_ATTACK, Direction, Critical, ForceHit))
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case DID_NO_DAMAGE:
      if(IsEnabled() && !CheckBalance(KickDamage))
	{
	  if(IsPlayer())
	    ADD_MESSAGE("The kick throws you off balance.");
	  else if(Kicker->IsPlayer())
	    ADD_MESSAGE("The kick throws %s off balance.", CHAR_DESCRIPTION(DEFINITE));

	  vector2d FallToPos = GetPos() + game::GetMoveVector(Direction);
	  FallTo(Kicker, FallToPos);
	}
    }
}

/* Return true if still in balance */

bool character::CheckBalance(double KickDamage)
{
  return !CanMove()
      || IsStuck()
      || !KickDamage
      || (!(GetMoveType() & FLY)
       && KickDamage * 5 < RAND() % GetSize());
}

void character::FallTo(character* GuiltyGuy, vector2d Where)
{
  EditAP(-500);
  lsquare* MoveToSquare[MAX_SQUARES_UNDER];
  int Squares = CalculateNewSquaresUnder(MoveToSquare, Where);

  if(Squares)
    {
      bool NoRoom = false;

      for(int c = 0; c < Squares; ++c)
	{
	  olterrain* Terrain = MoveToSquare[c]->GetOLTerrain();

	  if(Terrain && !CanMoveOn(Terrain))
	    {
	      NoRoom = true;
	      break;
	    }
	}

      if(NoRoom)
	{
	  if(HasHead())
	    {
	      if(IsPlayer()) 
		ADD_MESSAGE("You hit your head on the wall.");
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s hits %s head on the wall.", CHAR_NAME(DEFINITE), GetPossessivePronoun().CStr());
	    }

	  ReceiveDamage(GuiltyGuy, 1 + RAND() % 5, PHYSICAL_DAMAGE, HEAD);
	  CheckDeath(CONST_S("killed by hitting a wall"), GuiltyGuy);
	}
      else
	{
	  if(IsFreeForMe(MoveToSquare[0]))
	    Move(Where, true);

	  // Place code that handles characters bouncing to each other here
	}
    }
}

bool character::CheckCannibalism(const material* What) const
{ 
  return GetTorso()->GetMainMaterial()->IsSameAs(What); 
}

void character::StandIdleAI()
{
  SeekLeader(GetLeader());

  if(CheckForEnemies(true, true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader(GetLeader()))
    return;

  if(CheckForDoors())
    return;

  if(MoveTowardsHomePos())
    return;

  EditAP(-1000);
}

bool character::LoseConsciousness(int Counter, bool HungerFaint)
{
  if(!AllowUnconsciousness())
    return false;

  action* Action = GetAction();

  if(Action)
    {
      if(HungerFaint && !Action->AllowUnconsciousness())
	return false;

      if(Action->IsUnconsciousness())
	{
	  static_cast<unconsciousness*>(Action)->RaiseCounterTo(Counter);
	  return true;
	}
      else
	Action->Terminate(false);
    }

  if(IsPlayer())
    ADD_MESSAGE("You lose consciousness.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s loses consciousness.", CHAR_NAME(DEFINITE));

  unconsciousness* Unconsciousness = new unconsciousness(this);
  Unconsciousness->SetCounter(Counter);
  SetAction(Unconsciousness);
  return true;
}

void character::DeActivateVoluntaryAction(const festring& Reason)
{
  if(GetAction() && GetAction()->IsVoluntary())
    {
      if(IsPlayer())
	{
	  if(Reason.GetSize())
	    ADD_MESSAGE("%s", Reason.CStr());

	  if(game::BoolQuestion(CONST_S("Continue ") + GetAction()->GetDescription() + "? [y/N]"))
	    GetAction()->ActivateInDNDMode();
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

  vector2d Pos = GetPos();

  for(int c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled()
	&& (*i)->CanBeSeenBy(this, false, true)
	&& ((*i)->CanMove() || (*i)->GetPos().IsAdjacent(Pos))
	&& (*i)->CanAttack())
	  {
	    if(IsPlayer())
	      {
		ADD_MESSAGE("%s seems to be hostile.", (*i)->CHAR_NAME(DEFINITE));

		if(game::BoolQuestion(CONST_S("Continue ") + GetAction()->GetDescription() + "? [y/N]"))
		  GetAction()->ActivateInDNDMode();
		else
		  GetAction()->Terminate(false);
	      }
	    else
	      GetAction()->Terminate(false);

	    return;
	  }
}

bool character::CheckForEnemies(bool CheckDoors, bool CheckGround, bool MayMoveRandomly, bool RunTowardsTarget)
{
  if(!IsEnabled())
    return false;

  bool HostileCharsNear = false;
  character* NearestChar = 0;
  long NearestDistance = 0x7FFFFFFF;
  vector2d Pos = GetPos();

  for(int c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled() && GetAttribute(WISDOM) < (*i)->GetAttackWisdomLimit())
	  {
	    long ThisDistance = Max<long>(abs((*i)->GetPos().X - Pos.X), abs((*i)->GetPos().Y - Pos.Y));

	    if(ThisDistance <= GetLOSRangeSquare())
	      HostileCharsNear = true;

	    if((ThisDistance < NearestDistance
	     || (ThisDistance == NearestDistance && !(RAND() % 3)))
	    && (*i)->CanBeSeenBy(this, false, IsGoingSomeWhere())
	    && (!IsGoingSomeWhere() || HasClearRouteTo((*i)->GetPos())))
	      {
		NearestChar = *i;
		NearestDistance = ThisDistance;
	      }
	  }

  if(NearestChar)
    {
      if(GetAttribute(INTELLIGENCE) >= 10 || IsSpy())
	game::CallForAttention(GetPos(), 100);

      if(SpecialEnemySightedReaction(NearestChar))
	return true;

      if(IsExtraCoward() && !StateIsActivated(PANIC) && NearestChar->GetRelativeDanger(this) >= 0.5)
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s sees %s.", CHAR_NAME(DEFINITE), NearestChar->CHAR_DESCRIPTION(DEFINITE));

	  BeginTemporaryState(PANIC, 500 + RAND() % 500);
	}

      if(!IsRetreating())
	{
	  if(CheckGround && NearestDistance > 2 && CheckForUsefulItemsOnGround(false))
	    return true;

	  SetGoingTo(NearestChar->GetPos());
	}
      else
	SetGoingTo((Pos << 1) - NearestChar->GetPos());

      return MoveTowardsTarget(true);
    }
  else
    {
      character* Leader = GetLeader();

      if(Leader == this)
	Leader = 0;

      if(!Leader && IsGoingSomeWhere())
	{
	  if(!MoveTowardsTarget(RunTowardsTarget))
	    {
	      TerminateGoingTo();
	      return false;
	    }
	  else
	    {
	      if(!IsEnabled())
		return true;

	      if(GetPos() == GoingTo)
		TerminateGoingTo();

	      return true;
	    }
	}
      else
	{
	  if((!Leader || (Leader && !IsGoingSomeWhere())) && HostileCharsNear)
	    {
	      if(CheckDoors && CheckForDoors())
		return true;

	      if(CheckGround && CheckForUsefulItemsOnGround())
		return true;

	      if(MayMoveRandomly && MoveRandomly()) // one has heard that an enemy is near but doesn't know where
		return true;
	    }

	  return false;
	}
    }
}

bool character::CheckForDoors()
{
  if(!CanOpen() || !IsEnabled())
    return false;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square && Square->GetOLTerrain() && Square->GetOLTerrain()->Open(this))
	return true;
    }

  return false;
}

bool character::CheckForUsefulItemsOnGround(bool CheckFood)
{
  if(StateIsActivated(PANIC) || !IsEnabled())
    return false;

  itemvector ItemVector;
  GetStackUnder()->FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->CanBeSeenBy(this) && ItemVector[c]->IsPickable(this))
      {
	if(!(CommandFlags & DONT_CHANGE_EQUIPMENT)
	&& TryToEquip(ItemVector[c]))
	  return true;

	if(CheckFood && UsesNutrition() && !CheckIfSatiated()
	&& TryToConsume(ItemVector[c]))
	  return true;
      }

  return false;
}

bool character::FollowLeader(character* Leader)
{
  if(!Leader || Leader == this || !IsEnabled())
    return false;

  if(CommandFlags & FOLLOW_LEADER && Leader->CanBeSeenBy(this))
    {
      vector2d Distance = GetPos() - GoingTo;

      if(abs(Distance.X) <= 2 && abs(Distance.Y) <= 2)
	return false;
      else
	return MoveTowardsTarget(false);
    }
  else
    if(IsGoingSomeWhere())
      if(!MoveTowardsTarget(true))
	{
	  TerminateGoingTo();
	  return false;
	}
      else
	return true;
    else
      return false;
}

void character::SeekLeader(const character* Leader)
{
  if(Leader && Leader != this)
    if(Leader->CanBeSeenBy(this))
      {
	if(CommandFlags & FOLLOW_LEADER)
	  SetGoingTo(Leader->GetPos());
      }
    else if(!IsGoingSomeWhere())
      {
	team* Team = GetTeam();

	for(std::list<character*>::const_iterator i = Team->GetMember().begin();
	    i != Team->GetMember().end(); ++i)
	  if((*i)->IsEnabled()
	  && (*i)->GetID() != GetID()
	  && (CommandFlags & FOLLOW_LEADER)
	  == ((*i)->CommandFlags & FOLLOW_LEADER)
	  && (*i)->CanBeSeenBy(this))
	    {
	      vector2d Pos = (*i)->GetPos();
	      vector2d Distance = GetPos() - Pos;

	      if(abs(Distance.X) > 2 && abs(Distance.Y) > 2)
		{
		  SetGoingTo(Pos);
		  break;
		}
	    }
      }
}

int character::GetMoveEase() const
{
  switch(BurdenState)
    {
    case OVER_LOADED:
    case STRESSED: return 50;
    case BURDENED: return 75;
    case UNBURDENED: return 100;
    }

  return 666;
}

int character::GetLOSRange() const
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
	{
	  const char* CrawlVerb = StateIsActivated(LEVITATION) ? "float" : "crawl";
	  ADD_MESSAGE("You try very hard to %s forward. But your load is too heavy.", CrawlVerb);
	  EditAP(-1000);
	  return true;
	}
      else
	return false;
    }

  double Danger = GetRelativeDanger(Who);

  if(IsPlayer())
    Danger *= 4;
  else if(Who->IsPlayer())
    Danger *= .25;

  if(IsSmall() && Who->IsSmall() 
   && (Forced || (Who->CanBeDisplaced() && Danger > 1.))
   && !IsStuck() && !Who->IsStuck()
   && (!Who->GetAction() || Who->GetAction()->TryDisplace())
   && CanMove() && Who->CanMove() && Who->CanMoveOn(GetLSquareUnder()))
    {
      if(IsPlayer())
	ADD_MESSAGE("You displace %s!", Who->CHAR_DESCRIPTION(DEFINITE));
      else if(Who->IsPlayer())
	ADD_MESSAGE("%s displaces you!", CHAR_DESCRIPTION(DEFINITE));
      else if(CanBeSeenByPlayer() || Who->CanBeSeenByPlayer())
	ADD_MESSAGE("%s displaces %s!", CHAR_DESCRIPTION(DEFINITE), Who->CHAR_DESCRIPTION(DEFINITE));

      lsquare* OldSquareUnder1[MAX_SQUARES_UNDER];
      lsquare* OldSquareUnder2[MAX_SQUARES_UNDER];
      int c;

      for(c = 0; c < GetSquaresUnder(); ++c)
	OldSquareUnder1[c] = GetLSquareUnder(c);

      for(c = 0; c < Who->GetSquaresUnder(); ++c)
	OldSquareUnder2[c] = Who->GetLSquareUnder(c);

      vector2d Pos = GetPos();
      vector2d WhoPos = Who->GetPos();
      Remove();
      Who->Remove();
      PutTo(WhoPos);
      Who->PutTo(Pos);
      EditAP(-GetMoveAPRequirement(GetSquareUnder()->GetEntryDifficulty()) - 500);
      EditNP(-12 * GetSquareUnder()->GetEntryDifficulty());
      EditExperience(AGILITY, 75, GetSquareUnder()->GetEntryDifficulty() << 7);

      if(IsPlayer())
	ShowNewPosInfo();

      if(Who->IsPlayer())
	Who->ShowNewPosInfo();

      SignalStepFrom(OldSquareUnder1);
      Who->SignalStepFrom(OldSquareUnder2);
      return true;
    }
  else
    {
      if(IsPlayer())
	{
	  ADD_MESSAGE("%s resists!", Who->CHAR_DESCRIPTION(DEFINITE));
	  EditAP(-1000);
	  return true;
	}
      else
	return false;
    }
}

void character::SetNP(long What)
{
  int OldGetHungerState = GetHungerState();
  NP = What;

  if(IsPlayer())
    {
      if(GetHungerState() == STARVING && OldGetHungerState > STARVING)
	DeActivateVoluntaryAction(CONST_S("You are getting really hungry."));
      else if(GetHungerState() == VERY_HUNGRY && OldGetHungerState > VERY_HUNGRY)
	DeActivateVoluntaryAction(CONST_S("You are getting very hungry."));
      else if(GetHungerState() == HUNGRY && OldGetHungerState > HUNGRY)
	DeActivateVoluntaryAction(CONST_S("You are getting hungry."));
    }
}

void character::ShowNewPosInfo() const
{
  msgsystem::EnterBigMessageMode();
  vector2d Pos = GetPos();

  if(Pos.X < game::GetCamera().X + 3 || Pos.X >= game::GetCamera().X + game::GetScreenXSize() - 3)
    game::UpdateCameraX();

  if(Pos.Y < game::GetCamera().Y + 3 || Pos.Y >= game::GetCamera().Y + game::GetScreenYSize() - 3)
    game::UpdateCameraY();

  game::SendLOSUpdateRequest();
  game::DrawEverythingNoBlit();
  UpdateESPLOS();

  if(!game::IsInWilderness())
    {
      if(GetLSquareUnder()->IsDark() && !game::GetSeeWholeMapCheatMode())
	ADD_MESSAGE("It's dark in here!");

      GetLSquareUnder()->ShowSmokeMessage();
      itemvectorvector PileVector;
      GetStackUnder()->Pile(PileVector, this, CENTER);

      if(PileVector.size())
	{
	  bool Feel = !GetLSquareUnder()->IsTransparent() || GetLSquareUnder()->IsDark();

	  if(PileVector.size() == 1)
	    {
	      if(Feel)
		ADD_MESSAGE("You feel %s lying here.", PileVector[0][0]->GetName(INDEFINITE, PileVector[0].size()).CStr(), PileVector[0].size() == 1 ? "is" : "are");
	      else
		ADD_MESSAGE("%s %s lying here.", PileVector[0][0]->GetName(INDEFINITE, PileVector[0].size()).CStr(), PileVector[0].size() == 1 ? "is" : "are");
	    }
	  else
	    {
	      int Items = 0;

	      for(uint c = 0; c < PileVector.size(); ++c)
		if((Items += PileVector[c].size()) > 3)
		  break;

	      if(Items > 3)
		{
		  if(Feel)
		    ADD_MESSAGE("You feel several items lying here.");
		  else
		    ADD_MESSAGE("Several items are lying here.");
		}
	      else if(Items)
		{
		  if(Feel)
		    ADD_MESSAGE("You feel a few items lying here.");
		  else
		    ADD_MESSAGE("A few items are lying here.");
		}
	    }
	}

      festring SideItems;
      GetLSquareUnder()->GetSideItemDescription(SideItems);

      if(!SideItems.IsEmpty())
	ADD_MESSAGE("There is %s.", SideItems.CStr());
		
      if(GetLSquareUnder()->HasEngravings())
	{
	  if(CanRead())
	    ADD_MESSAGE("Something has been engraved here: \"%s\"", GetLSquareUnder()->GetEngraved());
	  else
	    ADD_MESSAGE("Something has been engraved here.");
	}
    }

  msgsystem::LeaveBigMessageMode();
}

void character::Hostility(character* Enemy)
{
  if(Enemy == this || !Enemy || !Team || !Enemy->Team)
    return;

  if(!IsAlly(Enemy))
    GetTeam()->Hostility(Enemy->GetTeam());
  else if(Enemy->IsEnabled() && IsPlayer() && !Enemy->IsPlayer()) // I believe both may be players due to polymorph feature...
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
  for(int c = 0; c < 10; ++c)
    {
      vector2d ToTry = game::GetMoveVector(RAND() & 7);

      if(GetLevel()->IsValidPos(GetPos() + ToTry))
	{
	  lsquare* Square = GetNearLSquare(GetPos() + ToTry);

	  if(!Square->IsDangerousForAIToStepOn(this)
	  && !Square->IsScaryForAIToStepOn(this)
	  && (!Square->GetOLTerrain()
	   || !Square->GetOLTerrain()->IsDoor())
	  && TryMove(ToTry, false, false))
	    return true;
	}
    }

  return false;
}

void character::GoOn(go* Go, bool FirstStep)
{
  vector2d MoveVector = ApplyStateModification(game::GetMoveVector(Go->GetDirection()));
  lsquare* MoveToSquare[MAX_SQUARES_UNDER];
  int Squares = CalculateNewSquaresUnder(MoveToSquare, GetPos() + MoveVector);

  if(!Squares || !CanMoveOn(MoveToSquare[0]))
    {
      Go->Terminate(false);
      return;
    }

  uint OldRoomIndex = GetLSquareUnder()->GetRoomIndex();
  uint CurrentRoomIndex = MoveToSquare[0]->GetRoomIndex();

  if((OldRoomIndex && (CurrentRoomIndex != OldRoomIndex)) && !FirstStep)
    {
      Go->Terminate(false);
      return;
    }

  for(int c = 0; c < Squares; ++c)
    if((MoveToSquare[c]->GetCharacter() && GetTeam() != MoveToSquare[c]->GetCharacter()->GetTeam()) || MoveToSquare[c]->IsDangerous(this))
      {
	Go->Terminate(false);
	return;
      }

  int OKDirectionsCounter = 0;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square && CanMoveOn(Square))
	++OKDirectionsCounter;
    }

  if(!Go->IsWalkingInOpen())
    {
      if(OKDirectionsCounter > 2)
	{
	  Go->Terminate(false);
	  return;
	}
    }
  else
    if(OKDirectionsCounter <= 2)
      Go->SetIsWalkingInOpen(false);

  square* BeginSquare = GetSquareUnder();

  if(!TryMove(MoveVector, true, game::PlayerIsRunning())
  || BeginSquare == GetSquareUnder()
  || (CurrentRoomIndex && (OldRoomIndex != CurrentRoomIndex)))
    {
      Go->Terminate(false);
      return;
    }

  if(GetStackUnder()->GetVisibleItems(this))
    {
      Go->Terminate(false);
      return;
    }

  game::DrawEverything();
}

void character::SetTeam(team* What)
{
  if(Team)
    int esko = esko = 2;

  Team = What;
  SetTeamIterator(What->Add(this));
}

void character::ChangeTeam(team* What)
{
  if(Team)
    Team->Remove(GetTeamIterator());

  Team = What;
  SendNewDrawRequest();

  if(Team)
    SetTeamIterator(Team->Add(this));
}

bool character::ChangeRandomAttribute(int HowMuch)
{
  for(int c = 0; c < 50; ++c)
    {
      int AttribID = RAND() % ATTRIBUTES;

      if(EditAttribute(AttribID, HowMuch))
	return true;
    }

  return false;
}

int character::RandomizeReply(long& Said, int Replies)
{
  bool NotSaid = false;

  for(int c = 0; c < Replies; ++c)
    if(!(Said & (1 << c)))
      {
	NotSaid = true;
	break;
      }

  if(!NotSaid)
    Said = 0;

  long ToSay;
  while(Said & 1 << (ToSay = RAND() % Replies));
  Said |= 1 << ToSay;
  return ToSay;
}

void character::DisplayInfo(festring& Msg)
{
  if(IsPlayer())
    Msg << " You are " << GetStandVerb() << " here.";
  else
    {
      Msg << ' ' << GetName(INDEFINITE).CapitalizeCopy() << " is " << GetStandVerb() << " here. " << GetPersonalPronoun().CapitalizeCopy();
      const char* Separator1 = GetAction() ? "," : " and";
      const char* Separator2 = " and";

      if(GetTeam() == PLAYER->GetTeam())
	Msg << " is tame";
      else
	{
	  int Relation = GetRelation(PLAYER);

	  if(Relation == HOSTILE)
	    Msg << " is hostile";
	  else if(Relation == UNCARING)
	    {
	      Msg << " does not care about you";
	      Separator1 = Separator2 = " and is";
	    }
	  else
	    Msg << " is friendly";
	}

      if(StateIsActivated(PANIC))
	{
	  Msg << Separator1 << " panicked";
	  Separator2 = " and";
	}

      if(GetAction())
	Msg << Separator2 << ' ' << GetAction()->GetDescription();

      Msg << '.';
    }
}

void character::TestWalkability()
{
  if(!IsEnabled())
    return;

  square* SquareUnder = !game::IsInWilderness()
		      ? GetSquareUnder() : PLAYER->GetSquareUnder();

  if(SquareUnder->IsFatalToStay() && !CanMoveOn(SquareUnder))
    {
      bool Alive = false;

      if(!game::IsInWilderness() || IsPlayer())
	for(int d = 0; d < GetNeighbourSquares(); ++d)
	  {
	    square* Square = GetNeighbourSquare(d);

	    if(Square && CanMoveOn(Square) && IsFreeForMe(Square))
	      {
		if(IsPlayer())
		  ADD_MESSAGE("%s.", SquareUnder->SurviveMessage(this));
		else if(CanBeSeenByPlayer())
		  ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), SquareUnder->MonsterSurviveMessage(this));
		Move(Square->GetPos(), true); // actually, this shouldn't be a teleport move
		SquareUnder->SurviveEffect(this);
		Alive = true;
		break;
	      }
	  }

      if(!Alive)
	{
	  if(IsPlayer())
	    {
	      Remove();
	      SendToHell();
	      festring DeathMsg = festring(SquareUnder->DeathMessage(this));
	      game::AskForKeyPress(DeathMsg + ". [press any key to continue]");
	      festring Msg = SquareUnder->ScoreEntry(this);
	      PLAYER->AddScoreEntry(Msg);
	      game::End(Msg);
	    }
	  else
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), SquareUnder->MonsterDeathVerb(this));

	      Die(0, SquareUnder->ScoreEntry(this), false, true, false);
	    }
	}
    }
}

int character::GetSize() const
{
  return GetTorso()->GetSize();
}

void character::SetMainMaterial(material* NewMaterial, int SpecialFlags)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->SetMainMaterial(NewMaterial, SpecialFlags);

  for(int c = 1; c < BodyParts; ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetMainMaterial()->GetVolume());
      GetBodyPart(c)->SetMainMaterial(NewMaterial, SpecialFlags);
    }
}

void character::ChangeMainMaterial(material* NewMaterial, int SpecialFlags)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->ChangeMainMaterial(NewMaterial, SpecialFlags);

  for(int c = 1; c < BodyParts; ++c)
    {
      NewMaterial = NewMaterial->Clone(GetBodyPart(c)->GetMainMaterial()->GetVolume());
      GetBodyPart(c)->ChangeMainMaterial(NewMaterial, SpecialFlags);
    }
}

void character::SetSecondaryMaterial(material*, int)
{
  ABORT("Illegal character::SetSecondaryMaterial call!");
}

void character::ChangeSecondaryMaterial(material*, int)
{
  ABORT("Illegal character::ChangeSecondaryMaterial call!");
}

void character::TeleportRandomly(bool Intentional)
{
  /*StuckTo = 0;
  SetStuckToBodyPart(NONE_INDEX);*/

  if(StateIsActivated(TELEPORT_CONTROL))
    {
      if(IsPlayer())
	{
	  vector2d PlayersInput = game::PositionQuestion(CONST_S("Where do you wish to teleport? [direction keys move cursor, space accepts]"), GetPos(), &game::TeleportHandler, 0, false);

	  if(PlayersInput == ERROR_VECTOR) // esc pressed
	    PlayersInput = GetPos();

	  lsquare* Square = GetNearLSquare(PlayersInput);

	  if(CanMoveOn(Square) || game::GoThroughWallsCheatIsActive())
	    {
	      if(Square->GetPos() == GetPos())
		{
		  ADD_MESSAGE("You disappear and reappear.");
		  return;
		}

	      if(IsFreeForMe(Square))
		{
		  if((PlayersInput - GetPos()).GetLengthSquare() <= GetTeleportRangeSquare())
		    {
		      EditExperience(INTELLIGENCE, 100, 1 << 10);
		      Move(PlayersInput, true);
		      return;
		    }
		  else
		    ADD_MESSAGE("You cannot concentrate yourself enough to control a teleport that far.");
		}
	      else
		ADD_MESSAGE("You feel that something weird has happened, but can't really tell what exactly.");
	    }
	  else
	    ADD_MESSAGE("You feel like having been hit by something really hard from the inside.");
	}
      else if(!Intentional)
	{
	  if(IsGoingSomeWhere())
	    {
	      vector2d Where = GetLevel()->GetNearestFreeSquare(this, GoingTo);

	      if(Where != ERROR_VECTOR
	      && (Where - GetPos()).GetLengthSquare() <= GetTeleportRangeSquare())
		{
		  EditExperience(INTELLIGENCE, 100, 1 << 10);
		  Move(Where, true);
		}
	    }

	  return;
	}
    }

  Move(GetLevel()->GetRandomSquare(this), true);

  if(!IsPlayer() && CanBeSeenByPlayer())
    ADD_MESSAGE("%s appears.", CHAR_NAME(INDEFINITE));

  if(GetAction() && GetAction()->IsVoluntary())
    GetAction()->Terminate(false);
}

void character::RestoreHP()
{
  DoForBodyParts(this, &bodypart::RestoreHP);

  /*for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->RestoreHP();
    }*/

  HP = MaxHP;
}

void character::RestoreLivingHP()
{
  HP = 0;

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->CanRegenerate())
	{
	  BodyPart->RestoreHP();
	  HP += BodyPart->GetHP();
	}
    }
}

bool character::AllowDamageTypeBloodSpill(int Type)
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
    }

  ABORT("Unknown blood effect destroyed the dungeon!");
  return false;
}

/* Returns truly done damage */

int character::ReceiveBodyPartDamage(character* Damager, int Damage, int Type,int BodyPartIndex, int Direction, bool PenetrateResistance, bool Critical, bool ShowNoDamageMsg, bool CaptureBodyPart)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);

  if(!Damager || Damager->AttackMayDamageArmor())
    BodyPart->DamageArmor(Damager, Damage, Type);

  if(!PenetrateResistance)
    Damage -= (BodyPart->GetTotalResistance(Type) >> 1) + RAND() % ((BodyPart->GetTotalResistance(Type) >> 1) + 1);

  if(int(Damage) < 1)
    if(Critical)
      Damage = 1;
    else
      {
	if(ShowNoDamageMsg)
	  {
	    if(IsPlayer())
	      ADD_MESSAGE("You are not hurt.");
	    else if(CanBeSeenByPlayer())
	      ADD_MESSAGE("%s is not hurt.", GetPersonalPronoun().CStr());
	  }

	return 0;
      }

  if(Critical && AllowDamageTypeBloodSpill(Type) && !game::IsInWilderness())
    {
      BodyPart->SpillBlood(2 + (RAND() & 1));

      for(int d = 0; d < GetNeighbourSquares(); ++d)
	{
	  lsquare* Square = GetNeighbourLSquare(d);

	  if(Square && Square->IsFlyable())
	    BodyPart->SpillBlood(1, Square->GetPos());
	}
    }

  if(BodyPart->ReceiveDamage(Damager, Damage, Type, Direction)
  && BodyPartCanBeSevered(BodyPartIndex))
    {
      if(DamageTypeDestroysBodyPart(Type))
	{
	  if(IsPlayer())
	    ADD_MESSAGE("Your %s is destroyed!", BodyPart->GetBodyPartName().CStr());
	  else if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s %s is destroyed!", GetPossessivePronoun().CStr(), BodyPart->GetBodyPartName().CStr());

	  GetBodyPart(BodyPartIndex)->DropEquipment();
	  item* Severed = SevereBodyPart(BodyPartIndex);

	  if(Severed)
	    Severed->DestroyBodyPart(!game::IsInWilderness() ? GetStackUnder() : GetStack());

	  SendNewDrawRequest();

	  if(IsPlayer())
	    game::AskForKeyPress(CONST_S("Bodypart destroyed! [press any key to continue]"));
	}
      else
	{
	  if(IsPlayer())
	    ADD_MESSAGE("Your %s is severed off!", BodyPart->GetBodyPartName().CStr());
	  else if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s %s is severed off!", GetPossessivePronoun().CStr(), BodyPart->GetBodyPartName().CStr());

	  item* Severed = SevereBodyPart(BodyPartIndex);
	  SendNewDrawRequest();

	  if(Severed)
	    {
	      if(CaptureBodyPart)
		Damager->GetLSquareUnder()->AddItem(Severed);
	      else if(!game::IsInWilderness())
		{
		  /** No multi-tile humanoid support! */

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
	    game::AskForKeyPress(CONST_S("Bodypart severed! [press any key to continue]"));
	}

      if(CanPanicFromSeveredBodyPart()
      && RAND() % 100 < GetPanicLevel()
      && !StateIsActivated(PANIC) && !IsDead())
	BeginTemporaryState(PANIC, 1000 + RAND() % 1001);

      SpecialBodyPartSeverReaction();
    }

  if(!IsDead())
    CheckPanic(500);

  return Damage;
}

/* Returns 0 if bodypart disappears */

item* character::SevereBodyPart(int BodyPartIndex, bool ForceDisappearance, stack* EquipmentDropStack)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);

  if(StateIsActivated(LEPROSY))
    BodyPart->GetMainMaterial()->SetIsInfectedByLeprosy(true);

  if(ForceDisappearance
  || BodyPartsDisappearWhenSevered()
  || StateIsActivated(POLYMORPHED)
  || game::AllBodyPartsVanish())
    {
      BodyPart->DropEquipment(EquipmentDropStack);
      BodyPart->RemoveFromSlot();
      CalculateAttributeBonuses();
      CalculateBattleInfo();
      BodyPart->SendToHell();
      SignalPossibleTransparencyChange();
      RemoveTraps(BodyPartIndex);
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
      BodyPart->Enable();
      SignalPossibleTransparencyChange();
      RemoveTraps(BodyPartIndex);
      return BodyPart;
    }
}

/* The second int is actually TargetFlags, which is not used here, but seems to be used in humanoid::ReceiveDamage. Returns true if the character really receives damage */

bool character::ReceiveDamage(character* Damager, int Damage, int Type, int, int Direction, bool, bool PenetrateArmor, bool Critical, bool ShowMsg)
{
  bool Affected = !!ReceiveBodyPartDamage(Damager, Damage, Type, 0, Direction, PenetrateArmor, Critical, ShowMsg);

  if(DamageTypeAffectsInventory(Type))
    {
      for(int c = 0; c < GetEquipmentSlots(); ++c)
	{
	  item* Equipment = GetEquipment(c);

	  if(Equipment)
	    Equipment->ReceiveDamage(Damager, Damage, Type);
	}

      GetStack()->ReceiveDamage(Damager, Damage, Type);
    }

  return Affected;
}

festring character::GetDescription(int Case) const
{
  if(IsPlayer())
    return CONST_S("you");
  else if(CanBeSeenByPlayer())
    return GetName(Case);
  else
    return CONST_S("something");
}

festring character::GetPersonalPronoun(bool PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return CONST_S("you");
  else if(GetSex() == UNDEFINED || (PlayersView && !CanBeSeenByPlayer() && !game::GetSeeWholeMapCheatMode()))
    return CONST_S("it");
  else if(GetSex() == MALE)
    return CONST_S("he");
  else
    return CONST_S("she");
}

festring character::GetPossessivePronoun(bool PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return CONST_S("your");
  else if(GetSex() == UNDEFINED || (PlayersView && !CanBeSeenByPlayer() && !game::GetSeeWholeMapCheatMode()))
    return CONST_S("its");
  else if(GetSex() == MALE)
    return CONST_S("his");
  else
    return CONST_S("her");
}

festring character::GetObjectPronoun(bool PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return CONST_S("you");
  else if(GetSex() == UNDEFINED || (PlayersView && !CanBeSeenByPlayer() && !game::GetSeeWholeMapCheatMode()))
    return CONST_S("it");
  else if(GetSex() == MALE)
    return CONST_S("him");
  else
    return CONST_S("her");
}

void character::AddName(festring& String, int Case) const
{
  if(!(Case & PLURAL) && AssignedName.GetSize())
    {
      if(!ShowClassDescription())
	String << AssignedName;
      else
	{
	  String << AssignedName << ' ';
	  id::AddName(String, (Case|ARTICLE_BIT)&~INDEFINE_BIT);
	}
    }
  else
    id::AddName(String, Case);
}

int character::GetHungerState() const
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
  else if(GetNP() > VERY_HUNGER_LEVEL)
    return VERY_HUNGRY;
  else
    return STARVING;
}

bool character::CanConsume(material* Material) const
{
  return !!(GetConsumeFlags() & Material->GetConsumeType());
}

void character::SetTemporaryStateCounter(long State, int What)
{
  for(int c = 0; c < STATES; ++c)
    if((1 << c) & State)
      TemporaryStateCounter[c] = What;
}

void character::EditTemporaryStateCounter(long State, int What)
{
  for(int c = 0; c < STATES; ++c)
    if((1 << c) & State)
      TemporaryStateCounter[c] += What;
}

int character::GetTemporaryStateCounter(long State) const
{
  for(int c = 0; c < STATES; ++c)
    if((1 << c) & State)
      return TemporaryStateCounter[c];

  ABORT("Illegal GetTemporaryStateCounter request!");
  return 0;
}

bool character::CheckKick() const
{
  if(!CanKick())
    {
      if(IsPlayer())
	ADD_MESSAGE("This race can't kick.");

      return false;
    }
  else
    return true;
}

int character::GetResistance(int Type) const
{
  switch(Type&0xFFF)
    {
    case PHYSICAL_DAMAGE:
    case SOUND:
    case DRAIN:
      return 0;
    case ENERGY: return GetEnergyResistance();
    case FIRE: return GetFireResistance();
    case POISON: return GetPoisonResistance();
    case ELECTRICITY: return GetElectricityResistance();
    case ACID: return GetAcidResistance();
    }

  ABORT("Resistance lack detected!");
  return 0;
}

void character::Regenerate()
{
  if(HP == MaxHP)
    return;

  long RegenerationBonus = 0;
  bool NoHealableBodyParts = true;

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->CanRegenerate())
	{
	  RegenerationBonus += BodyPart->GetMaxHP();

	  if(NoHealableBodyParts && BodyPart->GetHP() < BodyPart->GetMaxHP())
	    NoHealableBodyParts = false;
	}
    }

  if(!RegenerationBonus || NoHealableBodyParts)
    return;

  RegenerationBonus *= (50 + GetAttribute(ENDURANCE));

  if(Action && Action->IsRest())
    if(SquaresUnder == 1)
      RegenerationBonus *= GetSquareUnder()->GetRestModifier() << 1;
    else
      {
	int Lowest = GetSquareUnder(0)->GetRestModifier();

	for(int c = 1; c < GetSquaresUnder(); ++c)
	  {
	    int Mod = GetSquareUnder(c)->GetRestModifier();

	    if(Mod < Lowest)
	      Lowest = Mod;
	  }

	RegenerationBonus *= Lowest << 1;
      }

  RegenerationCounter += RegenerationBonus;

  while(RegenerationCounter > 1250000)
    {
      bodypart* BodyPart = HealHitPoint();

      if(!BodyPart)
	break;

      EditNP(-Max(7500 / MaxHP, 1));
      RegenerationCounter -= 1250000;
      int HP = BodyPart->GetHP();
      EditExperience(ENDURANCE, Min(1000 * BodyPart->GetMaxHP() / (HP * HP), 300), 1000);
    }
}

void character::PrintInfo() const
{
  felist Info(CONST_S("Information about ") + GetName(DEFINITE));

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if((EquipmentEasilyRecognized(c) || game::WizardModeIsActive()) && Equipment)
	{
	  int ImageKey = game::AddToItemDrawVector(itemvector(1, Equipment));
	  Info.AddEntry(festring(GetEquipmentName(c)) + ": " + Equipment->GetName(INDEFINITE), LIGHT_GRAY, 0, ImageKey, true);
	}
    }

  if(Info.IsEmpty())
    ADD_MESSAGE("There's nothing special to tell about %s.", CHAR_NAME(DEFINITE));
  else
    {
      game::SetStandardListAttributes(Info);
      Info.SetEntryDrawer(game::ItemEntryDrawer);
      Info.Draw();
    }

  game::ClearItemDrawVector();
}

bool character::TryToRiseFromTheDead()
{
  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	{
	  BodyPart->ResetSpoiling();

	  if(BodyPart->CanRegenerate() || BodyPart->GetHP() < 1)
	    BodyPart->SetHP(1);
	}
    }

  ResetStates();
  return true;
}

bool character::RaiseTheDead(character*)
{
  bool Useful = false;

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(!BodyPart && CanCreateBodyPart(c))
	{
	  CreateBodyPart(c)->SetHP(1);

	  if(IsPlayer())
	    ADD_MESSAGE("Suddenly you grow a new %s.", GetBodyPartName(c).CStr());
	  else if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s grows a new %s.", CHAR_NAME(DEFINITE), GetBodyPartName(c).CStr());

	  Useful = true;
	}
      else if(BodyPart && BodyPart->CanRegenerate() && BodyPart->GetHP() < 1)
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

void character::SetSize(int Size)
{
  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->SetSize(GetBodyPartSize(c, Size));
    }
}

long character::GetBodyPartSize(int I, int TotalSize) const
{
  if(I == TORSO_INDEX)
    return TotalSize;
  else
    {
      ABORT("Weird bodypart size request for a character!");
      return 0;
    }
}

long character::GetBodyPartVolume(int I) const
{
  if(I == TORSO_INDEX)
    return GetTotalVolume();
  else
    {
      ABORT("Weird bodypart volume request for a character!");
      return 0;
    }
}

void character::CreateBodyParts(int SpecialFlags)
{
  for(int c = 0; c < BodyParts; ++c) 
    if(CanCreateBodyPart(c))
      CreateBodyPart(c, SpecialFlags);
}

void character::RestoreBodyParts()
{
  for(int c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      CreateBodyPart(c);
}

void character::UpdatePictures()
{
  if(!PictureUpdatesForbidden)
    DoIndexParameterRoutine(this, &character::JustUpdateTheDamnBodyPartPicture, BodyParts);
    /*for(int c = 0; c < BodyParts; ++c)
      UpdateBodyPartPicture(c);*/
}

bodypart* character::MakeBodyPart(int I) const
{
  if(I == TORSO_INDEX)
    return new normaltorso(0, NO_MATERIALS);
  else
    {
      ABORT("Weird bodypart to make for a character!");
      return 0;
    }
}

bodypart* character::CreateBodyPart(int I, int SpecialFlags)
{
  bodypart* BodyPart = MakeBodyPart(I);
  BodyPart->InitMaterials(CreateBodyPartMaterial(I, GetBodyPartVolume(I)), false);
  BodyPart->SetSize(GetBodyPartSize(I, GetTotalSize()));
  BodyPart->SetBloodMaterial(GetBloodMaterial());
  SetBodyPart(I, BodyPart);
  BodyPart->InitSpecialAttributes();

  if(!(SpecialFlags & NO_PIC_UPDATE))
    JustUpdateTheDamnBodyPartPicture(I);

  if(!Initializing)
    {
      CalculateBattleInfo();
      SendNewDrawRequest();
      SignalPossibleTransparencyChange();
    }

  return BodyPart;
}

vector2d character::GetBodyPartBitmapPos(int I, bool) const
{
  if(I == TORSO_INDEX)
    return GetTorsoBitmapPos();
  else
    {
      ABORT("Weird bodypart BitmapPos request for a character!");
      return vector2d();
    }
}

void character::UpdateBodyPartPicture(int I, bool Severed)
{
  bodypart* BodyPart = GetBodyPart(I);

  if(BodyPart)
    {
      BodyPart->SetBitmapPos(GetBodyPartBitmapPos(I, Severed));
      BodyPart->GetMainMaterial()->SetSkinColor(GetBodyPartColorA(I, Severed));
      BodyPart->GetMainMaterial()->SetSkinColorIsSparkling(BodyPartColorAIsSparkling(I, Severed));
      BodyPart->SetMaterialColorB(GetBodyPartColorB(I, Severed));
      BodyPart->SetMaterialColorC(GetBodyPartColorC(I, Severed));
      BodyPart->SetMaterialColorD(GetBodyPartColorD(I, Severed));
      BodyPart->SetIsSparklingB(BodyPartColorBIsSparkling(I, Severed));
      BodyPart->SetIsSparklingC(BodyPartColorCIsSparkling(I, Severed));
      BodyPart->SetIsSparklingD(BodyPartColorDIsSparkling(I, Severed));
      BodyPart->SetSpecialFlags(GetSpecialBodyPartFlags(I));
      BodyPart->SetWobbleData(GetBodyPartWobbleData(I));
      BodyPart->UpdatePictures();
    }
}

void character::LoadDataBaseStats()
{
  for(int c = 0; c < BASE_ATTRIBUTES; ++c)
    {
      BaseExperience[c] = GetNaturalExperience(c);

      if(BaseExperience[c])
	LimitRef(BaseExperience[c], MIN_EXP, MAX_EXP);
    }

  SetMoney(GetDefaultMoney());
  const fearray<long>& Skills = GetKnownCWeaponSkills();

  if(Skills.Size)
    {
      const fearray<long>& Hits = GetCWeaponSkillHits();

      if(Hits.Size == 1)
	{
	  for(uint c = 0; c < Skills.Size; ++c)
	    CWeaponSkill[Skills[c]].AddHit(Hits[0] * 100);
	}
      else if(Hits.Size == Skills.Size)
	{
	  for(uint c = 0; c < Skills.Size; ++c)
	    CWeaponSkill[Skills[c]].AddHit(Hits[c] * 100);
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

void character::Initialize(int NewConfig, int SpecialFlags)
{
  Initializing = InNoMsgMode = true;
  CalculateBodyParts();
  CalculateAllowedWeaponSkillCategories();
  CalculateSquaresUnder();
  BodyPartSlot = new bodypartslot[BodyParts];
  OriginalBodyPartID = new std::list<ulong>[BodyParts];
  CWeaponSkill = new cweaponskill[AllowedWeaponSkillCategories];
  SquareUnder = new square*[SquaresUnder];

  if(SquaresUnder == 1)
    *SquareUnder = 0;
  else
    memset(SquareUnder, 0, SquaresUnder * sizeof(square*));

  int c;

  for(c = 0; c < BodyParts; ++c)
    BodyPartSlot[c].SetMaster(this);

  if(!(SpecialFlags & LOAD))
    {
      ID = game::CreateNewCharacterID(this);
      databasecreator<character>::InstallDataBase(this, NewConfig);
      LoadDataBaseStats();
      TemporaryState |= GetClassStates();

      if(TemporaryState)
	for(c = 0; c < STATES; ++c)
	  if(TemporaryState & (1 << c))
	    TemporaryStateCounter[c] = PERMANENT;

      CreateBodyParts(SpecialFlags | NO_PIC_UPDATE);
      InitSpecialAttributes();
      CommandFlags = GetDefaultCommandFlags();

      if(GetAttribute(INTELLIGENCE) < 8) // gum
	CommandFlags &= ~DONT_CONSUME_ANYTHING_VALUABLE;

      if(!GetDefaultName().IsEmpty())
	SetAssignedName(GetDefaultName());
    }

  VirtualConstructor(SpecialFlags & LOAD);

  if(!(SpecialFlags & LOAD))
    {
      if(!(SpecialFlags & NO_EQUIPMENT))
	CreateInitialEquipment((SpecialFlags & NO_EQUIPMENT_PIC_UPDATE) >> 1);

      if(!(SpecialFlags & NO_PIC_UPDATE))
	UpdatePictures();

      CalculateAll();
      RestoreHP();
      RestoreStamina();
    }

  Initializing = InNoMsgMode = false;
}

bool character::TeleportNear(character* Caller)
{
  vector2d Where = GetLevel()->GetNearestFreeSquare(this, Caller->GetPos());

  if(Where == ERROR_VECTOR)
    return false;

  Move(Where, true);
  return true;
}

void character::ReceiveHeal(long Amount)
{
  int c;

  for(c = 0; c < Amount / 10; ++c)
    if(!HealHitPoint())
      break;

  Amount -= c * 10;

  if(RAND() % 10 < Amount)
    HealHitPoint();

  if(Amount >= 250 || RAND() % 250 < Amount)
    {
      bodypart* NewBodyPart = GenerateRandomBodyPart();

      if(!NewBodyPart)
	return;

      NewBodyPart->SetHP(1);

      if(IsPlayer())
	ADD_MESSAGE("You grow a new %s.", NewBodyPart->GetBodyPartName().CStr()); 
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s grows a new %s.", CHAR_NAME(DEFINITE), NewBodyPart->GetBodyPartName().CStr());
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
    Vomit(GetPos(), SizeOfEffect);

  if(!(RAND() % 3) && SizeOfEffect >= 500
  && EditAttribute(ENDURANCE, SizeOfEffect / 500))
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
}

void character::ReceiveOmmelUrine(long Amount)
{
  EditExperience(ARM_STRENGTH, 500, Amount << 4);
  EditExperience(LEG_STRENGTH, 500, Amount << 4);

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelCerumen(long Amount)
{
  EditExperience(INTELLIGENCE, 500, Amount << 5);
  EditExperience(WISDOM, 500, Amount << 5);

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelSweat(long Amount)
{
  EditExperience(AGILITY, 500, Amount << 4);
  EditExperience(DEXTERITY, 500, Amount << 4);
  RestoreStamina();

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelTears(long Amount)
{
  EditExperience(PERCEPTION, 500, Amount << 4);
  EditExperience(CHARISMA, 500, Amount << 4);

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelSnot(long Amount)
{
  EditExperience(ENDURANCE, 500, Amount << 5);
  RestoreLivingHP();

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::AddOmmelConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel a primitive force coursing through your veins.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s looks more powerful.", CHAR_NAME(DEFINITE));
}

void character::ReceivePepsi(long Amount)
{
  ReceiveDamage(0, Amount / 100, POISON, TORSO);
  EditExperience(PERCEPTION, Amount, 1 << 14);

  if(CheckDeath(CONST_S("was poisoned by pepsi"), 0))
    return;

  if(IsPlayer())
    game::DoEvilDeed(Amount / 10);
}

void character::AddPepsiConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Urgh. You feel your guruism fading away.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks very lame.", CHAR_NAME(DEFINITE));
}

void character::ReceiveDarkness(long Amount)
{
  EditExperience(INTELLIGENCE, -Amount / 5, 1 << 13);
  EditExperience(WISDOM, -Amount / 5, 1 << 13);
  EditExperience(CHARISMA, -Amount / 5, 1 << 13);

  if(IsPlayer())
    game::DoEvilDeed(int(Amount / 50));
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
  /* As it is commonly known, the possibility of fainting per 500 cubic
     centimeters of kobold flesh is exactly 5%. */

  if(!(RAND() % 20))
    {
      if(IsPlayer())
	ADD_MESSAGE("You lose control of your legs and fall down.");

      LoseConsciousness(250 + RAND_N(250));
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

/* Returns true if character manages to unstuck himself (from all traps).
   vector2d is the direction which the character has tried to escape to. */

/*bool character::TryToUnstuck(vector2d Direction)
{
  return StuckTo->TryToUnstuck(this, StuckToBodyPart, Direction);
}*/

/*bool character::IsStuck() const
{
  return GetStuckToBodyPart() != NONE_INDEX;
}*/

bool character::RawEditAttribute(double& Experience, int Amount) const
{
  /* Check if the attribute is disabled for creature */

  if(!Experience)
    return false;

  if((Amount < 0 && Experience < 2 * EXP_MULTIPLIER)
  || (Amount > 0 && Experience > 999 * EXP_MULTIPLIER))
    return false;

  Experience += Amount * EXP_MULTIPLIER;
  LimitRef<double>(Experience, MIN_EXP, MAX_EXP);
  return true;
}

void character::DrawPanel(bool AnimationDraw) const
{
  if(AnimationDraw)
    {
      DrawStats(true);
      return;
    }

  igraph::BlitBackGround(19 + (game::GetScreenXSize() << 4), 0, RES_X - 19 - (game::GetScreenXSize() << 4), RES_Y);
  igraph::BlitBackGround(16, 45 + (game::GetScreenYSize() << 4), game::GetScreenXSize() << 4, 9);
  FONT->Printf(DOUBLE_BUFFER, 16, 45 + (game::GetScreenYSize() << 4), WHITE, "%s", GetPanelName().CStr());
  game::UpdateAttributeMemory();
  int PanelPosX = RES_X - 96;
  int PanelPosY = DrawStats(false);
  PrintAttribute("End", ENDURANCE, PanelPosX, PanelPosY++);
  PrintAttribute("Per", PERCEPTION, PanelPosX, PanelPosY++);
  PrintAttribute("Int", INTELLIGENCE, PanelPosX, PanelPosY++);
  PrintAttribute("Wis", WISDOM, PanelPosX, PanelPosY++);
  PrintAttribute("Cha", CHARISMA, PanelPosX, PanelPosY++);
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Siz  %d", GetSize());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, 
	       IsInBadCondition() ? RED : WHITE, "HP %d/%d", GetHP(), GetMaxHP());
  //  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Sta %d/%d", Stamina / 1000, MaxStamina / 1000);
  ++PanelPosY;
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Gold: %d", GetMoney());
  ++PanelPosY;

  /*int D = int(game::GetGameSituationDanger() * 1000);
  int E = Max(255 - D / 4, 0);
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, MakeRGB16(255, E, E), "Dan: %d", D);
  ++PanelPosY;*/

  if(game::IsInWilderness())
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Worldmap");
  else
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "%s", game::GetCurrentDungeon()->GetShortLevelDescription(game::GetCurrentLevelIndex()).CapitalizeCopy().CStr());

  ivantime Time;
  game::GetTime(Time);
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Day %d", Time.Day);
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Time %d:%s%d", Time.Hour, Time.Min < 10 ? "0" : "", Time.Min);
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Turn %d", game::GetTurn());

  ++PanelPosY;

  if(GetAction())
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "%s", festring(GetAction()->GetDescription()).CapitalizeCopy().CStr());

  for(int c = 0; c < STATES; ++c)
    if(!(StateData[c].Flags & SECRET) && StateIsActivated(1 << c) && (1 << c != HASTE || !StateIsActivated(SLOW)) && (1 << c != SLOW || !StateIsActivated(HASTE)))
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, (1 << c) & EquipmentState || TemporaryStateCounter[c] == PERMANENT ? BLUE : WHITE, "%s", StateData[c].Description);

  /* Make this more elegant!!! */

  if(GetHungerState() == STARVING)
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, RED, "Starving");
  else if(GetHungerState() == VERY_HUNGRY)
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, BLUE, "Very hungry");
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
    }

  //if(GetBurdenState() != UNBURDENED)
  //  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, GetVerbalBurdenStateColor(), "%s", festring(GetVerbalBurdenState()).Capitalize().CStr);

  switch(GetTirednessState())
    {
    case FAINTING:
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, RED, "Fainting");
      break;
    case EXHAUSTED:
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Exhausted");
      break;
    }

  if(game::PlayerIsRunning())
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Running");
}

void character::CalculateDodgeValue()
{
  DodgeValue = 0.05 * GetMoveEase() * GetAttribute(AGILITY) / sqrt(GetSize());

  if(GetMoveType() & FLY)
    DodgeValue *= 2;

  if(DodgeValue < 1)
    DodgeValue = 1;
}

bool character::DamageTypeAffectsInventory(int Type)
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
    }

  ABORT("Unknown reaping effect destroyed dungeon!");
  return false;
}

int character::CheckForBlockWithArm(character* Enemy, item* Weapon, arm* Arm, double WeaponToHitValue, int Damage, int Success, int Type)
{
  int BlockStrength = Arm->GetBlockCapability();
  double BlockValue = Arm->GetBlockValue();

  if(BlockStrength && BlockValue)
    {
      item* Blocker = Arm->GetWielded();

      if(RAND() % int(100 + WeaponToHitValue / BlockValue / (1 << BlocksSinceLastTurn) * (100 + Success)) < 100)
	{
	  int NewDamage = BlockStrength < Damage ? Damage - BlockStrength : 0;

	  switch(Type)
	    {
	    case UNARMED_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->UnarmedHitNoun(), !!NewDamage);
	      break;
	    case WEAPON_ATTACK:
	      AddBlockMessage(Enemy, Blocker, "attack", !!NewDamage);
	      break;
	    case KICK_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->KickNoun(), !!NewDamage);
	      break;
	    case BITE_ATTACK:
	      AddBlockMessage(Enemy, Blocker, Enemy->BiteNoun(), !!NewDamage);
	      break;
	    }

	  long Weight = Blocker->GetWeight();
	  long StrExp = Limit(15 * Weight / 200L, 75L, 300L);
	  long DexExp = Weight ? Limit(75000L / Weight, 75L, 300L) : 300;
	  Arm->EditExperience(ARM_STRENGTH, StrExp, 1 << 8);
	  Arm->EditExperience(DEXTERITY, DexExp, 1 << 8);
	  EditStamina(-10000 / GetAttribute(ARM_STRENGTH), false);

	  if(Arm->TwoHandWieldIsActive())
	    {
	      arm* PairArm = Arm->GetPairArm();
	      PairArm->EditExperience(ARM_STRENGTH, StrExp, 1 << 8);
	      PairArm->EditExperience(DEXTERITY, DexExp, 1 << 8);
	    }

	  Blocker->WeaponSkillHit(Enemy->CalculateWeaponSkillHits(this));
	  Blocker->ReceiveDamage(this, Damage, PHYSICAL_DAMAGE);

	  if(Weapon)
	    Weapon->ReceiveDamage(Enemy, Damage - NewDamage, PHYSICAL_DAMAGE);

	  if(BlocksSinceLastTurn < 16)
	    ++BlocksSinceLastTurn;

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

void character::SignalEquipmentAdd(int EquipmentIndex)
{
  item* Equipment = GetEquipment(EquipmentIndex);

  if(Equipment->IsInCorrectSlot(EquipmentIndex))
    {
      long AddedStates = Equipment->GetGearStates();

      if(AddedStates)
	for(int c = 0; c < STATES; ++c)
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

void character::SignalEquipmentRemoval(int)
{
  //bool CouldFly = !!(GetMoveType() & FLY);
  CalculateEquipmentState();
  CalculateAttributeBonuses();

  /*if(CouldFly && !(GetMoveType() & FLY))
    {
      if(!game::IsInWilderness() && !GetLSquareUnder()->IsFreezed())
	SignalStepFrom(0);

      TestWalkability();
    }*/
}

void character::CalculateEquipmentState()
{
  int c;
  long Back = EquipmentState;
  EquipmentState = 0;

  for(c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment->IsInCorrectSlot(c))
	EquipmentState |= Equipment->GetGearStates();
    }

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

void character::BeginTemporaryState(long State, int Counter)
{
  if(!Counter)
    return;

  int Index;

  if(State == POLYMORPHED)
    ABORT("No Polymorphing with BeginTemporaryState!");

  for(Index = 0; Index < STATES; ++Index)
    if(1 << Index == State)
      break;

  if(Index == STATES)
    ABORT("BeginTemporaryState works only when State == 2 ^ n!");

  if(TemporaryStateIsActivated(State))
    {
      int OldCounter = GetTemporaryStateCounter(State);

      if(OldCounter != PERMANENT)
	EditTemporaryStateCounter(State, Max(Counter, 50 - OldCounter));
    }
  else if(StateData[Index].IsAllowed == 0 || (this->*StateData[Index].IsAllowed)())
    {
      ActivateTemporaryState(State);
      SetTemporaryStateCounter(State, Max(Counter, 50));

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

  for(int c = 0; c < STATES; ++c)
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

		  if(!TemporaryStateCounter[c])
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
  ForceEndPolymorph();
}

character* character::ForceEndPolymorph()
{
  if(IsPlayer())
    ADD_MESSAGE("You return to your true form.");
  else if(game::IsInWilderness())
    {
      ActivateTemporaryState(POLYMORPHED);
      SetTemporaryStateCounter(POLYMORPHED, 10);
      return this; // fast gum solution, state ends when the player enters a dungeon
    }
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s returns to %s true form.", CHAR_NAME(DEFINITE), GetPossessivePronoun().CStr());

  RemoveTraps();

  if(GetAction())
    GetAction()->Terminate(false);

  vector2d Pos = GetPos();
  SendToHell();
  Remove();
  character* Char = GetPolymorphBackup();
  InNoMsgMode = Char->InNoMsgMode = true;
  Char->ChangeTeam(GetTeam());

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(Char);

  SetPolymorphBackup(0);
  Char->PutToOrNear(Pos);
  Char->Enable();
  Char->SetPolymorphed(false);
  GetStack()->MoveItemsTo(Char->GetStack());
  DonateEquipmentTo(Char);
  Char->SetMoney(GetMoney());
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
  return Char;
}

void character::LycanthropyHandler()
{
  if(!(RAND() % 2000))
    {
      if(StateIsActivated(POLYMORPH_CONTROL)
      && !game::BoolQuestion(CONST_S("Do you wish to change into a werewolf? [y/N]")))
	return;

      Polymorph(new werewolfwolf, 1000 + RAND() % 2000);
    }
}

void character::SaveLife()
{
  if(TemporaryStateIsActivated(LIFE_SAVED))
    {
      if(IsPlayer())
	ADD_MESSAGE("But wait! You glow briefly red and seem to be in a better shape!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("But wait, suddenly %s glows briefly red and seems to be in a better shape!", GetPersonalPronoun().CStr());

      DeActivateTemporaryState(LIFE_SAVED);
    }
  else
    {
      item* LifeSaver = 0;

      for(int c = 0; c < GetEquipmentSlots(); ++c)
	{
	  item* Equipment = GetEquipment(c);

	  if(Equipment && Equipment->IsInCorrectSlot(c) && Equipment->GetGearStates() & LIFE_SAVED)
	    LifeSaver = Equipment;
	}

      if(!LifeSaver)
	ABORT("The Universe can only kill you once!");

      if(IsPlayer())
	ADD_MESSAGE("But wait! Your %s glows briefly red and disappears and you seem to be in a better shape!", LifeSaver->CHAR_NAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("But wait, suddenly %s %s glows briefly red and disappears and %s seems to be in a better shape!", GetPossessivePronoun().CStr(), LifeSaver->CHAR_NAME(UNARTICLED), GetPersonalPronoun().CStr());

      LifeSaver->RemoveFromSlot();
      LifeSaver->SendToHell();
    }

  if(IsPlayer())
    game::AskForKeyPress(CONST_S("Life saved! [press any key to continue]"));

  RestoreBodyParts();
  ResetSpoiling();
  RestoreHP();
  RestoreStamina();
  ResetStates();

  if(GetNP() < SATIATED_LEVEL)
    SetNP(SATIATED_LEVEL);

  SendNewDrawRequest();

  if(GetAction())
    GetAction()->Terminate(false);
}

character* character::PolymorphRandomly(int MinDanger, int MaxDanger, int Time)
{
  character* NewForm = 0;

  if(StateIsActivated(POLYMORPH_CONTROL))
    {
      if(IsPlayer())
	{
	  if(!GetNewFormForPolymorphWithControl(NewForm))
	    return NewForm;
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

void character::StartReading(item* Item, long Time)
{
  study* Read = new study(this);
  Read->SetLiteratureID(Item->GetID());

  if(game::WizardModeIsActive())
    Time = 1;

  Read->SetCounter(Time);
  SetAction(Read);

  if(IsPlayer())
    ADD_MESSAGE("You start reading %s.", Item->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s starts reading %s.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(DEFINITE));
}

/* Call when one makes something with his/her/its hands.
 * Difficulty of 5 takes about one turn, so it's the most common to use. */

void character::DexterityAction(int Difficulty)
{
  EditAP(-20000 * Difficulty / APBonus(GetAttribute(DEXTERITY)));
  EditExperience(DEXTERITY, Difficulty * 15, 1 << 7);
}

/* If Theoretically == true, range is not a factor. */

bool character::CanBeSeenByPlayer(bool Theoretically, bool IgnoreESP) const
{
  if(IsEnabled() && !game::IsGenerating())
    {
      bool MayBeESPSeen = PLAYER->IsEnabled() && !IgnoreESP && PLAYER->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5;
      bool MayBeInfraSeen = PLAYER->IsEnabled() && PLAYER->StateIsActivated(INFRA_VISION) && IsWarm();
      bool Visible = !StateIsActivated(INVISIBLE) || MayBeESPSeen || MayBeInfraSeen;

      if(game::IsInWilderness())
	return Visible;

      if(MayBeESPSeen
      && (Theoretically
       || GetDistanceSquareFrom(PLAYER) <= PLAYER->GetESPRangeSquare()))
	return true;
      else if(!Visible)
	return false;
      else
	return Theoretically || SquareUnderCanBeSeenByPlayer(MayBeInfraSeen);
    }
  else
    return false;
}

bool character::CanBeSeenBy(const character* Who, bool Theoretically, bool IgnoreESP) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer(Theoretically, IgnoreESP);
  else
    {
      if(IsEnabled() && !game::IsGenerating())
	{
	  bool MayBeESPSeen = Who->IsEnabled() && !IgnoreESP && Who->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5;
	  bool MayBeInfraSeen = Who->IsEnabled() && Who->StateIsActivated(INFRA_VISION) && IsWarm();
	  bool Visible = !StateIsActivated(INVISIBLE) || MayBeESPSeen || MayBeInfraSeen;

	  if(game::IsInWilderness())
	    return Visible;

	  if(MayBeESPSeen
	  && (Theoretically
	   || GetDistanceSquareFrom(Who) <= Who->GetESPRangeSquare()))
	    return true;
	  else if(!Visible)
	    return false;
	  else
	    return Theoretically || SquareUnderCanBeSeenBy(Who, MayBeInfraSeen);
	}
      else
	return false;
    }
}

bool character::SquareUnderCanBeSeenByPlayer(bool IgnoreDarkness) const
{
  int S1 = SquaresUnder, S2 = PLAYER->SquaresUnder;

  if(S1 == 1 && S2 == 1)
    {
      if(GetSquareUnder()->CanBeSeenByPlayer(IgnoreDarkness))
	return true;

      if(IgnoreDarkness)
	{
	  int LOSRangeSquare = PLAYER->GetLOSRangeSquare();

	  if((GetPos() - PLAYER->GetPos()).GetLengthSquare() <= LOSRangeSquare)
	    {
	      eyecontroller::Map = GetLevel()->GetMap();
	      return mapmath<eyecontroller>::DoLine(PLAYER->GetPos().X, PLAYER->GetPos().Y, GetPos().X, GetPos().Y, SKIP_FIRST);
	    }
	}

      return false;
    }
  else
    {
      for(int c1 = 0; c1 < S1; ++c1)
	{
	  lsquare* Square = GetLSquareUnder(c1);

	  if(Square->CanBeSeenByPlayer(IgnoreDarkness))
	    return true;
	  else if(IgnoreDarkness)
	    {
	      vector2d Pos = Square->GetPos();
	      int LOSRangeSquare = PLAYER->GetLOSRangeSquare();

	      for(int c2 = 0; c2 < S2; ++c2)
		{
		  vector2d PlayerPos = PLAYER->GetPos(c2);

		  if((Pos - PlayerPos).GetLengthSquare() <= LOSRangeSquare)
		    {
		      eyecontroller::Map = GetLevel()->GetMap();

		      if(mapmath<eyecontroller>::DoLine(PlayerPos.X, PlayerPos.Y, Pos.X, Pos.Y, SKIP_FIRST))
			return true;
		    }
		}
	    }
	}

      return false;
    }
}

bool character::SquareUnderCanBeSeenBy(const character* Who, bool IgnoreDarkness) const
{
  int S1 = SquaresUnder, S2 = Who->SquaresUnder;
  int LOSRangeSquare = Who->GetLOSRangeSquare();

  if(S1 == 1 && S2 == 1)
    return GetSquareUnder()->CanBeSeenFrom(Who->GetPos(), LOSRangeSquare, IgnoreDarkness);
  else
    {
      for(int c1 = 0; c1 < S1; ++c1)
	{
	  lsquare* Square = GetLSquareUnder(c1);

	  for(int c2 = 0; c2 < S2; ++c2)
	    if(Square->CanBeSeenFrom(Who->GetPos(c2), LOSRangeSquare, IgnoreDarkness))
	      return true;
	}

      return false;
    }
}

int character::GetDistanceSquareFrom(const character* Who) const
{
  int S1 = SquaresUnder, S2 = Who->SquaresUnder;

  if(S1 == 1 && S2 == 1)
    return (GetPos() - Who->GetPos()).GetLengthSquare();
  else
    {
      vector2d MinDist(0x7FFF, 0x7FFF);
      int MinLength = 0xFFFF;

      for(int c1 = 0; c1 < S1; ++c1)
	for(int c2 = 0; c2 < S2; ++c2)
	  {
	    vector2d Dist = GetPos(c1) - Who->GetPos(c2);

	    if(Dist.X < 0)
	      Dist.X = -Dist.X;

	    if(Dist.Y < 0)
	      Dist.Y = -Dist.Y;

	    if(Dist.X <= MinDist.X && Dist.Y <= MinDist.Y)
	      {
		MinDist = Dist;
		MinLength = Dist.GetLengthSquare();
	      }
	    else if(Dist.X < MinDist.X || Dist.Y < MinDist.Y)
	      {
		int Length = Dist.GetLengthSquare();

		if(Length < MinLength)
		  {
		    MinDist = Dist;
		    MinLength = Length;
		  }
	      }
	  }

      return MinLength;
    }
}

void character::AttachBodyPart(bodypart* BodyPart)
{
  SetBodyPart(BodyPart->GetBodyPartIndex(), BodyPart);

  if(!AllowSpoil())
    BodyPart->ResetSpoiling();

  BodyPart->ResetPosition();
  BodyPart->UpdatePictures();
  CalculateAttributeBonuses();
  CalculateBattleInfo();
  SendNewDrawRequest();
  SignalPossibleTransparencyChange();
}

/* Returns true if the character has all bodyparts, false if not. */

bool character::HasAllBodyParts() const
{
  for(int c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      return false;

  return true;
}

bodypart* character::GenerateRandomBodyPart()
{
  int NeededBodyPart[MAX_BODYPARTS];
  int Index = 0;

  for(int c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      NeededBodyPart[Index++] = c;

  return Index ? CreateBodyPart(NeededBodyPart[RAND() % Index]) : 0;
}

/* Searches the character's Stack and if it find some bodyparts there that are the character's old bodyparts returns a stackiterator to one of them (choosen in random). If no fitting bodyparts are found the function returns 0 */

bodypart* character::FindRandomOwnBodyPart(bool AllowNonLiving) const
{
  itemvector LostAndFound;

  for(int c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c))
      for(std::list<ulong>::iterator i = OriginalBodyPartID[c].begin(); i != OriginalBodyPartID[c].end(); ++i)
	{
	  bodypart* Found = static_cast<bodypart*>(SearchForItem(*i));

	  if(Found && (AllowNonLiving || Found->CanRegenerate()))
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
    {
      lsquare* Where = GetLSquareUnder()->GetRandomAdjacentSquare();

      /* slightly more probable to vomit on self than other squares */

      if(RAND_8 && (!Where->GetCharacter() || Where->GetCharacter()->IsPet()))
	Vomit(Where->GetPos(), 500 + RAND() % 250);
      else
	Vomit(GetPos(), 500 + RAND() % 250);
    }

  int Damage = 0;

  for(int Used = 0; Used < GetTemporaryStateCounter(POISONED); Used += 100)
    if(!(RAND() % 100))
      ++Damage;

  if(Damage)
    {
      ReceiveDamage(0, Damage, POISON, ALL, 8, false, false, false, false);
      CheckDeath(CONST_S("died of acute poisoning"), 0);
    }
}

bool character::IsWarm() const
{
  return CombineBodyPartPredicates<OR_BOOLS>(this, &bodypart::IsWarm);
}

void character::BeginInvisibility()
{
  UpdatePictures();
  SendNewDrawRequest();
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
  SendNewDrawRequest();
}

void character::EndInfraVision()
{
  if(IsPlayer() && IsEnabled())
    GetArea()->SendNewDrawRequest();
}

void character::EndESP()
{
  if(IsPlayer() && IsEnabled())
    GetArea()->SendNewDrawRequest();
}

void character::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance, int SquareIndex, bool AllowAnimate) const
{
  if(PLAYER->IsEnabled()
  && ((PLAYER->StateIsActivated(ESP)
    && GetAttribute(INTELLIGENCE) >= 5
    && (PLAYER->GetPos() - GetPos()).GetLengthSquare() <= PLAYER->GetESPRangeSquare())
   || (PLAYER->StateIsActivated(INFRA_VISION) && IsWarm())))
    Luminance = ivanconfig::GetContrastLuminance();

  DrawBodyParts(Bitmap, Pos, Luminance, SquareIndex, AllowAnimate);
  
  if(GetTeam() == PLAYER->GetTeam() && !IsPlayer()
  && SquareIndex == GetTameSymbolSquareIndex())
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(Bitmap, 32, 16, Pos, 16, 16, ivanconfig::GetContrastLuminance());

  if(GetMoveType() & FLY
  && SquareIndex == GetFlySymbolSquareIndex())
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(Bitmap, 128, 16, Pos, 16, 16, ivanconfig::GetContrastLuminance());

  if(!(GetMoveType() & FLY)
  && GetSquareUnder()->GetSquareWalkability() & SWIM
  && SquareIndex == GetSwimmingSymbolSquareIndex())
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(Bitmap, 240, 16, Pos, 16, 16, ivanconfig::GetContrastLuminance());

  if(GetAction() && GetAction()->IsUnconsciousness()
  && SquareIndex == GetUnconsciousSymbolSquareIndex())
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(Bitmap, 224, 16, Pos, 16, 16, ivanconfig::GetContrastLuminance());
}

void character::DrawBodyParts(bitmap* Bitmap, vector2d Pos, color24 Luminance, int SquareIndex, bool AllowAnimate, bool AllowAlpha) const
{
  GetTorso()->Draw(Bitmap, Pos, Luminance, SquareIndex, AllowAnimate, AllowAlpha);
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
    PolymorphRandomly(1, 999999, 200 + RAND() % 800);
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
  felist Info(CONST_S("Information about ") + GetDescription(DEFINITE));
  AddSpecialStethoscopeInfo(Info);
  Info.AddEntry(CONST_S("Endurance: ") + GetAttribute(ENDURANCE), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Perception: ") + GetAttribute(PERCEPTION), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Intelligence: ") + GetAttribute(INTELLIGENCE), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Wisdom: ") + GetAttribute(WISDOM), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Charisma: ") + GetAttribute(CHARISMA), LIGHT_GRAY);
  Info.AddEntry(CONST_S("HP: ") + GetHP() + "/" + GetMaxHP(), IsInBadCondition() ? RED : LIGHT_GRAY);

  if(GetAction())
    Info.AddEntry(festring(GetAction()->GetDescription()).CapitalizeCopy(), LIGHT_GRAY);

  for(int c = 0; c < STATES; ++c)
    if(StateIsActivated(1 << c) && (1 << c != HASTE || !StateIsActivated(SLOW)) && (1 << c != SLOW || !StateIsActivated(HASTE)))
      Info.AddEntry(StateData[c].Description, LIGHT_GRAY);

  switch(GetTirednessState())
    {
    case FAINTING:
      Info.AddEntry("Fainting", RED);
      break;
    case EXHAUSTED:
      Info.AddEntry("Exhausted", LIGHT_GRAY);
      break;
    }

  game::SetStandardListAttributes(Info);
  Info.Draw();
}

bool character::CanUseStethoscope(bool PrintReason) const
{
  if(PrintReason)
    ADD_MESSAGE("This type of monster can't use a stethoscope.");

  return false;
}

/* Effect used by at least Sophos. 
 * NOTICE: Doesn't check for death! */

void character::TeleportSomePartsAway(int NumberToTeleport)
{
  for(int c = 0; c < NumberToTeleport; ++c)
    {
      int RandomBodyPart = GetRandomNonVitalBodyPart();

      if(RandomBodyPart == NONE_INDEX)
	{
	  for(; c < NumberToTeleport; ++c)
	    {
	      GetTorso()->SetHP((GetTorso()->GetHP() << 2) / 5);
	      long TorsosVolume = GetTorso()->GetMainMaterial()->GetVolume() / 10;

	      if(!TorsosVolume)
		break;
	      
	      long Amount = (RAND() % TorsosVolume) + 1;
	      item* Lump = GetTorso()->GetMainMaterial()->CreateNaturalForm(Amount);
	      GetTorso()->GetMainMaterial()->EditVolume(-Amount);
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
		ADD_MESSAGE("Your %s teleports away.", GetBodyPartName(RandomBodyPart).CStr());
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s %s teleports away.", GetPossessivePronoun().CStr(), GetBodyPartName(RandomBodyPart).CStr());
	    }
	  else
	    {
	      if(IsPlayer())
		ADD_MESSAGE("Your %s disappears.", GetBodyPartName(RandomBodyPart).CStr());
	      else if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s %s dissappears.", GetPossessivePronoun().CStr(), GetBodyPartName(RandomBodyPart).CStr());
	    }
	}	
    }
}

/* Returns an index of a random bodypart that is not vital. If no non-vital bodypart is found returns NONE_INDEX */

int character::GetRandomNonVitalBodyPart() const
{
  int OKBodyPart[MAX_BODYPARTS];
  int OKBodyParts = 0;

  for(int c = 0; c < BodyParts; ++c)
    if(GetBodyPart(c) && !BodyPartIsVital(c))
      OKBodyPart[OKBodyParts++] = c;

  return OKBodyParts ? OKBodyPart[RAND() % OKBodyParts] : NONE_INDEX;
}

void character::CalculateVolumeAndWeight()
{
  Volume = Stack->GetVolume();
  Weight = Stack->GetWeight();
  BodyVolume = 0;
  CarriedWeight = Weight;

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	{
	  BodyVolume += BodyPart->GetBodyPartVolume();
	  Volume += BodyPart->GetVolume();
	  CarriedWeight += BodyPart->GetCarriedWeight();
	  Weight += BodyPart->GetWeight();
	}
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

void character::SignalEmitationIncrease(color24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
    {
      game::CombineLights(Emitation, EmitationUpdate);

      if(MotherEntity)
	MotherEntity->SignalEmitationIncrease(EmitationUpdate);
      else if(SquareUnder[0] && !game::IsInWilderness())
	for(int c = 0; c < GetSquaresUnder(); ++c)
	  GetLSquareUnder()->SignalEmitationIncrease(EmitationUpdate);
    }
}

void character::SignalEmitationDecrease(color24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
    {
      color24 Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation)
	{
	  if(MotherEntity)
	    MotherEntity->SignalEmitationDecrease(EmitationUpdate);
	  else if(SquareUnder[0] && !game::IsInWilderness())
	    for(int c = 0; c < GetSquaresUnder(); ++c)
	      GetLSquareUnder(c)->SignalEmitationDecrease(EmitationUpdate);
	}
    }
}

void character::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	game::CombineLights(Emitation, BodyPart->GetEmitation());
    }

  game::CombineLights(Emitation, Stack->GetEmitation());
}

void character::CalculateAll()
{
  Initializing = true;
  CalculateAttributeBonuses();
  CalculateVolumeAndWeight();
  CalculateEmitation();
  CalculateBodyPartMaxHPs(0);
  CalculateMaxStamina();
  CalculateBurdenState();
  CalculateBattleInfo();
  Initializing = false;
}

void character::CalculateHP()
{
  HP = SumBodyPartProperties(this, &bodypart::GetHP);
}

void character::CalculateMaxHP()
{
  MaxHP = SumBodyPartProperties(this, &bodypart::GetMaxHP);
}

void character::CalculateBodyPartMaxHPs(ulong Flags)
{
  DoForBodyParts(this, &bodypart::CalculateMaxHP, Flags);

  /*for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->CalculateMaxHP(MayChangeHPs, CheckUsability);
    }*/

  CalculateMaxHP();
  CalculateHP();
}

bool character::EditAttribute(int Identifier, int Value)
{
  if(Identifier == ENDURANCE && UseMaterialAttributes())
    return false;

  if(RawEditAttribute(BaseExperience[Identifier], Value))
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

bool character::ActivateRandomState(int Flags, int Time, long Seed)
{
  femath::SaveSeed();

  if(Seed)
    femath::SetSeed(Seed);

  long ToBeActivated = GetRandomState(Flags|DUR_TEMPORARY);
  femath::LoadSeed();

  if(!ToBeActivated)
    return false;

  BeginTemporaryState(ToBeActivated, Time);
  return true;
}

bool character::GainRandomIntrinsic(int Flags)
{
  long ToBeActivated = GetRandomState(Flags|DUR_PERMANENT);

  if(!ToBeActivated)
    return false;

  GainIntrinsic(ToBeActivated);
  return true;
}

/* Returns 0 if state not found */

long character::GetRandomState(int Flags) const
{
  long OKStates[STATES];
  int NumberOfOKStates = 0;

  for(int c = 0; c < STATES; ++c)
    if(StateData[c].Flags & Flags & DUR_FLAGS && StateData[c].Flags & Flags & SRC_FLAGS)
      OKStates[NumberOfOKStates++] = 1 << c;
  
  return NumberOfOKStates ? OKStates[RAND() % NumberOfOKStates] : 0;
}

int characterprototype::CreateSpecialConfigurations(characterdatabase** TempConfig, int Configs)
{
  if(TempConfig[0]->CreateDivineConfigurations)
    Configs = databasecreator<character>::CreateDivineConfigurations(this, TempConfig, Configs);

  if(TempConfig[0]->CreateGolemMaterialConfigurations)
    for(int c = 1; c < protocontainer<material>::GetSize(); ++c)
      {
	const material::prototype* Proto = protocontainer<material>::GetProto(c);
	const material::database*const* MaterialConfigData = Proto->GetConfigData();
	const material::database*const* End = MaterialConfigData + Proto->GetConfigSize();

	//for(material::databasemap::const_iterator i = MaterialConfig.begin(); i != MaterialConfig.end(); ++i)
	for(++MaterialConfigData; MaterialConfigData != End; ++MaterialConfigData)
	  {
	    const material::database* MaterialDataBase = *MaterialConfigData;

	    if(MaterialDataBase->IsGolemMaterial)
	      {
		character::database* ConfigDataBase = new character::database(**TempConfig);
		ConfigDataBase->InitDefaults(this, MaterialDataBase->Config);
		ConfigDataBase->Adjective = MaterialDataBase->NameStem;
		ConfigDataBase->AdjectiveArticle = MaterialDataBase->Article;
		ConfigDataBase->AttachedGod = MaterialDataBase->AttachedGod;
		TempConfig[Configs++] = ConfigDataBase;
	      }
	  }
      }

  return Configs;
}

double character::GetTimeToDie(const character* Enemy, int Damage, double ToHitValue, bool AttackIsBlockable, bool UseMaxHP) const
{
  double DodgeValue = GetDodgeValue();

  if(!Enemy->CanBeSeenBy(this, true))
    ToHitValue *= 2;

  if(!CanBeSeenBy(Enemy, true))
    DodgeValue *= 2;

  double MinHits = 1000;
  bool First = true;

  for(int c = 0; c < BodyParts; ++c)
    if(BodyPartIsVital(c) && GetBodyPart(c))
      {
	double Hits = GetBodyPart(c)->GetTimeToDie(Damage, ToHitValue, DodgeValue, AttackIsBlockable, UseMaxHP);

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

double character::GetRelativeDanger(const character* Enemy, bool UseMaxHP) const
{
  double Danger = Enemy->GetTimeToKill(this, UseMaxHP) / GetTimeToKill(Enemy, UseMaxHP);
  int EnemyAP = Enemy->GetMoveAPRequirement(1);
  int ThisAP = GetMoveAPRequirement(1);

  if(EnemyAP > ThisAP)
    Danger *= 1.25;
  else if(ThisAP > EnemyAP)
    Danger *= 0.80;

  if(!Enemy->CanBeSeenBy(this, true))
    Danger *= Enemy->IsPlayer() ? 0.2 : 0.5;

  if(!CanBeSeenBy(Enemy, true))
    Danger *= IsPlayer() ? 5. : 2.;

  if(GetAttribute(INTELLIGENCE) < 10 && !IsPlayer())
    Danger *= 0.80;

  if(Enemy->GetAttribute(INTELLIGENCE) < 10 && !Enemy->IsPlayer())
    Danger *= 1.25;

  return Limit(Danger, 0.001, 1000.);
}

festring character::GetBodyPartName(int I, bool Articled) const
{
  if(I == TORSO_INDEX)
    return Articled ? CONST_S("a torso") : CONST_S("torso");
  else
    {
      ABORT("Illegal character bodypart name request!");
      return "";
    }
}

item* character::SearchForItem(ulong ID) const
{
  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment->GetID() == ID)
	return Equipment;
    }

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->GetID() == ID)
      return *i;

  return 0;
}

bool character::ContentsCanBeSeenBy(const character* Viewer) const
{
  return Viewer == this;
}

bool character::HitEffect(character* Enemy, item* Weapon, vector2d HitPos, int Type, int BodyPartIndex, int Direction, bool BlockedByArmour)
{
  if(Weapon)
    return Weapon->HitEffect(this, Enemy, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  switch(Type)
    {
    case UNARMED_ATTACK:
      return Enemy->SpecialUnarmedEffect(this, HitPos, BodyPartIndex, Direction, BlockedByArmour);
    case KICK_ATTACK:
      return Enemy->SpecialKickEffect(this, HitPos, BodyPartIndex, Direction, BlockedByArmour);
    case BITE_ATTACK:
      return Enemy->SpecialBiteEffect(this, HitPos, BodyPartIndex, Direction, BlockedByArmour);
    }

  return false;
}

void character::WeaponSkillHit(item* Weapon, int Type, int Hits)
{
  int Category;

  switch(Type)
    {
    case UNARMED_ATTACK:
      Category = UNARMED;
      break;
    case WEAPON_ATTACK:
      Weapon->WeaponSkillHit(Hits);
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

  if(GetCWeaponSkill(Category)->AddHit(Hits))
    {
      CalculateBattleInfo();

      if(IsPlayer())
	GetCWeaponSkill(Category)->AddLevelUpMessage(Category);
    }
}

/* Returns 0 if character cannot be duplicated */

character* character::Duplicate(ulong Flags)
{
  if(!(Flags & IGNORE_PROHIBITIONS) && !CanBeCloned())
    return 0;

  character* Char = RawDuplicate();

  if(Flags & MIRROR_IMAGE)
    {
      DuplicateEquipment(Char, Flags & ~IGNORE_PROHIBITIONS);
      Char->SetLifeExpectancy(Flags >> LE_BASE_SHIFT & LE_BASE_RANGE,
			      Flags >> LE_RAND_SHIFT & LE_RAND_RANGE);
    }

  Char->CalculateAll();
  Char->CalculateEmitation();
  Char->UpdatePictures();
  Char->Initializing = Char->InNoMsgMode = false;
  return Char;
}

bool character::TryToEquip(item* Item)
{
  if(!Item->AllowEquip()
  || !CanUseEquipment()
  || GetAttribute(WISDOM) >= Item->GetWearWisdomLimit()
  || Item->GetSquaresUnder() != 1)
    return false;

  for(int e = 0; e < GetEquipmentSlots(); ++e)
    if(CanUseEquipment(e))
      {
	sorter Sorter = EquipmentSorter(e);

	if((Sorter == 0 || (Item->*Sorter)(this))
	&& ((e != RIGHT_WIELDED_INDEX && e != LEFT_WIELDED_INDEX)
	 || Item->IsWeapon(this)
	 || Item->IsShield(this))
	&& AllowEquipment(Item, e))
	  {
	    item* OldEquipment = GetEquipment(e);

	    if(BoundToUse(OldEquipment, e))
	      continue;

	    lsquare* LSquareUnder = GetLSquareUnder();
	    stack* StackUnder = LSquareUnder->GetStack();
	    msgsystem::DisableMessages();
	    PictureUpdatesForbidden = true;
	    LSquareUnder->Freeze();
	    StackUnder->Freeze();
	    double Danger = GetRelativeDanger(PLAYER);

	    if(OldEquipment)
	      OldEquipment->RemoveFromSlot();

	    Item->RemoveFromSlot();
	    SetEquipment(e, Item);
	    double NewDanger = GetRelativeDanger(PLAYER);
	    Item->RemoveFromSlot();
	    StackUnder->AddItem(Item);

	    if(OldEquipment)
	      SetEquipment(e, OldEquipment);

	    msgsystem::EnableMessages();
	    PictureUpdatesForbidden = false;
	    LSquareUnder->UnFreeze();
	    StackUnder->UnFreeze();

	    if(OldEquipment)
	      {
		if(NewDanger > Danger || BoundToUse(Item, e))
		  {
		    room* Room = GetRoom();

		    if(!Room || Room->PickupItem(this, Item, 1))
		      {
			if(CanBeSeenByPlayer())
			  ADD_MESSAGE("%s drops %s %s and equips %s instead.", CHAR_NAME(DEFINITE), CHAR_POSSESSIVE_PRONOUN, OldEquipment->CHAR_NAME(UNARTICLED), Item->CHAR_NAME(INDEFINITE));

			if(Room)
			  Room->DropItem(this, OldEquipment, 1);

			OldEquipment->MoveTo(StackUnder);
			Item->RemoveFromSlot();
			SetEquipment(e, Item);
			DexterityAction(5);
			return true;
		      }
		  }
	      }
	    else
	      {
		if(NewDanger > Danger
		|| (NewDanger == Danger
		 && e != RIGHT_WIELDED_INDEX && e != LEFT_WIELDED_INDEX)
		|| BoundToUse(Item, e))
		  {
		    room* Room = GetRoom();

		    if(!Room || Room->PickupItem(this, Item, 1))
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
      }

  return false;
}

bool character::TryToConsume(item* Item)
{
  if(Item->IsConsumable(this) && Item->CanBeEatenByAI(this)
  && (!GetRoom() || GetRoom()->ConsumeItem(this, Item, 1)))
    {
      ConsumeItem(Item, Item->GetConsumeMaterial(this)->GetConsumeVerb());
      return true;
    }
  else
    return false;
}

void character::UpdateESPLOS() const
{
  if(StateIsActivated(ESP) && !game::IsInWilderness())
    for(int c = 0; c < game::GetTeams(); ++c)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
	  (*i)->SendNewDrawRequest();
}

int character::GetCWeaponSkillLevel(const item* Item) const
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

void character::CheckPanic(int Ticks)
{
  if(GetPanicLevel() > 1 && !StateIsActivated(PANIC)
  && GetHP() * 100 < RAND() % (GetPanicLevel() * GetMaxHP() << 1))
    BeginTemporaryState(PANIC, ((Ticks * 3) >> 2) + RAND() % ((Ticks >> 1) + 1)); // 25% randomness to ticks...
}

/* returns 0 if fails else the newly created character */

character* character::DuplicateToNearestSquare(character* Cloner, ulong Flags)
{
  character* NewlyCreated = Duplicate(Flags);

  if(!NewlyCreated)
    return 0;

  if(Flags & CHANGE_TEAM && Cloner)
    NewlyCreated->ChangeTeam(Cloner->GetTeam());

  NewlyCreated->PutNear(GetPos());
  return NewlyCreated;
}

void character::SignalSpoil()
{
  if(GetMotherEntity())
    GetMotherEntity()->SignalSpoil(0);
  else
    Disappear(0, "spoil", &item::IsVeryCloseToSpoiling);
}

bool character::CanHeal() const
{
  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->CanRegenerate() && BodyPart->GetHP() < BodyPart->GetMaxHP())
	return true;
    }

  return false;
}

int character::GetRelation(const character* Who) const
{
  return GetTeam()->GetRelation(Who->GetTeam());
}

void character::CalculateAttributeBonuses()
{
  int c;

  for(c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->CalculateAttributeBonuses();
    }

  int BackupBonus[BASE_ATTRIBUTES];
  int BackupCarryingBonus = CarryingBonus;
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
      {
	CalculateBodyPartMaxHPs();
	CalculateMaxStamina();
      }

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
      CalculateMaxStamina();
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

  if((Amount >= 100 || RAND_N(100) < Amount) && StateIsActivated(PARASITIZED))
    {
      if(IsPlayer())
	ADD_MESSAGE("Something in your belly didn't seem to like this stuff.");

      DeActivateTemporaryState(PARASITIZED);
      Amount -= Min(100L, Amount);
    }
  
  if((Amount >= 100 || RAND_N(100) < Amount) && StateIsActivated(LEPROSY))
    {
      if(IsPlayer())
	ADD_MESSAGE("You are not falling to pieces anymore.");

      DeActivateTemporaryState(LEPROSY);
      Amount -= Min(100L, Amount);
    }
}

void character::AddAntidoteConsumeEndMessage() const
{
  if(StateIsActivated(POISONED)) // true only if the antidote didn't cure the poison completely
    {
      if(IsPlayer())
	ADD_MESSAGE("Your body processes the poison in your veins with rapid speed.");
    }
}

bool character::IsDead() const
{
  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPartIsVital(c) && (!BodyPart || BodyPart->GetHP() < 1))
	return true;
    }

  return false;
}

void character::SignalSpoilLevelChange()
{
  if(GetMotherEntity())
    GetMotherEntity()->SignalSpoilLevelChange(0);
  else
    UpdatePictures();
}

void character::AddOriginalBodyPartID(int I, ulong What)
{
  if(std::find(OriginalBodyPartID[I].begin(), OriginalBodyPartID[I].end(), What) == OriginalBodyPartID[I].end())
    {
      OriginalBodyPartID[I].push_back(What);

      if(OriginalBodyPartID[I].size() > 100)
	OriginalBodyPartID[I].erase(OriginalBodyPartID[I].begin());
    }
}

void character::AddToInventory(const fearray<contentscript<item> >& ItemArray, int SpecialFlags)
{
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
		Stack->AddItem(Item);
		Item->SpecialGenerationHandler();
	      }
	  }
      }
}

bool character::HasHadBodyPart(const item* Item) const
{
  for(int c = 0; c < BodyParts; ++c)
    if(std::find(OriginalBodyPartID[c].begin(), OriginalBodyPartID[c].end(), Item->GetID()) != OriginalBodyPartID[c].end())
      return true;

  return GetPolymorphBackup() && GetPolymorphBackup()->HasHadBodyPart(Item);
}

festring& character::ProcessMessage(festring& Msg) const
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
  SEARCH_N_REPLACE("@Nu", GetName(UNARTICLED).CapitalizeCopy());
  SEARCH_N_REPLACE("@Ni", GetName(INDEFINITE).CapitalizeCopy());
  SEARCH_N_REPLACE("@Nd", GetName(DEFINITE).CapitalizeCopy());
  SEARCH_N_REPLACE("@Du", GetDescription(UNARTICLED).CapitalizeCopy());
  SEARCH_N_REPLACE("@Di", GetDescription(INDEFINITE).CapitalizeCopy());
  SEARCH_N_REPLACE("@Dd", GetDescription(DEFINITE).CapitalizeCopy());
  SEARCH_N_REPLACE("@Pp", GetPersonalPronoun().CapitalizeCopy());
  SEARCH_N_REPLACE("@Sp", GetPossessivePronoun().CapitalizeCopy());
  SEARCH_N_REPLACE("@Op", GetObjectPronoun().CapitalizeCopy());
  SEARCH_N_REPLACE("@Gd", GetMasterGod()->GetName());
  return Msg;
}

void character::ProcessAndAddMessage(festring Msg) const
{
  ADD_MESSAGE("%s", ProcessMessage(Msg).CStr());
}

void character::BeTalkedTo()
{
  static long Said;

  if(GetRelation(PLAYER) == HOSTILE)
    ProcessAndAddMessage(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().Size)]);
  else
    ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().Size)]);
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

void character::DamageAllItems(character* Damager, int Damage, int Type)
{
  GetStack()->ReceiveDamage(Damager, Damage, Type);

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	Equipment->ReceiveDamage(Damager, Damage, Type);
    }
}

bool character::Equips(const item* Item) const
{
  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment->GetID() == Item->GetID())
	return true;
    }

  return false;
}

void character::PrintBeginConfuseMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel quite happy.");
}

void character::PrintEndConfuseMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("The world is boring again.");
}

vector2d character::ApplyStateModification(vector2d TryDirection) const
{
  if(!StateIsActivated(CONFUSED) || RAND() & 15 || game::IsInWilderness())
    return TryDirection;
  else
    {
      vector2d To = GetLevel()->GetFreeAdjacentSquare(this, GetPos(), true);

      if(To == ERROR_VECTOR)
	return TryDirection;
      else
	{
	  To -= GetPos();

	  if(To != TryDirection && IsPlayer())
	    ADD_MESSAGE("Whoa! You somehow don't manage to walk straight.");

	  return To;
	}
    }
}

void character::AddConfuseHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("This stuff is confusing.");
}

item* character::SelectFromPossessions(const festring& Topic, sorter SorterFunction)
{
  itemvector ReturnVector;
  SelectFromPossessions(ReturnVector, Topic, NO_MULTI_SELECT, SorterFunction);
  return !ReturnVector.empty() ? ReturnVector[0] : 0;
}

void character::SelectFromPossessions(itemvector& ReturnVector, const festring& Topic, int Flags, sorter SorterFunction)
{
  if(!CanUseEquipment())
    {
      GetStack()->DrawContents(ReturnVector, this, Topic, Flags, SorterFunction);
      return;
    }

  felist List(Topic);
  bool InventoryPossible = GetStack()->SortedItems(this, SorterFunction);

  if(InventoryPossible)
    List.AddEntry(CONST_S("choose from inventory"), LIGHT_GRAY, 20, game::AddToItemDrawVector(itemvector()));

  bool Any = false;
  itemvector Item;
  festring Entry;

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && (SorterFunction == 0 || (Equipment->*SorterFunction)(this)))
	{
	  Item.push_back(Equipment);
	  Entry = GetEquipmentName(c);
	  Entry << ':';
	  Entry.Resize(20);
	  Equipment->AddInventoryEntry(this, Entry, 1, true);
	  AddSpecialEquipmentInfo(Entry, c);
	  int ImageKey = game::AddToItemDrawVector(itemvector(1, Equipment));
	  List.AddEntry(Entry, LIGHT_GRAY, 20, ImageKey, true);
	  Any = true;
	}
    }

  if(Any)
    {
      game::SetStandardListAttributes(List);
      List.SetFlags(SELECTABLE|DRAW_BACKGROUND_AFTERWARDS);
      List.SetEntryDrawer(game::ItemEntryDrawer);
      game::DrawEverythingNoBlit();
      int Chosen = List.Draw();
      game::ClearItemDrawVector();

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
    {
      game::ClearItemDrawVector();
      GetStack()->DrawContents(ReturnVector, this, Topic, Flags, SorterFunction);
    }
}

bool character::EquipsSomething(sorter SorterFunction)
{
  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && (SorterFunction == 0 || (Equipment->*SorterFunction)(this)))
	return true;
    }

  return false;
}

material* character::CreateBodyPartMaterial(int, long Volume) const
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

bool character::MoveTowardsHomePos()
{
  if(HomeDataIsValid() && IsEnabled())
    {
      SetGoingTo(HomeData->Pos);
      return MoveTowardsTarget(false)
	  || (!GetPos().IsAdjacent(HomeData->Pos) && MoveRandomly());
    }
  else
    return false;
}

bool character::TryToChangeEquipment(stack* MainStack, stack* SecStack, int Chosen)
{
  if(!GetBodyPartOfEquipment(Chosen))
    {
      ADD_MESSAGE("Bodypart missing!");
      return false;
    }

  item* OldEquipment = GetEquipment(Chosen);

  if(!IsPlayer() && BoundToUse(OldEquipment, Chosen))
    {
      ADD_MESSAGE("%s refuses to unequip %s.", CHAR_DESCRIPTION(DEFINITE), OldEquipment->CHAR_NAME(DEFINITE));
      return false;
    }

  if(OldEquipment)
    GetEquipment(Chosen)->MoveTo(MainStack);

  sorter Sorter = EquipmentSorter(Chosen);

  if(!MainStack->SortedItems(this, Sorter)
  && (!SecStack || !SecStack->SortedItems(this, Sorter)))
    {
      ADD_MESSAGE("You haven't got any item that could be used for this purpose.");
      return false;
    }
  else
    {
      game::DrawEverythingNoBlit();
      itemvector ItemVector;
      //int Return = MainStack->DrawContents(ItemVector, this, CONST_S("Choose ") + GetEquipmentName(Chosen) + ':', NONE_AS_CHOICE|NO_MULTI_SELECT, Sorter);
      int Return = MainStack->DrawContents(ItemVector,
					   SecStack,
					   this,
					   CONST_S("Choose ") + GetEquipmentName(Chosen) + ':',
					   SecStack ? CONST_S("Items in your inventory") : CONST_S(""),
					   SecStack ? festring(CONST_S("Items in ") + GetPossessivePronoun() + " inventory") : CONST_S(""),
					   SecStack ? festring(GetDescription(DEFINITE) + " is " + GetVerbalBurdenState()) : CONST_S(""),
					   GetVerbalBurdenStateColor(),
					   NONE_AS_CHOICE|NO_MULTI_SELECT,
					   Sorter);


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
	    Item->MoveTo(MainStack); // small bug?
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
  EditNP(-5);

  if(!(RAND() % 250))
    {
      if(IsPlayer())
	ADD_MESSAGE("Ugh. You feel something violently carving its way through your intestines.");

      ReceiveDamage(0, 1, POISON, TORSO, 8, false, false, false, false);
      CheckDeath(CONST_S("killed by a vile parasite"), 0);
    }
}

bool character::CanFollow() const
{
  return CanMove() && !StateIsActivated(PANIC) && !IsStuck();
}

festring character::GetKillName() const
{
  if(!GetPolymorphBackup())
    return GetName(INDEFINITE);
  else
    {
      festring KillName;
      GetPolymorphBackup()->AddName(KillName, INDEFINITE);
      KillName << " polymorphed into ";
      id::AddName(KillName, INDEFINITE);
      return KillName;
    }
}

festring character::GetPanelName() const
{
  festring Name;
  Name << AssignedName << " the " << game::GetVerbalPlayerAlignment() << ' ';
  id::AddName(Name, UNARTICLED);
  return Name;
}

long character::GetMoveAPRequirement(int Difficulty) const
{
  return (!StateIsActivated(PANIC) ? 10000000 : 8000000) * Difficulty / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
}

bodypart* character::HealHitPoint()
{
  int NeedHeal = 0, NeedHealIndex[MAX_BODYPARTS];

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->CanRegenerate() && BodyPart->GetHP() < BodyPart->GetMaxHP())
	NeedHealIndex[NeedHeal++] = c;
    }

  if(NeedHeal)
    {
      bodypart* BodyPart = GetBodyPart(NeedHealIndex[RAND() % NeedHeal]);
      BodyPart->IncreaseHP();
      ++HP;
      return BodyPart;
    }
  else
    return 0;
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

void character::SetBodyPart(int I, bodypart* What)
{
  BodyPartSlot[I].PutInItem(What);

  if(What)
    {
      What->SignalPossibleUsabilityChange();
      What->Disable();
      AddOriginalBodyPartID(I, What->GetID());

      if(What->GetMainMaterial()->IsInfectedByLeprosy())
	GainIntrinsic(LEPROSY);
      else if(StateIsActivated(LEPROSY))
	What->GetMainMaterial()->SetIsInfectedByLeprosy(true);
    }
}

bool character::ConsumeItem(item* Item, const festring& ConsumeVerb)
{
  if(IsPlayer() && HasHadBodyPart(Item) && !game::BoolQuestion(CONST_S("Are you sure? You may be able to put it back... [y/N]")))
    return false;

  if(Item->IsOnGround() && GetRoom() && !GetRoom()->ConsumeItem(this, Item, 1))
    return false;
     
  if(IsPlayer())
    ADD_MESSAGE("You begin %s %s.", ConsumeVerb.CStr(), Item->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s begins %s %s.", CHAR_NAME(DEFINITE), ConsumeVerb.CStr(), Item->CHAR_NAME(DEFINITE));

  consume* Consume = new consume(this);
  Consume->SetDescription(ConsumeVerb);
  Consume->SetConsumingID(Item->GetID());
  SetAction(Consume);
  DexterityAction(5);
  return true;
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

void character::GetHitByExplosion(const explosion* Explosion, int Damage)
{
  int DamageDirection = GetPos() == Explosion->Pos ? RANDOM_DIR : game::CalculateRoughDirection(GetPos() - Explosion->Pos);

  if(!IsPet() && Explosion->Terrorist && Explosion->Terrorist->IsPet())
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

  if(IsEnabled())
    {
      ReceiveDamage(Explosion->Terrorist, Damage >> 1, PHYSICAL_DAMAGE, ALL, DamageDirection, true, false, false, false);
      CheckDeath(Explosion->DeathMsg, Explosion->Terrorist);
    }
}

void character::SortAllItems(itemvector& AllItems, const character* Character, sorter Sorter)
{
  GetStack()->SortAllItems(AllItems, Character, Sorter);

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	Equipment->SortAllItems(AllItems, Character, Sorter);
    }
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

void character::Search(int Perception)
{
  for(int d = 0; d < GetExtendedNeighbourSquares(); ++d)
    {
      lsquare* LSquare = GetNeighbourLSquare(d);

      if(LSquare) 
	LSquare->GetStack()->Search(this, Min(Perception, 200));
    }
}

// surprisingly returns 0 if fails

character* character::GetRandomNeighbour(int RelationFlags) const
{
  character* Chars[MAX_NEIGHBOUR_SQUARES];
  int Index = 0;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      lsquare* LSquare = GetNeighbourLSquare(d);

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
  for(int c = 0; c < STATES; ++c)
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

void characterdatabase::InitDefaults(const characterprototype* NewProtoType, int NewConfig)
{
  IsAbstract = false;
  ProtoType = NewProtoType;
  Config = NewConfig;
  Alias.Clear();
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
  if(GetStack()->GetItems() && game::BoolQuestion(CONST_S("Do you want to see your inventory? [y/n]"), REQUIRES_ANSWER))
    {
      GetStack()->DrawContents(this, CONST_S("Your inventory"), NO_SELECT);

      for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
	i->DrawContents(this);

      for(int c = 0; c < GetEquipmentSlots(); ++c)
	if(GetEquipment(c))
	  GetEquipment(c)->DrawContents(this);
    }

  if(game::BoolQuestion(CONST_S("Do you want to see your message history? [y/n]"), REQUIRES_ANSWER))
    msgsystem::DrawMessageHistory();

  if(!game::MassacreListsEmpty() && game::BoolQuestion(CONST_S("Do you want to see your massacre history? [y/n]"), REQUIRES_ANSWER))
    game::DisplayMassacreLists();
}

bool character::EditAllAttributes(int Amount)
{
  int c;
  bool MayEditMore = false;

  for(c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->EditAllAttributes(Amount))
	MayEditMore = true;
    }

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    if(BaseExperience[c])
      {
	BaseExperience[c] += Amount * EXP_MULTIPLIER;
	LimitRef(BaseExperience[c], MIN_EXP, MAX_EXP);

	if((Amount < 0 && BaseExperience[c] != MIN_EXP)
	|| (Amount > 0 && BaseExperience[c] != MAX_EXP))
	  MayEditMore = true;
      }

  CalculateAll();
  RestoreHP();
  RestoreStamina();

  if(IsPlayer())
    {
      game::SendLOSUpdateRequest();
      UpdateESPLOS();
    }

  return MayEditMore;
}

#ifdef WIZARD

void character::AddAttributeInfo(festring& Entry) const
{
  Entry.Resize(57);
  Entry << GetAttribute(ENDURANCE);
  Entry.Resize(60);
  Entry << GetAttribute(PERCEPTION);
  Entry.Resize(63);
  Entry << GetAttribute(INTELLIGENCE);
  Entry.Resize(66);
  Entry << GetAttribute(WISDOM);
  Entry.Resize(69);
  Entry << GetAttribute(CHARISMA);
  Entry.Resize(72);
  Entry << GetAttribute(MANA);
}

void character::AddDefenceInfo(felist& List) const
{
  festring Entry;

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	{
	  Entry = CONST_S("   ");
	  Entry << BodyPart->GetBodyPartName();
	  Entry.Resize(60);
	  Entry << BodyPart->GetMaxHP();
	  Entry.Resize(70);
	  Entry << BodyPart->GetTotalResistance(PHYSICAL_DAMAGE);
	  List.AddEntry(Entry, LIGHT_GRAY);
	}
    }
}

void character::DetachBodyPart()
{
  ADD_MESSAGE("You haven't got any extra bodyparts.");
}

#endif

void character::ReceiveHolyBanana(long Amount)
{
  Amount <<= 1;
  EditExperience(ARM_STRENGTH, Amount, 1 << 13);
  EditExperience(LEG_STRENGTH, Amount, 1 << 13);
  EditExperience(DEXTERITY, Amount, 1 << 13);
  EditExperience(AGILITY, Amount, 1 << 13);
  EditExperience(ENDURANCE, Amount, 1 << 13);
  EditExperience(PERCEPTION, Amount, 1 << 13);
  EditExperience(INTELLIGENCE, Amount, 1 << 13);
  EditExperience(WISDOM, Amount, 1 << 13);
  EditExperience(CHARISMA, Amount, 1 << 13);
  RestoreLivingHP();
}

void character::AddHolyBananaConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel a mysterious strengthening fire coursing through your body.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("For a moment %s is surrounded by a swirling fire aura.", CHAR_NAME(DEFINITE));
}

bool character::PreProcessForBone()
{
  if(IsPet() && IsEnabled())
    {
      Die();
      return true;
    }

  if(GetAction())
    GetAction()->Terminate(false);

  if(TemporaryStateIsActivated(POLYMORPHED))
    {
      character* PolymorphBackup = GetPolymorphBackup();
      EndPolymorph();
      PolymorphBackup->PreProcessForBone();
      return true;
    }

  if(MustBeRemovedFromBone())
    return false;
  else if(IsUnique() && !CanBeGenerated())
    game::SignalQuestMonsterFound();

  RestoreLivingHP();
  ResetStates();
  RemoveTraps();
  GetStack()->PreProcessForBone();
  int c;

  for(c = 0; c < GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->PreProcessForBone();
    }

  for(c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	Equipment->PreProcessForBone();
    }

  game::RemoveCharacterID(ID);
  ID = -ID;
  game::AddCharacterID(this, ID);
  return true;
}

bool character::PostProcessForBone(double& DangerSum, int& Enemies)
{
  if(PostProcessForBone())
    {
      if(GetRelation(PLAYER) == HOSTILE)
	{
	  double Danger = GetRelativeDanger(PLAYER, true);

	  if(Danger > 99.)
	    game::SetTooGreatDangerFound(true);
	  else if(!IsUnique() && !IgnoreDanger())
	    {
	      DangerSum += Danger;
	      ++Enemies;
	    }
	}

      return true;
    }
  else
    return false;
}

bool character::PostProcessForBone()
{
  ulong NewID = game::CreateNewCharacterID(this);
  game::GetBoneCharacterIDMap().insert(std::pair<ulong, ulong>(-ID, NewID));
  game::RemoveCharacterID(ID);
  ID = NewID;

  if(IsUnique() && CanBeGenerated())
    {
      if(DataBase->Flags & HAS_BEEN_GENERATED)
	return false;
      else
	SignalGeneration();
    }

  GetStack()->PostProcessForBone();
  int c;

  for(c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	Equipment->PostProcessForBone();
    }

  for(c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
        BodyPart->PostProcessForBone();
    }

  return true;
}

void character::FinalProcessForBone()
{
  Player = false;
  GetStack()->FinalProcessForBone();
  int c;

  for(c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	Equipment->FinalProcessForBone();
    }

  for(c = 0; c < BodyParts; ++c)
    {
      for(std::list<ulong>::iterator i = OriginalBodyPartID[c].begin(); i != OriginalBodyPartID[c].end();)
	{
	  boneidmap::iterator BI = game::GetBoneItemIDMap().find(*i);

	  if(BI == game::GetBoneItemIDMap().end())
	    {
	      std::list<ulong>::iterator Dirt = i++;
	      OriginalBodyPartID[c].erase(Dirt);
	    }
	  else
	    {
	      *i = BI->second;
	      ++i;
	    }
	}
    }
}

bool character::HasRepairableBodyParts() const
{
  return CombineBodyPartPredicates<true>(this, &bodypart::IsRepairable);
  /*for(int c = 0; c < BodyParts; ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->IsRepairable())
      return true;

  return false;*/
}

void character::SetSoulID(ulong What)
{
  if(GetPolymorphBackup())
    GetPolymorphBackup()->SetSoulID(What);
}

bool character::SearchForItem(const item* Item) const
{
  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment == Item)
	return true;
    }

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(*i == Item)
      return true;

  return false;
}

item* character::SearchForItem(const sweaponskill* SWeaponSkill) const
{
  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && SWeaponSkill->IsSkillOf(Equipment))
	return Equipment;
    }

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(SWeaponSkill->IsSkillOf(*i))
      return *i;

  return 0;
}

void character::PutNear(vector2d Pos)
{
  vector2d NewPos = game::GetCurrentLevel()->GetNearestFreeSquare(this, Pos, false);

  if(NewPos == ERROR_VECTOR)
    do
      {
	NewPos = game::GetCurrentLevel()->GetRandomSquare(this);
      }
    while(NewPos == Pos);

  PutTo(NewPos);
}

void character::PutToOrNear(vector2d Pos)
{
  if(game::IsInWilderness() || (CanMoveOn(game::GetCurrentLevel()->GetLSquare(Pos)) && IsFreeForMe(game::GetCurrentLevel()->GetLSquare(Pos))))
    PutTo(Pos);
  else
    PutNear(Pos);
}

void character::PutTo(vector2d Pos)
{
  SquareUnder[0] = game::GetCurrentArea()->GetSquare(Pos);
  SquareUnder[0]->AddCharacter(this);
}

void character::Remove()
{
  SquareUnder[0]->RemoveCharacter();
  SquareUnder[0] = 0;
}

void character::SendNewDrawRequest() const
{
  for(int c = 0; c < SquaresUnder; ++c)
    {
      square* Square = GetSquareUnder(c);

      if(Square)
	Square->SendNewDrawRequest();
    }
}

bool character::IsOver(vector2d Pos) const
{
  for(int c = 0; c < SquaresUnder; ++c)
    {
      square* Square = GetSquareUnder(c);

      if(Square && Square->GetPos() == Pos)
	return true;
    }

  return false;
}

bool character::CanTheoreticallyMoveOn(const lsquare* LSquare) const
{
  return !!(GetMoveType() & LSquare->GetTheoreticalWalkability());
}

bool character::CanMoveOn(const lsquare* LSquare) const
{
  return !!(GetMoveType() & LSquare->GetWalkability());
}

bool character::CanMoveOn(const square* Square) const
{
  return !!(GetMoveType() & Square->GetSquareWalkability());
}

bool character::CanMoveOn(const olterrain* OLTerrain) const
{
  return !!(GetMoveType() & OLTerrain->GetWalkability());
}

bool character::CanMoveOn(const oterrain* OTerrain) const
{
  return !!(GetMoveType() & OTerrain->GetWalkability());
}

bool character::IsFreeForMe(square* Square) const
{
  return !Square->GetCharacter() || Square->GetCharacter() == this;
}

void character::LoadSquaresUnder()
{
  SquareUnder[0] = game::GetSquareInLoad();
}

bool character::AttackAdjacentEnemyAI()
{
  if(!IsEnabled())
    return false;

  character* Char[MAX_NEIGHBOUR_SQUARES];
  vector2d Pos[MAX_NEIGHBOUR_SQUARES];
  int Dir[MAX_NEIGHBOUR_SQUARES];
  int Index = 0;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
	{
	  character* Enemy = Square->GetCharacter();

	  if(Enemy && (GetRelation(Enemy) == HOSTILE || StateIsActivated(CONFUSED)))
	    {
	      Dir[Index] = d;
	      Pos[Index] = Square->GetPos();
	      Char[Index++] = Enemy;
	    }
	}
    }

  if(Index)
    {
      int ChosenIndex = RAND() % Index;
      Hit(Char[ChosenIndex], Pos[ChosenIndex], Dir[ChosenIndex]);
      return true;
    }

  return false;
}

void character::SignalStepFrom(lsquare** OldSquareUnder)
{
  int c;
  lsquare* NewSquareUnder[MAX_SQUARES_UNDER];

  for(c = 0; c < GetSquaresUnder(); ++c)
    NewSquareUnder[c] = GetLSquareUnder(c);

  for(c = 0; c < GetSquaresUnder(); ++c)
    if(IsEnabled() && GetLSquareUnder(c) == NewSquareUnder[c])
      NewSquareUnder[c]->StepOn(this, OldSquareUnder);
}

int character::GetSumOfAttributes() const
{
  return GetAttribute(ENDURANCE) + GetAttribute(PERCEPTION) + GetAttribute(INTELLIGENCE) + GetAttribute(WISDOM) + GetAttribute(CHARISMA) + GetAttribute(ARM_STRENGTH) + GetAttribute(AGILITY);
}

void character::IntelligenceAction(int Difficulty)
{
  EditAP(-20000 * Difficulty / APBonus(GetAttribute(INTELLIGENCE)));
  EditExperience(INTELLIGENCE, Difficulty * 15, 1 << 7);
}

struct walkabilitycontroller
{
  static bool Handler(int x, int y)
  {
    return x >= 0 && y >= 0 && x < LevelXSize && y < LevelYSize
	&& Map[x][y]->GetTheoreticalWalkability() & MoveType;
  }
  static lsquare*** Map;
  static int LevelXSize, LevelYSize;
  static int MoveType;
};

lsquare*** walkabilitycontroller::Map;
int walkabilitycontroller::LevelXSize, walkabilitycontroller::LevelYSize;
int walkabilitycontroller::MoveType;

bool character::CreateRoute()
{
  Route.clear();

  if(GetAttribute(INTELLIGENCE) >= 10 && !StateIsActivated(CONFUSED))
    {
      vector2d Pos = GetPos();
      walkabilitycontroller::Map = GetLevel()->GetMap();
      walkabilitycontroller::LevelXSize = GetLevel()->GetXSize();
      walkabilitycontroller::LevelYSize = GetLevel()->GetYSize();
      walkabilitycontroller::MoveType = GetMoveType();
      node* Node;

      for(int c = 0; c < game::GetTeams(); ++c)
	for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	  {
	    character* Char = *i;

	    if(Char->IsEnabled()
	    && !Char->Route.empty()
	    && Char->GetMoveType() & GetMoveType() == Char->GetMoveType())
	      {
		vector2d CharGoingTo = Char->Route[0];
		vector2d iPos = Char->Route.back();

		if((GoingTo - CharGoingTo).GetLengthSquare() <= 100
		&& (Pos - iPos).GetLengthSquare() <= 100
		&& mapmath<walkabilitycontroller>::DoLine(CharGoingTo.X, CharGoingTo.Y, GoingTo.X, GoingTo.Y, SKIP_FIRST)
		&& mapmath<walkabilitycontroller>::DoLine(Pos.X, Pos.Y, iPos.X, iPos.Y, SKIP_FIRST))
		  {
		    if(!Illegal.empty() && Illegal.find(Char->Route.back()) != Illegal.end())
		      continue;

		    Node = GetLevel()->FindRoute(CharGoingTo, GoingTo, Illegal, GetMoveType());

		    if(Node)
		      while(Node->Last)
			{
			  Route.push_back(Node->Pos);
			  Node = Node->Last;
			}
		    else
		      {
			Route.clear();
			continue;
		      }

		    Route.insert(Route.end(), Char->Route.begin(), Char->Route.end());
		    Node = GetLevel()->FindRoute(Pos, iPos, Illegal, GetMoveType());

		    if(Node)
		      while(Node->Last)
			{
			  Route.push_back(Node->Pos);
			  Node = Node->Last;
			}
		    else
		      {
			Route.clear();
			continue;
		      }

		    IntelligenceAction(1);
		    return true;
		  }
	      }
	  }

      Node = GetLevel()->FindRoute(Pos, GoingTo, Illegal, GetMoveType());

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

void character::SetGoingTo(vector2d What)
{
  if(GoingTo != What)
    {
      GoingTo = What;
      Route.clear();
      Illegal.clear();
    }
}

void character::TerminateGoingTo()
{
  GoingTo = ERROR_VECTOR;
  Route.clear();
  Illegal.clear();
}

bool character::CheckForFood(int Radius)
{
  if(StateIsActivated(PANIC) || !UsesNutrition() || !IsEnabled())
    return false;

  vector2d Pos = GetPos();
  int x, y;

  for(int r = 1; r <= Radius; ++r)
    {
      x = Pos.X - r;

      if(x >= 0)
	for(y = Pos.Y - r; y <= Pos.Y + r; ++y)
	  if(CheckForFoodInSquare(vector2d(x, y)))
	    return true;

      x = Pos.X + r;

      if(x < GetLevel()->GetXSize())
	for(y = Pos.Y - r; y <= Pos.Y + r; ++y)
	  if(CheckForFoodInSquare(vector2d(x, y)))
	    return true;

      y = Pos.Y - r;

      if(y >= 0)
	for(x = Pos.X - r; x <= Pos.X + r; ++x)
	  if(CheckForFoodInSquare(vector2d(x, y)))
	    return true;

      y = Pos.Y + r;

      if(y < GetLevel()->GetYSize())
	for(x = Pos.X - r; x <= Pos.X + r; ++x)
	  if(CheckForFoodInSquare(vector2d(x, y)))
	    return true;
    }

  return false;
}

bool character::CheckForFoodInSquare(vector2d Pos)
{
  level* Level = GetLevel();

  if(Level->IsValidPos(Pos))
    {
      lsquare* Square = Level->GetLSquare(Pos);
      stack* Stack = Square->GetStack();

      if(Stack->GetItems())
	for(stackiterator i = Stack->GetBottom(); i.HasItem(); ++i)
	  if(i->IsConsumable(this)
	  && i->IsPickable(this)
	  && i->CanBeSeenBy(this)
	  && i->CanBeEatenByAI(this)
	  && (!Square->GetRoomIndex()
	   || Square->GetRoom()->AllowFoodSearch()))
	    {
	      SetGoingTo(Pos);
	      return MoveTowardsTarget(false);
	    }
    }

  return false;
}

void character::SetConfig(int NewConfig, int SpecialFlags)
{
  databasecreator<character>::InstallDataBase(this, NewConfig);
  CalculateAll();
  CheckIfSeen();

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdatePictures();
}

bool character::IsOver(const item* Item) const
{
  for(int c1 = 0; c1 < Item->GetSquaresUnder(); ++c1)
    for(int c2 = 0; c2 < SquaresUnder; ++c2)
      if(Item->GetPos(c1) == GetPos(c2))
	return true;

  return false;
}

bool character::CheckConsume(const festring& Verb) const
{
  if(!UsesNutrition())
    {
      if(IsPlayer())
	ADD_MESSAGE("In this form you can't and don't need to %s.", Verb.CStr());

      return false;
    }

  return true;
}

void character::PutTo(lsquare* To)
{
  PutTo(To->GetPos());
}

double character::RandomizeBabyExperience(double SumE)
{
  if(!SumE)
    return 0;

  double E = (SumE / 4) - (SumE / 32) + (double(RAND()) / MAX_RAND) * (SumE / 16 + 1);
  return Limit(E, MIN_EXP, MAX_EXP);
  //return Limit<int>((SumA & 3 ? (SumA >> 2) + (RAND() % (SumA & 3) ? 1 : 0) : SumA >> 2) + (RAND() & 1) - (RAND() & 1), 0, 100);
}

liquid* character::CreateBlood(long Volume) const
{
  return new liquid(GetBloodMaterial(), Volume);
}

void character::SpillFluid(character* Spiller, liquid* Liquid, int SquareIndex)
{
  long ReserveVolume = Liquid->GetVolume() >> 1;
  Liquid->EditVolume(-ReserveVolume);
  GetStack()->SpillFluid(Spiller, Liquid, long(Liquid->GetVolume() * sqrt(double(GetStack()->GetVolume()) / GetVolume())));
  Liquid->EditVolume(ReserveVolume);
  int c;
  long Modifier[MAX_BODYPARTS], ModifierSum = 0;

  for(c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      {
	Modifier[c] = long(sqrt(GetBodyPart(c)->GetVolume()));

	if(Modifier[c])
	  Modifier[c] *= 1 + (RAND() & 3);

	ModifierSum += Modifier[c];
      }
    else
      Modifier[c] = 0;

  for(c = 1; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && IsEnabled())
      GetBodyPart(c)->SpillFluid(Spiller, Liquid->CloneLiquid(Liquid->GetVolume() * Modifier[c] / ModifierSum), SquareIndex);

  if(IsEnabled())
    {
      Liquid->SetVolume(Liquid->GetVolume() * Modifier[TORSO_INDEX] / ModifierSum);
      GetTorso()->SpillFluid(Spiller, Liquid, SquareIndex);
    }
}

void character::StayOn(liquid* Liquid)
{
  Liquid->TouchEffect(this, TORSO_INDEX);
}

bool character::IsAlly(const character* Char) const
{
  return Char->GetTeam()->GetID() == GetTeam()->GetID();
}

void character::ResetSpoiling()
{
  for(int c = 0; c < BodyParts; ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->ResetSpoiling();
}

item* character::SearchForItem(const character* Char, sorter Sorter) const
{
  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && (Equipment->*Sorter)(Char))
	return Equipment;
    }

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(((*i)->*Sorter)(Char))
      return *i;

  return 0;
}

bool character::DetectMaterial(const material* Material) const
{
  if(GetStack()->DetectMaterial(Material))
    return true;

  int c;

  for(c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->DetectMaterial(Material))
      return true;

  for(c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c) && GetEquipment(c)->DetectMaterial(Material))
      return true;

  return false;
}

bool character::DamageTypeDestroysBodyPart(int Type)
{
  return (Type&0xFFF) != PHYSICAL_DAMAGE;
}

bool character::CheckIfTooScaredToHit(const character* Enemy) const
{
  if(IsPlayer() && StateIsActivated(PANIC))
    for(int d = 0; d < GetNeighbourSquares(); ++d)
      {
	square* Square = GetNeighbourSquare(d);

	if(Square)
	  {
	    if(CanMoveOn(Square)
	    && (!Square->GetCharacter()
	     || Square->GetCharacter()->IsPet()))
	      {
		ADD_MESSAGE("You are too scared to attack %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
		return true;
	      }
	  }
      }

  return false;
}

void character::PrintBeginLevitationMessage() const
{
  if(!(GetMoveType() & FLY))
    if(IsPlayer())
      ADD_MESSAGE("You rise into the air like a small hot-air balloon.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s begins to float.", CHAR_NAME(DEFINITE));
}

void character::PrintEndLevitationMessage() const
{
  if(!(GetMoveType() & FLY))
    if(IsPlayer())
      ADD_MESSAGE("You descent gently onto the ground.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s drops onto the ground.", CHAR_NAME(DEFINITE));
}

bool character::IsLimbIndex(int I)
{
  switch(I)
    {
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX:
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX:
      return true;
    }

  return false;
}

void character::EditExperience(int Identifier, double Value, double Speed)
{
  if(!AllowExperience()
  || (Identifier == ENDURANCE && UseMaterialAttributes()))
    return;

  int Change = RawEditExperience(BaseExperience[Identifier],
				 GetNaturalExperience(Identifier),
				 Value, Speed);

  if(!Change)
    return;

  const char* PlayerMsg = 0, * NPCMsg = 0;

  switch(Identifier)
    {
    case ENDURANCE:
      if(Change > 0)
	{
	  PlayerMsg = "You feel tougher than anything!";

	  if(IsPet())
	    NPCMsg = "Suddenly %s looks tougher.";
	}
      else
	{
	  PlayerMsg = "You feel less healthy.";

	  if(IsPet())
	    NPCMsg = "Suddenly %s looks less healthy.";
	}

      CalculateBodyPartMaxHPs();
      CalculateMaxStamina();
      break;
    case PERCEPTION:
      if(IsPlayer())
	{
	  if(Change > 0)
	    PlayerMsg = "You now see the world in much better detail than before.";
	  else 
	    {
	      PlayerMsg = "You feel very guru.";
	      game::GetGod(VALPURUS)->AdjustRelation(100);
	    }

	  game::SendLOSUpdateRequest();
	}
      break;
    case INTELLIGENCE:
      if(IsPlayer())
	{
	  if(Change > 0)
	    PlayerMsg = "Suddenly the inner structure of the Multiverse around you looks quite simple.";
	  else
	    PlayerMsg = "It surely is hard to think today.";

	  UpdateESPLOS();
	}

      break;
    case WISDOM:
      if(IsPlayer())
	{
	  if(Change > 0)
	    PlayerMsg = "You feel your life experience increasing all the time.";
	  else
	    PlayerMsg = "You feel like having done something unwise.";
	}

      break;
    case CHARISMA:
      if(Change > 0)
	{
	  PlayerMsg = "You feel very confident of your social skills.";

	  if(IsPet())
	    if(GetAttribute(CHARISMA) <= 15)
	      NPCMsg = "%s looks less ugly.";
	    else
	      NPCMsg = "%s looks more attractive.";
	}
      else
	{
	  PlayerMsg = "You feel somehow disliked.";

	  if(IsPet())
	    if(GetAttribute(CHARISMA) < 15)
	      NPCMsg = "%s looks more ugly.";
	    else
	      NPCMsg = "%s looks less attractive.";
	}

      break;
    case MANA:
      if(Change > 0)
	{
	  PlayerMsg = "You feel magical forces coursing through your body!";
	  NPCMsg = "You notice an odd glow around %s.";
	}
      else
	{
	  PlayerMsg = "You feel your magical abilities withering slowly.";
	  NPCMsg = "You notice strange vibrations in the air around %s. But they disappear rapidly.";
	}

      break;
    }

  if(IsPlayer())
    ADD_MESSAGE(PlayerMsg);
  else if(NPCMsg && CanBeSeenByPlayer())
    ADD_MESSAGE(NPCMsg, CHAR_NAME(DEFINITE));

  CalculateBattleInfo();
}

int character::RawEditExperience(double& Exp, double NaturalExp, double Value, double Speed) const
{
  double OldExp = Exp;

  if(!OldExp || !Value
  || (Value > 0 && OldExp >= NaturalExp * (100 + Value) / 100)
  || (Value < 0 && OldExp <= NaturalExp * (100 + Value) / 100))
    return 0;

  if(!IsPlayer())
    Speed *= 2;

  Exp += (NaturalExp * (100 + Value) - 100 * OldExp) * Speed * EXP_DIVISOR;
  LimitRef(Exp, MIN_EXP, MAX_EXP);
  int NewA = int(Exp * EXP_DIVISOR);
  int OldA = int(OldExp * EXP_DIVISOR);
  int Delta = NewA - OldA;

  if(Delta > 0)
    Exp = Max(Exp, (NewA + 0.05) * EXP_MULTIPLIER);
  else if(Delta < 0)
    Exp = Min(Exp, (NewA + 0.95) * EXP_MULTIPLIER);

  LimitRef(Exp, MIN_EXP, MAX_EXP);
  return Delta;
}

int character::GetAttribute(int Identifier, bool AllowBonus) const
{
  int A = int(BaseExperience[Identifier] * EXP_DIVISOR);
  return A && AllowBonus ? Max(A + AttributeBonus[Identifier], 1) : A;
}

int characterdatabase::* ExpPtr[ATTRIBUTES] =
{
  &characterdatabase::DefaultEndurance,
  &characterdatabase::DefaultPerception,
  &characterdatabase::DefaultIntelligence,
  &characterdatabase::DefaultWisdom,
  &characterdatabase::DefaultCharisma,
  &characterdatabase::DefaultMana,
  &characterdatabase::DefaultArmStrength,
  &characterdatabase::DefaultLegStrength,
  &characterdatabase::DefaultDexterity,
  &characterdatabase::DefaultAgility
};

void characterdatabase::PostProcess()
{
  double AM = (100 + AttributeBonus) * EXP_MULTIPLIER / 100;

  for(int c = 0; c < ATTRIBUTES; ++c)
    NaturalExperience[c] = this->*ExpPtr[c] * AM;
}

void character::EditDealExperience(long Price)
{
  EditExperience(CHARISMA, sqrt(Price) / 5, 1 << 9);
}

void character::PrintBeginLeprosyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel bad. Very bad.");
}

void character::PrintEndLeprosyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel that you're OK."); // CHANGE OR DIE
}

void character::TryToInfectWithLeprosy(const character* Infector)
{
  if(!IsImmuneToLeprosy()
  && ((GetRelation(Infector) == HOSTILE
    && !RAND_N(25 * GetAttribute(ENDURANCE))) 
   || !RAND_N(250 * GetAttribute(ENDURANCE))))
    GainIntrinsic(LEPROSY);
}

void character::SignalGeneration()
{
  const_cast<database*>(DataBase)->Flags |= HAS_BEEN_GENERATED;
}

void character::CheckIfSeen()
{
  if(IsPlayer() || CanBeSeenByPlayer())
    SignalSeen();
}

void character::SignalSeen()
{
  if(!(WarnFlags & WARNED)
  && GetRelation(PLAYER) == HOSTILE)
    {
      double Danger = GetRelativeDanger(PLAYER);

      if(Danger > 5.)
	{
	  game::SetDangerFound(Max(game::GetDangerFound(), Danger));

	  if(Danger > 500. && !(WarnFlags & HAS_CAUSED_PANIC))
	    {
	      WarnFlags |= HAS_CAUSED_PANIC;
	      game::SetCausePanicFlag(true);
	    }

	  WarnFlags |= WARNED;
	}
    }

  const_cast<database*>(DataBase)->Flags |= HAS_BEEN_SEEN;
}

int character::GetPolymorphIntelligenceRequirement() const
{
  if(DataBase->PolymorphIntelligenceRequirement == DEPENDS_ON_ATTRIBUTES)
    return Max(GetAttributeAverage() - 5, 0);
  else
    return DataBase->PolymorphIntelligenceRequirement;
}

void character::RemoveAllItems()
{
  GetStack()->Clean();

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	{
	  Equipment->RemoveFromSlot();
	  Equipment->SendToHell();
	}
    }
}

int character::CalculateWeaponSkillHits(const character* Enemy) const
{
  if(Enemy->IsPlayer())
    {
      configid ConfigID(GetType(), GetConfig());
      const dangerid& DangerID = game::GetDangerMap().find(ConfigID)->second;
      return Min(int(DangerID.EquippedDanger * 2000), 1000);
    }
  else
    return Min(int(GetRelativeDanger(Enemy, true) * 2000), 1000);
}

/* Target mustn't have any equipment */

void character::DonateEquipmentTo(character* Character)
{
  if(IsPlayer())
    {
      ulong* EquipmentMemory = game::GetEquipmentMemory();

      for(int c = 0; c < MAX_EQUIPMENT_SLOTS; ++c)
	{
	  item* Item = GetEquipment(c);

	  if(Item)
	    {
	      if(Character->CanUseEquipment(c))
		{
		  Item->RemoveFromSlot();
		  Character->SetEquipment(c, Item);
		}
	      else
		{
		  EquipmentMemory[c] = Item->GetID();
		  Item->MoveTo(Character->GetStack());
		}
	    }
	  else if(CanUseEquipment(c))
	    EquipmentMemory[c] = 0;
	  else if(EquipmentMemory[c]
	       && Character->CanUseEquipment(c))
	    {
	      for(stackiterator i = Character->GetStack()->GetBottom();
		  i.HasItem(); ++i)
		if(i->GetID() == EquipmentMemory[c])
		  {
		    item* Item = *i;
		    Item->RemoveFromSlot();
		    Character->SetEquipment(c, Item);
		    break;
		  }

	      EquipmentMemory[c] = 0;
	    }
	}
    }
  else
    {
      for(int c = 0; c < GetEquipmentSlots(); ++c)
	{
	  item* Item = GetEquipment(c);

	  if(Item)
	    {
	      if(Character->CanUseEquipment(c))
		{
		  Item->RemoveFromSlot();
		  Character->SetEquipment(c, Item);
		}
	      else
		Item->MoveTo(Character->GetStackUnder());
	    }
	}
    }
}

void character::ReceivePeaSoup(long)
{
  lsquare* Square = GetLSquareUnder();

  if(Square->IsFlyable())
    Square->AddSmoke(new gas(FART, 250));
}

void character::AddPeaSoupConsumeEndMessage() const
{
  if(IsPlayer())
    {
      if(CanHear())
	ADD_MESSAGE("Mmmh! The soup is very tasty. You hear a small puff.");
      else
	ADD_MESSAGE("Mmmh! The soup is very tasty.");
    }
  else if(CanBeSeenByPlayer() && PLAYER->CanHear()) // change someday
    ADD_MESSAGE("You hear a small puff.");
}

void character::CalculateMaxStamina()
{
  MaxStamina = TorsoIsAlive() ? GetAttribute(ENDURANCE) * 10000 : 0;
}

void character::EditStamina(int Amount, bool CanCauseUnconsciousness)
{
  if(!TorsoIsAlive())
    return;

  int UnconsciousnessStamina = MaxStamina >> 3;

  if(!CanCauseUnconsciousness && Amount < 0)
    {
      if(Stamina > UnconsciousnessStamina)
	{
	  Stamina += Amount;

	  if(Stamina < UnconsciousnessStamina)
	    Stamina = UnconsciousnessStamina;
	}

      return;
    }

  int OldStamina = Stamina;
  Stamina += Amount;

  if(Stamina > MaxStamina)
    Stamina = MaxStamina;
  else if(Stamina < 0)
    {
      Stamina = 0;
      LoseConsciousness(250 + RAND_N(250));
    }
  else if(IsPlayer())
    {
      if(OldStamina >= MaxStamina >> 2 && Stamina < MaxStamina >> 2)
	ADD_MESSAGE("You are getting a little tired.");
      else if(OldStamina >= UnconsciousnessStamina
	   && Stamina < UnconsciousnessStamina)
	{
	  ADD_MESSAGE("You are seriously out of breath!");
	  game::SetPlayerIsRunning(false);
	}
    }

  if(IsPlayer() && StateIsActivated(PANIC)
  && GetTirednessState() != FAINTING)
    game::SetPlayerIsRunning(true);
}

void character::RegenerateStamina()
{
  if(GetTirednessState() != UNTIRED)
    {
      EditExperience(ENDURANCE, 50, 1);

      if(Sweats() && TorsoIsAlive() && !RAND_N(30) && !game::IsInWilderness())
	{
	  long Volume = long(.05 * sqrt(GetBodyVolume()));

	  if(GetTirednessState() == FAINTING)
	    Volume <<= 1;

	  for(int c = 0; c < SquaresUnder; ++c)
	    GetLSquareUnder(c)->SpillFluid(0, CreateSweat(Volume), false, false);
	}
	  
    }

  int Bonus = 1;

  if(Action)
    {
      if(Action->IsRest())
	{
	  if(SquaresUnder == 1)
	    Bonus = GetSquareUnder()->GetRestModifier() << 1;
	  else
	    {
	      int Lowest = GetSquareUnder(0)->GetRestModifier();

	      for(int c = 1; c < GetSquaresUnder(); ++c)
		{
		  int Mod = GetSquareUnder(c)->GetRestModifier();

		  if(Mod < Lowest)
		    Lowest = Mod;
		}

	      Bonus = Lowest << 1;
	    }
	}
      else if(Action->IsUnconsciousness())
	Bonus = 2;
    }

  int Plus1 = 100;

  switch(GetHungerState())
    {
    case OVER_LOADED:
      Plus1 = 25;
      break;
    case STRESSED:
      Plus1 = 50;
      break;
    case BURDENED:
      Plus1 = 75;
      break;
    }

  int Plus2 = 100;

  if(IsPlayer())
    switch(GetHungerState())
      {
      case STARVING:
	Plus2 = 25;
	break;
      case VERY_HUNGRY:
	Plus2 = 50;
	break;
      case HUNGRY:
	Plus2 = 75;
	break;
      }

  Stamina += Plus1 * Plus2 * Bonus / 1000;

  if(Stamina > MaxStamina)
    Stamina = MaxStamina;

  if(IsPlayer() && StateIsActivated(PANIC)
  && GetTirednessState() != FAINTING)
    game::SetPlayerIsRunning(true);
}

void character::BeginPanic()
{
  if(IsPlayer() && GetTirednessState() != FAINTING)
    game::SetPlayerIsRunning(true);

  DeActivateVoluntaryAction();
}

void character::EndPanic()
{
  if(IsPlayer())
    game::SetPlayerIsRunning(false);
}

int character::GetTirednessState() const
{
  if(Stamina >= MaxStamina >> 2)
    return UNTIRED;
  else if(Stamina >= MaxStamina >> 3)
    return EXHAUSTED;
  else
    return FAINTING;
}

void character::ReceiveBlackUnicorn(long Amount)
{
  if(!(RAND() % 160))
    game::DoEvilDeed(Amount / 50);

  BeginTemporaryState(TELEPORT, Amount / 100);

  for(int c = 0; c < STATES; ++c)
    if(StateData[c].Flags & DUR_TEMPORARY)
      {
	BeginTemporaryState(1 << c, Amount / 100);

	if(!IsEnabled())
	  return;
      }
}

void character::ReceiveGrayUnicorn(long Amount)
{
  if(!(RAND() % 80))
    game::DoEvilDeed(Amount / 50);

  BeginTemporaryState(TELEPORT, Amount / 100);

  for(int c = 0; c < STATES; ++c)
    if(1 << c != TELEPORT)
      {
	DecreaseStateCounter(1 << c, -Amount / 100);

	if(!IsEnabled())
	  return;
      }
}

void character::ReceiveWhiteUnicorn(long Amount)
{
  if(!(RAND() % 40))
    game::DoEvilDeed(Amount / 50);

  BeginTemporaryState(TELEPORT, Amount / 100);
  DecreaseStateCounter(LYCANTHROPY, -Amount / 100);
  DecreaseStateCounter(POISONED, -Amount / 100);
  DecreaseStateCounter(PARASITIZED, -Amount / 100);
  DecreaseStateCounter(LEPROSY, -Amount / 100);
}

/* Counter should be negative. Removes intrinsics. */

void character::DecreaseStateCounter(long State, int Counter)
{
  int Index;

  for(Index = 0; Index < STATES; ++Index)
    if(1 << Index == State)
      break;

  if(Index == STATES)
    ABORT("DecreaseTemporaryStateCounter works only when State == 2 ^ n!");

  if(TemporaryState & State)
    {
      if(TemporaryStateCounter[Index] == PERMANENT
      || (TemporaryStateCounter[Index] += Counter) <= 0)
	{
	  TemporaryState &= ~State;

	  if(!(EquipmentState & State))
	    {
	      if(StateData[Index].EndHandler)
		{
		  (this->*StateData[Index].EndHandler)();

		  if(!IsEnabled())
		    return;
		}

	      (this->*StateData[Index].PrintEndMessage)();
	    }
	}
    }
}

bool character::IsImmuneToLeprosy() const
{
  return DataBase->IsImmuneToLeprosy || UseMaterialAttributes();
}

void character::LeprosyHandler()
{
  EditExperience(ARM_STRENGTH, -25, 1 << 1);
  EditExperience(LEG_STRENGTH, -25, 1 << 1);
  EditExperience(DEXTERITY, -25, 1 << 1);
  EditExperience(AGILITY, -25, 1 << 1);  
  EditExperience(ENDURANCE, -25, 1 << 1);
  EditExperience(CHARISMA, -25, 1 << 1);
}

bodypart* character::SearchForOriginalBodyPart(int I) const
{
  for(stackiterator i1 = GetStackUnder()->GetBottom(); i1.HasItem(); ++i1)
    {
      for(std::list<ulong>::iterator i2 = OriginalBodyPartID[I].begin();
	  i2 != OriginalBodyPartID[I].end(); ++i2)
	if(i1->GetID() == *i2)
	  return static_cast<bodypart*>(*i1);
    }

  return 0;
}

void character::SetLifeExpectancy(int Base, int RandPlus)
{
  int c;

  for(c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->SetLifeExpectancy(Base, RandPlus);
    }

  for(c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	Equipment->SetLifeExpectancy(Base, RandPlus);
    }
}

/* Receiver should be a fresh duplicate of this */

void character::DuplicateEquipment(character* Receiver, ulong Flags)
{
  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	{
	  item* Duplicate = Equipment->Duplicate(Flags);
	  Receiver->SetEquipment(c, Duplicate);
	}
    }
}

void character::Disappear(corpse* Corpse, const char* Verb, bool (item::*ClosePredicate)() const)
{
  bool TorsoDisappeared = false;
  bool CanBeSeen = Corpse ? Corpse->CanBeSeenByPlayer() : IsPlayer() || CanBeSeenByPlayer();
  int c;

  if((GetTorso()->*ClosePredicate)())
    {
      if(CanBeSeen)
	if(Corpse)
	  ADD_MESSAGE("%s %ss.", Corpse->CHAR_NAME(DEFINITE), Verb);
	else if(IsPlayer())
	  ADD_MESSAGE("You %s.", Verb);
	else
	  ADD_MESSAGE("%s %ss.", CHAR_NAME(DEFINITE), Verb);

      TorsoDisappeared = true;

      for(c = 0; c < GetEquipmentSlots(); ++c)
	{
	  item* Equipment = GetEquipment(c);

	  if(Equipment && (Equipment->*ClosePredicate)())
	    {
	      Equipment->RemoveFromSlot();
	      Equipment->SendToHell();
	    }
	}

      itemvector ItemVector;
      GetStack()->FillItemVector(ItemVector);

      for(uint c = 0; c < ItemVector.size(); ++c)
	if(ItemVector[c] && (ItemVector[c]->*ClosePredicate)())
	  {
	    ItemVector[c]->RemoveFromSlot();
	    ItemVector[c]->SendToHell();
	  }
    }

  for(c = 1; c < GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	if((BodyPart->*ClosePredicate)())
	  {
	    if(!TorsoDisappeared && CanBeSeen)
	      if(IsPlayer())
		ADD_MESSAGE("Your %s %ss.", GetBodyPartName(c).CStr(), Verb);
	      else
		ADD_MESSAGE("The %s of %s %ss.", GetBodyPartName(c).CStr(), CHAR_NAME(DEFINITE), Verb);

	    BodyPart->DropEquipment();
	    item* BodyPart = SevereBodyPart(c);

	    if(BodyPart)
	      BodyPart->SendToHell();
	  }
	else if(TorsoDisappeared)
	  {
	    BodyPart->DropEquipment();
	    item* BodyPart = SevereBodyPart(c);

	    if(BodyPart)
	      if(Corpse)
		Corpse->GetSlot()->AddFriendItem(BodyPart);
	      else if(!game::IsInWilderness())
		GetStackUnder()->AddItem(BodyPart);
	      else
		BodyPart->SendToHell();
	  }
    }

  if(TorsoDisappeared)
    {
      if(Corpse)
	{
	  Corpse->RemoveFromSlot();
	  Corpse->SendToHell();
	}
      else
	CheckDeath(festring(Verb) + "ed", 0, false, true, false, false);
    }
  else
    CheckDeath(festring(Verb) + "ed", 0, false, false, true, false);
}

void character::SignalDisappearance()
{
  if(GetMotherEntity())
    GetMotherEntity()->SignalDisappearance();
  else
    Disappear(0, "disappear", &item::IsVeryCloseToDisappearance);
}

bool character::HornOfFearWorks() const
{
  return CanHear() && GetPanicLevel() > RAND() % 33;
}

void character::BeginLeprosy()
{
  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->GetMainMaterial()->SetIsInfectedByLeprosy(true);
    }
}

void character::EndLeprosy()
{
  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->GetMainMaterial()->SetIsInfectedByLeprosy(false);
    }
}

bool character::IsSameAs(const character* What) const
{
  return What->GetType() == GetType()
      && What->GetConfig() == GetConfig();
}

ulong character::GetCommandFlags() const
{
  return !StateIsActivated(PANIC)
       ? CommandFlags
       : CommandFlags|FLEE_FROM_ENEMIES;
}

ulong character::GetConstantCommandFlags() const
{
  return !StateIsActivated(PANIC)
       ? DataBase->ConstantCommandFlags
       : DataBase->ConstantCommandFlags|FLEE_FROM_ENEMIES;
}

ulong character::GetPossibleCommandFlags() const
{
  int Int = GetAttribute(INTELLIGENCE);
  ulong Flags = ALL_COMMAND_FLAGS;

  if(!CanMove() || Int < 4)
    Flags &= ~FOLLOW_LEADER;

  if(!CanMove() || Int < 6)
    Flags &= ~FLEE_FROM_ENEMIES;

  if(!CanUseEquipment() || Int < 8)
    Flags &= ~DONT_CHANGE_EQUIPMENT;

  if(!UsesNutrition() || Int < 8)
    Flags &= ~DONT_CONSUME_ANYTHING_VALUABLE;

  return Flags;
}

bool character::IsRetreating() const
{
  return StateIsActivated(PANIC)
      || (CommandFlags & FLEE_FROM_ENEMIES && IsPet());
}

bool character::ChatMenu()
{
  if(GetAction() && !GetAction()->CanBeTalkedTo())
    {
      ADD_MESSAGE("%s is silent.", CHAR_DESCRIPTION(DEFINITE));
      PLAYER->EditAP(-200);
      return true;
    }

  ulong ManagementFlags = GetManagementFlags();

  if(ManagementFlags == CHAT_IDLY || !IsPet())
    return ChatIdly();

  static const char*const ChatMenuEntry[CHAT_MENU_ENTRIES] =
  {
    "Change equipment",
    "Take items",
    "Give items",
    "Issue commands",
    "Chat idly",
  };

  static const petmanagementfunction PMF[CHAT_MENU_ENTRIES] =
  {
    &character::ChangePetEquipment,
    &character::TakePetItems,
    &character::GivePetItems,
    &character::IssuePetCommands,
    &character::ChatIdly
  };

  felist List(CONST_S("Choose action:"));
  game::SetStandardListAttributes(List);
  List.AddFlags(SELECTABLE);
  int c, i;

  for(c = 0; c < CHAT_MENU_ENTRIES; ++c)
    if(1 << c & ManagementFlags)
      List.AddEntry(ChatMenuEntry[c], LIGHT_GRAY);

  int Chosen = List.Draw();

  if(Chosen & FELIST_ERROR_BIT)
    return false;

  for(c = 0, i = 0; c < CHAT_MENU_ENTRIES; ++c)
    if(1 << c & ManagementFlags && i++ == Chosen)
      return (this->*PMF[c])();

  return false; // dummy
}

bool character::ChangePetEquipment()
{
  if(EquipmentScreen(PLAYER->GetStack(), GetStack()))
    {
      DexterityAction(3);
      return true;
    }

  return false;
}

bool character::TakePetItems()
{
  bool Success = false;
  stack::SetSelected(0);

  for(;;)
    {
      itemvector ToTake;
      game::DrawEverythingNoBlit();
      GetStack()->DrawContents(ToTake,
			       0,
			       PLAYER,
			       CONST_S("What do you want to take from ") + CHAR_DESCRIPTION(DEFINITE) + '?',
			       CONST_S(""),
			       CONST_S(""),
			       GetDescription(DEFINITE) + " is " + GetVerbalBurdenState(),
			       GetVerbalBurdenStateColor(),
			       REMEMBER_SELECTED);

      if(ToTake.empty())
	break;

      for(uint c = 0; c < ToTake.size(); ++c)
	ToTake[c]->MoveTo(PLAYER->GetStack());

      ADD_MESSAGE("You take %s.", ToTake[0]->GetName(DEFINITE, ToTake.size()).CStr());
      Success = true;
    }

  if(Success)
    {
      DexterityAction(2);
      PLAYER->DexterityAction(2);
    }

  return Success;
}

bool character::GivePetItems()
{
  bool Success = false;
  stack::SetSelected(0);

  for(;;)
    {
      itemvector ToGive;
      game::DrawEverythingNoBlit();
      PLAYER->GetStack()->DrawContents(ToGive,
				       0,
				       this,
				       CONST_S("What do you want to give to ") + CHAR_DESCRIPTION(DEFINITE) + '?',
				       CONST_S(""),
				       CONST_S(""),
				       GetDescription(DEFINITE) + " is " + GetVerbalBurdenState(),
				       GetVerbalBurdenStateColor(),
				       REMEMBER_SELECTED);

      if(ToGive.empty())
	break;

      for(uint c = 0; c < ToGive.size(); ++c)
	ToGive[c]->MoveTo(GetStack());

      ADD_MESSAGE("You give %s to %s.", ToGive[0]->GetName(DEFINITE, ToGive.size()).CStr(), CHAR_DESCRIPTION(DEFINITE));
      Success = true;
    }

  if(Success)
    {
      DexterityAction(2);
      PLAYER->DexterityAction(2);
    }

  return Success;
}

bool character::IssuePetCommands()
{
  ulong PossibleC = GetPossibleCommandFlags();

  if(!PossibleC)
    {
      ADD_MESSAGE("%s cannot be commanded.", CHAR_DESCRIPTION(DEFINITE));
      return false;
    }

  ulong OldC = GetCommandFlags();
  ulong NewC = OldC, VaryFlags = 0;
  game::CommandScreen(CONST_S("Issue commands to ") + GetDescription(DEFINITE), PossibleC, GetConstantCommandFlags(), VaryFlags, NewC);

  if(NewC == OldC)
    return false;

  SetCommandFlags(NewC);
  PLAYER->EditAP(-500);
  PLAYER->EditExperience(CHARISMA, 25, 1 << 7);
  return true;
}

bool character::ChatIdly()
{
  if(!TryToTalkAboutScience())
    {
      BeTalkedTo();
      PLAYER->EditExperience(CHARISMA, 75, 1 << 7);
    }

  PLAYER->EditAP(-1000);
  return true;
}

bool character::EquipmentScreen(stack* MainStack, stack* SecStack)
{
  if(!CanUseEquipment())
    {
      ADD_MESSAGE("%s cannot use equipment.", CHAR_DESCRIPTION(DEFINITE));
      return false;
    }

  int Chosen = 0;
  bool EquipmentChanged = false;
  felist List(CONST_S("Equipment menu [ESC exits]"));
  festring Entry;

  for(;;)
    {
      List.Empty();
      List.EmptyDescription();

      if(!IsPlayer())
	{
	  List.AddDescription(CONST_S(""));
	  List.AddDescription(festring(GetDescription(DEFINITE) + " is " + GetVerbalBurdenState()).CapitalizeCopy(), GetVerbalBurdenStateColor());
	}

      for(int c = 0; c < GetEquipmentSlots(); ++c)
	{
	  Entry = GetEquipmentName(c);
	  Entry << ':';
	  Entry.Resize(20);
	  item* Equipment = GetEquipment(c);

	  if(Equipment)
	    {
	      Equipment->AddInventoryEntry(this, Entry, 1, true);
	      AddSpecialEquipmentInfo(Entry, c);
	      int ImageKey = game::AddToItemDrawVector(itemvector(1, Equipment));
	      List.AddEntry(Entry, LIGHT_GRAY, 20, ImageKey, true);
	    }
	  else
	    {
	      Entry << (GetBodyPartOfEquipment(c) ? "-" : "can't use");
	      List.AddEntry(Entry, LIGHT_GRAY, 20, game::AddToItemDrawVector(itemvector()));
	    }
	}

      game::DrawEverythingNoBlit();
      game::SetStandardListAttributes(List);
      List.SetFlags(SELECTABLE|DRAW_BACKGROUND_AFTERWARDS);
      List.SetEntryDrawer(game::ItemEntryDrawer);
      Chosen = List.Draw();
      game::ClearItemDrawVector();

      if(Chosen >= GetEquipmentSlots())
	break;

      EquipmentChanged = TryToChangeEquipment(MainStack, SecStack, Chosen);
    }

  if(EquipmentChanged)
    DexterityAction(5);

  return EquipmentChanged;
}

ulong character::GetManagementFlags() const
{
  ulong Flags = ALL_MANAGEMENT_FLAGS;

  if(!CanUseEquipment())
    Flags &= ~CHANGE_EQUIPMENT;

  if(!GetStack()->GetItems())
    Flags &= ~TAKE_ITEMS;

  if(!WillCarryItems())
    Flags &= ~GIVE_ITEMS;

  if(!GetPossibleCommandFlags())
    Flags &= ~ISSUE_COMMANDS;

  return Flags;
}

const char* VerbalBurdenState[] = { "overloaded", "stressed", "burdened", "unburdened" };
color16 VerbalBurdenStateColor[] = { RED, BLUE, BLUE, WHITE };

const char* character::GetVerbalBurdenState() const
{
  return VerbalBurdenState[BurdenState];
}

color16 character::GetVerbalBurdenStateColor() const
{
  return VerbalBurdenStateColor[BurdenState];
}

int character::GetAttributeAverage() const
{
  return GetSumOfAttributes() / 7;
}

const festring& character::GetStandVerb() const
{
  static festring Hovering = "hovering";
  static festring Swimming = "swimming";

  if(StateIsActivated(LEVITATION))
    return Hovering;

  if(GetSquareUnder()->GetSquareWalkability() & SWIM)
    return Swimming;

  return DataBase->StandVerb;
}

bool character::CheckApply() const
{
  if(!CanApply())
    {
      ADD_MESSAGE("This monster type cannot apply.");
      return false;
    }

  return true;
}

void character::EndLevitation()
{
  if(!(GetMoveType() & FLY))
    {
      if(!game::IsInWilderness() && !GetLSquareUnder()->IsFreezed())
	SignalStepFrom(0);

      TestWalkability();
    }
}

bool character::CanMove() const
{
  return !IsRooted() || StateIsActivated(LEVITATION);
}

void character::CalculateEnchantments()
{
  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	Equipment->CalculateEnchantment();
    }

  GetStack()->CalculateEnchantments();
}

bool character::GetNewFormForPolymorphWithControl(character*& NewForm)
{
  festring Topic, Temp;
  NewForm = 0;

  while(!NewForm)
    {
      festring Temp = game::DefaultQuestion(CONST_S("What do you want to become? [press '?' for a list]"),
					    game::GetDefaultPolymorphTo(),
					    &game::PolymorphControlKeyHandler);
      NewForm = protosystem::CreateMonster(Temp);

      if(NewForm)
	{
	  if(NewForm->IsSameAs(this))
	    {
	      delete NewForm;
	      ADD_MESSAGE("You choose not to polymorph.");
	      NewForm = this;
	      return false;
	    }

	  if(PolymorphBackup && NewForm->IsSameAs(PolymorphBackup))
	    {
	      delete NewForm;
	      NewForm = ForceEndPolymorph();
	      return false;
	    }

	  if(NewForm->GetPolymorphIntelligenceRequirement()
	     > GetAttribute(INTELLIGENCE)
	     && !game::WizardModeIsActive())
	    {
	      ADD_MESSAGE("You feel your mind isn't yet powerful enough to call forth the form of %s.", NewForm->CHAR_NAME(INDEFINITE));
	      delete NewForm;
	      NewForm = 0;
	    }
	  else
	    NewForm->RemoveAllItems();
	}
    }

  return true;
}

liquid* character::CreateSweat(long Volume) const
{
  return new liquid(GetSweatMaterial(), Volume);
}

bool character::TeleportRandomItem(bool TryToHinderVisibility)
{
  if(IsImmuneToItemTeleport())
    return false;

  itemvector ItemVector;
  std::vector<long> PossibilityVector;
  int TotalPossibility = 0;

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    {
      ItemVector.push_back(*i);
      int Possibility = i->GetTeleportPriority();

      if(TryToHinderVisibility)
	Possibility += i->GetHinderVisibilityBonus(this);

      PossibilityVector.push_back(Possibility);
      TotalPossibility += Possibility;
    }

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	{
	  ItemVector.push_back(Equipment);
	  int Possibility = Equipment->GetTeleportPriority();

	  if(TryToHinderVisibility)
	    Possibility += Equipment->GetHinderVisibilityBonus(this);

	  PossibilityVector.push_back(Possibility <<= 1);
	  TotalPossibility += Possibility;
	}
    }

  if(!TotalPossibility)
    return false;

  int Chosen = femath::WeightedRand(PossibilityVector, TotalPossibility);
  item* Item = ItemVector[Chosen];
  bool Equipped = PLAYER->Equips(Item);
  bool Seen = Item->CanBeSeenByPlayer();
  Item->RemoveFromSlot();

  if(Seen)
    ADD_MESSAGE("%s disappears.", Item->CHAR_NAME(DEFINITE));

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment lost! [press any key to continue]"));

  vector2d Pos = GetPos();
  int Range = Item->GetEmitation() && TryToHinderVisibility ? 25 : 5;
  rect Border(Pos + vector2d(-Range, -Range), Pos + vector2d(Range, Range));
  Pos = GetLevel()->GetRandomSquare(this, 0, &Border);

  if(Pos == ERROR_VECTOR)
    Pos = GetLevel()->GetRandomSquare();

  GetNearLSquare(Pos)->GetStack()->AddItem(Item);

  if(Item->CanBeSeenByPlayer())
    ADD_MESSAGE("%s appears.", Item->CHAR_NAME(INDEFINITE));

  return true;
}

bool character::HasClearRouteTo(vector2d Pos) const
{
  pathcontroller::Map = GetLevel()->GetMap();
  pathcontroller::Character = this;
  vector2d ThisPos = GetPos();
  return mapmath<pathcontroller>::DoLine(ThisPos.X, ThisPos.Y, Pos.X, Pos.Y, SKIP_FIRST);
}

bool character::IsTransparent() const
{
  return !IsEnormous() || GetTorso()->GetMainMaterial()->IsTransparent();
}

void character::SignalPossibleTransparencyChange()
{
  if(!game::IsInWilderness())
    for(int c = 0; c < SquaresUnder; ++c)
      {
	lsquare* Square = GetLSquareUnder(c);

	if(Square)
	  Square->SignalPossibleTransparencyChange();
      }
}

int character::GetCursorData() const
{
  int Bad = 0;
  int Color = game::PlayerIsRunning() ? BLUE_CURSOR : DARK_CURSOR;

  for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && BodyPart->IsUsable())
	{
	  int ConditionColorIndex = BodyPart->GetConditionColorIndex();

	  if((BodyPartIsVital(c) && !ConditionColorIndex)
	  || (ConditionColorIndex <= 1 && ++Bad == 2))
	    return Color|FLASH;
	}
      else if(++Bad == 2)
	return Color|FLASH;
    }

  Color = game::PlayerIsRunning() ? YELLOW_CURSOR : RED_CURSOR;
  return Bad ? Color|FLASH : Color;
}

void character::TryToName()
{
  if(!IsPet())
    ADD_MESSAGE("%s refuses to let YOU decide what %s's called.", CHAR_NAME(DEFINITE), CHAR_PERSONAL_PRONOUN);
  else if(IsPlayer())
    ADD_MESSAGE("You can't rename yourself.");
  else if(!IsNameable())
    ADD_MESSAGE("%s refuses to be called anything else but %s.", CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
  else
    {
      festring Topic = CONST_S("What name will you give to ") + GetName(DEFINITE) + '?';
      festring Name = game::StringQuestion(Topic, WHITE, 0, 80, true);

      if(Name.GetSize())
	SetAssignedName(Name);
    }
}

double character::GetSituationDanger(const character* Enemy, vector2d ThisPos, vector2d EnemyPos, bool SeesEnemy) const
{
  double Danger;

  if(IgnoreDanger() && !IsPlayer())
    {
      if(Enemy->IgnoreDanger() && !Enemy->IsPlayer())
	Danger = double(GetHP()) * GetHPRequirementForGeneration() / (Enemy->GetHP() * Enemy->GetHPRequirementForGeneration());
      else
	Danger = .25 * GetHPRequirementForGeneration() / Enemy->GetHP();
    }
  else if(Enemy->IgnoreDanger() && !Enemy->IsPlayer())
    Danger = 4. * GetHP() / Enemy->GetHPRequirementForGeneration();
  else
    Danger = GetRelativeDanger(Enemy);

  Danger *= 3. / ((EnemyPos - ThisPos).GetManhattanLength() + 2);

  if(!SeesEnemy)
    Danger *= .2;

  if(StateIsActivated(PANIC))
    Danger *= .2;

  Danger *= double(GetHP()) * Enemy->GetMaxHP() / (Enemy->GetHP() * GetMaxHP());
  return Danger;
}

void character::ModifySituationDanger(double& Danger) const
{
  switch(GetTirednessState())
    {
    case FAINTING:
      Danger *= 1.5;
    case EXHAUSTED:
      Danger *= 1.25;
    }

  for(int c = 0; c < STATES; ++c)
    if(StateIsActivated(1 << c) && StateData[c].SituationDangerModifier != 0)
      (this->*StateData[c].SituationDangerModifier)(Danger);
}

void character::LycanthropySituationDangerModifier(double& Danger) const
{
  character* Wolf = new werewolfwolf;
  double DangerToWolf = GetRelativeDanger(Wolf);
  Danger *= pow(DangerToWolf, 0.1);
  delete Wolf;
}

void character::PoisonedSituationDangerModifier(double& Danger) const
{
  int C = GetTemporaryStateCounter(POISONED);
  Danger *= (1 + (C * C) / (GetHP() * 10000. * (GetGlobalResistance(POISON) + 1)));
}

void character::PolymorphingSituationDangerModifier(double& Danger) const
{
  if(!StateIsActivated(POLYMORPH_CONTROL))
    Danger *= 1.5;
}

void character::PanicSituationDangerModifier(double& Danger) const
{
  Danger *= 1.5;
}

void character::ConfusedSituationDangerModifier(double& Danger) const
{
  Danger *= 1.5;
}

void character::ParasitizedSituationDangerModifier(double& Danger) const
{
  Danger *= 1.25;
}

void character::LeprosySituationDangerModifier(double& Danger) const
{
  Danger *= 1.5;
}

void character::AddRandomScienceName(festring& String) const
{
  const festring& Attribute = GetScienceTalkAttribute().GetRandomElement();
  festring Science = GetScienceTalkName().GetRandomElement().CStr();
  festring Prefix;

  if(islower(Science[0]) && Science.Find(' ') >= 0)
    Prefix = GetScienceTalkPrefix().GetRandomElement();

  int L = Prefix.GetSize();

  if(L && Prefix[L - 1] == Science[0])
    Science.Erase(0, 1);

  if(!Attribute.IsEmpty() && !RAND_N(3))
    {
      const festring& OtherAttribute = GetScienceTalkAttribute().GetRandomElement();

      if(!OtherAttribute.IsEmpty() && OtherAttribute.Find("the ", 0, 4)
      && OtherAttribute != Attribute)
	String << Attribute << ' ' << OtherAttribute << ' ';
    }
  else
    {
      String << Attribute;

      if(!Attribute.IsEmpty())
	String << ' ';
    }

  String << Prefix << Science;
}

bool character::TryToTalkAboutScience()
{
  if(GetRelation(PLAYER) == HOSTILE
  || GetScienceTalkPossibility() <= RAND_N(100)
  || PLAYER->GetAttribute(INTELLIGENCE) < GetScienceTalkIntelligenceRequirement()
  || PLAYER->GetAttribute(WISDOM) < GetScienceTalkWisdomRequirement())
    return false;

  festring Science;

  if(RAND_N(3))
    AddRandomScienceName(Science);
  else
    {
      switch(RAND_N(3))
	{
	case 0: Science = "the relation of "; break;
	case 1: Science = "the differences of "; break;
	case 2: Science = "the similarities of "; break;
	}

      AddRandomScienceName(Science);
      Science << " and ";
      AddRandomScienceName(Science);
    }

  ADD_MESSAGE("You have a rather pleasant chat about %s with %s.", Science.CStr(), CHAR_NAME(DEFINITE));
  PLAYER->EditExperience(INTELLIGENCE, 1000, 50. * GetScienceTalkIntelligenceModifier() / ++ScienceTalks);
  PLAYER->EditExperience(WISDOM, 1000, 50. * GetScienceTalkWisdomModifier() / ++ScienceTalks);
  return true;
}

bool character::IsUsingWeaponOfCategory(int Category) const
{
  return ((GetMainWielded() && GetMainWielded()->GetWeaponCategory() == Category)
       || (GetSecondaryWielded() && GetSecondaryWielded()->GetWeaponCategory() == Category));
}

bool character::TryToUnStickTraps(vector2d Dir)
{
  if(!TrapData)
    return true;

  std::vector<trapdata> TrapVector;

  for(const trapdata* T = TrapData; T; T = T->Next)
    TrapVector.push_back(*TrapData);

  for(uint c = 0; c < TrapVector.size(); ++c)
    if(IsEnabled())
      {
	entity* Trap = game::SearchTrap(TrapVector[c].TrapID);

	if(!Trap->Exists())
	  int esko = 2;

	if(Trap->GetVictimID() == GetID() && Trap->TryToUnStick(this, Dir))
	  break;
      }

  return !TrapData;
}

struct trapidcomparer
{
  trapidcomparer(ulong ID) : ID(ID) { }
  bool operator()(const trapdata* T) const { return T->TrapID == ID; }
  ulong ID;
};

void character::RemoveTrap(ulong ID)
{
  trapdata*& T = ListFind(TrapData, trapidcomparer(ID));
  T = T->Next;
  DoForBodyParts(this, &bodypart::SignalPossibleUsabilityChange);
}

void character::AddTrap(ulong ID, ulong BodyParts)
{
  trapdata*& T = ListFind(TrapData, trapidcomparer(ID));

  if(T)
    T->BodyParts |= BodyParts;
  else
    T = new trapdata(ID, GetID(), BodyParts);

  DoForBodyParts(this, &bodypart::SignalPossibleUsabilityChange);
}

bool character::IsStuckToTrap(ulong ID) const
{
  for(const trapdata* T = TrapData; T; T = T->Next)
    if(T->TrapID == ID)
      return true;

  return false;
}

void character::RemoveTraps()
{
  for(trapdata* T = TrapData; T;)
    {
      entity* Trap = game::SearchTrap(T->TrapID);

      if(Trap)
	Trap->UnStick();

      trapdata* ToDel = T;
      T = T->Next;
      delete ToDel;
    }

  TrapData = 0;
  DoForBodyParts(this, &bodypart::SignalPossibleUsabilityChange);
}

void character::RemoveTraps(int BodyPartIndex)
{
  for(trapdata* T = TrapData; T;)
    if(T->BodyParts & (1 << BodyPartIndex))
      {
	entity* Trap = game::SearchTrap(T->TrapID);

	if(Trap)
	  Trap->UnStick();

	trapdata* ToDel = T;

	if(T == TrapData)
	  TrapData = T->Next;

	T = T->Next;
	delete ToDel;
      }
    else
      T = T->Next;

  if(GetBodyPart(BodyPartIndex))
    GetBodyPart(BodyPartIndex)->SignalPossibleUsabilityChange();
}

festring character::GetTrapDescription() const
{
  festring Desc;
  std::pair<entity*, int> TrapStack[3];
  int Index = 0;

  for(const trapdata* T = TrapData; T; T = T->Next)
    {
      if(Index < 3)
	{
	  entity* Trap = game::SearchTrap(T->TrapID);

	  if(Trap)
	    {
	      int c;

	      for(c = 0; c < Index; ++c)
		if(TrapStack[c].first->GetTrapType() == Trap->GetTrapType())
		  ++TrapStack[c].second;

	      if(c == Index)
		TrapStack[Index++] = std::pair<entity*, int>(Trap, 1);
	    }
	}
      else
	{
	  ++Index;
	  break;
	}
    }

  if(Index <= 3)
    {
      TrapStack[0].first->AddTrapName(Desc, TrapStack[0].second);

      if(Index == 2)
	{
	  Desc << " and ";
	  TrapStack[1].first->AddTrapName(Desc, TrapStack[1].second);
	}
      else if(Index == 3)
	{
	  Desc << ", ";
	  TrapStack[1].first->AddTrapName(Desc, TrapStack[1].second);
	  Desc << " and ";
	  TrapStack[2].first->AddTrapName(Desc, TrapStack[2].second);
	}
    }
  else
    Desc << "lots of traps";

  return Desc;
}

int character::RandomizeHurtBodyPart(ulong BodyParts) const
{
  int BodyPartIndex[MAX_BODYPARTS];
  int Index = 0;

  for(int c = 0; c < GetBodyParts(); ++c)
    if(1 << c & BodyParts)
      BodyPartIndex[Index++] = c;

  if(!Index)
    int esko = 2;

  return BodyPartIndex[RAND_N(Index)];
}

bool character::BodyPartIsStuck(int I) const
{
  for(const trapdata* T = TrapData; T; T = T->Next)
    if(1 << I & T->BodyParts)
      return true;

  return false;
}

void character::PrintAttribute(const char* Desc, int I, int PanelPosX, int PanelPosY) const
{
  int Attribute = GetAttribute(I);
  int NoBonusAttribute = GetAttribute(I, false);
  color16 C = game::GetAttributeColor(I);
  festring Description = Desc;
  Description.Resize(5);
  festring ToPrint = Description + Attribute;
  ToPrint.Resize(8);
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY * 10, C, ToPrint.CStr());

  if(Attribute != NoBonusAttribute)
    {
      int Where = PanelPosX + (ToPrint.GetSize() << 3);
      //      FONT->Printf(DOUBLE_BUFFER, Where , PanelPosY * 10, WHITE, 
      //		   "/");      
      FONT->Printf(DOUBLE_BUFFER, Where + 8, PanelPosY * 10, LIGHT_GRAY, 
		   "%d", NoBonusAttribute); 
    }

  //  const char* Format = AWB == ANB ? "%s %d" : "%s %d/%d";
  //  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY * 10, C, Format, Desc, AWB, ANB);
}

bool character::AllowUnconsciousness() const
{
  return DataBase->AllowUnconsciousness && TorsoIsAlive();
}

/*bool character::IsTooHurtToRegainConsciousness() const
{
  return AllowUnconsciousness() && GetTorso()->IsBadlyHurt();
}*/

bool character::CanPanic() const
{
  return !Action || !Action->IsUnconsciousness();
}

int character::GetRandomBodyPart(ulong Possible) const
{
  int OKBodyPart[MAX_BODYPARTS];
  int OKBodyParts = 0;

  for(int c = 0; c < BodyParts; ++c)
    if(1 << c & Possible && GetBodyPart(c))
      OKBodyPart[OKBodyParts++] = c;

  return OKBodyParts ? OKBodyPart[RAND_N(OKBodyParts)] : NONE_INDEX;
}
