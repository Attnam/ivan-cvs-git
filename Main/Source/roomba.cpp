#include "roomba.h"
#include "lterrade.h"
#include "error.h"
#include "charde.h"
#include "save.h"

void room::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  SaveFile << Pos << Size << Door << Index << DivineMaster;
}

void room::Load(inputfile& SaveFile)
{
  SaveFile >> Pos >> Size >> Door >> Index >> DivineMaster;
}

void room::HandleInstantiatedOLTerrain(olterrain* Terrain)
{
  if(Terrain->IsDoor())
    Door.push_back(Terrain->GetPos());
}

void room::HandleInstantiatedCharacter(character* Character)
{
  Character->SetHomeRoom(Index);
}

room* roomprototype::CloneAndLoad(inputfile& SaveFile) const
{
  room* Room = Clone();
  Room->Load(SaveFile);
  return Room;
}

roomprototype::roomprototype()
{
  Index = protocontainer<room>::Add(this);
}
