#ifndef __POOL_H__
#define __POOL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

class entity;

struct entityinfo
{
  entityinfo(entity* Entity) : Entity(Entity), Exists(true), HasBe(false) { }
  entity* Entity;
  bool Exists;
  bool HasBe;
};

class entitypool
{
 public:
  static std::list<entityinfo>::iterator Add(entity* Entity) { return Pool.insert(Pool.end(), entityinfo(Entity)); }
  static void Remove(std::list<entityinfo>::iterator Iterator) { Pool.erase(Iterator); }
  static void Be();
  static void BurnTheDead();
 private:
  static std::list<entityinfo> Pool;
};

#endif
