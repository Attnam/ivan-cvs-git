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
#include "error.h"
#include "database.h"

bool olterrain::GoUp(character* Who) const // Try to go up
{
  if(game::GetCurrent() && game::GetCurrent() != 9 && game::WizardModeActivated())
    {
      GetLevelUnder()->RemoveCharacter(Who->GetPos());
      game::GetCurrentDungeon()->SaveLevel();
      game::SetCurrent(game::GetCurrent() - 1);
      game::GetCurrentDungeon()->PrepareLevel();
      vector2d Pos = GetLevelUnder()->RandomSquare(Who, true);
      GetLevelUnder()->FastAddCharacter(Pos, Who);
      GetLevelUnder()->Luxify();
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

	    if(!GetLevelUnder()->CollectCreatures(TempPlayerGroup, Who, true))
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
      GetLevelUnder()->RemoveCharacter(Who->GetPos());
      game::GetCurrentDungeon()->SaveLevel();
      game::SetCurrent(game::GetCurrent() + 1);
      game::GetCurrentDungeon()->PrepareLevel();
      vector2d Pos = GetLevelUnder()->RandomSquare(Who, true);
      GetLevelUnder()->FastAddCharacter(Pos, Who);
      GetLevelUnder()->Luxify();
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

void lterrain::Load(inputfile& SaveFile)
{
  SquareUnder = game::GetSquareInLoad();
  object::Load(SaveFile);
}

void glterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  lterrain::Save(SaveFile);
}

void glterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  InstallDataBase();
}

void olterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  lterrain::Save(SaveFile);
  SaveFile << HP;
}

void olterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  InstallDataBase();
  SaveFile >> HP;
}

vector2d lterrain::GetPos() const
{
  return GetLSquareUnder()->GetPos();
}

bool lterrain::SitOn(character* Sitter)
{
  if(GetSitMessage().length())
    {
      ADD_MESSAGE("%s", GetSitMessage().c_str());
      Sitter->EditAP(-1000);
      return true;
    }
  else
    return false;
}

void olterrain::Break()
{
  GetLSquareUnder()->ChangeOLTerrainAndUpdateLights(new empty);
}

glterrain* glterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  glterrain* Terrain = Cloner(0, false, true);
  Terrain->Load(SaveFile);
  return Terrain;
}

olterrain* olterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  olterrain* Terrain = Cloner(0, false, true);
  Terrain->Load(SaveFile);
  return Terrain;
}

void lterrain::Initialize(uchar NewConfig, bool CallGenerateMaterials, bool Load)
{
  if(!Load)
    {
      Config = NewConfig;
      InstallDataBase();
      RandomizeVisualEffects();

      if(CallGenerateMaterials)
	GenerateMaterials();
    }

  VirtualConstructor(Load);

  if(CallGenerateMaterials)
    UpdatePictures();
}

glterrainprototype::glterrainprototype(glterrainprototype* Base, glterrain* (*Cloner)(ushort, bool, bool), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<glterrain>::Add(this);
}

olterrainprototype::olterrainprototype(olterrainprototype* Base, olterrain* (*Cloner)(ushort, bool, bool), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<olterrain>::Add(this);
}

bool lterrain::CanBeSeenByPlayer() const
{
  return GetSquareUnder()->CanBeSeenByPlayer();
}

bool lterrain::CanBeSeenBy(character* Who) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer();
  else
    return GetSquareUnder()->CanBeSeenFrom(Who->GetPos(), Who->LOSRangeSquare());
}

void glterrain::InstallDataBase()
{
  ::database<glterrain>::InstallDataBase(this);
}

void olterrain::InstallDataBase()
{
  ::database<olterrain>::InstallDataBase(this);
}

void olterrain::ShowRestMessage(character*) const
{
  if(GetRestMessage().length())
    ADD_MESSAGE("%s", GetRestMessage().c_str());
}

/*lsquare* lterrain::GetLSquareUnder() const
{
  return static_cast<lsquare*>(SquareUnder);
}

level* lterrain::GetLevelUnder() const
{ 
  return static_cast<level*>(SquareUnder->GetAreaUnder());
}*/
