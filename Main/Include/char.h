/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __CHAR_H__
#define __CHAR_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "bodypart.h"
#include "script.h"
#include "wskill.h"

#define CHAR_PERSONAL_PRONOUN GetPersonalPronoun(true).CStr()
#define CHAR_POSSESSIVE_PRONOUN GetPossessivePronoun(true).CStr()
#define CHAR_OBJECT_PRONOUN GetObjectPronoun(true).CStr()
#define CHAR_PERSONAL_PRONOUN_THIRD_PERSON_VIEW GetPersonalPronoun(false).CStr()
#define CHAR_POSSESSIVE_PRONOUN_THIRD_PERSON_VIEW GetPossessivePronoun(false).CStr()
#define CHAR_OBJECT_PRONOUN_THIRD_PERSON_VIEW GetObjectPronoun(false).CStr()

class go;
class team;
class wsquare;
class cweaponskill;
class action;
class characterprototype;
struct homedata;

typedef std::vector<std::pair<double, int> > blockvector;
typedef bool (item::*sorter)(const character*) const;
typedef bool (character::*petmanagementfunction)();

inline int APBonus(int Attribute)
{
  return Attribute >= 10 ? 90 + Attribute : 50 + Attribute * 5;
}

struct characterdatabase : public databasebase
{
  typedef characterprototype prototype;
  void InitDefaults(const prototype*, int);
  bool AllowRandomInstantiation() const { return CanBeGenerated && !IsUnique; }
  void PostProcess();
  const prototype* ProtoType;
  double NaturalExperience[ATTRIBUTES];
  ulong Flags;
  bool IsAbstract;
  bool CanRead;
  bool IsCharmable;
  bool CanBeGenerated;
  bool CanOpen;
  bool CanBeDisplaced;
  bool IsUnique;
  bool IsNameable;
  bool IsPolymorphable;
  bool CanUseEquipment;
  bool CanKick;
  bool CanTalk;
  bool CanBeWished;
  bool CreateDivineConfigurations;
  bool CreateGolemMaterialConfigurations;
  bool CanBeCloned;
  bool CanZap;
  bool HasFeet;
  bool IgnoreDanger;
  bool IsExtraCoward;
  bool SpillsBlood;
  bool HasEyes;
  bool HasHead;
  bool CanThrow;
  bool UsesNutrition;
  bool BodyPartsDisappearWhenSevered;
  bool CanBeConfused;
  bool CanApply;
  bool BiteCapturesBodyPart;
  bool IsPlant;
  bool DestroysWalls;
  bool IsRooted;
  bool HasSecondaryMaterial;
  bool IsImmuneToLeprosy;
  bool AutomaticallySeen;
  bool CanHear;
  int DefaultEndurance;
  int DefaultPerception;
  int DefaultIntelligence;
  int DefaultWisdom;
  int DefaultCharisma;
  int DefaultMana;
  int DefaultArmStrength;
  int DefaultLegStrength;
  int DefaultDexterity;
  int DefaultAgility;
  long DefaultMoney;
  int TotalSize;
  int Sex;
  int CriticalModifier;
  festring StandVerb;
  int Frequency;
  int EnergyResistance;
  int FireResistance;
  int PoisonResistance;
  int ElectricityResistance;
  int AcidResistance;
  int ConsumeFlags;
  long TotalVolume;
  packedvector2d HeadBitmapPos;
  packedvector2d TorsoBitmapPos;
  packedvector2d ArmBitmapPos;
  packedvector2d LegBitmapPos;
  packedvector2d RightArmBitmapPos;
  packedvector2d LeftArmBitmapPos;
  packedvector2d RightLegBitmapPos;
  packedvector2d LeftLegBitmapPos;
  packedvector2d GroinBitmapPos;
  packedcolor16 ClothColor;
  packedcolor16 SkinColor;
  packedcolor16 CapColor;
  packedcolor16 HairColor;
  packedcolor16 EyeColor;
  packedcolor16 TorsoMainColor;
  packedcolor16 BeltColor;
  packedcolor16 BootColor;
  packedcolor16 TorsoSpecialColor;
  packedcolor16 ArmMainColor;
  packedcolor16 GauntletColor;
  packedcolor16 ArmSpecialColor;
  packedcolor16 LegMainColor;
  packedcolor16 LegSpecialColor;
  color24 BaseEmitation;
  festring Article;
  festring Adjective;
  festring AdjectiveArticle;
  festring NameSingular;
  festring NamePlural;
  festring PostFix;
  int ArticleMode;
  int BaseUnarmedStrength;
  int BaseBiteStrength;
  int BaseKickStrength;
  int AttackStyle;
  long ClassStates;
  fearray<festring> Alias;
  contentscript<item> Helmet;
  contentscript<item> Amulet;
  contentscript<item> Cloak;
  contentscript<item> BodyArmor;
  contentscript<item> Belt;
  contentscript<item> RightWielded;
  contentscript<item> LeftWielded;
  contentscript<item> RightRing;
  contentscript<item> LeftRing;
  contentscript<item> RightGauntlet;
  contentscript<item> LeftGauntlet;
  contentscript<item> RightBoot;
  contentscript<item> LeftBoot;
  int AttributeBonus;
  fearray<long> KnownCWeaponSkills;
  fearray<long> CWeaponSkillHits;
  int RightSWeaponSkillHits;
  int LeftSWeaponSkillHits;
  int PanicLevel;
  fearray<contentscript<item> > Inventory;
  int DangerModifier;
  festring DefaultName;
  fearray<festring> FriendlyReplies;
  fearray<festring> HostileReplies;
  int FleshMaterial;
  festring DeathMessage;
  int HPRequirementForGeneration;
  int DayRequirementForGeneration;
  int AttackWisdomLimit;
  int AttachedGod;
  packedvector2d WieldedPosition;
  int SparklingFlags;
  int MoveType;
  int BloodMaterial;
  int VomitMaterial;
  int PolymorphIntelligenceRequirement;
  ulong DefaultCommandFlags;
  ulong ConstantCommandFlags;
  bool WillCarryItems;
  festring ForceVomitMessage;
  int SweatMaterial;
  bool Sweats;
  bool IsImmuneToItemTeleport;
  bool AlwaysUseMaterialAttributes;
  bool IsEnormous;
  fearray<festring> ScienceTalkAttribute;
  fearray<festring> ScienceTalkPrefix;
  fearray<festring> ScienceTalkName;
  int ScienceTalkPossibility;
  int ScienceTalkIntelligenceModifier;
  int ScienceTalkWisdomModifier;
  int ScienceTalkIntelligenceRequirement;
  int ScienceTalkWisdomRequirement;
};

class characterprototype
{
 public:
  friend class databasecreator<character>;
  friend class protosystem;
  characterprototype(characterprototype*, character* (*)(int, int), const char*);
  character* Clone(int Config = 0, int SpecialFlags = 0) const { return Cloner(Config, SpecialFlags); }
  character* CloneAndLoad(inputfile&) const;
  int GetIndex() const { return Index; }
  const characterprototype* GetBase() const { return Base; }
  const char* GetClassID() const { return ClassID; }
  int CreateSpecialConfigurations(characterdatabase**, int);
  const characterdatabase* ChooseBaseForConfig(characterdatabase** TempConfig, int, int) { return *TempConfig; }
  const characterdatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  characterprototype* Base;
  characterdatabase** ConfigData;
  characterdatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  character* (*Cloner)(int, int);
  const char* ClassID;
};

class character : public entity, public id
{
 public:
  friend class databasecreator<character>;
  friend class corpse;
  typedef characterprototype prototype;
  typedef characterdatabase database;
  character(donothing);
  character(const character&);
  virtual ~character();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool CanWield() const { return false; }
  virtual bool Catches(item*) { return false; }
  bool CheckDeath(const festring&, const character* = 0, bool = false, bool = false, bool = true, bool = true);
  bool DodgesFlyingItem(item*, double);
  virtual bool Hit(character*, vector2d, int, bool = false) = 0;
  bool OpenPos(vector2d);
  bool ReadItem(item*);
  bool TestForPickup(item*) const;
  void ThrowItem(int, item*);
  bool TryMove(vector2d, bool, bool);
  bool HasHeadOfElpuri() const;
  bool HasGoldenEagleShirt() const;
  bool HasPetrussNut() const;
  bool RemoveEncryptedScroll();
  bool IsPlayer() const { return Player; }
  bool Engrave(const festring&);
  void AddScoreEntry(const festring&, double = 1, bool = true) const;
  long GetAP() const { return AP; }
  long GetNP() const { return NP; }
  stack* GetStack() const { return Stack; }
  int GetBurdenState() const { return BurdenState; }
  bool MakesBurdened(long What) const { return long(GetCarryingStrength()) * 2500 < What; }
  virtual int TakeHit(character*, item*, bodypart*, vector2d, double, double, int, int, int, bool, bool);
  int GetLOSRange() const;
  int GetLOSRangeSquare() const { return GetLOSRange() * GetLOSRange(); }
  int GetESPRange() const { return GetAttribute(INTELLIGENCE) / 3; }
  int GetESPRangeSquare() const { return GetESPRange() * GetESPRange(); }
  int GetTeleportRange() const { return GetAttribute(INTELLIGENCE); }
  int GetTeleportRangeSquare() const { return GetTeleportRange() * GetTeleportRange(); }
  void AddMissMessage(const character*) const;
  void AddPrimitiveHitMessage(const character*, const festring&, const festring&, int) const;
  void AddWeaponHitMessage(const character*, const item*, int, bool = false) const;
  //virtual void ApplyExperience(bool = false);
  virtual void BeTalkedTo();
  void ReceiveDarkness(long);
  void Die(const character* = 0, const festring& = CONST_S(""), bool = false, bool = true, bool = true);
  void HasBeenHitByItem(character*, item*, int, double, int);
  void Hunger();
  void Move(vector2d, bool, bool = false);
  virtual bool MoveRandomly();
  void ReceiveNutrition(long);
  void ReceiveOmmelUrine(long);
  void ReceivePepsi(long);
  void ReceiveSchoolFood(long);
  void Regenerate();
  void SetAP(long What) { AP = What; }
  void SetIsPlayer(bool What) { Player = What; }
  void SetNP(long);
  void Vomit(vector2d, int, bool = true);
  virtual void Be();
  bool Polymorph(character*, int);
  void BeKicked(character*, item*, bodypart*, vector2d, double, double, int, int, bool, bool);
  void FallTo(character*, vector2d);
  bool CheckCannibalism(const material*) const;
  void ActivateTemporaryState(long What) { TemporaryState |= What; }
  void DeActivateTemporaryState(long What) { TemporaryState &= ~What; }
  void ActivateEquipmentState(long What) { EquipmentState |= What; }
  void DeActivateEquipmentState(long What) { EquipmentState &= ~What; }
  bool TemporaryStateIsActivated(long What) const { return !!(TemporaryState & What); }	
  bool EquipmentStateIsActivated(long What) const { return !!(EquipmentState & What); }
  bool StateIsActivated(long What) const { return TemporaryState & What || EquipmentState & What; }
  virtual bool Faint(int, bool = false);
  void SetTemporaryStateCounter(long, int);
  void DeActivateVoluntaryAction(const festring& = CONST_S(""));
  void ActionAutoTermination();
  team* GetTeam() const { return Team; }
  void SetTeam(team*);
  void ChangeTeam(team*);
  virtual int GetMoveEase() const;
  double GetDodgeValue() const { return DodgeValue; }
  long GetMoney() const { return Money; }
  void SetMoney(long What) { Money = What; }
  void EditMoney(long What) { Money += What; }
  bool Displace(character*, bool = false);
  bool CheckStarvationDeath(const festring&);
  void ShowNewPosInfo() const;
  void Hostility(character*);
  stack* GetGiftStack() const;
  bool MoveRandomlyInRoom();
  std::list<character*>::iterator GetTeamIterator();
  void SetTeamIterator(std::list<character*>::iterator);
  void ReceiveKoboldFlesh(long);
  bool ChangeRandomAttribute(int);
  int RandomizeReply(long&, int);
  virtual void CreateInitialEquipment(int);
  void DisplayInfo(festring&);
  virtual bool SpecialEnemySightedReaction(character*) { return false; }
  void TestWalkability();
  void EditAP(long);
  void EditNP(long What) { NP += What; }
  void SetSize(int);
  virtual int GetSize() const;
  torso* GetTorso() const { return static_cast<torso*>(*BodyPartSlot[TORSO_INDEX]); }
  humanoidtorso* GetHumanoidTorso() const { return static_cast<humanoidtorso*>(*BodyPartSlot[TORSO_INDEX]); }
  void SetTorso(torso* What) { SetBodyPart(TORSO_INDEX, What); }
  bodypart* GetBodyPart(int I) const { return static_cast<bodypart*>(*BodyPartSlot[I]); }
  void SetBodyPart(int, bodypart*);
  void SetMainMaterial(material*, int = 0);
  void ChangeMainMaterial(material*, int = 0);
  void SetSecondaryMaterial(material*, int = 0);
  void ChangeSecondaryMaterial(material*, int = 0);
  void RestoreHP();
  void RestoreLivingHP();
  void RestoreStamina() { Stamina = MaxStamina; }
  virtual bool ReceiveDamage(character*, int, int, int = ALL, int = 8, bool = false, bool = false, bool = false, bool = true);
  virtual int ReceiveBodyPartDamage(character*, int, int, int, int = 8, bool = false, bool = false, bool = true, bool = false);
  virtual bool BodyPartIsVital(int) const { return true; }
  void RestoreBodyParts();
  const festring& GetAssignedName() const { return AssignedName; }
  void SetAssignedName(const festring& What) { AssignedName = What; }
  festring GetDescription(int) const;
  festring GetPersonalPronoun(bool = true) const;
  festring GetPossessivePronoun(bool = true) const;
  festring GetObjectPronoun(bool = true) const;
  virtual bool BodyPartCanBeSevered(int) const;
  virtual void AddName(festring&, int) const;
  void ReceiveHeal(long);
  virtual item* GetMainWielded() const { return 0; }
  virtual item* GetSecondaryWielded() const { return 0; }
  virtual void SetMainWielded(item*) { }
  virtual void SetSecondaryWielded(item*) { }
  int GetHungerState() const;
  bool ConsumeItem(item*, const festring&);
  virtual bool CanConsume(material*) const;
  action* GetAction() const { return Action; }
  void SetAction(action* What) { Action = What; }
  virtual void SwitchToDig(item*, vector2d) { }
  virtual void SetRightWielded(item*) { }
  virtual void SetLeftWielded(item*) { }
  void GoOn(go*, bool = false);
  virtual bool CheckKick() const;
  virtual int OpenMultiplier() const { return 2; }
  virtual int CloseMultiplier() const { return 2; }
  virtual bool CheckThrow() const;
  virtual bool CheckOffer() const { return true; }
  int GetTemporaryStateCounter(long) const;
  void EditTemporaryStateCounter(long, int);
  static bool AllowDamageTypeBloodSpill(int);
  bool ClosePos(vector2d);
  int GetResistance(int) const;
  virtual int GetGlobalResistance(int Type) const { return GetResistance(Type); }
  virtual const char* GetEquipmentName(int) const;
  virtual bodypart* GetBodyPartOfEquipment(int) const { return 0; }
  virtual item* GetEquipment(int) const { return 0; }
  virtual int GetEquipmentSlots() const { return 0; }
  virtual sorter EquipmentSorter(int) const { return 0; }
  virtual void SetEquipment(int, item*) { }
  void AddHealingLiquidConsumeEndMessage() const;
  void AddSchoolFoodConsumeEndMessage() const;
  void AddSchoolFoodHitMessage() const;
  void AddOmmelConsumeEndMessage() const;
  void AddPepsiConsumeEndMessage() const;
  void AddFrogFleshConsumeEndMessage() const;
  void AddKoboldFleshConsumeEndMessage() const;
  void AddKoboldFleshHitMessage() const;
  void AddBoneConsumeEndMessage() const;
  void PrintInfo() const;
  virtual item* SevereBodyPart(int, bool = false, stack* = 0);
  virtual bool TryToRiseFromTheDead();
  virtual bool RaiseTheDead(character*);
  bodypart* CreateBodyPart(int, int = 0);
  virtual bool CanUseEquipment(int) const;
  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  void SetParameters(int) { }
  double GetNaturalExperience(int Identifier) const { return DataBase->NaturalExperience[Identifier]; }
  DATA_BASE_VALUE(int, Config);
  DATA_BASE_VALUE(int, DefaultEndurance);
  DATA_BASE_VALUE(int, DefaultPerception);
  DATA_BASE_VALUE(int, DefaultIntelligence);
  DATA_BASE_VALUE(int, DefaultWisdom);
  DATA_BASE_VALUE(int, DefaultCharisma);
  DATA_BASE_VALUE(int, DefaultMana);
  DATA_BASE_VALUE(int, DefaultArmStrength);
  DATA_BASE_VALUE(int, DefaultLegStrength);
  DATA_BASE_VALUE(int, DefaultDexterity);
  DATA_BASE_VALUE(int, DefaultAgility);
  DATA_BASE_VALUE(long, DefaultMoney);
  DATA_BASE_VALUE(int, TotalSize);
  DATA_BASE_BOOL(CanRead);
  DATA_BASE_BOOL(IsCharmable);
  DATA_BASE_VALUE(int, Sex);
  DATA_BASE_BOOL(CanBeGenerated);
  DATA_BASE_VALUE(int, CriticalModifier);
  DATA_BASE_BOOL(CanOpen);
  DATA_BASE_BOOL(CanBeDisplaced);
  DATA_BASE_VALUE(int, EnergyResistance);
  DATA_BASE_VALUE(int, FireResistance);
  DATA_BASE_VALUE(int, PoisonResistance);
  DATA_BASE_VALUE(int, ElectricityResistance);
  DATA_BASE_VALUE(int, AcidResistance);
  DATA_BASE_VALUE(int, ConsumeFlags);
  DATA_BASE_VALUE(long, TotalVolume);
  virtual DATA_BASE_VALUE(vector2d, HeadBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, TorsoBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, ArmBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, LegBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, RightArmBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, LeftArmBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, RightLegBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, LeftLegBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, GroinBitmapPos);
  virtual DATA_BASE_VALUE(color16, ClothColor);
  virtual DATA_BASE_VALUE(color16, SkinColor);
  virtual DATA_BASE_VALUE(color16, CapColor);
  virtual DATA_BASE_VALUE(color16, HairColor);
  virtual DATA_BASE_VALUE(color16, EyeColor);
  virtual DATA_BASE_VALUE(color16, TorsoMainColor);
  virtual DATA_BASE_VALUE(color16, BeltColor);
  virtual DATA_BASE_VALUE(color16, BootColor);
  virtual DATA_BASE_VALUE(color16, TorsoSpecialColor);
  virtual DATA_BASE_VALUE(color16, ArmMainColor);
  virtual DATA_BASE_VALUE(color16, GauntletColor);
  virtual DATA_BASE_VALUE(color16, ArmSpecialColor);
  virtual DATA_BASE_VALUE(color16, LegMainColor);
  virtual DATA_BASE_VALUE(color16, LegSpecialColor);
  virtual DATA_BASE_BOOL(IsNameable);
  virtual DATA_BASE_VALUE(color24, BaseEmitation); // devirtualize ASAP
  DATA_BASE_VALUE(const festring&, Article);
  DATA_BASE_VALUE(const festring&, Adjective);
  DATA_BASE_VALUE(const festring&, AdjectiveArticle);
  DATA_BASE_VALUE(const festring&, NameSingular);
  DATA_BASE_VALUE(const festring&, NamePlural);
  DATA_BASE_VALUE(const festring&, PostFix);
  DATA_BASE_VALUE(int, ArticleMode);
  DATA_BASE_BOOL(CanZap);
  virtual DATA_BASE_BOOL(IsPolymorphable);
  DATA_BASE_VALUE(int, BaseUnarmedStrength);
  DATA_BASE_VALUE(int, BaseBiteStrength);
  DATA_BASE_VALUE(int, BaseKickStrength);
  DATA_BASE_VALUE(int, AttackStyle);
  DATA_BASE_BOOL(CanUseEquipment);
  DATA_BASE_BOOL(CanKick);
  DATA_BASE_BOOL(CanTalk);
  DATA_BASE_BOOL(CanBeWished);
  DATA_BASE_VALUE(long, ClassStates);
  DATA_BASE_VALUE(const fearray<festring>&, Alias);
  DATA_BASE_BOOL(CreateGolemMaterialConfigurations);
  //DATA_BASE_VALUE(int, AttributeBonus);
  DATA_BASE_VALUE(const fearray<long>&, KnownCWeaponSkills);
  DATA_BASE_VALUE(const fearray<long>&, CWeaponSkillHits);
  DATA_BASE_VALUE(int, RightSWeaponSkillHits);
  DATA_BASE_VALUE(int, LeftSWeaponSkillHits);
  DATA_BASE_VALUE(int, PanicLevel);
  DATA_BASE_BOOL(CanBeCloned);
  DATA_BASE_VALUE(const festring&, DefaultName);
  DATA_BASE_VALUE(const fearray<festring>&, FriendlyReplies);
  DATA_BASE_VALUE(const fearray<festring>&, HostileReplies);
  DATA_BASE_VALUE(int, FleshMaterial);
  virtual DATA_BASE_BOOL(HasFeet);
  virtual DATA_BASE_VALUE(const festring&, DeathMessage);
  DATA_BASE_VALUE(int, HPRequirementForGeneration);
  DATA_BASE_BOOL(IsExtraCoward);
  DATA_BASE_BOOL(SpillsBlood);
  DATA_BASE_BOOL(HasEyes);
  virtual DATA_BASE_BOOL(HasHead);
  DATA_BASE_BOOL(CanThrow);
  DATA_BASE_BOOL(UsesNutrition);
  DATA_BASE_VALUE(int, AttackWisdomLimit);
  DATA_BASE_BOOL(IsUnique);
  DATA_BASE_VALUE(int, AttachedGod);
  DATA_BASE_BOOL(BodyPartsDisappearWhenSevered);
  DATA_BASE_VALUE(int, Frequency);
  DATA_BASE_BOOL(CanBeConfused);
  DATA_BASE_BOOL(CanApply);
  DATA_BASE_VALUE(vector2d, WieldedPosition);
  DATA_BASE_VALUE(int, SparklingFlags);
  DATA_BASE_BOOL(IgnoreDanger);
  DATA_BASE_BOOL(BiteCapturesBodyPart);
  DATA_BASE_BOOL(IsPlant);
  DATA_BASE_BOOL(DestroysWalls);
  DATA_BASE_BOOL(IsRooted);
  DATA_BASE_VALUE(int, BloodMaterial);
  DATA_BASE_VALUE(int, VomitMaterial);
  DATA_BASE_BOOL(AutomaticallySeen);
  DATA_BASE_VALUE(ulong, DefaultCommandFlags);
  DATA_BASE_BOOL(WillCarryItems);
  DATA_BASE_VALUE(int, SweatMaterial);
  DATA_BASE_BOOL(Sweats);
  DATA_BASE_BOOL(IsImmuneToItemTeleport);
  DATA_BASE_BOOL(AlwaysUseMaterialAttributes);
  DATA_BASE_BOOL(IsEnormous);
  DATA_BASE_VALUE(const fearray<festring>&, ScienceTalkAttribute);
  DATA_BASE_VALUE(const fearray<festring>&, ScienceTalkPrefix);
  DATA_BASE_VALUE(const fearray<festring>&, ScienceTalkName);
  DATA_BASE_VALUE(int, ScienceTalkPossibility);
  DATA_BASE_VALUE(int, ScienceTalkIntelligenceModifier);
  DATA_BASE_VALUE(int, ScienceTalkWisdomModifier);
  DATA_BASE_VALUE(int, ScienceTalkIntelligenceRequirement);
  DATA_BASE_VALUE(int, ScienceTalkWisdomRequirement);
  int GetType() const { return GetProtoType()->GetIndex(); }
  void TeleportRandomly(bool = false);
  bool TeleportNear(character*);
  bool IsStuck() const;
  virtual void InitSpecialAttributes() { }
  virtual void Kick(lsquare*, int, bool = false) = 0;
  virtual int GetAttribute(int) const;
  virtual bool EditAttribute(int, int);
  virtual void EditExperience(int, double, double);
  /*bool CheckForAttributeIncrease(int&, long&, bool = false);
  bool CheckForAttributeDecrease(int&, long&, bool = false);*/
  bool RawEditAttribute(double&, int) const;
  void DrawPanel(bool) const;
  virtual int DrawStats(bool) const = 0;
  virtual int GetCarryingStrength() const = 0;
  static bool DamageTypeAffectsInventory(int);
  void SetStuckTo(item* What) { StuckTo = What; }
  item* GetStuckTo() const { return StuckTo; }
  void SetStuckToBodyPart(int What) { StuckToBodyPart = What; }
  int GetStuckToBodyPart() const { return StuckToBodyPart; }
  bool TryToUnstuck(vector2d);
  virtual int GetRandomStepperBodyPart() const;
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  virtual int CheckForBlock(character*, item*, double, int Damage, int, int) { return Damage; }
  int CheckForBlockWithArm(character*, item*, arm*, double, int, int, int);
  void AddBlockMessage(const character*, const item*, const festring&, bool) const;
  character* GetPolymorphBackup() const { return PolymorphBackup; }
  void SetPolymorphBackup(character* What) { PolymorphBackup = What; }
  cweaponskill* GetCWeaponSkill(int I) const { return &CWeaponSkill[I]; }
  virtual bool AddSpecialSkillInfo(felist&) const { return false; }
  virtual bool CheckBalance(double);
  long GetStateAPGain(long) const;
  virtual long GetMoveAPRequirement(int) const;
  virtual void SignalEquipmentAdd(int);
  virtual void SignalEquipmentRemoval(int);
  void BeginTemporaryState(long, int);
  void GainIntrinsic(long);
  void HandleStates();
  void PrintBeginPolymorphControlMessage() const;
  void PrintEndPolymorphControlMessage() const;
  void PrintBeginLifeSaveMessage() const;
  void PrintEndLifeSaveMessage() const;
  void PrintBeginLycanthropyMessage() const;
  void PrintEndLycanthropyMessage() const;
  void PrintBeginHasteMessage() const;
  void PrintEndHasteMessage() const;
  void PrintBeginSlowMessage() const;
  void PrintEndSlowMessage() const;
  void PrintBeginSearchingMessage() const;
  void PrintEndSearchingMessage() const;
  void EndPolymorph();
  character* ForceEndPolymorph();
  void LycanthropyHandler();
  void SearchingHandler();
  void SaveLife();
  void BeginInvisibility();
  void BeginInfraVision();
  void BeginESP();
  void EndInvisibility();
  void EndInfraVision();
  void EndESP();
  character* PolymorphRandomly(int, int, int);
  virtual bool EquipmentEasilyRecognized(int) const { return true; }
  void StartReading(item*, long);
  void DexterityAction(int);
  void IntelligenceAction(int);
  virtual void SWeaponSkillTick() { }
  void PrintBeginInvisibilityMessage() const;
  void PrintEndInvisibilityMessage() const;
  void PrintBeginInfraVisionMessage() const;
  void PrintEndInfraVisionMessage() const;
  void PrintBeginESPMessage() const;
  void PrintEndESPMessage() const;
  bool CanBeSeenByPlayer(bool = false, bool = false) const;
  bool CanBeSeenBy(const character*, bool = false, bool = false) const;
  void AttachBodyPart(bodypart*);
  bool HasAllBodyParts() const;
  bodypart* FindRandomOwnBodyPart(bool = false) const;
  bodypart* GenerateRandomBodyPart();
  void PrintBeginPoisonedMessage() const;
  void PrintEndPoisonedMessage() const;
  bool IsWarm() const;
  void CalculateEquipmentState();
  void Draw(bitmap*, vector2d, color24, int, bool) const;
  virtual void DrawBodyParts(bitmap*, vector2d, color24, int, bool, bool = true) const;
  god* GetMasterGod() const;
  void PoisonedHandler();
  void PrintBeginTeleportMessage() const;
  void PrintEndTeleportMessage() const;
  void TeleportHandler();
  void PrintEndTeleportControlMessage() const;
  void PrintBeginTeleportControlMessage() const;
  void PolymorphHandler();
  void PrintEndPolymorphMessage() const;
  void PrintBeginPolymorphMessage() const;
  void DisplayStethoscopeInfo(character*) const;
  virtual bool CanUseStethoscope(bool) const;
  virtual bool IsUsingArms() const;
  virtual bool IsUsingLegs() const;
  virtual bool IsUsingHead() const;
  dungeon* GetDungeon() const { return static_cast<level*>(GetSquareUnder()->GetArea())->GetDungeon(); }
  level* GetLevel() const { return static_cast<level*>(GetSquareUnder()->GetArea()); }
  area* GetArea() const { return GetSquareUnder()->GetArea(); }
  virtual square* GetNeighbourSquare(int) const;
  virtual lsquare* GetNeighbourLSquare(int) const;
  virtual wsquare* GetNeighbourWSquare(int) const;
  stack* GetStackUnder(int I = 0) const { return static_cast<lsquare*>(GetSquareUnder(I))->GetStack(); }
  square* GetNearSquare(vector2d Pos) const { return GetSquareUnder()->GetArea()->GetSquare(Pos); }
  square* GetNearSquare(int x, int y) const { return GetSquareUnder()->GetArea()->GetSquare(x, y); }
  lsquare* GetNearLSquare(vector2d Pos) const { return static_cast<lsquare*>(GetSquareUnder()->GetArea()->GetSquare(Pos)); }
  lsquare* GetNearLSquare(int x, int y) const { return static_cast<lsquare*>(GetSquareUnder()->GetArea()->GetSquare(x, y)); }
  wsquare* GetNearWSquare(vector2d) const;
  wsquare* GetNearWSquare(int, int) const;
  vector2d GetPos(int I = 0) const { return GetSquareUnder(I)->GetPos(); }
  square* GetSquareUnder(int I = 0) const { return !MotherEntity ? SquareUnder[I] : MotherEntity->GetSquareUnderEntity(I); }
  virtual square* GetSquareUnderEntity(int I = 0) const { return GetSquareUnder(I); }
  lsquare* GetLSquareUnder(int I = 0) const { return static_cast<lsquare*>(GetSquareUnder(I)); }
  int GetRandomNonVitalBodyPart();
  void TeleportSomePartsAway(int);
  virtual void SignalVolumeAndWeightChange();
  virtual void SignalBodyPartVolumeAndWeightChange() { }
  void CalculateVolumeAndWeight();
  long GetVolume() const { return Volume; }
  long GetBodyVolume() const { return BodyVolume; }
  long GetWeight() const { return Weight; }
  long GetCarriedWeight() const { return CarriedWeight; }
  virtual void SignalEmitationIncrease(color24);
  virtual void SignalEmitationDecrease(color24);
  void CalculateEmitation();
  void CalculateAll();
  void CalculateHP();
  void CalculateMaxHP();
  int GetHP() const { return HP; }
  int GetMaxHP() const { return MaxHP; }
  void CalculateBodyPartMaxHPs(bool = true);
  bool IsInitializing() const { return Initializing; }
  bool IsInNoMsgMode() const { return InNoMsgMode; }
  bool ActivateRandomState(int, int, long = 0);
  long GetRandomState(int) const;
  bool GainRandomIntrinsic(int);
  virtual void CalculateBattleInfo() = 0;
  void CalculateBurdenState();
  virtual void CalculateDodgeValue();
  virtual void CalculateBodyParts() { BodyParts = 1; }
  virtual void CalculateAllowedWeaponSkillCategories();
  int GetBodyParts() const { return BodyParts; }
  int GetAllowedWeaponSkillCategories() const { return AllowedWeaponSkillCategories; }
  double GetRelativeDanger(const character*, bool = false) const;
  double GetTimeToDie(const character*, int, double, bool, bool) const;
  virtual double GetTimeToKill(const character*, bool) const = 0;
  virtual void AddSpecialEquipmentInfo(festring&, int) const { }
  virtual festring GetBodyPartName(int, bool = false) const;
  item* SearchForItem(ulong) const;
  bool SearchForItem(const item*) const;
  item* SearchForItem(const sweaponskill*) const;
  bool ContentsCanBeSeenBy(const character*) const;
  festring GetBeVerb() const;
  virtual void CreateBlockPossibilityVector(blockvector&, double) const { }
  virtual bool SpecialUnarmedEffect(character*, vector2d, int, int, bool) { return false; }
  virtual bool SpecialKickEffect(character*, vector2d, int, int, bool) { return false; }
  virtual bool SpecialBiteEffect(character*, vector2d, int, int, bool) { return false; }
  bool HitEffect(character*, item*, vector2d, int, int, int, bool);
  void WeaponSkillHit(item*, int, int);
  character* Duplicate(ulong = 0);
  room* GetRoom(int I = 0) const { return GetLSquareUnder(I)->GetRoom(); }
  bool TryToEquip(item*);
  bool TryToConsume(item*);
  void UpdateESPLOS() const;
  int GetCWeaponSkillLevel(const item*) const;
  virtual int GetSWeaponSkillLevel(const item*) const { return 0; }
  void PrintBeginPanicMessage() const;
  void PrintEndPanicMessage() const;
  void CheckPanic(int);
  character* DuplicateToNearestSquare(character*, ulong = 0);
  void SignalSpoil();
  void SignalSpoilLevelChange();
  virtual bool UseMaterialAttributes() const = 0;
  bool IsPolymorphed() const { return Polymorphed; }
  void SetPolymorphed(bool What) { Polymorphed = What; }
  bool IsInBadCondition() const { return HP * 3 < MaxHP; }
  bool IsInBadCondition(int HP) const { return HP * 3 < MaxHP; }
  int GetCondition() const;
  void UpdatePictures();
  bool CanHeal() const;
  void SetGoingTo(vector2d);
  int GetRelation(const character*) const;
  void CalculateAttributeBonuses();
  void ApplyEquipmentAttributeBonuses(item*);
  void ReceiveAntidote(long);
  void AddAntidoteConsumeEndMessage() const;
  bool IsDead() const;
  void AddOriginalBodyPartID(int, ulong);
  void AddToInventory(const fearray<contentscript<item> >&, int);
  bool HasHadBodyPart(const item*) const;
  void ProcessAndAddMessage(festring) const;
  virtual bool CheckZap();
  void SetEndurance(int);
  void SetPerception(int);
  void SetIntelligence(int);
  void SetWisdom(int);
  void SetCharisma(int);
  void SetMana(int);
  void DamageAllItems(character*, int, int);
  bool Equips(const item*) const;
  void PrintBeginConfuseMessage() const;
  void PrintEndConfuseMessage() const;
  vector2d ApplyStateModification(vector2d) const;
  void AddConfuseHitMessage() const;
  item* SelectFromPossessions(const festring&, sorter = 0);
  void SelectFromPossessions(itemvector&, const festring&, int, sorter = 0);
  bool EquipsSomething(sorter = 0);
  bool CheckTalk();
  virtual bool CanCreateBodyPart(int) const { return true; }
  virtual bool HandleCharacterBlockingTheWay(character*, vector2d, int) { return false; }
  virtual festring& ProcessMessage(festring&) const;
  virtual bool IsHumanoid() const { return false; }
  virtual bool IsHuman() const { return false; }
  bool IsOnGround() const;
  virtual bool CheckIfEquipmentIsNotUsable(int) const { return false; }
  virtual bool MoveTowardsHomePos();
  virtual void SetWayPoints(const fearray<packedvector2d>&) { }
  bool TryToChangeEquipment(stack*, stack*, int);
  void PrintBeginParasitizedMessage() const;
  void PrintEndParasitizedMessage() const;
  void ParasitizedHandler();
  bool CanFollow() const;
  bool LeftOversAreUnique() const;
  virtual festring GetKillName() const;
  festring GetPanelName() const;
  virtual void AddSpecialStethoscopeInfo(felist&) const = 0;
  virtual item* GetPairEquipment(int) const { return 0; }
  bodypart* HealHitPoint();
  void CreateHomeData();
  room* GetHomeRoom() const;
  bool HomeDataIsValid() const;
  void SetHomePos(vector2d);
  void RemoveHomeData();
  ulong GetID() const { return ID; }
  void AddESPConsumeMessage() const;
  const std::list<ulong>& GetOriginalBodyPartID(int) const;
  void GetHitByExplosion(const explosion*, int);
  bool CanBePoisoned() const { return TorsoIsAlive(); }
  bool CanBeParasitized() const { return TorsoIsAlive(); }
  void SortAllItems(itemvector&, const character* = 0, sorter = 0);
  character* GetRandomNeighbourEnemy() const;
  void Search(int);
  character* GetRandomNeighbour(int = (HOSTILE|UNCARING|FRIEND)) const;
  virtual bool IsRetreating() const;
  virtual bool IsMushroom() const { return false; }
  void ResetStates();
  virtual head* Behead() { return 0; }
  void PrintBeginGasImmunityMessage() const;
  void PrintEndGasImmunityMessage() const;
  void ShowAdventureInfo() const;
  virtual bool BoundToUse(const item*, int) const { return false; }
  virtual bool IsBananaGrower() const { return false; }
  virtual int GetRandomApplyBodyPart() const;
#ifdef WIZARD
  virtual void AddAttributeInfo(festring&) const;
  virtual void AddAttackInfo(felist&) const = 0;
  virtual void AddDefenceInfo(felist&) const;
  virtual void DetachBodyPart();
#endif
  void ReceiveHolyBanana(long);
  void AddHolyBananaConsumeEndMessage() const;
  virtual bool PreProcessForBone();
  bool PostProcessForBone(double&, int&);
  bool PostProcessForBone();
  bool HasRepairableBodyParts() const;
  virtual void FinalProcessForBone();
  virtual bool EditAllAttributes(int);
  virtual void SetSoulID(ulong);
  virtual bool SuckSoul(character*) { return false; }
  virtual bool MustBeRemovedFromBone() const;
  bool TorsoIsAlive() const { return GetTorso()->IsAlive(); }
  bool PictureUpdatesAreForbidden() const { return PictureUpdatesForbidden; }
  virtual int GetArms() const { return 0; }
  bool IsPet() const;
  virtual void PutTo(vector2d);
  void PutTo(lsquare*);
  void PutNear(vector2d);
  void PutToOrNear(vector2d);
  virtual void Remove();
  bool IsSmall() const { return SquaresUnder == 1; }
  bool IsOver(vector2d) const;
  bool IsOver(const item*) const;
  bool SquareUnderCanBeSeenByPlayer(bool) const;
  bool SquareUnderCanBeSeenBy(const character*, bool) const;
  int GetDistanceSquareFrom(const character*) const;
  virtual bool CanTheoreticallyMoveOn(const lsquare*) const;
  virtual bool CanMoveOn(const lsquare*) const;
  virtual bool CanMoveOn(const square*) const;
  bool CanMoveOn(const olterrain*) const;
  bool CanMoveOn(const oterrain*) const;
  bool IsMainPos(vector2d What) const { return GetPos() == What; }
  virtual void CalculateSquaresUnder() { SquaresUnder = 1; }
  int GetSquaresUnder() const { return SquaresUnder; }
  virtual int GetSquareIndex(vector2d) const { return 0; }
  virtual int GetNeighbourSquares() const { return 8; }
  virtual int GetExtendedNeighbourSquares() const { return 9; }
  virtual int CalculateNewSquaresUnder(lsquare**, vector2d) const;
  virtual bool IsFreeForMe(square*) const;
  void SendNewDrawRequest() const;
  square* GetNaturalNeighbourSquare(int I) const { return character::GetNeighbourSquare(I); }
  lsquare* GetNaturalNeighbourLSquare(int I) const { return character::GetNeighbourLSquare(I); }
  void SignalStepFrom(lsquare**);
  virtual void SpecialBodyDefenceEffect(character*, bodypart*, int) { }
  virtual int GetSumOfAttributes() const;
  bool IsGoingSomeWhere() const { return GoingTo != ERROR_VECTOR; }
  virtual bool CreateRoute();
  void TerminateGoingTo();
  virtual bool IsSpy() const { return false; }
  bool CheckForFood(int);
  bool CheckForFoodInSquare(vector2d);
  virtual bool CheckIfSatiated() { return GetNP() > SATIATED_LEVEL; }
  virtual void SignalNaturalGeneration() { }
  virtual bool IsBunny() const { return false; }
  void SetConfig(int, int = 0);
  bodypartslot* GetBodyPartSlot(int I) { return &BodyPartSlot[I]; }
  virtual bool CheckConsume(const festring&) const;
  virtual int GetTameSymbolSquareIndex() const { return 0; }
  virtual int GetFlySymbolSquareIndex() const { return 0; }
  virtual bool PlaceIsIllegal(vector2d Pos, vector2d Illegal) const { return Pos == Illegal; }
  liquid* CreateBlood(long) const;
  void SpillFluid(character*, liquid*, int = 0);
  virtual void StayOn(liquid*);
  virtual head* GetVirtualHead() const { return 0; }
  bool IsAlly(const character*) const;
  virtual bool CanVomit() const { return TorsoIsAlive(); }
  ulong GetLastAcidMsgMin() const { return LastAcidMsgMin; }
  void SetLastAcidMsgMin(ulong What) { LastAcidMsgMin = What; }
  virtual bool AllowSpoil() const { return false; }
  void Disappear(corpse*, const char*, bool (item::*)() const);
  void ResetSpoiling();
  virtual character* GetLeader() const;
  int GetMoveType() const;
  virtual bool IsSumoWrestler() const { return false; }
  void InitMaterials(const materialscript*, const materialscript*, bool) { }
  item* SearchForItem(const character*, sorter) const;
  virtual character* CreateZombie() const { return 0; }
  virtual festring GetZombieDescription() const;
  virtual bool CanAttack() const { return true; }
  bool DetectMaterial(const material*) const;
  bool CheckIfTooScaredToHit(const character*) const;
  void PrintBeginLevitationMessage() const;
  void PrintEndLevitationMessage() const;
  void EditDealExperience(long);
  int RawEditExperience(double&, double, double, double) const;
  virtual void LeprosyHandler();
  virtual void TryToInfectWithLeprosy(const character*);
  void PrintBeginLeprosyMessage() const;
  void PrintEndLeprosyMessage() const;
  void SignalGeneration();
  void CheckIfSeen();
  void SignalSeen();
  bool HasBeenSeen() const;
  int GetPolymorphIntelligenceRequirement() const;
  void RemoveAllItems();
  int CalculateWeaponSkillHits(const character*) const;
  void DonateEquipmentTo(character*);
  void ReceivePeaSoup(long);
  void AddPeaSoupConsumeEndMessage() const;
  void CalculateMaxStamina();
  void EditStamina(int, bool);
  void RegenerateStamina();
  void BeginPanic();
  void EndPanic();
  int GetTirednessState() const;
  int GetStamina() const { return Stamina; }
  int GetMaxStamina() const { return MaxStamina; }
  void SetGenerationDanger(double What) { GenerationDanger = What; }
  double GetGenerationDanger() const { return GenerationDanger; }
  void ReceiveBlackUnicorn(long);
  void ReceiveGrayUnicorn(long);
  void ReceiveWhiteUnicorn(long);
  void DecreaseStateCounter(long, int);
  bool IsImmuneToLeprosy() const;
  bodypart* SearchForOriginalBodyPart(int) const;
  void SetLifeExpectancy(int, int);
  virtual void DuplicateEquipment(character*, ulong);
  virtual void SignalDisappearance();
  virtual bool HornOfFearWorks() const;
  virtual bool CanHear() const;
  void BeginLeprosy();
  void EndLeprosy();
  void ReceiveOmmelCerumen(long);
  void ReceiveOmmelSweat(long);
  void ReceiveOmmelTears(long);
  void ReceiveOmmelSnot(long);
  bool IsSameAs(const character*) const;
  ulong GetCommandFlags() const;
  void SetCommandFlags(ulong What) { CommandFlags = What; }
  ulong GetPossibleCommandFlags() const;
  ulong GetConstantCommandFlags() const;
  virtual bool AllowEquipment(const item*, int) const { return true; }
  bool ChatMenu();
  bool ChangePetEquipment();
  bool TakePetItems();
  bool GivePetItems();
  bool IssuePetCommands();
  bool ChatIdly();
  bool EquipmentScreen(stack*, stack*);
  ulong GetManagementFlags() const;
  const char* GetVerbalBurdenState() const;
  color16 GetVerbalBurdenStateColor() const;
  virtual int GetAttributeAverage() const;
  virtual const festring& GetStandVerb() const;
  virtual bool CheckApply() const;
  virtual bool CanForceVomit() const { return CanVomit(); }
  void EndLevitation();
  DATA_BASE_VALUE(festring, ForceVomitMessage);
  virtual bool CanMove() const;
  void CalculateEnchantments();
  bool GetNewFormForPolymorphWithControl(character*&);
  liquid* CreateSweat(long) const;
  bool IsTemporary() const;
  bool TeleportRandomItem(bool);
  bool HasClearRouteTo(vector2d) const;
  virtual bool IsTransparent() const;
  void SignalPossibleTransparencyChange();
  int GetCursorData() const;
  void TryToName();
  double GetSituationDanger(const character*) const;
  virtual void ModifySituationDanger(double&) const;
  void LycanthropySituationDangerModifier(double&) const;
  void PoisonedSituationDangerModifier(double&) const;
  void PolymorphingSituationDangerModifier(double&) const;
  void PanicSituationDangerModifier(double&) const;
  void ConfusedSituationDangerModifier(double&) const;
  void ParasitizedSituationDangerModifier(double&) const;
  void LeprosySituationDangerModifier(double&) const;
  bool TryToTalkAboutScience();
  bool IsUsingWeaponOfCategory(int) const;
  virtual bool IsKamikazeDwarf() const { return false; }
  void AddRandomScienceName(festring&) const;
 protected:
  static bool DamageTypeDestroysBodyPart(int);
  virtual void LoadSquaresUnder();
  virtual bodypart* MakeBodyPart(int) const;
  virtual character* RawDuplicate() const = 0;
  virtual void SpecialTurnHandler() { }
  void Initialize(int, int);
  virtual void VirtualConstructor(bool) { }
  void LoadDataBaseStats();
  virtual vector2d GetBodyPartBitmapPos(int, bool = false) const;
  virtual color16 GetBodyPartColorA(int, bool = false) const;
  virtual color16 GetBodyPartColorB(int, bool = false) const;
  virtual color16 GetBodyPartColorC(int, bool = false) const;
  virtual color16 GetBodyPartColorD(int, bool = false) const;
  virtual bool BodyPartColorAIsSparkling(int, bool = false) const;
  virtual bool BodyPartColorBIsSparkling(int, bool = false) const;
  virtual bool BodyPartColorCIsSparkling(int, bool = false) const;
  virtual bool BodyPartColorDIsSparkling(int, bool = false) const;
  virtual long GetBodyPartSize(int, int) const;
  virtual long GetBodyPartVolume(int) const;
  void UpdateBodyPartPicture(int, bool = false);
  int ChooseBodyPartToReceiveHit(double, double);
  virtual void CreateBodyParts(int);
  virtual material* CreateBodyPartMaterial(int, long) const;
  virtual bool ShowClassDescription() const { return true; }
  void SeekLeader(const character*);
  virtual bool CheckForUsefulItemsOnGround(bool = true);
  bool CheckForDoors();
  bool CheckForEnemies(bool, bool, bool, bool = false);
  bool FollowLeader(character*);
  void StandIdleAI();
  virtual void CreateCorpse(lsquare*);
  void GetPlayerCommand();
  virtual void GetAICommand();
  bool MoveTowardsTarget(bool);
  virtual const char* FirstPersonUnarmedHitVerb() const;
  virtual const char* FirstPersonCriticalUnarmedHitVerb() const;
  virtual const char* ThirdPersonUnarmedHitVerb() const;
  virtual const char* ThirdPersonCriticalUnarmedHitVerb() const;
  virtual const char* FirstPersonKickVerb() const;
  virtual const char* FirstPersonCriticalKickVerb() const;
  virtual const char* ThirdPersonKickVerb() const;
  virtual const char* ThirdPersonCriticalKickVerb() const;
  virtual const char* FirstPersonBiteVerb() const;
  virtual const char* FirstPersonCriticalBiteVerb() const;
  virtual const char* ThirdPersonBiteVerb() const;
  virtual const char* ThirdPersonCriticalBiteVerb() const;
  virtual const char* UnarmedHitNoun() const;
  virtual const char* KickNoun() const;
  virtual const char* BiteNoun() const;
  virtual bool AttackIsBlockable(int) const { return true; }
  virtual bool AttackMayDamageArmor() const { return true; }
  virtual int GetSpecialBodyPartFlags(int) const { return ST_NORMAL; }
  virtual int GetBodyPartWobbleData(int) const { return 0; }
  virtual int ModifyBodyPartHitPreference(int, int Modifier) const { return Modifier; }
  virtual int ModifyBodyPartToHitChance(int, int Chance) const { return Chance; }
  virtual bool CanPanicFromSeveredBodyPart() const { return true; }
  virtual void SpecialBodyPartSeverReaction() { }
  bool AttackAdjacentEnemyAI();
  double RandomizeBabyExperience(double);
  static bool IsLimbIndex(int);
  virtual bool AllowExperience() const { return true; }
  stack* Stack;
  long NP, AP;
  bool Player;
  long TemporaryState;
  int TemporaryStateCounter[STATES];
  team* Team;
  vector2d GoingTo;
  long Money;
  std::list<character*>::iterator TeamIterator;
  bodypartslot* BodyPartSlot;
  festring AssignedName;
  action* Action;
  const database* DataBase;
  int StuckToBodyPart;
  item* StuckTo; // Bad naming. Sorry.
  //int BaseAttribute[BASE_ATTRIBUTES];
  double BaseExperience[BASE_ATTRIBUTES];
  std::list<ulong>* OriginalBodyPartID;
  entity* MotherEntity;
  character* PolymorphBackup;
  cweaponskill* CWeaponSkill;
  long EquipmentState;
  square** SquareUnder;
  long Volume;
  long Weight;
  long CarriedWeight;
  long BodyVolume;
  int HP;
  int MaxHP;
  bool Initializing;
  int BurdenState;
  double DodgeValue;
  int AllowedWeaponSkillCategories;
  int BodyParts;
  bool Polymorphed;
  bool InNoMsgMode;
  long RegenerationCounter;
  int AttributeBonus[BASE_ATTRIBUTES];
  int CarryingBonus;
  homedata* HomeData;
  ulong ID;
  bool PictureUpdatesForbidden;
  int SquaresUnder;
  std::vector<vector2d> Route;
  std::set<vector2d> Illegal;
  ulong LastAcidMsgMin;
  int Stamina;
  int MaxStamina;
  int BlocksSinceLastTurn;
  double GenerationDanger;
  ulong CommandFlags;
  bool HasBeenWarned;
  int ScienceTalks;
};

#ifdef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__
#define CHARACTER_PROTOTYPE(name, base, baseproto)\
character* name##_Clone(int Config, int SpecialFlags) { return new name(Config, SpecialFlags); }\
characterprototype name##_ProtoType(baseproto, &name##_Clone, #name);\
name::name(int Config, int SpecialFlags) : base(donothing()) { Initialize(Config, SpecialFlags); }\
name::name(donothing D) : base(D) { }\
const characterprototype* name::GetProtoType() const { return &name##_ProtoType; }\
character* name::RawDuplicate() const { return new name(*this); }
#define ABSTRACT_CHARACTER_PROTOTYPE(name, base, baseproto)\
characterprototype name##_ProtoType(baseproto, 0, #name);\
name::name(donothing D) : base(D) { }\
const characterprototype* name::GetProtoType() const { return &name##_ProtoType; }
#else
#define CHARACTER_PROTOTYPE(name, base, baseproto)
#define ABSTRACT_CHARACTER_PROTOTYPE(name, base, baseproto)
#endif

#define CHARACTER(name, base, data)\
\
name : public base\
{\
 public:\
  name(int = 0, int = 0);\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  data\
 protected:\
  virtual character* RawDuplicate() const;\
}; CHARACTER_PROTOTYPE(name, base, &base##_ProtoType);

#define ABSTRACT_CHARACTER(name, base, data)\
\
name : public base\
{\
 public:\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  data\
}; ABSTRACT_CHARACTER_PROTOTYPE(name, base, &base##_ProtoType);

#endif
