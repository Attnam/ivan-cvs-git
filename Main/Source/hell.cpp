#include "hell.h"
#include "entity.h"

std::vector<entity*> hell::ToBeBurned;

void hell::Burn()
{
  for(ushort c = 0; c < ToBeBurned.size(); ++c)
    delete ToBeBurned[c];

  ToBeBurned.clear();
}
