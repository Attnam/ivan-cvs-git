#include "area.h"
#include "allocate.h"
#include "square.h"
#include "bitmap.h"
#include "charba.h"
#include "materba.h"
#include "proto.h"
#include "save.h"
#include "graphics.h"

area::area(ushort XSize, ushort YSize) : XSize(XSize), YSize(YSize), XSizeTimesYSize(XSize * YSize)
{
	Alloc2D(Map, XSize, YSize);
	Alloc2D(FlagMap, XSize, YSize);

	for(ulong c = 0; c < XSizeTimesYSize; ++c)
		FlagMap[0][c] = 0;
}

area::~area()
{
	for(ulong c = 0; c < XSizeTimesYSize; ++c)
		delete Map[0][c];

	delete [] Map;
	delete [] FlagMap;
}

void area::Save(outputfile& SaveFile) const
{
	SaveFile << XSize << YSize;

	SaveFile.GetBuffer().write((char*)FlagMap[0], sizeof(ushort) * XSizeTimesYSize);
}

void area::Load(inputfile& SaveFile)
{
	game::SetAreaInLoad(this);

	SaveFile >> XSize >> YSize;

	XSizeTimesYSize = XSize * YSize;

	Alloc2D(Map, XSize, YSize);
	Alloc2D(FlagMap, XSize, YSize);

	SaveFile.GetBuffer().read((char*)FlagMap[0], sizeof(ushort) * XSizeTimesYSize);
}

void area::RemoveCharacter(vector2d Pos)
{
	Map[Pos.X][Pos.Y]->RemoveCharacter();
}

void area::AddCharacter(vector2d Pos, character* Guy)
{
	Map[Pos.X][Pos.Y]->AddCharacter(Guy);
}

void area::UpdateLOS()
{
	EmptyFlags();

	DO_RECTANGLE(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, 0, 0, GetXSize() - 1, GetYSize() - 1, game::GetPlayer()->LOSRange(),
			{game::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, XPointer,	Top,		game::FlagHandler);
			 game::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, XPointer,	Bottom,		game::FlagHandler); },
			{game::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, Left,	YPointer,	game::FlagHandler);
			 game::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, Rigth,	YPointer,	game::FlagHandler); })
}

void area::EmptyFlags()
{
	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
			Map[x][y]->EmptyFlag();
}

void area::SendNewDrawRequest()
{
	ushort XMax = GetXSize() < game::GetCamera().X + 50 ? GetXSize() : game::GetCamera().X + 50;
	ushort YMax = GetYSize() < game::GetCamera().Y + 30 ? GetYSize() : game::GetCamera().Y + 30;

	for(ushort x = game::GetCamera().X; x < XMax; x++)
		for(ushort y = game::GetCamera().Y; y < YMax; y++)
			Map[x][y]->SendNewDrawRequest();

	DOUBLEBUFFER->ClearToColor(0, 32, 800, 480);
}

