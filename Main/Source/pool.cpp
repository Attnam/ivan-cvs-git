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
  for(CurrentEntity = Pool.begin(); CurrentEntity != Pool.end(); ++CurrentEntity)
    (*CurrentEntity)->Be();
}

void pool::Remove(std::list<entity*>::iterator Iterator)
{
  if(Iterator == CurrentEntity)
    ++CurrentEntity;

  Pool.erase(Iterator);
}
