#include <queue>

#include "wsquare.h"
#include "bitmap.h"
#include "worldmap.h"
#include "message.h"
#include "charba.h"
#include "femath.h"
#include "wterraba.h"
#include "game.h"
#include "strover.h"
#include "igraph.h"
#include "save.h"
#include "proto.h"

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
    if(!(Case & ARTICLEBIT))
      return NameStem();
    else
      if(!(Case & INDEFINEBIT))
	return "the " + NameStem();
      else
	return Article() + " " + NameStem();
  else
    if(!(Case & ARTICLEBIT))
      return NameStem() + " terrains";
    else
      if(!(Case & INDEFINEBIT))
	return "the " + NameStem() + " terrains";
      else
	return NameStem() + " terrains";
}

vector2d wterrain::GetPos() const
{
  return GetWSquareUnder()->GetPos();
}

void gwterrain::DrawToTileBuffer(bool) const
{
  igraph::GetWTerrainGraphic()->Blit(igraph::GetTileBuffer(), GetBitmapPos(0), 0, 0, 16, 16);

  std::priority_queue<prioritypair> Neighbour;

  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, GetWorldMapUnder()->GetXSize(), GetWorldMapUnder()->GetYSize(),
  {
    gwterrain* DoNeighbour = GetWorldMapUnder()->GetWSquare(DoX, DoY)->GetGWTerrain();

    if(DoNeighbour->Priority() > Priority())
      Neighbour.push(prioritypair(DoNeighbour->Priority(), DoNeighbour->GetBitmapPos(0) - (game::GetMoveVector(DoIndex) << 4)));
  });

  while(Neighbour.size())
    {
      igraph::GetWTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), Neighbour.top().BitmapPos, 0, 0, 16, 16);
      Neighbour.pop();
    }
}

void owterrain::DrawToTileBuffer(bool) const
{
  igraph::GetWTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos(0), 0, 0, 16, 16);
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

void wterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
}

void wterrain::Load(inputfile&)
{
  WSquareUnder = (wsquare*)game::GetSquareInLoad();
}

gwterrain* gwterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  gwterrain* Terrain = Clone();
  Terrain->Load(SaveFile);
  return Terrain;
}

owterrain* owterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  owterrain* Terrain = Clone();
  Terrain->Load(SaveFile);
  return Terrain;
}

gwterrainprototype::gwterrainprototype()
{
  Index = protocontainer<gwterrain>::Add(this);
}

owterrainprototype::owterrainprototype()
{
  Index = protocontainer<owterrain>::Add(this);
}
