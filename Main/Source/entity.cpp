#include "entity.h"
#include "game.h"
#include "lsquare.h"
#include "hell.h"
#include "materba.h"
#include "proto.h"

entity::entity(const entity& Entity) : Emitation(Entity.Emitation), EntityFlags(Entity.EntityFlags)
{
  if(EntityFlags & HAS_BE)
    PoolIterator = pool::Add(this);
}

entity::entity(uchar EntityFlags) : Emitation(0), EntityFlags(EntityFlags|EXISTS)
{
  if(EntityFlags & HAS_BE)
    PoolIterator = pool::Add(this);
}

entity::~entity()
{
  if(EntityFlags & HAS_BE)
    pool::Remove(PoolIterator);
}

/*
 * Calling SendToHell() marks the entity dead,
 * so that it will be removed by hell::Burn() at the end of the tick.
 * In general, you should never delete an entity instead of calling this,
 * because pool::Be() will crash if the entity currently Be()ing is deleted.
 */

void entity::SendToHell()
{
  if(EntityFlags & EXISTS)
    {
      hell::Add(this);
      EntityFlags ^= EXISTS;

      if(EntityFlags & HAS_BE)
	{
	  pool::Remove(PoolIterator);
	  EntityFlags ^= HAS_BE;
	}
    }
}

/*
 * These functions tells the poolsystem whether the Be() function of the entity
 * ought to be called during each tick, thus allowing it to alter itself independently.
 * If the entity is stabile, use Disable(), since it speeds up the game.
 */

void entity::Enable()
{
  if(!(EntityFlags & HAS_BE))
    {
      PoolIterator = pool::Add(this);
      EntityFlags |= HAS_BE;
    }
}

void entity::Disable()
{
  if(EntityFlags & HAS_BE)
    {
      pool::Remove(PoolIterator);
      EntityFlags ^= HAS_BE;
    }
}
