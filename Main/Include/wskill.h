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

class outputfile;
class inputfile;

class weaponskill
{
public:
	weaponskill(uchar, float = 1.0f);
	void Turn(bool = false);
	uchar GetLevel() const { return Level; }
	ulong GetHits() const { return Hits; }
	ulong GetHitCounter() const { return HitCounter; }
	void AddHit(bool = false);
	void AddHit(ulong, bool = false);
	void SubHit(bool = false);
	void SubHit(ulong, bool = false);
	float GetBonus() { return 1.0f + 0.15f * Level; }
	ushort GetLevelMap(ushort Index) const { return LevelMap[Index]; }
	ushort GetUnuseTurnMap(ushort Index) const { return UnuseTurnMap[Index]; }
	ushort GetUnusePenaltyMap(ushort Index) const { return UnusePenaltyMap[Index]; }
	std::string Name() const { return SkillName[Index]; }
	void Save(outputfile&) const;
	void Load(inputfile&);
private:
	static ushort LevelMap[];
	static ushort UnuseTurnMap[];
	static ushort UnusePenaltyMap[];
	static std::string SkillName[];
	uchar Index, Level;
	ushort Hits, HitCounter;
	float HitMultiplier;
};

inline outputfile& operator<<(outputfile& SaveFile, weaponskill* WeaponSkill)
{
	WeaponSkill->Save(SaveFile);

	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, weaponskill* WeaponSkill)
{
	WeaponSkill->Load(SaveFile);

	return SaveFile;
}

#endif
