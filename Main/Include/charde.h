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
	virtual void Save(outputfile&) const;
	virtual uchar GetLegType() const RET(LegType)
	virtual uchar GetTorsoType() const RET(TorsoType)
	virtual uchar GetArmType() const RET(ArmType)
	virtual uchar GetHeadType() const RET(HeadType)
	virtual uchar GetShieldType() const RET(ShieldType)
	virtual bool CanWield() const RET(true)
	virtual bool CanWear() const RET(true)
	virtual void SetLegType(uchar Value) { LegType = Value; }
	virtual void SetTorsoType(uchar Value) { TorsoType = Value; }
	virtual void SetArmType(uchar Value) { ArmType = Value; }
	virtual void SetHeadType(uchar Value) { HeadType = Value; }
	virtual void SetShieldType(uchar Value) { ShieldType = Value; }
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
	uchar LegType;
	uchar TorsoType;
	uchar ArmType;
	uchar HeadType;
	uchar ShieldType;
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
		SetLegType(rand() % NUMBER_OF_HUMAN_LEGS);
		SetTorsoType(rand() % NUMBER_OF_HUMAN_TORSOS);
		SetArmType(rand() % NUMBER_OF_HUMAN_ARMS);
		SetHeadType(rand() % NUMBER_OF_HUMAN_HEADS);
		SetShieldType(0);
		SetMoney(200 + rand() % 101);
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
		SetLegType(1);
		SetTorsoType(5);
		SetArmType(3);
		SetHeadType(10);
		SetShieldType(0);
		SetHealTimer(100);
		SetStoryState(0);
		game::SetPerttu(this);
	},
public:
	virtual ~perttu();
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
	virtual uchar GetStoryState() const { return StoryState; }
	virtual void SetStoryState(uchar What) { StoryState = What; }
	virtual bool HasInfraVision() const { return true; }
protected:
	virtual std::string NameSingular() const RET("Perttu, the Überpriest of the Great Frog")
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
	human,
	InitMaterials(new humanflesh),
	{
		SetSize(170);
		SetAgility(10);
		SetStrength(10);
		SetEndurance(15);
		SetPerception(18);
		SetLegType(rand() % 3);
		SetTorsoType(2 + rand() % 2);
		SetArmType(rand() % 5);
		SetHeadType(5 + rand() % 2);
		SetShieldType(0);
		SetMoney(20);
	},
public:
	virtual void CreateInitialEquipment();
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 30000; else return 0; }
	virtual void BeTalkedTo(character*);
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
		SetAgility(15);
		SetStrength(30);
		SetEndurance(30);
		SetPerception(21);
		SetLegType(4);
		SetTorsoType(8);
		SetArmType(5);
		SetHeadType(7);
		SetShieldType(1);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual void GetAICommand() { StandIdleAI(); }
	virtual void CreateInitialEquipment();
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
	virtual void BeTalkedTo(character*);
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
		SetLegType(2);
		SetTorsoType(7);
		SetArmType(6);
		SetHeadType(4);
		SetShieldType(0);
		SetMoney(3000 + rand() % 2001);
	},
public:
	virtual void GetAICommand() { StandIdleAI(); }
	virtual void CreateInitialEquipment();
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100000; else return 0; }
	virtual void BeTalkedTo(character*);
protected:
	virtual std::string NameSingular() const RET("shopkeeper")
	virtual float GetMeleeStrength() const RET(2000)
);

class CHARACTER
(
	priest,
	human,
	InitMaterials(new humanflesh),
	{
		SetSize(180);
		SetAgility(10);
		SetStrength(20);
		SetEndurance(20);
		SetPerception(18);
		SetLegType(2);
		SetTorsoType(7);
		SetArmType(6);
		SetHeadType(9);
		SetShieldType(0);
	},
public:
	virtual void GetAICommand() { StandIdleAI(); }
	virtual void CreateInitialEquipment();
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 100000; else return 0; }
	virtual void BeTalkedTo(character*);
protected:
	virtual std::string NameSingular() const RET("priest")
	virtual float GetMeleeStrength() const RET(1000)
);

class CHARACTER
(
	oree,
	character,
	InitMaterials(2, new pepsi, new leather),
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
	virtual void BeTalkedTo(character*);
	virtual bool HasInfraVision() const { return true; }
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
		SetStrength(25);
		SetEndurance(30);
		SetPerception(30);
	},
public:
	virtual ushort Possibility() const RET(5)
	virtual uchar GetSex() const RET(MALE)
	virtual bool CanWield() const RET(true)
	virtual bool CanKick() const RET(true)
	virtual ushort CalculateArmorModifier() const RET(61)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 110000; else return 0; }
	virtual void BeTalkedTo(character*);
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
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual bool Charmable() const RET(false)
	virtual bool Polymorph() const RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 277500; else return 0; }
	virtual ulong GetBloodColor() const RET(BLACK)
	virtual void BeTalkedTo(character*);
	virtual ushort CalculateArmorModifier() const RET(10)
	virtual bool Hit(character*);
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
	virtual ushort Possibility() const RET(50)
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
	fallenvalpurist,
	character,
	InitMaterials(2, new bone, new leather),
	{
		SetSize(150);
		SetAgility(10);
		SetStrength(5);
		SetEndurance(10);
		SetPerception(15);
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual bool CanWield() const RET(true)
	virtual bool CanKick() const RET(true)
	virtual void SpillBlood(uchar) {}
	virtual void SpillBlood(uchar, vector2d) {}
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
	virtual void BeTalkedTo(character*);
protected:
	virtual std::string DeathMessage() { return Name(DEFINITE) + " is transformed into a crumpled heap of bones."; }
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
	InitMaterials(2, new goblinoidflesh, new leather),
	{
		SetSize(100);
		SetAgility(15);
		SetStrength(10);
		SetEndurance(15);
		SetPerception(18);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual bool CanWield() const RET(true)
	virtual bool CanKick() const RET(true)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 25000; else return 0; }
	virtual void BeTalkedTo(character*);
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
	virtual ushort Possibility() const RET(20)
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
	virtual ushort Possibility() const RET(100)
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
	virtual ushort Possibility() const RET(100)
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
	virtual ushort Possibility() const RET(100)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 40000; else return 0; }
protected:
	virtual vector2d GetBitmapPos() const RETV(288,0)
	virtual std::string NameSingular() const RET("donkey")
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength() const RET(1000)
	virtual std::string TalkVerb() const { return "neighs"; }
);

class CHARACTER
(
	ivan,
	human,
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
		SetHeadType(12);
		SetShieldType(0);
	},
public:
	virtual void CreateInitialEquipment();
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 120000; else return 0; }
	virtual void BeTalkedTo(character*);
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual bool HasInfraVision() const { return true; }
	virtual uchar CriticalModifier() const { return 4; }
protected:
	virtual std::string DeathMessage() { return "Ivan falls groaning bravely: \"Party revenges Ivan!\""; }
	virtual std::string NameSingular() const RET("Ivan")
	virtual float GetMeleeStrength() const RET(2000)
);

class CHARACTER
(
	hunter,
	human,
	InitMaterials(new humanflesh),
	{
		SetSize(180);
		SetAgility(20);
		SetStrength(20);
		SetEndurance(20);
		SetPerception(24);
		SetLegType(6);
		SetTorsoType(12);
		SetArmType(8);
		SetHeadType(13);
		SetShieldType(0);
	},
public:
	virtual void CreateInitialEquipment();
	virtual ushort Possibility() const RET(0)
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
	virtual ushort Possibility() const RET(0)
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
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 500000; else return 0; }
	virtual void GetAICommand() {}
	virtual std::string StandVerb() const { return "swimming"; }
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
		SetSize(15);
		SetAgility(30);
		SetStrength(5);
		SetEndurance(5);
		SetPerception(18);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 2500; else return 0; }
	virtual ulong GetBloodColor() const RET(WHITE)
protected:
	virtual vector2d GetBitmapPos() const RETV(80,0)
	virtual std::string NameSingular() const RET("light frog")
);

class CHARACTER
(
	slave,
	human,
	InitMaterials(new humanflesh),
	{
		SetSize(160);
		SetAgility(10);
		SetStrength(20);
		SetEndurance(10);
		SetPerception(12);
		SetLegType(6);
		SetTorsoType(0);
		SetArmType(9);
		SetHeadType(14);
		SetShieldType(0);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 60000; else return 0; }
	virtual void BeTalkedTo(character*);
	virtual void GetAICommand();
protected:
	virtual std::string NameSingular() const RET("slave")
	virtual float GetMeleeStrength() const RET(1000)
);

#endif
