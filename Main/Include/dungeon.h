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
  dungeon(uchar);
  ~dungeon();
  bool PrepareLevel(ushort, bool = true);
  void SaveLevel(const festring&, ushort, bool = true);
  level* LoadLevel(const festring&, ushort);
  level* GetLevel(ushort Index) const { return Level[Index]; }
  ushort GetLevels() const;
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetIndex(uchar What) { Index = What; }
  uchar GetIndex() const { return Index; }
  const levelscript* GetLevelScript(ushort);
  vector2d GetWorldMapPos() { return WorldMapPos; }
  void SetWorldMapPos(vector2d What) { WorldMapPos = What; }
  festring GetLevelDescription(ushort);
  festring GetShortLevelDescription(ushort);
  level* LoadLevel(inputfile&, ushort);
  bool IsGenerated(ushort Index) const { return Generated[Index]; }
  void SetIsGenerated(ushort Index, bool What) { Generated[Index] = What; }
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
