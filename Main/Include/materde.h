#ifndef __MATERDE_H__
#define __MATERDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "materba.h"
#include "graphics.h"

class MATERIAL
(
  iron,
  material,
 public:
  virtual ushort GetHitValue() const { return 100; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 8000; }
  virtual ushort OfferValue() const { return 10; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(56, 56, 56); }
  virtual ulong RawPrice() const { return GetVolume() >> 8; }
  virtual bool CanBeDigged() const { return false; }
 protected:
  virtual std::string NameStem() const { return "iron"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  valpurium,
  material,
 public:
  virtual ushort GetHitValue() const { return 400; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 3000; }
  virtual ushort OfferValue() const { return 100; }
  virtual uchar Alignment() const { return GOOD; }
  virtual ushort GetEmitation() const { return 320; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(192, 192, 192); }
  virtual bool CanBeWished() const { return false; }
  virtual ulong RawPrice() const { return GetVolume() << 2; }
  virtual bool CanBeDigged() const { return false; }
 protected:
  virtual std::string NameStem() const { return "valpurium"; }
);

class MATERIAL
(
  stone,
  material,
 public:
  virtual ushort GetHitValue() const { return 60; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 3000; }
  virtual ushort OfferValue() const { return 5; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(56, 56, 56); }
 protected:
  virtual std::string NameStem() const { return "stone"; }
);

class MATERIAL
(
  bananaflesh,
  material,
 public:
  virtual ushort GetHitValue() const { return 5; }
  virtual uchar GetConsumeType() const { return FRUIT; }
  virtual ushort GetDensity() const { return 1200; }
  virtual ushort OfferValue() const { return 10; }
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
  virtual short NutritionValue() const { return 200; }
  virtual ushort GetColor() const { return MAKE_RGB(200, 200, 0); }
  virtual ulong RawPrice() const { return GetVolume() >> 4; }
 protected:
  virtual std::string NameStem() const { return "banana flesh"; }
);

class MATERIAL
(
  gravel,
  material,
 public:
  virtual ushort GetHitValue() const { return 40; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 2500; }
  virtual ushort OfferValue() const { return 1; }
  virtual ushort GetColor() const { return MAKE_RGB(72, 72, 72); }
 protected:
  virtual std::string NameStem() const { return "gravel"; }
);

class MATERIAL
(
  moraine,
  material,
 public:
  virtual ushort GetHitValue() const { return 20; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 2500; }
  virtual ushort OfferValue() const { return 2; }
  virtual ushort GetColor() const { return MAKE_RGB(111, 74, 37); }
 protected:
  virtual std::string NameStem() const { return "moraine"; }
);

class MATERIAL
(
  schoolfood,
  material,
 public:
  virtual ushort GetHitValue() const { return 5; }
  virtual uchar GetConsumeType() const { return SCHOOLFOOD; }
  virtual ushort GetDensity() const { return 1500; }
  virtual ushort OfferValue() const { return 20; }
  virtual uchar Alignment() const { return EVIL; }
  virtual short NutritionValue() const { return 100; }
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual ushort GetColor() const { return MAKE_RGB(111, 74, 37); }
  virtual bool GetIsBadFoodForAI() const { return true; }
 protected:
  virtual std::string NameStem() const { return "school food"; }
);

class MATERIAL
(
  air,
  material,
 public:
  virtual ushort GetHitValue() const { return 0; }
  virtual ushort GetDensity() const { return 1; }
  virtual ushort OfferValue() const { return 0; }
  virtual ushort GetColor() const { return MAKE_RGB(112, 112, 200); }
 protected:
  virtual std::string NameStem() const { return "air"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  wood,
  material,
 public:
  virtual ushort GetHitValue() const { return 30; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 500; }
  virtual ushort OfferValue() const { return 5; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(140, 100, 60); }
 protected:
  virtual std::string NameStem() const { return "wood"; }
);

class MATERIAL
(
  flesh,
  material,
 public:

  virtual ushort GetHitValue() const { return 10; }
  virtual uchar GetConsumeType() const { return MEAT; }
  virtual ushort GetDensity() const { return 1200; }
  virtual ushort OfferValue() const { return 15; }
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
  virtual short NutritionValue() const { return 200; }
  virtual ushort GetColor() const { return MAKE_RGB(64, 64, 64); }
 protected:
  virtual std::string NameStem() const { return "flesh"; }
);

class MATERIAL
(
  goblinoidflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 10; }
  virtual ushort GetColor() const { return MAKE_RGB(0, 128, 0); }
  virtual ulong RawPrice() const { return GetVolume() >> 4; }
 protected:
  virtual std::string NameStem() const { return "green flesh"; }
);

class MATERIAL
(
  pork,
  flesh,
 public:
  virtual ushort OfferValue() const { return 20; }
  virtual short NutritionValue() const { return 400; }
  virtual ushort GetColor() const { return MAKE_RGB(128, 128, 128); }
  virtual ulong RawPrice() const { return GetVolume() >> 3; }
 protected:
  virtual std::string NameStem() const { return "pork"; }
);

class MATERIAL
(
  beef,
  flesh,
 public:
  virtual ushort OfferValue() const { return 20; }
  virtual short NutritionValue() const { return 400; }
  virtual ushort GetColor() const { return MAKE_RGB(128, 128, 128); }
  virtual ulong RawPrice() const { return GetVolume() >> 3; }
 protected:
  virtual std::string NameStem() const { return "beef"; }
);

class MATERIAL
(
  bone,
  material,
 public:
  virtual ushort GetHitValue() const { return 30; }
  virtual ushort GetDensity() const { return 2000; }
  virtual ushort OfferValue() const { return 5; }
  virtual uchar GetConsumeType() const { return BONE; }
  virtual void EatEffect(character*, float, float);
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(144, 144, 144); }
  virtual short NutritionValue() const { return 20; }
 protected:
  virtual std::string NameStem() const { return "bone"; }
);

class MATERIAL
(
  frogflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 500; }
  virtual uchar Alignment() const { return EVIL; }
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual short NutritionValue() const { return 1000; }
  virtual bool GetIsBadFoodForAI() const { return true; }
 protected:
  virtual std::string NameStem() const { return "frog flesh"; }
);

class MATERIAL
(
  darkfrogflesh,
  frogflesh,
 public:
  virtual ushort GetColor() const { return MAKE_RGB(48, 48, 48); }
 protected:
  virtual std::string NameStem() const { return "dark frog flesh"; }
);

class MATERIAL
(
  elpuriflesh,
  darkfrogflesh,
 public:
  virtual ushort GetHitValue() const { return 30; }
  virtual ushort GetDensity() const { return 2400; }
  virtual ushort OfferValue() const { return 1; }
 protected:
  virtual std::string NameStem() const { return "Elpuri's flesh"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  glass,
  material,
 public:
  virtual ushort GetHitValue() const { return 30; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 2500; }
  virtual ushort OfferValue() const { return 5; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(128, 128, 255); }
 protected:
  virtual std::string NameStem() const { return "glass"; }
);

class MATERIAL
(
  omleurine,
  material,
 public:
  virtual ushort GetHitValue() const { return 1; }
  virtual uchar GetConsumeType() const { return LIQUID; }
  virtual ushort GetDensity() const { return 1000; }
  virtual ushort OfferValue() const { return 100; }
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual short NutritionValue() const { return 30; }
  virtual ushort GetColor() const { return MAKE_RGB(128, 128, 0); }
  virtual bool CanBeWished() const { return false; }
  virtual ulong RawPrice() const { return GetVolume() >> 2; }
 protected:
  virtual std::string NameStem() const { return "Omle urine"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  bananapeal,
  material,
 public:
  virtual ushort GetHitValue() const { return 10; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 500; }
  virtual ushort OfferValue() const { return 1; }
  virtual ushort GetColor() const { return MAKE_RGB(176, 176, 0); }
 protected:
  virtual std::string NameStem() const { return "banana peal"; }
);

class MATERIAL
(
  parchment,
  material,
 public:
  virtual ushort GetHitValue() const { return 15; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 600; }
  virtual ushort OfferValue() const { return 5; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(176, 176, 176); }
  virtual bool IsFlammable() const { return true; }
 protected:
  virtual std::string NameStem() const { return "parchment"; }
);

class MATERIAL
(
  cloth,
  material,
 public:
  virtual ushort GetHitValue() const { return 5; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 100; }
  virtual ushort OfferValue() const { return 5; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(64, 64, 160); }
  virtual bool IsFlammable() const { return true; }
 protected:
  virtual std::string NameStem() const { return "cloth"; }
);

class MATERIAL
(
  humanflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 30; }
  virtual uchar Alignment() const { return EVIL; }
  virtual ushort GetColor() const { return MAKE_RGB(180, 120, 90); }
 protected:
  virtual std::string NameStem() const { return "human flesh"; }
);

class MATERIAL
(
  slime,
  material,
 public:
  virtual ushort GetHitValue() const { return 2; }
  virtual uchar GetConsumeType() const { return MEAT; }
  virtual ushort GetDensity() const { return 400; }
  virtual ushort OfferValue() const { return 1; }
  virtual short NutritionValue() const { return 100; }
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
  virtual ushort GetColor() const { return MAKE_RGB(0, 128, 0); }
 protected:
  virtual std::string NameStem() const { return "slime"; }
);

class MATERIAL
(
  brownslime,
  slime,
 public:
  virtual uchar GetConsumeType() const { return MEAT; }
  virtual ushort OfferValue() const { return 1; }
  virtual ushort GetColor() const { return MAKE_RGB(111, 74, 37); }
 protected:
  virtual std::string NameStem() const { return "brown slime"; }
);

class MATERIAL
(
  wolfflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 15; }
  virtual ushort GetColor() const { return MAKE_RGB(64, 64, 64); }
  virtual ulong RawPrice() const { return GetVolume() >> 4; }
 protected:
  virtual std::string NameStem() const { return "wolf flesh"; }
);

class MATERIAL
(
  dogflesh,
  wolfflesh,
 public:
  virtual ushort OfferValue() const { return 15; }
  virtual uchar Alignment() const { return EVIL; }
  virtual ushort GetColor() const { return MAKE_RGB(111, 74, 37); }
 protected:
  virtual std::string NameStem() const { return "dog flesh"; }
);

class MATERIAL
(
  ennerbeastflesh,
  humanflesh, //???
 public:
  virtual ushort GetHitValue() const { return 30; }
  virtual ushort GetDensity() const { return 1800; }
  virtual ushort OfferValue() const { return 5; }
  virtual ushort GetColor() const { return MAKE_RGB(112, 112, 64); }
  virtual bool GetIsBadFoodForAI() const { return true; }
 protected:
  virtual std::string NameStem() const { return "Enner Beast flesh"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  pepsi,
  material,
 public:
  virtual ushort GetHitValue() const { return 200; }
  virtual uchar GetConsumeType() const { return LIQUID; }
  virtual ushort GetDensity() const { return 1500; }
  virtual ushort OfferValue() const { return 50; }
  virtual uchar Alignment() const { return EVIL; }
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual short NutritionValue() const { return 400; }
  virtual ushort GetColor() const { return MAKE_RGB(48, 48, 48); }
  virtual bool GetIsBadFoodForAI() const { return true; }
 protected:
  virtual std::string NameStem() const { return "pepsi"; }
);

class MATERIAL
(
  mithril,
  material,
 public:
  virtual ushort GetHitValue() const { return 200; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 5000; }
  virtual ushort OfferValue() const { return 25; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(176, 176, 176); }
  virtual ulong RawPrice() const { return GetVolume() >> 1; }
  virtual bool CanBeDigged() const { return false; }
 protected:
  virtual std::string NameStem() const { return "mithril"; }
);

class MATERIAL
(
  spiderflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 4; }
  virtual ushort GetColor() const { return MAKE_RGB(160, 160, 160); }
 protected:
  virtual std::string NameStem() const { return "spider flesh"; }
);

class MATERIAL
(
  jackalflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 6; }
  virtual short NutritionValue() const { return 200; }
  virtual ushort GetColor() const { return MAKE_RGB(224, 224, 224); }
 protected:
  virtual std::string NameStem() const { return "jackal flesh"; }
);

class MATERIAL
(
  donkeyflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 6; }
  virtual ushort GetColor() const { return MAKE_RGB(80, 80, 80); }
  virtual short NutritionValue() const { return 200; }
 protected:
  virtual std::string NameStem() const { return "donkey flesh"; }
);

class MATERIAL
(
  marble,
  material,
 public:
  virtual ushort GetHitValue() const { return 50; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 3000; }
  virtual ushort OfferValue() const { return 7; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(160, 160, 160); }
 protected:
  virtual std::string NameStem() const { return "marble"; }
);

class MATERIAL
(
  gold,
  material,
 public:
  virtual ushort GetHitValue() const { return 30; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 20000; }
  virtual ushort OfferValue() const { return 50; }
  virtual uchar Alignment() const { return NEUTRAL; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(200, 200, 0); }
  virtual bool CanBeWished() const { return false; }
  virtual ulong RawPrice() const { return GetVolume() << 1; }
 protected:
  virtual std::string NameStem() const { return "gold"; }
);

class MATERIAL
(
  grass,
  material,
 public:
  virtual ushort GetHitValue() const { return 2; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 100; }
  virtual ushort OfferValue() const { return 2; }
  virtual ushort GetColor() const { return MAKE_RGB(32, 96, 32); }
  virtual bool IsFlammable() const { return true; }
 protected:
  virtual std::string NameStem() const { return "grass"; }
);

class MATERIAL
(
  kiwiflesh,
  material,
 public:
  virtual ushort GetHitValue() const { return 5; }
  virtual uchar GetConsumeType() const { return FRUIT; }
  virtual ushort GetDensity() const { return 1200; }
  virtual ushort OfferValue() const { return 11; }
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
  virtual short NutritionValue() const { return 150; }
  virtual ushort GetColor() const { return MAKE_RGB(160, 112, 32); }
 protected:
  virtual std::string NameStem() const { return "fruit flesh"; }
);

class MATERIAL
(
  pineappleflesh,
  material,
 public:
  virtual ushort GetHitValue() const { return 10; }
  virtual uchar GetConsumeType() const { return FRUIT; }
  virtual ushort GetDensity() const { return 1200; }
  virtual ushort OfferValue() const { return 12; }
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
  virtual short NutritionValue() const { return 150; }
  virtual ushort GetColor() const { return MAKE_RGB(192, 112, 96); }
  virtual ulong RawPrice() const { return GetVolume() >> 4; }
 protected:
  virtual std::string NameStem() const { return "pineapple flesh"; }
);

class MATERIAL
(
  leather,
  material,
 public:
  virtual ushort GetHitValue() const { return 10; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 500; }
  virtual ushort OfferValue() const { return 10; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(111, 74, 37); }
  virtual bool IsFlexible() const { return true; }
  virtual bool IsFlammable() const { return true; }
 protected:
  virtual std::string NameStem() const { return "leather"; }
);

class MATERIAL
(
  lightfrogflesh,
  frogflesh,
 public:
  virtual ushort GetColor() const { return MAKE_RGB(32, 88, 32); }
 protected:
  virtual std::string NameStem() const { return "light frog flesh"; }
);

class MATERIAL
(
  dolphinflesh,
  flesh,
 public:
  virtual ushort GetColor() const { return MAKE_RGB(64, 64, 64); }
 protected:
  virtual std::string NameStem() const { return "dolphin flesh"; }
);

class MATERIAL
(
  polarbearflesh,
  flesh,
 public:
  virtual ushort GetColor() const { return MAKE_RGB(128, 128, 128); }
  virtual ulong RawPrice() const { return GetVolume() >> 4; }
 protected:
  virtual std::string NameStem() const { return "polar bear flesh"; }
);

class MATERIAL
(
  leaf,
  material,
 public:
  virtual ushort GetHitValue() const { return 3; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 500; }
  virtual ushort OfferValue() const { return 10; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(0, 160, 0); }
  virtual bool IsFlammable() const { return true; }
 protected:
  virtual std::string NameStem() const { return "leaf"; }
);

class MATERIAL
(
  expensivefabric,
  cloth,
 public:
  virtual ushort GetColor() const { return MAKE_RGB(176, 0, 0); }
  virtual bool IsFlammable() const { return true; }
 protected:
  virtual std::string NameStem() const { return "expensive fabric"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  water,
  material,
 public:
  virtual ushort GetHitValue() const { return 1; }
  virtual uchar GetConsumeType() const { return LIQUID; }
  virtual ushort GetDensity() const { return 1000; }
  virtual ushort OfferValue() const { return 1; }
  virtual short NutritionValue() const { return 10; }
  virtual ushort GetColor() const { return MAKE_RGB(64, 64, 192); }
  virtual bool CanBeWished() const { return false; }
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "water"; }
);

class MATERIAL
(
  palmleaf,
  leaf,
 protected:
  virtual std::string NameStem() const { return "palm leaf"; }
);

class MATERIAL
(
  sulfur,
  material,
 public:
  virtual ushort GetHitValue() const { return 50; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 2000; }
  virtual ushort OfferValue() const { return 10; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(128, 0, 0); }
 protected:
  virtual std::string NameStem() const { return "sulfur"; }
);

class MATERIAL
(
  batflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 7; }
  virtual ushort GetColor() const { return MAKE_RGB(48, 48, 48); }
  virtual short NutritionValue() const { return 200; }
 protected:
  virtual std::string NameStem() const { return "bat flesh"; }
);

class MATERIAL
(
  werewolfflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 13; }
  virtual ushort GetColor() const { return MAKE_RGB(90, 64, 64); }
 protected:
  virtual std::string NameStem() const { return "werewolf flesh"; }
);

class MATERIAL
(
  koboldflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 14; }
  virtual ushort GetColor() const { return MAKE_RGB(40, 120, 120); }
  virtual short NutritionValue() const { return 100; }
  virtual void EatEffect(character*, float, float);
  virtual bool GetIsBadFoodForAI() const { return true; }
 protected:
  virtual std::string NameStem() const { return "kobold flesh"; }
);

class MATERIAL
(
  gibberlingflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 12; }
  virtual ushort GetColor() const { return MAKE_RGB(100, 100, 200); }
 protected:
  virtual std::string NameStem() const { return "gibberling flesh"; }
);

class MATERIAL
(
  catflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 12; }
  virtual ushort GetColor() const { return MAKE_RGB(50, 50, 50); }
 protected:
  virtual std::string NameStem() const { return "cat flesh"; }
);

class MATERIAL
(
  ratflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 8; }
  virtual ushort GetColor() const { return MAKE_RGB(180, 100, 40); }
 protected:
  virtual std::string NameStem() const { return "rat flesh"; }
);

class MATERIAL
(
  angelflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 8; }
  virtual ushort GetColor() const { return MAKE_RGB(200, 200, 200); }
 protected:
  virtual std::string NameStem() const { return "angel flesh"; }
);

class MATERIAL
(
  goodleather,
  leather,
 public:
  virtual bool IsSolid() const { return false; }
  virtual ushort GetColor() const { return MAKE_RGB(160, 160, 0); }
  virtual bool CanBeWished() const { return false; }
 protected:
  virtual std::string NameStem() const { return "good leather"; }
);

class MATERIAL
(
  neutralleather,
  leather,
 public:
  virtual bool IsSolid() const { return false; }
  virtual ushort GetColor() const { return MAKE_RGB(20, 120, 200); }
  virtual bool CanBeWished() const { return false; }
 protected:
  virtual std::string NameStem() const { return "neutral leather"; }
);

class MATERIAL
(
  evilleather,
  leather,
 public:
  virtual bool IsSolid() const { return false; }
  virtual ushort GetColor() const { return MAKE_RGB(200, 0, 0); }
  virtual bool CanBeWished() const { return false; }
 protected:
  virtual std::string NameStem() const { return "evil leather"; }
);

class MATERIAL
(
  dwarfflesh,
  flesh,
 public:
  virtual ushort GetColor() const { return MAKE_RGB(180, 120, 90); }
 protected:
  virtual std::string NameStem() const { return "dwarf flesh"; }
);

class MATERIAL
(
  gunpowder,
  material,
 public:
  virtual ushort GetHitValue() const { return 5; }
  virtual uchar GetConsumeType() const { return ODD; }
  virtual ushort GetDensity() const { return 250; }
  virtual ushort OfferValue() const { return 5; }
  virtual uchar Alignment() const { return NEUTRAL; }
  virtual ushort GetColor() const { return MAKE_RGB(30, 30, 30); }
  virtual bool CanBeWished() const { return false; }
  virtual bool IsExplosive() const { return true; }
  virtual ushort ExplosivePower() const { return GetVolume() / 250; }
 protected:
  virtual std::string NameStem() const { return "gunpowder"; }
);

class MATERIAL
(
  daemonflesh,
  flesh,
 public:
  virtual ushort GetColor() const { return MAKE_RGB(180, 0, 0); }
 protected:
  virtual std::string NameStem() const { return "daemon flesh"; }
);

class MATERIAL
(
  blood,
  material, // should be "liquid"
 public:
  virtual ushort GetHitValue() const { return 200; }
  virtual uchar GetConsumeType() const { return LIQUID; }
  virtual ushort GetDensity() const { return 1000; }
  virtual ushort OfferValue() const { return 50; }
  virtual short NutritionValue() const { return 10; }
  virtual ushort GetColor() const { return MAKE_RGB(160, 0, 0); }
  virtual bool CanBeWished() const { return false; }
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
  virtual uchar Alignment() const { return EVIL; }
 protected:
  virtual std::string NameStem() const { return "blood"; }
);

class MATERIAL
(
  mammothflesh,
  flesh,
 public:
  virtual ushort GetColor() const { return MAKE_RGB(100, 130, 160); }
 protected:
  virtual std::string NameStem() const { return "mammoth flesh"; }
);

class MATERIAL
(
  bread,
  material,
 public:
  virtual ushort GetHitValue() const { return 10; }
  virtual uchar GetConsumeType() const { return MEAT; }
  virtual ushort GetDensity() const { return 200; }
  virtual ushort OfferValue() const { return 7; }
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
  virtual short NutritionValue() const { return 200; }
  virtual ushort GetColor() const { return MAKE_RGB(170, 110, 60); }
  virtual ulong RawPrice() const { return GetVolume() >> 4; }
 protected:
  virtual std::string NameStem() const { return "bread"; }

);

class MATERIAL
(
  unicornhorn,
  material,
 public:
  virtual ushort GetHitValue() const { return 125; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 2000; }
  virtual ushort OfferValue() const { return 10; }
  virtual ushort GetColor() const { return MAKE_RGB(180, 180, 180); }
  virtual ulong RawPrice() const { return GetVolume() >> 8; }
 protected:
  virtual std::string NameStem() const { return "unicorn horn"; }
);

class MATERIAL
(
  blackunicornflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 35; }
  virtual ushort GetColor() const { return MAKE_RGB(48, 48, 48); }
  virtual uchar Alignment() const { return GOOD; } // this means that good gods like sacrifises made of it
 protected:
  virtual std::string NameStem() const { return "black unicorn flesh"; } 
);

class MATERIAL
(
  grayunicornflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 35; }
  virtual ushort GetColor() const { return MAKE_RGB(112, 112, 112); }
 protected:
  virtual std::string NameStem() const { return "gray unicorn flesh"; } 
);

class MATERIAL
(
  whiteunicornflesh,
  flesh,
 public:
  virtual ushort OfferValue() const { return 35; }
  virtual ushort GetColor() const { return MAKE_RGB(160, 160, 160); }
  virtual uchar Alignment() const { return EVIL; }
 protected:
  virtual std::string NameStem() const { return "white unicorn flesh"; } 
);

class MATERIAL
(
  diamond,
  material,
 public:
  virtual ushort GetHitValue() const { return 300; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 3500; }
  virtual ushort OfferValue() const { return 50; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(180, 180, 180); }
  virtual ulong RawPrice() const { return ulong(GetVolume() * 3); }
 protected:
  virtual std::string NameStem() const { return "diamond"; }
);

class MATERIAL
(
 silver,
 material,
 public:
  virtual ushort GetHitValue() const { return 50; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 10500; }
  virtual ushort OfferValue() const { return 30; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(100, 100, 100); }
  virtual bool CanBeWished() const { return false; }
  virtual ulong RawPrice() const { return ulong(GetVolume() * 1.5); }
 protected:
  virtual std::string NameStem() const { return "silver"; }
); 

class MATERIAL
(
 sapphire,
 material,
 public:
  virtual ushort GetHitValue() const { return 300; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 3500; }
  virtual ushort OfferValue() const { return 50; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(120, 120, 170); }
  virtual ulong RawPrice() const { return ulong(GetVolume() * 2); }
 protected:
  virtual std::string NameStem() const { return "sapphire"; }
 );

class MATERIAL
(
 ruby,
 material,
 public:
  virtual ushort GetHitValue() const { return 300; }
  virtual uchar GetConsumeType() const { return HARD; }
  virtual ushort GetDensity() const { return 3500; }
  virtual ushort OfferValue() const { return 50; }
  virtual bool IsSolid() const { return true; }
  virtual ushort GetColor() const { return MAKE_RGB(200, 0, 0); }
  virtual ulong RawPrice() const { return ulong(GetVolume() * 2.5); }
 protected:
  virtual std::string NameStem() const { return "ruby"; }
 );
 
#endif

