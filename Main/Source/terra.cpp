#include "terra.h"
#include "charba.h"

bool gterrain::GetIsWalkable(character* ByWho) const
{
  return !ByWho || ByWho->CanWalk() || ByWho->CanFly();
}


