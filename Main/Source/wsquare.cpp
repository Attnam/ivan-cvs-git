#include <cmath>

#include "stdover.h"
#include "wsquare.h"
#include "charba.h"
#include "worldmap.h"
#include "bitmap.h"
#include "graphics.h"
#include "config.h"
#include "wterraba.h"
#include "proto.h"
#include "game.h"
#include "save.h"
#include "igraph.h"

wsquare::wsquare(worldmap* WorldMapUnder, vector2d Pos) : square(WorldMapUnder, Pos), GWTerrain(0), OWTerrain(0)
{
}

wsquare::~wsquare()
{
  delete GetGWTerrain();
  delete GetOWTerrain();
}

void wsquare::Save(outputfile& SaveFile) const
{
  square::Save(SaveFile);

  SaveFile << GWTerrain << OWTerrain;
}

void wsquare::Load(inputfile& SaveFile)
{
  square::Load(SaveFile);

  SaveFile >> GWTerrain >> OWTerrain;
}

/*bool wsquare::DrawTerrain(bool Animate) const
{
  GetGWTerrain()->DrawToTileBuffer(Animate);
  GetOWTerrain()->DrawToTileBuffer(Animate);

  return true;
}

bool wsquare::DrawCharacter(bool Animate) const
{
  if(Character && Character->CanBeSeenByPlayer())
    {
      Character->DrawToTileBuffer(Animate);
      return true;
    }
  else
    return false;
}*/

void wsquare::DrawStaticContents(bitmap* Bitmap, vector2d Pos, ushort Luminance, bool RealDraw) const
{
  GWTerrain->Draw(Bitmap, Pos, Luminance, false, RealDraw);
  OWTerrain->Draw(Bitmap, Pos, Luminance, true, RealDraw);
}

/*void wsquare::DrawCharacter(bool Animate) const
{
  if(Character && Character->CanBeSeenByPlayer())
    {
      Character->DrawToTileBuffer(Animate);
      return true;
    }
  else
    return false;
}*/

void wsquare::UpdateMemorized()
{
  if(MemorizedUpdateRequested)
    {
      ushort Luminance = (256 - ushort(75.0f * log(1.0f + fabs(GetWorldMapUnder()->GetAltitude(Pos)) / 500.0f)));
      DrawStaticContents(Memorized, vector2d(0, 0), Luminance, false);
      igraph::GetFOWGraphic()->MaskedBlit(Memorized);

      //DrawTerrain(false);

      //igraph::GetTileBuffer()->Blit(GetMemorized(), Luminance);
      //igraph::GetFOWGraphic()->MaskedBlit(GetMemorized());

      MemorizedUpdateRequested = false;
    }
}

void wsquare::Draw()
{
  if(NewDrawRequested || GetAnimatedEntities())
    {
      vector2d BitPos = game::CalculateScreenCoordinates(GetPos());
      ushort Luminance = (256 - ushort(75.0f * log(1.0f + fabs(GetWorldMapUnder()->GetAltitude(Pos)) / 500.0f))) * configuration::GetContrastLuminance() >> 8;
      DrawStaticContents(DOUBLEBUFFER, BitPos, Luminance, true);

      if(Character && (Character->CanBeSeenByPlayer() || game::GetSeeWholeMapCheat()))
	Character->Draw(DOUBLEBUFFER, BitPos, configuration::GetContrastLuminance(), true, true);

      //ushort ContrastLuminance = (configuration::GetContrast() << 8) / 100;

      //DrawTerrain(true);

      //igraph::GetTileBuffer()->Blit(igraph::GetTileBuffer(), Luminance);

      /*if(!configuration::GetOutlineCharacters())
	{
	  DrawCharacter(true);
	  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);
	}
      else
	{
	  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);

	  if(GetCharacter())
	    {
	      igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);
	      DrawCharacter(true);
	      igraph::GetTileBuffer()->Outline(configuration::GetCharacterOutlineColor());
	      igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);
	    }
	}*/

      NewDrawRequested = false;
    }
}

void wsquare::ChangeWTerrain(gwterrain* NewGround, owterrain* NewOver)
{
  ChangeGWTerrain(NewGround);
  ChangeOWTerrain(NewOver);
}

void wsquare::ChangeGWTerrain(gwterrain* NewGround)
{
  if(GetGWTerrain()->IsAnimated())
    DecAnimatedEntities();

  delete GetGWTerrain();
  SetGWTerrain(NewGround);

  if(NewGround->IsAnimated())
    IncAnimatedEntities();

  DescriptionChanged = NewDrawRequested = MemorizedUpdateRequested = true;
}

void wsquare::ChangeOWTerrain(owterrain* NewOver)
{
  if(GetOWTerrain()->IsAnimated())
    DecAnimatedEntities();

  delete GetOWTerrain();
  SetOWTerrain(NewOver);

  if(NewOver->IsAnimated())
    IncAnimatedEntities();

  DescriptionChanged = NewDrawRequested = MemorizedUpdateRequested = true;
}

void wsquare::SetWTerrain(gwterrain* NewGround, owterrain* NewOver)
{
  SetGWTerrain(NewGround);
  SetOWTerrain(NewOver);
}

void wsquare::SetGWTerrain(gwterrain* What)
{
  GWTerrain = What;

  if(What)
    {
      What->SetWSquareUnder(this);

      if(What->IsAnimated())
	IncAnimatedEntities();
    }
}

void wsquare::SetOWTerrain(owterrain* What)
{
  OWTerrain = What;

  if(What)
    {
      What->SetWSquareUnder(this);

      if(What->IsAnimated())
	IncAnimatedEntities();
    }
}

void wsquare::UpdateMemorizedDescription(bool)
{
  if(DescriptionChanged)
    {
      /*std::string Continent = GetWorldMapUnder()->GetContinentUnder(Pos) ? " of continent " + GetWorldMapUnder()->GetContinentUnder(Pos)->GetName() : "";

	if(GetOWTerrain()->Name(UNARTICLED) != "atmosphere")
	SetMemorizedDescription(GetOWTerrain()->Name(INDEFINITE) + " on " + GetGWTerrain()->Name(INDEFINITE) + Continent + ", height: " + GetWorldMapUnder()->GetAltitude(Pos) + " meters");
	else
	SetMemorizedDescription(GetGWTerrain()->Name(INDEFINITE) + Continent + ", height: " + GetWorldMapUnder()->GetAltitude(Pos) + " meters");*/

      if(GetOWTerrain()->Name(UNARTICLED) != "atmosphere")
	SetMemorizedDescription(GetOWTerrain()->Name(INDEFINITE) + " surrounded by " + GetGWTerrain()->Name(UNARTICLED));
      else
	SetMemorizedDescription(GetGWTerrain()->Name(UNARTICLED));

      DescriptionChanged = false;
    }
}

gterrain* wsquare::GetGTerrain() const
{
  return GWTerrain;
}

oterrain* wsquare::GetOTerrain() const
{
  return OWTerrain;
}

void wsquare::SetLastSeen(ulong What)
{
  if(LastSeen == What)
    return;

  if(!LastSeen)
    Memorized = new bitmap(16, 16);

  if(LastSeen < What - 1)
    NewDrawRequested = true;

  UpdateMemorized();
  UpdateMemorizedDescription();
  LastSeen = What;
}
