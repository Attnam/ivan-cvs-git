#ifndef __LEVEL_H__
#define __LEVEL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "typedef.h"
#include "vector2d.h"
#include "area.h"
#include "roomba.h"

class lsquare;
class character;
class area;
class bitmap;
class outputfile;
class inputfile;
class levelscript;
class roomscript;
class squarescript;
class glterrain;
class olterrain;

class level : public area
{
 public:
  level() : Room(1, (room*)0) { }
  virtual ~level();
  void Generate();
  vector2d RandomSquare(character*, bool, bool = false) const;
  void FastAddCharacter(vector2d, character*);
  void GenerateMonsters();
  lsquare* GetLSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  lsquare* GetLSquare(ushort x, ushort y) const { return Map[x][y]; }
  void GenerateTunnel(vector2d, vector2d, bool);
  void ExpandPossibleRoute(vector2d, vector2d, bool);
  void ExpandStillPossibleRoute(vector2d, vector2d, bool);
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Luxify();
  ushort GetIdealPopulation() const;
  void GenerateNewMonsters(ushort, bool = true);
  void AttachPos(vector2d);
  void CreateRandomTunnel();
  void CreateItems(ushort);
  void CreateStairs(bool);
  bool MakeRoom(roomscript*);
  vector2d GetUpStairs() { return UpStairs; }
  vector2d GetDownStairs() { return DownStairs; }
  vector2d GetWorldMapEntry() { return WorldMapEntry; }
  void SetUpStairs(vector2d What) { UpStairs = What; }
  void SetDownStairs(vector2d What) { DownStairs = What; }
  void SetWorldMapEntry(vector2d What) { WorldMapEntry = What; }
  void ParticleTrail(vector2d, vector2d);
  std::string GetLevelMessage() { return LevelMessage; }
  void SetLevelMessage(const std::string& What) { LevelMessage = What; }
  void SetLevelScript(levelscript* What) { LevelScript = What; }
  bool GetOnGround() const;
  bool IsValid(vector2d) const;
  levelscript* GetLevelScript() const { return LevelScript; }
  virtual void MoveCharacter(vector2d, vector2d);
  virtual ushort GetLOSModifier() const;
  ushort CalculateMinimumEmitationRadius(ushort) const;
  room* GetRoom(ushort) const;
  void SetRoom(ushort Index, room* What) { Room[Index] = What; }
  void AddRoom(room*);
  void Explosion(character*, const std::string&, vector2d, ushort, bool = true);
  bool CollectCreatures(std::vector<character*>&, character*, bool);
  void ApplyLSquareScript(squarescript*);
  virtual void Draw() const;
 protected:
  void GenerateLanterns(ushort, ushort, uchar) const;
  void CreateWallSquare(glterrain*, olterrain*, ushort, ushort, uchar, uchar) const;
  lsquare*** Map;
  levelscript* LevelScript;
  std::string LevelMessage;
  std::vector<vector2d> Door;
  vector2d UpStairs, DownStairs, WorldMapEntry;
  std::vector<room*> Room;
};

inline outputfile& operator<<(outputfile& SaveFile, level* Level)
{
  Level->Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, level*& Level)
{
  Level = new level;
  Level->Load(SaveFile);
  return SaveFile;
}

#endif


