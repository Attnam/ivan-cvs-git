/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

#ifndef __BODYPART_H__
#define __BODYPART_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "item.h"

class humanoid;
class sweaponskill;

class ABSTRACT_ITEM
(
  bodypart,
  item,
 public:
  friend class corpse;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetGraphicsContainerIndex() const;
  character* GetMaster() const { return Master; }
  humanoid* GetHumanoidMaster() const;
  void SetMaster(character* What) { Master = What; }
  virtual int GetStrengthValue() const;
  int GetMaxHP() const { return MaxHP; }
  void SetHP(int);
  int GetHP() const { return HP; }
  void EditHP(int);
  void IncreaseHP() { ++HP; }
  virtual int GetTotalResistance(int) const = 0;
  virtual bool ReceiveDamage(character*, int, int, int);
  const festring& GetOwnerDescription() const { return OwnerDescription; }
  void SetOwnerDescription(const festring& What) { OwnerDescription = What; }
  bool IsUnique() const { return Unique; }
  void SetIsUnique(bool What) { Unique = What; }
  virtual void DropEquipment(stack* = 0) { }
  //virtual bool ApplyExperience() { return false; }
  virtual void InitSpecialAttributes() { }
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalEquipmentRemoval(gearslot*);
  virtual void Mutate();
  long GetBodyPartVolume() const { return BodyPartVolume; }
  long GetCarriedWeight() const { return CarriedWeight; }
  virtual item* GetEquipment(int) const { return 0; }
  virtual int GetEquipmentSlots() const { return 0; }
  virtual void CalculateVolumeAndWeight();
  virtual void CalculateEmitation();
  virtual void CalculateMaxHP(bool = true);
  virtual void SignalVolumeAndWeightChange();
  void RestoreHP() { HP = MaxHP; }
  virtual void CalculateDamage() { }
  virtual void CalculateToHitValue() { }
  virtual void CalculateAPCost() { }
  void CalculateAttackInfo();
  double GetTimeToDie(int, double, double, bool, bool) const;
  virtual double GetRoughChanceToHit(double, double) const;
  const festring& GetBodyPartName() const { return GetNameSingular(); }
  void RandomizePosition();
  void ResetPosition() { SpecialFlags &= ~0x7; }
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  bool IsAlive() const;
  virtual long GetTruePrice() const;
  void SpillBlood(int);
  void SpillBlood(int, vector2d);
  virtual void Be();
  bool IsInBadCondition() const { return (HP << 1) + HP < MaxHP || (HP == 1 && MaxHP != 1); }
  bool IsInBadCondition(int HP) const { return (HP << 1) + HP < MaxHP || (HP == 1 && MaxHP != 1); }
  int GetConditionColorIndex() const;
  void SetBitmapPos(vector2d What) { BitmapPos = What; }
  void SetSpecialFlags(int What) { SpecialFlags = What; }
  void SetMaterialColorB(color16 What) { ColorB = What; }
  void SetMaterialColorC(color16 What) { ColorC = What; }
  void SetMaterialColorD(color16 What) { ColorD = What; }
  virtual void SignalEnchantmentChange();
  virtual void CalculateAttributeBonuses() { }
  virtual void SignalSpoilLevelChange(material*);
  virtual bool CanBeEatenByAI(const character*) const;
  virtual bool DamageArmor(character*, int, int) { return false; }
  bool CannotBeSevered(int);
  virtual bool EditAllAttributes(int) { return false; }
  virtual void Draw(bitmap*, vector2d, color24, int, bool, bool) const;
  void SetIsSparklingB(bool What) { IsSparklingB = What; }
  void SetIsSparklingC(bool What) { IsSparklingC = What; }
  void SetIsSparklingD(bool What) { IsSparklingD = What; }
  virtual int GetSpecialFlags() const;
  bool IsRepairable() const;
  bool IsWarm() const;
  bool UseMaterialAttributes() const;
  bool CanRegenerate() const;
  virtual square* GetSquareUnder(int = 0) const;
  virtual lsquare* GetLSquareUnder(int = 0) const;
  virtual item* GetArmorToReceiveFluid(bool) const { return 0; }
  virtual void SpillFluid(character*, liquid*, int = 0);
  void StayOn(liquid*);
  void SetBloodMaterial(int What) { BloodMaterial = What; }
  int GetBloodMaterial() const { return BloodMaterial; }
  liquid* CreateBlood(long) const;
  virtual bool UpdateArmorPictures() { return false; }
  virtual void DrawArmor(bitmap*, vector2d, color24, bool) const { }
  virtual void UpdatePictures();
  item* GetExternalBodyArmor() const;
  item* GetExternalCloak() const;
  virtual void ReceiveAcid(material*, const festring&, long);
  virtual bool ShowFluids() const { return false; }
  virtual void TryToRust(long);
  virtual bool AllowFluidBe() const;
  virtual material* RemoveMaterial(material*);
  virtual void CopyAttributes(const bodypart*) { }
  virtual void DestroyBodyPart(stack*);
  virtual void SetLifeExpectancy(int, int);
  virtual void SpecialEatEffect(character*, int);
 protected:
  virtual bool AllowFluids() const { return true; }
  virtual bool IsSparkling(int) const;
  virtual alpha GetMaxAlpha() const;
  virtual void GenerateMaterials() { }
  virtual void VirtualConstructor(bool);
  virtual void AddPostFix(festring&) const;
  virtual bool ShowMaterial() const { return false; }
  virtual int GetArticleMode() const;
  virtual color16 GetMaterialColorA(int) const;
  virtual color16 GetMaterialColorB(int) const { return ColorB; }
  virtual color16 GetMaterialColorC(int) const { return ColorC; }
  virtual color16 GetMaterialColorD(int) const { return ColorD; }
  virtual vector2d GetBitmapPos(int) const { return BitmapPos; }
  void UpdateArmorPicture(graphicdata&, item*, int, vector2d (item::*)(int) const, bool = false) const;
  void DrawEquipment(const graphicdata&, bitmap*, vector2d, color24, bool) const;
  festring OwnerDescription;
  vector2d BitmapPos;
  color16 ColorB;
  color16 ColorC;
  color16 ColorD;
  int SpecialFlags;
  int HP;
  int MaxHP;
  bool Unique;
  long CarriedWeight;
  long BodyPartVolume;
  character* Master;
  bool IsSparklingB;
  bool IsSparklingC;
  bool IsSparklingD;
  int BloodMaterial;
  uchar SpillBloodCounter;
);

class ITEM
(
  head,
  bodypart,
 public:
  head(const head&);
  virtual ~head();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetTotalResistance(int) const;
  void SetHelmet(item* What) { HelmetSlot.PutInItem(What); }
  item* GetHelmet() const { return *HelmetSlot; }
  void SetAmulet(item* What) { AmuletSlot.PutInItem(What); }
  item* GetAmulet() const { return *AmuletSlot; }
  virtual void DropEquipment(stack* = 0);
  virtual int GetBodyPartIndex() const;
  double GetBiteDamage() const { return BiteDamage; }
  int GetBiteMinDamage() const;
  int GetBiteMaxDamage() const;
  double GetBiteToHitValue() const { return BiteToHitValue; }
  long GetBiteAPCost() const { return BiteAPCost; }
  virtual void InitSpecialAttributes();
  virtual item* GetEquipment(int) const;
  virtual int GetEquipmentSlots() const { return 2; }
  int GetBaseBiteStrength() const { return BaseBiteStrength; }
  void SetBaseBiteStrength(long What) { BaseBiteStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  virtual bool DamageArmor(character*, int, int);
  virtual head* Behead();
  virtual item* GetArmorToReceiveFluid(bool) const;
 protected:
  void UpdateHeadArmorPictures(graphicdata&) const;
  virtual void VirtualConstructor(bool);
  gearslot HelmetSlot;
  gearslot AmuletSlot;
  int BaseBiteStrength;
  double BiteToHitValue;
  double BiteDamage;
  long BiteAPCost;
);

class ABSTRACT_ITEM
(
  torso,
  bodypart,
 public:
  virtual int GetBodyPartIndex() const;
  virtual double GetRoughChanceToHit(double, double) const;
);

class ITEM
(
  normaltorso,
  torso,
 public:
  virtual int GetGraphicsContainerIndex() const;
  virtual int GetTotalResistance(int) const;
);

class ITEM
(
  humanoidtorso,
  torso,
 public:
  humanoidtorso(const humanoidtorso&);
  virtual ~humanoidtorso();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetTotalResistance(int) const;
  void SetBodyArmor(item* What) { BodyArmorSlot.PutInItem(What); }
  item* GetBodyArmor() const { return *BodyArmorSlot; }
  void SetCloak(item* What) { CloakSlot.PutInItem(What); }
  item* GetCloak() const { return *CloakSlot; }
  void SetBelt(item* What) { BeltSlot.PutInItem(What); }
  item* GetBelt() const { return *BeltSlot; }
  virtual void DropEquipment(stack* = 0);
  virtual item* GetEquipment(int) const;
  virtual int GetEquipmentSlots() const { return 3; }
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalVolumeAndWeightChange();
  virtual bool DamageArmor(character*, int, int);
  virtual item* GetArmorToReceiveFluid(bool) const;
 protected:
  virtual void VirtualConstructor(bool);
  void UpdateTorsoArmorPictures(graphicdata&, graphicdata&, graphicdata&) const;
  gearslot BodyArmorSlot;
  gearslot CloakSlot;
  gearslot BeltSlot;
);

class ABSTRACT_ITEM
(
  arm,
  bodypart,
 public:
  arm(const arm&);
  virtual ~arm();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetTotalResistance(int) const;
  double GetWieldedDamage() const;
  double GetWieldedToHitValue() const;
  void SetWielded(item* What) { WieldedSlot.PutInItem(What); }
  item* GetWielded() const { return *WieldedSlot; }
  void SetGauntlet(item* What) { GauntletSlot.PutInItem(What); }
  item* GetGauntlet() const { return *GauntletSlot; }
  void SetRing(item* What) { RingSlot.PutInItem(What); }
  item* GetRing() const { return *RingSlot; }
  virtual void DropEquipment(stack* = 0);
  double GetUnarmedToHitValue() const;
  double GetUnarmedDamage() const;
  void Hit(character*, vector2d, int, bool = false);
  int GetAttribute(int) const;
  bool EditAttribute(int, int);
  void EditExperience(int, double, double);
  //virtual bool ApplyExperience();
  void SetStrength(int What) { StrengthExperience = What * EXP_MULTIPLIER; }
  void SetDexterity(int What) { DexterityExperience = What * EXP_MULTIPLIER; }
  virtual void InitSpecialAttributes();
  virtual void Mutate();
  /*int GetDexterity() const { return Dexterity; }
  int GetStrength() const { return Strength; }*/
  virtual arm* GetPairArm() const = 0;
  long GetWieldedAPCost() const;
  long GetUnarmedAPCost() const;
  virtual item* GetEquipment(int) const;
  virtual int GetEquipmentSlots() const { return 3; }
  int GetBaseUnarmedStrength() const { return BaseUnarmedStrength; }
  void SetBaseUnarmedStrength(long What) { BaseUnarmedStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  double GetDamage() const { return Damage; }
  int GetMinDamage() const;
  int GetMaxDamage() const;
  double GetToHitValue() const { return ToHitValue; }
  long GetAPCost() const { return APCost; }
  bool PairArmAllowsMelee() const;
  virtual void SignalVolumeAndWeightChange();
  bool TwoHandWieldIsActive() const;
  double GetBlockChance(double) const;
  int GetBlockCapability() const;
  void WieldedSkillHit(int);
  double GetBlockValue() const;
  void ApplyEquipmentAttributeBonuses(item*);
  virtual void CalculateAttributeBonuses();
  int GetWieldedHitStrength() const;
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalEquipmentRemoval(gearslot*);
  void ApplyDexterityPenalty(item*);
  virtual bool DamageArmor(character*, int, int);
  bool CheckIfWeaponTooHeavy(const char*) const;
  virtual bool EditAllAttributes(int);
  void AddAttackInfo(felist&) const;
  void AddDefenceInfo(felist&) const;
  void UpdateWieldedPicture();
  void DrawWielded(bitmap*, vector2d, color24, bool) const;
  virtual bool IsRightArm() const = 0;
  virtual void UpdatePictures();
  virtual double GetTypeDamage(const character*) const;
  virtual item* GetArmorToReceiveFluid(bool) const;
  virtual void CopyAttributes(const bodypart*);
  double GetStrengthExperience() const { return StrengthExperience; }
  double GetDexterityExperience() const { return DexterityExperience; }
 protected:
  virtual sweaponskill*& GetCurrentSWeaponSkill() const = 0;
  virtual void VirtualConstructor(bool);
  void UpdateArmArmorPictures(graphicdata&, graphicdata&, int) const;
  gearslot WieldedSlot;
  gearslot GauntletSlot;
  gearslot RingSlot;
  /*int Strength;
  int Dexterity;*/
  double StrengthExperience;
  double DexterityExperience;
  int BaseUnarmedStrength;
  double Damage;
  double ToHitValue;
  long APCost;
  int StrengthBonus;
  int DexterityBonus;
  graphicdata WieldedGraphicData;
);

class ITEM
(
  rightarm,
  arm,
 public:
  rightarm(const rightarm&);
  virtual int GetBodyPartIndex() const;
  virtual arm* GetPairArm() const;
  virtual bool IsRightArm() const { return true; }
  virtual int GetSpecialFlags() const;
 protected:
  virtual sweaponskill*& GetCurrentSWeaponSkill() const;
  virtual void VirtualConstructor(bool);
);

class ITEM
(
  leftarm,
  arm,
 public:
  leftarm(const leftarm&);
  virtual int GetBodyPartIndex() const;
  virtual arm* GetPairArm() const;
  virtual bool IsRightArm() const { return false; }
  virtual int GetSpecialFlags() const;
 protected:
  virtual sweaponskill*& GetCurrentSWeaponSkill() const;
  virtual void VirtualConstructor(bool);
);

class ITEM
(
  groin,
  bodypart,
 public:
  virtual int GetTotalResistance(int) const;
  virtual int GetBodyPartIndex() const;
  virtual bool DamageArmor(character*, int, int);
  virtual int GetSpecialFlags() const;
  virtual item* GetArmorToReceiveFluid(bool) const;
  void UpdateGroinArmorPictures(graphicdata&) const;
);

class ABSTRACT_ITEM
(
  leg,
  bodypart,
 public:
  leg(const leg&);
  virtual ~leg();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetTotalResistance(int) const;
  void SetBoot(item* What) { BootSlot.PutInItem(What); }
  item* GetBoot() const { return *BootSlot; }
  virtual void DropEquipment(stack* = 0);
  double GetKickToHitValue() const { return KickToHitValue; }
  double GetKickDamage() const { return KickDamage; }
  int GetKickMinDamage() const;
  int GetKickMaxDamage() const;
  int GetAttribute(int) const;
  bool EditAttribute(int, int);
  void EditExperience(int, double, double);
  //virtual bool ApplyExperience();
  /*void SetStrength(int What) { Strength = What; }
  void SetAgility(int What) { Agility = What; }
  int GetAgility() const { return Agility; }
  int GetStrength() const { return Strength; }*/
  virtual void InitSpecialAttributes();
  virtual void Mutate();
  long GetKickAPCost() const { return KickAPCost; }
  virtual item* GetEquipment(int) const;
  virtual int GetEquipmentSlots() const { return 1; }
  long GetBaseKickStrength() const { return BaseKickStrength; }
  void SetBaseKickStrength(long What) { BaseKickStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  void ApplyEquipmentAttributeBonuses(item*);
  virtual void CalculateAttributeBonuses();
  virtual void SignalEquipmentAdd(gearslot*);
  void ApplyAgilityPenalty(item*);
  virtual void SignalVolumeAndWeightChange();
  virtual bool DamageArmor(character*, int, int);
  virtual bool EditAllAttributes(int);
  void AddAttackInfo(felist&) const;
  virtual item* GetArmorToReceiveFluid(bool) const;
  virtual void CopyAttributes(const bodypart*);
  double GetStrengthExperience() const { return StrengthExperience; }
  double GetAgilityExperience() const { return AgilityExperience; }
 protected:
  virtual void VirtualConstructor(bool);
  void UpdateLegArmorPictures(graphicdata&, graphicdata&, int) const;
  gearslot BootSlot;
  /*int Strength;
  int Agility;*/
  double StrengthExperience;
  double AgilityExperience;
  int BaseKickStrength;
  double KickDamage;
  double KickToHitValue;
  long KickAPCost;
  int StrengthBonus;
  int AgilityBonus;
);

class ITEM
(
  rightleg,
  leg,
 public:
  rightleg(const rightleg&);
  virtual int GetBodyPartIndex() const;
  virtual int GetSpecialFlags() const;
 protected:
  virtual void VirtualConstructor(bool);
);

class ITEM
(
  leftleg,
  leg,
 public:
  leftleg(const leftleg&);
  virtual int GetBodyPartIndex() const;
  virtual int GetSpecialFlags() const;
 protected:
  virtual void VirtualConstructor(bool);
);

class ITEM
(
  corpse,
  item,
 public:
  corpse(const corpse&);
  virtual ~corpse();
  virtual int GetOfferValue(int) const;
  virtual double GetWeaponStrength() const;
  virtual bool CanBeEatenByAI(const character*) const;
  virtual int GetStrengthValue() const;
  character* GetDeceased() const { return Deceased; }
  void SetDeceased(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsDestroyable(const character*) const;
  virtual long GetTruePrice() const;
  virtual int GetMaterials() const { return 2; }
  virtual bool RaiseTheDead(character*);
  virtual void CalculateVolumeAndWeight();
  virtual void CalculateEmitation();
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual int GetSpoilLevel() const;
  virtual material* GetMaterial(int) const;
  virtual head* Behead();
  virtual bool CanBeCloned() const;
  virtual int GetAttachedGod() const;
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone();
  virtual bool SuckSoul(character*, character* = 0);
  virtual character* TryNecromancy(character*);
  virtual void Cannibalize();
  virtual material* GetConsumeMaterial(const character*, materialpredicate) const;
  virtual bool DetectMaterial(const material*) const;
  virtual void SetLifeExpectancy(int, int);
  virtual void Be();
  virtual void SignalDisappearance();
  virtual bool IsValuable() const;
  virtual bool AddRustLevelDescription(festring&, bool) const { return false; }
  virtual bool Necromancy(character*);
 protected:
  virtual bool IsSparkling(int) const;
  virtual void GenerateMaterials() { }
  virtual color16 GetMaterialColorA(int) const;
  virtual color16 GetMaterialColorB(int) const;
  virtual alpha GetAlphaA(int) const;
  virtual alpha GetAlphaB(int) const;
  virtual bool ShowMaterial() const { return false; }
  virtual void AddPostFix(festring&) const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetSize() const;
  virtual int GetArticleMode() const;
  virtual int GetRustDataA() const;
  character* Deceased;
);

class ITEM
(
  eddytorso,
  normaltorso,
 protected:
  virtual int GetClassAnimationFrames() const { return 8; }
  virtual vector2d GetBitmapPos(int) const;
);

class ITEM
(
  largetorso,
  normaltorso,
 public:
  virtual void SignalStackAdd(stackslot*, void (stack::*)(item*));
  virtual int GetSquareIndex(vector2d) const;
  virtual void Draw(bitmap*, vector2d, color24, int, bool, bool) const;
  virtual void CalculateSquaresUnder() { SquaresUnder = 4; }
 protected:
  virtual vector2d GetBitmapPos(int I) const { return GetLargeBitmapPos(BitmapPos, I); }
  virtual void ModifyAnimationFrames(int& AF) const { AF <<= 2; }
);

class ITEM
(
  largecorpse,
  corpse,
 public:
  virtual void SignalStackAdd(stackslot*, void (stack::*)(item*));
  virtual int GetSquareIndex(vector2d) const;
  virtual void Draw(bitmap*, vector2d, color24, int, bool, bool) const;
  virtual void CalculateSquaresUnder() { SquaresUnder = 4; }
 protected:
  virtual vector2d GetBitmapPos(int I) const { return GetLargeBitmapPos(item::GetBitmapPos(I), I); }
  virtual void ModifyAnimationFrames(int& AF) const { AF <<= 2; }
);

class ITEM
(
  ennerhead,
  head,
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual vector2d GetBitmapPos(int) const;
);

class ITEM
(
  playerkindhead,
  head,
 public:
  playerkindhead(const playerkindhead& Head) : head(Head) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool UpdateArmorPictures();
  virtual void DrawArmor(bitmap*, vector2d, color24, bool) const;
  virtual bool ShowFluids() const { return true; }
 protected:
  graphicdata HelmetGraphicData;
);

class ITEM
(
  playerkindtorso,
  humanoidtorso,
 public:
  playerkindtorso(const playerkindtorso& Torso) : humanoidtorso(Torso) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool UpdateArmorPictures();
  virtual void DrawArmor(bitmap*, vector2d, color24, bool) const;
  virtual void SignalVolumeAndWeightChange();
  virtual bool ShowFluids() const { return true; }
 protected:
  graphicdata TorsoArmorGraphicData;
  graphicdata CloakGraphicData;
  graphicdata BeltGraphicData;
);

class ITEM
(
  playerkindrightarm,
  rightarm,
 public:
  playerkindrightarm(const playerkindrightarm& Arm) : rightarm(Arm) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool UpdateArmorPictures();
  virtual void DrawArmor(bitmap*, vector2d, color24, bool) const;
  virtual bool ShowFluids() const { return true; }
 protected:
  graphicdata ArmArmorGraphicData;
  graphicdata GauntletGraphicData;
);

class ITEM
(
  playerkindleftarm,
  leftarm,
 public:
  playerkindleftarm(const playerkindleftarm& Arm) : leftarm(Arm) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool UpdateArmorPictures();
  virtual void DrawArmor(bitmap*, vector2d, color24, bool) const;
  virtual bool ShowFluids() const { return true; }
 protected:
  graphicdata ArmArmorGraphicData;
  graphicdata GauntletGraphicData;
);

class ITEM
(
  playerkindgroin,
  groin,
 public:
  playerkindgroin(const playerkindgroin& Groin) : groin(Groin) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool UpdateArmorPictures();
  virtual void DrawArmor(bitmap*, vector2d, color24, bool) const;
  virtual bool ShowFluids() const { return true; }
 protected:
  graphicdata GroinArmorGraphicData;
);

class ITEM
(
  playerkindrightleg,
  rightleg,
 public:
  playerkindrightleg(const playerkindrightleg& Leg) : rightleg(Leg) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool UpdateArmorPictures();
  virtual void DrawArmor(bitmap*, vector2d, color24, bool) const;
  virtual bool ShowFluids() const { return true; }
 protected:
  graphicdata LegArmorGraphicData;
  graphicdata BootGraphicData;
);

class ITEM
(
  playerkindleftleg,
  leftleg,
 public:
  playerkindleftleg(const playerkindleftleg& Leg) : leftleg(Leg) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool UpdateArmorPictures();
  virtual void DrawArmor(bitmap*, vector2d, color24, bool) const;
  virtual bool ShowFluids() const { return true; }
 protected:
  graphicdata LegArmorGraphicData;
  graphicdata BootGraphicData;
);

class ITEM
(
  magicmushroomtorso,
  normaltorso,
 protected:
  virtual int GetClassAnimationFrames() const { return 64; }
  virtual vector2d GetBitmapPos(int) const;
);

class ITEM
(
  dogtorso,
  normaltorso,
 protected:
  virtual void Draw(bitmap*, vector2d, color24, int, bool, bool) const;
  virtual int GetClassAnimationFrames() const { return 16; }
  virtual vector2d GetBitmapPos(int) const;
);

class ITEM
(
  blinkdogtorso,
  dogtorso,
 protected:
  virtual alpha GetAlphaA(int) const;
  virtual int GetClassAnimationFrames() const { return 64; }
);

#endif
