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

class entity
{
 public:
  entity(bool);
  virtual ~entity();
  virtual void Be() { }
  bool Exists() const { return ExistsBool; }
  void SendToHell();
  bool HasBe() const { return HasBeBool; }
  void SetHasBe(bool);
  bool IsEnabled() const { return Exists() && HasBe(); }
  ulong GetVolume() const { return Volume; }
  void SetVolume(ulong What) { Volume = What; }
  virtual void EditVolume(long What) { Volume += What; }
  ulong GetWeight() const { return Weight; }
  void SetWeight(ulong What) { Weight = What; }
  virtual void EditWeight(long What) { Weight += What; }
  virtual square* GetSquareUnder() const = 0;
 protected:
  std::list<entity*>::iterator PoolIterator;
  bool InPool;
  bool ExistsBool;
  bool HasBeBool;
  ulong Volume;
  ulong Weight;
};

#endif

