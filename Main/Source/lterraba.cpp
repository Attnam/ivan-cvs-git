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
      game::GetCurrentArea()->UpdateLOS();
      if(configuration::GetAutoSaveInterval())
	game::Save(game::GetAutoSaveFileName().c_str());
      return true;
    }
  else
    if(!game::GetCurrent() && game::GetWizardMode())
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

void lterrain::Save(outputfile& SaveFile) const
{
  object::Save(SaveFile);
  SaveFile << VisualFlags;
}

void lterrain::Load(inputfile& SaveFile)
{
  object::Load(SaveFile);
  SaveFile >> VisualFlags;
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
    Picture[globalwindowhandler::GetTick() % GetAnimationFrames()]->MaskedBlit(igraph::GetTileBuffer(), VisualFlags);
  else
    Picture[0]->MaskedBlit(igraph::GetTileBuffer(), VisualFlags);
}

void olterrain::DrawToTileBuffer(bool Animate) const
{
  if(Animate)
    Picture[globalwindowhandler::GetTick() % GetAnimationFrames()]->AlphaBlit(igraph::GetTileBuffer(), VisualFlags);
  else
    Picture[0]->AlphaBlit(igraph::GetTileBuffer(), VisualFlags);
}

bool lterrain::Open(character* Opener)
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

void lterrain::HandleVisualEffects()
{
  uchar Flags = 0, AcceptedFlags = OKVisualEffects();

  for(ushort c = 0; c < 8; ++c)
    if((AcceptedFlags & (1 << c)) && (RAND() % 2))
      Flags |= 1 << c;

  SetVisualFlags(Flags);
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
  Config = NewConfig;
  VirtualConstructor(Load);

  if(!Load)
    {
      HandleVisualEffects();

      if(CallGenerateMaterials)
	GenerateMaterials();
    }
}

glterrainprototype::glterrainprototype()
{
  Index = protocontainer<glterrain>::Add(this);
}

olterrainprototype::olterrainprototype()
{
  Index = protocontainer<olterrain>::Add(this);
}

