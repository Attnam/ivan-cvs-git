#include "entity.h"
#include "game.h"

std::list<entityinfo> entitypool::Pool;

void entitypool::Be()
{
  for(std::list<entityinfo>::iterator i = Pool.begin(); i != Pool.end();)
    if(i->Exists)
      {
	if(i->HasBe)
	    i->Entity->Be();

	++i;
      }
    else
      {
	std::list<entityinfo>::iterator Dirt = i++;
	delete Dirt->Entity;
      }
}

void entitypool::BurnTheDead()
{
  for(std::list<entityinfo>::iterator i = Pool.begin(); i != Pool.end();)
    if(i->Exists)
      {
	++i;
      }
    else
      {
	std::list<entityinfo>::iterator Dirt = i++;
	delete Dirt->Entity;
      }
}
