#include "roomba.h"
#include "save.h"
#include "lterrade.h"
#include "error.h"
#include "charba.h"

room::room(bool SetStats) : Master(0)
{
  if(SetStats)
    ABORT("Baa!");
}

void room::Save(outputfile& SaveFile) const
{
  typeable::Save(SaveFile);

  SaveFile << Pos << Size << Door << Index << DivineOwner;
}

void room::Load(inputfile& SaveFile)
{
  typeable::Load(SaveFile);

  SaveFile >> Pos >> Size >> Door >> Index >> DivineOwner;
}

void room::HandleInstantiatedOverLevelTerrain(overlevelterrain* Terrain)
{
  if(Terrain->IsDoor())
    Door.push_back(Terrain->GetPos());

  if(Terrain->GetType() == altar::StaticType())
    ((altar*)Terrain)->SetOwnerGod(DivineOwner);
}

void room::HandleInstantiatedCharacter(character* Character)
{
  Character->SetHomeRoom(Index);
}
