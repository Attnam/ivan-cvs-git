#ifndef __HUMAN_H__
#define __HUMAN_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "char.h"

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
  virtual bool BodyPartIsVital(ushort) const;
  virtual bool BodyPartCanBeSevered(ushort) const;
  virtual item* GetMainWielded() const;
  virtual item* GetSecondaryWielded() const;
  virtual void SetMainWielded(item*);
  virtual void SetSecondaryWielded(item*);
  virtual std::string GetEquipmentName(ushort) const;
  virtual bodypart* GetBodyPartOfEquipment(ushort) const;
  virtual item* GetEquipment(ushort) const;
  virtual ushort GetEquipmentSlots() const { return 13; }
  virtual void SwitchToDig(item*, vector2d);
  virtual uchar GetLegs() const;
  virtual uchar GetArms() const;
  virtual bool CheckKick() const;
  virtual uchar OpenMultiplier() const;
  virtual uchar CloseMultiplier() const;
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
  virtual ushort GetCarryingStrength() const;
  virtual ushort GetRandomStepperBodyPart() const;
  virtual ushort CheckForBlock(character*, item*, float, ushort, short, uchar);
  virtual bool AddSpecialSkillInfo(felist&) const;
  virtual bool CheckBalance(float);
  virtual long GetMoveAPRequirement(uchar) const;
  virtual void DetachBodyPart();
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
  virtual void DrawBodyParts(bitmap*, vector2d, ulong, bool) const;
  virtual bool CanUseStethoscope(bool) const;
  virtual bool IsUsingArms() const;
  virtual bool IsUsingLegs() const;
  virtual bool IsUsingHead() const;
  virtual void AddAttackInfo(felist&) const;
  virtual void CalculateBattleInfo();
  leg* GetKickLeg() const;
  virtual void CalculateBodyParts();
  virtual void CalculateAllowedWeaponSkillCategories();
  virtual bool HasFeet() const;
  virtual void AddSpecialEquipmentInfo(std::string&, ushort) const;
  virtual void CreateInitialEquipment(ushort);
  virtual std::string GetBodyPartName(ushort, bool = false) const;
  virtual void CreateBlockPossibilityVector(blockvector&, float) const;
  void CheckIfSWeaponSkillRemovalNeeded(sweaponskill*);
  virtual item* SevereBodyPart(ushort);
  virtual void AddDefenceInfo(felist&) const;
  virtual uchar GetSWeaponSkillLevel(const item*) const;
  virtual bool IsAlive() const;
  virtual void ShowBattleInfo();
  virtual void AddSpecialMovePenaltyInfo(felist&) const;
  virtual void CalculateDodgeValue();
  virtual bool CheckZap();
  virtual void AddAttributeInfo(std::string&) const;
  virtual bool IsHumanoid() const { return true; }
  virtual bool CheckTalk();
  virtual bool CanCreateBodyPart(ushort) const { return true; }
  virtual bool CheckIfEquipmentIsNotUsable(ushort) const;
  virtual void AddSpecialStethoscopeInfo(felist&) const;
  virtual item* GetPairEquipment(ushort) const;
  virtual bool HasHead() const { return GetHead() != 0; }
  virtual const std::string& GetStandVerb() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBodyPartBitmapPos(ushort);
  virtual ushort GetBodyPartColorB(ushort);
  virtual ushort GetBodyPartColorC(ushort);
  virtual ushort GetBodyPartColorD(ushort);
  virtual ulong GetBodyPartSize(ushort, ushort);
  virtual ulong GetBodyPartVolume(ushort) const;
  virtual bodypart* MakeBodyPart(ushort) const;
  virtual const std::string& GetDeathMessage() const;
  std::vector<sweaponskill*> SWeaponSkill;
  sweaponskill* CurrentRightSWeaponSkill;
  sweaponskill* CurrentLeftSWeaponSkill;
);

class CHARACTER
(
  human,
  humanoid,
 protected:
  virtual void VirtualConstructor(bool);
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
 protected:
  virtual void VirtualConstructor(bool);
  virtual void CreateCorpse(lsquare*);
  virtual void GetAICommand();
  ushort LastHealed;
);

class CHARACTER
(
  farmer,
  humanoid,
 protected:
  virtual vector2d GetHeadBitmapPos() const;
  virtual vector2d GetRightArmBitmapPos() const;
  virtual vector2d GetLeftArmBitmapPos() const { return GetRightArmBitmapPos(); }
);

class CHARACTER
(
  guard,
  humanoid,
 public:
  virtual ~guard();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void GetAICommand();
  virtual void SetWayPoints(const std::vector<vector2d>& What) { WayPoints = What; }
 protected:
  virtual void VirtualConstructor(bool);
  std::vector<vector2d> WayPoints;
  ushort NextWayPoint;
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
  virtual std::string FirstPersonBiteVerb() const;
  virtual std::string FirstPersonCriticalBiteVerb() const;
  virtual std::string ThirdPersonBiteVerb() const;
  virtual std::string ThirdPersonCriticalBiteVerb() const;
  virtual std::string BiteNoun() const;
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
  skeleton,
  humanoid,
 public:
  virtual void BeTalkedTo();
  virtual item* SevereBodyPart(ushort);
  virtual bool BodyPartIsVital(ushort) const;
  virtual ulong GetBodyPartVolume(ushort) const;
 protected:
  virtual void CreateCorpse(lsquare*);
);

class CHARACTER
(
  goblin,
  humanoid,
  ;
);

class CHARACTER
(
  golem,
  humanoid,
 public:
  virtual bool MoveRandomly();
  virtual bool CheckForUsefulItemsOnGround() { return false; }
 protected:
  virtual material* CreateBodyPartMaterial(ushort, ulong) const;
);

class CHARACTER
(
  communist,
  humanoid,
 public:
  virtual bool MoveRandomly();
  virtual void BeTalkedTo();
 protected:
  virtual bool ShowClassDescription() const;
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
  virtual bool SpecialEnemySightedReaction(character*);
);

class CHARACTER
(
  housewife,
  humanoid,
 public:
  virtual bool SpecialEnemySightedReaction(character*);
 protected:
  virtual ushort GetHairColor() const;
  virtual vector2d GetHeadBitmapPos() const;
);

class CHARACTER
(
  femaleslave,
  humanoid,
 public:
  virtual void DrawBodyParts(bitmap*, vector2d, ulong, bool) const;
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
  virtual bool BodyPartIsVital(ushort Index) const;
  virtual void CreateBodyParts(ushort);
  virtual item* SevereBodyPart(ushort);
 protected:
  virtual void CreateCorpse(lsquare*);
);

class CHARACTER
(
  imp,
  humanoid,
  ;
);

class CHARACTER
(
  mistress,
  humanoid,
 public:
  virtual ushort TakeHit(character*, item*, float, float, short, uchar, bool, bool);
  virtual bool ReceiveDamage(character*, ushort, uchar, uchar = ALL, uchar = 8, bool = false, bool = false, bool = false, bool = true);
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
  angel,
  humanoid,
 public:
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool AttachBodyPartsOfFriendsNear(); 
  virtual void CreateBodyParts(ushort);
  virtual bool BodyPartIsVital(ushort) const;
  virtual ushort GetAttribute(ushort) const;
  virtual ulong GetBaseEmitation() const;
  virtual bool CanCreateBodyPart(ushort) const;
  virtual const std::string& GetStandVerb() const { return character::GetStandVerb(); }
 protected:
  virtual ushort GetTorsoMainColor() const;
  virtual ushort GetArmMainColor() const;
  virtual void VirtualConstructor(bool);
  virtual void CreateInitialEquipment(ushort);
  virtual void CreateCorpse(lsquare*) { SendToHell(); }
  virtual void AddPostFix(std::string&) const;
  virtual void GetAICommand();
  ushort LastHealed;
);

class CHARACTER
(
  kamikazedwarf,
  humanoid,
 public:
  virtual bool Hit(character*, bool = false);
  virtual bool CheckForUsefulItemsOnGround() { return false; }
  virtual void GetAICommand();
  virtual void CreateInitialEquipment(ushort);
  virtual void DrawBodyParts(bitmap*, vector2d, ulong, bool) const;
 protected:
  virtual ushort GetTorsoMainColor() const;
  virtual ushort GetArmMainColor() const;
  virtual ushort GetLegMainColor() const;
);

class CHARACTER
(
  genie,
  humanoid,
 public:
  virtual void CreateBodyParts(ushort);
  virtual bool BodyPartIsVital(ushort) const;
  virtual ushort GetAttribute(ushort) const;
  virtual bool CanCreateBodyPart(ushort) const;
  virtual const std::string& GetStandVerb() const { return character::GetStandVerb(); }
 protected:
  virtual void CreateCorpse(lsquare*) { SendToHell(); }
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
