#include "entity.h"
#include "game.h"
#include "lsquare.h"

entity::entity() : SquareUnder(0)
{
  SetPoolIterator(entitypool::Add(this));
}

entity::~entity()
{
  entitypool::Remove(GetPoolIterator());
}

void entity::Save(outputfile&) const
{
}

void entity::Load(inputfile&)
{
  SquareUnder = game::GetSquareInLoad();
}

lsquare* entity::GetLSquareUnder() const
{
  return (lsquare*)SquareUnder;
}

void entity::SetLSquareUnder(lsquare* What)
{
  SquareUnder = What;
}

/*
 * Tells whether an entity exists, i.e. hasn't been marked as removable by the poolsystem.
 */

bool entity::GetExists() const
{
  return GetPoolIterator()->Exists;
}

/*
 * Calling SetExists(false) marks the entity dead,
 * so that it will be removed by entitypool::Be() when first appropriate.
 * In general, you should never delete an entity instead of calling this.
 * This is because entitypool::Be() will crash if the entity currently Be()ing is deleted,
 * and also because deleting entities in a destructor of an entity might
 * crash because of pool cleaning optimization.
 *
 * Calling SetExists(true) reverses the preceding,
 * but make sure you do it before the poolsystem has had a chance of removing the entity.
 */

void entity::SetExists(bool What)
{
  GetPoolIterator()->Exists = What;
}

/*
 * Returns true if the entity wishes its Be() function to be called by the
 * poolsystem during each tick, false otherwise.
 */

bool entity::GetHasBe() const
{
  return GetPoolIterator()->HasBe;
}

/*
 * Tells the poolsystem whether the Be() function of the entity ought to be called
 * during each tick, thus allowing it to alter itself independently.
 * If the entity is stabile, set this to false, since it speeds up the game.
 */

void entity::SetHasBe(bool What)
{
  GetPoolIterator()->HasBe = What;
}
