#ifndef __CHARBA_H__
#define __CHARBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define CHARDESCRIPTION(Case) Description(Case).c_str()

#define HAS_HIT 0
#define HAS_BLOCKED 1
#define HAS_DODGED 2
#define HAS_DIED 3

#define NUMBER_OF_HUMAN_LEGS  10
#define NUMBER_OF_HUMAN_TORSOS  14
#define NUMBER_OF_HUMAN_ARMS  14
#define NUMBER_OF_HUMAN_HEADS  23

#define BLOATEDLEVEL 40000
#define SATIATEDLEVEL 30000
#define NOTHUNGERLEVEL 5000
#define HUNGERLEVEL 1000

#define OVERLOADED    0
#define STRESSED    1
#define BURDENED    2
#define UNBURDENED    3

#define VERYHUNGRY 0
#define HUNGRY 1
#define NOTHUNGRY 2
#define SATIATED 3
#define BLOATED 4

#define STATES      8

#define FAINTED      0
#define CONSUMING    1
#define POLYMORPHED    2
#define RESTING      3
#define DIGGING      4
#define GOING      5
#define HASTE 6
#define SLOW 7

#define HEAD 0
#define TORSO 1
#define RIGHTARM 2
#define LEFTARM 4
#define ARMS RIGHTARM|LEFTARM
#define GROIN 8
#define RIGHTLEG 16
#define LEFTLEG 32
#define LEGS RIGHTLEG|LEFTLEG
#define OTHER 64
#define ALL 255

#define PHYSICALDAMAGE 1
#define SOUND 2
#define ACID 3
#define ENERGY 4
#define FIRE 5
#define POISON 6
#define BULIMIA 7
#define OMLEURINE

#include <list>

#include "game.h"
#include "object.h"
#include "typedef.h"
#include "vector2d.h"
#include "igraph.h"
#include "id.h"

class square;
class bitmap;
class character;
class item;
class stack;
class material;
class lsquare;
class wsquare;
class outputfile;
class inputfile;
class team;
class torso;
class humanoidtorso;
class bodypart;
class characterslot;

/* Presentation of the character class */

class character : public type, public entity, public id
{
 public:
  character(bool, bool, bool, bool = true, bool = true);
  virtual ~character();
  virtual character* Clone(bool = true, bool = true, bool = true) const = 0;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool CanRead() const { return true; }
  virtual bool CanWear() const { return false; }
  //virtual bool CanWield() const { return false; }
  virtual bool Charmable() const { return true; }
  virtual bool Catches(item*, float) { return false; }
  virtual bool CheckBulimia() const;
  virtual bool CheckDeath(std::string, bool = false);
  //virtual bool CheckIfConsumable(item*) const;
  virtual bool ConsumeItem(item*, stack*);
  virtual bool ConsumeItemType(uchar) const;
  virtual bool DodgesFlyingItem(item*, float);
  virtual bool Hit(character*);
  virtual bool OpenItem();
  virtual bool OpenPos(vector2d);
  virtual bool ReadItem(item*);
  virtual bool TestForPickup(item*) const;
  virtual bool ThrowItem(uchar, item*);
  virtual bool TryMove(vector2d, bool = true);
  virtual bool HasHeadOfElpuri() const;
  virtual bool HasGoldenEagleShirt() const;
  virtual bool HasPetrussNut() const;
  virtual bool GetIsPlayer() const { return IsPlayer; }
  virtual bool Apply();
  virtual bool Close();
  virtual bool Consume();
  virtual bool DecreaseContrast();
  virtual bool Dip();
  virtual bool DrawMessageHistory();
  virtual bool Drop();
  virtual bool Engrave(std::string);
  virtual bool ForceVomit();
  virtual bool GainAllItems();
  virtual bool GoDown();
  virtual bool GoUp();
  virtual bool IncreaseContrast();
  virtual bool Kick();
  virtual bool Look();
  virtual bool LowerStats();
  virtual bool NOP();
  virtual bool Offer();
  virtual bool Open();
  virtual bool PickUp();
  virtual bool Pray();
  virtual bool Quit();
  virtual bool RaiseStats();
  virtual bool Read();
  virtual bool Save();
  virtual bool SeeWholeMap();
  virtual bool ShowInventory();
  virtual bool ShowKeyLayout();
  virtual bool ShowWeaponSkills();
  virtual bool Talk();
  virtual bool Throw();
  virtual bool WalkThroughWalls();
  virtual bool EqupmentScreen();
  virtual bool WhatToEngrave();
  virtual bool WizardMode();
  virtual void AddScoreEntry(std::string, float = 1, bool = true) const;
  virtual long Score() const;
  virtual float GetAttackStrength() const;
  virtual wsquare* GetWSquareUnder() const;
  virtual long GetAgilityExperience() const { return AgilityExperience; }
  virtual long GetAP() const { return AP; }
  virtual long GetEnduranceExperience() const { return EnduranceExperience; }
  virtual long GetNP() const { return NP; }
  virtual long GetPerceptionExperience() const { return PerceptionExperience; }
  virtual long GetStrengthExperience() const { return StrengthExperience; }
  virtual short GetHP() const;
  virtual stack* GetStack() const { return Stack; }
  virtual uchar GetBurdenState(ulong = 0) const;
  virtual uchar GetSex() const { return UNDEFINED; }
  virtual uchar TakeHit(character*, item*, float, float, short, bool);
  virtual ulong CurrentDanger();
  virtual ulong MaxDanger();
  virtual ulong GetBloodColor() const;
  virtual ushort GetRegenerationCounter() const { return RegenerationCounter; }
  virtual ushort GetAgility() const { return Agility; }
  virtual item* GetConsumingCurrently() const { return StateVariables.Consuming.ConsumingCurrently; }
  virtual ushort GetEmitation() const;
  virtual ushort GetEndurance() const { return Endurance; }
  virtual ushort GetPerception() const { return Perception; }
  virtual ushort GetStrength() const { return Strength; }
  virtual ushort LOSRange() const;
  virtual ushort LOSRangeSquare() const;
  virtual bool CanBeGenerated() const { return true; }
  virtual vector2d GetPos() const;
  //virtual std::string GetFailToHurtMessage(character*) const;
  virtual void AddMissMessage(character*) const;
  virtual void AddHitMessage(character*, item*, uchar, bool = false) const;
  virtual void ApplyExperience();
  virtual void BeTalkedTo(character*);
  virtual void Darkness(long);
  virtual void Die(bool = false);
  virtual void DrawToTileBuffer() const;
  virtual void HasBeenHitByItem(item*, float);
  virtual void HealFully(character*) { }
  virtual void Hunger(ushort = 1);
  virtual void Move(vector2d, bool = false);
  virtual bool MoveRandomly();
  virtual void ReceiveBulimiaDamage();
  virtual void ReceiveFireDamage(character*, std::string, long);
  virtual void ReceiveNutrition(long);
  virtual void ReceiveOmleUrineEffect(long);
  virtual void ReceivePepsiEffect(long);
  virtual void ReceiveSchoolFoodEffect(long);
  virtual void ReceiveSound(char*, short, float);
  virtual void Regenerate(ushort = 1);
  virtual void SetAgility(ushort What) { Agility = What; if(short(Agility) < 1) Agility = 1; if(Agility > 99) Agility = 99; }
  virtual void SetAgilityExperience(long What) { AgilityExperience = What; }
  virtual void SetAP(long What) { AP = What; }
  virtual void SetConsumingCurrently(item* What) { StateVariables.Consuming.ConsumingCurrently = What; }
  virtual void SetEndurance(ushort What) { Endurance = What; if(short(Endurance) < 1) Endurance = 1; if(Endurance > 99) Endurance = 99; }
  virtual void SetEnduranceExperience(long What) { EnduranceExperience = What; }
  virtual void SetIsPlayer(bool What) { IsPlayer = What; }
  virtual void SetNP(long);
  virtual void SetPerception(ushort What) { Perception = What; if(short(Perception) < 1) Perception = 1; if(Perception > 99) Perception = 99; }
  virtual void SetPerceptionExperience(long What) { PerceptionExperience = What; }
  virtual void SetRegenerationCounter(long What) { RegenerationCounter = What; }
  virtual void SetSquareUnder(square* Square);
  virtual void SetStrength(ushort What) { Strength = What; if(short(Strength) < 1) Strength = 1; if(Strength > 99) Strength = 99; }
  virtual void SetStrengthExperience(long What) { StrengthExperience = What; }
  virtual void SpillBlood(uchar);
  virtual void SpillBlood(uchar, vector2d);
  virtual void Vomit(ushort);
  virtual void Be();
  virtual bool Zap();
  virtual bool Polymorph(character*, ushort);
  virtual bool CanKick() const { return false; }
  virtual void BeKicked(ushort, bool, uchar, character*);
  virtual void FallTo(vector2d, bool);
  virtual bool CheckCannibalism(ushort);
  virtual void VirtualConstructor() { }
  virtual void CharacterSpeciality(ushort = 1) { }
  virtual void ActivateState(uchar Index) { State |= 1 << Index; }
  virtual void DeActivateState(uchar Index) { State &= ~(1 << Index); }
  virtual bool StateIsActivated(uchar Index) const { return State & (1 << Index) ? true : false; }
  virtual void Faint();
  virtual void FaintHandler();
  virtual void ConsumeHandler();
  virtual void PolymorphHandler();
  virtual void SetStateCounter(uchar Index, ushort What) { StateCounter[Index] = What; }
  virtual bool CanMove();
  virtual void DeActivateVoluntaryStates(std::string = "");
  virtual void EndFainted();
  virtual void EndConsuming();
  virtual void EndPolymorph();
  virtual void StruckByWandOfStriking(character*, std::string);
  virtual void StateAutoDeactivation();
  virtual team* GetTeam() const { return Team; }
  virtual void SetTeam(team*);
  virtual void ChangeTeam(team*);
  virtual bool RestUntilHealed();
  virtual void RestHandler();
  virtual void EndRest();
  virtual void DigHandler();
  virtual void EndDig();
  //virtual void SetOldWieldedItem(item* What) { StateVariables.Digging.OldWieldedItem = What; }
  virtual void SetSquareBeingDigged(vector2d What) { StateVariables.Digging.SquareBeingDiggedX = What.X; StateVariables.Digging.SquareBeingDiggedY = What.Y; }
  //virtual item* GetOldWieldedItem() const { return StateVariables.Digging.OldWieldedItem; }
  virtual vector2d GetSquareBeingDigged() const { return vector2d(StateVariables.Digging.SquareBeingDiggedX, StateVariables.Digging.SquareBeingDiggedY); }
  virtual bool OutlineCharacters();
  virtual bool OutlineItems();
  virtual float GetThrowGetStrengthModifier() const;
  virtual short GetMaxHP() const;
  virtual ushort GetMeleeAttributeModifier() const;
  virtual float GetToHitValue() const;
  virtual float GetDodgeValue() const;
  virtual bool RaiseGodRelations();
  virtual bool LowerGodRelations();
  virtual bool GainDivineKnowledge();
  virtual ulong GetMoney() const { return Money; }
  virtual void SetMoney(ulong What) { Money = What; }
  virtual void SetHomeRoom(uchar What) { HomeRoom = What; }
  virtual uchar GetHomeRoom() const { return HomeRoom; }
  virtual bool Displace(character*);
  virtual bool Sit();
  virtual void AddSpecialItemInfo(std::string&, item*) { }
  virtual void AddSpecialItemInfoDescription(std::string&) { }
  virtual long StatScore() const;
  virtual bool CheckStarvationDeath(std::string);
  virtual void ShowNewPosInfo() const;
  virtual void Hostility(character*);
  virtual void KickHit() { }
  virtual bool HasInfraVision() const { return false; }
  virtual uchar CriticalModifier() const { return 20; }
  virtual std::string StandVerb() const { return "standing"; }
  virtual stack* GetGiftStack() const;
  virtual bool MoveRandomlyInRoom();
  virtual bool CanOpenDoors() const { return true; }
  virtual void GoHandler();
  virtual void EndGoing();
  virtual bool Go();
  virtual bool ShowConfigScreen();
  virtual std::list<character*>::iterator GetTeamIterator() { return TeamIterator; }
  virtual void SetTeamIterator(std::list<character*>::iterator What) { TeamIterator = What; }
  virtual bool CanBeDisplaced() const { return true; }
  virtual void ReceiveKoboldFleshEffect(long);
  virtual bool ChangeRandomStat(short);
  virtual uchar RandomizeReply(uchar, bool*);
  virtual ushort Frequency() const { return 10000; }
  virtual ushort DangerLevel();
  virtual void CreateInitialEquipment() { }
  virtual void DisplayInfo();
  virtual bool SpecialEnemySightedReaction(character*) { return false; }
  virtual bool CanWalk() const { return true; }
  virtual bool CanSwim() const { return false; }
  virtual bool CanFly() const { return false; }
  virtual void TestWalkability();
  virtual void EditAP(long What) { AP += What; }
  virtual void EditNP(long What) { NP += What; }
  virtual void EditStrengthExperience(long What) { StrengthExperience += What; }
  virtual void EditEnduranceExperience(long What) { EnduranceExperience += What; }
  virtual void EditAgilityExperience(long What) { AgilityExperience += What; }
  virtual void EditPerceptionExperience(long What) { PerceptionExperience += What; }
  virtual void EditRegenerationCounter(long What) { RegenerationCounter += What; }
  virtual void SetSize(ushort);
  virtual ushort GetSize() const;
  virtual torso* GetTorso() const;
  virtual humanoidtorso* GetHumanoidTorso() const;
  virtual void SetTorso(torso* What);
  virtual bodypart* GetBodyPart(ushort) const;
  virtual void SetBodyPart(ushort, bodypart*);
  virtual void SetMaterial(uchar, material*);
  virtual void Teleport();
  virtual bool SecretKnowledge();
  virtual void RestoreHP();
  virtual bool ReceiveEffect(short, uchar, uchar = ALL, uchar = 8, bool = false, bool = false, bool = false);
  virtual bool ReceiveBodyPartPhysicalDamage(short, uchar, uchar = 8, bool = false, bool = false);
  virtual bool BodyPartVital(uchar);
  virtual void RestoreBodyParts();
  virtual bool AssignName();
  virtual std::string GetAssignedName() const { return AssignedName; }
  virtual void SetAssignedName(std::string What) { AssignedName = What; }
  virtual bool CanBeAssignedAName() const { return true; }
  virtual bool ReceiveBodyPartEffect(short, uchar, uchar, uchar = 8, bool = false, bool = false);
  virtual std::string Description(uchar) const;
  virtual std::string PersonalPronoun() const;
  virtual std::string PossessivePronoun() const;
  virtual std::string ObjectPronoun() const;
  virtual bool BodyPartCanBeSevered(uchar) const;
  virtual std::string Name(uchar) const;
  virtual void ReceiveHeal(long);
  virtual void Haste(ushort);
  virtual void EndHaste();
  virtual void HasteHandler();
  virtual float GetAPStateMultiplier() const;
  virtual void Slow(ushort);
  virtual void SlowHandler();
  virtual void EndSlow();
  virtual item* GetMainWielded() const { return 0; }
  virtual item* GetSecondaryWielded() const { return 0; }
  virtual item* GetBodyArmor() const { return 0; }
  virtual void SetMainWielded(item*) { }
  virtual void SetSecondaryWielded(item*) { }
  virtual void SetBodyArmor(item*) { }
  virtual bool CanWieldInMainHand() const { return false; }
  virtual bool CanWieldInSecondaryHand() const { return false; }
  virtual uchar GetHungerState() const;
  virtual characterslot* GetTorsoSlot() const { return GetBodyPartSlot(0); }
  virtual characterslot* GetBodyPartSlot(ushort) const;
  virtual bool VirtualEquipmentScreen();
  //  virtual uchar GetLegs() const;
  virtual bool CheckKick();
  virtual short GetLengthOfOpen(vector2d) const { return -500; }
  virtual bool CheckThrow() const { return true; }  
  virtual bool CheckApply() const { return true; }
  virtual bool CheckOffer() const { return true; }
 protected:
  virtual ushort TotalSize() const = 0;
  virtual uchar ChooseBodyPartToReceiveHit(float, float);
  virtual void CreateBodyParts();
  virtual void CreateTorso();
  virtual material* CreateTorsoFlesh(ulong) const = 0;
  virtual material* CreateTorsoBone(ulong) const;
  virtual uchar TorsoBonePercentile() const { return 20; }
  virtual ulong TorsoVolume() const { return TotalVolume(); }
  virtual ulong TotalVolume() const = 0;
  virtual uchar BodyParts() const { return 1; }
  virtual vector2d GetBitmapPos() const = 0;
  virtual void AllocateBodyPartArray();
  virtual ushort TorsoSize() const;
  virtual std::string MaterialDescription(bool) const;
  virtual bool ShowClassDescription() const { return true; }
  virtual void SeekLeader();
  virtual bool CheckForUsefulItemsOnGround();
  virtual bool CheckForDoors();
  virtual bool CheckForEnemies(bool);
  virtual bool FollowLeader();
  virtual void StandIdleAI();
  virtual void CreateCorpse();
  virtual std::string DeathMessage() { return Name(DEFINITE) + " is slain."; }
  virtual void SetDefaultStats() = 0;
  virtual void GetPlayerCommand();
  virtual void GetAICommand();
  virtual bool MoveTowards(vector2d);
  virtual float GetMeleeStrength() const { return 0; }
  virtual std::string ThirdPersonWeaponHitVerb(bool Critical) const { return NormalThirdPersonHitVerb(Critical); }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return NormalThirdPersonHitVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return NormalFirstPersonHitVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return NormalThirdPersonHitVerb(Critical); }
  virtual std::string NormalFirstPersonHitVerb(bool Critical) const { return Critical ? "critically hit" : "hit"; }
  virtual std::string NormalThirdPersonHitVerb(bool Critical) const { return Critical ? "critically hits" : "hits"; }
  virtual std::string FirstPersonBiteVerb(bool Critical) const { return Critical ? "critically bite" : "bite"; }
  virtual std::string ThirdPersonBiteVerb(bool Critical) const { return Critical ? "critically bites" : "bites"; }
  virtual std::string FirstPersonPSIVerb(bool Critical) const { return Critical ? "emit powerful psi waves at" : "emit psi waves at"; }
  virtual std::string ThirdPersonPSIVerb(bool Critical) const { return Critical ? "emits powerful psi waves at" : "emits psi waves at"; }
  virtual std::string FirstPersonBrownSlimeVerb(bool Critical) const { return Critical ? "vomit very acidous brown slime at" : "vomit brown slime at"; }
  virtual std::string ThirdPersonBrownSlimeVerb(bool Critical) const { return Critical ? "vomits very acidous brown slime at" : "vomits brown slime at"; }
  virtual std::string FirstPersonBloodVerb(bool Critical) const { return Critical ? "vomit very acidous blood at" : "vomit acidous blood at"; }
  virtual std::string ThirdPersonBloodVerb(bool Critical) const { return Critical ? "vomits very acidous blood at" : "vomits acidous blood at"; }
  virtual std::string TalkVerb() const { return "grunts"; }
  stack* Stack;
  ushort Strength, Endurance, Agility, Perception, RegenerationCounter;
  long NP, AP;
  long StrengthExperience, EnduranceExperience, AgilityExperience, PerceptionExperience;
  bool IsPlayer;
  uchar State;
  short StateCounter[STATES];
  void (character::*StateHandler[STATES])();
  team* Team;
  vector2d WayPoint;
  union statevariables
  {
    struct consuming
    {
      item* ConsumingCurrently;
    } Consuming;
    struct digging
    {
      ushort SquareBeingDiggedX;
      ushort SquareBeingDiggedY;
      //item* OldWieldedItem;
    } Digging;
    struct going
    {
      uchar Direction;
      bool WalkingInOpen;
    } Going;
  } StateVariables;
  ulong Money;
  uchar HomeRoom;
  std::list<character*>::iterator TeamIterator;
  characterslot* BodyPartSlot;
  std::string AssignedName;
};

#ifdef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DECLARATIONS__

#define CHARACTER_PROTOINSTALLER(name, base, setstats)\
  \
  static class name##_protoinstaller\
  {\
   public:\
    name##_protoinstaller() : Index(protocontainer<character>::Add(new name(false, false, false, false, false))) { }\
    ushort GetIndex() const { return Index; }\
   private:\
    ushort Index;\
  } name##_ProtoInstaller;\
  \
  name::name(bool MakeBodyParts, bool SetStats, bool CreateEquipment, bool AllocBodyParts, bool AddToPool) : base(false, false, false, false, AddToPool)\
  {\
    if(AllocBodyParts)\
      {\
	AllocateBodyPartArray();\
      }\
    \
    if(MakeBodyParts)\
      {\
	CreateBodyParts();\
      }\
    \
    if(SetStats)\
      {\
	SetDefaultStats();\
	RestoreHP();\
      }\
    \
    if(CreateEquipment)\
      CreateInitialEquipment();\
  }\
  \
  name::name(material* FirstMaterial, bool SetStats, bool CreateEquipment) : base(false, false, false, false)\
  {\
    AllocateBodyPartArray();\
    CreateBodyParts();\
    SetMaterial(0, FirstMaterial);\
    \
    if(SetStats)\
      {\
	SetDefaultStats();\
	RestoreHP();\
      }\
    \
    if(CreateEquipment)\
      CreateInitialEquipment();\
  }\
  \
  character* name::Clone(bool MakeBodyParts, bool SetStats, bool CreateEquipment) const\
  {\
    return new name(MakeBodyParts, SetStats, CreateEquipment);\
  }\
  \
  type* name::CloneAndLoad(inputfile& SaveFile) const\
  {\
    character* Char = new name(false, false, false);\
    Char->Load(SaveFile);\
    return Char;\
  }\
  \
  void name::SetDefaultStats() { setstats }\
  ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
  const character* const name::GetPrototype() { return protocontainer<character>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

#define CHARACTER_PROTOINSTALLER(name, base, setstats)

#endif

#define CHARACTER(name, base, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool = true, bool = true, bool = true, bool = true, bool = true);\
  name(material*, bool = true, bool = true);\
  virtual character* Clone(bool = true, bool = true, bool = true) const;\
  virtual type* CloneAndLoad(inputfile&) const;\
  static ushort StaticType();\
  static const character* const GetPrototype();\
  virtual std::string ClassName() const { return #name; }\
 protected:\
  virtual void SetDefaultStats();\
  virtual ushort Type() const;\
  data\
}; CHARACTER_PROTOINSTALLER(name, base, setstats)

#define ABSTRACT_CHARACTER(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool MakeBodyParts, bool SetStats, bool CreateEquipment, bool AllocBodyParts, bool AddToPool = true) : base(MakeBodyParts, SetStats, CreateEquipment, AllocBodyParts, AddToPool) { VirtualConstructor(); }\
  data\
};

#endif



