#ifndef __CHARDE_H__
#define __CHARDE_H__

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
	virtual item* GetTorsoArmor() const RET(Armor.Torso)
	virtual bool SetTorsoArmor(item* What) { Armor.Torso = What; return true;}
	virtual uchar GetSex() const RET(MALE)
	virtual ushort CalculateArmorModifier() const;
	virtual void Save(outputfile&) const;
	virtual bool CanWield() const RET(true)
	virtual bool CanWear() const RET(true)
	virtual bool CanKick() const RET(true)
	virtual float GetAttackStrength() const;
	virtual bool Hit(character*);
	virtual gweaponskill* GetCategoryWeaponSkill(uchar Index) const { return CategoryWeaponSkill[Index]; }
	virtual void CharacterSpeciality();
	virtual bool ShowWeaponSkills();
	virtual void SetWielded(item*);
	virtual sweaponskill* GetCurrentSingleWeaponSkill() const { return CurrentSingleWeaponSkill; }
	virtual void SetCurrentSingleWeaponSkill(sweaponskill* What) { CurrentSingleWeaponSkill = What; }
	virtual sweaponskill* GetSingleWeaponSkill(ushort Index) const { return SingleWeaponSkill[Index]; }
	virtual void SetSingleWeaponSkill(ushort Index, sweaponskill* What) { SingleWeaponSkill[Index] = What; }
	virtual float GetToHitValue() const;
	virtual void ReceiveSound(char*, short, float);
	virtual long StatScore() const;
	virtual void AddSpecialItemInfo(std::string&, item*);
	virtual void AddSpecialItemInfoDescription(std::string&);
	virtual void KickHit();
	virtual bool CanBeGenerated() const { return false; }
protected:
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual float GetMeleeStrength() const RET(1000)
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

inline humanoid::armor::armor() : Torso(0)/*, Legs(0), Hands(0), Head(0), Feet(0)*/ {}

class ABSTRACT_CHARACTER
(
	complexhumanoid,
	humanoid,
public:
	virtual void DrawToTileBuffer() const;
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual uchar GetLegType() const RET(LegType)
	virtual uchar GetTorsoType() const RET(TorsoType)
	virtual uchar GetArmType() const RET(ArmType)
	virtual uchar GetHeadType() const RET(HeadType)
	virtual uchar GetShieldType() const RET(ShieldType)
	virtual void SetLegType(uchar Value) { LegType = Value; }
	virtual void SetTorsoType(uchar Value) { TorsoType = Value; }
	virtual void SetArmType(uchar Value) { ArmType = Value; }
	virtual void SetHeadType(uchar Value) { HeadType = Value; }
	virtual void SetShieldType(uchar Value) { ShieldType = Value; }
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
	virtual std::string NameSingular() const RET("human")
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
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual void BeTalkedTo(character*);
	virtual ushort GetEmitation() const RET(333)
	virtual void HealFully(character*);
	virtual void SetHealTimer(ushort What) { HealTimer = What; }
	virtual ushort GetHealTimer() RET(HealTimer)
	virtual void ReceiveFireDamage(long) {}
	virtual void Save(outputfile&) const;
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
	virtual void AddHitMessage(character*, const bool = false) const;
	virtual uchar GetStoryState() const { return StoryState; }
	virtual void SetStoryState(uchar What) { StoryState = What; }
	virtual bool HasInfraVision() const { return true; }
	virtual bool CanBeDisplaced() const { return false; }
protected:
	virtual std::string NameSingular() const RET("Petrus, the Überpriest of the Great Frog")
	virtual void CreateCorpse();
	virtual std::string DeathMessage() { return "The Überpriest disappears in a bright light and his left nut is left behind."; }
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(20000)
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
	virtual std::string NameSingular() const RET("farmer")
);

class CHARACTER
(
	guard,
	complexhumanoid,
	InitMaterials(new humanflesh),
	{
		SetSize(180);
		SetAgility(15);
		SetStrength(25);
		SetEndurance(30);
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
	virtual std::string NameSingular() const RET("guard")
	virtual float GetMeleeStrength() const RET(2000)
);

class CHARACTER
(
	shopkeeper,
	complexhumanoid,
	InitMaterials(new humanflesh),
	{
		SetSize(160);
		SetAgility(10);
		SetStrength(25);
		SetEndurance(30);
		SetPerception(25);
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
protected:
	virtual std::string NameSingular() const RET("shopkeeper")
	virtual float GetMeleeStrength() const RET(2000)
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
protected:
	virtual std::string NameSingular() const RET("priest")
);

class CHARACTER
(
	oree,
	humanoid,
	InitMaterials(2, new pepsi, new leather),
	{
		SetSize(225);
		SetAgility(50);
		SetStrength(30);
		SetEndurance(30);
		SetPerception(30);
	},
public:
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual uchar GetSex() const RET(MALE)
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
	virtual bool CanKick() const RET(true)
	virtual ulong GetBloodColor() const RET(BLACK)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 110000; else return 0; }
	virtual void BeTalkedTo(character*);
	virtual bool HasInfraVision() const { return true; }
	virtual bool CanBeDisplaced() const { return false; }
protected:
	virtual vector2d GetBitmapPos() const RETV(208,0)
	virtual std::string NameSingular() const RET("Oree the Pepsi Daemon King")
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(30000)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonPepsiVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonPepsiVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonPepsiVerb(Critical))
);

class CHARACTER
(
	darkknight,
	humanoid,
	InitMaterials(new humanflesh),
	{
		SetSize(200);
		SetAgility(30);
		SetStrength(25);
		SetEndurance(30);
		SetPerception(30);
	},
public:
	virtual bool CanBeGenerated() const { return true; }
	virtual uchar GetSex() const RET(MALE)
	virtual bool CanWield() const RET(true)
	virtual bool CanKick() const RET(true)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 110000; else return 0; }
	virtual void BeTalkedTo(character*);
protected:
	virtual vector2d GetBitmapPos() const RETV(128,0)
	virtual std::string NameSingular() const RET("dark knight")
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(5000)
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
	virtual bool Polymorph() const RET(false)
	virtual bool CanKick() const RET(true)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
	virtual void BeTalkedTo(character*);
protected:
	virtual vector2d GetBitmapPos() const RETV(96,0)
	virtual std::string NameSingular() const RET("Enner Beast")
	virtual float GetMeleeStrength() const RET(100000)
	virtual void GetAICommand();
	virtual void CreateCorpse();
);

class ABSTRACT_CHARACTER
(
	frog,
	character,
protected:
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(20000)
	virtual std::string TalkVerb() const { return "croaks"; }
	virtual bool CanOpenDoors(void) const { return false; }
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
	virtual ulong GetBloodColor() const RET(BLACK)
	virtual bool HasInfraVision() const { return true; }
protected:
	virtual vector2d GetBitmapPos() const RETV(80,0)
	virtual std::string NameSingular() const RET("dark frog")
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
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 277500; else return 0; }
	virtual ulong GetBloodColor() const RET(BLACK)
	virtual void BeTalkedTo(character*);
	virtual ushort CalculateArmorModifier() const RET(10)
	virtual bool Hit(character*);
	virtual bool CanBeDisplaced() const { return false; }
protected:
	virtual std::string DeathMessage() { return Name(DEFINITE) + " groans horribly and drops " + game::PossessivePronoun(GetSex()) + " head."; }
	virtual vector2d GetBitmapPos() const RETV(64,0)
	virtual std::string NameSingular() const RET("Elpuri the Dark Frog")
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
	virtual void SpillBlood(uchar) {}
	virtual void SpillBlood(uchar, vector2d) {}
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 500000; else return 0; }
	virtual void BeTalkedTo(character*);
	virtual bool HasInfraVision() const { return true; }
	virtual std::string StandVerb() const { return "floating"; }
protected:
	virtual vector2d GetBitmapPos() const RETV(48,0)
	virtual std::string NameSingular() const RET("pure mass of Bill's will")
	virtual std::string NamePlural() const RET("pure masses of Bill's will")
	virtual void CreateCorpse() {}
	virtual std::string DeathMessage() { return Name(DEFINITE) + " vanishes from existence."; }
	virtual float GetMeleeStrength() const RET(20000)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonPSIVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonPSIVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonPSIVerb(Critical))
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
	virtual bool CanWield() const RET(true)
	virtual bool CanKick() const RET(true)
	virtual void SpillBlood(uchar) {}
	virtual void SpillBlood(uchar, vector2d) {}
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
	virtual void BeTalkedTo(character*);
protected:
	virtual std::string DeathMessage() { return Name(DEFINITE) + " is transformed into a crumpled heap of bones."; }
	virtual vector2d GetBitmapPos() const RETV(112,0)
	virtual std::string NameSingular() const RET("skeleton")
	virtual void CreateCorpse();
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(1000)
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
	virtual bool CanWield() const RET(true)
	virtual bool CanKick() const RET(true)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 25000; else return 0; }
	virtual void BeTalkedTo(character*);
protected:
	virtual vector2d GetBitmapPos() const RETV(144,0)
	virtual std::string NameSingular() const RET("goblin")
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(2000)
);

class ABSTRACT_CHARACTER
(
	mommo,
	character,
public:
	virtual std::string StandVerb() const { return "bubbling"; }
protected:
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBrownSlimeVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual float GetMeleeStrength() const RET(20000)
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
	virtual ulong GetBloodColor() const RET(MAKE_RGB(7,155,0))
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 250000; else return 0; }
protected:
	virtual std::string NameSingular() const RET("conical mommo slime")
	virtual vector2d GetBitmapPos() const RETV(176,0)

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
	virtual ulong GetBloodColor() const RET(MAKE_RGB(7,155,0))
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 150000; else return 0; }
protected:
	virtual std::string NameSingular() const RET("flat mommo slime")
	virtual vector2d GetBitmapPos() const RETV(192,0)
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
	virtual void MoveRandomly();
	virtual std::string Name(uchar Case) const RET(NameWithMaterial(Case))
	virtual void DrawToTileBuffer() const;
	virtual void SpillBlood(uchar) {}
	virtual void SpillBlood(uchar, vector2d) {}
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100000; else return 0; }
	virtual void BeTalkedTo(character*);
	virtual bool CheckForUsefulItemsOnGround() { return false; }
	virtual bool CanKick() const RET(true)
protected:
	virtual std::string DeathMessage() { return "The Holy Words of " + Name(DEFINITE) + " fly away and the monster collapses."; }
	virtual std::string NameSingular() const RET("golem")
	virtual vector2d GetBitmapPos() const RETV(256,0)
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
	virtual std::string NameSingular() const RET("wolf")
	virtual std::string NamePlural() const RET("wolves")
	virtual vector2d GetBitmapPos() const RETV(224,0)
	virtual float GetMeleeStrength() const RET(7500)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
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
		SetStrength(8);
		SetEndurance(8);
		SetPerception(21);
	},
public:
	virtual bool Catches(item*, float);
	virtual bool ConsumeItemType(uchar) const;
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(240,0)
	virtual std::string NameSingular() const RET("dog")
	virtual float GetMeleeStrength() const RET(5000)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
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
	virtual ulong GetBloodColor() const RET(BLACK)
	virtual bool HasInfraVision() const { return true; }
protected:
	virtual vector2d GetBitmapPos() const RETV(272,0)
	virtual std::string NameSingular() const RET("spider")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(10000)
	virtual void CreateCorpse() {}
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
		SetStrength(5);
		SetEndurance(5);
		SetPerception(18);
	},
public:
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 20000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(304,0)
	virtual std::string NameSingular() const RET("jackal")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))																																																																																																																																													//Jackals are unoriginal
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(5000)
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
	virtual vector2d GetBitmapPos() const RETV(288,0)
	virtual std::string NameSingular() const RET("mutant donkey")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(1000)
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
		SetStrength(30);
		SetEndurance(50);
		SetPerception(18);
		SetLegType(5);
		SetTorsoType(9);
		SetArmType(7);
		SetHeadType(11);
		SetShieldType(0);
	},
public:
	virtual void MoveRandomly();
	virtual void CreateInitialEquipment();
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 120000; else return 0; }
	virtual void BeTalkedTo(character*);
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual bool HasInfraVision() const { return true; }
	virtual uchar CriticalModifier() const { return 4; }
protected:
	virtual std::string DeathMessage() { return "Ivan falls groaning bravely: \"Party revenges Ivan!\""; }
	virtual std::string NameSingular() const RET("Ivan")
	virtual float GetMeleeStrength() const RET(5000)
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
	virtual std::string NameSingular() const RET("hunter")
	virtual float GetMeleeStrength() const RET(2000)
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
	virtual std::string NameSingular() const RET("polar bear")
	virtual float GetMeleeStrength() const RET(10000)
	virtual vector2d GetBitmapPos() const RETV(336,0)
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
	virtual void GetAICommand() {}
	virtual std::string StandVerb() const { return "swimming"; }
	virtual bool CanBeDisplaced() const { return false; }
protected:
	virtual std::string NameSingular() const RET("female dolphin in season")
	virtual float GetMeleeStrength() const RET(1000)
	virtual vector2d GetBitmapPos() const RETV(320,0)
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
	virtual void MoveRandomly() { MoveRandomlyInRoom(); }
protected:
	virtual vector2d GetBitmapPos() const RETV(80,0)
	virtual std::string NameSingular() const RET("light frog")
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
	virtual std::string NameSingular() const RET("slave")
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
	virtual uchar GetSex() const RET(FEMALE)
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual void MoveRandomly() { MoveRandomlyInRoom(); }
protected:
	virtual std::string NameSingular() const RET("Petrus's wife")
	virtual float GetMeleeStrength() const RET(500)
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
	virtual std::string NameSingular() const RET("Petrus's wife number 1")
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
	virtual std::string NameSingular() const RET("Petrus's wife number 2")
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
	virtual std::string NameSingular() const RET("Petrus's wife number 3")
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
	virtual std::string NameSingular() const RET("Petrus's wife number 4")
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
	virtual std::string NameSingular() const RET("Petrus's wife number 5")
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
	virtual std::string NameSingular() const RET("Petrus's wife number 6")
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
	virtual uchar GetSex() const RET(FEMALE)
protected:
	virtual std::string NameSingular() const RET("housewife")
	virtual float GetMeleeStrength() const RET(500)
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
	virtual void CreateInitialEquipment();
	virtual uchar GetSex() const RET(FEMALE)
protected:
	virtual std::string NameSingular() const RET("female slave")
	virtual float GetMeleeStrength() const RET(500)
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
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
	virtual void BeTalkedTo(character*);
protected:
	virtual std::string NameSingular() const RET("librarian")
	virtual float GetMeleeStrength() const RET(500)
	virtual void GetAICommand() { StandIdleAI(); }
);

class CHARACTER
(
	zombie,
	complexhumanoid,
	InitMaterials(new humanflesh),
	{
		SetSize(160);
		SetAgility(10);
		SetStrength(8);
		SetEndurance(5);
		SetPerception(15);
		SetLegType(0);		// Needs GFX
		SetTorsoType(0);	// Needs GFX
		SetArmType(0);		// Needs GFX
		SetHeadType(0);		// Needs GFX
		SetShieldType(0);	// Needs GFX
	},
public:
	virtual bool CanBeGenerated() const { return true; }
	virtual void BeTalkedTo(character*);
	virtual void SpillBlood(uchar, vector2d);
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
protected:
	virtual std::string NameSingular() const RET("zombie")
);


class CHARACTER
(
	imp,
	character,
	InitMaterials(new impflesh),
	{
		SetSize(50);
		SetAgility(25);
		SetStrength(10);
		SetEndurance(7);
		SetPerception(12);
	},
public:
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(368,0)
	virtual std::string NameSingular() const RET("imp")
	virtual float GetMeleeStrength() const RET(15000)
);

class CHARACTER
(
	bat,
	character,
	InitMaterials(new batflesh),
	{
		SetSize(20);
		SetAgility(45);
		SetStrength(2);
		SetEndurance(1);
		SetPerception(25);
	},
public:
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 3000; else return 0; }
	virtual std::string StandVerb() const { return "flying"; }
	virtual bool CanOpenDoors(void) const { return false; }
	virtual bool HasInfraVision() const { return true; }
protected:
	virtual vector2d GetBitmapPos() const RETV(464,0)
	virtual std::string NameSingular() const RET("bat")
	virtual float GetMeleeStrength() const RET(5000)
);


class CHARACTER
(
	mistress,
	character,
	InitMaterials(new humanflesh),
	{
		SetSize(180);
		SetAgility(20);
		SetStrength(25);
		SetEndurance(50);
		SetPerception(10);
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
	virtual bool CanWield() const RET(true)
	virtual uchar GetSex() const RET(FEMALE)
protected:
	virtual vector2d GetBitmapPos() const RETV(352,0)
	virtual void CreateInitialEquipment();
	virtual std::string NameSingular() const RET("mistress")
);


class CHARACTER
(
	werewolf,
	human,
	InitMaterials(new werewolfflesh),
	{
		SetMoney(RAND() % 30);
		SetChangeCounter(0);
		if(RAND() % 2) 
			ChangeIntoHuman();
		else
			ChangeIntoWolf();
	},
public:
	virtual void Load(inputfile&);
	virtual void Save(outputfile&) const;
	virtual ushort Possibility() const RET(125)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
	virtual bool HasInfraVision() const { return GetIsWolf(); }
	virtual void ChangeIntoHuman();
	virtual void ChangeIntoWolf();
	virtual void Be();
	virtual bool GetIsWolf() const { return IsWolf; } 
	virtual void SetIsWolf(bool What) { IsWolf = What; }
	virtual void SetChangeCounter(ushort What) { ChangeCounter = What; }
	virtual ushort GetChangeCounter() { return ChangeCounter; }
	virtual ulong werewolf::MaxDanger();
	virtual bool CanWield() const { return !GetIsWolf(); } 
protected:
	virtual std::string NameSingular() const RET("werewolf")
	virtual float GetMeleeStrength() const;
	bool IsWolf;
	unsigned short ChangeCounter;
);


class CHARACTER
(
	kobold,
	character,
	InitMaterials(new koboldflesh),
	{
		SetSize(70);
		SetAgility(12);
		SetStrength(8);
		SetEndurance(6);
		SetPerception(12);
	},
public:
	virtual ushort Possibility() const RET(75)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 90000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(448,0)
	virtual std::string NameSingular() const RET("kobold")
	virtual float GetMeleeStrength() const RET(15000)
);

class CHARACTER	// it should be noted that I no idea what 
(				// a gibberling is.... sooooo....
	gibberling,
	character,
	InitMaterials(new gibberlingflesh),
	{
		SetSize(90);
		SetAgility(5);
		SetStrength(4);
		SetEndurance(3);
		SetPerception(12);
	},
public:
	virtual ushort Possibility() const RET(125)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 70000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(480,0)
	virtual std::string NameSingular() const RET("gibberling")
	virtual float GetMeleeStrength() const RET(5000)
);

class CHARACTER	
(				
	largecat,
	character,
	InitMaterials(new catflesh),
	{
		SetSize(60);
		SetAgility(23);
		SetStrength(8);
		SetEndurance(6);
		SetPerception(20);
	},
public:
	virtual ushort Possibility() const RET(125)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(496,0)
	virtual std::string NameSingular() const RET("large cat")
	virtual float GetMeleeStrength() const RET(15000)
);


class CHARACTER	
(				
	largerat,
	character,
	InitMaterials(new ratflesh),
	{
		SetSize(30);
		SetAgility(8);
		SetStrength(3);
		SetEndurance(2);
		SetPerception(10);
	},
public:
	virtual ushort Possibility() const RET(150)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 3000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(512,0)
	virtual std::string NameSingular() const RET("large rat")
	virtual float GetMeleeStrength() const RET(4000)
);



class CHARACTER	
(				
	angel,
	character,
	InitMaterials(2, new angelflesh, new pork), // 2nd is temp material.
	{
		SetSize(30);
		SetAgility(60);
		SetStrength(70);
		SetEndurance(80);
		SetPerception(40);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(432,0)
	virtual std::string NameSingular() const RET("angel")
	virtual float GetMeleeStrength() const RET(40000)
);



#endif
