#include <algorithm>

#include "wsquare.h"
#include "bitmap.h"
#include "worldmap.h"
#include "message.h"
#include "charba.h"
#include "femath.h"
#include "wterraba.h"
#include "game.h"
#include "festring.h"
#include "igraph.h"
#include "save.h"
#include "proto.h"
#include "whandler.h"

void wterrain::AddName(std::string& String, uchar Case) const
{
  if(!(Case & PLURAL))
    if(!(Case & ARTICLE_BIT))
      String << GetNameStem();
    else
      if(!(Case & INDEFINE_BIT))
	String << "the " << GetNameStem();
      else
	String << (LongerArticle() ? "an " : "a ") << GetNameStem();
  else
    if(!(Case & ARTICLE_BIT))
      String << GetNameStem() << " terrains";
    else
      if(!(Case & INDEFINE_BIT))
	String << "the " << GetNameStem() << " terrains";
      else
	String << GetNameStem() << " terrains";
}

std::string wterrain::GetName(uchar Case) const
{
  std::string Name;
  AddName(Name, Case);
  return Name;
}

void gwterrain::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool, bool AllowAnimate) const
{
  igraph::GetWTerrainGraphic()->Blit(Bitmap, GetBitmapPos(!AllowAnimate || AnimationFrames == 1 ? 0 : globalwindowhandler::GetTick() % AnimationFrames), Pos, 16, 16, Luminance);

  for(ushort c = 0; c < Neighbour.size(); ++c)
    igraph::GetWTerrainGraphic()->MaskedBlit(Bitmap, Neighbour[c].first, Pos, 16, 16, Luminance);
}

void owterrain::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool, bool AllowAnimate) const
{
  igraph::GetWTerrainGraphic()->MaskedBlit(Bitmap, GetBitmapPos(!AllowAnimate || AnimationFrames == 1 ? 0 : globalwindowhandler::GetTick() % AnimationFrames), Pos, 16, 16, Luminance);
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
  gwterrain* Terrain = Cloner(true);
  Terrain->Load(SaveFile);
  return Terrain;
}

owterrain* owterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  owterrain* Terrain = Cloner(true);
  Terrain->Load(SaveFile);
  return Terrain;
}

gwterrainprototype::gwterrainprototype(gwterrain* (*Cloner)(bool), const std::string& ClassId) : Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<gwterrain>::Add(this);
}

owterrainprototype::owterrainprototype(owterrain* (*Cloner)(bool), const std::string& ClassId) : Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<owterrain>::Add(this);
}

bool DrawOrderer(const std::pair<vector2d, uchar>& Pair1, const std::pair<vector2d, uchar>& Pair2)
{
  return Pair1.second < Pair2.second;
}

void gwterrain::CalculateNeighbourBitmapPoses()
{
  Neighbour.clear();

  for(ushort d = 0; d < 8; ++d)
    {
      wsquare* NeighbourSquare = GetWorldMapUnder()->GetNeighbourWSquare(GetPos(), d);

      if(NeighbourSquare)
	{
	  gwterrain* DoNeighbour = NeighbourSquare->GetGWTerrain();

	  if(DoNeighbour->Priority() > Priority())
	    Neighbour.push_back(std::pair<vector2d, uchar>(DoNeighbour->GetBitmapPos(0) - (game::GetMoveVector(d) << 4), DoNeighbour->Priority()));
	}
    }

  std::sort(Neighbour.begin(), Neighbour.end(), DrawOrderer);
}

bool owterrain::Enter(bool DirectionUp) const
{
  if(DirectionUp)
    {
      ADD_MESSAGE("You jump into the air. For some reason you don't get too far above.");
      return false;
    }

  uchar AttachedDungeon = GetAttachedDungeon();

  if(!AttachedDungeon)
    {
      ADD_MESSAGE("There seems to be nothing of interest here.");
      return false;
    }

  std::vector<character*> Group;

  /* These must be backupped, since LeaveWorldMap destroys the terrain */

  uchar AttachedArea = GetAttachedArea();
  uchar AttachedEntry = GetAttachedEntry();

  if(game::LeaveWorldMap(Group))
    {
      game::SetCurrentDungeon(AttachedDungeon);
      game::EnterArea(Group, AttachedArea, AttachedEntry);
      return true;
    }
  else
    return false;
}
