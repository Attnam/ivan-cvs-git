#ifndef __NONHUMAN_H__
#define __NONHUMAN_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "char.h"

class ABSTRACT_CHARACTER
(
  nonhumanoid,
  character,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void CalculateUnarmedDamage();
  void CalculateKickDamage();
  void CalculateBiteDamage();
  void CalculateUnarmedToHitValue();
  void CalculateKickToHitValue();
  void CalculateBiteToHitValue();
  void CalculateUnarmedAPCost();
  void CalculateKickAPCost();
  void CalculateBiteAPCost();
  float GetUnarmedDamage() const { return UnarmedDamage; }
  ushort GetUnarmedMinDamage() const;
  ushort GetUnarmedMaxDamage() const;
  float GetKickDamage() const { return KickDamage; }
  ushort GetKickMinDamage() const;
  ushort GetKickMaxDamage() const;
  float GetBiteDamage() const { return BiteDamage; }
  ushort GetBiteMinDamage() const;
  ushort GetBiteMaxDamage() const;
  float GetUnarmedToHitValue() const { return UnarmedToHitValue; }
  float GetKickToHitValue() const { return KickToHitValue; }
  float GetBiteToHitValue() const { return BiteToHitValue; }
  long GetUnarmedAPCost() const { return UnarmedAPCost; }
  long GetKickAPCost() const { return KickAPCost; }
  long GetBiteAPCost() const { return BiteAPCost; }
  virtual void Kick(lsquare*, bool = false);
  virtual bool Hit(character*, bool = false);
  virtual void UnarmedHit(character*, bool = false);
  virtual void InitSpecialAttributes();
  virtual float GetTimeToKill(const character*, bool) const;
  virtual void ApplyExperience(bool = false);
  virtual ushort GetAttribute(ushort) const;
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual ushort DrawStats(bool) const;
  virtual void Bite(character*, bool = false);
  virtual void RaiseStats();
  virtual void LowerStats();
  virtual ushort GetCarryingStrength() const;
  virtual void AddAttackInfo(felist&) const;
  virtual void CalculateBattleInfo();
  void CalculateUnarmedAttackInfo();
  void CalculateKickAttackInfo();
  void CalculateBiteAttackInfo();
  virtual bool IsAlive() const;
  virtual void ShowBattleInfo();
  void ShowUnarmedInfo() const;
  void ShowKickInfo() const;
  void ShowBiteInfo() const;
  void SetStrength(ushort What) { Strength = What; }
  void SetAgility(ushort What) { Agility = What; }
  virtual void AddAttributeInfo(std::string&) const;
  virtual void AddSpecialStethoscopeInfo(felist&) const;
 protected:
  ushort Strength;
  ushort Agility;
  long StrengthExperience;
  long AgilityExperience;
  float UnarmedDamage;
  float KickDamage;
  float BiteDamage;
  float UnarmedToHitValue;
  float KickToHitValue;
  float BiteToHitValue;
  long UnarmedAPCost;
  long KickAPCost;
  long BiteAPCost;
);

class CHARACTER
(
  frog,
  nonhumanoid,
 public:
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
);

class CHARACTER
(
  elpuri,
  frog,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool Hit(character*, bool = false);
  virtual ushort ReceiveBodyPartDamage(character*, ushort, uchar, uchar, uchar = 8, bool = false, bool = false, bool = true);
  virtual bool SpecialEnemySightedReaction(character*);
  virtual bool Faint(ushort, bool = false) { return false; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
  bool Active;
);

class CHARACTER
(
  billswill,
  nonhumanoid,
 protected:
  virtual void CreateCorpse(lsquare*) { SendToHell(); }
  virtual std::string FirstPersonUnarmedHitVerb() const;
  virtual std::string FirstPersonCriticalUnarmedHitVerb() const;
  virtual std::string ThirdPersonUnarmedHitVerb() const;
  virtual std::string ThirdPersonCriticalUnarmedHitVerb() const;
  virtual bool AttackIsBlockable(uchar) const { return false; }
);

class CHARACTER
(
  mommo,
  nonhumanoid,
 public:
  virtual bool IsAnimated() const { return true; }
 protected:
  virtual bodypart* MakeBodyPart(ushort) const;
  virtual std::string FirstPersonBiteVerb() const;
  virtual std::string FirstPersonCriticalBiteVerb() const;
  virtual std::string ThirdPersonBiteVerb() const;
  virtual std::string ThirdPersonCriticalBiteVerb() const;
  virtual std::string BiteNoun() const;
  virtual void CreateCorpse(lsquare*);
);

class CHARACTER
(
  wolf,
  nonhumanoid,
 protected:
  virtual ushort GetSkinColor() const;
);

class CHARACTER
(
  dog,
  nonhumanoid,
 public:
  virtual bool Catches(item*);
  virtual void BeTalkedTo();
);

class CHARACTER
(
  spider,
  nonhumanoid,
 public:
  virtual bool SpecialBiteEffect(character*, uchar, uchar, bool);
 protected:
  virtual void CreateCorpse(lsquare*) { SendToHell(); }
);

class CHARACTER
(
  jackal,
  nonhumanoid,
  ;
);

class CHARACTER
(
  ass,
  nonhumanoid,
  ;
);

class CHARACTER
(
  polarbear,
  nonhumanoid,
  ;
);

class CHARACTER
(
  dolphin,
  nonhumanoid,
 protected:
  virtual uchar GetSpecialBodyPartFlags(ushort) const;
  virtual void SpecialTurnHandler() { UpdatePictures(); }
);

class CHARACTER
(
  bat,
  nonhumanoid,
  ;
);

class CHARACTER  
(        
  largecat,
  nonhumanoid,
  ;
);

class CHARACTER  
(        
  largerat,
  nonhumanoid,
  ;
);

class CHARACTER  
(        
  mammoth,
  nonhumanoid,
  ;
);

class CHARACTER
(
  unicorn,
  nonhumanoid,
 public:
  virtual bool SpecialEnemySightedReaction(character*);
);

class CHARACTER
(
  lion,
  nonhumanoid,
  ;
);

class CHARACTER
(
  carnivorousplant,
  nonhumanoid,
 protected:
  virtual ushort GetTorsoSpecialColor() const;
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
);

class CHARACTER
(
  buffalo,
  nonhumanoid,
  ;
);

class CHARACTER
(
  snake,
  nonhumanoid,
 protected:
  virtual bool SpecialBiteEffect(character*, uchar, uchar, bool);
);

class CHARACTER
(
  genetrixvesana,
  carnivorousplant,
 protected:
  virtual ushort GetTorsoSpecialColor() const;
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
);

class CHARACTER
(
  ostrich,
  nonhumanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual bool HandleCharacterBlockingTheWay(character*);
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  bool HasBeenOnLandingSite;
);

class CHARACTER
(
  chameleon,
  nonhumanoid,
 public:
  virtual ushort TakeHit(character*, item*, float, float, short, uchar, bool, bool);
  virtual bool SpecialEnemySightedReaction(character*);
 protected:
  virtual ushort GetSkinColor() const;
  virtual void SpecialTurnHandler() { UpdatePictures(); }
);

class CHARACTER  
(
  floatingeye,
  nonhumanoid,
 public:
  virtual bool Hit(character*, bool);
  virtual ushort TakeHit(character*, item*, float, float, short, uchar, bool, bool);
 protected:
  virtual void GetAICommand();
);

class CHARACTER
(
  eddy,
  nonhumanoid,
 public:
  virtual bool Hit(character*, bool = false);
  virtual void CreateCorpse(lsquare*) { SendToHell(); }
  virtual bool IsAnimated() const { return true; }
 protected:
  virtual bodypart* MakeBodyPart(ushort) const;
);

class CHARACTER
(
  mushroom,
  nonhumanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetSpecies(ushort);
  virtual ushort GetSpecies() const { return Species; }
 protected:
  virtual void GetAICommand();
  virtual mushroom* GetChildMushroom() const;
  virtual ushort GetTorsoMainColor() const;
  virtual void VirtualConstructor(bool);
  virtual void Reproduce();
  ushort Species;
);

class CHARACTER
(
  mutatedmushroom,
  mushroom,
 protected:
  virtual void GetAICommand();
  virtual mushroom* GetChildMushroom() const;
);

class CHARACTER
(
  ghost,
  nonhumanoid,
 protected:
  virtual void GetAICommand();
);

#endif
