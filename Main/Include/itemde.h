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
  virtual ulong Price() const;
  virtual bool IsDippable(character*) const { return true; }*/
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material* What) { SetMaterial(ContainedMaterial, What, GetDefaultContainedVolume()); }
  virtual void ChangeContainedMaterial(material* What) { ChangeMaterial(ContainedMaterial, What, GetDefaultContainedVolume()); }
  virtual void InitMaterials(material* M1, material* M2, bool CUP = true) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), ContainedMaterial, M2, GetDefaultContainedVolume(), CUP); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual material* GetConsumeMaterial() const { return ContainedMaterial; }
  virtual void SetConsumeMaterial(material* NewMaterial) { SetContainedMaterial(NewMaterial); }
  virtual void ChangeConsumeMaterial(material* NewMaterial) { ChangeContainedMaterial(NewMaterial); }
  virtual uchar GetMaterials() const { return 2; }
  virtual material* GetMaterial(ushort) const;
 protected:
  virtual void GenerateMaterials();
  virtual ushort GetMaterialColor1(ushort) const;
  virtual uchar GetAlpha1(ushort) const;
  material* ContainedMaterial;
);

class ITEM
(
  banana,
  materialcontainer,
 public:
  virtual ulong Price() const { return GetContainedMaterial()->RawPrice(); }
  virtual bool Zap(character*, vector2d, uchar);
  virtual uchar GetCharges() const { return Charges; }
  virtual void SetCharges(uchar What) { Charges = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { SetCharges(6); }
  virtual bool IsZappable(character*) const { return true; }
  virtual bool IsChargeable(character*) const { return true; }
  virtual void GenerateLeftOvers(character*);
  virtual bool IsAnimated() const { return true; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort GetAnimationFrames() const { return 20; }
  virtual ushort GetMaterialColor0(ushort) const;
  uchar Charges;
);

class ITEM
(
  holybanana,
  banana,
  ;
);

class ITEM
(
  lantern,
  item,
 public:
  virtual void PositionedDrawToTileBuffer(uchar, bool) const;
  virtual void SignalSquarePositionChange(bool);
  virtual void SetOnWall(bool What) { OnWall = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, OnWall ? 192 : 256); }
  bool OnWall;
);

class ITEM
(
  can,
  materialcontainer,
 public:
  virtual item* TryToOpen(character*);
  virtual item* PrepareForConsuming(character*);
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual item* BetterVersion() const;
  virtual bool IsOpenable(character*) const { return true; }
 protected:
  virtual std::string GetPostFix() const { return ContainerPostFix(); }
  virtual std::string GetAdjective() const { return !GetContainedMaterial() ? "empty" : ""; }
  virtual std::string GetAdjectiveArticle() const { return "an"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, GetContainedMaterial() ? 288 : 304); }
);

class ITEM
(
  lump,
  item,
 public:
  virtual void ReceiveHitEffect(character*, character*);
  virtual material* CreateDipMaterial();
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
  virtual bool IsDipDestination(character*) const { return true; }
 protected:
  virtual std::string GetPostFix() const { return LumpyPostFix(); }
  virtual bool ShowMaterial() const { return false; }
);

class ABSTRACT_ITEM
(
  meleeweapon,
  item,
 public:
  virtual void ReceiveHitEffect(character*, character*);
  virtual void DipInto(material*, character*);
  virtual ulong Price() const;
  virtual bool IsDippable(character*) const { return true; }
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material* What) { SetMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume()); }
  virtual void ChangeSecondaryMaterial(material* What) { ChangeMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume()); }
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material* What) { SetMaterial(ContainedMaterial, What, GetDefaultContainedVolume()); }
  virtual void ChangeContainedMaterial(material* What) { ChangeMaterial(ContainedMaterial, What, GetDefaultContainedVolume()); }
  virtual void InitMaterials(material* M1, material* M2, material* M3, bool CUP = true) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), SecondaryMaterial, M2, GetDefaultSecondaryVolume(), ContainedMaterial, M3, GetDefaultContainedVolume(), CUP); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetMaterials() const { return 3; }
  virtual material* GetMaterial(ushort) const;
 protected:
  virtual void GenerateMaterials();
  virtual ushort GetMaterialColor1(ushort) const;
  virtual uchar GetAlpha1(ushort) const;
  material* SecondaryMaterial;
  material* ContainedMaterial;
);

class ITEM
(
  longsword,
  meleeweapon,
  ;
);

class ITEM
(
  twohandedsword,
  longsword,
  ;
);

class ITEM
(
  curvedtwohandedsword,
  twohandedsword,
  ;
);

class ITEM
(
  valpurusjustifier,
  longsword,
  ;
);

class ITEM
(
  axe,
  meleeweapon,
  ;
);

class ITEM
(
  pickaxe,
  axe,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(character*) const;
  ;
);

class ITEM
(
  spear,
  meleeweapon,
  ;
);

class ABSTRACT_ITEM
(
  bodyarmor,
  item,
 public:
  virtual ulong Price() const;
  virtual bool IsBodyArmor(character*) const { return true; }
);

class ITEM
(
  platemail,
  bodyarmor,
 public:
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual std::string GetNameSingular() const;
);

class ITEM
(
  chainmail,
  bodyarmor,
  ;
);

class ABSTRACT_ITEM
(
  shirt,
  bodyarmor,
  ;
);

class ITEM
(
  goldeneagleshirt,
  shirt,
 public:
  virtual ushort GetStrengthValue() const { return 100; }
  virtual short CalculateOfferValue(char) const { return 750; }
  virtual bool IsGoldenEagleShirt() const { return true; };
  virtual bool IsConsumable(character*) const { return false; }
);

class ITEM
(
  potion,
  materialcontainer,
 public:
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual item* BetterVersion() const;
  virtual std::string GetConsumeVerb() const { return "drinking"; }
  virtual void DipInto(material*, character*);
  virtual material* CreateDipMaterial();
  virtual bool IsDippable(character*) const { return !GetContainedMaterial(); }
  virtual void GenerateLeftOvers(character*);
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual std::string GetPostFix() const { return ContainerPostFix(); }
  virtual std::string GetAdjective() const { return !GetContainedMaterial() ? "empty" : ""; }
  virtual std::string GetAdjectiveArticle() const { return "an"; }
);

class ITEM
(
  bananapeals,
  item,
 public:
  virtual item* BetterVersion() const { return new banana; }
  virtual bool GetStepOnEffect(character *);
);

class ITEM
(
  brokenbottle,
  item,
 public:
  virtual item* BetterVersion() const;
  virtual bool GetStepOnEffect(character*);
);

class ABSTRACT_ITEM
(
  scroll,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual bool IsReadable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
);

class ITEM
(
  scrollofcreatemonster,
  scroll,
 public:
  virtual bool Read(character*);
);

class ITEM
(
  scrollofteleportation,
  scroll,
 public:
  virtual bool Read(character*);
);

class ITEM
(
  scrollofcharging,
  scroll,
 public:
  virtual bool Read(character*);
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
  virtual bool IsPetrussNut() const { return true; }
  virtual bool IsConsumable(character*) const { return false; }
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
  poleaxe,
  axe,
  ;
);

class ITEM
(
  spikedmace,
  meleeweapon,
  ;
);

class ITEM
(
  neercseulb,
  spikedmace,
  ;
);

class ITEM
(
  loaf,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial() ? GetMainMaterial()->RawPrice() : 0; }
 protected:
  virtual std::string GetPostFix() const { return LumpyPostFix(); }
  virtual bool ShowMaterial() const { return false; }
);

class ITEM
(
  scrollofwishing,
  scroll,
 public:
  virtual bool Read(character*);
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
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*) { return false; };
  virtual void ChargeFully(character*) { SetTimesUsed(0); }
  virtual bool IsAppliable(character*) const { return true; }
  virtual bool IsZappable(character*) const { return true; }
  virtual bool IsChargeable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ushort GetBeamColor() const = 0;
  virtual void VirtualConstructor(bool);
  virtual std::string GetPostFix() const;
  uchar Charges;
  uchar TimesUsed;
);

class ITEM
(
  wandofpolymorph,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual ushort GetBeamColor() const { return BLUE; }
  virtual void VirtualConstructor(bool);
);

class ITEM
(
  brokenlantern,
  lantern,
 public:
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
 protected:
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, OnWall ? 208 : 304); }
);

class ITEM
(
  scrollofchangematerial,
  scroll,
 public:
  virtual bool Read(character*);
);

class ITEM
(
  avatarofvalpurus,
  item,
 public:
  virtual bool IsTheAvatar() const { return true; }
  virtual bool IsConsumable(character*) const { return false; }
);

class ITEM
(
  wandofstriking,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return WHITE; }
);

class ITEM
(
  brokenplatemail,
  platemail,
 public:
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
);

class ITEM
(
  kiwi,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
);

class ITEM
(
  pineapple,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
);

class ITEM
(
  palmbranch,
  item,
  ;
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
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual bool Apply(character*);
  virtual bool IsAppliable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual std::string GetPostFix() const { return ContainerPostFix(); }
);

class ITEM
(
  holybook,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual void SetDivineMaster(uchar);
  virtual bool Read(character*);
  virtual bool IsReadable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort GetMaterialColor0(ushort) const;
  virtual std::string GetPostFix() const { return GetDivineMasterDescription(DivineMaster); }
  virtual bool ShowMaterial() const { return false; }
  uchar DivineMaster;
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
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GetInhabitedByGenie() const { return InhabitedByGenie; }
  virtual void SetInhabitedByGenie(bool What) { InhabitedByGenie = What; }
  virtual bool Apply(character*);
  virtual bool IsAppliable(character*) const { return true; }
 protected:
  virtual void VirtualConstructor(bool);
  bool InhabitedByGenie;
);

class ITEM
(
  stone,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice() * 2; }
);

class ITEM
(
  wandoffireballs,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return YELLOW; }
);

class ITEM
(
  scrolloftaming,
  scroll,
 public:
  virtual bool Read(character*);
);

class ITEM
(
  wandofteleportation,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return GREEN; }
);

class ITEM
(
  mine,
  materialcontainer,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetCharged() const { return Charged; }
  virtual void SetCharged(bool What) { Charged = What; }
  virtual bool GetStepOnEffect(character *);
  virtual bool IsChargeable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual void VirtualConstructor(bool);
  bool Charged;
);

class ITEM
(
  wandofhaste,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return RED; }
);

class ITEM
(
  wandofslow,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual void VirtualConstructor(bool);
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
  virtual bool IsAppliable(character*) const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool CanOpenDoors() const { return true; }
  virtual bool CanOpenLockType(uchar AnotherLockType) const { return LockType == AnotherLockType; }
 protected:
  virtual void VirtualConstructor(bool);
  uchar LockType;
);

class ITEM
(
  shield,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the shield can be calculated somehow
);

class ITEM
(
  cloak,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the cloak can be calculated somehow
  virtual bool IsCloak(character*) const { return true; }
);

class ITEM
(
  boot,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the boots can be calculated someho
  virtual bool IsBoot(character*) const { return true; }
);

class ITEM
(
  gauntlet, 
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the gauntlets can be calculated somehow
  virtual bool IsGauntlet(character*) const { return true; }
);

class ITEM
(
  belt, 
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual bool IsBelt(character*) const { return true; }
  virtual ushort GetFormModifier() const; 
);

class ITEM
(
  ring, 
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual bool IsRing(character*) const { return true; }
);

class ITEM
(
  amulet, 
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual bool IsAmulet(character*) const { return true; }
);

class ABSTRACT_ITEM
(
  bodypart,
  materialcontainer,
 public:
  friend class corpse;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRHUMANOID; }
  virtual character* GetMaster() const;
  virtual humanoid* GetHumanoidMaster() const;
  virtual ushort GetStrengthValue() const;
  virtual short GetMaxHP() const;
  virtual void SetHP(short What) { HP = What; }
  virtual short GetHP() const { return HP; }
  virtual void EditHP(short What) { HP += What; }
  virtual ushort GetTotalResistance(uchar) const = 0;
  virtual bool ReceiveDamage(character*, short, uchar);
  virtual std::string GetOwnerDescription() const { return OwnerDescription; }
  virtual void SetOwnerDescription(const std::string& What) { OwnerDescription = What; }
  virtual bool GetUnique() const { return Unique; }
  virtual void SetUnique(bool What) { Unique = What; }
  virtual characterslot* GetCharacterSlot() const;
  virtual void SignalGearUpdate() { }
  virtual ushort GetRegenerationCounter() const { return RegenerationCounter; }
  virtual void SetRegenerationCounter(ushort What) { RegenerationCounter = What; }
  virtual void EditRegenerationCounter(short What) { RegenerationCounter += What; }
  virtual void Regenerate(ushort);
  virtual ushort DangerWeight() const = 0;
  virtual ushort Danger(ulong, bool) const;
  virtual ulong GetTotalWeight() const { return 0; }
  virtual void DropEquipment() { }
  virtual material* GetConsumeMaterial() const { return MainMaterial; }
  virtual void SetConsumeMaterial(material* NewMaterial) { SetMainMaterial(NewMaterial); }
  virtual void ChangeConsumeMaterial(material* NewMaterial) { ChangeMainMaterial(NewMaterial); }
  virtual void SetAnimationFrames(ushort What) { AnimationFrames = What; }
  virtual bool IsAnimated() const { return AnimationFrames > 1; }
  virtual std::vector<vector2d>& GetBitmapPosVector() { return BitmapPos; }
  virtual std::vector<ushort>& GetColor1Vector() { return Color1; }
  virtual std::vector<ushort>& GetColor2Vector() { return Color2; }
  virtual std::vector<ushort>& GetColor3Vector() { return Color3; }
 protected:
  //virtual void GenerateMaterials() { }
  virtual void VirtualConstructor(bool);
  virtual ushort GetAnimationFrames() const { return AnimationFrames; }
  virtual std::string GetPostFix() const { return GetOwnerDescription(); }
  virtual bool ShowMaterial() const { return false; }
  virtual uchar GetArticleMode() const { return Unique ? DEFINITEARTICLE : NORMALARTICLE; }
  virtual ushort GetMaterialColor0(ushort) const;
  virtual ushort GetMaterialColor1(ushort Frame) const { return Color1[Frame]; }
  virtual ushort GetMaterialColor2(ushort Frame) const { return Color2[Frame]; }
  virtual ushort GetMaterialColor3(ushort Frame) const { return Color3[Frame]; }
  virtual vector2d GetBitmapPos(ushort Frame) const { return BitmapPos[Frame]; }
  std::string OwnerDescription;
  std::vector<vector2d> BitmapPos;
  std::vector<ushort> Color1;
  std::vector<ushort> Color2;
  std::vector<ushort> Color3;
  ushort AnimationFrames;
  short HP;
  bool Unique;
  ushort RegenerationCounter;
);

class ITEM
(
  head,
  bodypart,
 public:
  virtual ~head();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetTotalResistance(uchar) const;
  virtual void SetHelmet(item* What) { HelmetSlot.SetItem(What); }
  virtual item* GetHelmet() const { return *HelmetSlot; }
  virtual void SetAmulet(item* What) { AmuletSlot.SetItem(What); }
  virtual item* GetAmulet() const { return *AmuletSlot; }
  virtual ushort DangerWeight() const;
  virtual ulong GetTotalWeight() const;
  virtual void DropEquipment();
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual void VirtualConstructor(bool);
  gearslot HelmetSlot;
  gearslot AmuletSlot;
);

class ABSTRACT_ITEM
(
  torso,
  bodypart,
 public:
  virtual ushort DangerWeight() const;
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual bool ReceiveDamage(character*, short, uchar);
);

class ITEM
(
  normaltorso,
  torso,
 public:
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRCHARACTER; }
  virtual ushort GetTotalResistance(uchar) const;
);

class ITEM
(
  humanoidtorso,
  torso,
 public:
  virtual ~humanoidtorso();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRHUMANOID; }
  virtual ushort GetTotalResistance(uchar) const;
  virtual void SetBodyArmor(item* What) { BodyArmorSlot.SetItem(What); }
  virtual item* GetBodyArmor() const { return *BodyArmorSlot; }
  virtual void SetCloak(item* What) { CloakSlot.SetItem(What); }
  virtual item* GetCloak() const { return *CloakSlot; }
  virtual void SetBelt(item* What) { BeltSlot.SetItem(What); }
  virtual item* GetBelt() const { return *BeltSlot; }
  virtual ulong GetTotalWeight() const;
  virtual void DropEquipment();
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
  virtual ~arm();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetTotalResistance(uchar) const;
  virtual sweaponskill* GetCurrentSingleWeaponSkill() const { return CurrentSingleWeaponSkill; }
  virtual void SetCurrentSingleWeaponSkill(sweaponskill* What) { CurrentSingleWeaponSkill = What; }
  virtual ushort GetSingleWeaponSkills() const { return SingleWeaponSkill.size(); }
  virtual sweaponskill* GetSingleWeaponSkill(ushort Index) const { return SingleWeaponSkill[Index]; }
  virtual void SetSingleWeaponSkill(ushort Index, sweaponskill* What) { SingleWeaponSkill[Index] = What; }
  virtual void Be();
  virtual float GetWieldedStrength(bool);
  virtual float GetMeleeStrength();
  virtual float GetWieldedToHitValue(bool);
  virtual float GetMeleeToHitValue();
  virtual void SetWielded(item*);
  virtual item* GetWielded() const { return *WieldedSlot; }
  virtual void SetGauntlet(item* What) { GauntletSlot.SetItem(What); }
  virtual item* GetGauntlet() const { return *GauntletSlot; }
  virtual void SetRing(item* What) { RingSlot.SetItem(What); }
  virtual item* GetRing() const { return *RingSlot; }
  virtual void SignalGearUpdate();
  virtual ushort DangerWeight() const;
  virtual ulong GetTotalWeight() const;
  virtual void DropEquipment();
  virtual void AddCurrentSingleWeaponSkillInfo(felist&);
 protected:
  virtual void VirtualConstructor(bool);
  gearslot WieldedSlot;
  gearslot GauntletSlot;
  gearslot RingSlot;
  std::vector<sweaponskill*> SingleWeaponSkill;
  sweaponskill* CurrentSingleWeaponSkill;
);

class ITEM
(
  rightarm,
  arm,
 public:
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual uchar GetSpecialType(ushort) const { return STRIGHTARM; }
);

class ITEM
(
  leftarm,
  arm,
 public:
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual uchar GetSpecialType(ushort) const { return STLEFTARM; }
);

class ITEM
(
  groin,
  bodypart,
 public:
  virtual ushort GetTotalResistance(uchar) const;
  virtual ushort DangerWeight() const;
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual uchar GetSpecialType(ushort) const { return STGROIN; }
);

class ABSTRACT_ITEM
(
  leg,
  bodypart,
 public:
  virtual ~leg();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetTotalResistance(uchar) const;
  virtual void SetBoot(item* What) { BootSlot.SetItem(What); }
  virtual item* GetBoot() const { return *BootSlot; }
  virtual ushort DangerWeight() const;
  virtual ulong GetTotalWeight() const;
  virtual void DropEquipment();
 protected:
  virtual void VirtualConstructor(bool);
  gearslot BootSlot;
);

class ITEM
(
  rightleg,
  leg,
 public:
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual uchar GetSpecialType(ushort) const { return STRIGHTLEG; }
);

class ITEM
(
  leftleg,
  leg,
 public:
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual uchar GetSpecialType(ushort) const { return STLEFTLEG; }
);

class ITEM
(
  headofelpuri,
  item, // can't wear equipment, so not "head"
 public:
  virtual bool IsHeadOfElpuri() const { return true; }
  virtual bool IsConsumable(character*) const { return false; }
);

class ITEM
(
  corpse,
  item,
 public:
  virtual ~corpse();
  virtual bool Consume(character*, long);
  virtual ushort GetEmitation() const;
  virtual bool IsConsumable(character*) const;
  virtual short CalculateOfferValue(char) const;
  virtual float GetWeaponStrength() const;
  virtual bool IsBadFoodForAI(character*) const;
  virtual ushort GetStrengthValue() const;
  virtual ulong GetVolume() const;
  virtual void Be() { }
  virtual void SetMainMaterial(material*);
  virtual void ChangeMainMaterial(material*);
  virtual void SetContainedMaterial(material*);
  virtual void ChangeContainedMaterial(material*);
  //virtual material* CreateDipMaterial();
  //virtual bool CatWillCatchAndConsume() const { return GetConsumeMaterial()->GetType() == ratflesh::StaticType(); }
  virtual bool IsDipDestination(character*) const { return true; }
  virtual character* GetDeceased() const { return Deceased; }
  virtual void SetDeceased(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void AddConsumeEndMessage(character*) const;
  virtual void GenerateLeftOvers(character*);
  virtual long Score() const;
  virtual ulong GetWeight() const;
  virtual bool IsDestroyable() const;
  virtual ulong Price() const;
  virtual item* PrepareForConsuming(character*);
  virtual uchar GetMaterials() const { return 2; }
  virtual material* GetMaterial(ushort) const;
  virtual bool RaiseTheDead(character*);
 protected:
  virtual void GenerateMaterials() { }
  virtual ushort GetMaterialColor0(ushort) const;
  virtual ushort GetMaterialColor1(ushort) const;
  virtual uchar GetAlpha0(ushort) const;
  virtual bool ShowMaterial() const { return false; }
  virtual std::string GetPostFix() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual ushort GetSize() const;
  character* Deceased;
);

class ITEM
(
  wandoflocking,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return WHITE; }
);

class ITEM
(
  wandofresurrection,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return BLACK; }
);

class ITEM
(
  ringoffireresistance,
  ring,
  ;
);

class ITEM
(
  amuletoflifesaving,
  amulet,
 public: 
  virtual bool SavesLifeWhenWorn() const { return true; }
);

class ITEM
(
  whistle,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(character*) const { return true; }
  virtual void BlowEffect(character*);
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
  //virtual item* TryToOpen(character*);
  virtual bool IsOpenable(character*) const { return true; }
  virtual bool TryKey(item*, character*);
  virtual void Lock() { Locked = true; }
  virtual uchar GetLockType() const { return LockType; }
 protected:
  virtual void VirtualConstructor(bool);
  ulong StorageVolume;
  stack* Contained;
  uchar LockType;
  bool Locked;
);

#endif

