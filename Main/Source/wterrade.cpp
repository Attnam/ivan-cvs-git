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

bool attnam::Enter(bool DirectionUp) const
{
  if(!owterrain::Enter(DirectionUp))
    return false;

  if(game::GetPlayer()->HasGoldenEagleShirt() && game::GetPetrus() && game::GetTeam(ATTNAM_TEAM)->GetRelation(game::GetPlayer()->GetTeam()) != HOSTILE && game::GetPetrus()->GetStoryState() < 3)
    {
      game::GetCurrentLevel()->GetLSquare(30, 52)->KickAnyoneStandingHereAway();
      game::GetPetrus()->Move(vector2d(30, 52), true);
      game::GetPetrus()->ChangeTeam(game::GetTeam(FINAL_BATTLE_TEAM));
      game::GetPetrus()->SetStoryState(3);
    }

  if(configuration::GetAutoSaveInterval())
    game::Save(game::GetAutoSaveFileName().c_str());

  return true;
}

bool ocean::IsWalkable(const character* ByWho) const
{
  return ByWho && (ByWho->CanSwim() || ByWho->CanFly());
}
