#include "area.h"
#include "allocate.h"
#include "lsquare.h"
#include "bitmap.h"
#include "terrain.h"

area::area(ushort XSize, ushort YSize) : XSize(XSize), YSize(YSize), XSizeTimesYSize(XSize * YSize)
{
	Map = Alloc2D<square*>(XSize, YSize);

	FlagMap = Alloc2D<ushort>(XSize, YSize);

	for(ulong c = 0; c < XSizeTimesYSize; c++)
		FlagMap[0][c] = 0;

	Memorized = new bitmap(XSize << 4, YSize << 4);
}

area::~area(void)
{
	for(ulong c = 0; c < XSizeTimesYSize; c++)
		delete Map[0][c];

	delete [] Map;
	delete [] FlagMap;

	delete Memorized;
}

void area::Save(std::ofstream* SaveFile) const
{
	SaveFile->write((char*)&XSize, sizeof(XSize));
	SaveFile->write((char*)&YSize, sizeof(YSize));

	SaveFile->write((char*)FlagMap[0], sizeof(ushort) * XSizeTimesYSize);
}

area::area(std::ifstream* SaveFile)
{
	SaveFile->read((char*)&XSize, sizeof(XSize));
	SaveFile->read((char*)&YSize, sizeof(YSize));

	XSizeTimesYSize = XSize * YSize;

	Map = Alloc2D<square*>(XSize, YSize);

	FlagMap = Alloc2D<ushort>(XSize, YSize);

	SaveFile->read((char*)FlagMap[0], sizeof(ushort) * XSizeTimesYSize);

	Memorized = new bitmap(XSize << 4, YSize << 4);
}

void area::RemoveCharacter(vector Pos)
{
	Map[Pos.X][Pos.Y]->RemoveCharacter();
}

void area::AddCharacter(vector Pos, character* Guy)
{
	Map[Pos.X][Pos.Y]->AddCharacter(Guy);
}

vector area::RandomSquare(bool Walkablility) const
{
	vector Pos(rand() % XSize, rand() % YSize);

	while(Map[Pos.X][Pos.Y]->GetOverTerrain()->GetIsWalkable() != Walkablility)
	{
		Pos.X = rand() % XSize;
		Pos.Y = rand() % YSize;
	}

	return Pos;
}
