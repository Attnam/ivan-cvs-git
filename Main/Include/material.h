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

#include <string>

#include "typedef.h"

#include "typeable.h"
#include "proto.h"

/* Presentation of material class */

class character;
class item;
class object;

class material : public typeable
{
public: 
	virtual ~material(void)					{}
	virtual uchar GetFleshColor(void) const			{ return GetItemColor(); }
	virtual uchar GetItemColor(void) const = 0;
	virtual std::string Name(uchar Case = 0) const;
	virtual ushort GetHitValue(void) const = 0;
	virtual uchar GetConsumeType(void) const			{ return ODD; }
	virtual ulong GetVolume(void) const			{ return Volume; }
	virtual ulong GetWeight(void) const			{ return ulong(float(Volume) * GetDensity() / 1000); }
	virtual ushort GetDensity(void) const = 0;
	virtual ushort TakeDipVolumeAway(void);
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual ushort GetArmorValue(void) const { return GetHitValue(); }
	virtual void SetVolume(ulong What) { Volume = What; }
	virtual ushort GetEmitation(void) const { return 0; }
	virtual ushort OfferValue(void) const = 0;
	virtual uchar Alignment(void) const				{ return NEUTRAL; }
	virtual void EatEffect(character*, float = 100, float = 1.0f)	{ }
	virtual void HitEffect(character*)			{ }
	virtual short NutritionValue(void) const			{ return 0; }
	virtual void MinusAmount(float Amount)			{ SetVolume(GetVolume() - ulong(GetVolume() * Amount / 100)); }
	virtual material* Clone(ulong Volume) const = 0;
	virtual material* Clone(void) const = 0;
	virtual bool IsType(ushort QType) const { return Type() == QType; }
	virtual bool IsSolid(void) const { return false; }
	virtual void Be(void) {}
	/*static ushort GetProtoIndexBegin(void) { return ProtoIndexBegin; }
	static ushort GetProtoIndexEnd(void) { return ProtoIndexEnd; }
	static ushort GetProtoAmount(void) { return ProtoIndexEnd - ProtoIndexBegin; }*/
protected:
	virtual std::string NameStem(void) const = 0;
	virtual std::string Article(void) const { return "a"; }
	virtual void NormalFoodEffect(character*, float, float);
	ulong Volume;
	//static ushort ProtoIndexBegin, ProtoIndexEnd;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define MATERIAL(name, base, data)\
	\
	name : public base\
	{\
	public:\
		name(ulong IVolume) { Volume = IVolume; }\
		name(void) {}\
		virtual material* Clone(ulong Volume) const { return new name(Volume); }\
		virtual material* Clone(void) const { return new name; }\
		virtual typeable* CloneAndLoad(std::ifstream& SaveFile) const { material* Mat = new name; Mat->Load(SaveFile); return Mat; }\
		static ushort StaticType(void);\
	protected:\
		virtual ushort Type(void) const;\
		data\
	};\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller(void) : Index(protocontainer<material>::Add(new name)) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	ushort name::StaticType(void) { return name##_ProtoInstaller.GetIndex(); }\
	ushort name::Type(void) const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define MATERIAL(name, base, data)\
	\
	name : public base\
	{\
	public:\
		name(ulong IVolume) { Volume = IVolume; }\
		name(void) {}\
		virtual material* Clone(ulong Volume) const { return new name(Volume); }\
		virtual material* Clone(void) const { return new name; }\
		virtual typeable* CloneAndLoad(std::ifstream& SaveFile) const { material* Mat = new name; Mat->Load(SaveFile); return Mat; }\
		static ushort StaticType(void);\
	protected:\
		virtual ushort Type(void) const;\
		data\
	};

#endif

//BEGIN_PROTOTYPING(material)

class MATERIAL
(
	iron,
	material,
public:
	virtual ushort GetHitValue(void) const				{ return 100; }
	virtual uchar GetConsumeType(void) const				{ return HARD; }
	virtual ushort GetDensity(void) const				{ return 8000; }
	virtual ushort OfferValue(void) const				{ return 10; }
	virtual uchar GetItemColor(void) const			{ return LGRAY; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "iron"; }
	virtual std::string Article(void) const { return "an"; }
);

class MATERIAL
(
	valpurium,
	material,
public:
	virtual ushort GetHitValue(void) const				{ return 400; }
	virtual uchar GetConsumeType(void) const				{ return HARD; }
	virtual ushort GetDensity(void) const				{ return 3000; }
	virtual ushort OfferValue(void) const				{ return 100; }
	virtual uchar Alignment(void) const				{ return GOOD; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "valpurium"; }
);

class MATERIAL
(
	stone,
	material,
public:
	virtual ushort GetHitValue(void) const				{ return 50; }
	virtual uchar GetConsumeType(void) const				{ return HARD; }
	virtual ushort GetDensity(void) const				{ return 3000; }
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar GetItemColor(void) const			{ return DGRAY; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "stone"; }
);

class MATERIAL
(
	bananaflesh,
	material,
public:
	virtual ushort GetHitValue(void) const				{ return 5; }
	virtual uchar GetConsumeType(void) const				{ return FRUIT; }
	virtual ushort GetDensity(void) const				{ return 1200; }
	virtual ushort OfferValue(void) const				{ return 10; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual short NutritionValue(void) const			{ return 175; }
	virtual uchar GetItemColor(void) const			{ return YELLOW; }
protected:
	virtual std::string NameStem(void) const	{ return "banana flesh"; }
);

class MATERIAL
(
	gravel,
	material,
public:
	virtual ushort GetHitValue(void) const				{ return 40; }
	virtual uchar GetConsumeType(void) const				{ return ODD; }
	virtual ushort GetDensity(void) const				{ return 2500; }
	virtual ushort OfferValue(void) const				{ return 1; }
	virtual uchar GetItemColor(void) const			{ return LGRAY; }
protected:
	virtual std::string NameStem(void) const	{ return "gravel"; }
);

class MATERIAL
(
	moraine,
	material,
public:
	
	virtual ushort GetHitValue(void) const				{ return 25; }
	virtual uchar GetConsumeType(void) const				{ return ODD; }
	virtual ushort GetDensity(void) const				{ return 2500; }
	virtual ushort OfferValue(void) const				{ return 2; }
	virtual uchar GetItemColor(void) const			{ return BROWN; }
protected:
	virtual std::string NameStem(void) const	{ return "moraine"; }
);

class MATERIAL
(
	schoolfood,
	material,
public:
	virtual ushort GetHitValue(void) const				{ return 5; }
	virtual uchar GetConsumeType(void) const				{ return SCHOOLFOOD; }
	virtual ushort GetDensity(void) const				{ return 1500; }
	virtual ushort OfferValue(void) const				{ return 20; }
	virtual uchar Alignment(void) const				{ return EVIL; }
	virtual short NutritionValue(void) const			{ return 75; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual uchar GetItemColor(void) const			{ return BROWN; }
protected:
	virtual std::string NameStem(void) const	{ return "school food"; }
);

class MATERIAL
(
	air,
	material,
public:
	virtual ushort GetHitValue(void) const				{ return 0; }
	virtual ushort GetDensity(void) const				{ return 1; }
	virtual ushort OfferValue(void) const				{ return 0; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
protected:
	virtual std::string NameStem(void) const	{ return "air"; }
	virtual std::string Article(void) const { return "an"; }
);

class MATERIAL
(
	wood,
	material,
public:
	virtual ushort GetHitValue(void) const				{ return 20; }
	virtual uchar GetConsumeType(void) const				{ return HARD; }
	virtual ushort GetDensity(void) const				{ return 500; }
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar GetItemColor(void) const			{ return BROWN; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "wood"; }
);

class MATERIAL
(
	flesh,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 15; }
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort GetDensity(void) const				{ return 1200; }
	virtual ushort OfferValue(void) const				{ return 15; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual short NutritionValue(void) const			{ return 50; }
	virtual uchar GetFleshColor(void) const			{ return FLESH; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
protected:
	virtual std::string NameStem(void) const	{ return "flesh"; }
);

class MATERIAL
(
	goblinoidflesh,
	flesh,
public:
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort OfferValue(void) const				{ return 10; }
	virtual uchar GetItemColor(void) const			{ return GREEN; }
protected:
	virtual std::string NameStem(void) const	{ return "green flesh"; }
);

class MATERIAL
(
	pork,
	flesh,
public:
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort OfferValue(void) const				{ return 20; }
	virtual short NutritionValue(void) const			{ return 500; }
	virtual uchar GetItemColor(void) const			{ return BROWN; }
protected:
	virtual std::string NameStem(void) const	{ return "pork"; }
);

class MATERIAL
(
	beef,
	flesh,
public:
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort OfferValue(void) const				{ return 20; }
	virtual short NutritionValue(void) const			{ return 500; }
	virtual uchar GetItemColor(void) const			{ return BROWN; }
protected:
	virtual std::string NameStem(void) const	{ return "beef"; }
);

class MATERIAL
(
	bone,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 25; }
	virtual ushort GetDensity(void) const				{ return 2000; }
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar GetConsumeType(void) const				{ return BONE; }
	virtual void EatEffect(character*, float, float);
	virtual uchar GetFleshColor(void) const			{ return FLESH; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "bone"; }
);

class MATERIAL
(
	darkfrogflesh,
	flesh,
public:
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort OfferValue(void) const				{ return 50; }
	virtual uchar Alignment(void) const				{ return EVIL; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual uchar GetItemColor(void) const			{ return BLACK; }
protected:
	virtual std::string NameStem(void) const	{ return "dark frog flesh"; }
);

class MATERIAL
(
	elpuriflesh,
	darkfrogflesh,
public:
	virtual ushort GetHitValue(void) const 				{ return 30; }
	virtual ushort GetDensity(void) const				{ return 2400; }
	virtual ushort OfferValue(void) const				{ return 1; }
protected:
	virtual std::string NameStem(void) const	{ return "Elpuri's flesh"; }
	virtual std::string Article(void) const { return "an"; }
);

class MATERIAL
(
	glass,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 30; }
	virtual uchar GetConsumeType(void) const				{ return HARD; }
	virtual ushort GetDensity(void) const				{ return 2500; }
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar GetItemColor(void) const			{ return BLUE; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "glass"; }
);

class MATERIAL
(
	omleurine,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 1; }
	virtual uchar GetConsumeType(void) const				{ return LIQUID; }
	virtual ushort GetDensity(void) const				{ return 1000; }
	virtual ushort OfferValue(void) const				{ return 100; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual short NutritionValue(void) const			{ return 50; }
	virtual uchar GetItemColor(void) const			{ return YELLOW; }
protected:
	virtual std::string NameStem(void) const	{ return "Omle urine"; }
	virtual std::string Article(void) const { return "an"; }
);

class MATERIAL
(
	bananapeal,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 10; }
	virtual uchar GetConsumeType(void) const				{ return ODD; }
	virtual ushort GetDensity(void) const				{ return 500; }
	virtual ushort OfferValue(void) const				{ return 1; }
	virtual uchar GetItemColor(void) const			{ return YELLOW; }
protected:
	virtual std::string NameStem(void) const	{ return "banana peal"; }
);

class MATERIAL
(
	parchment,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 15; }
	virtual uchar GetConsumeType(void) const				{ return ODD; }
	virtual ushort GetDensity(void) const				{ return 600; }
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "parchment"; }
);

class MATERIAL
(
	cloth,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 15; }
	virtual uchar GetConsumeType(void) const				{ return ODD; }
	virtual ushort GetDensity(void) const				{ return 100; }
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar GetItemColor(void) const			{ return BLUE; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "cloth"; }
);

class MATERIAL
(
	humanflesh,
	flesh,
public:
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort OfferValue(void) const				{ return 30; }
	virtual uchar Alignment(void) const				{ return EVIL; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
protected:
	virtual std::string NameStem(void) const	{ return "human flesh"; }
);

class MATERIAL
(
	slime,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 2; }
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort GetDensity(void) const 				{ return 400; }
	virtual ushort OfferValue(void) const				{ return 1; }
	virtual short NutritionValue(void) const			{ return 25; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual uchar GetFleshColor(void) const			{ return FLESH; }
	virtual uchar GetItemColor(void) const			{ return GREEN; }
protected:
	virtual std::string NameStem(void) const	{ return "slime"; }
);

class MATERIAL
(
	brownslime,
	slime,
public:
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort OfferValue(void) const				{ return 1; }
	virtual short NutritionValue(void) const			{ return 25; }
	virtual uchar GetItemColor(void) const			{ return BROWN; }
protected:
	virtual std::string NameStem(void) const	{ return "brown slime"; }
);

class MATERIAL
(
	wolfflesh,
	flesh,
public:
	virtual uchar GetConsumeType(void) const				{ return MEAT; }
	virtual ushort OfferValue(void) const				{ return 15; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
protected:
	virtual std::string NameStem(void) const	{ return "wolf flesh"; }
);

class MATERIAL
(
	dogflesh,
	wolfflesh,
public:
	virtual ushort OfferValue(void) const				{ return 15; }
	virtual uchar Alignment(void) const				{ return EVIL; }
protected:
	virtual std::string NameStem(void) const	{ return "dog flesh"; }
);

class MATERIAL
(
	ennerbeastflesh,
	humanflesh, //???
public:
	virtual ushort GetHitValue(void) const 				{ return 30; }
	virtual ushort GetDensity(void) const				{ return 2400; }
	virtual ushort OfferValue(void) const				{ return 5; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
protected:
	virtual std::string NameStem(void) const	{ return "Enner Beast flesh"; }
	virtual std::string Article(void) const { return "an"; }
);

class MATERIAL
(
	pepsi,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 200; }
	virtual uchar GetConsumeType(void) const				{ return LIQUID; }
	virtual ushort GetDensity(void) const 				{ return 1500; }
	virtual ushort OfferValue(void) const				{ return 50; }
	virtual uchar Alignment(void) const				{ return EVIL; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual short NutritionValue(void) const			{ return 100; }
	virtual uchar GetFleshColor(void) const			{ return FLESH; }
	virtual uchar GetItemColor(void) const			{ return BLACK; }
protected:
	virtual std::string NameStem(void) const	{ return "pepsi"; }
);

class MATERIAL
(
	mithril,
	material,
public:
	virtual ushort GetHitValue(void) const 				{ return 200; }
	virtual uchar GetConsumeType(void) const				{ return HARD; }
	virtual ushort GetDensity(void) const 				{ return 5000; }
	virtual ushort OfferValue(void) const				{ return 25; }
	virtual uchar GetItemColor(void) const			{ return WHITE; }
	virtual bool IsSolid(void) const { return true; }
protected:
	virtual std::string NameStem(void) const	{ return "mithril"; }
);

//FINISH_PROTOTYPING(material)

#endif
