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
	game::SaveWorldMap(game::SaveName(), true);
	game::SetWorldMap(0);
	game::GetDungeon(1)->PrepareLevel(0);
	game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetWorldMapEntry(), Who);
	game::GetCurrentLevel()->Luxify();
	//game::GetCurrentLevel()->UpdateLOS();
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
	game::SaveWorldMap(game::SaveName(), true);
	game::SetWorldMap(0);
	game::GetDungeon(0)->PrepareLevel(0);
	game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetWorldMapEntry(), Who);
	game::GetCurrentLevel()->Luxify();
	//game::GetCurrentLevel()->UpdateLOS();
	game::SendLOSUpdateRequest();
	game::UpdateCamera();
	return true;
}
