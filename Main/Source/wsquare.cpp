#include "wsquare.h"
#include "terrain.h"
#include "game.h"
#include "igraph.h"
#include "area.h"
#include "char.h"

worldmapsquare::worldmapsquare(area* MotherArea, vector Pos) : square(MotherArea, Pos)
{
}

worldmapsquare::~worldmapsquare(void)
{
}

void worldmapsquare::DrawToTileBuffer(void)
{
	GroundTerrain->DrawToTileBuffer();
	OverTerrain->DrawToTileBuffer();
}

void worldmapsquare::UpdateMemorizedAndDraw(void)
{
	SKnown(true);

	DrawToTileBuffer();

	if(CCharacter())
		CCharacter()->DrawToTileBuffer();

	igraph::BlitTileBuffer(vector((CPos().X - game::CCamera().X) << 4, (CPos().Y - game::CCamera().Y + 2) << 4), 256);
}
