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
  virtual ushort GetTotalResistance(ushort) const = 0;
  virtual bool ReceiveDamage(character*, ushort, ushort);
  const festring& GetOwnerDescription() const { return OwnerDescription; }
  void SetOwnerDescription(const festring& What) { OwnerDescription = What; }
  bool IsUnique() const { return Unique; }
  void SetIsUnique(bool What) { Unique = What; }
  virtual void DropEquipment() { }
  virtual material* GetConsumeMaterial() const { return MainMaterial; }
  virtual void SetConsumeMaterial(material*, ushort = 0);
  virtual void ChangeConsumeMaterial(material*, ushort = 0);
  virtual bool ApplyExperience() { return false; }
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
  const festring& GetBodyPartName() const { return GetNameSingular(); }
  void RandomizePosition();
  void ResetPosition() { SpecialFlags &= ~0x7; }
  virtual void SignalSpoil(material*);
  virtual bool IsVeryCloseToSpoiling() const;
  virtual bool CanBePiledWith(const item*, const character*) const;
  bool IsAlive() const;
  virtual ulong GetTruePrice() const;
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
  virtual bool DamageArmor(character*, ushort, ushort) { return false; }
  bool CannotBeSevered(ushort);
  virtual bool IsDipDestination(const character*) const;
  virtual material* CreateDipMaterial();
  virtual bool EditAllAttributes(short) { return false; }
  virtual void Draw(bitmap*, vector2d, ulong, bool, bool) const;
  void SetIsSparklingB(bool What) { IsSparklingB = What; }
  void SetIsSparklingC(bool What) { IsSparklingC = What; }
  void SetIsSparklingD(bool What) { IsSparklingD = What; }
  virtual ushort GetSpecialFlags() const;
  bool IsRepairable() const;
  bool IsWarm() const;
  bool UseMaterialAttributes() const;
  bool CanRegenerate() const;
 protected:
  virtual bool IsSparkling(ushort) const;
  virtual uchar GetMaxAlpha() const;
  virtual void GenerateMaterials() { }
  virtual void VirtualConstructor(bool);
  virtual void AddPostFix(festring&) const;
  virtual bool ShowMaterial() const { return false; }
  virtual uchar GetArticleMode() const;
  virtual ushort GetMaterialColorA(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const { return ColorB; }
  virtual ushort GetMaterialColorC(ushort) const { return ColorC; }
  virtual ushort GetMaterialColorD(ushort) const { return ColorD; }
  virtual vector2d GetBitmapPos(ushort) const { return BitmapPos; }
  festring OwnerDescription;
  vector2d BitmapPos;
  ushort ColorB;
  ushort ColorC;
  ushort ColorD;
  ushort SpecialFlags;
  short HP;
  short MaxHP;
  bool Unique;
  ulong CarriedWeight;
  ulong BodyPartVolume;
  ushort BloodColor;
  character* Master;
  bool IsSparklingB;
  bool IsSparklingC;
  bool IsSparklingD;
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
  virtual ushort GetTotalResistance(ushort) const;
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
  virtual bool DamageArmor(character*, ushort, ushort);
  virtual head* Behead();
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
  virtual ushort GetTotalResistance(ushort) const;
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
  virtual ushort GetTotalResistance(ushort) const;
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
  virtual bool DamageArmor(character*, ushort, ushort);
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
  virtual ushort GetTotalResistance(ushort) const;
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
  void EditExperience(ushort, long, bool = true);
  virtual bool ApplyExperience();
  void SetStrength(ushort What) { Strength = What; }
  void SetDexterity(ushort What) { Dexterity = What; }
  virtual void InitSpecialAttributes();
  virtual void Mutate();
  ushort GetDexterity() const { return Dexterity; }
  ushort GetStrength() const { return Strength; }
  virtual arm* GetPairArm() const = 0;
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
  virtual void SignalVolumeAndWeightChange();
  bool TwoHandWieldIsActive() const;
  float GetBlockChance(float) const;
  ushort GetBlockCapability() const;
  void WieldedSkillHit();
  float GetBlockValue() const;
  void ApplyEquipmentAttributeBonuses(item*);
  virtual void CalculateAttributeBonuses();
  short GetWieldedHitStrength() const;
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalEquipmentRemoval(gearslot*);
  void ApplyDexterityPenalty(item*);
  virtual bool DamageArmor(character*, ushort, ushort);
  bool CheckIfWeaponTooHeavy(const char*) const;
  virtual bool EditAllAttributes(short);
  void AddAttackInfo(felist&) const;
  void AddDefenceInfo(felist&) const;
  void UpdateWieldedPicture();
  void DrawWielded(bitmap*, vector2d, ulong, bool) const;
  virtual bool IsRightArm() const = 0;
  virtual void UpdatePictures();
  virtual ushort GetAnimationFrames() const;
  virtual float GetTypeDamage(const character*) const;
 protected:
  virtual sweaponskill*& GetCurrentSWeaponSkill() const = 0;
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
  ushort WieldedAnimationFrames;
  bitmap** WieldedPicture;
  tilemap::iterator* WieldedGraphicIterator;
);

class ITEM
(
  rightarm,
  arm,
 public:
  rightarm(const rightarm&);
  virtual uchar GetBodyPartIndex() const;
  virtual arm* GetPairArm() const;
  virtual bool IsRightArm() const { return true; }
  virtual ushort GetSpecialFlags() const;
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
  virtual uchar GetBodyPartIndex() const;
  virtual arm* GetPairArm() const;
  virtual bool IsRightArm() const { return false; }
  virtual ushort GetSpecialFlags() const;
 protected:
  virtual sweaponskill*& GetCurrentSWeaponSkill() const;
  virtual void VirtualConstructor(bool);
);

class ITEM
(
  groin,
  bodypart,
 public:
  virtual ushort GetTotalResistance(ushort) const;
  virtual uchar GetBodyPartIndex() const;
  virtual bool DamageArmor(character*, ushort, ushort);
  virtual ushort GetSpecialFlags() const;
 protected:
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
  virtual ushort GetTotalResistance(ushort) const;
  void SetBoot(item* What) { BootSlot.PutInItem(What); }
  item* GetBoot() const { return *BootSlot; }
  virtual void DropEquipment();
  float GetKickToHitValue() const { return KickToHitValue; }
  float GetKickDamage() const { return KickDamage; }
  ushort GetKickMinDamage() const;
  ushort GetKickMaxDamage() const;
  ushort GetAttribute(ushort) const;
  bool EditAttribute(ushort, short);
  void EditExperience(ushort, long, bool = true);
  virtual bool ApplyExperience();
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
  virtual bool DamageArmor(character*, ushort, ushort);
  virtual bool EditAllAttributes(short);
  void AddAttackInfo(felist&) const;
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
  virtual ushort GetSpecialFlags() const;
 protected:
  virtual void VirtualConstructor(bool);
);

class ITEM
(
  leftleg,
  leg,
 public:
  leftleg(const leftleg&);
  virtual uchar GetBodyPartIndex() const;
  virtual ushort GetSpecialFlags() const;
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
  virtual bool IsConsumable(const character*) const;
  virtual short GetOfferValue(uchar) const;
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
  virtual ulong GetTruePrice() const;
  virtual ushort GetMaterials() const { return 2; }
  virtual bool RaiseTheDead(character*);
  virtual const char* GetConsumeVerb() const;
  virtual bool IsEatable(const character* Eater) const { return IsConsumable(Eater); }
  virtual bool IsDrinkable(const character*) const { return false; }
  virtual void CalculateVolumeAndWeight();
  virtual void CalculateEmitation();
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual uchar GetSpoilLevel() const;
  virtual bool Consume(character*, long);
  virtual material* GetMaterial(ushort) const;
  virtual bool IsStupidToConsume() const;
  virtual head* Behead();
  virtual bool CanBeCloned() const;
  virtual uchar GetAttachedGod() const;
  virtual bool IsDipDestination(const character*) const;
  virtual material* CreateDipMaterial();
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone();
  virtual bool SuckSoul(character*, character* = 0);
 protected:
  virtual bool IsSparkling(ushort) const;
  virtual void GenerateMaterials() { }
  virtual ushort GetMaterialColorA(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaA(ushort) const;
  virtual uchar GetAlphaB(ushort) const;
  virtual bool ShowMaterial() const { return false; }
  virtual void AddPostFix(festring&) const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual ushort GetSize() const;
  virtual uchar GetArticleMode() const;
  character* Deceased;
);

class ITEM
(
  eddytorso,
  normaltorso,
 public:
  virtual ushort GetClassAnimationFrames() const { return 8; }
  virtual vector2d GetBitmapPos(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
);

class ITEM
(
  mommotorso,
  normaltorso,
 public:
  virtual ushort GetClassAnimationFrames() const { return 64; }
  virtual vector2d GetBitmapPos(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
);

#endif

