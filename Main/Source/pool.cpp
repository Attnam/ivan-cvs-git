#include "pool.h"
#include "object.h"

std::list<object*> objectpool::Pool;

void objectpool::Be()
{
	for(std::list<object*>::iterator i = Pool.begin(); i != Pool.end(); ++i)
		if((*i)->GetExists())
			(*i)->Be();
		else
		{
			std::list<object*>::iterator Dirt = i++;
			delete (*Dirt);
		}
}



