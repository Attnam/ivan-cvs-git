#ifndef __MISCITEM_H__
#define __MISCITEM_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <set>

#include "item.h"

class ABSTRACT_ITEM
(
  materialcontainer,
  item,
 public:
  materialcontainer(const materialcontainer&);
  virtual ~materialcontainer();
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material*, ushort = 0);
  virtual void ChangeContainedMaterial(material*, ushort = 0);
  void InitMaterials(material*, material*, bool = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual material* GetConsumeMaterial() const { return ContainedMaterial; }
  virtual void SetConsumeMaterial(material*, ushort = 0);
  virtual void ChangeConsumeMaterial(material*, ushort = 0);
  virtual ushort GetMaterials() const { return 2; }
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual void Be();
  virtual uchar GetSpoilLevel() const;
  virtual material* GetMaterial(ushort) const;
  virtual uchar GetAttachedGod() const;
 protected:
  virtual bool IsSparkling(ushort) const;
  virtual void GenerateMaterials();
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaB(ushort) const;
  virtual uchar GetRustDataB() const;
  material* ContainedMaterial;
);

class ITEM
(
  banana,
  materialcontainer,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { TimesUsed = 0; }
  virtual bool IsZappable(const character*) const { return true; }
  virtual bool IsChargeable(const character*) const { return true; }
  virtual void GenerateLeftOvers(character*);
  virtual void SignalSpoil(material*);
  virtual bool IsBanana() const { return true; }
 protected:
  virtual void VirtualConstructor(bool);
  uchar TimesUsed;
  uchar Charges;
);

class ITEM
(
  holybanana,
  banana,
 public:
  virtual bool HitEffect(character*, character*, vector2d, uchar, uchar, bool);
  virtual bool Zap(character*, vector2d, uchar);
  virtual void Be() { }
  virtual ushort GetSpecialFlags() const;
  virtual void AddInventoryEntry(const character*, festring&, ushort, bool) const;
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
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
  virtual bool AllowAlphaEverywhere() const { return true; }
  virtual ushort GetSpecialFlags() const;
 protected:
  virtual bool HasSpecialAnimation() const { return !IsBroken(); }
  virtual ushort GetClassAnimationFrames() const { return !IsBroken() ? 32 : 1; }
  virtual ushort GetMaterialColorA(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const;
  virtual ushort GetMaterialColorC(ushort) const;
  virtual ushort GetMaterialColorD(ushort) const;
  virtual uchar GetAlphaA(ushort) const { return 255; }
  virtual uchar GetAlphaB(ushort) const;
  virtual uchar GetAlphaC(ushort) const;
  virtual uchar GetAlphaD(ushort) const;
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBitmapPos(ushort) const;
  uchar SquarePosition;
);

class ITEM
(
  can,
  materialcontainer,
 public:
  virtual item* BetterVersion() const;
  virtual void GenerateLeftOvers(character*);
  virtual void DipInto(liquid*, character*);
  virtual bool IsDippable(const character*) const { return !ContainedMaterial; }
  virtual bool IsDipDestination(const character*) const;
  virtual liquid* CreateDipLiquid();
  virtual bool AllowSpoil() const { return false; } // temporary
  virtual bool HasBetterVersion() const { return !ContainedMaterial; }
 protected:
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
  virtual bool AddAdjective(festring&, bool) const;
  virtual vector2d GetBitmapPos(ushort) const;
);

class ITEM
(
  lump,
  item,
 public:
  virtual bool HitEffect(character*, character*, vector2d, uchar, uchar, bool);
 protected:
  virtual void AddPostFix(festring& String) const { AddLumpyPostFix(String); }
  virtual bool ShowMaterial() const { return false; }
);

class ITEM
(
  potion,
  materialcontainer,
 public:
  virtual item* BetterVersion() const;
  virtual void DipInto(liquid*, character*);
  virtual liquid* CreateDipLiquid();
  virtual bool IsDippable(const character*) const { return !ContainedMaterial; }
  virtual void GenerateLeftOvers(character*);
  virtual void Break(character*, uchar);
  virtual bool IsDipDestination(const character*) const;
  virtual bool IsExplosive() const;
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
  virtual bool HasBetterVersion() const { return !ContainedMaterial; }
  virtual bool EffectIsGood() const;
 protected:
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
  virtual bool AddAdjective(festring&, bool) const;
);

class ITEM
(
  bananapeels,
  item,
 public:
  virtual item* BetterVersion() const;
  virtual bool HasBetterVersion() const { return true; }
  virtual void StepOnEffect(character*);
);

class ITEM
(
  brokenbottle,
  item,
 public:
  virtual bool IsBroken() const { return true; }
  virtual item* BetterVersion() const;
  virtual bool HasBetterVersion() const { return true; }
  virtual void StepOnEffect(character*);
  virtual item* Fix();
);

class ABSTRACT_ITEM
(
  scroll,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual bool IsReadable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
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
  virtual bool DogWillCatchAndConsume() const;
);

class ITEM
(
  loaf,
  item,
 protected:
  virtual void AddPostFix(festring& String) const { AddLumpyPostFix(String); }
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

class ITEM
(
  wand,
  item,
 public:
  virtual bool Apply(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { TimesUsed = 0; }
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool IsZappable(const character*) const { return true; }
  virtual bool IsChargeable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
  virtual bool Zap(character*, vector2d, uchar);
  virtual void AddInventoryEntry(const character*, festring&, ushort, bool) const;
  virtual ulong GetPrice() const;
  void BreakEffect(character*, const festring&);
 protected:
  virtual void VirtualConstructor(bool);
  uchar Charges;
  uchar TimesUsed;
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
  kiwi,
  item,
  ;
);

class ITEM
(
  pineapple,
  item,
  ;
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
  backpack,
  materialcontainer,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
  virtual bool IsExplosive() const;
  virtual ulong GetTotalExplosivePower() const;
  virtual void SpillFluid(character*, liquid*, ushort = 0);
 protected:
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
);

class ITEM
(
  holybook,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual bool Read(character*);
  virtual bool IsReadable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
  virtual void FinishReading(character*);
 protected:
  virtual ushort GetMaterialColorA(ushort) const;
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
  virtual ulong GetTruePrice() const;
 protected:
  virtual bool ShowMaterial() const;
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
  mine,
  materialcontainer,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void StepOnEffect(character*);
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
  virtual bool IsActive() const { return Active; }
  virtual void SetIsActive(bool);
  virtual bool CanBeSeenBy(const character*) const;
  virtual bool Apply(character* User);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool IsDangerousForAI(const character* Stepper) const { return WillExplode(Stepper); }
  virtual bool WillExplode(const character*) const;
  virtual ushort GetTeam() const { return Team; }
  virtual void SetTeam(ushort What) { Team = What; }
  virtual bool CheckPickUpEffect(character*);
  virtual void Search(const character*, ushort);
  virtual bool IsDangerous() const { return IsActive(); }
  virtual void FinalProcessForBone();
  virtual void TeleportRandomly();
 protected:
  virtual bool AddAdjective(festring&, bool) const;
  virtual void VirtualConstructor(bool);
  bool Active;
  ushort Team;
  std::set<ushort> DiscoveredByTeam;
);

class ITEM
(
  key,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool CanOpenDoors() const { return true; }
  virtual bool CanOpenLockType(ushort AnotherLockType) const { return GetConfig() == AnotherLockType; }
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
  whistle,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual void BlowEffect(character*);
 protected:
  virtual ushort GetMaterialColorB(ushort) const;
);

class ITEM
(
  magicalwhistle,
  whistle,
 public:
  virtual void BlowEffect(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void FinalProcessForBone();
 protected:
  virtual void VirtualConstructor(bool);
  ulong LastUsed;
);

class ITEM
(
  itemcontainer,
  item,
 public:
  itemcontainer(const itemcontainer&);
  virtual ~itemcontainer();
  virtual bool Open(character*);
  virtual bool IsOpenable(const character*) const { return true; }
  virtual bool TryKey(item*, character*);
  virtual bool HasLock() const { return true; }
  virtual void Lock() { Locked = true; }
  virtual bool IsLocked() const { return Locked; }
  virtual void SetIsLocked(bool What) { Locked = What; }
  virtual stack* GetContained() const { return Contained; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool Polymorph(character*, stack*);
  virtual void CalculateVolumeAndWeight();
  virtual bool ContentsCanBeSeenBy(const character*) const;
  virtual ulong GetTruePrice() const;
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
  virtual void DrawContents(const character*);
  virtual bool Apply(character* Applier) { return Open(Applier); }
  virtual bool IsAppliable(const character*) const { return true; }
  virtual void SetItemsInside(const std::list<contentscript<item> >&, ushort);
  virtual void GenerateLeftOvers(character*);
  virtual bool AllowContentEmitation() const { return false; }
  virtual bool IsDestroyable() const;
  virtual short GetOfferValue(uchar) const;
  virtual void SortAllItems(itemvector&, const character*, bool (*)(const item*, const character*)) const;
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone();
 protected:
  virtual ushort GetMaterialColorB(ushort) const;
  virtual void VirtualConstructor(bool);
  stack* Contained;
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
  virtual void SetIsActive(bool);
  virtual bool CanBeSeenBy(const character*) const;
  virtual bool Apply(character*);
  virtual vector2d GetBitmapPos(ushort) const;
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool IsDangerousForAI(const character*) const { return IsActive(); }
  virtual ushort GetTeam() const { return Team; }
  virtual void SetTeam(ushort What) { Team = What; }
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar);
  virtual void Search(const character*, ushort);
  virtual bool IsDangerous() const { return IsActive(); }
  virtual bool IsStuck() const;
  virtual void Fly(character*, uchar, ushort);
  virtual void FinalProcessForBone();
  virtual void TeleportRandomly();
 protected:
  virtual bool AddAdjective(festring&, bool) const;
  virtual void VirtualConstructor(bool);
  bool Active;
  ushort Team;
  std::set<ushort> DiscoveredByTeam;
);

class ITEM
(
  stethoscope,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; };
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

class ITEM
(
  scrollofrepair,
  scroll,
 public:
  virtual bool Read(character*);
  virtual void FinishReading(character*);
);

class ITEM
(
  encryptedscroll,
  scroll,
 public:
  virtual void Be() { }
  virtual bool Read(character*);
  virtual bool ReceiveDamage(character*, ushort, ushort, uchar) { return false; }
  virtual bool IsEncryptedScroll() const { return true; }
);

class ITEM
(
  horn,
  item,
 public:
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual void FinalProcessForBone();
 protected:
  virtual void VirtualConstructor(bool);
  ulong LastUsed;
);

class ITEM
(
  carrot,
  item,
 public:
  virtual bool BunnyWillCatchAndConsume() const;
 protected:
  virtual ushort GetMaterialColorB(ushort) const;
);

class ITEM
(
  charmlyre,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void FinalProcessForBone();
 protected:
  virtual ushort GetMaterialColorB(ushort) const;
  virtual void VirtualConstructor(bool);
  ulong LastUsed;
);

#endif
