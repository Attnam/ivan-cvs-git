#include "entity.h"
#include "game.h"

std::list<entityinfo> entitypool::Pool;

/*
 * Calls the Be() function of each self-changeable entity during each tick,
 * thus allowing acting characters, spoiling food etc.
 * Also handles removal of entities marked as dead by calling SetExists(false).
 */

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

/*
 * Brutally removes all entities.
 */

void entitypool::KillEverything()
{
  for(std::list<entityinfo>::iterator i = Pool.begin(); i != Pool.end();)
    {
      std::list<entityinfo>::iterator Dirt = i++;
      delete Dirt->Entity;
    }
}
