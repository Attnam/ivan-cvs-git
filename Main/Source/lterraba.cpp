#include "lterrade.h"
#include "level.h"
#include "charba.h"
#include "dungeon.h"
#include "message.h"
#include "lsquare.h"
#include "worldmap.h"
#include "femath.h"
#include "config.h"
#include "whandler.h"
#include "save.h"
#include "proto.h"

bool olterrain::GoUp(character* Who) const // Try to go up
{
  if(game::GetCurrent() && game::GetCurrent() != 9 && game::WizardModeActivated())
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
      game::GetCurrentArea()->UpdateLOS();
      if(configuration::GetAutoSaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    if(!game::GetCurrent() && game::WizardModeActivated())
      {
	if(Who->IsPlayer())
	  {
	    std::vector<character*> TempPlayerGroup;

	    if(!GetLSquareUnder()->GetLevelUnder()->CollectCreatures(TempPlayerGroup, Who, true))
	      return false;

	    game::GetCurrentArea()->RemoveCharacter(Who->GetPos());
	    game::GetCurrentDungeon()->SaveLevel();
	    game::LoadWorldMap();

	    game::GetWorldMap()->GetPlayerGroup().swap(TempPlayerGroup);

	    game::SetIsInWilderness(true);
	    game::GetCurrentArea()->AddCharacter(game::GetCurrentDungeon()->GetWorldMapPos(), Who);
	    game::SendLOSUpdateRequest();
	    game::UpdateCamera();
	    game::GetCurrentArea()->UpdateLOS();
	    if(configuration::GetAutoSaveInterval())
	      game::Save(game::GetAutoSaveFileName().c_str());
	    return true;
	  }

	return false;
      }
    else
      {
	if(Who->IsPlayer())
	  ADD_MESSAGE("You can't go up.");

	return false;
      }
}

bool olterrain::GoDown(character* Who) const // Try to go down
{
  if(game::GetCurrent() < game::GetLevels() - 2 && game::WizardModeActivated())
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
      game::GetCurrentArea()->UpdateLOS();
      if(configuration::GetAutoSaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    {
      if(Who->IsPlayer())
	ADD_MESSAGE("You can't go down.");

      return false;
    }
}

void olterrain::Save(outputfile& SaveFile) const
{
  lterrain::Save(SaveFile);
  SaveFile << HP;
}

void olterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  SaveFile >> HP;
}

void glterrain::DrawToTileBuffer(bool Animate) const
{
  if(Animate)
    Picture[globalwindowhandler::GetTick() % GetAnimationFrames()]->MaskedBlit(igraph::GetTileBuffer());
  else
    Picture[0]->MaskedBlit(igraph::GetTileBuffer());
}

void olterrain::DrawToTileBuffer(bool Animate) const
{
  if(Animate)
    Picture[globalwindowhandler::GetTick() % GetAnimationFrames()]->AlphaBlit(igraph::GetTileBuffer());
  else
    Picture[0]->AlphaBlit(igraph::GetTileBuffer());
}

bool lterrain::Open(character*)
{
  return false;
}

bool lterrain::Close(character* Closer)
{
  if(Closer->IsPlayer())
    ADD_MESSAGE("There isn't anything to close, %s.", game::Insult());

  return false;
}

vector2d lterrain::GetPos() const
{
  return GetLSquareUnder()->GetPos();
}

bool glterrain::SitOn(character*)
{
  ADD_MESSAGE("You sit for some time. Nothing happens.");
  return true;
}

void olterrain::Break()
{
  GetLSquareUnder()->ChangeOLTerrainAndUpdateLights(new empty);
}

glterrain* glterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  glterrain* Terrain = Clone(0, false, true);
  Terrain->Load(SaveFile);
  return Terrain;
}

olterrain* olterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  olterrain* Terrain = Clone(0, false, true);
  Terrain->Load(SaveFile);
  return Terrain;
}

void lterrain::Initialize(uchar NewConfig, bool CallGenerateMaterials, bool Load)
{
  if(!Load)
    {
      Config = NewConfig;
      RandomizeVisualEffects();

      if(CallGenerateMaterials)
	GenerateMaterials();
    }

  VirtualConstructor(Load);

  if(CallGenerateMaterials)
    UpdatePictures();
}

glterrainprototype::glterrainprototype()
{
  Index = protocontainer<glterrain>::Add(this);
}

olterrainprototype::olterrainprototype()
{
  Index = protocontainer<olterrain>::Add(this);
}

