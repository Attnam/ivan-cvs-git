#include "terra.h"
#include "charba.h"

bool gterrain::IsWalkable(character* ByWho) const
{
  return !ByWho || ByWho->CanWalk() || ByWho->CanFly();
}
