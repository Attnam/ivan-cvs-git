#ifndef __MATERDE_H__
#define __MATERDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "materba.h"
#include "felibdef.h"

class ABSTRACT_MATERIAL
(
  organicsubstance,
  material,
 public:
  virtual bool Be();
  virtual bool HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  ushort SpoilCounter;
);

class MATERIAL
(
  iron,
  material,
 protected:
  virtual std::string NameStem() const { return "iron"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  valpurium,
  material,
 protected:
  virtual std::string NameStem() const { return "valpurium"; }
);

class MATERIAL
(
  stone,
  material,
 protected:
  virtual std::string NameStem() const { return "stone"; }
);

class MATERIAL
(
  bananaflesh,
  material,
 public:
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "banana flesh"; }
);

class MATERIAL
(
  gravel,
  material,
 protected:
  virtual std::string NameStem() const { return "gravel"; }
);

class MATERIAL
(
  moraine,
  material,
 protected:
  virtual std::string NameStem() const { return "moraine"; }
);

class MATERIAL
(
  schoolfood,
  material,
 public:
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual void AddConsumeEndMessage(character*) const;
 protected:
  virtual std::string NameStem() const { return "school food"; }
);

class MATERIAL
(
  air,
  material,
 protected:
  virtual std::string NameStem() const { return "air"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  wood,
  material,
 protected:
  virtual std::string AdjectiveStem() const { return "wooden"; }
  virtual std::string NameStem() const { return "wood"; }
);

class MATERIAL
(
  flesh,
  material,
 public:
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "flesh"; }
);

class MATERIAL
(
  goblinoidflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "green flesh"; }
);

class MATERIAL
(
  pork,
  flesh,
 protected:
  virtual std::string NameStem() const { return "pork"; }
);

class MATERIAL
(
  beef,
  flesh,
 protected:
  virtual std::string NameStem() const { return "beef"; }
);

class MATERIAL
(
  bone,
  material,
 public:
  virtual void AddConsumeEndMessage(character*) const;
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "bone"; }
);

class MATERIAL
(
  frogflesh,
  flesh,
 public:
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual void AddConsumeEndMessage(character*) const;
 protected:
  virtual std::string NameStem() const { return "frog flesh"; }
);

class MATERIAL
(
  darkfrogflesh,
  frogflesh,
 protected:
  virtual std::string NameStem() const { return "dark frog flesh"; }
);

class MATERIAL
(
  elpuriflesh,
  darkfrogflesh,
 protected:
  virtual std::string NameStem() const { return "Elpuri's flesh"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  glass,
  material,
 protected:
  virtual std::string NameStem() const { return "glass"; }
);

class MATERIAL
(
  omleurine,
  material,
 public:
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual void AddConsumeEndMessage(character*) const;
 protected:
  virtual std::string NameStem() const { return "Omle urine"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  bananapeal,
  material,
 protected:
  virtual std::string NameStem() const { return "banana peal"; }
);

class MATERIAL
(
  parchment,
  material,
 protected:
  virtual std::string NameStem() const { return "parchment"; }
);

class MATERIAL
(
  cloth,
  material,
 protected:
  virtual std::string NameStem() const { return "cloth"; }
);

class MATERIAL
(
  humanflesh,
  flesh,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual std::vector<ushort>& GetSkinColorVector() { return SkinColor; }
 protected:
  virtual std::string NameStem() const { return "human flesh"; }
  std::vector<ushort> SkinColor;
);

class MATERIAL
(
  slime,
  material,
 public:
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "slime"; }
);

class MATERIAL
(
  brownslime,
  slime,
 protected:
  virtual std::string NameStem() const { return "brown slime"; }
);

class MATERIAL
(
  wolfflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "wolf flesh"; }
);

class MATERIAL
(
  dogflesh,
  wolfflesh,
 protected:
  virtual std::string NameStem() const { return "dog flesh"; }
);

class MATERIAL
(
  ennerbeastflesh,
  humanflesh, //???
 protected:
  virtual std::string NameStem() const { return "enner beast flesh"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  pepsi,
  material,
 public:
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual void AddConsumeEndMessage(character*) const;
 protected:
  virtual std::string NameStem() const { return "pepsi"; }
);

class MATERIAL
(
  mithril,
  material,
 protected:
  virtual std::string NameStem() const { return "mithril"; }
);

class MATERIAL
(
  spiderflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "spider flesh"; }
);

class MATERIAL
(
  jackalflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "jackal flesh"; }
);

class MATERIAL
(
  donkeyflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "donkey flesh"; }
);

class MATERIAL
(
  marble,
  material,
 protected:
  virtual std::string NameStem() const { return "marble"; }
);

class MATERIAL
(
  gold,
  material,
 protected:
  virtual std::string AdjectiveStem() const { return "golden"; }
  virtual std::string NameStem() const { return "gold"; }
);

class MATERIAL
(
  grass,
  material,
 protected:
  virtual std::string AdjectiveStem() const { return "grassy"; }
  virtual std::string NameStem() const { return "grass"; }
);

class MATERIAL
(
  kiwiflesh,
  material,
 public:
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "fruit flesh"; }
);

class MATERIAL
(
  pineappleflesh,
  material,
 public:
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "pineapple flesh"; }
);

class MATERIAL
(
  leather,
  material,
 protected:
  virtual std::string NameStem() const { return "leather"; }
);

class MATERIAL
(
  lightfrogflesh,
  frogflesh,
 protected:
  virtual std::string NameStem() const { return "light frog flesh"; }
);

class MATERIAL
(
  dolphinflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "dolphin flesh"; }
);

class MATERIAL
(
  polarbearflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "polar bear flesh"; }
);

class MATERIAL
(
  leaf,
  material,
 protected:
  virtual std::string NameStem() const { return "leaf"; }
);

class MATERIAL
(
  fabric,
  cloth,
 protected:
  virtual std::string NameStem() const { return "fabric"; }
  virtual std::string Article() const { return "an"; }
);

class MATERIAL
(
  water,
  material,
 public:
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
 protected:
  virtual std::string NameStem() const { return "sulfur"; }
);

class MATERIAL
(
  batflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "bat flesh"; }
);

class MATERIAL
(
  werewolfflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "werewolf flesh"; }
);

class MATERIAL
(
  koboldflesh,
  flesh,
 public:
  virtual void EatEffect(character*, float, float);
  virtual void AddConsumeEndMessage(character*) const;
 protected:
  virtual std::string NameStem() const { return "kobold flesh"; }
);

class MATERIAL
(
  gibberlingflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "gibberling flesh"; }
);

class MATERIAL
(
  catflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "cat flesh"; }
);

class MATERIAL
(
  ratflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "rat flesh"; }
);

class MATERIAL
(
  angelflesh,
  humanflesh,
 protected:
  virtual std::string NameStem() const { return "angel flesh"; }
);

class MATERIAL
(
  dwarfflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "dwarf flesh"; }
);

class MATERIAL
(
  gunpowder,
  material,
 public:
  virtual ushort ExplosivePower() const { return GetVolume() / 250; }
 protected:
  virtual std::string NameStem() const { return "gunpowder"; }
);

class MATERIAL
(
  daemonflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "daemon flesh"; }
);

class MATERIAL
(
  blood,
  material, // should be "liquid"
 public:
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "blood"; }
);

class MATERIAL
(
  mammothflesh,
  flesh,
 public:
  /*  virtual ushort GetColor() const { return MAKE_RGB(100, 130, 160); } */
 protected:
  virtual std::string NameStem() const { return "mammoth flesh"; }
);

class MATERIAL
(
  bread,
  material,
 public:
  virtual void EatEffect(character* Eater, float Amount, float NPModifier) { NormalFoodEffect(Eater, Amount, NPModifier); MinusAmount(Amount); }
 protected:
  virtual std::string NameStem() const { return "bread"; }

);

class MATERIAL
(
  unicornhorn,
  material,
 protected:
  virtual std::string NameStem() const { return "unicorn horn"; }
);

class MATERIAL
(
  blackunicornflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "black unicorn flesh"; } 
);

class MATERIAL
(
  grayunicornflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "gray unicorn flesh"; } 
);

class MATERIAL
(
  whiteunicornflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "white unicorn flesh"; } 
);

class MATERIAL
(
  diamond,
  material,
 protected:
  virtual std::string NameStem() const { return "diamond"; }
);

class MATERIAL
(
  silver,
  material,
 protected:
  virtual std::string NameStem() const { return "silver"; }
); 

class MATERIAL
(
  sapphire,
  material,
 protected:
  virtual std::string NameStem() const { return "sapphire"; }
);

class MATERIAL
(
  ruby,
  material,
 protected:
  virtual std::string NameStem() const { return "ruby"; }
);

class MATERIAL
(
  healingliquid,
  material,
 public:
  virtual void EatEffect(character*, float, float);
  virtual void HitEffect(character* Enemy);
  virtual void AddConsumeEndMessage(character*) const;
 protected:
  virtual std::string NameStem() const { return "healing liquid"; }
);

class MATERIAL
(
  bronze,
  material,
 protected:
  virtual std::string NameStem() const { return "bronze"; }
);

class MATERIAL
(
  copper,
  material,
 protected:
  virtual std::string NameStem() const { return "copper"; }
);

class MATERIAL
(
  lionflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "lion flesh"; }
);

class MATERIAL
(
  buffaloflesh,
  flesh,
 protected:
  virtual std::string NameStem() const { return "buffalo flesh"; }
);

#endif
