#include "typeable.h"
#include "save.h"
#include "rand.h"

void typeable::Save(outputfile& SaveFile) const
{
	SaveFile << Type();
}
