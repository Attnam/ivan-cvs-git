#include <cmath>

#include "strover.h"
#include "wsquare.h"
#include "charba.h"
#include "worldmap.h"
#include "bitmap.h"
#include "graphics.h"
#include "config.h"

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

bool wsquare::DrawTerrain() const
{
  GetGWTerrain()->DrawToTileBuffer();
  GetOWTerrain()->DrawToTileBuffer();

  return true;
}

bool wsquare::DrawCharacters() const
{
  if(GetCharacter())
    {
      GetCharacter()->DrawToTileBuffer();
      return true;
    }
  else
    return false;
}

void wsquare::UpdateMemorized()
{
  if(MemorizedUpdateRequested)
    {
      ushort Luminance = 256 - ushort(75.0f * log(1.0f + fabs(GetWorldMapUnder()->GetAltitude(Pos)) / 500.0f));

      DrawTerrain();

      igraph::GetTileBuffer()->Blit(GetMemorized(), Luminance);
      igraph::GetFOWGraphic()->MaskedBlit(GetMemorized());

      MemorizedUpdateRequested = false;
    }
}

void wsquare::Draw()
{
  if(NewDrawRequested)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(GetPos());

      ushort Luminance = 256 - ushort(75.0f * log(1.0f + fabs(GetWorldMapUnder()->GetAltitude(Pos)) / 500.0f));
      ushort ContrastLuminance = ushort(256.0f * configuration::GetContrast() / 100);

      DrawTerrain();

      igraph::GetTileBuffer()->Blit(igraph::GetTileBuffer(), Luminance);

      if(!configuration::GetOutlineCharacters())
	{
	  DrawCharacters();
	  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);
	}
      else
	{
	  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);

	  if(GetCharacter())
	    {
	      igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);
	      DrawCharacters();
	      igraph::GetTileBuffer()->Outline(configuration::GetCharacterOutlineColor());
	      igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);
	    }
	}

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
  delete GetGWTerrain();
  SetGWTerrain(NewGround);
  DescriptionChanged = NewDrawRequested = MemorizedUpdateRequested = true;
}

void wsquare::ChangeOWTerrain(owterrain* NewOver)
{
  delete GetOWTerrain();
  SetOWTerrain(NewOver);
  DescriptionChanged = NewDrawRequested = MemorizedUpdateRequested = true;
}

void wsquare::SetGWTerrain(gwterrain* What)
{
  GWTerrain = What;

  if(What)
    What->SetWSquareUnder(this);
}

void wsquare::SetOWTerrain(owterrain* What)
{
  OWTerrain = What;

  if(What)
    What->SetWSquareUnder(this);
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

