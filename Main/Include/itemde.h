#ifndef __ITEMDE_H__
#define __ITEMDE_H__

#include "itemba.h"
#include "materde.h"

class ABSTRACT_ITEM
(
	meleeweapon,
	item,
public:
	virtual void ReceiveHitEffect(character*, character*);
	virtual void DipInto(item*);
	virtual bool CanBeDippedInto(item*) const RET(Material[2] ? false : true)
	virtual bool CanBeDipped() const RET(true)
);

class ITEM
(
	banana,
	item,
	InitMaterials(2, new bananapeal(30), new bananaflesh(150)),
	{
		SetSize(20);
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual std::string Name(uchar Case) const RET(NameSized(Case,"a", 15, 40))
	virtual uchar GetConsumeType() const RET(Material[1]->GetConsumeType())
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 112))
	virtual bool Consume(character*, float = 100);
	virtual std::string NameSingular() const RET("banana")
	virtual std::string NamePlural() const RET("bananas")
	virtual float OfferModifier() const RET(1)
	virtual vector2d GetBitmapPos() const RETV(0,112)
protected:
	virtual ushort GetFormModifier() const RET(25)
);

class ITEM
(
	holybananaofliukasvipro,
	banana,
	InitMaterials(2, new bananapeal(40), new bananaflesh(300)),
	{
		SetSize(30);
	},
public:
	virtual ushort Possibility() const RET(1)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, bananapeal::StaticType()))
	virtual std::string NameSingular() const RET("holy banana of Liukas Vipro")
	virtual std::string NamePlural() const RET("holy bananas of Liukas Vipro")
	virtual float OfferModifier() const RET(40)
	virtual long Score() const RET(250)
	virtual vector2d GetBitmapPos() const RETV(0,112)
protected:
	virtual ushort GetFormModifier() const RET(35)
);

class ITEM
(
	lamp,
	item,
	InitMaterials(new glass(850)),
	{
		SetSize(30);
	},
public:
	virtual ushort Possibility() const RET(10)
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual ushort GetEmitation() const RET(300)
	virtual std::string NameSingular() const RET("lamp")
	virtual std::string NamePlural() const RET("lamps")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 128))
	virtual float OfferModifier() const RET(1)
	virtual vector2d GetBitmapPos() const RETV(0,192)
	virtual void ImpactDamage(ushort, bool, stack*);
	virtual bool ReceiveSound(float, bool, stack*);
protected:
	virtual ushort GetFormModifier() const RET(30)
);

class ITEM
(
	can,
	item,
	InitMaterials(2, new iron(50), rand() % 2 ? (material*)new bananaflesh(600) : (material*)new schoolfood(600)),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(100)
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual ushort TryToOpen(stack*);
	virtual uchar GetConsumeType() const RET(Material[1] ? Material[1]->GetConsumeType() : ODD)
	virtual std::string NameSingular() const RET("can")
	virtual std::string NamePlural() const RET("cans")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 144))
	virtual float OfferModifier() const RET(0.5)
	virtual ushort PrepareForConsuming(character*, stack*);
	virtual vector2d GetBitmapPos() const;
protected:
	virtual ushort GetFormModifier() const RET(20)
);

class ITEM
(
	lump,
	item,
	InitMaterials(rand() % 2 ? (material*)new bananaflesh(600) : (material*)new schoolfood(600)),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameThingsThatAreLikeLumps(Case, "a")) 
	virtual uchar GetConsumeType() const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual void ReceiveHitEffect(character*, character*);
	virtual bool CanBeDippedInto(item*) const RET(true)
	virtual material* BeDippedInto();
	virtual std::string NameSingular() const RET("lump")
	virtual std::string NamePlural() const RET("lumps")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 112))
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(144,48)
	virtual bool CanBeWished() const RET(false)
protected:
	virtual ushort GetFormModifier() const RET(10)
);

class ITEM
(
	sword,
	meleeweapon,
	InitMaterials(3, new iron(2500), new iron(100), 0),
	{
		SetSize(150);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,32))
	virtual std::string NameSingular() const RET("sword")
	virtual std::string NamePlural() const RET("swords")
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(0,0)
protected:
	virtual ushort GetFormModifier() const RET(100)
);

class ITEM
(
	twohandedsword,
	sword,
	InitMaterials(3, new iron(5500), new iron(250), 0),
	{
		SetSize(175);
	},
public:
	virtual ushort Possibility() const RET(3)
	virtual std::string NameSingular() const RET("two-handed sword")
	virtual std::string NamePlural() const RET("two-handed swords")
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual float OfferModifier() const RET(0.25)
protected:
	virtual ushort GetFormModifier() const RET(125)
);

class ITEM
(
	curvedtwohandedsword,
	twohandedsword,
	InitMaterials(3, new iron(5500), new iron(250), 0),
	{
		SetSize(175);
	},
public:
	virtual ushort Possibility() const RET(1)
	virtual std::string NameSingular() const RET("curved two-handed sword")
	virtual std::string NamePlural() const RET("curved two-handed swords")
	virtual float OfferModifier() const RET(0.25)
	virtual vector2d GetBitmapPos() const RETV(0,16)
protected:
	virtual ushort GetFormModifier() const RET(150)
);

class ITEM
(
	valpurijustifier,
	sword,
	InitMaterials(3, new valpurium(6500), new valpurium(300), 0),
	{
		SetSize(200);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, valpurium::StaticType()))
	virtual std::string NameSingular() const RET("holy broadsword named Valpuri's Justifier")
	virtual std::string NamePlural() const RET("holy broadswords named Valpuri's Justifier")
	virtual float OfferModifier() const RET(0.5)
	virtual long Score() const RET(1000)
	virtual vector2d GetBitmapPos() const RETV(0,64)
	virtual bool CanBeWished() const RET(false)
protected:
	virtual ushort GetFormModifier() const RET(400)
);

class ITEM
(
	axe,
	meleeweapon,
	InitMaterials(3, new iron(450), new iron(900), 0),
	{
		SetSize(125);
	},
public:
	virtual ushort Possibility() const RET(25)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,16))
	virtual std::string NameSingular() const RET("axe")
	virtual std::string NamePlural() const RET("axes")
	virtual float OfferModifier() const RET(0.25)
	virtual vector2d GetBitmapPos() const RETV(144,256)
protected:
	virtual ushort GetFormModifier() const RET(150)
);

class ITEM
(
	pickaxe,
	axe,
	InitMaterials(3, new iron(1000), new wood(1050), 0),
	{
		SetSize(150);
	},
public:
	virtual ushort Possibility() const RET(10)
	virtual std::string NameSingular() const RET("pick-axe")
	virtual std::string NamePlural() const RET("pick-axes")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 64))
	virtual float OfferModifier() const RET(0.25)
	virtual vector2d GetBitmapPos() const RETV(0,96)
	virtual bool Apply(character*);
protected:
	virtual ushort GetFormModifier() const RET(150)
);

class ITEM
(
	spear,
	meleeweapon,
	InitMaterials(3, new iron(150), new wood(1500), 0),
	{
		SetSize(200);
	},
public:
	virtual ushort Possibility() const RET(25)
	virtual vector2d GetInHandsPic() const RET(vector2d(160,96))
	virtual bool CanBeDippedInto(item*) const RET(Material[2] ? false : true)
	virtual std::string NameSingular() const RET("spear")
	virtual std::string NamePlural() const RET("spears")
	virtual float OfferModifier() const RET(1)
	virtual vector2d GetBitmapPos() const RETV(144,144)
protected:
	virtual ushort GetFormModifier() const RET(200)
);

class ABSTRACT_ITEM
(
	torsoarmor,
	item,
public:
	virtual bool CanBeWorn() const RET(true)
protected:
	virtual ushort GetFormModifier() const RET(15)
);

class ITEM
(
	platemail,
	torsoarmor,
	InitMaterials(new iron(4000)),
	{
		SetSize(75);
	},
public:
	virtual ushort Possibility() const RET(3)
	virtual ushort GetArmorValue() const;
	virtual std::string NameSingular() const RET("plate mail")
	virtual std::string NamePlural() const RET("plate mails")
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(144,128)
);

class ITEM
(
	chainmail,
	torsoarmor,
	InitMaterials(new iron(2000)),
	{
		SetSize(75);
	},
public:
	virtual ushort Possibility() const RET(10)
	virtual ushort GetArmorValue() const;
	virtual std::string NameSingular() const RET("chain mail")
	virtual std::string NamePlural() const RET("chain mails")
	virtual float OfferModifier() const RET(0.5)
	virtual vector2d GetBitmapPos() const RETV(144,96)
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
	InitMaterials(new cloth(1000)),
	{
		SetSize(60);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual ushort GetArmorValue() const RET(10)
	virtual std::string NameSingular() const RET("Maakotka shirt")
	virtual std::string NamePlural() const RET("Maakotka shirts")
	virtual short CalculateOfferValue(char) const RET(750)
	virtual long Score() const RET(1000)
	virtual bool IsMaakotkaShirt() const RET(true);
	virtual vector2d GetBitmapPos() const RETV(144,112)
	virtual bool CanBeWished() const RET(false)
	virtual bool Destroyable() const { return false; }
	virtual ushort GetEmitation() const RET(333)
);

class ITEM
(
	corpse,
	item,
	InitMaterials(new humanflesh(60000)),
	{
		SetSize(0);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual uchar GetConsumeType() const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual std::string NameSingular() const RET("corpse")
	virtual std::string NamePlural() const RET("corpses")
	virtual float OfferModifier() const RET(0.01f)
	virtual vector2d GetBitmapPos() const RETV(144,192)
	virtual bool CanBeWished() const RET(false)
protected:
	virtual ushort GetFormModifier() const RET(20)
);

class ITEM
(
	potion,
	item,
	InitMaterials(2, new glass(50), rand() % 3 ? 0 : new omleurine(1500)),
	{
		SetSize(30);
	},
public:
	virtual ushort Possibility() const RET(25)
	virtual uchar GetConsumeType() const RET(Material[1] ? Material[1]->GetConsumeType() : ODD)
	virtual bool Consume(character*, float = 100);
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual std::string NameSingular() const RET("bottle")
	virtual std::string NamePlural() const RET("bottles")
	virtual void ImpactDamage(ushort, bool, stack*);
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual float OfferModifier() const RET(0.1f)
	virtual vector2d GetBitmapPos() const RETV(0,144)
protected:
	virtual ushort GetFormModifier() const RET(40)
);

class ITEM
(
	bananapeals,
	item,
	InitMaterials(new bananapeal(15)),
	{
		SetSize(20);
	},
public:
	virtual ushort Possibility() const RET(25)
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", bananapeal::StaticType()))
	virtual std::string NameSingular() const RET("banana peal")
	virtual std::string NamePlural() const RET("banana peals")
	virtual item* BetterVersion() const { return new banana; }
	virtual float OfferModifier() const RET(0)
	virtual long Score() const RET(-1)
	virtual vector2d GetBitmapPos() const RETV(0,128)
protected:
	virtual ushort GetFormModifier() const RET(20)
);

class ITEM
(
	brokenbottle,
	item,
	InitMaterials(new glass(50)),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(25)
	virtual std::string NameSingular() const RET("broken bottle")
	virtual std::string NamePlural() const RET("broken bottles")
	virtual item* BetterVersion() const { item* P = new potion(false); P->InitMaterials(2, new glass(50), new omleurine(1500)); return P; }
	virtual float OfferModifier() const RET(0)
	virtual vector2d GetBitmapPos() const RETV(144,160)
protected:
	virtual ushort GetFormModifier() const RET(60)
);

class ABSTRACT_ITEM
(
	scroll,
	item,
public:
	virtual bool CanBeRead(character*) const;
protected:
	virtual ushort GetFormModifier() const RET(30)
);

class ITEM
(
	scrollofcreatemonster,
	scroll,
	InitMaterials(new parchment(200)),
	{
		SetSize(30);
	},
public:
	virtual ushort Possibility() const RET(25)
	virtual std::string NameSingular() const RET("scroll of create monster")
	virtual std::string NamePlural() const RET("scrolls of create monster")
	virtual float OfferModifier() const RET(5)
	virtual vector2d GetBitmapPos() const RETV(144,176)
	virtual bool Read(character*);
);

class ITEM
(
	scrollofteleport,
	scroll,
	InitMaterials(new parchment(200)),
	{
		SetSize(30);
	},
public:
	virtual ushort Possibility() const RET(25)
	virtual std::string NameSingular() const RET("scroll of teleportation")
	virtual std::string NamePlural() const RET("scrolls of teleportation")
	virtual float OfferModifier() const RET(5)
	virtual vector2d GetBitmapPos() const RETV(144,176)
	virtual bool Read(character*);
);

class ITEM
(
	head,
	item,
	InitMaterials(new humanflesh(2000)),
	{
		SetSize(0);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string NameSingular() const RET("head")
	virtual std::string NamePlural() const RET("heads")
	virtual float OfferModifier() const RET(0.1f)
	virtual vector2d GetBitmapPos() const RETV(0,0)
	virtual bool CanBeWished() const RET(false)
protected:
	virtual ushort GetFormModifier() const RET(10)
);

class ITEM
(
	headofelpuri,
	head,
	InitMaterials(new elpuriflesh(25000)),
	{
		SetSize(60);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, elpuriflesh::StaticType()))
	virtual bool IsHeadOfElpuri() const RET(true)
	virtual std::string NameSingular() const RET("head of Elpuri")
	virtual std::string NamePlural() const RET("heads of Elpuri")
	virtual long Score() const RET(500);
	virtual vector2d GetBitmapPos() const RETV(144,0)
	virtual bool CanBeWished() const RET(false)
	virtual bool Destroyable() const { return false; }
);

class ITEM
(
	nut,
	item,
	InitMaterials(new humanflesh(15)),
	{
		SetSize(0);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string NameSingular() const RET("nut")
	virtual std::string NamePlural() const RET("nuts")
	virtual float OfferModifier() const RET(10)
	virtual vector2d GetBitmapPos() const RETV(0,0)
protected:
	virtual ushort GetFormModifier() const RET(10)
);

class ITEM
(
	leftnutofperttu,
	nut,
	InitMaterials(new humanflesh(150)),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual bool IsPerttusNut() const RET(true)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, humanflesh::StaticType()))
	virtual std::string NameSingular() const RET("left nut of Perttu")
	virtual std::string NamePlural() const RET("left nuts of Perttu")		//???
	virtual long Score() const RET(2500)
	virtual vector2d GetBitmapPos() const RETV(144,208)
	virtual item* CreateWishedItem() const;
	virtual bool Destroyable() const { return false; }
);

class ITEM
(
	abone,
	item,
	InitMaterials(new bone(2000)),
	{
		SetSize(50);
	},
public:
	virtual ushort Possibility() const RET(50)
	virtual std::string NameSingular() const RET("bone")
	virtual std::string NamePlural() const RET("bones")
	virtual std::string Name(uchar Case) const RET(NameSized(Case,"a", 15, 40))
	virtual float OfferModifier() const RET(0.1f)
	virtual bool Consume(character*, float = 100);
	virtual bool DogWillCatchAndConsume() const RET(true);
	virtual uchar GetConsumeType() const RET(Material[0]->GetConsumeType());
	virtual vector2d GetBitmapPos() const RETV(144,240)
protected:
	virtual ushort GetFormModifier() const RET(50)
);

class ITEM
(
	poleaxe,
	axe,
	InitMaterials(3, new iron(1500), new wood(3000), 0),
	{
		SetSize(225);
	},
public:
	virtual ushort Possibility() const RET(15)
	virtual std::string NameSingular() const RET("poleaxe")
	virtual std::string NamePlural() const RET("poleaxes")
	virtual float OfferModifier() const RET(0.25f)
	virtual vector2d GetBitmapPos() const RETV(0,80)
protected:
	virtual ushort GetFormModifier() const RET(100)
);

class ITEM
(
	spikedmace,
	meleeweapon,
	InitMaterials(3, new iron(5000), new wood(3000), 0),
	{
		SetSize(150);
	},
public:
	virtual ushort Possibility() const RET(5)
	virtual std::string NameSingular() const RET("spiked mace")
	virtual std::string NamePlural() const RET("spiked maces")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 0))
	virtual float OfferModifier() const RET(0.125)
	virtual vector2d GetBitmapPos() const RETV(0,32)
protected:
	virtual ushort GetFormModifier() const RET(75)
);

class ITEM
(
	htaedfoneercseulb,
	spikedmace,
	InitMaterials(3, new mithril(15000), new iron(8000), new darkfrogflesh(1000)),
	{
		SetSize(200);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, mithril::StaticType()))
	virtual std::string NameSingular() const RET("ancient mace named H'taed Foneer Cse-ulb")
	virtual std::string NamePlural() const RET("ancient maces named H'taed Foneer Cse-ulb")
	virtual vector2d GetInHandsPic() const RET(vector2d(160, 0))
	virtual float OfferModifier() const RET(0.25)
	virtual long Score() const RET(1000)
	virtual vector2d GetBitmapPos() const RETV(0,32)
	virtual bool CanBeWished() const RET(false)
protected:
	virtual ushort GetFormModifier() const RET(100)
);

class ITEM
(
	loaf,
	item,
	InitMaterials(rand() % 2 ? (material*)new pork(2000) : (material*)new beef(2000)),
	{
		SetSize(40);
	},
public:
	virtual ushort Possibility() const RET(200)
	virtual std::string Name(uchar Case) const RET(NameThingsThatAreLikeLumps(Case, "a")) 
	virtual std::string NameSingular() const RET("loaf")
	virtual std::string NamePlural() const RET("loaves")
	virtual uchar GetConsumeType() const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual float OfferModifier() const RET(0.125)
	virtual vector2d GetBitmapPos() const RETV(0,272)
protected:
	virtual ushort GetFormModifier() const RET(15)
);

class ITEM
(
	scrollofwishing,
	scroll,
	InitMaterials(new parchment(200)),
	{
		SetSize(30);
	},
public:
	virtual ushort Possibility() const RET(1)
	virtual std::string NameSingular() const RET("scroll of wishing")
	virtual std::string NamePlural() const RET("scrolls of wishing")
	virtual float OfferModifier() const RET(50)
	virtual vector2d GetBitmapPos() const RETV(144,176)
	virtual bool CanBeWished() const RET(false)
	virtual bool Read(character*);
);

class ITEM
(
	cheapcopyofleftnutofperttu,
	nut,
	InitMaterials(new glass(150)),
	{
		SetSize(10);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string NameSingular() const RET("cheap copy of left nut of Perttu")
	virtual std::string NamePlural() const RET("cheap copies of left nut of Perttu")
	virtual long Score() const RET(1)
	virtual vector2d GetBitmapPos() const RETV(144,208)
);

class ABSTRACT_ITEM
(
	wand,
	item,
public:
	virtual bool Apply(character*);
	virtual bool CanBeZapped() const RET(true)
	virtual uchar GetCharge() const { return Charge; } 
	virtual void SetCharge(uchar What) { Charge = What; }
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
protected:
	virtual ushort GetFormModifier() const RET(40)
	uchar Charge;
);

class ITEM
(
	wandofpolymorph,
	wand,
	InitMaterials(new glass(100)),
	{
		SetSize(10);
		SetCharge(rand() % 6);
	},
public:
	virtual ushort Possibility() const RET(3)
	virtual std::string NameSingular() const RET("wand of polymorph")
	virtual std::string NamePlural() const RET("wands of polymorph")
	virtual float OfferModifier() const RET(30)
	virtual vector2d GetBitmapPos() const RETV(0,288)
	virtual bool Zap(vector2d, uchar);
);

class ITEM
(
	arrow,
	item,
	InitMaterials(2, new wood(100), new iron(4)),
	{
		SetSize(100);
	},
public:
	virtual ushort Possibility() const RET(40)
	virtual std::string NameSingular() const RET("arrow")
	virtual std::string NamePlural() const RET("arrows")
	virtual float OfferModifier() const RET(0.5f)
	virtual vector2d GetBitmapPos() const RETV(144,80)
protected:
	virtual ushort GetFormModifier() const RET(70)
);

class ITEM
(
	headofennerbeast,
	head,
	InitMaterials(new ennerbeastflesh(7000)),
	{
		SetSize(50);
	},
public:
	virtual ushort Possibility() const RET(0)
	virtual std::string Name(uchar Case) const RET(NameNormal(Case, "a"))
	virtual std::string NameSingular() const RET("head of an Enner Beast")
	virtual std::string NamePlural() const RET("heads of an Enner Beast")
	virtual long Score() const RET(250);
	virtual vector2d GetBitmapPos() const RETV(0,176)
	virtual bool CanBeWished() const RET(true)
);

class ITEM
(
	brokenlamp,
	item,
	InitMaterials(new glass(70)),
	{
		SetSize(15);
	},
public:
	virtual ushort Possibility() const RET(3)
	virtual std::string NameSingular() const RET("broken lamp")
	virtual std::string NamePlural() const RET("broken lamps")
	virtual item* BetterVersion() const { item* P = new lamp(new glass(50)); return P; }
	virtual float OfferModifier() const RET(0)
	virtual vector2d GetBitmapPos() const RETV(0,304)
protected:
	virtual ushort GetFormModifier() const RET(55)
);

#endif
