#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>

#include "dynarray.h"
#include "typedef.h"
#include "vector2d.h"

#include "area.h"

class levelsquare;
class character;
class area;
class bitmap;
class outputfile;
class inputfile;

/* Presentation of the level class */

class level : public area
{
public:
	level(ushort, ushort, ushort);
	level() {}
	virtual ~level() {}
	virtual void Generate();
	virtual vector2d RandomSquare(bool) const;
	virtual void Draw() const;
	virtual void FastAddCharacter(vector2d, character*);
	virtual void HandleCharacters();
	virtual levelsquare* GetLevelSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
	virtual levelsquare* GetLevelSquare(ushort x, ushort y) const { return Map[x][y]; }
	virtual void PutPlayer(bool);
	virtual void PutPlayerAround(vector2d Pos);
	virtual void GenerateTunnel(vector2d, vector2d, bool);
	virtual void PutStairs(vector2d);
	virtual void ExpandPossibleRoute(vector2d, vector2d, bool);
	virtual void ExpandStillPossibleRoute(vector2d, vector2d, bool);
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void Luxify();
	virtual ushort GetPopulation() const { return Population; }
	virtual ushort GetIdealPopulation() const { return 25; }
	virtual void GenerateNewMonsters(ushort);
	virtual void AttachPos(vector2d);
	virtual void CreateRandomTunnel();
	virtual void CreateItems(ushort);
	virtual void CreateMonsters(ushort);
	virtual vector2d CreateDownStairs();
	virtual bool MakeRoom(vector2d, vector2d, bool = true, uchar = 0);
	virtual vector2d GetUpStairs() { return UpStairs; }
	virtual vector2d GetDownStairs() { return DownStairs; }
	virtual void SetUpStairs(vector2d What) { UpStairs = What; }
	virtual void SetDownStairs(vector2d What) { DownStairs = What; }
	virtual void ParticleTrail(vector2d, vector2d);
	virtual std::string GetLevelMessage() { return LevelMessage; }
	virtual void SetLevelMessage(std::string What) { LevelMessage = What; }
protected:
	levelsquare*** Map;
	std::string LevelMessage;
	dynarray<vector2d, uchar> KeyPoint, Door;
	vector2d UpStairs, DownStairs;
	ushort LevelIndex;
	ushort Population;
};

#endif
