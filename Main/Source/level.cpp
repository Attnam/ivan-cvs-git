#include "level.h"
#include "charba.h"
#include "error.h"
#include "itemde.h"
#include "lsquare.h"
#include "stack.h"
#include "lterrade.h"
#include "proto.h"
#include "save.h"
#include "script.h"

/*level::level(ushort Index) : area(XSize, YSize), LevelIndex(Index)
{
	Map = (levelsquare***)area::Map;

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
		{
			Map[x][y] = new levelsquare(this, vector2d(x, y));
			Map[x][y]->ChangeLevelTerrain(new floor, new earth);
		}
}*/

/*level::level(ushort XSize, ushort YSize, ushort Index) : area(XSize, YSize), LevelIndex(Index)
{
	Map = (levelsquare***)area::Map;

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
		{
			Map[x][y] = new levelsquare(this, vector2d(x, y));
			Map[x][y]->ChangeLevelTerrain(new floor, new earth);
		}
}*/

#undef DRAW
#define DRAW/*							\
{								\
	DOUBLEBUFFER->ClearToColor(0);				\
								\
	for(ushort x = 0; x < XSize; x++)				\
		for(ushort y = 0; y < YSize; y++)			\
		{						\
			Map[x][y]->GetGroundLevelTerrain()->Draw(DOUBLEBUFFER, vector2d((x - game::GetCamera().X) << 4, (y - game::GetCamera().Y + 1) << 4), 256);			\
			Map[x][y]->GetOverLevelTerrain()->Draw(DOUBLEBUFFER, vector2d((x - game::GetCamera().X) << 4, (y - game::GetCamera().Y + 1) << 4), 256);			\
																					\
			if(FlagMap[x][y] & FORBIDDEN)															\
				igraph::GetFOWGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (x - game::GetCamera().X) << 4, (y - game::GetCamera().Y + 1) << 4, 16, 16, 0);	\
																					\
			if(FlagMap[x][y] & ON_POSSIBLE_ROUTE)														\
				igraph::GetFOWGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (x - game::GetCamera().X) << 4, (y - game::GetCamera().Y + 1) << 4, 16, 16, 128);	\
																					\
			if(FlagMap[x][y] & STILL_ON_POSSIBLE_ROUTE)													\
				igraph::GetFOWGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (x - game::GetCamera().X) << 4, (y - game::GetCamera().Y + 1) << 4, 16, 16, 256);	\
																					\
			if(FlagMap[x][y] & PREFERRED)															\
				igraph::GetFOWGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (x - game::GetCamera().X) << 4, (y - game::GetCamera().Y + 1) << 4, 16, 16, 511);	\
		}						\
								\
	graphics::BlitToDBToScreen()				\
								\
	getkey();						\
}*/

void level::ExpandPossibleRoute(vector2d Origo, vector2d Target, bool XMode)
{
	#define CHECK(x, y) (!(FlagMap[x][y] & ON_POSSIBLE_ROUTE) && !(FlagMap[x][y] & FORBIDDEN))

	#define CALL_EXPAND(x, y)					\
	{								\
		ExpandPossibleRoute(vector2d(x, y), Target, XMode);	\
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

void level::ExpandStillPossibleRoute(vector2d Origo, vector2d Target, bool XMode)
{
	#define CHECK(x, y) (!(FlagMap[x][y] & STILL_ON_POSSIBLE_ROUTE) && (FlagMap[x][y] & ON_POSSIBLE_ROUTE))

	#define CALL_EXPAND(x, y)						\
	{									\
		ExpandStillPossibleRoute(vector2d(x, y), Target, XMode);		\
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

void level::GenerateTunnel(vector2d From, vector2d Target, bool XMode)
{
	FlagMap[From.X][From.Y] |= ON_POSSIBLE_ROUTE;

	ExpandPossibleRoute(From, Target, XMode);

	DRAW

	if(!(FlagMap[Target.X][Target.Y] & ON_POSSIBLE_ROUTE))
		ABORT("Route code error during level generate! Contact Timo!");

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

					Map[x][y]->ChangeOverLevelTerrain(new empty);
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

/*void level::PutStairs(vector2d Pos)
{
	Map[Pos.X][Pos.Y]->ChangeLevelTerrain(new floor, new stairsup);

	UpStairs = Pos;

	FlagMap[Pos.X][Pos.Y] |= FORBIDDEN;

	KeyPoint.Add(Pos);
}*/

void level::Generate(levelscript* GenLevelScript)
{
	LevelScript = GenLevelScript;

	Initialize(LevelScript->GetSize()->X, LevelScript->GetSize()->Y);

	Map = (levelsquare***)area::Map;

	if(LevelScript->GetLevelMessage(false))
		LevelMessage = *LevelScript->GetLevelMessage();

	/*const groundlevelterrain* const FillGroundTerrainProto = protocontainer<groundlevelterrain>::GetProto(LevelScript->GetFillSquare()->GetGroundTerrain()->GetTerrainType());
	const material* const FillGroundMaterialProto = protocontainer<material>::GetProto(LevelScript->GetFillSquare()->GetGroundTerrain()->GetMaterialType(false));
	const overlevelterrain* const FillOverTerrainProto = protocontainer<overlevelterrain>::GetProto(LevelScript->GetFillSquare()->GetOverTerrain()->GetTerrainType());
	const material* const FillOverMaterialProto = protocontainer<material>::GetProto(LevelScript->GetFillSquare()->GetOverTerrain()->GetMaterialType(false));*/

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
		{
			Map[x][y] = new levelsquare(this, vector2d(x, y));

			/*groundlevelterrain* GTerrain;

			if(FillGroundMaterialProto)
			{
				GTerrain = FillGroundTerrainProto->Clone(false);
				GTerrain->InitMaterials(FillGroundMaterialProto->Clone());
			}
			else
				GTerrain = FillGroundTerrainProto->Clone();

			overlevelterrain* OTerrain;

			if(FillOverMaterialProto)
			{
				OTerrain = FillOverTerrainProto->Clone(false);
				OTerrain->InitMaterials(FillOverMaterialProto->Clone());
			}
			else
				OTerrain = FillOverTerrainProto->Clone();*/

			Map[x][y]->ChangeLevelTerrain(LevelScript->GetFillSquare()->GetGroundTerrain()->Instantiate(), LevelScript->GetFillSquare()->GetOverTerrain()->Instantiate());
		}

	for(ushort c = 0; c < *LevelScript->GetRooms(); ++c)
	{
		//ushort XPos = 2 + rand() % (XSize - 6), YPos = 2 + rand() % (YSize - 6);//, Width = 4 + rand() % 8, Height = 4 + rand() % 8;

		std::map<uchar, roomscript*>::iterator RoomIterator = LevelScript->GetRoom().find(c);

		roomscript* RoomScript;

		if(RoomIterator != LevelScript->GetRoom().end())
		{
			while(true)
			{
				RoomScript = RoomIterator->second;

				if(*RoomScript->GetReCalculate())
				{
					inputfile ScriptFile("Script/Dungeon.dat");
					RoomScript->ReadFrom(ScriptFile, true);
				}

				if(MakeRoom(RoomScript))
					break;
			}
		}
		else
		{
			for(uchar i = 0; i < 10; i++)
			{
				RoomScript = LevelScript->GetRoomDefault();

				if(*RoomScript->GetReCalculate())
				{
					inputfile ScriptFile("Script/Dungeon.dat");
					RoomScript->ReadFrom(ScriptFile, true);
				}

				if(MakeRoom(RoomScript))
					break;
			}
		}
	}

	//for(c = 0; c < KeyPoint.Length(); ++c)
	//	AttachPos(KeyPoint.Access(c));

	if(*LevelScript->GetGenerateUpStairs())
		CreateStairs(true);

	if(*LevelScript->GetGenerateDownStairs())
		CreateStairs(false);

	CreateItems(*LevelScript->GetItems());

	for(c = 0; c < LevelScript->GetSquare().size(); ++c)
	{
		squarescript* Square = LevelScript->GetSquare()[c];

		vector2d Pos;

		if(Square->GetPosScript()->GetRandom())
			Pos = RandomSquare(*Square->GetPosScript()->GetIsWalkable());
		else
			Pos = *Square->GetPosScript()->GetVector();

		Map[Pos.X][Pos.Y]->ApplyScript(Square);
	}

	/*DRAW;

	ushort Rooms = 5 + rand() % 16;

	{
	for(ushort c = 0, i = 0; c < Rooms; ++c, ++i)
	{
		ushort XPos = 2 + rand() % (XSize - 6), YPos = 2 + rand() % (YSize - 6), Width = 4 + rand() % 8, Height = 4 + rand() % 8;

		if(!MakeRoom(vector2d(XPos, YPos), vector2d(Width, Height), LevelIndex ? true : false))
		{
			if(i < 10)
				c--;
			else
				i = 0xFFFF;

			continue;
		}
	}
	}

	for(ushort c = 0; c < KeyPoint.Length(); ++c)
		AttachPos(KeyPoint.Access(c));

	DRAW

	CreateItems(40);*/
}

void level::AttachPos(vector2d What)
{
	vector2d Pos = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while(!(FlagMap[Pos.X][Pos.Y] & PREFERRED))
		Pos = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	FlagMap[What.X][What.Y] &= ~FORBIDDEN;
	FlagMap[What.X][What.Y] |= PREFERRED;

	GenerateTunnel(What, Pos, rand() % 2 ? true : false);

	FlagMap[What.X][What.Y] |= FORBIDDEN;
	FlagMap[What.X][What.Y] &= ~PREFERRED;
}

void level::CreateRandomTunnel()
{
	vector2d Pos = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while(!(FlagMap[Pos.X][Pos.Y] & PREFERRED))
		Pos = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	vector2d T(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while(FlagMap[T.X][T.Y] & FORBIDDEN)
		T = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	GenerateTunnel(Pos, T, rand() % 2 ? true : false);
}

void level::CreateItems(ushort Amount)
{
	for(uchar x = 0; x < Amount; x++)
	{
		vector2d Pos = RandomSquare(true);

		Map[Pos.X][Pos.Y]->Stack->FastAddItem(protosystem::BalancedCreateItem());
	}
}


void level::CreateMonsters(ushort Amount)
{
	for(uchar x = 0; x < Amount; x++)
	{
		vector2d Pos = RandomSquare(true);

		if(!Map[Pos.X][Pos.Y]->Character)
			Map[Pos.X][Pos.Y]->FastAddCharacter(protosystem::BalancedCreateMonster());
	}
}

void level::CreateStairs(bool Up)
{
	vector2d Pos = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while((FlagMap[Pos.X][Pos.Y] & PREFERRED) || (FlagMap[Pos.X][Pos.Y] & FORBIDDEN))
		Pos = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	//game::GetLevel(LevelIndex + 1)->PutStairs(Pos);

	Map[Pos.X][Pos.Y]->ChangeOverLevelTerrain(Up ? (overlevelterrain*)(new stairsup) : (overlevelterrain*)(new stairsdown));

	if(Up)
		UpStairs = Pos;
	else
		DownStairs = Pos;

	vector2d Target = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while(!(FlagMap[Target.X][Target.Y] & PREFERRED))
		Target = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	GenerateTunnel(Pos, Target, rand() % 2 ? true : false);

	FlagMap[Pos.X][Pos.Y] |= FORBIDDEN;
	FlagMap[Pos.X][Pos.Y] &= ~PREFERRED;
}

bool level::MakeRoom(roomscript* RoomScript)
{
	ushort XPos = RoomScript->GetPos()->X, YPos = RoomScript->GetPos()->Y, Width = RoomScript->GetSize()->X, Height = RoomScript->GetSize()->Y;

	ushort BXPos = XPos, BYPos = YPos;

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

	/*const groundlevelterrain* const WallGroundTerrainProto = protocontainer<groundlevelterrain>::GetProto(RoomScript->GetWallSquare()->GetGroundTerrain()->GetTerrainType());
	const overlevelterrain* const WallOverTerrainProto = protocontainer<overlevelterrain>::GetProto(RoomScript->GetWallSquare()->GetOverTerrain()->GetTerrainType());

	const groundlevelterrain* const FloorGroundTerrainProto = protocontainer<groundlevelterrain>::GetProto(RoomScript->GetFloorSquare()->GetGroundTerrain()->GetTerrainType());
	const overlevelterrain* const FloorOverTerrainProto = protocontainer<overlevelterrain>::GetProto(RoomScript->GetFloorSquare()->GetOverTerrain()->GetTerrainType());

	const groundlevelterrain* const DoorGroundTerrainProto = protocontainer<groundlevelterrain>::GetProto(RoomScript->GetDoorSquare()->GetGroundTerrain()->GetTerrainType());
	const overlevelterrain* const DoorOverTerrainProto = protocontainer<overlevelterrain>::GetProto(RoomScript->GetDoorSquare()->GetOverTerrain()->GetTerrainType());*/

	{
	for(ushort x = XPos; x < XPos + Width; x++)
	{
		Map[x][YPos]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[x][YPos] |= FORBIDDEN;

		Map[x][YPos + Height - 1]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[x][YPos + Height - 1] |= FORBIDDEN;

		if(!(rand() % 7) && x != XPos && x != XPos + Width - 1)
			Map[x][YPos]->GetSideStack(2)->FastAddItem(new lamp);

		if(!(rand() % 7) && x != XPos && x != XPos + Width - 1)
			Map[x][YPos + Height - 1]->GetSideStack(0)->FastAddItem(new lamp);
	}
	}

	for(ushort y = YPos; y < YPos + Height; y++)
	{
		Map[XPos][y]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[XPos][y] |= FORBIDDEN;
		Map[XPos + Width - 1][y]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[XPos + Width - 1][y] |= FORBIDDEN;

		if(!(rand() % 7) && y != YPos && y != YPos + Height - 1)
			Map[XPos][y]->GetSideStack(1)->FastAddItem(new lamp);

		if(!(rand() % 7) && y != YPos && y != YPos + Height - 1)
			Map[XPos + Width - 1][y]->GetSideStack(3)->FastAddItem(new lamp);
	}

	for(ushort x = XPos + 1; x < XPos + Width - 1; x++)
		for(ushort y = YPos + 1; y < YPos + Height - 1; y++)
		{
			Map[x][y]->ChangeLevelTerrain(RoomScript->GetFloorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetFloorSquare()->GetOverTerrain()->Instantiate());

			FlagMap[x][y] |= FORBIDDEN;
		}

	if(*RoomScript->GetAltarPossible() && !(rand() % 4))
	{
		vector2d Pos(XPos + 1 + rand() % (Width-2), YPos + 1 + rand() % (Height-2));
		Map[Pos.X][Pos.Y]->ChangeOverLevelTerrain(new altar);

		uchar Owner = ((altar*)Map[Pos.X][Pos.Y]->GetOverLevelTerrain())->GetOwnerGod();

		for(ushort x = XPos + 1; x < XPos + Width - 1; x++)
			for(ushort y = YPos + 1; y < YPos + Height - 1; y++)
				Map[x][y]->SetDivineOwner(Owner);
	}

	/*if(DivineOwner)
		for(ushort x = XPos + 1; x < XPos + Width - 1; x++)
			for(ushort y = YPos + 1; y < YPos + Height - 1; y++)
				Map[x][y]->SetDivineOwner(DivineOwner);*/

	if(Door.Length())
	{
		vector2d LPos = Door.Access(rand() % Door.Length());

		ushort LXPos = LPos.X, LYPos = LPos.Y;

		FlagMap[LXPos][LYPos] &= ~FORBIDDEN;
		FlagMap[LXPos][LYPos] |= PREFERRED;

		Map[LXPos][LYPos]->ChangeLevelTerrain(RoomScript->GetDoorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOverTerrain()->Instantiate()); //BUG! Wrong room!
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

		Map[XPos][YPos]->ChangeLevelTerrain(RoomScript->GetDoorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOverTerrain()->Instantiate());
		Map[XPos][YPos]->Clean();

		GenerateTunnel(vector2d(XPos, YPos), vector2d(LXPos, LYPos), rand() % 2 ? true : false);

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

	Door.Add(vector2d(XPos, YPos));

	for(ushort c = 0; c < RoomScript->GetSquare().size(); ++c)
	{
		squarescript* Square = RoomScript->GetSquare()[c];

		vector2d Pos;

		if(Square->GetPosScript()->GetRandom())
			//Pos = RandomSquare(*Square->GetPosScript()->GetIsWalkable());
			ABORT("Illegal command: Random square positioning not supported in roomscript!");
		else
			Pos = *Square->GetPosScript()->GetVector();

		Map[BXPos + Pos.X][BYPos + Pos.Y]->ApplyScript(Square);
	}

	return true;
}

/*bool level::MakeRoom(vector2d Pos, vector2d Size, bool AltarPossible, uchar DivineOwner)
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
		Map[x][YPos]->ChangeLevelTerrain(new parquet, new wall);
		FlagMap[x][YPos] |= FORBIDDEN;

		Map[x][YPos + Height - 1]->ChangeLevelTerrain(new parquet, new wall);
		FlagMap[x][YPos + Height - 1] |= FORBIDDEN;

		if(!(rand() % 7) && x != XPos && x != XPos + Width - 1)
			Map[x][YPos]->GetSideStack(2)->FastAddItem(new lamp);

		if(!(rand() % 7) && x != XPos && x != XPos + Width - 1)
			Map[x][YPos + Height - 1]->GetSideStack(0)->FastAddItem(new lamp);
	}
	}

	for(ushort y = YPos; y < YPos + Height; y++)
	{
		Map[XPos][y]->ChangeLevelTerrain(new parquet, new wall);
		FlagMap[XPos][y] |= FORBIDDEN;
		Map[XPos + Width - 1][y]->ChangeLevelTerrain(new parquet, new wall);
		FlagMap[XPos + Width - 1][y] |= FORBIDDEN;

		if(!(rand() % 7) && y != YPos && y != YPos + Height - 1)
			Map[XPos][y]->GetSideStack(1)->FastAddItem(new lamp);

		if(!(rand() % 7) && y != YPos && y != YPos + Height - 1)
			Map[XPos + Width - 1][y]->GetSideStack(3)->FastAddItem(new lamp);
	}

	for(ushort x = XPos + 1; x < XPos + Width - 1; x++)
		for(ushort y = YPos + 1; y < YPos + Height - 1; y++)
		{
			Map[x][y]->ChangeLevelTerrain(new parquet, new empty);

			FlagMap[x][y] |= FORBIDDEN;
		}

	if(AltarPossible && !(rand() % 4))
	{
		vector2d Pos(XPos + 1 + rand() % (Width-2), YPos + 1 + rand() % (Height-2));
		Map[Pos.X][Pos.Y]->ChangeOverLevelTerrain(new altar);

		uchar Owner = ((altar*)Map[Pos.X][Pos.Y]->GetOverLevelTerrain())->GetOwnerGod();

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
		vector2d LPos = Door.Access(rand() % Door.Length());

		ushort LXPos = LPos.X, LYPos = LPos.Y;

		FlagMap[LXPos][LYPos] &= ~FORBIDDEN;
		FlagMap[LXPos][LYPos] |= PREFERRED;

		Map[LXPos][LYPos]->ChangeOverLevelTerrain(new door);

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

		Map[XPos][YPos]->ChangeOverLevelTerrain(new door);

		Map[XPos][YPos]->Clean();

		GenerateTunnel(vector2d(XPos, YPos), vector2d(LXPos, LYPos), rand() % 2 ? true : false);

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

	Door.Add(vector2d(XPos, YPos));

	return true;
}*/

void level::HandleCharacters()
{
	Population = 0;

	{
	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
		{	
			Population += Map[x][y]->GetPopulation();

			Map[x][y]->HandleFluids();
		}
	}

	if(Population < GetIdealPopulation() && *LevelScript->GetGenerateMonsters())// && LevelIndex != 9)
		GenerateNewMonsters(GetIdealPopulation() - Population);
}

void level::PutPlayer(bool)
{
	vector2d Pos = RandomSquare(true);
	Map[Pos.X][Pos.Y]->FastAddCharacter(game::GetPlayer());
	game::GetPlayer()->SetSquareUnder(Map[Pos.X][Pos.Y]);
}

void level::PutPlayerAround(vector2d Pos)
{
	DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, XSize, YSize, if(Map[DoX][DoY]->GetOverLevelTerrain()->GetIsWalkable()) {Map[DoX][DoY]->FastAddCharacter(game::GetPlayer()); game::GetPlayer()->SetSquareUnder(Map[DoX][DoY]); return; });
}

void level::Save(outputfile& SaveFile) const
{
	area::Save(SaveFile);

	{
	for(ulong c = 0; c < XSizeTimesYSize; ++c)
		Map[0][c]->Save(SaveFile);
	}

	ushort Length = KeyPoint.Length();

	SaveFile << Length;

	for(ushort c = 0; c < Length; ++c)
		SaveFile << KeyPoint.Access(c);

	Length = Door.Length();

	SaveFile << Length;

	for(c = 0; c < Length; ++c)
		SaveFile << Door.Access(c);

	SaveFile << UpStairs << DownStairs << LevelMessage;
}

void level::Load(inputfile& SaveFile)
{
	area::Load(SaveFile);

	Map = (levelsquare***)area::Map;

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
		{
			Map[x][y] = new levelsquare(this, vector2d(x, y)); //GGG
			Map[x][y]->Load(SaveFile);
		}

	ushort Length;

	SaveFile >> Length;

	for(ushort c = 0; c < Length; ++c)
	{
		vector2d Pos;

		SaveFile >> Pos;

		KeyPoint.Add(Pos);
	}

	SaveFile >> Length;

	for(c = 0; c < Length; ++c)
	{
		vector2d Pos;

		SaveFile >> Pos;

		Door.Add(Pos);
	}

	SaveFile >> UpStairs >> DownStairs >> LevelMessage;
}

void level::Luxify()
{
	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
			Map[x][y]->Emitate();
}

void level::FastAddCharacter(vector2d Pos, character* Guy)
{
	Map[Pos.X][Pos.Y]->FastAddCharacter(Guy);
}

void level::Draw() const
{
	ushort XMax = GetXSize() < game::GetCamera().X + 50 ? GetXSize() : game::GetCamera().X + 50;
	ushort YMax = GetYSize() < game::GetCamera().Y + 30 ? GetYSize() : game::GetCamera().Y + 30;

	if(game::GetSeeWholeMapCheat())
		for(ushort x = game::GetCamera().X; x < XMax; x++)
			for(ushort y = game::GetCamera().Y; y < YMax; y++)
			{
				long xDist = long(x) - game::GetPlayer()->GetPos().X, yDist = long(y) - game::GetPlayer()->GetPos().Y;

				if(Map[x][y]->RetrieveFlag() && (xDist * xDist + yDist * yDist) <= game::GetPlayer()->LOSRangeLevelSquare())
					Map[x][y]->UpdateMemorizedAndDraw();
				else
					Map[x][y]->DrawCheat();
			}
	else
		for(ushort x = game::GetCamera().X; x < XMax; x++)
			for(ushort y = game::GetCamera().Y; y < YMax; y++)
			{
				long xDist = (long(x) - game::GetPlayer()->GetPos().X), yDist = (long(y) - game::GetPlayer()->GetPos().Y);

				if(Map[x][y]->RetrieveFlag() && (xDist * xDist + yDist * yDist) <= game::GetPlayer()->LOSRangeLevelSquare())
					Map[x][y]->UpdateMemorizedAndDraw();
				else
					Map[x][y]->DrawMemorized();
			}
}

void level::GenerateNewMonsters(ushort HowMany)
{
	vector2d Pos;

	for(uchar c = 0; c < HowMany; ++c)
	{
                Pos = vector2d(0,0);

		for(uchar cc = 0; cc < 30; ++c)
		{
			Pos = RandomSquare(true);

			if(abs(short(Pos.X) - game::GetPlayer()->GetPos().X) > 6 && abs(short(Pos.Y) - game::GetPlayer()->GetPos().Y) > 6 && !Map[Pos.X][Pos.Y]->Character)
				break;
		}

		if(!(Pos.X == 0 && Pos.Y == 0)) Map[Pos.X][Pos.Y]->AddCharacter(protosystem::BalancedCreateMonster());
	}
}

vector2d level::RandomSquare(bool Walkablility) const
{
	vector2d Pos(rand() % XSize, rand() % YSize);

	while(Map[Pos.X][Pos.Y]->GetOverLevelTerrain()->GetIsWalkable() != Walkablility)
	{
		Pos.X = rand() % XSize;
		Pos.Y = rand() % YSize;
	}

	return Pos;
}

void level::ParticleTrail(vector2d StartPos, vector2d EndPos)
{
	if(StartPos.X != EndPos.X && StartPos.Y != EndPos.Y)
		ABORT("666th rule of thermodynamics - Particles don't move the way you want them to move.");
	// NEEDS SOME WORK!
}
