#define __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DECLARATIONS__

#include "proto.h"

std::vector<groundworldmapterrain*>	protocontainer<groundworldmapterrain>::ProtoData;
std::vector<overworldmapterrain*>	protocontainer<overworldmapterrain>::ProtoData;
std::map<std::string, ushort>		protocontainer<groundworldmapterrain>::CodeNameMap;
std::map<std::string, ushort>		protocontainer<overworldmapterrain>::CodeNameMap;

#include "wterrade.h"

#undef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DECLARATIONS__

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
  iosystem::TextScreen("Entering dungeon...\n\nThis may take some time, please wait.", WHITE, false);
  game::GetWorldMap()->RemoveCharacter(Who->GetPos());
  game::SetInWilderness(false);
  game::SetCurrentDungeon(1);
  game::SetCurrent(0);
  std::vector<character*> Temp;
  game::GetWorldMap()->GetPlayerGroup().swap(Temp);
  game::SaveWorldMap(game::SaveName(), true);
  game::SetWorldMap(0);
  game::GetDungeon(1)->PrepareLevel(0);
  game::GetCurrentLevel()->GetSquare(game::GetCurrentLevel()->GetWorldMapEntry())->KickAnyoneStandingHereAway();
  game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetWorldMapEntry(), Who);
	
  for(uchar c = 0; c < Temp.size(); ++c)
    game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(Temp[c], Who->GetPos()), Temp[c]);

  game::GetCurrentLevel()->Luxify();
  game::SendLOSUpdateRequest();
  game::UpdateCamera();

  if(Who->HasGoldenEagleShirt() && game::GetPetrus() && game::GetTeam(2)->GetRelation(Who->GetTeam()) != HOSTILE && game::GetPetrus()->GetStoryState() < 3)
    {
      game::GetCurrentLevel()->GetSquare(vector2d(30, 52))->KickAnyoneStandingHereAway();
      game::GetPetrus()->Move(vector2d(30, 52), true);
      game::GetPetrus()->ChangeTeam(game::GetTeam(3));
      game::GetPetrus()->SetStoryState(3);
    }

  if(configuration::GetAutosaveInterval())
    game::Save(game::GetAutoSaveFileName().c_str());

  return true;
}

bool elpuricave::GoDown(character* Who) const
{
  iosystem::TextScreen("Entering dungeon...\n\nThis may take some time, please wait.", WHITE, false);
  game::GetWorldMap()->RemoveCharacter(Who->GetPos());
  game::SetInWilderness(false);
  game::SetCurrentDungeon(0);
  game::SetCurrent(0);
  std::vector<character*> Temp;
  game::GetWorldMap()->GetPlayerGroup().swap(Temp);
  game::SaveWorldMap(game::SaveName(), true);
  game::SetWorldMap(0);
  game::GetDungeon(0)->PrepareLevel(0);
  game::GetCurrentLevel()->GetSquare(game::GetCurrentLevel()->GetWorldMapEntry())->KickAnyoneStandingHereAway();
  game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetWorldMapEntry(), Who);

  for(uchar c = 0; c < Temp.size(); ++c)
    game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(Temp[c], Who->GetPos()), Temp[c]);

  game::GetCurrentLevel()->Luxify();
  game::SendLOSUpdateRequest();
  game::UpdateCamera();

  if(configuration::GetAutosaveInterval())
    game::Save(game::GetAutoSaveFileName().c_str());

  return true;
}

bool ocean::GetIsWalkable(character* ByWho) const
{
  return ByWho && (ByWho->CanSwim() || ByWho->CanFly());
}
