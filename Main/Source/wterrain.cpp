#include "wterrain.h"
#include "wsquare.h"
#include "igraph.h"
#include "bitmap.h"
#include "object.h"
#include "material.h"
#include "proto.h"

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

void groundworldmapterrain::DrawToTileBuffer(void) const
{
	igraph::GetWorldMapTerrainGraphic()->Blit(igraph::GetTileBuffer(), GetBitmapPos().X, GetBitmapPos().Y, 0, 0, 16, 16);
}

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
	return dynamic_cast<worldmapsquare*>(SquareUnder);
}
