#include "terra.h"
#include "charba.h"

bool gterrain::IsWalkable(const character* ByWho) const
{
  return !ByWho || ByWho->CanWalk() || ByWho->CanFly();
}
