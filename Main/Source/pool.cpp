/* Compiled through coreset.cpp */

entitylist pool::Pool;
entitylist::iterator pool::CurrentEntity = pool::Pool.end();
entitylist pool::Hell;

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

void pool::Remove(entitylist::iterator Iterator)
{
  if(Iterator == CurrentEntity)
    ++CurrentEntity;

  Pool.erase(Iterator);
}

void pool::BurnHell()
{
  for(entitylist::iterator i = Hell.begin(); i != Hell.end(); ++i)
    delete *i;

  Hell.clear();
}
