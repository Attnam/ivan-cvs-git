#include "wskill.h"
#include "message.h"

ushort weaponskill::LevelMap[] = { 0, 50, 100, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000, 65535 };
ushort weaponskill::UnuseTurnMap[] = { 0, 10, 25, 40, 75, 150, 200, 400, 800, 1000, 1000 };
ushort weaponskill::UnusePenaltyMap[] = { 65535, 10000, 5000, 4000, 3000, 1500, 1000, 800, 400, 200, 100 };
std::string weaponskill::Name[] = { "Uncategorized", "Unarmed Combat", "Daggers", "Small Swords", "Large Swords", "Clubs", "Hammers", "Maces", "Flails", "Axes", "Halberds", "Spears" };

weaponskill::weaponskill(uchar Index, float HitMultiplier) : Index(Index), Level(0), Hits(0), HitCounter(0), HitMultiplier(HitMultiplier)
{
}

void weaponskill::Turn(bool AddMsg)
{
	if(HitCounter++ == UnuseTurnMap[Level])
	{
		SubHit(UnusePenaltyMap[Level], AddMsg);

		HitCounter = 0;
	}
}

void weaponskill::AddHit(bool AddMsg)
{
	if(Hits != 0xFFDC)
		if(++Hits == ulong(LevelMap[Level + 1] * HitMultiplier))
		{
			++Level;

			if(AddMsg)
				ADD_MESSAGE("You advance to skill level %d with %s!", Level, Name[Index].c_str());
		}

	HitCounter = 0;
}

void weaponskill::AddHit(ulong AddHits, bool AddMsg)
{
	if(Hits <= 0xFFDC - AddHits)
		Hits += AddHits;
	else
		Hits = 0xFFDC;

	uchar OldLevel = Level;

	while(Hits >= ulong(LevelMap[Level + 1] * HitMultiplier))
		++Level;

	if(AddMsg && Level != OldLevel)
		ADD_MESSAGE("You advance to skill level %d with %s!", Level, Name[Index].c_str());

	HitCounter = 0;
}

void weaponskill::SubHit(bool AddMsg)
{
	if(Hits)
	{
		--Hits;

		if(Level && Hits < ulong(LevelMap[Level] * HitMultiplier))
		{
			--Level;

			if(AddMsg)
				ADD_MESSAGE("You have not practised enough with %s and your skill level is reduced to %d!", Name[Index].c_str(), Level);
		}
	}
}

void weaponskill::SubHit(ulong SubHits, bool AddMsg)
{
	if(Hits >= SubHits)
		Hits -= SubHits;
	else
		Hits = 0;

	uchar OldLevel = Level;

	while(Level && Hits < ulong(LevelMap[Level] * HitMultiplier))
		--Level;

	if(AddMsg && Level != OldLevel)
		ADD_MESSAGE("You have not practised enough with %s and your skill level is reduced to %d!", Name[Index].c_str(), Level);
}
