#include <queue>

#include "area.h"
#include "wterrain.h"
#include "wsquare.h"
#include "igraph.h"
#include "bitmap.h"
#include "object.h"
#include "material.h"
#include "proto.h"
#include "worldmap.h"
#include "game.h"

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

vector worldmapterrain::GetPos(void) const
{
	return GetWorldMapSquareUnder()->GetPos();
}

/*void groundworldmapterrain::DrawNeighbour(vector Pos, uchar NeighbourIndex) const
{
	
}*/

struct prioritypair
{
	prioritypair(uchar Priority, vector BitmapPos) : Priority(Priority), BitmapPos(BitmapPos) {}
	bool operator < (const prioritypair& AnotherPair) const { return Priority > AnotherPair.Priority; }
	uchar Priority;
	vector BitmapPos;
};

void groundworldmapterrain::DrawToTileBuffer(void) const
{
	igraph::GetWorldMapTerrainGraphic()->Blit(igraph::GetTileBuffer(), GetBitmapPos().X, GetBitmapPos().Y, 0, 0, 16, 16);

	std::priority_queue<prioritypair> Neighbour;

	DO_FOR_SQUARES_AROUND(GetWorldMapSquareUnder()->GetPos().X, GetWorldMapSquareUnder()->GetPos().Y, GetWorldMapUnder()->GetXSize(), GetWorldMapUnder()->GetYSize(),
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

/*void groundworldmapterrain::DrawToTileBuffer(void) const
{
	igraph::GetWorldMapTerrainGraphic()->Blit(igraph::GetTileBuffer(), GetBitmapPos().X, GetBitmapPos().Y, 0, 0, 16, 16);

	//std::priority_queue<prioritypair> Neighbour;

	DO_FOR_SQUARES_AROUND_IN_TWO_PARTS(GetWorldMapSquareUnder()->GetPos().X, GetWorldMapSquareUnder()->GetPos().Y, GetWorldMapUnder()->GetXSize(), GetWorldMapUnder()->GetYSize(),
	{
		vector NeighbourBitmap = GetWorldMapUnder()->GetWorldMapSquare(DoX, DoY)->GetGroundWorldMapTerrain()->GetBitmapPos() - (game::GetMoveVector(DoIndex) << 4);

		igraph::GetWorldMapTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), NeighbourBitmap.X, NeighbourBitmap.Y, 0, 0, 16, 16);

		//if(DoNeighbour->Priority() > Priority())
		//	Neighbour.push(prioritypair(DoNeighbour->Priority(), ));
	}, ;, ;)

	/while(Neighbour.size())
	{
		igraph::GetWorldMapTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), Neighbour.top().BitmapPos.X, Neighbour.top().BitmapPos.Y, 0, 0, 16, 16);
		Neighbour.pop();
	}/
}*/

void overworldmapterrain::DrawToTileBuffer(void) const
{
	igraph::GetWorldMapTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X, GetBitmapPos().Y, 0, 0, 16, 16);
}

void overworldmapterrain::Save(std::ofstream& SaveFile) const
{
	typeable::Save(SaveFile);
	overterrain::Save(SaveFile);
}

void overworldmapterrain::Load(std::ifstream& SaveFile)
{
	typeable::Load(SaveFile);
	overterrain::Load(SaveFile);
}

worldmapsquare* worldmapterrain::GetWorldMapSquareUnder(void) const
{
	return (worldmapsquare*)SquareUnder;
}

worldmap* worldmapterrain::GetWorldMapUnder(void) const
{
	return (worldmap*)AreaUnder;
}
