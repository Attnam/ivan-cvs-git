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
  virtual uchar GetGraphicsContainerIndex() const;
  character* GetMaster() const { return Master; }
  humanoid* GetHumanoidMaster() const;
  void SetMaster(character* What) { Master = What; }
  virtual ushort GetStrengthValue() const;
  short GetMaxHP() const { return MaxHP; }
  void SetHP(short);
  short GetHP() const { return HP; }
  void EditHP(short);
  void IncreaseHP() { ++HP; }
  virtual ushort GetTotalResistance(uchar) const = 0;
  virtual bool ReceiveDamage(character*, ushort, uchar);
  const std::string& GetOwnerDescription() const { return OwnerDescription; }
  void SetOwnerDescription(const std::string& What) { OwnerDescription = What; }
  bool IsUnique() const { return Unique; }
  void SetIsUnique(bool What) { Unique = What; }
  virtual void DropEquipment() { }
  virtual material* GetConsumeMaterial() const { return MainMaterial; }
  virtual void SetConsumeMaterial(material*, ushort = 0);
  virtual void ChangeConsumeMaterial(material*, ushort = 0);
  virtual bool ApplyExperience() { return false; }
  virtual void RaiseStats() { }
  virtual void LowerStats() { }
  virtual void InitSpecialAttributes();
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalEquipmentRemoval(gearslot*);
  virtual void Mutate();
  ulong GetBodyPartVolume() const { return BodyPartVolume; }
  ulong GetCarriedWeight() const { return CarriedWeight; }
  virtual item* GetEquipment(ushort) const { return 0; }
  virtual ushort GetEquipmentSlots() const { return 0; }
  virtual void CalculateVolumeAndWeight();
  virtual void CalculateEmitation();
  virtual void CalculateMaxHP(bool = true);
  virtual void SignalVolumeAndWeightChange();
  void RestoreHP() { HP = MaxHP; }
  virtual void CalculateDamage() { }
  virtual void CalculateToHitValue() { }
  virtual void CalculateAPCost() { }
  void CalculateAttackInfo();
  float GetTimeToDie(ushort, float, float, bool, bool) const;
  virtual float GetRoughChanceToHit(float, float) const;
  const std::string& GetBodyPartName() const { return GetNameSingular(); }
  void RandomizePosition();
  void ResetPosition() { SpecialFlags &= ~0x7; }
  virtual void SignalSpoil(material*);
  virtual bool IsVeryCloseToSpoiling() const;
  virtual bool CanBePiledWith(const item*, const character*) const;
  bool IsAlive() const;
  virtual ulong GetPrice() const;
  void SpillBlood(ushort);
  void SpillBlood(ushort, vector2d);
  void SetBloodColor(ushort What) { BloodColor = What; }
  ushort GetBloodColor() const { return BloodColor; }
  virtual void Be();
  bool IsInBadCondition() const { return HP * 3 < MaxHP || (HP == 1 && MaxHP != 1); }
  bool IsInBadCondition(short HP) const { return HP * 3 < MaxHP || (HP == 1 && MaxHP != 1); }
  ushort GetConditionColor() const;
  void SetBitmapPos(vector2d What) { BitmapPos = What; }
  void SetSpecialFlags(uchar What) { SpecialFlags = What; }
  void SetMaterialColorB(ushort What) { ColorB = What; }
  void SetMaterialColorC(ushort What) { ColorC = What; }
  void SetMaterialColorD(ushort What) { ColorD = What; }
  virtual void SignalEnchantmentChange();
  virtual void CalculateAttributeBonuses() { }
  virtual void SignalSpoilLevelChange(material*);
  virtual bool CanBeEatenByAI(const character*) const;
  virtual bool DamageArmor(character*, ushort, uchar) { return false; }
  bool CannotBeSevered(uchar);
 protected:
  virtual bool IsSparkling(ushort) const { return false; }
  virtual uchar GetMaxAlpha(ushort) const;
  virtual void GenerateMaterials() { }
  virtual void VirtualConstructor(bool);
  virtual void AddPostFix(std::string&) const;
  virtual bool ShowMaterial() const { return false; }
  virtual uchar GetArticleMode() const;
  virtual ushort GetMaterialColorA(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const { return ColorB; }
  virtual ushort GetMaterialColorC(ushort) const { return ColorC; }
  virtual ushort GetMaterialColorD(ushort) const { return ColorD; }
  virtual vector2d GetBitmapPos(ushort) const { return BitmapPos; }
  virtual uchar GetSpecialFlags() const { return SpecialFlags; }
  std::string OwnerDescription;
  vector2d BitmapPos;
  ushort ColorB;
  ushort ColorC;
  ushort ColorD;
  uchar SpecialFlags;
  short HP;
  short MaxHP;
  bool Unique;
  ulong CarriedWeight;
  ulong BodyPartVolume;
  ushort BloodColor;
  character* Master;
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
  virtual ushort GetTotalResistance(uchar) const;
  void SetHelmet(item* What) { HelmetSlot.PutInItem(What); }
  item* GetHelmet() const { return *HelmetSlot; }
  void SetAmulet(item* What) { AmuletSlot.PutInItem(What); }
  item* GetAmulet() const { return *AmuletSlot; }
  virtual void DropEquipment();
  virtual uchar GetBodyPartIndex() const;
  float GetBiteDamage() const { return BiteDamage; }
  ushort GetBiteMinDamage() const;
  ushort GetBiteMaxDamage() const;
  float GetBiteToHitValue() const { return BiteToHitValue; }
  long GetBiteAPCost() const { return BiteAPCost; }
  virtual void InitSpecialAttributes();
  virtual item* GetEquipment(ushort) const;
  virtual ushort GetEquipmentSlots() const { return 2; }
  ulong GetBaseBiteStrength() const { return BaseBiteStrength; }
  void SetBaseBiteStrength(ulong What) { BaseBiteStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  void ShowBiteInfo() const;
  virtual bool DamageArmor(character*, ushort, uchar);
 protected:
  virtual void VirtualConstructor(bool);
  gearslot HelmetSlot;
  gearslot AmuletSlot;
  ulong BaseBiteStrength;
  float BiteToHitValue;
  float BiteDamage;
  long BiteAPCost;
);

class ABSTRACT_ITEM
(
  torso,
  bodypart,
 public:
  virtual uchar GetBodyPartIndex() const;
  virtual float GetRoughChanceToHit(float, float) const;
);

class ITEM
(
  normaltorso,
  torso,
 public:
  virtual uchar GetGraphicsContainerIndex() const;
  virtual ushort GetTotalResistance(uchar) const;
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
  virtual ushort GetTotalResistance(uchar) const;
  void SetBodyArmor(item* What) { BodyArmorSlot.PutInItem(What); }
  item* GetBodyArmor() const { return *BodyArmorSlot; }
  void SetCloak(item* What) { CloakSlot.PutInItem(What); }
  item* GetCloak() const { return *CloakSlot; }
  void SetBelt(item* What) { BeltSlot.PutInItem(What); }
  item* GetBelt() const { return *BeltSlot; }
  virtual void DropEquipment();
  virtual item* GetEquipment(ushort) const;
  virtual ushort GetEquipmentSlots() const { return 3; }
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalVolumeAndWeightChange();
  virtual bool DamageArmor(character*, ushort, uchar);
 protected:
  virtual void VirtualConstructor(bool);
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
  virtual ushort GetTotalResistance(uchar) const;
  float GetWieldedDamage() const;
  float GetWieldedToHitValue() const;
  void SetWielded(item* What) { WieldedSlot.PutInItem(What); }
  item* GetWielded() const { return *WieldedSlot; }
  void SetGauntlet(item* What) { GauntletSlot.PutInItem(What); }
  item* GetGauntlet() const { return *GauntletSlot; }
  void SetRing(item* What) { RingSlot.PutInItem(What); }
  item* GetRing() const { return *RingSlot; }
  virtual void DropEquipment();
  float GetUnarmedToHitValue() const;
  float GetUnarmedDamage() const;
  virtual void Hit(character*, bool = false);
  ushort GetAttribute(ushort) const;
  bool EditAttribute(ushort, short);
  void EditExperience(ushort, long);
  virtual bool ApplyExperience();
  virtual void RaiseStats();
  virtual void LowerStats();
  void SetStrength(ushort What) { Strength = What; }
  void SetDexterity(ushort What) { Dexterity = What; }
  virtual void InitSpecialAttributes();
  virtual void Mutate();
  ushort GetDexterity() const { return Dexterity; }
  ushort GetStrength() const { return Strength; }
  virtual arm* GetPairArm() const = 0;
  virtual sweaponskill* GetCurrentSWeaponSkill() const = 0;
  long GetWieldedAPCost() const;
  long GetUnarmedAPCost() const;
  virtual item* GetEquipment(ushort) const;
  virtual ushort GetEquipmentSlots() const { return 3; }
  ulong GetBaseUnarmedStrength() const { return BaseUnarmedStrength; }
  void SetBaseUnarmedStrength(ulong What) { BaseUnarmedStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  float GetDamage() const { return Damage; }
  ushort GetMinDamage() const;
  ushort GetMaxDamage() const;
  float GetToHitValue() const { return ToHitValue; }
  long GetAPCost() const { return APCost; }
  bool PairArmAllowsMelee() const;
  void AddAttackInfo(felist&) const;
  virtual void SignalVolumeAndWeightChange();
  bool TwoHandWieldIsActive() const;
  float GetBlockChance(float) const;
  ushort GetBlockCapability() const;
  void WieldedSkillHit();
  float GetBlockValue() const;
  void AddDefenceInfo(felist&) const;
  void ApplyEquipmentAttributeBonuses(item*);
  virtual void CalculateAttributeBonuses();
  short GetWieldedHitStrength() const;
  virtual void SignalEquipmentAdd(gearslot*);
  void ApplyDexterityPenalty(item*);
  void ShowAttackInfo() const;
  void ShowWieldedAttackInfo() const;
  void ShowDefenceInfo() const;
  void ShowUnarmedInfo() const;
  virtual bool DamageArmor(character*, ushort, uchar);
  bool CheckIfWeaponTooHeavy(const std::string&) const;
 protected:
  virtual void VirtualConstructor(bool);
  gearslot WieldedSlot;
  gearslot GauntletSlot;
  gearslot RingSlot;
  ushort Strength;
  ushort Dexterity;
  long StrengthExperience;
  long DexterityExperience;
  ulong BaseUnarmedStrength;
  float Damage;
  float ToHitValue;
  long APCost;
  short StrengthBonus;
  short DexterityBonus;
);

class ITEM
(
  rightarm,
  arm,
 public:
  rightarm(const rightarm&);
  virtual uchar GetBodyPartIndex() const;
  virtual arm* GetPairArm() const;
  virtual sweaponskill* GetCurrentSWeaponSkill() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual uchar GetSpecialFlags() const;
);

class ITEM
(
  leftarm,
  arm,
 public:
  leftarm(const leftarm&);
  virtual uchar GetBodyPartIndex() const;
  virtual arm* GetPairArm() const;
  virtual sweaponskill* GetCurrentSWeaponSkill() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual uchar GetSpecialFlags() const;
);

class ITEM
(
  groin,
  bodypart,
 public:
  virtual ushort GetTotalResistance(uchar) const;
  virtual uchar GetBodyPartIndex() const;
  virtual bool DamageArmor(character*, ushort, uchar);
 protected:
  virtual uchar GetSpecialFlags() const;
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
  virtual ushort GetTotalResistance(uchar) const;
  void SetBoot(item* What) { BootSlot.PutInItem(What); }
  item* GetBoot() const { return *BootSlot; }
  virtual void DropEquipment();
  float GetKickToHitValue() const { return KickToHitValue; }
  float GetKickDamage() const { return KickDamage; }
  ushort GetKickMinDamage() const;
  ushort GetKickMaxDamage() const;
  ushort GetAttribute(ushort) const;
  bool EditAttribute(ushort, short);
  void EditExperience(ushort, long);
  virtual bool ApplyExperience();
  virtual void RaiseStats();
  virtual void LowerStats();
  void SetStrength(ushort What) { Strength = What; }
  void SetAgility(ushort What) { Agility = What; }
  ushort GetAgility() const { return Agility; }
  ushort GetStrength() const { return Strength; }
  virtual void InitSpecialAttributes();
  virtual void Mutate();
  long GetKickAPCost() const { return KickAPCost; }
  virtual item* GetEquipment(ushort) const;
  virtual ushort GetEquipmentSlots() const { return 1; }
  ulong GetBaseKickStrength() const { return BaseKickStrength; }
  void SetBaseKickStrength(ulong What) { BaseKickStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  void ApplyEquipmentAttributeBonuses(item*);
  virtual void CalculateAttributeBonuses();
  virtual void SignalEquipmentAdd(gearslot*);
  void ApplyAgilityPenalty(item*);
  virtual void SignalVolumeAndWeightChange();
  void ShowKickInfo() const;
  virtual bool DamageArmor(character*, ushort, uchar);
 protected:
  virtual void VirtualConstructor(bool);
  gearslot BootSlot;
  ushort Strength;
  ushort Agility;
  long StrengthExperience;
  long AgilityExperience;
  ulong BaseKickStrength;
  float KickDamage;
  float KickToHitValue;
  long KickAPCost;
  short StrengthBonus;
  short AgilityBonus;
);

class ITEM
(
  rightleg,
  leg,
 public:
  rightleg(const rightleg&);
  virtual uchar GetBodyPartIndex() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual uchar GetSpecialFlags() const;
);

class ITEM
(
  leftleg,
  leg,
 public:
  leftleg(const leftleg&);
  virtual uchar GetBodyPartIndex() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual uchar GetSpecialFlags() const;
);

class ITEM
(
  corpse,
  item,
 public:
  corpse(const corpse&);
  virtual ~corpse();
  virtual bool IsConsumable(const character*) const;
  virtual short GetOfferValue(char) const;
  virtual float GetWeaponStrength() const;
  virtual bool CanBeEatenByAI(const character*) const;
  virtual ushort GetStrengthValue() const;
  virtual void Be() { }
  character* GetDeceased() const { return Deceased; }
  void SetDeceased(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void AddConsumeEndMessage(character*) const;
  virtual void GenerateLeftOvers(character*);
  virtual long GetScore() const;
  virtual bool IsDestroyable() const;
  virtual ulong GetPrice() const;
  virtual ushort GetMaterials() const { return 2; }
  virtual bool RaiseTheDead(character*);
  virtual std::string GetConsumeVerb() const;
  virtual bool IsEatable(const character* Eater) const { return IsConsumable(Eater); }
  virtual bool IsDrinkable(const character*) const { return false; }
  virtual void CalculateVolumeAndWeight();
  virtual void CalculateEmitation();
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual uchar GetFlyAmount() const;
  virtual bool Consume(character*, long);
  virtual material* GetMaterial(ushort) const;
 protected:
  virtual bool IsSparkling(ushort) const { return false; }
  virtual void GenerateMaterials() { }
  virtual ushort GetMaterialColorA(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaA(ushort) const;
  virtual bool ShowMaterial() const { return false; }
  virtual void AddPostFix(std::string&) const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual ushort GetSize() const;
  virtual uchar GetArticleMode() const;
  character* Deceased;
);

#endif