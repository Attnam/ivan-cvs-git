#ifndef __CHARBA_H__
#define __CHARBA_H__

#define HAS_HIT 0
#define HAS_BLOCKED 1
#define HAS_DODGED 2
#define HAS_DIED 3

#define NUMBER_OF_HUMAN_LEGS	10
#define NUMBER_OF_HUMAN_TORSOS	14
#define NUMBER_OF_HUMAN_ARMS	14
#define NUMBER_OF_HUMAN_HEADS	23

#define HUNGERLEVEL 		500
#define CRITICALHUNGERLEVEL 	100

#define OVERLOADED		0
#define STRESSED		1
#define BURDENED		2
#define UNBURDENED		3

#define STATES			5

#define FAINTED		0
#define CONSUMING		1
#define POLYMORPHED		2
#define RESTING		3
#define DIGGING		4

#define CHARACTER_OUTLINE_COLOR	BLUE

#include "game.h"
#include "object.h"
#include "typedef.h"
#include "vector2d.h"
#include "igraph.h"

class square;
class bitmap;
class character;
class item;
class stack;
class material;
class levelsquare;
class worldmapsquare;
class outputfile;
class inputfile;
class team;

/* Presentation of the character class */

class character : public object
{
public:
	character(bool, bool, bool, bool = true);
	virtual ~character();
	virtual character* Clone(bool = true, bool = true, bool = true) const = 0;
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual bool CanRead() const { return true; }
	virtual bool CanWear() const { return false; }
	virtual bool CanWield() const { return false; }
	virtual bool Charmable() const { return true; }
	virtual bool Catches(item*, float) { return false; }
	virtual bool CheckBulimia() const;
	virtual bool CheckDeath(std::string, bool = false);
	virtual bool CheckIfConsumable(ushort) const;
	virtual bool ConsumeItem(item*, stack*);
	virtual bool ConsumeItemType(uchar) const;
	virtual bool DodgesFlyingItem(item*, float);
	virtual bool Hit(character*);
	virtual bool OpenItem();
	virtual bool OpenPos(vector2d);
	virtual bool ReadItem(int, stack*);
	virtual bool TestForPickup(item*) const;
	virtual bool ThrowItem(uchar, item*);
	virtual bool TryMove(vector2d, bool = true);
	virtual bool HasHeadOfElpuri() const;
	virtual bool HasMaakotkaShirt() const;
	virtual bool HasPerttusNut() const;
	virtual bool GetIsPlayer() const { return IsPlayer; }
	virtual bool Apply();
	virtual bool Close();
	virtual bool Consume();
	virtual bool DecreaseSoftGamma();
	virtual bool Dip();
	virtual bool DrawMessageHistory();
	virtual bool Drop();
	virtual bool Engrave(std::string);
	virtual bool ForceVomit();
	virtual bool GainAllItems();
	virtual bool GoDown();
	virtual bool GoUp();
	virtual bool IncreaseSoftGamma();
	virtual bool Kick();
	virtual bool Look();
	virtual bool LowerStats();
	virtual bool NOP();
	virtual bool Offer();
	virtual bool Open();
	virtual bool PickUp();
	virtual bool Pray();
	virtual bool Quit();
	virtual bool RaiseStats();
	virtual bool Read();
	virtual bool Save();
	virtual bool SeeWholeMap();
	virtual bool ShowInventory();
	virtual bool ShowKeyLayout();
	virtual bool ShowWeaponSkills();
	virtual bool Talk();
	virtual bool Throw();
	virtual bool WalkThroughWalls();
	virtual bool WearArmor();
	virtual bool WhatToEngrave();
	virtual bool Wield();
	virtual bool WizardMode();
	virtual void AddScoreEntry(std::string, float = 1, bool = true) const;
	virtual long Score() const;
	virtual float GetAttackStrength() const;
	virtual item* GetTorsoArmor() const				{ return 0; }
	virtual item* GetWielded() const				{ return Wielded; }
	virtual worldmapsquare* GetWorldMapSquareUnder() const;
	virtual long GetAgilityExperience() const { return AgilityExperience; }
	virtual long GetAP() const { return AP; }
	virtual long GetEnduranceExperience() const { return EnduranceExperience; }
	virtual long GetNP() const					{ return NP; }
	virtual long GetPerceptionExperience() const { return PerceptionExperience; }
	virtual long GetStrengthExperience() const { return StrengthExperience; }
	virtual short GetHP() const					{ return HP; }
	virtual stack* GetStack() const				{ return Stack; }
	virtual uchar GetBurdenState(ulong = 0) const;
	virtual uchar GetSex() const { return UNDEFINED; }
	virtual uchar TakeHit(character*, short);
	virtual ulong Danger() const;
	virtual ulong GetBloodColor() const;
	virtual ushort CalculateArmorModifier() const;
	virtual ushort GetRegenerationCounter() const { return RegenerationCounter; }
	virtual ushort GetAgility() const					{ return Agility; }
	virtual item* GetConsumingCurrently() const { return StateVariables.Consuming.ConsumingCurrently; }
	virtual ushort GetEmitation() const;
	virtual ushort GetEndurance() const					{ return Endurance; }
	virtual ushort GetPerception() const				{ return Perception; }
	virtual ushort GetStrength() const					{ return Strength; }
	virtual ushort LOSRange() const;
	virtual ushort LOSRangeSquare() const;
	virtual ushort Possibility() const = 0;
	virtual vector2d GetPos() const;
	virtual void AddBlockMessage(character*) const;
	virtual void AddDodgeMessage(character*) const;
	virtual void AddHitMessage(character*, const bool = false) const;
	virtual void ApplyExperience();
	virtual void BeTalkedTo(character*);
	virtual void Darkness(long);
	virtual void Die(bool = false);
	virtual void DrawToTileBuffer() const;
	virtual void HasBeenHitByItem(item*, float);
	virtual void HealFully(character*) {}
	virtual void Hunger(ushort = 1);
	virtual void Move(vector2d, bool = false);
	virtual void MoveRandomly();
	virtual void ReceiveBulimiaDamage();
	virtual void ReceiveFireDamage(long);
	virtual void ReceiveNutrition(long);
	virtual void ReceiveOmleUrineEffect(long);
	virtual void ReceivePepsiEffect(long);
	virtual void ReceiveSchoolFoodEffect(long);
	virtual void ReceiveSound(char*, short, float);
	virtual void Regenerate(ushort = 1);
	virtual void SetAgility(ushort What) { Agility = What; if(short(Agility) < 1) Agility = 1; if(Agility > 99) Agility = 99; }
	virtual void SetAgilityExperience(long What) { AgilityExperience = What; }
	virtual void SetAP(long What) { AP = What; }
	virtual void SetConsumingCurrently(item* What) { StateVariables.Consuming.ConsumingCurrently = What; }
	virtual void SetEndurance(ushort What) { Endurance = What; if(short(Endurance) < 1) Endurance = 1; if(Endurance > 99) Endurance = 99; }
	virtual void SetEnduranceExperience(long What) { EnduranceExperience = What; }
	virtual void SetHP(short What) { HP = What; }
	virtual void SetIsPlayer(bool What) { IsPlayer = What; }
	virtual void SetNP(long);
	virtual void SetPerception(ushort What) { Perception = What; if(short(Perception) < 1) Perception = 1; if(Perception > 99) Perception = 99; }
	virtual void SetPerceptionExperience(long What) { PerceptionExperience = What; }
	virtual void SetRegenerationCounter(long What) { RegenerationCounter = What; }
	virtual void SetSquareUnder(square* Square);
	virtual void SetStrength(ushort What) { Strength = What; if(short(Strength) < 1) Strength = 1; if(Strength > 99) Strength = 99; }
	virtual void SetStrengthExperience(long What) { StrengthExperience = What; }
	virtual void SetWielded(item* Something) { Wielded = Something; }
	virtual void SpillBlood(uchar);
	virtual void SpillBlood(uchar, vector2d);
	virtual void Vomit(ushort);
	virtual void Be();
	virtual bool Zap();
	virtual bool Polymorph(character*);
	virtual bool SetTorsoArmor(item* What) RET(false)
	virtual bool CanKick() const RET(false)
	virtual void BeKicked(ushort, bool, uchar, character*);
	virtual void FallTo(vector2d, bool);
	virtual bool CheckCannibalism(ushort);
	virtual uchar GetGraphicsContainerIndex() const { return GCHARACTER; }
	virtual void VirtualConstructor() {}
	virtual void CharacterSpeciality() {}
	virtual void ActivateState(uchar Index) { State |= 1 << Index; }
	virtual void DeActivateState(uchar Index) { State &= ~(1 << Index); }
	virtual bool StateIsActivated(uchar Index) const { return State & (1 << Index) ? true : false; }
	virtual void Faint();
	virtual void FaintHandler();
	virtual void ConsumeHandler();
	virtual void PolymorphHandler();
	virtual void SetStateCounter(uchar Index, ushort What) { StateCounter[Index] = What; }
	virtual bool CanMove();
	virtual void DeActivateVoluntaryStates(std::string = "");
	virtual void EndFainted();
	virtual void EndConsuming();
	virtual void EndPolymorph();
	virtual void StruckByWandOfStriking();
	virtual void StateAutoDeactivation();
	virtual team* GetTeam() const { return Team; }
	virtual void SetTeam(team* What) { Team = What; }
	virtual bool RestUntilHealed();
	virtual void RestHandler();
	virtual void EndRest();
	virtual void DigHandler();
	virtual void EndDig();
	virtual void SetOldWieldedItem(item* What) { StateVariables.Digging.OldWieldedItem = What; }
	virtual void SetSquareBeingDigged(vector2d What) { StateVariables.Digging.SquareBeingDiggedX = What.X; StateVariables.Digging.SquareBeingDiggedY = What.Y; }
	virtual item* GetOldWieldedItem() const { return StateVariables.Digging.OldWieldedItem; }
	virtual vector2d GetSquareBeingDigged() const { return vector2d(StateVariables.Digging.SquareBeingDiggedX, StateVariables.Digging.SquareBeingDiggedY); }
	virtual bool OutlineCharacters();
	virtual bool OutlineItems();
	virtual float GetThrowStrengthModifier() const;
	virtual ushort GetMaxHP() const { return GetEndurance() << 1; }
	virtual ushort GetMeleeAttributeModifier() const;
	virtual float GetToHitValue() const;
	virtual float GetDodgeValue() const;
	virtual bool RaiseGodRelations();
	virtual bool LowerGodRelations();
	virtual bool GainDivineKnowledge();
	virtual ulong GetMoney() const { return Money; }
	virtual void SetMoney(ulong What) { Money = What; }
	virtual void SetHomeRoom(uchar What) { HomeRoom = What; }
	virtual uchar GetHomeRoom() const { return HomeRoom; }
	virtual bool Displace(character*);
	virtual bool Sit();
	virtual void AddSpecialItemInfo(std::string&, item*) {}
	virtual void AddSpecialItemInfoDescription(std::string&) {}
	virtual long StatScore() const;
	virtual bool CheckStarvationDeath(std::string);
	virtual void ShowNewPosInfo() const;
	virtual void Hostility(character*);
	virtual void KickHit() {}
	virtual bool HasInfraVision() const { return false; }
	virtual bool SetAutosaveInterval();
	virtual uchar CriticalModifier() const { return 20; }
	virtual std::string StandVerb() const { return "standing"; }
	virtual stack* GetGiftStack() const;
	virtual void MoveRandomlyInRoom();
protected:
	virtual void SeekLeader();
	virtual bool CheckForUsefulItemsOnGround();
	virtual bool CheckForDoors();
	virtual bool CheckForEnemies();
	virtual bool FollowLeader();
	virtual void StandIdleAI();
	virtual void CreateCorpse();
	virtual std::string DeathMessage() { return Name(DEFINITE) + " dies screaming."; }
	virtual void CreateInitialEquipment() {}
	virtual void SetDefaultStats() = 0;
	virtual void GetPlayerCommand();
	virtual void GetAICommand();
	virtual void MoveTowards(vector2d);
	virtual float GetMeleeStrength() const				{ return 0; }
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
	virtual std::string TalkVerb() const { return "grunts"; }
	stack* Stack;
	item* Wielded;
	ushort Strength, Endurance, Agility, Perception, RegenerationCounter;
	short HP;
	long NP, AP;
	long StrengthExperience, EnduranceExperience, AgilityExperience, PerceptionExperience;
	bool IsPlayer;
	uchar State;
	ushort StateCounter[STATES];
	void (character::*StateHandler[STATES])();
	team* Team;
	vector2d WayPoint;
	union statevariables
	{
		struct consuming
		{
			item* ConsumingCurrently;
		} Consuming;
		struct digging
		{
			ushort SquareBeingDiggedX;
			ushort SquareBeingDiggedY;
			item* OldWieldedItem;
		} Digging;
	} StateVariables;
	ulong Money;
	uchar HomeRoom;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define CHARACTER_PROTOINSTALLER(name, base, initmaterials, setstats)\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller() : Index(protocontainer<character>::Add(new name(false, false, false, false))) {}\
		ushort GetIndex() const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	name::name(bool CreateMaterials, bool SetStats, bool CreateEquipment, bool AddToPool) : base(false, false, false, AddToPool) { if(CreateMaterials) initmaterials ; if(SetStats) { SetDefaultStats(); SetHP(GetMaxHP()); } if(CreateEquipment) CreateInitialEquipment(); }\
	character* name::Clone(bool CreateMaterials, bool SetStats, bool CreateEquipment) const { return new name(CreateMaterials, SetStats, CreateEquipment); }\
	typeable* name::CloneAndLoad(inputfile& SaveFile) const { character* Char = new name(false, false, false); Char->Load(SaveFile); return Char; }\
	void name::SetDefaultStats() { setstats }\
	ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
	const character* const name::GetPrototype() { return protocontainer<character>::GetProto(StaticType()); }\
	ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define CHARACTER_PROTOINSTALLER(name, base, initmaterials, setstats)

#endif

#define CHARACTER(name, base, initmaterials, setstats, data)\
\
name : public base\
{\
public:\
	name(bool = true, bool = true, bool = true, bool = true);\
	name(material* Material, bool SetStats = true, bool CreateEquipment = true) : base(false, false, false) { InitMaterials(Material); if(SetStats) { SetDefaultStats(); SetHP(GetMaxHP()); } if(CreateEquipment) CreateInitialEquipment(); VirtualConstructor(); }\
	virtual character* Clone(bool = true, bool = true, bool = true) const;\
	virtual typeable* CloneAndLoad(inputfile&) const;\
	static ushort StaticType();\
	static const character* const GetPrototype();\
	virtual std::string ClassName() const { return #name; }\
protected:\
	virtual void SetDefaultStats();\
	virtual ushort Type() const;\
	data\
}; CHARACTER_PROTOINSTALLER(name, base, initmaterials, setstats)

#define ABSTRACT_CHARACTER(name, base, data)\
\
name : public base\
{\
public:\
	name(bool CreateMaterials, bool SetStats, bool CreateEquipment, bool AddToPool = true) : base(CreateMaterials, SetStats, CreateEquipment, AddToPool) { VirtualConstructor(); }\
	data\
};

#endif
