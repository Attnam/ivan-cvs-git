#ifndef __INDEPEND_H__
#define __INDEPEND_H__

#define CNAME(Case) Name(Case).c_str()

#define	UNARTICLED		0	//0000000
#define	PLURAL			1	//0000001
#define	DEFINEBIT		2	//0000010
#define	DEFINITE		2	//0000010
#define	INDEFINEBIT		4	//0000100
#define	INDEFINITE		6	//0000110

#define NAME public: virtual std::string Name(uchar Case)
#define NAME_SINGULAR protected: virtual std::string NameSingular(void) const
#define NAME_PLURAL protected: virtual std::string NamePlural(void) const
#define DRAW_TO_TILE_BUFFER public: virtual void DrawToTileBuffer(void)
#define C_BITMAP_POS protected: virtual vector CBitmapPos(void)

#include <string>
#include <fstream>

#include "typedef.h"
#include "vector.h"

class independency
{
public:
	independency(void) {}
	independency(std::ifstream*) {}
	~independency(void) {}
	virtual std::string Name(uchar Case) { return NameNormal(Case, "a"); }
	virtual void Save(std::ofstream*);
	virtual void DrawToTileBuffer(void) = 0;
	virtual std::string CNameSingular(void) const { return NameSingular(); }
	virtual std::string CNamePlural(void) const { return NamePlural(); }
protected:
	virtual std::string NameSingular(void) const = 0;
	virtual std::string NamePlural(void) const = 0;
	virtual std::string NameNormal(uchar, std::string);
	virtual std::string NameProperNoun(uchar);
	virtual ushort Type(void) = 0;
	virtual vector CBitmapPos(void) = 0;
};

#endif
