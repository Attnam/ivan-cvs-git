#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

class outputfile;
class inputfile;
class character;

class terrain
{
 public:
  virtual bool IsWalkable() const { return true; }
  virtual void StepOn(character*) { }
  virtual std::string SurviveMessage() const { return "somehow you survive"; }
  virtual std::string MonsterSurviveMessage() const { return "somehow survives"; }
  virtual std::string DeathMessage() const { return "strangely enough, you die"; }
  virtual std::string MonsterDeathVerb() const { return "dies"; }
  virtual std::string ScoreEntry() const { return "died on unfriendly terrain"; }
  virtual bool IsFatalToStay() const { return false; }
};

class gterrain : public terrain
{
 public:
  virtual bool IsWalkable(const character*) const;
  virtual uchar GetEntryDifficulty() const = 0;
};

class oterrain : public terrain
{
 public:
  virtual bool Enter(bool) const = 0;
  virtual uchar GetRestModifier() const { return 1; }
  virtual void ShowRestMessage(character*) const { }
};

#endif



