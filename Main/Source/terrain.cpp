#include "terrain.h"

void overterrain::Save(std::ofstream* SaveFile) const
{
	SaveFile->write((char*)&IsWalkable, sizeof(IsWalkable));
}

void overterrain::Load(std::ifstream* SaveFile)
{
	SaveFile->read((char*)&IsWalkable, sizeof(IsWalkable));
}
