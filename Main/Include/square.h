#ifndef __SQUARE_H__
#define __SQUARE_H__

#include <fstream>

#include "typedef.h"
#include "vector.h"

class area;
class material;
class bitmap;
class character;
class terrain;
class groundterrain;
class overterrain;

class square
{
public:
	square(area*, vector);
	square(area*, std::ifstream*, vector);
	~square(void);
	virtual void Save(std::ofstream*);
	virtual void DrawCheat(void);
	virtual void DrawMemorized(void);
	virtual void UpdateMemorizedAndDraw(void) = 0;
	virtual void DrawToTileBuffer(void) = 0;
	virtual void SCharacter(character* What ) { Character = What; }
	virtual void AddCharacter(character* Guy);
	virtual void RemoveCharacter(void);
	virtual character* CCharacter(void)		{return Character;}
	virtual bool CKnown(void)				{return Known;}
	virtual void ChangeTerrain(groundterrain*, overterrain*);
	virtual vector CPos(void)			{return Pos;}
	virtual void SKnown(bool What) { Known = What; }
	virtual void SGroundTerrain(groundterrain* What) { GroundTerrain = What; }
	virtual void SOverTerrain(overterrain* What) { OverTerrain = What; }
	virtual groundterrain* CGroundTerrain(void)	{return GroundTerrain;}
	virtual overterrain* COverTerrain(void)		{return OverTerrain;}
	virtual ushort CPopulation(void) { if(Character) return 1; else return 0; }
	virtual area* CMotherArea(void) { return MotherArea; }
protected:
	area* MotherArea;
	groundterrain* GroundTerrain;
	overterrain* OverTerrain;
	character* Rider, * Character, * Flyer;
	vector Pos;
	bool Known;
};

#endif
