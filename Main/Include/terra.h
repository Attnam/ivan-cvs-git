#ifndef __TERRA_H__
#define __TERRA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

class character;

class terrain
{
 public:
  virtual bool IsWalkable() const { return true; }
  virtual void StepOn(character*) { }
  virtual std::string SurviveMessage() const;
  virtual std::string MonsterSurviveMessage() const;
  virtual std::string DeathMessage() const;
  virtual std::string MonsterDeathVerb() const;
  virtual std::string ScoreEntry() const;
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
