#include "wsquare.h"
#include "igraph.h"
#include "char.h"
#include "worldmap.h"
#include "wterrain.h"
#include "proto.h"
#include "material.h"
#include "bitmap.h"

worldmapsquare::worldmapsquare(worldmap* MotherWorldMap, vector Pos) : square(MotherWorldMap, Pos)
{
}

worldmapsquare::~worldmapsquare(void)
{
	delete GetGroundWorldMapTerrain();
	delete GetOverWorldMapTerrain();
}

void worldmapsquare::Save(std::ofstream& SaveFile) const
{
	square::Save(SaveFile);
}

void worldmapsquare::Load(std::ifstream& SaveFile)
{
	square::Load(SaveFile);
}

void worldmapsquare::DrawToTileBuffer(void) const
{
	GetGroundWorldMapTerrain()->DrawToTileBuffer();
	GetOverWorldMapTerrain()->DrawToTileBuffer();
}

void worldmapsquare::UpdateMemorizedAndDraw(void)
{
	SetKnown(true);

	DrawToTileBuffer();

	igraph::GetTileBuffer()->MaskedBlit(MotherArea->GetMemorized(), 0, 0, Pos.X << 4, Pos.Y << 4, 16, 16);
	igraph::GetFOWGraphic()->MaskedBlit(MotherArea->GetMemorized(), 0, 0, Pos.X << 4, Pos.Y << 4, 16, 16);

	if(GetCharacter())
		GetCharacter()->DrawToTileBuffer();

	igraph::BlitTileBuffer(vector((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4));
}

void worldmapsquare::ChangeWorldMapTerrain(groundworldmapterrain* NewGround, overworldmapterrain* NewOver)
{
	delete GetGroundWorldMapTerrain();
	SetGroundWorldMapTerrain(NewGround);
	delete GetOverWorldMapTerrain();
	SetOverWorldMapTerrain(NewOver);
	TerrainChanged = true;
}

void worldmapsquare::SetGroundWorldMapTerrain(groundworldmapterrain* What)
{
	GroundTerrain = What;

	if(What)
		What->SetSquareUnder(this);
}

void worldmapsquare::SetOverWorldMapTerrain(overworldmapterrain* What)
{
	OverTerrain = What;

	if(What)
		What->SetSquareUnder(this);
}

groundworldmapterrain* worldmapsquare::GetGroundWorldMapTerrain(void) const
{
	return (groundworldmapterrain*)GroundTerrain;
}

overworldmapterrain* worldmapsquare::GetOverWorldMapTerrain(void) const
{
	return (overworldmapterrain*)OverTerrain;
}

void worldmapsquare::UpdateMemorizedDescription(void)
{
	if(TerrainChanged)
	{
		if(GetOverTerrain()->Name(UNARTICLED) != "atmosphere")
			SetMemorizedDescription(GetOverTerrain()->Name(INDEFINITE) + " on " + GetGroundTerrain()->Name(INDEFINITE));
		else
			SetMemorizedDescription(GetGroundTerrain()->Name(INDEFINITE));

		TerrainChanged = false;
	}
}
