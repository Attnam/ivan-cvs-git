#ifndef __SQUARE_H__
#define __SQUARE_H__

#include <fstream>

#include "typedef.h"
#include "vector.h"

class area;
class material;
class bitmap;
class character;
class levelterrain;
class groundterrain;
class overterrain;

class square
{
public:
	square(area*, vector);
	~square(void);
	virtual void Save(std::ofstream*) const;
	virtual void Load(std::ifstream*);
	virtual void DrawCheat(void) const;
	virtual void DrawMemorized(void) const;
	virtual void UpdateMemorizedAndDraw(void) = 0;
	virtual void DrawToTileBuffer(void) const = 0;
	virtual void SetCharacter(character* What ) { Character = What; }
	virtual void AddCharacter(character* Guy);
	virtual void RemoveCharacter(void);
	virtual character* CCharacter(void) const		{return Character;}
	virtual bool GetKnown(void) const				{return Known;}
	virtual vector GetPos(void) const			{return Pos;}
	virtual void SetKnown(bool What) { Known = What; }
	virtual ushort GetPopulation(void) const { if(Character) return 1; else return 0; }
	virtual area* GetMotherArea(void) const { return MotherArea; }
	virtual void EmptyFlag(void)			{Flag = false;}
	virtual void SetFlag(void)			{Flag = true;}
	virtual bool RetrieveFlag(void) const		{return Flag;}
	virtual groundterrain* GetGroundTerrain(void) { return GroundTerrain; }
	virtual void SetGroundTerrain(groundterrain* What) { GroundTerrain = What; }
	virtual overterrain* GetOverTerrain(void) { return OverTerrain; }
	virtual void SetOverTerrain(overterrain* What) { OverTerrain = What; }
protected:
	groundterrain* GroundTerrain;
	overterrain* OverTerrain;
	area* MotherArea;
	character* Rider, * Character, * Flyer;
	vector Pos;
	bool Known, Flag;
};

#endif



