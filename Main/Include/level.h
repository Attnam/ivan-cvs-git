#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <fstream>

#include "dynarray.h"
#include "typedef.h"
#include "vector.h"

#include "area.h"

class levelsquare;
class vector;
class character;
class area;
class bitmap;

/* Presentation of the level class */

class level : public area
{
public:
	level(ushort, ushort, ushort);
	level(void) {}
	virtual ~level(void);
	virtual void Generate(void);
	virtual vector RandomSquare(bool) const;
	virtual void Draw(void) const;
	virtual void FastAddCharacter(vector, character*);
	virtual void HandleCharacters(void);
	virtual levelsquare* GetLevelSquare(vector Pos) const { return Map[Pos.X][Pos.Y]; }
	virtual void PutPlayer(bool);
	virtual void PutPlayerAround(vector Pos);
	virtual void GenerateTunnel(vector, vector, bool);
	virtual void PutStairs(vector);
	virtual void ExpandPossibleRoute(vector, vector, bool);
	virtual void ExpandStillPossibleRoute(vector, vector, bool);
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual void Luxify(void);
	virtual ushort GetPopulation(void) const { return Population; }
	virtual ushort CIdealPopulation(void) const { return 25; }
	virtual void GenerateNewMonsters(ushort);
	virtual void AttachPos(vector);
	virtual void CreateRandomTunnel(void);
	virtual void CreateItems(ushort);
	virtual void CreateMonsters(ushort);
	virtual vector CreateDownStairs(void);
	virtual bool MakeRoom(vector, vector, bool = true, uchar = 0);
	virtual bitmap* GetFluidBuffer(void) const { return FluidBuffer; }
	virtual vector GetUpStairs(void) { return UpStairs; }
	virtual vector GetDownStairs(void) { return DownStairs; }
	virtual void SetUpStairs(vector What) { UpStairs = What; }
	virtual void SetDownStairs(vector What) { DownStairs = What; }
	virtual void ParticleTrail(vector, vector);
protected:
	levelsquare*** Map;
	bitmap* FluidBuffer;
	dynarray<vector, uchar> KeyPoint, Door;
	vector UpStairs, DownStairs;
	ushort LevelIndex;
	ushort Population;
};

#endif
