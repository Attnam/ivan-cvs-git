/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

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
  double GetUnarmedDamage() const { return UnarmedDamage; }
  int GetUnarmedMinDamage() const;
  int GetUnarmedMaxDamage() const;
  double GetKickDamage() const { return KickDamage; }
  int GetKickMinDamage() const;
  int GetKickMaxDamage() const;
  double GetBiteDamage() const { return BiteDamage; }
  int GetBiteMinDamage() const;
  int GetBiteMaxDamage() const;
  double GetUnarmedToHitValue() const { return UnarmedToHitValue; }
  double GetKickToHitValue() const { return KickToHitValue; }
  double GetBiteToHitValue() const { return BiteToHitValue; }
  long GetUnarmedAPCost() const { return UnarmedAPCost; }
  long GetKickAPCost() const { return KickAPCost; }
  long GetBiteAPCost() const { return BiteAPCost; }
  virtual void Kick(lsquare*, int, bool = false);
  virtual bool Hit(character*, vector2d, int, bool = false);
  virtual void UnarmedHit(character*, vector2d, int, bool = false);
  virtual void InitSpecialAttributes();
  virtual double GetTimeToKill(const character*, bool) const;
  virtual int GetAttribute(int, bool = true) const;
  virtual bool EditAttribute(int, int);
  virtual void EditExperience(int, double, double);
  virtual int DrawStats(bool) const;
  virtual void Bite(character*, vector2d, int, bool = false);
  virtual int GetCarryingStrength() const;
  virtual void CalculateBattleInfo();
  void CalculateUnarmedAttackInfo();
  void CalculateKickAttackInfo();
  void CalculateBiteAttackInfo();
  virtual bool UseMaterialAttributes() const;
  /*void SetStrength(int What) { Strength = What; }
  void SetAgility(int What) { Agility = What; }*/
  virtual void AddSpecialStethoscopeInfo(felist&) const;
  virtual bool EditAllAttributes(int);
  virtual void AddAttributeInfo(festring&) const;
  virtual void AddAttackInfo(felist&) const;
 protected:
  /*int Strength;
  int Agility;*/
  double StrengthExperience;
  double AgilityExperience;
  double UnarmedDamage;
  double KickDamage;
  double BiteDamage;
  double UnarmedToHitValue;
  double KickToHitValue;
  double BiteToHitValue;
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
  billswill,
  nonhumanoid,
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual const char* FirstPersonBiteVerb() const;
  virtual const char* FirstPersonCriticalBiteVerb() const;
  virtual const char* ThirdPersonBiteVerb() const;
  virtual const char* ThirdPersonCriticalBiteVerb() const;
  virtual bool AttackIsBlockable(int) const { return false; }
  virtual bool AttackMayDamageArmor() const { return false; }
);

class CHARACTER
(
  mommo,
  nonhumanoid,
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual bool CanVomit() const { return true; }
  virtual void CreateCorpse(lsquare*);
  virtual bool Hit(character*, vector2d, int, bool);
  virtual void GetAICommand();
);

class CHARACTER
(
  wolf,
  nonhumanoid,
 protected:
  virtual color16 GetSkinColor() const;
);

class CHARACTER
(
  dog,
  nonhumanoid,
 public:
  virtual bool Catches(item*);
  virtual void BeTalkedTo();
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
);

class CHARACTER
(
  spider,
  nonhumanoid,
 public:
  virtual bool SpecialBiteEffect(character*, vector2d, int, int, bool);
  virtual void GetAICommand();
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
  bear,
  nonhumanoid,
  ;
);

class CHARACTER
(
  dolphin,
  nonhumanoid,
 protected:
  virtual int GetSpecialBodyPartFlags(int) const;
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
  virtual int TakeHit(character*, item*, bodypart*, vector2d, double, double, int, int, int, bool, bool);
  virtual bool SpecialEnemySightedReaction(character*);
 protected:
  void MonsterTeleport(const char*);
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
  virtual color16 GetTorsoSpecialColor() const;
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
  virtual bool SpecialBiteEffect(character*, vector2d, int, int, bool);
);

class CHARACTER
(
  ostrich,
  nonhumanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual bool HandleCharacterBlockingTheWay(character*, vector2d, int);
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  bool HasDroppedBananas;
);

class CHARACTER
(
  chameleon,
  nonhumanoid,
 public:
  virtual int TakeHit(character*, item*, bodypart*, vector2d, double, double, int, int, int, bool, bool);
  virtual bool SpecialEnemySightedReaction(character*);
 protected:
  virtual color16 GetSkinColor() const;
  virtual void SpecialTurnHandler() { UpdatePictures(); }
);

class CHARACTER  
(
  floatingeye,
  nonhumanoid,
 public:
  virtual bool Hit(character*, vector2d, int, bool);
  virtual int TakeHit(character*, item*, bodypart*, vector2d, double, double, int, int, int, bool, bool);
  virtual void SetWayPoints(const fearray<packedvector2d>&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsSpy() const { return true; }
  virtual bool CanAttack() const { return false; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  std::vector<vector2d> WayPoints;
  uint NextWayPoint;
);

class CHARACTER
(
  eddy,
  nonhumanoid,
 public:
  virtual bool Hit(character*, vector2d, int, bool = false);
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
);

class CHARACTER
(
  mushroom,
  nonhumanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetSpecies(int);
  int GetSpecies() const { return Species; }
  virtual bool IsMushroom() const { return true; }
 protected:
  virtual void GetAICommand();
  virtual color16 GetTorsoMainColor() const { return Species; }
  virtual void VirtualConstructor(bool);
  int Species;
);

class CHARACTER
(
  magicmushroom,
  mushroom,
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
);

class CHARACTER
(
  ghost,
  nonhumanoid,
 public:
  virtual void AddName(festring&, int) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetOwnerSoul(const festring& What) { OwnerSoul = What; }
  virtual bool IsNameable() const { return OwnerSoul.IsEmpty(); }
  virtual bool RaiseTheDead(character*);
  virtual int ReceiveBodyPartDamage(character*, int, int, int, int = 8, bool = false, bool = false, bool = true, bool = false);
  virtual bool SpecialEnemySightedReaction(character*);
  void SetIsActive(bool What) { Active = What; }
  virtual bool IsPolymorphable() const { return MaxHP < 100; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual int GetBodyPartWobbleData(int) const;
  virtual const char* FirstPersonBiteVerb() const;
  virtual const char* FirstPersonCriticalBiteVerb() const;
  virtual const char* ThirdPersonBiteVerb() const;
  virtual const char* ThirdPersonCriticalBiteVerb() const;
  virtual bool AttackIsBlockable(int) const { return false; }
  virtual bool AttackMayDamageArmor() const { return false; }
  virtual void GetAICommand();
  festring OwnerSoul;
  bool Active;
);

class CHARACTER
(
  twoheadedmoose,
  nonhumanoid,
 public:
  virtual bool Hit(character*, vector2d, int, bool = false);
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

class CHARACTER
(
  invisiblestalker,
  nonhumanoid,
  ;
);

class ABSTRACT_CHARACTER
(
  largecreature,
  nonhumanoid,
 public:
  virtual void CalculateSquaresUnder() { SquaresUnder = 4; }
  virtual int GetSquareIndex(vector2d) const;
  virtual int GetNeighbourSquares() const { return 12; }
  virtual int GetExtendedNeighbourSquares() const { return 16; }
  virtual square* GetNeighbourSquare(int) const;
  virtual lsquare* GetNeighbourLSquare(int) const;
  virtual wsquare* GetNeighbourWSquare(int) const;
  virtual int CalculateNewSquaresUnder(lsquare**, vector2d) const;
  virtual bool IsFreeForMe(square*) const;
  virtual bool CanMoveOn(const lsquare*) const;
  virtual bool CanMoveOn(const square*) const;
  virtual void PutTo(vector2d);
  virtual void Remove();
  virtual bool CreateRoute();
  virtual bool CanTheoreticallyMoveOn(const lsquare*) const;
  virtual int GetFlySymbolSquareIndex() const { return 1; }
  virtual int GetSwimmingSymbolSquareIndex() const { return 3; }
  virtual int GetUnconsciousSymbolSquareIndex() const { return 2; }
  virtual bool PlaceIsIllegal(vector2d, vector2d) const;
  bool PartCanMoveOn(const lsquare*) const;
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void CreateCorpse(lsquare*);
  virtual void LoadSquaresUnder();
);

class CHARACTER
(
  elpuri,
  largecreature,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool Hit(character*, vector2d, int, bool = false);
  virtual int ReceiveBodyPartDamage(character*, int, int, int, int = 8, bool = false, bool = false, bool = true, bool = false);
  virtual bool SpecialEnemySightedReaction(character*);
  virtual bool MustBeRemovedFromBone() const;
  virtual bool TryToRiseFromTheDead();
 protected:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
  bool Active;
);

class CHARACTER
(
  genetrixvesana,
  largecreature,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void FinalProcessForBone();
 protected:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
  virtual bool MustBeRemovedFromBone() const;
  long TurnsExisted;
);

class CHARACTER
(
  hedgehog,
  nonhumanoid,
 public:
  virtual void SpecialBodyDefenceEffect(character*, bodypart*, int);
);

class CHARACTER
(
  bunny,
  nonhumanoid,
 public:
  virtual bool CheckIfSatiated() { return GetNP() > BLOATED_LEVEL; }
  virtual void SignalNaturalGeneration();
  virtual bool IsBunny() const { return true; }
  virtual bool Catches(item*);
 protected:
  bool CheckForMatePartner();
  virtual void GetAICommand();
);

class CHARACTER
(
  vladimir,
  largecreature,
 public:
  virtual bool MustBeRemovedFromBone() const;
  virtual color16 GetSkinColor() const;
  virtual void SpecialTurnHandler() { UpdatePictures(); }
);

class CHARACTER
(
  hattifattener,
  nonhumanoid,
 public:
  bool Hit(character*, vector2d, int, bool) { return false; }
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual int GetSpecialBodyPartFlags(int) const;
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
);

class CHARACTER
(
  blinkdog,
  dog,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int TakeHit(character*, item*, bodypart*, vector2d, double, double, int, int, int, bool, bool);
  virtual bool SpecialEnemySightedReaction(character*);
 protected:
  virtual void VirtualConstructor(bool);
  virtual bodypart* MakeBodyPart(int) const;
  void MonsterTeleport(const char*);
  bool SummonFriend();
  int SummonModifier;
);

class CHARACTER
(
  mysticfrog,
  frog,
 public:
  virtual void GetAICommand();
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual bodypart* MakeBodyPart(int) const;
  int GetSpellAPCost() const { return 1500; }
);

#endif
