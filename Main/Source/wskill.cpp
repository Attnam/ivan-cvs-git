#include "wskill.h"
#include "message.h"
#include "save.h"

ushort gweaponskill::LevelMap[] = { 0, 50, 100, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000, 65535 };
ushort gweaponskill::UnuseTurnMap[] = { 65535, 10000, 5000, 4000, 3000, 1500, 1000, 800, 400, 200, 100 };
ushort gweaponskill::UnusePenaltyMap[] = { 0, 10, 25, 40, 75, 150, 200, 400, 800, 1000, 1000 };
std::string gweaponskill::SkillName[] = { "uncategorized", "unarmed combat", "daggers", "small swords", "large swords", "clubs", "hammers", "maces", "flails", "axes", "halberds", "spears" };

ushort sweaponskill::LevelMap[] = { 0, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 65535 };
ushort sweaponskill::UnuseTurnMap[] = { 65535, 2500, 1500, 1000, 500, 500, 500, 250, 125, 50, 25 };
ushort sweaponskill::UnusePenaltyMap[] = { 0, 5, 10, 20, 25, 100, 200, 250, 500, 500, 500 };

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

bool weaponskill::Turn()
{
	if(HitCounter++ == GetUnuseTurnMap(Level))
		return SubHit(GetUnusePenaltyMap(Level));
	else
		return false;
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
	ADD_MESSAGE("You have not practised enough with %s lately.", SkillName[Index].c_str());
	ADD_MESSAGE("Your skill level is reduced to %d!", Level);
}

void sweaponskill::AddLevelUpMessage(std::string WeaponName) const
{
	ADD_MESSAGE("You advance to skill level %d with your %s!", Level, WeaponName.c_str());
}

void sweaponskill::AddLevelDownMessage(std::string WeaponName) const
{
	ADD_MESSAGE("You have not practised enough with your %s lately.", WeaponName.c_str());
	ADD_MESSAGE("Your skill level is reduced to %d!", Level);
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

