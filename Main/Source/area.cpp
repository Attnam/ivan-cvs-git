#include "area.h"
#include "allocate.h"
#include "square.h"
#include "bitmap.h"
#include "charba.h"
#include "materba.h"
#include "proto.h"
#include "save.h"
#include "graphics.h"
#include "terra.h"
#include "error.h"
#include "femath.h"

area::area(ushort InitXSize, ushort InitYSize)
{
	Initialize(InitXSize, InitYSize);
}

void area::Initialize(ushort InitXSize, ushort InitYSize)
{
	XSize = InitXSize;
	YSize = InitYSize;

	XSizeTimesYSize = XSize * YSize;

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
	game::LOSTurn();

	ushort Radius = game::GetPlayer()->LOSRange();
	ulong RadiusSquare = Radius * Radius;

	DO_FILLED_RECTANGLE(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, 0, 0, GetXSize() - 1, GetYSize() - 1, Radius,
	{
		if(GetHypotSquare(long(game::GetPlayer()->GetPos().X) - XPointer, long(game::GetPlayer()->GetPos().Y) - YPointer) <= RadiusSquare)
			femath::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, XPointer, YPointer, RadiusSquare, game::LOSHandler);
	})

	game::RemoveLOSUpdateRequest();
}

void area::SendNewDrawRequest()
{
	ushort XMax = GetXSize() < game::GetCamera().X + 50 ? GetXSize() : game::GetCamera().X + 50;
	ushort YMax = GetYSize() < game::GetCamera().Y + 30 ? GetYSize() : game::GetCamera().Y + 30;

	for(ushort x = game::GetCamera().X; x < XMax; ++x)
		for(ushort y = game::GetCamera().Y; y < YMax; ++y)
			Map[x][y]->SendNewDrawRequest();

	DOUBLEBUFFER->Fill(0, 32, 800, 480, 0);
}

void area::MoveCharacter(vector2d From, vector2d To)
{
	character* Backup = GetSquare(From)->GetCharacter();
	GetSquare(From)->RemoveCharacter();
	GetSquare(To)->AddCharacter(Backup);
}

vector2d area::FreeSquareSeeker(vector2d StartPos, vector2d Prohibited, uchar MaxDistance)
{
	DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize(), GetYSize(),
	{
		vector2d Vector = vector2d(DoX, DoY);

		if(GetSquare(Vector)->GetOverTerrain()->GetIsWalkable() && !GetSquare(Vector)->GetCharacter() && Vector != Prohibited)
			return Vector;
	})

	if(MaxDistance)
		DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize(), GetYSize(),
		{
			vector2d Vector = vector2d(DoX, DoY);

			if(GetSquare(Vector)->GetOverTerrain()->GetIsWalkable() && Vector != Prohibited)
			{
				Vector = FreeSquareSeeker(Vector, StartPos, MaxDistance - 1);

				if(Vector.X != 0xFFFF)
					return Vector;
			}
		})

	return vector2d(0xFFFF, 0xFFFF);
}

vector2d area::GetNearestFreeSquare(vector2d StartPos)
{
	if(GetSquare(StartPos)->GetOverTerrain()->GetIsWalkable() && !GetSquare(StartPos)->GetCharacter())
		return StartPos;

	DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize(), GetYSize(),
	{
		vector2d Vector = vector2d(DoX, DoY);

		if(GetSquare(Vector)->GetOverTerrain()->GetIsWalkable() && !GetSquare(Vector)->GetCharacter())
			return Vector;
	})

	for(ushort c = 0; c < 20; ++c)
		DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize(), GetYSize(),
		{
			vector2d Vector = vector2d(DoX, DoY);

			if(GetSquare(Vector)->GetOverTerrain()->GetIsWalkable())
			{
				Vector = FreeSquareSeeker(Vector, StartPos, c);

				if(Vector.X != 0xFFFF)
					return Vector;
			}
		})

	ABORT("No room for character. Character unhappy.");

	return vector2d(0xFFFF, 0xFFFF);
}

void area::Draw() const
{
	ushort XMax = GetXSize() < game::GetCamera().X + 50 ? GetXSize() : game::GetCamera().X + 50;
	ushort YMax = GetYSize() < game::GetCamera().Y + 30 ? GetYSize() : game::GetCamera().Y + 30;

	if(!game::GetSeeWholeMapCheat())
		for(ushort x = game::GetCamera().X; x < XMax; ++x)
			for(ushort y = game::GetCamera().Y; y < YMax; ++y)
				if(Map[x][y]->GetLastSeen() == game::GetLOSTurns())
					Map[x][y]->Draw();
				else
					Map[x][y]->DrawMemorized();
	else
		for(ushort x = game::GetCamera().X; x < XMax; ++x)
			for(ushort y = game::GetCamera().Y; y < YMax; ++y)
				Map[x][y]->Draw();
}
