#include "wterrain.h"
#include "wsquare.h"
#include "igraph.h"
#include "bitmap.h"

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

void overworldmapterrain::Save(std::ofstream* SaveFile) const
{
	worldmapterrain::Save(SaveFile);
	overterrain::Save(SaveFile);

	//SaveFile->write((char*)&IsWalkable, sizeof(IsWalkable));
}

void overworldmapterrain::Load(std::ifstream* SaveFile)
{
	worldmapterrain::Load(SaveFile);
	overterrain::Load(SaveFile);

	//SaveFile->read((char*)&IsWalkable, sizeof(IsWalkable));
}