#ifndef __GODDE_H__
#define __GODDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "godba.h"
#include "felibdef.h"

class GOD
(
  valpurus,
  god,
 public:
  virtual std::string Name() const { return "Valpurus"; }
  virtual std::string Description() const { return "King of Gods"; }
  virtual uchar Alignment() const { return ALPP; }
  virtual void Pray();
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual std::string GetPriestMessage() const;
  virtual ushort GetColor() const { return MakeRGB16(160, 160, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  legifer,
  god,
 public:
  virtual std::string Name() const { return "Legifer"; }
  virtual std::string Description() const { return "god of law and order"; }
  virtual uchar Alignment() const { return ALP; }
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual std::string GetPriestMessage() const;
  virtual ushort GetColor() const { return MakeRGB16(160, 160, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  atavus,
  god,
 public:
  virtual std::string Name() const { return "Atavus"; }
  virtual std::string Description() const { return "god of charity and munificence"; }
  virtual uchar Alignment() const { return ALP; }
  virtual void Pray();
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual ushort GetColor() const { return MakeRGB16(160, 160, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  dulcis,
  god,
 public:
  virtual std::string Name() const { return "Dulcis"; }
  virtual std::string Description() const { return "goddess of love and art"; }
  virtual uchar Alignment() const { return AL; }
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual ushort GetColor() const { return MakeRGB16(160, 160, 0); }
  virtual std::string GetPriestMessage() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  seges,
  god,
 public:
  virtual std::string Name() const { return "Seges"; }
  virtual std::string Description() const { return "goddess of health and nutrition"; }
  virtual uchar Alignment() const { return AL; }
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual std::string GetPriestMessage() const;
  virtual ushort GetColor() const { return MakeRGB16(160, 160, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  sophos,
  god,
 public:
  virtual std::string Name() const { return "Sophos"; }
  virtual std::string Description() const { return "god of knowledge and magic"; }
  virtual uchar Alignment() const { return ALM; }
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual std::string GetPriestMessage() const;
  virtual ushort GetColor() const { return MakeRGB16(160, 160, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  silva,
  god,
 public:
  virtual std::string Name() const { return "Silva"; }
  virtual std::string Description() const { return "goddess of nature"; }
  virtual uchar Alignment() const { return ANP; }
  virtual uchar BasicAlignment() const { return NEUTRAL; }
  virtual ushort GetColor() const { return MakeRGB16(20, 120, 200); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  loricatus,
  god,
 public:
  virtual std::string Name() const { return "Loricatus"; }
  virtual std::string Description() const { return "god of fire, machines and weaponry"; }
  virtual uchar Alignment() const { return AN; }
  virtual uchar BasicAlignment() const { return NEUTRAL; }
  virtual ushort GetColor() const { return MakeRGB16(20, 120, 200); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  mellis,
  god,
 public:
  virtual std::string Name() const { return "Mellis"; }
  virtual std::string Description() const { return "god of money, trade and politics"; }
  virtual uchar Alignment() const { return ANM; }
  virtual uchar BasicAlignment() const { return NEUTRAL; }
  virtual ushort GetColor() const { return MakeRGB16(20, 120, 200); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  cleptia,
  god,
 public:
  virtual std::string Name() const { return "Cleptia"; }
  virtual std::string Description() const { return "goddess of assassins and thieves"; }
  virtual uchar Alignment() const { return ACP; }
  virtual uchar BasicAlignment() const { return EVIL; }
  virtual ushort GetColor() const { return MakeRGB16(200, 0, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  nefas,
  god,
 public:
  virtual std::string Name() const { return "Nefas"; }
  virtual std::string Description() const { return "goddess of forbidden pleasures"; }
  virtual uchar Alignment() const { return AC; }
  virtual uchar BasicAlignment() const { return EVIL; }
  virtual ushort GetColor() const { return MakeRGB16(200, 0, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  scabies,
  god,
 public:
  virtual std::string Name() const { return "Scabies"; }
  virtual std::string Description() const { return "goddess of mutations, disease and famine"; }
  virtual uchar Alignment() const { return AC; }
  virtual uchar BasicAlignment() const { return EVIL; }
  virtual void PlayerVomitedOnAltar();
  virtual ushort GetColor() const { return MakeRGB16(200, 0, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  infuscor,
  god,
 public:
  virtual std::string Name() const { return "Infuscor"; }
  virtual std::string Description() const { return "goddess of wrong knowledge and vile magic"; }
  virtual uchar Alignment() const { return ACM; }
  virtual uchar BasicAlignment() const { return EVIL; }
  virtual ushort GetColor() const { return MakeRGB16(200, 0, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  cruentus,
  god,
 public:
  virtual std::string Name() const { return "Cruentus"; }
  virtual std::string Description() const { return "god of war and blood"; }
  virtual uchar Alignment() const { return ACM; }
  virtual void Pray();
  virtual uchar BasicAlignment() const { return EVIL; }
  virtual ushort GetColor() const { return MakeRGB16(200, 0, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  mortifer,
  god,
 public:
  virtual std::string Name() const { return "Mortifer"; }
  virtual std::string Description() const { return "Destroyer of Worlds"; }
  virtual uchar Alignment() const { return ACMM; }
  virtual void Pray();
  virtual uchar BasicAlignment() const { return EVIL; }
  virtual ushort GetColor() const { return MakeRGB16(200, 0, 0); }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

#endif

