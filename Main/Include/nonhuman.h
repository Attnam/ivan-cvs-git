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
  virtual void Kick(lsquare*, uchar, bool = false);
  virtual bool Hit(character*, vector2d, uchar, bool = false);
  virtual void UnarmedHit(character*, vector2d, uchar, bool = false);
  virtual void InitSpecialAttributes();
  virtual float GetTimeToKill(const character*, bool) const;
  virtual void ApplyExperience(bool = false);
  virtual ushort GetAttribute(ushort) const;
  virtual bool EditAttribute(ushort, short);
  virtual void EditExperience(ushort, long);
  virtual ushort DrawStats(bool) const;
  virtual void Bite(character*, vector2d, uchar, bool = false);
  virtual ushort GetCarryingStrength() const;
  virtual void CalculateBattleInfo();
  void CalculateUnarmedAttackInfo();
  void CalculateKickAttackInfo();
  void CalculateBiteAttackInfo();
  virtual bool UseMaterialAttributes() const;
  void SetStrength(ushort What) { Strength = What; }
  void SetAgility(ushort What) { Agility = What; }
  virtual void AddSpecialStethoscopeInfo(felist&) const;
  virtual bool EditAllAttributes(short);
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
  billswill,
  nonhumanoid,
 protected:
  virtual const char* FirstPersonBiteVerb() const;
  virtual const char* FirstPersonCriticalBiteVerb() const;
  virtual const char* ThirdPersonBiteVerb() const;
  virtual const char* ThirdPersonCriticalBiteVerb() const;
  virtual bool AttackIsBlockable(uchar) const { return false; }
  virtual bool AttackMayDamageArmor() const { return false; }
);

class CHARACTER
(
  mommo,
  nonhumanoid,
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
  virtual bool SpecialBiteEffect(character*, vector2d, uchar, uchar, bool);
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
  virtual ushort TakeHit(character*, item*, bodypart*, vector2d, float, float, short, uchar, uchar, bool, bool);
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
  virtual void GetAICommand() { AttackAdjacentEnemyAI(); }
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
  virtual bool SpecialBiteEffect(character*, vector2d, uchar, uchar, bool);
);

class CHARACTER
(
  ostrich,
  nonhumanoid,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual bool HandleCharacterBlockingTheWay(character*, vector2d, uchar);
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  bool HasBeenOnLandingSite;
);

class CHARACTER
(
  chameleon,
  nonhumanoid,
 public:
  virtual ushort TakeHit(character*, item*, bodypart*, vector2d, float, float, short, uchar, uchar, bool, bool);
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
  virtual bool Hit(character*, vector2d, uchar, bool);
  virtual ushort TakeHit(character*, item*, bodypart*, vector2d, float, float, short, uchar, uchar, bool, bool);
  virtual void SetWayPoints(const std::vector<vector2d>&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsSpy() const { return true; }
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
  virtual bool Hit(character*, vector2d, uchar, bool = false);
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
 public:
  virtual void AddName(festring&, uchar) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetOwnerSoul(const festring& What) { OwnerSoul = What; }
  virtual bool IsNameable() const { return OwnerSoul.IsEmpty(); }
  virtual bool RaiseTheDead(character*);
  virtual ushort ReceiveBodyPartDamage(character*, ushort, ushort, uchar, uchar = 8, bool = false, bool = false, bool = true, bool = false);
  virtual bool SpecialEnemySightedReaction(character*);
  void SetIsActive(bool What) { Active = What; }
  virtual bool IsPolymorphable() const { return MaxHP < 100; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual const char* FirstPersonBiteVerb() const;
  virtual const char* FirstPersonCriticalBiteVerb() const;
  virtual const char* ThirdPersonBiteVerb() const;
  virtual const char* ThirdPersonCriticalBiteVerb() const;
  virtual bool AttackIsBlockable(uchar) const { return false; }
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
  virtual bool Hit(character*, vector2d, uchar, bool = false);
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
  virtual ushort GetSquareIndex(vector2d) const;
  virtual ushort GetNeighbourSquares() const { return 12; }
  virtual ushort GetExtendedNeighbourSquares() const { return 16; }
  virtual square* GetNeighbourSquare(ushort) const;
  virtual lsquare* GetNeighbourLSquare(ushort) const;
  virtual wsquare* GetNeighbourWSquare(ushort) const;
  virtual ushort CalculateNewSquaresUnder(lsquare**, vector2d) const;
  virtual bool IsFreeForMe(square*) const;
  virtual bool CanMoveOn(const lsquare*) const;
  virtual bool CanMoveOn(const square*) const;
  virtual void PutTo(vector2d);
  virtual void Remove();
  virtual bool CreateRoute();
  virtual bool CanTheoreticallyMoveOn(const lsquare*) const;
  virtual ushort GetFlySymbolSquareIndex() const { return 1; }
  virtual bool PlaceIsIllegal(vector2d, vector2d) const;
 protected:
  virtual bodypart* MakeBodyPart(ushort) const;
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
  virtual bool Hit(character*, vector2d, uchar, bool = false);
  virtual ushort ReceiveBodyPartDamage(character*, ushort, ushort, uchar, uchar = 8, bool = false, bool = false, bool = true, bool = false);
  virtual bool SpecialEnemySightedReaction(character*);
  virtual bool Faint(ushort, bool = false) { return false; }
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
 protected:
  virtual void VirtualConstructor(bool);
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
  virtual bool MustBeRemovedFromBone() const;
  ulong TurnsExisted;
);

class CHARACTER
(
  hedgehog,
  nonhumanoid,
 public:
  virtual void SpecialBodyDefenceEffect(character*, bodypart*, uchar);
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
  ushort RandomizeBabyAttribute(ushort);
  virtual void GetAICommand();
);

class CHARACTER
(
  vladimir,
  largecreature,
 public:
  virtual bool MustBeRemovedFromBone() const;
  virtual ushort GetSkinColor() const;
  virtual void SpecialTurnHandler() { UpdatePictures(); }
);

class CHARACTER
(
  hattifattener,
  nonhumanoid,
 public:
  bool Hit(character*, vector2d, uchar, bool) { return false; }
 protected:
  virtual void GetAICommand();
  virtual bodypart* MakeBodyPart(ushort) const;
  virtual void CreateCorpse(lsquare*);
);

#endif
