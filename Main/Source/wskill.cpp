#include "wskill.h"
#include "message.h"
#include "save.h"

ushort gweaponskill::LevelMap[] = { 0, 50, 100, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000, 65535 };
ushort gweaponskill::UnuseTickMap[] = { 50000, 50000, 50000, 40000, 30000, 15000, 10000, 8000, 4000, 2000, 1000 };
ushort gweaponskill::UnusePenaltyMap[] = { 5, 10, 25, 40, 75, 150, 200, 400, 800, 1000, 1000 };

std::string gweaponskill::SkillName[] = { "unarmed combat", "kicking", "biting", "uncategorized", "daggers", "small swords", "large swords", "clubs", "hammers", "maces", "flails", "axes", "halberds", "spears", "whips" };

ushort sweaponskill::LevelMap[] = { 0, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 65535 };
ushort sweaponskill::UnuseTickMap[] = { 50000, 25000, 15000, 10000, 5000, 5000, 5000, 2500, 1250, 500, 250 };
ushort sweaponskill::UnusePenaltyMap[] = { 1, 5, 10, 20, 25, 100, 200, 250, 500, 500, 500 };

weaponskill::weaponskill() : Level(0), Hits(0), HitCounter(0), HitMultiplier(1.0f)
{
}

void weaponskill::Save(outputfile& SaveFile) const
{
  SaveFile << Level << Hits << HitCounter << HitMultiplier;
}

void weaponskill::Load(inputfile& SaveFile)
{
  SaveFile >> Level >> Hits >> HitCounter >> HitMultiplier;
}

bool weaponskill::Turn(ushort Turns)
{
  HitCounter += Turns;
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

  if(Hits != 0xFFDC)
    if(++Hits == ulong(GetLevelMap(Level + 1) * HitMultiplier))
      {
	++Level;
	return true;
      }

  return false;
}

bool weaponskill::AddHit(ulong AddHits)
{
  HitCounter = 0;

  if(Hits <= 0xFFDC - AddHits)
    Hits += AddHits;
  else
    Hits = 0xFFDC;

  uchar OldLevel = Level;

  while(Hits >= ulong(GetLevelMap(Level + 1) * HitMultiplier))
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

      if(Level && Hits < ulong(GetLevelMap(Level) * HitMultiplier))
	{
	  --Level;
	  HitCounter = 0;
	  return true;
	}
    }

  return false;
}

bool weaponskill::SubHit(ulong SubHits)
{
  if(Hits >= SubHits)
    Hits -= SubHits;
  else
    Hits = 0;

  uchar OldLevel = Level;

  while(Level && Hits < ulong(GetLevelMap(Level) * HitMultiplier))
    {
      --Level;
      HitCounter = 0;
    }

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
