#include "wterrade.h"
#include "game.h"
#include "message.h"
#include "dungeon.h"
#include "level.h"
#include "worldmap.h"
#include "charba.h"

bool attnam::GoDown(character* Who) const
{
	game::GetDungeon(1)->GenerateIfNeeded();
	game::GetWorldMap()->RemoveCharacter(Who->GetPos());
	game::SetInWilderness(false);
	game::SetCurrentDungeon(1);
	game::SetCurrent(0);
	delete game::GetWorldMap();
	game::SetWorldMap(0);
	game::GetCurrentDungeon()->LoadLevel();
	game::GetCurrentLevel()->PutPlayerAround(game::GetCurrentLevel()->GetUpStairs());
	game::GetCurrentLevel()->Luxify();
	game::GetCurrentLevel()->UpdateLOS();
	game::UpDateCameraX();
	game::UpDateCameraY();
	return true;
}

bool elpuricave::GoDown(character* Who) const
{
	game::GetDungeon(0)->GenerateIfNeeded();
	game::GetWorldMap()->RemoveCharacter(Who->GetPos());
	game::SetInWilderness(false);
	game::SetCurrentDungeon(0);
	game::SetCurrent(0);
	delete game::GetWorldMap();
	game::SetWorldMap(0);
	game::GetCurrentDungeon()->LoadLevel();
	game::GetCurrentLevel()->PutPlayerAround(game::GetCurrentLevel()->GetUpStairs());
	game::GetCurrentLevel()->Luxify();
	game::GetCurrentLevel()->UpdateLOS();
	game::UpDateCameraX();
	game::UpDateCameraY();
	return true;
}
