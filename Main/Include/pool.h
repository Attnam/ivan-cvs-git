#ifndef __POOL_H__
#define __POOL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

class entity;

class pool
{
 public:
  static std::list<entity*>::iterator Add(entity* Entity) { return Pool.insert(Pool.end(), Entity); }
  static void Remove(std::list<entity*>::iterator);
  static void Be();
 private:
  static std::list<entity*> Pool;
  static std::list<entity*>::iterator CurrentEntity;
};

#endif

