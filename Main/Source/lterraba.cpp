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

void lterrain::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
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
  return LSquareUnder->GetPos();
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
  glterrain* Terrain = Cloner(0, LOAD);
  Terrain->Load(SaveFile);
  Terrain->CalculateAll();
  return Terrain;
}

olterrain* olterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  olterrain* Terrain = Cloner(0, LOAD);
  Terrain->Load(SaveFile);
  Terrain->CalculateAll();
  return Terrain;
}

void lterrain::Initialize(ushort NewConfig, ushort SpecialFlags)
{
  if(!(SpecialFlags & LOAD))
    {
      Config = NewConfig;
      InstallDataBase();
      RandomizeVisualEffects();

      if(!(SpecialFlags & NO_MATERIALS))
	GenerateMaterials();
    }

  VirtualConstructor(SpecialFlags & LOAD);

  if(!(SpecialFlags & LOAD))
    {
      if(!(SpecialFlags & NO_MATERIALS))
	{
	  CalculateAll();

	  if(!(SpecialFlags & NO_PIC_UPDATE))
	    UpdatePictures();
	}
    }
}

glterrainprototype::glterrainprototype(glterrainprototype* Base, glterrain* (*Cloner)(ushort, ushort), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<glterrain>::Add(this);
}

olterrainprototype::olterrainprototype(olterrainprototype* Base, olterrain* (*Cloner)(ushort, ushort), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<olterrain>::Add(this);
}

bool lterrain::CanBeSeenByPlayer() const
{
  return LSquareUnder->CanBeSeenByPlayer();
}

bool lterrain::CanBeSeenBy(character* Who) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer();
  else
    return LSquareUnder->CanBeSeenFrom(Who->GetPos(), Who->LOSRangeSquare());
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

void lterrain::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
    {
      game::CombineLights(Emitation, EmitationUpdate);

      if(LSquareUnder)
	LSquareUnder->SignalEmitationIncrease(EmitationUpdate);
    }
}

void lterrain::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
    {
      ulong Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation && LSquareUnder)
	LSquareUnder->SignalEmitationDecrease(EmitationUpdate);
    }
}

bool olterrain::Enter(bool DirectionUp) const
{
  /* "Temporary" gum solutions */

  std::vector<character*> Group;

  if(game::WizardModeIsActive())
    if(!DirectionUp && game::GetCurrent() < game::GetLevels() - 1 && game::LeaveLevel(Group, true))
      {
	game::EnterArea(Group, game::GetCurrent() + 1, RANDOM);
	return true;
      }
    else if(DirectionUp && game::GetCurrent() >= 1 && game::LeaveLevel(Group, true))
      {
	game::EnterArea(Group, game::GetCurrent() - 1, RANDOM);
	return true;
      }

  if(DirectionUp)
    ADD_MESSAGE("You can't go up.");
  else
    ADD_MESSAGE("You can't go down.");

  return false;
}

void olterrain::VirtualConstructor(bool Load)
{
  if(!Load)
    CalculateHP();
}

ushort olterrain::GetStrengthValue() const
{
  return GetMainMaterial()->GetStrengthValue() / 20;
}

void olterrain::ReceiveDamage(character* Villain, ushort What, uchar Type)
{
  if(CanBeDestroyed() && What > GetStrengthValue())
    {
      EditHP(GetStrengthValue() - What);

      if(HP <= 0)
	{
	  room* Room = GetLSquareUnder()->GetRoomClass();

	  if(Room)
	    Room->DestroyTerrain(Villain, this);

	  Break();
	}
    }
}

void olterrain::CalculateHP()
{
  if(GetMainMaterial())
    {
      ulong SV = GetMainMaterial()->GetStrengthValue();
      HP = SV * SV * GetHPModifier() / 5000;
    }
}
