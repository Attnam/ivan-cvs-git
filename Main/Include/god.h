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

#include "materba.h"

class item;
class outputfile;
class inputfile;

class god
{
public:
	god() : Relation(0), Timer(0), Known(false) {}
	virtual void Pray();
	virtual std::string Name() const = 0;
	virtual std::string Description() const = 0;
	virtual uchar Alignment() const = 0;
	virtual std::string CompleteDescription() const;
	virtual void ApplyDivineTick(ushort Turns) { Timer -= Turns; if(Timer < 0) Timer = 0; }
	virtual void AdjustRelation(god*, bool);
	virtual void AdjustRelation(short Amount);
	virtual void AdjustTimer(long Amount);
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void SetRelation(short Value) { Relation = Value; }
	virtual void SetTimer(long Value) { Timer = Value; }
	virtual bool ReceiveOffer(item*);
	virtual uchar BasicAlignment() const { return NEUTRAL; }
	virtual short GetRelation() const { return Relation; }
	virtual void PrintRelation() const;
	virtual void SetKnown(bool What) { Known = What; }
	virtual bool GetKnown() const { return Known; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
	short Relation;
	long Timer;
	bool Known;
};

inline outputfile& operator<<(outputfile& SaveFile, god* God)
{
	God->Save(SaveFile);
	return SaveFile;
}

class valpuri : public god
{
public:
	virtual std::string Name() const { return "Valpuri"; }
	virtual std::string Description() const { return "king of gods"; }
	virtual uchar Alignment() const { return ALPP; }
	virtual void Pray();
	virtual uchar BasicAlignment() const { return GOOD; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class venius : public god
{
public:
	virtual std::string Name() const { return "Venius"; }
	virtual std::string Description() const { return "god of paladins and holy fire"; }
	virtual uchar Alignment() const { return ALP; }
	virtual uchar BasicAlignment() const { return GOOD; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class atavus : public god
{
public:
	virtual std::string Name() const { return "Atavus"; }
	virtual std::string Description() const { return "god of gifts"; }
	virtual uchar Alignment() const { return ALP; }
	virtual void Pray();
	virtual uchar BasicAlignment() const { return GOOD; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class dulcis : public god
{
public:
	virtual std::string Name() const { return "Dulcis"; }
	virtual std::string Description() const { return "goddess of music"; }
	virtual uchar Alignment() const { return AL; }
	virtual uchar BasicAlignment() const { return GOOD; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class inasnum : public god
{
public:
	virtual std::string Name() const { return "Inasnum"; }
	virtual std::string Description() const { return "god of prophecies and dreams"; }
	virtual uchar Alignment() const { return AL; }
	virtual uchar BasicAlignment() const { return GOOD; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class seges : public god
{
public:
	virtual std::string Name() const { return "Seges"; }
	virtual std::string Description() const { return "goddess of health and nutrition"; }
	virtual uchar Alignment() const { return AL; }
	virtual uchar BasicAlignment() const { return GOOD; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class consummo : public god
{
public:
	virtual std::string Name() const { return "Consummo"; }
	virtual std::string Description() const { return "god of knowledge, wisdom and understanding"; }
	virtual uchar Alignment() const { return ALM; }
	virtual uchar BasicAlignment() const { return GOOD; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class loricatus : public god
{
public:
	virtual std::string Name() const { return "Loricatus"; }
	virtual std::string Description() const { return "god of fire, machines and weaponry"; }
	virtual uchar Alignment() const { return AN; }
	virtual uchar BasicAlignment() const { return NEUTRAL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class mellis : public god
{
public:
	virtual std::string Name() const { return "Mellis"; }
	virtual std::string Description() const { return "god of money, trade and politics"; }
	virtual uchar Alignment() const { return ANM; }
	virtual uchar BasicAlignment() const { return NEUTRAL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class calamus : public god
{
public:
	virtual std::string Name() const { return "Calamus"; }
	virtual std::string Description() const { return "god of assassins, thieves & other secretive activities"; }
	virtual uchar Alignment() const { return ACP; }
	virtual uchar BasicAlignment() const { return EVIL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class pestifer : public god
{
public:
	virtual std::string Name() const { return "Pestifer"; }
	virtual std::string Description() const { return "god of pain, misery and annoying noise"; }
	virtual uchar Alignment() const { return AC; }
	virtual uchar BasicAlignment() const { return EVIL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class macellarius : public god
{
public:
	virtual std::string Name() const { return "Macellarius"; }
	virtual std::string Description() const { return "god of greed and forbidden pleasures"; }
	virtual uchar Alignment() const { return AC; }
	virtual uchar BasicAlignment() const { return EVIL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class scabies : public god
{
public:
	virtual std::string Name() const { return "Scabies"; }
	virtual std::string Description() const { return "goddess of mutations, deseases and famine"; }
	virtual uchar Alignment() const { return AC; }
	virtual uchar BasicAlignment() const { return EVIL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class infuscor : public god
{
public:
	virtual std::string Name() const { return "Infuscor"; }
	virtual std::string Description() const { return "goddess of wrong knowledge and vile magic"; }
	virtual uchar Alignment() const { return ACM; }
	virtual uchar BasicAlignment() const { return EVIL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class cruentus : public god
{
public:
	virtual std::string Name() const { return "Cruentus"; }
	virtual std::string Description() const { return "god of war and blood"; }
	virtual uchar Alignment() const { return ACM; }
	virtual void Pray();
	virtual uchar BasicAlignment() const { return EVIL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

class erado : public god
{
public:
	virtual std::string Name() const { return "Erado"; }
	virtual std::string Description() const { return "destroyer of worlds"; }
	virtual uchar Alignment() const { return ACMM; }
	virtual void Pray();
	virtual uchar BasicAlignment() const { return EVIL; }
protected:
	virtual void PrayGoodEffect();
	virtual void PrayBadEffect();
};

#endif
