#ifndef __ENTITY_H__
#define __ENTITY_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

#include "typedef.h"
#include "pool.h"
#include "ivandef.h"

class square;
class lsquare;
class outputfile;
class inputfile;
class material;
class character;

class entity
{
 public:
  entity(uchar);
  entity(const entity&);
  virtual ~entity();
  virtual void Be() { }
  bool Exists() const { return (EntityFlags & EXISTS) != 0; }
  void SendToHell();
  bool IsEnabled() const { return (EntityFlags & HAS_BE) != 0; }
  void SetIsEnabled(bool What) { if(What) Enable(); else Disable(); }
  void Enable();
  void Disable();
  virtual square* GetSquareUnderEntity() const = 0;
  virtual void SignalVolumeAndWeightChange() { }
  ulong GetEmitation() const { return Emitation; }
  virtual void SignalEmitationIncrease(ulong) { }
  virtual void SignalEmitationDecrease(ulong) { }
  virtual bool ContentsCanBeSeenBy(const character*) const { return false; }
  virtual bool AllowSpoil() const { return false; }
  virtual void SignalSpoil(material*) { }
  virtual void SignalSpoilLevelChange(material*) { }
  virtual bool IsOnGround() const = 0;
  virtual bool AllowContentEmitation() const { return true; }
 protected:
  std::list<entity*>::iterator PoolIterator;
  ulong Emitation;
  uchar EntityFlags;
};

#endif
