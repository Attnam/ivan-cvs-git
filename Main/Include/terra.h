#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "typeable.h"
#include "drawable.h"

class outputfile;
class inputfile;

class groundterrain : virtual public typeable, virtual public drawable
{
};

class overterrain : virtual public typeable, virtual public drawable
{
public:
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void SetIsWalkable(bool What)			{ IsWalkable = What; }
	virtual bool GetIsWalkable() const			{ return IsWalkable; }
protected:
	bool IsWalkable;
};

#endif
