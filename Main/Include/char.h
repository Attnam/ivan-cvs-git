#ifndef __CHAR_H__
#define __CHAR_H__

#define HAS_HIT 0
#define HAS_BLOCKED 1
#define HAS_DODGED 2
#define HAS_DIED 3

#define HOSTILE 0
#define NEUTRAL 1
#define FRIEND 2

#define NUMBER_OF_HUMAN_ARMS	12
#define NUMBER_OF_HUMAN_HEADS	16
#define NUMBER_OF_HUMAN_LEGS	12
#define NUMBER_OF_HUMAN_TORSOS	12

#define HUNGERLEVEL 		200 
#define CRITICALHUNGERLEVEL 	60

#define OVERLOADED		0
#define STRESSED		1
#define BURDENED		2
#define UNBURDENED		3

#include "game.h"
#include "graphics.h"
#include "object.h"
#include "typedef.h"
#include "vector.h"

#include "proto.h"
#include "material.h"

class square;
class bitmap;
class character;
class item;
class stack;
class material;
class levelsquare;

/* Presentation of the character class */

class character : public object
{
public:
	character(bool, bool, bool, bool = true);
	virtual ~character(void);
	virtual character* Clone(bool = true, bool = true, bool = true) const = 0;
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual bool CanRead(void) const { return true; }
	virtual bool CanWear(void) const { return false; }
	virtual bool CanWield(void) const { return false; }
	virtual bool Charmable(void) const { return true; }
	virtual bool Catches(item*, float, bool) { return false; }
	virtual bool CheckBulimia(void) const;
	virtual bool CheckDeath(std::string);
	virtual bool CheckIfConsumable(ushort) const;
	virtual bool ConsumeItem(int, stack*);
	virtual bool ConsumeItemType(uchar) const;
	virtual bool DodgesFlyingItem(item*, float, bool);
	virtual bool Hit(character*);
	virtual bool OpenItem(void);
	virtual bool OpenPos(vector);
	virtual bool ReadItem(int, stack*);
	virtual bool TestForPickup(item*) const;
	virtual bool ThrowItem(uchar, item*);
	virtual bool TryMove(vector);
	virtual bool WearItem(item*) { return false; }
	virtual bool HasHeadOfElpuri(void) const;
	virtual bool HasMaakotkaShirt(void) const;
	virtual bool HasPerttusNut(void) const;
	virtual bool GetFainted(void) const { return Fainted; }
	virtual bool GetHasActed(void) const { return HasActed; }
	virtual bool GetIsPlayer(void) const { return IsPlayer; }
	virtual bool Apply(void);
	virtual bool Close(void);
	virtual bool Consume(void);
	virtual bool DecreaseGamma(void);
	virtual bool DecreaseSoftGamma(void);
	virtual bool Dip(void);
	virtual bool DrawMessageHistory(void);
	virtual bool Drop(void);
	virtual bool Engrave(std::string);
	virtual bool ForceVomit(void);
	virtual bool GainAllItems(void);
	virtual bool GoDown(void);
	virtual bool GoUp(void);
	virtual bool IncreaseGamma(void);
	virtual bool IncreaseSoftGamma(void);
	virtual bool Kick(void);
	virtual bool Look(void);
	virtual bool LowerStats(void);
	virtual bool NOP(void);
	virtual bool Offer(void);
	virtual bool Open(void);
	virtual bool PickUp(void);
	virtual bool Pray(void);
	virtual bool Quit(void);
	virtual bool RaiseStats(void);
	virtual bool Read(void);
	virtual bool Save(void);
	virtual bool ScreenShot(void);
	virtual bool SeeWholeMap(void);
	virtual bool ShowInventory(void);
	virtual bool ShowKeyLayout(void);
	virtual bool Talk(void);
	virtual bool Throw(void);
	virtual bool WalkThroughWalls(void);
	virtual bool WearArmor(void);
	virtual bool WhatToEngrave(void);
	virtual bool Wield(void);
	virtual bool WizardMode(void);
	virtual long AddScoreEntry(std::string, float = 1) const;
	virtual long Score(void) const;
	virtual float GetAttackStrength(void) const;
	virtual float GetDifficulty(void) const;
	virtual item* GetTorsoArmor(void) const				{ return 0; }
	virtual item* GetWielded(void) const				{ return Wielded; }
	virtual levelsquare* GetLevelSquareUnder(void) const { return (levelsquare*)SquareUnder; }
	virtual long GetAgilityExperience(void) const { return AgilityExperience; }
	virtual long GetAP(void) const { return AP; }
	virtual long GetAPsToBeEaten(void) const { return APsToBeEaten; }
	virtual long GetEnduranceExperience(void) const { return EnduranceExperience; }
	virtual long GetNP(void) const					{ return NP; }
	virtual long GetPerceptionExperience(void) const { return PerceptionExperience; }
	virtual long GetStrengthExperience(void) const { return StrengthExperience; }
	virtual short GetHP(void) const					{ return HP; }
	virtual stack* GetStack(void) const				{ return Stack; }
	virtual uchar GetBurdenState(ulong = 0) const;
	virtual uchar GetRelations(void) const { return Relations; }
	virtual uchar GetSex(void) const { return UNDEFINED; }
	virtual uchar TakeHit(ushort, short, float, character*);
	virtual ulong Danger(void) const = 0;
	virtual ulong GetBloodColor(void) const { return MAKE_RGB(100,0,0); }
	virtual ushort CalculateArmorModifier(void) const;
	virtual ushort CRegenerationCounter(void) const { return RegenerationCounter; }
	virtual ushort GetAgility(void) const					{ return Agility; }
	virtual ushort GetConsumingCurrently(void) const { return EatingCurrently; }
	virtual ushort GetEmitation(void) const;
	virtual ushort GetEndurance(void) const					{ return Endurance; }
	virtual ushort GetPerception(void) const				{ return Perception; }
	virtual ushort GetStrength(void) const					{ return Strength; }
	virtual ushort LOSRange(void) const { return GetPerception() / 3; }
	virtual ushort LOSRangeLevelSquare(void) const { return GetPerception() * GetPerception() / 9; }
	virtual ushort Possibility(void) const = 0;
	virtual vector GetPos(void) const;
	virtual void Act(void);
	virtual void AddBlockMessage(character*) const;
	virtual void AddDodgeMessage(character*) const;
	virtual void AddHitMessage(character*, const bool = false) const;
	virtual void ApplyExperience(void);
	virtual void BeTalkedTo(character*);
	virtual void ContinueEating(void);
	virtual void Darkness(long);
	virtual void Die(void);
	virtual void DrawToTileBuffer(void) const;
	virtual void HasBeenHitByItem(item*, float, bool);
	virtual void HealFully(character*) {}
	virtual void Hunger(ushort = 1);
	virtual void Move(vector, bool = false);
	virtual void MoveRandomly(void);
	virtual void ReceiveBulimiaDamage(void);
	virtual void ReceiveFireDamage(long);
	virtual void ReceiveNutrition(long);
	virtual void ReceiveOmleUrineEffect(long);
	virtual void ReceivePepsiEffect(long);
	virtual void ReceiveSchoolFoodEffect(long);
	virtual void ReceiveSound(char*, short, float);
	virtual void Regenerate(ushort = 1);
	virtual void SetAgility(ushort What) { Agility = What; if(short(Agility) < 1) Agility = 1; }
	virtual void SetAgilityExperience(long What) { AgilityExperience = What; }
	virtual void SetAP(long What) { AP = What; }
	virtual void SetAPsToBeEaten(long What) { APsToBeEaten = What; }
	virtual void SetConsumingCurrently(ushort What) { EatingCurrently = What; }
	virtual void SetEndurance(ushort What) { Endurance = What; if(short(Endurance) < 1) Endurance = 1; }
	virtual void SetEnduranceExperience(long What) { EnduranceExperience = What; }
	virtual void SetFainted(bool To) { Fainted = To; }
	virtual void SetHasActed(bool HA)				{HasActed = HA; }
	virtual void SetHP(short What) { HP = What; }
	virtual void SetIsPlayer(bool What) { IsPlayer = What; }
	virtual void SetMaterial(ushort Where, material* What) { Material[Where] = What; }
	virtual void SetNP(long What) { NP = What; }
	virtual void SetPerception(ushort What) { Perception = What; if(short(Perception) < 1) Perception = 1; }
	virtual void SetPerceptionExperience(long What) { PerceptionExperience = What; }
	virtual void SetRegenerationCounter(long What) { RegenerationCounter = What; }
	virtual void SetRelations(uchar What) { Relations = What; }
	virtual void SetSquareUnder(square* Square);
	virtual void SetStrength(ushort What) { Strength = What; if(short(Strength) < 1) Strength = 1; }
	virtual void SetStrengthExperience(long What) { StrengthExperience = What; }
	virtual void SetWielded(item* Something) { Wielded = Something; }
	virtual void SpillBlood(uchar);
	virtual void StopEating(void);
	virtual void Vomit(ushort);
	virtual void Be(void) { HasActed = false; Act(); }
	static ushort GetProtoIndexBegin(void) { return ProtoIndexBegin; }
	static ushort GetProtoIndexEnd(void) { return ProtoIndexEnd; }
	static ushort GetProtoAmount(void) { return ProtoIndexEnd - ProtoIndexBegin; }
protected:
	virtual void CreateCorpse(void);
	virtual std::string DeathMessage(void) { return Name(DEFINITE) + " dies screaming."; }
	virtual void CreateInitialEquipment(void) {}
	virtual void SetDefaultStats(void) = 0;
	virtual void GetPlayerCommand(void);
	virtual void GetAICommand(void);
	virtual void Charge(character*);
	virtual float GetMeleeStrength(void) const				{ return 0; }
	virtual void HostileAICommand(void);
	virtual void NeutralAICommand(void);
	virtual void FriendAICommand(void);
	virtual std::string ThirdPersonWeaponHitVerb(bool Critical) const	{ return NormalThirdPersonHitVerb(Critical); }
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const	{ return NormalThirdPersonHitVerb(Critical); }
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const	{ return NormalFirstPersonHitVerb(Critical); }
	virtual std::string AICombatHitVerb(character*, bool Critical) const	{ return NormalThirdPersonHitVerb(Critical); }
	virtual std::string NormalFirstPersonHitVerb(bool Critical) const	{ return Critical ? "critically hit" : "hit"; }
	virtual std::string NormalThirdPersonHitVerb(bool Critical) const	{ return Critical ? "critically hits" : "hits"; }
	virtual std::string FirstPersonBiteVerb(bool Critical) const		{ return Critical ? "critically bite" : "bite"; }
	virtual std::string ThirdPersonBiteVerb(bool Critical) const		{ return Critical ? "critically bites" : "bites"; }
	virtual std::string FirstPersonPSIVerb(bool Critical) const		{ return Critical ? "emit very powerful psychic waves at" : "emit psychic waves at"; }
	virtual std::string ThirdPersonPSIVerb(bool Critical) const		{ return Critical ? "emits very powerful psychic waves at" : "emits psychic waves at"; }
	virtual std::string FirstPersonBrownSlimeVerb(bool Critical) const	{ return Critical ? "vomit extremely acidous brown slime at" : "vomit brown slime at"; }
	virtual std::string ThirdPersonBrownSlimeVerb(bool Critical) const	{ return Critical ? "vomits extremely acidous brown slime at" : "vomits brown slime at"; }
	virtual std::string FirstPersonPepsiVerb(bool Critical) const		{ return Critical ? "vomit extremely stale pepsi at" : "vomit pepsi at"; }
	virtual std::string ThirdPersonPepsiVerb(bool Critical) const		{ return Critical ? "vomits extremely stale pepsi at" : "vomits pepsi at"; }
	stack* Stack;
	item* Wielded;
	ushort Strength, Endurance, Agility, Perception, RegenerationCounter;
	short HP;
	long NP, AP;
	long StrengthExperience, EnduranceExperience, AgilityExperience, PerceptionExperience;
	bool HasActed;
	uchar Relations;
	bool Fainted;
	ushort EatingCurrently;
	long APsToBeEaten;
	bool Dead;
	bool IsPlayer;
	static ushort ProtoIndexBegin, ProtoIndexEnd;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define CHARACTER(name, base, initmaterials, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool = true, bool = true, bool = true, bool = true);\
		name(material* Material, bool SetStats = true, bool CreateEquipment = true) : base(false, false, false) { InitMaterials(Material); if(SetStats) { SetDefaultStats(); SetHP(GetEndurance() * 2); } if(CreateEquipment) CreateInitialEquipment(); }\
		virtual character* Clone(bool = true, bool = true, bool = true) const;\
		virtual typeable* CloneAndLoad(std::ifstream&) const;\
	protected:\
		virtual void SetDefaultStats(void);\
		virtual ushort Type(void) const;\
		data\
	};\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller(void) : Index(prototypesystem::Add(new name(false, false, false, false))) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	name::name(bool CreateMaterials, bool SetStats, bool CreateEquipment, bool AddToPool) : base(false, false, false, AddToPool) { if(CreateMaterials) initmaterials ; if(SetStats) { SetDefaultStats(); SetHP(GetEndurance() * 2); } if(CreateEquipment) CreateInitialEquipment(); }\
	character* name::Clone(bool CreateMaterials, bool SetStats, bool CreateEquipment) const { return new name(CreateMaterials, SetStats, CreateEquipment); }\
	typeable* name::CloneAndLoad(std::ifstream& SaveFile) const { character* Char = new name(false, false, false); Char->Load(SaveFile); return Char; }\
	void name::SetDefaultStats(void) { setstats }\
	ushort name::Type(void) const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define CHARACTER(name, base, initmaterials, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool = true, bool = true, bool = true, bool = true);\
		name(material* Material, bool SetStats = true, bool CreateEquipment = true) : base(false, false, false) { InitMaterials(Material); if(SetStats) { SetDefaultStats(); SetHP(GetEndurance() * 2); } if(CreateEquipment) CreateInitialEquipment(); }\
		virtual character* Clone(bool = true, bool = true, bool = true) const;\
		virtual typeable* CloneAndLoad(std::ifstream&) const;\
	protected:\
		virtual void SetDefaultStats(void);\
		virtual ushort Type(void) const;\
		data\
	};

#endif

#define ABSTRACT_CHARACTER(name, base, data)\
\
name : public base\
{\
public:\
	name(bool CreateMaterials, bool SetStats, bool CreateEquipment, bool AddToPool = true) : base(CreateMaterials, SetStats, CreateEquipment, AddToPool) {}\
	data\
};

BEGIN_PROTOTYPING(character)

class ABSTRACT_CHARACTER
(
	humanoid,
	character,
public:
	virtual void Load(std::ifstream&);
	virtual void DrawToTileBuffer(void) const;
	virtual bool WearArmor(void);
	virtual item* GetTorsoArmor(void) const RET(Armor.Torso)
	virtual uchar GetSex(void) const RET(MALE)
	virtual ushort CalculateArmorModifier(void) const;
	virtual bool Drop(void);
	virtual bool Wield(void);
	virtual void Save(std::ofstream&) const;
	virtual bool Throw(void);
	virtual uchar GetArmType(void) const RET(ArmType)
	virtual uchar GetHeadType(void) const RET(HeadType)
	virtual bool CanWield(void) const RET(true)
	virtual bool CanWear(void) const RET(true)
	virtual bool WearItem(item* What) { Armor.Torso = What; return true; }
	virtual bool Apply(void);
	virtual void SetArmType(uchar Value) { ArmType = Value; }
	virtual void SetHeadType(uchar Value) { HeadType = Value; }
	virtual void SetLegType(uchar Value) { LegType = Value; }
	virtual void SetTorsoType(uchar Value) { TorsoType = Value; }
protected:
	virtual vector GetBitmapPos(void) const RETV(0,0)
	virtual float GetMeleeStrength(void) const RET(2000)
	struct armor
	{
	public:
		armor(void);
		item* Torso;
		item* Legs;
		item* Hands;
		item* Head;
		item* Feet;
	} Armor;
	uchar ArmType;
	uchar HeadType;
	uchar LegType;
	uchar TorsoType;
);

inline humanoid::armor::armor(void) : Torso(0), Legs(0), Hands(0), Head(0), Feet(0) {}

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
	virtual ulong Danger(void) const RET(0)
	virtual ushort Possibility(void) const RET(0)
protected:
	virtual std::string NameSingular(void) const RET("human")
	virtual std::string NamePlural(void) const RET("humans")
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
		SetHeadType(15);
		SetLegType(2);
		SetTorsoType(3);
		SetRelations(1);
		SetHealTimer(100);
	},
public:
	virtual void Load(std::ifstream&);
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual void BeTalkedTo(character*);
	virtual ushort GetEmitation(void) const RET(333)
	virtual void HealFully(character*);
	virtual void SetHealTimer(ushort What) { HealTimer = What; }
	virtual ushort GetHealTimer(void) RET(HealTimer)
	virtual void ReceiveFireDamage(long) {}
	virtual void Save(std::ofstream&) const;
	virtual ulong Danger(void) const RET(150000)
	virtual bool Charmable(void) const RET(false)
	virtual ushort Possibility(void) const RET(0)
protected:
	virtual std::string NameSingular(void) const RET("Perttu, the Überpriest of the Great Frog")
	virtual std::string NamePlural(void) const RET("Perttus, the Überpriests of the Great Frog")
	virtual void CreateCorpse(void);
	virtual std::string DeathMessage(void) { return Name(DEFINITE) + " disappears in a bright light and his left nut is left behind."; }
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) const RET(10000)
	virtual void NeutralAICommand(void);
	ushort HealTimer;
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
	virtual ushort Possibility(void) const RET(0)
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual uchar GetSex(void) const RET(MALE)
	virtual ushort CalculateArmorModifier(void) const RET(10)
	virtual ulong Danger(void) const RET(30000)
	virtual bool Charmable(void) const RET(false)
protected:
	virtual vector GetBitmapPos(void) const RETV(208,0)
	virtual std::string NameSingular(void) const RET("Oree the Pepsi Daemon King")
	virtual std::string NamePlural(void) const RET("Orees the Pepsi Daemon Kings")
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) const RET(40000)
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
	virtual ushort Possibility(void) const RET(5)
	virtual uchar GetSex(void) const RET(MALE)
	virtual bool CanWield(void) const RET(true)
	virtual ulong Danger(void) const RET(750)
	virtual bool Charmable(void) const RET(false)
protected:
	virtual vector GetBitmapPos(void) const RETV(128,0)
	virtual std::string NameSingular(void) const RET("Bill's SWAT commando")
	virtual std::string NamePlural(void) const RET("Bill's SWAT commandos")
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) const RET(10000)
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
	virtual ushort Possibility(void) const RET(0)
	virtual bool Hit(character*);
	virtual ulong Danger(void) const RET(2500);
	virtual bool Charmable(void) const RET(false)
protected:
	virtual vector GetBitmapPos(void) const RETV(96,0)
	virtual std::string NameSingular(void) const RET("Enner Beast")
	virtual std::string NamePlural(void) const RET("Enner Beasts")
	virtual float GetMeleeStrength(void) const RET(200000)
	virtual void HostileAICommand(void);
);

class ABSTRACT_CHARACTER
(
	frog,
	character,
protected:
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength(void) const RET(20000)
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
	virtual ushort Possibility(void) const RET(100)
	virtual ulong Danger(void) const RET(25)
protected:
	virtual vector GetBitmapPos(void) const RETV(80,0)
	virtual std::string NameSingular(void) const RET("dark frog")
	virtual std::string NamePlural(void) const RET("dark frogs")
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
	virtual std::string DeathMessage(void) { return Name(DEFINITE) + " groans horribly and drops " + game::PossessivePronoun(GetSex()) + " head."; }
	virtual ushort Possibility(void) const RET(0)
	virtual std::string Name(uchar Case) const RET(NameProperNoun(Case))
	virtual ulong Danger(void) const RET(5000)
	virtual bool Charmable(void) const RET(false)
protected:
	virtual vector GetBitmapPos(void) const RETV(64,0)
	virtual std::string NameSingular(void) const RET("Elpuri the Dark Frog")
	virtual std::string NamePlural(void) const RET("Elpuris the Dark Frogs")
	virtual void CreateCorpse(void);
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
	virtual ushort Possibility(void) const RET(50)
	virtual void SpillBlood(uchar) {}
	virtual ulong Danger(void) const RET(75)
	virtual bool Charmable(void) const RET(false)
protected:
	virtual vector GetBitmapPos(void) const RETV(48,0)
	virtual std::string NameSingular(void) const RET("pure mass of Bill's will")
	virtual std::string NamePlural(void) const RET("pure masses of Bill's will")
	virtual void CreateCorpse(void) {}
	virtual std::string DeathMessage(void) { return Name(DEFINITE) + " vanishes from existence."; }
	virtual float GetMeleeStrength(void) const RET(30000)
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
	virtual std::string DeathMessage(void) { return Name(DEFINITE) + " is transformed into a crumpled heap of bones."; }
	virtual ushort Possibility(void) const RET(50)
	virtual bool CanWield(void) const RET(true)
	virtual ulong Danger(void) const RET(25)
protected:
	virtual vector GetBitmapPos(void) const RETV(112,0)
	virtual std::string NameSingular(void) const RET("fallen valpurist")
	virtual std::string NamePlural(void) const RET("fallen valpurists")
	virtual void CreateCorpse(void);
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) const RET(5000)
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
	virtual ushort Possibility(void) const RET(100)
	virtual bool CanWield(void) const RET(true)
	virtual ulong Danger(void) const RET(25)
protected:
	virtual vector GetBitmapPos(void) const RETV(144,0)
	virtual std::string NameSingular(void) const RET("frog-goblin hybrid")
	virtual std::string NamePlural(void) const RET("frog-goblin hybrids")
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) const RET(5000)
);

class ABSTRACT_CHARACTER
(
	mommo,
	character,
protected:
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBrownSlimeVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual float GetMeleeStrength(void) const RET(25000)
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
	virtual ushort Possibility(void) const RET(25)
	virtual ulong Danger(void) const RET(250)
	virtual ulong GetBloodColor(void) const RET(MAKE_RGB(7,155,0))
protected:
	virtual std::string NameSingular(void) const RET("conical mommo slime")
	virtual std::string NamePlural(void) const RET("conical mommo slimes")
	virtual vector GetBitmapPos(void) const RETV(176,0)

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
	virtual ushort Possibility(void) const RET(75)
	virtual ulong Danger(void) const RET(75)
	virtual ulong GetBloodColor(void) const RET(MAKE_RGB(7,155,0))
protected:
	virtual std::string NameSingular(void) const RET("flat mommo slime")
	virtual std::string NamePlural(void) const RET("flat mommo slimes")
	virtual vector GetBitmapPos(void) const RETV(192,0)
);

class CHARACTER
(
	golem,
	character,
	InitMaterials(prototypesystem::CreateRandomSolidMaterial(100000)),
	{
		SetSize(250);
		SetAgility(5);
		SetStrength(35);
		SetEndurance(20);
		SetPerception(12);
	},
public:
	virtual std::string DeathMessage(void) { return "The Holy Words of " + Name(DEFINITE) + " fly away and the monster collapses."; }
	virtual ushort Possibility(void) const RET(20)
	virtual ushort CalculateArmorModifier(void) const;
	virtual void MoveRandomly(void);
	virtual std::string Name(uchar Case) const RET(NameWithMaterial(Case))
	virtual ulong Danger(void) const;
	virtual void DrawToTileBuffer(void) const;
	virtual void SpillBlood(uchar) {}
protected:
	virtual std::string NameSingular(void) const RET("golem")
	virtual std::string NamePlural(void) const RET("golems")
	virtual vector GetBitmapPos(void) const RETV(256,0)
	virtual float GetMeleeStrength(void) const;
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
	virtual ushort Possibility(void) const RET(40)
	virtual ulong Danger(void) const RET(20)
protected:
	virtual std::string NameSingular(void) const RET("wolf")
	virtual std::string NamePlural(void) const RET("wolves")
	virtual vector GetBitmapPos(void) const RETV(224,0)
	virtual float GetMeleeStrength(void) const RET(7500)
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
	virtual ushort Possibility(void) const RET(20)
	virtual ulong Danger(void) const RET(10)
	virtual bool Catches(item*, float, bool);
	virtual bool ConsumeItemType(uchar) const;
protected:
	virtual vector GetBitmapPos(void) const RETV(240,0)
	virtual std::string NameSingular(void) const RET("dog")
	virtual std::string NamePlural(void) const RET("dogs")
	virtual float GetMeleeStrength(void) const RET(5000)
	virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, bool Critical) const RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, bool Critical) const RET(ThirdPersonBiteVerb(Critical))
);

FINISH_PROTOTYPING(character)

#endif

