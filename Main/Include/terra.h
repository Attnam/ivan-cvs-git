#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "typeable.h"

class outputfile;
class inputfile;
class character;

class groundterrain : virtual public typeable//, virtual public drawable
{
public:
	virtual void DrawToTileBuffer() const = 0;
};

class overterrain : virtual public typeable//, virtual public drawable
{
public:
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void SetIsWalkable(bool What)			{ IsWalkable = What; }
	virtual bool GetIsWalkable() const			{ return IsWalkable; }
	virtual bool GoUp(character*) const = 0;
	virtual bool GoDown(character*) const = 0;
	virtual void DrawToTileBuffer() const = 0;
protected:
	bool IsWalkable;
};

#endif
