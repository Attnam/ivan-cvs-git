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
  friend class rightarm;
  friend class leftarm;
  humanoid(const humanoid&);
  virtual ~humanoid();
  virtual bool CanWield() const;
  virtual bool Hit(character*, vector2d, int, bool = false);
  virtual int GetSize() const;
  head* GetHead() const { return static_cast<head*>(*BodyPartSlot[HEAD_INDEX]); }
  arm* GetRightArm() const { return static_cast<arm*>(*BodyPartSlot[RIGHT_ARM_INDEX]); }
  arm* GetLeftArm() const { return static_cast<arm*>(*BodyPartSlot[LEFT_ARM_INDEX]); }
  groin* GetGroin() const { return static_cast<groin*>(*BodyPartSlot[GROIN_INDEX]); }
  leg* GetRightLeg() const { return static_cast<leg*>(*BodyPartSlot[RIGHT_LEG_INDEX]); }
  leg* GetLeftLeg() const { return static_cast<leg*>(*BodyPartSlot[LEFT_LEG_INDEX]); }
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
  virtual bool ReceiveDamage(character*, int, int, int = ALL, int = 8, bool = false, bool = false, bool = false, bool = true);
  virtual bool BodyPartIsVital(int) const;
  virtual bool BodyPartCanBeSevered(int) const;
  virtual item* GetMainWielded() const;
  virtual item* GetSecondaryWielded() const;
  virtual const char* GetEquipmentName(int) const;
  virtual bodypart* GetBodyPartOfEquipment(int) const;
  virtual item* GetEquipment(int) const;
  virtual int GetEquipmentSlots() const { return 13; }
  virtual void SwitchToDig(item*, vector2d);
  virtual int GetUsableLegs() const;
  virtual int GetUsableArms() const;
  virtual bool CheckKick() const;
  virtual int OpenMultiplier() const;
  virtual int CloseMultiplier() const;
  virtual bool CheckThrow() const;
  virtual bool CheckOffer() const;
  virtual sorter EquipmentSorter(int) const;
  virtual void SetEquipment(int, item*);
  virtual void DrawSilhouette(bitmap*, vector2d, bool) const;
  virtual int GetGlobalResistance(int) const;
  virtual bool TryToRiseFromTheDead();
  virtual bool HandleNoBodyPart(int);
  virtual void Kick(lsquare*, int, bool = false);
  virtual double GetTimeToKill(const character*, bool) const;
  virtual int GetAttribute(int, bool = true) const;
  virtual bool EditAttribute(int, int);
  virtual void EditExperience(int, double, double);
  virtual int DrawStats(bool) const;
  virtual void Bite(character*, vector2d, int, bool = false);
  virtual int GetCarryingStrength() const;
  virtual int GetRandomStepperBodyPart() const;
  virtual int CheckForBlock(character*, item*, double, int, int, int);
  virtual bool AddSpecialSkillInfo(felist&) const;
  virtual bool CheckBalance(double);
  virtual long GetMoveAPRequirement(int) const;
  virtual vector2d GetEquipmentPanelPos(int) const;
  virtual bool EquipmentEasilyRecognized(int) const;
  sweaponskill* GetCurrentRightSWeaponSkill() const { return CurrentRightSWeaponSkill; }
  void SetCurrentRightSWeaponSkill(sweaponskill* What) { CurrentRightSWeaponSkill = What; }
  sweaponskill* GetCurrentLeftSWeaponSkill() const { return CurrentLeftSWeaponSkill; }
  void SetCurrentLeftSWeaponSkill(sweaponskill* What) { CurrentLeftSWeaponSkill = What; }
  virtual void SWeaponSkillTick();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SignalEquipmentAdd(int);
  virtual void SignalEquipmentRemoval(int);
  virtual void DrawBodyParts(bitmap*, vector2d, color24, int, bool, bool = true) const;
  virtual bool CanUseStethoscope(bool) const;
  virtual bool IsUsingArms() const;
  virtual bool IsUsingLegs() const;
  virtual bool IsUsingHead() const;
  virtual void CalculateBattleInfo();
  virtual void CalculateBodyParts();
  virtual void CalculateAllowedWeaponSkillCategories();
  virtual bool HasALeg() const { return HasAUsableLeg(); }
  virtual void AddSpecialEquipmentInfo(festring&, int) const;
  virtual void CreateInitialEquipment(int);
  virtual festring GetBodyPartName(int, bool = false) const;
  virtual void CreateBlockPossibilityVector(blockvector&, double) const;
  virtual item* SevereBodyPart(int, bool = false, stack* = 0);
  virtual int GetSWeaponSkillLevel(const item*) const;
  virtual bool UseMaterialAttributes() const;
  virtual void CalculateDodgeValue();
  virtual bool CheckZap();
  virtual bool IsHumanoid() const { return true; }
  virtual bool CheckTalk();
  virtual bool CheckIfEquipmentIsNotUsable(int) const;
  virtual void AddSpecialStethoscopeInfo(felist&) const;
  virtual item* GetPairEquipment(int) const;
  virtual bool HasHead() const { return !!GetHead(); }
  virtual const festring& GetStandVerb() const;
  virtual head* Behead();
  virtual void AddAttributeInfo(festring&) const;
  virtual void AddAttackInfo(felist&) const;
  virtual void AddDefenceInfo(felist&) const;
  virtual void DetachBodyPart();
  virtual int GetRandomApplyBodyPart() const;
  void EnsureCurrentSWeaponSkillIsCorrect(sweaponskill*&, const item*);
  virtual int GetSumOfAttributes() const;
  virtual bool CheckConsume(const festring&) const;
  virtual bool CanConsume(material*) const;
  virtual bool PreProcessForBone();
  virtual void FinalProcessForBone();
  virtual void StayOn(liquid*);
  virtual head* GetVirtualHead() const { return GetHead(); }
  virtual character* CreateZombie() const;
  virtual void LeprosyHandler();
  virtual void DropRandomNonVitalBodypart();
  virtual void DropBodyPart(int);
  virtual void DuplicateEquipment(character*, ulong);
  bool IsImmuneToLeprosy() const;
  virtual int GetAttributeAverage() const;
  virtual bool CanVomit() const;
  virtual bool CheckApply() const;
  virtual bool CanForceVomit() const { return TorsoIsAlive() && HasAUsableArm(); }
  virtual bool IsTransparent() const;
  virtual void ModifySituationDanger(double&) const;
  virtual int RandomizeTryToUnStickBodyPart(ulong) const;
  virtual bool AllowUnconsciousness() const;
  //virtual bool IsTooHurtToRegainConsciousness() const;
  virtual bool CanChokeOnWeb(web*) const;
  virtual bool BrainsHurt() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBodyPartBitmapPos(int, bool = false) const;
  virtual color16 GetBodyPartColorB(int, bool = false) const;
  virtual color16 GetBodyPartColorC(int, bool = false) const;
  virtual color16 GetBodyPartColorD(int, bool = false) const;
  virtual bool BodyPartColorBIsSparkling(int, bool = false) const;
  virtual bool BodyPartColorCIsSparkling(int, bool = false) const;
  virtual bool BodyPartColorDIsSparkling(int, bool = false) const;
  virtual long GetBodyPartSize(int, int) const;
  virtual long GetBodyPartVolume(int) const;
  virtual bodypart* MakeBodyPart(int) const;
  virtual const festring& GetDeathMessage() const;
  virtual vector2d GetDrawDisplacement(int) const { return vector2d(0, 0); }
  bool HasAUsableArm() const;
  bool HasAUsableLeg() const;
  bool HasTwoUsableLegs() const;
  bool CanAttackWithAnArm() const;
  bool RightArmIsUsable() const;
  bool LeftArmIsUsable() const;
  bool RightLegIsUsable() const;
  bool LeftLegIsUsable() const;
  std::list<sweaponskill*> SWeaponSkill;
  sweaponskill* CurrentRightSWeaponSkill;
  sweaponskill* CurrentLeftSWeaponSkill;
  static int DrawOrder[];
);

class CHARACTER
(
  playerkind,
  humanoid,
 public:
  playerkind(const playerkind&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetSoulID(ulong);
  virtual bool SuckSoul(character*);
  virtual bool TryToRiseFromTheDead();
  virtual void FinalProcessForBone();
  virtual void BeTalkedTo();
  virtual bool IsHuman() const { return true; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual bodypart* MakeBodyPart(int) const;
  ulong SoulID;
  bool IsBonePlayer;
  bool IsClone;
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
  virtual void FinalProcessForBone();
  virtual bool MoveTowardsHomePos();
 protected:
  virtual void VirtualConstructor(bool);
  virtual void CreateCorpse(lsquare*);
  virtual void GetAICommand();
  ulong LastHealed;
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
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void GetAICommand();
  virtual void SetWayPoints(const fearray<packedvector2d>&);
  virtual bool MoveTowardsHomePos();
 protected:
  virtual void VirtualConstructor(bool);
  std::vector<vector2d> WayPoints;
  uint NextWayPoint;
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
 public:
  virtual void Bite(character*, vector2d, int, bool = false);
  virtual void GetAICommand();
 protected:
  virtual const char* FirstPersonBiteVerb() const;
  virtual const char* FirstPersonCriticalBiteVerb() const;
  virtual const char* ThirdPersonBiteVerb() const;
  virtual const char* ThirdPersonCriticalBiteVerb() const;
  virtual const char* BiteNoun() const;
  void CallForMonsters();
);

class CHARACTER
(
  darkknight,
  humanoid,
 protected:
  virtual int ModifyBodyPartHitPreference(int, int) const;
  virtual int ModifyBodyPartToHitChance(int, int) const;
  virtual bool CanPanicFromSeveredBodyPart() const { return false; }
  virtual void SpecialBodyPartSeverReaction();
);

class CHARACTER
(
  ennerbeast,
  humanoid,
 public:
  virtual bool Hit(character*, vector2d, int, bool = false);
  virtual bool MustBeRemovedFromBone() const;
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
  virtual bool AttackIsBlockable(int) const { return false; }
);

class CHARACTER
(
  skeleton,
  humanoid,
 public:
  virtual void BeTalkedTo();
  virtual item* SevereBodyPart(int, bool = false, stack* = 0);
  virtual bool BodyPartIsVital(int) const;
  virtual long GetBodyPartVolume(int) const;
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
  virtual bool CheckForUsefulItemsOnGround(bool = true) { return false; }
  virtual void BeTalkedTo();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetItemVolume(long What) { ItemVolume = What; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual bool AddAdjective(festring&, bool) const;
  virtual material* CreateBodyPartMaterial(int, long) const;
  virtual void CreateCorpse(lsquare*);
  long ItemVolume;
);

class CHARACTER
(
  communist,
  humanoid,
 public:
  virtual bool MoveRandomly();
  virtual void BeTalkedTo();
  virtual bool BoundToUse(const item*, int) const;
  virtual bool MustBeRemovedFromBone() const;
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
  virtual void CreateBodyParts(int);
);

class CHARACTER
(
  slave,
  playerkind,
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
  virtual int GetHairColor() const;
  virtual vector2d GetHeadBitmapPos() const;
);

class CHARACTER
(
  femaleslave,
  humanoid,
 public:
  virtual void BeTalkedTo();
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
  virtual bool BodyPartIsVital(int) const;
  virtual void CreateBodyParts(int);
  virtual bool AllowSpoil() const { return true; }
  void SetDescription(const festring What) { Description = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual festring GetZombieDescription() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual void AddPostFix(festring&) const;
  virtual void GetAICommand();
  virtual bool AllowExperience() const { return false; }
  festring Description;
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
  virtual int TakeHit(character*, item*, bodypart*, vector2d, double, double, int, int, int, bool, bool);
  virtual bool ReceiveDamage(character*, int, int, int = ALL, int = 8, bool = false, bool = false, bool = false, bool = true);
  virtual bool AllowEquipment(const item*, int) const;
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
 public:
  virtual festring GetKillName() const;
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
  virtual bool BodyPartIsVital(int) const;
  virtual int GetAttribute(int, bool = true) const;
  virtual color24 GetBaseEmitation() const;
  virtual bool CanCreateBodyPart(int) const;
  virtual const festring& GetStandVerb() const { return character::GetStandVerb(); }
  virtual void FinalProcessForBone();
  virtual void CreateInitialEquipment(int);
 protected:
  virtual int GetTorsoMainColor() const;
  virtual int GetArmMainColor() const;
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  ulong LastHealed;
);

class CHARACTER
(
  kamikazedwarf,
  humanoid,
 public:
  virtual bool Hit(character*, vector2d, int, bool = false);
  virtual bool CheckForUsefulItemsOnGround(bool = true) { return false; }
  virtual void GetAICommand();
  virtual void CreateInitialEquipment(int);
  virtual bool IsKamikazeDwarf() const { return true; }
 protected:
  virtual int GetTorsoMainColor() const;
  virtual int GetGauntletColor() const;
  virtual int GetLegMainColor() const;
  virtual vector2d GetDrawDisplacement(int) const;
  virtual int GetWSkillHits() const { return 10000; }
  virtual bool IsElite() const { return false; }
);

class CHARACTER
(
  genie,
  humanoid,
 public:
  virtual bool BodyPartIsVital(int) const;
  virtual int GetAttribute(int, bool = true) const;
  virtual bool CanCreateBodyPart(int) const;
  virtual const festring& GetStandVerb() const { return character::GetStandVerb(); }
);

class CHARACTER
(
  orc,
  humanoid,
 protected:
  virtual void VirtualConstructor(bool);
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
  virtual festring& ProcessMessage(festring&) const;
  virtual bool IsBananaGrower() const { return true; }
  festring GetProfession() const { return Profession; }
 protected:
  virtual bool HandleCharacterBlockingTheWay(character*, vector2d, int);
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  void RandomizeProfession();
  festring Profession;
  bool HasDroppedBananas;
  bool FeedingSumo;
);

class CHARACTER
(
  imperialist,
  humanoid,
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void BeTalkedTo();
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
  virtual void GetAICommand();
  virtual void CreateBodyParts(int);
);

class CHARACTER
(
  encourager,
  humanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void FinalProcessForBone();
 protected:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  ulong LastHit;
);

class CHARACTER
(
  darkmage,
  humanoid,
 public:
  virtual void GetAICommand();
 protected:
  int GetSpellAPCost() const;
);

class CHARACTER
(
  necromancer,
  humanoid,
 public:
  virtual void GetAICommand();
  virtual bool TryToRaiseZombie();
  virtual void RaiseSkeleton();
 protected:
  int GetSpellAPCost() const;
);

class CHARACTER
(
  sumowrestler,
  humanoid,
 public:
  virtual void BeTalkedTo();
  virtual bool CheckIfSatiated() { return GetNP() > BLOATED_LEVEL; }
  virtual bool IsSumoWrestler() const { return true; }
  virtual bool CanUseEquipment(int) const;
 protected:
  virtual void GetAICommand();
);

class CHARACTER
(
  tourist,
  humanoid,
 public:
  virtual character* GetLeader() const;
 protected:
  virtual void GetAICommand();
);

class CHARACTER
(
  veterankamikazedwarf,
  kamikazedwarf,
 protected:
  virtual void VirtualConstructor(bool);
  virtual int GetTorsoMainColor() const;
  virtual int GetGauntletColor() const;
  virtual int GetLegMainColor() const;
  virtual int GetWSkillHits() const { return 50000; }
  virtual bool IsElite() const { return true; }
);

class CHARACTER  
(        
  archangel,
  angel,
 public:
  virtual void CreateInitialEquipment(int);
 protected:
  virtual int GetTorsoMainColor() const;
  virtual int GetArmMainColor() const;
);

class CHARACTER
(
  tailor,
  humanoid,
 public:
  virtual void BeTalkedTo();
  virtual void GetAICommand() { StandIdleAI(); }
);

#endif
