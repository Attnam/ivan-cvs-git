#include "terra.h"
#include "save.h"

void overterrain::Save(outputfile& SaveFile) const
{
	SaveFile << IsWalkable;
}

void overterrain::Load(inputfile& SaveFile)
{
	SaveFile >> IsWalkable;
}
