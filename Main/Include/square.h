#ifndef __SQUARE_H__
#define __SQUARE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "area.h"

class gterrain;
class oterrain;

class square
{
 public:
  square(area*, vector2d);
  virtual ~square();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetCharacter(character* What) { Character = What; }
  virtual void AddCharacter(character*);
  virtual void RemoveCharacter();
  virtual character* GetCharacter() const { return Character; }
  ulong GetLastSeen() const { return LastSeen; }
  vector2d GetPos() const { return Pos; }
  area* GetArea() const { return AreaUnder; }
  virtual gterrain* GetGTerrain() const = 0;
  virtual oterrain* GetOTerrain() const = 0;
  std::string GetMemorizedDescription() { return MemorizedDescription; }
  void SetMemorizedDescription(const std::string& What) { MemorizedDescription = What; }
  virtual bool CanBeSeenByPlayer(bool = false) const;
  virtual bool CanBeSeenFrom(vector2d, ulong, bool = false) const;
  void SendNewDrawRequest() { NewDrawRequested = true; }
  std::string SurviveMessage(character*) const;
  std::string MonsterSurviveMessage(character*) const;
  std::string DeathMessage(character*) const;
  std::string MonsterDeathVerb(character*) const;
  std::string ScoreEntry(character*) const;
  bool IsFatalToStay() const;
  uchar GetEntryDifficulty() const;
  uchar GetRestModifier() const;
  ushort GetAnimatedEntities() const { return AnimatedEntities; }
  void IncAnimatedEntities() { ++AnimatedEntities; }
  void DecAnimatedEntities() { --AnimatedEntities; }
  bool CanBeSeenBy(const character*, bool = false) const;
  ulong GetLuminance() const { return Luminance; }
  square* GetNeighbourSquare(ushort Index) const { return AreaUnder->GetNeighbourSquare(Pos, Index); }
  square* GetNearSquare(vector2d Pos) const { return AreaUnder->GetSquare(Pos); }
  virtual bool SquareIsWalkable(const character* = 0) const = 0;
  virtual void DisplaySmokeInfo(std::string&) const { }
 protected:
  std::string MemorizedDescription;
  area* AreaUnder;
  character* Character;
  vector2d Pos;
  bool NewDrawRequested;
  ulong LastSeen;
  ushort AnimatedEntities;
  ulong Luminance;
  bool DescriptionChanged;
};

#endif
