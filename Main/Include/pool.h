#ifndef __POOL_H__
#define __POOL_H__

#pragma warning(disable : 4786)

#include <list>

class object;

class objectpool
{
public:
	static std::list<object*>::iterator Add(object* Object) { return Pool.insert(Pool.end(), Object); }
	static void Remove(std::list<object*>::iterator Iterator) { Pool.erase(Iterator); }
	static void Be();
private:
	static std::list<object*> Pool;
};

#endif
