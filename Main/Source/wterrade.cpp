#include "wterrade.h"
#include "game.h"
#include "message.h"
#include "dungeon.h"
#include "level.h"
#include "worldmap.h"
#include "charba.h"
#include "feio.h"
#include "igraph.h"
#include "square.h"
#include "graphics.h"

bool attnam::GoDown(character* Who) const
{
	iosystem::TextScreen("Entering dungeon...\n\nThis may take some time, please wait.", WHITE, false);
	game::GetDungeon(1)->SetWorldMapPos(SquareUnder->GetPos());
	game::GetWorldMap()->RemoveCharacter(Who->GetPos());
	game::SetInWilderness(false);
	game::SetCurrentDungeon(1);
	game::SetCurrent(0);
	std::vector<character*> Temp;
	uchar c;
	
	for(c = 0; c < game::GetWorldMap()->GetPlayerGroup()->size(); ++c)
		Temp.push_back((*game::GetWorldMap()->GetPlayerGroup())[c]);
	game::SaveWorldMap(game::SaveName(), true);
	game::SetWorldMap(0);
	game::GetDungeon(1)->PrepareLevel(0);
	game::GetCurrentLevel()->GetSquare(game::GetCurrentLevel()->GetWorldMapEntry())->KickAnyoneStandingHereAway();
	game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetWorldMapEntry(), Who);
	
	for(c = 0; c < Temp.size(); ++c)
	{
		game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(Who->GetPos()), Temp[c]);
		Temp[c]->SetExists(true);
	}

	game::GetCurrentLevel()->Luxify();
	game::SendLOSUpdateRequest();
	game::UpdateCamera();
	return true;
}

bool elpuricave::GoDown(character* Who) const
{
	iosystem::TextScreen("Entering dungeon...\n\nThis may take some time, please wait.", WHITE, false);
	game::GetDungeon(0)->SetWorldMapPos(SquareUnder->GetPos());
	game::GetWorldMap()->RemoveCharacter(Who->GetPos());
	game::SetInWilderness(false);
	game::SetCurrentDungeon(0);
	game::SetCurrent(0);
	std::vector<character*> Temp;
	uchar c;
	
	for(c = 0; c < game::GetWorldMap()->GetPlayerGroup()->size(); ++c)
		Temp.push_back(game::GetWorldMap()->GetPlayerGroupMember(c));
	
	game::SaveWorldMap(game::SaveName(), true);
	game::SetWorldMap(0);
	game::GetDungeon(0)->PrepareLevel(0);
	game::GetCurrentLevel()->GetSquare(game::GetCurrentLevel()->GetWorldMapEntry())->KickAnyoneStandingHereAway();
	game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetWorldMapEntry(), Who);

	for(c = 0; c < Temp.size(); ++c)
	{
		game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(Who->GetPos()), Temp[c]);
		Temp[c]->SetExists(true);
	}

	game::GetCurrentLevel()->Luxify();
	game::SendLOSUpdateRequest();
	game::UpdateCamera();
	return true;
}
