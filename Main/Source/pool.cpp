#include "entity.h"
#include "game.h"

std::list<entity*> pool::Pool;
std::list<entity*>::iterator pool::CurrentEntity = pool::Pool.end();

/*
 * Calls the Be() function of each self-changeable entity during each tick,
 * thus allowing acting characters, spoiling food etc.
 * Also handles removal of entities marked as dead by calling SendToHell().
 */

void pool::Be()
{
  //static std::list<entity*>::iterator Dirt;

  for(CurrentEntity = Pool.begin(); CurrentEntity != Pool.end(); ++CurrentEntity)
    (*CurrentEntity)->Be();

    /*if((*i)->Exists())
      {
	//if((*i)->HasBe())
	(*i)->Be();

	++i;
      }
    else
      {
	Dirt = i++;
	delete *Dirt;
      }*/
}

void pool::Remove(std::list<entity*>::iterator Iterator)
{
  if(Iterator == CurrentEntity)
    ++CurrentEntity;

  Pool.erase(Iterator);
}

/*
 * Brutally removes all entities marked as dead.
 */

/*void entitypool::BurnTheDead()
{
  static std::list<entity*>::iterator Dirt;
  ulong NewSize = 0, OldSize = Pool.size();

  while(NewSize != OldSize)
    {
      OldSize = Pool.size();

      for(std::list<entity*>::iterator i = Pool.begin(); i != Pool.end();)
	if((*i)->Exists())
	  ++i;
	else
	  {
	    Dirt = i++;
	    delete *Dirt;
	  }

      NewSize = Pool.size();
    }
}*/

