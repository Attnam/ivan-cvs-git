#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <fstream>

#include "typeable.h"
#include "drawable.h"

class groundterrain : virtual public typeable, virtual public drawable
{
};

class overterrain : virtual public typeable, virtual public drawable
{
public:
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual void SetIsWalkable(bool What)			{ IsWalkable = What; }
	virtual bool GetIsWalkable(void) const			{ return IsWalkable; }
protected:
	bool IsWalkable;
};

#endif

