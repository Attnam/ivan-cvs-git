#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#include <string>

#include "typedef.h"
#include "save.h"

#include "game.h"

/* Presentation of the dungeon class */

class level;
class outputfile;
class inputfile;

class dungeon
{
public:
	dungeon() : Generated(false) {};
	~dungeon();
	void Generate(uchar);
	void SaveLevel(std::string = game::SaveName(), ushort = game::GetCurrent(), bool = true);
	void LoadLevel(std::string = game::SaveName(), ushort = game::GetCurrent());
	level* GetLevel(uchar Index) const { return Level[Index]; }
	uchar GetLevels() const { return Levels; }
	void Save(outputfile&) const;
	void Load(inputfile&);
	void GenerateIfNeeded() { if(!Generated) Generate(Index); }
	void SetIndex(uchar What) { Index = What; }
	uchar GetIndex() { return Index; }
private:
	uchar*** BlockMap;
	level** Level;
	uchar Levels;
	bool Generated;
	uchar Index;
};

inline outputfile& operator<<(outputfile& SaveFile, dungeon* Dungeon)
{
	if(Dungeon)
	{
		SaveFile.GetBuffer().put(1);
		Dungeon->Save(SaveFile);
	}
	else
		SaveFile.GetBuffer().put(0);

	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, dungeon*& Dungeon)
{
	if(SaveFile.GetBuffer().get())
	{
		Dungeon = new dungeon;
		Dungeon->Load(SaveFile);
	}

	return SaveFile;
}

#endif
