#ifndef __LEVEL_H__
#define __LEVEL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>
#include <queue>
#include <set>

#include "area.h"
#include "festring.h"

class levelscript;
class roomscript;
class squarescript;
class glterrain;
class olterrain;
class dungeon;
class lsquare;
class room;
struct node;

struct nodepointerstorer
{
  nodepointerstorer(node* Node) : Node(Node) { }
  bool operator<(const nodepointerstorer&) const;
  node* Node;
};

typedef std::priority_queue<nodepointerstorer> nodequeue;

struct node
{
  node(ushort x, ushort y, lsquare* Square) : Pos(x, y), Square(Square) { }
  void CalculateNextNodes();
  lsquare* Square;
  node* Last;
  vector2d Pos;
  ulong Distance;
  ulong Remaining;
  ulong TotalDistanceEstimate;
  ulong Diagonals;
  bool InNodeQueue;
  bool Processed;
  static node*** NodeMap;
  static uchar RequiredWalkability;
  static const character* SpecialMover;
  static vector2d To;
  static uchar** WalkabilityMap;
  static ushort XSize, YSize;
  static nodequeue* NodeQueue;
};

struct explosion
{
  character* Terrorist;
  festring DeathMsg;
  vector2d Pos;
  ulong ID;
  ushort Strength;
  ushort RadiusSquare;
  uchar Size;
  bool HurtNeutrals;
};

class level : public area
{
 public:
  level();
  virtual ~level();
  void Generate(ushort);
  vector2d GetRandomSquare(const character* = 0, uchar = 0, const rect* = 0) const;
  void GenerateMonsters();
  lsquare* GetLSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  lsquare* GetLSquare(ushort x, ushort y) const { return Map[x][y]; }
  void GenerateTunnel(ushort, ushort, ushort, ushort, bool);
  void ExpandPossibleRoute(ushort, ushort, ushort, ushort, bool);
  void ExpandStillPossibleRoute(ushort, ushort, ushort, ushort, bool);
  void Save(outputfile&) const;
  void Load(inputfile&);
  void FiatLux();
  ushort GetIdealPopulation() const { return IdealPopulation; }
  ushort GetDifficulty() const { return Difficulty; }
  void GenerateNewMonsters(ushort, bool = true);
  void AttachPos(ushort, ushort);
  void AttachPos(vector2d Pos) { AttachPos(Pos.X, Pos.Y); }
  void CreateItems(ushort);
  bool MakeRoom(const roomscript*);
  void ParticleTrail(vector2d, vector2d);
  festring GetLevelMessage() { return LevelMessage; }
  void SetLevelMessage(const festring& What) { LevelMessage = What; }
  void SetLevelScript(const levelscript* What) { LevelScript = What; }
  bool IsOnGround() const;
  const levelscript* GetLevelScript() const { return LevelScript; }
  ushort GetLOSModifier() const;
  ushort CalculateMinimumEmitationRadius(ulong) const;
  room* GetRoom(ushort) const;
  void SetRoom(ushort, room*);
  void AddRoom(room*);
  void Explosion(character*, const festring&, vector2d, ushort, bool = true);
  bool CollectCreatures(std::vector<character*>&, character*, bool);
  void ApplyLSquareScript(const squarescript*);
  virtual void Draw(bool) const;
  lsquare* GetNeighbourLSquare(vector2d, ushort) const;
  vector2d GetEntryPos(const character*, uchar) const;
  void GenerateRectangularRoom(std::vector<vector2d>&, std::vector<vector2d>&, std::vector<vector2d>&, const roomscript*, room*, vector2d, vector2d);
  void Reveal();
  static void (level::*GetBeam(ushort))(character*, const festring&, vector2d, ulong, uchar, uchar, uchar);
  void ParticleBeam(character*, const festring&, vector2d, ulong, uchar, uchar, uchar);
  void LightningBeam(character*, const festring&, vector2d, ulong, uchar, uchar, uchar);
  void ShieldBeam(character*, const festring&, vector2d, ulong, uchar, uchar, uchar);
  dungeon* GetDungeon() const { return Dungeon; }
  void SetDungeon(dungeon* What) { Dungeon = What; }
  uchar GetIndex() const { return Index; }
  void SetIndex(uchar What) { Index = What; }
  bool DrawExplosion(const explosion*) const;
  ushort TriggerExplosions(ushort);
  lsquare*** GetMap() const { return Map; }
  vector2d GetNearestFreeSquare(const character*, vector2d, bool = true) const;
  vector2d FreeSquareSeeker(const character*, vector2d, vector2d, uchar, bool) const;
  vector2d GetFreeAdjacentSquare(const character*, vector2d, bool) const;
  static void (level::*GetBeamEffectVisualizer(ushort))(const rect&, ushort) const;
  void ParticleVisualizer(const rect&, ushort) const;
  void LightningVisualizer(const rect&, ushort) const;
  bool PreProcessForBone();
  bool PostProcessForBone();
  void FinalProcessForBone();
  void GenerateDungeon(ushort);
  void GenerateDesert();
  void GenerateJungle();
  void GenerateSteppe();
  void GenerateLeafyForest();
  void GenerateEvergreenForest();
  void GenerateTundra();
  void GenerateGlacier();
  void CreateTunnelNetwork(ushort, ushort, ushort, ushort, vector2d);
  void SetWalkability(vector2d Pos, uchar What) { WalkabilityMap[Pos.X][Pos.Y] = What; }
  node* FindRoute(vector2d, vector2d, const std::set<vector2d>&, uchar, const character* = 0);
  void AddToAttachQueue(vector2d);
 protected:
  void GenerateLanterns(ushort, ushort, uchar) const;
  void CreateRoomSquare(glterrain*, olterrain*, ushort, ushort, uchar) const;
  lsquare*** Map;
  const levelscript* LevelScript;
  festring LevelMessage;
  std::vector<vector2d> Door;
  std::vector<room*> Room;
  uchar IdealPopulation;
  uchar MonsterGenerationInterval;
  uchar Difficulty;
  dungeon* Dungeon;
  uchar Index;
  std::vector<explosion*> ExplosionQueue;
  std::vector<bool> PlayerHurt;
  node*** NodeMap;
  uchar** WalkabilityMap;
  std::vector<vector2d> AttachQueue;
};

outputfile& operator<<(outputfile&, const level*);
inputfile& operator>>(inputfile&, level*&);

#endif
