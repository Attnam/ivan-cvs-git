#include "lterraba.h"
#include "level.h"
#include "charba.h"
#include "dungeon.h"
#include "message.h"
#include "igraph.h"
#include "lsquare.h"
#include "worldmap.h"
#include "femath.h"
#include "femath.h"
#include "config.h"

bool overlevelterrain::GoUp(character* Who) const // Try to go up
{
  if(game::GetCurrent() && game::GetCurrent() != 9 && game::GetWizardMode())
    {
      game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
      game::GetCurrentDungeon()->SaveLevel();
      game::SetCurrent(game::GetCurrent() - 1);
      game::GetCurrentDungeon()->PrepareLevel();
      vector2d Pos = game::GetCurrentLevel()->RandomSquare(Who, true);
      game::GetCurrentLevel()->FastAddCharacter(Pos, Who);
      game::GetCurrentLevel()->Luxify();
      game::SendLOSUpdateRequest();
      game::UpdateCamera();
      if(configuration::GetAutosaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    if(!game::GetCurrent() && game::GetWizardMode())
      {
	if(Who->GetIsPlayer())
	  {
	    std::vector<character*> TempPlayerGroup;

	    if(!GetLevelSquareUnder()->GetLevelUnder()->CollectCreatures(TempPlayerGroup, Who, true))
	      return false;

	    game::GetCurrentArea()->RemoveCharacter(Who->GetPos());
	    game::GetCurrentDungeon()->SaveLevel();
	    game::LoadWorldMap();

	    game::GetWorldMap()->GetPlayerGroup().swap(TempPlayerGroup);

	    game::SetInWilderness(true);
	    game::GetCurrentArea()->AddCharacter(game::GetCurrentDungeon()->GetWorldMapPos(), Who);
	    game::SendLOSUpdateRequest();
	    game::UpdateCamera();
	    if(configuration::GetAutosaveInterval())
	      game::Save(game::GetAutoSaveFileName().c_str());
	    return true;
	  }

	return false;
      }
    else
      {
	if(Who->GetIsPlayer())
	  ADD_MESSAGE("You can't go up.");

	return false;
      }
}

bool overlevelterrain::GoDown(character* Who) const // Try to go down
{
  if(game::GetCurrent() < game::GetLevels() - 2 && game::GetWizardMode())
    {
      game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
      game::GetCurrentDungeon()->SaveLevel();
      game::SetCurrent(game::GetCurrent() + 1);
      game::GetCurrentDungeon()->PrepareLevel();
      vector2d Pos = game::GetCurrentLevel()->RandomSquare(Who, true);
      game::GetCurrentLevel()->FastAddCharacter(Pos, Who);
      game::GetCurrentLevel()->Luxify();
      game::SendLOSUpdateRequest();
      game::UpdateCamera();
      if(configuration::GetAutosaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    {
      if(Who->GetIsPlayer())
	ADD_MESSAGE("You can't go down.");

      return false;
    }
}

void levelterrain::Save(outputfile& SaveFile) const
{
  object::Save(SaveFile);

  SaveFile << VisualFlags;
}

void levelterrain::Load(inputfile& SaveFile)
{
  object::Load(SaveFile);

  SaveFile >> VisualFlags;
}

void groundlevelterrain::DrawToTileBuffer() const
{
  Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, VisualFlags);
}

void overlevelterrain::DrawToTileBuffer() const
{
  Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, VisualFlags);
}

bool levelterrain::Open(character* Opener)
{
  if(Opener->GetIsPlayer())
    ADD_MESSAGE("There isn't anything to open, %s.", game::Insult());

  return false;
}

bool levelterrain::Close(character* Closer)
{
  if(Closer->GetIsPlayer())
    ADD_MESSAGE("There isn't anything to close, %s.", game::Insult());

  return false;
}

vector2d levelterrain::GetPos() const
{
  return GetLevelSquareUnder()->GetPos();
}

void levelterrain::HandleVisualEffects()
{
  uchar Flags = 0, AcceptedFlags = OKVisualEffects();

  for(uchar c = 0; c < 8; ++c)
    if((AcceptedFlags & (1 << c)) && (RAND() % 2))
      Flags |= 1 << c;

  SetVisualFlags(Flags);
}

bool groundlevelterrain::SitOn(character*)
{
  ADD_MESSAGE("You sit for some time. Nothing happens.");
  return true;
}
