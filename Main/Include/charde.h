#ifndef __CHARDE_H__
#define __CHARDE_H__

#include "charba.h"
#include "graphics.h"

class weaponskill
{
public:
	weaponskill() : Hits(0), LastHit(0) {}
	void Turn() {}
	uchar GetLevel();
	ulong GetHits();
	ulong GetLastHit();
	void AddHit() { Hits++; }
private:
	uchar Level;
	ulong Hits;
	ulong LastHit;
};

class ABSTRACT_CHARACTER
(
	humanoid,
	character,
public:
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
protected:
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual float GetMeleeStrength() const RET(2000)
	struct armor
	{
		armor();
		item* Torso;
		item* Legs;
		item* Hands;
		item* Head;
		item* Feet;
	} Armor;
	struct weaponskills
	{
		weaponskill LargeSwords;
		weaponskill Maces;
		weaponskill Axes;
		weaponskill Spears;
	} WeaponSkills;
	uchar ArmType;
	uchar HeadType;
	uchar LegType;
	uchar TorsoType;
);

inline humanoid::armor::armor() : Torso(0), Legs(0), Hands(0), Head(0), Feet(0) {}

class CHARACTER
(
	human,
	humanoid,
	InitMaterials(new humanflesh(80000)),
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
	virtual ulong Danger() const RET(0)
	virtual ushort Possibility() const RET(0)
protected:
	virtual std::string NameSingular() const RET("human")
	virtual std::string NamePlural() const RET("humans")
);
                      
class CHARACTER
(
	perttu,
	human,
	InitMaterials(new humanflesh(80000)),
	{
		SetSize(200);
		SetAgility(80);
		SetStrength(80);
		SetEndurance(80);
		SetPerception(80);
		SetArmType(3);
		SetHeadType(10);
		SetLegType(1);
		SetTorsoType(5);
		SetRelations(1);
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
	virtual ulong Danger() const RET(150000)
	virtual bool Charmable() const RET(false)
	virtual ushort Possibility() const RET(0)
	virtual bool Polymorph() RET(false)
protected:
	virtual std::string NameSingular() const RET("Perttu, the Überpriest of the Great Frog")
	virtual std::string NamePlural() const RET("Perttus, the Überpriests of the Great Frog")
	virtual void CreateCorpse();
	virtual std::string DeathMessage() { return Name(DEFINITE) + " disappears in a bright light and his left nut is left behind."; }
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(10000)
	virtual void NeutralAICommand();
	ushort HealTimer;
);

class CHARACTER
(
	farmer,
	human,
	InitMaterials(new humanflesh(30000)),
	{
		SetSize(170);
		SetAgility(5);
		SetStrength(15);
		SetEndurance(10);
		SetPerception(9);
		SetArmType(rand() % 7);
		SetHeadType(4 + rand() % 3);
		SetLegType(rand() % 3);
		SetTorsoType(2 + rand() % 2);
		SetRelations(1);
	},
public:
	virtual ushort Possibility() const RET(0)
protected:
	virtual std::string NameSingular() const RET("farmer")
	virtual std::string NamePlural() const RET("farmers")
	virtual float GetMeleeStrength() const RET(1000)
);

class CHARACTER
(
	cityguard,
	human,
	InitMaterials(new humanflesh(40000)),
	{
		SetSize(180);
		SetAgility(20);
		SetStrength(20);
		SetEndurance(15);
		SetPerception(21);
		SetArmType(5);
		SetHeadType(7);
		SetLegType(3);
		SetTorsoType(1);
		SetRelations(1);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual void NeutralAICommand() { SoldierAICommand(); }
protected:
	virtual std::string NameSingular() const RET("city guard")
	virtual std::string NamePlural() const RET("city guards")
	virtual float GetMeleeStrength() const RET(5000)
);

class CHARACTER
(
	shopkeeper,
	human,
	InitMaterials(new humanflesh(80000)),
	{
		SetSize(160);
		SetAgility(5);
		SetStrength(30);
		SetEndurance(30);
		SetPerception(30);
		SetArmType(6);
		SetHeadType(9);
		SetLegType(2);
		SetTorsoType(8);
		SetRelations(1);
	},
public:
	virtual ushort Possibility() const RET(0)
protected:
	virtual std::string NameSingular() const RET("shopkeeper")
	virtual std::string NamePlural() const RET("shopkeepers")
	virtual float GetMeleeStrength() const RET(10000)
);

class CHARACTER
(
	oree,
	character,
	InitMaterials(new pepsi(110000)),
	{
		SetSize(225);
		SetAgility(40);
		SetStrength(40);
		SetEndurance(40);
		SetPerception(27);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual uchar GetSex() const RET(MALE)
	virtual ushort CalculateArmorModifier() const RET(10)
	virtual ulong Danger() const RET(30000)
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
	virtual bool CanKick() const RET(true)
	virtual ulong GetBloodColor() const RET(MAKE_RGB(255,255,255))
protected:
	virtual vector2d GetBitmapPos() const RETV(208,0)
	virtual std::string NameSingular() const RET("Oree the Pepsi Daemon King")
	virtual std::string NamePlural() const RET("Orees the Pepsi Daemon Kings")
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(40000)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonPepsiVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonPepsiVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonPepsiVerb(Critical))
);

class CHARACTER
(
	swatcommando,
	character,
	InitMaterials(new humanflesh(110000)),
	{
		SetSize(200);
		SetAgility(30);
		SetStrength(20);
		SetEndurance(15);
		SetPerception(27);
	},
public:
	virtual ushort Possibility() const RET(5)
	virtual uchar GetSex() const RET(MALE)
	virtual bool CanWield() const RET(true)
	virtual ulong Danger() const RET(750)
	virtual bool Charmable() const RET(false)
	virtual bool CanKick() const RET(true)
protected:
	virtual vector2d GetBitmapPos() const RETV(128,0)
	virtual std::string NameSingular() const RET("Bill's SWAT commando")
	virtual std::string NamePlural() const RET("Bill's SWAT commandos")
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(10000)
);

class CHARACTER
(
	ennerbeast,
	character,
	InitMaterials(new ennerbeastflesh(60000)),
	{
		SetSize(150);
		SetAgility(10);
		SetStrength(5);
		SetEndurance(10);
		SetPerception(9);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual bool Hit(character*);
	virtual ulong Danger() const RET(2500);
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
	virtual bool CanKick() const RET(true)
protected:
	virtual vector2d GetBitmapPos() const RETV(96,0)
	virtual std::string NameSingular() const RET("Enner Beast")
	virtual std::string NamePlural() const RET("Enner Beasts")
	virtual float GetMeleeStrength() const RET(200000)
	virtual void HostileAICommand();
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
	InitMaterials(new darkfrogflesh(100)),
	{
		SetSize(15);
		SetAgility(20);
		SetStrength(1);
		SetEndurance(2);
		SetPerception(15);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual ulong Danger() const RET(25)
protected:
	virtual vector2d GetBitmapPos() const RETV(80,0)
	virtual std::string NameSingular() const RET("dark frog")
	virtual std::string NamePlural() const RET("dark frogs")
);

class CHARACTER
(
	elpuri,
	darkfrog,
	InitMaterials(new elpuriflesh(8000000)),
	{
		SetSize(300);
		SetAgility(50);
		SetStrength(50);
		SetEndurance(50);
		SetPerception(18);
	},
public:
	virtual std::string DeathMessage() { return Name(DEFINITE) + " groans horribly and drops " + game::PossessivePronoun(GetSex()) + " head."; }
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual ulong Danger() const RET(5000)
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
protected:
	virtual vector2d GetBitmapPos() const RETV(64,0)
	virtual std::string NameSingular() const RET("Elpuri the Dark Frog")
	virtual std::string NamePlural() const RET("Elpuris the Dark Frogs")
	virtual void CreateCorpse();
);

class CHARACTER
(
	billswill,
	character,
	InitMaterials(new air(500000)),
	{
		SetSize(100);
		SetAgility(40);
		SetStrength(5);
		SetEndurance(5);
		SetPerception(27);
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual void SpillBlood(uchar) {}
	virtual ulong Danger() const RET(75)
	virtual bool Charmable() const RET(false)
protected:
	virtual vector2d GetBitmapPos() const RETV(48,0)
	virtual std::string NameSingular() const RET("pure mass of Bill's will")
	virtual std::string NamePlural() const RET("pure masses of Bill's will")
	virtual void CreateCorpse() {}
	virtual std::string DeathMessage() { return Name(DEFINITE) + " vanishes from existence."; }
	virtual float GetMeleeStrength() const RET(30000)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonPSIVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonPSIVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonPSIVerb(Critical))
);

class CHARACTER
(
	fallenvalpurist,
	character,
	InitMaterials(new bone(60000)),
	{
		SetSize(150);
		SetAgility(10);
		SetStrength(5);
		SetEndurance(5);
		SetPerception(15);
	},
public:
	virtual std::string DeathMessage() { return Name(DEFINITE) + " is transformed into a crumpled heap of bones."; }
	virtual ushort Possibility() const RET(50)
	virtual bool CanWield() const RET(true)
	virtual ulong Danger() const RET(25)
	virtual bool CanKick() const RET(true)
	virtual void SpillBlood(uchar) {}
protected:
	virtual vector2d GetBitmapPos() const RETV(112,0)
	virtual std::string NameSingular() const RET("fallen valpurist")
	virtual std::string NamePlural() const RET("fallen valpurists")
	virtual void CreateCorpse();
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(5000)
);

class CHARACTER
(
	froggoblin,
	character,
	InitMaterials(new goblinoidflesh(25000)),
	{
		SetSize(100);
		SetAgility(15);
		SetStrength(10);
		SetEndurance(5);
		SetPerception(21);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual ulong Danger() const RET(25)
	virtual bool CanKick() const RET(true)
protected:
	virtual vector2d GetBitmapPos() const RETV(144,0)
	virtual std::string NameSingular() const RET("frog-goblin hybrid")
	virtual std::string NamePlural() const RET("frog-goblin hybrids")
	virtual void CreateInitialEquipment();
	virtual float GetMeleeStrength() const RET(5000)
);

class ABSTRACT_CHARACTER
(
	mommo,
	character,
protected:
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBrownSlimeVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual float GetMeleeStrength() const RET(25000)
);

class CHARACTER
(
	conicalmommo,
	mommo,
	InitMaterials(new brownslime(250000)),
	{
		SetSize(100);
		SetAgility(1);
		SetStrength(2);
		SetEndurance(40);
		SetPerception(9);
	},
public:
	virtual ushort Possibility() const RET(25)
	virtual ulong Danger() const RET(250)
	virtual ulong GetBloodColor() const RET(MAKE_RGB(7,155,0))
protected:
	virtual std::string NameSingular() const RET("conical mommo slime")
	virtual std::string NamePlural() const RET("conical mommo slimes")
	virtual vector2d GetBitmapPos() const RETV(176,0)

);

class CHARACTER
(
	flatmommo,
	mommo,
	InitMaterials(new brownslime(150000)),
	{
		SetSize(50);
		SetAgility(2);
		SetStrength(1);
		SetEndurance(20);
		SetPerception(9);
	},
public:
	virtual ushort Possibility() const RET(75)
	virtual ulong Danger() const RET(75)
	virtual ulong GetBloodColor() const RET(MAKE_RGB(7,155,0))
protected:
	virtual std::string NameSingular() const RET("flat mommo slime")
	virtual std::string NamePlural() const RET("flat mommo slimes")
	virtual vector2d GetBitmapPos() const RETV(192,0)
);

class CHARACTER
(
	golem,
	character,
	InitMaterials(protosystem::CreateRandomSolidMaterial(100000)),
	{
		SetSize(250);
		SetAgility(5);
		SetStrength(35);
		SetEndurance(20);
		SetPerception(12);
	},
public:
	virtual std::string DeathMessage() { return "The Holy Words of " + Name(DEFINITE) + " fly away and the monster collapses."; }
	virtual ushort Possibility() const RET(20)
	virtual ushort CalculateArmorModifier() const;
	virtual void MoveRandomly();
	virtual std::string Name(uchar Case) const RET(NameWithMaterial(Case))
	virtual ulong Danger() const;
	virtual void DrawToTileBuffer() const;
	virtual void SpillBlood(uchar) {}
protected:
	virtual std::string NameSingular() const RET("golem")
	virtual std::string NamePlural() const RET("golems")
	virtual vector2d GetBitmapPos() const RETV(256,0)
	virtual float GetMeleeStrength() const;
);

class CHARACTER
(
	wolf,
	character,
	InitMaterials(new wolfflesh(30000)),
	{
		SetSize(100);
		SetAgility(20);
		SetStrength(6);
		SetEndurance(6);
		SetPerception(24);
	},
public:
	virtual ushort Possibility() const RET(40)
	virtual ulong Danger() const RET(20)
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
	InitMaterials(new dogflesh(20000)),
	{
		SetSize(75);
		SetAgility(15);
		SetStrength(4);
		SetEndurance(4);
		SetPerception(21);
	},
public:
	virtual ushort Possibility() const RET(20)
	virtual ulong Danger() const RET(10)
	virtual bool Catches(item*, float);
	virtual bool ConsumeItemType(uchar) const;
protected:
	virtual vector2d GetBitmapPos() const RETV(240,0)
	virtual std::string NameSingular() const RET("dog")
	virtual std::string NamePlural() const RET("dogs")
	virtual float GetMeleeStrength() const RET(5000)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
);

class CHARACTER
(
	spider,
	character,
	InitMaterials(new spiderflesh(50)),
	{
		SetSize(10);
		SetAgility(4);
		SetStrength(1);
		SetEndurance(1);
		SetPerception(5);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual ulong Danger() const RET(5)
protected:
	virtual vector2d GetBitmapPos() const RETV(272,0)
	virtual std::string NameSingular() const RET("spider")
	virtual std::string NamePlural() const RET("spiders")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(2000)
);

class CHARACTER
(
	jackal,
	character,
	InitMaterials(new jackalflesh(20000)),
	{
		SetSize(80);
		SetAgility(6);
		SetStrength(6);
		SetEndurance(4);
		SetPerception(20);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual ulong Danger() const RET(6)
protected:
	virtual vector2d GetBitmapPos() const RETV(304,0)
	virtual std::string NameSingular() const RET("jackal")
	virtual std::string NamePlural() const RET("jackals")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))																																																																																																																																													//Jackals are unoriginal
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(3000)
);

class CHARACTER
(
	donkey,
	character,
	InitMaterials(new donkeyflesh(83000)),
	{
		SetSize(150);
		SetAgility(4);
		SetStrength(6);
		SetEndurance(6);
		SetPerception(17);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual ulong Danger() const RET(6)
protected:
	virtual vector2d GetBitmapPos() const RETV(288,0)
	virtual std::string NameSingular() const RET("donkey")
	virtual std::string NamePlural() const RET("donkeys")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(3000)
);

#endif
