#include "entity.h"
#include "game.h"
#include "lsquare.h"

entity::entity(bool AddToPool, bool HasBe) : InPool(AddToPool), SquareUnder(0)
{
  if(AddToPool)
    {
      entityinfo EntityInfo = { this, true, HasBe };
      SetPoolIterator(entitypool::Add(EntityInfo));
    }
}

entity::~entity()
{
  if(InPool)
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

bool entity::GetExists() const
{
  return GetPoolIterator()->Exists;
}

void entity::SetExists(bool What)
{
  GetPoolIterator()->Exists = What;
}

void entity::SetHasBe(bool What)
{
  GetPoolIterator()->HasBe = What;
}
