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

#define CHARDESCRIPTION(Case) Description(Case).c_str()

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
class gweaponskill;
class stackslot;
class god;
typedef std::list<stackslot*>::iterator stackiterator;
template <class type> class database;

struct characterdatabase
{
  void InitDefaults() { IsAbstract = false; }
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
  std::string TalkVerb;
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
  ushort TorsoSpecialColor;
  ushort ArmMainColor;
  ushort ArmSpecialColor;
  ushort LegMainColor;
  ushort LegSpecialColor;
  uchar HeadBonePercentile;
  uchar TorsoBonePercentile;
  uchar ArmBonePercentile;
  uchar RightArmBonePercentile;
  uchar LeftArmBonePercentile;
  uchar GroinBonePercentile;
  uchar LegBonePercentile;
  uchar RightLegBonePercentile;
  uchar LeftLegBonePercentile;
  bool IsNameable;
  ushort BaseEmitation;
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
};

class characterprototype
{
 public:
  friend class database<character>;
  characterprototype(characterprototype*, character* (*)(ushort, bool, bool), const std::string&);
  character* Clone(ushort Config = 0, bool CreateEquipment = true) const { return Cloner(Config, CreateEquipment, false); }
  character* CloneAndLoad(inputfile&) const;
  ushort GetIndex() const { return Index; }
  const characterdatabase* GetDataBase() const { return &DataBase; }
  const characterprototype* GetBase() const { return Base; }
  PROTODATABASEVALUE(ushort, Frequency);
  PROTODATABASEBOOL(CanBeGenerated);
  PROTODATABASEBOOL(IsAbstract);
  PROTODATABASEBOOL(CanBeWished);
  PROTODATABASEVALUE(const std::vector<std::string>&, Alias);
  const std::map<ushort, characterdatabase>& GetConfig() const { return Config; }
  const std::string& GetClassId() const { return ClassId; }
 protected:
  ushort Index;
  characterdatabase DataBase;
  characterprototype* Base;
  std::map<ushort, characterdatabase> Config;
  character* (*Cloner)(ushort, bool, bool);
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
  virtual stack* GetStack() const { return Stack; }
  virtual uchar GetBurdenState() const { return BurdenState; }
  virtual bool MakesBurdened(ulong What) const { return ulong(GetCarryingStrength()) * 2500 < What; }
  virtual uchar TakeHit(character*, item*, float, float, short, uchar, bool);
  virtual ulong CurrentDanger() const;
  virtual ulong MaxDanger() const;
  virtual ushort LOSRange() const;
  virtual ushort LOSRangeSquare() const;
  virtual ushort ESPRange() const;
  virtual ushort ESPRangeSquare() const;
  virtual void AddMissMessage(character*) const;
  virtual void AddPrimitiveHitMessage(character*, const std::string&, const std::string&, uchar) const;
  virtual void AddWeaponHitMessage(character*, item*, uchar, bool = false) const;
  virtual void ApplyExperience(bool = false);
  virtual void BeTalkedTo(character*);
  virtual void ReceiveDarkness(long);
  virtual void Die(bool = false);
  virtual void HasBeenHitByItem(character*, item*, float);
  virtual void HealFully(character*) { }
  virtual void Hunger(ushort = 1);
  virtual void Move(vector2d, bool = false);
  virtual bool MoveRandomly();
  virtual void ReceiveNutrition(long);
  virtual void ReceiveOmleUrine(long);
  virtual void ReceivePepsi(long);
  virtual void ReceivePoison(long);
  void Regenerate();
  void SetAP(long What) { AP = What; }
  void SetIsPlayer(bool What) { Player = What; }
  void SetNP(long);
  virtual void SpillBlood(uchar);
  virtual void SpillBlood(uchar, vector2d);
  virtual void Vomit(ushort);
  virtual void Be();
  virtual bool Zap();
  virtual bool Polymorph(character*, ushort);
  virtual void BeKicked(character*, float, float, short, bool);
  virtual void FallTo(character*, vector2d);
  virtual bool CheckCannibalism(ushort) const;
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
  virtual team* GetTeam() const { return Team; }
  virtual void SetTeam(team*);
  virtual void ChangeTeam(team*);
  virtual bool RestUntilHealed();
  virtual bool OutlineCharacters();
  virtual bool OutlineItems();
  virtual uchar GetMoveEase() const;
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
  virtual void AddSpecialItemInfo(std::string&, item*) { }
  virtual void AddSpecialItemInfoDescription(std::string&) { }
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
  virtual uchar RandomizeReply(uchar, bool*);
  virtual ushort DangerLevel() const;
  virtual void CreateInitialEquipment() { }
  virtual void DisplayInfo(std::string&);
  virtual bool SpecialEnemySightedReaction(character*) { return false; }
  virtual void TestWalkability();
  void EditAP(long What) { AP += What; if(AP > 1200) AP = 1200; }
  void EditNP(long What) { NP += What; }
  virtual void SetSize(ushort);
  virtual ushort GetSize() const;
  torso* GetTorso() const { return static_cast<torso*>(*BodyPartSlot[TORSOINDEX]); }
  humanoidtorso* GetHumanoidTorso() const { return static_cast<humanoidtorso*>(*BodyPartSlot[TORSOINDEX]); }
  void SetTorso(torso* What) { SetBodyPart(TORSOINDEX, What); }
  bodypart* GetBodyPart(ushort Index) const { return static_cast<bodypart*>(*BodyPartSlot[Index]); }
  void SetBodyPart(ushort, bodypart*);
  virtual void SetMainMaterial(material*);
  virtual void ChangeMainMaterial(material*);
  virtual void SetSecondaryMaterial(material*);
  virtual void ChangeSecondaryMaterial(material*);
  virtual void SetContainedMaterial(material*);
  virtual void ChangeContainedMaterial(material*);
  virtual void Teleport(vector2d);
  virtual bool SecretKnowledge();
  virtual void RestoreHP();
  virtual bool ReceiveDamage(character*, short, uchar, uchar = ALL, uchar = 8, bool = false, bool = false, bool = false);
  virtual bool ReceiveBodyPartDamage(character*, short, uchar, uchar, uchar = 8, bool = false, bool = false);
  virtual bool BodyPartVital(ushort) const { return true; }
  virtual void RestoreBodyParts();
  virtual bool AssignName();
  virtual std::string GetAssignedName() const { return AssignedName; }
  virtual void SetAssignedName(const std::string& What) { AssignedName = What; }
  virtual std::string Description(uchar) const;
  virtual std::string PersonalPronoun() const;
  virtual std::string PossessivePronoun() const;
  virtual std::string ObjectPronoun() const;
  virtual bool BodyPartCanBeSevered(ushort) const;
  virtual void AddName(std::string&, uchar) const;
  virtual void ReceiveHeal(long);
  virtual item* GetMainWielded() const { return 0; }
  virtual item* GetSecondaryWielded() const { return 0; }
  virtual item* GetBodyArmor() const { return 0; }
  virtual void SetMainWielded(item*) { }
  virtual void SetSecondaryWielded(item*) { }
  virtual void SetBodyArmor(item*) { }
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
  virtual void BlockDamageType(uchar);
  virtual bool AllowDamageTypeBloodSpill(uchar) const;
  virtual bool DamageTypeCanSeverBodyPart(uchar) const;
  virtual void TalkTo(character*);
  virtual bool ClosePos(vector2d);
  virtual ushort GetResistance(uchar) const;
  virtual ushort GlobalResistance(uchar Type) const { return GetResistance(Type); }
  virtual std::string EquipmentName(ushort) const { return ""; }
  virtual bodypart* GetBodyPartOfEquipment(ushort) const { return 0; }
  virtual item* GetEquipment(ushort) const { return 0; }
  virtual ushort EquipmentSlots() const { return 0; }
  virtual bool (*EquipmentSorter(ushort) const)(item*, character*) { return 0; }
  virtual void SetEquipment(ushort, item*) { }
  virtual bool ScrollMessagesDown();
  virtual bool ScrollMessagesUp();
  virtual void AddHealingLiquidConsumeEndMessage() const;
  virtual void AddSchoolFoodConsumeEndMessage() const;
  virtual void AddSchoolFoodHitMessage() const;
  virtual void AddOmleUrineConsumeEndMessage() const;
  virtual void AddPepsiConsumeEndMessage() const;
  virtual void AddFrogFleshConsumeEndMessage() const;
  virtual void AddKoboldFleshConsumeEndMessage() const;
  virtual void AddKoboldFleshHitMessage() const;
  virtual void AddBoneConsumeEndMessage() const;
  virtual void AddInfo(felist&) const = 0;
  virtual void PrintInfo() const;
  virtual bodypart* SevereBodyPart(ushort);
  virtual bool IsAnimated() const { return false; }
  virtual void CompleteRiseFromTheDead();
  virtual bool RaiseTheDead(character*);
  virtual void CreateBodyPart(ushort);
  virtual bool CanUseEquipment(ushort Index) const { return CanUseEquipment() && Index < EquipmentSlots() && GetBodyPartOfEquipment(Index); }
  virtual const prototype* GetProtoType() const { return &character_ProtoType; }
  const database* GetDataBase() const { return DataBase; }
  virtual void SetParameters(uchar) { }
  virtual DATABASEVALUE(ushort, DefaultArmStrength);
  virtual DATABASEVALUE(ushort, DefaultLegStrength);
  virtual DATABASEVALUE(ushort, DefaultDexterity);
  virtual DATABASEVALUE(ushort, DefaultAgility);
  virtual DATABASEVALUE(ushort, DefaultEndurance);
  virtual DATABASEVALUE(ushort, DefaultPerception);
  virtual DATABASEVALUE(ushort, DefaultIntelligence);
  virtual DATABASEVALUE(ushort, DefaultWisdom);
  virtual DATABASEVALUE(ushort, DefaultCharisma);
  virtual DATABASEVALUE(ushort, DefaultMana);
  virtual DATABASEVALUE(ulong, DefaultMoney);
  virtual DATABASEVALUE(ushort, TotalSize);
  virtual DATABASEBOOL(CanRead);
  virtual DATABASEBOOL(IsCharmable);
  virtual DATABASEVALUE(uchar, Sex);
  virtual DATABASEVALUE(ulong, BloodColor);
  virtual DATABASEBOOL(CanBeGenerated);
  virtual DATABASEVALUE(uchar, CriticalModifier);
  virtual DATABASEVALUE(const std::string&, StandVerb);
  virtual DATABASEBOOL(CanOpen);
  virtual DATABASEBOOL(CanBeDisplaced);
  virtual DATABASEVALUE(ushort, Frequency);
  virtual DATABASEBOOL(CanWalk);
  virtual DATABASEBOOL(CanSwim);
  virtual DATABASEBOOL(CanFly);
  virtual DATABASEVALUE(ushort, PhysicalDamageResistance);
  virtual DATABASEVALUE(ushort, SoundResistance);
  virtual DATABASEVALUE(ushort, EnergyResistance);
  virtual DATABASEVALUE(ushort, AcidResistance);
  virtual DATABASEVALUE(ushort, FireResistance);
  virtual DATABASEVALUE(ushort, PoisonResistance);
  virtual DATABASEVALUE(ushort, BulimiaResistance);
  virtual DATABASEBOOL(IsUnique);
  virtual DATABASEVALUE(ushort, ConsumeFlags);
  virtual DATABASEVALUE(ulong, TotalVolume);
  virtual DATABASEVALUE(const std::string&, TalkVerb);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, HeadBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, TorsoBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, ArmBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, LegBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, RightArmBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, LeftArmBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, RightLegBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, LeftLegBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(vector2d, GroinBitmapPos, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, ClothColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, SkinColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, CapColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, HairColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, EyeColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, TorsoMainColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, BeltColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, TorsoSpecialColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, ArmMainColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, ArmSpecialColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, LegMainColor, ushort);
  virtual DATABASEVALUEWITHPARAMETER(ushort, LegSpecialColor, ushort);
  virtual DATABASEVALUE(uchar, HeadBonePercentile);
  virtual DATABASEVALUE(uchar, TorsoBonePercentile);
  virtual DATABASEVALUE(uchar, ArmBonePercentile);
  virtual DATABASEVALUE(uchar, RightArmBonePercentile);
  virtual DATABASEVALUE(uchar, LeftArmBonePercentile);
  virtual DATABASEVALUE(uchar, GroinBonePercentile);
  virtual DATABASEVALUE(uchar, LegBonePercentile);
  virtual DATABASEVALUE(uchar, RightLegBonePercentile);
  virtual DATABASEVALUE(uchar, LeftLegBonePercentile);
  virtual DATABASEBOOL(IsNameable);
  virtual DATABASEVALUE(ushort, BaseEmitation);
  virtual DATABASEVALUE(const std::string&, Article);
  virtual DATABASEVALUE(const std::string&, Adjective);
  virtual DATABASEVALUE(const std::string&, AdjectiveArticle);
  virtual DATABASEVALUE(const std::string&, NameSingular);
  virtual DATABASEVALUE(const std::string&, NamePlural);
  virtual DATABASEVALUE(const std::string&, PostFix);
  virtual DATABASEVALUE(uchar, ArticleMode);
  virtual DATABASEBOOL(IsPolymorphable);
  virtual DATABASEVALUE(ulong, BaseUnarmedStrength);
  virtual DATABASEVALUE(ulong, BaseBiteStrength);
  virtual DATABASEVALUE(ulong, BaseKickStrength);
  virtual DATABASEVALUE(uchar, AttackStyle);
  virtual DATABASEBOOL(CanUseEquipment);
  virtual DATABASEBOOL(CanKick);
  virtual DATABASEBOOL(CanTalk);
  virtual DATABASEVALUE(ushort, ClassStates);
  virtual DATABASEBOOL(CanBeWished);
  virtual DATABASEVALUE(const std::vector<std::string>&, Alias);
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual void TeleportRandomly();
  virtual bool TeleportNear(character*);
  static character* Clone(ushort, bool, bool) { return 0; }
  virtual bool IsStuck() const;
  virtual void InitSpecialAttributes() { }
  virtual void Kick(lsquare*) = 0;
  virtual void SpecialBiteEffect(character*) { }
  virtual float GetAttackStrengthDanger() const = 0;
  virtual ushort GetAttribute(ushort Identifier) const { return BaseAttribute[Identifier]; }
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort Identifier, long Value) { BaseExperience[Identifier] += Value; }
  virtual bool CheckForAttributeIncrease(ushort&, long&, bool = false);
  virtual bool CheckForAttributeDecrease(ushort&, long&, bool = false);
  virtual bool RawEditAttribute(ushort&, short&, bool = false);
  virtual void DrawPanel(bool) const;
  virtual ushort DrawStats(bool) const = 0;
  virtual ushort GetCarryingStrength() const = 0;
  ulong GetOriginalBodyPartID(ushort Index) const { return OriginalBodyPartID[Index]; }
  void SetOriginalBodyPartID(ushort Index, ulong ID) { OriginalBodyPartID[Index] = ID; }
  virtual bool DamageTypeAffectsInventory(uchar) const;
  virtual void SetStuckTo(item* What) {  StuckTo = What; }
  virtual item* GetStuckTo() const { return StuckTo; }
  virtual void SetStuckToBodyPart(ushort What) { StuckToBodyPart = What; }
  virtual ushort GetStuckToBodyPart() const { return StuckToBodyPart; }
  virtual bool TryToUnstuck(vector2d);
  virtual ushort GetRandomStepperBodyPart() const { return TORSOINDEX; }
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  virtual ushort CheckForBlock(character*, item*, float, ushort Damage, short, uchar) { return Damage; }
  virtual ushort CheckForBlockWithItem(character*, item*, item*, float, float, ushort, short, uchar);
  virtual void AddBlockMessage(character*, item*, const std::string&, bool) const;
  virtual character* GetPolymorphBackup() const { return PolymorphBackup; }
  virtual void SetPolymorphBackup(character* What) { PolymorphBackup = What; }
  gweaponskill* GetCategoryWeaponSkill(ushort Index) const { return CategoryWeaponSkill[Index]; }
  virtual bool AddSpecialSkillInfo(felist&) const { return false; }
  virtual bool CheckBalance(float);
  long GetStateAPGain(long) const;
  virtual long GetMoveAPRequirement(uchar Difficulty) const { return (long(GetAttribute(AGILITY)) - 200) * Difficulty * GetMoveEase() / 20; }
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
  virtual void CharacterSpeciality() { }
  virtual void PrintBeginInvisibilityMessage() const;
  virtual void PrintEndInvisibilityMessage() const;
  virtual void PrintBeginInfraVisionMessage() const;
  virtual void PrintEndInfraVisionMessage() const;
  virtual void PrintBeginESPMessage() const;
  virtual void PrintEndESPMessage() const;
  virtual bool CanBeSeenByPlayer() const;
  virtual bool CanBeSeenBy(character*) const;
  virtual bool DetachBodyPart();
  virtual bodypart* MakeBodyPart(ushort);
  virtual void AttachBodyPart(bodypart*, ushort);
  virtual uchar GetBodyParts() const { return 1; }
  virtual bool HasAllBodyParts() const;
  virtual stackiterator FindRandomOwnBodyPart();
  virtual bodypart* GenerateRandomBodyPart();
  virtual std::vector<character*> GetFriendsAround() const;
  virtual bodypart* TryAttachRandomOldBodyPart();
  virtual bodypart* AttachOldBodyPartFromStack(stackiterator, stack*);
  virtual void PrintBeginPoisonedMessage() const;
  virtual void PrintEndPoisonedMessage() const;
  virtual bool IsWarm() const;
  virtual void CalculateEquipmentState();
  virtual void Draw(bitmap*, vector2d, ushort, bool, bool) const;
  virtual void DrawBodyParts(bitmap*, vector2d, ushort, bool, bool) const;
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
  virtual square* GetSquareUnder() const { return SquareUnder; }
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
  virtual void SignalEmitationIncrease(ushort);
  virtual void SignalEmitationDecrease(ushort);
  void CalculateEmitation();
  void CalculateAll();
  void CalculateHP();
  void CalculateMaxHP();
  short GetHP() const { return HP; }
  short GetMaxHP() const { return MaxHP; }
  void CalculateBodyPartMaxHPs();
  bool IsInitializing() const { return Initializing; }
  bool ActivateRandomState(ushort);
  ushort GetRandomNotActivatedState();
  bool GainRandomInstric();
  virtual void CalculateBattleInfo() = 0;
  void CalculateBurdenState();
  void CalculateDodgeValue();
 protected:
  virtual void SpecialTurnHandler() { }
  virtual uchar GetAllowedWeaponSkillCategories() const { return MARTIAL_SKILL_CATEGORIES; }
  void Initialize(uchar, bool, bool);
  virtual void VirtualConstructor(bool);
  virtual void LoadDataBaseStats();
  void InstallDataBase();
  virtual vector2d GetBodyPartBitmapPos(ushort, ushort);
  virtual ushort GetBodyPartColorA(ushort, ushort);
  virtual ushort GetBodyPartColorB(ushort, ushort);
  virtual ushort GetBodyPartColorC(ushort, ushort);
  virtual ushort GetBodyPartColorD(ushort, ushort);
  virtual ushort GetBodyPartAnimationFrames(ushort) const { return 1; }
  virtual ulong GetBodyPartSize(ushort, ushort);
  virtual ulong GetBodyPartVolume(ushort);
  virtual uchar GetBodyPartBonePercentile(ushort);
  virtual void UpdateBodyPartPicture(ushort);
  virtual void UpdateBodyPartPictures();
  virtual uchar ChooseBodyPartToReceiveHit(float, float);
  virtual void CreateBodyParts();
  virtual material* CreateBodyPartFlesh(ushort, ulong) const = 0;
  virtual material* CreateBodyPartBone(ushort, ulong) const;
  virtual bool AddMaterialDescription(std::string&, bool) const;
  virtual bool ShowClassDescription() const { return true; }
  virtual void SeekLeader();
  virtual bool CheckForUsefulItemsOnGround();
  virtual bool CheckForDoors();
  virtual bool CheckForEnemies(bool);
  virtual bool FollowLeader();
  virtual void StandIdleAI();
  virtual void CreateCorpse();
  virtual std::string GetDeathMessage() { return GetName(DEFINITE) + " is slain."; }
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
  virtual uchar GetSpecialBodyPartFlags(ushort, ushort) const { return STNORMAL; }
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
  ushort BaseAttribute[BASEATTRIBUTES];
  long BaseExperience[BASEATTRIBUTES];
  ulong* OriginalBodyPartID;
  entity* MotherEntity;
  character* PolymorphBackup;
  gweaponskill** CategoryWeaponSkill;
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
  name(ushort Config = 0, bool CreateEquipment = true, bool Load = false) : base(donothing()) { Initialize(Config, CreateEquipment, Load); }\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static character* Clone(ushort Config, bool CreateEquipment, bool Load) { return new name(Config, CreateEquipment, Load); }\
 protected:\
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
  static character* Clone(ushort, bool, bool) { return 0; }\
 protected:\
  static prototype name##_ProtoType;\
  data\
}; CHARACTER_PROTOTYPE(name, &base##_ProtoType);

#endif

