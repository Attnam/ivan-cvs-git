#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#include <fstream>
#include <string>

#include "typedef.h"

#include "game.h"

/* Presentation of the dungeon class */

class level;

class dungeon
{
public:
	dungeon(void) {};
	dungeon(uchar);
	~dungeon(void);
	void Generate(void);
	void SaveLevel(std::string = game::SaveName(), ushort = game::GetCurrent(), bool = true);
	void LoadLevel(std::string = game::SaveName(), ushort = game::GetCurrent());
	level* GetLevel(uchar Index) const { return Level[Index]; }
	uchar GetLevels(void) const { return Levels; }
	void Save(std::ofstream&) const;
	void Load(std::ifstream&);
private:
	uchar*** BlockMap;
	level** Level;
	uchar Levels;
};

inline std::ofstream& operator<<(std::ofstream& SaveFile, dungeon* Dungeon)
{
	Dungeon->Save(SaveFile);
	return SaveFile;
}

#endif
