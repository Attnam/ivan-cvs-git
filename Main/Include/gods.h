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
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual void Pray();
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  legifer,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  atavus,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  dulcis,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  seges,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  sophos,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  silva,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  loricatus,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  mellis,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  cleptia,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  nefas,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  scabies,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual bool PlayerVomitedOnAltar(liquid*);
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  infuscor,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  cruentus,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  mortifer,
  god,
 public:
  virtual const char* GetName() const;
  virtual const char* GetDescription() const;
  virtual int GetAlignment() const;
  virtual void Pray();
  virtual int GetBasicAlignment() const;
  virtual color16 GetColor() const;
  virtual color16 GetEliteColor() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

#endif
