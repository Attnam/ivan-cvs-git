#ifndef __ITEMDE_H__
#define __ITEMDE_H__

#pragma warning(disable : 4786)

#include "itemba.h"
#include "materde.h"
#include "wskill.h"
#include "rand.h"
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
	virtual ushort Possibility() const RET(200)
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", bananapeal::StaticType()))
	virtual uchar GetConsumeType() const RET(GetMaterial(1)->GetConsumeType())
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 112))
	virtual bool Consume(character*, float);
	virtual std::string NameSingular() const RET("banana")
	virtual float OfferModifier() const RET(1)
	virtual vector2d GetBitmapPos() const RETV(0,112)
	virtual uchar GetWeaponCategory() const { return CLUBS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; case 1: return 150; default: return 0; } }
	virtual ulong Price() const { return GetMaterial(1)->RawPrice(); }
	virtual ulong ConsumeLimit() const { return GetMaterial(1)->GetVolume(); }
	virtual uchar GetConsumeMaterial() const { return 1; }
protected:
	virtual ushort GetFormModifier() const RET(50)
);

class ITEM
(
	holybananaofliukasvipro,
	banana,
	InitMaterials(2, new bananapeal, new bananaflesh),
	{
		SetSize(30);
	},
public:
	virtual ushort Possibility() const RET(5)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, bananapeal::StaticType()))
	virtual std::string NameSingular() const RET("holy banana of Liukas Vipro")
	virtual std::string NamePlural() const RET("holy bananas of Liukas Vipro")
	virtual float OfferModifier() const RET(40)
	virtual long Score() const RET(250)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 400; case 1: return 1500; default: return 0; } }
	virtual ulong Price() const { return 1000; }
	virtual bool Polymorph(stack*) { return false; }
protected:
	virtual ushort GetFormModifier() const RET(200)
);

class ITEM
(
	lamp,
	item,
	InitMaterials(new glass),
	{
		SetSize(30);
		SetOnWall(false);
		UpdatePicture();
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual ushort GetEmitation() const RET(300)
	virtual std::string NameSingular() const RET("lamp")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 160))
	virtual float OfferModifier() const RET(1)
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
	virtual ushort GetFormModifier() const RET(30)
	bool OnWall;
);

class ITEM
(
	can,
	item,
	InitMaterials(2, new iron, RAND() % 5 ? (material*)new bananaflesh : (material*)new schoolfood),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(200)
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual item* TryToOpen(character*, stack*);
	virtual uchar GetConsumeType() const RET(GetMaterial(1) ? GetMaterial(1)->GetConsumeType() : ODD)
	virtual std::string NameSingular() const RET("can")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 144))
	virtual float OfferModifier() const RET(0.5)
	virtual item* PrepareForConsuming(character*, stack*);
	virtual vector2d GetBitmapPos() const { return vector2d(16, GetMaterial(1) ? 288 : 304); }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; case 1: return 500; default: return 0; } }
	virtual ulong Price() const { return GetMaterial(1) ? GetMaterial(1)->RawPrice() : 0; }
	virtual item* BetterVersion(void) const;
	virtual uchar GetConsumeMaterial() const { return 1; }
protected:
	virtual ushort GetFormModifier() const RET(30)
);

class ITEM
(
	lump,
	item,
	InitMaterials(RAND() % 5 ? (material*)new bananaflesh : (material*)new schoolfood),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(200)
	virtual std::string Name(uchar Case) const RET(NameThingsThatAreLikeLumps(Case, "a")) 
	virtual void ReceiveHitEffect(character*, character*);
	virtual bool CanBeDippedInto(item*) const RET(true)
	virtual material* BeDippedInto();
	virtual std::string NameSingular() const RET("lump")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 112))
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(16,48)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
	virtual ulong Price() const { return GetMaterial(0)->RawPrice(); }
protected:
	virtual ushort GetFormModifier() const RET(15)
);

class ABSTRACT_ITEM
(
	meleeweapon,
	item,
public:
	virtual void ReceiveHitEffect(character*, character*);
	virtual void DipInto(item*);
	virtual bool CanBeDippedInto(item*) const RET(GetMaterial(2) ? false : true)
	virtual bool CanBeDipped() const RET(true)
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
	virtual ushort Possibility() const RET(25)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,16))
	virtual std::string NameSingular() const RET("long sword")
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(16,336)
	virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2400; case 1: return 100; case 2: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(150)
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
	virtual ushort Possibility() const RET(3)
	virtual std::string NameSingular() const RET("two-handed sword")
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual float OfferModifier() const RET(0.25)
	virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; case 2: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(175)
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
	virtual ushort Possibility() const RET(0)
	virtual std::string NameSingular() const RET("curved two-handed sword")
	virtual float OfferModifier() const RET(0.25)
	virtual vector2d GetBitmapPos() const RETV(0,16)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; case 2: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(200)
);

class ITEM
(
	valpurijustifier,
	longsword,
	InitMaterials(3, new valpurium, new valpurium, 0),
	{
		SetSize(200);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, valpurium::StaticType()))
	virtual std::string NameSingular() const RET("holy broadsword \"Valpuri's Justifier\"")
	virtual float OfferModifier() const RET(0.5)
	virtual long Score() const RET(1000)
	virtual vector2d GetBitmapPos() const RETV(0,64)
	virtual bool CanBeWished() const RET(false)
	virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 6700; case 1: return 300; case 2: return 100; default: return 0; } }
	virtual bool IsMaterialChangeable() const { return false; }
	virtual bool Polymorph(stack*) { return false; }
protected:
	virtual ushort GetFormModifier() const RET(400)
);

class ITEM
(
	axe,
	meleeweapon,
	InitMaterials(3, RAND() % 5 ? (material*)new stone : rand() % 20 ? (material*)new iron : (material*)new mithril, new wood, 0),
	{
		SetSize(125);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,176))
	virtual std::string NameSingular() const RET("axe")
	virtual float OfferModifier() const RET(0.25)
	virtual vector2d GetBitmapPos() const RETV(16,256)
	virtual uchar GetWeaponCategory() const { return AXES; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1400; case 1: return 1600; case 2: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(150)
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
	virtual ushort Possibility() const RET(50)
	virtual std::string NameSingular() const RET("pick-axe")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 64))
	virtual float OfferModifier() const RET(0.25)
	virtual vector2d GetBitmapPos() const RETV(0,96)
	virtual bool Apply(character*, stack*);
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; case 1: return 2000; case 2: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(100)
);

class ITEM
(
	spear,
	meleeweapon,
	InitMaterials(3, RAND() % 5 ? (material*)new stone : rand() % 20 ? (material*)new iron : (material*)new mithril, new wood, 0),
	{
		SetSize(200);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,96))
	virtual std::string NameSingular() const RET("spear")
	virtual float OfferModifier() const RET(1)
	virtual vector2d GetBitmapPos() const RETV(16,144)
	virtual uchar GetWeaponCategory() const { return SPEARS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 200; case 1: return 1600; case 2: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(200)
);

class ABSTRACT_ITEM
(
	torsoarmor,
	item,
public:
	virtual bool CanBeWorn() const RET(true)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,144))
	virtual ulong Price() const;
protected:
	virtual ushort GetFormModifier() const RET(20)
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
	virtual ushort Possibility() const RET(5)
	virtual ushort GetArmorValue() const;
	virtual std::string NameSingular() const RET("plate mail")
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(16,128)
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
	virtual ushort Possibility() const RET(50)
	virtual ushort GetArmorValue() const;
	virtual std::string NameSingular() const RET("chain mail")
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(16,96)
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
	maakotkashirt,
	shirt,
	InitMaterials(new cloth),
	{
		SetSize(60);
	},
public:
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, cloth::StaticType()))
	virtual ushort Possibility() const RET(0)
	virtual ushort GetArmorValue() const RET(10)
	virtual std::string NameSingular() const RET("Holy Maakotka Shirt")
	virtual short CalculateOfferValue(char) const RET(750)
	virtual long Score() const RET(2500)
	virtual bool IsMaakotkaShirt() const RET(true);
	virtual vector2d GetBitmapPos() const RETV(16,112)
	virtual bool CanBeWished() const RET(false)
	virtual bool Destroyable() const { return false; }
	virtual ushort GetEmitation() const RET(333)
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
	virtual ushort Possibility() const RET(10)
	virtual std::string NameSingular() const RET("corpse")
	virtual float OfferModifier() const RET(0.01f)
	virtual vector2d GetBitmapPos() const RETV(16,192)
	virtual bool CanBeDippedInto(item*) const RET(true)
	virtual material* BeDippedInto();
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60000; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,144))
	virtual ulong Price() const { return GetMaterial(0) ? GetMaterial(0)->RawPrice() : 0; }
	virtual void SetBloodColor(ushort);
protected:
	virtual ushort GetFormModifier() const RET(15)
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
	virtual ushort Possibility() const RET(100)
	virtual uchar GetConsumeType() const RET(GetMaterial(1) ? GetMaterial(1)->GetConsumeType() : ODD)
	virtual bool Consume(character*, float);
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual std::string NameSingular() const RET("bottle")
	virtual bool ImpactDamage(ushort, bool, stack*);
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual float OfferModifier() const RET(0.1f)
	virtual vector2d GetBitmapPos() const RETV(0,160)
	virtual bool ReceiveSound(float, bool, stack*);
	virtual uchar GetWeaponCategory() const { return CLUBS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; case 1: return 1500; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
	virtual void ColorChangeSpeciality(uchar, bool);
	virtual ulong Price() const { return GetMaterial(1) ? GetMaterial(1)->RawPrice() : 0; }
	virtual ulong ConsumeLimit() const { return GetMaterial(1)->GetVolume(); }
	virtual uchar GetConsumeMaterial() const { return 1; }
	virtual item* BetterVersion() const;
	virtual std::string GetConsumeVerb() const { return std::string("drinking"); }
protected:
	virtual ushort GetFormModifier() const RET(40)
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
	virtual ushort Possibility() const RET(100)
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", bananapeal::StaticType()))
	virtual std::string NameSingular() const RET("banana peal")
	virtual item* BetterVersion() const { return new banana; }
	virtual float OfferModifier() const RET(0)
	virtual vector2d GetBitmapPos() const RETV(0,128)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,112))
protected:
	virtual ushort GetFormModifier() const RET(70)
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
	virtual ushort Possibility() const RET(50)
	virtual std::string NameSingular() const RET("broken bottle")
	virtual item* BetterVersion() const;
	virtual float OfferModifier() const RET(0)
	virtual vector2d GetBitmapPos() const RETV(16,160)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
	virtual void GetStepOnEffect(character*);
protected:
	virtual ushort GetFormModifier() const RET(100)
);

class ABSTRACT_ITEM
(
	scroll,
	item,
public:
	virtual bool CanBeRead(character*) const;
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
	virtual vector2d GetBitmapPos() const RETV(16,176)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
protected:
	virtual ushort GetFormModifier() const RET(40)
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
	virtual ushort Possibility() const RET(100)
	virtual std::string NameSingular() const RET("scroll of create monster")
	virtual std::string NamePlural() const RET("scrolls of create monster")
	virtual float OfferModifier() const RET(5)
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
	virtual ushort Possibility() const RET(50)
	virtual std::string NameSingular() const RET("scroll of teleportation")
	virtual std::string NamePlural() const RET("scrolls of teleportation")
	virtual float OfferModifier() const RET(5)
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
	virtual ushort Possibility() const RET(0)
	virtual std::string NameSingular() const RET("head")
	virtual float OfferModifier() const RET(0.1f)
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual bool CanBeWished() const RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,144))
protected:
	virtual ushort GetFormModifier() const RET(20)
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
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, elpuriflesh::StaticType()))
	virtual bool IsHeadOfElpuri() const RET(true)
	virtual std::string NameSingular() const RET("head of Elpuri")
	virtual long Score() const RET(1000);
	virtual vector2d GetBitmapPos() const RETV(16,0)
	virtual bool CanBeWished() const RET(false)
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
	virtual ushort Possibility() const RET(5)
	virtual std::string NameSingular() const RET("nut")
	virtual float OfferModifier() const RET(10)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 25; default: return 0; } }
	virtual vector2d GetBitmapPos() const RETV(16,208)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
protected:
	virtual ushort GetFormModifier() const RET(20)
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
	virtual ushort Possibility() const RET(0)
	virtual bool IsPetrussNut() const RET(true)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, humanflesh::StaticType()))
	virtual std::string NameSingular() const RET("left nut of Petrus")
	virtual long Score() const RET(5000)
	virtual item* CreateWishedItem() const;
	virtual bool Destroyable() const { return false; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
	virtual ushort GetEmitation() const RET(333)
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
	virtual ushort Possibility() const RET(100)
	virtual std::string NameSingular() const RET("bone")
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", bone::StaticType()))
	virtual float OfferModifier() const RET(0.1f)
	virtual bool DogWillCatchAndConsume() const RET(true);
	virtual vector2d GetBitmapPos() const RETV(16,240)
	virtual uchar GetWeaponCategory() const { return CLUBS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,32))
protected:
	virtual ushort GetFormModifier() const RET(70)
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
	virtual ushort Possibility() const RET(50)
	virtual std::string NameSingular() const RET("poleaxe")
	virtual float OfferModifier() const RET(0.25f)
	virtual vector2d GetBitmapPos() const RETV(0,80)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2500; case 1: return 2000; case 2: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(150)
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
	virtual ushort Possibility() const RET(20)
	virtual std::string NameSingular() const RET("spiked mace")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 192))
	virtual float OfferModifier() const RET(0.125f)
	virtual vector2d GetBitmapPos() const RETV(0,32)
	virtual uchar GetWeaponCategory() const { return MACES; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; case 1: return 2000; case 2: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(125)
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
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, mithril::StaticType()))
	virtual std::string NameSingular() const RET("ancient mace \"Neerc Se-Ulb\"")
	virtual float OfferModifier() const RET(0.25)
	virtual long Score() const RET(1000)
	virtual bool CanBeWished() const RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 12000; case 1: return 6000; case 2: return 2500; default: return 0; } }
	virtual bool IsMaterialChangeable() const { return false; }
	virtual bool Polymorph(stack*) { return false; }
protected:
	virtual ushort GetFormModifier() const RET(150)
);

class ITEM
(
	loaf,
	item,
	InitMaterials(RAND() % 2 ? (material*)new pork : (material*)new beef),
	{
		SetSize(40);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual std::string Name(uchar Case) const RET(NameThingsThatAreLikeLumps(Case, "a")) 
	virtual std::string NameSingular() const RET("loaf")
	virtual std::string NamePlural() const RET("loaves")
	virtual float OfferModifier() const RET(0.125)
	virtual vector2d GetBitmapPos() const RETV(0,272)
	virtual uchar GetWeaponCategory() const { return CLUBS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
	virtual ulong Price() const { return GetMaterial(0) ? GetMaterial(0)->RawPrice() : 0; }
protected:
	virtual ushort GetFormModifier() const RET(30)
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
	virtual ushort Possibility() const RET(1)
	virtual std::string NameSingular() const RET("scroll of wishing")
	virtual std::string NamePlural() const RET("scrolls of wishing")
	virtual float OfferModifier() const RET(50)
	virtual bool CanBeWished() const RET(false)
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
	virtual ushort Possibility() const RET(5)
	virtual std::string NameSingular() const RET("cheap copy of the left nut of Petrus")
	virtual std::string NamePlural() const RET("cheap copies of the left nut of Petrus")
	virtual long Score() const RET(1)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
	virtual ulong Price() const { return 500; }
);

class ABSTRACT_ITEM
(
	wand,
	item,
public:
	virtual bool Apply(character*, stack*);
	virtual bool CanBeZapped() const RET(true)
	virtual uchar GetCharge() const { return Charge; } 
	virtual void SetCharge(uchar What) { Charge = What; }
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual vector2d GetBitmapPos() const RETV(0,288)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
	virtual bool IsExplosive() const { return true; }
protected:
	virtual ushort GetFormModifier() const RET(80)
	uchar Charge;
);

class ITEM
(
	wandofpolymorph,
	wand,
	InitMaterials(new glass),
	{
		SetSize(10);
		SetCharge(2 + RAND() % 4);
	},
public:
	virtual ushort Possibility() const RET(20)
	virtual std::string NameSingular() const RET("wand of polymorph")
	virtual std::string NamePlural() const RET("wands of polymorph")
	virtual float OfferModifier() const RET(30)
	virtual bool Zap(character*, vector2d, uchar);
	virtual ulong Price() const { return 500; }
	virtual bool PolymorphSpawnable() const { return false; }
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
	virtual ushort Possibility() const RET(200)
	virtual std::string NameSingular() const RET("arrow")
	virtual float OfferModifier() const RET(0.5f)
	virtual vector2d GetBitmapPos() const RETV(16,80)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 100; default: return 0; } }
	virtual bool UseThrowStrengthModifier() const RET(true)
protected:
	virtual ushort GetFormModifier() const RET(50)
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
	virtual ushort Possibility() const RET(5)
	virtual std::string Name(uchar Case) const RET(NameNormal(Case, "a"))
	virtual std::string NameSingular() const RET("head of an Enner Beast")
	virtual long Score() const RET(250);
	virtual vector2d GetBitmapPos() const RETV(0,176)
	virtual bool CanBeWished() const RET(true)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; default: return 0; } }
	virtual ulong Price() const { return 100; }
);

class ITEM
(
	brokenlamp,
	lamp,
	InitMaterials(new glass),
	{
		SetSize(15);
		SetOnWall(false);
		UpdatePicture();
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual std::string NameSingular() const RET("broken lamp")
	virtual item* BetterVersion() const { return new lamp; }
	virtual float OfferModifier() const RET(0)
	virtual vector2d GetBitmapPos() const { return vector2d(0, OnWall ? 208 : 304); }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
	virtual bool ImpactDamage(ushort, bool, stack*) { return false; }
	virtual ushort GetEmitation() const RET(0)
protected:
	virtual ushort GetFormModifier() const RET(80)
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
	virtual ushort Possibility() const RET(3)
	virtual std::string NameSingular() const RET("scroll of change material")
	virtual std::string NamePlural() const RET("scrolls of change material")
	virtual float OfferModifier() const RET(40)
	virtual bool Read(character*);
	virtual ulong Price() const { return 500; }
);

class ITEM
(
	avatarofvalpuri,
	item,
	InitMaterials(new valpurium),
	{
		SetSize(10);
	},
public:
	virtual bool IsTheAvatar() const RET(true)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, valpurium::StaticType()))
	virtual ushort Possibility() const RET(0)
	virtual std::string NameSingular() const RET("Avatar of Valpuri")
	virtual vector2d GetBitmapPos() const RETV(0,320)
	virtual bool CanBeWished() const RET(false)
	virtual bool Destroyable() const { return false; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,112))
	virtual bool IsMaterialChangeable() const { return false; }
	virtual bool Consumable(character*) const { return false; }
	virtual bool Polymorph(stack*) { return false; }
protected:
	virtual ushort GetFormModifier() const RET(20)
);

class ITEM
(
	wandofstriking,
	wand,
	InitMaterials(new marble),
	{
		SetSize(12);
		SetCharge(1 + RAND() % 3);
	},
public:
	virtual ushort Possibility() const RET(20)
	virtual std::string NameSingular() const RET("wand of striking")
	virtual std::string NamePlural() const RET("wands of striking")
	virtual float OfferModifier() const RET(10)
	virtual bool Zap(character*, vector2d, uchar);
	virtual ulong Price() const { return 500; }
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
	virtual ushort Possibility() const RET(10)
	virtual std::string NameSingular() const RET("broken plate mail")
	virtual float OfferModifier() const RET(0.1f)
	virtual vector2d GetBitmapPos() const RETV(0,352)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,144))
	virtual ushort GetArmorValue() const;
protected:
	virtual ushort GetFormModifier() const RET(30)
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
	virtual ushort Possibility() const RET(20)
	virtual std::string NameSingular() const RET("bow")
	virtual float OfferModifier() const RET(0.3f)
	virtual vector2d GetBitmapPos() const RETV(16,320)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
	virtual float GetThrowStrengthModifier() const RET(4)
protected:
	virtual ushort GetFormModifier() const RET(40)
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
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", kiwiflesh::StaticType()))
	virtual ushort Possibility() const RET(20)
	virtual std::string NameSingular() const RET("kiwi")
	virtual float OfferModifier() const RET(0.4f)
	virtual vector2d GetBitmapPos() const RETV(0,384)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(20)
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
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", pineappleflesh::StaticType()))
	virtual ushort Possibility() const RET(25)
	virtual std::string NameSingular() const RET("pineapple")
	virtual float OfferModifier() const RET(0.4f)
	virtual vector2d GetBitmapPos() const RETV(0,368)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(20)
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
	virtual vector2d GetBitmapPos() const RETV(0,240)
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", palmleaf::StaticType()))
	virtual ushort Possibility() const RET(10)
	virtual std::string NameSingular() const RET("palm branch")
	virtual float OfferModifier() const RET(0.3f)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 208))
protected:
	virtual ushort GetFormModifier() const RET(50)
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
	virtual ushort Possibility() const RET(25)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,224))
	virtual std::string NameSingular() const RET("whip")
	virtual float OfferModifier() const RET(0.5f)
	virtual vector2d GetBitmapPos() const RETV(32,0)
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
	virtual ushort Possibility() const RET(10)
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual uchar GetConsumeType() const RET(GetMaterial(1) ? GetMaterial(1)->GetConsumeType() : ODD)
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 144))
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(32, 16)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; case 1: return 10000; default: return 0; } }
	virtual ulong Price() const { return GetMaterial(1) ? GetMaterial(1)->RawPrice() : 0; }
	virtual uchar GetConsumeMaterial() const { return 1; }
	virtual bool Apply(character*, stack*);
	virtual bool IsExplosive() const { return (GetMaterial(1) && GetMaterial(1)->IsExplosive()) ? true : false; }
protected:
	virtual std::string NameSingular() const RET("backpack")
	virtual ushort GetFormModifier() const RET(20)
);

class ITEM
(
	holybook,
	item,
	InitMaterials(new parchment),
	{	
		SetSize(25);
		SetOwnerGod(1 + RAND() % game::GetGodNumber());
	},
public:
	virtual bool CanBeRead(character*) const;
	virtual std::string Name(uchar Case) const { return NameHandleDefaultMaterial(Case, "a", parchment::StaticType()) + OwnerGodDescription(OwnerGod); }
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 128))
	virtual ushort Possibility() const RET(5)
	virtual float OfferModifier() const RET(0.4f)
	virtual vector2d GetBitmapPos() const RETV(32,32)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2000; default: return 0; } }
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual uchar GetOwnerGod() const { return OwnerGod; }
	virtual void SetOwnerGod(uchar What) { OwnerGod = What; }
	virtual bool Read(character*);
protected:
	virtual std::string NameSingular() const RET("holy book")
	virtual ushort GetFormModifier() const RET(30)
	uchar OwnerGod;
);

#endif
