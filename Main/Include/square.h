/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

#ifndef __SQUARE_H__
#define __SQUARE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "area.h"
#include "festring.h"
#include "ivandef.h"

class gterrain;
class oterrain;
class festring;

class square
{
 public:
  square(area*, vector2d);
  virtual ~square();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void AddCharacter(character*);
  virtual void RemoveCharacter();
  virtual character* GetCharacter() const { return Character; }
  ulong GetLastSeen() const { return LastSeen; }
  vector2d GetPos() const { return Pos; }
  area* GetArea() const { return AreaUnder; }
  virtual gterrain* GetGTerrain() const = 0;
  virtual oterrain* GetOTerrain() const = 0;
  festring GetMemorizedDescription() { return MemorizedDescription; }
  void SetMemorizedDescription(const festring& What) { MemorizedDescription = What; }
  virtual bool CanBeSeenByPlayer(bool = false) const = 0;
  virtual bool CanBeSeenFrom(vector2d, long, bool = false) const = 0;
  void SendNewDrawRequest() { Flags |= NEW_DRAW_REQUEST; }
  void SendStrongNewDrawRequest() { Flags |= STRONG_NEW_DRAW_REQUEST; }
  const char* SurviveMessage(character*) const;
  const char* MonsterSurviveMessage(character*) const;
  const char* DeathMessage(character*) const;
  const char* MonsterDeathVerb(character*) const;
  const char* ScoreEntry(character*) const;
  bool IsFatalToStay() const;
  int GetEntryDifficulty() const;
  int GetRestModifier() const;
  void IncAnimatedEntities() { ++AnimatedEntities; }
  void DecAnimatedEntities() { --AnimatedEntities; }
  bool CanBeSeenBy(const character*, bool = false) const;
  color24 GetLuminance() const { return Luminance; }
  square* GetNeighbourSquare(int I) const { return AreaUnder->GetNeighbourSquare(Pos, I); }
  square* GetNearSquare(vector2d Pos) const { return AreaUnder->GetSquare(Pos); }
  virtual int GetSquareWalkability() const = 0;
  void SetCharacter(character* What) { Character = What; }
  void AddFlags(ulong What) { Flags |= What; }
  void RemoveFlags(ulong What) { Flags &= ~What; }
  virtual bool HasBeenSeen() const { return !!LastSeen; }
 protected:
  festring MemorizedDescription;
  area* AreaUnder;
  character* Character;
  vector2d Pos;
  color24 Luminance;
  ulong Flags;
  ushort AnimatedEntities;
  uchar LastSeen;
};

#endif
