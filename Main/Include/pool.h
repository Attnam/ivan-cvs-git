#ifndef __POOL_H__
#define __POOL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

class object;

struct objectinfo
{
  object* Object;
  bool Exists;
  bool HasBe;
};

class objectpool
{
 public:
  static std::list<objectinfo>::iterator Add(objectinfo ObjectInfo) { return Pool.insert(Pool.end(), ObjectInfo); }
  static void Remove(std::list<objectinfo>::iterator Iterator) { Pool.erase(Iterator); }
  static void Be();
  static void BurnTheDead();
 private:
  static std::list<objectinfo> Pool;
};

#endif
