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
class dungeonscript;

class dungeon
{
public:
	dungeon() {}
	dungeon(uchar);// : Generated(false) {};
	~dungeon();
	void PrepareLevel(ushort = game::GetCurrent());
	void SaveLevel(std::string = game::SaveName(), ushort = game::GetCurrent(), bool = true);
	void LoadLevel(std::string = game::SaveName(), ushort = game::GetCurrent());
	level* GetLevel(uchar Index) const { return Level[Index]; }
	uchar GetLevels() const;
	void Save(outputfile&) const;
	void Load(inputfile&);
	//void GenerateIfNeeded() { if(!Generated) Generate(); }
	void SetIndex(uchar What) { Index = What; }
	uchar GetIndex() { return Index; }
private:
	void Initialize();
	//uchar*** BlockMap;
	dungeonscript* DungeonScript;
	level** Level;
	//uchar Levels;
	//bool Generated;
	uchar Index;
	bool* Generated;
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
