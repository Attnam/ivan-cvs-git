#ifndef __TYPEABLE_H__
#define __TYPEABLE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

class outputfile;
class inputfile;

class typeable
{
public:
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&) {}
	virtual typeable* CloneAndLoad(inputfile&) const = 0;
	virtual ushort GetType() { return Type(); }
	virtual std::string ClassName() const = 0;
protected:
	virtual ushort Type() const = 0;
};

#endif



