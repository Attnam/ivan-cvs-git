#ifndef __ITEMDE_H__
#define __ITEMDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "itemba.h"
#include "materde.h"
#include "wskill.h"
#include "slot.h"

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
  virtual ushort GetMaterialColor1(ushort) const;
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
  virtual std::string NameSingular() const { return "banana"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(BANANAPEAL), MAKE_MATERIAL(BANANAFLESH)); }
  virtual void VirtualConstructor(bool);
  virtual ushort GetAnimationFrames() const { return 20; }
  virtual ushort GetMaterialColor0(ushort) const;
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != BANANAPEAL; }
  uchar Charges;
);

class ITEM
(
  holybanana,
  banana,
 public:
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string NameSingular() const { return "banana of Liukas Vipro"; }
  virtual std::string NamePlural() const { return "bananas of Liukas Vipro"; }
  virtual std::string Adjective() const { return "holy"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != BANANAPEAL; }
  virtual bool ForceDefiniteArticle() const { return true; }
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
  virtual std::string NameSingular() const { return "lantern"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GLASS)); }
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
  virtual std::string NameSingular() const { return "can"; }
  virtual item* PrepareForConsuming(character*);
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual item* BetterVersion() const;
  virtual bool IsOpenable(character*) const { return true; }
 protected:
  virtual void GenerateMaterials();
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return GetContainedMaterial() ? true : false; }
  virtual std::string Adjective() const { return "empty"; }
  virtual std::string AdjectiveArticle() const { return "an"; }
  virtual bool ShowAdjective() const { return GetContainedMaterial() ? false : true; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, GetContainedMaterial() ? 288 : 304); }
);

class ITEM
(
  lump,
  item,
 public:
  virtual void ReceiveHitEffect(character*, character*);
  virtual material* CreateDipMaterial();
  virtual std::string NameSingular() const { return "lump"; }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
  virtual bool IsDipDestination(character*) const { return true; }
 protected:
  virtual void GenerateMaterials();
  virtual std::string PostFix() const { return LumpyPostFix(); }
  virtual bool ShowPostFix() const { return true; }
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
  virtual ushort GetMaterialColor1(ushort) const;
  material* SecondaryMaterial;
  material* ContainedMaterial;
);

class ITEM
(
  longsword,
  meleeweapon,
 protected:
  virtual std::string NameSingular() const { return "long sword"; }
  virtual void GenerateMaterials();
);

class ITEM
(
  twohandedsword,
  longsword,
 protected:
  virtual std::string NameSingular() const { return "two-handed sword"; }
  virtual void GenerateMaterials();
);

class ITEM
(
  curvedtwohandedsword,
  twohandedsword,
 protected:
  virtual void GenerateMaterials();
  virtual std::string Adjective() const { return "curved"; }
  virtual bool ShowAdjective() const { return true; }
);

class ITEM
(
  valpurusjustifier,
  longsword,
 public:
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string NameSingular() const { return "broadsword \"Valpurus's Justifier\""; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(VALPURIUM), MAKE_MATERIAL(VALPURIUM), 0); }
  virtual std::string Adjective() const { return "holy"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != VALPURIUM; }
  virtual bool ForceDefiniteArticle() const { return true; }
);

class ITEM
(
  axe,
  meleeweapon,
 protected:
  virtual void GenerateMaterials();
  virtual std::string NameSingular() const { return "axe"; }
  virtual std::string Article() const { return "an"; }
);

class ITEM
(
  pickaxe,
  axe,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(character*) const;
 protected:
  virtual std::string NameSingular() const { return "pick-axe"; }
  virtual void GenerateMaterials();
);

class ITEM
(
  spear,
  meleeweapon,
 protected:
  virtual std::string NameSingular() const { return "spear"; }
  virtual void GenerateMaterials();
);

class ABSTRACT_ITEM
(
  bodyarmor,
  item,
 public:
  virtual ulong Price() const;
  virtual uchar GetCategory() const { return BODYARMOR; }
  virtual bool IsBodyArmor(character*) const { return true; }
);

class ITEM
(
  platemail,
  bodyarmor,
 public:
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual std::string NameSingular() const;
  virtual void GenerateMaterials();
);

class ITEM
(
  chainmail,
  bodyarmor,
 protected:
  virtual std::string NameSingular() const { return "chain mail"; }
  virtual void GenerateMaterials();
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
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string NameSingular() const { return "Shirt of the Golden Eagle"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(CLOTH)); }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != CLOTH; }
  virtual bool ForceDefiniteArticle() const { return true; }
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
  virtual std::string NameSingular() const { return "bottle"; }
  virtual void GenerateMaterials();
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return GetContainedMaterial() ? true : false; }
  virtual std::string Adjective() const { return "empty"; }
  virtual std::string AdjectiveArticle() const { return "an"; }
  virtual bool ShowAdjective() const { return GetContainedMaterial() ? false : true; }
);

class ITEM
(
  bananapeals,
  item,
 public:
  virtual item* BetterVersion() const { return new banana; }
  virtual bool GetStepOnEffect(character *);
 protected:
  virtual std::string NameSingular() const { return "banana peal"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(BANANAPEAL)); }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != BANANAPEAL; }
);

class ITEM
(
  brokenbottle,
  item,
 public:
  virtual item* BetterVersion() const;
  virtual bool GetStepOnEffect(character*);
 protected:
  virtual std::string NameSingular() const { return "bottle"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GLASS)); }
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
);

class ABSTRACT_ITEM
(
  scroll,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual bool IsReadable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(PARCHMENT)); }
);

class ITEM
(
  scrollofcreatemonster,
  scroll,
 public:
  virtual bool Read(character*);
 protected:
  virtual std::string NameSingular() const { return "scroll of create monster"; }
  virtual std::string NamePlural() const { return "scrolls of create monster"; }
);

class ITEM
(
  scrollofteleport,
  scroll,
 public:
  virtual bool Read(character*);
 protected:
  virtual std::string NameSingular() const { return "scroll of teleportation"; }
  virtual std::string NamePlural() const { return "scrolls of teleportation"; }
);

class ITEM
(
  scrollofcharging,
  scroll,
 public:
  virtual bool Read(character*);
 protected:
  virtual std::string NameSingular() const { return "scroll of charging"; }
  virtual std::string NamePlural() const { return "scrolls of charging"; }
);

class ITEM
(
  nut,
  item,
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(HUMANFLESH)); }
  virtual std::string NameSingular() const { return "nut"; }
);

class ITEM
(
  leftnutofpetrus,
  nut,
 public:
  virtual bool IsPetrussNut() const { return true; }
  virtual bool IsConsumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string NameSingular() const { return "left nut of Petrus"; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != HUMANFLESH; }
  virtual bool ForceDefiniteArticle() const { return true; }
);

class ITEM
(
  abone,
  item,
 public:
  virtual bool DogWillCatchAndConsume() const { return GetConsumeMaterial()->GetConfig() == BONE; }
 protected:
  virtual std::string NameSingular() const { return "bone"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(BONE)); }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != BONE; }

);

class ITEM
(
  poleaxe,
  axe,
 protected:
  virtual void GenerateMaterials();
  virtual std::string NameSingular() const { return "poleaxe"; }
);

class ITEM
(
  spikedmace,
  meleeweapon,
 protected:
  virtual std::string NameSingular() const { return "spiked mace"; }
  virtual void GenerateMaterials();
);

class ITEM
(
  neercseulb,
  spikedmace,
 public:
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string NameSingular() const { return "mace \"Neerc Se-Ulb\""; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(DIAMOND), MAKE_MATERIAL(DIAMOND), MAKE_MATERIAL(FROGFLESH)); }
  virtual std::string Adjective() const { return "ancient"; }
  virtual std::string AdjectiveArticle() const { return "an"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != DIAMOND; }
  virtual bool ForceDefiniteArticle() const { return true; }
);

class ITEM
(
  loaf,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial() ? GetMainMaterial()->RawPrice() : 0; }
 protected:
  virtual std::string NameSingular() const { return "loaf"; }
  virtual std::string NamePlural() const { return "loaves"; }
  virtual void GenerateMaterials();
  virtual std::string PostFix() const { return LumpyPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual bool ShowMaterial() const { return false; }
);

class ITEM
(
  scrollofwishing,
  scroll,
 public:
  virtual bool Read(character*);
 protected:
  virtual std::string NameSingular() const { return "scroll of wishing"; }
  virtual std::string NamePlural() const { return "scrolls of wishing"; }
);

class ITEM
(
  cheapcopyofleftnutofpetrus,
  nut,
 protected:
  virtual std::string NameSingular() const { return "copy of the left nut of Petrus"; }
  virtual std::string NamePlural() const { return "copies of the left nut of Petrus"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GLASS)); }
  virtual std::string Adjective() const { return "cheap"; }
  virtual bool ShowAdjective() const { return true; }
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
  virtual std::string wand::PostFix() const;
  virtual bool ShowPostFix() const { return true; }
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
  virtual std::string NameSingular() const { return "wand of polymorph"; }
  virtual std::string NamePlural() const { return "wands of polymorph"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GLASS)); }
  virtual void VirtualConstructor(bool);
);

class ITEM
(
  brokenlantern,
  lantern,
 public:
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
 protected:
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, OnWall ? 208 : 304); }
);

class ITEM
(
  scrollofchangematerial,
  scroll,
 public:
  virtual bool Read(character*);
 protected:
  virtual std::string NameSingular() const { return "scroll of change material"; }
  virtual std::string NamePlural() const { return "scrolls of change material"; }
);

class ITEM
(
  avatarofvalpurus,
  item,
 public:
  virtual bool IsTheAvatar() const { return true; }
  virtual bool IsConsumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string NameSingular() const { return "Avatar of Valpurus"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(VALPURIUM)); }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != VALPURIUM; }
);

class ITEM
(
  wandofstriking,
  wand,
 public:
  //virtual float OfferModifier() const { return 10; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual std::string NameSingular() const { return "wand of striking"; }
  virtual std::string NamePlural() const { return "wands of striking"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(MARBLE)); }
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return WHITE; }
);

class ITEM
(
  brokenplatemail,
  bodyarmor,
 protected:
  virtual std::string NameSingular() const { return "plate mail"; }
  virtual void GenerateMaterials();
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
);

class ITEM
(
  kiwi,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
 protected:
  virtual std::string NameSingular() const { return "kiwi"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(KIWIFLESH)); }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != KIWIFLESH; }
);

class ITEM
(
  pineapple,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
 protected:
  virtual std::string NameSingular() const { return "pineapple"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(PINEAPPLEFLESH)); }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != PINEAPPLEFLESH; }
);

class ITEM
(
  palmbranch,
  item,
 protected:
  virtual std::string NameSingular() const { return "palm branch"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(PALMLEAF)); }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != PALMLEAF; }
);

class ITEM
(
  whip,
  meleeweapon,
 public:
  virtual bool IsWhip() const { return true; }
 protected:
  virtual std::string NameSingular() const { return "whip"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(LEATHER), MAKE_MATERIAL(WOOD), 0); }
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
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(LEATHER), MAKE_MATERIAL(GUNPOWDER)); }
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual std::string NameSingular() const { return "backpack"; }
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
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(PARCHMENT)); }
  virtual void VirtualConstructor(bool);
  virtual ushort GetMaterialColor0(ushort) const;
  virtual std::string NameSingular() const { return "holy book"; }
  virtual std::string PostFix() const { return DivineMasterDescription(DivineMaster); }
  virtual bool ShowPostFix() const { return true; }
  virtual bool ShowMaterial() const { return false; }
  uchar DivineMaster;
);

class ITEM
(
  fiftymillionroubles,
  item,
 protected:
  virtual std::string NameSingular() const { return "pile of 50 million roubles"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(PARCHMENT)); }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != PARCHMENT; }
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
  virtual std::string NameSingular() const { return "oil lamp"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GOLD)); }
  virtual void VirtualConstructor(bool);
  virtual std::string Article() const { return "an"; }
  bool InhabitedByGenie;
);

class ITEM
(
  astone,
  item,
 public:
  virtual std::string NameSingular() const { return "stone"; }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice() * 2; }
 protected:
  virtual void GenerateMaterials();
);

class ITEM
(
  wandoffireballs,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual std::string NameSingular() const { return "wand of fireballs"; }
  virtual std::string NamePlural() const { return "wands of fireballs"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(MARBLE)); }
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return YELLOW; }
);

class ITEM
(
  scrolloftaming,
  scroll,
 public:
  virtual bool Read(character*);
 protected:
  virtual std::string NameSingular() const { return "scroll of taming"; }
  virtual std::string NamePlural() const { return "scrolls of taming"; }
);

class ITEM
(
  wandofteleportation,
  wand,
 public:
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
 protected:
  virtual std::string NameSingular() const { return "wand of teleportation"; }
  virtual std::string NamePlural() const { return "wands of teleportation"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GOLD)); }
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
  virtual std::string NameSingular() const { return "mine"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(IRON), MAKE_MATERIAL(GUNPOWDER)); }
  virtual void VirtualConstructor(bool);
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != IRON; }
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
  virtual std::string NameSingular() const { return "wand of haste"; }
  virtual std::string NamePlural() const { return "wands of haste"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(IRON)); }
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
  virtual std::string NameSingular() const { return "wand of slow"; }
  virtual std::string NamePlural() const { return "wands of slow"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(IRON)); }
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
 protected:
  virtual std::string NameSingular() const { return "key"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(IRON)); }
  virtual void VirtualConstructor(bool);
  uchar LockType;
);

class ITEM
(
  shield,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the shield can be calculated somehow
 protected:
  virtual std::string NameSingular() const { return "shield"; }
  virtual void GenerateMaterials();
);

class ITEM
(
  cloak,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the cloak can be calculated somehow
  virtual bool IsCloak(character*) const { return true; }
 protected:
  virtual std::string NameSingular() const { return "cloak"; }
  virtual void GenerateMaterials();
);

class ITEM
(
  boot,
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the boots can be calculated someho
  virtual bool IsBoot(character*) const { return true; }
 protected:
  virtual std::string NameSingular() const { return "boot"; }
  virtual void GenerateMaterials();
);

class ITEM
(
  gauntlet, 
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the gauntlets can be calculated somehow
  virtual bool IsGauntlet(character*) const { return true; }
 protected:
  virtual std::string NameSingular() const { return "gauntlet"; }
  virtual void GenerateMaterials();
);

class ITEM
(
  belt, 
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual bool IsBelt(character*) const { return true; }
 protected:
  virtual std::string NameSingular() const { return "belt"; }
  virtual void GenerateMaterials();
  virtual ushort GetFormModifier() const; 
);

class ITEM
(
  ring, 
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual bool IsRing(character*) const { return true; }
 protected:
  virtual std::string NameSingular() const { return "ring"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(DIAMOND)); }
);

class ITEM
(
  amulet, 
  item,
 public:
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual bool IsAmulet(character*) const { return true; }
 protected:
  virtual std::string NameSingular() const { return "amulet"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GOLD)); }
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
  virtual void GenerateMaterials() { }
  virtual void VirtualConstructor(bool);
  virtual ushort GetAnimationFrames() const { return AnimationFrames; }
  virtual std::string PostFix() const { return GetOwnerDescription(); }
  virtual bool ShowPostFix() const { return !GetMaster(); }
  virtual bool ShowMaterial() const { return false; }
  virtual bool ForceDefiniteArticle() const { return Unique; }
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
  virtual std::string NameSingular() const { return "head"; }
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
  virtual std::string NameSingular() const { return "torso"; }
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
  virtual std::string NameSingular() const { return "right arm"; }
);

class ITEM
(
  leftarm,
  arm,
 public:
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual uchar GetSpecialType(ushort) const { return STLEFTARM; }
  virtual std::string NameSingular() const { return "left arm"; }
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
  virtual std::string NameSingular() const { return "groin"; }
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
  virtual std::string NameSingular() const { return "right leg"; }
);

class ITEM
(
  leftleg,
  leg,
 public:
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual uchar GetSpecialType(ushort) const { return STLEFTLEG; }
  virtual std::string NameSingular() const { return "left leg"; }
);

class ITEM
(
  headofelpuri,
  item, // can't wear equipment, so not "head"
 public:
  virtual bool IsHeadOfElpuri() const { return true; }
  virtual bool IsConsumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(ELPURIFLESH)); }
  virtual std::string NameSingular() const { return "head of Elpuri"; }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetConfig() != ELPURIFLESH; }
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
  virtual std::string NameSingular() const { return "corpse"; }
  virtual void GenerateMaterials() { }
  virtual ushort GetMaterialColor0(ushort) const;
  virtual ushort GetMaterialColor1(ushort) const;
  virtual bool ShowMaterial() const { return false; }
  virtual std::string PostFix() const;
  virtual bool ShowPostFix() const { return true; }
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
  virtual std::string NameSingular() const { return "wand of locking"; }
  virtual std::string NamePlural() const { return "wands of locking"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(COPPER)); }
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
  virtual std::string NameSingular() const { return "wand of resurrection"; }
  virtual std::string NamePlural() const { return "wands of resurrection"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(BONE)); }
  virtual void VirtualConstructor(bool);
  virtual ushort GetBeamColor() const { return BLACK; }
);

class ITEM
(
  ringoffireresistance,
  ring,
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GOLD)); }
  virtual std::string NameSingular() const { return "ring of fire resistance"; }
  virtual std::string NamePlural() const { return "rings of fire resistance"; }
);

class ITEM
(
  amuletoflifesaving,
  amulet,
 public: 
  virtual bool SavesLifeWhenWorn() const { return true; }
  protected:
  virtual std::string NameSingular() const { return "amulet of life saving"; }
  virtual std::string NamePlural() const { return "amulets of life saving"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GOLD)); }
);

class ITEM
(
  whistle,
  item,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(character*) const { return true; }
  virtual void BlowEffect(character*);
 protected:
  virtual std::string NameSingular() const { return "whistle"; }
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(TIN)); }
);

class ITEM
(
  magicalwhistle,
  whistle,
 public:
  virtual void BlowEffect(character*);
 protected:
  virtual std::string NameSingular() const { return "super whistle"; }
  virtual std::string Adjective() const { return "magical"; }
);

#endif

