#define __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "wterraba.h"

std::vector<gwterrain::prototype*> protocontainer<gwterrain>::ProtoData;
std::vector<owterrain::prototype*> protocontainer<owterrain>::ProtoData;
valuemap protocontainer<gwterrain>::CodeNameMap;
valuemap protocontainer<owterrain>::CodeNameMap;

#include "wterrade.h"

#undef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__

#include "message.h"
#include "dungeon.h"
#include "level.h"
#include "worldmap.h"
#include "charde.h"
#include "feio.h"
#include "square.h"
#include "team.h"
#include "config.h"

bool attnam::GoDown(character* Who) const
{
  game::GetWorldMap()->RemoveCharacter(Who->GetPos());
  game::SetIsInWilderness(false);
  game::SetCurrentDungeon(1);
  game::SetCurrent(0);
  std::vector<character*> Temp;
  game::GetWorldMap()->GetPlayerGroup().swap(Temp);
  game::SaveWorldMap(game::SaveName(), true);
  game::SetWorldMap(0);
  game::GetDungeon(1)->PrepareLevel(0);
  game::GetCurrentLevel()->GetLSquare(game::GetCurrentLevel()->GetWorldMapEntry())->KickAnyoneStandingHereAway();
  game::GetCurrentLevel()->AddCharacter(game::GetCurrentLevel()->GetWorldMapEntry(), Who);
	
  for(ushort c = 0; c < Temp.size(); ++c)
    {
      vector2d Pos = game::GetCurrentLevel()->GetNearestFreeSquare(Temp[c], Who->GetPos());

      if(Pos == DIR_ERROR_VECTOR)
	Pos = game::GetCurrentLevel()->RandomSquare(Temp[c], true);

      game::GetCurrentLevel()->AddCharacter(Pos, Temp[c]);
    }

  game::SendLOSUpdateRequest();
  game::UpdateCamera();
  game::GetCurrentArea()->UpdateLOS();

  if(Who->HasGoldenEagleShirt() && game::GetPetrus() && game::GetTeam(2)->GetRelation(Who->GetTeam()) != HOSTILE && game::GetPetrus()->GetStoryState() < 3)
    {
      game::GetCurrentLevel()->GetLSquare(30, 52)->KickAnyoneStandingHereAway();
      game::GetPetrus()->Move(vector2d(30, 52), true);
      game::GetPetrus()->ChangeTeam(game::GetTeam(3));
      game::GetPetrus()->SetStoryState(3);
    }

  if(configuration::GetAutoSaveInterval())
    game::Save(game::GetAutoSaveFileName().c_str());

  return true;
}

bool elpuricave::GoDown(character* Who) const
{
  game::GetWorldMap()->RemoveCharacter(Who->GetPos());
  game::SetIsInWilderness(false);
  game::SetCurrentDungeon(0);
  game::SetCurrent(0);
  std::vector<character*> Temp;
  game::GetWorldMap()->GetPlayerGroup().swap(Temp);
  game::SaveWorldMap(game::SaveName(), true);
  game::SetWorldMap(0);
  game::GetDungeon(0)->PrepareLevel(0);
  game::GetCurrentLevel()->GetLSquare(game::GetCurrentLevel()->GetWorldMapEntry())->KickAnyoneStandingHereAway();
  game::GetCurrentLevel()->AddCharacter(game::GetCurrentLevel()->GetWorldMapEntry(), Who);

  for(ushort c = 0; c < Temp.size(); ++c)
    {
      vector2d Pos = game::GetCurrentLevel()->GetNearestFreeSquare(Temp[c], Who->GetPos());

      if(Pos == DIR_ERROR_VECTOR)
	Pos = game::GetCurrentLevel()->RandomSquare(Temp[c], true);

      game::GetCurrentLevel()->AddCharacter(Pos, Temp[c]);
    }

  game::SendLOSUpdateRequest();
  game::UpdateCamera();
  game::GetCurrentArea()->UpdateLOS();

  if(configuration::GetAutoSaveInterval())
    game::Save(game::GetAutoSaveFileName().c_str());

  return true;
}

bool ocean::IsWalkable(const character* ByWho) const
{
  return ByWho && (ByWho->CanSwim() || ByWho->CanFly());
}

