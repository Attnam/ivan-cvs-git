#include "unit.h"
#include "materba.h"
#include "save.h"

unit::~unit()
{
  for(ushort c = 0; c < Material.size(); ++c)
    if(!PreserveBit[c])
      delete Material[c];

  Material.resize(0);
}

void unit::Save(outputfile& SaveFile) const
{
  entity::Save(SaveFile);
  SaveFile << Material;
}

void unit::Load(inputfile& SaveFile)
{
  entity::Load(SaveFile);
  SaveFile >> Material;

  for(ushort c = 0; c < Material.size(); ++c)
    if(Material[c])
      Material[c]->SetMotherEntity(this);

  PreserveBit.resize(Material.size(), false);
}

ushort unit::GetEmitation() const
{
  ushort Emitation = 0;

  for(ushort c = 0; c < Material.size(); ++c)
    if(Material[c])
      if(Material[c]->GetEmitation() > Emitation)
	Emitation = Material[c]->GetEmitation();

  return Emitation;
}

void unit::PreserveMaterial(ushort Index)
{
  PreserveBit[Index] = true;
}

material* unit::GetMaterial(ushort Index) const
{
  if(Index < Material.size())
    return Material[Index];
  else
    return 0;
}

ushort unit::GetMaterials() const
{
  return Material.size();
}
