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

/* Presentation of the level class */

class level : public area
{
 public:
  level() : Room(1, (room*)0) { }
  virtual ~level() { }
  virtual void Generate();
  virtual vector2d RandomSquare(character*, bool, bool = false) const;
  virtual void FastAddCharacter(vector2d, character*);
  virtual void GenerateMonsters();
  virtual lsquare* GetLSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  virtual lsquare* GetLSquare(ushort x, ushort y) const { return Map[x][y]; }
  virtual void GenerateTunnel(vector2d, vector2d, bool);
  virtual void ExpandPossibleRoute(vector2d, vector2d, bool);
  virtual void ExpandStillPossibleRoute(vector2d, vector2d, bool);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Luxify();
  virtual ushort GetIdealPopulation() const;
  virtual void GenerateNewMonsters(ushort, bool = true);
  virtual void AttachPos(vector2d);
  virtual void CreateRandomTunnel();
  virtual void CreateItems(ushort);
  virtual void CreateStairs(bool);
  virtual bool MakeRoom(roomscript*);
  virtual vector2d GetUpStairs() { return UpStairs; }
  virtual vector2d GetDownStairs() { return DownStairs; }
  virtual vector2d GetWorldMapEntry() { return WorldMapEntry; }
  virtual void SetUpStairs(vector2d What) { UpStairs = What; }
  virtual void SetDownStairs(vector2d What) { DownStairs = What; }
  virtual void SetWorldMapEntry(vector2d What) { WorldMapEntry = What; }
  virtual void ParticleTrail(vector2d, vector2d);
  virtual std::string GetLevelMessage() { return LevelMessage; }
  virtual void SetLevelMessage(const std::string& What) { LevelMessage = What; }
  virtual void SetLevelScript(levelscript* What) { LevelScript = What; }
  virtual bool GetOnGround() const;
  virtual bool IsValid(vector2d) const;
  virtual levelscript* GetLevelScript() const { return LevelScript; }
  virtual void MoveCharacter(vector2d, vector2d);
  virtual ushort GetLOSModifier() const;
  virtual ushort CalculateMinimumEmitationRadius(ushort) const;
  virtual room* GetRoom(ushort) const;
  virtual void SetRoom(ushort Index, room* What) { Room[Index] = What; }
  virtual void AddRoom(room*);
  virtual void Explosion(character*, const std::string&, vector2d, ushort, bool = true);
  virtual bool CollectCreatures(std::vector<character*>&, character*, bool);
  virtual void ApplyLSquareScript(squarescript*);
 protected:
  lsquare*** Map;
  levelscript* LevelScript;
  std::string LevelMessage;
  std::vector<vector2d> Door;
  vector2d UpStairs, DownStairs, WorldMapEntry;
  std::vector<room*> Room;
};

#endif


