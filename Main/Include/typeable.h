#ifndef __TYPEABLE_H__
#define __TYPEABLE_H__

#include <fstream>
#include <string>

#include "typedef.h"

class area;
class square;

class typeable
{
public:
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual std::string Name(uchar Case) const = 0;
	virtual typeable* CloneAndLoad(std::ifstream&) const = 0;
	virtual area* GetAreaUnder(void) const { return AreaUnder; }
	virtual void SetAreaUnder(area* What) { AreaUnder = What; }
	virtual square* GetSquareUnder(void) const { return SquareUnder; }
	virtual void SetSquareUnder(square* What) { SquareUnder = What; }
protected:
	virtual ushort Type(void) const = 0;
	area* AreaUnder;
	square* SquareUnder;
};

#endif

