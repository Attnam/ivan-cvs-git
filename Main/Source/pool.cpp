#include "pool.h"
#include "object.h"

std::list<object*> objectpool::Pool;

void objectpool::Be(void)
{
	for(std::list<object*>::iterator i = Pool.begin(); i != Pool.end(); i++)
		(*i)->Be();
}
