#ifndef __TYPEABLE_H__
#define __TYPEABLE_H__

#include <string>

#include "typedef.h"

class area;
class square;
class outputfile;
class inputfile;

class typeable
{
public:
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual std::string Name(uchar Case) const = 0;
	virtual typeable* CloneAndLoad(inputfile&) const = 0;
	virtual area* GetAreaUnder() const { return AreaUnder; }
	virtual void SetAreaUnder(area* What) { AreaUnder = What; }
	virtual square* GetSquareUnder() const { return SquareUnder; }
	virtual void SetSquareUnder(square* What) { SquareUnder = What; }
	virtual ushort GetType() { return Type(); }
	virtual std::string ClassName() const = 0;
protected:
	virtual ushort Type() const = 0;
	area* AreaUnder;
	square* SquareUnder;
};

#endif
