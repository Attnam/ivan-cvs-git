#ifndef __CONT_H__
#define __CONT_H__

#include <vector>
#include <string>
#include <fstream>

#include "typedef.h"
#include "vector.h"

class worldmap;

class continent
{
public:
	friend class worldmap;
	continent(void) {}
	continent(uchar Index) : Index(Index) {}
	void AttachTo(continent&);
	void Add(vector);
	void Save(std::ofstream&) const;
	void Load(std::ifstream&);
	ulong Size(void) const { return Member.size(); }
	uchar GetIndex(void) const { return Index; }
private:
	static uchar** ContinentBuffer;
	std::string Name;
	std::vector<vector> Member;
	uchar Index;
};

inline void continent::Add(vector Pos)
{
	Member.push_back(Pos);
	ContinentBuffer[Pos.X][Pos.Y] = Index;
}

inline std::ofstream& operator<<(std::ofstream& SaveFile, continent& Continent)
{
	Continent.Save(SaveFile);
	return SaveFile;
}

inline std::ifstream& operator>>(std::ifstream& SaveFile, continent& Continent)
{
	Continent.Load(SaveFile);
	return SaveFile;
}

#endif
