#ifndef __WSKILL_H__
#define __WSKILL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

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
  bool Tick();
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
  virtual ushort GetLevelMap(ushort) const;
  virtual ulong GetUnuseTickMap(ushort) const;
  virtual ushort GetUnusePenaltyMap(ushort) const;
  const char* GetName() const;
  ushort GetBonus() const { return 100 + 10 * Level; }
  void AddLevelUpMessage() const;
  void AddLevelDownMessage() const;
 private:
  uchar Index;
};

inline outputfile& operator<<(outputfile& SaveFile, const cweaponskill* WeaponSkill)
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
  virtual ushort GetLevelMap(ushort) const;
  virtual ulong GetUnuseTickMap(ushort) const;
  virtual ushort GetUnusePenaltyMap(ushort) const;
  ushort GetBonus() const { return Level ? 115 + 5 * (Level - 1) : 100; }
  void AddLevelUpMessage(const char*) const;
  void AddLevelDownMessage(const char*) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetID(ulong What) { ID = What; }
  ulong GetID() const { return ID; }
 private:
  ulong ID;
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
