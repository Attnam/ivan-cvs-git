#ifndef __WSKILL_H__
#define __WSKILL_H__

#ifdef VC
#pragma warning(disable : 4786)
#pragma warning(disable : 4146)
#endif

#include "typedef.h"

class outputfile;
class inputfile;
class item;

class weaponskill
{
 public:
  weaponskill() : Level(0), Hits(0), HitCounter(0) { }
  int GetLevel() const { return Level; }
  int GetHits() const { return Hits; }
  uint GetHitCounter() const { return HitCounter; }
  bool Tick();
  bool AddHit();
  bool AddHit(int);
  bool SubHit();
  bool SubHit(int);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetLevelMap(int) const = 0;
  virtual ulong GetUnuseTickMap(int) const = 0;
  virtual int GetUnusePenaltyMap(int) const = 0;
 protected:
  int Level;
  int Hits;
  uint HitCounter;
};

class cweaponskill : public weaponskill
{
 public:
  virtual int GetLevelMap(int) const;
  virtual ulong GetUnuseTickMap(int) const;
  virtual int GetUnusePenaltyMap(int) const;
  const char* GetName(int) const;
  int GetBonus() const { return 100 + 10 * Level; }
  void AddLevelUpMessage(int) const;
  void AddLevelDownMessage(int) const;
};

inline outputfile& operator<<(outputfile& SaveFile, const cweaponskill& WeaponSkill)
{
  WeaponSkill.Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, cweaponskill& WeaponSkill)
{
  WeaponSkill.Load(SaveFile);
  return SaveFile;
}

class sweaponskill : public weaponskill
{
 public:
  sweaponskill() : ID(0), Weight(0), Config(0) { }
  sweaponskill(const item*);
  virtual int GetLevelMap(int) const;
  virtual ulong GetUnuseTickMap(int) const;
  virtual int GetUnusePenaltyMap(int) const;
  int GetBonus() const { return Level ? 115 + 5 * (Level - 1) : 100; }
  void AddLevelUpMessage(const char*) const;
  void AddLevelDownMessage(const char*) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  bool IsSkillOf(const item*) const;
  bool IsSkillOfCloneMother(const item*, ulong) const;
  void SetID(ulong What) { ID = What; }
  ulong GetID() const { return ID; }
  void PreProcessForBone() { ID = -ID; }
 private:
  ulong ID;
  long Weight;
  int Config;
};

inline outputfile& operator<<(outputfile& SaveFile, const sweaponskill* WeaponSkill)
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
