#ifndef __ITEMDE_H__
#define __ITEMDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "itemba.h"
#include "materde.h"
#include "wskill.h"
#include "game.h"
#include "lterrade.h"
#include "slot.h"

class ITEM
(
  banana,
  item,
  InitMaterials(2, new bananapeal, new bananaflesh),
  {
    SetSize(20);
    SetCharges(6);
  },
 public:
  virtual ushort Possibility() const { return 100; }
  //virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", bananapeal::StaticType()); }
  virtual uchar GetConsumeType() const { return GetContainedMaterial()->GetConsumeType(); }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual bool Consume(character*, float);
  virtual std::string NameSingular() const { return "banana"; }
  virtual float OfferModifier() const { return 1; }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; case 1: return 150; default: return 0; } }
  virtual ulong Price() const { return GetContainedMaterial()->RawPrice(); }
  virtual ulong ConsumeLimit() const { return GetContainedMaterial()->GetVolume(); }
  virtual uchar GetConsumeMaterial() const { return 1; }
  virtual bool CanBeZapped() const { return true; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual uchar GetCharges() const { return Charges; }
  virtual void SetCharges(uchar What) { Charges = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsChargable() const { return true; }
  virtual void ChargeFully(character*) { SetCharges(6); }
 protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,112); }
  virtual ushort GetFormModifier() const { return 50; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != bananapeal::StaticType(); }
  uchar Charges;
);

class ITEM
(
  holybanana,
  banana,
  InitMaterials(2, new bananapeal, new bananaflesh),
  {
    SetSize(30);
  },
 public:
  virtual ushort Possibility() const { return 5; }
  //virtual std::string Name(uchar Case) const { return NameArtifact(Case, bananapeal::StaticType()); }
  virtual std::string NameSingular() const { return "banana of Liukas Vipro"; }
  virtual std::string NamePlural() const { return "bananas of Liukas Vipro"; }
  virtual float OfferModifier() const { return 40; }
  virtual long Score() const { return 250; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 400; case 1: return 1500; default: return 0; } }
  virtual ulong Price() const { return 1000; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string Adjective() const { return "holy"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != bananapeal::StaticType(); }
  //virtual std::string Article() const { return "the"; }
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
    UpdatePicture();
  },
 public:
  virtual ushort Possibility() const { return 50; }
  virtual void PositionedDrawToTileBuffer(uchar) const;
  virtual ushort GetEmitation() const { return 300; }
  virtual std::string NameSingular() const { return "lantern"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 160); }
  virtual float OfferModifier() const { return 1; }
  virtual bool ImpactDamage(ushort);
  virtual bool ReceiveSound(float);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return 50; }
  virtual void SignalSquarePositionChange(bool);
  virtual void SetOnWall(bool What) { OnWall = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, OnWall ? 192 : 256); }
  virtual ushort GetFormModifier() const { return 30; }
  bool OnWall;
);

class ITEM
(
  can,
  item,
  InitMaterials(2, new iron, RAND() % 10 ? (material*)new bananaflesh : (material*)new schoolfood),
  {
    SetSize(10);
  },
 public:
  virtual ushort Possibility() const { return 200; }
  virtual void PositionedDrawToTileBuffer(uchar) const;
  //virtual std::string Name(uchar Case) const { return NameContainer(Case); }
  virtual item* TryToOpen(character*, stack*);
  virtual uchar GetConsumeType() const { return GetContainedMaterial() ? GetContainedMaterial()->GetConsumeType() : ODD; }
  virtual std::string NameSingular() const { return "can"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 144); }
  virtual float OfferModifier() const { return 0.5; }
  virtual item* PrepareForConsuming(character*, stack*);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; case 1: return 500; default: return 0; } }
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual item* BetterVersion() const;
  virtual uchar GetConsumeMaterial() const { return 1; }
 protected:
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(16, GetContainedMaterial() ? 288 : 304); }
  virtual ushort GetFormModifier() const { return 30; }
);

class ITEM
(
  lump,
  item,
  InitMaterials(RAND() % 10 ? (material*)new bananaflesh : (material*)new schoolfood),
  {
    SetSize(10);
  },
 public:
  virtual ushort Possibility() const { return 200; }
  //virtual std::string Name(uchar Case) const { return NameThingsThatAreLikeLumps(Case, "a"); }
  virtual void ReceiveHitEffect(character*, character*);
  virtual bool CanBeDippedInto(item*) const { return true; }
  virtual material* BeDippedInto();
  virtual std::string NameSingular() const { return "lump"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual float OfferModifier() const { return 0.5; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
 protected:
  virtual std::string PostFix() const { return LumpyPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual bool ShowMaterial() const { return false; }
  virtual ushort GetStrengthModifier() const { return 75; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,48); }
  virtual ushort GetFormModifier() const { return 15; }
);

class ABSTRACT_ITEM
(
  meleeweapon,
  item,
 public:
  virtual void ReceiveHitEffect(character*, character*);
  virtual void DipInto(item*);
  virtual bool CanBeDippedInto(item*) const { return GetMaterial(2) ? false : true; }
  virtual bool CanBeDipped() const { return true; }
  virtual ulong Price() const;
);

class ITEM
(
  longsword,
  meleeweapon,
  InitMaterials(3, RAND() % 50 ? (material*)new iron : (material*)new mithril, new iron, 0),
  {
    SetSize(150);
  },
 public:
  virtual ushort Possibility() const { return 25; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,16); }
  virtual std::string NameSingular() const { return "long sword"; }
  virtual float OfferModifier() const { return 0.5; }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2400; case 1: return 100; case 2: return 100; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,336); }
  virtual ushort GetFormModifier() const { return 150; }
);

class ITEM
(
  twohandedsword,
  longsword,
  InitMaterials(3, RAND() % 50 ? (material*)new iron : (material*)new mithril, new iron, 0),
  {
    SetSize(175);
  },
 public:
  virtual ushort Possibility() const { return 3; }
  virtual std::string NameSingular() const { return "two-handed sword"; }
  virtual float OfferModifier() const { return 0.25; }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; case 2: return 100; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 250; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,0); }
  virtual ushort GetFormModifier() const { return 175; }
);

class ITEM
(
  curvedtwohandedsword,
  twohandedsword,
  InitMaterials(3, new iron, new iron, 0),
  {
    SetSize(175);
  },
 public:
  virtual ushort Possibility() const { return 0; }
  virtual std::string NameSingular() const { return "two-handed sword"; }
  virtual float OfferModifier() const { return 0.25; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; case 2: return 100; default: return 0; } }
 protected:
  virtual std::string Adjective() const { return "curved"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 250; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,16); }
  virtual ushort GetFormModifier() const { return 200; }
);

class ITEM
(
  valpurusjustifier,
  longsword,
  InitMaterials(3, new valpurium, new valpurium, 0),
  {
    SetSize(200);
  },
 public:
  virtual ushort Possibility() const { return 0; }
  //virtual std::string Name(uchar Case) const { return NameArtifact(Case, valpurium::StaticType()); }
  virtual std::string NameSingular() const { return "broadsword \"Valpurus's Justifier\""; }
  virtual float OfferModifier() const { return 0.5; }
  virtual long Score() const { return 1000; }
  virtual bool CanBeWished() const { return false; }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 6700; case 1: return 300; case 2: return 100; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string Adjective() const { return "holy"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != valpurium::StaticType(); }
  //virtual std::string Article() const { return "the"; }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 400; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,64); }
  virtual ushort GetFormModifier() const { return 400; }
);

class ITEM
(
  axe,
  meleeweapon,
  InitMaterials(3, RAND() % 5 ? (material*)new stone : RAND() % 20 ? (material*)new iron : (material*)new mithril, new wood, 0),
  {
    SetSize(125);
  },
 public:
  virtual ushort Possibility() const { return 100; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,176); }
  virtual std::string NameSingular() const { return "axe"; }
  virtual float OfferModifier() const { return 0.25; }
  virtual uchar GetWeaponCategory() const { return AXES; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1400; case 1: return 1600; case 2: return 100; default: return 0; } }
 protected:
  virtual std::string Article() const { return "an"; }
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,256); }
  virtual ushort GetFormModifier() const { return 150; }
);

class ITEM
(
  pickaxe,
  axe,
  InitMaterials(3, RAND() % 50 ? (material*)new iron : (material*)new mithril, new wood, 0),
  {
    SetSize(150);
  },
 public:
  virtual ushort Possibility() const { return 50; }
  virtual std::string NameSingular() const { return "pick-axe"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 64); }
  virtual float OfferModifier() const { return 0.25; }
  virtual bool Apply(character*, stack*);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; case 1: return 2000; case 2: return 100; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,96); }
  virtual ushort GetFormModifier() const { return 100; }
);

class ITEM
(
  spear,
  meleeweapon,
  InitMaterials(3, RAND() % 5 ? (material*)new stone : RAND() % 20 ? (material*)new iron : (material*)new mithril, new wood, 0),
  {
    SetSize(200);
  },
 public:
  virtual ushort Possibility() const { return 100; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,96); }
  virtual std::string NameSingular() const { return "spear"; }
  virtual float OfferModifier() const { return 1; }
  virtual uchar GetWeaponCategory() const { return SPEARS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 200; case 1: return 1600; case 2: return 100; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 75; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,144); }
  virtual ushort GetFormModifier() const { return 200; }
);

class ABSTRACT_ITEM
(
  bodyarmor,
  item,
 public:
  virtual bool CanBeWorn() const { return true; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  virtual ulong Price() const;
 protected:
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  platemail,
  bodyarmor,
  InitMaterials(new iron),
  {
    SetSize(75);
  },
 public:
  virtual ushort Possibility() const { return 3; }
  //virtual ushort GetArmorValue() const;
  virtual std::string NameSingular() const { return "plate mail"; }
  virtual float OfferModifier() const { return 0.5; }
  virtual bool ImpactDamage(ushort);
  virtual bool ReceiveSound(float);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 200; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,128); }
);

class ITEM
(
  chainmail,
  bodyarmor,
  InitMaterials(new iron),
  {
    SetSize(75);
  },
 public:
  virtual ushort Possibility() const { return 50; }
  //virtual ushort GetArmorValue() const;
  virtual std::string NameSingular() const { return "chain mail"; }
  virtual float OfferModifier() const { return 0.5; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,96); }
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
  //virtual std::string Name(uchar Case) const { return NameArtifact(Case, cloth::StaticType()); }
  virtual ushort Possibility() const { return 0; }
  virtual ushort GetStrengthValue() const { return 100; }
  virtual std::string NameSingular() const { return "Shirt of the Golden Eagle"; }
  virtual short CalculateOfferValue(char) const { return 750; }
  virtual long Score() const { return 2500; }
  virtual bool IsGoldenEagleShirt() const { return true; };
  virtual bool CanBeWished() const { return false; }
  virtual bool Destroyable() const { return false; }
  virtual ushort GetEmitation() const { return 333; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Consumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != cloth::StaticType(); }
  //virtual std::string Article() const { return "the"; }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; } // not used
  virtual vector2d GetBitmapPos() const { return vector2d(16,112); }
);

class ITEM
(
  corpse,
  item,
  InitMaterials(new humanflesh),
  {
    SetSize(0);
    SetBloodColor(MAKE_RGB(75,0,0));
  },
 public:
  virtual ushort Possibility() const { return 10; }
  virtual std::string NameSingular() const { return "corpse"; }
  virtual float OfferModifier() const { return 0.01f; }
  virtual bool CanBeDippedInto(item*) const { return true; }
  virtual material* BeDippedInto();
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  virtual ulong Price() const { return GetMainMaterial() ? GetMainMaterial()->RawPrice() : 0; }
  virtual void SetBloodColor(ushort);
  virtual bool CatWillCatchAndConsume() const { return GetMainMaterial()->GetType() == ratflesh::StaticType(); }
 protected:
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,192); }
  virtual ushort GetFormModifier() const { return 15; }
  virtual float NPModifier() const { return 0.01f; }
);

class ITEM
(
  potion,
  item,
  //  InitMaterials(2, new glass, RAND() % 2 ? (material*)new water : (material*)new omleurine),
  GeneratePotionMaterials(),
  {
    SetSize(30);
  },
 public:
  virtual ushort Possibility() const { return 100; }
  virtual uchar GetConsumeType() const { return GetContainedMaterial() ? GetContainedMaterial()->GetConsumeType() : ODD; }
  virtual bool Consume(character*, float);
  //virtual std::string Name(uchar Case) const { return NameContainer(Case); }
  virtual std::string NameSingular() const { return "bottle"; }
  virtual bool ImpactDamage(ushort);
  virtual void PositionedDrawToTileBuffer(uchar) const;
  virtual float OfferModifier() const { return 0.1f; }
  virtual bool ReceiveSound(float);
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; case 1: return 1500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual ulong ConsumeLimit() const { return GetContainedMaterial()->GetVolume(); }
  virtual uchar GetConsumeMaterial() const { return 1; }
  virtual item* BetterVersion() const;
  virtual std::string GetConsumeVerb() const { return std::string("drinking"); }
  virtual void GeneratePotionMaterials();
  virtual bool HasBeenDippedInFountain(character*,fountain*);
  virtual bool CanBeDipped() const { return true; }
 protected:
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,160); }
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
  virtual ushort Possibility() const { return 50; }
  //virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", bananapeal::StaticType()); }
  virtual std::string NameSingular() const { return "banana peal"; }
  virtual item* BetterVersion() const { return new banana; }
  virtual float OfferModifier() const { return 0; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,112); }
  virtual bool GetStepOnEffect(character *);
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != bananapeal::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,128); }
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
  virtual ushort Possibility() const { return 50; }
  virtual std::string NameSingular() const { return "bottle"; }
  virtual item* BetterVersion() const;
  virtual float OfferModifier() const { return 0; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual bool GetStepOnEffect(character*);
 protected:
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,160); }
  virtual ushort GetFormModifier() const { return 100; }
);

class ABSTRACT_ITEM
(
  scroll,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
 protected:
  virtual ushort GetStrengthModifier() const { return 25; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,176); }
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
  virtual ushort Possibility() const { return 100; }
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
  virtual ushort Possibility() const { return 50; }
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
  virtual ushort Possibility() const { return 5; }
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
  virtual ushort Possibility() const { return 5; }
  virtual std::string NameSingular() const { return "nut"; }
  virtual float OfferModifier() const { return 10; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 25; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
 protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,208); }
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
  virtual ushort Possibility() const { return 0; }
  virtual bool IsPetrussNut() const { return true; }
  //virtual std::string Name(uchar Case) const { return NameArtifact(Case, humanflesh::StaticType()); }
  virtual std::string NameSingular() const { return "left nut of Petrus"; }
  virtual long Score() const { return 5000; }
  virtual item* CreateWishedItem() const;
  virtual bool Destroyable() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ushort GetEmitation() const { return 333; }
  virtual bool Consumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != humanflesh::StaticType(); }
  //virtual std::string Article() const { return "the"; }
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
  virtual ushort Possibility() const { return 100; }
  virtual std::string NameSingular() const { return "bone"; }
  //virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", bone::StaticType()); }
  virtual float OfferModifier() const { return 0.1f; }
  virtual bool DogWillCatchAndConsume() const { return GetMainMaterial()->GetType() == bone::StaticType(); }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,32); }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != bone::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,240); }
  virtual ushort GetFormModifier() const { return 70; }
);

class ITEM
(
  poleaxe,
  axe,
  InitMaterials(3, RAND() % 50 ? (material*)new iron : (material*)new mithril, new wood, 0),
  {
    SetSize(225);
  },
 public:
  virtual ushort Possibility() const { return 50; }
  virtual std::string NameSingular() const { return "poleaxe"; }
  virtual float OfferModifier() const { return 0.25f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2500; case 1: return 2000; case 2: return 100; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,80); }
  virtual ushort GetFormModifier() const { return 150; }
);

class ITEM
(
  spikedmace,
  meleeweapon,
  InitMaterials(3, RAND() % 50 ? (material*)new iron : (material*)new mithril, new wood, 0),
  {
    SetSize(150);
  },
 public:
  virtual ushort Possibility() const { return 20; }
  virtual std::string NameSingular() const { return "spiked mace"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 192); }
  virtual float OfferModifier() const { return 0.125f; }
  virtual uchar GetWeaponCategory() const { return MACES; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; case 1: return 2000; case 2: return 100; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 400; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,32); }
  virtual ushort GetFormModifier() const { return 125; }
);

class ITEM
(
  neercseulb,
  spikedmace,
  InitMaterials(3, new diamond, new diamond, new darkfrogflesh),
  {
    SetSize(200);
  },
 public:
  virtual ushort Possibility() const { return 0; }
  //virtual std::string Name(uchar Case) const { return NameArtifact(Case, diamond::StaticType()); }
  virtual std::string NameSingular() const { return "mace \"Neerc Se-Ulb\""; }
  virtual float OfferModifier() const { return 0.25; }
  virtual long Score() const { return 1000; }
  virtual bool CanBeWished() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 12000; case 1: return 6000; case 2: return 2500; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
  virtual std::string Adjective() const { return "ancient"; }
  virtual std::string AdjectiveArticle() const { return "an"; }
  virtual bool ShowAdjective() const { return true; }
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != diamond::StaticType(); }
  //virtual std::string Article() const { return "the"; }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 600; }
  virtual ushort GetFormModifier() const { return 150; }
);

class ITEM
(
  loaf,
  item,
  InitMaterials(CreateLoafMaterials()),
  {
    SetSize(40);
  },
 public:
  virtual ushort Possibility() const { return 100; }
  //virtual std::string Name(uchar Case) const { return NameThingsThatAreLikeLumps(Case, "a"); }
  virtual std::string NameSingular() const { return "loaf"; }
  virtual std::string NamePlural() const { return "loaves"; }
  virtual float OfferModifier() const { return 0.125; }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual ulong Price() const { return GetMainMaterial() ? GetMainMaterial()->RawPrice() : 0; }
  virtual material* CreateLoafMaterials();
 protected:
  virtual std::string PostFix() const { return LumpyPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual bool ShowMaterial() const { return false; }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,272); }
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
  virtual ushort Possibility() const { return 1; }
  virtual std::string NameSingular() const { return "scroll of wishing"; }
  virtual std::string NamePlural() const { return "scrolls of wishing"; }
  virtual float OfferModifier() const { return 50; }
  virtual bool CanBeWished() const { return false; }
  virtual bool Read(character*);
  virtual ulong Price() const { return 2000; }
  virtual bool PolymorphSpawnable() const { return false; }
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
  virtual ushort Possibility() const { return 5; }
  virtual std::string NameSingular() const { return "copy of the left nut of Petrus"; }
  virtual std::string NamePlural() const { return "copies of the left nut of Petrus"; }
  virtual long Score() const { return 1; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ulong Price() const { return 500; } 
 protected:
  virtual std::string Adjective() const { return "cheap"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
);

class ABSTRACT_ITEM
(
  wand,
  item,
 public:
  virtual bool Apply(character*, stack*);
  virtual bool CanBeZapped() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
  virtual bool IsExplosive() const { return true; }
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
  //virtual std::string Name(uchar Case) const;
  virtual uchar GetCharges() const { return Charges; }
  virtual void SetCharges(uchar What) { Charges = What; }
  virtual uchar GetTimesUsed() const { return TimesUsed; }
  virtual void SetTimesUsed(uchar What) { TimesUsed = What; }
  virtual void Beam(character*, std::string, uchar, uchar);
  virtual bool BeamEffect(character*, std::string, uchar, lsquare*) { return false; };
  virtual ushort GetBeamColor() const = 0;
  virtual bool StruckByWandOfStriking(character*, std::string);
  virtual void ChargeFully(character*) { SetTimesUsed(0); }
  virtual bool IsChargable() const { return true; }
 protected:
  virtual std::string wand::PostFix() const;
  virtual bool ShowPostFix() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,288); }
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
    SetSize(10);
    SetCharges(2 + RAND() % 4);
    SetTimesUsed(0);
  },
 public:
  virtual ushort Possibility() const { return 20; }
  virtual std::string NameSingular() const { return "wand of polymorph"; }
  virtual std::string NamePlural() const { return "wands of polymorph"; }
  virtual float OfferModifier() const { return 30; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 500; }
  virtual bool PolymorphSpawnable() const { return false; }
  virtual bool BeamEffect(character*, std::string, uchar, lsquare*);
  virtual ushort GetBeamColor() const { return BLUE; }
);

/*class ITEM
(
  arrow,
  item,
  InitMaterials(2, new wood, new iron),
  {
    SetSize(100);
  },
 public:
  virtual ushort Possibility() const { return 200; }
  virtual std::string NameSingular() const { return "arrow"; }
  virtual float OfferModifier() const { return 0.5f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
  virtual bool UseThrowGetStrengthModifier() const { return true; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(16,80); }
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
    UpdatePicture();
  },
 public:
  virtual ushort Possibility() const { return 25; }
  virtual std::string NameSingular() const { return "lantern"; }
  virtual item* BetterVersion() const { return new lantern; }
  virtual float OfferModifier() const { return 0; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual bool ImpactDamage(ushort) { return false; }
  virtual ushort GetEmitation() const { return 0; }
 protected:
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, OnWall ? 208 : 304); }
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
  virtual ushort Possibility() const { return 3; }
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
  //virtual std::string Name(uchar Case) const { return NameArtifact(Case, valpurium::StaticType()); }
  virtual ushort Possibility() const { return 0; }
  virtual std::string NameSingular() const { return "Avatar of Valpurus"; }
  virtual bool CanBeWished() const { return false; }
  virtual bool Destroyable() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,112); }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Consumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
  virtual ulong Price() const { return 2000000000; }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != valpurium::StaticType(); }
  //virtual std::string Article() const { return "the"; }
  virtual bool ForceDefiniteArticle() const { return true; }
  virtual ushort GetStrengthModifier() const { return 400; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,320); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  wandofstriking,
  wand,
  InitMaterials(new marble),
  {
    SetSize(12);
    SetCharges(1 + RAND() % 3);
    SetTimesUsed(0);
  },
 public:
  virtual ushort Possibility() const { return 20; }
  virtual std::string NameSingular() const { return "wand of striking"; }
  virtual std::string NamePlural() const { return "wands of striking"; }
  virtual float OfferModifier() const { return 10; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 500; }
  virtual ushort GetBeamColor() const { return WHITE; }
  virtual bool BeamEffect(character*, std::string, uchar, lsquare*);
);

class ITEM
(
  brokenplatemail,
  bodyarmor,
  InitMaterials(new iron),
  {
    SetSize(70);
  },
 public:
  virtual ushort Possibility() const { return 10; }
  virtual std::string NameSingular() const { return "plate mail"; }
  virtual float OfferModifier() const { return 0.1f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  //virtual ushort GetArmorValue() const;
 protected:
  virtual std::string Adjective() const { return "broken"; }
  virtual bool ShowAdjective() const { return true; }
  virtual ushort GetStrengthModifier() const { return 75; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,352); }
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
  virtual ushort Possibility() const { return 20; }
  virtual std::string NameSingular() const { return "bow"; }
  virtual float OfferModifier() const { return 0.3f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual float GetThrowGetStrengthModifier() const { return 4; }
 protected:
  virtual vector2d GetBitmapPos() const { return vector2d(16,320); }
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
  //virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", kiwiflesh::StaticType()); }
  virtual ushort Possibility() const { return 25; }
  virtual std::string NameSingular() const { return "kiwi"; }
  virtual float OfferModifier() const { return 0.4f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != kiwiflesh::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,384); }
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
  //virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", pineappleflesh::StaticType()); }
  virtual ushort Possibility() const { return 25; }
  virtual std::string NameSingular() const { return "pineapple"; }
  virtual float OfferModifier() const { return 0.4f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != pineappleflesh::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,368); }
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
  //virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", palmleaf::StaticType()); }
  virtual ushort Possibility() const { return 10; }
  virtual std::string NameSingular() const { return "palm branch"; }
  virtual float OfferModifier() const { return 0.3f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 208); }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != palmleaf::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,240); }
  virtual ushort GetFormModifier() const { return 50; }
);

class ITEM
(
  whip,
  meleeweapon,
  InitMaterials(new leather),
  {
    SetSize(200);
  },
 public:
  virtual ushort Possibility() const { return 25; }
  virtual vector2d GetInHandsPic() const { return vector2d(160,224); }
  virtual std::string NameSingular() const { return "whip"; }
  virtual float OfferModifier() const { return 0.5f; }
  virtual uchar GetWeaponCategory() const { return WHIPS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
 protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(32,0); }
  virtual ushort GetFormModifier() const;
);

class ITEM
(
  backpack,
  item,
  InitMaterials(2, new leather, new gunpowder),
  {
    SetSize(80);
  },
 public:
  virtual ushort Possibility() const { return 10; }
  //virtual std::string Name(uchar Case) const { return NameContainer(Case); }
  virtual uchar GetConsumeType() const { return GetContainedMaterial() ? GetContainedMaterial()->GetConsumeType() : ODD; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 144); }
  virtual float OfferModifier() const { return 0.5; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; case 1: return 10000; default: return 0; } }
  virtual ulong Price() const { return GetContainedMaterial() ? GetContainedMaterial()->RawPrice() : 0; }
  virtual uchar GetConsumeMaterial() const { return 1; }
  virtual bool Apply(character*, stack*);
  virtual bool IsExplosive() const { return (GetContainedMaterial() && GetContainedMaterial()->IsExplosive()) ? true : false; }
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
  virtual bool StruckByWandOfStriking(character*, std::string);
 protected:
  virtual std::string PostFix() const { return ContainerPostFix(); }
  virtual bool ShowPostFix() const { return true; }
  virtual ushort GetStrengthModifier() const { return 200; }
  virtual vector2d GetBitmapPos() const { return vector2d(32, 16); }
  virtual std::string NameSingular() const { return "backpack"; }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  holybook,
  item,
  InitMaterials(1, 0),
  {  
    SetSize(25);
    SetOwnerGod(1 + RAND() % (game::GetGods() - 1));
  },
 public:
  virtual bool CanBeRead(character*) const;
  //virtual std::string Name(uchar Case) const { return NameNormal(Case, "a") + OwnerGodDescription(OwnerGod); }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 128); }
  virtual ushort Possibility() const { return 10; }
  virtual float OfferModifier() const { return 0.4f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetOwnerGod() const { return OwnerGod; }
  virtual void SetOwnerGod(uchar);
  virtual bool Read(character*);
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
 protected:
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos() const { return vector2d(32,32); }
  virtual std::string NameSingular() const { return "holy book"; }
  virtual ushort GetFormModifier() const { return 30; }
  virtual std::string PostFix() const { return OwnerGodDescription(OwnerGod); }
  virtual bool ShowPostFix() const { return true; }
  virtual bool ShowMaterial() const { return false; }
  uchar OwnerGod;
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
  //virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", parchment::StaticType()); }
  virtual ushort Possibility() const { return 0; }
  virtual std::string NameSingular() const { return "pile of 50 million roubles"; }
  virtual float OfferModifier() const { return 0.01f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return 2; }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != parchment::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 25; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,336); }
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  oillamp,
  item,
  InitMaterials(new gold),
  {
    SetSize(30);
    UpdatePicture();
    SetInhabitedByGenie(!(rand() % 2));
  },
 public:
  virtual ushort Possibility() const { return 25; }
  virtual ushort GetEmitation() const { return 256; }
  virtual std::string NameSingular() const { return "oil lamp"; }
  virtual float OfferModifier() const { return 1; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 150; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GetInhabitedByGenie() const { return InhabitedByGenie; }
  virtual void SetInhabitedByGenie(bool What) { InhabitedByGenie = What; }
  virtual bool Apply(character*, stack*);
  virtual bool CanBeWished() const { return false; }
 protected:
  virtual std::string Article() const { return "an"; }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(32,48); }
  virtual ushort GetFormModifier() const { return 30; }
  bool InhabitedByGenie;
);

class ITEM
(
  astone,
  item,
  GenerateStoneMaterials(),
  {
    SetSize(30);
  },
 public:
  virtual ushort Possibility() const { return 5; }
  //virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
  virtual std::string NameSingular() const { return "stone"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual float OfferModifier() const { return 0.7f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 150; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice() * 2; }
  virtual void GenerateStoneMaterials();
 protected:
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,48); }
  virtual ushort GetFormModifier() const { return 45; }
);

class ITEM
(
  wandoffireballs,
  wand,
  InitMaterials(new marble),
  {
    SetSize(12);
    SetCharges(1 + RAND() % 3);
    SetTimesUsed(0);
  },
 public:
  virtual ushort Possibility() const { return 20; }
  virtual std::string NameSingular() const { return "wand of fireballs"; }
  virtual std::string NamePlural() const { return "wands of fireballs"; }
  virtual float OfferModifier() const { return 15; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 500; }
  virtual ushort GetBeamColor() const { return YELLOW; }
  virtual bool BeamEffect(character*, std::string, uchar, lsquare*);
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
  virtual ushort Possibility() const { return 5; }
  virtual std::string NameSingular() const { return "scroll of taming"; }
  virtual std::string NamePlural() const { return "scrolls of taming"; }
  virtual float OfferModifier() const { return 5; }
  virtual bool Read(character*);
  virtual ulong Price() const { return 350; }
);

class ABSTRACT_ITEM
(
  bodypart,
  item,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort Possibility() const { return 0; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 0; default: return 0; } }
  virtual void SetBitmapPos(vector2d What) { BitmapPos = What; }
  virtual uchar GetGraphicsContainerIndex() const { return GRHUMANOID; }
  virtual void SetColor(ushort Index, ushort What) { Color[Index] = What; }
  virtual character* GetMaster() const;
  //virtual void SetMaster(character*);
  virtual humanoid* GetHumanoidMaster() const;
  virtual bool AutoInitializable() const { return false; }
  virtual ushort GetStrengthValue() const;
  virtual short GetMaxHP() const;
  virtual void SetHP(short What) { HP = What; }
  virtual short GetHP() const { return HP; }
  virtual void EditHP(short What) { HP += What; }
  virtual ushort GetArmoredStrengthValue() const = 0;
  virtual bool ReceivePhysicalDamage(short);
  virtual std::string GetOwnerDescription() const { return OwnerDescription; }
  virtual void SetOwnerDescription(std::string What) { OwnerDescription = What; }
  virtual bool GetUnique() const { return Unique; }
  virtual void SetUnique(bool What) { Unique = What; }
  //virtual bool IsAttached() const;
  /*virtual void SetAttached(bool What) { Attached = What; }*/
  virtual characterslot* GetCharacterSlot() const;
 protected:
  virtual std::string PostFix() const { return GetOwnerDescription(); }
  virtual bool ShowPostFix() const { return !GetMaster(); }
  virtual bool ShowMaterial() const { return false; }
  virtual bool ForceDefiniteArticle() const { return Unique; }
  virtual ushort GetMaterialColor0() const { return Color[0]; }
  virtual ushort GetMaterialColor1() const { return Color[1]; }
  virtual ushort GetMaterialColor2() const { return Color[2]; }
  virtual ushort GetMaterialColor3() const { return Color[3]; }
  virtual vector2d GetBitmapPos() const { return BitmapPos; }
  std::string OwnerDescription;
  vector2d BitmapPos;
  ushort Color[4];
  //character* Master;
  short HP;
  bool Unique;
  //bool Attached;
);

class ITEM
(
  head,
  bodypart,
  InitMaterials(new humanflesh),
  {
    SetUnique(false);
  },
 public:
  virtual float OfferModifier() const { return 0.1f; }
  virtual bool CanBeWished() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  virtual ushort GetArmoredStrengthValue() const;
 protected:
  virtual ushort GetStrengthModifier() const { return 150; }
  virtual std::string NameSingular() const { return "head"; }
  virtual ushort GetFormModifier() const { return 20; }
  /*helmet* Helmet;
  amulet* Amulet;*/
);

class ABSTRACT_ITEM
(
  torso,
  bodypart,
 public:
 protected:
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
  },
 public:
  virtual ushort GetMaterialColor0() const { return item::GetMaterialColor0(); }
  virtual ushort GetMaterialColor1() const { return item::GetMaterialColor1(); }
  virtual ushort GetMaterialColor2() const { return item::GetMaterialColor2(); }
  virtual ushort GetMaterialColor3() const { return item::GetMaterialColor3(); }
  virtual uchar GetGraphicsContainerIndex() const { return GRCHARACTER; }
  virtual ushort GetArmoredStrengthValue() const;
);

class ITEM
(
  humanoidtorso,
  torso,
  ;,
  {
    SetBodyArmor(0);
    SetUnique(false);
  },
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetGraphicsContainerIndex() const { return GRHUMANOID; }
  virtual ushort GetArmoredStrengthValue() const;
  virtual void SetBodyArmor(item*);
  virtual item* GetBodyArmor() const { return *BodyArmorSlot; }
 protected:
  characterslot BodyArmorSlot;
  //bodyarmor* BodyArmor;
  /*cloak* Cloak;
  belt* Belt;*/
);

class ABSTRACT_ITEM
(
  arm,
  bodypart,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetArmoredStrengthValue() const;
  virtual void SetWielded(item*);
  virtual item* GetWielded() const { return *WieldSlot; }
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
 protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  //gauntlet* Gauntlet;
  characterslot WieldSlot;
  //ring* Ring;
  std::vector<sweaponskill*> SingleWeaponSkill;
  sweaponskill* CurrentSingleWeaponSkill;
);

class ITEM
(
  rightarm,
  arm,
  ;,
  {
    WieldSlot.SetItem(0);
    SetCurrentSingleWeaponSkill(0);
    SetHasBe(true);
    SetUnique(false);
  },
 public:
 protected:
  virtual uchar GetSpecialType() const { return STRIGHTARM; }
  virtual std::string NameSingular() const { return "right arm"; }
);

class ITEM
(
  leftarm,
  arm,
  ;,
  {
    WieldSlot.SetItem(0);
    SetCurrentSingleWeaponSkill(0);
    SetHasBe(true);
    SetUnique(false);
  },
 public:
 protected:
  virtual uchar GetSpecialType() const { return STLEFTARM; }
  virtual std::string NameSingular() const { return "left arm"; }
);

class ITEM
(
  groin,
  bodypart,
  ;,
  {
    SetUnique(false);
  },
 public:
  virtual ushort GetArmoredStrengthValue() const;
 protected:
  virtual ushort GetStrengthModifier() const { return 100; }
  virtual uchar GetSpecialType() const { return STGROIN; }
  virtual std::string NameSingular() const { return "groin"; }
);

class ABSTRACT_ITEM
(
  leg,
  bodypart,
 public:
  virtual ushort GetArmoredStrengthValue() const;
 protected:
  virtual ushort GetStrengthModifier() const { return 75; }
  //boot* Boot;
);

class ITEM
(
  rightleg,
  leg,
  ;,
  {
    SetUnique(false);
  },
 public:
 protected:
  virtual uchar GetSpecialType() const { return STRIGHTLEG; }
  virtual std::string NameSingular() const { return "right leg"; }
);

class ITEM
(
  leftleg,
  leg,
  ;,
  {
    SetUnique(false);
  },
 public:
 protected:
  virtual uchar GetSpecialType() const { return STLEFTLEG; }
  virtual std::string NameSingular() const { return "left leg"; }
);

class ITEM
(
  headofelpuri,
  head,
  InitMaterials(new elpuriflesh),
  {
    SetSize(60);
    //SetMaster(0);
    SetUnique(true);
    SetOwnerDescription("of Elpuri, the Master Dark Frog");
  },
 public:
  virtual ushort Possibility() const { return 0; }
  //virtual std::string Name(uchar Case) const { return NameArtifact(Case, elpuriflesh::StaticType()); }
  virtual bool IsHeadOfElpuri() const { return true; }
  //virtual std::string NameSingular() const { return "head of Elpuri"; }
  virtual long Score() const { return 1000; };
  virtual bool CanBeWished() const { return false; }
  virtual bool Destroyable() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 25000; default: return 0; } }
  virtual bool Consumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
  virtual bool AutoInitializable() const { return true; }
 protected:
  //virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != elpuriflesh::StaticType(); }
  //virtual std::string Article() const { return "the"; }
  //virtual bool ForceDefiniteArticle() const { return true; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,0); }
);

class ITEM
(
  headofennerbeast,
  head,
  InitMaterials(new ennerbeastflesh),
  {
    SetSize(50);
    //SetMaster(0);
    SetUnique(false);
    SetOwnerDescription("of an enner beast");
  },
 public:
  virtual ushort Possibility() const { return 5; }
  //virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
  //virtual std::string NameSingular() const { return "head of an enner beast"; }
  virtual long Score() const { return 250; };
  virtual bool CanBeWished() const { return true; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; default: return 0; } }
  virtual ulong Price() const { return 100; }
  virtual bool AutoInitializable() const { return true; }
 protected:
  //virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != ennerbeastflesh::StaticType(); }
  virtual vector2d GetBitmapPos() const { return vector2d(0,176); }
);

class ITEM
(
  wandofteleportation,
  wand,
  InitMaterials(new gold),
  {
    SetSize(10);
    SetCharges(2 + RAND() % 4);
    SetTimesUsed(0);
  },
 public:
  virtual ushort Possibility() const { return 20; }
  virtual std::string NameSingular() const { return "wand of teleportation"; }
  virtual std::string NamePlural() const { return "wands of teleportation"; }
  virtual float OfferModifier() const { return 30; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual ulong Price() const { return 500; }
  virtual bool BeamEffect(character*, std::string, uchar, lsquare*);
  virtual ushort GetBeamColor() const { return GREEN; }
);

class ITEM
(
  mine,
  item,
  InitMaterials(2, new iron, new gunpowder),
  {
    SetSize(5);
    SetCharged(RAND() % 5 ? true : false);
  },
 public:
  //  virtual bool Apply(character*, stack*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
  virtual bool IsExplosive() const { return (GetContainedMaterial() && GetContainedMaterial()->IsExplosive()) ? true : false; }
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
  //virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", iron::StaticType()); }
  virtual uchar GetCharged() const { return Charged; }
  virtual bool StruckByWandOfStriking(character*, std::string);
  virtual void SetCharged(bool What) { Charged = What; }
  bool IsChargable() const { return true; }
  virtual bool GetStepOnEffect(character *);
  virtual ushort Possibility() const { return 20; }
  virtual std::string NameSingular() const { return "mine"; }
 protected:
  virtual bool ShowMaterial() const { return GetMainMaterial()->GetType() != iron::StaticType(); }
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,288); }
  virtual ushort GetFormModifier() const { return 30; }
  bool Charged;
);

class ITEM
(
  wandofhaste,
  wand,
  InitMaterials(new iron),
  {
    SetSize(10);
    SetCharges(2 + RAND() % 4);
    SetTimesUsed(0);
  },
 public:
  virtual ushort Possibility() const { return 30; }
  virtual std::string NameSingular() const { return "wand of haste"; }
  virtual std::string NamePlural() const { return "wands of haste"; }
  virtual float OfferModifier() const { return 20; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, std::string, uchar, lsquare*);
  virtual ushort GetBeamColor() const { return RED; }
);

class ITEM
(
  wandofslow,
  wand,
  InitMaterials(new iron),
  {
    SetSize(10);
    SetCharges(2 + RAND() % 4);
    SetTimesUsed(0);
  },
 public:
  virtual ushort Possibility() const { return 30; }
  virtual std::string NameSingular() const { return "wand of slow"; }
  virtual std::string NamePlural() const { return "wands of slow"; }
  virtual float OfferModifier() const { return 20; }
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool BeamEffect(character*, std::string, uchar, lsquare*);
  virtual ushort GetBeamColor() const { return GREEN; }
);

class ITEM
(
  key,
  item,
  InitMaterials(new iron),
  {
   SetSize(5);
   SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
  },
 public:
  virtual ushort Possibility() const { return 80; }
  virtual std::string NameSingular() const { return "key"; }
  virtual float OfferModifier() const { return 0.5; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 15; default: return 0; } }
  virtual ulong Price() const { return 10; }
  virtual bool Apply(character*, stack*);
  virtual void SetLockType(uchar What) { LockType = What; }
  virtual uchar GetLockType() const { return LockType; }
 protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,48); } // Alcohol and Mr. Graphics Guy don't fit together very well
  virtual ushort GetFormModifier() const { return 40; }
  uchar LockType;
);

class ITEM
(
  shield,
  item,
  InitMaterials(new iron),
  {  
    SetSize(40);
  },
 public:
  virtual ushort Possibility() const { return 30; }
  virtual std::string NameSingular() const { return "shield"; }
  virtual float OfferModifier() const { return 0.4f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4800; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the shield can be calculated somehow
 protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(32,336); }
  virtual ushort GetFormModifier() const { return 30; } 
);

class ITEM
(
  cloak,
  item,
  GenerateCloakMaterials(),
  {  
    SetSize(220);
  },
 public:
  virtual ushort Possibility() const { return 30; }
  virtual std::string NameSingular() const { return "cloak"; }
  virtual float OfferModifier() const { return 0.4f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 10000; default: return 0; } }
  virtual ulong Price() const { return GetMainMaterial()->RawPrice(); } // This should be overwritten, when the effectivness of the cloak can be calculated somehow
  virtual void GenerateCloakMaterials();
 protected:
  virtual ushort GetStrengthModifier() const { return 30; }
  virtual vector2d GetBitmapPos() const { return vector2d(32,352); }
  virtual ushort GetFormModifier() const { return 20; } 
);

class ITEM
(
 boot,
 item,
 GenerateBootMaterials(),
 {
   SetSize(30);
 },
 public:
 virtual ushort Possibility() const { return 30; }
 virtual std::string NameSingular() const { return "boot"; }
 virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 600; default: return 0; } } 
  virtual ulong Price() const { return GetMaterial(0)->RawPrice(); } // This should be overwritten, when the effectivness of the boots can be calculated someho
  virtual void GenerateBootMaterials();
  protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 400); }
  virtual ushort GetFormModifier() const { return 20; } 
);

class ITEM
(
 gauntlet, 
 item,
 GenerateGauntletMaterials(),
 {
   SetSize(30);
 },
 public:
 virtual ushort Possibility() const { return 30; }
 virtual std::string NameSingular() const { return "gauntlet"; }
 virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 600; default: return 0; } } 
  virtual ulong Price() const { return GetMaterial(0)->RawPrice(); } // This should be overwritten, when the effectivness of the gauntlets can be calculated somehow
  virtual void GenerateGauntletMaterials();
  protected:
  virtual ushort GetStrengthModifier() const { return 55; }
  virtual vector2d GetBitmapPos() const { return vector2d(32, 368); }
  virtual ushort GetFormModifier() const { return 20; } 
);

class ITEM
(
 belt, 
 item,
 InitMaterials(new leather),
 {
   SetSize(150);
 },
 public:
 virtual ushort Possibility() const { return 30; }
 virtual std::string NameSingular() const { return "belt"; }
 virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } } 
  virtual ulong Price() const { return GetMaterial(0)->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  virtual uchar GetWeaponCategory() const { return WHIPS; }
  protected:
  virtual ushort GetStrengthModifier() const { return 50; }
  virtual vector2d GetBitmapPos() const { return vector2d(32, 384); }
  virtual ushort GetFormModifier() const;
);

class ITEM
(
 ring, 
 item,
 GenerateRingMaterials(),
 {
   SetSize(2);
 },
 public:
 virtual ushort Possibility() const { return 30; }
 virtual std::string NameSingular() const { return "ring"; }
 virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 3; default: return 0; } } 
  virtual ulong Price() const { return GetMaterial(0)->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  void GenerateRingMaterials();
  protected:
  virtual ushort GetStrengthModifier() const { return 80; }
  virtual vector2d GetBitmapPos() const { return vector2d(16, 400); }
  virtual ushort GetFormModifier() const { return 20; } 
);

class ITEM
(
 amulet, 
 item,
 GenerateAmuletMaterials(),
 {
   SetSize(20);
 },
 public:
 virtual ushort Possibility() const { return 30; }
 virtual std::string NameSingular() const { return "amulet"; }
 virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 15; default: return 0; } } 
  virtual ulong Price() const { return GetMaterial(0)->RawPrice(); } // This should be overwritten, when the effectivness of the belt can be calculated somehow
  void GenerateAmuletMaterials();
  protected:
  virtual ushort GetStrengthModifier() const { return 30; }
  virtual vector2d GetBitmapPos() const { return vector2d(32, 400); }
  virtual ushort GetFormModifier() const { return 50; } 
);

#endif
