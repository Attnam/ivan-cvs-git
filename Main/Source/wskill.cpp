#include "wskill.h"
#include "message.h"
#include "save.h"
#include "ivandef.h"

ushort CWeaponSkillLevelMap[] = { 0, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 65535 };
ulong CWeaponSkillUnuseTickMap[] = { 500000, 250000, 200000, 150000, 50000, 30000, 25000, 20000, 15000, 12500, 10000 };
ushort CWeaponSkillUnusePenaltyMap[] = { 10, 15, 25, 50, 75, 100, 200, 600, 1000, 2500, 3000 };
std::string CWeaponSkillName[WEAPON_SKILL_CATEGORIES] = { "unarmed combat", "kicking", "biting", "uncategorized", "small swords", "large swords", "blunt weapons", "axes", "polearms", "whips", "shields" };

ushort SWeaponSkillLevelMap[] = { 0, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 65535 };
ulong SWeaponSkillUnuseTickMap[] = { 100000, 100000, 40000, 30000, 20000, 15000, 10000, 7500, 5000, 2500, 2000 };
ushort SWeaponSkillUnusePenaltyMap[] = { 5, 5, 5, 15, 25, 50, 150, 250, 500, 1000, 1500 };

ushort cweaponskill::GetLevelMap(ushort Index) const { return CWeaponSkillLevelMap[Index]; }
ulong cweaponskill::GetUnuseTickMap(ushort Index) const { return CWeaponSkillUnuseTickMap[Index]; }
ushort cweaponskill::GetUnusePenaltyMap(ushort Index) const { return CWeaponSkillUnusePenaltyMap[Index]; }
const std::string& cweaponskill::GetName() const { return CWeaponSkillName[Index]; }

ushort sweaponskill::GetLevelMap(ushort Index) const { return SWeaponSkillLevelMap[Index]; }
ulong sweaponskill::GetUnuseTickMap(ushort Index) const { return SWeaponSkillUnuseTickMap[Index]; }
ushort sweaponskill::GetUnusePenaltyMap(ushort Index) const { return SWeaponSkillUnusePenaltyMap[Index]; }

void weaponskill::Save(outputfile& SaveFile) const
{
  SaveFile << Level << Hits << HitCounter;
}

void weaponskill::Load(inputfile& SaveFile)
{
  SaveFile >> Level >> Hits >> HitCounter;
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

bool weaponskill::AddHit(ushort AddHits)
{
  if(!AddHits)
    return false;

  HitCounter = 0;

  if(Hits <= 50000 - AddHits)
    Hits += AddHits;
  else
    Hits = 50000;

  uchar OldLevel = Level;

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

bool weaponskill::SubHit(ushort SubHits)
{
  if(!SubHits)
    return false;

  if(Hits >= SubHits)
    Hits -= SubHits;
  else
    Hits = 0;

  uchar OldLevel = Level;

  while(Level && Hits < GetLevelMap(Level))
    --Level;

  return Level != OldLevel;
}

void cweaponskill::AddLevelUpMessage() const
{
  ADD_MESSAGE("You advance to skill level %d with %s!", Level, CWeaponSkillName[Index].c_str());
}

void cweaponskill::AddLevelDownMessage() const
{
  ADD_MESSAGE("You have not practised enough with %s lately. Your skill level is reduced to %d!", CWeaponSkillName[Index].c_str(), Level);
}

void sweaponskill::AddLevelUpMessage(const std::string& WeaponName) const
{
  ADD_MESSAGE("You advance to skill level %d with your %s!", Level, WeaponName.c_str());
}

void sweaponskill::AddLevelDownMessage(const std::string& WeaponName) const
{
  ADD_MESSAGE("You have not practised enough with your %s lately. Your skill level is reduced to %d!", WeaponName.c_str(), Level);
}

void sweaponskill::Save(outputfile& SaveFile) const
{
  weaponskill::Save(SaveFile);
  SaveFile << ID;
}

void sweaponskill::Load(inputfile& SaveFile)
{
  weaponskill::Load(SaveFile);
  SaveFile >> ID;
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
