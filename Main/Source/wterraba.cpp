#include <queue>

#include "wsquare.h"
#include "bitmap.h"
#include "worldmap.h"
#include "message.h"
#include "charba.h"
#include "femath.h"
#include "wterraba.h"
#include "game.h"
#include "stdover.h"
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
      return GetNameStem();
    else
      if(!(Case & INDEFINEBIT))
	return "the " + GetNameStem();
      else
	return GetArticle() + " " + GetNameStem();
  else
    if(!(Case & ARTICLEBIT))
      return GetNameStem() + " terrains";
    else
      if(!(Case & INDEFINEBIT))
	return "the " + GetNameStem() + " terrains";
      else
	return GetNameStem() + " terrains";
}

vector2d wterrain::GetPos() const
{
  return GetWSquareUnder()->GetPos();
}

void gwterrain::Draw(bitmap* Bitmap, vector2d Pos, ushort Luminance, bool, bool) const
{
  igraph::GetWTerrainGraphic()->Blit(Bitmap, GetBitmapPos(0), Pos, 16, 16, Luminance);
  std::priority_queue<prioritypair> Neighbour;

  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, GetWorldMapUnder()->GetXSize(), GetWorldMapUnder()->GetYSize(),
  {
    gwterrain* DoNeighbour = GetWorldMapUnder()->GetWSquare(DoX, DoY)->GetGWTerrain();

    if(DoNeighbour->Priority() > Priority())
      Neighbour.push(prioritypair(DoNeighbour->Priority(), DoNeighbour->GetBitmapPos(0) - (game::GetMoveVector(DoIndex) << 4)));
  });

  while(Neighbour.size())
    {
      igraph::GetWTerrainGraphic()->MaskedBlit(Bitmap, Neighbour.top().BitmapPos, Pos, 16, 16, Luminance);
      Neighbour.pop();
    }
}

void owterrain::Draw(bitmap* Bitmap, vector2d Pos, ushort Luminance, bool, bool) const
{
  igraph::GetWTerrainGraphic()->MaskedBlit(Bitmap, GetBitmapPos(0), Pos, 16, 16, Luminance);
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

void wterrain::Load(inputfile&)
{
  WSquareUnder = (wsquare*)game::GetSquareInLoad();
}

void gwterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
}

void owterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
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

