#include "pool.h"
#include "object.h"
#include "game.h"

std::list<object*> objectpool::Pool;

void objectpool::Be()
{
  for(std::list<object*>::iterator i = Pool.begin(); i != Pool.end(); ++i)
    if((*i)->GetExists())
      {
	(*i)->Be();

	if(!game::GetRunning())
	  return;
      }
}

void objectpool::BurnTheDead()
{
  for(std::list<object*>::iterator i = Pool.begin(); i != Pool.end();)
    if(!(*i)->GetExists())
      {
	std::list<object*>::iterator Dirt = i++;
	delete (*Dirt);
      }
    else
      ++i;
}
