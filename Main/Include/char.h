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

#define CHARACTER_TYPES		16

#define OVERLOADED		0
#define STRESSED		1
#define BURDENED		2
#define UNBURDENED		3

#define KEYS_PER_PAGE		15

#include "typedef.h"
#include "vector.h"

#include "game.h"
#include "igraph.h"
#include "object.h"
#include "stack.h"

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
	character(bool = true, bool = true, bool = true);
	character(std::ifstream*);
	~character(void);
	virtual void DrawToTileBuffer(void);
	virtual void Act(void);
	virtual bool Hit(character*);
	virtual uchar TakeHit(ushort, short, float, character*);
	virtual bool Consume(void);
	virtual void Hunger(ushort = 1);
	virtual bool TryMove(vector);
	virtual bool Drop(void);
	virtual bool ConsumeItem(int, stack*);
	virtual void Regenerate(ushort = 1);
	virtual void Move(vector, bool = false);
	virtual bool ShowInventory(void);
	virtual bool PickUp(void);
	virtual bool Quit(void);
	virtual bool Wield(void);
	virtual void Die(void);
	virtual bool OpenItem(void);
	virtual void ReceiveSound(char*, short, float);
	virtual item* CWielded(void)					{return Wielded;}
	virtual stack* CStack(void)					{return Stack;}
	virtual ushort CEmitation(void);
	virtual vector CPos(void);
	virtual bool CHasActed(void)					{return HasActed;}
	virtual ushort CStrength(void)					{return Strength;}
	virtual ushort CAgility(void)					{return Agility;}
	virtual ushort CEndurance(void)					{return Endurance;}
	virtual ushort CPerception(void)				{return Perception;}
	virtual ushort CSize(void)					{return Size;}	 
	virtual short CHP(void)					{return HP;}
	virtual long CNP(void)					{return NP;}
	virtual void SSquareUnder(square* Square);
	virtual void SHasActed(bool HA)				{HasActed = HA;}
	virtual bool WearArmor(void);
	virtual item* CTorsoArmor(void)				{return 0;}
	virtual bool ConsumeItemType(uchar);
	virtual void ReceiveFireDamage(long);
	virtual void ReceiveSchoolFoodEffect(long);
	virtual void ReceiveNutrition(long);
	virtual void ReceiveOmleUrineEffect(long);
	virtual void ReceivePepsiEffect(long);
	virtual void Darkness(long);
	virtual void ReceiveBulimiaDamage(void);
	virtual uchar CRelations(void) { return Relations; }
	virtual void AddBlockMessage(character*);
	virtual void AddDodgeMessage(character*);
	virtual void AddHitMessage(character*, const bool = false);
	virtual uchar GetSex(void) {return UNDEFINED;}
	virtual void BeTalkedTo(character*);
	virtual bool Talk(void);
	virtual bool GoUp(void);
	virtual bool GoDown(void);
	virtual bool Open(void);
	virtual bool Close(void);
	virtual bool NOP(void);
	virtual ushort CalculateArmorModifier(void);
	virtual void ApplyExperience(void);
	virtual bool HasHeadOfElpuri(void);
	virtual bool HasPerttusNut(void);
	virtual bool HasMaakotkaShirt(void);
	virtual bool Save(void);
	virtual bool Read(void);
	virtual bool ReadItem(int, stack*);
	virtual bool CanRead(void) { return true; } // for now everything and everybody can read...
	virtual uchar GetBurdenState(ulong = 0);
	virtual bool Dip(void);
	virtual void Save(std::ofstream*);
	virtual bool WizardMode(void);
	virtual bool RaiseStats(void);
	virtual bool LowerStats(void);
	virtual bool SeeWholeMap(void);
	virtual bool IncreaseGamma(void);
	virtual bool DecreaseGamma(void);
	virtual bool IncreaseSoftGamma(void);
	virtual bool DecreaseSoftGamma(void);
	virtual bool WalkThroughWalls(void);
	virtual float CWeaponStrength(void);
	virtual bool ShowKeyLayout(void);
	virtual bool Look(void);
	virtual long CStrengthExperience(void) {return StrengthExperience;}
	virtual long CEnduranceExperience(void) {return EnduranceExperience;}
	virtual long CAgilityExperience(void) {return AgilityExperience;}
	virtual long CPerceptionExperience(void) {return PerceptionExperience;}
	virtual float CAttackStrength(void);
	virtual float CDifficulty(void);
	virtual bool Engrave(std::string);
	virtual bool WhatToEngrave(void);
	virtual ushort CRegenerationCounter(void) { return RegenerationCounter; }
	virtual void MoveRandomly(void);
	virtual void SWielded(item* Something) { Wielded = Something; }
	virtual void SMaterial(ushort Where, material* What) { Material[Where] = What; }
	virtual void SHP(short What) { HP = What; }
	virtual void SStrengthExperience(long What) { StrengthExperience = What; }
	virtual void SAgilityExperience(long What) { AgilityExperience = What; }
	virtual void SEnduranceExperience(long What) { EnduranceExperience = What; }
	virtual void SPerceptionExperience(long What) { PerceptionExperience = What; }
	virtual square* CSquareUnder(void) { return SquareUnder; }
	virtual levelsquare* CLevelSquareUnder(void) { return (levelsquare*)SquareUnder; }
	virtual void SAP(long What) { AP = What; }
	virtual bool CFainted(void) { return Fainted; }
	virtual void SFainted(bool To) { Fainted = To; }
	virtual void SNP(long What) { NP = What; }
	virtual void SRelations(uchar What) { Relations = What; }
	virtual long CAP(void) { return AP; }
	virtual void SStrength(ushort What) { Strength = What; if(short(Strength) < 1) Strength = 1; }
	virtual void SEndurance(ushort What) { Endurance = What; if(short(Endurance) < 1) Endurance = 1; }
	virtual void SAgility(ushort What) { Agility = What; if(short(Agility) < 1) Agility = 1; }
	virtual void SPerception(ushort What) { Perception = What; if(short(Perception) < 1) Perception = 1; }
	virtual void SRegenerationCounter(long What) { RegenerationCounter = What; }
	virtual bool TestForPickup(item*);
	virtual bool CanWield(void) { return false; }
	virtual bool CanWear(void) { return false; }
	virtual bool WearItem(item*) { return false; }
	virtual bool OpenPos(vector);
	virtual bool Pray(void);
	virtual void SpillBlood(uchar);
	virtual void HealFully(character*) {}
	virtual bool Kick(void);
	virtual bool ScreenShot(void);
	virtual bool Offer(void);
	virtual ushort LOSRange(void) { return Perception / 3; }
	virtual ushort LOSRangeLevelSquare(void) { return Perception * Perception / 9; }
	virtual long Score(void);
	virtual long AddScoreEntry(std::string, float = 1);
	virtual bool CheckDeath(std::string);
	virtual ulong Danger(void) = 0;
	virtual bool Charmable(void) { return true; }
	virtual bool CheckBulimia(void);
	virtual bool CheckIfConsumable(ushort);
	virtual bool DrawMessageHistory(void);
	virtual bool Throw(void);
	virtual bool ThrowItem(uchar, item*);
	virtual void HasBeenHitByItem(item*, float, bool);
	virtual bool Catches(item*, float, bool) { return false; }
	virtual bool DodgesFlyingItem(item*, float, bool);
	virtual ulong CBloodColor(void) { return RGB(100,0,0); }
	virtual void SConsumingCurrently(ushort What) { EatingCurrently = What; }
	virtual ushort CConsumingCurrently(void) { return EatingCurrently; }
	virtual void ContinueEating(void);
	virtual void StopEating(void);
	virtual void SAPsToBeEaten(long What) { APsToBeEaten = What; }
	virtual long CAPsToBeEaten(void) { return APsToBeEaten; }
	virtual void Vomit(ushort);
	virtual character* Clone(void) const = 0;
	virtual character* Load(std::ifstream*) const = 0;
	virtual ushort Possibility(void) const = 0;
	virtual bool Apply(void);
	virtual bool GainAllItems(void);
	virtual bool ForceVomit(void);
protected:
	virtual void CreateInitialEquipment(void) {}
	virtual void SetDefaultStats(void) = 0;
	virtual void GetPlayerCommand(void);
	virtual void GetAICommand(void);
	virtual void Charge(character*);
	virtual float GetMeleeStrength(void)				{return 0;}
	virtual void HostileAICommand(void);
	virtual void NeutralAICommand(void);
	virtual void FriendAICommand(void);
	virtual std::string ThirdPersonWeaponHitVerb(const bool Critical)	{ return NormalThirdPersonHitVerb(Critical); }
	virtual std::string ThirdPersonMeleeHitVerb(const bool Critical)	{ return NormalThirdPersonHitVerb(Critical); }
	virtual std::string FirstPersonHitVerb(character*, const bool Critical)	{ return NormalFirstPersonHitVerb(Critical); }
	virtual std::string AICombatHitVerb(character*, const bool Critical)	{ return NormalThirdPersonHitVerb(Critical); }
	virtual std::string NormalFirstPersonHitVerb(const bool Critical)	{ return Critical ? "critically hit" : "hit"; }
	virtual std::string NormalThirdPersonHitVerb(const bool Critical)	{ return Critical ? "critically hits" : "hits"; }
	virtual std::string FirstPersonBiteVerb(const bool Critical)		{ return Critical ? "critically bite" : "bite"; }
	virtual std::string ThirdPersonBiteVerb(const bool Critical)		{ return Critical ? "critically bites" : "bites"; }
	virtual std::string FirstPersonPSIVerb(const bool Critical)		{ return Critical ? "emit very powerful psychic waves at" : "emit psychic waves at"; }
	virtual std::string ThirdPersonPSIVerb(const bool Critical)		{ return Critical ? "emits very powerful psychic waves at" : "emits psychic waves at"; }
	virtual std::string FirstPersonBrownSlimeVerb(const bool Critical)	{ return Critical ? "vomit extremely acidous brown slime at" : "vomit brown slime at"; }
	virtual std::string ThirdPersonBrownSlimeVerb(const bool Critical)	{ return Critical ? "vomits extremely acidous brown slime at" : "vomits brown slime at"; }
	virtual std::string FirstPersonPepsiVerb(const bool Critical)		{ return Critical ? "vomit extremely stale pepsi at" : "vomit pepsi at"; }
	virtual std::string ThirdPersonPepsiVerb(const bool Critical)		{ return Critical ? "vomits extremely stale pepsi at" : "vomits pepsi at"; }
	square* SquareUnder;
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
};
/*
#undef RET
#define RET(Val) { return Val; }
#undef RETV
#define RETV(XVal,YVal) { return vector(XVal, YVal); }

#define WEAR_ARMOR public: virtual bool WearArmor(void)
#define C_TORSO_ARMOR public: virtual item* CTorsoArmor(void)
#define GET_SEX public: virtual uchar GetSex(void)
#define CALCULATE_ARMOR_MODIFIER public: virtual ushort CalculateArmorModifier(void)
#define DROP public: virtual bool Drop(void)
#define WIELD public: virtual bool Wield(void)
#define SAVE public: virtual void Save(std::ofstream*)
#define C_ARM_TYPE public: virtual uchar CArmType(void)
#define C_HEAD_TYPE public: virtual uchar CHeadType(void)
#define GET_MELEE_STRENGTH protected: virtual float GetMeleeStrength(void)
#define ADD_HIT_MESSAGE protected: virtual void AddHitMessage(character* Enemy, const bool Critical = false)
#define BE_TALKED_TO public: virtual void BeTalkedTo(character*)
#define C_EMITATION public: virtual ushort CEmitation(void)
#define DIE public: virtual void Die(void)
#define GET_SEX public: virtual uchar GetSex(void)
#define MOVE_RANDOMLY public: virtual void MoveRandomly(void)
#define HIT virtual bool Hit(character*)
#define CAN_WIELD public: virtual bool CanWield(void)
#define CAN_WEAR public: virtual bool CanWear(void)
#define WEAR_ITEM public: virtual bool WearItem(item* What)
#define SPILL_BLOOD public: virtual void SpillBlood(uchar)
#define HOSTILE_AI_COMMAND protected: virtual void HostileAICommand(void)
#define NEUTRAL_AI_COMMAND protected: virtual void NeutralAICommand(void)
#define FRIEND_AI_COMMAND protected: virtual void FriendAICommand(void)
#define HEAL_FULLY public: virtual void HealFully(character*)
#define S_HEALTIMER public: virtual void SHealTimer(ushort What)
#define C_HEALTIMER public: virtual ushort CHealTimer(void)
#define SAVE public: virtual void Save(std::ofstream*)
#define RECEIVE_FIRE_DAMAGE virtual void ReceiveFireDamage(long)
#define OFFER public: virtual bool Offer(void)
#define DANGER public: virtual ulong Danger(void)
#define CHARMABLE public: virtual bool Charmable(void)
#define THROW public: virtual bool Throw(void)
#define THIRD_PERSON_WEAPON_HIT_VERB protected: virtual std::string ThirdPersonWeaponHitVerb(const bool Critical)
#define THIRD_PERSON_MELEE_HIT_VERB protected: virtual std::string ThirdPersonMeleeHitVerb(const bool Critical)
#define FIRST_PERSON_HIT_VERB protected: virtual std::string FirstPersonHitVerb(character*, const bool Critical)
#define AI_COMBAT_HIT_VERB protected: virtual std::string AICombatHitVerb(character*, const bool Critical)
#define CATCHES public: virtual bool Catches(item*, float, bool)
#define CONSUME_ITEM_TYPE public: virtual bool ConsumeItemType(uchar)
#define CREATE_INITIAL_EQUIPMENT protected: virtual void CreateInitialEquipment(void)
#undef POSSIBILITY
#define POSSIBILITY public: virtual ushort Possibility(void) const
#undef APPLY
#define APPLY public: virtual bool Apply(void)
*/
#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define CHARACTER(name, base, initmaterials, setstats, loader, destructor, data)\
	\
	class name : public base\
	{\
	public:\
		name(bool CreateMaterials = true, bool SetStats = true, bool CreateEquipment = true) : base(false, false, false) { if(CreateMaterials) initmaterials ; if(SetStats) { SetDefaultStats(); SHP(CEndurance() * 2); } if(CreateEquipment) CreateInitialEquipment(); }\
		name(std::ifstream* SaveFile) : base(SaveFile) loader\
		virtual ~name() destructor\
		virtual character* Clone(void) const { return new name; }\
		virtual character* Load(std::ifstream* SaveFile) const { return new name(SaveFile); }\
	protected:\
		virtual void SetDefaultStats(void) { setstats }\
		virtual ushort Type(void);\
		data\
	};\
	\
	class proto_##name\
	{\
	public:\
		proto_##name(void) : Index(game::CCharacterPrototype().Add(new name)) {}\
		ushort GetIndex(void) { return Index; }\
	private:\
		ushort Index;\
	} static Proto_##name;\
	\
	ushort name::Type(void) { return Proto_##name.GetIndex(); }

#else

	#define CHARACTER(name, base, initmaterials, setstats, loader, destructor, data)\
	\
	class name : public base\
	{\
	public:\
		name(bool CreateMaterials = true, bool SetStats = true, bool CreateEquipment = true) : base(false, false, false) { if(CreateMaterials) initmaterials ; if(SetStats) { SetDefaultStats(); SHP(CEndurance() * 2); } if(CreateEquipment) CreateInitialEquipment(); }\
		name(std::ifstream* SaveFile) : base(SaveFile) loader\
		virtual ~name() destructor\
		virtual character* Clone(void) const { return new name; }\
		virtual character* Load(std::ifstream* SaveFile) const { return new name(SaveFile); }\
	protected:\
		virtual void SetDefaultStats(void) { setstats }\
		virtual ushort Type(void);\
		data\
	};

#endif

#define ABSTRACT_CHARACTER(name, base, loader, destructor, data)\
\
class name : public base\
{\
public:\
	name(bool CreateMaterials, bool SetStats, bool CreateEquipment) : base(CreateMaterials, SetStats, CreateEquipment) {}\
	name(std::ifstream* SaveFile) : base(SaveFile) loader\
	virtual ~name() destructor\
	data\
};

ABSTRACT_CHARACTER(
	humanoid,
	character,
	{
		ushort Index;
		SaveFile->read((char*)&Index, sizeof(Index));
		Armor.Torso = Index != 0xFFFF ? Stack->CItem(Index) : 0;
		SaveFile->read((char*)&ArmType, sizeof(ArmType));
		SaveFile->read((char*)&HeadType, sizeof(HeadType));
		SaveFile->read((char*)&LegType, sizeof(LegType));
		SaveFile->read((char*)&TorsoType, sizeof(TorsoType));
	},
	{},
public:
	virtual void DrawToTileBuffer(void);
	virtual bool WearArmor(void);
	virtual item* CTorsoArmor(void) RET(Armor.Torso)
	virtual uchar GetSex(void) RET(MALE)
	virtual ushort CalculateArmorModifier(void);
	virtual bool Drop(void);
	virtual bool Wield(void);
	virtual void Save(std::ofstream*);
	virtual bool Throw(void);
	virtual uchar CArmType(void) RET(ArmType)
	virtual uchar CHeadType(void) RET(HeadType)
	virtual bool CanWield(void) RET(true)
	virtual bool CanWear(void) RET(true)
	virtual bool WearItem(item* What) { Armor.Torso = What; return true; }
	class armor {public: armor(void); item* Torso; item* Legs; item* Hands; item* Head; item* Feet;} Armor;
	uchar ArmType; uchar HeadType; uchar LegType; uchar TorsoType;
	virtual vector CBitmapPos(void) RETV(0,0)
	virtual bool Apply(void);
	virtual void SArmType(uchar Value) { ArmType = Value; }
	virtual void SHeadType(uchar Value) { HeadType = Value; }
	virtual void SLegType(uchar Value) { LegType = Value; }
	virtual void STorsoType(uchar Value) { TorsoType = Value; }
protected:
	virtual float GetMeleeStrength(void) RET(2000)
);

inline humanoid::armor::armor(void) : Torso(0), Legs(0), Hands(0), Head(0), Feet(0) {}

CHARACTER(
	human,
	humanoid,
	InitMaterials(new humanflesh(80000)),
	{
		SSize(150 + rand() % 51);
		SAgility(15 + rand() % 11);
		SStrength(10 + rand() % 6);
		SEndurance(10 + rand() % 6);
		SPerception(10 + rand() % 6);
		SArmType(rand() % NUMBER_OF_HUMAN_ARMS);
		SHeadType(rand() % NUMBER_OF_HUMAN_HEADS);
		SLegType(rand() % NUMBER_OF_HUMAN_LEGS);
		STorsoType(rand() % NUMBER_OF_HUMAN_TORSOS);
	},
	{},
	{},
public:
	virtual std::string NameSingular(void) const RET("human")
	virtual std::string NamePlural(void) const RET("humans")
	virtual ulong Danger(void) RET(0)
	virtual ushort Possibility(void) const RET(0)
);
                      
CHARACTER(
	perttu,
	human,
	InitMaterials(new humanflesh(80000)),
	{
		SSize(200);
		SAgility(80);
		SStrength(80);
		SEndurance(80);
		SPerception(80);
		SArmType(3);
		SHeadType(15);
		SLegType(2);
		STorsoType(3);
		SRelations(1);
		SHealTimer(100);
	},
	{
		SaveFile->read((char*)&HealTimer, sizeof(HealTimer));
	},
	{},
public:
	virtual std::string NameSingular(void) const RET("Perttu, the Überpriest of the Great Frog")
	virtual std::string NamePlural(void) const RET("Perttus, the Überpriests of the Great Frog")
	virtual std::string Name(uchar Case) RET(NameProperNoun(Case))
	virtual void BeTalkedTo(character*);
	virtual ushort CEmitation(void) RET(333)
	virtual void Die(void);
	virtual void HealFully(character*);
	virtual void SHealTimer(ushort What) { HealTimer = What; }
	virtual ushort CHealTimer(void) RET(HealTimer)
	virtual void ReceiveFireDamage(long) {}
	protected: ushort HealTimer;
	virtual void Save(std::ofstream*);
	virtual ulong Danger(void) RET(150000)
	virtual bool Charmable(void) RET(false)
	virtual ushort Possibility(void) const RET(0)
protected:
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) RET(10000)
	virtual void NeutralAICommand(void);
);

CHARACTER(
	oree,
	character,
	InitMaterials(new pepsi(110000)),
	{
		SSize(225);
		SAgility(40);
		SStrength(40);
		SEndurance(40);
		SPerception(27);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(0)
	virtual std::string Name(uchar Case) RET(NameProperNoun(Case))
	virtual uchar GetSex(void) RET(MALE)
	virtual ushort CalculateArmorModifier(void) RET(10)
	virtual std::string NameSingular(void) const RET("Oree the Pepsi Daemon King")
	virtual std::string NamePlural(void) const RET("Orees the Pepsi Daemon Kings")
	virtual ulong Danger(void) RET(30000)
	virtual bool Charmable(void) RET(false)
	virtual vector CBitmapPos(void) RETV(208,0)
protected:
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) RET(40000)
	virtual std::string ThirdPersonMeleeHitVerb(const bool Critical) RET(ThirdPersonPepsiVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, const bool Critical) RET(FirstPersonPepsiVerb(Critical))
	virtual std::string AICombatHitVerb(character*, const bool Critical) RET(ThirdPersonPepsiVerb(Critical))
);

CHARACTER(
	swatcommando,
	character,
	InitMaterials(new humanflesh(110000)),
	{
		SSize(200);
		SAgility(30);
		SStrength(20);
		SEndurance(15);
		SPerception(27);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(5)
	virtual uchar GetSex(void) RET(MALE)
	virtual std::string NameSingular(void) const RET("Bill's SWAT commando")
	virtual std::string NamePlural(void) const RET("Bill's SWAT commandos")
	virtual bool CanWield(void) RET(true)
	virtual bool CanWear(void) RET(false)
	virtual ulong Danger(void) RET(750)
	virtual bool Charmable(void) RET(false)
	virtual vector CBitmapPos(void) RETV(128,0)
protected:
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) RET(10000)
);

CHARACTER(
	ennerbeast,
	character,
	InitMaterials(new ennerbeastflesh(60000)),
	{
		SSize(150);
		SAgility(10);
		SStrength(5);
		SEndurance(10);
		SPerception(9);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(0)
	virtual bool Hit(character*);
	virtual std::string NameSingular(void) const RET("Enner Beast")
	virtual std::string NamePlural(void) const RET("Enner Beasts")
	virtual ulong Danger(void) RET(2500);
	virtual bool Charmable(void) RET(false)
	virtual vector CBitmapPos(void) RETV(96,0)
protected:
	virtual float GetMeleeStrength(void) RET(200000)
	virtual void HostileAICommand(void);
);

ABSTRACT_CHARACTER(
	frog,
	character,
	{},
	{},
protected:
	virtual std::string ThirdPersonMeleeHitVerb(const bool Critical) RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, const bool Critical) RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, const bool Critical) RET(ThirdPersonBiteVerb(Critical))
	virtual float GetMeleeStrength(void) RET(20000)
);

CHARACTER(
	darkfrog,
	frog,
	InitMaterials(new darkfrogflesh(100)),
	{
		SSize(15);
		SAgility(20);
		SStrength(1);
		SEndurance(2);
		SPerception(15);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(100)
	virtual std::string NameSingular(void) const RET("dark frog")
	virtual std::string NamePlural(void) const RET("dark frogs")
	virtual ulong Danger(void) RET(25)
	virtual vector CBitmapPos(void) RETV(80,0)
);

CHARACTER(
	elpuri,
	darkfrog,
	InitMaterials(new elpuriflesh(8000000)),
	{
		SSize(300);
		SAgility(50);
		SStrength(50);
		SEndurance(50);
		SPerception(18);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(0)
	virtual std::string Name(uchar Case) RET(NameProperNoun(Case))
	virtual std::string NameSingular(void) const RET("Elpuri the Dark Frog")
	virtual std::string NamePlural(void) const RET("Elpuris the Dark Frogs")
	virtual void Die(void);
	virtual ulong Danger(void) RET(5000)
	virtual bool Charmable(void) RET(false)
	virtual vector CBitmapPos(void) RETV(64,0)
);

CHARACTER(
	billswill,
	character,
	InitMaterials(new air(500000)),
	{
		SSize(100);
		SAgility(40);
		SStrength(5);
		SEndurance(5);
		SPerception(27);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(50)
	virtual void Die(void);
	virtual std::string NameSingular(void) const RET("pure mass of Bill's will")
	virtual std::string NamePlural(void) const RET("pure masses of Bill's will")
	virtual void SpillBlood(uchar) {}
	virtual ulong Danger(void) RET(75)
	virtual bool Charmable(void) RET(false)
	virtual vector CBitmapPos(void) RETV(48,0)
protected:
	virtual float GetMeleeStrength(void) RET(30000)
	virtual std::string ThirdPersonMeleeHitVerb(const bool Critical) RET(ThirdPersonPSIVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, const bool Critical) RET(FirstPersonPSIVerb(Critical))
	virtual std::string AICombatHitVerb(character*, const bool Critical) RET(ThirdPersonPSIVerb(Critical))
);

CHARACTER(
	fallenvalpurist,
	character,
	InitMaterials(new bone(60000)),
	{
		SSize(150);
		SAgility(10);
		SStrength(5);
		SEndurance(5);
		SPerception(15);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(50)
	virtual std::string NameSingular(void) const RET("fallen valpurist")
	virtual std::string NamePlural(void) const RET("fallen valpurists")
	virtual bool CanWield(void) RET(true)
	virtual bool CanWear(void) RET(false)
	virtual void Die(void);
	virtual ulong Danger(void) RET(25)
	virtual vector CBitmapPos(void) RETV(112,0)
protected:
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) RET(5000)
);

CHARACTER(
	froggoblin,
	character,
	InitMaterials(new goblinoidflesh(25000)),
	{
		SSize(100);
		SAgility(15);
		SStrength(10);
		SEndurance(5);
		SPerception(21);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(100)
	virtual std::string NameSingular(void) const RET("frog-goblin hybrid")
	virtual std::string NamePlural(void) const RET("frog-goblin hybrids")
	virtual bool CanWield(void) RET(true)
	virtual ulong Danger(void) RET(25)
	virtual vector CBitmapPos(void) RETV(144,0)
protected:
	virtual void CreateInitialEquipment(void);
	virtual float GetMeleeStrength(void) RET(5000)
);

ABSTRACT_CHARACTER(
	mommo,
	character,
	{},
	{},
protected:
	virtual std::string ThirdPersonMeleeHitVerb(const bool Critical) RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, const bool Critical) RET(FirstPersonBrownSlimeVerb(Critical))
	virtual std::string AICombatHitVerb(character*, const bool Critical) RET(ThirdPersonBrownSlimeVerb(Critical))
	virtual float GetMeleeStrength(void) RET(25000)
);

CHARACTER(
	conicalmommo,
	mommo,
	InitMaterials(new brownslime(250000)),
	{
		SSize(100);
		SAgility(1);
		SStrength(2);
		SEndurance(40);
		SPerception(9);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(25)
	virtual std::string NameSingular(void) const RET("conical mommo slime")
	virtual std::string NamePlural(void) const RET("conical mommo slimes")
	virtual ulong Danger(void) RET(250)
	virtual vector CBitmapPos(void) RETV(176,0)
);

CHARACTER(
	flatmommo,
	mommo,
	InitMaterials(new brownslime(150000)),
	{
		SSize(50);
		SAgility(2);
		SStrength(1);
		SEndurance(20);
		SPerception(9);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(75)
	virtual std::string NameSingular(void) const RET("flat mommo slime")
	virtual std::string NamePlural(void) const RET("flat mommo slimes")
	virtual ulong Danger(void) RET(75)
	virtual vector CBitmapPos(void) RETV(192,0)
);

CHARACTER(
	golem,
	character,
	InitMaterials(game::CreateRandomSolidMaterial(100000)),
	{
		SSize(250);
		SAgility(5);
		SStrength(35);
		SEndurance(20);
		SPerception(12);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(20)
	virtual ushort CalculateArmorModifier(void);
	virtual void Die(void);
	virtual void MoveRandomly(void);
	virtual std::string NameSingular(void) const;
	virtual std::string NamePlural(void) const RET("golems")
	virtual ulong Danger(void);
	virtual vector CBitmapPos(void) RETV(256,0)
	virtual void DrawToTileBuffer(void);
	virtual void SpillBlood(uchar) {}
protected:
	virtual float GetMeleeStrength(void);
);

CHARACTER(
	wolf,
	character,
	InitMaterials(new wolfflesh(30000)),
	{
		SSize(100);
		SAgility(20);
		SStrength(6);
		SEndurance(6);
		SPerception(24);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(40)
	virtual std::string NameSingular(void) const RET("wolf")
	virtual std::string NamePlural(void) const RET("wolves")
	virtual ulong Danger(void) RET(20)
	virtual vector CBitmapPos(void) RETV(224,0)
protected:
	virtual float GetMeleeStrength(void) RET(7500)
	virtual std::string ThirdPersonMeleeHitVerb(const bool Critical) RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, const bool Critical) RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, const bool Critical) RET(ThirdPersonBiteVerb(Critical))
);

CHARACTER(
	dog,
	character,
	InitMaterials(new dogflesh(20000)),
	{
		SSize(75);
		SAgility(15);
		SStrength(4);
		SEndurance(4);
		SPerception(21);
	},
	{},
	{},
public:
	virtual ushort Possibility(void) const RET(20)
	virtual std::string NameSingular(void) const RET("dog")
	virtual std::string NamePlural(void) const RET("dogs")
	virtual ulong Danger(void) RET(10)
	virtual bool Catches(item*, float, bool);
	virtual bool ConsumeItemType(uchar);
	virtual vector CBitmapPos(void) RETV(240,0)
protected:
	virtual float GetMeleeStrength(void) RET(5000)
	virtual std::string ThirdPersonMeleeHitVerb(const bool Critical) RET(ThirdPersonBiteVerb(Critical))
	virtual std::string FirstPersonHitVerb(character*, const bool Critical) RET(FirstPersonBiteVerb(Critical))
	virtual std::string AICombatHitVerb(character*, const bool Critical) RET(ThirdPersonBiteVerb(Critical))
);

#endif
