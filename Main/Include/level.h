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
#include "ivandef.h"

class levelscript;
class roomscript;
class squarescript;
class glterrain;
class olterrain;
class dungeon;
class lsquare;
class room;
class item;
class liquid;
class level;
class material;
struct node;
struct emitter;
template <class type> struct fearray;

struct nodepointerstorer
{
  nodepointerstorer(node* Node) : Node(Node) { }
  bool operator<(const nodepointerstorer&) const;
  node* Node;
};

typedef std::priority_queue<nodepointerstorer> nodequeue;
typedef std::vector<item*> itemvector;
typedef std::vector<character*> charactervector;
typedef std::vector<emitter> emittervector;
typedef std::vector<ulong> sunemittervector;

struct node
{
  node(int x, int y, lsquare* Square) : Square(Square), Pos(x, y) { }
  void CalculateNextNodes();
  lsquare* Square;
  node* Last;
  vector2d Pos;
  long Distance;
  long Remaining;
  long TotalDistanceEstimate;
  long Diagonals;
  bool InNodeQueue;
  bool Processed;
  static node*** NodeMap;
  static int RequiredWalkability;
  static const character* SpecialMover;
  static vector2d To;
  static uchar** WalkabilityMap;
  static int XSize, YSize;
  static nodequeue* NodeQueue;
};

struct explosion
{
  character* Terrorist;
  festring DeathMsg;
  vector2d Pos;
  ulong ID;
  int Strength;
  int RadiusSquare;
  int Size;
  bool HurtNeutrals;
};

class level : public area
{
 public:
  level();
  virtual ~level();
  void Generate(int);
  vector2d GetRandomSquare(const character* = 0, int = 0, const rect* = 0) const;
  void GenerateMonsters();
  lsquare* GetLSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
  lsquare* GetLSquare(int x, int y) const { return Map[x][y]; }
  void GenerateTunnel(int, int, int, int, bool);
  void ExpandPossibleRoute(int, int, int, int, bool);
  void ExpandStillPossibleRoute(int, int, int, int, bool);
  void Save(outputfile&) const;
  void Load(inputfile&);
  void FiatLux();
  int GetIdealPopulation() const { return IdealPopulation; }
  double GetDifficulty() const { return Difficulty; }
  void GenerateNewMonsters(int, bool = true);
  void AttachPos(int, int);
  void AttachPos(vector2d Pos) { AttachPos(Pos.X, Pos.Y); }
  void CreateItems(int);
  bool MakeRoom(const roomscript*);
  void ParticleTrail(vector2d, vector2d);
  festring GetLevelMessage() { return LevelMessage; }
  void SetLevelMessage(const festring& What) { LevelMessage = What; }
  void SetLevelScript(const levelscript* What) { LevelScript = What; }
  bool IsOnGround() const;
  const levelscript* GetLevelScript() const { return LevelScript; }
  int GetLOSModifier() const;
  room* GetRoom(int) const;
  void SetRoom(int, room*);
  void AddRoom(room*);
  void Explosion(character*, const festring&, vector2d, int, bool = true);
  bool CollectCreatures(charactervector&, character*, bool);
  void ApplyLSquareScript(const squarescript*);
  virtual void Draw(bool) const;
  vector2d GetEntryPos(const character*, int) const;
  void GenerateRectangularRoom(std::vector<vector2d>&, std::vector<vector2d>&, std::vector<vector2d>&, const roomscript*, room*, vector2d, vector2d);
  void Reveal();
  static void (level::*GetBeam(int))(character*, const festring&, vector2d, color16, int, int, int);
  void ParticleBeam(character*, const festring&, vector2d, color16, int, int, int);
  void LightningBeam(character*, const festring&, vector2d, color16, int, int, int);
  void ShieldBeam(character*, const festring&, vector2d, color16, int, int, int);
  dungeon* GetDungeon() const { return Dungeon; }
  void SetDungeon(dungeon* What) { Dungeon = What; }
  int GetIndex() const { return Index; }
  void SetIndex(int What) { Index = What; }
  bool DrawExplosion(const explosion*) const;
  int TriggerExplosions(int);
  lsquare*** GetMap() const { return Map; }
  vector2d GetNearestFreeSquare(const character*, vector2d, bool = true) const;
  vector2d FreeSquareSeeker(const character*, vector2d, vector2d, int, bool) const;
  vector2d GetFreeAdjacentSquare(const character*, vector2d, bool) const;
  static void (level::*GetBeamEffectVisualizer(int))(const fearray<lsquare*>&, color16) const;
  void ParticleVisualizer(const fearray<lsquare*>&, color16) const;
  void LightningVisualizer(const fearray<lsquare*>&, color16) const;
  bool PreProcessForBone();
  bool PostProcessForBone();
  void FinalProcessForBone();
  void GenerateDungeon(int);
  void GenerateDesert();
  void GenerateJungle();
  void GenerateSteppe();
  void GenerateLeafyForest();
  void GenerateEvergreenForest();
  void GenerateTundra();
  void GenerateGlacier();
  void CreateTunnelNetwork(int, int, int, int, vector2d);
  void SetWalkability(vector2d Pos, int What) { WalkabilityMap[Pos.X][Pos.Y] = What; }
  node* FindRoute(vector2d, vector2d, const std::set<vector2d>&, int, const character* = 0);
  void AddToAttachQueue(vector2d);
  void CollectEverything(itemvector&, charactervector&);
  void CreateGlobalRain(liquid*, vector2d);
  void CheckSunLight();
  color24 GetSunLightEmitation() const { return SunLightEmitation; }
  void InitSquarePartEmitationTicks();
  color24 GetAmbientLuminance() const { return AmbientLuminance; }
  void ForceEmitterNoxify(const emittervector&) const;
  void ForceEmitterEmitation(const emittervector&, const sunemittervector&, ulong = 0) const;
  void UpdateLOS();
  void EnableGlobalRain();
  void DisableGlobalRain();
  void InitLastSeen();
  lsquare** GetSquareStack() const { return SquareStack; }
  color24 GetNightAmbientLuminance() const { return NightAmbientLuminance; }
  int DetectMaterial(const material*);
  void BlurMemory();
  void CalculateLuminances();
  int AddRadiusToSquareStack(vector2d, long) const;
  olterrain* GetRandomFountainWithWater(olterrain*) const;
 protected:
  bool GenerateLanterns(int, int, int) const;
  bool GenerateWindows(int, int) const;
  void CreateRoomSquare(glterrain*, olterrain*, int, int, int, int) const;
  void EmitSunBeams();
  void EmitSunBeam(vector2d, ulong, int) const;
  void ChangeSunLight();
  void EmitSunLight(vector2d);
  lsquare*** Map;
  const levelscript* LevelScript;
  festring LevelMessage;
  std::vector<vector2d> Door;
  std::vector<room*> Room;
  int IdealPopulation;
  int MonsterGenerationInterval;
  double Difficulty;
  dungeon* Dungeon;
  int Index;
  std::vector<explosion*> ExplosionQueue;
  std::vector<bool> PlayerHurt;
  node*** NodeMap;
  uchar** WalkabilityMap;
  std::vector<vector2d> AttachQueue;
  liquid* GlobalRainLiquid;
  vector2d GlobalRainSpeed;
  color24 SunLightEmitation;
  vector2d SunLightDirection;
  color24 AmbientLuminance;
  static ulong NextExplosionID;
  lsquare** SquareStack;
  color24 NightAmbientLuminance;
};

outputfile& operator<<(outputfile&, const level*);
inputfile& operator>>(inputfile&, level*&);

#endif
