#include "object.h"
#include "game.h"

std::list<objectinfo> objectpool::Pool;

void objectpool::Be()
{
  for(std::list<objectinfo>::iterator i = Pool.begin(); i != Pool.end();)
    if(i->Exists)
      {
	if(i->HasBe)
	    i->Object->Be();

	++i;
      }
    else
      {
	std::list<objectinfo>::iterator Dirt = i++;
	delete Dirt->Object;
      }
}

void objectpool::BurnTheDead()
{
  for(std::list<objectinfo>::iterator i = Pool.begin(); i != Pool.end();)
    if(i->Exists)
      {
	++i;
      }
    else
      {
	std::list<objectinfo>::iterator Dirt = i++;
	delete Dirt->Object;
      }
}
