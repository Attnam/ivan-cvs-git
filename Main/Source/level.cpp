#include <cstdlib>
#include <fstream>

#include "level.h"
#include "char.h"
#include "error.h"
#include "graphics.h"
#include "bitmap.h"
#include "game.h"
#include "item.h"
#include "material.h"
#include "igraph.h"
#include "lsquare.h"
#include "stack.h"
#include "terrain.h"
#include "whandler.h"

level::level(ushort XSize, ushort YSize, ushort Index) : area(XSize, YSize), LevelIndex(Index)
{
	Map = (levelsquare***)area::Map;

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
		{
			Map[x][y] = new levelsquare(this, vector(x, y));
			Map[x][y]->ChangeTerrain(new floory, new earth);
		}

	FluidBuffer = new bitmap(XSize << 4, YSize << 4);
}

level::~level(void)
{
	delete FluidBuffer;
}

#undef DRAW
#define DRAW/*							\
{								\
	DOUBLEBUFFER->ClearToColor(0);				\
								\
	for(ushort x = 0; x < XSize; x++)				\
		for(ushort y = 0; y < YSize; y++)			\
		{						\
			Map[x][y]->GetGroundTerrain()->Draw(DOUBLEBUFFER, vector((x - game::CCamera().X) << 4, (y - game::CCamera().Y + 1) << 4), 256);			\
			Map[x][y]->GetOverTerrain()->Draw(DOUBLEBUFFER, vector((x - game::CCamera().X) << 4, (y - game::CCamera().Y + 1) << 4), 256);			\
																					\
			if(FlagMap[x][y] & FORBIDDEN)															\
				igraph::GetFOWGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (x - game::CCamera().X) << 4, (y - game::CCamera().Y + 1) << 4, 16, 16, 0);	\
																					\
			if(FlagMap[x][y] & ON_POSSIBLE_ROUTE)														\
				igraph::GetFOWGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (x - game::CCamera().X) << 4, (y - game::CCamera().Y + 1) << 4, 16, 16, 128);	\
																					\
			if(FlagMap[x][y] & STILL_ON_POSSIBLE_ROUTE)													\
				igraph::GetFOWGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (x - game::CCamera().X) << 4, (y - game::CCamera().Y + 1) << 4, 16, 16, 256);	\
																					\
			if(FlagMap[x][y] & PREFERRED)															\
				igraph::GetFOWGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (x - game::CCamera().X) << 4, (y - game::CCamera().Y + 1) << 4, 16, 16, 511);	\
		}						\
								\
	graphics::BlitToDBToScreen()				\
								\
	getkey();						\
}*/

void level::ExpandPossibleRoute(vector Origo, vector Target, bool XMode)
{
	#define CHECK(x, y) (!(FlagMap[x][y] & ON_POSSIBLE_ROUTE) && !(FlagMap[x][y] & FORBIDDEN))

	#define CALL_EXPAND(x, y)					\
	{								\
		ExpandPossibleRoute(vector(x, y), Target, XMode);	\
									\
		if(FlagMap[Target.X][Target.Y] & ON_POSSIBLE_ROUTE)	\
			return;						\
	}

	FlagMap[Origo.X][Origo.Y] |= ON_POSSIBLE_ROUTE;

	if(XMode)
	{
		if(Target.X < Origo.X)
			if(CHECK(Origo.X - 1, Origo.Y))
				CALL_EXPAND(Origo.X - 1, Origo.Y)

		if(Target.X > Origo.X)
			if(CHECK(Origo.X + 1, Origo.Y))
				CALL_EXPAND(Origo.X + 1, Origo.Y)

		if(Target.Y < Origo.Y)
			if(CHECK(Origo.X, Origo.Y - 1))
				CALL_EXPAND(Origo.X, Origo.Y - 1)

		if(Target.Y > Origo.Y)
			if(CHECK(Origo.X, Origo.Y + 1))
				CALL_EXPAND(Origo.X, Origo.Y + 1);

		if(Target.X <= Origo.X)
			if(Origo.X < XSize - 2 && CHECK(Origo.X + 1, Origo.Y))
				CALL_EXPAND(Origo.X + 1, Origo.Y);

		if(Target.X >= Origo.X)
			if(Origo.X > 1 && CHECK(Origo.X - 1, Origo.Y))
				CALL_EXPAND(Origo.X - 1, Origo.Y);

		if(Target.Y <= Origo.Y)
			if(Origo.Y < YSize - 2 && CHECK(Origo.X, Origo.Y + 1))
				CALL_EXPAND(Origo.X, Origo.Y + 1);

		if(Target.Y >= Origo.Y)
			if(Origo.Y > 1 && CHECK(Origo.X, Origo.Y - 1))
				CALL_EXPAND(Origo.X, Origo.Y - 1);
	}
	else
	{
		if(Target.Y < Origo.Y)
			if(CHECK(Origo.X, Origo.Y - 1))
				CALL_EXPAND(Origo.X, Origo.Y - 1)

		if(Target.Y > Origo.Y)
			if(CHECK(Origo.X, Origo.Y + 1))
				CALL_EXPAND(Origo.X, Origo.Y + 1);

		if(Target.X < Origo.X)
			if(CHECK(Origo.X - 1, Origo.Y))
				CALL_EXPAND(Origo.X - 1, Origo.Y)

		if(Target.X > Origo.X)
			if(CHECK(Origo.X + 1, Origo.Y))
				CALL_EXPAND(Origo.X + 1, Origo.Y)

		if(Target.Y <= Origo.Y)
			if(Origo.Y < YSize - 2 && CHECK(Origo.X, Origo.Y + 1))
				CALL_EXPAND(Origo.X, Origo.Y + 1);

		if(Target.Y >= Origo.Y)
			if(Origo.Y > 1 && CHECK(Origo.X, Origo.Y - 1))
				CALL_EXPAND(Origo.X, Origo.Y - 1);

		if(Target.X <= Origo.X)
			if(Origo.X < XSize - 2 && CHECK(Origo.X + 1, Origo.Y))
				CALL_EXPAND(Origo.X + 1, Origo.Y);

		if(Target.X >= Origo.X)
			if(Origo.X > 1 && CHECK(Origo.X - 1, Origo.Y))
				CALL_EXPAND(Origo.X - 1, Origo.Y);
	}

	#undef CHECK

	#undef CALL_EXPAND
}

void level::ExpandStillPossibleRoute(vector Origo, vector Target, bool XMode)
{
	#define CHECK(x, y) (!(FlagMap[x][y] & STILL_ON_POSSIBLE_ROUTE) && (FlagMap[x][y] & ON_POSSIBLE_ROUTE))

	#define CALL_EXPAND(x, y)						\
	{									\
		ExpandStillPossibleRoute(vector(x, y), Target, XMode);		\
										\
		if(FlagMap[Target.X][Target.Y] & STILL_ON_POSSIBLE_ROUTE)	\
			return;							\
	}

	FlagMap[Origo.X][Origo.Y] |= STILL_ON_POSSIBLE_ROUTE;

	if(XMode)
	{
		if(Target.X < Origo.X)
			if(CHECK(Origo.X - 1, Origo.Y))
				CALL_EXPAND(Origo.X - 1, Origo.Y)

		if(Target.X > Origo.X)
			if(CHECK(Origo.X + 1, Origo.Y))
				CALL_EXPAND(Origo.X + 1, Origo.Y)

		if(Target.Y < Origo.Y)
			if(CHECK(Origo.X, Origo.Y - 1))
				CALL_EXPAND(Origo.X, Origo.Y - 1)

		if(Target.Y > Origo.Y)
			if(CHECK(Origo.X, Origo.Y + 1))
				CALL_EXPAND(Origo.X, Origo.Y + 1);

		if(Target.X <= Origo.X)
			if(Origo.X < XSize - 2 && CHECK(Origo.X + 1, Origo.Y))
				CALL_EXPAND(Origo.X + 1, Origo.Y);

		if(Target.X >= Origo.X)
			if(Origo.X > 1 && CHECK(Origo.X - 1, Origo.Y))
				CALL_EXPAND(Origo.X - 1, Origo.Y);

		if(Target.Y <= Origo.Y)
			if(Origo.Y < YSize - 2 && CHECK(Origo.X, Origo.Y + 1))
				CALL_EXPAND(Origo.X, Origo.Y + 1);

		if(Target.Y >= Origo.Y)
			if(Origo.Y > 1 && CHECK(Origo.X, Origo.Y - 1))
				CALL_EXPAND(Origo.X, Origo.Y - 1);
	}
	else
	{
		if(Target.Y < Origo.Y)
			if(CHECK(Origo.X, Origo.Y - 1))
				CALL_EXPAND(Origo.X, Origo.Y - 1)

		if(Target.Y > Origo.Y)
			if(CHECK(Origo.X, Origo.Y + 1))
				CALL_EXPAND(Origo.X, Origo.Y + 1);

		if(Target.X < Origo.X)
			if(CHECK(Origo.X - 1, Origo.Y))
				CALL_EXPAND(Origo.X - 1, Origo.Y)

		if(Target.X > Origo.X)
			if(CHECK(Origo.X + 1, Origo.Y))
				CALL_EXPAND(Origo.X + 1, Origo.Y)

		if(Target.Y <= Origo.Y)
			if(Origo.Y < YSize - 2 && CHECK(Origo.X, Origo.Y + 1))
				CALL_EXPAND(Origo.X, Origo.Y + 1);

		if(Target.Y >= Origo.Y)
			if(Origo.Y > 1 && CHECK(Origo.X, Origo.Y - 1))
				CALL_EXPAND(Origo.X, Origo.Y - 1);

		if(Target.X <= Origo.X)
			if(Origo.X < XSize - 2 && CHECK(Origo.X + 1, Origo.Y))
				CALL_EXPAND(Origo.X + 1, Origo.Y);

		if(Target.X >= Origo.X)
			if(Origo.X > 1 && CHECK(Origo.X - 1, Origo.Y))
				CALL_EXPAND(Origo.X - 1, Origo.Y);
	}

	#undef CHECK

	#undef CALL_EXPAND
}

void level::GenerateTunnel(vector From, vector Target, bool XMode)
{
	FlagMap[From.X][From.Y] |= ON_POSSIBLE_ROUTE;

	ExpandPossibleRoute(From, Target, XMode);

	DRAW

	if(!(FlagMap[Target.X][Target.Y] & ON_POSSIBLE_ROUTE))
		globalerrorhandler::Abort("Route code error during level generate! Contact Timo!");

	{
	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
			if((FlagMap[x][y] & ON_POSSIBLE_ROUTE) && !(FlagMap[x][y] & PREFERRED) && !(x == From.X && y == From.Y) && !(x == Target.X && y == Target.Y))
			{
				FlagMap[x][y] &= ~ON_POSSIBLE_ROUTE;

				FlagMap[From.X][From.Y] |= STILL_ON_POSSIBLE_ROUTE;

				ExpandStillPossibleRoute(From, Target, XMode);

				if(!(FlagMap[Target.X][Target.Y] & STILL_ON_POSSIBLE_ROUTE))
				{
					FlagMap[x][y] |= ON_POSSIBLE_ROUTE | PREFERRED;

					Map[x][y]->ChangeTerrain(new floory, new empty);
				}

				for(ushort X = 0; X < XSize; X++)
					for(ushort Y = 0; Y < YSize; Y++)
						FlagMap[X][Y] &= ~STILL_ON_POSSIBLE_ROUTE;
			}
	}

	for(ushort x = 1; x < XSize - 1; x++)
		for(ushort y = 1; y < YSize - 1; y++)
			FlagMap[x][y] &= ~ON_POSSIBLE_ROUTE;
}

void level::PutStairs(vector Pos)
{
	Map[Pos.X][Pos.Y]->ChangeTerrain(new floory, new stairsup);

	FlagMap[Pos.X][Pos.Y] |= FORBIDDEN;

	KeyPoint.Add(Pos);
}

void level::Generate(void)
{
	DRAW;

	ushort Rooms = 5 + rand() % 16;

	{
	for(ushort c = 0, i = 0; c < Rooms; c++, i++)
	{
		ushort XPos = 2 + rand() % (XSize - 6), YPos = 2 + rand() % (YSize - 6), Width = 4 + rand() % 8, Height = 4 + rand() % 8;

		if(!MakeRoom(vector(XPos, YPos), vector(Width, Height)))
		{
			if(i < 10)
				c--;
			else
				i = 0xFFFF;

			continue;
		}
	}
	}

	for(ushort c = 0; c < KeyPoint.Length(); c++)
		AttachPos(KeyPoint.Access(c));

	DRAW

	CreateItems(40);
}

void level::AttachPos(vector What)
{
	vector Pos = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while(!(FlagMap[Pos.X][Pos.Y] & PREFERRED))
		Pos = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	FlagMap[What.X][What.Y] &= ~FORBIDDEN;
	FlagMap[What.X][What.Y] |= PREFERRED;

	GenerateTunnel(What, Pos, rand() % 2 ? true : false);

	FlagMap[What.X][What.Y] |= FORBIDDEN;
	FlagMap[What.X][What.Y] &= ~PREFERRED;
}

void level::CreateRandomTunnel(void)
{
	vector Pos = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while(!(FlagMap[Pos.X][Pos.Y] & PREFERRED))
		Pos = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	vector T(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while(FlagMap[T.X][T.Y] & FORBIDDEN)
		T = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	GenerateTunnel(Pos, T, rand() % 2 ? true : false);
}

void level::CreateItems(ushort Amount)
{
	for(uchar x = 0; x < Amount; x++)
	{
		vector Pos = RandomSquare(true);

		Map[Pos.X][Pos.Y]->Stack->FastAddItem(game::BalancedCreateItem());
	}
}


void level::CreateMonsters(ushort Amount)
{
	for(uchar x = 0; x < Amount; x++)
	{
		vector Pos = RandomSquare(true);

		if(!Map[Pos.X][Pos.Y]->Character)
			Map[Pos.X][Pos.Y]->FastAddCharacter(game::BalancedCreateMonster());
	}
}

vector level::CreateDownStairs(void)
{
	vector Pos = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while((FlagMap[Pos.X][Pos.Y] & PREFERRED) || (FlagMap[Pos.X][Pos.Y] & FORBIDDEN) || (game::GetLevel(LevelIndex + 1)->GetFlag(Pos) & FORBIDDEN))
		Pos = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	game::GetLevel(LevelIndex + 1)->PutStairs(Pos);

	Map[Pos.X][Pos.Y]->ChangeTerrain(new floory, new stairsdown);

	vector Target = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while(!(FlagMap[Target.X][Target.Y] & PREFERRED))
		Target = vector(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	GenerateTunnel(Pos, Target, rand() % 2 ? true : false);

	FlagMap[Pos.X][Pos.Y] |= FORBIDDEN;
	FlagMap[Pos.X][Pos.Y] &= ~PREFERRED;

	return Pos;
}

bool level::MakeRoom(vector Pos, vector Size, bool AltarPossible, uchar DivineOwner)
{
	ushort XPos = Pos.X, YPos = Pos.Y, Width = Size.X, Height = Size.Y;

	if(XPos + Width > XSize - 2)
		Width = XSize - XPos - 2;

	if(YPos + Height > YSize - 2)
		Height = YSize - YPos - 2;

	if(Width < 3 || Height < 3)
		return false;

	{
	for(ushort x = XPos - 1; x <= XPos + Width; x++)
		for(ushort y = YPos - 1; y <= YPos + Height; y++)
			if(FlagMap[x][y] & FORBIDDEN || FlagMap[x][y] & PREFERRED)
				return false;
	}

	{
	for(ushort x = XPos; x < XPos + Width; x++)
	{
		Map[x][YPos]->ChangeTerrain(new parquet, new wall);
		FlagMap[x][YPos] |= FORBIDDEN;

		Map[x][YPos + Height - 1]->ChangeTerrain(new parquet, new wall);
		FlagMap[x][YPos + Height - 1] |= FORBIDDEN;

		if(!(rand() % 7) && x != XPos && x != XPos + Width - 1)
			Map[x][YPos]->GetSideStack(2)->FastAddItem(new lamp);

		if(!(rand() % 7) && x != XPos && x != XPos + Width - 1)
			Map[x][YPos + Height - 1]->GetSideStack(0)->FastAddItem(new lamp);
	}
	}

	for(ushort y = YPos; y < YPos + Height; y++)
	{
		Map[XPos][y]->ChangeTerrain(new parquet, new wall);
		FlagMap[XPos][y] |= FORBIDDEN;
		Map[XPos + Width - 1][y]->ChangeTerrain(new parquet, new wall);
		FlagMap[XPos + Width - 1][y] |= FORBIDDEN;

		if(!(rand() % 7) && y != YPos && y != YPos + Height - 1)
			Map[XPos][y]->GetSideStack(1)->FastAddItem(new lamp);

		if(!(rand() % 7) && y != YPos && y != YPos + Height - 1)
			Map[XPos + Width - 1][y]->GetSideStack(3)->FastAddItem(new lamp);
	}

	for(ushort x = XPos + 1; x < XPos + Width - 1; x++)
		for(ushort y = YPos + 1; y < YPos + Height - 1; y++)
		{
			Map[x][y]->ChangeTerrain(new parquet, new empty);

			FlagMap[x][y] |= FORBIDDEN;
		}

	if(AltarPossible && !(rand() % 4))
	{
		vector Pos(XPos + 1 + rand() % (Width-2), YPos + 1 + rand() % (Height-2));
		Map[Pos.X][Pos.Y]->ChangeTerrain(new parquet, new altar);

		uchar Owner = ((altar*)Map[Pos.X][Pos.Y]->GetOverTerrain())->GetOwnerGod();

		for(ushort x = XPos + 1; x < XPos + Width - 1; x++)
			for(ushort y = YPos + 1; y < YPos + Height - 1; y++)
				Map[x][y]->SetDivineOwner(Owner);
	}

	if(DivineOwner)
		for(ushort x = XPos + 1; x < XPos + Width - 1; x++)
			for(ushort y = YPos + 1; y < YPos + Height - 1; y++)
				Map[x][y]->SetDivineOwner(DivineOwner);

	if(Door.Length())
	{
		vector LPos = Door.Access(rand() % Door.Length());

		ushort LXPos = LPos.X, LYPos = LPos.Y;

		FlagMap[LXPos][LYPos] &= ~FORBIDDEN;
		FlagMap[LXPos][LYPos] |= PREFERRED;

		Map[LXPos][LYPos]->ChangeTerrain(new parquet, new door);

		Map[LXPos][LYPos]->Clean();

		ushort BXPos = XPos, BYPos = YPos;

		if(rand() % 2)
		{
			XPos += rand() % (Width - 2) + 1;

			if(rand() % 2)
				YPos += Height - 1;
		}
		else
		{
			YPos += rand() % (Height - 2) + 1;

			if(rand() % 2)
				XPos += Width - 1;
		}

		FlagMap[XPos][YPos] &= ~FORBIDDEN;
		FlagMap[XPos][YPos] |= PREFERRED;

		Map[XPos][YPos]->ChangeTerrain(new parquet, new door);

		Map[XPos][YPos]->Clean();

		GenerateTunnel(vector(XPos, YPos), vector(LXPos, LYPos), rand() % 2 ? true : false);

		FlagMap[LXPos][LYPos] |= FORBIDDEN;
		FlagMap[LXPos][LYPos] &= ~PREFERRED;

		FlagMap[XPos][YPos] |= FORBIDDEN;
		FlagMap[XPos][YPos] &= ~PREFERRED;

		XPos = BXPos; YPos = BYPos;
	}

	DRAW

	if(rand() % 2)
	{
		XPos += rand() % (Width - 2) + 1;

		if(rand() % 2)
			YPos += Height - 1;
	}
	else
	{
		YPos += rand() % (Height - 2) + 1;

		if(rand() % 2)
			XPos += Width - 1;
	}

	Door.Add(vector(XPos, YPos));

	return true;
}

void level::HandleCharacters(void)
{
	Population = 0;

	{
	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
		{
			Map[x][y]->HandleCharacters();
			
			Population += Map[x][y]->GetPopulation();

			Map[x][y]->HandleFluids();
			if(!game::GetRunning())
				return;
		}
	}

	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
			if(Map[x][y]->CCharacter())
				Map[x][y]->CCharacter()->SetHasActed(false);

	if(Population < CIdealPopulation() && LevelIndex != 9)
		GenerateNewMonsters(CIdealPopulation() - Population);
}

void level::EmptyFlags(void)
{
	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
			Map[x][y]->EmptyFlag();
}

void level::PutPlayer(bool)
{
	vector Pos = RandomSquare(true);
	Map[Pos.X][Pos.Y]->FastAddCharacter(game::GetPlayer());
}

void level::PutPlayerAround(vector Pos)
{
	DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, XSize, YSize, if(Map[DoX][DoY]->GetOverTerrain()->GetIsWalkable()) {Map[DoX][DoY]->FastAddCharacter(game::GetPlayer()); game::GetPlayer()->SetSquareUnder(Map[DoX][DoY]); return;});
}

void level::Save(std::ofstream* SaveFile) const
{
	area::Save(SaveFile);

	{
	for(ulong c = 0; c < XSizeTimesYSize; c++)
		Map[0][c]->Save(SaveFile);
	}

	ushort Length = KeyPoint.Length();

	SaveFile->write((char*)&Length, sizeof(Length));

	for(ushort c = 0; c < Length; c++)
		SaveFile->write((char*)&KeyPoint.Access(c), sizeof(KeyPoint.Access(c)));

	Length = Door.Length();

	SaveFile->write((char*)&Length, sizeof(Length));

	for(c = 0; c < Length; c++)
		SaveFile->write((char*)&Door.Access(c), sizeof(Door.Access(c)));
}

level::level(std::ifstream* SaveFile, ushort Index) : area(SaveFile), LevelIndex(Index)
{
	Map = (levelsquare***)area::Map;

	FluidBuffer = new bitmap(XSize << 4, YSize << 4);

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
			Map[x][y] = new levelsquare(this, SaveFile, vector(x, y)); //GGG

	ushort Length;

	SaveFile->read((char*)&Length, sizeof(Length));

	for(ushort c = 0; c < Length; c++)
	{
		vector Pos;

		SaveFile->read((char*)&Pos, sizeof(Pos));

		KeyPoint.Add(Pos);
	}

	SaveFile->read((char*)&Length, sizeof(Length));

	for(c = 0; c < Length; c++)
	{
		vector Pos;

		SaveFile->read((char*)&Pos, sizeof(Pos));

		Door.Add(Pos);
	}
}

void level::Luxify(void)
{
	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
			Map[x][y]->Emitate();
}

void level::FastAddCharacter(vector Pos, character* Guy)
{
	Map[Pos.X][Pos.Y]->FastAddCharacter(Guy);
}

void level::Draw(void) const
{
	ushort XMax = game::GetCurrentLevel()->GetXSize() < game::CCamera().X + 50 ? game::GetCurrentLevel()->GetXSize() : game::CCamera().X + 50;
	ushort YMax = game::GetCurrentLevel()->GetYSize() < game::CCamera().Y + 30 ? game::GetCurrentLevel()->GetYSize() : game::CCamera().Y + 30;

	if(game::GetSeeWholeMapCheat())
		for(ushort x = game::CCamera().X; x < XMax; x++)
			for(ushort y = game::CCamera().Y; y < YMax; y++)
			{
				long xDist = long(x) - game::GetPlayer()->GetPos().X, yDist = long(y) - game::GetPlayer()->GetPos().Y;

				if(game::GetCurrentLevel()->GetLevelSquare(vector(x, y))->RetrieveFlag() && (xDist * xDist + yDist * yDist) <= game::GetPlayer()->LOSRangeLevelSquare())
					game::GetCurrentLevel()->GetLevelSquare(vector(x, y))->UpdateMemorizedAndDraw();
				else
					game::GetCurrentLevel()->GetLevelSquare(vector(x, y))->DrawCheat();
			}
	else
		for(ushort x = game::CCamera().X; x < XMax; x++)
			for(ushort y = game::CCamera().Y; y < YMax; y++)
			{
				long xDist = (long(x) - game::GetPlayer()->GetPos().X), yDist = (long(y) - game::GetPlayer()->GetPos().Y);

				if(game::GetCurrentLevel()->GetLevelSquare(vector(x, y))->RetrieveFlag() && (xDist * xDist + yDist * yDist) <= game::GetPlayer()->LOSRangeLevelSquare())
					game::GetCurrentLevel()->GetLevelSquare(vector(x, y))->UpdateMemorizedAndDraw();
				else
					game::GetCurrentLevel()->GetLevelSquare(vector(x, y))->DrawMemorized();
			}
}

void level::UpdateLOS(void)
{
	game::GetCurrentLevel()->EmptyFlags();

	DO_BIG_RECTANGLE(0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1,	              {game::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, XPointer, 0,                                  game::FlagHandler);
	               game::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, XPointer, game::GetCurrentLevel()->GetYSize() - 1, game::FlagHandler);},
	              {game::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, 0,                                  YPointer, game::FlagHandler);
	               game::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, game::GetCurrentLevel()->GetXSize() - 1, YPointer, game::FlagHandler);})
}

void level::GenerateNewMonsters(ushort HowMany)
{
	vector Pos;
	for(uchar c = 0; c < HowMany; c++)
	{
                Pos = vector(0,0);
		for(uchar cc = 0; cc < 30; c++)
		{
			Pos = RandomSquare(true);
			if(abs(short(Pos.X) - game::GetPlayer()->GetPos().X) > 6 && abs(short(Pos.Y) - game::GetPlayer()->GetPos().Y) > 6 && !Map[Pos.X][Pos.Y]->Character)
				break;
		}
		if(!(Pos.X == 0 && Pos.Y == 0)) Map[Pos.X][Pos.Y]->AddCharacter(game::BalancedCreateMonster());

	}
}
