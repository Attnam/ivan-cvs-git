#ifndef __POOL_H__
#define __POOL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

class entity;

struct entityinfo
{
  entity* Entity;
  bool Exists;
  bool HasBe;
};

class entitypool
{
 public:
  static std::list<entityinfo>::iterator Add(entityinfo EntityInfo) { return Pool.insert(Pool.end(), EntityInfo); }
  static void Remove(std::list<entityinfo>::iterator Iterator) { Pool.erase(Iterator); }
  static void Be();
  static void BurnTheDead();
 private:
  static std::list<entityinfo> Pool;
};

#endif
