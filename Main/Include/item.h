#ifndef __ITEM_H__
#define __ITEM_H__

#include <stdlib.h>

#include <cmath>

#include "typedef.h"
#include "vector.h"

#include "game.h"
#include "object.h"

class bitmap;
class character;
class material;
class object;
class vector;
class stack;

/* Presentation of the item class */

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	class proto_item
	{
	public:
		proto_item(bool) {}
	};

#endif

class item : public object
{
public:
	item(ushort, bool);
	item(std::ifstream* SaveFile, ushort MaterialQuantity = 1) : object(SaveFile) {}
	virtual float GetWeaponStrength(void);
	virtual void DrawToTileBuffer(void);
	virtual void PositionedDrawToTileBuffer(uchar);
	virtual std::string Name(uchar Case) {return NameWithMaterial(Case);}
	virtual ushort CEmitation(void);
	virtual vector GetInHandsPic(void) {return vector(0,0);}
	virtual uchar GetConsumeType(void) {return ODD;}
	virtual ushort TryToOpen(stack*) {return 0xFFFF;}
	virtual ulong CWeight(void);
	virtual bool Consume(character*, float = 100) { return false; };
	virtual ushort GetArmorValue(void) {return 100;}
	virtual bool IsHeadOfElpuri(void) {return false;}
	virtual bool IsPerttusNut(void) {return false;}
	virtual bool IsMaakotkaShirt(void) {return false;}
	virtual bool CanBeRead(character*);
	virtual bool Read(character*);
	virtual void ReceiveHitEffect(character*, character*) {}
	virtual bool CanBeDippedInto(item*) {return false;}
	virtual void DipInto(item*) {}
	virtual material* BeDippedInto(void) {return 0;}
	virtual bool CanBeDipped(void) {return false;}
	virtual void Save(std::ofstream* SaveFile) {object::Save(SaveFile);}
	virtual bool CanBeWorn(void) {return false;}
	virtual void SMaterial(ushort Where, material* What) { Material[Where] = What; }
	virtual bool Consumable(character*);
	virtual item* BetterVersion(void) { return 0; }
	virtual void ImpactDamage(ushort, bool, stack*) {};
	virtual short CalculateOfferValue(char);
	virtual float OfferModifier(void) { return 0; }
	virtual long Score(void) { return 0; }
	virtual bool Destroyable(void);
	virtual bool Fly(uchar, ushort, stack*, bool = true);
	virtual bool HitCharacter(character*, float, bool);
	virtual bool DogWillCatchAndConsume(void) { return false; }
	virtual uchar GetDipMaterialNumber(void) { return CMaterials() - 1; }
	virtual ushort PrepareForConsuming(character*, stack*); // Stack where the item IS
	virtual item* Clone(void) const = 0;
	virtual item* Load(std::ifstream*) const = 0;
	virtual ushort Possibility(void) const = 0;
	virtual bool CanBeWished(void) { return true; }
	virtual item* CreateWishedItem(void) { return 0; }
	virtual bool Apply(character*) { ADD_MESSAGE("You can't apply this!"); return false; }
protected:
	virtual ushort CFormModifier(void) {return 0;}
};

#undef RET
#define RET(Val) { return Val; }
#undef RETV
#define RETV(XVal,YVal) { return vector(XVal, YVal); }

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define ITEM(name, base, imaterials, dparameters, constructor, load, type, possibility, data)\
	\
	class name : public base\
	{\
	public:\
		name(ushort Size, bool CreateMaterials) : base(Size, CreateMaterials) constructor\
		name(bool CreateMaterials = true) : base dparameters constructor\
		name(std::ifstream* SaveFile, ushort MaterialQuantity = imaterials) : base(SaveFile, MaterialQuantity) load\
		virtual item* Clone(void) const {return new name;}\
		virtual item* Load(std::ifstream* SaveFile) const {return new name(SaveFile, 0);}\
		virtual ushort Possibility(void) const { return possibility; }\
	protected:\
		virtual ushort Type(void) { return type; }\
		data\
	};\
	\
	class proto_##name : public proto_##base\
	{\
	public:\
		proto_##name(bool Add = true) : proto_##base(false) { if(Add) game::CItemPrototype().Add(type, new name); }\
	} static Proto_##name;

	#define ABSTRACT_ITEM(name, base, imaterials, constructor, load, data)\
	class name : public base\
	{\
	public:\
		name(ushort Size, bool CreateMaterials) : base(Size, CreateMaterials) {}\
		name(std::ifstream* SaveFile, ushort MaterialQuantity = imaterials) : base(SaveFile, MaterialQuantity) {}\
	protected:\
		data\
	};\
	\
	class proto_##name : public proto_##base\
	{\
	public:\
		proto_##name(bool Add) : proto_##base(Add) {}\
	};

#else

	#define ITEM(name, base, imaterials, dparameters, constructor, load, type, possibility, data)\
	\
	class name : public base\
	{\
	public:\
		name(ushort Size, bool CreateMaterials) : base(Size, CreateMaterials) constructor\
		name(bool CreateMaterials = true) : base dparameters constructor\
		name(std::ifstream* SaveFile, ushort MaterialQuantity = imaterials) : base(SaveFile, MaterialQuantity) load\
		virtual item* Clone(void) const {return new name;}\
		virtual item* Load(std::ifstream* SaveFile) const {return new name(SaveFile, 0);}\
		virtual ushort Possibility(void) const { return possibility; }\
	protected:\
		virtual ushort Type(void) { return type; }\
		data\
	};

	#define ABSTRACT_ITEM(name, base, imaterials, constructor, load, data)\
	class name : public base\
	{\
	public:\
		name(ushort Size, bool CreateMaterials) : base(Size, CreateMaterials) {}\
		name(std::ifstream* SaveFile, ushort MaterialQuantity = imaterials) : base(SaveFile, MaterialQuantity) {}\
	protected:\
		data\
	};

#endif

#define GET_CONSUME_TYPE public: virtual uchar GetConsumeType(void)
#define CONSUME public: virtual bool Consume(character*, float = 100)
#define C_FORM_MODIFIER protected: virtual ushort CFormModifier(void)
#define C_EMITATION public: virtual ushort CEmitation(void)
#define POSITIONED_DRAW_TO_TILE_BUFFER public: virtual void PositionedDrawToTileBuffer(uchar)
#define TRY_TO_OPEN public: virtual ushort TryToOpen(stack*)
#define RECEIVE_HIT_EFFECT public: virtual void ReceiveHitEffect(character*, character*)
#define CAN_BE_DIPPED_INTO public: virtual bool CanBeDippedInto(item*)
#define BE_DIPPED_INTO public: virtual material* BeDippedInto(void)
#define CAN_BE_READ public: virtual bool CanBeRead(character*)
#define READ public: virtual bool Read(character*)
#define IS_HEAD_OF_ELPURI public: virtual bool IsHeadOfElpuri(void)
#define IS_PERTTUS_NUT public: virtual bool IsPerttusNut(void)
#define IS_MAAKOTKA_SHIRT public: virtual bool IsMaakotkaShirt(void)
#define GET_IN_HANDS_PIC public: virtual vector GetInHandsPic(void)
#define DIP_INTO public: virtual void DipInto(item*)
#define CAN_BE_WORN public: virtual bool CanBeWorn(void)
#define GET_ARMOR_VALUE public: virtual ushort GetArmorValue(void)
#define BETTER_VERSION public: virtual item* BetterVersion(void)
#define IMPACT_DAMAGE public: virtual void ImpactDamage(ushort, bool, stack*)
#define OFFER_MODIFIER public: virtual float OfferModifier(void) 
#define CALCULATE_OFFER_VALUE public: virtual short CalculateOfferValue(char)
#define SCORE public: virtual long Score(void)
#define DOG_WILL_CATCH_AND_CONSUME public: virtual bool DogWillCatchAndConsume(void)
#define CAN_BE_DIPPED public: virtual bool CanBeDipped(void)
#define PREPARE_FOR_CONSUMING public: virtual ushort PrepareForConsuming(character*, stack*)
#define CAN_BE_WISHED public: virtual bool CanBeWished(void)
#define CREATE_WISHED_ITEM public: virtual item* CreateWishedItem(void)
#undef APPLY
#define APPLY public: virtual bool Apply(character*)

ABSTRACT_ITEM(
	meleeweapon,
	item,
	3,
	{},
	{},
	RECEIVE_HIT_EFFECT;
	DIP_INTO;
	CAN_BE_DIPPED_INTO RET(Material[2] ? false : true)
	CAN_BE_DIPPED RET(true)
);

ITEM(
	banana,
	item,
	2,
	(
	 20,
	 false
	),
	{ if(CreateMaterials) InitMaterials(2, new bananapeal(30), new bananaflesh(150)); },
	{},
	1,
	50,
	NAME RET(NameSized(Case,"a", 15, 40))
	GET_CONSUME_TYPE RET(Material[1]->CConsumeType())
	GET_IN_HANDS_PIC RET(vector(160, 112))
	CONSUME;
	NAME_SINGULAR RET("banana")
	NAME_PLURAL RET("bananas")
	C_FORM_MODIFIER RET(25)
	OFFER_MODIFIER RET(1)
	C_BITMAP_POS RETV(0,112)
);

ITEM(
	holybananaofliukasvipro,
	banana,
	2,
	(
	 30,
	 false
	),
	{ if(CreateMaterials) InitMaterials(2, new bananapeal(40), new bananaflesh(300)); },
	{},
	2,
	1,
	NAME RET(NameArtifact(Case, IBANANAPEAL))
	NAME_SINGULAR RET("holy banana of Liukas Vipro")
	NAME_PLURAL RET("holy bananas of Liukas Vipro")
	C_FORM_MODIFIER RET(35)
	OFFER_MODIFIER RET(40)
	SCORE RET(250)
	C_BITMAP_POS RETV(0,112)
);

ITEM(
	lamp,
	item,
	1,
	(
	 30,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new glass(850)); },
	{},
	3,
	10,
	POSITIONED_DRAW_TO_TILE_BUFFER;
	C_EMITATION RET(300)
	NAME_SINGULAR RET("lamp")
	NAME_PLURAL RET("lamps")
	C_FORM_MODIFIER RET(30)
	GET_IN_HANDS_PIC RET(vector(160, 128))
	OFFER_MODIFIER RET(1)
	C_BITMAP_POS RETV(0,192)
);

ITEM(
	can,
	item,
	2,
	(
	 10,
	 false
	),
	{ if(CreateMaterials) InitMaterials(2, new iron(50), rand() % 2 ? (material*)(new bananaflesh(600)) : (material*)(new schoolfood(600))); },
	{},
	4,
	100,
	POSITIONED_DRAW_TO_TILE_BUFFER;
	NAME RET(NameContainer(Case))
	TRY_TO_OPEN;
	GET_CONSUME_TYPE RET(Material[1] ? Material[1]->CConsumeType() : ODD)
	NAME_SINGULAR RET("can")
	NAME_PLURAL RET("cans")
	C_FORM_MODIFIER RET(20)
	GET_IN_HANDS_PIC RET(vector(160, 144))
	OFFER_MODIFIER RET(0.5)
	PREPARE_FOR_CONSUMING;
	C_BITMAP_POS RETV(144,288)
);

ITEM(
	lump,
	item,
	1,
	(
	 10,
	 false
	),
	{ if(CreateMaterials) InitMaterials(rand() % 2 ? (material*)(new bananaflesh(600)) : (material*)(new schoolfood(600))); },
	{},
	5,
	0,
	NAME RET(NameThingsThatAreLikeLumps(Case, "a")) 
	GET_CONSUME_TYPE RET(Material[0]->CConsumeType())
	CONSUME;
	RECEIVE_HIT_EFFECT;
	CAN_BE_DIPPED_INTO RET(true)
	BE_DIPPED_INTO;
	NAME_SINGULAR RET("lump")
	NAME_PLURAL RET("lumps")
	C_FORM_MODIFIER RET(10)
	GET_IN_HANDS_PIC RET(vector(160, 112))
	OFFER_MODIFIER RET(0.5)
	C_BITMAP_POS RETV(144,48)
	CAN_BE_WISHED RET(false)
);

ITEM(
	sword,
	meleeweapon,
	3,
	(
	 150,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new iron(2500), new iron(100), 0); },
	{},
	6,
	0,
	GET_IN_HANDS_PIC RET(vector(160,32))
	NAME_SINGULAR RET("sword")
	NAME_PLURAL RET("swords")
	C_FORM_MODIFIER RET(100)
	OFFER_MODIFIER RET(0.5)
	C_BITMAP_POS RETV(0,0)
);

ITEM(
	twohandedsword,
	sword,
	3,
	(
	 175,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new iron(5500), new iron(250), 0); },
	{},
	7,
	3,
	NAME_SINGULAR RET("two-handed sword")
	NAME_PLURAL RET("two-handed swords")
	C_FORM_MODIFIER RET(125)
	OFFER_MODIFIER RET(0.25)
	C_BITMAP_POS RETV(0,0)
);

ITEM(
	curvedtwohandedsword,
	twohandedsword,
	3,
	(
	 175,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new iron(5500), new iron(250), 0); },
	{},
	8,
	1,
	NAME_SINGULAR RET("curved two-handed sword")
	NAME_PLURAL RET("curved two-handed swords")
	C_FORM_MODIFIER RET(150)
	OFFER_MODIFIER RET(0.25)
	C_BITMAP_POS RETV(0,16)
);

ITEM(
	valpurijustifier,
	sword,
	3,
	(
	 200,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new valpurium(6500), new valpurium(300), 0); },
	{},
	9,
	0,
	NAME RET(NameArtifact(Case, IVALPURIUM))
	NAME_SINGULAR RET("holy broadsword named Valpuri's Justifier")
	NAME_PLURAL RET("holy broadswords named Valpuri's Justifier")
	C_FORM_MODIFIER RET(400)
	OFFER_MODIFIER RET(0.5)
	SCORE RET(1000)
	C_BITMAP_POS RETV(0,64)
	CAN_BE_WISHED RET(false)
);

ITEM(
	axe,
	meleeweapon,
	3,
	(
	 125,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new iron(450), new iron(900), 0); },
	{},
	10,
	25,
	GET_IN_HANDS_PIC RET(vector(160,16))
	NAME_SINGULAR RET("axe")
	NAME_PLURAL RET("axes")
	C_FORM_MODIFIER RET(150)
	OFFER_MODIFIER RET(0.25)
	C_BITMAP_POS RETV(144,256)
);

ITEM(
	pickaxe,
	axe,
	3,
	(
	 150,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new iron(1000), new wood(1050), 0); },
	{},
	11,
	10,
	NAME_SINGULAR RET("pick-axe")
	NAME_PLURAL RET("pick-axes")
	C_FORM_MODIFIER RET(150)
	GET_IN_HANDS_PIC RET(vector(160, 64))
	OFFER_MODIFIER RET(0.25)
	C_BITMAP_POS RETV(0,96)
	APPLY;
);

ITEM(
	spear,
	meleeweapon,
	3,
	(
	 200,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new iron(150), new wood(1500), 0); },
	{},
	12,
	25,
	GET_IN_HANDS_PIC RET(vector(160,96))
	CAN_BE_DIPPED_INTO RET(Material[2] ? false : true)
	NAME_SINGULAR RET("spear")
	NAME_PLURAL RET("spears")
	C_FORM_MODIFIER RET(200)
	OFFER_MODIFIER RET(1)
	C_BITMAP_POS RETV(144,144)
);

ABSTRACT_ITEM(
	torsoarmor,
	item,
	1,
	{},
	{},
	CAN_BE_WORN RET(true)
);

ITEM(
	platemail,
	torsoarmor,
	1,
	(
	 75,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new iron(4000)); },
	{},
	13,
	3,
	GET_ARMOR_VALUE { float Base = 80 - sqrt(Material[0]->GetHitValue()) * 3; if(Base < 0) Base = 0; if(Base > 100) Base = 100; return ushort(Base); }
	C_FORM_MODIFIER RET(15)
	NAME_SINGULAR RET("plate mail")
	NAME_PLURAL RET("plate mails")
	OFFER_MODIFIER RET(0.5)
	C_BITMAP_POS RETV(144,128)
);

ITEM(
	chainmail,
	torsoarmor,
	1,
	(
	 75,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new iron(2000)); },
	{},
	14,
	10,
	GET_ARMOR_VALUE { float Base = 90 - sqrt(Material[0]->GetHitValue()) * 2; if(Base < 0) Base = 0; if(Base > 100) Base = 100; return ushort(Base); }
	C_FORM_MODIFIER RET(15)
	NAME_SINGULAR RET("chain mail")
	NAME_PLURAL RET("chain mails")
	OFFER_MODIFIER RET(0.5)
	C_BITMAP_POS RETV(144,96)
);

ABSTRACT_ITEM(
	shirt,
	torsoarmor,
	1,
	{},
	{},
	private: void Temporary(void) {} //...
);

ITEM(
	maakotkashirt,
	shirt,
	1,
	(
	 60,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new cloth(1000)); },
	{},
	15,
	0,
	GET_ARMOR_VALUE RET(10)
	C_FORM_MODIFIER RET(15)
	NAME_SINGULAR RET("Maakotka shirt")
	NAME_PLURAL RET("Maakotka shirts")
	CALCULATE_OFFER_VALUE RET(750)
	SCORE RET(1000)
	IS_MAAKOTKA_SHIRT RET(true);
	C_BITMAP_POS RETV(144,112)
	CAN_BE_WISHED RET(false)
);

ITEM(
	corpse,
	item,
	1,
	(
	 0,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new humanflesh(60000)); },
	{},
	16,
	0,
	GET_CONSUME_TYPE RET(Material[0]->CConsumeType())
	CONSUME;
	NAME_SINGULAR RET("corpse")
	NAME_PLURAL RET("corpses")
	C_FORM_MODIFIER RET(20)
	OFFER_MODIFIER RET(0.01f)
	C_BITMAP_POS RETV(144,192)
	CAN_BE_WISHED RET(false)
);

ITEM(
	potion,
	item,
	2,
	(
	 30,
	 false
	),
	{ if(CreateMaterials) InitMaterials(2, new glass(50), rand() % 3 ? 0 : new omleurine(1500)); },
	{},
	17,
	25,
	GET_CONSUME_TYPE RET(Material[1] ? Material[1]->CConsumeType() : ODD)
	CONSUME;
	NAME RET(NameContainer(Case))
	NAME_SINGULAR RET("bottle")
	NAME_PLURAL RET("bottles")
	C_FORM_MODIFIER RET(40)
	IMPACT_DAMAGE;
	POSITIONED_DRAW_TO_TILE_BUFFER;
	OFFER_MODIFIER RET(0.1f)
	C_BITMAP_POS RETV(0,144)
);

ITEM(
	bananapeals,
	item,
	1,
	(
	 20,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new bananapeal(15)); },
	{},
	18,
	25,
	NAME RET(NameHandleDefaultMaterial(Case, "a", IBANANAPEAL))
	NAME_SINGULAR RET("banana peal")
	NAME_PLURAL RET("banana peals")
	C_FORM_MODIFIER RET(20)
	BETTER_VERSION { return new banana; }
	OFFER_MODIFIER RET(0)
	SCORE RET(-1)
	C_BITMAP_POS RETV(0,128)
);

ITEM(
	brokenbottle,
	item,
	1,
	(
	 10,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new glass(50)); },
	{},
	19,
	25,
	NAME_SINGULAR RET("broken bottle")
	NAME_PLURAL RET("broken bottles")
	C_FORM_MODIFIER RET(60)
	BETTER_VERSION { item* P = new potion(30, false); P->InitMaterials(2, new glass(50), new omleurine(1500)); return P; }
	OFFER_MODIFIER RET(0)
	C_BITMAP_POS RETV(144,160)
);

ABSTRACT_ITEM(
	scroll,
	item,
	1,
	{},
	{},
	CAN_BE_READ;
	C_FORM_MODIFIER RET(30)
);

ITEM(
	scrollofcreatemonster,
	scroll,
	1,
	(
	 30,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new parchment(200)); },
	{},
	20,
	25,
	READ;
	NAME_SINGULAR RET("scroll of create monster")
	NAME_PLURAL RET("scrolls of create monster")
	OFFER_MODIFIER RET(5)
	C_BITMAP_POS RETV(144,176)
);

ITEM(
	scrollofteleport,
	scroll,
	1,
	(
	 30,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new parchment(200)); },
	{},
	21,
	25,
	READ;
	NAME_SINGULAR RET("scroll of teleportation")
	NAME_PLURAL RET("scrolls of teleportation")
	OFFER_MODIFIER RET(5)
	C_BITMAP_POS RETV(144,176)
);

ITEM(
	head,
	item,
	1,
	(
	 0,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new humanflesh(2000)); },
	{},
	22,
	0,
	NAME_SINGULAR RET("head")
	NAME_PLURAL RET("heads")
	C_FORM_MODIFIER RET(10)
	OFFER_MODIFIER RET(0.1f)
	C_BITMAP_POS RETV(0,0)
	CAN_BE_WISHED RET(false)
);

ITEM(
	headofelpuri,
	head,
	1,
	(
	 60,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new elpuriflesh(25000)); },
	{},
	23,
	0,
	NAME RET(NameArtifact(Case, IELPURIFLESH))
	IS_HEAD_OF_ELPURI RET(true)
	NAME_SINGULAR RET("head of Elpuri")
	NAME_PLURAL RET("heads of Elpuri")
	SCORE RET(500);
	C_BITMAP_POS RETV(144,0)
	CAN_BE_WISHED RET(false)
);

ITEM(
	nut,
	item,
	1,
	(
	 0,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new humanflesh(15)); },
	{},
	24,
	0,
	C_FORM_MODIFIER RET(10)
	NAME_SINGULAR RET("nut")
	NAME_PLURAL RET("nuts")
	OFFER_MODIFIER RET(10)
	C_BITMAP_POS RETV(0,0)
	CAN_BE_WISHED RET(false)
);

ITEM(
	leftnutofperttu,
	nut,
	1,
	(
	 10,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new humanflesh(150)); },
	{},
	25,
	0,
	IS_PERTTUS_NUT RET(true)
	NAME RET(NameArtifact(Case, IHUMANFLESH))
	NAME_SINGULAR RET("left nut of Perttu")
	NAME_PLURAL RET("left nuts of Perttu")		//???
	SCORE RET(2500)
	C_BITMAP_POS RETV(144,208)
	CREATE_WISHED_ITEM;
);

ITEM(
	abone,
	item,
	1,
	(
	 50,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new bone(2000)); },
	{},
	26,
	50,
	NAME_SINGULAR RET("bone")
	NAME_PLURAL RET("bones")
	C_FORM_MODIFIER RET(50)
	NAME RET(NameSized(Case,"a", 15, 40))
	OFFER_MODIFIER RET(0.1f)
	CONSUME;
	DOG_WILL_CATCH_AND_CONSUME RET(true);
	GET_CONSUME_TYPE RET(Material[0]->CConsumeType());
	C_BITMAP_POS RETV(144,240)
);

ITEM(
	poleaxe,
	axe,
	3,
	(
	 225,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new iron(1500), new wood(3000), 0); },
	{},
	27,
	15,
	C_FORM_MODIFIER RET(100)
	NAME_SINGULAR RET("poleaxe")
	NAME_PLURAL RET("poleaxes")
	OFFER_MODIFIER RET(0.25f)
	C_BITMAP_POS RETV(0,80)
);

ITEM(
	spikedmace,
	meleeweapon,
	3,
	(
	 150,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new iron(5000), new wood(3000), 0); },
	{},
	28,
	5,
	C_FORM_MODIFIER RET(75)
	NAME_SINGULAR RET("spiked mace")
	NAME_PLURAL RET("spiked maces")
	GET_IN_HANDS_PIC RET(vector(160, 0))
	OFFER_MODIFIER RET(0.125)
	C_BITMAP_POS RETV(0,32)
);

ITEM(
	htaedfoneercseulb,
	spikedmace,
	3,
	(
	 200,
	 false
	),
	{ if(CreateMaterials) InitMaterials(3, new mithril(15000), new iron(8000), new darkfrogflesh(1000)); },
	{},
	29,
	0,
	NAME RET(NameArtifact(Case, IMITHRIL))
	C_FORM_MODIFIER RET(100)
	NAME_SINGULAR RET("ancient mace named H'taed Foneer Cse-ulb")
	NAME_PLURAL RET("ancient maces named H'taed Foneer Cse-ulb")
	GET_IN_HANDS_PIC RET(vector(160, 0))
	OFFER_MODIFIER RET(0.25)
	SCORE RET(1000)
	C_BITMAP_POS RETV(0,32)
	CAN_BE_WISHED RET(false) 
);

ITEM(
	loaf,
	item,
	1,
	(
	 40,
	 false
	),
	{ if(CreateMaterials) InitMaterials(1, rand() % 2 ? (material*)(new pork(2000)) : (material*)(new beef(2000))); },
	{},
	30,
	200,
	NAME RET(NameThingsThatAreLikeLumps(Case, "a")) 
	NAME_SINGULAR RET("loaf")
	NAME_PLURAL RET("loaves")
	GET_CONSUME_TYPE RET(Material[0]->CConsumeType())
	CONSUME;
	C_FORM_MODIFIER RET(15)
	OFFER_MODIFIER RET(0.125)
	C_BITMAP_POS RETV(0,272)
);

ITEM(
	scrollofwishing,
	scroll,
	1,
	(
	 30,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new parchment(200)); },
	{},
	31,
	1,
	READ;
	NAME_SINGULAR RET("scroll of wishing")
	NAME_PLURAL RET("scrolls of wishing")
	OFFER_MODIFIER RET(50)
	C_BITMAP_POS RETV(144,176)
	CAN_BE_WISHED RET(false)
);

ITEM(
	cheapcopyofleftnutofperttu,
	nut,
	1,
	(
	 10,
	 false
	),
	{ if(CreateMaterials) InitMaterials(new glass(150)); },
	{},
	32,
	0,
	NAME_SINGULAR RET("cheap copy of left nut of Perttu")
	NAME_PLURAL RET("cheap copies of left nut of Perttu")		//???
	SCORE RET(1)
	C_BITMAP_POS RETV(144,208)
);
#endif
