#ifndef __SQUARE_H__
#define __SQUARE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"
#include "vector2d.h"

class area;
class material;
class bitmap;
class character;
class gterrain;
class oterrain;
class outputfile;
class inputfile;
class squarescript;

class square
{
 public:
  square(area*, vector2d);
  virtual ~square();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void DrawMemorized();
  void SetCharacter(character* What) { Character = What; }
  virtual void AddCharacter(character*);
  virtual void RemoveCharacter();
  virtual character* GetCharacter() const { return Character; }
  ulong GetLastSeen() const { return LastSeen; }
  vector2d GetPos() const { return Pos; }
  ushort GetPopulation() const { return Character ? 1 : 0; }
  area* GetAreaUnder() const { return AreaUnder; }
  virtual gterrain* GetGTerrain() const = 0;
  virtual oterrain* GetOTerrain() const = 0;
  std::string GetMemorizedDescription() { return MemorizedDescription; }
  void SetMemorizedDescription(const std::string& What) { MemorizedDescription = What; }
  virtual bool CanBeSeenByPlayer(bool = false) const;
  virtual bool CanBeSeenFrom(vector2d, ulong, bool = false) const;
  void SendNewDrawRequest() { NewDrawRequested = true; }
  bitmap* GetMemorized() const { return Memorized; }
  void SetDescriptionChanged(bool What) { DescriptionChanged = What; }
  void KickAnyoneStandingHereAway();
  void SendMemorizedUpdateRequest() { MemorizedUpdateRequested = true; }
  bool IsWalkable(character*) const;
  std::string SurviveMessage(character*) const;
  std::string DeathMessage(character*) const;
  std::string MonsterDeathVerb(character*) const;
  std::string ScoreEntry(character*) const;
  uchar GetEntryDifficulty() const;
  uchar RestModifier() const;
  ushort GetAnimatedEntities() const { return AnimatedEntities; }
  void IncAnimatedEntities() { ++AnimatedEntities; }
  void DecAnimatedEntities() { --AnimatedEntities; }
  bool CanBeSeenBy(character*) const;
 protected:
  std::string MemorizedDescription;
  area* AreaUnder;
  character* Character;
  vector2d Pos;
  bool NewDrawRequested, MemorizedUpdateRequested;
  bitmap* Memorized;
  ulong LastSeen;
  bool DescriptionChanged;
  ushort AnimatedEntities;
};

inline outputfile& operator<<(outputfile& SaveFile, square* Square)
{
  Square->Save(SaveFile);
  return SaveFile;
}

#endif


