#include "entity.h"
#include "game.h"
#include "lsquare.h"
#include "hell.h"
#include "materba.h"
#include "proto.h"

entity::entity(const entity& Entity) : ExistsBool(Entity.ExistsBool), HasBeBool(Entity.HasBeBool), Emitation(Entity.Emitation)
{
  if(HasBeBool)
    PoolIterator = pool::Add(this);
}

entity::entity(bool HasBeBool) : ExistsBool(true), HasBeBool(HasBeBool), Emitation(0)
{
  if(HasBeBool)
    PoolIterator = pool::Add(this);
}

entity::~entity()
{
  if(HasBeBool)
    pool::Remove(PoolIterator);
}

/*
 * Calling SendToHell() marks the entity dead,
 * so that it will be removed by hell::Burn() at the end of the tick.
 * In general, you should never delete an entity instead of calling this.
 * This is because pool::Be() will crash if the entity currently Be()ing is deleted.
 */

void entity::SendToHell()
{
  if(ExistsBool)
    {
      hell::Add(this);
      ExistsBool = false;

      if(HasBeBool)
	{
	  pool::Remove(PoolIterator);
	  HasBeBool = false;
	}
    }
}

/*
 * Tells the poolsystem whether the Be() function of the entity ought to be called
 * during each tick, thus allowing it to alter itself independently.
 * If the entity is stabile, set this to false, since it speeds up the game.
 */

void entity::SetHasBe(bool What)
{
  if(What && !HasBeBool)
    {
      PoolIterator = pool::Add(this);
      HasBeBool = true;
    }
  else if(!What && HasBeBool)
    {
      pool::Remove(PoolIterator);
      HasBeBool = false;
    }
}
