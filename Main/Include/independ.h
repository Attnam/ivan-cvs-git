#ifndef __INDEPEND_H__
#define __INDEPEND_H__

#define CNAME(Case) Name(Case).c_str()

#define	UNARTICLED		0	//0000000
#define	PLURAL			1	//0000001
#define	DEFINEBIT		2	//0000010
#define	DEFINITE		2	//0000010
#define	INDEFINEBIT		4	//0000100
#define	INDEFINITE		6	//0000110

#include <string>
#include <ivanfstr.h>

#include "typedef.h"
#include "vector.h"

class independency
{
public:
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
	virtual void Save(std::ofstream&) const;
	virtual void DrawToTileBuffer(void) const = 0;
	virtual std::string GetNameSingular(void) const { return NameSingular(); }
	virtual std::string GetNamePlural(void) const { return NamePlural(); }
protected:
	virtual std::string NameSingular(void) const = 0;
	virtual std::string NamePlural(void) const = 0;
	virtual std::string NameNormal(uchar, std::string) const;
	virtual std::string NameProperNoun(uchar) const;
	virtual ushort Type(void) const = 0;
	virtual vector GetBitmapPos(void) const = 0;
};

#endif

