#ifndef __ITEMDE_H__
#define __ITEMDE_H__

#include "itemba.h"
#include "materde.h"
#include "wskill.h"


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
	virtual std::string Name(uchar Case) const RET(NameSized(Case,"a", 15, 40))
	virtual uchar GetConsumeType() const RET(Material[1]->GetConsumeType())
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 112))
	virtual bool Consume(character*, float = 100);
	virtual std::string NameSingular() const RET("banana")
	virtual float OfferModifier() const RET(1)
	virtual vector2d GetBitmapPos() const RETV(0,112)
	virtual uchar GetWeaponCategory() const { return CLUBS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 40; case 1: return 150; default: return 0; } }
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
	virtual ushort Possibility() const RET(10)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, bananapeal::StaticType()))
	virtual std::string NameSingular() const RET("holy banana of Liukas Vipro")
	virtual std::string NamePlural() const RET("holy bananas of Liukas Vipro")
	virtual float OfferModifier() const RET(40)
	virtual long Score() const RET(250)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 400; case 1: return 1500; default: return 0; } }
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
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual ushort GetEmitation() const RET(300)
	virtual std::string NameSingular() const RET("lamp")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 160))
	virtual float OfferModifier() const RET(1)
	virtual vector2d GetBitmapPos() const RETV(0,192)
	virtual bool ImpactDamage(ushort, bool, stack*);
	virtual bool ReceiveSound(float, bool, stack*);
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(30)
);

class ITEM
(
	can,
	item,
	InitMaterials(2, new iron, rand() % 2 ? (material*)new bananaflesh : (material*)new schoolfood),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(200)
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual item* TryToOpen(stack*);
	virtual uchar GetConsumeType() const RET(Material[1] ? Material[1]->GetConsumeType() : ODD)
	virtual std::string NameSingular() const RET("can")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 144))
	virtual float OfferModifier() const RET(0.5)
	virtual item* PrepareForConsuming(character*, stack*);
	virtual vector2d GetBitmapPos() const;
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 50; case 1: return 500; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(30)
);

class ITEM
(
	lump,
	item,
	InitMaterials(rand() % 2 ? (material*)new bananaflesh : (material*)new schoolfood),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(200)
	virtual std::string Name(uchar Case) const RET(NameThingsThatAreLikeLumps(Case, "a")) 
	virtual uchar GetConsumeType() const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual void ReceiveHitEffect(character*, character*);
	virtual bool CanBeDippedInto(item*) const RET(true)
	virtual material* BeDippedInto();
	virtual std::string NameSingular() const RET("lump")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 112))
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(16,48)
	virtual bool CanBeWished() const RET(false)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
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
	virtual bool CanBeDippedInto(item*) const RET(Material[GetMaterials()] ? false : true)
	virtual bool CanBeDipped() const RET(true)
);

class ITEM
(
	longsword,
	meleeweapon,
	InitMaterials(3, new iron, new iron, 0),
	{
		SetSize(150);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,16))
	virtual std::string NameSingular() const RET("long sword")
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2400; case 1: return 100; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(150)
);

class ITEM
(
	twohandedsword,
	longsword,
	InitMaterials(3, new iron, new iron, 0),
	{
		SetSize(175);
	},
public:
	virtual ushort Possibility() const RET(10)
	virtual std::string NameSingular() const RET("two-handed sword")
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual float OfferModifier() const RET(0.25)
	virtual uchar GetWeaponCategory() const { return LARGE_SWORDS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; default: return 0; } }
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
	virtual ushort Possibility() const RET(1)
	virtual std::string NameSingular() const RET("curved two-handed sword")
	virtual float OfferModifier() const RET(0.25)
	virtual vector2d GetBitmapPos() const RETV(0,16)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5500; case 1: return 250; default: return 0; } }
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
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 6700; case 1: return 300; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(400)
);

class ITEM
(
	axe,
	meleeweapon,
	InitMaterials(3, rand() % 5 ? (material*)new stone : (material*)new iron, new wood, 0),
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
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1400; case 1: return 1600; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(150)
);

class ITEM
(
	pickaxe,
	axe,
	InitMaterials(3, new iron, new wood, 0),
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
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1500; case 1: return 2000; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(100)
);

class ITEM
(
	spear,
	meleeweapon,
	InitMaterials(3, rand() % 5 ? (material*)new stone : (material*)new iron, new wood, 0),
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
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 200; case 1: return 1600; default: return 0; } }
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
	virtual ushort Possibility() const RET(0)
	virtual ushort GetArmorValue() const RET(10)
	virtual std::string NameSingular() const RET("Maakotka shirt")
	virtual short CalculateOfferValue(char) const RET(750)
	virtual long Score() const RET(1000)
	virtual bool IsMaakotkaShirt() const RET(true);
	virtual vector2d GetBitmapPos() const RETV(16,112)
	virtual bool CanBeWished() const RET(false)
	virtual bool Destroyable() const { return false; }
	virtual ushort GetEmitation() const RET(333)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
);

class ITEM
(
	corpse,
	item,
	InitMaterials(new humanflesh),
	{
		SetSize(0);
	},
public:
	virtual ushort Possibility() const RET(10)
	virtual uchar GetConsumeType() const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual std::string NameSingular() const RET("corpse")
	virtual float OfferModifier() const RET(0.01f)
	virtual vector2d GetBitmapPos() const RETV(16,192)
	virtual bool CanBeWished() const RET(false)
	virtual bool CanBeDippedInto(item*) const RET(true)
	virtual material* BeDippedInto();
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60000; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,144))
protected:
	virtual ushort GetFormModifier() const RET(15)
	virtual float NPModifier() const { return 0.01f; }
);

class ITEM
(
	potion,
	item,
	InitMaterials(2, new glass, rand() % 2 ? 0 : new omleurine),
	{
		SetSize(30);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual uchar GetConsumeType() const RET(Material[1] ? Material[1]->GetConsumeType() : ODD)
	virtual bool Consume(character*, float = 100);
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual std::string NameSingular() const RET("bottle")
	virtual bool ImpactDamage(ushort, bool, stack*);
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual float OfferModifier() const RET(0.1f)
	virtual vector2d GetBitmapPos() const RETV(0,144)
	virtual bool ReceiveSound(float, bool, stack*);
	virtual uchar GetWeaponCategory() const { return CLUBS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 60; case 1: return 1500; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
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
	virtual void GetStepOnEffect(character*, bool); 
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
	virtual long Score() const RET(500);
	virtual vector2d GetBitmapPos() const RETV(16,0)
	virtual bool CanBeWished() const RET(false)
	virtual bool Destroyable() const { return false; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 25000; default: return 0; } }
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
	virtual ushort Possibility() const RET(10)
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
	leftnutofperttu,
	nut,
	InitMaterials(new humanflesh),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual bool IsPerttusNut() const RET(true)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, humanflesh::StaticType()))
	virtual std::string NameSingular() const RET("left nut of Perttu")
	virtual long Score() const RET(2500)
	virtual item* CreateWishedItem() const;
	virtual bool Destroyable() const { return false; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
	virtual ushort GetEmitation() const RET(333)
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
	virtual std::string Name(uchar Case) const RET(NameSized(Case,"a", 15, 40))
	virtual float OfferModifier() const RET(0.1f)
	virtual bool Consume(character*, float = 100);
	virtual bool DogWillCatchAndConsume() const RET(true);
	virtual uchar GetConsumeType() const RET(Material[0]->GetConsumeType());
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
	InitMaterials(3, new iron, new wood, 0),
	{
		SetSize(225);
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual std::string NameSingular() const RET("poleaxe")
	virtual float OfferModifier() const RET(0.25f)
	virtual vector2d GetBitmapPos() const RETV(0,80)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 2500; case 1: return 2000; default: return 0; } }
protected:
	virtual ushort GetFormModifier() const RET(150)
);

class ITEM
(
	spikedmace,
	meleeweapon,
	InitMaterials(3, new iron, new wood, 0),
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
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; case 1: return 2000; default: return 0; } }
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
protected:
	virtual ushort GetFormModifier() const RET(150)
);

class ITEM
(
	loaf,
	item,
	InitMaterials(rand() % 2 ? (material*)new pork : (material*)new beef),
	{
		SetSize(40);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual std::string Name(uchar Case) const RET(NameThingsThatAreLikeLumps(Case, "a")) 
	virtual std::string NameSingular() const RET("loaf")
	virtual std::string NamePlural() const RET("loaves")
	virtual uchar GetConsumeType() const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual float OfferModifier() const RET(0.125)
	virtual vector2d GetBitmapPos() const RETV(0,272)
	virtual uchar GetWeaponCategory() const { return CLUBS; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
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
);

class ITEM
(
	cheapcopyofleftnutofperttu,
	nut,
	InitMaterials(new glass),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(10)
	virtual std::string NameSingular() const RET("cheap copy of the left nut of Perttu")
	virtual std::string NamePlural() const RET("cheap copies of the left nut of Perttu")
	virtual long Score() const RET(1)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 500; default: return 0; } }
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
		SetCharge(2 + rand() % 4);
	},
public:
	virtual ushort Possibility() const RET(20)
	virtual std::string NameSingular() const RET("wand of polymorph")
	virtual std::string NamePlural() const RET("wands of polymorph")
	virtual float OfferModifier() const RET(30)
	virtual bool Zap(character*, vector2d, uchar);
);

class ITEM
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
);

class ITEM
(
	headofennerbeast,
	head,
	InitMaterials(new ennerbeastflesh),
	{
		SetSize(50);
	},
public:
	virtual ushort Possibility() const RET(10)
	virtual std::string Name(uchar Case) const RET(NameNormal(Case, "a"))
	virtual std::string NameSingular() const RET("head of an Enner Beast")
	virtual long Score() const RET(250);
	virtual vector2d GetBitmapPos() const RETV(0,176)
	virtual bool CanBeWished() const RET(true)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 5000; default: return 0; } }
);

class ITEM
(
	brokenlamp,
	item,
	InitMaterials(new glass),
	{
		SetSize(15);
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual std::string NameSingular() const RET("broken lamp")
	virtual item* BetterVersion() const { item* P = new lamp; return P; }
	virtual float OfferModifier() const RET(0)
	virtual vector2d GetBitmapPos() const RETV(0,304)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 1000; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,128))
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
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, valpurium::StaticType()))
	virtual ushort Possibility() const RET(0)
	virtual std::string NameSingular() const RET("avatar of Valpuri")
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual bool CanBeWished() const RET(false)
	virtual bool Destroyable() const { return false; }
	virtual void CheckPickUpEffect(character*);
	virtual bool CanAIPickup() const { return false; }
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 250; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,112))
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
		SetCharge(1 + rand() % 3);
	},
public:
	virtual ushort Possibility() const RET(20)
	virtual std::string NameSingular() const RET("wand of striking")
	virtual std::string NamePlural() const RET("wands of striking")
	virtual float OfferModifier() const RET(10)
	virtual bool Zap(character*, vector2d, uchar);
);

class ITEM
(
	brokenplatemail,
	item,
	InitMaterials(new iron),
	{
		SetSize(70);
	},
public:
	virtual ushort Possibility() const RET(10)
	virtual std::string NameSingular() const RET("broken plate mail")
	virtual float OfferModifier() const RET(0.1f)
	virtual vector2d GetBitmapPos() const RETV(16,128)
	virtual ulong GetDefaultVolume(ushort Index) const { switch(Index) { case 0: return 4000; default: return 0; } }
	virtual vector2d GetInHandsPic() const RET(vector2d(160,144))
protected:
	virtual ushort GetFormModifier() const RET(30)
);

class ITEM
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
);

#endif
