#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "save.h"

#include "game.h"

/* Presentation of the dungeon class */

class level;
class outputfile;
class inputfile;
class dungeonscript;
class levelscript;

class dungeon
{
 public:
  dungeon() { }
  dungeon(uchar);
  ~dungeon();
  void PrepareLevel(ushort = game::GetCurrent());
  void SaveLevel(const std::string& = game::SaveName(), ushort = game::GetCurrent(), bool = true);
  void LoadLevel(const std::string& = game::SaveName(), ushort = game::GetCurrent());
  level* GetLevel(uchar Index) const { return Level[Index]; }
  uchar GetLevels() const;
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetIndex(uchar What) { Index = What; }
  uchar GetIndex() { return Index; }
  levelscript* GetLevelScript(ushort);
  vector2d GetWorldMapPos() { return WorldMapPos; }
  void SetWorldMapPos(vector2d What) { WorldMapPos = What; }
  std::string GetLevelDescription(uchar);
 private:
  void Initialize();
  dungeonscript* DungeonScript;
  level** Level;
  uchar Index;
  bool* Generated;
  vector2d WorldMapPos;
};

inline outputfile& operator<<(outputfile& SaveFile, dungeon* Dungeon)
{
  if(Dungeon)
    {
      SaveFile.Put(1);
      Dungeon->Save(SaveFile);
    }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, dungeon*& Dungeon)
{
  if(SaveFile.Get())
    {
      Dungeon = new dungeon;
      Dungeon->Load(SaveFile);
    }

  return SaveFile;
}

#endif


