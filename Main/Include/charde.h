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
  humanoid(const humanoid&);
  virtual bool CanWield() const;
  virtual bool Hit(character*, bool = false);
  virtual ushort GetSize() const;
  head* GetHead() const { return static_cast<head*>(*BodyPartSlot[HEAD_INDEX]); }
  rightarm* GetRightArm() const { return static_cast<rightarm*>(*BodyPartSlot[RIGHT_ARM_INDEX]); }
  leftarm* GetLeftArm() const { return static_cast<leftarm*>(*BodyPartSlot[LEFT_ARM_INDEX]); }
  groin* GetGroin() const { return static_cast<groin*>(*BodyPartSlot[GROIN_INDEX]); }
  rightleg* GetRightLeg() const { return static_cast<rightleg*>(*BodyPartSlot[RIGHT_LEG_INDEX]); }
  leftleg* GetLeftLeg() const { return static_cast<leftleg*>(*BodyPartSlot[LEFT_LEG_INDEX]); }
  void SetHead(head* What) { SetBodyPart(HEAD_INDEX, What); }
  void SetRightArm(rightarm* What) { SetBodyPart(RIGHT_ARM_INDEX, What); }
  void SetLeftArm(leftarm* What) { SetBodyPart(LEFT_ARM_INDEX, What); }
  void SetGroin(groin* What) { SetBodyPart(GROIN_INDEX, What); }
  void SetRightLeg(rightleg* What) { SetBodyPart(RIGHT_LEG_INDEX, What); }
  void SetLeftLeg(leftleg* What) { SetBodyPart(LEFT_LEG_INDEX, What); }
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
  arm* GetMainArm() const;
  arm* GetSecondaryArm() const;
  virtual bool ReceiveDamage(character*, ushort, uchar, uchar = ALL, uchar = 8, bool = false, bool = false, bool = false, bool = true);
  virtual bool BodyPartIsVital(ushort Index) const { return Index == TORSO_INDEX || Index == HEAD_INDEX || Index == GROIN_INDEX; }
  virtual bool BodyPartCanBeSevered(ushort Index) const { return Index != TORSO_INDEX && Index != GROIN_INDEX; }
  virtual item* GetMainWielded() const;
  virtual item* GetSecondaryWielded() const;
  virtual void SetMainWielded(item*);
  virtual void SetSecondaryWielded(item*);
  virtual std::string EquipmentName(ushort) const;
  virtual bodypart* GetBodyPartOfEquipment(ushort) const;
  virtual item* GetEquipment(ushort) const;
  virtual ushort GetEquipmentSlots() const { return 13; }
  virtual void SwitchToDig(item*, vector2d);
  virtual uchar GetLegs() const;
  virtual uchar GetArms() const;
  virtual bool CheckKick() const;
  virtual uchar OpenMultiplier() const { return GetRightArm() || GetLeftArm() ? 1 : 3; }
  virtual uchar CloseMultiplier() const { return GetRightArm() || GetLeftArm() ? 1 : 2; }
  virtual bool CheckThrow() const;
  virtual bool CheckOffer() const;
  virtual bool (*EquipmentSorter(ushort) const)(const item*, const character*);
  virtual void SetEquipment(ushort, item*);
  virtual void DrawSilhouette(bitmap*, vector2d, bool) const;
  virtual ushort GlobalResistance(uchar) const;
  virtual void CompleteRiseFromTheDead();
  virtual bool HandleNoBodyPart(ushort);
  virtual void Kick(lsquare*, bool = false);
  virtual float GetTimeToKill(const character*, bool) const;
  virtual ushort GetAttribute(ushort) const;
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual ushort DrawStats(bool) const;
  virtual void Bite(character*, bool = false);
  virtual ushort GetCarryingStrength() const { return Max<ushort>(GetAttribute(LEG_STRENGTH), 1) + CarryingBonus; }
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
  virtual void DrawBodyParts(bitmap*, vector2d, ulong, bool, bool) const;
  virtual bool CanUseStethoscope(bool) const;
  virtual bool IsUsingArms() const;
  virtual bool IsUsingLegs() const;
  virtual bool IsUsingHead() const;
  virtual void AddAttackInfo(felist&) const;
  virtual void CalculateBattleInfo();
  leg* GetKickLeg() const;
  virtual void CalculateBodyParts() { BodyParts = HUMANOID_BODYPARTS; }
  virtual void CalculateAllowedWeaponSkillCategories() { AllowedWeaponSkillCategories = WEAPON_SKILL_CATEGORIES; }
  virtual bool HasFeet() const { return GetLeftLeg() || GetRightLeg(); }
  virtual void AddSpecialEquipmentInfo(std::string&, ushort) const;
  virtual void CreateInitialEquipment(ushort);
  virtual std::string GetBodyPartName(ushort, bool = false) const;
  virtual void CreateBlockPossibilityVector(blockvector&, float) const;
  void CheckIfSWeaponSkillRemovalNeeded(sweaponskill*);
  virtual item* SevereBodyPart(ushort);
  virtual void AddDefenceInfo(felist&) const;
  virtual uchar GetSWeaponSkillLevel(const item*) const;
  virtual bool IsAlive() const;
  virtual bool ShowBattleInfo();
  virtual void AddSpecialMovePenaltyInfo(felist&) const;
  virtual void CalculateDodgeValue();
  virtual bool CheckZap();
  virtual void AddAttributeInfo(std::string&) const;
  virtual bool CheckTalk();
  virtual bool CanCreateBodyPart(ushort) { return true; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBodyPartBitmapPos(ushort);
  virtual ushort GetBodyPartColorB(ushort);
  virtual ushort GetBodyPartColorC(ushort);
  virtual ushort GetBodyPartColorD(ushort);
  virtual ulong GetBodyPartSize(ushort, ushort);
  virtual ulong GetBodyPartVolume(ushort) const;
  virtual bodypart* MakeBodyPart(ushort);
  virtual const std::string& GetDeathMessage() const;
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
  void CalculateUnarmedDamage();
  void CalculateKickDamage();
  void CalculateBiteDamage();
  void CalculateUnarmedToHitValue();
  void CalculateKickToHitValue();
  void CalculateBiteToHitValue();
  void CalculateUnarmedAPCost();
  void CalculateKickAPCost();
  void CalculateBiteAPCost();
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
  virtual void Kick(lsquare*, bool = false);
  virtual bool Hit(character*, bool = false);
  virtual void UnarmedHit(character*, bool = false);
  virtual void InitSpecialAttributes();
  virtual float GetTimeToKill(const character*, bool) const;
  virtual void ApplyExperience(bool = false);
  virtual ushort GetAttribute(ushort) const;
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual ushort DrawStats(bool) const;
  virtual void Bite(character*, bool = false);
  virtual bool RaiseStats();
  virtual bool LowerStats();
  virtual ushort GetCarryingStrength() const { return (Max<ushort>(GetAttribute(LEG_STRENGTH), 1) << 1) + CarryingBonus; }
  virtual void AddAttackInfo(felist&) const;
  virtual void CalculateBattleInfo();
  void CalculateUnarmedAttackInfo();
  void CalculateKickAttackInfo();
  void CalculateBiteAttackInfo();
  virtual bool IsAlive() const { return GetTorso()->IsAlive(); }
  virtual bool ShowBattleInfo();
  void ShowUnarmedInfo() const;
  void ShowKickInfo() const;
  void ShowBiteInfo() const;
  void SetStrength(ushort What) { Strength = What; }
  void SetAgility(ushort What) { Agility = What; }
  virtual void AddAttributeInfo(std::string&) const;
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
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void BeTalkedTo();
  bool HealFully(character*);
  void SetHealTimer(ushort What) { HealTimer = What; }
  ushort GetHealTimer() const { return HealTimer; }
  uchar GetStoryState() const { return StoryState; }
  void SetStoryState(uchar What) { StoryState = What; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual void CreateCorpse();
  virtual void GetAICommand();
  ushort HealTimer;
  uchar StoryState;
);

class CHARACTER
(
  farmer,
  humanoid,
 protected:
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, (4 + (RAND() & 1)) * 16); }
  virtual vector2d GetRightArmBitmapPos() const { return vector2d(64, (RAND() & 1) * 16); }
  virtual vector2d GetLeftArmBitmapPos() const { return GetRightArmBitmapPos(); }
);

class CHARACTER
(
  guard,
  humanoid,
 public:
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  shopkeeper,
  humanoid,
 public:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  priest,
  humanoid,
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void BeTalkedTo();
);

class CHARACTER
(
  oree,
  humanoid,
 protected:
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
  ;
);

class CHARACTER
(
  ennerbeast,
  humanoid,
 public:
  virtual bool Hit(character*, bool = false);
 protected:
  virtual void GetAICommand();
  virtual bool AttackIsBlockable(uchar) const { return false; }
);

class CHARACTER
(
  frog,
  nonhumanoid,
 public:
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
);

class CHARACTER
(
  elpuri,
  frog,
 public:
  virtual bool Hit(character*, bool = false);
 protected:
  virtual void CreateCorpse();
);

class CHARACTER
(
  billswill,
  nonhumanoid,
 protected:
  virtual void CreateCorpse() { SendToHell(); }
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
  virtual void BeTalkedTo();
  virtual item* SevereBodyPart(ushort);
  virtual bool BodyPartIsVital(ushort Index) const { return Index == GROIN_INDEX || Index == TORSO_INDEX; }
 protected:
  virtual void CreateCorpse();
);

class CHARACTER
(
  goblin,
  humanoid,
  ;
);

class CHARACTER
(
  mommo,
  nonhumanoid,
 protected:
  virtual std::string FirstPersonBiteVerb() const { return "vomit acidous slime at"; }
  virtual std::string FirstPersonCriticalBiteVerb() const { return "vomit very acidous slime at"; }
  virtual std::string ThirdPersonBiteVerb() const { return "vomits acidous slime at"; }
  virtual std::string ThirdPersonCriticalBiteVerb() const { return "vomits very acidous slime at"; }
  virtual std::string BiteNoun() const { return "slime"; }
);

class CHARACTER
(
  golem,
  humanoid,
 public:
  virtual bool MoveRandomly();
  virtual bool CheckForUsefulItemsOnGround() { return false; }
 protected:
  virtual material* CreateBodyPartMaterial(ushort, ulong Volume) const { return MAKE_MATERIAL(Config, Volume); }
);

class CHARACTER
(
  wolf,
  nonhumanoid,
 protected:
  virtual ushort GetSkinColor() const;
);

class CHARACTER
(
  dog,
  nonhumanoid,
 public:
  virtual bool Catches(item*, float);
  virtual void BeTalkedTo();
);

class CHARACTER
(
  spider,
  nonhumanoid,
 public:
  virtual bool SpecialBiteEffect(character*, uchar, uchar, bool);
 protected:
  virtual void CreateCorpse() { SendToHell(); }
);

class CHARACTER
(
  jackal,
  nonhumanoid,
  ;
);

class CHARACTER
(
  ass,
  nonhumanoid,
  ;
);

class CHARACTER
(
  communist,
  humanoid,
 public:
  virtual bool MoveRandomly();
  virtual void BeTalkedTo();
 protected:
  virtual bool ShowClassDescription() const { return GetAssignedName() != "Ivan"; }
);

class CHARACTER
(
  hunter,
  humanoid,
 public:
  virtual void BeTalkedTo();
 protected:
  virtual void CreateBodyParts(ushort);
);

class CHARACTER
(
  polarbear,
  nonhumanoid,
  ;
);

class CHARACTER
(
  dolphin,
  nonhumanoid,
 protected:
  virtual uchar GetSpecialBodyPartFlags(ushort) const { return (RAND() % 8)&~FLIP; }
  virtual void SpecialTurnHandler() { UpdatePictures(); }
);

class CHARACTER
(
  slave,
  humanoid,
 public:
  virtual void BeTalkedTo();
  virtual void GetAICommand();
);

class CHARACTER
(
  petrusswife,
  humanoid,
 public:
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
);

class CHARACTER
(
  housewife,
  humanoid,
 protected:
  virtual ushort GetHairColor() const;
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, (RAND() % 6) * 16); }
);

class CHARACTER
(
  femaleslave,
  humanoid,
 protected:
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  librarian,
  humanoid,
 public:
  virtual void BeTalkedTo();
 protected:
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  zombie,
  humanoid,
 public:
  virtual void BeTalkedTo();
  virtual bool BodyPartIsVital(ushort Index) const { return Index == GROIN_INDEX || Index == TORSO_INDEX; }
  virtual void CreateBodyParts(ushort);
);

class CHARACTER
(
  imp,
  humanoid,
  ;
);

class CHARACTER
(
  bat,
  nonhumanoid,
  ;
);

class CHARACTER
(
  mistress,
  humanoid,
  ;
);

class CHARACTER
(
  werewolfhuman,
  humanoid,
  ;
);

class CHARACTER
(
  werewolfwolf,
  humanoid,
  ;
);

class CHARACTER
(
  kobold,
  humanoid,
  ;
);

class CHARACTER
(
  gibberling,
  humanoid,
  ;
);

class CHARACTER  
(        
  largecat,
  nonhumanoid,
 public:
  virtual bool Catches(item*, float);
  ;
);

class CHARACTER  
(        
  largerat,
  nonhumanoid,
  ;
);

class CHARACTER  
(        
  angel,
  humanoid,
 public:
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool AttachBodyPartsOfFriendsNear(); 
  void SetHealTimer(ushort What) { HealTimer = What; }
  ushort GetHealTimer() const { return HealTimer; }
  virtual void CreateBodyParts(ushort);
  virtual bool BodyPartIsVital(ushort Index) const { return Index == TORSO_INDEX || Index == HEAD_INDEX; }
  virtual ushort GetAttribute(ushort) const;
  virtual ulong GetBaseEmitation() const;
  virtual bool CanCreateBodyPart(ushort) const;
 protected:
  virtual ushort GetTorsoMainColor() const;
  virtual ushort GetArmMainColor() const;
  virtual void VirtualConstructor(bool);
  virtual void CreateInitialEquipment(ushort);
  virtual void CreateCorpse() { SendToHell(); }
  virtual void AddPostFix(std::string& String) const { AddDivineMasterDescription(String, GetConfig()); }
  virtual void GetAICommand();
  ushort HealTimer;
);

class CHARACTER
(
  kamikazedwarf,
  humanoid,
 public:
  virtual float GetTimeToKill(const character*, bool) const;
  virtual bool Hit(character*, bool = false);
  virtual bool CheckForUsefulItemsOnGround() { return false; }
  virtual void GetAICommand();
  virtual void CreateInitialEquipment(ushort);
  virtual void DrawBodyParts(bitmap*, vector2d, ulong, bool, bool) const;
 protected:
  virtual ushort GetTorsoMainColor() const;
  virtual ushort GetArmMainColor() const;
  virtual ushort GetLegMainColor() const;
);

class CHARACTER  
(        
  mammoth,
  nonhumanoid,
  ;
);

class CHARACTER
(
  unicorn,
  nonhumanoid,
 public:
  virtual bool SpecialEnemySightedReaction(character*);
);

class CHARACTER
(
  genie,
  humanoid,
 public:
  virtual void CreateBodyParts(ushort);
  virtual bool BodyPartIsVital(ushort Index) const { return Index == TORSO_INDEX || Index == HEAD_INDEX; }
  virtual ushort GetAttribute(ushort) const;
  virtual bool CanCreateBodyPart(ushort) const;
 protected:
  virtual void CreateCorpse() { SendToHell(); }
);

class CHARACTER
(
  lion,
  nonhumanoid,
  ;
);

class CHARACTER
(
  carnivorousplant,
  nonhumanoid,
 protected:
  virtual ushort GetTorsoSpecialColor() const;
  virtual void GetAICommand();
);

class CHARACTER
(
  buffalo,
  nonhumanoid,
  ;
);

class CHARACTER
(
  snake,
  nonhumanoid,
 protected:
  virtual bool SpecialBiteEffect(character*, uchar, uchar, bool);
);

class CHARACTER
(
  orc,
  humanoid,
  ;
);

class CHARACTER
(
  cossack,
  humanoid,
  ;
);

class CHARACTER
(
  bananagrower,
  humanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void BeTalkedTo();
  virtual std::string& ProcessMessage(std::string&) const;
  virtual std::string GetProfessionDescription() const;
 protected:
  virtual bool HandleCharacterBlockingTheWay(character*);
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  uchar Profession;
  bool HasBeenOnLandingSite;
);

class CHARACTER
(
  imperialist,
  humanoid,
 public:
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  smith,
  humanoid,
 public:
  virtual void BeTalkedTo();
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  genetrixvesana,
  carnivorousplant,
 protected:
  virtual ushort GetTorsoSpecialColor() const;
  virtual void GetAICommand();
);

class CHARACTER
(
  ostrich,
  nonhumanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual bool HandleCharacterBlockingTheWay(character*);
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  bool HasBeenOnLandingSite;
);

class CHARACTER
(
  elder,
  humanoid,
 protected:
  virtual void CreateBodyParts(ushort);
);

class CHARACTER
(
  encourager,
  humanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  ulong LastHit;
);

#endif
