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
  virtual ushort GetCarryingStrength() const;
  virtual void CalculateBattleInfo();
  void CalculateUnarmedAttackInfo();
  void CalculateKickAttackInfo();
  void CalculateBiteAttackInfo();
  virtual bool IsAlive() const;
  void SetStrength(ushort What) { Strength = What; }
  void SetAgility(ushort What) { Agility = What; }
  virtual void AddSpecialStethoscopeInfo(felist&) const;
  virtual void RaiseStats();
  virtual void LowerStats();
  virtual void AddAttributeInfo(festring&) const;
  virtual void AddAttackInfo(felist&) const;
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
  virtual ushort ReceiveBodyPartDamage(character*, ushort, ushort, uchar, uchar = 8, bool = false, bool = false, bool = true);
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
  virtual const char* FirstPersonBiteHitVerb() const;
  virtual const char* FirstPersonCriticalBiteHitVerb() const;
  virtual const char* ThirdPersonBiteHitVerb() const;
  virtual const char* ThirdPersonCriticalBiteHitVerb() const;
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
  virtual const char* FirstPersonBiteVerb() const;
  virtual const char* FirstPersonCriticalBiteVerb() const;
  virtual const char* ThirdPersonBiteVerb() const;
  virtual const char* ThirdPersonCriticalBiteVerb() const;
  virtual const char* BiteNoun() const;
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
  virtual ushort GetSpecialBodyPartFlags(ushort, bool = false) const;
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
  virtual ushort TakeHit(character*, item*, float, float, short, uchar, bool, bool);
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
  virtual void SetWayPoints(const std::vector<vector2d>&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  std::vector<vector2d> WayPoints;
  ushort NextWayPoint;
);

class CHARACTER
(
  eddy,
  nonhumanoid,
 public:
  virtual bool Hit(character*, bool = false);
  virtual bool IsAnimated() const { return true; }
 protected:
  virtual bodypart* MakeBodyPart(ushort) const;
  virtual void GetAICommand();
);

class CHARACTER
(
  mushroom,
  nonhumanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetSpecies(ushort);
  ushort GetSpecies() const { return Species; }
  virtual bool IsMushroom() const { return true; }
 protected:
  virtual void GetAICommand();
  virtual ushort GetTorsoMainColor() const { return Species; }
  virtual void VirtualConstructor(bool);
  ushort Species;
);

class CHARACTER
(
  magicmushroom,
  mushroom,
 protected:
  virtual void GetAICommand();
);

class CHARACTER
(
  ghost,
  nonhumanoid,
 protected:
  virtual const char* FirstPersonBiteHitVerb() const;
  virtual const char* FirstPersonCriticalBiteHitVerb() const;
  virtual const char* ThirdPersonBiteHitVerb() const;
  virtual const char* ThirdPersonCriticalBiteHitVerb() const;
);

class CHARACTER
(
  twoheadedmoose,
  nonhumanoid,
 public:
  virtual bool Hit(character*, bool = false);
);

class CHARACTER
(
  magpie,
  nonhumanoid,
 public:
  virtual void GetAICommand();
  virtual bool IsRetreating() const;
 protected:
  virtual const char* FirstPersonBiteVerb() const;
  virtual const char* FirstPersonCriticalBiteVerb() const;
  virtual const char* ThirdPersonBiteVerb() const;
  virtual const char* ThirdPersonCriticalBiteVerb() const;
);

class CHARACTER
(
  skunk,
  nonhumanoid,
 public:
  virtual void GetAICommand();
);

#endif
