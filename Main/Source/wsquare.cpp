#include "wsquare.h"
#include "lterrain.h"
#include "game.h"
#include "igraph.h"
#include "area.h"
#include "char.h"
#include "worldmap.h"
#include "wterrain.h"

worldmapsquare::worldmapsquare(worldmap* MotherWorldMap, vector Pos) : square(MotherWorldMap, Pos), MotherWorldMap(MotherWorldMap), GroundWorldMapTerrain(0), OverWorldMapTerrain(0)
{
}

worldmapsquare::~worldmapsquare(void)
{
	delete GroundWorldMapTerrain;
	delete OverWorldMapTerrain;
}

void worldmapsquare::DrawToTileBuffer(void) const
{
	GroundWorldMapTerrain->DrawToTileBuffer();

	if(OverWorldMapTerrain)
		OverWorldMapTerrain->DrawToTileBuffer();
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
	delete GroundWorldMapTerrain;
	SetGroundWorldMapTerrain(NewGround);
	GetGroundWorldMapTerrain()->SetWorldMapSquareUnder(this);
	delete OverWorldMapTerrain;
	SetOverWorldMapTerrain(NewOver);
	if(NewOver)
		GetOverWorldMapTerrain()->SetWorldMapSquareUnder(this);
}
