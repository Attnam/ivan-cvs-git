#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#include "area.h"

class worldmapsquare;

class worldmap : public area
{
public:
	worldmap(ushort, ushort);
	worldmap(void) {}
	//worldmap(std::ifstream*);
	~worldmap(void);
	virtual void Generate(void);
	virtual void Draw(void) const;
	virtual void Save(std::ofstream*) const;
	virtual void Load(std::ifstream*);
	virtual worldmapsquare* GetWorldMapSquare(vector Pos) const {return Map[Pos.X][Pos.Y];}
protected:
	worldmapsquare*** Map;
};

#endif
