#ifndef __CHARBA_H__
#define __CHARBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>
#include <map>
#include <vector>

#include "wsquare.h"
#include "itemde.h"
#include "script.h"
#include "femath.h"

#define CHAR_PERSONAL_PRONOUN GetPersonalPronoun().c_str()
#define CHAR_POSSESSIVE_PRONOUN GetPossessivePronoun().c_str()
#define CHAR_OBJECT_PRONOUN GetObjectPronoun().c_str()

typedef std::vector<std::pair<float, ushort> > blockvector;

class felist;
class bitmap;
class character;
class stack;
class material;
class outputfile;
class inputfile;
class team;
class torso;
class humanoidtorso;
class bodypart;
class characterslot;
class action;
class go;
class cweaponskill;
class stackslot;
class god;
template <class type> class database;

inline long APBonus(long Attribute) { return Attribute >= 10 ? 90 + Attribute : 50 + Attribute * 5; }

struct characterdatabase
{
  void InitDefaults(ushort) { IsAbstract = false; }
  ushort DefaultArmStrength;
  ushort DefaultLegStrength;
  ushort DefaultDexterity;
  ushort DefaultAgility;
  ushort DefaultEndurance;
  ushort DefaultPerception;
  ushort DefaultIntelligence;
  ushort DefaultWisdom;
  ushort DefaultCharisma;
  ushort DefaultMana;
  ulong DefaultMoney;
  ushort TotalSize;
  bool CanRead;
  bool IsCharmable;
  uchar Sex;
  ulong BloodColor;
  bool CanBeGenerated;
  uchar CriticalModifier;
  std::string StandVerb;
  bool CanOpen;
  bool CanBeDisplaced;
  ushort Frequency;
  bool CanWalk;
  bool CanSwim;
  bool CanFly;
  ushort PhysicalDamageResistance;
  ushort SoundResistance;
  ushort EnergyResistance;
  ushort AcidResistance;
  ushort FireResistance;
  ushort PoisonResistance;
  ushort BulimiaResistance;
  bool IsUnique;
  ushort ConsumeFlags;
  ulong TotalVolume;
  vector2d HeadBitmapPos;
  vector2d TorsoBitmapPos;
  vector2d ArmBitmapPos;
  vector2d LegBitmapPos;
  vector2d RightArmBitmapPos;
  vector2d LeftArmBitmapPos;
  vector2d RightLegBitmapPos;
  vector2d LeftLegBitmapPos;
  vector2d GroinBitmapPos;
  ushort ClothColor;
  ushort SkinColor;
  ushort CapColor;
  ushort HairColor;
  ushort EyeColor;
  ushort TorsoMainColor;
  ushort BeltColor;
  ushort BootColor;
  ushort TorsoSpecialColor;
  ushort ArmMainColor;
  ushort ArmSpecialColor;
  ushort LegMainColor;
  ushort LegSpecialColor;
  bool IsNameable;
  ulong BaseEmitation;
  std::string Article;
  std::string Adjective;
  std::string AdjectiveArticle;
  std::string NameSingular;
  std::string NamePlural;
  std::string PostFix;
  uchar ArticleMode;
  bool IsAbstract;
  bool IsPolymorphable;
  ulong BaseUnarmedStrength;
  ulong BaseBiteStrength;
  ulong BaseKickStrength;
  uchar AttackStyle;
  bool CanUseEquipment;
  bool CanKick;
  bool CanTalk;
  ushort ClassStates;
  bool CanBeWished;
  std::vector<std::string> Alias;
  bool CreateDivineConfigurations;
  bool CreateGolemMaterialConfigurations;
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
  short AttributeBonus;
  std::vector<long> KnownCWeaponSkills;
  std::vector<long> CWeaponSkillHits;
  ushort RightSWeaponSkillHits;
  ushort LeftSWeaponSkillHits;
  uchar PanicLevel;
  bool CanBeCloned;
  std::vector<contentscript<item> > Inventory;
  ushort DangerModifier;
  std::string DefaultName;
  std::vector<std::string> FriendlyReplies;
  std::vector<std::string> HostileReplies;
  bool CanZap;
  vector2d InHandsPicPos;
  vector2d ShieldPos;
  ushort InHandsPicColorA;
  ushort InHandsPicColorB;
  ushort ShieldColor;
};

class characterprototype
{
 public:
  friend class database<character>;
  characterprototype(characterprototype*, character* (*)(ushort, ushort), const std::string&);
  character* Clone(ushort Config = 0, ushort SpecialFlags = 0) const { return Cloner(Config, SpecialFlags); }
  character* CloneAndLoad(inputfile&) const;
  ushort GetIndex() const { return Index; }
  const characterprototype* GetBase() const { return Base; }
  const std::map<ushort, characterdatabase>& GetConfig() const { return Config; }
  const std::string& GetClassId() const { return ClassId; }
  void CreateSpecialConfigurations();
  bool IsAbstract() const { return Config.begin()->second.IsAbstract; }
  const characterdatabase& ChooseBaseForConfig(ushort) { return Config.begin()->second; }
 protected:
  ushort Index;
  characterprototype* Base;
  std::map<ushort, characterdatabase> Config;
  character* (*Cloner)(ushort, ushort);
  std::string ClassId;
};

class character : public entity, public id
{
 public:
  friend class database<character>;
  friend class corpse;
  typedef characterprototype prototype;
  typedef characterdatabase database;
  typedef std::map<ushort, characterdatabase> databasemap;
  character(donothing);
  character(const character&);
  virtual ~character();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool CanWield() const { return false; }
  virtual bool Catches(item*, float) { return false; }
  virtual bool CheckBulimia() const { return GetHungerState() == BLOATED; }
  virtual bool CheckDeath(const std::string&, bool = false);
  virtual bool DodgesFlyingItem(item*, float);
  virtual bool Hit(character*) = 0;
  virtual bool OpenItem();
  virtual bool OpenPos(vector2d);
  virtual bool ReadItem(item*);
  virtual bool TestForPickup(item*) const;
  virtual bool ThrowItem(uchar, item*);
  virtual bool TryMove(vector2d, bool = true);
  virtual bool HasHeadOfElpuri() const;
  virtual bool HasGoldenEagleShirt() const;
  virtual bool HasPetrussNut() const;
  bool IsPlayer() const { return Player; }
  virtual bool Apply();
  virtual bool Close();
  virtual bool Eat();
  virtual bool Drink();
  virtual bool Dip();
  virtual bool DrawMessageHistory();
  virtual bool Drop();
  virtual bool Engrave(const std::string&);
  virtual bool ForceVomit();
  virtual bool GainAllItems();
  virtual bool GoDown();
  virtual bool GoUp();
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
  virtual bool EquipmentScreen();
  virtual bool WhatToEngrave();
  virtual bool WizardMode();
  virtual void AddScoreEntry(const std::string&, float = 1, bool = true) const;
  virtual long GetScore() const;
  long GetAP() const { return AP; }
  long GetNP() const { return NP; }
  stack* GetStack() const { return Stack; }
  virtual uchar GetBurdenState() const { return BurdenState; }
  virtual bool MakesBurdened(ulong What) const { return ulong(GetCarryingStrength()) * 2500 < What; }
  virtual uchar TakeHit(character*, item*, float, float, short, uchar, bool);
  virtual ushort LOSRange() const;
  virtual ushort LOSRangeSquare() const;
  virtual ushort ESPRange() const;
  virtual ushort ESPRangeSquare() const;
  virtual void AddMissMessage(const character*) const;
  virtual void AddPrimitiveHitMessage(const character*, const std::string&, const std::string&, uchar) const;
  virtual void AddWeaponHitMessage(const character*, const item*, uchar, bool = false) const;
  virtual void ApplyExperience(bool = false);
  virtual void BeTalkedTo();
  virtual void ReceiveDarkness(long);
  virtual void Die(bool = false);
  virtual void HasBeenHitByItem(character*, item*, float);
  virtual void Hunger();
  virtual void Move(vector2d, bool = false);
  virtual bool MoveRandomly();
  virtual void ReceiveNutrition(long);
  virtual void ReceiveOmmelUrine(long);
  virtual void ReceivePepsi(long);
  virtual void ReceiveSchoolFood(long);
  void Regenerate();
  void SetAP(long What) { AP = What; }
  void SetIsPlayer(bool What) { Player = What; }
  void SetNP(long);
  virtual void Vomit(ushort);
  virtual void Be();
  virtual bool Zap();
  virtual bool Polymorph(character*, ushort);
  virtual void BeKicked(character*, item*, float, float, short, bool);
  virtual void FallTo(character*, vector2d);
  virtual bool CheckCannibalism(const material*) const;
  void ActivateTemporaryState(ushort What) { TemporaryState |= What; }
  void DeActivateTemporaryState(ushort What) { TemporaryState &= ~What; }
  void ActivateEquipmentState(ushort What) { EquipmentState |= What; }
  void DeActivateEquipmentState(ushort What) { EquipmentState &= ~What; }
  bool TemporaryStateIsActivated(ushort What) const { return (TemporaryState & What) != 0; }	
  bool EquipmentStateIsActivated(ushort What) const { return (EquipmentState & What) != 0; }
  bool StateIsActivated(ushort What) const { return TemporaryState & What || EquipmentState & What; }
  virtual void Faint();
  void SetTemporaryStateCounter(ushort, ushort);
  virtual void DeActivateVoluntaryAction(const std::string& = "");
  virtual void ActionAutoTermination();
  team* GetTeam() const { return Team; }
  void SetTeam(team*);
  virtual void ChangeTeam(team*);
  virtual bool RestUntilHealed();
  virtual bool OutlineCharacters();
  virtual bool OutlineItems();
  virtual ushort GetMoveEase() const;
  float GetDodgeValue() const { return DodgeValue; }
  virtual bool RaiseGodRelations();
  virtual bool LowerGodRelations();
  virtual bool GainDivineKnowledge();
  virtual ulong GetMoney() const { return Money; }
  virtual void SetMoney(ulong What) { Money = What; }
  virtual void EditMoney(long What) { Money += What; }
  virtual void SetHomeRoom(uchar What) { HomeRoom = What; }
  virtual uchar GetHomeRoom() const { return HomeRoom; }
  virtual bool Displace(character*);
  virtual bool Sit();
  virtual long GetStatScore() const;
  virtual bool CheckStarvationDeath(const std::string&);
  virtual void ShowNewPosInfo() const;
  virtual void Hostility(character*);
  virtual stack* GetGiftStack() const;
  virtual bool MoveRandomlyInRoom();
  virtual bool Go();
  virtual bool ShowConfigScreen();
  virtual std::list<character*>::iterator GetTeamIterator() { return TeamIterator; }
  virtual void SetTeamIterator(std::list<character*>::iterator What) { TeamIterator = What; }
  virtual void ReceiveKoboldFlesh(long);
  virtual bool ChangeRandomStat(short);
  ushort RandomizeReply(std::vector<bool>&);
  virtual void CreateInitialEquipment(ushort SpecialFlags) { AddToInventory(DataBase->Inventory, SpecialFlags); }
  virtual void DisplayInfo(std::string&);
  virtual bool SpecialEnemySightedReaction(character*) { return false; }
  virtual void TestWalkability();
  void EditAP(long What) { AP += What; if(AP > 1200) AP = 1200; }
  void EditNP(long What) { NP += What; }
  virtual void SetSize(ushort);
  virtual ushort GetSize() const;
  torso* GetTorso() const { return static_cast<torso*>(*BodyPartSlot[TORSO_INDEX]); }
  humanoidtorso* GetHumanoidTorso() const { return static_cast<humanoidtorso*>(*BodyPartSlot[TORSO_INDEX]); }
  void SetTorso(torso* What) { SetBodyPart(TORSO_INDEX, What); }
  bodypart* GetBodyPart(ushort Index) const { return static_cast<bodypart*>(*BodyPartSlot[Index]); }
  void SetBodyPart(ushort, bodypart*);
  virtual void SetMainMaterial(material*, ushort = 0);
  virtual void ChangeMainMaterial(material*, ushort = 0);
  virtual void SetSecondaryMaterial(material*, ushort = 0);
  virtual void ChangeSecondaryMaterial(material*, ushort = 0);
  virtual void SetContainedMaterial(material*, ushort = 0);
  virtual void ChangeContainedMaterial(material*, ushort = 0);
  virtual void Teleport(vector2d);
  virtual bool SecretKnowledge();
  virtual void RestoreHP();
  virtual bool ReceiveDamage(character*, ushort, uchar, uchar = ALL, uchar = 8, bool = false, bool = false, bool = false, bool = true);
  virtual ushort ReceiveBodyPartDamage(character*, ushort, uchar, uchar, uchar = 8, bool = false, bool = false, bool = true);
  virtual bool BodyPartVital(ushort) const { return true; }
  virtual void RestoreBodyParts();
  virtual bool AssignName();
  virtual std::string GetAssignedName() const { return AssignedName; }
  virtual void SetAssignedName(const std::string& What) { AssignedName = What; }
  virtual std::string GetDescription(uchar) const;
  virtual std::string GetPersonalPronoun() const;
  virtual std::string GetPossessivePronoun() const;
  virtual std::string GetObjectPronoun() const;
  virtual bool BodyPartCanBeSevered(ushort) const;
  virtual void AddName(std::string&, uchar) const;
  virtual void ReceiveHeal(long);
  virtual item* GetMainWielded() const { return 0; }
  virtual item* GetSecondaryWielded() const { return 0; }
  virtual void SetMainWielded(item*) { }
  virtual void SetSecondaryWielded(item*) { }
  virtual uchar GetHungerState() const;
  virtual bool ConsumeItem(item*);
  virtual bool CanConsume(material*) const;
  action* GetAction() const { return Action; }
  void SetAction(action* What) { Action = What; }
  virtual void SwitchToDig(item*, vector2d) { }
  virtual void SetRightWielded(item*) { }
  virtual void SetLeftWielded(item*) { }
  virtual void GoOn(go*);
  virtual bool CheckKick() const;
  virtual uchar OpenMultiplier() const { return 2; }
  virtual uchar CloseMultiplier() const { return 2; }
  virtual bool CheckThrow() const { return true; }  
  virtual bool CheckOffer() const { return true; }
  ushort GetTemporaryStateCounter(ushort) const;
  void EditTemporaryStateCounter(ushort, short);
  virtual bool AllowDamageTypeBloodSpill(uchar) const;
  virtual bool ClosePos(vector2d);
  virtual ushort GetResistance(uchar) const;
  virtual ushort GlobalResistance(uchar Type) const { return GetResistance(Type); }
  virtual std::string EquipmentName(ushort) const { return ""; }
  virtual bodypart* GetBodyPartOfEquipment(ushort) const { return 0; }
  virtual item* GetEquipment(ushort) const { return 0; }
  virtual ushort GetEquipmentSlots() const { return 0; }
  virtual bool (*EquipmentSorter(ushort) const)(item*, const character*) { return 0; }
  virtual void SetEquipment(ushort, item*) { }
  virtual bool ScrollMessagesDown();
  virtual bool ScrollMessagesUp();
  virtual void AddHealingLiquidConsumeEndMessage() const;
  virtual void AddSchoolFoodConsumeEndMessage() const;
  virtual void AddSchoolFoodHitMessage() const;
  virtual void AddOmmelUrineConsumeEndMessage() const;
  virtual void AddPepsiConsumeEndMessage() const;
  virtual void AddFrogFleshConsumeEndMessage() const;
  virtual void AddKoboldFleshConsumeEndMessage() const;
  virtual void AddKoboldFleshHitMessage() const;
  virtual void AddBoneConsumeEndMessage() const;
  virtual void PrintInfo() const;
  virtual item* SevereBodyPart(ushort);
  virtual bool IsAnimated() const { return false; }
  virtual void CompleteRiseFromTheDead();
  virtual bool RaiseTheDead(character*);
  virtual bodypart* CreateBodyPart(ushort, ushort = 0);
  virtual bool CanUseEquipment(ushort Index) const { return CanUseEquipment() && Index < GetEquipmentSlots() && GetBodyPartOfEquipment(Index); }
  virtual const prototype* GetProtoType() const { return &character_ProtoType; }
  const database* GetDataBase() const { return DataBase; }
  virtual void SetParameters(uchar) { }
  DATA_BASE_VALUE(ushort, DefaultArmStrength);
  DATA_BASE_VALUE(ushort, DefaultLegStrength);
  DATA_BASE_VALUE(ushort, DefaultDexterity);
  DATA_BASE_VALUE(ushort, DefaultAgility);
  DATA_BASE_VALUE(ushort, DefaultEndurance);
  DATA_BASE_VALUE(ushort, DefaultPerception);
  DATA_BASE_VALUE(ushort, DefaultIntelligence);
  DATA_BASE_VALUE(ushort, DefaultWisdom);
  DATA_BASE_VALUE(ushort, DefaultCharisma);
  DATA_BASE_VALUE(ushort, DefaultMana);
  DATA_BASE_VALUE(ulong, DefaultMoney);
  virtual DATA_BASE_VALUE(ushort, TotalSize);
  virtual DATA_BASE_BOOL(CanRead);
  virtual DATA_BASE_BOOL(IsCharmable);
  virtual DATA_BASE_VALUE(uchar, Sex);
  virtual DATA_BASE_VALUE(ulong, BloodColor);
  virtual DATA_BASE_BOOL(CanBeGenerated);
  virtual DATA_BASE_VALUE(uchar, CriticalModifier);
  virtual DATA_BASE_VALUE(const std::string&, StandVerb);
  virtual DATA_BASE_BOOL(CanOpen);
  virtual DATA_BASE_BOOL(CanBeDisplaced);
  virtual DATA_BASE_VALUE(ushort, Frequency);
  virtual DATA_BASE_BOOL(CanWalk);
  virtual DATA_BASE_BOOL(CanSwim);
  virtual DATA_BASE_BOOL(CanFly);
  virtual DATA_BASE_VALUE(ushort, PhysicalDamageResistance);
  virtual DATA_BASE_VALUE(ushort, SoundResistance);
  virtual DATA_BASE_VALUE(ushort, EnergyResistance);
  virtual DATA_BASE_VALUE(ushort, AcidResistance);
  virtual DATA_BASE_VALUE(ushort, FireResistance);
  virtual DATA_BASE_VALUE(ushort, PoisonResistance);
  virtual DATA_BASE_VALUE(ushort, BulimiaResistance);
  virtual DATA_BASE_BOOL(IsUnique);
  virtual DATA_BASE_VALUE(ushort, ConsumeFlags);
  virtual DATA_BASE_VALUE(ulong, TotalVolume);
  virtual DATA_BASE_VALUE(vector2d, HeadBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, TorsoBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, ArmBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, LegBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, RightArmBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, LeftArmBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, RightLegBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, LeftLegBitmapPos);
  virtual DATA_BASE_VALUE(vector2d, GroinBitmapPos);
  virtual DATA_BASE_VALUE(ushort, ClothColor);
  virtual DATA_BASE_VALUE(ushort, SkinColor);
  virtual DATA_BASE_VALUE(ushort, CapColor);
  virtual DATA_BASE_VALUE(ushort, HairColor);
  virtual DATA_BASE_VALUE(ushort, EyeColor);
  virtual DATA_BASE_VALUE(ushort, TorsoMainColor);
  virtual DATA_BASE_VALUE(ushort, BeltColor);
  virtual DATA_BASE_VALUE(ushort, BootColor);
  virtual DATA_BASE_VALUE(ushort, TorsoSpecialColor);
  virtual DATA_BASE_VALUE(ushort, ArmMainColor);
  virtual DATA_BASE_VALUE(ushort, ArmSpecialColor);
  virtual DATA_BASE_VALUE(ushort, LegMainColor);
  virtual DATA_BASE_VALUE(ushort, LegSpecialColor);
  virtual DATA_BASE_BOOL(IsNameable);
  virtual DATA_BASE_VALUE(ulong, BaseEmitation);
  DATA_BASE_VALUE(const std::string&, Article);
  DATA_BASE_VALUE(const std::string&, Adjective);
  DATA_BASE_VALUE(const std::string&, AdjectiveArticle);
  DATA_BASE_VALUE(const std::string&, NameSingular);
  DATA_BASE_VALUE(const std::string&, NamePlural);
  DATA_BASE_VALUE(const std::string&, PostFix);
  DATA_BASE_VALUE(uchar, ArticleMode);
  DATA_BASE_BOOL(CanZap);
  virtual DATA_BASE_BOOL(IsPolymorphable);
  virtual DATA_BASE_VALUE(ulong, BaseUnarmedStrength);
  virtual DATA_BASE_VALUE(ulong, BaseBiteStrength);
  virtual DATA_BASE_VALUE(ulong, BaseKickStrength);
  virtual DATA_BASE_VALUE(uchar, AttackStyle);
  virtual DATA_BASE_BOOL(CanUseEquipment);
  virtual DATA_BASE_BOOL(CanKick);
  virtual DATA_BASE_BOOL(CanTalk);
  virtual DATA_BASE_VALUE(ushort, ClassStates);
  virtual DATA_BASE_BOOL(CanBeWished);
  virtual DATA_BASE_VALUE(const std::vector<std::string>&, Alias);
  virtual DATA_BASE_BOOL(CreateGolemMaterialConfigurations);
  DATA_BASE_VALUE(short, AttributeBonus);
  DATA_BASE_VALUE(const std::vector<long>&, KnownCWeaponSkills);
  DATA_BASE_VALUE(const std::vector<long>&, CWeaponSkillHits);
  DATA_BASE_VALUE(ushort, RightSWeaponSkillHits);
  DATA_BASE_VALUE(ushort, LeftSWeaponSkillHits);
  virtual DATA_BASE_VALUE(uchar, PanicLevel);
  virtual DATA_BASE_BOOL(CanBeCloned);
  DATA_BASE_VALUE(const std::string&, DefaultName);
  DATA_BASE_VALUE(const std::vector<std::string>&, FriendlyReplies);
  DATA_BASE_VALUE(const std::vector<std::string>&, HostileReplies);
  DATA_BASE_VALUE(vector2d, InHandsPicPos);
  DATA_BASE_VALUE(vector2d, ShieldPos);
  DATA_BASE_VALUE(ushort, InHandsPicColorA);
  DATA_BASE_VALUE(ushort, InHandsPicColorB);
  DATA_BASE_VALUE(ushort, ShieldColor);
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual void TeleportRandomly();
  virtual bool TeleportNear(character*);
  static character* Clone(ushort, ushort) { return 0; }
  virtual bool IsStuck() const;
  virtual void InitSpecialAttributes() { }
  virtual void Kick(lsquare*) = 0;
  virtual ushort GetAttribute(ushort Identifier) const { return Max(BaseAttribute[Identifier] + AttributeBonus[Identifier], 1); }
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual bool CheckForAttributeIncrease(ushort&, long&, bool = false);
  virtual bool CheckForAttributeDecrease(ushort&, long&, bool = false);
  virtual bool RawEditAttribute(ushort&, short&, bool = false);
  virtual void DrawPanel(bool) const;
  virtual ushort DrawStats(bool) const = 0;
  virtual ushort GetCarryingStrength() const = 0;
  const std::list<ulong>& GetOriginalBodyPartID(ushort Index) const { return OriginalBodyPartID[Index]; }
  virtual bool DamageTypeAffectsInventory(uchar) const;
  virtual void SetStuckTo(item* What) { StuckTo = What; }
  virtual item* GetStuckTo() const { return StuckTo; }
  virtual void SetStuckToBodyPart(ushort What) { StuckToBodyPart = What; }
  virtual ushort GetStuckToBodyPart() const { return StuckToBodyPart; }
  virtual bool TryToUnstuck(vector2d);
  virtual ushort GetRandomStepperBodyPart() const { return TORSO_INDEX; }
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  virtual ushort CheckForBlock(character*, item*, float, ushort Damage, short, uchar) { return Damage; }
  virtual ushort CheckForBlockWithArm(character*, item*, arm*, float, ushort, short, uchar);
  virtual void AddBlockMessage(const character*, const item*, const std::string&, bool) const;
  virtual character* GetPolymorphBackup() const { return PolymorphBackup; }
  virtual void SetPolymorphBackup(character* What) { PolymorphBackup = What; }
  cweaponskill* GetCWeaponSkill(ushort Index) const { return CWeaponSkill[Index]; }
  virtual bool AddSpecialSkillInfo(felist&) const { return false; }
  virtual bool CheckBalance(float);
  long GetStateAPGain(long) const;
  virtual long GetMoveAPRequirement(uchar Difficulty) const { return 10000000 * Difficulty / (APBonus(GetAttribute(AGILITY)) * GetMoveEase()); }
  virtual bool EquipmentHasNoPairProblems(ushort) const { return true; }
  virtual void SignalEquipmentAdd(ushort);
  virtual void SignalEquipmentRemoval(ushort);
  ushort GetConfig() const { return Config; }
  virtual void BeginTemporaryState(ushort, ushort);
  virtual void GainIntrinsic(ushort What) { BeginTemporaryState(What, 0); }
  virtual void LoseIntrinsic(ushort);
  void HandleStates();
  virtual void PrintBeginPolymorphControlMessage() const;
  virtual void PrintEndPolymorphControlMessage() const;
  virtual void PrintBeginLifeSaveMessage() const;
  virtual void PrintEndLifeSaveMessage() const;
  virtual void PrintBeginLycanthropyMessage() const;
  virtual void PrintEndLycanthropyMessage() const;
  virtual void PrintBeginHasteMessage() const;
  virtual void PrintEndHasteMessage() const;
  virtual void PrintBeginSlowMessage() const;
  virtual void PrintEndSlowMessage() const;
  virtual void EndPolymorph();
  virtual void LycanthropyHandler();
  virtual void SaveLife();
  virtual void BeginInvisibility();
  virtual void BeginInfraVision();
  virtual void BeginESP();
  virtual void EndInvisibility();
  virtual void EndInfraVision();
  virtual void EndESP();
  virtual void PolymorphRandomly(ushort);
  virtual bool EquipmentEasilyRecognized(ushort) const { return true; }
  virtual void StartReading(item*, ulong);
  virtual void DexterityAction(ushort);
  virtual void SWeaponSkillTick() { }
  virtual void PrintBeginInvisibilityMessage() const;
  virtual void PrintEndInvisibilityMessage() const;
  virtual void PrintBeginInfraVisionMessage() const;
  virtual void PrintEndInfraVisionMessage() const;
  virtual void PrintBeginESPMessage() const;
  virtual void PrintEndESPMessage() const;
  bool CanBeSeenByPlayer(bool = false) const;
  bool CanBeSeenBy(const character*, bool = false) const;
  virtual bool DetachBodyPart();
  virtual bodypart* MakeBodyPart(ushort);
  virtual void AttachBodyPart(bodypart*);
  virtual bool HasAllBodyParts() const;
  virtual bodypart* FindRandomOwnBodyPart() const;
  virtual bodypart* GenerateRandomBodyPart();
  virtual void PrintBeginPoisonedMessage() const;
  virtual void PrintEndPoisonedMessage() const;
  virtual bool IsWarm() const;
  virtual void CalculateEquipmentState();
  virtual void Draw(bitmap*, vector2d, ulong, bool, bool) const;
  virtual void DrawBodyParts(bitmap*, vector2d, ulong, bool, bool) const;
  void SetConfig(ushort);
  virtual god* GetMasterGod() const;
  virtual void PoisonedHandler();
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
  virtual bool IsUsingArms() const { return (GetAttackStyle() & USE_ARMS) != 0; }
  virtual bool IsUsingLegs() const { return (GetAttackStyle() & USE_LEGS) != 0; }
  virtual bool IsUsingHead() const { return (GetAttackStyle() & USE_HEAD) != 0; }
  virtual void AddAttackInfo(felist&) const = 0;
  level* GetLevelUnder() const { return static_cast<level*>(SquareUnder->GetAreaUnder()); }
  area* GetAreaUnder() const { return SquareUnder->GetAreaUnder(); }
  square* GetNeighbourSquare(ushort Index) const { return SquareUnder->GetNeighbourSquare(Index); }
  lsquare* GetNeighbourLSquare(ushort Index) const { return static_cast<lsquare*>(SquareUnder)->GetNeighbourLSquare(Index); }
  wsquare* GetNeighbourWSquare(ushort Index) const { return static_cast<wsquare*>(SquareUnder)->GetNeighbourWSquare(Index); }
  stack* GetStackUnder() const { return static_cast<lsquare*>(SquareUnder)->GetStack(); }
  square* GetNearSquare(vector2d Pos) const { return SquareUnder->GetAreaUnder()->GetSquare(Pos); }
  square* GetNearSquare(ushort x, ushort y) const { return SquareUnder->GetAreaUnder()->GetSquare(x, y); }
  lsquare* GetNearLSquare(vector2d Pos) const { return static_cast<lsquare*>(SquareUnder->GetAreaUnder()->GetSquare(Pos)); }
  lsquare* GetNearLSquare(ushort x, ushort y) const { return static_cast<lsquare*>(SquareUnder->GetAreaUnder()->GetSquare(x, y)); }
  vector2d GetPos() const { return SquareUnder->GetPos(); }
  virtual square* GetSquareUnder() const;
  void SetSquareUnder(square* What) { SquareUnder = What; }
  lsquare* GetLSquareUnder() const { return static_cast<lsquare*>(SquareUnder); }
  wsquare* GetWSquareUnder() const { return static_cast<wsquare*>(SquareUnder); }
  uchar GetRandomNonVitalBodyPart();
  void TeleportSomePartsAway(ushort);
  virtual void SignalVolumeAndWeightChange();
  void CalculateVolumeAndWeight();
  ulong GetVolume() const { return Volume; }
  ulong GetBodyVolume() const { return BodyVolume; }
  ulong GetWeight() const { return Weight; }
  ulong GetCarriedWeight() const { return CarriedWeight; }
  virtual void SignalEmitationIncrease(ulong);
  virtual void SignalEmitationDecrease(ulong);
  void CalculateEmitation();
  void CalculateAll();
  void CalculateHP();
  void CalculateMaxHP();
  short GetHP() const { return HP; }
  short GetMaxHP() const { return MaxHP; }
  void CalculateBodyPartMaxHPs();
  bool IsInitializing() const { return Initializing; }
  bool IsInNoMsgMode() const { return InNoMsgMode; }
  bool ActivateRandomState(ushort);
  ushort GetRandomNotActivatedState();
  bool GainRandomInstric();
  virtual void CalculateBattleInfo() = 0;
  void CalculateBurdenState();
  virtual void CalculateDodgeValue();
  virtual void CalculateBodyParts() { BodyParts = 1; }
  virtual void CalculateAllowedWeaponSkillCategories() { AllowedWeaponSkillCategories = MARTIAL_SKILL_CATEGORIES; }
  uchar GetBodyParts() const { return BodyParts; }
  uchar GetAllowedWeaponSkillCategories() const { return AllowedWeaponSkillCategories; }
  virtual float GetRelativeDanger(const character*, bool = false) const;
  virtual float GetTimeToDie(const character*, ushort, float, bool, bool) const;
  virtual float GetTimeToKill(const character*, bool) const = 0;
  virtual bool HasFeet() const { return true; }
  float GetDangerModifier() const;
  virtual void AddSpecialEquipmentInfo(std::string&, ushort) const { }
  virtual bool ReloadDatafiles();
  virtual bool SummonMonster();
  virtual std::string GetBodyPartName(ushort, bool = false) const;
  virtual item* SearchForItemWithID(ulong) const;
  virtual bool ContentsCanBeSeenBy(const character*) const;
  std::string GetBeVerb() const { return IsPlayer() ? "are" : "is"; }
  virtual void CreateBlockPossibilityVector(blockvector&, float) const { }
  virtual bool SpecialUnarmedEffect(character*, uchar, uchar, bool) { return false; }
  virtual bool SpecialKickEffect(character*, uchar, uchar, bool) { return false; }
  virtual bool SpecialBiteEffect(character*, uchar, uchar, bool) { return false; }
  virtual bool HitEffect(character*, item*, uchar, uchar, uchar, bool);
  virtual void WeaponSkillHit(item*, uchar);
  virtual void AddDefenceInfo(felist&) const;
  character* Duplicate() const;
  room* GetRoomUnder() const { return GetLSquareUnder()->GetRoomClass(); }
  virtual bool TryToEquip(item*);
  virtual bool TryToConsume(item*);
  void UpdateESPLOS() const;
  virtual uchar GetCWeaponSkillLevel(const item*) const;
  virtual uchar GetSWeaponSkillLevel(const item*) const { return 0; }
  virtual void PrintBeginPanicMessage() const;
  virtual void PrintEndPanicMessage() const;
  virtual void CheckPanic(ushort);
  virtual character* CloneToNearestSquare(character*) const;
  virtual void SignalSpoil();
  virtual void SignalSpoilLevelChange();
  virtual bool IsAlive() const = 0;
  bool IsPolymorphed() const { return Polymorphed; }
  void SetPolymorphed(bool What) { Polymorphed = What; }
  bool IsInBadCondition() const { return HP * 3 < MaxHP; }
  bool IsInBadCondition(short HP) const { return HP * 3 < MaxHP; }
  virtual void UpdatePictures();
  virtual bool CanHeal() const;
  virtual vector2d GetWaypoint() const { return WayPoint; }
  virtual void SetWayPoint(vector2d What) { WayPoint = What; }
  virtual void ReceiveFluidSpill(material*, ushort);
  uchar GetRelation(const character*) const;
  virtual void CalculateAttributeBonuses();
  virtual void ApplyEquipmentAttributeBonuses(item*);
  virtual void ReceiveAntidote(long);
  virtual void AddAntidoteConsumeEndMessage() const;
  virtual bool IsDead() const;
  virtual bool ShowBattleInfo() = 0;
  void AddOriginalBodyPartID(ushort, ulong);
  void AddToInventory(const std::vector<contentscript<item> >&, ushort);
  virtual void ShowDodgeAndMoveInfo() const;
  virtual void AddSpecialMovePenaltyInfo(felist&) const { }
  virtual bool HasHadBodyPart(const item*) const;
  void MakeReply(std::string) const;
  virtual bool CheckZap();
  void SetEndurance(ushort What) { BaseAttribute[ENDURANCE] = What; }
  void SetPerception(ushort What) { BaseAttribute[PERCEPTION] = What; }
  void SetIntelligence(ushort What) { BaseAttribute[INTELLIGENCE] = What; }
  void SetWisdom(ushort What) { BaseAttribute[WISDOM] = What; }
  void SetCharisma(ushort What) { BaseAttribute[CHARISMA] = What; }
  void SetMana(ushort What) { BaseAttribute[MANA] = What; }
  virtual void DamageAllItems(character*, ushort, uchar);
  virtual bool Equips(const item*) const;
  virtual void AddAttributeInfo(std::string&) const;
  virtual bool LevelTeleport();
 protected:
  virtual character* RawDuplicate() const = 0;
  virtual void SpecialTurnHandler() { }
  void Initialize(ushort, ushort);
  virtual void VirtualConstructor(bool) { }
  virtual void LoadDataBaseStats();
  void InstallDataBase();
  virtual vector2d GetBodyPartBitmapPos(ushort);
  virtual ushort GetBodyPartColorA(ushort);
  virtual ushort GetBodyPartColorB(ushort);
  virtual ushort GetBodyPartColorC(ushort);
  virtual ushort GetBodyPartColorD(ushort);
  virtual ulong GetBodyPartSize(ushort, ushort);
  virtual ulong GetBodyPartVolume(ushort) const;
  virtual void UpdateBodyPartPicture(ushort);
  virtual uchar ChooseBodyPartToReceiveHit(float, float);
  virtual void CreateBodyParts(ushort);
  virtual material* CreateBodyPartMaterial(ushort, ulong) const = 0;
  virtual bool AddMaterialDescription(std::string&, bool) const;
  virtual bool ShowClassDescription() const { return true; }
  virtual void SeekLeader();
  virtual bool CheckForUsefulItemsOnGround();
  virtual bool CheckForDoors();
  virtual bool CheckForEnemies(bool, bool);
  virtual bool FollowLeader();
  virtual void StandIdleAI();
  virtual void CreateCorpse();
  virtual std::string GetDeathMessage() const { return GetName(DEFINITE) + " is slain."; }
  virtual void GetPlayerCommand();
  virtual void GetAICommand();
  virtual bool MoveTowards(vector2d);
  virtual std::string FirstPersonUnarmedHitVerb() const { return "hit"; }
  virtual std::string FirstPersonCriticalUnarmedHitVerb() const { return "critically hit"; }
  virtual std::string ThirdPersonUnarmedHitVerb() const { return "hits"; }
  virtual std::string ThirdPersonCriticalUnarmedHitVerb() const { return "critically hits"; }
  virtual std::string FirstPersonKickVerb() const { return "kick"; }
  virtual std::string FirstPersonCriticalKickVerb() const { return "critically kick"; }
  virtual std::string ThirdPersonKickVerb() const { return "kicks"; }
  virtual std::string ThirdPersonCriticalKickVerb() const { return "critically kicks"; }
  virtual std::string FirstPersonBiteVerb() const { return "bite"; }
  virtual std::string FirstPersonCriticalBiteVerb() const { return "critically bite"; }
  virtual std::string ThirdPersonBiteVerb() const { return "bites"; }
  virtual std::string ThirdPersonCriticalBiteVerb() const { return "critically bites"; }
  virtual std::string UnarmedHitNoun() const { return "hit"; }
  virtual std::string KickNoun() const { return "kick"; }
  virtual std::string BiteNoun() const { return "attack"; }
  virtual bool AttackIsBlockable(uchar) const { return true; }
  virtual uchar GetSpecialBodyPartFlags(ushort) const { return ST_NORMAL; }
  stack* Stack;
  long NP, AP;
  bool Player;
  ushort TemporaryState;
  short TemporaryStateCounter[STATES];
  team* Team;
  vector2d WayPoint;
  ulong Money;
  uchar HomeRoom;
  std::list<character*>::iterator TeamIterator;
  characterslot* BodyPartSlot;
  std::string AssignedName;
  action* Action;
  ushort Config;
  const database* DataBase;
  ushort StuckToBodyPart;
  item* StuckTo; // Bad naming. Sorry.
  ushort BaseAttribute[BASE_ATTRIBUTES];
  long BaseExperience[BASE_ATTRIBUTES];
  std::list<ulong>* OriginalBodyPartID;
  entity* MotherEntity;
  character* PolymorphBackup;
  cweaponskill** CWeaponSkill;
  ushort EquipmentState;
  static void (character::*PrintBeginStateMessage[])() const;
  static void (character::*PrintEndStateMessage[])() const;
  static void (character::*BeginStateHandler[])();
  static void (character::*EndStateHandler[])();
  static void (character::*StateHandler[])();
  static std::string StateDescription[];
  static bool StateIsSecret[];
  static bool StateCanBeRandomlyActivated[];
  square* SquareUnder;
  static prototype character_ProtoType;
  ulong Volume;
  ulong Weight;
  ulong CarriedWeight;
  ulong BodyVolume;
  short HP;
  short MaxHP;
  bool Initializing;
  uchar BurdenState;
  float DodgeValue;
  uchar AllowedWeaponSkillCategories;
  uchar BodyParts;
  bool Polymorphed;
  bool InNoMsgMode;
  ulong RegenerationCounter;
  short AttributeBonus[BASE_ATTRIBUTES];
  short CarryingBonus;
};

#ifdef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__
#define CHARACTER_PROTOTYPE(name, baseproto) characterprototype name::name##_ProtoType(baseproto, &name::Clone, #name);
#else
#define CHARACTER_PROTOTYPE(name, baseproto)
#endif

#define CHARACTER(name, base, data)\
\
name : public base\
{\
 public:\
  name(ushort Config = 0, ushort SpecialFlags = 0) : base(donothing()) { Initialize(Config, SpecialFlags); }\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static character* Clone(ushort Config, ushort SpecialFlags) { return new name(Config, SpecialFlags); }\
 protected:\
  virtual character* RawDuplicate() const { return new name(*this); }\
  static prototype name##_ProtoType;\
  data\
}; CHARACTER_PROTOTYPE(name, &base##_ProtoType);

#define ABSTRACT_CHARACTER(name, base, data)\
\
name : public base\
{\
 public:\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static character* Clone(ushort, ushort) { return 0; }\
 protected:\
  static prototype name##_ProtoType;\
  data\
}; CHARACTER_PROTOTYPE(name, &base##_ProtoType);

#endif
