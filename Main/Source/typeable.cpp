#include "typeable.h"
#include "game.h"
#include "save.h"
#include "level.h"
#include "lsquare.h"

typeable::typeable() : AreaUnder(0), SquareUnder(0)
{
}

void typeable::Save(outputfile& SaveFile) const
{
	SaveFile << Type();
}

void typeable::Load(inputfile& SaveFile)
{
	AreaUnder = game::GetAreaInLoad();
	SquareUnder = game::GetSquareInLoad();
}

level* typeable::GetLevelUnder() const
{
	return (level*)AreaUnder;
}

void typeable::SetLevelUnder(level* What)
{
	AreaUnder = What;
}

levelsquare* typeable::GetLevelSquareUnder() const
{
	return (levelsquare*)SquareUnder;
}

void typeable::SetLevelSquareUnder(levelsquare* What)
{
	SquareUnder = What;
}
