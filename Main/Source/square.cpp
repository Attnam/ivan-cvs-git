#include "charba.h"
#include "square.h"
#include "lterraba.h"
#include "igraph.h"
#include "level.h"
#include "proto.h"
#include "materba.h"
#include "strover.h"
#include "save.h"
#include "config.h"
#include "femath.h"

square::square(area* AreaUnder, vector2d Pos) : AreaUnder(AreaUnder), Character(0), Pos(Pos), NewDrawRequested(true), MemorizedUpdateRequested(true), Memorized(0), LastSeen(0), DescriptionChanged(true)
{
}

square::~square()
{
  delete Character;
  delete Memorized;
}

void square::Save(outputfile& SaveFile) const
{
  SaveFile << Character << LastSeen << DescriptionChanged;

  if(LastSeen)
    GetMemorized()->Save(SaveFile);

  SaveFile << MemorizedDescription;
}

void square::Load(inputfile& SaveFile)
{
  game::SetSquareInLoad(this);

  SaveFile >> Character >> LastSeen >> DescriptionChanged;

  if(LastSeen)
    {
      Memorized = new bitmap(16, 16);
      GetMemorized()->Load(SaveFile);
    }

  SaveFile >> MemorizedDescription;
}

void square::AddCharacter(character* Guy)
{
  Character = Guy;
  Guy->SetSquareUnder(this);
  NewDrawRequested = true;
}

void square::DrawMemorized()
{
  if(GetLastSeen() && (NewDrawRequested || GetLastSeen() == game::GetLOSTurns() - 1))
    {	
      GetMemorized()->Blit(DOUBLEBUFFER, 0, 0, (GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4, 16, 16, ushort(256.0f * configuration::GetContrast() / 100));
      NewDrawRequested = false;
    }
}

void square::RemoveCharacter()
{
  SetCharacter(0);
  NewDrawRequested = true;
}

bool square::CanBeSeen(bool) const
{
  if(GetLastSeen() == game::GetLOSTurns())
    return true;
  else
    return false;
}

bool square::CanBeSeenFrom(vector2d FromPos, ulong MaxDistance, bool) const
{
  return femath::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, MaxDistance, game::EyeHandler);
}

void square::SetLastSeen(ulong What)
{
  if(!GetLastSeen())
    Memorized = new bitmap(16, 16);

  if(GetLastSeen() < What - 1 || !GetOverTerrain()->GetIsWalkable())
    SendNewDrawRequest();

  UpdateMemorized();
  UpdateMemorizedDescription();

  LastSeen = What;
}

void square::KickAnyoneStandingHereAway()
{
  if(Character)
    {
      GetAreaUnder()->AddCharacter(GetAreaUnder()->GetNearestFreeSquare(Character, GetPos()), Character);
      RemoveCharacter();
    }
}

bool square::GetIsWalkable(character* Char) const
{
  return GetOverTerrain()->GetIsWalkable() && GetGroundTerrain()->GetIsWalkable(Char);
}
