#include "roomba.h"
#include "lterrade.h"
#include "error.h"
#include "charde.h"
#include "save.h"
#include "godba.h"
#include "message.h"

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
  room* Room = Cloner(true);
  Room->Load(SaveFile);
  return Room;
}

roomprototype::roomprototype(room* (*Cloner)(bool), const std::string& ClassId) : Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<room>::Add(this);
}

void room::DestroyTerrain(character* Who, olterrain*)
{
  if(Master)
    Who->Hostility(Master);

  if(DivineMaster)
    game::GetGod(DivineMaster)->AdjustRelation(GetGodRelationAdjustment());
}

/* returns true if player agrees to continue */

bool room::CheckDestroyTerrain(character* Infidel, olterrain* Terrain) 
{
  if(!Master || Infidel == Master || Master->GetRelation(Infidel) == HOSTILE)
    return true;

  ADD_MESSAGE("%s might not like this.", Master->CHAR_NAME(DEFINITE));

  if(game::BoolQuestion("Are you sure you want to do this? [y/N]"))
    {
      DestroyTerrain(Infidel, Terrain);
      return true;
    }
  else
    return false; 
}
