#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"

class level;
class outputfile;
class inputfile;
class dungeonscript;
class levelscript;
class festring;

class dungeon
{
 public:
  dungeon();
  dungeon(int);
  ~dungeon();
  bool PrepareLevel(int, bool = true);
  void SaveLevel(const festring&, int, bool = true);
  level* LoadLevel(const festring&, int);
  level* GetLevel(int I) const { return Level[I]; }
  int GetLevels() const;
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetIndex(int What) { Index = What; }
  int GetIndex() const { return Index; }
  const levelscript* GetLevelScript(int);
  vector2d GetWorldMapPos() { return WorldMapPos; }
  void SetWorldMapPos(vector2d What) { WorldMapPos = What; }
  festring GetLevelDescription(int);
  festring GetShortLevelDescription(int);
  level* LoadLevel(inputfile&, int);
  bool IsGenerated(int I) const { return Generated[I]; }
  void SetIsGenerated(int I, bool What) { Generated[I] = What; }
 private:
  void Initialize();
  const dungeonscript* DungeonScript;
  level** Level;
  int Index;
  bool* Generated;
  vector2d WorldMapPos;
};

outputfile& operator<<(outputfile&, const dungeon*);
inputfile& operator>>(inputfile&, dungeon*&);

#endif
