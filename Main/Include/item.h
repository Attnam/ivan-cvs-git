#ifndef __ITEM_H__
#define __ITEM_H__

#include "typedef.h"
#include "vector.h"

#include "object.h"
#include "material.h"

#include "proto.h"

class bitmap;
class character;
class material;
class object;
class vector;
class stack;

/* Presentation of the item class */

class item : public object
{
public:
	item(bool, bool, bool = true);
	virtual float GetWeaponStrength(void) const;
	virtual void DrawToTileBuffer(void) const;
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
	virtual ushort GetEmitation(void) const;
	virtual vector GetInHandsPic(void) const { return vector(0,0); }
	virtual uchar GetConsumeType(void) const { return ODD; }
	virtual ushort TryToOpen(stack*) { return 0xFFFF; }
	virtual ulong GetWeight(void) const;
	virtual bool Consume(character*, float = 100) { return false; };
	virtual ushort GetArmorValue(void) const { return 100; }
	virtual bool IsHeadOfElpuri(void) const { return false; }
	virtual bool IsPerttusNut(void) const { return false; }
	virtual bool IsMaakotkaShirt(void) const { return false; }
	virtual bool CanBeRead(character*) const { return false; };
	virtual bool Read(character*) { return false; };
	virtual void ReceiveHitEffect(character*, character*) {}
	virtual bool CanBeDippedInto(item*) const { return false; }
	virtual void DipInto(item*) {}
	virtual material* BeDippedInto(void) { return 0; }
	virtual bool CanBeDipped(void) const { return false; }
	virtual bool CanBeWorn(void) const { return false; }
	virtual void SetMaterial(ushort Where, material* What) { Material[Where] = What; }
	virtual bool Consumable(character*) const;
	virtual item* BetterVersion(void) const { return 0; }
	virtual void ImpactDamage(ushort, bool, stack*) {};
	virtual short CalculateOfferValue(char) const;
	virtual float OfferModifier(void) const { return 0; }
	virtual long Score(void) const { return 0; }
	virtual bool Destroyable(void) const { return true; }
	virtual bool Fly(uchar, ushort, stack*, bool = true);
	virtual bool HitCharacter(character*, float, bool);
	virtual bool DogWillCatchAndConsume(void) const { return false; }
	virtual uchar GetDipMaterialNumber(void) const { return GetMaterials() - 1; }
	virtual ushort PrepareForConsuming(character*, stack*); // Stack where the item IS
	virtual item* Clone(bool = true, bool = true) const = 0;
	virtual ushort Possibility(void) const = 0;
	virtual bool CanBeWished(void) const { return true; }
	virtual item* CreateWishedItem(void) const;
	virtual bool Apply(character*);
	static ushort GetProtoIndexBegin(void) { return ProtoIndexBegin; }
	static ushort GetProtoIndexEnd(void) { return ProtoIndexEnd; }
	static ushort GetProtoAmount(void) { return ProtoIndexEnd - ProtoIndexBegin; }
	virtual bool Zap(vector, uchar);
	virtual bool CanBeZapped(void) const { return false; }
	virtual bool Polymorph(stack*);
protected:
	virtual void SetDefaultStats(void) = 0;
	virtual ushort GetFormModifier(void) const { return 0; }
	static ushort ProtoIndexBegin, ProtoIndexEnd;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define ITEM(name, base, initmaterials, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool = true, bool = true, bool = true);\
		name(material* Material, bool SetStats = true) : base(false, false) { InitMaterials(Material); if(SetStats) SetDefaultStats(); }\
		virtual item* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
		virtual typeable* CloneAndLoad(std::ifstream& SaveFile) const { item* Item = new name(false, false); Item->Load(SaveFile); return Item; }\
	protected:\
		virtual void SetDefaultStats(void);\
		virtual ushort Type(void) const;\
		data\
	};\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller(void) : Index(prototypesystem::Add(new name(false, false, false))) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	name::name(bool CreateMaterials, bool SetStats, bool AddToPool) : base(false, false, AddToPool) { if(CreateMaterials) initmaterials ; if(SetStats) SetDefaultStats(); }\
	void name::SetDefaultStats(void) { setstats }\
	ushort name::Type(void) const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define ITEM(name, base, initmaterials, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool = true, bool = true, bool = true);\
		name(material* Material, bool SetStats = true) : base(false, false) { InitMaterials(Material); if(SetStats) SetDefaultStats(); }\
		virtual item* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
		virtual typeable* CloneAndLoad(std::ifstream& SaveFile) const { item* Item = new name(false, false); Item->Load(SaveFile); return Item; }\
	protected:\
		virtual void SetDefaultStats(void);\
		virtual ushort Type(void) const;\
		data\
	};

#endif

#define ABSTRACT_ITEM(name, base, data)\
\
name : public base\
{\
public:\
	name(bool CreateMaterials, bool SetStats, bool AddToPool = true) : base(CreateMaterials, SetStats, AddToPool) {}\
	data\
};

BEGIN_PROTOTYPING(item)

class ABSTRACT_ITEM
(
	meleeweapon,
	item,
public:
	virtual void ReceiveHitEffect(character*, character*);
	virtual void DipInto(item*);
	virtual bool CanBeDippedInto(item*) const RET(Material[2] ? false : true)
	virtual bool CanBeDipped(void) const RET(true)
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
	virtual ushort Possibility(void) const RET(50)
	virtual std::string Name(uchar Case) const RET(NameSized(Case,"a", 15, 40))
	virtual uchar GetConsumeType(void) const RET(Material[1]->GetConsumeType())
	virtual vector GetInHandsPic(void) const RET(vector(160, 112))
	virtual bool Consume(character*, float = 100);
	virtual std::string NameSingular(void) const RET("banana")
	virtual std::string NamePlural(void) const RET("bananas")
	virtual float OfferModifier(void) const RET(1)
	virtual vector GetBitmapPos(void) const RETV(0,112)
protected:
	virtual ushort GetFormModifier(void) const RET(25)
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
	virtual ushort Possibility(void) const RET(1)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, bananapeal::StaticType()))
	virtual std::string NameSingular(void) const RET("holy banana of Liukas Vipro")
	virtual std::string NamePlural(void) const RET("holy bananas of Liukas Vipro")
	virtual float OfferModifier(void) const RET(40)
	virtual long Score(void) const RET(250)
	virtual vector GetBitmapPos(void) const RETV(0,112)
protected:
	virtual ushort GetFormModifier(void) const RET(35)
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
	virtual ushort Possibility(void) const RET(10)
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual ushort GetEmitation(void) const RET(300)
	virtual std::string NameSingular(void) const RET("lamp")
	virtual std::string NamePlural(void) const RET("lamps")
	virtual vector GetInHandsPic(void) const RET(vector(160, 128))
	virtual float OfferModifier(void) const RET(1)
	virtual vector GetBitmapPos(void) const RETV(0,192)
protected:
	virtual ushort GetFormModifier(void) const RET(30)
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
	virtual ushort Possibility(void) const RET(100)
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual ushort TryToOpen(stack*);
	virtual uchar GetConsumeType(void) const RET(Material[1] ? Material[1]->GetConsumeType() : ODD)
	virtual std::string NameSingular(void) const RET("can")
	virtual std::string NamePlural(void) const RET("cans")
	virtual vector GetInHandsPic(void) const RET(vector(160, 144))
	virtual float OfferModifier(void) const RET(0.5)
	virtual ushort PrepareForConsuming(character*, stack*);
	virtual vector GetBitmapPos(void) const RETV(144,288)
protected:
	virtual ushort GetFormModifier(void) const RET(20)
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
	virtual ushort Possibility(void) const RET(0)
	virtual std::string Name(uchar Case) const RET(NameThingsThatAreLikeLumps(Case, "a")) 
	virtual uchar GetConsumeType(void) const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual void ReceiveHitEffect(character*, character*);
	virtual bool CanBeDippedInto(item*) const RET(true)
	virtual material* BeDippedInto(void);
	virtual std::string NameSingular(void) const RET("lump")
	virtual std::string NamePlural(void) const RET("lumps")
	virtual vector GetInHandsPic(void) const RET(vector(160, 112))
	virtual float OfferModifier(void) const RET(0.5)
	virtual vector GetBitmapPos(void) const RETV(144,48)
	virtual bool CanBeWished(void) const RET(false)
protected:
	virtual ushort GetFormModifier(void) const RET(10)
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
	virtual ushort Possibility(void) const RET(0)
	virtual vector GetInHandsPic(void) const RET(vector(160,32))
	virtual std::string NameSingular(void) const RET("sword")
	virtual std::string NamePlural(void) const RET("swords")
	virtual float OfferModifier(void) const RET(0.5)
	virtual vector GetBitmapPos(void) const RETV(0,0)
protected:
	virtual ushort GetFormModifier(void) const RET(100)
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
	virtual ushort Possibility(void) const RET(3)
	virtual std::string NameSingular(void) const RET("two-handed sword")
	virtual std::string NamePlural(void) const RET("two-handed swords")
	virtual vector GetBitmapPos(void) const RETV(0,0)
	virtual float OfferModifier(void) const RET(0.25)
protected:
	virtual ushort GetFormModifier(void) const RET(125)
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
	virtual ushort Possibility(void) const RET(1)
	virtual std::string NameSingular(void) const RET("curved two-handed sword")
	virtual std::string NamePlural(void) const RET("curved two-handed swords")
	virtual float OfferModifier(void) const RET(0.25)
	virtual vector GetBitmapPos(void) const RETV(0,16)
protected:
	virtual ushort GetFormModifier(void) const RET(150)
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
	virtual ushort Possibility(void) const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, valpurium::StaticType()))
	virtual std::string NameSingular(void) const RET("holy broadsword named Valpuri's Justifier")
	virtual std::string NamePlural(void) const RET("holy broadswords named Valpuri's Justifier")
	virtual float OfferModifier(void) const RET(0.5)
	virtual long Score(void) const RET(1000)
	virtual vector GetBitmapPos(void) const RETV(0,64)
	virtual bool CanBeWished(void) const RET(false)
protected:
	virtual ushort GetFormModifier(void) const RET(400)
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
	virtual ushort Possibility(void) const RET(25)
	virtual vector GetInHandsPic(void) const RET(vector(160,16))
	virtual std::string NameSingular(void) const RET("axe")
	virtual std::string NamePlural(void) const RET("axes")
	virtual float OfferModifier(void) const RET(0.25)
	virtual vector GetBitmapPos(void) const RETV(144,256)
protected:
	virtual ushort GetFormModifier(void) const RET(150)
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
	virtual ushort Possibility(void) const RET(10)
	virtual std::string NameSingular(void) const RET("pick-axe")
	virtual std::string NamePlural(void) const RET("pick-axes")
	virtual vector GetInHandsPic(void) const RET(vector(160, 64))
	virtual float OfferModifier(void) const RET(0.25)
	virtual vector GetBitmapPos(void) const RETV(0,96)
	virtual bool Apply(character*);
protected:
	virtual ushort GetFormModifier(void) const RET(150)
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
	virtual ushort Possibility(void) const RET(25)
	virtual vector GetInHandsPic(void) const RET(vector(160,96))
	virtual bool CanBeDippedInto(item*) const RET(Material[2] ? false : true)
	virtual std::string NameSingular(void) const RET("spear")
	virtual std::string NamePlural(void) const RET("spears")
	virtual float OfferModifier(void) const RET(1)
	virtual vector GetBitmapPos(void) const RETV(144,144)
protected:
	virtual ushort GetFormModifier(void) const RET(200)
);

class ABSTRACT_ITEM
(
	torsoarmor,
	item,
public:
	virtual bool CanBeWorn(void) const RET(true)
protected:
	virtual ushort GetFormModifier(void) const RET(15)
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
	virtual ushort Possibility(void) const RET(3)
	virtual ushort GetArmorValue(void) const;
	virtual std::string NameSingular(void) const RET("plate mail")
	virtual std::string NamePlural(void) const RET("plate mails")
	virtual float OfferModifier(void) const RET(0.5)
	virtual vector GetBitmapPos(void) const RETV(144,128)
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
	virtual ushort Possibility(void) const RET(10)
	virtual ushort GetArmorValue(void) const;
	virtual std::string NameSingular(void) const RET("chain mail")
	virtual std::string NamePlural(void) const RET("chain mails")
	virtual float OfferModifier(void) const RET(0.5)
	virtual vector GetBitmapPos(void) const RETV(144,96)
);

class ABSTRACT_ITEM
(
	shirt,
	torsoarmor,
private:
	void Temporary(void) {} //...
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
	virtual ushort Possibility(void) const RET(0)
	virtual ushort GetArmorValue(void) const RET(10)
	virtual std::string NameSingular(void) const RET("Maakotka shirt")
	virtual std::string NamePlural(void) const RET("Maakotka shirts")
	virtual short CalculateOfferValue(char) const RET(750)
	virtual long Score(void) const RET(1000)
	virtual bool IsMaakotkaShirt(void) const RET(true);
	virtual vector GetBitmapPos(void) const RETV(144,112)
	virtual bool CanBeWished(void) const RET(false)
	virtual bool Destroyable(void) const { return false; }
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
	virtual ushort Possibility(void) const RET(0)
	virtual uchar GetConsumeType(void) const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual std::string NameSingular(void) const RET("corpse")
	virtual std::string NamePlural(void) const RET("corpses")
	virtual float OfferModifier(void) const RET(0.01f)
	virtual vector GetBitmapPos(void) const RETV(144,192)
	virtual bool CanBeWished(void) const RET(false)
protected:
	virtual ushort GetFormModifier(void) const RET(20)
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
	virtual ushort Possibility(void) const RET(25)
	virtual uchar GetConsumeType(void) const RET(Material[1] ? Material[1]->GetConsumeType() : ODD)
	virtual bool Consume(character*, float = 100);
	virtual std::string Name(uchar Case) const RET(NameContainer(Case))
	virtual std::string NameSingular(void) const RET("bottle")
	virtual std::string NamePlural(void) const RET("bottles")
	virtual void ImpactDamage(ushort, bool, stack*);
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual float OfferModifier(void) const RET(0.1f)
	virtual vector GetBitmapPos(void) const RETV(0,144)
protected:
	virtual ushort GetFormModifier(void) const RET(40)
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
	virtual ushort Possibility(void) const RET(25)
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", bananapeal::StaticType()))
	virtual std::string NameSingular(void) const RET("banana peal")
	virtual std::string NamePlural(void) const RET("banana peals")
	virtual item* BetterVersion(void) const { return new banana; }
	virtual float OfferModifier(void) const RET(0)
	virtual long Score(void) const RET(-1)
	virtual vector GetBitmapPos(void) const RETV(0,128)
protected:
	virtual ushort GetFormModifier(void) const RET(20)
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
	virtual ushort Possibility(void) const RET(25)
	virtual std::string NameSingular(void) const RET("broken bottle")
	virtual std::string NamePlural(void) const RET("broken bottles")
	virtual item* BetterVersion(void) const { item* P = new potion(false); P->InitMaterials(2, new glass(50), new omleurine(1500)); return P; }
	virtual float OfferModifier(void) const RET(0)
	virtual vector GetBitmapPos(void) const RETV(144,160)
protected:
	virtual ushort GetFormModifier(void) const RET(60)
);

class ABSTRACT_ITEM
(
	scroll,
	item,
public:
	virtual bool CanBeRead(character*) const;
protected:
	virtual ushort GetFormModifier(void) const RET(30)
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
	virtual ushort Possibility(void) const RET(25)
	virtual std::string NameSingular(void) const RET("scroll of create monster")
	virtual std::string NamePlural(void) const RET("scrolls of create monster")
	virtual float OfferModifier(void) const RET(5)
	virtual vector GetBitmapPos(void) const RETV(144,176)
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
	virtual ushort Possibility(void) const RET(25)
	virtual std::string NameSingular(void) const RET("scroll of teleportation")
	virtual std::string NamePlural(void) const RET("scrolls of teleportation")
	virtual float OfferModifier(void) const RET(5)
	virtual vector GetBitmapPos(void) const RETV(144,176)
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
	virtual ushort Possibility(void) const RET(0)
	virtual std::string NameSingular(void) const RET("head")
	virtual std::string NamePlural(void) const RET("heads")
	virtual float OfferModifier(void) const RET(0.1f)
	virtual vector GetBitmapPos(void) const RETV(0,0)
	virtual bool CanBeWished(void) const RET(false)
protected:
	virtual ushort GetFormModifier(void) const RET(10)
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
	virtual ushort Possibility(void) const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, elpuriflesh::StaticType()))
	virtual bool IsHeadOfElpuri(void) const RET(true)
	virtual std::string NameSingular(void) const RET("head of Elpuri")
	virtual std::string NamePlural(void) const RET("heads of Elpuri")
	virtual long Score(void) const RET(500);
	virtual vector GetBitmapPos(void) const RETV(144,0)
	virtual bool CanBeWished(void) const RET(false)
	virtual bool Destroyable(void) const { return false; }
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
	virtual ushort Possibility(void) const RET(0)
	virtual std::string NameSingular(void) const RET("nut")
	virtual std::string NamePlural(void) const RET("nuts")
	virtual float OfferModifier(void) const RET(10)
	virtual vector GetBitmapPos(void) const RETV(0,0)
protected:
	virtual ushort GetFormModifier(void) const RET(10)
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
	virtual ushort Possibility(void) const RET(0)
	virtual bool IsPerttusNut(void) const RET(true)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, humanflesh::StaticType()))
	virtual std::string NameSingular(void) const RET("left nut of Perttu")
	virtual std::string NamePlural(void) const RET("left nuts of Perttu")		//???
	virtual long Score(void) const RET(2500)
	virtual vector GetBitmapPos(void) const RETV(144,208)
	virtual item* CreateWishedItem(void) const;
	virtual bool Destroyable(void) const { return false; }
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
	virtual ushort Possibility(void) const RET(50)
	virtual std::string NameSingular(void) const RET("bone")
	virtual std::string NamePlural(void) const RET("bones")
	virtual std::string Name(uchar Case) const RET(NameSized(Case,"a", 15, 40))
	virtual float OfferModifier(void) const RET(0.1f)
	virtual bool Consume(character*, float = 100);
	virtual bool DogWillCatchAndConsume(void) const RET(true);
	virtual uchar GetConsumeType(void) const RET(Material[0]->GetConsumeType());
	virtual vector GetBitmapPos(void) const RETV(144,240)
protected:
	virtual ushort GetFormModifier(void) const RET(50)
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
	virtual ushort Possibility(void) const RET(15)
	virtual std::string NameSingular(void) const RET("poleaxe")
	virtual std::string NamePlural(void) const RET("poleaxes")
	virtual float OfferModifier(void) const RET(0.25f)
	virtual vector GetBitmapPos(void) const RETV(0,80)
protected:
	virtual ushort GetFormModifier(void) const RET(100)
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
	virtual ushort Possibility(void) const RET(5)
	virtual std::string NameSingular(void) const RET("spiked mace")
	virtual std::string NamePlural(void) const RET("spiked maces")
	virtual vector GetInHandsPic(void) const RET(vector(160, 0))
	virtual float OfferModifier(void) const RET(0.125)
	virtual vector GetBitmapPos(void) const RETV(0,32)
protected:
	virtual ushort GetFormModifier(void) const RET(75)
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
	virtual ushort Possibility(void) const RET(0)
	virtual std::string Name(uchar Case) const RET(NameArtifact(Case, mithril::StaticType()))
	virtual std::string NameSingular(void) const RET("ancient mace named H'taed Foneer Cse-ulb")
	virtual std::string NamePlural(void) const RET("ancient maces named H'taed Foneer Cse-ulb")
	virtual vector GetInHandsPic(void) const RET(vector(160, 0))
	virtual float OfferModifier(void) const RET(0.25)
	virtual long Score(void) const RET(1000)
	virtual vector GetBitmapPos(void) const RETV(0,32)
	virtual bool CanBeWished(void) const RET(false)
protected:
	virtual ushort GetFormModifier(void) const RET(100)
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
	virtual ushort Possibility(void) const RET(200)
	virtual std::string Name(uchar Case) const RET(NameThingsThatAreLikeLumps(Case, "a")) 
	virtual std::string NameSingular(void) const RET("loaf")
	virtual std::string NamePlural(void) const RET("loaves")
	virtual uchar GetConsumeType(void) const RET(Material[0]->GetConsumeType())
	virtual bool Consume(character*, float = 100);
	virtual float OfferModifier(void) const RET(0.125)
	virtual vector GetBitmapPos(void) const RETV(0,272)
protected:
	virtual ushort GetFormModifier(void) const RET(15)
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
	virtual ushort Possibility(void) const RET(1)
	virtual std::string NameSingular(void) const RET("scroll of wishing")
	virtual std::string NamePlural(void) const RET("scrolls of wishing")
	virtual float OfferModifier(void) const RET(50)
	virtual vector GetBitmapPos(void) const RETV(144,176)
	virtual bool CanBeWished(void) const RET(false)
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
	virtual ushort Possibility(void) const RET(0)
	virtual std::string NameSingular(void) const RET("cheap copy of left nut of Perttu")
	virtual std::string NamePlural(void) const RET("cheap copies of left nut of Perttu")		//???
	virtual long Score(void) const RET(1)
	virtual vector GetBitmapPos(void) const RETV(144,208)
);

class ABSTRACT_ITEM
(
	wand,
	item,
public:
	virtual bool Apply(character*);
	virtual bool CanBeZapped(void) const RET(true)
	virtual uchar GetCharge(void) const { return Charge; } 
	virtual void SetCharge(uchar What) { Charge = What; } 
protected:
	virtual ushort GetFormModifier(void) const RET(40)
	uchar Charge;
);

class ITEM
(
	wandofpolymorph,
	wand,
	InitMaterials(new glass(100)),
	{
		SetSize(10);
		SetCharge(2);
	},
public:
	virtual ushort Possibility(void) const RET(3)
	virtual std::string NameSingular(void) const RET("wand of polymorph")
	virtual std::string NamePlural(void) const RET("wands of polymorph")
	virtual float OfferModifier(void) const RET(30)
	virtual vector GetBitmapPos(void) const RETV(0,288)
	virtual bool Zap(vector, uchar);
);

FINISH_PROTOTYPING(item)

#endif

