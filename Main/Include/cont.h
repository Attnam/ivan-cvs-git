#ifndef __CONT_H__
#define __CONT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>
#include <string>

#include "typedef.h"
#include "vector2d.h"
#include "save.h"

class worldmap;
class outputfile;
class inputfile;

class continent
{
public:
	friend class worldmap;
	continent() {}
	continent(uchar Index) : Index(Index) {}
	void AttachTo(continent*);
	void Add(vector2d);
	void Save(outputfile&) const;
	void Load(inputfile&);
	ulong Size() const { return Member.size(); }
	uchar GetIndex() const { return Index; }
	void GenerateInfo();
	std::string GetName() const { return Name; }
	ushort GetGroundTerrainAmount(ushort Type) { return GroundTerrainAmount[Type]; }
	vector2d GetRandomMember(ushort);
private:
	static ushort** TypeBuffer;
	static short** AltitudeBuffer;
	static uchar** ContinentBuffer;
	std::string Name;
	std::vector<vector2d> Member;
	std::vector<ushort> GroundTerrainAmount;
	uchar Index;
};

inline void continent::Add(vector2d Pos)
{
	Member.push_back(Pos);
	ContinentBuffer[Pos.X][Pos.Y] = Index;
}

inline outputfile& operator<<(outputfile& SaveFile, continent* Continent)
{
	if(Continent)
	{
		SaveFile.GetBuffer().put(1);
		Continent->Save(SaveFile);
	}
	else
		SaveFile.GetBuffer().put(0);

	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, continent*& Continent)
{
	if(SaveFile.GetBuffer().get())
	{
		Continent = new continent;
		Continent->Load(SaveFile);
	}

	return SaveFile;
}

#endif



