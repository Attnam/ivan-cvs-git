#ifndef __ITEMDE_H__
#define __ITEMDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "itemba.h"
#include "materde.h"
#include "wskill.h"
#include "game.h"

class ITEM
(
  banana,
  item,
  InitMaterials(2, new bananapeal, new bananaflesh),
  {
    SetSize(20);
  },
 public:
  virtual ushort Possibility() const { return 200; }
  virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", bananapeal::StaticType()); }
  virtual uchar GetConsumeType() const { return GetMaterial(1)->GetConsumeType(); }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual bool Consume(character*, float);
  virtual std::string NameSingular() const { return "banana"; }
  virtual float OfferModifier() const { return 1; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,112); }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; case 1: return 150; default: return 0; } }
  virtual ulong Price() const { return GetMaterial(1)->RawPrice(); }
  virtual ulong ConsumeLimit() const { return GetMaterial(1)->GetVolume(); }
  virtual uchar GetConsumeMaterial() const { return 1; }
 protected:
  virtual ushort GetFormModifier() const { return 50; }
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
  virtual std::string Name(uchar Case) const { return NameArtifact(Case, bananapeal::StaticType()); }
  virtual std::string NameSingular() const { return "holy banana of Liukas Vipro"; }
  virtual std::string NamePlural() const { return "holy bananas of Liukas Vipro"; }
  virtual float OfferModifier() const { return 40; }
  virtual long Score() const { return 250; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 400; case 1: return 1500; default: return 0; } }
  virtual ulong Price() const { return 1000; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(0, OnWall ? 192 : 256); }
  virtual bool ImpactDamage(ushort, bool, stack*);
  virtual bool ReceiveSound(float, bool, stack*);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return 50; }
  virtual void SignalSquarePositionChange(bool);
  virtual void SetOnWall(bool What) { OnWall = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
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
  virtual std::string Name(uchar Case) const { return NameContainer(Case); }
  virtual item* TryToOpen(character*, stack*);
  virtual uchar GetConsumeType() const { return GetMaterial(1) ? GetMaterial(1)->GetConsumeType() : ODD; }
  virtual std::string NameSingular() const { return "can"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 144); }
  virtual float OfferModifier() const { return 0.5; }
  virtual item* PrepareForConsuming(character*, stack*);
  virtual vector2d GetBitmapPos() const { return vector2d(16, GetMaterial(1) ? 288 : 304); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; case 1: return 500; default: return 0; } }
  virtual ulong Price() const { return GetMaterial(1) ? GetMaterial(1)->RawPrice() : 0; }
  virtual item* BetterVersion() const;
  virtual uchar GetConsumeMaterial() const { return 1; }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameThingsThatAreLikeLumps(Case, "a"); }
  virtual void ReceiveHitEffect(character*, character*);
  virtual bool CanBeDippedInto(item*) const { return true; }
  virtual material* BeDippedInto();
  virtual std::string NameSingular() const { return "lump"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual float OfferModifier() const { return 0.5; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,48); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ulong Price() const { return GetMaterial(0)->RawPrice(); }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(16,336); }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2400; case 1: return 100; case 2: return 100; default: return 0; } }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(0,0); }
  virtual float OfferModifier() const { return 0.25; }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; case 2: return 100; default: return 0; } }
 protected:
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
  virtual std::string NameSingular() const { return "curved two-handed sword"; }
  virtual float OfferModifier() const { return 0.25; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,16); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; case 2: return 100; default: return 0; } }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameArtifact(Case, valpurium::StaticType()); }
  virtual std::string NameSingular() const { return "holy broadsword \"Valpurus's Justifier\""; }
  virtual float OfferModifier() const { return 0.5; }
  virtual long Score() const { return 1000; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,64); }
  virtual bool CanBeWished() const { return false; }
  virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 6700; case 1: return 300; case 2: return 100; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(16,256); }
  virtual uchar GetWeaponCategory() const { return AXES; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1400; case 1: return 1600; case 2: return 100; default: return 0; } }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(0,96); }
  virtual bool Apply(character*, stack*);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; case 1: return 2000; case 2: return 100; default: return 0; } }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(16,144); }
  virtual uchar GetWeaponCategory() const { return SPEARS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 200; case 1: return 1600; case 2: return 100; default: return 0; } }
 protected:
  virtual ushort GetFormModifier() const { return 200; }
);

class ABSTRACT_ITEM
(
  torsoarmor,
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
  torsoarmor,
  InitMaterials(new iron),
  {
    SetSize(75);
  },
 public:
  virtual ushort Possibility() const { return 5; }
  virtual ushort GetArmorValue() const;
  virtual std::string NameSingular() const { return "plate mail"; }
  virtual float OfferModifier() const { return 0.5; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,128); }
  virtual bool ImpactDamage(ushort, bool, stack*);
  virtual bool ReceiveSound(float, bool, stack*);
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
);

class ITEM
(
  chainmail,
  torsoarmor,
  InitMaterials(new iron),
  {
    SetSize(75);
  },
 public:
  virtual ushort Possibility() const { return 50; }
  virtual ushort GetArmorValue() const;
  virtual std::string NameSingular() const { return "chain mail"; }
  virtual float OfferModifier() const { return 0.5; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,96); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
);

class ABSTRACT_ITEM
(
  shirt,
  torsoarmor,
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
  virtual std::string Name(uchar Case) const { return NameArtifact(Case, cloth::StaticType()); }
  virtual ushort Possibility() const { return 0; }
  virtual ushort GetArmorValue() const { return 10; }
  virtual std::string NameSingular() const { return "Shirt of the Golden Eagle"; }
  virtual short CalculateOfferValue(char) const { return 750; }
  virtual long Score() const { return 2500; }
  virtual bool IsGoldenEagleShirt() const { return true; };
  virtual vector2d GetBitmapPos() const { return vector2d(16,112); }
  virtual bool CanBeWished() const { return false; }
  virtual bool Destroyable() const { return false; }
  virtual ushort GetEmitation() const { return 333; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Consumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
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
  virtual vector2d GetBitmapPos() const { return vector2d(16,192); }
  virtual bool CanBeDippedInto(item*) const { return true; }
  virtual material* BeDippedInto();
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  virtual ulong Price() const { return GetMaterial(0) ? GetMaterial(0)->RawPrice() : 0; }
  virtual void SetBloodColor(ushort);
  virtual bool CatWillCatchAndConsume() const { return GetMaterial(0)->GetType() == ratflesh::StaticType(); }
 protected:
  virtual ushort GetFormModifier() const { return 15; }
  virtual float NPModifier() const { return 0.01f; }
);

class ITEM
(
  potion,
  item,
  InitMaterials(2, new glass, RAND() % 2 ? (material*)new water : (material*)new omleurine),
  {
    SetSize(30);
  },
 public:
  virtual ushort Possibility() const { return 100; }
  virtual uchar GetConsumeType() const { return GetMaterial(1) ? GetMaterial(1)->GetConsumeType() : ODD; }
  virtual bool Consume(character*, float);
  virtual std::string Name(uchar Case) const { return NameContainer(Case); }
  virtual std::string NameSingular() const { return "bottle"; }
  virtual bool ImpactDamage(ushort, bool, stack*);
  virtual void PositionedDrawToTileBuffer(uchar) const;
  virtual float OfferModifier() const { return 0.1f; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,160); }
  virtual bool ReceiveSound(float, bool, stack*);
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; case 1: return 1500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual void ColorChangeSpeciality(uchar, bool);
  virtual ulong Price() const { return GetMaterial(1) ? GetMaterial(1)->RawPrice() : 0; }
  virtual ulong ConsumeLimit() const { return GetMaterial(1)->GetVolume(); }
  virtual uchar GetConsumeMaterial() const { return 1; }
  virtual item* BetterVersion() const;
  virtual std::string GetConsumeVerb() const { return std::string("drinking"); }
 protected:
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
  virtual ushort Possibility() const { return 100; }
  virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", bananapeal::StaticType()); }
  virtual std::string NameSingular() const { return "banana peal"; }
  virtual item* BetterVersion() const { return new banana; }
  virtual float OfferModifier() const { return 0; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,128); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,112); }
 protected:
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
  virtual std::string NameSingular() const { return "broken bottle"; }
  virtual item* BetterVersion() const;
  virtual float OfferModifier() const { return 0; }
  virtual vector2d GetBitmapPos() const { return vector2d(16,160); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual bool GetStepOnEffect(character*);
 protected:
  virtual ushort GetFormModifier() const { return 100; }
);

class ABSTRACT_ITEM
(
  scroll,
  item,
 public:
  virtual bool CanBeRead(character*) const;
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
  virtual vector2d GetBitmapPos() const { return vector2d(16,176); }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
 protected:
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
  head,
  item,
  InitMaterials(new humanflesh),
  {
    SetSize(0);
  },
 public:
  virtual ushort Possibility() const { return 0; }
  virtual std::string NameSingular() const { return "head"; }
  virtual float OfferModifier() const { return 0.1f; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,0); }
  virtual bool CanBeWished() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
 protected:
  virtual ushort GetFormModifier() const { return 20; }
);

class ITEM
(
  headofelpuri,
  head,
  InitMaterials(new elpuriflesh),
  {
    SetSize(60);
  },
 public:
  virtual ushort Possibility() const { return 0; }
  virtual std::string Name(uchar Case) const { return NameArtifact(Case, elpuriflesh::StaticType()); }
  virtual bool IsHeadOfElpuri() const { return true; }
  virtual std::string NameSingular() const { return "head of Elpuri"; }
  virtual long Score() const { return 1000; };
  virtual vector2d GetBitmapPos() const { return vector2d(16,0); }
  virtual bool CanBeWished() const { return false; }
  virtual bool Destroyable() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 25000; default: return 0; } }
  virtual bool Consumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
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
  virtual vector2d GetBitmapPos() const { return vector2d(16,208); }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameArtifact(Case, humanflesh::StaticType()); }
  virtual std::string NameSingular() const { return "left nut of Petrus"; }
  virtual long Score() const { return 5000; }
  virtual item* CreateWishedItem() const;
  virtual bool Destroyable() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ushort GetEmitation() const { return 333; }
  virtual bool Consumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
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
  virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", bone::StaticType()); }
  virtual float OfferModifier() const { return 0.1f; }
  virtual bool DogWillCatchAndConsume() const { return GetMaterial(0)->GetType() == bone::StaticType(); }
  virtual vector2d GetBitmapPos() const { return vector2d(16,240); }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,32); }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(0,80); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2500; case 1: return 2000; case 2: return 100; default: return 0; } }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(0,32); }
  virtual uchar GetWeaponCategory() const { return MACES; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; case 1: return 2000; case 2: return 100; default: return 0; } }
 protected:
  virtual ushort GetFormModifier() const { return 125; }
);

class ITEM
(
  neercseulb,
  spikedmace,
  InitMaterials(3, new mithril, new mithril, new darkfrogflesh),
  {
    SetSize(200);
  },
 public:
  virtual ushort Possibility() const { return 0; }
  virtual std::string Name(uchar Case) const { return NameArtifact(Case, mithril::StaticType()); }
  virtual std::string NameSingular() const { return "ancient mace \"Neerc Se-Ulb\""; }
  virtual float OfferModifier() const { return 0.25; }
  virtual long Score() const { return 1000; }
  virtual bool CanBeWished() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 12000; case 1: return 6000; case 2: return 2500; default: return 0; } }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameThingsThatAreLikeLumps(Case, "a"); }
  virtual std::string NameSingular() const { return "loaf"; }
  virtual std::string NamePlural() const { return "loaves"; }
  virtual float OfferModifier() const { return 0.125; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,272); }
  virtual uchar GetWeaponCategory() const { return CLUBS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual ulong Price() const { return GetMaterial(0) ? GetMaterial(0)->RawPrice() : 0; }
  virtual material* CreateLoafMaterials();
 protected:
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
  virtual std::string NameSingular() const { return "cheap copy of the left nut of Petrus"; }
  virtual std::string NamePlural() const { return "cheap copies of the left nut of Petrus"; }
  virtual long Score() const { return 1; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
  virtual ulong Price() const { return 500; }
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
  virtual vector2d GetBitmapPos() const { return vector2d(0,288); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
  virtual bool IsExplosive() const { return true; }
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
  virtual std::string Name(uchar Case) const;
  virtual uchar GetCharges() const { return Charges; }
  virtual void SetCharges(uchar What) { Charges = What; }
  virtual uchar GetTimesUsed() const { return TimesUsed; }
  virtual void SetTimesUsed(uchar What) { TimesUsed = What; }
  virtual void Beam(character*, std::string, uchar, uchar);
  virtual void BeamEffect(character*, std::string, uchar, levelsquare*) {};
  virtual ushort GetBeamColor() const = 0;
  virtual bool StruckByWandOfStriking(character*, std::string, stack*);
 protected:
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
  virtual void BeamEffect(character*, std::string, uchar, levelsquare*);
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
  virtual vector2d GetBitmapPos() const { return vector2d(16,80); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
  virtual bool UseThrowStrengthModifier() const { return true; }
 protected:
  virtual ushort GetFormModifier() const { return 50; }
);*/

class ITEM
(
  headofennerbeast,
  head,
  InitMaterials(new ennerbeastflesh),
  {
    SetSize(50);
  },
 public:
  virtual ushort Possibility() const { return 5; }
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
  virtual std::string NameSingular() const { return "head of an Enner Beast"; }
  virtual long Score() const { return 250; };
  virtual vector2d GetBitmapPos() const { return vector2d(0,176); }
  virtual bool CanBeWished() const { return true; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; default: return 0; } }
  virtual ulong Price() const { return 100; }
);

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
  virtual ushort Possibility() const { return 50; }
  virtual std::string NameSingular() const { return "broken lantern"; }
  virtual item* BetterVersion() const { return new lantern; }
  virtual float OfferModifier() const { return 0; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, OnWall ? 208 : 304); }
  virtual vector2d GetInHandsPic() const { return vector2d(160,128); }
  virtual bool ImpactDamage(ushort, bool, stack*) { return false; }
  virtual ushort GetEmitation() const { return 0; }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameArtifact(Case, valpurium::StaticType()); }
  virtual ushort Possibility() const { return 0; }
  virtual std::string NameSingular() const { return "Avatar of Valpurus"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,320); }
  virtual bool CanBeWished() const { return false; }
  virtual bool Destroyable() const { return false; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,112); }
  virtual bool IsMaterialChangeable() const { return false; }
  virtual bool Consumable(character*) const { return false; }
  virtual bool Polymorph(stack*) { return false; }
 protected:
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
  virtual ushort GetBeamColor() const { return YELLOW; }
  virtual void BeamEffect(character*, std::string, uchar, levelsquare*);
);

class ITEM
(
  brokenplatemail,
  torsoarmor,
  InitMaterials(new iron),
  {
    SetSize(70);
  },
 public:
  virtual ushort Possibility() const { return 10; }
  virtual std::string NameSingular() const { return "broken plate mail"; }
  virtual float OfferModifier() const { return 0.1f; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,352); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160,144); }
  virtual ushort GetArmorValue() const;
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(16,320); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual float GetThrowStrengthModifier() const { return 4; }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", kiwiflesh::StaticType()); }
  virtual ushort Possibility() const { return 20; }
  virtual std::string NameSingular() const { return "kiwi"; }
  virtual float OfferModifier() const { return 0.4f; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,384); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; default: return 0; } }
  virtual ulong Price() const { return GetMaterial(0)->RawPrice(); }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", pineappleflesh::StaticType()); }
  virtual ushort Possibility() const { return 25; }
  virtual std::string NameSingular() const { return "pineapple"; }
  virtual float OfferModifier() const { return 0.4f; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,368); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return GetMaterial(0)->RawPrice(); }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(0,240); }
  virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", palmleaf::StaticType()); }
  virtual ushort Possibility() const { return 10; }
  virtual std::string NameSingular() const { return "palm branch"; }
  virtual float OfferModifier() const { return 0.3f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 208); }
 protected:
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
  virtual vector2d GetBitmapPos() const { return vector2d(32,0); }
  virtual uchar GetWeaponCategory() const { return WHIPS; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameContainer(Case); }
  virtual uchar GetConsumeType() const { return GetMaterial(1) ? GetMaterial(1)->GetConsumeType() : ODD; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 144); }
  virtual float OfferModifier() const { return 0.5; }
  virtual vector2d GetBitmapPos() const { return vector2d(32, 16); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; case 1: return 10000; default: return 0; } }
  virtual ulong Price() const { return GetMaterial(1) ? GetMaterial(1)->RawPrice() : 0; }
  virtual uchar GetConsumeMaterial() const { return 1; }
  virtual bool Apply(character*, stack*);
  virtual bool IsExplosive() const { return (GetMaterial(1) && GetMaterial(1)->IsExplosive()) ? true : false; }
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
  virtual bool StruckByWandOfStriking(character*, std::string, stack*);
 protected:
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
    SetOwnerGod(1 + RAND() % game::GetGodNumber());
  },
 public:
  virtual bool CanBeRead(character*) const;
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a") + OwnerGodDescription(OwnerGod); }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 128); }
  virtual ushort Possibility() const { return 5; }
  virtual float OfferModifier() const { return 0.4f; }
  virtual vector2d GetBitmapPos() const { return vector2d(32,32); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetOwnerGod() const { return OwnerGod; }
  virtual void SetOwnerGod(uchar);
  virtual bool Read(character*);
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long);
 protected:
  virtual std::string NameSingular() const { return "holy book"; }
  virtual ushort GetFormModifier() const { return 30; }
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
  virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", parchment::StaticType()); }
  virtual vector2d GetBitmapPos() const { return vector2d(0,336); }
  virtual ushort Possibility() const { return 0; }
  virtual std::string NameSingular() const { return "pile of 50 million roubles"; }
  virtual float OfferModifier() const { return 0.01f; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return 2; }
 protected:
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
    SetInhabitedByGenie(!(rand() % 4));
  },
 public:
  virtual ushort Possibility() const { return 10; }
  virtual ushort GetEmitation() const { return 256; }
  virtual std::string NameSingular() const { return "oil lamp"; }
  virtual float OfferModifier() const { return 1; }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
  virtual ulong Price() const { return 100; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GetInhabitedByGenie() const { return InhabitedByGenie; }
  virtual void SetInhabitedByGenie(bool What) { InhabitedByGenie = What; }
  virtual bool Apply(character*, stack*);
  virtual vector2d GetBitmapPos() const { return vector2d(32,48); }
  virtual bool CanBeWished() const { return false; }
 protected:
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
  virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
  virtual std::string NameSingular() const { return "stone"; }
  virtual vector2d GetInHandsPic() const { return vector2d(160, 112); }
  virtual float OfferModifier() const { return 0.7f; }
  virtual vector2d GetBitmapPos() const { return vector2d(0,48); }
  virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 150; default: return 0; } }
  virtual ulong Price() const { return GetMaterial(0)->RawPrice() * 2; }
  virtual void GenerateStoneMaterials();
 protected:
  virtual ushort GetFormModifier() const { return 45; }
);

#endif
