#ifndef __CHARDE_H__
#define __CHARDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "charba.h"
#include "materde.h"
#include "femath.h"
#include "proto.h"
#include "wskill.h"

class head;
class arm;
class rightarm;
class leftarm;
class groin;
class rightleg;
class leftleg;
class humanoidtorso;

class ABSTRACT_CHARACTER
(
  humanoid,
  character,
 public:
  virtual bool CanWield() const;
  virtual bool Hit(character*);
  virtual void AddSpecialItemInfo(std::string&, item*) const;
  virtual void AddSpecialItemInfoDescription(std::string&) const;
  virtual ushort GetSize() const;
  head* GetHead() const { return static_cast<head*>(*BodyPartSlot[HEADINDEX]); }
  rightarm* GetRightArm() const { return static_cast<rightarm*>(*BodyPartSlot[RIGHTARMINDEX]); }
  leftarm* GetLeftArm() const { return static_cast<leftarm*>(*BodyPartSlot[LEFTARMINDEX]); }
  groin* GetGroin() const { return static_cast<groin*>(*BodyPartSlot[GROININDEX]); }
  rightleg* GetRightLeg() const { return static_cast<rightleg*>(*BodyPartSlot[RIGHTLEGINDEX]); }
  leftleg* GetLeftLeg() const { return static_cast<leftleg*>(*BodyPartSlot[LEFTLEGINDEX]); }
  void SetHead(head* What) { SetBodyPart(HEADINDEX, What); }
  void SetRightArm(rightarm* What) { SetBodyPart(RIGHTARMINDEX, What); }
  void SetLeftArm(leftarm* What) { SetBodyPart(LEFTARMINDEX, What); }
  void SetGroin(groin* What) { SetBodyPart(GROININDEX, What); }
  void SetRightLeg(rightleg* What) { SetBodyPart(RIGHTLEGINDEX, What); }
  void SetLeftLeg(leftleg* What) { SetBodyPart(LEFTLEGINDEX, What); }
  item* GetHelmet() const { return GetHead() ? GetHead()->GetHelmet() : 0; }
  item* GetAmulet() const { return GetHead() ? GetHead()->GetAmulet() : 0; }
  item* GetCloak() const { return GetHumanoidTorso() ? GetHumanoidTorso()->GetCloak() : 0; }
  item* GetBodyArmor() const { return GetHumanoidTorso() ? GetHumanoidTorso()->GetBodyArmor() : 0; }
  item* GetBelt() const { return GetHumanoidTorso() ? GetHumanoidTorso()->GetBelt() : 0; }
  item* GetRightWielded() const { return GetRightArm() ? GetRightArm()->GetWielded() : 0; }
  item* GetLeftWielded() const { return GetLeftArm() ? GetLeftArm()->GetWielded() : 0; }
  item* GetRightRing() const { return GetRightArm() ? GetRightArm()->GetRing() : 0; }
  item* GetLeftRing() const { return GetLeftArm() ? GetLeftArm()->GetRing() : 0; }
  item* GetRightGauntlet() const { return GetRightArm() ? GetRightArm()->GetGauntlet() : 0; }
  item* GetLeftGauntlet() const { return GetLeftArm() ? GetLeftArm()->GetGauntlet() : 0; }
  item* GetRightBoot() const { return GetRightLeg() ? GetRightLeg()->GetBoot() : 0; }
  item* GetLeftBoot() const { return GetLeftLeg() ? GetLeftLeg()->GetBoot() : 0; }
  void SetHelmet(item* What) { GetHead()->SetHelmet(What); }
  void SetAmulet(item* What) { GetHead()->SetAmulet(What); }
  void SetCloak(item* What) { GetHumanoidTorso()->SetCloak(What); }
  void SetBodyArmor(item* What) { GetHumanoidTorso()->SetBodyArmor(What); }
  void SetBelt(item* What) { GetHumanoidTorso()->SetBelt(What); }
  void SetRightWielded(item* What) { GetRightArm()->SetWielded(What); }
  void SetLeftWielded(item* What) { GetLeftArm()->SetWielded(What); }
  void SetRightRing(item* What) { GetRightArm()->SetRing(What); }
  void SetLeftRing(item* What) { GetLeftArm()->SetRing(What); }
  void SetRightGauntlet(item* What) { GetRightArm()->SetGauntlet(What); }
  void SetLeftGauntlet(item* What) { GetLeftArm()->SetGauntlet(What); }
  void SetRightBoot(item* What) { GetRightLeg()->SetBoot(What); }
  void SetLeftBoot(item* What) { GetLeftLeg()->SetBoot(What); }
  virtual arm* GetMainArm() const;
  virtual arm* GetSecondaryArm() const;
  virtual bool ReceiveDamage(character*, ushort, uchar, uchar = ALL, uchar = 8, bool = false, bool = false, bool = false);
  virtual bool BodyPartVital(ushort Index) const { return Index == TORSOINDEX || Index == HEADINDEX || Index == GROININDEX; }
  virtual bool BodyPartCanBeSevered(ushort Index) const { return Index != TORSOINDEX && Index != GROININDEX && GetBodyPart(Index); }
  virtual item* GetMainWielded() const;
  virtual item* GetSecondaryWielded() const;
  virtual void SetMainWielded(item*);
  virtual void SetSecondaryWielded(item*);
  virtual std::string EquipmentName(ushort) const;
  virtual bodypart* GetBodyPartOfEquipment(ushort) const;
  virtual item* GetEquipment(ushort) const;
  virtual ushort EquipmentSlots() const { return 13; }
  virtual void SwitchToDig(item*, vector2d);
  virtual uchar GetLegs() const;
  virtual uchar GetArms() const;
  virtual bool CheckKick() const;
  virtual uchar OpenMultiplier() const { return GetRightArm() || GetLeftArm() ? 1 : 3; }
  virtual uchar CloseMultiplier() const { return GetRightArm() || GetLeftArm() ? 1 : 2; }
  virtual bool CheckThrow() const;
  virtual bool CheckOffer() const;
  virtual bool (*EquipmentSorter(ushort) const)(item*, const character*);
  virtual void SetEquipment(ushort, item*);
  virtual void DrawSilhouette(bitmap*, vector2d, bool) const;
  virtual ushort GlobalResistance(uchar) const;
  virtual void AddInfo(felist&) const;
  virtual void CompleteRiseFromTheDead();
  virtual bool HandleNoBodyPart(ushort);
  virtual bool CheckWearEquipment() const { return true; }
  virtual void Kick(lsquare*);
  virtual float GetTimeToKill(const character*, bool) const;
  virtual ushort GetAttribute(ushort) const;
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual ushort DrawStats(bool) const;
  virtual void Bite(character*);
  virtual ushort GetCarryingStrength() const { return Max<ushort>(GetAttribute(LEGSTRENGTH), 1); }
  virtual ushort GetRandomStepperBodyPart() const;
  virtual ushort CheckForBlock(character*, item*, float, ushort, short, uchar);
  virtual bool AddSpecialSkillInfo(felist&) const;
  virtual bool CheckBalance(float);
  virtual long GetMoveAPRequirement(uchar) const;
  virtual bool EquipmentHasNoPairProblems(ushort) const;
  virtual bool DetachBodyPart();
  virtual vector2d GetEquipmentPanelPos(ushort) const;
  virtual bool EquipmentEasilyRecognized(ushort) const;
  sweaponskill* GetCurrentRightSWeaponSkill() const { return CurrentRightSWeaponSkill; }
  void SetCurrentRightSWeaponSkill(sweaponskill* What) { CurrentRightSWeaponSkill = What; }
  sweaponskill* GetCurrentLeftSWeaponSkill() const { return CurrentLeftSWeaponSkill; }
  void SetCurrentLeftSWeaponSkill(sweaponskill* What) { CurrentLeftSWeaponSkill = What; }
  sweaponskill* GetSWeaponSkill(ushort Index) const { return SWeaponSkill[Index]; }
  void SetSWeaponSkill(ushort Index, sweaponskill* What) { SWeaponSkill[Index] = What; }
  ushort GetSWeaponSkills() const { return SWeaponSkill.size(); }
  virtual void SWeaponSkillTick();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SignalEquipmentAdd(ushort);
  virtual void SignalEquipmentRemoval(ushort);
  virtual uchar GetBodyParts() const { return HUMANOID_BODYPARTS; }
  virtual void DrawBodyParts(bitmap*, vector2d, ushort, bool, bool) const;
  virtual bool CanUseStethoscope(bool) const;
  virtual bool IsUsingArms() const;
  virtual bool IsUsingLegs() const;
  virtual bool IsUsingHead() const;
  virtual void AddAttackInfo(felist&) const;
  virtual void CalculateBattleInfo();
  virtual leg* GetKickLeg() const;
  virtual void CalculateBodyParts() { BodyParts = HUMANOID_BODYPARTS; }
  virtual void CalculateAllowedWeaponSkillCategories() { AllowedWeaponSkillCategories = WEAPON_SKILL_CATEGORIES; }
  virtual bool HasFeet() const;
  virtual void AddSpecialEquipmentInfo(std::string&, ushort) const;
  virtual void CreateInitialEquipment();
  virtual std::string GetBodyPartName(ushort, bool = false) const;
  virtual void CreateBlockPossibilityVector(blockvector&, float) const;
  void CheckIfSWeaponSkillRemovalNeeded(sweaponskill*);
  virtual item* SevereBodyPart(ushort);
  virtual void AddDefenceInfo(felist&) const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBodyPartBitmapPos(ushort, ushort);
  virtual ushort GetBodyPartColorB(ushort, ushort);
  virtual ushort GetBodyPartColorC(ushort, ushort);
  virtual ushort GetBodyPartColorD(ushort, ushort);
  virtual material* CreateBodyPartFlesh(ushort, ulong) const;
  virtual ulong GetBodyPartSize(ushort, ushort);
  virtual ulong GetBodyPartVolume(ushort);
  virtual uchar GetBodyPartBonePercentile(ushort);
  virtual bodypart* MakeBodyPart(ushort);
  virtual std::string GetDeathMessage() const;
  std::vector<sweaponskill*> SWeaponSkill;
  sweaponskill* CurrentRightSWeaponSkill;
  sweaponskill* CurrentLeftSWeaponSkill;
);

class ABSTRACT_CHARACTER
(
  nonhumanoid,
  character,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void CalculateUnarmedDamage();
  virtual void CalculateKickDamage();
  virtual void CalculateBiteDamage();
  virtual void CalculateUnarmedToHitValue();
  virtual void CalculateKickToHitValue();
  virtual void CalculateBiteToHitValue();
  virtual void CalculateUnarmedAPCost();
  virtual void CalculateKickAPCost();
  virtual void CalculateBiteAPCost();
  float GetUnarmedDamage() const { return UnarmedDamage; }
  ushort GetUnarmedMinDamage() const { return ushort(UnarmedDamage * 0.75f); }
  ushort GetUnarmedMaxDamage() const { return ushort(UnarmedDamage * 1.25f + 1); }
  float GetKickDamage() const { return KickDamage; }
  ushort GetKickMinDamage() const { return ushort(KickDamage * 0.75f); }
  ushort GetKickMaxDamage() const { return ushort(KickDamage * 1.25f + 1); }
  float GetBiteDamage() const { return BiteDamage; }
  ushort GetBiteMinDamage() const { return ushort(BiteDamage * 0.75f); }
  ushort GetBiteMaxDamage() const { return ushort(BiteDamage * 1.25f + 1); }
  float GetUnarmedToHitValue() const { return UnarmedToHitValue; }
  float GetKickToHitValue() const { return KickToHitValue; }
  float GetBiteToHitValue() const { return BiteToHitValue; }
  long GetUnarmedAPCost() const { return UnarmedAPCost; }
  long GetKickAPCost() const { return KickAPCost; }
  long GetBiteAPCost() const { return BiteAPCost; }
  virtual void Kick(lsquare*);
  virtual void AddInfo(felist&) const;
  virtual bool Hit(character*);
  virtual void UnarmedHit(character*);
  virtual void InitSpecialAttributes();
  virtual float GetTimeToKill(const character*, bool) const;
  virtual void ApplyExperience(bool = false);
  virtual ushort GetAttribute(ushort) const;
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual ushort DrawStats(bool) const;
  virtual void Bite(character*);
  virtual bool RaiseStats();
  virtual bool LowerStats();
  virtual ushort GetCarryingStrength() const { return Max<ushort>(GetAttribute(LEGSTRENGTH) << 1, 1); }
  virtual void AddAttackInfo(felist&) const;
  virtual void CalculateBattleInfo();
  virtual void CalculateUnarmedAttackInfo();
  virtual void CalculateKickAttackInfo();
  virtual void CalculateBiteAttackInfo();
 protected:
  ushort Strength;
  ushort Agility;
  long StrengthExperience;
  long AgilityExperience;
  float UnarmedDamage;
  float KickDamage;
  float BiteDamage;
  float UnarmedToHitValue;
  float KickToHitValue;
  float BiteToHitValue;
  long UnarmedAPCost;
  long KickAPCost;
  long BiteAPCost;
);

class CHARACTER
(
  human,
  humanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetTotalSize() const { return TotalSize; }
  virtual void SetTotalSize(ushort What) { TotalSize = What; }
 protected:
  virtual void VirtualConstructor(bool);
  ushort TotalSize;
);

class CHARACTER
(
  petrus,
  humanoid,
 public:
  virtual ~petrus();
  virtual void Load(inputfile&);
  virtual void BeTalkedTo(character*);
  bool HealFully(character*);
  virtual void SetHealTimer(ushort What) { HealTimer = What; }
  virtual ushort GetHealTimer() const { return HealTimer; }
  virtual void Save(outputfile&) const;
  virtual uchar GetStoryState() const { return StoryState; }
  virtual void SetStoryState(uchar What) { StoryState = What; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual void CreateCorpse();
  virtual std::string GetDeathMessage() const { return "The High Priest disappears in a bright light and only his left nut is left behind."; }
  virtual void GetAICommand();
  ushort HealTimer;
  uchar StoryState;
);

class CHARACTER
(
  farmer,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual vector2d GetHeadBitmapPos(ushort) const { return vector2d(96, (4 + (RAND() & 1)) * 16); }
  virtual vector2d GetRightArmBitmapPos(ushort) const { return vector2d(64, (RAND() & 1) * 16); }
  virtual vector2d GetLeftArmBitmapPos(ushort Frame) const { return GetRightArmBitmapPos(Frame); }
  virtual vector2d GetGroinBitmapPos(ushort) const { return vector2d(0, (RAND() & 1) * 16); }
  virtual vector2d GetRightLegBitmapPos(ushort Frame) const { return GetGroinBitmapPos(Frame); }
  virtual vector2d GetLeftLegBitmapPos(ushort Frame) const { return GetGroinBitmapPos(Frame); }
);

class CHARACTER
(
  guard,
  humanoid,
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void BeTalkedTo(character*);
);

class CHARACTER
(
  shopkeeper,
  humanoid,
 public:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void BeTalkedTo(character*);
);

class CHARACTER
(
  priest,
  humanoid,
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void BeTalkedTo(character*);
);

class CHARACTER
(
  oree,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual void VirtualConstructor(bool);
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DAEMONFLESH, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + "vomits blood for one last time and then dies."; }
  virtual std::string FirstPersonBiteVerb() const { return "vomit acidous blood at"; }
  virtual std::string FirstPersonCriticalBiteVerb() const { return "vomit very acidous blood at"; }
  virtual std::string ThirdPersonBiteVerb() const { return "vomits acidous blood at"; }
  virtual std::string ThirdPersonCriticalBiteVerb() const { return "vomits very acidous blood at"; }
  virtual std::string BiteNoun() const { return "liquid"; }
);

class CHARACTER
(
  darkknight,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
);

class CHARACTER
(
  ennerbeast,
  humanoid,
 public:
  virtual bool Hit(character*);
  virtual void BeTalkedTo(character*);
  /*virtual ulong CurrentDanger() const { return character::CurrentDanger() * 5; }
  virtual ulong MaxDanger() const { return character::MaxDanger() * 5; }*/
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(ENNERBEASTFLESH, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " dies and the world is finally freed from this terrible monster."; }
  virtual void GetAICommand();
  virtual bool AttackIsBlockable(uchar) const { return false; }
);

class ABSTRACT_CHARACTER
(
  frog,
  nonhumanoid,
  ;
);

class CHARACTER
(
  darkfrog,
  frog,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(FROGFLESH, Volume); }
);

class CHARACTER
(
  elpuri,
  darkfrog,
 public:
  virtual void VirtualConstructor(bool);
  virtual void BeTalkedTo(character*);
  virtual bool Hit(character*);
 protected:
  virtual void CreateCorpse();
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(ELPURIFLESH, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " groans horribly and drops " + GetPossessivePronoun() + " head."; }
);

class CHARACTER
(
  billswill,
  nonhumanoid,
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual void CalculateUnarmedDamage();
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new gas(AIR, Volume); }
  virtual void CreateCorpse() { SendToHell(); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " vanishes from existence."; }
  virtual std::string FirstPersonUnarmedHitVerb() const { return "emit psi waves at"; }
  virtual std::string FirstPersonCriticalUnarmedHitVerb() const { return "emit powerful psi waves at"; }
  virtual std::string ThirdPersonUnarmedHitVerb() const { return "emits psi waves at"; }
  virtual std::string ThirdPersonCriticalUnarmedHitVerb() const { return "emits powerful psi waves at"; }
  virtual bool AttackIsBlockable(uchar) const { return false; }
);

class CHARACTER
(
  skeleton,
  humanoid,
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual item* SevereBodyPart(ushort);
  virtual bool BodyPartVital(ushort Index) const { return Index == GROININDEX || Index == TORSOINDEX; }
 protected:
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " is transformed into a crumpled heap of bones."; }
  virtual void CreateCorpse();
);

class CHARACTER
(
  goblin,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(GOBLINOIDFLESH, Volume); }
);

class CHARACTER
(
  mommo,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(BROWNSLIME, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " turns into lifeless goo."; }
  virtual std::string FirstPersonBiteVerb() const { return "vomit acidous slime at"; }
  virtual std::string FirstPersonCriticalBiteVerb() const { return "vomit very acidous slime at"; }
  virtual std::string ThirdPersonBiteVerb() const { return "vomits acidous slime at"; }
  virtual std::string ThirdPersonCriticalBiteVerb() const { return "vomits very acidous slime at"; }
  virtual std::string BiteNoun() const { return "slime"; }
  virtual bool HasFeet() const { return false; }
);

class CHARACTER
(
  golem,
  humanoid,
 public:
  virtual bool MoveRandomly();
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual bool CheckForUsefulItemsOnGround() { return false; }
 protected:
  virtual std::string GetDeathMessage() const { return "The Holy Words of " + GetName(DEFINITE) + " fly away and the monster collapses."; }
);

class CHARACTER
(
  wolf,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(WOLFFLESH, Volume); }
);

class CHARACTER
(
  dog,
  nonhumanoid,
 public:
  virtual bool Catches(item*, float);
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DOGFLESH, Volume); }
);

class CHARACTER
(
  spider,
  nonhumanoid,
 public:
  virtual bool SpecialBiteEffect(character*, uchar, uchar, bool);
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(SPIDERFLESH, Volume); }
  virtual void CreateCorpse() { SendToHell(); }
);

class CHARACTER
(
  jackal,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(JACKALFLESH, Volume); }
);

class CHARACTER
(
  donkey,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DONKEYFLESH, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " neighs one last time and dies."; }
);

class CHARACTER
(
  communist,
  humanoid,
 public:
  virtual bool MoveRandomly();
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual void VirtualConstructor(bool);
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " falls groaning bravely: \"Party revenges " + GetName(UNARTICLED) + "\"!"; }
  virtual bool ShowClassDescription() const { return GetAssignedName() != "Ivan"; }
);

class CHARACTER
(
  hunter,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual void CreateBodyParts();
);

class CHARACTER
(
  polarbear,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(POLARBEARFLESH, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " groans terribly and falls dead to the ground."; }
);

class CHARACTER
(
  dolphin,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DOLPHINFLESH, Volume); }
  virtual uchar GetSpecialBodyPartFlags(ushort, ushort) const { return (RAND() % 8)&~FLIP; }
  virtual void SpecialTurnHandler() { UpdateBodyPartPictures(); }
);

class CHARACTER
(
  lightfrog,
  frog,
 public:
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(FROGFLESH, Volume); }
);

class CHARACTER
(
  slave,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
  virtual void GetAICommand();
);

class CHARACTER
(
  petrusswife,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
);

class CHARACTER
(
  housewife,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort GetHairColor(ushort) const;
  virtual vector2d GetHeadBitmapPos(ushort) const { return vector2d(112, (RAND() % 6) * 16); }
);

class CHARACTER
(
  femaleslave,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  librarian,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  zombie,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
  virtual void SpillBlood(uchar, vector2d);
  virtual bool BodyPartVital(ushort Index) const { return Index == GROININDEX || Index == TORSOINDEX; }
  virtual void CreateBodyParts();
 protected:
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " is slain (again)."; }
);

class CHARACTER
(
  imp,
  humanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(SULFUR, Volume); }
);

class CHARACTER
(
  bat,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(BATFLESH, Volume); }
);

class CHARACTER
(
  mistress,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
);

class CHARACTER
(
  werewolfhuman,
  humanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(WEREWOLFFLESH, Volume); }
);

class CHARACTER
(
  werewolfwolf,
  humanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(WEREWOLFFLESH, Volume); }
);

class CHARACTER
(
  kobold,
  humanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(KOBOLDFLESH, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " dies yelling like a tortured hyena."; }
);

class CHARACTER
(
  gibberling,
  humanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(GIBBERLINGFLESH, Volume); }
);

class CHARACTER  
(        
  largecat,
  nonhumanoid,
 public:
  virtual bool Catches(item*, float);
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(CATFLESH, Volume); }
);

class CHARACTER  
(        
  largerat,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(RATFLESH, Volume); }
);

class CHARACTER  
(        
  angel,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool AttachBodyPartsOfFriendsNear(); 
  virtual void SetHealTimer(ushort What) { HealTimer = What; }
  virtual ushort GetHealTimer() const { return HealTimer; }
  virtual void CreateBodyParts();
  virtual bool BodyPartVital(ushort Index) const { return Index == TORSOINDEX || Index == HEADINDEX; }
  virtual ushort GetAttribute(ushort) const;
 protected:
  virtual ushort GetTorsoMainColor(ushort) const;
  virtual ushort GetArmMainColor(ushort) const;
  virtual void VirtualConstructor(bool);
  virtual void CreateInitialEquipment();
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " leaves this mortal plane behind."; }
  virtual void CreateCorpse() { SendToHell(); }
  virtual void AddPostFix(std::string& String) const { AddDivineMasterDescription(String, GetConfig()); }
  virtual void GetAICommand();
  ushort HealTimer;
);

class ABSTRACT_CHARACTER
(
  dwarf,
  humanoid,
 public:
  virtual void DrawBodyParts(bitmap*, vector2d, ushort, bool, bool) const;
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DWARFFLESH, Volume); }
);

class CHARACTER
(
  kamikazedwarf,
  dwarf,
 public:
  virtual void BeTalkedTo(character*);
  /*virtual ulong CurrentDanger() const { return character::CurrentDanger() * 50; }
  virtual ulong MaxDanger() const { return character::MaxDanger() * 50; }*/
  virtual bool Hit(character*);
  virtual bool CheckForUsefulItemsOnGround() { return false; }
  virtual void GetAICommand();
  virtual void CreateInitialEquipment();
 protected:
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " dies smiling."; }
);

class CHARACTER  
(        
  mammoth,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(MAMMOTHFLESH, Volume); }
);

class CHARACTER
(
  unicorn,
  nonhumanoid,
 public:
  virtual void SetAlignment(uchar What) { Alignment = What; }
  virtual uchar GetAlignment() const { return Alignment; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool SpecialEnemySightedReaction(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual void CreateBodyParts();
  virtual material* CreateBodyPartFlesh(ushort, ulong) const;
  uchar Alignment;
);

class CHARACTER
(
  genie,
  humanoid,
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual void CreateBodyParts();
  virtual bool BodyPartVital(ushort Index) const { return Index == TORSOINDEX || Index == HEADINDEX; }
  virtual ushort GetAttribute(ushort) const;
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new gas(AIR, Volume); }
  virtual void CreateCorpse() { SendToHell(); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " vanishes from existence."; }
);

class CHARACTER
(
  lion,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(LIONFLESH, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " growls ans is slain."; }
);

class CHARACTER
(
  carnivorousplant,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(FIBER, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " is destroyed."; }
  virtual ushort GetTorsoSpecialColor(ushort) const { return RAND() % WHITE; } // the flower
  virtual void GetAICommand();
);

class CHARACTER
(
  buffalo,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(BUFFALOFLESH, Volume); }
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " snarls one last time."; }
);

class CHARACTER
(
  snake,
  nonhumanoid,
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(SNAKEFLESH, Volume); }
  virtual bool SpecialBiteEffect(character*, uchar, uchar, bool);
);

class CHARACTER
(
  orc,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(ORCFLESH, Volume); }
);


class CHARACTER
(
  cossack,
  humanoid,
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " falls groaning bravely: \"Hope there's vodka in hell!"; }
);

#endif

