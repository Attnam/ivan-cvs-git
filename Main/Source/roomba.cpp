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
  type::Save(SaveFile);

  SaveFile << Pos << Size << Door << Index << DivineOwner;
}

void room::Load(inputfile& SaveFile)
{
  type::Load(SaveFile);

  SaveFile >> Pos >> Size >> Door >> Index >> DivineOwner;
}

void room::HandleInstantiatedOLTerrain(olterrain* Terrain)
{
  if(Terrain->IsDoor())
    Door.push_back(Terrain->GetPos());

  if(Terrain->GetType() == altar::StaticType())
    ((altar*)Terrain)->SetOwnerGod(DivineOwner);
}

void room::HandleInstantiatedCharacter(character* Character)
{
  Character->SetHomeRoom(Index);

  /* This is a highly temporary gum solution... That most likely will be forgotten here. */

  if(Character->GetType() == kamikazedwarf::StaticType())
    {
      ((kamikazedwarf*)Character)->SetMaster(DivineOwner);
      ((holybook*)Character->GetWielded())->SetOwnerGod(DivineOwner);
    }
}


