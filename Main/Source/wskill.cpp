#include "wskill.h"
#include "message.h"
#include "save.h"

ushort gweaponskill::LevelMap[] = { 0, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 65535 };
ulong gweaponskill::UnuseTickMap[] = { 500000, 250000, 200000, 150000, 50000, 30000, 25000, 20000, 15000, 12500, 10000 };
ushort gweaponskill::UnusePenaltyMap[] = { 10, 15, 25, 50, 75, 100, 200, 600, 1000, 2500, 3000 };

std::string gweaponskill::SkillName[] = { "unarmed combat", "kicking", "biting", "uncategorized", "daggers", "small swords", "large swords", "clubs", "hammers", "maces", "flails", "axes", "halberds", "spears", "whips" };

ushort sweaponskill::LevelMap[] = { 0, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 65535 };
ulong sweaponskill::UnuseTickMap[] = { 100000, 100000, 40000, 30000, 20000, 15000, 10000, 7500, 5000, 2500, 2000 };
ushort sweaponskill::UnusePenaltyMap[] = { 5, 5, 5, 15, 25, 50, 150, 250, 500, 1000, 1500 };

weaponskill::weaponskill() : Level(0), Hits(0), HitCounter(0)
{
}

void weaponskill::Save(outputfile& SaveFile) const
{
  SaveFile << Level << Hits << HitCounter;
}

void weaponskill::Load(inputfile& SaveFile)
{
  SaveFile >> Level >> Hits >> HitCounter;
}

bool weaponskill::Tick(ushort Ticks)
{
  HitCounter += Ticks;
  bool LevelChange = false;

  while(HitCounter >= GetUnuseTickMap(Level))
    {
      HitCounter -= GetUnuseTickMap(Level);

      if(SubHit(GetUnusePenaltyMap(Level)))
	LevelChange = true;
    }

  return LevelChange;
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
  HitCounter = 0;

  if(Hits <= 50000 - AddHits)
    Hits += AddHits;
  else
    Hits = 50000;

  uchar OldLevel = Level;

  while(Hits >= GetLevelMap(Level + 1))
    ++Level;

  if(Level != OldLevel)
    return true;
  else
    return false;
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
  if(Hits >= SubHits)
    Hits -= SubHits;
  else
    Hits = 0;

  uchar OldLevel = Level;

  while(Level && Hits < GetLevelMap(Level))
    --Level;

  if(Level != OldLevel)
    return true;
  else
    return false;
}

void gweaponskill::AddLevelUpMessage() const
{
  ADD_MESSAGE("You advance to skill level %d with %s!", Level, SkillName[Index].c_str());
}

void gweaponskill::AddLevelDownMessage() const
{
  ADD_MESSAGE("You have not practised enough with %s lately. Your skill level is reduced to %d!", SkillName[Index].c_str(), Level);
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
