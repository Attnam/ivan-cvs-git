#include "type.h"
#include "save.h"

void type::Save(outputfile& SaveFile) const
{
  SaveFile << Type();
}


