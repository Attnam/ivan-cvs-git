#ifndef __GODS_H__
#define __GODS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "god.h"

class GOD
(
  valpurus,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual void Pray();
  virtual uchar GetBasicAlignment() const;
  virtual std::string GetPriestMessage() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  legifer,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual std::string GetPriestMessage() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  atavus,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual void Pray();
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  dulcis,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
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
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual std::string GetPriestMessage() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  sophos,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual std::string GetPriestMessage() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  silva,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  loricatus,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  mellis,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  cleptia,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  nefas,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  scabies,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual void PlayerVomitedOnAltar();
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  infuscor,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  cruentus,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  mortifer,
  god,
 public:
  virtual std::string GetName() const;
  virtual std::string GetDescription() const;
  virtual uchar GetAlignment() const;
  virtual void Pray();
  virtual uchar GetBasicAlignment() const;
  virtual ushort GetColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

#endif
