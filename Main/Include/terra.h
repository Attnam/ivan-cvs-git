#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typeable.h"

class outputfile;
class inputfile;
class character;

class terrain
{
 public:
  virtual bool GetIsWalkable() const { return true; }
  virtual void StepOn(character*) {}
  virtual std::string SurviveMessage() const { return "somehow you survive"; }
  virtual std::string DeathMessage() const { return "strangely enough, you die"; }
  virtual std::string MonsterDeathVerb() const { return "dies"; }
  virtual std::string ScoreEntry() const { return "died on unfriendly terrain"; }
};

class groundterrain : public terrain
{
 public:
  virtual bool GetIsWalkable(character*) const;
  virtual ushort GetEntryAPRequirement() const = 0;
};

class overterrain : public terrain
{
 public:
  virtual bool GoUp(character*) const = 0;
  virtual bool GoDown(character*) const = 0;
  virtual uchar RestModifier() const { return 1; }
  virtual void ShowRestMessage(character*) const { }
};

#endif
