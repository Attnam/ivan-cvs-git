#ifndef __AREA_H__
#define __AREA_H__

#include <fstream>

#include "typedef.h"
#include "vector.h"

class character;
class square;
class bitmap;

class area
{
public:
	area(ushort, ushort);
	area(std::ifstream*);
	~area(void);
	virtual void Generate(void) = 0;
	virtual void Draw(void) const = 0;
	virtual vector RandomSquare(bool) const;
	virtual void AddCharacter(vector, character*);
	virtual void RemoveCharacter(vector);
	virtual void Save(std::ofstream*) const;
	virtual ushort CFlag(vector Pos) const { return FlagMap[Pos.X][Pos.Y]; }
	virtual square* operator [] (vector Pos) const {return Map[Pos.X][Pos.Y];}
	virtual square* CSquare(vector Pos) const {return Map[Pos.X][Pos.Y];}
	virtual ushort CXSize(void) const {return XSize;}
	virtual ushort CYSize(void) const {return YSize;}
	virtual bitmap* CMemorized(void) const { return Memorized; }
protected:
	square*** Map;
	bitmap* Memorized;
	ushort** FlagMap;
	ushort XSize, YSize;
	ulong XSizeTimesYSize;
};

#endif
