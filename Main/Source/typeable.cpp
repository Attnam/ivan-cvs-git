#include "typeable.h"
#include "game.h"
#include "save.h"

void typeable::Save(outputfile& SaveFile) const
{
	SaveFile << Type();
}

void typeable::Load(inputfile& SaveFile)
{
	AreaUnder = game::GetAreaInLoad();
	SquareUnder = game::GetSquareInLoad();
}
