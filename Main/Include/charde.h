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
  virtual ~humanoid();
  /*virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);*/
  virtual bool CanWield() const;
  virtual bool Hit(character*);
  //virtual void CharacterSpeciality(ushort = 1);
  //virtual bool ShowWeaponSkills();
  //virtual long GetStatScore() const;
  virtual void AddSpecialItemInfo(std::string&, item*);
  virtual void AddSpecialItemInfoDescription(std::string&);
  //virtual void KickHit();
  virtual ushort GetSize() const;
  virtual head* GetHead() const;
  virtual void SetHead(head* What);
  virtual rightarm* GetRightArm() const;
  virtual void SetRightArm(rightarm* What);
  virtual leftarm* GetLeftArm() const;
  virtual void SetLeftArm(leftarm* What);
  virtual groin* GetGroin() const;
  virtual void SetGroin(groin* What);
  virtual rightleg* GetRightLeg() const;
  virtual void SetRightLeg(rightleg* What);
  virtual leftleg* GetLeftLeg() const;
  virtual void SetLeftLeg(leftleg* What);
  virtual humanoidtorso* GetHumanoidTorso() const;
  virtual void SetHumanoidTorso(humanoidtorso* What);
  virtual arm* GetMainArm() const;
  virtual arm* GetSecondaryArm() const;
  virtual void DrawToTileBuffer(bool) const;
  virtual bool ReceiveDamage(character*, short, uchar, uchar = ALL, uchar = 8, bool = false, bool = false, bool = false);
  virtual bool BodyPartVital(ushort);
  virtual bool BodyPartCanBeSevered(ushort) const;
  virtual item* GetMainWielded() const;
  virtual item* GetSecondaryWielded() const;
  virtual void SetMainWielded(item*);
  virtual void SetSecondaryWielded(item*);
  //virtual void RightHit(character*);
  //virtual void LeftHit(character*);
  virtual float CalculateRightAttackStrength() const;
  virtual float CalculateLeftAttackStrength() const;
  virtual float CalculateRightToHitValue() const;
  virtual float CalculateLeftToHitValue() const;
  virtual std::string EquipmentName(ushort) const;
  virtual bodypart* GetBodyPartOfEquipment(ushort) const;
  virtual item* GetEquipment(ushort) const;
  virtual ushort EquipmentSlots() const { return 13; }
  virtual item* GetHelmet() const;
  virtual item* GetAmulet() const;
  virtual item* GetCloak() const;
  virtual item* GetBodyArmor() const;
  virtual item* GetBelt() const;
  virtual item* GetRightWielded() const;
  virtual item* GetLeftWielded() const;
  virtual item* GetRightRing() const;
  virtual item* GetLeftRing() const;
  virtual item* GetRightGauntlet() const;
  virtual item* GetLeftGauntlet() const;
  virtual item* GetRightBoot() const;
  virtual item* GetLeftBoot() const;
  virtual void SwitchToDig(item*, vector2d);
  virtual uchar GetLegs() const;
  virtual uchar GetArms() const;
  virtual bool CheckKick() const;
  //virtual float GetAPStateMultiplier() const;
  virtual short GetLengthOfOpen(vector2d) const;
  virtual short GetLengthOfClose(vector2d) const;
  virtual bool CheckThrow() const;
  virtual bool CheckOffer() const;
  virtual void SetHelmet(item*);
  virtual void SetAmulet(item*);
  virtual void SetCloak(item*);
  virtual void SetBodyArmor(item*);
  virtual void SetBelt(item*);
  virtual void SetRightWielded(item*);
  virtual void SetLeftWielded(item*);
  virtual void SetRightRing(item*);
  virtual void SetLeftRing(item*);
  virtual void SetRightGauntlet(item*);
  virtual void SetLeftGauntlet(item*);
  virtual void SetRightBoot(item*);
  virtual void SetLeftBoot(item*);
  virtual bool (*EquipmentSorter(ushort) const)(item*, character*);
  virtual void SetEquipment(ushort, item*);
  virtual bool DrawSilhouette(bitmap*, vector2d) const;
  virtual ushort GlobalResistance(uchar) const;
  virtual void AddInfo(felist&) const;
  virtual void CompleteRiseFromTheDead();
  virtual bool HandleNoBodyPart(ushort);
  virtual bool CheckWearEquipment() const { return true; }
  virtual void Kick(lsquare*);
  //virtual float CalculateTotalAttackStrength() const;
  virtual float GetAttackStrengthDanger() const;
  virtual ushort GetAttribute(ushort) const;
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual ushort DrawStats() const;
  virtual void Bite(character*);
  virtual ushort GetCarryingStrength() const { return GetAttribute(LEGSTRENGTH); }
  virtual ushort GetRandomStepperBodyPart() const;
  virtual ushort CheckForBlock(character*, item*, float, ushort, short, uchar);
  virtual bool AddSpecialSkillInfo(felist&) const;
  virtual bool CheckBalance(float);
  virtual long CalculateMoveAPRequirement(long) const;
  virtual bool EquipmentHasNoPairProblems(ushort) const;
  virtual bool DetachBodyPart();
  virtual vector2d GetEquipmentPanelPos(ushort) const;
 protected:
  virtual vector2d GetBodyPartBitmapPos(ushort, ushort);
  virtual ushort GetBodyPartColor1(ushort, ushort);
  virtual ushort GetBodyPartColor2(ushort, ushort);
  virtual ushort GetBodyPartColor3(ushort, ushort);
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(HUMANFLESH, Volume); }
  virtual ulong GetBodyPartSize(ushort, ushort);
  virtual ulong GetBodyPartVolume(ushort);
  virtual uchar GetBodyPartBonePercentile(ushort);
  virtual bodypart* MakeBodyPart(ushort);
  virtual uchar BodyParts() const { return 7; }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " dies screaming."; }
  virtual uchar AllowedWeaponSkillCategories() const { return WEAPON_SKILL_CATEGORIES; }
);

class ABSTRACT_CHARACTER
(
  nonhumanoid,
  character,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual float CalculateUnarmedStrength() const;
  virtual float CalculateKickStrength() const;
  virtual float CalculateBiteStrength() const;
  virtual float CalculateUnarmedToHitValue() const;
  virtual float CalculateKickToHitValue() const;
  virtual float CalculateBiteToHitValue() const;
  virtual void Kick(lsquare*);
  virtual void AddInfo(felist&) const;
  //virtual float CalculateTotalAttackStrength() const;
  virtual bool Hit(character*);
  virtual void UnarmedHit(character*);
  virtual void InitSpecialAttributes();
  virtual float GetAttackStrengthDanger() const;
  /*virtual ushort GetStrength() const;
  virtual ushort GetAgility() const;*/
  virtual void ApplyExperience();
  virtual ushort GetAttribute(ushort) const;
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual ushort DrawStats() const;
  virtual void Bite(character*);
  virtual bool RaiseStats();
  virtual bool LowerStats();
  virtual ushort GetCarryingStrength() const { return Strength; }
 protected:
  ushort Strength;
  ushort Agility;
  long StrengthExperience;
  long AgilityExperience;
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
  //virtual std::string NameSingular() const { return "human"; }
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
  virtual void HealFully(character*);
  virtual void SetHealTimer(ushort What) { HealTimer = What; }
  virtual ushort GetHealTimer() { return HealTimer; }
  virtual void Save(outputfile&) const;
  virtual uchar GetStoryState() const { return StoryState; }
  virtual void SetStoryState(uchar What) { StoryState = What; }
  virtual void CreateInitialEquipment();
 protected:
  virtual void VirtualConstructor(bool);
  //virtual std::string NameSingular() const { return "High Priest of the Great Frog"; }
  virtual void CreateCorpse();
  virtual std::string GetDeathMessage() { return "The High Priest disappears in a bright light and only his left nut is left behind."; }
  virtual void GetAICommand();
  ushort HealTimer;
  uchar StoryState;
);

class CHARACTER
(
  farmer,
  humanoid,
 public:
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort GetHairColor(ushort Frame) const { return RAND() % 2 ? humanoid::GetHairColor(Frame) : MAKE_RGB(160, 160, 160); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, (4 + RAND() % 2) * 16); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, (RAND() % 2) * 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0,(RAND() % 3) * 16); }
  //virtual std::string NameSingular() const { return "farmer"; }
);

class CHARACTER
(
  guard,
  humanoid,
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort ClothColor() const { return MAKE_RGB(0, 128, 128); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 112); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 128); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 80); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 64); }
  virtual ulong TotalVolume() const { return 60000; }
  //virtual std::string NameSingular() const { return "guard"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 180; }
);

class CHARACTER
(
  shopkeeper,
  humanoid,
  /*{
    SetMoney(3000 + RAND() % 2001);
  },*/
 public:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 96, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 64); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 32); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 32); }
  virtual ulong TotalVolume() const { return 100000; }
  //virtual std::string NameSingular() const { return "shopkeeper"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 160; }
);

class CHARACTER
(
  priest,
  humanoid,
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
  virtual bool CanSwim() const { return true; }
 protected:
  virtual ushort CapColor() const { return MAKE_RGB(180, 0, 80); }
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 96, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 128); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 32); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 32); }
  virtual ulong TotalVolume() const { return 100000; }
  //virtual std::string NameSingular() const { return "priest"; }
  virtual ushort TotalSize() const { return 180; }
);

class CHARACTER
(
  oree,
  humanoid,
 public:
  virtual bool Charmable() const { return false; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual void CreateInitialEquipment();
  virtual bool CanBeAssignedAName() const { return false; }
  virtual bool IsUnique() const { return true; }
 protected:
  virtual void VirtualConstructor(bool);

  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 32); } 
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 160); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(80, 160); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 144); }

  virtual ushort ClothColor() const { return MAKE_RGB(30, 30, 30); }
  //virtual ushort SkinColor() const { return MAKE_RGB(160, 0, 0); }
  virtual ushort EyeColor() const { return MAKE_RGB(64, 0, 0); }

  virtual ulong TotalVolume() const { return 110000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DAEMONFLESH, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + "vomits blood for one last time and then dies."; }
  //virtual std::string NameSingular() const { return "Blood Daemon King"; }
  //virtual float GetMeleeStrength() const { return 30000; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBloodVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBloodVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBloodVerb(Critical); }*/
  virtual std::string FirstPersonBiteVerb() const { return "vomit acidous blood at"; }
  virtual std::string FirstPersonCriticalBiteVerb() const { return "vomit very acidous blood at"; }
  virtual std::string ThirdPersonBiteVerb() const { return "vomits acidous blood at"; }
  virtual std::string ThirdPersonCriticalBiteVerb() const { return "vomits very acidous blood at"; }
  virtual std::string BiteNoun() const { return "liquid"; }
  virtual ushort TotalSize() const { return 225; }
);

class CHARACTER
(
  darkknight,
  humanoid,
 public:
  //static bool CanBeGenerated() { return true; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 240); } 
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 176); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 64); }
  virtual ushort ClothColor() const { return MAKE_RGB(0,0,30); }
  virtual ushort EyeColor() const { return MAKE_RGB(100, 0, 0); }
  virtual ulong TotalVolume() const { return 110000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(128,0); }
  //virtual std::string NameSingular() const { return "dark knight"; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual ushort TotalSize() const { return 200; }
);

class CHARACTER
(
  ennerbeast,
  humanoid,
 public:
  virtual bool CanOpen() const { return false; }
  //static bool CanBeGenerated() { return false; }
  virtual bool Hit(character*);
  virtual void BeTalkedTo(character*);
  virtual ulong CurrentDanger() const { return character::CurrentDanger() * 5; }
  virtual ulong MaxDanger() const { return character::MaxDanger() * 5; }
 protected:
  //virtual void CreateHead();
  //virtual std::string GetArticle() const { return "an"; }
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(ENNERBEASTFLESH, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " dies and the world is finally freed from this terrible monster."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(96,0); }
  //virtual std::string NameSingular() const { return "enner beast"; }
  //virtual float GetMeleeStrength() const { return 100000; }
  virtual void GetAICommand();
  virtual ushort TotalSize() const { return 150; }
);

class ABSTRACT_CHARACTER
(
  frog,
  nonhumanoid,
 public:
  virtual bool CanOpen() const { return false; }
 protected:
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "croaks"; }
);

class CHARACTER
(
  darkfrog,
  frog,
 public:
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual bool HasInfraVision() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 2500; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(FROGFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(80,0); }
  //virtual std::string NameSingular() const { return "dark frog"; }
  virtual ushort TotalSize() const { return 25; }
);

class CHARACTER
(
  elpuri,
  darkfrog,
 public:
  virtual void VirtualConstructor(bool);
  //static bool CanBeGenerated() { return false; }
  virtual bool Charmable() const { return false; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual void BeTalkedTo(character*);
  virtual bool Hit(character*);
  virtual bool CanBeDisplaced() const { return false; }
  virtual bool CanBeAssignedAName() const { return false; }
  virtual bool CanOpen() const { return true; }
  virtual bool IsUnique() const { return true; }
 protected:
  virtual void CreateCorpse();
  virtual ulong TotalVolume() const { return 277500; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(ELPURIFLESH, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " groans horribly and drops " + PossessivePronoun() + " head."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(64,0); }
  //virtual std::string NameSingular() const { return "Master Dark Frog"; }
  virtual ushort TotalSize() const { return 300; }
);

class CHARACTER
(
  billswill,
  nonhumanoid,
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual std::string StandVerb() const { return "floating"; }
  //static ushort Frequency() { return 100; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 500000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new gas(AIR, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(48,0); }
  //virtual std::string NameSingular() const { return "pure mass of Bill's will"; }
  //virtual std::string NamePlural() const { return "pure masses of Bill's will"; }
  virtual void CreateCorpse() { SendToHell(); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " vanishes from existence."; }
  //virtual float GetMeleeStrength() const { return 20000; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonPSIVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonPSIVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonPSIVerb(Critical); }*/
  virtual std::string FirstPersonUnarmedHitVerb() const { return "emit psi waves at"; }
  virtual std::string FirstPersonCriticalUnarmedHitVerb() const { return "emit powerful psi waves at"; }
  virtual std::string ThirdPersonUnarmedHitVerb() const { return "emits psi waves at"; }
  virtual std::string ThirdPersonCriticalUnarmedHitVerb() const { return "emits powerful psi waves at"; }
  virtual ushort TotalSize() const { return 100; }
  virtual bool AttackIsBlockable(uchar) const { return false; }
);

class CHARACTER
(
  skeleton,
  humanoid,
 public:
  //static bool CanBeGenerated() { return true; }
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
  //virtual uchar GetSex() const { return MALE + RAND() % 2; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(144, 144, 144); }
  virtual ushort EyeColor() const { return MAKE_RGB(100, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(111,74, 37); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 208);}
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 96); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 96); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 80); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " is transformed into a crumpled heap of bones."; }
  //virtual std::string NameSingular() const { return "skeleton"; }
  virtual void CreateCorpse();
  //virtual float GetMeleeStrength() const { return 1000; }
  virtual ushort TotalSize() const { return 150; }
);

class CHARACTER
(
  goblin,
  humanoid,
 public:
  //static bool CanBeGenerated() { return true; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
  virtual uchar GetSex() const { return UNDEFINED; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(0, 128, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(111,74,37); } 
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 48); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32,112); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 144); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 64); }
  virtual ulong TotalVolume() const { return 25000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(GOBLINOIDFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(144,0); }
  //virtual std::string NameSingular() const { return "goblin"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 100; }
);

class ABSTRACT_CHARACTER
(
  mommo,
  nonhumanoid,
 public:
  virtual std::string StandVerb() const { return "bubbling"; }
  virtual bool CanOpen() const { return false; }
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(BROWNSLIME, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " turns into lifeless goo."; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBrownSlimeVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBrownSlimeVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBrownSlimeVerb(Critical); }*/
  virtual std::string FirstPersonBiteVerb() const { return "vomit acidous slime at"; }
  virtual std::string FirstPersonCriticalBiteVerb() const { return "vomit very acidous slime at"; }
  virtual std::string ThirdPersonBiteVerb() const { return "vomits acidous slime at"; }
  virtual std::string ThirdPersonCriticalBiteVerb() const { return "vomits very acidous slime at"; }
  virtual std::string BiteNoun() const { return "slime"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "vibrates oddly"; }
);

class CHARACTER
(
  golem,
  humanoid,
  /*{
    SetAgility(5);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(12);
  },*/
 public:
  virtual bool MoveRandomly();
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual bool CheckForUsefulItemsOnGround() { return false; }
 protected:
  virtual ulong TotalVolume() const { return 100000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong) const;
  virtual std::string GetDeathMessage() { return "The Holy Words of " + GetName(DEFINITE) + " fly away and the monster collapses."; }
  //virtual std::string NameSingular() const { return "golem"; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(256,0); }
  //virtual float GetMeleeStrength() const;
  virtual ushort TotalSize() const { return 250; }
  virtual bool ShowMaterial() const { return true; }
);

class CHARACTER
(
  wolf,
  nonhumanoid,
  /*{
    SetAgility(20);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(24);
  },*/
 protected:
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(WOLFFLESH, Volume); }
  //virtual std::string NameSingular() const { return "wolf"; }
  //virtual std::string NamePlural() const { return "wolves"; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(224,0); }
  //virtual float GetMeleeStrength() const { return 7500; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  virtual std::string TalkVerb() const { return "howls"; }
  virtual ushort TotalSize() const { return 100; }
);

class CHARACTER
(
  dog,
  nonhumanoid,
  /*{
    SetAgility(15);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },*/
 public:
  virtual bool Catches(item*, float);
 protected:
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DOGFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(240,0); }
  //virtual std::string NameSingular() const { return "dog"; }
  //virtual float GetMeleeStrength() const { return 5000; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  virtual std::string TalkVerb() const { return "barks"; }
  virtual ushort TotalSize() const { return 70; }
);

class CHARACTER
(
  spider,
  nonhumanoid,
  /*{
    SetAgility(5);
    SetStrength(2);
    SetEndurance(1);
    SetPerception(9);
  },*/
 public:
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanOpen() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 100; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(SPIDERFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(272,0); }
  //virtual std::string NameSingular() const { return "spider"; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  //virtual float GetMeleeStrength() const { return 10000; }
  virtual void CreateCorpse() { SendToHell(); }
  virtual std::string TalkVerb() const { return "says nothing"; }
  virtual ushort TotalSize() const { return 10; }
);

class CHARACTER
(
  jackal,
  nonhumanoid,
  /*{
    SetAgility(10);
    SetStrength(3);
    SetEndurance(3);
    SetPerception(18);
  },*/
 protected:
  virtual ulong TotalVolume() const { return 20000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(JACKALFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(304,0); }
  //virtual std::string NameSingular() const { return "jackal"; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }                                                                                                                                                                                                                                                                                          //Jackals are unoriginal
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "howls"; }
  virtual ushort TotalSize() const { return 80; }
);

class CHARACTER
(
  donkey,
  nonhumanoid,
  /*{
    SetAgility(5);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(15);
  },*/
 protected:
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DONKEYFLESH, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " neighs one last time and then dies."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(288,0); }
  //virtual std::string NameSingular() const { return "mutant donkey"; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  //virtual float GetMeleeStrength() const { return 1000; }
  virtual std::string TalkVerb() const { return "neighs"; }
  virtual ushort TotalSize() const { return 150; }
);

class CHARACTER
(
  communist,
  humanoid,
  /*{
    SetAgility(20);
    SetStrength(50);
    SetEndurance(50);
    SetPerception(18);
    SetAssignedName("Ivan");
  },*/
 public:
  virtual bool MoveRandomly();
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual uchar CriticalModifier() const { return 4; }
  virtual bool CanBeAssignedAName() const { return false; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort BeltColor() const { return MAKE_RGB(0, 0, 0); }
  virtual ushort ArmSpecialColor() const { return MAKE_RGB(160, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(64, 56, 24); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 176); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 144); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 112); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0,0); }
  virtual ulong TotalVolume() const { return 120000; }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " falls groaning bravely: \"Party revenges " + GetName(UNARTICLED) + "\"!"; }
  //virtual std::string NameSingular() const { return "communist"; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual ushort TotalSize() const { return 230; }
  virtual bool ShowClassDescription() const { return GetAssignedName() != "Ivan"; }
);

class CHARACTER
(
  hunter,
  humanoid,
  /*{
    SetAgility(20);
    SetStrength(15);
    SetEndurance(15);
    SetPerception(24);
  },*/
 public:
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual void CreateBodyPart(ushort);
  virtual ushort ClothColor() const { return MAKE_RGB(128, 80, 48); }
  virtual ushort BeltColor() const { return MAKE_RGB(144, 96, 60); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 192); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 192); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 128); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 96); }
  virtual ulong TotalVolume() const { return 80000; }
  //virtual std::string NameSingular() const { return "hunter"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 180; }
);

class CHARACTER
(
  polarbear,
  nonhumanoid,
  /*{
    SetAgility(10);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(24);
  },*/
 public:
  //static bool CanBeGenerated() { return false; }
 protected:
  virtual ulong TotalVolume() const { return 500000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(POLARBEARFLESH, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " groans terribly and falls dead to the ground."; }
  //virtual std::string NameSingular() const { return "polar bear"; }
  //virtual float GetMeleeStrength() const { return 10000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(336,0); }
  virtual std::string TalkVerb() const { return "growls"; }
  virtual ushort TotalSize() const { return 250; }
);

class CHARACTER
(
  dolphin,
  nonhumanoid,
  /*{
    SetAgility(30);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(30);
  },*/
 public:
  //static bool CanBeGenerated() { return false; }
  virtual std::string StandVerb() const { return "swimming"; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual bool CanOpen() const { return false; }
  virtual bool CanWalk() const { return false; }
  virtual bool CanSwim() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 150000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DOLPHINFLESH, Volume); }
  //virtual std::string NameSingular() const { return "female dolphin in season"; }
  //virtual float GetMeleeStrength() const { return 1000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(320,0); }
  virtual std::string TalkVerb() const { return "peeps passionately to you"; }
  virtual ushort TotalSize() const { return 300; }
);

class CHARACTER
(
  lightfrog,
  frog,
  /*{
    SetAgility(30);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(18);
  },*/
 public:
  //static bool CanBeGenerated() { return false; }
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
  virtual bool CanSwim() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 2500; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(FROGFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(80,0); }
  //virtual std::string NameSingular() const { return "light frog"; }
  virtual ushort TotalSize() const { return 25; }
);

class CHARACTER
(
  slave,
  humanoid,
  /*{
    SetAgility(10);
    SetStrength(20);
    SetEndurance(15);
    SetPerception(15);
  },*/
 public:
  virtual void BeTalkedTo(character*);
  virtual void GetAICommand();
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(128, 80, 48); }
  virtual ushort HairColor() const { return MAKE_RGB(80, 48, 32); }
  virtual ushort EyeColor() const { return MAKE_RGB(64, 48, 24); }
  virtual ushort ClothColor() const { return MAKE_RGB(56, 48, 20); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96,0); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32,0); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 0); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0,16); }
  virtual ulong TotalVolume() const { return 60000; }
  //virtual std::string NameSingular() const { return "slave"; }
  virtual ushort TotalSize() const { return 160; }
);

class CHARACTER
(
  petrusswife,
  humanoid,
  /*{
    SetAgility(10);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },*/
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
 protected:
  virtual ushort ClothColor() const { return MAKE_RGB(150, 0, 0); }
  virtual ushort BeltColor() const { return MAKE_RGB(180, 180, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112,0); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 160); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 160); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 112); }
  virtual ulong TotalVolume() const { return 50000; }
  //virtual std::string NameSingular() const { return "Petrus's wife"; }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual ushort TotalSize() const { return 170; }
  //virtual bool ShowArticle() const { return false; }
  //virtual uchar GetArticleMode() const { return NOARTICLE; }
);

class CHARACTER
(
  housewife,
  humanoid,
  /*{
    SetAgility(15);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(24);
  },*/
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
 protected:
  virtual ushort TorsoMainColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ArmMainColor() const { return MAKE_RGB(100, 100, 100); }
  virtual ushort LegMainColor() const { return MAKE_RGB(180, 80, 0); }
  virtual ushort HairColor() const { return MAKE_RGB(60, 48, 24); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, (RAND() % 6) * 16); } //may produce gibberling headed housewives...
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 160); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 160); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 112); }
  virtual ulong TotalVolume() const { return 70000; }
  //virtual std::string NameSingular() const { return "housewife"; }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual ushort TotalSize() const { return 160; }
);

class CHARACTER
(
  femaleslave,
  humanoid,
  /*{
    SetAgility(10);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(18);
  },*/
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
  virtual void CreateInitialEquipment();
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(160, 100, 64); }
  virtual ushort HairColor() const { return MAKE_RGB(80, 48, 32); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 96); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 208); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 208); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 144); }
  virtual ulong TotalVolume() const { return 40000; }
  //virtual std::string NameSingular() const { return "female slave"; }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual void GetAICommand() { StandIdleAI(); }
  virtual ushort TotalSize() const { return 170; }
);

class CHARACTER
(
  librarian,
  humanoid,
  /*{
    SetAgility(5);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(12);
  },*/
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ClothColor() const { return MAKE_RGB(48, 48, 48); }
  virtual ushort ArmMainColor() const { return MAKE_RGB(144, 144, 144); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 224); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 80); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 0); }
  virtual ulong TotalVolume() const { return 80000; }
  //virtual std::string NameSingular() const { return "librarian"; }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual void GetAICommand() { StandIdleAI(); }
  virtual ushort TotalSize() const { return 170; }
);

class CHARACTER
(
  zombie,
  humanoid,
  /*{
    SetAgility(5);
    SetStrength(10);
    SetEndurance(5);
    SetPerception(12);
  },*/
 public:
  //static bool CanBeGenerated() { return true; }
  virtual void BeTalkedTo(character*);
  virtual void SpillBlood(uchar, vector2d);
  //virtual float GetMeleeStrength() const { return 1500; }
  virtual uchar GetSex() const { return UNDEFINED; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(0, 120, 120); }
  virtual ushort EyeColor() const { return MAKE_RGB(200, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(56, 16, 96); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 112); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 224); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 224); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 160); }
  virtual ulong TotalVolume() const { return 50000; }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " is slain (again)."; }
  //virtual std::string NameSingular() const { return "zombie"; }
  virtual ushort TotalSize() const { return 160; }
);

class CHARACTER
(
  imp,
  humanoid,
  /*{
    SetAgility(15);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(15);
  },*/
 public:
  virtual uchar GetSex() const { return UNDEFINED; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(128, 0, 0); }
  virtual ushort EyeColor() const { return MAKE_RGB(150, 80, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(111,74,37); }
  virtual ushort CapColor() const { return MAKE_RGB(30,0,0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 16); } 
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 112); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 32); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 80); }
  //virtual std::string GetArticle() const { return "an"; }
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(SULFUR, Volume); }
  //virtual std::string NameSingular() const { return "imp"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual ushort TotalSize() const { return 100; }
);

class CHARACTER
(
  bat,
  nonhumanoid,
  /*{
    SetAgility(40);
    SetStrength(2);
    SetEndurance(1);
    SetPerception(24);
  },*/
 public:
  virtual std::string StandVerb() const { return "flying"; }
  virtual bool CanOpen() const { return false; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 1000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(BATFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(464,0); }
  //virtual std::string NameSingular() const { return "bat"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "squeaks"; }
  virtual ushort TotalSize() const { return 20; }
);

class CHARACTER
(
  mistress,
  humanoid,
  /*{
    SetAgility(35);
    SetStrength(25);
    SetEndurance(50);
    SetPerception(30);
  },*/
 public:
  virtual void BeTalkedTo(character*);
  //static bool CanBeGenerated() { return true; }
  virtual uchar GetSex() const { return FEMALE; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual void CreateInitialEquipment();
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(255, 212, 192); }
  virtual ushort HairColor() const { return MAKE_RGB(35, 35, 35); }
  virtual ushort ClothColor() const { return MAKE_RGB(35, 35, 35); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 240); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 96); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(80, 80); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 32); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(352,0); }
  //virtual std::string NameSingular() const { return "mistress"; }
  virtual ushort TotalSize() const { return 180; }
);

class werewolfwolf;

class CHARACTER
(
  werewolfhuman,
  humanoid,
 public:
  //virtual void Load(inputfile&);
  //virtual void Save(outputfile&) const;
  //virtual void Be();
  //virtual ushort GetChangeCounter() const { return ChangeCounter; }
  //virtual void SetChangeCounter(ushort What) { ChangeCounter = What; }
  //virtual ulong MaxDanger() const;
  //virtual werewolfwolf* GetWolfForm() const { return WolfForm; }
  //virtual void SetWolfForm(werewolfwolf* What) { WolfForm = What; }
 protected:
  //virtual void VirtualConstructor(bool);
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(WEREWOLFFLESH, Volume); }
  //ushort ChangeCounter;
  //werewolfwolf* WolfForm;
);

class CHARACTER
(
  werewolfwolf,
  humanoid,
 public:
  /*virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  //virtual void Be();
  virtual ushort GetChangeCounter() const { return ChangeCounter; }
  virtual void SetChangeCounter(ushort What) { ChangeCounter = What; }
  virtual ulong MaxDanger() const;
  virtual werewolfhuman* GetHumanForm() const { return HumanForm; }
  virtual void SetHumanForm(werewolfhuman* What) { HumanForm = What; }*/
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(WEREWOLFFLESH, Volume); }
  //ushort ChangeCounter;
  //werewolfhuman* HumanForm;
);

class CHARACTER
(
  kobold,
  humanoid,
  /*{
    SetAgility(10);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(12);
  },*/
 public:
  virtual uchar GetSex() const { return UNDEFINED; }
  //static bool CanBeGenerated() { return true; }
  virtual void CreateInitialEquipment();
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(30, 100, 110); }
  virtual ushort HairColor() const { return MAKE_RGB(35, 35, 35); }
  virtual ushort ClothColor() const { return MAKE_RGB(111, 74, 37); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 208); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 176); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 176); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 112); }
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(KOBOLDFLESH, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " dies yelling like a tortured hyena."; }
  //virtual std::string NameSingular() const { return "kobold"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 90; }
);

class CHARACTER
(
  gibberling,
  humanoid,
  /*{
    SetAgility(20);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(15);
  },*/
 public:
  virtual uchar GetSex() const { return UNDEFINED; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(100, 100, 200); }
  virtual ushort HairColor() const { return MAKE_RGB(50, 20, 80); }
  virtual ushort ClothColor() const { return MAKE_RGB(111, 74, 37); }
  virtual ushort EyeColor() const { return MAKE_RGB(50,0,0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 32); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 144); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(80, 144); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 128); }
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(GIBBERLINGFLESH, Volume); }
  //virtual std::string NameSingular() const { return "gibberling"; }
  //virtual float GetMeleeStrength() const { return 10000; }
  virtual ushort TotalSize() const { return 90; }
);

class CHARACTER  
(        
  largecat,
  nonhumanoid,
  /*{
    SetAgility(25);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },*/
 public:
  virtual bool Catches(item*, float);
 protected:
  virtual ulong TotalVolume() const { return 15000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(CATFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(496,0); }
  //virtual std::string NameSingular() const { return "large cat"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "mews"; }
  virtual ushort TotalSize() const { return 60; }
);

class CHARACTER  
(        
  largerat,
  nonhumanoid,
  /*{
    SetAgility(10);
    SetStrength(3);
    SetEndurance(2);
    SetPerception(12);
  },*/
 public:
  virtual bool CanOpen() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 2000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(RATFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(512,0); }
  //virtual std::string NameSingular() const { return "large rat"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "squeaks"; }
  virtual ushort TotalSize() const { return 30; }
);

class CHARACTER  
(        
  angel,
  humanoid,
  /*{
    SetAgility(35);
    SetStrength(35);
    SetEndurance(35);
    SetPerception(45);
  },*/
 public:
  virtual void BeTalkedTo(character*);
  virtual bool Charmable() const { return false; }
  virtual bool HasInfraVision() const { return true; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual uchar GetSex() const { return FEMALE; }
  virtual void SetDivineMaster(uchar);
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " leaves this mortal plane behind."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(432,0); }
  //virtual std::string NameSingular() const { return "angel"; }
  //virtual float GetMeleeStrength() const { return 10000; }
  virtual void CreateCorpse() { SendToHell(); }
  virtual ushort TotalSize() const { return 180; }
  //virtual std::string GetArticle() const { return "an"; }
  virtual std::string GetPostFix() const { return GetDivineMasterDescription(DivineMaster); }
  uchar DivineMaster;
);

class ABSTRACT_CHARACTER
(
  dwarf,
  humanoid,
 public:
  virtual void DrawToTileBuffer(bool) const;
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(DWARFFLESH, Volume); }
);

class CHARACTER
(
  kamikazedwarf,
  dwarf,
  /*{
    SetAgility(20);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(24);
    SetDivineMaster(1 + RAND() % (game::GetGods() - 1));
  },*/
 public:
  virtual void VirtualConstructor(bool);
  virtual void BeTalkedTo(character*);
  //static bool CanBeGenerated() { return true; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ulong CurrentDanger() const { return character::CurrentDanger() * 50; }
  virtual ulong MaxDanger() const { return character::MaxDanger() * 50; }
  virtual bool Hit(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual bool CheckForUsefulItemsOnGround() { return false; }
  virtual void GetAICommand();
  virtual void CreateInitialEquipment();
  //static ushort Frequency() { return 1000; }
  virtual void SetDivineMaster(uchar);
 protected:
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 160); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 16); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 0); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 208); }
  virtual ushort HairColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ClothColor() const { return MAKE_RGB(111, 74, 37); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " dies smiling."; }
  //virtual std::string NameSingular() const { return "kamikaze dwarf"; }
  virtual ushort TotalSize() const { return 130; }
  uchar DivineMaster;
);

class CHARACTER  
(        
  mammoth,
  nonhumanoid,
  /*{
    SetAgility(20);
    SetStrength(80);
    SetEndurance(80);
    SetPerception(18);
  },*/
 protected:
  virtual ulong TotalVolume() const { return 5000000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(MAMMOTHFLESH, Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(528,0); }
  //virtual std::string NameSingular() const { return "mammoth"; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "roars"; }
  virtual ushort TotalSize() const { return 500; }
);

class CHARACTER
(
  unicorn,
  nonhumanoid,
  /*{
    SetAgility(40);
    SetStrength(10);
    SetEndurance(25);
    SetPerception(18);
    SetAlignment(RAND() % 3);
  },*/
 public:
  //virtual void VirtualConstructor(bool);
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "neighs"; }
  virtual void SetAlignment(uchar What) { Alignment = What; }
  virtual uchar GetAlignment() const { return Alignment; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool SpecialEnemySightedReaction(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual void CreateBodyParts();
  virtual ulong TotalVolume() const { return 200000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(544, 0); }
  //virtual std::string NameSingular() const;
  virtual material* CreateBodyPartFlesh(ushort, ulong) const;
  virtual ushort TotalSize() const { return 200; }
  uchar Alignment;
);

class CHARACTER
(
  genie,
  humanoid,
  /*{
    SetAgility(30);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(18);
  },*/
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual std::string StandVerb() const { return "floating"; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 200000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new gas(AIR, Volume); } // temporary
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(416,0); }
  //virtual std::string NameSingular() const { return "genie"; }
  virtual void CreateCorpse() { SendToHell(); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " vanishes from existence."; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual ushort TotalSize() const { return 250; }
);

class CHARACTER
(
  lion,
  nonhumanoid,
  /*{
    SetAgility(40);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(24);
  },*/
 protected:
  virtual ulong TotalVolume() const { return 150000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(LIONFLESH, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " growls."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(576,0); }
  //virtual std::string NameSingular() const { return "lion"; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  //virtual float GetMeleeStrength() const { return 3000; }
  virtual std::string TalkVerb() const { return "growls"; }
  virtual ushort TotalSize() const { return 200; }
);

class CHARACTER
(
  carnivorousplant,
  nonhumanoid,
  /*{
    SetAgility(5);
    SetStrength(30);
    SetEndurance(4);
    SetPerception(5);
  },*/
 public:
  virtual bool CanOpen() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 20000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(LEAF, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " howls."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(0,16); }
  //virtual std::string NameSingular() const { return "carnivorous plant"; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  //virtual float GetMeleeStrength() const { return 2500; }
  virtual std::string TalkVerb() const { return "is silent"; }
  virtual ushort TotalSize() const { return 250; }
  virtual void GetAICommand();
  //virtual void CreateTorso();
);

class CHARACTER
(
  buffalo,
  nonhumanoid,
  /*{
    SetAgility(4);
    SetStrength(3);
    SetEndurance(30);
    SetPerception(24);
  },*/
 protected:
  virtual ulong TotalVolume() const { return 200000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return MAKE_MATERIAL(BUFFALOFLESH, Volume); }
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " snarls one last time."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(16,16); }
  //virtual std::string NameSingular() const { return "buffalo"; }
  /*virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }*/
  //virtual float GetMeleeStrength() const { return 500; }
  virtual std::string TalkVerb() const { return "snarls"; }
  virtual ushort TotalSize() const { return 250; }
);

#endif

