#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#define GOOD 0
#define NEUTRAL 1
#define EVIL 2

#define BLACK 0
#define DGRAY 1
#define LGRAY 2
#define WHITE 3
#define YELLOW 4
#define BROWN 5
#define RED 6
#define BLUE 7
#define GREEN 8
#define FLESH 8

/* ConsumeTypes */

#define ODD 		0
#define FRUIT 		1
#define MEAT 		2
#define SPOILED 	3
#define HARD 		4
#define SCHOOLFOOD 	5
#define CONTAINER	6
#define LIQUID		7
#define BONE		8

#define EFOOD			1
#define ESCHOOLFOOD		2
#define EOMLEURINE		3
#define EDARKNESS		4
#define EFIRE			5
#define EPEPSI			6

#include "typedef.h"

#include "independ.h"

/* Presentation of material class */

class character;
class item;
class object;

class material 
{
public: 
	virtual ~material(void)					{}
	virtual uchar CFleshColor(void) const			{ return CItemColor(); }
	virtual uchar CItemColor(void) const = 0;
	virtual std::string Name(uchar Case = 0) const = 0;
	virtual ushort GetHitValue(void) const = 0;
	virtual uchar CConsumeType(void) const			{return ODD;}
	virtual ulong CVolume(void) const			{return Volume;}
	virtual ulong CWeight(void) const			{return ulong(float(Volume) * GetDensity() / 1000);}
	virtual ushort GetDensity(void) const			{return 1;}
	virtual uchar EffectType(void)				{return EFOOD;}
	virtual ushort TakeDipVolumeAway(void);
	virtual void Save(std::ofstream*) const;
	virtual void Load(std::ifstream*);
	virtual ushort CArmorValue(void) const { return GetHitValue(); }
	virtual void SVolume(ulong What) { Volume = What; }
	virtual ushort CEmitation(void) const { return 0; }
	virtual ushort OfferValue(void) const = 0;
	virtual uchar Alignment(void) const				{ return NEUTRAL; }
	virtual void EatEffect(character*, float = 100, float = 1.0f)	{ }
	virtual void HitEffect(character*)			{ }
	virtual short NutritionValue(void) const			{ return 0; }
	virtual void MinusAmount(float Amount)			{ SVolume(CVolume() - ulong(CVolume() * Amount / 100)); }
	virtual material* Clone(ulong Volume) const = 0;
	virtual material* Clone(void) const = 0;
	virtual bool IsType(ushort QType) const { return Type() == QType; }
	virtual bool IsSolid(void) const { return false; }
protected:
	virtual void NormalFoodEffect(character*, float, float);
	virtual ushort Type(void) const = 0;
	ulong Volume;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define MATERIAL(name, base, data)\
	\
	class name : public base\
	{\
	public:\
		name(ulong IVolume) { Volume = IVolume; }\
		name(void) {}\
		virtual material* Clone(ulong Volume) const { return new name(Volume); }\
		virtual material* Clone(void) const { return new name; }\
		static ushort StaticType(void);\
	protected:\
		virtual ushort Type(void) const;\
		data\
	};\
	\
	class proto_##name\
	{\
	public:\
		proto_##name(void) : Index(game::AddProtoType(new name)) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static Proto_##name;\
	\
	ushort name::StaticType(void) { return Proto_##name.GetIndex(); }\
	ushort name::Type(void) const { return Proto_##name.GetIndex(); }

#else

	#define MATERIAL(name, base, data)\
	\
	class name : public base\
	{\
	public:\
		name(ulong IVolume) { Volume = IVolume; }\
		name(void) {}\
		virtual material* Clone(ulong Volume) const { return new name(Volume); }\
		virtual material* Clone(void) const { return new name; }\
		static ushort StaticType(void);\
	protected:\
		virtual ushort Type(void) const;\
		data\
	};

#endif

MATERIAL(
	iron,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "iron" : "an iron";}
	virtual ushort GetHitValue(void) const				{return 100;}
	virtual uchar CConsumeType(void) const				{return HARD;}
	virtual ushort GetDensity(void) const				{return 8000;}
	virtual ushort OfferValue(void) const				{ return 10; }
	virtual uchar CItemColor(void) const			{ return LGRAY; }
	virtual bool IsSolid(void) const { return true; }
);

MATERIAL(
	valpurium,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "valpurium" : "a valpurium";}
	virtual ushort GetHitValue(void) const				{return 400;}
	virtual uchar CConsumeType(void) const				{return HARD;}
	virtual ushort GetDensity(void) const				{return 3000;}
	virtual ushort OfferValue(void) const				{ return 100; }
	virtual uchar Alignment(void) const				{ return GOOD; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
);

MATERIAL(
	stone,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "stone" : "a stone";}
	virtual ushort GetHitValue(void) const				{return 50;}
	virtual uchar CConsumeType(void) const				{return HARD;}
	virtual ushort GetDensity(void) const				{return 3000;}
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar CItemColor(void) const			{ return DGRAY; }
	virtual bool IsSolid(void) const { return true; }
);

MATERIAL(
	bananaflesh,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "banana flesh" : "a banana flesh";}
	virtual ushort GetHitValue(void) const				{return 5;}
	virtual uchar CConsumeType(void) const				{return FRUIT;}
	virtual ushort GetDensity(void) const				{return 1200;}
	virtual uchar EffectType(void)				{return EFOOD;}
	virtual ushort OfferValue(void) const				{ return 10; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual short NutritionValue(void) const			{ return 175; }
	virtual uchar CItemColor(void) const			{ return YELLOW; }
);

MATERIAL(
	gravel,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "gravel" : "a gravel";}
	virtual ushort GetHitValue(void) const				{return 40;}
	virtual uchar CConsumeType(void) const				{return ODD;}
	virtual ushort GetDensity(void) const				{return 2500;}
	virtual ushort OfferValue(void) const				{ return 1; }
	virtual uchar CItemColor(void) const			{ return LGRAY; }
);

MATERIAL(
	moraine,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "moraine" : "a moraine";}
	virtual ushort GetHitValue(void) const				{return 25;}
	virtual uchar CConsumeType(void) const				{return ODD;}
	virtual ushort GetDensity(void) const				{return 2500;}
	virtual ushort OfferValue(void) const				{ return 2; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
);

MATERIAL(
	schoolfood,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "school food" : "a school food";}
	virtual ushort GetHitValue(void) const				{return 5;}
	virtual uchar CConsumeType(void) const				{return SCHOOLFOOD;}
	virtual ushort GetDensity(void) const				{return 1500;}
	virtual uchar EffectType(void)				{return ESCHOOLFOOD;}
	virtual ushort OfferValue(void) const				{ return 20; }
	virtual uchar Alignment(void) const				{ return EVIL; }
	virtual short NutritionValue(void) const			{ return 75; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual uchar CItemColor(void) const			{ return BROWN; }
);

MATERIAL(
	air,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "air" : "an air";}
	virtual ushort GetHitValue(void) const				{return 0;}
	virtual ushort GetDensity(void) const				{return 1;}
	virtual ushort OfferValue(void) const				{ return 0; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
);

MATERIAL(
	wood,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "wood" : "a wood";}
	virtual ushort GetHitValue(void) const				{return 20;}
	virtual uchar CConsumeType(void) const				{return HARD;}
	virtual ushort GetDensity(void) const				{return 500;}
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
	virtual bool IsSolid(void) const { return true; }
);

MATERIAL(
	flesh,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "flesh" : "a flesh";}
	virtual ushort GetHitValue(void) const 				{return 15;}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual ushort GetDensity(void) const				{return 1200;}
	virtual ushort OfferValue(void) const				{ return 15; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual short NutritionValue(void) const			{ return 50; }
	virtual uchar CFleshColor(void) const			{ return FLESH; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
);

MATERIAL(
	goblinoidflesh,
	flesh,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "green flesh" : "a green flesh";}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual ushort OfferValue(void) const				{ return 10; }
	virtual uchar CItemColor(void) const			{ return GREEN; }
);

MATERIAL(
	pork,
	flesh,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "pork" : "a pork";}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual ushort OfferValue(void) const				{ return 20; }
	virtual short NutritionValue(void) const			{ return 500; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
);

MATERIAL(
	beef,
	flesh,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "beef" : "a beef";}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual ushort OfferValue(void) const				{ return 20; }
	virtual short NutritionValue(void) const			{ return 500; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
);

MATERIAL(
	bone,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "bone" : "a bone";}
	virtual ushort GetHitValue(void) const 				{return 25;}
	virtual ushort GetDensity(void) const				{return 2000;}
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar CConsumeType(void) const				{ return BONE; }
	virtual void EatEffect(character*, float, float);
	virtual uchar CFleshColor(void) const			{ return FLESH; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
);

MATERIAL(
	darkfrogflesh,
	flesh,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "dark forg flesh" : "a dark frog flesh";}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual uchar EffectType(void) 				{return EDARKNESS;}
	virtual ushort OfferValue(void) const				{ return 50; }
	virtual uchar Alignment(void) const				{ return EVIL; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual uchar CItemColor(void) const			{ return BLACK; }
);

MATERIAL(
	elpuriflesh,
	darkfrogflesh,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "Elpuri's flesh" : "an Elpuri's flesh";}
	virtual ushort GetHitValue(void) const 				{return 30;}
	virtual ushort GetDensity(void) const				{return 2400;}
	virtual ushort OfferValue(void) const				{ return 1; }
);

MATERIAL(
	glass,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "glass" : "a glass";}
	virtual ushort GetHitValue(void) const 				{return 30;}
	virtual uchar CConsumeType(void) const				{return HARD;}
	virtual ushort GetDensity(void) const				{return 2500;}
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
);

MATERIAL(
	omleurine,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "Omle urine" : "an Omle urine";}
	virtual ushort GetHitValue(void) const 				{return 1;}
	virtual uchar CConsumeType(void) const				{return LIQUID;}
	virtual ushort GetDensity(void) const				{return 1000;}
	virtual uchar EffectType(void) 				{return EOMLEURINE;}
	virtual ushort OfferValue(void) const				{ return 100; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual short NutritionValue(void) const			{ return 50; }
	virtual uchar CItemColor(void) const			{ return YELLOW; }
);

MATERIAL(
	bananapeal,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "banana peal" : "a banana peal";}
	virtual ushort GetHitValue(void) const 				{return 10;}
	virtual uchar CConsumeType(void) const				{return ODD;}
	virtual ushort GetDensity(void) const				{return 500;}
	virtual ushort OfferValue(void) const				{ return 1; }
	virtual uchar CItemColor(void) const			{ return YELLOW; }
);

MATERIAL(
	parchment,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "parchment" : "a parchment";}
	virtual ushort GetHitValue(void) const 				{return 15;}
	virtual uchar CConsumeType(void) const				{return ODD;}
	virtual ushort GetDensity(void) const				{return 600;}
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
);

MATERIAL(
	cloth,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "cloth" : "a cloth";}
	virtual ushort GetHitValue(void) const 				{return 15;}
	virtual uchar CConsumeType(void) const				{return ODD;}
	virtual ushort GetDensity(void) const				{return 100;}
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar CItemColor(void) const			{ return BLUE; }
	virtual bool IsSolid(void) const { return true; }
);

MATERIAL(
	humanflesh,
	flesh,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "human flesh" : "a human flesh";}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual ushort CEmitation(void) const				{ return 192; }
	virtual ushort OfferValue(void) const				{ return 30; }
	virtual uchar Alignment(void) const				{ return EVIL; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
);

MATERIAL(
	slime,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "slime" : "a slime";}
	virtual ushort GetHitValue(void) const 				{return 2;}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual ushort GetDensity(void) const 				{return 400;}
	virtual ushort OfferValue(void) const				{ return 1; }
	virtual short NutritionValue(void) const			{ return 25; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual uchar CFleshColor(void) const			{ return FLESH; }
	virtual uchar CItemColor(void) const			{ return GREEN; }
);

MATERIAL(
	brownslime,
	slime,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "brown slime" : "a brown slime";}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual ushort OfferValue(void) const				{ return 1; }
	virtual short NutritionValue(void) const			{ return 25; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
);

MATERIAL(
	wolfflesh,
	flesh,
public:
	virtual ~wolfflesh(void)				{}
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "wolf flesh" : "a wolf flesh";}
	virtual uchar CConsumeType(void) const				{return MEAT;}
	virtual ushort OfferValue(void) const				{ return 15; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
);

MATERIAL(
	dogflesh,
	wolfflesh,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "dog flesh" : "a dog flesh";}
	virtual ushort OfferValue(void) const				{ return 15; }
	virtual uchar Alignment(void) const				{ return EVIL; }
);

MATERIAL(
	ennerbeastflesh,
	humanflesh, //???
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "Enner Beast flesh" : "an Enner Beast flesh";}
	virtual ushort GetHitValue(void) const 				{return 30;}
	virtual ushort GetDensity(void) const				{return 2400;}
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
);

MATERIAL(
	pepsi,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "pepsi" : "a pepsi";}
	virtual ushort GetHitValue(void) const 				{return 200;}
	virtual uchar CConsumeType(void) const				{return LIQUID;}
	virtual ushort GetDensity(void) const 				{return 1500;}
	virtual ushort OfferValue(void) const				{ return 50; }
	virtual uchar Alignment(void) const				{ return EVIL; }
	virtual uchar EffectType(void) 				{ return EPEPSI; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual short NutritionValue(void) const			{ return 100; }
	virtual uchar CFleshColor(void) const			{ return FLESH; }
	virtual uchar CItemColor(void) const			{ return BLACK; }
);

MATERIAL(
	mithril,
	material,
public:
	virtual std::string Name(uchar Case) const				{return !(Case & INDEFINEBIT) ? "mithril" : "a mithril";}
	virtual ushort GetHitValue(void) const 				{return 200;}
	virtual uchar CConsumeType(void) const				{return HARD;}
	virtual ushort GetDensity(void) const 				{return 5000;}
	virtual ushort OfferValue(void) const				{ return 25; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
);

#endif
