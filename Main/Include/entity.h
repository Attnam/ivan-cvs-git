#ifndef __ENTITY_H__
#define __ENTITY_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

#include "typedef.h"
#include "pool.h"

class square;
class lsquare;
class outputfile;
class inputfile;
class material;
class character;

class entity
{
 public:
  entity(bool);
  entity(const entity&);
  virtual ~entity();
  virtual void Be() { }
  bool Exists() const { return ExistsBool; }
  void SendToHell();
  bool HasBe() const { return HasBeBool; }
  void SetHasBe(bool);
  bool IsEnabled() const { return Exists() && HasBe(); }
  virtual square* GetSquareUnder() const = 0;
  virtual void SignalVolumeAndWeightChange() { }
  ulong GetEmitation() const { return Emitation; }
  virtual void SignalEmitationIncrease(ulong) { }
  virtual void SignalEmitationDecrease(ulong) { }
  virtual bool ContentsCanBeSeenBy(const character*) const { return false; }
  virtual bool AllowSpoil() const { return false; }
  virtual void SignalSpoil(material*) { }
  virtual void SignalSpoilLevelChange(material*) { }
  virtual bool IsOnGround() const = 0;
 protected:
  std::list<entity*>::iterator PoolIterator;
  bool ExistsBool;
  bool HasBeBool;
  ulong Emitation;
};

#endif
