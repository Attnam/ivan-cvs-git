#include "entity.h"
#include "game.h"
#include "save.h"
#include "materba.h"
#include "lsquare.h"

entity::entity(bool AddToPool, bool HasBe) : InPool(AddToPool), Picture(0), SquareUnder(0)
{
  if(AddToPool)
    {
      entityinfo EntityInfo = { this, true, HasBe };
      SetPoolIterator(entitypool::Add(EntityInfo));
      ID = game::CreateNewEntityID();
    }
}

entity::~entity()
{
  for(ushort c = 0; c < Material.size(); ++c)
    if(!PreserveBit[c])
      delete Material[c];

  Material.resize(0);

  if(InPool)
    entitypool::Remove(GetPoolIterator());
}

void entity::Save(outputfile& SaveFile) const
{
  SaveFile << Material << ID;
}

void entity::Load(inputfile& SaveFile)
{
  SquareUnder = game::GetSquareInLoad();

  game::PopEntityID(ID);

  SaveFile >> Material >> ID;

  for(ushort c = 0; c < Material.size(); ++c)
    if(Material[c])
      Material[c]->SetMotherEntity(this);

  PreserveBit.resize(Material.size(), false);
}

ushort entity::GetEmitation() const
{
  ushort Emitation = 0;

  for(ushort c = 0; c < Material.size(); ++c)
    if(Material[c])
      if(Material[c]->GetEmitation() > Emitation)
	Emitation = Material[c]->GetEmitation();

  return Emitation;
}

void entity::PreserveMaterial(ushort Index)
{
  PreserveBit[Index] = true;
}

levelsquare* entity::GetLevelSquareUnder() const
{
  return (levelsquare*)SquareUnder;
}

void entity::SetLevelSquareUnder(levelsquare* What)
{
  SquareUnder = What;
}

material* entity::GetMaterial(ushort Index) const
{
  if(Index < Material.size())
    return Material[Index];
  else
    return 0;
}

bool entity::GetExists() const
{
  return GetPoolIterator()->Exists;
}

void entity::SetExists(bool What)
{
  GetPoolIterator()->Exists = What;
}
