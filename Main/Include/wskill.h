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
  weaponskill() : Level(0), Hits(0), HitCounter(0) { }
  uchar GetLevel() const { return Level; }
  ulong GetHits() const { return Hits; }
  ulong GetHitCounter() const { return HitCounter; }
  bool AddHit();
  bool AddHit(ushort);
  bool SubHit();
  bool SubHit(ushort);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetLevelMap(ushort) const = 0;
  virtual ulong GetUnuseTickMap(ushort) const = 0;
  virtual ushort GetUnusePenaltyMap(ushort) const = 0;
 protected:
  uchar Level;
  ushort Hits;
  ushort HitCounter;
};

class cweaponskill : public weaponskill
{
 public:
  cweaponskill(uchar Index) : Index(Index) { }
  bool Tick();
  ushort GetLevelMap(ushort Index) const { return LevelMap[Index]; }
  ulong GetUnuseTickMap(ushort Index) const { return UnuseTickMap[Index]; }
  ushort GetUnusePenaltyMap(ushort Index) const { return UnusePenaltyMap[Index]; }
  const std::string& Name() const { return SkillName[Index]; }
  uchar GetEffectBonus() const { return 100 + 10 * Level; }
  uchar GetAPBonus() const { return 100 - 5 * Level; }
  void AddLevelUpMessage() const;
  void AddLevelDownMessage() const;
 private:
  static ushort LevelMap[];
  static ulong UnuseTickMap[];
  static ushort UnusePenaltyMap[];
  static std::string SkillName[];
  uchar Index;
};

inline outputfile& operator<<(outputfile& SaveFile, cweaponskill* WeaponSkill)
{
  WeaponSkill->Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, cweaponskill* WeaponSkill)
{
  WeaponSkill->Load(SaveFile);
  return SaveFile;
}

class sweaponskill : public weaponskill
{
 public:
  bool Tick();
  ushort GetLevelMap(ushort Index) const { return LevelMap[Index]; }
  ulong GetUnuseTickMap(ushort Index) const { return UnuseTickMap[Index]; }
  ushort GetUnusePenaltyMap(ushort Index) const { return UnusePenaltyMap[Index]; }
  uchar GetEffectBonus() const { return Level ? 115 + 5 * (Level - 1) : 100; }
  uchar GetAPBonus() const { return Level ? 93 - 2 * (Level - 1) : 100; }
  void AddLevelUpMessage(const std::string&) const;
  void AddLevelDownMessage(const std::string&) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetID(ulong What) { ID = What; }
  ulong GetID() const { return ID; }
 private:
  static ushort LevelMap[];
  static ulong UnuseTickMap[];
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


