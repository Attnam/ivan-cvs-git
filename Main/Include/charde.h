#ifndef __CHARDE_H__
#define __CHARDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "charba.h"
#include "graphics.h"
#include "wskill.h"

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
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(0,0); }
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

class ABSTRACT_CHARACTER
(
  complexhumanoid,
  humanoid,
 public:
  virtual void DrawToTileBuffer() const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetLegType() const { return LegType; }
  virtual uchar GetTorsoType() const { return TorsoType; }
  virtual uchar GetArmType() const { return ArmType; }
  virtual uchar GetHeadType() const { return HeadType; }
  virtual uchar GetShieldType() const { return ShieldType; }
  virtual void SetLegType(uchar Value) { LegType = Value; }
  virtual void SetTorsoType(uchar Value) { TorsoType = Value; }
  virtual void SetArmType(uchar Value) { ArmType = Value; }
  virtual void SetHeadType(uchar Value) { HeadType = Value; }
  virtual void SetShieldType(uchar Value) { ShieldType = Value; }
  virtual void DrawLegs(vector2d) const;
  virtual void DrawTorso(vector2d) const;
  virtual void DrawArms(vector2d) const;
  virtual void DrawHead(vector2d) const;
  virtual void DrawShield(vector2d) const;
  virtual void DrawInHandsPic(vector2d) const;
 protected:
  uchar LegType;
  uchar TorsoType;
  uchar ArmType;
  uchar HeadType;
  uchar ShieldType;
);

class CHARACTER
(
  human,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(150 + RAND() % 51);
    SetAgility(15 + RAND() % 11);
    SetStrength(10 + RAND() % 6);
    SetEndurance(10 + RAND() % 6);
    SetPerception(10 + RAND() % 6);
    SetLegType(RAND() % 3);
    SetTorsoType(0);
    SetArmType(RAND() % 5);
    SetHeadType(RAND() % 15);
    SetShieldType(0);
    SetMoney(200 + RAND() % 101);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
 protected:
  virtual std::string NameSingular() const { return "human"; }
);

class CHARACTER
(
  petrus,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(225);
    SetAgility(75);
    SetStrength(75);
    SetEndurance(75);
    SetPerception(75);
    SetLegType(1);
    SetTorsoType(5);
    SetArmType(3);
    SetHeadType(9);
    SetShieldType(0);
    SetHealTimer(100);
    SetStoryState(0);
    game::SetPetrus(this);
  },
 public:
  virtual ~petrus();
  virtual void Load(inputfile&);
  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual void BeTalkedTo(character*);
  virtual ushort GetEmitation() const { return 333; }
  virtual void HealFully(character*);
  virtual void SetHealTimer(ushort What) { HealTimer = What; }
  virtual ushort GetHealTimer() { return HealTimer; }
  virtual void ReceiveFireDamage(character*, std::string, long) { }
  virtual void Save(outputfile&) const;
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
  virtual void AddHitMessage(character*, const bool = false) const;
  virtual uchar GetStoryState() const { return StoryState; }
  virtual void SetStoryState(uchar What) { StoryState = What; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual void CreateInitialEquipment();
 protected:
  virtual std::string NameSingular() const { return "Petrus, the High Priest of the Great Frog"; }
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
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(170);
    SetAgility(10);
    SetStrength(15);
    SetEndurance(20);
    SetPerception(18);
    SetLegType(RAND() % 3);
    SetTorsoType(2 + RAND() % 2);
    SetArmType(RAND() % 5);
    SetHeadType(5 + RAND() % 2);
    SetShieldType(0);
    SetMoney(20);
  },
 public:
  virtual void CreateInitialEquipment();
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
  virtual void BeTalkedTo(character*);
 protected:
  virtual std::string NameSingular() const { return "farmer"; }
);

class CHARACTER
(
  guard,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(180);
    SetAgility(15);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(24);
    SetLegType(4);
    SetTorsoType(8);
    SetArmType(5);
    SetHeadType(7);
    SetShieldType(1);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
  virtual void BeTalkedTo(character*);
 protected:
  virtual std::string NameSingular() const { return "guard"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class CHARACTER
(
  shopkeeper,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(160);
    SetAgility(10);
    SetStrength(30);
    SetEndurance(25);
    SetPerception(30);
    SetLegType(2);
    SetTorsoType(7);
    SetArmType(6);
    SetHeadType(4);
    SetShieldType(0);
    SetMoney(3000 + RAND() % 2001);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
 protected:
  virtual std::string NameSingular() const { return "shopkeeper"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class CHARACTER
(
  priest,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(180);
    SetAgility(10);
    SetStrength(20);
    SetEndurance(15);
    SetPerception(18);
    SetLegType(2);
    SetTorsoType(7);
    SetArmType(6);
    SetHeadType(8);
    SetShieldType(0);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual bool CanSwim() const { return true; }
 protected:
  virtual std::string NameSingular() const { return "priest"; }
);

class CHARACTER
(
  oree,
  humanoid,
  InitMaterials(2, new daemonflesh, new leather),
  {
    SetSize(225);
    SetAgility(50);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(30);
  },
 public:
  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 110000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual void CreateInitialEquipment();
 protected:
  virtual std::string DeathMessage() { return Name(DEFINITE) + "vomits blood for one last time and then dies."; }
  virtual vector2d GetBitmapPos() const { return vector2d(208,0); }
  virtual std::string NameSingular() const { return "Oree the Blood Daemon King"; }
  virtual float GetMeleeStrength() const { return 30000; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBloodVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBloodVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBloodVerb(Critical); }
);

class CHARACTER
(
  darkknight,
  humanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(200);
    SetAgility(30);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(30);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 110000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(128,0); }
  virtual std::string NameSingular() const { return "dark knight"; }
  virtual float GetMeleeStrength() const { return 5000; }
);

class CHARACTER
(
  ennerbeast,
  character,
  InitMaterials(new ennerbeastflesh),
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual ulong CurrentDanger() { return character::CurrentDanger() * 5; }
  virtual ulong MaxDanger() { return character::MaxDanger() * 5; }
 protected:
  virtual std::string DeathMessage() { return Name(DEFINITE) + " dies and the world is finally freed from this terrible monster."; }
  virtual vector2d GetBitmapPos() const { return vector2d(96,0); }
  virtual std::string NameSingular() const { return "Enner Beast"; }
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
  InitMaterials(new darkfrogflesh),
  {
    SetSize(25);
    SetAgility(30);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(18);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 2500; else return 0; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual bool HasInfraVision() const { return true; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(80,0); }
  virtual std::string NameSingular() const { return "dark frog"; }
);

class CHARACTER
(
  elpuri,
  darkfrog,
  InitMaterials(new elpuriflesh),
  {
    SetSize(300);
    SetAgility(10);
    SetStrength(30);
    SetEndurance(50);
    SetPerception(30);
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 277500; else return 0; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual void BeTalkedTo(character*);
  virtual ushort CalculateArmorModifier() const { return 10; }
  virtual bool Hit(character*);
  virtual bool CanBeDisplaced() const { return false; }
 protected:
  virtual std::string DeathMessage() { return Name(DEFINITE) + " groans horribly and drops " + game::PossessivePronoun(GetSex()) + " head."; }
  virtual vector2d GetBitmapPos() const { return vector2d(64,0); }
  virtual std::string NameSingular() const { return "Elpuri the Dark Frog"; }
  virtual void CreateCorpse();
);

class CHARACTER
(
  billswill,
  character,
  InitMaterials(new air),
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 500000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual std::string StandVerb() const { return "floating"; }
  virtual ushort Frequency() const { return 100; }
  virtual bool CanFly() const { return true; }
 protected:
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
  InitMaterials(2, new bone, new leather),
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
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
  InitMaterials(2, new goblinoidflesh, new leather),
  {
    SetSize(100);
    SetAgility(15);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(18);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 25000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
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
  InitMaterials(new brownslime),
  {
    SetSize(100);
    SetAgility(2);
    SetStrength(4);
    SetEndurance(50);
    SetPerception(9);
  },
 public:
  virtual ulong GetBloodColor() const { return MAKE_RGB(7,155,0); }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 250000; else return 0; }
 protected:
  virtual std::string NameSingular() const { return "conical mommo slime"; }
  virtual vector2d GetBitmapPos() const { return vector2d(176,0); }

);

class CHARACTER
(
  flatmommo,
  mommo,
  InitMaterials(new brownslime),
  {
    SetSize(50);
    SetAgility(4);
    SetStrength(2);
    SetEndurance(25);
    SetPerception(9);
  },
 public:
  virtual ulong GetBloodColor() const { return MAKE_RGB(7,155,0); }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 150000; else return 0; }
 protected:
  virtual std::string NameSingular() const { return "flat mommo slime"; }
  virtual vector2d GetBitmapPos() const { return vector2d(192,0); }
);

class CHARACTER
(
  golem,
  character,
  InitMaterials(protosystem::CreateRandomSolidMaterial()),
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
  virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
  virtual void DrawToTileBuffer() const;
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual bool CheckForUsefulItemsOnGround() { return false; }
  virtual bool CanKick() const { return true; }
 protected:
  virtual std::string DeathMessage() { return "The Holy Words of " + Name(DEFINITE) + " fly away and the monster collapses."; }
  virtual std::string NameSingular() const { return "golem"; }
  virtual vector2d GetBitmapPos() const { return vector2d(256,0); }
  virtual float GetMeleeStrength() const;
);

class CHARACTER
(
  wolf,
  character,
  InitMaterials(new wolfflesh),
  {
    SetSize(100);
    SetAgility(20);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(24);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 40000; else return 0; }
 protected:
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
  InitMaterials(new dogflesh),
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
 protected:
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
  InitMaterials(new spiderflesh),
  {
    SetSize(10);
    SetAgility(5);
    SetStrength(2);
    SetEndurance(1);
    SetPerception(9);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100; else return 0; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanOpenDoors() const { return false; }
 protected:
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
  InitMaterials(new jackalflesh),
  {
    SetSize(80);
    SetAgility(10);
    SetStrength(3);
    SetEndurance(3);
    SetPerception(18);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 20000; else return 0; }
 protected:
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
  InitMaterials(new donkeyflesh),
  {
    SetSize(150);
    SetAgility(5);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(15);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 40000; else return 0; }
 protected:
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
  ivan,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(230);
    SetAgility(20);
    SetStrength(50);
    SetEndurance(50);
    SetPerception(18);
    SetLegType(5);
    SetTorsoType(9);
    SetArmType(7);
    SetHeadType(11);
    SetShieldType(0);
  },
 public:
  virtual bool MoveRandomly();
  virtual void CreateInitialEquipment();
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 120000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual bool HasInfraVision() const { return true; }
  virtual uchar CriticalModifier() const { return 4; }
 protected:
  virtual std::string DeathMessage() { return "Ivan falls groaning bravely: \"Party revenges Ivan!\""; }
  virtual std::string NameSingular() const { return "Ivan"; }
  virtual float GetMeleeStrength() const { return 5000; }
);

class CHARACTER
(
  hunter,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(180);
    SetAgility(20);
    SetStrength(15);
    SetEndurance(15);
    SetPerception(24);
    SetLegType(6);
    SetTorsoType(12);
    SetArmType(8);
    SetHeadType(12);
    SetShieldType(0);
  },
 public:
  virtual void CreateInitialEquipment();
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
  virtual void BeTalkedTo(character*);
 protected:
  virtual std::string NameSingular() const { return "hunter"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class CHARACTER
(
  polarbear,
  character,
  InitMaterials(new polarbearflesh),
  {
    SetSize(250);
    SetAgility(10);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(24);
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 500000; else return 0; }
 protected:
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
  InitMaterials(new dolphinflesh),
  {
    SetSize(300);
    SetAgility(30);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(30);
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 150000; else return 0; }
  virtual std::string StandVerb() const { return "swimming"; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual bool CanOpenDoors() const { return false; }
  virtual bool CanWalk() const { return false; }
  virtual bool CanSwim() const { return true; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
 protected:
  virtual std::string NameSingular() const { return "female dolphin in season"; }
  virtual float GetMeleeStrength() const { return 1000; }
  virtual vector2d GetBitmapPos() const { return vector2d(320,0); }
  virtual std::string TalkVerb() const { return "peeps passionately to you"; }
);

class CHARACTER
(
  lightfrog,
  frog,
  InitMaterials(new lightfrogflesh),
  {
    SetSize(25);
    SetAgility(30);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(18);
  },
 public:
  virtual bool CanBeGenerated() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 2500; else return 0; }
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
  virtual bool CanSwim() const { return true; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(80,0); }
  virtual std::string NameSingular() const { return "light frog"; }
);

class CHARACTER
(
  slave,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(160);
    SetAgility(10);
    SetStrength(20);
    SetEndurance(15);
    SetPerception(15);
    SetLegType(6);
    SetTorsoType(0);
    SetArmType(9);
    SetHeadType(13);
    SetShieldType(0);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual void GetAICommand();
 protected:
  virtual std::string NameSingular() const { return "slave"; }
);

class CHARACTER
(
  petrusswife,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(170);
    SetAgility(10);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
    SetLegType(7);
    SetTorsoType(10);
    SetArmType(10);
    SetHeadType(16 + RAND() % 6);
    SetShieldType(0);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 50000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
  virtual std::string Name(uchar Case) const { return NameProperNoun(Case); }
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
 protected:
  virtual std::string NameSingular() const { return "Petrus's wife"; }
  virtual float GetMeleeStrength() const { return 500; }
);

class CHARACTER
(
  petrusswife1,
  petrusswife,
  InitMaterials(new humanflesh),
  {
    petrusswife::SetDefaultStats();
    SetHeadType(16);
  },
 protected:
  virtual std::string NameSingular() const { return "Petrus's wife number 1"; }
);

class CHARACTER
(
  petrusswife2,
  petrusswife,
  InitMaterials(new humanflesh),
  {
    petrusswife::SetDefaultStats();
    SetHeadType(17);
  },
 protected:
  virtual std::string NameSingular() const { return "Petrus's wife number 2"; }
);

class CHARACTER
(
  petrusswife3,
  petrusswife,
  InitMaterials(new humanflesh),
  {
    petrusswife::SetDefaultStats();
    SetHeadType(18);
  },
 protected:
  virtual std::string NameSingular() const { return "Petrus's wife number 3"; }
);

class CHARACTER
(
  petrusswife4,
  petrusswife,
  InitMaterials(new humanflesh),
  {
    petrusswife::SetDefaultStats();
    SetHeadType(19);
  },
 protected:
  virtual std::string NameSingular() const { return "Petrus's wife number 4"; }
);

class CHARACTER
(
  petrusswife5,
  petrusswife,
  InitMaterials(new humanflesh),
  {
    petrusswife::SetDefaultStats();
    SetHeadType(20);
  },
 protected:
  virtual std::string NameSingular() const { return "Petrus's wife number 5"; }
);

class CHARACTER
(
  petrusswife6,
  petrusswife,
  InitMaterials(new humanflesh),
  {
    petrusswife::SetDefaultStats();
    SetHeadType(21);
  },
 protected:
  virtual std::string NameSingular() const { return "Petrus's wife number 6"; }
);

class CHARACTER
(
  housewife,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(160);
    SetAgility(15);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(24);
    SetLegType(8);
    SetTorsoType(11);
    SetArmType(11);
    SetHeadType(16 + RAND() % 6);
    SetShieldType(0);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 70000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
 protected:
  virtual std::string NameSingular() const { return "housewife"; }
  virtual float GetMeleeStrength() const { return 500; }
);

class CHARACTER
(
  femaleslave,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(170);
    SetAgility(10);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(18);
    SetLegType(9);
    SetTorsoType(13);
    SetArmType(13);
    SetHeadType(22);
    SetShieldType(0);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 40000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
  virtual void CreateInitialEquipment();
 protected:
  virtual std::string NameSingular() const { return "female slave"; }
  virtual float GetMeleeStrength() const { return 500; }
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  librarian,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(170);
    SetAgility(5);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(12);
    SetLegType(1);
    SetTorsoType(5);
    SetArmType(12);
    SetHeadType(14);
    SetShieldType(0);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
  virtual void BeTalkedTo(character*);
 protected:
  virtual std::string NameSingular() const { return "librarian"; }
  virtual float GetMeleeStrength() const { return 500; }
  virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
  zombie,
  complexhumanoid,
  InitMaterials(new humanflesh),
  {
    SetSize(160);
    SetAgility(5);
    SetStrength(10);
    SetEndurance(5);
    SetPerception(12);
    SetLegType(10);
    SetTorsoType(14);
    SetArmType(14);
    SetHeadType(23);
    SetShieldType(0);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual void BeTalkedTo(character*);
  virtual void SpillBlood(uchar, vector2d);
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 50000; else return 0; }
  virtual float GetMeleeStrength() const { return 1500; }
 protected:
  virtual std::string DeathMessage() { return Name(DEFINITE) + " is slain (again)."; }
  virtual std::string NameSingular() const { return "zombie"; }
);

class CHARACTER
(
  imp,
  character,
  InitMaterials(new sulfur),
  {
    SetSize(100);
    SetAgility(15);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(15);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 40000; else return 0; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(368,0); }
  virtual std::string NameSingular() const { return "imp"; }
  virtual float GetMeleeStrength() const { return 20000; }
);

class CHARACTER
(
  bat,
  character,
  InitMaterials(new batflesh),
  {
    SetSize(20);
    SetAgility(40);
    SetStrength(2);
    SetEndurance(1);
    SetPerception(24);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 1000; else return 0; }
  virtual std::string StandVerb() const { return "flying"; }
  virtual bool CanOpenDoors() const { return false; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(464,0); }
  virtual std::string NameSingular() const { return "bat"; }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "squeaks"; }
);

class CHARACTER
(
  mistress,
  humanoid,
  InitMaterials(new humanflesh),
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
  virtual uchar GetSex() const { return FEMALE; }
  virtual float GetMeleeStrength() const { return 5000; }
  virtual void CreateInitialEquipment();
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(352,0); }
  virtual std::string NameSingular() const { return "mistress"; }
);

class CHARACTER
(
  werewolf,
  complexhumanoid,
  InitMaterials(new werewolfflesh),
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
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
  InitMaterials(new koboldflesh),
  {
    SetSize(90);
    SetAgility(10);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(12);
  },
 public:
  virtual bool CanBeGenerated() const { return true; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
  virtual void CreateInitialEquipment();
 protected:
  virtual std::string DeathMessage() { return Name(DEFINITE) + " dies yelling like a tortured hyena."; }
  virtual vector2d GetBitmapPos() const { return vector2d(480,0); }
  virtual std::string NameSingular() const { return "kobold"; }
  virtual float GetMeleeStrength() const { return 2000; }
);

class CHARACTER
(
  gibberling,
  character,
  InitMaterials(new gibberlingflesh),
  {
    SetSize(90);
    SetAgility(20);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(15);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(448,0); }
  virtual std::string NameSingular() const { return "gibberling"; }
  virtual float GetMeleeStrength() const { return 10000; }
);

class CHARACTER  
(        
  largecat,
  character,
  InitMaterials(new catflesh),
  {
    SetSize(60);
    SetAgility(25);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 15000; else return 0; }
  virtual bool Catches(item*, float);
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(496,0); }
  virtual std::string NameSingular() const { return "large cat"; }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "mews"; }
);

class CHARACTER  
(        
  largerat,
  character,
  InitMaterials(new ratflesh),
  {
    SetSize(30);
    SetAgility(10);
    SetStrength(3);
    SetEndurance(2);
    SetPerception(12);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 2000; else return 0; }
  virtual bool CanOpenDoors() const { return false; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(512,0); }
  virtual std::string NameSingular() const { return "large rat"; }
  virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "squeaks"; }
);

class CHARACTER  
(        
  angel,
  humanoid,
  InitMaterials(new angelflesh),
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
  virtual uchar GetSex() const { return FEMALE; }
  virtual void SetMaster(uchar);
  virtual uchar GetMaster() const { return Master; }
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "an") + OwnerGodDescription(Master); }
  virtual bool CanFly() const { return true; }
 protected:
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
  complexhumanoid,
 public:
  virtual void DrawLegs(vector2d) const;
  virtual void DrawHead(vector2d) const;
);

class CHARACTER
(
  kamikazedwarf,
  dwarf,
  InitMaterials(new dwarfflesh),
  {
    SetSize(130);
    SetAgility(20);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(24);
    SetLegType(13);
    SetTorsoType(17);
    SetArmType(16);
    SetHeadType(26);
    SetShieldType(0);
    SetMaster(1 + RAND() % game::GetGodNumber());
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual bool CanBeGenerated() const { return true; }
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
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
  virtual std::string DeathMessage() { return Name(DEFINITE) + " dies smiling."; }
  virtual vector2d GetBitmapPos() const { return vector2d(400,0); }
  virtual std::string NameSingular() const { return "kamikaze dwarf"; }
  uchar Master;
);

class CHARACTER  
(        
  mammoth,
  character,
  InitMaterials(new mammothflesh),
  {
    SetSize(500);
    SetAgility(20);
    SetStrength(80);
    SetEndurance(80);
    SetPerception(18);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 500000; else return 0; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(528,0); }
  virtual std::string NameSingular() const { return "mammoth"; }
  virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "roars"; }
);

class CHARACTER
(
  unicorn,
  character,
  RandomizeFleshMaterial(),
  {
    SetSize(200);
    SetAgility(40);
    SetStrength(10);
    SetEndurance(25);
    SetPerception(18);
  },
 public:
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 200000; else return 0; }
  virtual vector2d GetBitmapPos() const { return vector2d(544, 0); }
  virtual std::string NameSingular() const;
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
  uchar Alignment;
);

class CHARACTER
(
  genie,
  humanoid,
  InitMaterials(3, new air, new air, new air),
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
  virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 200000; else return 0; }
  virtual void BeTalkedTo(character*);
  virtual std::string StandVerb() const { return "floating"; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(416,0); }
  virtual std::string NameSingular() const { return "genie"; }
  virtual void CreateCorpse() { }
  virtual std::string DeathMessage() { return Name(DEFINITE) + " vanishes from existence."; }
  virtual float GetMeleeStrength() const { return 5000; }
);

#endif


