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

/* EVIL!! VERY EVIL! */

#define IMATERIAL	-1 // I = Index
#define IIRON 		1
#define IVALPURIUM	2
#define ISTONE		3
#define IBANANAFLESH	4
#define IGRAVEL 	5
#define IMORAINE	6
#define ISCHOOLFOOD	7
#define IAIR		8
#define IWOOD		9
#define IFLESH		10
#define IBONE		11
#define IDARKFROGFLESH 	12
#define IELPURIFLESH	13
#define IGLASS		14
#define IOMLEURINE	15
#define IBANANAPEAL	16
#define IPARCHMENT	17
#define ICLOTH		18
#define IHUMANFLESH	19
#define ISLIME		20
#define IBROWNSLIME	21
#define IGOBLINOIDFLESH	22
#define IPORK		23
#define IBEEF		24
#define IPEPSI		25
#define IENNERBEASTFLESH 27
#define IWOLFFLESH	28
#define IDOGFLESH	29
#define IMITHRIL	30

/* REALLY! */

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
	material(ulong);
	material(std::ifstream*);
	virtual ~material(void)					{}
	virtual uchar CFleshColor(void) const const			{ return CItemColor(); }
	virtual uchar CItemColor(void) const = 0;
	virtual std::string Name(uchar Case = 0)		{return !(Case & INDEFINEBIT) ? "unknown substance" : "an unknown substance";}
	virtual ushort GetHitValue(void)			{return 0;}
	virtual uchar CConsumeType(void)			{return ODD;}
	virtual ulong CVolume(void)				{return Volume;}
	virtual ulong CWeight(void)				{return ulong(float(Volume) * GetDensity() / 1000);}
	virtual ushort GetDensity(void)				{return 1;}
	virtual uchar EffectType(void)				{return EFOOD;}
	virtual ushort Type(void)				{return 0;}
	virtual ushort TakeDipVolumeAway(void);
	virtual void Save(std::ofstream*);
	virtual ushort CArmorValue(void) { return GetHitValue(); }
	virtual void SVolume(ulong What) { Volume = What; }
	virtual ushort CEmitation(void) { return 0; }
	virtual ushort OfferValue(void)				{ return 0; }
	virtual uchar Alignment(void)				{ return NEUTRAL; }
	virtual void EatEffect(character*, float = 100, float = 1.0f)	{ }
	virtual void HitEffect(character*)			{ }
	virtual short NutritionValue(void)			{ return 0; }
	virtual void MinusAmount(float Amount)			{ SVolume(CVolume() - ulong(CVolume() * Amount / 100)); }
protected:
	virtual void NormalFoodEffect(character*, float, float);
	ulong Volume;
};

#include <cstdarg>

inline material** NewMaterial(ushort Materials, ...)
{
	material** Material = new material*[Materials];

	va_list AP;

	va_start(AP, Materials);

	for(ushort c = 0; c < Materials; c++)
		Material[c] = va_arg(AP, material*);

	va_end(AP);

	return Material;
}

class iron : public material
{
public:
	iron(ulong Volume) : material(Volume)		{}
	iron(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~iron(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "iron" : "an iron";}
	virtual ushort GetHitValue(void)				{return 100;}
	virtual uchar CConsumeType(void)				{return HARD;}
	virtual ushort GetDensity(void)				{return 8000;}
	virtual ushort Type(void)					{return IIRON;}
	virtual ushort OfferValue(void)				{ return 10; }
	virtual uchar CItemColor(void) const			{ return LGRAY; }
};

class valpurium : public material
{
public:
	valpurium(ulong Volume) : material(Volume)		{}
	valpurium(std::ifstream* SaveFile) : material(SaveFile)	{}
	virtual ~valpurium(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "valpurium" : "a valpurium";}
	virtual ushort GetHitValue(void)				{return 400;}
	virtual uchar CConsumeType(void)				{return HARD;}
	virtual ushort GetDensity(void)				{return 3000;}
	virtual ushort Type(void)					{return IVALPURIUM;}
	virtual ushort OfferValue(void)				{ return 100; }
	virtual uchar Alignment(void)				{ return GOOD; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class stone : public material
{
public:
	stone(ulong Volume) : material(Volume)		{}
	stone(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~stone(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "stone" : "a stone";}
	virtual ushort GetHitValue(void)				{return 50;}
	virtual uchar CConsumeType(void)				{return HARD;}
	virtual ushort GetDensity(void)				{return 3000;}
	virtual ushort Type(void)					{return ISTONE;}
	virtual ushort OfferValue(void)				{ return 5; }
	virtual uchar CItemColor(void) const			{ return DGRAY; }
};

class bananaflesh : public material
{
public:
	bananaflesh(ulong Volume) : material(Volume)	{}
	bananaflesh(std::ifstream* SaveFile) : material(SaveFile)	{}
	virtual ~bananaflesh(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "banana flesh" : "a banana flesh";}
	virtual ushort GetHitValue(void)				{return 5;}
	virtual uchar CConsumeType(void)				{return FRUIT;}
	virtual ushort GetDensity(void)				{return 1200;}
	virtual ushort Type(void)					{return IBANANAFLESH;}
	virtual uchar EffectType(void)				{return EFOOD;}
	virtual ushort OfferValue(void)				{ return 10; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual short NutritionValue(void)			{ return 175; }
	virtual uchar CItemColor(void) const			{ return YELLOW; }
};

class gravel : public material
{
public:
	gravel(ulong Volume) : material(Volume)		{}
	gravel(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~gravel(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "gravel" : "a gravel";}
	virtual ushort GetHitValue(void)				{return 40;}
	virtual uchar CConsumeType(void)				{return ODD;}
	virtual ushort GetDensity(void)				{return 2500;}
	virtual ushort Type(void)					{return IGRAVEL;}
	virtual ushort OfferValue(void)				{ return 1; }
	virtual uchar CItemColor(void) const			{ return LGRAY; }
};

class moraine : public material
{
public:
	moraine(ulong Volume) : material(Volume) 		{}
	moraine(std::ifstream* SaveFile) : material(SaveFile)	{}
	virtual ~moraine(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "moraine" : "a moraine";}
	virtual ushort GetHitValue(void)				{return 25;}
	virtual uchar CConsumeType(void)				{return ODD;}
	virtual ushort GetDensity(void)				{return 2500;}
	virtual ushort Type(void)					{return IMORAINE;}
	virtual ushort OfferValue(void)				{ return 2; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
};

class schoolfood : public material
{
public:
	schoolfood(ulong Volume) : material(Volume)	{}
	schoolfood(std::ifstream* SaveFile) : material(SaveFile)	{}
	virtual ~schoolfood(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "school food" : "a school food";}
	virtual ushort GetHitValue(void)				{return 5;}
	virtual uchar CConsumeType(void)				{return SCHOOLFOOD;}
	virtual ushort GetDensity(void)				{return 1500;}
	virtual uchar EffectType(void)				{return ESCHOOLFOOD;}
	virtual ushort Type(void)					{return ISCHOOLFOOD;}
	virtual ushort OfferValue(void)				{ return 20; }
	virtual uchar Alignment(void)				{ return EVIL; }
	virtual short NutritionValue(void)			{ return 75; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual uchar CItemColor(void) const			{ return BROWN; }
};

class air : public material
{
public:
	air(ulong Volume) : material(Volume)		{}
	air(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~air(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "air" : "an air";}
	virtual ushort GetHitValue(void)				{return 0;}
	virtual ushort GetDensity(void)				{return 1;}
	virtual ushort Type(void)					{return IAIR;}
	virtual ushort OfferValue(void)				{ return 0; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class wood : public material
{
public:
	wood(ulong Volume) : material(Volume)		{}
	wood(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~wood(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "wood" : "a wood";}
	virtual ushort GetHitValue(void)				{return 20;}
	virtual uchar CConsumeType(void)				{return HARD;}
	virtual ushort GetDensity(void)				{return 500;}
	virtual ushort Type(void)					{return IWOOD;}
	virtual ushort OfferValue(void)				{ return 5; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
};

class flesh : public material
{
public:	
	flesh(ulong Volume) : material(Volume)		{}
	flesh(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~flesh(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "flesh" : "a flesh";}
	virtual ushort GetHitValue(void) 				{return 15;}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort GetDensity(void)				{return 1200;}
	virtual ushort Type(void)					{return IFLESH;}
	virtual ushort OfferValue(void)				{ return 15; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual short NutritionValue(void)			{ return 50; }
	virtual uchar CFleshColor(void) const			{ return FLESH; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class goblinoidflesh : public flesh
{
public:
	goblinoidflesh(ulong Volume) : flesh(Volume)		{}
	goblinoidflesh(std::ifstream* SaveFile) : flesh(SaveFile)		{}
	virtual ~goblinoidflesh(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "green flesh" : "a green flesh";}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort Type(void)					{return IGOBLINOIDFLESH;}
	virtual ushort OfferValue(void)				{ return 10; }
	virtual uchar CItemColor(void) const			{ return GREEN; }
};

class pork : public flesh
{
public:
	pork(ulong Volume) : flesh(Volume)			{}
	pork(std::ifstream* SaveFile) : flesh(SaveFile)		{}
	virtual ~pork(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "pork" : "a pork";}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort Type(void)					{return IPORK;}
	virtual ushort OfferValue(void)				{ return 20; }
	virtual short NutritionValue(void)			{ return 500; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
};

class beef : public flesh
{
public:	
	beef(ulong Volume) : flesh(Volume)			{}
	beef(std::ifstream* SaveFile) : flesh(SaveFile)		{}
	virtual ~beef(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "beef" : "a beef";}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort Type(void)					{return IBEEF;}
	virtual ushort OfferValue(void)				{ return 20; }
	virtual short NutritionValue(void)			{ return 500; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
};

class bone : public material
{
public:	
	bone(ulong Volume) : material(Volume)		{}
	bone(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~bone(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "bone" : "a bone";}
	virtual ushort GetHitValue(void) 				{return 25;}
	virtual ushort GetDensity(void)				{return 2000;}
	virtual ushort Type(void)					{return IBONE;}
	virtual ushort OfferValue(void)				{ return 5; }
	virtual uchar CConsumeType(void)				{ return BONE; }
	virtual void EatEffect(character*, float, float);
	virtual uchar CFleshColor(void) const			{ return FLESH; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class darkfrogflesh : public flesh
{
public:	
	darkfrogflesh(ulong Volume) : flesh(Volume)		{}
	darkfrogflesh(std::ifstream* SaveFile) : flesh(SaveFile)	{}
	virtual ~darkfrogflesh(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "dark forg flesh" : "a dark frog flesh";}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort Type(void)					{return IDARKFROGFLESH;}
	virtual uchar EffectType(void) 				{return EDARKNESS;}
	virtual ushort OfferValue(void)				{ return 50; }
	virtual uchar Alignment(void)				{ return EVIL; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual uchar CItemColor(void) const			{ return BLACK; }
};

class elpuriflesh : public darkfrogflesh
{
public:
	elpuriflesh(ulong Volume) : darkfrogflesh(Volume)		{}
	elpuriflesh(std::ifstream* SaveFile) : darkfrogflesh(SaveFile) {}
	virtual ~elpuriflesh()                                  {}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "Elpuri's flesh" : "an Elpuri's flesh";}
	virtual ushort GetHitValue(void) 				{return 30;}
	virtual ushort GetDensity(void)				{return 2400;}
	virtual ushort Type(void)					{return IELPURIFLESH;}
	virtual ushort OfferValue(void)				{ return 1; }
};

class glass : public material
{
public:	
	glass(ulong Volume) : material(Volume)		{}
	glass(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~glass(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "glass" : "a glass";}
	virtual ushort GetHitValue(void) 				{return 30;}
	virtual uchar CConsumeType(void)				{return HARD;}
	virtual ushort GetDensity(void)				{return 2500;}
	virtual ushort Type(void)					{return IGLASS;}
	virtual ushort OfferValue(void)				{ return 5; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class omleurine : public material
{
public:	
	omleurine(ulong Volume) : material(Volume)	{}
	omleurine(std::ifstream* SaveFile) : material(SaveFile)	{}
	virtual ~omleurine(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "Omle urine" : "an Omle urine";}
	virtual ushort GetHitValue(void) 				{return 1;}
	virtual uchar CConsumeType(void)				{return LIQUID;}
	virtual ushort GetDensity(void)				{return 1000;}
	virtual uchar EffectType(void) 				{return EOMLEURINE;}
	virtual ushort Type(void)					{return IOMLEURINE;}
	virtual ushort OfferValue(void)				{ return 100; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual short NutritionValue(void)			{ return 50; }
	virtual uchar CItemColor(void) const			{ return YELLOW; }
};

class bananapeal : public material
{
public:
	bananapeal(ulong Volume) : material(Volume)	{}
	bananapeal(std::ifstream* SaveFile) : material(SaveFile)	{}
	virtual ~bananapeal(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "banana peal" : "a banana peal";}
	virtual ushort GetHitValue(void) 				{return 10;}
	virtual uchar CConsumeType(void)				{return ODD;}
	virtual ushort GetDensity(void)				{return 500;}
	virtual ushort Type(void)					{return IBANANAPEAL;}
	virtual ushort OfferValue(void)				{ return 1; }
	virtual uchar CItemColor(void) const			{ return YELLOW; }
};

class parchment : public material
{
public:	
	parchment(ulong Volume) : material(Volume)		{}
	parchment(std::ifstream* SaveFile) : material(SaveFile)	{}
	virtual ~parchment(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "parchment" : "a parchment";}
	virtual ushort GetHitValue(void) 				{return 15;}
	virtual uchar CConsumeType(void)				{return ODD;}
	virtual ushort GetDensity(void)				{return 600;}
	virtual ushort Type(void)					{return IPARCHMENT;}
	virtual ushort OfferValue(void)				{ return 5; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class cloth : public material
{
public:	
	cloth(ulong Volume) : material(Volume)		{}
	cloth(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~cloth(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "cloth" : "a cloth";}
	virtual ushort GetHitValue(void) 				{return 15;}
	virtual uchar CConsumeType(void)				{return ODD;}
	virtual ushort GetDensity(void)				{return 100;}
	virtual ushort Type(void)					{return ICLOTH;}
	virtual ushort OfferValue(void)				{ return 5; }
	virtual uchar CItemColor(void) const			{ return BLUE; }
};

class humanflesh : public flesh
{
public:	
	humanflesh(ulong Volume) : flesh(Volume)			{}
	humanflesh(std::ifstream* SaveFile) : flesh(SaveFile)	{}
	virtual ~humanflesh(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "human flesh" : "a human flesh";}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort Type(void)					{return IHUMANFLESH;}
	virtual ushort CEmitation(void)				{ return 192; }
	virtual ushort OfferValue(void)				{ return 30; }
	virtual uchar Alignment(void)				{ return EVIL; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class slime : public material
{
public:	
	slime(ulong Volume) : material(Volume)		{}
	slime(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~slime(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "slime" : "a slime";}
	virtual ushort GetHitValue(void) 				{return 2;}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort GetDensity(void) 				{return 400;}
	virtual ushort Type(void)					{return ISLIME;}
	virtual ushort OfferValue(void)				{ return 1; }
	virtual short NutritionValue(void)			{ return 25; }
	virtual void EatEffect(character* Eater, float Amount, float NPModifier)	{ NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
	virtual uchar CFleshColor(void) const			{ return FLESH; }
	virtual uchar CItemColor(void) const			{ return GREEN; }
};

class brownslime : public slime
{
public:	
	brownslime(ulong Volume) : slime(Volume)		{}
	brownslime(std::ifstream* SaveFile) : slime(SaveFile)	{}
	virtual ~brownslime(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "brown slime" : "a brown slime";}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort Type(void)					{return IBROWNSLIME;}
	virtual ushort OfferValue(void)				{ return 1; }
	virtual short NutritionValue(void)			{ return 25; }
	virtual uchar CItemColor(void) const			{ return BROWN; }
};

class wolfflesh : public flesh
{
public:	
	wolfflesh(ulong Volume) : flesh(Volume)			{}
	wolfflesh(std::ifstream* SaveFile) : flesh(SaveFile)		{}
	virtual ~wolfflesh(void)				{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "wolf flesh" : "a wolf flesh";}
	virtual uchar CConsumeType(void)				{return MEAT;}
	virtual ushort Type(void)					{return IWOLFFLESH;}
	virtual ushort OfferValue(void)				{ return 15; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class dogflesh : public wolfflesh
{
public:
	dogflesh(ulong Volume) : wolfflesh(Volume)		{}
	dogflesh(std::ifstream* SaveFile) : wolfflesh(SaveFile) 	{}
	virtual ~dogflesh()                                  	{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "dog flesh" : "a dog flesh";}
	virtual ushort Type(void)					{return IDOGFLESH;}
	virtual ushort OfferValue(void)				{ return 15; }
	virtual uchar Alignment(void)				{ return EVIL; }
};

class ennerbeastflesh : public humanflesh
{
public:
	ennerbeastflesh(ulong Volume) : humanflesh(Volume)	{}
	ennerbeastflesh(std::ifstream* SaveFile) : humanflesh(SaveFile) 	{}
	virtual ~ennerbeastflesh()                                  	{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "Enner Beast flesh" : "an Enner Beast flesh";}
	virtual ushort GetHitValue(void) 				{return 30;}
	virtual ushort GetDensity(void)				{return 2400;}
	virtual ushort Type(void)					{return IENNERBEASTFLESH;}
	virtual ushort OfferValue(void)				{ return 5; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

class pepsi : public material
{
public:	
	pepsi(ulong Volume) : material(Volume)		{}
	pepsi(std::ifstream* SaveFile) : material(SaveFile)		{}
	virtual ~pepsi(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "pepsi" : "a pepsi";}
	virtual ushort GetHitValue(void) 				{return 200;}
	virtual uchar CConsumeType(void)				{return LIQUID;}
	virtual ushort GetDensity(void) 				{return 1500;}
	virtual ushort Type(void)					{return IPEPSI;}
	virtual ushort OfferValue(void)				{ return 50; }
	virtual uchar Alignment(void)				{ return EVIL; }
	virtual uchar EffectType(void) 				{ return EPEPSI; }
	virtual void EatEffect(character*, float, float);
	virtual void HitEffect(character* Enemy);
	virtual short NutritionValue(void)			{ return 100; }
	virtual uchar CFleshColor(void) const			{ return FLESH; }
	virtual uchar CItemColor(void) const			{ return BLACK; }
};

class mithril : public material
{
public:	
	mithril(ulong Volume) : material(Volume)		{}
	mithril(std::ifstream* SaveFile) : material(SaveFile)	{}
	virtual ~mithril(void)					{}
	virtual std::string Name(uchar Case)				{return !(Case & INDEFINEBIT) ? "mithril" : "a mithril";}
	virtual ushort GetHitValue(void) 				{return 200;}
	virtual uchar CConsumeType(void)				{return HARD;}
	virtual ushort GetDensity(void) 				{return 5000;}
	virtual ushort Type(void)					{return IMITHRIL;}
	virtual ushort OfferValue(void)				{ return 25; }
	virtual uchar CItemColor(void) const			{ return WHITE; }
};

#endif
