#ifndef __ITEMDE_H__
#define __ITEMDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "itemba.h"
#include "materde.h"
#include "wskill.h"
#include "game.h" // remove this
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
  InitMaterials(new bananapeal, new bananaflesh),
  {
    SetSize(20);
    SetCharges(6);
  },
 public:
  //static ushort Possibility() { return 100; }
  //virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual std::string NameSingular() const { return "banana"; }
  //virtual float OfferModifier() const { return 1; }
  //virtual uchar GetWeaponCategory() const { return CLUBS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; case 1: return 150; default: return 0; } }
  virtual ulong Price() const { return GetContainedMaterial()->RawPrice(); }
  //virtual bool CanBeZapped() const { return true; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual uchar GetCharges() const { return Charges; }
  virtual void SetCharges(uchar What) { Charges = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  //virtual bool IsChargable() const { return true; }
  virtual void ChargeFully(character*) { SetCharges(6); }
  //virtual uchar GetCategory() const { return FOOD; }
  virtual bool IsZappable(character*) const { return true; }
  virtual bool IsChargeable(character*) const { return true; }
  virtual void GenerateLeftOvers(character*);
  virtual bool IsAnimated() const { return true; }
 protected:
  virtual ushort GetAnimationFrames() const { return 20; }
  virtual ushort GetMaterialColor0(ushort) const;
  //virtual ulong GetDefaultMainVolume() const { return 40; }
  //virtual ulong GetDefaultContainedVolume() const { return 150; }
  //virtual ushort GetStrengthModifier() const { return 50; }
  //virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,112); }
  //virtual ushort GetFormModifier() const { return 50; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != bananapeal::StaticType(); }
  uchar Charges;
);

class ITEM
(
  holybanana,
  banana,
  InitMaterials(new bananapeal, new bananaflesh),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 5; }
  virtual std::string NameSingular() const { return "banana of Liukas Vipro"; }
  virtual std::string NamePlural() const { return "bananas of Liukas Vipro"; }
  virtual float OfferModifier() const { return 40; }
  virtual long Score() const { return 250; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 400; case 1: return 1500; default: return 0; } }
  virtual ulong Price() const { return 1000; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 400; }
  virtual ulong GetDefaultContainedVolume() const { return 1500; }
  virtual std::string Adjective() const { return "holy"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != bananapeal::StaticType(); }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 250; }
  virtual ushort GetFormModifier() const { return 200; }
);

class ITEM
(
  lantern,
  item,
  InitMaterials(new glass),
  {
    SetSize(30);
    SetOnWall(false);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual void PositionedDrawToTileBuffer(uchar, bool) const;
  virtual ushort GetEmitation() const { return 300; }
  virtual std::string NameSingular() const { return "lantern"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 160); }
  virtual float OfferModifier() const { return 1; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return 50; }
  virtual void SignalSquarePositionChange(bool);
  virtual void SetOnWall(bool What) { OnWall = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetCategory() const { return TOOL; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1000; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, OnWall ? 192 : 256); }
  virtual ushort GetFormModifier() const { return 30; }
  bool OnWall;
);

class ITEM
(
  can,
  materialcontainer,
  GenerateMaterials(),
  {
    SetSize(10);
  },
 public:
  static ushort Possibility() { return 200; }
  virtual item* TryToOpen(character*);
  virtual std::string NameSingular() const { return "can"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 144); }
  virtual float OfferModifier() const { return 0.5; }
  virtual item* PrepareForConsuming(character*);
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; case 1: return 500; default: return 0; } }
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual item* BetterVersion() const;
  virtual uchar GetCategory() const { return FOOD; }
  virtual bool IsOpenable(character*) const { return true; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 50; }
  virtual ulong GetDefaultContainedVolume() const { return 500; }
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return GetContainedMaterial() ? true : false; }
  virtual std::string Adjective() const { return "empty"; }
  virtual std::string AdjectiveArticle() const { return "an"; }
  virtual bool ShowAdjective() const { return GetContainedMaterial() ? false : true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, GetContainedMaterial() ? 288 : 304); }
  virtual ushort GetFormModifier() const { return 30; }
);

class ITEM
(
  lump,
  item,
  GenerateMaterials(),
  {
    SetSize(10);
  },
 public:
  static ushort Possibility() { return 200; }
  virtual void ReceiveHitEffect(character*, character*);
  virtual material* CreateDipMaterial();
  virtual std::string NameSingular() const { return "lump"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual float OfferModifier() const { return 0.5; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
  virtual uchar GetCategory() const { return FOOD; }
  virtual bool IsDipDestination(character*) const { return true; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 500; }
  virtual std::string PostFix() const { return LumpyPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual bool ShowMaterial() const { return false; }
  virtual ushort GetStrengthModifier() const { return 75; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,48); }
  virtual ushort GetFormModifier() const { return 15; }
);

class ABSTRACT_ITEM
(
  meleeweapon,
  item,
 public:
  virtual void ReceiveHitEffect(character*, character*);
  virtual void DipInto(material*, character*);
  virtual ulong Price() const;
  virtual uchar GetCategory() const { return WEAPON; }
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
  GenerateMaterials(),
  {
    SetSize(150);
  },
 public:
  static ushort Possibility() { return 25; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,16); }
  virtual std::string NameSingular() const { return "long sword"; }
  virtual float OfferModifier() const { return 0.5; }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2400; case 1: return 100; case 2: return 100; default: return 0; } }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 2400; }
  virtual ulong GetDefaultSecondaryVolume() const { return 100; }
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,336); }
  virtual ushort GetFormModifier() const { return 150; }
);

class ITEM
(
  twohandedsword,
  longsword,
  GenerateMaterials(),
  {
    SetSize(175);
  },
 public:
  static ushort Possibility() { return 3; }
  virtual std::string NameSingular() const { return "two-handed sword"; }
  virtual float OfferModifier() const { return 0.25; }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; case 2: return 100; default: return 0; } }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1500; }
  virtual ulong GetDefaultSecondaryVolume() const { return 250; }
  virtual ushort GetStrengthModifier() const { return 250; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,0); }
  virtual ushort GetFormModifier() const { return 175; }
);

class ITEM
(
  curvedtwohandedsword,
  twohandedsword,
  GenerateMaterials(),
  {
    SetSize(175);
  },
 public:
  static ushort Possibility() { return 0; }
  virtual std::string NameSingular() const { return "two-handed sword"; }
  virtual float OfferModifier() const { return 0.25; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; case 2: return 100; default: return 0; } }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 5500; }
  virtual ulong GetDefaultSecondaryVolume() const { return 250; }
  virtual std::string Adjective() const { return "curved"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 250; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,16); }
  virtual ushort GetFormModifier() const { return 200; }
);

class ITEM
(
  valpurusjustifier,
  longsword,
  InitMaterials(new valpurium, new valpurium, 0),
  {
    SetSize(200);
  },
 public:
  static ushort Possibility() { return 0; }
  virtual std::string NameSingular() const { return "broadsword \"Valpurus's Justifier\""; }
  virtual float OfferModifier() const { return 0.5; }
  virtual long Score() const { return 1000; }
  static bool CanBeWished() { return false; }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 6700; case 1: return 300; case 2: return 100; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 6700; }
  virtual ulong GetDefaultSecondaryVolume() const { return 300; }
  virtual std::string Adjective() const { return "holy"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != valpurium::StaticType(); }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 400; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,64); }
  virtual ushort GetFormModifier() const { return 400; }
);

class ITEM
(
  axe,
  meleeweapon,
  GenerateMaterials(),
  {
    SetSize(125);
  },
 public:
  static ushort Possibility() { return 100; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,176); }
  virtual std::string NameSingular() const { return "axe"; }
  virtual float OfferModifier() const { return 0.25; }
  virtual uchar GetWeaponCategory() const { return AXES; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1400; case 1: return 1600; case 2: return 100; default: return 0; } }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1400; }
  virtual ulong GetDefaultSecondaryVolume() const { return 1600; }
  virtual std::string Article() const { return "an"; }
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,256); }
  virtual ushort GetFormModifier() const { return 150; }
);

class ITEM
(
  pickaxe,
  axe,
  GenerateMaterials(),
  {
    SetSize(150);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual std::string NameSingular() const { return "pick-axe"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 64); }
  virtual float OfferModifier() const { return 0.25; }
  virtual bool Apply(character*);
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; case 1: return 2000; case 2: return 100; default: return 0; } }
  virtual bool IsAppliable(character*) const;
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1500; }
  virtual ulong GetDefaultSecondaryVolume() const { return 2000; }
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,96); }
  virtual ushort GetFormModifier() const { return 100; }
);

class ITEM
(
  spear,
  meleeweapon,
  GenerateMaterials(),
  {
    SetSize(200);
  },
 public:
  static ushort Possibility() { return 100; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,96); }
  virtual std::string NameSingular() const { return "spear"; }
  virtual float OfferModifier() const { return 1; }
  virtual uchar GetWeaponCategory() const { return SPEARS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 200; case 1: return 1600; case 2: return 100; default: return 0; } } protected:
  virtual ushort GetStrengthModifier() const { return 75; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,144); }
  virtual ushort GetFormModifier() const { return 200; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 200; }
  virtual ulong GetDefaultSecondaryVolume() const { return 1600; }
);

class ABSTRACT_ITEM
(
  bodyarmor,
  item,
 public:
  //virtual bool CanBeWorn() const { return true; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  virtual ulong Price() const;
  virtual uchar GetCategory() const { return BODYARMOR; }
  virtual bool IsBodyArmor(character*) const { return true; }
 protected:
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  platemail,
  bodyarmor,
  GenerateMaterials(),
  {
    SetSize(75);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual std::string NameSingular() const { return "plate mail"; }
  virtual float OfferModifier() const { return 0.5; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual void GenerateMaterials();
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 4000; }
  virtual ushort GetStrengthModifier() const { return 200; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,128); }
);

class ITEM
(
  chainmail,
  bodyarmor,
  GenerateMaterials(),
  {
    SetSize(75);
  },
 public:
  static ushort Possibility() { return 25; }
  virtual std::string NameSingular() const { return "chain mail"; }
  virtual float OfferModifier() const { return 0.5; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 2000; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,96); }
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
  InitMaterials(new cloth),
  {
    SetSize(60);
  },
 public:
  static ushort Possibility() { return 0; }
  virtual ushort GetStrengthValue() const { return 100; }
  virtual std::string NameSingular() const { return "Shirt of the Golden Eagle"; }
  virtual short CalculateOfferValue(char) const { return 750; }
  virtual long Score() const { return 2500; }
  virtual bool IsGoldenEagleShirt() const { return true; };
  static bool CanBeWished() { return false; }
  virtual bool Destroyable() const { return false; }
  virtual ushort GetEmitation() const { return 333; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool IsConsumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1000; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != cloth::StaticType(); }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; } // not used
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,112); }
);

class ITEM
(
  potion,
  materialcontainer,
  GenerateMaterials(),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 100; }
  virtual std::string NameSingular() const { return "bottle"; }
  virtual float OfferModifier() const { return 0.1f; }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; case 1: return 1500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual item* BetterVersion() const;
  virtual std::string GetConsumeVerb() const { return "drinking"; }
  virtual void DipInto(material*, character*);
  virtual uchar GetCategory() const { return POTION; }
  virtual material* CreateDipMaterial();
  virtual bool IsDippable(character*) const { return !GetContainedMaterial(); }
  virtual void GenerateLeftOvers(character*);
  virtual void GenerateMaterials();
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 60; }
  virtual ulong GetDefaultContainedVolume() const { return 1500; }
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return GetContainedMaterial() ? true : false; }
  virtual std::string Adjective() const { return "empty"; }
  virtual std::string AdjectiveArticle() const { return "an"; }
  virtual bool ShowAdjective() const { return GetContainedMaterial() ? false : true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,160); }
  virtual ushort GetFormModifier() const { return 40; }
);

class ITEM
(
  bananapeals,
  item,
  InitMaterials(new bananapeal),
  {
    SetSize(20);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual std::string NameSingular() const { return "banana peal"; }
  virtual item* BetterVersion() const { return new banana; }
  virtual float OfferModifier() const { return 0; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,112); }
  virtual bool GetStepOnEffect(character *);
  virtual uchar GetCategory() const { return MISC; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 40; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != bananapeal::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,128); }
  virtual ushort GetFormModifier() const { return 70; }
);

class ITEM
(
  brokenbottle,
  item,
  InitMaterials(new glass),
  {
    SetSize(10);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual std::string NameSingular() const { return "bottle"; }
  virtual item* BetterVersion() const;
  virtual float OfferModifier() const { return 0; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual bool GetStepOnEffect(character*);
  virtual uchar GetCategory() const { return MISC; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 60; }
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,160); }
  virtual ushort GetFormModifier() const { return 100; }
);

class ABSTRACT_ITEM
(
  scroll,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual uchar GetCategory() const { return SCROLL; }
  virtual bool IsReadable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 250; }
  virtual ushort GetStrengthModifier() const { return 25; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,176); }
  virtual ushort GetFormModifier() const { return 40; }
);

class ITEM
(
  scrollofcreatemonster,
  scroll,
  InitMaterials(new parchment),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual std::string NameSingular() const { return "scroll of create monster"; }
  virtual std::string NamePlural() const { return "scrolls of create monster"; }
  virtual float OfferModifier() const { return 5; }
  virtual bool Read(character*);
  virtual ulong Price() const { return 20; }
);

class ITEM
(
  scrollofteleport,
  scroll,
  InitMaterials(new parchment),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual std::string NameSingular() const { return "scroll of teleportation"; }
  virtual std::string NamePlural() const { return "scrolls of teleportation"; }
  virtual float OfferModifier() const { return 5; }
  virtual bool Read(character*);
  virtual ulong Price() const { return 50; }
);

class ITEM
(
  scrollofcharging,
  scroll,
  InitMaterials(new parchment),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 5; }
  virtual std::string NameSingular() const { return "scroll of charging"; }
  virtual std::string NamePlural() const { return "scrolls of charging"; }
  virtual float OfferModifier() const { return 5; }
  virtual bool Read(character*);
  virtual ulong Price() const { return 400; }
);

class ITEM
(
  nut,
  item,
  InitMaterials(new humanflesh),
  {
    SetSize(0);
  },
 public:
  static ushort Possibility() { return 5; }
  virtual std::string NameSingular() const { return "nut"; }
  virtual float OfferModifier() const { return 10; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 25; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual uchar GetCategory() const { return FOOD; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 25; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,208); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  leftnutofpetrus,
  nut,
  InitMaterials(new humanflesh),
  {
    SetSize(10);
  },
 public:
  static ushort Possibility() { return 0; }
  virtual bool IsPetrussNut() const { return true; }
  virtual std::string NameSingular() const { return "left nut of Petrus"; }
  virtual long Score() const { return 5000; }
  static item* CreateWishedItem();
  virtual bool Destroyable() const { return false; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ushort GetEmitation() const { return 333; }
  virtual bool IsConsumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
  virtual uchar GetCategory() const { return MISC; }
  static bool SpecialWishedItem() { return true; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 500; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != humanflesh::StaticType(); }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 2500; }
);

class ITEM
(
  abone,
  item,
  InitMaterials(new bone),
  {
    SetSize(50);
  },
 public:
  static ushort Possibility() { return 100; }
  virtual std::string NameSingular() const { return "bone"; }
  virtual float OfferModifier() const { return 0.1f; }
  virtual bool DogWillCatchAndConsume() const { return GetConsumeMaterial()->GetType() == bone::StaticType(); }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,32); }
  virtual uchar GetCategory() const { return MISC; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1500; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != bone::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,240); }
  virtual ushort GetFormModifier() const { return 70; }
);

class ITEM
(
  poleaxe,
  axe,
  GenerateMaterials(),
  {
    SetSize(225);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual std::string NameSingular() const { return "poleaxe"; }
  virtual float OfferModifier() const { return 0.25f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2500; case 1: return 2000; case 2: return 100; default: return 0; } }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 2500; }
  virtual ulong GetDefaultSecondaryVolume() const { return 2000; }
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,80); }
  virtual ushort GetFormModifier() const { return 150; }
);

class ITEM
(
  spikedmace,
  meleeweapon,
  GenerateMaterials(),
  {
    SetSize(150);
  },
 public:
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "spiked mace"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 192); }
  virtual float OfferModifier() const { return 0.125f; }
  virtual uchar GetWeaponCategory() const { return MACES; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; case 1: return 2000; case 2: return 100; default: return 0; } }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 5000; }
  virtual ulong GetDefaultSecondaryVolume() const { return 2000; }
  virtual ushort GetStrengthModifier() const { return 400; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,32); }
  virtual ushort GetFormModifier() const { return 125; }
);

class ITEM
(
  neercseulb,
  spikedmace,
  InitMaterials(new diamond, new diamond, new darkfrogflesh),
  {
    SetSize(200);
  },
 public:
  static ushort Possibility() { return 0; }
  virtual std::string NameSingular() const { return "mace \"Neerc Se-Ulb\""; }
  virtual float OfferModifier() const { return 0.25; }
  virtual long Score() const { return 1000; }
  static bool CanBeWished() { return false; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 12000; case 1: return 6000; case 2: return 2500; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 12000; }
  virtual ulong GetDefaultSecondaryVolume() const { return 6000; }
  virtual ulong GetDefaultContainedVolume() const { return 2500; }
  virtual std::string Adjective() const { return "ancient"; }
  virtual std::string AdjectiveArticle() const { return "an"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != diamond::StaticType(); }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 600; }
  virtual ushort GetFormModifier() const { return 150; }
);

class ITEM
(
  loaf,
  item,
  GenerateMaterials(),
  {
    SetSize(40);
  },
 public:
  static ushort Possibility() { return 100; }
  virtual std::string NameSingular() const { return "loaf"; }
  virtual std::string NamePlural() const { return "loaves"; }
  virtual float OfferModifier() const { return 0.125; }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual ulong Price() const { return GetMainMaterial() ? GetMainMaterial()->RawPrice() : 0; }
  virtual uchar GetCategory() const { return FOOD; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 500; }
  virtual std::string PostFix() const { return LumpyPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual bool ShowMaterial() const { return false; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,272); }
  virtual ushort GetFormModifier() const { return 30; }
);

class ITEM
(
  scrollofwishing,
  scroll,
  InitMaterials(new parchment),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 1; }
  virtual std::string NameSingular() const { return "scroll of wishing"; }
  virtual std::string NamePlural() const { return "scrolls of wishing"; }
  virtual float OfferModifier() const { return 50; }
  static bool CanBeWished() { return false; }
  virtual bool Read(character*);
  virtual ulong Price() const { return 2000; }
  static bool PolymorphSpawnable() { return false; }
  virtual bool IsStackable() const { return true; }
  virtual bool IsEqual(item* Item) const { return Item->GetType() == GetType(); }
);

class ITEM
(
  cheapcopyofleftnutofpetrus,
  nut,
  InitMaterials(new glass),
  {
    SetSize(10);
  },
 public:
  static ushort Possibility() { return 5; }
  virtual std::string NameSingular() const { return "copy of the left nut of Petrus"; }
  virtual std::string NamePlural() const { return "copies of the left nut of Petrus"; }
  virtual long Score() const { return 1; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ulong Price() const { return 500; }
  virtual uchar GetCategory() const { return MISC; }
 protected:
    virtual ulong GetDefaultMainVolume() const { return 500; }
  virtual std::string Adjective() const { return "cheap"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
);

class ABSTRACT_ITEM
(
  wand,
  item,
 public:
  virtual bool Apply(character*);
  //virtual bool CanBeZapped() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 200; default: return 0; } }
  //virtual bool IsExplosive() const { return true; }
  virtual uchar GetCharges() const { return Charges; }
  virtual void SetCharges(uchar What) { Charges = What; }
  virtual uchar GetTimesUsed() const { return TimesUsed; }
  virtual void SetTimesUsed(uchar What) { TimesUsed = What; }
  virtual void Beam(character*, const std::string&, uchar, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*) { return false; };
  virtual ushort GetBeamColor() const = 0;
  virtual void ChargeFully(character*) { SetTimesUsed(0); }
  //virtual bool IsChargable() const { return true; }
  virtual uchar GetCategory() const { return WAND; }
  virtual bool IsAppliable(character*) const { return true; }
  virtual bool IsZappable(character*) const { return true; }
  virtual bool IsChargeable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 200; }
  virtual std::string wand::PostFix() const;
  virtual bool ShowPostFix() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,288); }
  virtual ushort GetFormModifier() const { return 80; }
  uchar Charges;
  uchar TimesUsed;
);

class ITEM
(
  wandofpolymorph,
  wand,
  InitMaterials(new glass),
  {
    SetSize(30);
    SetCharges(2 + RAND() % 5);
    SetTimesUsed(0);
  },
 public:
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "wand of polymorph"; }
  virtual std::string NamePlural() const { return "wands of polymorph"; }
  virtual float OfferModifier() const { return 30; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 500; }
  static bool PolymorphSpawnable() { return false; }
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
  virtual ushort GetBeamColor() const { return BLUE; }
);

/*class ITEM
(
  arrow,
  item,
  InitMaterials(new wood, new iron),
  {
    SetSize(100);
  },
 public:
  static ushort Possibility() { return 200; }
  virtual std::string NameSingular() const { return "arrow"; }
  virtual float OfferModifier() const { return 0.5f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
  virtual bool UseThrowGetStrengthModifier() const { return true; }
 protected:
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,80); }
  virtual ushort GetFormModifier() const { return 50; }
);*/

class ITEM
(
  brokenlantern,
  lantern,
  InitMaterials(new glass),
  {
    SetSize(15);
    SetOnWall(false);
  },
 public:
  static ushort Possibility() { return 25; }
  virtual std::string NameSingular() const { return "lantern"; }
  virtual item* BetterVersion() const { return new lantern; }
  virtual float OfferModifier() const { return 0; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual ushort GetEmitation() const { return 0; }
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
 protected:
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, OnWall ? 208 : 304); }
  virtual ushort GetFormModifier() const { return 80; }
);

class ITEM
(
  scrollofchangematerial,
  scroll,
  InitMaterials(new parchment),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 3; }
  virtual std::string NameSingular() const { return "scroll of change material"; }
  virtual std::string NamePlural() const { return "scrolls of change material"; }
  virtual float OfferModifier() const { return 40; }
  virtual bool Read(character*);
  virtual ulong Price() const { return 500; }
);

class ITEM
(
  avatarofvalpurus,
  item,
  InitMaterials(new valpurium),
  {
    SetSize(10);
  },
 public:
  virtual bool IsTheAvatar() const { return true; }
  static ushort Possibility() { return 0; }
  virtual std::string NameSingular() const { return "Avatar of Valpurus"; }
  static bool CanBeWished() { return false; }
  virtual bool Destroyable() const { return false; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,112); }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool IsConsumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
  virtual ulong Price() const { return 2000000000; }
  virtual uchar GetCategory() const { return MISC; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 250; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != valpurium::StaticType(); }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 400; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,320); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  wandofstriking,
  wand,
  InitMaterials(new marble),
  {
    SetSize(30);
    SetCharges(2 + RAND() % 5);
    SetTimesUsed(0);
  },
 public:
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "wand of striking"; }
  virtual std::string NamePlural() const { return "wands of striking"; }
  virtual float OfferModifier() const { return 10; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 500; }
  virtual ushort GetBeamColor() const { return WHITE; }
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
);

class ITEM
(
  brokenplatemail,
  bodyarmor,
  GenerateMaterials(),
  {
    SetSize(70);
  },
 public:
  static ushort Possibility() { return 10; }
  virtual std::string NameSingular() const { return "plate mail"; }
  virtual float OfferModifier() const { return 0.1f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 4000; }
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 75; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,352); }
  virtual ushort GetFormModifier() const { return 30; }
);

/*class ITEM
(
  bow,
  item,
  InitMaterials(new wood),
  {
    SetSize(50);
  },
 public:
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "bow"; }
  virtual float OfferModifier() const { return 0.3f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual float GetThrowGetStrengthModifier() const { return 4; }
 protected:
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,320); }
  virtual ushort GetFormModifier() const { return 40; }
);*/

class ITEM
(
  kiwi,
  item,
  InitMaterials(new kiwiflesh),
  {  
    SetSize(10);
  },
 public:
  static ushort Possibility() { return 25; }
  virtual std::string NameSingular() const { return "kiwi"; }
  virtual float OfferModifier() const { return 0.4f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
  virtual uchar GetCategory() const { return FOOD; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 50; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != kiwiflesh::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,384); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  pineapple,
  item,
  InitMaterials(new pineappleflesh),
  {
    SetSize(20);
  },
 public:
  static ushort Possibility() { return 25; }
  virtual std::string NameSingular() const { return "pineapple"; }
  virtual float OfferModifier() const { return 0.4f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
  virtual uchar GetCategory() const { return FOOD; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1000; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != pineappleflesh::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,368); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  palmbranch,
  item,
  InitMaterials(new palmleaf),
  {
    SetSize(80);
  },
 public:
  static ushort Possibility() { return 10; }
  virtual std::string NameSingular() const { return "palm branch"; }
  virtual float OfferModifier() const { return 0.3f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 208); }
  virtual uchar GetCategory() const { return MISC; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 4000; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != palmleaf::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,240); }
  virtual ushort GetFormModifier() const { return 50; }
);

class ITEM
(
  whip,
  meleeweapon,
  InitMaterials(new leather, new wood, 0),
  {
    SetSize(200);
  },
 public:
  static ushort Possibility() { return 25; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,224); }
  virtual std::string NameSingular() const { return "whip"; }
  virtual float OfferModifier() const { return 0.5f; }
  virtual uchar GetWeaponCategory() const { return WHIPS; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1000; }
  virtual ulong GetDefaultSecondaryVolume() const { return 200; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32,0); }
  virtual ushort GetFormModifier() const;
);

class ITEM
(
  backpack,
  materialcontainer,
  InitMaterials(new leather, new gunpowder),
  {
    SetSize(80);
  },
 public:
  static ushort Possibility() { return 10; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 144); }
  virtual float OfferModifier() const { return 0.5; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; case 1: return 10000; default: return 0; } }
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual bool Apply(character*);
  //virtual bool IsExplosive() const { return (GetContainedMaterial() && GetContainedMaterial()->GetIsExplosive()) ? true : false; }
  virtual uchar GetCategory() const { return TOOL; }
  virtual bool IsAppliable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1000; }
  virtual ulong GetDefaultContainedVolume() const { return 10000; }
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual ushort GetStrengthModifier() const { return 200; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32, 16); }
  virtual std::string NameSingular() const { return "backpack"; }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  holybook,
  item,
  InitMaterials(new parchment),
  {  
    SetSize(25);

    /* Don't use SetDivineMaster, since it calls UpdatePicture()! */

    DivineMaster = 1 + RAND() % (game::GetGods() - 1);
  },
 public:
  virtual bool CanBeRead(character*) const;
  virtual vector2d GetInHandsPic() const { return vector2d(160, 128); }
  static ushort Possibility() { return 25; }
  virtual float OfferModifier() const { return 0.4f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual void SetDivineMaster(uchar);
  virtual bool Read(character*);
  virtual uchar GetCategory() const { return BOOK; }
  virtual bool IsReadable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 2000; }
  virtual ushort GetMaterialColor0(ushort) const;
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32,32); }
  virtual std::string NameSingular() const { return "holy book"; }
  virtual ushort GetFormModifier() const { return 30; }
  virtual std::string PostFix() const { return DivineMasterDescription(DivineMaster); }
  virtual bool ShowPostFix() const { return true; }
  virtual bool ShowMaterial() const { return false; }
  uchar DivineMaster;
);

class ITEM
(
  fiftymillionroubles,
  item,
  InitMaterials(new parchment),
  {
    SetSize(20);
  },
 public:
  static ushort Possibility() { return 0; }
  virtual std::string NameSingular() const { return "pile of 50 million roubles"; }
  virtual float OfferModifier() const { return 0.01f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return 2; }
  virtual uchar GetCategory() const { return VALUABLE; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1000; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != parchment::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 25; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,336); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  oillamp,
  item,
  InitMaterials(new gold),
  {
    SetSize(30);
    SetInhabitedByGenie(!(rand() % 2));
  },
 public:
  static ushort Possibility() { return 25; }
  virtual ushort GetEmitation() const { return 256; }
  virtual std::string NameSingular() const { return "oil lamp"; }
  virtual float OfferModifier() const { return 1; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 150; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GetInhabitedByGenie() const { return InhabitedByGenie; }
  virtual void SetInhabitedByGenie(bool What) { InhabitedByGenie = What; }
  virtual bool Apply(character*);
  static bool CanBeWished() { return false; }
  virtual uchar GetCategory() const { return TOOL; }
  virtual bool IsAppliable(character*) const { return true; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 150; }
  virtual std::string Article() const { return "an"; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32,48); }
  virtual ushort GetFormModifier() const { return 30; }
  bool InhabitedByGenie;
);

class ITEM
(
  astone,
  item,
  GenerateMaterials(),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 5; }
  virtual std::string NameSingular() const { return "stone"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual float OfferModifier() const { return 0.7f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 150; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice() * 2; }
  virtual uchar GetCategory() const { return VALUABLE; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 100; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,48); }
  virtual ushort GetFormModifier() const { return 45; }
);

class ITEM
(
  wandoffireballs,
  wand,
  InitMaterials(new marble),
  {
    SetSize(30);
    SetCharges(1 + RAND() % 3);
    SetTimesUsed(0);
  },
 public:
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "wand of fireballs"; }
  virtual std::string NamePlural() const { return "wands of fireballs"; }
  virtual float OfferModifier() const { return 15; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 500; }
  virtual ushort GetBeamColor() const { return YELLOW; }
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
);

class ITEM
(
  scrolloftaming,
  scroll,
  InitMaterials(new parchment),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 5; }
  virtual std::string NameSingular() const { return "scroll of taming"; }
  virtual std::string NamePlural() const { return "scrolls of taming"; }
  virtual float OfferModifier() const { return 5; }
  virtual bool Read(character*);
  virtual ulong Price() const { return 350; }
);

class ITEM
(
  wandofteleportation,
  wand,
  InitMaterials(new gold),
  {
    SetSize(30);
    SetCharges(2 + RAND() % 5);
    SetTimesUsed(0);
  },
 public:
  static ushort Possibility() { return 30; }
  virtual std::string NameSingular() const { return "wand of teleportation"; }
  virtual std::string NamePlural() const { return "wands of teleportation"; }
  virtual float OfferModifier() const { return 30; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 500; }
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
  virtual ushort GetBeamColor() const { return GREEN; }
);

class ITEM
(
  mine,
  materialcontainer,
  InitMaterials(new iron, new gunpowder),
  {
    SetSize(5);
    SetCharged(RAND() % 5 ? true : false);
  },
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
  //virtual bool IsExplosive() const { return (GetContainedMaterial() && GetContainedMaterial()->GetIsExplosive()) ? true : false; }
  virtual uchar GetCharged() const { return Charged; }
  virtual void SetCharged(bool What) { Charged = What; }
  //bool IsChargable() const { return true; }
  virtual bool GetStepOnEffect(character *);
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "mine"; }
  virtual uchar GetCategory() const { return TOOL; }
  virtual bool IsChargeable(character*) const { return true; }
  virtual bool ReceiveDamage(character*, short, uchar);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 100; }
  virtual ulong GetDefaultSecondaryVolume() const { return 10000; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != iron::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32,288); }
  virtual ushort GetFormModifier() const { return 30; }
  bool Charged;
);

class ITEM
(
  wandofhaste,
  wand,
  InitMaterials(new iron),
  {
    SetSize(30);
    SetCharges(2 + RAND() % 5);
    SetTimesUsed(0);
  },
 public:
  static ushort Possibility() { return 30; }
  virtual std::string NameSingular() const { return "wand of haste"; }
  virtual std::string NamePlural() const { return "wands of haste"; }
  virtual float OfferModifier() const { return 20; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
  virtual ushort GetBeamColor() const { return RED; }
);

class ITEM
(
  wandofslow,
  wand,
  InitMaterials(new iron),
  {
    SetSize(30);
    SetCharges(2 + RAND() % 5);
    SetTimesUsed(0);
  },
 public:
  static ushort Possibility() { return 30; }
  virtual std::string NameSingular() const { return "wand of slow"; }
  virtual std::string NamePlural() const { return "wands of slow"; }
  virtual float OfferModifier() const { return 20; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
  virtual ushort GetBeamColor() const { return GREEN; }
);

class ITEM
(
  key,
  item,
  InitMaterials(new iron),
  {
   SetSize(10);
   SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
  },
 public:
  static ushort Possibility() { return 50; }
  virtual std::string NameSingular() const { return "key"; }
  virtual float OfferModifier() const { return 0.5; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 15; default: return 0; } }
  virtual ulong Price() const { return 10; }
  virtual bool Apply(character*);
  virtual void SetLockType(uchar What) { LockType = What; }
  virtual uchar GetLockType() const { return LockType; }
  virtual uchar GetCategory() const { return TOOL; }
  virtual bool IsAppliable(character*) const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual ulong GetDefaultMainVolume() const { return 15; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,48); } // Alcohol and Mr. Graphics Guy don't fit together very well
  virtual ushort GetFormModifier() const { return 40; }
  uchar LockType;
);

class ITEM
(
  shield,
  item,
  GenerateMaterials(),
  {  
    SetSize(80);
  },
 public:
  static ushort Possibility() { return 30; }
  virtual std::string NameSingular() const { return "shield"; }
  virtual float OfferModifier() const { return 0.4f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the shield can be calculated somehow
  virtual uchar GetCategory() const { return SHIELD; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 1500; }
  virtual ushort GetStrengthModifier() const { return 200; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32,336); }
  virtual ushort GetFormModifier() const { return 30; }
);

class ITEM
(
  cloak,
  item,
  GenerateMaterials(),
  {  
    SetSize(200);
  },
 public:
  static ushort Possibility() { return 40; }
  virtual std::string NameSingular() const { return "cloak"; }
  virtual float OfferModifier() const { return 0.4f; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the cloak can be calculated somehow
  virtual uchar GetCategory() const { return CLOAK; }
  virtual bool IsCloak(character*) const { return true; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 4000; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32,352); }
  virtual ushort GetFormModifier() const { return 20; } 
);

class ITEM
(
  boot,
  item,
  GenerateMaterials(),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 40; }
  virtual std::string NameSingular() const { return "boot"; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 600; default: return 0; } } 
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the boots can be calculated someho
  virtual uchar GetCategory() const { return BOOT; }
  virtual bool IsBoot(character*) const { return true; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 600; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 400); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  gauntlet, 
  item,
  GenerateMaterials(),
  {
    SetSize(30);
  },
 public:
  static ushort Possibility() { return 40; }
  virtual std::string NameSingular() const { return "gauntlet"; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 400; default: return 0; } } 
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the gauntlets can be calculated somehow
  virtual uchar GetCategory() const { return GAUNTLET; }
  virtual bool IsGauntlet(character*) const { return true; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 400; }
  virtual ushort GetStrengthModifier() const { return 75; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32, 368); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  belt, 
  item,
  GenerateMaterials(),
  {
    SetSize(150);
  },
 public:
  static ushort Possibility() { return 40; }
  virtual std::string NameSingular() const { return "belt"; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } } 
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual uchar GetWeaponCategory() const { return WHIPS; }
  virtual uchar GetCategory() const { return BELT; }
  virtual bool IsBelt(character*) const { return true; }
  virtual void GenerateMaterials();
 protected:
  virtual ulong GetDefaultMainVolume() const { return 250; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32, 384); }
  virtual ushort GetFormModifier() const;
);

class ITEM
(
  ring, 
  item,
  InitMaterials(new diamond),
  {
    SetSize(2);
  },
 public:
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "ring"; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 3; default: return 0; } } 
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual uchar GetCategory() const { return RING; }
  virtual bool IsRing(character*) const { return true; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 3; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, 400); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  amulet, 
  item,
  InitMaterials(new gold),
  {
    SetSize(20);
  },
 public:
  static ushort Possibility() { return 10; }
  virtual std::string NameSingular() const { return "amulet"; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } } 
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual uchar GetCategory() const { return AMULET; }
  virtual bool IsAmulet(character*) const { return true; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 100; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32, 400); }
  virtual ushort GetFormModifier() const { return 50; }
);

class ABSTRACT_ITEM
(
  bodypart,
  materialcontainer,
 public:
  friend class corpse;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static ushort Possibility() { return 0; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 0; default: return 0; } }
  //virtual void SetBitmapPos(vector2d What) { BitmapPos = What; }
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRHUMANOID; }
  //virtual void SetColor(ushort Index, ushort What) { Color[Index] = What; }
  virtual character* GetMaster() const;
  virtual humanoid* GetHumanoidMaster() const;
  static bool AutoInitializable() { return false; }
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
  virtual uchar GetCategory() const { return FOOD; }
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
  virtual float NPModifier() const { return 0.01f; }
  std::string OwnerDescription;
  //vector2d BitmapPos;
  //ushort Color[4];
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
  InitMaterials(new humanflesh, new bone),
  {
    HelmetSlot.Init(this);
    AmuletSlot.Init(this);
    SetUnique(false);
    SetRegenerationCounter(0);
  },
 public:
  virtual ~head();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual float OfferModifier() const { return 0.1f; }
  static bool CanBeWished() { return false; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
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
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual std::string NameSingular() const { return "head"; }
  virtual ushort GetFormModifier() const { return 20; }
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
  virtual ushort GetStrengthModifier() const { return 250; }
  virtual std::string NameSingular() const { return "torso"; }
);

class ITEM
(
  normaltorso,
  torso,
  ;,
  {
    SetUnique(false);
    SetRegenerationCounter(0);
  },
 public:
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRCHARACTER; }
  virtual ushort GetTotalResistance(uchar) const;
);

class ITEM
(
  humanoidtorso,
  torso,
  ;,
  {
    BodyArmorSlot.Init(this);
    CloakSlot.Init(this);
    BeltSlot.Init(this);
    SetUnique(false);
    SetRegenerationCounter(0);
  },
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
  virtual ushort GetStrengthModifier() const { return 50; }
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
  ;,
  {
    WieldedSlot.Init(this);
    GauntletSlot.Init(this);
    RingSlot.Init(this);
    SetCurrentSingleWeaponSkill(0);
    SetHasBe(true);
    SetUnique(false);
    SetRegenerationCounter(0);
  },
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
  ;,
  {
    WieldedSlot.Init(this);
    GauntletSlot.Init(this);
    RingSlot.Init(this);
    SetCurrentSingleWeaponSkill(0);
    SetHasBe(true);
    SetUnique(false);
    SetRegenerationCounter(0);
  },
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
  ;,
  {
    SetUnique(false);
    SetRegenerationCounter(0);
  },
 public:
  virtual ushort GetTotalResistance(uchar) const;
  virtual ushort DangerWeight() const;
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual ushort GetStrengthModifier() const { return 100; }
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
  virtual ushort GetStrengthModifier() const { return 75; }
  gearslot BootSlot;
);

class ITEM
(
  rightleg,
  leg,
  ;,
  {
    BootSlot.Init(this);
    SetUnique(false);
    SetRegenerationCounter(0);
  },
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
  ;,
  {
    BootSlot.Init(this);
    SetUnique(false);
    SetRegenerationCounter(0);
  },
 public:
  virtual bool FitsBodyPartIndex(uchar, character*) const;
 protected:
  virtual uchar GetSpecialType(ushort) const { return STLEFTLEG; }
  virtual std::string NameSingular() const { return "left leg"; }
);

class ITEM
(
  headofelpuri,
  head,
  InitMaterials(new elpuriflesh, new bone),
  {
    HelmetSlot.Init(this);
    AmuletSlot.Init(this);
    SetSize(60);
    SetUnique(true);
    SetOwnerDescription("of Elpuri, the Master Dark Frog");
  },
 public:
  static ushort Possibility() { return 0; }
  virtual bool IsHeadOfElpuri() const { return true; }
  virtual long Score() const { return 1000; };
  static bool CanBeWished() { return false; }
  virtual bool Destroyable() const { return false; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 25000; default: return 0; } }
  virtual bool IsConsumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
  static bool AutoInitializable() { return true; }
  virtual uchar GetCategory() const { return MISC; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 25000; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16,0); }
);

class ITEM
(
  headofennerbeast,
  head,
  InitMaterials(new ennerbeastflesh, new bone),
  {
    HelmetSlot.Init(this);
    AmuletSlot.Init(this);
    SetSize(50);
    SetUnique(false);
    SetOwnerDescription("of an enner beast");
  },
 public:
  static ushort Possibility() { return 5; }
  virtual long Score() const { return 250; };
  static bool CanBeWished() { return true; }
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; default: return 0; } }
  virtual ulong Price() const { return 100; }
  static bool AutoInitializable() { return true; }
 protected:
  virtual ulong GetDefaultMainVolume() const { return 5000; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0,176); }
);

class ITEM
(
  corpse,
  item,
  /*InitMaterials(new humanflesh)*/;,
  {
    /*SetSize(0);
    SetBloodColor(MAKE_RGB(75,0,0));*/
  },
 public:
  virtual ~corpse();
  virtual bool Consume(character*, float);
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

  static ushort Possibility() { return 0; }
  virtual std::string NameSingular() const { return "corpse"; }
  virtual float OfferModifier() const { return 0.01f; }
  //virtual material* CreateDipMaterial();
  //virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  //virtual ulong Price() const { return GetMainMaterial() ? GetMainMaterial()->RawPrice() : 0; }
  //virtual bool CatWillCatchAndConsume() const { return GetConsumeMaterial()->GetType() == ratflesh::StaticType(); }
  virtual uchar GetCategory() const { return FOOD; }
  virtual bool IsDipDestination(character*) const { return true; }
  virtual character* GetDeceased() const { return Deceased; }
  virtual void SetDeceased(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void AddConsumeEndMessage(character*) const;
  virtual void GenerateLeftOvers(character*);
  virtual long Score() const;
  static bool CanBeWished() { return false; }
  static bool AutoInitializable() { return false; }
  static bool PolymorphSpawnable() { return false; }
  virtual ulong GetWeight() const;
  virtual bool Destroyable() const;
  virtual ulong Price() const;
  virtual item* PrepareForConsuming(character*);
  virtual uchar GetMaterials() const { return 2; }
  virtual material* GetMaterial(ushort) const;
  virtual bool RaiseTheDead(character*);
 protected:
  virtual ushort GetMaterialColor0(ushort) const;
  virtual ushort GetMaterialColor1(ushort) const;
  virtual bool ShowMaterial() const { return false; }
  virtual std::string PostFix() const;
  virtual bool ShowPostFix() const { return true; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos(ushort) const;
  virtual ushort GetSize() const;
  virtual ushort GetFormModifier() const { return 15; }
  character* Deceased;
);

class ITEM
(
  wandoflocking,
  wand,
  InitMaterials(new copper),
  {
    SetSize(30);
    SetCharges(2 + RAND() % 5);
    SetTimesUsed(0);
  },
 public:
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "wand of locking"; }
  virtual std::string NamePlural() const { return "wands of locking"; }
  virtual float OfferModifier() const { return 10; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 70; }
  virtual ushort GetBeamColor() const { return WHITE; }
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
);

class ITEM
(
  wandofresurrection,
  wand,
  InitMaterials(new bone),
  {
    SetSize(30);
    SetCharges(1 + RAND() % 2);
    SetTimesUsed(0);
  },
 public:
  static ushort Possibility() { return 20; }
  virtual std::string NameSingular() const { return "wand of resurrection"; }
  virtual std::string NamePlural() const { return "wands of resurrection"; }
  virtual float OfferModifier() const { return 10; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 400; }
  virtual ushort GetBeamColor() const { return BLACK; }
  virtual bool BeamEffect(character*, const std::string&, uchar, lsquare*);
);

class ITEM
(
 ringoffireresistance,
 ring,
 InitMaterials(new gold),
 {
  SetSize(3);
 },
 public:
  static ushort Possibility() { return 15; }
  virtual std::string NameSingular() const { return "ring of fire resistance"; }
  virtual std::string NamePlural() const { return "rings of fire resistance"; }
  virtual ushort FireResistance() const { return 10; }
);

#endif


