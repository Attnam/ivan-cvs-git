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
	virtual void Draw(void) = 0;
	virtual vector RandomSquare(const bool);
	virtual void AddCharacter(vector, character*);
	virtual void RemoveCharacter(vector);
	virtual void Save(std::ofstream*);
	virtual ushort CFlag(const vector Pos) { return FlagMap[Pos.X][Pos.Y]; }
	virtual square* operator [] (vector Pos) {return Map[Pos.X][Pos.Y];}
	virtual square* CSquare(vector Pos) {return Map[Pos.X][Pos.Y];}
	virtual ushort CXSize(void) {return XSize;}
	virtual ushort CYSize(void) {return YSize;}
	virtual bitmap* CMemorized(void) { return Memorized; }
protected:
	square*** Map;
	bitmap* Memorized;
	ushort** FlagMap;
	ushort XSize, YSize;
	ulong XSizeTimesYSize;
};

#endif
