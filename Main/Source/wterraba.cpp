#include <queue>

#include "wsquare.h"
#include "bitmap.h"
#include "worldmap.h"
#include "message.h"
#include "charba.h"
#include "femath.h"

struct prioritypair
{
  prioritypair(uchar Priority, vector2d BitmapPos) : Priority(Priority), BitmapPos(BitmapPos) { }
  bool operator < (const prioritypair& AnotherPair) const { return Priority > AnotherPair.Priority; }
  uchar Priority;
  vector2d BitmapPos;
};

std::string wterrain::Name(uchar Case) const
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

vector2d wterrain::GetPos() const
{
  return GetWSquareUnder()->GetPos();
}

void gwterrain::DrawToTileBuffer() const
{
  igraph::GetWTerrainGraphic()->Blit(igraph::GetTileBuffer(), GetBitmapPos().X, GetBitmapPos().Y, 0, 0, 16, 16);

  std::priority_queue<prioritypair> Neighbour;

  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, GetWorldMapUnder()->GetXSize(), GetWorldMapUnder()->GetYSize(),
  {
    gwterrain* DoNeighbour = GetWorldMapUnder()->GetWSquare(DoX, DoY)->GetGWTerrain();

    if(DoNeighbour->Priority() > Priority())
      Neighbour.push(prioritypair(DoNeighbour->Priority(), DoNeighbour->GetBitmapPos() - (game::GetMoveVector(DoIndex) << 4)));
  });

  while(Neighbour.size())
    {
      igraph::GetWTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), Neighbour.top().BitmapPos.X, Neighbour.top().BitmapPos.Y, 0, 0, 16, 16);
      Neighbour.pop();
    }
}

void owterrain::DrawToTileBuffer() const
{
  igraph::GetWTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X, GetBitmapPos().Y, 0, 0, 16, 16);
}

worldmap* wterrain::GetWorldMapUnder() const
{
  return GetWSquareUnder()->GetWorldMapUnder();
}

bool owterrain::GoUp(character*) const
{
  ADD_MESSAGE("You jump into the air. For some reason you don't get too far above.");

  return false;
}

bool owterrain::GoDown(character*) const
{
  ADD_MESSAGE("There seems to be nothing of interest here.");

  return false;
}

void wterrain::Load(inputfile& SaveFile)
{
  type::Load(SaveFile);

  WSquareUnder = (wsquare*)game::GetSquareInLoad();
}



