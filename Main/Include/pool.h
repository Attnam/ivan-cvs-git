#ifndef __POOL_H__
#define __POOL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

class object;

class objectpool
{
public:
	static std::list<object*>::iterator Add(object* Object) { return Pool.insert(Pool.end(), Object); }
	static void Remove(std::list<object*>::iterator Iterator) { Pool.erase(Iterator); }
	static void Be();
	static void BurnTheDead();
private:
	static std::list<object*> Pool;
};

#endif



