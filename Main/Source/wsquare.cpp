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

void worldmapsquare::Save(std::ofstream* SaveFile) const
{
	square::Save(SaveFile);

	GetGroundWorldMapTerrain()->Save(SaveFile);
	GetOverWorldMapTerrain()->Save(SaveFile);
}

void worldmapsquare::Load(std::ifstream* SaveFile)
{
	square::Load(SaveFile);

	SetGroundWorldMapTerrain(prototypesystem::LoadGroundWorldMapTerrain(SaveFile));
	SetOverWorldMapTerrain(prototypesystem::LoadOverWorldMapTerrain(SaveFile));
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

	if(CCharacter())
		CCharacter()->DrawToTileBuffer();

	igraph::BlitTileBuffer(vector((GetPos().X - game::CCamera().X) << 4, (GetPos().Y - game::CCamera().Y + 2) << 4));
}

void worldmapsquare::ChangeWorldMapTerrain(groundworldmapterrain* NewGround, overworldmapterrain* NewOver)
{
	delete GetGroundWorldMapTerrain();
	SetGroundWorldMapTerrain(NewGround);
	delete GetOverWorldMapTerrain();
	SetOverWorldMapTerrain(NewOver);
}

void worldmapsquare::SetGroundWorldMapTerrain(groundworldmapterrain* What)
{
	GroundTerrain = What;

	if(What)
		What->SetWorldMapSquareUnder(this);
}

void worldmapsquare::SetOverWorldMapTerrain(overworldmapterrain* What)
{
	OverTerrain = What;

	if(What)
		What->SetWorldMapSquareUnder(this);
}

groundworldmapterrain* worldmapsquare::GetGroundWorldMapTerrain(void) const
{
	return (groundworldmapterrain*)GroundTerrain;
}

overworldmapterrain* worldmapsquare::GetOverWorldMapTerrain(void) const
{
	return (overworldmapterrain*)OverTerrain;
}
