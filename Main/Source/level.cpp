#include <cmath>
#include <ctime>

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
#include "roomde.h"
#include "team.h"
#include "config.h"
#include "femath.h"

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

	if(FlagMap[Target.X][Target.Y] & ON_POSSIBLE_ROUTE)
		for(ushort x = 0; x < XSize; ++x)
			for(ushort y = 0; y < YSize; ++y)
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

					for(ushort X = 0; X < XSize; ++X)
						for(ushort Y = 0; Y < YSize; ++Y)
							FlagMap[X][Y] &= ~STILL_ON_POSSIBLE_ROUTE;
				}

	for(ushort x = 1; x < XSize - 1; ++x)
		for(ushort y = 1; y < YSize - 1; ++y)
			FlagMap[x][y] &= ~ON_POSSIBLE_ROUTE;
}

void level::Generate(levelscript* GenLevelScript)
{
	LevelScript = GenLevelScript;

	Initialize(LevelScript->GetSize()->X, LevelScript->GetSize()->Y);

	Map = (levelsquare***)area::Map;

	if(LevelScript->GetLevelMessage(false))
		LevelMessage = *LevelScript->GetLevelMessage();

	for(ushort x = 0; x < XSize; ++x)
		for(ulong y = 0; y < YSize; ++y)
		{
			Map[x][y] = new levelsquare(this, vector2d(x, y));
			Map[x][y]->ChangeLevelTerrain(LevelScript->GetFillSquare()->GetGroundTerrain()->Instantiate(), LevelScript->GetFillSquare()->GetOverTerrain()->Instantiate());
		}

	ushort c;

	for(c = 0; c < *LevelScript->GetRooms(); ++c)
	{
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
			for(uchar i = 0; i < 10; ++i)
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

	if(*LevelScript->GetGenerateUpStairs())
		CreateStairs(true);

	if(*LevelScript->GetGenerateDownStairs())
		CreateStairs(false);

	CreateItems(*LevelScript->GetItems());

	for(c = 0; c < LevelScript->GetSquare().size(); ++c)
	{
		squarescript* Square = LevelScript->GetSquare()[c];

		uchar Times = Square->GetTimes(false) ? *Square->GetTimes() : 1;

		for(uchar c = 0; c < Times; ++c)
		{
			vector2d Pos;

			if(Square->GetPosScript()->GetRandom())
			{
				if(Square->GetPosScript()->GetIsInRoom(false))
					for(Pos = RandomSquare(*Square->GetPosScript()->GetIsWalkable());; Pos = RandomSquare(*Square->GetPosScript()->GetIsWalkable()))
					{
						if((!GetLevelSquare(Pos)->GetRoom() && !*Square->GetPosScript()->GetIsInRoom()) || (GetLevelSquare(Pos)->GetRoom() && *Square->GetPosScript()->GetIsInRoom()))
							break;
					}
				else
					Pos = RandomSquare(*Square->GetPosScript()->GetIsWalkable());
			}
			else
				Pos = *Square->GetPosScript()->GetVector();

			Map[Pos.X][Pos.Y]->ApplyScript(Square, 0);
		}
	}
}

void level::AttachPos(vector2d What)
{
	vector2d Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	while(!(FlagMap[Pos.X][Pos.Y] & PREFERRED))
		Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	FlagMap[What.X][What.Y] &= ~FORBIDDEN;
	FlagMap[What.X][What.Y] |= PREFERRED;

	GenerateTunnel(What, Pos, RAND() % 2 ? true : false);

	FlagMap[What.X][What.Y] |= FORBIDDEN;
	FlagMap[What.X][What.Y] &= ~PREFERRED;
}

void level::CreateRandomTunnel()
{
	vector2d Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	while(!(FlagMap[Pos.X][Pos.Y] & PREFERRED))
		Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	vector2d T(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	while(FlagMap[T.X][T.Y] & FORBIDDEN)
		T = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	GenerateTunnel(Pos, T, RAND() % 2 ? true : false);
}

void level::CreateItems(ushort Amount)
{
	for(uchar x = 0; x < Amount; ++x)
	{
		vector2d Pos = RandomSquare(true);

		Map[Pos.X][Pos.Y]->Stack->FastAddItem(protosystem::BalancedCreateItem());
	}
}

void level::CreateStairs(bool Up)
{
	vector2d Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	while((FlagMap[Pos.X][Pos.Y] & PREFERRED) || (FlagMap[Pos.X][Pos.Y] & FORBIDDEN))
		Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	Map[Pos.X][Pos.Y]->ChangeOverLevelTerrain(Up ? (overlevelterrain*)(new stairsup) : (overlevelterrain*)(new stairsdown));

	if(Up)
		UpStairs = Pos;
	else
		DownStairs = Pos;

	vector2d Target = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	while(!(FlagMap[Target.X][Target.Y] & PREFERRED))
		Target = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	GenerateTunnel(Pos, Target, RAND() % 2 ? true : false);

	FlagMap[Pos.X][Pos.Y] |= FORBIDDEN;
	FlagMap[Pos.X][Pos.Y] &= ~PREFERRED;
}

bool level::MakeRoom(roomscript* RoomScript)
{
	ushort XPos = RoomScript->GetPos()->X, YPos = RoomScript->GetPos()->Y, Width = RoomScript->GetSize()->X, Height = RoomScript->GetSize()->Y;

	ushort BXPos = XPos, BYPos = YPos;

	if(XPos + Width > XSize - 2)
		return false;

	if(YPos + Height > YSize - 2)
		return false;

	{
	for(ushort x = XPos - 1; x <= XPos + Width; ++x)
		for(ushort y = YPos - 1; y <= YPos + Height; ++y)
			if(FlagMap[x][y] & FORBIDDEN || FlagMap[x][y] & PREFERRED)
				return false;
	}

	room* RoomClass = protocontainer<room>::GetProto(*RoomScript->GetType())->Clone();
	RoomClass->SetPos(vector2d(XPos, YPos));
	RoomClass->SetSize(vector2d(Width, Height));

	AddRoom(RoomClass);

	RoomClass->SetDivineOwner(*RoomScript->GetDivineOwner());

	{
	for(ushort x = XPos; x < XPos + Width; ++x)
	{
		Map[x][YPos]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[x][YPos] |= FORBIDDEN;

		Map[x][YPos + Height - 1]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[x][YPos + Height - 1] |= FORBIDDEN;

		if(*RoomScript->GetGenerateLamps() && !(RAND() % 7) && x != XPos && x != XPos + Width - 1)
		{
			lamp* Lamp = new lamp;
			Lamp->SignalSquarePositionChange(true);
			Map[x][YPos]->GetSideStack(2)->FastAddItem(Lamp);
		}

		if(*RoomScript->GetGenerateLamps() && !(RAND() % 7) && x != XPos && x != XPos + Width - 1)
		{
			lamp* Lamp = new lamp;
			Lamp->SignalSquarePositionChange(true);
			Map[x][YPos + Height - 1]->GetSideStack(0)->FastAddItem(Lamp);
		}

		if(RoomScript->GetDivineOwner(false) && *RoomScript->GetDivineOwner())
		{
			Map[x][YPos]->SetDivineOwner(*RoomScript->GetDivineOwner());
			Map[x][YPos + Height - 1]->SetDivineOwner(*RoomScript->GetDivineOwner());
		}

		Map[x][YPos]->SetRoom(RoomClass->GetIndex());
		Map[x][YPos + Height - 1]->SetRoom(RoomClass->GetIndex());
	}
	}

	for(ushort y = YPos; y < YPos + Height; ++y)
	{
		Map[XPos][y]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[XPos][y] |= FORBIDDEN;
		Map[XPos + Width - 1][y]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[XPos + Width - 1][y] |= FORBIDDEN;

		if(*RoomScript->GetGenerateLamps() && !(RAND() % 7) && y != YPos && y != YPos + Height - 1)
		{
			lamp* Lamp = new lamp;
			Lamp->SignalSquarePositionChange(true);
			Map[XPos][y]->GetSideStack(1)->FastAddItem(Lamp);
		}

		if(*RoomScript->GetGenerateLamps() && !(RAND() % 7) && y != YPos && y != YPos + Height - 1)
		{
			lamp* Lamp = new lamp;
			Lamp->SignalSquarePositionChange(true);
			Map[XPos + Width - 1][y]->GetSideStack(3)->FastAddItem(Lamp);
		}

		if(RoomScript->GetDivineOwner(false) && *RoomScript->GetDivineOwner())
		{
			Map[XPos][y]->SetDivineOwner(*RoomScript->GetDivineOwner());
			Map[XPos + Width - 1][y]->SetDivineOwner(*RoomScript->GetDivineOwner());
		}

		Map[XPos][y]->SetRoom(RoomClass->GetIndex());
		Map[XPos + Width - 1][y]->SetRoom(RoomClass->GetIndex());
	}

	for(ushort x = XPos + 1; x < XPos + Width - 1; ++x)
		for(ushort y = YPos + 1; y < YPos + Height - 1; ++y)
		{
			Map[x][y]->ChangeLevelTerrain(RoomScript->GetFloorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetFloorSquare()->GetOverTerrain()->Instantiate());

			FlagMap[x][y] |= FORBIDDEN;

			if(RoomScript->GetDivineOwner(false) && *RoomScript->GetDivineOwner())
				Map[x][y]->SetDivineOwner(*RoomScript->GetDivineOwner());

			Map[x][y]->SetRoom(RoomClass->GetIndex());
		}

	if(*RoomScript->GetGenerateFountains() && !(RAND() % 10))
	{
		vector2d Pos(XPos + 1 + RAND() % (Width-2), YPos + 1 + RAND() % (Height-2));
		Map[Pos.X][Pos.Y]->ChangeOverLevelTerrain(new fountain);
	}

	if(*RoomScript->GetAltarPossible() && !(RAND() % 5))
	{
		vector2d Pos(XPos + 1 + RAND() % (Width-2), YPos + 1 + RAND() % (Height-2));
		Map[Pos.X][Pos.Y]->ChangeOverLevelTerrain(new altar);

		uchar Owner = ((altar*)Map[Pos.X][Pos.Y]->GetOverLevelTerrain())->GetOwnerGod();

		for(ushort x = XPos + 1; x < XPos + Width - 1; ++x)
			for(ushort y = YPos + 1; y < YPos + Height - 1; ++y)
				Map[x][y]->SetDivineOwner(Owner);
	}

	if(*RoomScript->GetGenerateTunnel() && Door.Length())
	{
		vector2d LPos = Door.Access(RAND() % Door.Length());

		ushort LXPos = LPos.X, LYPos = LPos.Y;

		Map[LXPos][LYPos]->ChangeLevelTerrain(RoomScript->GetDoorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOverTerrain()->Instantiate()); //Bug! Wrong room!
		Map[LXPos][LYPos]->Clean();

		FlagMap[LXPos][LYPos] &= ~FORBIDDEN;
		FlagMap[LXPos][LYPos] |= PREFERRED;

		ushort BXPos = XPos, BYPos = YPos;

		if(RAND() % 2)
		{
			XPos += RAND() % (Width - 2) + 1;

			if(RAND() % 2)
				YPos += Height - 1;
		}
		else
		{
			YPos += RAND() % (Height - 2) + 1;

			if(RAND() % 2)
				XPos += Width - 1;
		}

		FlagMap[XPos][YPos] &= ~FORBIDDEN;
		FlagMap[XPos][YPos] |= PREFERRED;

		Map[XPos][YPos]->ChangeLevelTerrain(RoomScript->GetDoorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOverTerrain()->Instantiate());
		Map[XPos][YPos]->Clean();

		GenerateTunnel(vector2d(XPos, YPos), vector2d(LXPos, LYPos), RAND() % 2 ? true : false);

		FlagMap[LXPos][LYPos] |= FORBIDDEN;
		FlagMap[LXPos][LYPos] &= ~PREFERRED;

		FlagMap[XPos][YPos] |= FORBIDDEN;
		FlagMap[XPos][YPos] &= ~PREFERRED;

		XPos = BXPos; YPos = BYPos;
	}

	if(*RoomScript->GetGenerateDoor())
	{
		if(RAND() % 2)
		{
			XPos += RAND() % (Width - 2) + 1;

			if(RAND() % 2)
				YPos += Height - 1;
		}
		else
		{
			YPos += RAND() % (Height - 2) + 1;

			if(RAND() % 2)
				XPos += Width - 1;
		}

		Door.Add(vector2d(XPos, YPos));

		if(!*RoomScript->GetGenerateTunnel())
		{
			Map[XPos][YPos]->ChangeLevelTerrain(RoomScript->GetDoorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOverTerrain()->Instantiate());
			Map[XPos][YPos]->Clean();
		}
	}

	for(ushort c = 0; c < RoomScript->GetSquare().size(); ++c)
	{
		squarescript* Square = RoomScript->GetSquare()[c];

		uchar Times = Square->GetTimes(false) ? *Square->GetTimes() : 1;

		for(uchar c = 0; c < Times; ++c)
		{
			vector2d Pos;

			if(Square->GetPosScript()->GetRandom())
				ABORT("Illegal command: Random square positioning not supported in roomscript!");
			else
				Pos = *Square->GetPosScript()->GetVector();

			Map[BXPos + Pos.X][BYPos + Pos.Y]->ApplyScript(Square, RoomClass);
		}
	}

	if(RoomScript->GetCharacterMap(false))
	{
		XPos = BXPos + RoomScript->GetCharacterMap()->GetPos()->X;
		YPos = BYPos + RoomScript->GetCharacterMap()->GetPos()->Y;

		contentscript<character>* CharacterScript;

		for(ushort x = 0; x < RoomScript->GetCharacterMap()->GetSize()->X; ++x)
			for(ushort y = 0; y < RoomScript->GetCharacterMap()->GetSize()->Y; ++y)
				if((CharacterScript = RoomScript->GetCharacterMap()->GetContentScript(x, y)))
				{
					character* Char = CharacterScript->Instantiate();

					if(!Char->GetTeam())
						Char->SetTeam(game::GetTeam(*LevelScript->GetTeamDefault()));

					Map[XPos + x][YPos + y]->FastAddCharacter(Char);
					RoomClass->HandleInstantiatedCharacter(Char);
				}
	}

	if(RoomScript->GetItemMap(false))
	{
		XPos = BXPos + RoomScript->GetItemMap()->GetPos()->X;
		YPos = BYPos + RoomScript->GetItemMap()->GetPos()->Y;

		contentscript<item>* ItemScript;

		for(ushort x = 0; x < RoomScript->GetItemMap()->GetSize()->X; ++x)
			for(ushort y = 0; y < RoomScript->GetItemMap()->GetSize()->Y; ++y)
				if((ItemScript = RoomScript->GetItemMap()->GetContentScript(x, y)))
					Map[XPos + x][YPos + y]->GetStack()->FastAddItem(ItemScript->Instantiate());
	}

	if(RoomScript->GetGroundTerrainMap(false))
	{
		XPos = BXPos + RoomScript->GetGroundTerrainMap()->GetPos()->X;
		YPos = BYPos + RoomScript->GetGroundTerrainMap()->GetPos()->Y;

		contentscript<groundlevelterrain>* GroundTerrainScript;

		for(ushort x = 0; x < RoomScript->GetGroundTerrainMap()->GetSize()->X; ++x)
			for(ushort y = 0; y < RoomScript->GetGroundTerrainMap()->GetSize()->Y; ++y)
				if((GroundTerrainScript = RoomScript->GetGroundTerrainMap()->GetContentScript(x, y)))
					Map[XPos + x][YPos + y]->ChangeGroundLevelTerrain(GroundTerrainScript->Instantiate());
	}

	if(RoomScript->GetOverTerrainMap(false))
	{
		XPos = BXPos + RoomScript->GetOverTerrainMap()->GetPos()->X;
		YPos = BYPos + RoomScript->GetOverTerrainMap()->GetPos()->Y;

		contentscript<overlevelterrain>* OverTerrainScript;

		for(ushort x = 0; x < RoomScript->GetOverTerrainMap()->GetSize()->X; ++x)
			for(ushort y = 0; y < RoomScript->GetOverTerrainMap()->GetSize()->Y; ++y)
				if((OverTerrainScript = RoomScript->GetOverTerrainMap()->GetContentScript(x, y)))
				{
					overlevelterrain* Terrain = OverTerrainScript->Instantiate();
					Map[XPos + x][YPos + y]->ChangeOverLevelTerrain(Terrain);
					RoomClass->HandleInstantiatedOverLevelTerrain(Terrain);
				}
	}

	return true;
}

void level::HandleCharacters()
{
	Population = 0;

	{
	for(ushort x = 0; x < XSize; ++x)
		for(ushort y = 0; y < YSize; ++y)
		{	
			Population += Map[x][y]->GetPopulation();

			Map[x][y]->HandleFluids();
		}
	}

	if(Population < GetIdealPopulation() && *LevelScript->GetGenerateMonsters())
		if(!(RAND() % (2 << (11 - game::GetCurrent()))))
			GenerateNewMonsters(1);
}

void level::PutPlayer(bool)
{
	vector2d Pos = RandomSquare(true);
	Map[Pos.X][Pos.Y]->FastAddCharacter(game::GetPlayer());
	game::GetPlayer()->SetSquareUnder(Map[Pos.X][Pos.Y]);
}

void level::PutPlayerAround(vector2d Pos)
{
	DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, XSize, YSize, if(Map[DoX][DoY]->GetOverLevelTerrain()->GetIsWalkable() && !Map[DoX][DoY]->GetCharacter()) {Map[DoX][DoY]->FastAddCharacter(game::GetPlayer()); game::GetPlayer()->SetSquareUnder(Map[DoX][DoY]); return; });
	ABORT("Petrus is too popular!");
}

void level::Save(outputfile& SaveFile) const
{
	area::Save(SaveFile);

	SaveFile << Room;

	{
	for(ulong c = 0; c < XSizeTimesYSize; ++c)
		Map[0][c]->Save(SaveFile);
	}

	ushort Length = KeyPoint.Length();

	SaveFile << Length;

	ushort c;

	for(c = 0; c < Length; ++c)
		SaveFile << KeyPoint.Access(c);

	Length = Door.Length();

	SaveFile << Length;

	for(c = 0; c < Length; ++c)
		SaveFile << Door.Access(c);

	SaveFile << UpStairs << DownStairs << WorldMapEntry << LevelMessage;
}

void level::Load(inputfile& SaveFile)
{
	area::Load(SaveFile);

	Map = (levelsquare***)area::Map;

	SaveFile >> Room;

	for(ushort x = 0; x < XSize; ++x)
		for(ulong y = 0; y < YSize; ++y)
		{
			Map[x][y] = new levelsquare(this, vector2d(x, y));
			Map[x][y]->Load(SaveFile);
		}

	ushort Length;

	SaveFile >> Length;

	ushort c;

	for(c = 0; c < Length; ++c)
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

	SaveFile >> UpStairs >> DownStairs >> WorldMapEntry >> LevelMessage;
}

void level::Luxify()
{
	for(ushort x = 0; x < XSize; ++x)
		for(ushort y = 0; y < YSize; ++y)
			Map[x][y]->Emitate();
}

void level::FastAddCharacter(vector2d Pos, character* Guy)
{
	Map[Pos.X][Pos.Y]->FastAddCharacter(Guy);
}

void level::GenerateNewMonsters(ushort HowMany, bool ConsiderPlayer)
{
	vector2d Pos;

	for(uchar c = 0; c < HowMany; ++c)
	{
		Pos = vector2d(0,0);

		for(uchar cc = 0; cc < 30; ++c)
		{
			Pos = RandomSquare(true);
			
			if(!ConsiderPlayer || (abs(short(Pos.X) - game::GetPlayer()->GetPos().X) > 6 && abs(short(Pos.Y) - game::GetPlayer()->GetPos().Y) > 6))
				break;
		}

		if(!(Pos.X == 0 && Pos.Y == 0))
			Map[Pos.X][Pos.Y]->AddCharacter(protosystem::BalancedCreateMonster());
	}
}

vector2d level::RandomSquare(bool Walkablility, bool HasCharacter) const
{
	vector2d Pos(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

	for(ushort c = 0; (Map[Pos.X][Pos.Y]->GetOverLevelTerrain()->GetIsWalkable() != Walkablility || (HasCharacter && !Map[Pos.X][Pos.Y]->GetCharacter()) || (!HasCharacter && Map[Pos.X][Pos.Y]->GetCharacter())) && c < 1000; ++c)
	{
		Pos.X = 1 + RAND() % (XSize - 2);
		Pos.Y = 1 + RAND() % (YSize - 2);
	}

	return Pos;
}

void level::ParticleTrail(vector2d StartPos, vector2d EndPos)
{
	if(StartPos.X != EndPos.X && StartPos.Y != EndPos.Y)
		ABORT("666th rule of thermodynamics - Particles don't move the way you want them to move.");
	// NEEDS SOME WORK!
}

bool level::GetOnGround() const
{
	return *LevelScript->GetOnGround();
}

void level::MoveCharacter(vector2d From, vector2d To)
{
	GetLevelSquare(From)->MoveCharacter(GetLevelSquare(To));
}

ushort level::GetIdealPopulation() const 
{ 
	return 10 + (game::GetCurrent() << 2); 
}

ushort level::GetLOSModifier() const
{
	return *LevelScript->GetLOSModifier();
}

ushort level::CalculateMinimumEmitationRadius(ushort Emitation) const
{
	ushort Ambient = *LevelScript->GetAmbientLight();
	return ushort(sqrt(float(Emitation << 7) / (Ambient < LIGHT_BORDER ? LIGHT_BORDER : Ambient) - 128));
}

void level::AddRoom(room* NewRoom)
{
	NewRoom->SetIndex(Room.size());
	Room.push_back(NewRoom);
}

room* level::GetRoom(uchar Index) const
{
	if(!Index)
		ABORT("Access to room zero denied!");

	return Room[Index];
}

void level::Explosion(character* Terrorist, vector2d Pos, ushort Strength)
{
	ushort EmitChange = 300 + Strength * 2;

	if(EmitChange > 511)
		EmitChange = 511;

	GetLevelSquare(Pos)->SetTemporaryEmitation(EmitChange);

	ushort ContrastLuminance = ushort(256.0f * configuration::GetContrast() / 100);

	static ushort StrengthLimit[6] = { 150, 100, 75, 50, 25, 10 };
	static vector2d StrengthPicPos[7] = { vector2d(176, 176), vector2d(0, 144), vector2d(256, 32), vector2d(144, 32), vector2d(64, 32), vector2d(16, 32),vector2d(0, 32) };

	uchar Size = 6;

	for(uchar c = 0; c < 6; ++c)
		if(Strength >= StrengthLimit[c])
		{
			Size = c;
			break;
		}

	vector2d BPos = ((Pos - game::GetCamera() + vector2d(0,2)) << 4) - vector2d(16 * (6 - Size), 16 * (6 - Size));
	vector2d SizeVect(16 + 32 * (6 - Size), 16 + 32 * (6 - Size)), OldSizeVect = SizeVect;
	vector2d PicPos = StrengthPicPos[Size];

	while(true)
	{
		if(short(BPos.X) < 0)
			if(short(BPos.X) + SizeVect.X <= 0)
				break;
			else
			{
				PicPos.X -= BPos.X;
				SizeVect.X += BPos.X;
				BPos.X = 0;
			}

		if(short(BPos.Y) < 0)
			if(short(BPos.Y) + SizeVect.Y <= 0)
				break;
			else
			{
				PicPos.Y -= BPos.Y;
				SizeVect.Y += BPos.Y;
				BPos.Y = 0;
			}

		if(BPos.X >= XRES || BPos.Y >= YRES)
			break;

		if(BPos.X + SizeVect.X > XRES)
			SizeVect.X = XRES - BPos.X;

		if(BPos.Y + SizeVect.Y > YRES)
			SizeVect.Y = YRES - BPos.Y;

		DOUBLEBUFFER->Fill(0, 0, 800, 32, 0);
		game::DrawEverythingNoBlit(false);

		uchar Flags = RAND() % 8;

		if(!Flags || SizeVect != OldSizeVect)
			igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, PicPos.X, PicPos.Y, BPos.X, BPos.Y, SizeVect.X, SizeVect.Y, ContrastLuminance);
		else
		{
			bitmap ExplosionPic(SizeVect.X, SizeVect.Y);
			igraph::GetSymbolGraphic()->Blit(&ExplosionPic, PicPos.X, PicPos.Y, 0, 0, SizeVect.X, SizeVect.Y, Flags);
			ExplosionPic.MaskedBlit(DOUBLEBUFFER, 0, 0, BPos.X, BPos.Y, SizeVect.X, SizeVect.Y, ContrastLuminance);
		}

		graphics::BlitDBToScreen();
		game::GetCurrentArea()->SendNewDrawRequest();
		clock_t StartTime = clock();
		while(clock() - StartTime < 0.3f * CLOCKS_PER_SEC);
		break;
	}

	GetLevelSquare(Pos)->SetTemporaryEmitation(0);

	ushort Radius = 8 - Size;
	ushort RadiusSquare = Radius * Radius;
	ushort PlayerDamage = 0;
	bool PlayerHurt = false;

	DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, GetXSize() - 1, GetYSize() - 1, Radius,
	{
		ushort DistanceSquare = GetHypotSquare(long(Pos.X) - XPointer, long(Pos.Y) - YPointer);

		if(DistanceSquare <= RadiusSquare)
		{
			levelsquare* Square = GetLevelSquare(vector2d(XPointer, YPointer));
			character* Char = Square->GetCharacter();
			ushort Damage = Strength / (DistanceSquare + 1);

			if(Char)
				if(Char->GetIsPlayer())
				{
					PlayerDamage = Damage;
					PlayerHurt = true;
				}
				else
				{
					Terrorist->GetTeam()->Hostility(Char->GetTeam());
					Char->GetStack()->ImpactDamage(Damage, Square->CanBeSeen());
					Char->ReceiveFireDamage(Terrorist, Damage);
					Char->CheckGearExistence();
					Char->CheckDeath("killed by an explosion");
				}

			Square->GetStack()->ImpactDamage(Damage, Square->CanBeSeen());
			Square->GetStack()->ReceiveFireDamage(Terrorist, Damage);

			if(Damage >= 20 && Square->GetOverLevelTerrain()->CanBeDigged() && Square->GetOverLevelTerrain()->GetMaterial(0)->CanBeDigged())
				Square->ChangeOverLevelTerrainAndUpdateLights(new empty);
		}
	})

	if(PlayerHurt)
	{
		Terrorist->GetTeam()->Hostility(game::GetPlayer()->GetTeam());
		game::GetPlayer()->GetStack()->ImpactDamage(PlayerDamage, true);
		game::GetPlayer()->ReceiveFireDamage(Terrorist, PlayerDamage);
		game::GetPlayer()->CheckGearExistence();
		game::GetPlayer()->CheckDeath("killed by an explosion");
	}
}
