/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

/* Compiled through coreset.cpp */

entity::entity(const entity& Entity) : Emitation(Entity.Emitation), Flags(Entity.Flags)
{
  if(Flags & HAS_BE)
    pool::Add(this);
}

entity::entity(int Flags) : Emitation(0), Flags(Flags|EXISTS)
{
  if(Flags & HAS_BE)
    pool::Add(this);
}

entity::~entity()
{
  if(Flags & HAS_BE)
    pool::Remove(this);
}

/* Calling SendToHell() marks the entity dead,
 * so that it will be removed by hell::Burn() at the end of the tick.
 * In general, you should never delete an entity instead of calling this,
 * because pool::Be() will crash if the entity currently Be()ing is deleted. */

void entity::SendToHell()
{
  if(Flags & EXISTS)
    {
      if(Flags & HAS_BE)
	{
	  pool::Remove(this);
	  Flags ^= HAS_BE;
	}

      pool::AddToHell(this);
      Flags ^= EXISTS;
    }
}

/* These functions tell the poolsystem whether the Be() function of the entity
 * ought to be called during each tick, thus allowing it to alter itself independently.
 * If the entity is stabile, use Disable(), since it speeds up the game. */

void entity::Enable()
{
  if(!(Flags & HAS_BE))
    {
      pool::Add(this);
      Flags |= HAS_BE;
    }
}

void entity::Disable()
{
  if(Flags & HAS_BE)
    {
      pool::Remove(this);
      Flags ^= HAS_BE;
    }
}
