#ifndef __WSKILL_H__
#define __WSKILL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

class outputfile;
class inputfile;

class weaponskill
{
 public:
  weaponskill();
  bool Turn(ushort);
  uchar GetLevel() const { return Level; }
  ulong GetHits() const { return Hits; }
  ulong GetHitCounter() const { return HitCounter; }
  bool AddHit();
  bool AddHit(ulong);
  bool SubHit();
  bool SubHit(ulong);
  virtual ushort GetLevelMap(ushort Index) const = 0;
  virtual ushort GetUnuseTickMap(ushort Index) const = 0;
  virtual ushort GetUnusePenaltyMap(ushort Index) const = 0;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetHitMultiplier(float What) { HitMultiplier = What; }
  float GetHitMultiplier() const { return HitMultiplier; }
 protected:
  uchar Level;
  ushort Hits, HitCounter;
  float HitMultiplier;
};

class gweaponskill : public weaponskill
{
 public:
  gweaponskill(uchar Index) : Index(Index) { }
  ushort GetLevelMap(ushort Index) const { return LevelMap[Index]; }
  ushort GetUnuseTickMap(ushort Index) const { return UnuseTickMap[Index]; }
  ushort GetUnusePenaltyMap(ushort Index) const { return UnusePenaltyMap[Index]; }
  std::string Name() const { return SkillName[Index]; }
  float GetBonus() const { return 1.0f + 0.15f * Level; }
  void AddLevelUpMessage() const;
  void AddLevelDownMessage() const;
 private:
  static ushort LevelMap[];
  static ushort UnuseTickMap[];
  static ushort UnusePenaltyMap[];
  static std::string SkillName[];
  uchar Index;
};

inline outputfile& operator<<(outputfile& SaveFile, gweaponskill* WeaponSkill)
{
  WeaponSkill->Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, gweaponskill* WeaponSkill)
{
  WeaponSkill->Load(SaveFile);
  return SaveFile;
}

class sweaponskill : public weaponskill
{
 public:
  sweaponskill() { }
  ushort GetLevelMap(ushort Index) const { return LevelMap[Index]; }
  ushort GetUnuseTickMap(ushort Index) const { return UnuseTickMap[Index]; }
  ushort GetUnusePenaltyMap(ushort Index) const { return UnusePenaltyMap[Index]; }
  float GetBonus() const { return Level ? 1.2f + 0.05f * (Level - 1) : 1.0f; }
  void AddLevelUpMessage(const std::string&) const;
  void AddLevelDownMessage(const std::string&) const;
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetID(ulong What) { ID = What; }
  ulong GetID() const { return ID; }
 private:
  static ushort LevelMap[];
  static ushort UnuseTickMap[];
  static ushort UnusePenaltyMap[];
  ulong ID;
};

inline outputfile& operator<<(outputfile& SaveFile, sweaponskill* WeaponSkill)
{
  WeaponSkill->Save(SaveFile);

  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, sweaponskill*& WeaponSkill)
{
  WeaponSkill = new sweaponskill;
  WeaponSkill->Load(SaveFile);

  return SaveFile;
}

#endif


