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
	virtual void DrawToTileBuffer() const;
	virtual bool WearArmor();
	virtual item* GetTorsoArmor() const RET(Armor.Torso)
	virtual bool SetTorsoArmor(item* What) { Armor.Torso = What; return true;}
	virtual uchar GetSex() const RET(MALE)
	virtual ushort CalculateArmorModifier() const;
	virtual bool Drop();
	virtual bool Wield();
	virtual void Save(outputfile&) const;
	virtual bool Throw();
	virtual uchar GetArmType() const RET(ArmType)
	virtual uchar GetHeadType() const RET(HeadType)
	virtual bool CanWield() const RET(true)
	virtual bool CanWear() const RET(true)
	virtual bool Apply();
	virtual void SetArmType(uchar Value) { ArmType = Value; }
	virtual void SetHeadType(uchar Value) { HeadType = Value; }
	virtual void SetLegType(uchar Value) { LegType = Value; }
	virtual void SetTorsoType(uchar Value) { TorsoType = Value; }
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
	uchar ArmType;
	uchar HeadType;
	uchar LegType;
	uchar TorsoType;
);

inline humanoid::armor::armor() : Torso(0)/*, Legs(0), Hands(0), Head(0), Feet(0)*/ {}

class CHARACTER
(
	human,
	humanoid,
	InitMaterials(new humanflesh),
	{
		SetSize(150 + rand() % 51);
		SetAgility(15 + rand() % 11);
		SetStrength(10 + rand() % 6);
		SetEndurance(10 + rand() % 6);
		SetPerception(10 + rand() % 6);
		SetArmType(rand() % NUMBER_OF_HUMAN_ARMS);
		SetHeadType(rand() % NUMBER_OF_HUMAN_HEADS);
		SetLegType(rand() % NUMBER_OF_HUMAN_LEGS);
		SetTorsoType(rand() % NUMBER_OF_HUMAN_TORSOS);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
protected:
	virtual std::string NameSingular() const RET("human")
);
                      
class CHARACTER
(
	perttu,
	human,
	InitMaterials(new humanflesh),
	{
		SetSize(225);
		SetAgility(75);
		SetStrength(75);
		SetEndurance(75);
		SetPerception(75);
		SetArmType(3);
		SetHeadType(10);
		SetLegType(1);
		SetTorsoType(5);
		SetHealTimer(100);
	},
public:
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
	virtual ushort Possibility() const RET(0)
	virtual bool Polymorph() RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 80000; else return 0; }
	virtual void AddHitMessage(character*, const bool = false) const;
protected:
	virtual std::string NameSingular() const RET("Perttu, the Überpriest of the Great Frog")
	virtual void CreateCorpse();
	virtual std::string DeathMessage() { return Name(DEFINITE) + " disappears in a bright light and his left nut is left behind."; }
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(20000)
	virtual void GetAICommand();
	ushort HealTimer;
);

class CHARACTER
(
	farmer,
	human,
	InitMaterials(new humanflesh),
	{
		SetSize(170);
		SetAgility(10);
		SetStrength(10);
		SetEndurance(10);
		SetPerception(12);
		SetArmType(rand() % 7);
		SetHeadType(4 + rand() % 3);
		SetLegType(rand() % 3);
		SetTorsoType(2 + rand() % 2);
	},
public:
	virtual void CreateInitialEquipment();
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
protected:
	virtual std::string NameSingular() const RET("farmer")
);

class CHARACTER
(
	cityguard,
	human,
	InitMaterials(new humanflesh),
	{
		SetSize(180);
		SetAgility(20);
		SetStrength(20);
		SetEndurance(20);
		SetPerception(21);
		SetArmType(5);
		SetHeadType(7);
		SetLegType(3);
		SetTorsoType(1);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual void GetAICommand() { SoldierAICommand(); }
	virtual void CreateInitialEquipment();
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
protected:
	virtual std::string NameSingular() const RET("city guard")
);

class CHARACTER
(
	shopkeeper,
	human,
	InitMaterials(new humanflesh),
	{
		SetSize(160);
		SetAgility(10);
		SetStrength(30);
		SetEndurance(30);
		SetPerception(30);
		SetArmType(6);
		SetHeadType(9);
		SetLegType(2);
		SetTorsoType(8);
	},
public:
	virtual void CreateInitialEquipment();
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100000; else return 0; }
protected:
	virtual std::string NameSingular() const RET("shopkeeper")
	virtual float GetMeleeStrength() const RET(2000)
);

class CHARACTER
(
	oree,
	character,
	InitMaterials(new pepsi),
	{
		SetSize(225);
		SetAgility(50);
		SetStrength(30);
		SetEndurance(30);
		SetPerception(30);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual uchar GetSex() const RET(MALE)
	virtual ushort CalculateArmorModifier() const RET(10)
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
	virtual bool CanKick() const RET(true)
	virtual ulong GetBloodColor() const RET(BLACK)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 110000; else return 0; }
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
	swatcommando,
	character,
	InitMaterials(new humanflesh),
	{
		SetSize(200);
		SetAgility(30);
		SetStrength(20);
		SetEndurance(30);
		SetPerception(30);
	},
public:
	virtual ushort Possibility() const RET(5)
	virtual uchar GetSex() const RET(MALE)
	virtual bool CanWield() const RET(true)
	virtual bool Charmable() const RET(false)
	virtual bool CanKick() const RET(true)
	virtual ushort CalculateArmorModifier() const RET(50)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 110000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(128,0)
	virtual std::string NameSingular() const RET("Bill's SWAT commando")
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
	virtual ushort Possibility() const RET(0)
	virtual bool Hit(character*);
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
	virtual bool CanKick() const RET(true)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(96,0)
	virtual std::string NameSingular() const RET("Enner Beast")
	virtual float GetMeleeStrength() const RET(50000)
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
);

class CHARACTER
(
	darkfrog,
	frog,
	InitMaterials(new darkfrogflesh),
	{
		SetSize(15);
		SetAgility(30);
		SetStrength(5);
		SetEndurance(5);
		SetPerception(18);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 2500; else return 0; }
	virtual ulong GetBloodColor() const RET(BLACK)
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
	virtual std::string DeathMessage() { return Name(DEFINITE) + " groans horribly and drops " + game::PossessivePronoun(GetSex()) + " head."; }
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 277500; else return 0; }
	virtual ulong GetBloodColor() const RET(BLACK)
protected:
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
	virtual ushort Possibility() const RET(50)
	virtual void SpillBlood(uchar) {}
	virtual bool Charmable() const RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 500000; else return 0; }	
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
	fallenvalpurist,
	character,
	InitMaterials(new bone),
	{
		SetSize(150);
		SetAgility(10);
		SetStrength(5);
		SetEndurance(10);
		SetPerception(15);
	},
public:
	virtual std::string DeathMessage() { return Name(DEFINITE) + " is transformed into a crumpled heap of bones."; }
	virtual ushort Possibility() const RET(50)
	virtual bool CanWield() const RET(true)
	virtual bool CanKick() const RET(true)
	virtual void SpillBlood(uchar) {}
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(112,0)
	virtual std::string NameSingular() const RET("fallen valpurist")
	virtual void CreateCorpse();
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(1000)
);

class CHARACTER
(
	froggoblin,
	character,
	InitMaterials(new goblinoidflesh),
	{
		SetSize(100);
		SetAgility(15);
		SetStrength(10);
		SetEndurance(15);
		SetPerception(18);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual bool CanKick() const RET(true)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 25000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(144,0)
	virtual std::string NameSingular() const RET("frog-goblin hybrid")
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(2000)
);

class ABSTRACT_CHARACTER
(
	mommo,
	character,
protected:
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBrownSlimeVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual float GetMeleeStrength() const RET(20000)
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
	virtual ushort Possibility() const RET(25)
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
	virtual ushort Possibility() const RET(75)
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
	virtual std::string DeathMessage() { return "The Holy Words of " + Name(DEFINITE) + " fly away and the monster collapses."; }
	virtual ushort Possibility() const RET(20)
	virtual ushort CalculateArmorModifier() const;
	virtual void MoveRandomly();
	virtual std::string Name(uchar Case) const RET(NameWithMaterial(Case))
	virtual void DrawToTileBuffer() const;
	virtual void SpillBlood(uchar) {}
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100000; else return 0; }
protected:
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
	virtual ushort Possibility() const RET(40)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 40000; else return 0; }
protected:
	virtual std::string NameSingular() const RET("wolf")
	virtual std::string NamePlural() const RET("wolves")
	virtual vector2d GetBitmapPos() const RETV(224,0)
	virtual float GetMeleeStrength() const RET(7500)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
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
	virtual ushort Possibility() const RET(20)
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
	virtual ushort Possibility() const RET(100)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100; else return 0; }
	virtual ulong GetBloodColor() const RET(BLACK)
protected:
	virtual vector2d GetBitmapPos() const RETV(272,0)
	virtual std::string NameSingular() const RET("spider")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(10000)
	virtual void CreateCorpse() {}
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
	virtual ushort Possibility() const RET(100)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 20000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(304,0)
	virtual std::string NameSingular() const RET("jackal")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))																																																																																																																																													//Jackals are unoriginal
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(5000)
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
	virtual ushort Possibility() const RET(100)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 40000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(288,0)
	virtual std::string NameSingular() const RET("donkey")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(1000)
);

#endif
