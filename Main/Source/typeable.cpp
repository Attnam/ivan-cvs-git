#include "typeable.h"
#include "save.h"

void typeable::Save(outputfile& SaveFile) const
{
  SaveFile << Type();
}
