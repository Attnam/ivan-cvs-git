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
  void Generate(ushort);
  vector2d GetRandomSquare(const character* = 0, uchar = 0, const rect* = 0) const;
  void GenerateMonsters();
  lsquare* GetLSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  lsquare* GetLSquare(ushort x, ushort y) const { return Map[x][y]; }
  void GenerateTunnel(vector2d, vector2d, bool);
  void ExpandPossibleRoute(vector2d, vector2d, bool);
  void ExpandStillPossibleRoute(vector2d, vector2d, bool);
  void Save(outputfile&) const;
  void Load(inputfile&);
  void FiatLux();
  ushort GetIdealPopulation() const { return IdealPopulation; }
  ushort GetDifficulty() const { return Difficulty; }
  void GenerateNewMonsters(ushort, bool = true);
  void AttachPos(vector2d);
  void CreateItems(ushort);
  bool MakeRoom(roomscript*);
  void ParticleTrail(vector2d, vector2d);
  std::string GetLevelMessage() { return LevelMessage; }
  void SetLevelMessage(const std::string& What) { LevelMessage = What; }
  void SetLevelScript(levelscript* What) { LevelScript = What; }
  bool GetOnGround() const;
  levelscript* GetLevelScript() const { return LevelScript; }
  virtual void MoveCharacter(vector2d, vector2d);
  virtual ushort GetLOSModifier() const;
  ushort CalculateMinimumEmitationRadius(ulong) const;
  room* GetRoom(ushort) const;
  void SetRoom(ushort Index, room* What) { Room[Index] = What; }
  void AddRoom(room*);
  void Explosion(character*, const std::string&, vector2d, ushort, bool = true);
  bool CollectCreatures(std::vector<character*>&, character*, bool);
  void ApplyLSquareScript(squarescript*);
  virtual void Draw() const;
  lsquare* GetNeighbourLSquare(vector2d, ushort) const;
  vector2d GetEntryPos(const character*, uchar) const;
  void GenerateRectangularRoom(std::vector<vector2d>&, std::vector<vector2d>&, std::vector<vector2d>&, roomscript*, room*, vector2d, vector2d);
  void Reveal();
 protected:
  void GenerateLanterns(ushort, ushort, uchar) const;
  void CreateRoomSquare(glterrain*, olterrain*, ushort, ushort, uchar, uchar) const;
  lsquare*** Map;
  levelscript* LevelScript;
  std::string LevelMessage;
  std::vector<vector2d> Door;
  std::vector<room*> Room;
  uchar IdealPopulation;
  uchar MonsterGenerationInterval;
  uchar Difficulty;
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
