#include "roomba.h"
#include "lterrade.h"
#include "error.h"
#include "charde.h"
#include "itemde.h"

room::room(bool SetStats) : Master(0)
{
  if(SetStats)
    ABORT("Baa!");
}

void room::Save(outputfile& SaveFile) const
{
  SaveFile << Type();
  SaveFile << Pos << Size << Door << Index << DivineOwner;
}

void room::Load(inputfile& SaveFile)
{
  SaveFile >> Pos >> Size >> Door >> Index >> DivineOwner;
}

void room::HandleInstantiatedOLTerrain(olterrain* Terrain)
{
  if(Terrain->IsDoor())
    Door.push_back(Terrain->GetPos());

  if(DivineOwner)
    Terrain->SetDivineMaster(DivineOwner);
}

void room::HandleInstantiatedCharacter(character* Character)
{
  Character->SetHomeRoom(Index);

  if(DivineOwner)
    Character->SetDivineMaster(DivineOwner);
}


