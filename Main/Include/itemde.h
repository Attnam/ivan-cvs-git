#ifndef __ITEMDE_H__
#define __ITEMDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "felist.h"
#include "itemba.h"
#include "materde.h"
#include "wskill.h"
#include "slot.h"

class felist;

class ABSTRACT_ITEM
(
  materialcontainer,
  item,
 public:
  /*virtual void ReceiveHitEffect(character*, character*);
  virtual void DipInto(material*, character*);
  virtual ulong GetPrice() const;
  virtual bool IsDippable(character*) const { return true; }*/
  materialcontainer(const materialcontainer&);
  virtual ~materialcontainer();
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material* What, ushort SpecialFlags = 0) { SetMaterial(ContainedMaterial, What, GetDefaultContainedVolume(), SpecialFlags); }
  virtual void ChangeContainedMaterial(material* What, ushort SpecialFlags = 0) { ChangeMaterial(ContainedMaterial, What, GetDefaultContainedVolume(), SpecialFlags); }
  void InitMaterials(material* M1, material* M2, bool CUP = true) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), ContainedMaterial, M2, GetDefaultContainedVolume(), CUP); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual material* GetConsumeMaterial() const { return ContainedMaterial; }
  virtual void SetConsumeMaterial(material* NewMaterial, ushort SpecialFlags = 0) { SetContainedMaterial(NewMaterial, SpecialFlags); }
  virtual void ChangeConsumeMaterial(material* NewMaterial, ushort SpecialFlags = 0) { ChangeContainedMaterial(NewMaterial, SpecialFlags); }
  virtual uchar GetMaterials() const { return 2; }
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual bool HasContainedMaterial() const { return true; }
  virtual ulong GetPrice() const { return GetContainedMaterial() ? GetContainedMaterial()->GetRawPrice() + item::GetPrice() : item::GetPrice(); }
  virtual void Be();
  virtual uchar GetFlyAmount() const;
 protected:
  virtual bool IsSparkling(ushort) const;
  virtual material*& GetMaterialReference(ushort);
  virtual void GenerateMaterials();
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaB(ushort) const;
  material* ContainedMaterial;
);

class ITEM
(
  lantern,
  item,
 public:
  virtual void SignalSquarePositionChange(uchar);
  virtual void SetSquarePosition(uchar What) { SquarePosition = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual uchar GetAlphaA(ushort) const { return 255; }
  virtual uchar GetSpecialFlags() const;
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBitmapPos(ushort Frame) const { return SquarePosition == CENTER ? item::GetBitmapPos(Frame) : item::GetWallBitmapPos(Frame); }
  uchar SquarePosition;
);

class ITEM
(
  can,
  materialcontainer,
 public:
  virtual item* BetterVersion() const;
  virtual void GenerateLeftOvers(character*);
  virtual bool IsDipDestination(const character*) const { return true; }
  virtual material* CreateDipMaterial();
 protected:
  virtual void AddPostFix(std::string& String) const { AddContainerPostFix(String); }
  virtual bool AddAdjective(std::string& String, bool Articled) const { return AddEmptyAdjective(String, Articled); }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, GetContainedMaterial() ? 288 : 304); }
);

class ITEM
(
  lump,
  item,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
  virtual material* CreateDipMaterial();
  virtual ulong GetPrice() const { return GetMainMaterial()->GetRawPrice() + item::GetPrice(); }
  virtual bool IsDipDestination(const character*) const { return true; }
 protected:
  virtual void AddPostFix(std::string& String) const { AddLumpyPostFix(String); }
  virtual bool ShowMaterial() const { return false; }
);

class ITEM
(
  meleeweapon,
  item,
 public:
  meleeweapon(const meleeweapon&);
  virtual ~meleeweapon();
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
  virtual void DipInto(material*, character*);
  virtual ulong GetPrice() const;
  virtual bool IsDippable(const character*) const { return !ContainedMaterial; }
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material* What, ushort SpecialFlags = 0) { SetMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
  virtual void ChangeSecondaryMaterial(material* What, ushort SpecialFlags = 0) { ChangeMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material* What, ushort SpecialFlags = 0) { SetMaterial(ContainedMaterial, What, GetDefaultContainedVolume(), SpecialFlags); }
  virtual void ChangeContainedMaterial(material* What, ushort SpecialFlags = 0) { ChangeMaterial(ContainedMaterial, What, GetDefaultContainedVolume(), SpecialFlags); }
  void InitMaterials(material* M1, material* M2, material* M3, bool CUP = true) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), SecondaryMaterial, M2, GetDefaultSecondaryVolume(), ContainedMaterial, M3, GetDefaultContainedVolume(), CUP); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetMaterials() const { return 3; }
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual bool HasSecondaryMaterial() const { return true; }
  virtual bool HasContainedMaterial() const { return true; }
  virtual void Be();
  virtual bool IsWeapon(const character*) const { return true; }
  virtual char GetEnchantment() const { return Enchantment; }
  virtual void SetEnchantment(char);
  virtual void EditEnchantment(char);
  virtual float GetWeaponStrength() const;
  virtual ushort GetStrengthValue() const;
  virtual ushort GetEffectBonus() const { return 100 + 5 * Enchantment; }
  virtual ushort GetAPBonus() const { return 2000 / (20 + Enchantment); }
  virtual uchar GetFlyAmount() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual bool IsSparkling(ushort) const;
  virtual void AddPostFix(std::string&) const;
  virtual material*& GetMaterialReference(ushort);
  virtual void GenerateMaterials();
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaB(ushort) const;
  material* SecondaryMaterial;
  material* ContainedMaterial;
  char Enchantment;
);

class ITEM
(
  banana,
  meleeweapon,
 public:
  virtual ulong GetPrice() const { return GetSecondaryMaterial()->GetRawPrice() + item::GetPrice(); }
  virtual bool Zap(character*, vector2d, uchar);
  virtual uchar GetCharges() const { return Charges; }
  virtual void SetCharges(uchar What) { Charges = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { SetCharges(6); }
  virtual bool IsZappable(const character*) const { return true; }
  virtual bool IsChargeable(const character*) const { return true; }
  virtual void GenerateLeftOvers(character*);
  virtual void SignalSpoil(material*);
  virtual material* GetConsumeMaterial() const { return SecondaryMaterial; }
  virtual void SetConsumeMaterial(material* NewMaterial, ushort SpecialFlags = 0) { SetSecondaryMaterial(NewMaterial, SpecialFlags); }
  virtual void ChangeConsumeMaterial(material* NewMaterial, ushort SpecialFlags = 0) { ChangeSecondaryMaterial(NewMaterial, SpecialFlags); }
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
 protected:
  virtual void VirtualConstructor(bool);
  uchar Charges;
);

class ITEM
(
  holybanana,
  banana,
 public:
  virtual uchar GetSpecialFlags() const { return ST_FLAME; }
  virtual ulong GetPrice() const { return item::GetPrice(); }
);

class ITEM
(
  justifier,
  meleeweapon,
 protected:
  virtual void Be() { }
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual ushort GetOutlineColor(ushort) const;
);

class ITEM
(
  neercseulb,
  meleeweapon,
 protected:
  virtual void Be() { }
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual ushort GetOutlineColor(ushort) const;
);

class ITEM
(
  pickaxe,
  meleeweapon,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const;
  ;
);

class ABSTRACT_ITEM
(
  armor,
  item,
 public:
  virtual ulong GetPrice() const;
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsWeapon(const character*) const { return true; }
  virtual bool IsArmor(const character*) const { return true; }
  virtual char GetEnchantment() const { return Enchantment; }
  virtual void SetEnchantment(char);
  virtual void EditEnchantment(char);
  virtual ushort GetStrengthValue() const;
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual ushort GetInElasticityPenalty(ushort) const;
 protected:
  virtual void AddPostFix(std::string&) const;
  virtual void VirtualConstructor(bool);
  char Enchantment;
);

class ITEM
(
  bodyarmor,
  armor,
 public:
  virtual ulong GetPrice() const { return (armor::GetPrice() << 2) + GetEnchantedPrice(Enchantment); }
  virtual bool IsBodyArmor(const character*) const { return true; }
 protected:
  virtual const std::string& GetNameSingular() const { return GetMainMaterial()->GetFlexibility() >= 5 ? item::GetFlexibleNameSingular() : item::GetNameSingular(); }
);

class ITEM
(
  goldeneagleshirt,
  bodyarmor,
 public:
  virtual void Be() { }
  virtual bool IsGoldenEagleShirt() const { return true; }
  virtual bool IsConsumable(const character*) const { return false; }
 protected:
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual ushort GetOutlineColor(ushort) const;
);

class ITEM
(
  potion,
  materialcontainer,
 public:
  virtual item* BetterVersion() const;
  virtual void DipInto(material*, character*);
  virtual material* CreateDipMaterial();
  virtual bool IsDippable(const character*) const { return !ContainedMaterial; }
  virtual void GenerateLeftOvers(character*);
  virtual void Break();
  virtual bool IsDipDestination(const character*) const { return true; }
 protected:
  virtual void AddPostFix(std::string& String) const { AddContainerPostFix(String); }
  virtual bool AddAdjective(std::string& String, bool Articled) const { return AddEmptyAdjective(String, Articled); }
);

class ITEM
(
  bananapeels,
  item,
 public:
  virtual item* BetterVersion() const { return new banana; }
  virtual void StepOnEffect(character*);
);

class ITEM
(
  brokenbottle,
  item,
 public:
  virtual item* BetterVersion() const;
  virtual void StepOnEffect(character*);
);

class ABSTRACT_ITEM
(
  scroll,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual bool IsReadable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, ushort, uchar);
);

class ITEM
(
  scrollofcreatemonster,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  scrollofteleportation,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  scrollofcharging,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  nut,
  item,
  ;
);

class ITEM
(
  leftnutofpetrus,
  nut,
 public:
  virtual void Be() { }
  virtual bool IsPetrussNut() const { return true; }
  virtual bool IsConsumable(const character*) const { return false; }
);

class ITEM
(
  bone,
  item,
 public:
  virtual bool DogWillCatchAndConsume() const { return GetConsumeMaterial()->GetConfig() == BONE; }
);

class ITEM
(
  loaf,
  item,
 public:
  virtual ulong GetPrice() const { return GetMainMaterial()->GetRawPrice() + item::GetPrice(); }
 protected:
  virtual void AddPostFix(std::string& String) const { AddLumpyPostFix(String); }
  virtual bool ShowMaterial() const { return false; }
);

class ITEM
(
  scrollofwishing,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  cheapcopyofleftnutofpetrus,
  nut,
  ;
);

class ABSTRACT_ITEM
(
  wand,
  item,
 public:
  virtual bool Apply(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetCharges() const { return Charges; }
  virtual void SetCharges(uchar What) { Charges = What; }
  virtual uchar GetTimesUsed() const { return TimesUsed; }
  virtual void SetTimesUsed(uchar What) { TimesUsed = What; }
  virtual void Beam(character*, const std::string&, uchar, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*) { return false; }
  virtual void ChargeFully(character*) { SetTimesUsed(0); }
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool IsZappable(const character*) const { return true; }
  virtual bool IsChargeable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, ushort, uchar);
  virtual bool Zap(character*, vector2d, uchar);
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
  virtual bool IsExplosive() const { return true; }
  virtual ulong GetTotalExplosivePower() const { return 40; }
 protected:
  virtual ushort GetBeamColor() const { return WHITE; }
  virtual void VirtualConstructor(bool);
  uchar Charges;
  uchar TimesUsed;
);

class ITEM
(
  wandofpolymorph,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return BLUE; }
);

class ITEM
(
  scrollofchangematerial,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  avatarofvalpurus,
  item,
 public:
  virtual void Be() { }
  virtual bool IsTheAvatar() const { return true; }
  virtual bool IsConsumable(const character*) const { return false; }
);

class ITEM
(
  wandofstriking,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return WHITE; }
);

class ITEM
(
  kiwi,
  item,
 public:
  virtual ulong GetPrice() const { return GetMainMaterial()->GetRawPrice() + item::GetPrice(); }
);

class ITEM
(
  pineapple,
  item,
 public:
  virtual ulong GetPrice() const { return GetMainMaterial()->GetRawPrice() + item::GetPrice(); }
);

class ITEM
(
  palmbranch,
  item,
 public:
  virtual bool IsShield(const character*) const { return true; }
);

class ITEM
(
  whip,
  meleeweapon,
 public:
  virtual bool IsWhip() const { return true; }
 protected:
  virtual ushort GetFormModifier() const;
);

class ITEM
(
  backpack,
  materialcontainer,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, ushort, uchar);
  virtual bool IsExplosive() const { return GetContainedMaterial() && GetContainedMaterial()->IsExplosive(); }
  virtual ulong GetTotalExplosivePower() const { return GetContainedMaterial() ? GetContainedMaterial()->GetTotalExplosivePower() : 0; }
  virtual void ReceiveFluidSpill(material*);
 protected:
  virtual void AddPostFix(std::string& String) const { AddContainerPostFix(String); }
);

class ITEM
(
  holybook,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual bool Read(character*);
  virtual bool IsReadable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, ushort, uchar);
  virtual void FinishReading(character*);
 protected:
  virtual ushort GetMaterialColorA(ushort) const;
  virtual void AddPostFix(std::string& String) const { AddDivineMasterDescription(String, GetConfig()); }
  virtual bool ShowMaterial() const { return false; }
);

class ITEM
(
  fiftymillionroubles,
  item,
  ;
);

class ITEM
(
  oillamp,
  item,
 public:
  oillamp(const oillamp&);
  virtual ulong GetPrice() const { return GetMainMaterial()->GetRawPrice() + item::GetPrice(); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GetInhabitedByGenie() const { return InhabitedByGenie; }
  virtual void SetInhabitedByGenie(bool What) { InhabitedByGenie = What; }
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
 protected:
  virtual void VirtualConstructor(bool);
  bool InhabitedByGenie;
);

class ITEM
(
  stone,
  item,
 public:
  virtual ulong GetPrice() const { return GetMainMaterial()->GetRawPrice() * 2 + item::GetPrice(); }
);

class ITEM
(
  wandoffireballs,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return YELLOW; }
);

class ITEM
(
  scrolloftaming,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  wandofteleportation,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return GREEN; }
);

class ITEM
(
  mine,
  materialcontainer,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void StepOnEffect(character*);
  virtual bool ReceiveDamage(character*, ushort, uchar);
  virtual bool IsActive() const { return Active; }
  virtual void SetIsActive(bool What) { Active = What; }
  virtual bool CanBeSeenBy(const character*) const;
  virtual bool Apply(character* User);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool DangerousToStepOn(const character* Stepper) const { return WillExplode(Stepper); } 
  virtual bool WillExplode(const character*) const;
  virtual ushort GetTeam() const { return Team; }
  virtual void SetTeam(ushort What) { Team = What; }
  virtual ulong GetPrice() const;
 protected:
  virtual void VirtualConstructor(bool);
  bool Active;
  ushort Team;
);

class ITEM
(
  wandofhaste,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return RED; }
);

class ITEM
(
  wandofslow,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return GREEN; }
);

class ITEM
(
  key,
  item,
 public:
  virtual bool Apply(character*);
  virtual void SetLockType(uchar What) { LockType = What; }
  virtual uchar GetLockType() const { return LockType; }
  virtual bool IsAppliable(const character*) const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool CanOpenDoors() const { return true; }
  virtual bool CanOpenLockType(uchar AnotherLockType) const { return LockType == AnotherLockType; }
  virtual bool CanBePiledWith(const item*, const character*) const;
 protected:
  virtual const std::string& GetAdjective() const;
  virtual void VirtualConstructor(bool);
  uchar LockType;
);

class ITEM
(
  shield,
  armor,
 public:
  virtual ulong GetPrice() const;
  virtual bool IsShield(const character*) const { return true; }
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
  virtual ushort GetEffectBonus() const { return 100 + 10 * Enchantment; }
);

class ITEM
(
  cloak,
  armor,
 public:
  virtual ulong GetPrice() const { return armor::GetPrice() * 20 + GetEnchantedPrice(Enchantment); }
  virtual bool IsCloak(const character*) const { return true; }
 protected:
  virtual ushort GetMaterialColorB(ushort) const { return MakeRGB16(111, 64, 37); }
);

class ITEM
(
  boot,
  armor,
 public:
  virtual ulong GetPrice() const { return armor::GetPrice() / 5 + GetEnchantedPrice(Enchantment); }
  virtual bool IsBoot(const character*) const { return true; }
  virtual void SpecialGenerationHandler() { GetSlot()->AddFriendItem(Duplicate()); }
);

class ITEM
(
  gauntlet, 
  armor,
 public:
  virtual ulong GetPrice() const { return armor::GetPrice() / 3 + GetEnchantedPrice(Enchantment); }
  virtual bool IsGauntlet(const character*) const { return true; }
  virtual void SpecialGenerationHandler() { GetSlot()->AddFriendItem(Duplicate()); }
);

class ITEM
(
  belt, 
  armor,
 public:
  virtual ulong GetPrice() const { return armor::GetPrice() * 5 + GetEnchantedPrice(Enchantment); }
  virtual bool IsBelt(const character*) const { return true; }
  virtual ushort GetFormModifier() const; 
);

class ITEM
(
  ring, 
  item,
 public:
  virtual bool IsRing(const character*) const { return true; }
 protected:
  virtual ushort GetMaterialColorB(ushort) const { return MakeRGB16(200, 200, 200); }
);

class ITEM
(
  amulet, 
  item,
 public:
  virtual bool IsAmulet(const character*) const { return true; }
 protected:
  virtual ushort GetMaterialColorB(ushort) const { return MakeRGB16(111, 64, 37); }
);

class ABSTRACT_ITEM
(
  bodypart,
  materialcontainer,
 public:
  friend class corpse;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetGraphicsContainerIndex() const { return GR_HUMANOID; }
  character* GetMaster() const { return Master; }
  humanoid* GetHumanoidMaster() const;
  void SetMaster(character* What) { Master = What; }
  virtual ushort GetStrengthValue() const;
  short GetMaxHP() const { return MaxHP; }
  void SetHP(short);
  short GetHP() const { return HP; }
  void EditHP(short);
  void IncHP() { ++HP; }
  virtual ushort GetTotalResistance(uchar) const = 0;
  virtual bool ReceiveDamage(character*, ushort, uchar);
  const std::string& GetOwnerDescription() const { return OwnerDescription; }
  void SetOwnerDescription(const std::string& What) { OwnerDescription = What; }
  bool GetUnique() const { return Unique; }
  void SetUnique(bool What) { Unique = What; }
  virtual void DropEquipment() { }
  virtual material* GetConsumeMaterial() const { return MainMaterial; }
  virtual void SetConsumeMaterial(material* NewMaterial, ushort SpecialFlags = 0) { SetMainMaterial(NewMaterial, SpecialFlags); }
  virtual void ChangeConsumeMaterial(material* NewMaterial, ushort SpecialFlags = 0) { ChangeMainMaterial(NewMaterial, SpecialFlags); }
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
  virtual void CalculateMaxHP();
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
  bool IsAlive() const { return MainMaterial->IsAlive(); }
  virtual ulong GetPrice() const { return GetMainMaterial()->GetRawPrice() / 100 + item::GetPrice(); }
  void SpillBlood(ushort);
  void SpillBlood(ushort, vector2d);
  void SetBloodColor(ushort What) { BloodColor = What; }
  ushort GetBloodColor() const { return BloodColor; }
  virtual void Be();
  bool IsInBadCondition() const { return HP * 3 < MaxHP || (HP == 1 && MaxHP != 1); }
  bool IsInBadCondition(short HP) const { return HP * 3 < MaxHP || (HP == 1 && MaxHP != 1); }
  void SetBitmapPos(vector2d What) { BitmapPos = What; }
  void SetSpecialFlags(uchar What) { SpecialFlags = What; }
  void SetMaterialColorB(ushort What) { ColorB = What; }
  void SetMaterialColorC(ushort What) { ColorC = What; }
  void SetMaterialColorD(ushort What) { ColorD = What; }
  virtual void SignalEnchantmentChange();
  virtual void CalculateAttributeBonuses() { }
  virtual void SignalSpoilLevelChange(material*);
 protected:
  virtual bool IsSparkling(ushort) const { return false; }
  virtual uchar GetMaxAlpha(ushort) const;
  virtual void GenerateMaterials() { }
  virtual void VirtualConstructor(bool);
  virtual void AddPostFix(std::string&) const;
  virtual bool ShowMaterial() const { return false; }
  virtual uchar GetArticleMode() const { return Unique ? DEFINITE_ARTICLE : NORMAL_ARTICLE; }
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
  virtual uchar GetBodyPartIndex() const { return HEAD_INDEX; }
  float GetBiteDamage() const { return BiteDamage; }
  ushort GetBiteMinDamage() const { return ushort(BiteDamage * 0.75f); }
  ushort GetBiteMaxDamage() const { return ushort(BiteDamage * 1.25f + 1); }
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
  virtual uchar GetBodyPartIndex() const { return TORSO_INDEX; }
  virtual float GetRoughChanceToHit(float, float) const;
);

class ITEM
(
  normaltorso,
  torso,
 public:
  virtual uchar GetGraphicsContainerIndex() const { return GR_CHARACTER; }
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
  virtual uchar GetGraphicsContainerIndex() const { return GR_HUMANOID; }
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
  virtual void Hit(character*);
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
  ushort GetMinDamage() const { return ushort(Damage * 0.75f); }
  ushort GetMaxDamage() const { return ushort(Damage * 1.25f + 1); }
  float GetToHitValue() const { return ToHitValue; }
  long GetAPCost() const { return APCost; }
  bool PairArmAllowsMelee() const;
  void AddAttackInfo(felist&) const;
  virtual void SignalVolumeAndWeightChange();
  bool TwoHandWieldIsActive() const;
  float GetBlockChance(float) const;
  ushort GetBlockCapability() const;
  void WieldedSkillHit();
  float GetBlockValue() const { return GetToHitValue() * GetWielded()->GetBlockModifier() / 10000; }
  void AddDefenceInfo(felist&) const;
  void ApplyEquipmentAttributeBonuses(item*);
  virtual void CalculateAttributeBonuses();
  short GetWieldedHitStrength() const;
  void AddWieldedBattleInfo(felist&) const;
  virtual void SignalEquipmentAdd(gearslot*);
  void ApplyDexterityPenalty(item*);
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
  virtual uchar GetBodyPartIndex() const { return RIGHT_ARM_INDEX; }
  virtual arm* GetPairArm() const;
  virtual sweaponskill* GetCurrentSWeaponSkill() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual uchar GetSpecialFlags() const { return SpecialFlags|ST_RIGHT_ARM; }
);

class ITEM
(
  leftarm,
  arm,
 public:
  leftarm(const leftarm&);
  virtual uchar GetBodyPartIndex() const { return  LEFT_ARM_INDEX; }
  virtual arm* GetPairArm() const;
  virtual sweaponskill* GetCurrentSWeaponSkill() const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual uchar GetSpecialFlags() const { return SpecialFlags|ST_LEFT_ARM; }
);

class ITEM
(
  groin,
  bodypart,
 public:
  virtual ushort GetTotalResistance(uchar) const;
  virtual uchar GetBodyPartIndex() const { return GROIN_INDEX; }
 protected:
  virtual uchar GetSpecialFlags() const { return SpecialFlags|ST_GROIN; }
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
  ushort GetKickMinDamage() const { return ushort(KickDamage * 0.75f); }
  ushort GetKickMaxDamage() const { return ushort(KickDamage * 1.25f + 1); }
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
  virtual uchar GetBodyPartIndex() const { return RIGHT_LEG_INDEX; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual uchar GetSpecialFlags() const { return SpecialFlags|ST_RIGHT_LEG; }
);

class ITEM
(
  leftleg,
  leg,
 public:
  leftleg(const leftleg&);
  virtual uchar GetBodyPartIndex() const { return LEFT_LEG_INDEX; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual uchar GetSpecialFlags() const { return SpecialFlags|ST_LEFT_LEG; }
);

class ITEM
(
  headofelpuri,
  item, // can't wear equipment, so not "head"
 public:
  virtual bool IsHeadOfElpuri() const { return true; }
  virtual bool IsConsumable(const character*) const { return false; }
  virtual void Be() { }
);

class ITEM
(
  corpse,
  item,
 public:
  corpse(const corpse&);
  virtual ~corpse();
  virtual bool Consume(character*, long);
  virtual bool IsConsumable(const character*) const;
  virtual short GetOfferValue(char) const;
  virtual float GetWeaponStrength() const;
  virtual bool IsBadFoodForAI(character*) const;
  virtual ushort GetStrengthValue() const;
  virtual void Be() { }
  //virtual bool IsDipDestination(const character*) const { return true; }
  virtual character* GetDeceased() const { return Deceased; }
  virtual void SetDeceased(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void AddConsumeEndMessage(character*) const;
  virtual void GenerateLeftOvers(character*);
  virtual long Score() const;
  virtual bool IsDestroyable() const;
  virtual ulong GetPrice() const;
  virtual item* PrepareForConsuming(character*);
  virtual uchar GetMaterials() const { return 2; }
  virtual bool RaiseTheDead(character*);
  virtual std::string GetConsumeVerb() const;
  virtual bool IsEatable(const character* Eater) const { return IsConsumable(Eater); }
  virtual bool IsDrinkable(const character*) const { return false; }
  virtual void CalculateVolumeAndWeight();
  virtual void CalculateEmitation();
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual uchar GetFlyAmount() const;
 protected:
  virtual bool IsSparkling(ushort) const { return false; }
  virtual material*& GetMaterialReference(ushort);
  virtual void GenerateMaterials() { }
  virtual ushort GetMaterialColorA(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaA(ushort) const;
  virtual bool ShowMaterial() const { return false; }
  virtual void AddPostFix(std::string&) const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual ushort GetSize() const;
  character* Deceased;
);

class ITEM
(
  wandoflocking,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return WHITE; }
);

class ITEM
(
  wandofresurrection,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return BLACK; }
);

class ITEM
(
  whistle,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual void BlowEffect(character*);
  virtual ushort GetRange() const { return 20 * 20; }
);

class ITEM
(
  magicalwhistle,
  whistle,
 public:
  virtual void BlowEffect(character*);
);

class ITEM
(
  chest,
  item,
 public:
  chest(const chest&);
  virtual ~chest();
  virtual bool Open(character*);
  virtual bool IsOpenable(const character*) const { return true; }
  virtual bool TryKey(item*, character*);
  virtual void Lock() { Locked = true; }
  virtual uchar GetLockType() const { return LockType; }
  virtual void SetLockType(uchar What) { LockType = What; }
  virtual bool IsLocked() const { return Locked; }
  virtual void SetIsLocked(bool What) { Locked = What; }
  virtual stack* GetContained() const { return Contained; }
  virtual bool TakeSomethingFrom(character*);
  virtual bool PutSomethingIn(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool Polymorph(stack*);
  virtual ushort HowManyFits(item*) const;
  virtual void CalculateVolumeAndWeight();
  virtual bool ContentsCanBeSeenBy(const character*) const;
  virtual ulong GetPrice() const;
  virtual bool ReceiveDamage(character*, ushort, uchar);
  virtual void DrawContents(const character*);
  virtual bool Apply(character* Applier) { return Open(Applier); }
  virtual bool IsAppliable(const character*) const { return true; }
 protected:
  virtual ushort GetMaterialColorB(ushort) const { return MakeRGB16(80, 80, 80); }
  virtual void AddPostFix(std::string& String) const { AddLockPostFix(String, LockType); }
  virtual void VirtualConstructor(bool);
  stack* Contained;
  uchar LockType;
  bool Locked;
);

class ITEM
(
  beartrap,
  item,
 public:
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void StepOnEffect(character*);
  virtual bool TryToUnstuck(character*, ushort, vector2d);
  virtual bool CheckPickUpEffect(character*);
  virtual bool IsPickable(character*) const;
  virtual bool IsActive() const { return Active; }
  virtual void SetIsActive(bool What) { Active = What; UpdatePictures(); }
  virtual bool CanBeSeenBy(const character*) const;
  virtual bool Apply(character*);
  virtual vector2d GetBitmapPos(ushort) const;
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool DangerousToStepOn(const character*) const { return true; } 
  virtual ushort GetTeam() const { return Team; }
  virtual void SetTeam(ushort What) { Team = What; }
 protected:
  virtual void VirtualConstructor(bool);
  bool Active;
  ushort Team;
); 

class ITEM
(
  wandofdoorcreation,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
);

class ITEM
(
  wandofinvisibility,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return WHITE; }
);

class ITEM
(
  helmet,
  armor,
 public:
  virtual ulong GetPrice() const { return armor::GetPrice() / 3 + GetEnchantedPrice(Enchantment); }
  virtual bool IsHelmet(const character*) const { return true; }
);

class ITEM
(
  stethoscope,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; };
  virtual bool ListenTo(lsquare*,character*);
);

class ITEM
(
  flamingsword,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
  virtual uchar GetSpecialFlags() const { return ST_FLAME; }
);

class ITEM
(
  mjolak,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
);

class ITEM
(
  vermis,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
);

class ITEM
(
  turox,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
);

class ITEM
(
  whipofthievery,
  whip,
 public:
  virtual ulong GetPrice() const;
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
 protected:
  virtual bool CleptiaHelps(const character*, const character*) const;
);

class ITEM
(
  wandofcloning,
  wand,
 public:
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return GREEN; }
);

class ITEM
(
  scrollofenchantweapon,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  scrollofenchantarmor,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  skull,
  item,
  ;
);

#endif
