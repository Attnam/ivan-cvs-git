#include "entity.h"
#include "game.h"
#include "lsquare.h"
#include "hell.h"
#include "materba.h"
#include "proto.h"

entity::entity(bool HasBeBool) : SquareUnder(0), ExistsBool(true), HasBeBool(HasBeBool), Volume(0), Weight(0)
{
  if(HasBeBool)
    PoolIterator = pool::Add(this);
}

entity::~entity()
{
  if(HasBeBool)
    pool::Remove(PoolIterator);
}

/*void entity::Save(outputfile&) const
{
}*/

void entity::Load(inputfile&)
{
  SquareUnder = game::GetSquareInLoad();
}

lsquare* entity::GetLSquareUnder() const
{
  return (lsquare*)SquareUnder;
}

/*void entity::SetLSquareUnder(lsquare* What)
{
  SquareUnder = What;
}*/

/*
 * Tells whether an entity exists, i.e. hasn't been marked as removable by the poolsystem.
 */

/*bool entity::Exists() const
{
  return GetPoolIterator()->Exists;
}*/

/*
 * Calling SendToHell() marks the entity dead,
 * so that it will be removed by pool::Be() when first appropriate.
 * In general, you should never delete an entity instead of calling this.
 * This is because pool::Be() will crash if the entity currently Be()ing is deleted,
 * and also because deleting entities in a destructor of an entity might
 * crash because of pool cleaning optimization.
 *
 * Calling SetExists(true) reverses the preceding,
 * but make sure you do it before the poolsystem has had a chance of removing the entity.
 */

/*void entity::SetExists(bool What)
{
  GetPoolIterator()->Exists = What;
}*/

/*
 * Returns true if the entity wishes its Be() function to be called by the
 * poolsystem during each tick, false otherwise.
 */

/*bool entity::HasBe() const
{
  return GetPoolIterator()->HasBe;
}*/

/*
 * Tells the poolsystem whether the Be() function of the entity ought to be called
 * during each tick, thus allowing it to alter itself independently.
 * If the entity is stabile, set this to false, since it speeds up the game.
 */

/*void entity::SetHasBe(bool What)
{
  GetPoolIterator()->HasBe = What;
}*/

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

  /*if(ExistsBool && !HasBeBool)
    {
      PoolIterator = pool::Add(this);
      HasBeBool = true;
    }

  ExistsBool = false;*/
}

/*void SetExists(bool What)
{
  if(!What && ExistBool && !HasBeBool)
    PoolIterator = pool::Add(this);
  else if(What && !ExistBool && HasBeBool)

  ExistsBool = What;
}*/

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
