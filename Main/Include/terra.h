#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "typeable.h"

class outputfile;
class inputfile;
class character;

class terrain
{
public:
	virtual bool GetIsWalkable() const { return true; }
};

class groundterrain : public terrain
{
};

class overterrain : public terrain
{
public:
	virtual bool GoUp(character*) const = 0;
	virtual bool GoDown(character*) const = 0;
};

#endif
