#ifndef __WSKILL_H__
#define __WSKILL_H__

#define WEAPON_SKILL_GATEGORIES	12

#define UNCATEGORIZED	0
#define UNARMED		1
#define DAGGERS		2
#define SMALL_SWORDS	3
#define LARGE_SWORDS	4
#define CLUBS		5
#define HAMMERS		6
#define MACES		7
#define FLAILS		8
#define AXES		9
#define HALBERDS	10
#define SPEARS		11

#include <string>

#include "typedef.h"

class weaponskill
{
public:
	weaponskill(uchar, float = 1.0f);
	void Turn(bool = false);
	uchar GetLevel() { return Level; }
	ulong GetHits() { return Hits; }
	ulong GetHitCounter() { return HitCounter; }
	void AddHit(bool = false);
	void AddHit(ulong, bool = false);
	void SubHit(bool = false);
	void SubHit(ulong, bool = false);
	float GetBonus() { return 1.0f + 0.15f * Level; }
private:
	static ushort LevelMap[];
	static ushort UnuseTurnMap[];
	static ushort UnusePenaltyMap[];
	static std::string Name[];
	uchar Index, Level;
	ushort Hits, HitCounter;
	float HitMultiplier;
};

#endif
