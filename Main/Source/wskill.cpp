#include "wskill.h"
#include "message.h"
#include "save.h"
#include "item.h"

int CWeaponSkillLevelMap[] = { 0, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 65535 };
ulong CWeaponSkillUnuseTickMap[] = { 500000, 250000, 200000, 150000, 50000, 30000, 25000, 20000, 15000, 12500, 10000 };
int CWeaponSkillUnusePenaltyMap[] = { 10, 15, 25, 50, 75, 100, 200, 600, 1000, 2500, 3000 };
const char* CWeaponSkillName[WEAPON_SKILL_CATEGORIES] = { "unarmed combat", "kicking", "biting", "uncategorized", "small swords", "large swords", "blunt weapons", "axes", "polearms", "whips", "shields" };

int SWeaponSkillLevelMap[] = { 0, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 65535 };
ulong SWeaponSkillUnuseTickMap[] = { 100000, 100000, 40000, 30000, 20000, 15000, 10000, 7500, 5000, 2500, 2000 };
int SWeaponSkillUnusePenaltyMap[] = { 5, 5, 5, 15, 25, 50, 150, 250, 500, 1000, 1500 };

int cweaponskill::GetLevelMap(int I) const { return CWeaponSkillLevelMap[I]; }
ulong cweaponskill::GetUnuseTickMap(int I) const { return CWeaponSkillUnuseTickMap[I]; }
int cweaponskill::GetUnusePenaltyMap(int I) const { return CWeaponSkillUnusePenaltyMap[I]; }
const char* cweaponskill::GetName(int Category) const { return CWeaponSkillName[Category]; }

sweaponskill::sweaponskill(const item* Item) : ID(Item->GetID()), Weight(Item->GetWeight()), Config(Item->GetConfig()) { }
int sweaponskill::GetLevelMap(int I) const { return SWeaponSkillLevelMap[I]; }
ulong sweaponskill::GetUnuseTickMap(int I) const { return SWeaponSkillUnuseTickMap[I]; }
int sweaponskill::GetUnusePenaltyMap(int I) const { return SWeaponSkillUnusePenaltyMap[I]; }
bool sweaponskill::IsSkillOf(const item* Item) const { return ID == Item->GetID() && Weight == Item->GetWeight() && Config == Item->GetConfig(); }
bool sweaponskill::IsSkillOfCloneMother(const item* Item, ulong CMID) const { return ID == CMID && Weight == Item->GetWeight() && Config == Item->GetConfig(); }

void weaponskill::Save(outputfile& SaveFile) const
{
  SaveFile << (int)Level << (int)Hits << (int)HitCounter;
}

void weaponskill::Load(inputfile& SaveFile)
{
  SaveFile >> (int&)Level >> (int&)Hits >> (int&)HitCounter;
}

bool weaponskill::AddHit()
{
  HitCounter = 0;

  if(Hits != 50000)
    if(++Hits == GetLevelMap(Level + 1))
      {
	++Level;
	return true;
      }

  return false;
}

bool weaponskill::AddHit(int AddHits)
{
  if(!AddHits)
    return false;

  HitCounter = 0;

  if(Hits <= 50000 - AddHits)
    Hits += AddHits;
  else
    Hits = 50000;

  int OldLevel = Level;

  while(Hits >= GetLevelMap(Level + 1))
    ++Level;

  return Level != OldLevel;
}

bool weaponskill::SubHit()
{
  if(Hits)
    {
      --Hits;

      if(Level && Hits < GetLevelMap(Level))
	{
	  --Level;
	  return true;
	}
    }

  return false;
}

bool weaponskill::SubHit(int SubHits)
{
  if(!SubHits)
    return false;

  if(Hits >= SubHits)
    Hits -= SubHits;
  else
    Hits = 0;

  int OldLevel = Level;

  while(Level && Hits < GetLevelMap(Level))
    --Level;

  return Level != OldLevel;
}

void cweaponskill::AddLevelUpMessage(int Category) const
{
  ADD_MESSAGE("You advance to skill level %d with %s!", Level, CWeaponSkillName[Category]);
}

void cweaponskill::AddLevelDownMessage(int Category) const
{
  ADD_MESSAGE("You have not practised enough with %s lately. Your skill level is reduced to %d!", CWeaponSkillName[Category], Level);
}

void sweaponskill::AddLevelUpMessage(const char* WeaponName) const
{
  ADD_MESSAGE("You advance to skill level %d with your %s!", Level, WeaponName);
}

void sweaponskill::AddLevelDownMessage(const char* WeaponName) const
{
  ADD_MESSAGE("You have not practised enough with your %s lately. Your skill level is reduced to %d!", WeaponName, Level);
}

void sweaponskill::Save(outputfile& SaveFile) const
{
  weaponskill::Save(SaveFile);
  SaveFile << ID << Weight << (int)Config;
}

void sweaponskill::Load(inputfile& SaveFile)
{
  weaponskill::Load(SaveFile);
  SaveFile >> ID >> Weight >> (int&)Config;
}

bool weaponskill::Tick()
{
  if(Hits && HitCounter++ >= GetUnuseTickMap(Level))
    {
      HitCounter -= GetUnuseTickMap(Level);

      if(SubHit(GetUnusePenaltyMap(Level)))
	return true;
    }

  return false;
}
