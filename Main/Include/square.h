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
  virtual void DrawMemorized();
  virtual void Draw() = 0;
  virtual void UpdateMemorized() = 0;
  virtual bool DrawTerrain(bool) const = 0;
  virtual bool DrawCharacters(bool) const = 0;
  virtual void SetCharacter(character* What) { Character = What; }
  virtual void AddCharacter(character*);
  virtual void RemoveCharacter();
  virtual character* GetCharacter() const { return Character; }
  virtual ulong GetLastSeen() const 		{ return LastSeen; }
  virtual vector2d GetPos() const 	{ return Pos; }
  virtual void SetLastSeen(ulong);
  virtual ushort GetPopulation() const { if(Character) return 1; else return 0; }
  virtual area* GetAreaUnder() const { return AreaUnder; }
  virtual gterrain* GetGTerrain() const = 0;
  virtual oterrain* GetOTerrain() const = 0;
  virtual std::string GetMemorizedDescription() { return MemorizedDescription; }
  virtual void SetMemorizedDescription(const std::string& What) { MemorizedDescription = What; }
  virtual void UpdateMemorizedDescription(bool = false) = 0;
  virtual bool CanBeSeen(bool = false) const;
  virtual bool CanBeSeenFrom(vector2d, ulong, bool = false) const;
  virtual void SendNewDrawRequest() { NewDrawRequested = true; }
  virtual bitmap* GetMemorized() const { return Memorized; }
  virtual void SetDescriptionChanged(bool What) { DescriptionChanged = What; }
  virtual void StepOn(character*, square*) { }
  virtual void KickAnyoneStandingHereAway();
  virtual void SendMemorizedUpdateRequest() { MemorizedUpdateRequested = true; }
  virtual bool IsWalkable(character*) const;
  virtual std::string SurviveMessage(character*) const;
  virtual std::string DeathMessage(character*) const;
  virtual std::string MonsterDeathVerb(character*) const;
  virtual std::string ScoreEntry(character*) const;
  virtual ushort GetEntryAPRequirement() const;
  virtual uchar RestModifier() const;
  virtual ushort GetAnimatedEntities() const { return AnimatedEntities; }
  virtual void IncAnimatedEntities() { ++AnimatedEntities; }
  virtual void DecAnimatedEntities() { --AnimatedEntities; }
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


