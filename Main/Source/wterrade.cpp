#include "wterrade.h"
#include "game.h"
#include "message.h"
#include "dungeon.h"
#include "level.h"
#include "worldmap.h"
#include "charde.h"
#include "feio.h"
#include "igraph.h"
#include "square.h"
#include "graphics.h"
#include "team.h"

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
		game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(Who->GetPos()), Temp[c]);

	game::GetCurrentLevel()->Luxify();
	game::GetCurrentLevel()->UpdateLOS();
	game::UpdateCamera();

	if(Who->HasMaakotkaShirt() && game::GetPerttu() && game::GetTeam(2)->GetRelation(Who->GetTeam()) != HOSTILE && game::GetPerttu()->GetStoryState() < 3)
	{
		game::GetCurrentLevel()->GetSquare(vector2d(23, 36))->KickAnyoneStandingHereAway();
		game::GetPerttu()->Move(vector2d(23, 36), true);
		game::GetPerttu()->SetTeam(game::GetTeam(3));
		game::GetPerttu()->SetStoryState(3);
	}

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
		game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(Who->GetPos()), Temp[c]);

	game::GetCurrentLevel()->Luxify();
	game::GetCurrentLevel()->UpdateLOS();
	game::UpdateCamera();
	return true;
}
