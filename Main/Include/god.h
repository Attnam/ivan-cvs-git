#ifndef __GOD_H__
#define __GOD_H__

#define GOD_NAME Name().c_str()

#define ALPP	0   // A = Alignment
#define ALP	1   // L = Lawful
#define AL	2   // N = Neutral
#define ALM	3   // C = Chaotic
#define ANP	4   // P = Plus
#define AN	5   // M = Minus
#define ANM	6
#define ACP	7
#define AC	8
#define ACM	9
#define ACMM	10

#define NUMBER_OF_GODS 16

#include <string>

#include "typedef.h"

#include "material.h"

class item;

class god
{
public:
	god(void) : Relation(0), Timer(0) {}
	virtual void Pray(void);
	virtual std::string Name(void) const = 0;
	virtual std::string Description(void) const = 0;
	virtual uchar Alignment(void) const = 0;
	virtual std::string CompleteDescription(void) const;
	virtual void ApplyDivineTick(ushort Turns) { Timer -= Turns; if(Timer < 0) Timer = 0; }
	virtual void AdjustRelation(god*, bool);
	virtual void AdjustRelation(short Amount);
	virtual void AdjustTimer(long Amount);
	virtual void Save(std::ofstream*) const;
	virtual void Load(std::ifstream*);
	virtual void SetRelation(short Value) { Relation = Value; }
	virtual void SetTimer(long Value) { Timer = Value; }
	virtual bool ReceiveOffer(item*);
	virtual uchar BasicAlignment(void) const { return NEUTRAL; }
	virtual short GetRelation(void) const { return Relation; }
	virtual void PrintRelation(void) const;
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
	short Relation;
	long Timer;
};

class valpuri : public god
{
public:
	virtual std::string Name(void) const { return "Valpuri"; }
	virtual std::string Description(void) const { return "king of gods"; }
	virtual uchar Alignment(void) const { return ALPP; }
	virtual void Pray(void);
	virtual uchar BasicAlignment(void) const { return GOOD; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class venius : public god
{
public:
	virtual std::string Name(void) const { return "Venius"; }
	virtual std::string Description(void) const { return "god of paladins and holy fire"; }
	virtual uchar Alignment(void) const { return ALP; }
	virtual uchar BasicAlignment(void) const { return GOOD; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class atavus : public god
{
public:
	virtual std::string Name(void) const { return "Atavus"; }
	virtual std::string Description(void) const { return "god of gifts"; }
	virtual uchar Alignment(void) const { return ALP; }
	virtual void Pray(void);
	virtual uchar BasicAlignment(void) const { return GOOD; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class dulcis : public god
{
public:
	virtual std::string Name(void) const { return "Dulcis"; }
	virtual std::string Description(void) const { return "goddess of music"; }
	virtual uchar Alignment(void) const { return AL; }
	virtual uchar BasicAlignment(void) const { return GOOD; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class inasnum : public god
{
public:
	virtual std::string Name(void) const { return "Inasnum"; }
	virtual std::string Description(void) const { return "god of prophecies and dreams"; }
	virtual uchar Alignment(void) const { return AL; }
	virtual uchar BasicAlignment(void) const { return GOOD; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class seges : public god
{
public:
	virtual std::string Name(void) const { return "Seges"; }
	virtual std::string Description(void) const { return "goddess of health and nutrition"; }
	virtual uchar Alignment(void) const { return AL; }
	virtual uchar BasicAlignment(void) const { return GOOD; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class consummo : public god
{
public:
	virtual std::string Name(void) const { return "Consummo"; }
	virtual std::string Description(void) const { return "god of knowledge, wisdom and understanding"; }
	virtual uchar Alignment(void) const { return ALM; }
	virtual uchar BasicAlignment(void) const { return GOOD; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class loricatus : public god
{
public:
	virtual std::string Name(void) const { return "Loricatus"; }
	virtual std::string Description(void) const { return "god of fire, machines and weaponry"; }
	virtual uchar Alignment(void) const { return AN; }
	virtual uchar BasicAlignment(void) const { return NEUTRAL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class mellis : public god
{
public:
	virtual std::string Name(void) const { return "Mellis"; }
	virtual std::string Description(void) const { return "god of money, trade and politics"; }
	virtual uchar Alignment(void) const { return ANM; }
	virtual uchar BasicAlignment(void) const { return NEUTRAL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class calamus : public god
{
public:
	virtual std::string Name(void) const { return "Calamus"; }
	virtual std::string Description(void) const { return "god of assassins, thieves & other secretive activities"; }
	virtual uchar Alignment(void) const { return ACP; }
	virtual uchar BasicAlignment(void) const { return EVIL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class pestifer : public god
{
public:
	virtual std::string Name(void) const { return "Pestifer"; }
	virtual std::string Description(void) const { return "god of pain, misery and annoying noise"; }
	virtual uchar Alignment(void) const { return AC; }
	virtual uchar BasicAlignment(void) const { return EVIL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class macellarius : public god
{
public:
	virtual std::string Name(void) const { return "Macellarius"; }
	virtual std::string Description(void) const { return "god of greed and forbidden pleasures"; }
	virtual uchar Alignment(void) const { return AC; }
	virtual uchar BasicAlignment(void) const { return EVIL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class scabies : public god
{
public:
	virtual std::string Name(void) const { return "Scabies"; }
	virtual std::string Description(void) const { return "goddess of mutations, deseases and famine"; }
	virtual uchar Alignment(void) const { return AC; }
	virtual uchar BasicAlignment(void) const { return EVIL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class infuscor : public god
{
public:
	virtual std::string Name(void) const { return "Infuscor"; }
	virtual std::string Description(void) const { return "goddess of wrong knowledge and vile magic"; }
	virtual uchar Alignment(void) const { return ACM; }
	virtual uchar BasicAlignment(void) const { return EVIL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class cruentus : public god
{
public:
	virtual std::string Name(void) const { return "Cruentus"; }
	virtual std::string Description(void) const { return "god of war and blood"; }
	virtual uchar Alignment(void) const { return ACM; }
	virtual void Pray(void);
	virtual uchar BasicAlignment(void) const { return EVIL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

class erado : public god
{
public:
	virtual std::string Name(void) const { return "Erado"; }
	virtual std::string Description(void) const { return "destroyer of worlds"; }
	virtual uchar Alignment(void) const { return ACMM; }
	virtual void Pray(void);
	virtual uchar BasicAlignment(void) const { return EVIL; }
protected:
	virtual void PrayGoodEffect(void);
	virtual void PrayBadEffect(void);
};

#endif
