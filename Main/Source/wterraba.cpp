#include <queue>

#include "wterraba.h"
#include "wsquare.h"
#include "igraph.h"
#include "bitmap.h"
#include "object.h"
#include "materba.h"
#include "proto.h"
#include "worldmap.h"
#include "game.h"
#include "save.h"
#include "message.h"
#include "charba.h"
#include "team.h"
#include "femath.h"

struct prioritypair
{
	prioritypair(uchar Priority, vector2d BitmapPos) : Priority(Priority), BitmapPos(BitmapPos) {}
	bool operator < (const prioritypair& AnotherPair) const { return Priority > AnotherPair.Priority; }
	uchar Priority;
	vector2d BitmapPos;
};

std::string worldmapterrain::Name(uchar Case) const
{
	if(!(Case & PLURAL))
		if(!(Case & DEFINEBIT))
			return NameStem();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + NameStem();
			else
				return Article() + " " + NameStem();
	else
		if(!(Case & DEFINEBIT))
			return NameStem() + " terrains";
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + NameStem() + " terrains";
			else
				return NameStem() + " terrains";
}

vector2d worldmapterrain::GetPos() const
{
	return GetWorldMapSquareUnder()->GetPos();
}

void groundworldmapterrain::DrawToTileBuffer() const
{
	igraph::GetWorldMapTerrainGraphic()->Blit(igraph::GetTileBuffer(), GetBitmapPos().X, GetBitmapPos().Y, 0, 0, 16, 16);

	std::priority_queue<prioritypair> Neighbour;

	DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, GetWorldMapUnder()->GetXSize(), GetWorldMapUnder()->GetYSize(),
	{
		groundworldmapterrain* DoNeighbour = GetWorldMapUnder()->GetWorldMapSquare(DoX, DoY)->GetGroundWorldMapTerrain();

		if(DoNeighbour->Priority() > Priority())
			Neighbour.push(prioritypair(DoNeighbour->Priority(), DoNeighbour->GetBitmapPos() - (game::GetMoveVector(DoIndex) << 4)));
	})

	while(Neighbour.size())
	{
		igraph::GetWorldMapTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), Neighbour.top().BitmapPos.X, Neighbour.top().BitmapPos.Y, 0, 0, 16, 16);
		Neighbour.pop();
	}
}

void overworldmapterrain::DrawToTileBuffer() const
{
	igraph::GetWorldMapTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X, GetBitmapPos().Y, 0, 0, 16, 16);
}

worldmap* worldmapterrain::GetWorldMapUnder() const
{
	return GetWorldMapSquareUnder()->GetWorldMapUnder();
}

bool overworldmapterrain::GoUp(character*) const
{
	ADD_MESSAGE("You jump into the air. For some reason you don't get too far above.");

	return false;
}

bool overworldmapterrain::GoDown(character*) const
{
	ADD_MESSAGE("There seems to be nothing of interest here.");

	return false;
}

void worldmapterrain::Load(inputfile& SaveFile)
{
	typeable::Load(SaveFile);

	WorldMapSquareUnder = (worldmapsquare*)game::GetSquareInLoad();
}
