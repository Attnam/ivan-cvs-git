#include <cmath>

#include "strover.h"
#include "wsquare.h"
#include "igraph.h"
#include "charba.h"
#include "worldmap.h"
#include "wterraba.h"
#include "proto.h"
#include "materba.h"
#include "bitmap.h"
#include "save.h"
#include "graphics.h"
#include "config.h"

worldmapsquare::worldmapsquare(worldmap* WorldMapUnder, vector2d Pos) : square(WorldMapUnder, Pos), GroundWorldMapTerrain(0), OverWorldMapTerrain(0)
{
}

worldmapsquare::~worldmapsquare()
{
  delete GetGroundWorldMapTerrain();
  delete GetOverWorldMapTerrain();
}

void worldmapsquare::Save(outputfile& SaveFile) const
{
  square::Save(SaveFile);

  SaveFile << GroundWorldMapTerrain << OverWorldMapTerrain;
}

void worldmapsquare::Load(inputfile& SaveFile)
{
  square::Load(SaveFile);

  SaveFile >> GroundWorldMapTerrain >> OverWorldMapTerrain;
}

bool worldmapsquare::DrawTerrain() const
{
  GetGroundWorldMapTerrain()->DrawToTileBuffer();
  GetOverWorldMapTerrain()->DrawToTileBuffer();

  return true;
}

bool worldmapsquare::DrawCharacters() const
{
  if(GetCharacter())
    {
      GetCharacter()->DrawToTileBuffer();
      return true;
    }
  else
    return false;
}

void worldmapsquare::UpdateMemorized()
{
  if(MemorizedUpdateRequested)
    {
      ushort Luminance = 256 - ushort(75.0f * log(1.0f + fabs(GetWorldMapUnder()->GetAltitude(Pos)) / 500.0f));

      DrawTerrain();

      igraph::GetTileBuffer()->Blit(GetMemorized(), 0, 0, 0, 0, 16, 16, Luminance);
      igraph::GetFOWGraphic()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16);

      MemorizedUpdateRequested = false;
    }
}

void worldmapsquare::Draw()
{
  if(NewDrawRequested)
    {
      vector2d BitPos = vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4);

      ushort Luminance = 256 - ushort(75.0f * log(1.0f + fabs(GetWorldMapUnder()->GetAltitude(Pos)) / 500.0f));
      ushort ContrastLuminance = ushort(256.0f * configuration::GetContrast() / 100);

      DrawTerrain();

      igraph::GetTileBuffer()->Blit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, Luminance);

      if(!configuration::GetOutlineCharacters())
	{
	  DrawCharacters();
	  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, ContrastLuminance);
	}
      else
	{
	  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, ContrastLuminance);

	  if(GetCharacter())
	    {
	      igraph::GetTileBuffer()->Fill(0xF81F);
	      DrawCharacters();
	      igraph::GetTileBuffer()->Outline(configuration::GetCharacterOutlineColor());
	      igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, ContrastLuminance);
	    }
	}

      NewDrawRequested = false;
    }
}

void worldmapsquare::ChangeWorldMapTerrain(groundworldmapterrain* NewGround, overworldmapterrain* NewOver)
{
  ChangeGroundWorldMapTerrain(NewGround);
  ChangeOverWorldMapTerrain(NewOver);
}

void worldmapsquare::ChangeGroundWorldMapTerrain(groundworldmapterrain* NewGround)
{
  delete GetGroundWorldMapTerrain();
  SetGroundWorldMapTerrain(NewGround);
  DescriptionChanged = NewDrawRequested = MemorizedUpdateRequested = true;
}

void worldmapsquare::ChangeOverWorldMapTerrain(overworldmapterrain* NewOver)
{
  delete GetOverWorldMapTerrain();
  SetOverWorldMapTerrain(NewOver);
  DescriptionChanged = NewDrawRequested = MemorizedUpdateRequested = true;
}

void worldmapsquare::SetGroundWorldMapTerrain(groundworldmapterrain* What)
{
  GroundWorldMapTerrain = What;

  if(What)
    What->SetWorldMapSquareUnder(this);
}

void worldmapsquare::SetOverWorldMapTerrain(overworldmapterrain* What)
{
  OverWorldMapTerrain = What;

  if(What)
    What->SetWorldMapSquareUnder(this);
}

void worldmapsquare::UpdateMemorizedDescription(bool)
{
  if(DescriptionChanged)
    {
      /*std::string Continent = GetWorldMapUnder()->GetContinentUnder(Pos) ? " of continent " + GetWorldMapUnder()->GetContinentUnder(Pos)->GetName() : "";

	if(GetOverWorldMapTerrain()->Name(UNARTICLED) != "atmosphere")
	SetMemorizedDescription(GetOverWorldMapTerrain()->Name(INDEFINITE) + " on " + GetGroundWorldMapTerrain()->Name(INDEFINITE) + Continent + ", height: " + GetWorldMapUnder()->GetAltitude(Pos) + " meters");
	else
	SetMemorizedDescription(GetGroundWorldMapTerrain()->Name(INDEFINITE) + Continent + ", height: " + GetWorldMapUnder()->GetAltitude(Pos) + " meters");*/

      if(GetOverWorldMapTerrain()->Name(UNARTICLED) != "atmosphere")
	SetMemorizedDescription(GetOverWorldMapTerrain()->Name(INDEFINITE) + " surrounded by " + GetGroundWorldMapTerrain()->Name(UNARTICLED));
      else
	SetMemorizedDescription(GetGroundWorldMapTerrain()->Name(UNARTICLED));

      DescriptionChanged = false;
    }
}

groundterrain* worldmapsquare::GetGroundTerrain() const
{
  return GroundWorldMapTerrain;
}

overterrain* worldmapsquare::GetOverTerrain() const
{
  return OverWorldMapTerrain;
}
