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

void entity::Save(outputfile& SaveFile) const
{
}

void entity::Load(inputfile& SaveFile)
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

/* Temporary */

std::string entity::NameNormal(uchar Case, std::string Article, std::string Adjective) const
{
  if(!(Case & PLURAL))
    if(!(Case & DEFINEBIT))
      return Adjective + NameSingular();
    else
      if(!(Case & INDEFINEBIT))
	return std::string("the ") + Adjective + NameSingular();
      else
	return Article + " " + Adjective + NameSingular();
  else
    if(!(Case & DEFINEBIT))
      return Adjective + NamePlural();
    else
      if(!(Case & INDEFINEBIT))
	return std::string("the ") + Adjective + NamePlural();
      else
	return Adjective + NamePlural();
}

std::string entity::NameProperNoun(uchar Case) const
{
  if(!(Case & PLURAL))
    return NameSingular();
  else
    return NamePlural();
}

