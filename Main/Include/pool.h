#ifndef __POOL_H__
#define __POOL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

class entity;

typedef std::list<entity*> entitylist;

class pool
{
 public:
  static entitylist::iterator Add(entity* Entity) { return Pool.insert(Pool.end(), Entity); }
  static void Remove(entitylist::iterator);
  static void AddToHell(entity* DoomedOne) { Hell.push_back(DoomedOne); }
  static void BurnHell();
  static void Be();
 private:
  static entitylist Pool;
  static entitylist::iterator CurrentEntity;
  static entitylist Hell;
};

#endif
