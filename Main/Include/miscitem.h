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
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material*, int = 0);
  virtual void ChangeSecondaryMaterial(material*, int = 0);
  void InitMaterials(material*, material*, bool = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetMaterials() const { return 2; }
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual void Be();
  virtual int GetSpoilLevel() const;
  virtual material* GetMaterial(int) const;
  virtual int GetAttachedGod() const;
  virtual material* GetConsumeMaterial(const character*, materialpredicate = TrueMaterialPredicate) const;
  virtual material* RemoveMaterial(material*);
  material* RemoveMainMaterial();
  virtual material* RemoveSecondaryMaterial();
  virtual void CalculateEmitation();
  virtual void InitMaterials(const materialscript*, const materialscript*, bool);
 protected:
  virtual long GetMaterialPrice() const;
  virtual bool CalculateHasBe() const;
  virtual bool IsSparkling(int) const;
  virtual void GenerateMaterials();
  virtual color16 GetMaterialColorB(int) const;
  virtual alpha GetAlphaB(int) const;
  virtual int GetRustDataB() const;
  material* SecondaryMaterial;
);

class ITEM
(
  banana,
  materialcontainer,
 public:
  virtual bool Zap(character*, vector2d, int);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { TimesUsed = 0; }
  virtual bool IsZappable(const character*) const { return true; }
  virtual bool IsChargeable(const character*) const { return true; }
  virtual void SignalSpoil(material*);
  virtual bool IsBanana() const { return true; }
  virtual material* RemoveSecondaryMaterial();
 protected:
  virtual void VirtualConstructor(bool);
  int TimesUsed;
  int Charges;
);

class ITEM
(
  holybanana,
  banana,
 public:
  virtual bool HitEffect(character*, character*, vector2d, int, int, bool);
  virtual bool Zap(character*, vector2d, int);
  virtual void Be() { }
  virtual int GetSpecialFlags() const;
  virtual void AddInventoryEntry(const character*, festring&, int, bool) const;
  virtual bool ReceiveDamage(character*, int, int, int);
);

class ITEM
(
  lantern,
  item,
 public:
  virtual void SignalSquarePositionChange(int);
  virtual bool AllowAlphaEverywhere() const { return true; }
  virtual int GetSpecialFlags() const;
  virtual bool IsLanternOnWall() const { return GetSquarePosition() != CENTER; }
 protected:
  virtual int GetClassAnimationFrames() const { return !IsBroken() ? 32 : 1; }
  virtual color16 GetMaterialColorA(int) const;
  virtual color16 GetMaterialColorB(int) const;
  virtual color16 GetMaterialColorC(int) const;
  virtual color16 GetMaterialColorD(int) const;
  virtual alpha GetAlphaA(int) const { return 255; }
  virtual alpha GetAlphaB(int) const;
  virtual alpha GetAlphaC(int) const;
  virtual alpha GetAlphaD(int) const;
  virtual vector2d GetBitmapPos(int) const;
);

class ITEM
(
  can,
  materialcontainer,
 public:
  virtual item* BetterVersion() const;
  virtual void DipInto(liquid*, character*);
  virtual bool IsDippable(const character*) const { return !SecondaryMaterial; }
  virtual bool IsDipDestination(const character*) const;
  virtual liquid* CreateDipLiquid();
  virtual bool AllowSpoil() const { return false; } // temporary
  virtual bool HasBetterVersion() const { return !SecondaryMaterial; }
 protected:
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
  virtual bool AddAdjective(festring&, bool) const;
  virtual vector2d GetBitmapPos(int) const;
);

class ITEM
(
  lump,
  item,
 public:
  virtual bool HitEffect(character*, character*, vector2d, int, int, bool);
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
  virtual bool IsDippable(const character*) const { return !SecondaryMaterial; }
  virtual void Break(character*, int);
  virtual bool IsDipDestination(const character*) const;
  virtual bool IsExplosive() const;
  virtual bool ReceiveDamage(character*, int, int, int);
  virtual bool HasBetterVersion() const { return !SecondaryMaterial; }
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
  virtual bool IsBananaPeel() const { return true; }
  virtual bool IsDangerousForAI(const character*) const;
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
  virtual bool IsDangerousForAI(const character*) const;
);

class ABSTRACT_ITEM
(
  scroll,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual bool IsReadable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, int, int, int);
);

class ITEM
(
  scrollofcreatemonster,
  scroll,
 public:
  virtual void FinishReading(character*);
);

class ITEM
(
  scrollofteleportation,
  scroll,
 public:
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
  virtual bool DogWillCatchAndConsume(const character*) const;
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
  virtual bool ReceiveDamage(character*, int, int, int);
  virtual bool Zap(character*, vector2d, int);
  virtual void AddInventoryEntry(const character*, festring&, int, bool) const;
  virtual long GetPrice() const;
  void BreakEffect(character*, const festring&);
 protected:
  virtual void VirtualConstructor(bool);
  int Charges;
  int TimesUsed;
);

class ITEM
(
  scrollofchangematerial,
  scroll,
 public:
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
  virtual bool ReceiveDamage(character*, int, int, int);
  virtual bool IsExplosive() const;
  virtual long GetTotalExplosivePower() const;
  virtual void SpillFluid(character*, liquid*, int = 0);
 protected:
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
);

class ITEM
(
  holybook,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual bool IsReadable(const character*) const { return true; }
  virtual bool ReceiveDamage(character*, int, int, int);
  virtual void FinishReading(character*);
 protected:
  virtual color16 GetMaterialColorA(int) const;
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
  virtual long GetTruePrice() const;
 protected:
  virtual bool ShowMaterial() const;
);

class ITEM
(
  scrolloftaming,
  scroll,
 public:
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
  virtual bool ReceiveDamage(character*, int, int, int);
  virtual bool IsActive() const { return Active; }
  virtual void SetIsActive(bool);
  virtual bool CanBeSeenBy(const character*) const;
  virtual bool Apply(character* User);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool IsDangerousForAI(const character* Stepper) const { return WillExplode(Stepper); }
  virtual bool WillExplode(const character*) const;
  virtual int GetTeam() const { return Team; }
  virtual void SetTeam(int What) { Team = What; }
  virtual bool CheckPickUpEffect(character*);
  virtual void Search(const character*, int);
  virtual bool IsDangerous() const { return IsActive(); }
  virtual void FinalProcessForBone();
  virtual void TeleportRandomly();
 protected:
  virtual bool AddAdjective(festring&, bool) const;
  virtual void VirtualConstructor(bool);
  bool Active;
  int Team;
  std::set<int> DiscoveredByTeam;
);

class ITEM
(
  key,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool CanOpenDoors() const { return true; }
  virtual bool CanOpenLockType(int AnotherLockType) const { return GetConfig() == AnotherLockType; }
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
  virtual color16 GetMaterialColorB(int) const;
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
  virtual bool HasLock(const character*) const { return true; }
  virtual void Lock() { Locked = true; }
  virtual bool IsLocked() const { return Locked; }
  virtual void SetIsLocked(bool What) { Locked = What; }
  virtual stack* GetContained() const { return Contained; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool Polymorph(character*, stack*);
  virtual void CalculateVolumeAndWeight();
  virtual bool ContentsCanBeSeenBy(const character*) const;
  virtual long GetTruePrice() const;
  virtual bool ReceiveDamage(character*, int, int, int);
  virtual void DrawContents(const character*);
  virtual bool Apply(character* Applier) { return Open(Applier); }
  virtual bool IsAppliable(const character*) const { return true; }
  virtual void SetItemsInside(const fearray<contentscript<item> >&, int);
  virtual bool AllowContentEmitation() const { return false; }
  virtual bool IsDestroyable() const;
  virtual int GetOfferValue(int) const;
  virtual void SortAllItems(itemvector&, const character*, sorter) const;
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone();
  virtual material* RemoveMaterial(material*);
 protected:
  virtual color16 GetMaterialColorB(int) const;
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
  virtual bool TryToUnstuck(character*, int, vector2d);
  virtual bool CheckPickUpEffect(character*);
  virtual bool IsPickable(character*) const;
  virtual bool IsActive() const { return Active; }
  virtual void SetIsActive(bool);
  virtual bool CanBeSeenBy(const character*) const;
  virtual bool Apply(character*);
  virtual vector2d GetBitmapPos(int) const;
  virtual bool IsAppliable(const character*) const { return true; }
  virtual bool IsDangerousForAI(const character*) const { return IsActive(); }
  virtual int GetTeam() const { return Team; }
  virtual void SetTeam(int What) { Team = What; }
  virtual bool ReceiveDamage(character*, int, int, int);
  virtual void Search(const character*, int);
  virtual bool IsDangerous() const { return IsActive(); }
  virtual bool IsStuck() const;
  virtual void Fly(character*, int, int);
  virtual void FinalProcessForBone();
  virtual void TeleportRandomly();
 protected:
  virtual bool AddAdjective(festring&, bool) const;
  virtual void VirtualConstructor(bool);
  int GetBaseDamage() const;
  bool Active;
  int Team;
  std::set<int> DiscoveredByTeam;
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
  virtual bool ReceiveDamage(character*, int, int, int) { return false; }
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
  virtual bool BunnyWillCatchAndConsume(const character*) const;
 protected:
  virtual color16 GetMaterialColorB(int) const;
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
  virtual color16 GetMaterialColorB(int) const;
  virtual void VirtualConstructor(bool);
  ulong LastUsed;
);

class ITEM
(
  scrollofdetectmaterial,
  scroll,
 public:
  virtual void FinishReading(character*);
);

class ITEM
(
  stick,
  item,
 protected:
  virtual void AddPostFix(festring& String) const { AddLumpyPostFix(String); }
  virtual bool ShowMaterial() const { return false; }
);

class ITEM
(
  scrollofhardenmaterial,
  scroll,
 public:
  virtual void FinishReading(character*);
);

#endif
