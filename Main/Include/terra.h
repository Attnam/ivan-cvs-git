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
  virtual void StepOn(character*) { }
  virtual const char* SurviveMessage() const;
  virtual const char* MonsterSurviveMessage() const;
  virtual const char* DeathMessage() const;
  virtual const char* MonsterDeathVerb() const;
  virtual const char* ScoreEntry() const;
  virtual bool IsFatalToStay() const { return false; }
};

class gterrain : public terrain
{
 public:
  virtual bool IsWalkable(const character* = 0) const;
  virtual uchar GetEntryDifficulty() const = 0;
};

class oterrain : public terrain
{
 public:
  virtual bool IsWalkable(const character* = 0) const { return true; }
  virtual bool Enter(bool) const = 0;
  virtual uchar GetRestModifier() const { return 1; }
  virtual void ShowRestMessage(character*) const { }
};

#endif
