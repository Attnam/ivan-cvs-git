#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <fstream>

class groundterrain
{
public:
	virtual void DrawToTileBuffer(void) const = 0;
};

class overterrain
{
public:
	virtual void Save(std::ofstream*) const;
	virtual void Load(std::ifstream*);
	virtual void DrawToTileBuffer(void) const = 0;
	virtual void SetIsWalkable(bool What)			{ IsWalkable = What; }
	virtual bool GetIsWalkable(void) const			{ return IsWalkable; }
protected:
	bool IsWalkable;
};

#endif
