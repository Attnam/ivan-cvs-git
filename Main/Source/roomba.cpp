#include "roomba.h"
#include "lterrade.h"
#include "error.h"
#include "charde.h"
#include "itemde.h"
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

  if(DivineMaster)
    Terrain->SetDivineMaster(DivineMaster);
}

void room::HandleInstantiatedCharacter(character* Character)
{
  Character->SetHomeRoom(Index);

  if(DivineMaster)
    Character->SetDivineMaster(DivineMaster);
}

room* room_prototype::CloneAndLoad(inputfile& SaveFile) const
{
  room* Room = Clone();
  Room->Load(SaveFile);
  return Room;
}

room_prototype::room_prototype()
{
  Index = protocontainer<room>::Add(this);
}
