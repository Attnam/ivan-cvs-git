#include "typeable.h"
#include "game.h"

void typeable::Save(std::ofstream& SaveFile) const
{
	ushort TypeVar = Type();

	SaveFile.write((char*)&TypeVar, sizeof(TypeVar));
}

void typeable::Load(std::ifstream& SaveFile)
{
	AreaUnder = game::GetAreaInLoad();
	SquareUnder = game::GetSquareInLoad();
}
