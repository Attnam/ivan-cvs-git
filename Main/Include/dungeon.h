#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "vector2d.h"

class level;
class outputfile;
class inputfile;
class dungeonscript;
class levelscript;

class dungeon
{
 public:
  dungeon();
  dungeon(uchar);
  ~dungeon();
  bool PrepareLevel(ushort, bool = true);
  void SaveLevel(const std::string&, ushort, bool = true);
  level* LoadLevel(const std::string&, ushort);
  level* GetLevel(ushort Index) const { return Level[Index]; }
  ushort GetLevels() const;
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetIndex(uchar What) { Index = What; }
  uchar GetIndex() const { return Index; }
  const levelscript* GetLevelScript(ushort);
  vector2d GetWorldMapPos() { return WorldMapPos; }
  void SetWorldMapPos(vector2d What) { WorldMapPos = What; }
  std::string GetLevelDescription(ushort);
  std::string GetShortLevelDescription(ushort);
 private:
  void Initialize();
  const dungeonscript* DungeonScript;
  level** Level;
  uchar Index;
  bool* Generated;
  vector2d WorldMapPos;
};

outputfile& operator<<(outputfile&, const dungeon*);
inputfile& operator>>(inputfile&, dungeon*&);

#endif
