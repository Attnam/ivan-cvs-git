#ifndef __CHARDE_H__
#define __CHARDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "charba.h"
#include "graphics.h"
#include "wskill.h"
#include "materde.h"
#include "femath.h"

class head;
class rightarm;
class leftarm;
class groin;
class rightleg;
class leftleg;

class ABSTRACT_CHARACTER
(
  humanoid,
  character,
 public:
  virtual ~humanoid();
  virtual void VirtualConstructor();
  virtual void Load(inputfile&);
  virtual bool WearArmor();
  virtual item* GetTorsoArmor() const { return Armor.Torso; }
  virtual bool SetTorsoArmor(item* What) { Armor.Torso = What; return true; }
  virtual uchar GetSex() const { return MALE; }
  virtual ushort CalculateArmorModifier() const;
  virtual void Save(outputfile&) const;
  virtual bool CanWield() const { return true; }
  virtual bool CanWear() const { return true; }
  virtual bool CanKick() const { return true; }
  virtual float GetAttackStrength() const;
  virtual bool Hit(character*);
  virtual gweaponskill* GetCategoryWeaponSkill(uchar Index) const { return CategoryWeaponSkill[Index]; }
  virtual void CharacterSpeciality(ushort = 1);
  virtual bool ShowWeaponSkills();
  virtual void SetWielded(item*);
  virtual sweaponskill* GetCurrentSingleWeaponSkill() const { return CurrentSingleWeaponSkill; }
  virtual void SetCurrentSingleWeaponSkill(sweaponskill* What) { CurrentSingleWeaponSkill = What; }
  virtual sweaponskill* GetSingleWeaponSkill(ushort Index) const { return SingleWeaponSkill[Index]; }
  virtual void SetSingleWeaponSkill(ushort Index, sweaponskill* What) { SingleWeaponSkill[Index] = What; }
  virtual float GetToHitValue() const;
  virtual long StatScore() const;
  virtual void AddSpecialItemInfo(std::string&, item*);
  virtual void AddSpecialItemInfoDescription(std::string&);
  virtual void KickHit();
  virtual bool CanBeGenerated() const { return false; }
  virtual void CheckGearExistence();
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
  virtual void SetSize(ushort);

  virtual void DrawToTileBuffer() const;

 protected:

  virtual uchar GetHeadType() const { return 0; } // = 0;
  virtual uchar GetTorsoType() const { return 0; } // = 0;
  virtual uchar GetRightArmType() const { return GetArmType(); }
  virtual uchar GetLeftArmType() const { return GetArmType(); }
  virtual uchar GetArmType() const { return 0; } // = 0;
  virtual uchar GetGroinType() const { return GetLegType(); }
  virtual uchar GetRightLegType() const { return GetLegType(); }
  virtual uchar GetLeftLegType() const { return GetLegType(); }
  virtual uchar GetLegType() const { return 0; } // = 0;

  virtual ushort SkinColor() const { return MAKE_RGB(180, 120, 90); }

  virtual ushort CapColor() const { return ClothColor(); }
  virtual ushort HairColor() const { return MAKE_RGB(160, 80, 0); }
  virtual ushort EyeColor() const { return MAKE_RGB(112, 72, 42); }

  virtual ushort TorsoMainColor() const { return ClothColor(); }
  virtual ushort BeltColor() const { return MAKE_RGB(48, 48, 48); }
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 0, 0); }

  virtual ushort ArmMainColor() const { return ClothColor(); }
  virtual ushort ArmSpecialColor() const { return MAKE_RGB(0, 0, 0); }

  virtual ushort LegMainColor() const { return ClothColor(); }
  virtual ushort LegSpecialColor() const { return MAKE_RGB(0, 0, 0); }

  virtual ushort ClothColor() const { return MAKE_RGB(111, 74, 37); }

  virtual void CreateBodyParts();
  virtual void CreateHead();
  virtual void CreateTorso();
  virtual void CreateRightArm();
  virtual void CreateLeftArm();
  virtual void CreateGroin();
  virtual void CreateRightLeg();
  virtual void CreateLeftLeg();
  virtual void UpdateBodyPartPictures(bool = true);
  virtual void UpdateHeadPicture(bool = true);
  virtual void UpdateTorsoPicture(bool = true);
  virtual void UpdateRightArmPicture(bool = true);
  virtual void UpdateLeftArmPicture(bool = true);
  virtual void UpdateGroinPicture(bool = true);
  virtual void UpdateRightLegPicture(bool = true);
  virtual void UpdateLeftLegPicture(bool = true);
  virtual material* CreateHeadFlesh(ulong Volume) const { return CreateTorsoFlesh(Volume); }
  virtual material* CreateHeadBone(ulong Volume) const { return CreateTorsoBone(Volume); }
  virtual material* CreateRightArmFlesh(ulong Volume) const { return CreateArmFlesh(Volume); }
  virtual material* CreateRightArmBone(ulong Volume) const { return CreateArmBone(Volume); }
  virtual material* CreateLeftArmFlesh(ulong Volume) const { return CreateArmFlesh(Volume); }
  virtual material* CreateLeftArmBone(ulong Volume) const { return CreateArmBone(Volume); }
  virtual material* CreateArmFlesh(ulong Volume) const { return CreateTorsoFlesh(Volume); }
  virtual material* CreateArmBone(ulong Volume) const { return CreateTorsoBone(Volume); }
  virtual material* CreateGroinFlesh(ulong Volume) const { return CreateLegFlesh(Volume); }
  virtual material* CreateGroinBone(ulong Volume) const { return CreateLegBone(Volume); }
  virtual material* CreateRightLegFlesh(ulong Volume) const { return CreateLegFlesh(Volume); }
  virtual material* CreateRightLegBone(ulong Volume) const { return CreateLegBone(Volume); }
  virtual material* CreateLeftLegFlesh(ulong Volume) const { return CreateLegFlesh(Volume); }
  virtual material* CreateLeftLegBone(ulong Volume) const { return CreateLegBone(Volume); }
  virtual material* CreateLegFlesh(ulong Volume) const { return CreateTorsoFlesh(Volume); }
  virtual material* CreateLegBone(ulong Volume) const { return CreateTorsoBone(Volume); }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new humanflesh(Volume); }
  virtual uchar HeadBonePercentile() const { return 20; }
  virtual uchar TorsoBonePercentile() const { return 10; }
  virtual uchar RightArmBonePercentile() const { return ArmBonePercentile(); }
  virtual uchar LeftArmBonePercentile() const { return ArmBonePercentile(); }
  virtual uchar ArmBonePercentile() const { return 30; }
  virtual uchar GroinBonePercentile() const { return 40; }
  virtual uchar RightLegBonePercentile() const { return LegBonePercentile(); }
  virtual uchar LeftLegBonePercentile() const { return LegBonePercentile(); }
  virtual uchar LegBonePercentile() const { return 30; }
  virtual ulong HeadVolume() const;
  virtual ulong TorsoVolume() const;
  virtual ulong RightArmVolume() const { return ArmVolume(); }
  virtual ulong LeftArmVolume() const { return ArmVolume(); }
  virtual ulong ArmVolume() const;
  virtual ulong GroinVolume() const;
  virtual ulong RightLegVolume() const { return LegVolume(); }
  virtual ulong LeftLegVolume() const { return LegVolume(); }
  virtual ulong LegVolume() const;

  virtual uchar BodyParts() const { return 7; }

  virtual vector2d GetBitmapPos() const { return vector2d(0,0); } // remove this
  virtual float GetMeleeStrength() const { return 1000; }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " dies screaming."; }
  struct armor
  {
    armor();
    item* Torso;
    /*item* Legs;
    item* Hands;
    item* Head;
    item* Feet;*/
  } Armor;
  gweaponskill* CategoryWeaponSkill[WEAPON_SKILL_GATEGORIES];
  std::vector<sweaponskill*> SingleWeaponSkill;
  sweaponskill* CurrentSingleWeaponSkill;
);

inline humanoid::armor::armor() : Torso(0)/*, Legs(0), Hands(0), Head(0), Feet(0)*/ { }

class CHARACTER
(
  human,
  humanoid,
  {
    SetSize(150 + RAND() % 51);
    SetAgility(15 + RAND() % 11);
    SetStrength(10 + RAND() % 6);
    SetEndurance(10 + RAND() % 6);
    SetPerception(10 + RAND() % 6);
    SetMoney(200 + RAND() % 101);
  },
 public:
 protected:
  virtual uchar GetHeadType() const { return 0; }
  virtual uchar GetTorsoType() const { return 0; }
  virtual uchar GetArmType() const { return 0; }
  virtual uchar GetLegType() const { return 0; }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string NameSingular() const { return "human"; }
);

class CHARACTER
(
  petrus,
  humanoid,
  {
    SetSize(225);
    SetAgility(75);
    SetStrength(75);
    SetEndurance(75);
    SetPerception(75);
    SetHealTimer(100);
    SetStoryState(0);
    game::SetPetrus(this);
    ReName("Petrus");
  },
 public:
  virtual ~petrus();
  virtual void Load(inputfile&);
  //  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual void BeTalkedTo(character*);
  virtual ushort GetEmitation() const { return 333; }
  virtual void HealFully(character*);
  virtual void SetHealTimer(ushort What) { HealTimer = What; }
  virtual ushort GetHealTimer() { return HealTimer; }
  virtual void ReceiveFireDamage(character*, std::string, long) { }
  virtual void Save(outputfile&) const;
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual void AddHitMessage(character*, const bool = false) const;
  virtual uchar GetStoryState() const { return StoryState; }
  virtual void SetStoryState(uchar What) { StoryState = What; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual void CreateInitialEquipment();
  virtual bool CanBeAssignedAName() const { return false; }
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ClothColor() const { return MAKE_RGB(48, 48, 48); }
  virtual uchar GetHeadType() const { return 9; }
  virtual uchar GetTorsoType() const { return 5; }
  virtual uchar GetArmType() const { return 1; }
  virtual uchar GetLegType() const { return 0; }
  virtual ulong TotalVolume() const { return 80000; }
  virtual std::string NameSingular() const { return "High Priest of the Great Frog"; }
  virtual void CreateCorpse();
  virtual std::string DeathMessage() { return "The High Priest disappears in a bright light and only his left nut is left behind."; }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual void GetAICommand();
  ushort HealTimer;
  uchar StoryState;
);

class CHARACTER
(
  farmer,
  humanoid,
  {
    SetSize(170);
    SetAgility(10);
    SetStrength(15);
    SetEndurance(20);
    SetPerception(18);
    SetMoney(20);
  },
 public:
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 96, 0); }
  virtual ushort HairColor() const { return RAND() % 2 ? humanoid::HairColor() : MAKE_RGB(160, 160, 160); }
  virtual uchar GetHeadType() const { return 4 + RAND() % 2; }
  virtual uchar GetTorsoType() const { return 2; }
  virtual uchar GetArmType() const { return RAND() % 2; }
  virtual uchar GetLegType() const { return RAND() % 3; }
  virtual ulong TotalVolume() const { return 30000; }
  virtual std::string NameSingular() const { return "farmer"; }
);

class CHARACTER
(
  guard,
  humanoid,
  {
    SetSize(180);
    SetAgility(15);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(24);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort ClothColor() const { return MAKE_RGB(0, 128, 128); }
  virtual uchar GetHeadType() const { return 7; }
  virtual uchar GetTorsoType() const { return 8; }
  virtual uchar GetArmType() const { return 5; }
  virtual uchar GetLegType() const { return 4; }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string NameSingular() const { return "guard"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class CHARACTER
(
  shopkeeper,
  humanoid,
  {
    SetSize(160);
    SetAgility(10);
    SetStrength(30);
    SetEndurance(25);
    SetPerception(30);
    SetMoney(3000 + RAND() % 2001);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
 protected:
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 96, 0); }
  virtual uchar GetHeadType() const { return 4; }
  virtual uchar GetTorsoType() const { return 2; }
  virtual uchar GetArmType() const { return 1; }
  virtual uchar GetLegType() const { return 2; }
  virtual ulong TotalVolume() const { return 100000; }
  virtual std::string NameSingular() const { return "shopkeeper"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class CHARACTER
(
  priest,
  humanoid,
  {
    SetSize(180);
    SetAgility(10);
    SetStrength(20);
    SetEndurance(15);
    SetPerception(18);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
  virtual bool CanSwim() const { return true; }
 protected:
  virtual ushort CapColor() const { return MAKE_RGB(180, 0, 80); }
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 96, 0); }
  virtual uchar GetHeadType() const { return 8; }
  virtual uchar GetTorsoType() const { return 2; }
  virtual uchar GetArmType() const { return 1; }
  virtual uchar GetLegType() const { return 2; }
  virtual ulong TotalVolume() const { return 100000; }
  virtual std::string NameSingular() const { return "priest"; }
);

class CHARACTER
(
  oree,
  humanoid,
  {
    SetSize(225);
    SetAgility(50);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(30);
    ReName("Oree");
  },
 public:
  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual void CreateInitialEquipment();
  virtual bool CanBeAssignedAName() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 110000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new daemonflesh(Volume); }
  virtual std::string DeathMessage() { return Name(DEFINITE) + "vomits blood for one last time and then dies."; }
  virtual vector2d GetBitmapPos() const { return vector2d(208,0); }
  virtual std::string NameSingular() const { return "Blood Daemon King"; }
  virtual float GetMeleeStrength() const { return 30000; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBloodVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBloodVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBloodVerb(Critical); }
);

class CHARACTER
(
  darkknight,
  humanoid,
  {
    SetSize(200);
    SetAgility(30);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(30);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual ulong TotalVolume() const { return 110000; }
  virtual vector2d GetBitmapPos() const { return vector2d(128,0); }
  virtual std::string NameSingular() const { return "dark knight"; }
  virtual float GetMeleeStrength() const { return 5000; }
);

class CHARACTER
(
  ennerbeast,
  character,
  {
    SetSize(150);
    SetAgility(10);
    SetStrength(10);
    SetEndurance(25);
    SetPerception(12);
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
  virtual bool Hit(character*);
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual void BeTalkedTo(character*);
  virtual ulong CurrentDanger() { return character::CurrentDanger() * 5; }
  virtual ulong MaxDanger() { return character::MaxDanger() * 5; }
 protected:
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new ennerbeastflesh(Volume); }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " dies and the world is finally freed from this terrible monster."; }
  virtual vector2d GetBitmapPos() const { return vector2d(96,0); }
  virtual std::string NameSingular() const { return "enner beast"; }
  virtual float GetMeleeStrength() const { return 100000; }
  virtual void GetAICommand();
  virtual void CreateCorpse();
);

class ABSTRACT_CHARACTER
(
  frog,
  character,
 public:
  virtual bool CanOpenDoors() const { return false; }
 protected:
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "croaks"; }
);

class CHARACTER
(
  darkfrog,
  frog,
  {
    SetSize(25);
    SetAgility(30);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(18);
  },
 public:
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual bool HasInfraVision() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 2500; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new darkfrogflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(80,0); }
  virtual std::string NameSingular() const { return "dark frog"; }
);

class CHARACTER
(
  elpuri,
  darkfrog,
  {
    SetSize(300);
    SetAgility(10);
    SetStrength(30);
    SetEndurance(50);
    SetPerception(30);
    ReName("Elpuri");
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual void BeTalkedTo(character*);
  virtual ushort CalculateArmorModifier() const { return 10; }
  virtual bool Hit(character*);
  virtual bool CanBeDisplaced() const { return false; }
  virtual bool CanBeAssignedAName() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 277500; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new elpuriflesh(Volume); }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " groans horribly and drops " + game::PossessivePronoun(GetSex()) + " head."; }
  virtual vector2d GetBitmapPos() const { return vector2d(64,0); }
  virtual std::string NameSingular() const { return "master the dark frog"; }
  virtual void CreateCorpse();
);

class CHARACTER
(
  billswill,
  character,
  {
    SetSize(100);
    SetAgility(40);
    SetStrength(5);
    SetEndurance(20);
    SetPerception(27);
  },
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual std::string StandVerb() const { return "floating"; }
  virtual ushort Frequency() const { return 100; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 500000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new air(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(48,0); }
  virtual std::string NameSingular() const { return "pure mass of Bill's will"; }
  virtual std::string NamePlural() const { return "pure masses of Bill's will"; }
  virtual void CreateCorpse() { }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " vanishes from existence."; }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonPSIVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonPSIVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonPSIVerb(Critical); }
);

class CHARACTER
(
  skeleton,
  humanoid,
  {
    SetSize(150);
    SetAgility(10);
    SetStrength(5);
    SetEndurance(10);
    SetPerception(15);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " is transformed into a crumpled heap of bones."; }
  virtual vector2d GetBitmapPos() const { return vector2d(112,0); }
  virtual std::string NameSingular() const { return "skeleton"; }
  virtual void CreateCorpse();
  virtual float GetMeleeStrength() const { return 1000; }
);

class CHARACTER
(
  goblin,
  humanoid,
  {
    SetSize(100);
    SetAgility(15);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(18);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual ulong TotalVolume() const { return 25000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new goblinoidflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(144,0); }
  virtual std::string NameSingular() const { return "goblin"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class ABSTRACT_CHARACTER
(
  mommo,
  character,
 public:
  virtual std::string StandVerb() const { return "bubbling"; }
 protected:
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new brownslime(Volume); }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " turns into lifeless goo."; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBrownSlimeVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBrownSlimeVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBrownSlimeVerb(Critical); }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "vibrates oddly"; }
);

class CHARACTER
(
  conicalmommo,
  mommo,
  {
    SetSize(100);
    SetAgility(2);
    SetStrength(4);
    SetEndurance(50);
    SetPerception(9);
  },
 public:
  virtual ulong GetBloodColor() const { return MAKE_RGB(7,155,0); }
 protected:
  virtual ulong TotalVolume() const { return 250000; }
  virtual std::string NameSingular() const { return "conical mommo slime"; }
  virtual vector2d GetBitmapPos() const { return vector2d(176,0); }

);

class CHARACTER
(
  flatmommo,
  mommo,
  {
    SetSize(50);
    SetAgility(4);
    SetStrength(2);
    SetEndurance(25);
    SetPerception(9);
  },
 public:
  virtual ulong GetBloodColor() const { return MAKE_RGB(7,155,0); }
 protected:
  virtual ulong TotalVolume() const { return 150000; }
  virtual std::string NameSingular() const { return "flat mommo slime"; }
  virtual vector2d GetBitmapPos() const { return vector2d(192,0); }
);

class CHARACTER
(
  golem,
  character,
  {
    SetSize(250);
    SetAgility(5);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(12);
  },
 public:
  virtual ushort CalculateArmorModifier() const;
  virtual bool MoveRandomly();
  //virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual bool CheckForUsefulItemsOnGround() { return false; }
  virtual bool CanKick() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 100000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return protosystem::CreateRandomSolidMaterial(Volume); }
  virtual std::string DeathMessage() { return "The Holy Words of " + Name(DEFINITE) + " fly away and the monster collapses."; }
  virtual std::string NameSingular() const { return "golem"; }
  virtual vector2d GetBitmapPos() const { return vector2d(256,0); }
  virtual float GetMeleeStrength() const;
);

class CHARACTER
(
  wolf,
  character,
  {
    SetSize(100);
    SetAgility(20);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(24);
  },
 protected:
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new wolfflesh(Volume); }
  virtual std::string NameSingular() const { return "wolf"; }
  virtual std::string NamePlural() const { return "wolves"; }
  virtual vector2d GetBitmapPos() const { return vector2d(224,0); }
  virtual float GetMeleeStrength() const { return 7500; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string TalkVerb() const { return "howls"; }
);

class CHARACTER
(
  dog,
  character,
  {
    SetSize(70);
    SetAgility(15);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },
 public:
  virtual bool Catches(item*, float);
  virtual bool ConsumeItemType(uchar) const;
 protected:
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new dogflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(240,0); }
  virtual std::string NameSingular() const { return "dog"; }
  virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string TalkVerb() const { return "barks"; }
);

class CHARACTER
(
  spider,
  character,
  {
    SetSize(10);
    SetAgility(5);
    SetStrength(2);
    SetEndurance(1);
    SetPerception(9);
  },
 public:
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanOpenDoors() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 100; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new spiderflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(272,0); }
  virtual std::string NameSingular() const { return "spider"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual float GetMeleeStrength() const { return 10000; }
  virtual void CreateCorpse() { }
  virtual std::string TalkVerb() const { return "says nothing"; }
);

class CHARACTER
(
  jackal,
  character,
  {
    SetSize(80);
    SetAgility(10);
    SetStrength(3);
    SetEndurance(3);
    SetPerception(18);
  },
 protected:
  virtual ulong TotalVolume() const { return 20000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new jackalflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(304,0); }
  virtual std::string NameSingular() const { return "jackal"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }                                                                                                                                                                                                                                                                                          //Jackals are unoriginal
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "howls"; }
);

class CHARACTER
(
  donkey,
  character,
  {
    SetSize(150);
    SetAgility(5);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(15);
  },
 protected:
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new donkeyflesh(Volume); }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " neighs one last time and then dies."; }
  virtual vector2d GetBitmapPos() const { return vector2d(288,0); }
  virtual std::string NameSingular() const { return "mutant donkey"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual float GetMeleeStrength() const { return 1000; }
  virtual std::string TalkVerb() const { return "neighs"; }
);

class CHARACTER
(
  communist,
  humanoid,
  {
    SetSize(230);
    SetAgility(20);
    SetStrength(50);
    SetEndurance(50);
    SetPerception(18);
    ReName("Ivan");
  },
 public:
  virtual bool MoveRandomly();
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual bool HasInfraVision() const { return true; }
  virtual uchar CriticalModifier() const { return 4; }
  virtual bool ShowClassName() const { return false; } 
  virtual bool CanBeAssignedAName() const { return false; }
 protected:
  virtual ushort BeltColor() const { return MAKE_RGB(0, 0, 0); }
  virtual ushort ArmSpecialColor() const { return MAKE_RGB(160, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(64, 56, 24); }
  virtual uchar GetHeadType() const { return 11; }
  virtual uchar GetTorsoType() const { return 9; }
  virtual uchar GetArmType() const { return 7; }
  virtual uchar GetLegType() const { return 0; }
  virtual ulong TotalVolume() const { return 120000; }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " falls groaning bravely: \"Party revenges " + Name(DEFINITE) + "\"!"; }
  virtual std::string NameSingular() const { return "communist"; }
  virtual float GetMeleeStrength() const { return 5000; }
);

class CHARACTER
(
  hunter,
  humanoid,
  {
    SetSize(180);
    SetAgility(20);
    SetStrength(15);
    SetEndurance(15);
    SetPerception(24);
  },
 public:
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort ClothColor() const { return MAKE_RGB(128, 80, 48); }
  virtual ushort BeltColor() const { return MAKE_RGB(144, 96, 60); }
  virtual uchar GetHeadType() const { return 12; }
  virtual uchar GetTorsoType() const { return 12; }
  virtual uchar GetArmType() const { return 8; }
  virtual uchar GetLegType() const { return 6; }
  virtual ulong TotalVolume() const { return 80000; }
  virtual std::string NameSingular() const { return "hunter"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class CHARACTER
(
  polarbear,
  character,
  {
    SetSize(250);
    SetAgility(10);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(24);
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 500000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new polarbearflesh(Volume); }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " groans terribly and falls dead to the ground."; }
  virtual std::string NameSingular() const { return "polar bear"; }
  virtual float GetMeleeStrength() const { return 10000; }
  virtual vector2d GetBitmapPos() const { return vector2d(336,0); }
  virtual std::string TalkVerb() const { return "growls"; }
);

class CHARACTER
(
  dolphin,
  character,
  {
    SetSize(300);
    SetAgility(30);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(30);
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
  virtual std::string StandVerb() const { return "swimming"; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual bool CanOpenDoors() const { return false; }
  virtual bool CanWalk() const { return false; }
  virtual bool CanSwim() const { return true; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
 protected:
  virtual ulong TotalVolume() const { return 150000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new dolphinflesh(Volume); }
  virtual std::string NameSingular() const { return "female dolphin in season"; }
  virtual float GetMeleeStrength() const { return 1000; }
  virtual vector2d GetBitmapPos() const { return vector2d(320,0); }
  virtual std::string TalkVerb() const { return "peeps passionately to you"; }
);

class CHARACTER
(
  lightfrog,
  frog,
  {
    SetSize(25);
    SetAgility(30);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(18);
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
  virtual bool CanSwim() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 2500; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new lightfrogflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(80,0); }
  virtual std::string NameSingular() const { return "light frog"; }
);

class CHARACTER
(
  slave,
  humanoid,
  {
    SetSize(160);
    SetAgility(10);
    SetStrength(20);
    SetEndurance(15);
    SetPerception(15);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual void GetAICommand();
 protected:
  virtual ushort SkinColor() const { return MAKE_RGB(128, 80, 48); }
  virtual ushort HairColor() const { return MAKE_RGB(80, 48, 32); }
  virtual ushort EyeColor() const { return MAKE_RGB(64, 48, 24); }
  virtual ushort ClothColor() const { return MAKE_RGB(56, 48, 20); }
  virtual uchar GetHeadType() const { return 0; }
  virtual uchar GetTorsoType() const { return 0; }
  virtual uchar GetArmType() const { return 0; }
  virtual uchar GetLegType() const { return 1; }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string NameSingular() const { return "slave"; }
);

class CHARACTER
(
  petrusswife,
  humanoid,
  {
    SetSize(170);
    SetAgility(10);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
 protected:
  virtual ushort ClothColor() const { return MAKE_RGB(150, 0, 0); }
  virtual ushort BeltColor() const { return MAKE_RGB(180, 180, 0); }
  virtual uchar GetHeadType() const { return 16; }
  virtual uchar GetTorsoType() const { return 10; }
  virtual uchar GetArmType() const { return 10; }
  virtual uchar GetLegType() const { return 7; }
  virtual ulong TotalVolume() const { return 50000; }
  virtual std::string NameSingular() const { return "Petrus's wife"; }
  virtual float GetMeleeStrength() const { return 500; }
);

class CHARACTER
(
  petrusswife1,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(0, 0, 0); }
  virtual uchar GetHeadType() const { return 16; }
  virtual std::string NameSingular() const { return "Petrus's wife number 1"; }
);

class CHARACTER
(
  petrusswife2,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(0, 0, 0); }
  virtual uchar GetHeadType() const { return 17; }
  virtual std::string NameSingular() const { return "Petrus's wife number 2"; }
);

class CHARACTER
(
  petrusswife3,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(60, 48, 24); }
  virtual uchar GetHeadType() const { return 16; }
  virtual std::string NameSingular() const { return "Petrus's wife number 3"; }
);

class CHARACTER
(
  petrusswife4,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(200, 96, 0); }
  virtual uchar GetHeadType() const { return 19; }
  virtual std::string NameSingular() const { return "Petrus's wife number 4"; }
);

class CHARACTER
(
  petrusswife5,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(80, 64, 32); }
  virtual uchar GetHeadType() const { return 20; }
  virtual std::string NameSingular() const { return "Petrus's wife number 5"; }
);

class CHARACTER
(
  petrusswife6,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(144, 0, 0); }
  virtual uchar GetHeadType() const { return 21; }
  virtual std::string NameSingular() const { return "Petrus's wife number 6"; }
);

class CHARACTER
(
  housewife,
  humanoid,
  {
    SetSize(160);
    SetAgility(15);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(24);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
 protected:
  virtual ushort TorsoMainColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ArmMainColor() const { return MAKE_RGB(100, 100, 100); }
  virtual ushort LegMainColor() const { return MAKE_RGB(180, 80, 0); }
  virtual ushort HairColor() const { return MAKE_RGB(60, 48, 24); }
  virtual uchar GetHeadType() const { return 16 + RAND() % 6; } //may produce headless housewives...
  virtual uchar GetTorsoType() const { return 10; }
  virtual uchar GetArmType() const { return 10; }
  virtual uchar GetLegType() const { return 7; }
  virtual ulong TotalVolume() const { return 70000; }
  virtual std::string NameSingular() const { return "housewife"; }
  virtual float GetMeleeStrength() const { return 500; }
);

class CHARACTER
(
  femaleslave,
  humanoid,
  {
    SetSize(170);
    SetAgility(10);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(18);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
  virtual void CreateInitialEquipment();
 protected:
  virtual ushort SkinColor() const { return MAKE_RGB(160, 100, 64); }
  virtual ushort HairColor() const { return MAKE_RGB(80, 48, 32); }
  virtual uchar GetHeadType() const { return 22; }
  virtual uchar GetTorsoType() const { return 13; }
  virtual uchar GetArmType() const { return 13; }
  virtual uchar GetLegType() const { return 9; }
  virtual ulong TotalVolume() const { return 40000; }
  virtual std::string NameSingular() const { return "female slave"; }
  virtual float GetMeleeStrength() const { return 500; }
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  librarian,
  humanoid,
  {
    SetSize(170);
    SetAgility(5);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(12);
  },
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ClothColor() const { return MAKE_RGB(48, 48, 48); }
  virtual ushort ArmMainColor() const { return MAKE_RGB(144, 144, 144); }
  virtual ushort CapColor() const { return MAKE_RGB(160, 160, 160); } // temporary
  virtual uchar GetHeadType() const { return 14; }
  virtual uchar GetTorsoType() const { return 5; }
  virtual uchar GetArmType() const { return 1; }
  virtual uchar GetLegType() const { return 0; }
  virtual ulong TotalVolume() const { return 80000; }
  virtual std::string NameSingular() const { return "librarian"; }
  virtual float GetMeleeStrength() const { return 500; }
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  zombie,
  humanoid,
  {
    SetSize(160);
    SetAgility(5);
    SetStrength(10);
    SetEndurance(5);
    SetPerception(12);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual void BeTalkedTo(character*);
  virtual void SpillBlood(uchar, vector2d);
  virtual float GetMeleeStrength() const { return 1500; }
 protected:
  virtual ushort SkinColor() const { return MAKE_RGB(0, 120, 120); }
  virtual ushort EyeColor() const { return MAKE_RGB(200, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(56, 16, 96); }
  virtual uchar GetHeadType() const { return 23; }
  virtual uchar GetTorsoType() const { return 14; }
  virtual uchar GetArmType() const { return 14; }
  virtual uchar GetLegType() const { return 10; }
  virtual ulong TotalVolume() const { return 50000; }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " is slain (again)."; }
  virtual std::string NameSingular() const { return "zombie"; }
);

class CHARACTER
(
  imp,
  character,
  {
    SetSize(100);
    SetAgility(15);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(15);
  },
 protected:
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new sulfur(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(368,0); }
  virtual std::string NameSingular() const { return "imp"; }
  virtual float GetMeleeStrength() const { return 20000; }
);

class CHARACTER
(
  bat,
  character,
  {
    SetSize(20);
    SetAgility(40);
    SetStrength(2);
    SetEndurance(1);
    SetPerception(24);
  },
 public:
  virtual std::string StandVerb() const { return "flying"; }
  virtual bool CanOpenDoors() const { return false; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 1000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new batflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(464,0); }
  virtual std::string NameSingular() const { return "bat"; }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "squeaks"; }
);

class CHARACTER
(
  mistress,
  humanoid,
  {
    SetSize(180);
    SetAgility(35);
    SetStrength(25);
    SetEndurance(50);
    SetPerception(30);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual bool CanBeGenerated() const { return true; }
  virtual uchar GetSex() const { return FEMALE; }
  virtual float GetMeleeStrength() const { return 5000; }
  virtual void CreateInitialEquipment();
 protected:
  virtual ushort SkinColor() const { return MAKE_RGB(255, 212, 192); }
  virtual ushort HairColor() const { return MAKE_RGB(35, 35, 35); }
  virtual ushort ClothColor() const { return MAKE_RGB(35, 35, 35); }
  virtual uchar GetHeadType() const { return 31; }
  virtual uchar GetTorsoType() const { return 22; }
  virtual uchar GetArmType() const { return 21; }
  virtual uchar GetLegType() const { return 18; }
  virtual ulong TotalVolume() const { return 60000; }
  virtual vector2d GetBitmapPos() const { return vector2d(352,0); }
  virtual std::string NameSingular() const { return "mistress"; }
);

class CHARACTER
(
  werewolf,
  humanoid,
  {
    SetChangeCounter(RAND() % 2500);

    if(RAND() % 2) 
      ChangeIntoHuman();
    else
      ChangeIntoWolf();
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool HasInfraVision() const { return GetIsWolf(); }
  virtual void ChangeIntoHuman();
  virtual void ChangeIntoWolf();
  virtual void Be();
  virtual bool GetIsWolf() const { return IsWolf; } 
  virtual void SetIsWolf(bool What) { IsWolf = What; }
  virtual void SetChangeCounter(ushort What) { ChangeCounter = What; }
  virtual ushort GetChangeCounter() { return ChangeCounter; }
  virtual ulong MaxDanger();
  virtual bool CanWield() const { return !GetIsWolf(); } 
 protected:
  virtual ushort SkinColor() const { return IsWolf ? MAKE_RGB(88, 96, 88) : humanoid::SkinColor(); }
  virtual ushort EyeColor() const { return MAKE_RGB(160, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(96, 64, 32); }
  virtual uchar GetHeadType() const { return IsWolf ? 25 : 0; }
  virtual uchar GetTorsoType() const { return 16; }
  virtual uchar GetArmType() const { return IsWolf ? 15 : 0; }
  virtual uchar GetLegType() const { return IsWolf ? 12 : 11; }
  virtual ulong TotalVolume() const { return 80000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new werewolfflesh(Volume); }
  virtual std::string NameSingular() const { return "werewolf"; }
  virtual float GetMeleeStrength() const;
  virtual std::string TalkVerb() const { return IsWolf ? "howls" : "screams"; }
  ushort ChangeCounter;
  bool IsWolf;
);

class CHARACTER
(
  kobold,
  humanoid,
  {
    SetSize(90);
    SetAgility(10);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(12);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual void CreateInitialEquipment();
 protected:
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new koboldflesh(Volume); }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " dies yelling like a tortured hyena."; }
  virtual vector2d GetBitmapPos() const { return vector2d(480,0); }
  virtual std::string NameSingular() const { return "kobold"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class CHARACTER
(
  gibberling,
  character,
  {
    SetSize(90);
    SetAgility(20);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(15);
  },
 protected:
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new gibberlingflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(448,0); }
  virtual std::string NameSingular() const { return "gibberling"; }
  virtual float GetMeleeStrength() const { return 10000; }
);

class CHARACTER  
(        
  largecat,
  character,
  {
    SetSize(60);
    SetAgility(25);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },
 public:
  virtual bool Catches(item*, float);
 protected:
  virtual ulong TotalVolume() const { return 15000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new catflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(496,0); }
  virtual std::string NameSingular() const { return "large cat"; }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "mews"; }
);

class CHARACTER  
(        
  largerat,
  character,
  {
    SetSize(30);
    SetAgility(10);
    SetStrength(3);
    SetEndurance(2);
    SetPerception(12);
  },
 public:
  virtual bool CanOpenDoors() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 2000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new ratflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(512,0); }
  virtual std::string NameSingular() const { return "large rat"; }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "squeaks"; }
);

class CHARACTER  
(        
  angel,
  humanoid,
  {
    SetSize(180);
    SetAgility(35);
    SetStrength(35);
    SetEndurance(35);
    SetPerception(45);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual bool HasInfraVision() const { return true; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual uchar GetSex() const { return FEMALE; }
  virtual void SetMaster(uchar);
  virtual uchar GetMaster() const { return Master; }
  //virtual std::string Name(uchar Case) const { return NameNormal(Case, "an") + OwnerGodDescription(Master); }
  virtual bool CanFly() const { return true; }
  virtual std::string Article() const { return "an"; }
 protected:
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " leaves this mortal plane behind."; }
  virtual vector2d GetBitmapPos() const { return vector2d(432,0); }
  virtual std::string NameSingular() const { return "angel"; }
  virtual float GetMeleeStrength() const { return 10000; }
  virtual void CreateCorpse() { }
  uchar Master;
);

class ABSTRACT_CHARACTER
(
  dwarf,
  humanoid,
 public:
  virtual void DrawToTileBuffer() const;
 protected:
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new dwarfflesh(Volume); }
);

class CHARACTER
(
  kamikazedwarf,
  dwarf,
  {
    SetSize(130);
    SetAgility(20);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(24);
    SetMaster(1 + RAND() % (game::GetGods() - 1));
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual bool CanBeGenerated() const { return true; }
  virtual float GetMeleeStrength() const { return 2000; }
  virtual ulong CurrentDanger() { return character::CurrentDanger() * 50; }
  virtual ulong MaxDanger() { return character::MaxDanger() * 50; }
  virtual bool Hit(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void SetMaster(uchar What) { Master = What; }
  virtual uchar GetMaster() const { return Master; }
  virtual bool CheckForUsefulItemsOnGround() { return false; }
  virtual void GetAICommand();
  virtual void CreateInitialEquipment();
  virtual ushort Frequency() const { return 1000; }
 protected:
  virtual uchar GetHeadType() const { return 26; }
  virtual uchar GetTorsoType() const { return 17; }
  virtual uchar GetArmType() const { return 16; }
  virtual uchar GetLegType() const { return 13; }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " dies smiling."; }
  virtual vector2d GetBitmapPos() const { return vector2d(400,0); }
  virtual std::string NameSingular() const { return "kamikaze dwarf"; }
  uchar Master;
);

class CHARACTER  
(        
  mammoth,
  character,
  {
    SetSize(500);
    SetAgility(20);
    SetStrength(80);
    SetEndurance(80);
    SetPerception(18);
  },
 protected:
  virtual ulong TotalVolume() const { return 500000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new mammothflesh(Volume); }
  virtual vector2d GetBitmapPos() const { return vector2d(528,0); }
  virtual std::string NameSingular() const { return "mammoth"; }
  virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "roars"; }
);

class CHARACTER
(
  unicorn,
  character,
  //RandomizeFleshMaterial(),
  {
    SetSize(200);
    SetAgility(40);
    SetStrength(10);
    SetEndurance(25);
    SetPerception(18);
  },
 public:
  virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "neighs"; }
  virtual void SetAlignment(uchar What) { Alignment = What; }
  virtual uchar GetAlignment() const { return Alignment; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void RandomizeFleshMaterial();
  virtual bool SpecialEnemySightedReaction(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual ulong TotalVolume() const { return 200000; }
  virtual vector2d GetBitmapPos() const { return vector2d(544, 0); }
  virtual std::string NameSingular() const;
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new blackunicornflesh(Volume); } // temporary
  uchar Alignment;
);

class CHARACTER
(
  genie,
  humanoid,
  {
    SetSize(250);
    SetAgility(30);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(18);
  },
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual std::string StandVerb() const { return "floating"; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 200000; }
  virtual material* CreateTorsoFlesh(ulong Volume) const { return new air(Volume); } // temporary
  virtual vector2d GetBitmapPos() const { return vector2d(416,0); }
  virtual std::string NameSingular() const { return "genie"; }
  virtual void CreateCorpse() { }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " vanishes from existence."; }
  virtual float GetMeleeStrength() const { return 5000; }
);

#endif
