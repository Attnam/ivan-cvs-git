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
	virtual void AddCharacter(vector, character*);
	virtual void RemoveCharacter(vector);
	virtual void Save(std::ofstream*) const;
	virtual ushort GetFlag(vector Pos) const { return FlagMap[Pos.X][Pos.Y]; }
	virtual square* operator [] (vector Pos) const {return Map[Pos.X][Pos.Y];}
	virtual square* GetSquare(vector Pos) const {return Map[Pos.X][Pos.Y];}
	virtual ushort GetXSize(void) const {return XSize;}
	virtual ushort GetYSize(void) const {return YSize;}
	virtual bitmap* GetMemorized(void) const { return Memorized; }
	virtual void UpdateLOS(void);
	virtual void EmptyFlags(void);
protected:
	square*** Map;
	bitmap* Memorized;
	ushort** FlagMap;
	ushort XSize, YSize;
	ulong XSizeTimesYSize;
};

#endif
