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

	if(!(FlagMap[Target.X][Target.Y] & ON_POSSIBLE_ROUTE))
		ABORT("Route code error during level generate! Contact Timo!");

	{
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

	for(ushort c = 0; c < *LevelScript->GetRooms(); ++c)
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

		vector2d Pos;

		if(Square->GetPosScript()->GetRandom())
			Pos = RandomSquare(*Square->GetPosScript()->GetIsWalkable());
		else
			Pos = *Square->GetPosScript()->GetVector();

		Map[Pos.X][Pos.Y]->ApplyScript(Square);
	}
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
	for(uchar x = 0; x < Amount; ++x)
	{
		vector2d Pos = RandomSquare(true);

		Map[Pos.X][Pos.Y]->Stack->FastAddItem(protosystem::BalancedCreateItem());
	}
}

void level::CreateStairs(bool Up)
{
	vector2d Pos = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

	while((FlagMap[Pos.X][Pos.Y] & PREFERRED) || (FlagMap[Pos.X][Pos.Y] & FORBIDDEN))
		Pos = vector2d(1 + rand() % (XSize - 2), 1 + rand() % (YSize - 2));

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
		return false;

	if(YPos + Height > YSize - 2)
		return false;

	{
	for(ushort x = XPos - 1; x <= XPos + Width; ++x)
		for(ushort y = YPos - 1; y <= YPos + Height; ++y)
			if(FlagMap[x][y] & FORBIDDEN || FlagMap[x][y] & PREFERRED)
				return false;
	}

	{
	for(ushort x = XPos; x < XPos + Width; ++x)
	{
		Map[x][YPos]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[x][YPos] |= FORBIDDEN;

		Map[x][YPos + Height - 1]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[x][YPos + Height - 1] |= FORBIDDEN;

		if(*RoomScript->GetGenerateLamps() && !(rand() % 7) && x != XPos && x != XPos + Width - 1)
			Map[x][YPos]->GetSideStack(2)->FastAddItem(new lamp);

		if(*RoomScript->GetGenerateLamps() && !(rand() % 7) && x != XPos && x != XPos + Width - 1)
			Map[x][YPos + Height - 1]->GetSideStack(0)->FastAddItem(new lamp);

		if(RoomScript->GetDivineOwner(false) && *RoomScript->GetDivineOwner())
		{
			Map[x][YPos]->SetDivineOwner(*RoomScript->GetDivineOwner());
			Map[x][YPos + Height - 1]->SetDivineOwner(*RoomScript->GetDivineOwner());
		}
	}
	}

	for(ushort y = YPos; y < YPos + Height; ++y)
	{
		Map[XPos][y]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[XPos][y] |= FORBIDDEN;
		Map[XPos + Width - 1][y]->ChangeLevelTerrain(RoomScript->GetWallSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOverTerrain()->Instantiate());
		FlagMap[XPos + Width - 1][y] |= FORBIDDEN;

		if(*RoomScript->GetGenerateLamps() && !(rand() % 7) && y != YPos && y != YPos + Height - 1)
			Map[XPos][y]->GetSideStack(1)->FastAddItem(new lamp);

		if(*RoomScript->GetGenerateLamps() && !(rand() % 7) && y != YPos && y != YPos + Height - 1)
			Map[XPos + Width - 1][y]->GetSideStack(3)->FastAddItem(new lamp);

		if(RoomScript->GetDivineOwner(false) && *RoomScript->GetDivineOwner())
		{
			Map[XPos][y]->SetDivineOwner(*RoomScript->GetDivineOwner());
			Map[XPos + Width - 1][y]->SetDivineOwner(*RoomScript->GetDivineOwner());
		}
	}

	for(ushort x = XPos + 1; x < XPos + Width - 1; ++x)
		for(ushort y = YPos + 1; y < YPos + Height - 1; ++y)
		{
			Map[x][y]->ChangeLevelTerrain(RoomScript->GetFloorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetFloorSquare()->GetOverTerrain()->Instantiate());

			FlagMap[x][y] |= FORBIDDEN;

			if(RoomScript->GetDivineOwner(false) && *RoomScript->GetDivineOwner())
				Map[x][y]->SetDivineOwner(*RoomScript->GetDivineOwner());
		}

	if(*RoomScript->GetAltarPossible() && !(rand() % 4))
	{
		vector2d Pos(XPos + 1 + rand() % (Width-2), YPos + 1 + rand() % (Height-2));
		Map[Pos.X][Pos.Y]->ChangeOverLevelTerrain(new altar);

		uchar Owner = ((altar*)Map[Pos.X][Pos.Y]->GetOverLevelTerrain())->GetOwnerGod();

		for(ushort x = XPos + 1; x < XPos + Width - 1; ++x)
			for(ushort y = YPos + 1; y < YPos + Height - 1; ++y)
				Map[x][y]->SetDivineOwner(Owner);
	}

	if(*RoomScript->GetGenerateTunnel() && Door.Length())
	{
		vector2d LPos = Door.Access(rand() % Door.Length());

		ushort LXPos = LPos.X, LYPos = LPos.Y;

		Map[LXPos][LYPos]->ChangeLevelTerrain(RoomScript->GetDoorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOverTerrain()->Instantiate()); //Bug! Wrong room!
		Map[LXPos][LYPos]->Clean();

		FlagMap[LXPos][LYPos] &= ~FORBIDDEN;
		FlagMap[LXPos][LYPos] |= PREFERRED;

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

	if(*RoomScript->GetGenerateDoor())
	{
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

		if(!*RoomScript->GetGenerateTunnel())
		{
			Map[XPos][YPos]->ChangeLevelTerrain(RoomScript->GetDoorSquare()->GetGroundTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOverTerrain()->Instantiate());
			Map[XPos][YPos]->Clean();
		}
	}

	for(ushort c = 0; c < RoomScript->GetSquare().size(); ++c)
	{
		squarescript* Square = RoomScript->GetSquare()[c];

		vector2d Pos;

		if(Square->GetPosScript()->GetRandom())
			ABORT("Illegal command: Random square positioning not supported in roomscript!");
		else
			Pos = *Square->GetPosScript()->GetVector();

		Map[BXPos + Pos.X][BYPos + Pos.Y]->ApplyScript(Square);
	}

	if(RoomScript->GetCharacterMap(false))
	{
		XPos = BXPos + RoomScript->GetCharacterMap()->GetPos()->X;
		YPos = BYPos + RoomScript->GetCharacterMap()->GetPos()->Y;

		contentscript<character>* CharacterScript;

		for(ushort x = 0; x < RoomScript->GetCharacterMap()->GetSize()->X; ++x)
			for(ushort y = 0; y < RoomScript->GetCharacterMap()->GetSize()->Y; ++y)
				if(CharacterScript = RoomScript->GetCharacterMap()->GetContentScript(x, y))
				{
					character* Char = CharacterScript->Instantiate();

					if(!Char->GetTeam())
						Char->SetTeam(game::GetTeam(*LevelScript->GetTeamDefault()));

					Map[XPos + x][YPos + y]->FastAddCharacter(Char);
				}
	}

	if(RoomScript->GetItemMap(false))
	{
		XPos = BXPos + RoomScript->GetItemMap()->GetPos()->X;
		YPos = BYPos + RoomScript->GetItemMap()->GetPos()->Y;

		contentscript<item>* ItemScript;

		for(ushort x = 0; x < RoomScript->GetItemMap()->GetSize()->X; ++x)
			for(ushort y = 0; y < RoomScript->GetItemMap()->GetSize()->Y; ++y)
				if(ItemScript = RoomScript->GetItemMap()->GetContentScript(x, y))
					Map[XPos + x][YPos + y]->GetStack()->FastAddItem(ItemScript->Instantiate());
	}

	if(RoomScript->GetGroundTerrainMap(false))
	{
		XPos = BXPos + RoomScript->GetGroundTerrainMap()->GetPos()->X;
		YPos = BYPos + RoomScript->GetGroundTerrainMap()->GetPos()->Y;

		contentscript<groundlevelterrain>* GroundTerrainScript;

		for(ushort x = 0; x < RoomScript->GetGroundTerrainMap()->GetSize()->X; ++x)
			for(ushort y = 0; y < RoomScript->GetGroundTerrainMap()->GetSize()->Y; ++y)
				if(GroundTerrainScript = RoomScript->GetGroundTerrainMap()->GetContentScript(x, y))
					Map[XPos + x][YPos + y]->ChangeGroundLevelTerrain(GroundTerrainScript->Instantiate());
	}

	if(RoomScript->GetOverTerrainMap(false))
	{
		XPos = BXPos + RoomScript->GetOverTerrainMap()->GetPos()->X;
		YPos = BYPos + RoomScript->GetOverTerrainMap()->GetPos()->Y;

		contentscript<overlevelterrain>* OverTerrainScript;

		for(ushort x = 0; x < RoomScript->GetCharacterMap()->GetSize()->X; ++x)
			for(ushort y = 0; y < RoomScript->GetOverTerrainMap()->GetSize()->Y; ++y)
				if(OverTerrainScript = RoomScript->GetOverTerrainMap()->GetContentScript(x, y))
					Map[XPos + x][YPos + y]->ChangeOverLevelTerrain(OverTerrainScript->Instantiate());
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
		if(!(rand() % (2 << (10 - game::GetCurrent()))))
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

	ABORT("Perttu is too popular!");
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

	SaveFile << UpStairs << DownStairs << WorldMapEntry << LevelMessage;
}

void level::Load(inputfile& SaveFile)
{
	area::Load(SaveFile);

	Map = (levelsquare***)area::Map;

	for(ushort x = 0; x < XSize; ++x)
		for(ulong y = 0; y < YSize; ++y)
		{
			Map[x][y] = new levelsquare(this, vector2d(x, y));
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

void level::Draw() const
{
	ushort XMax = GetXSize() < game::GetCamera().X + 50 ? GetXSize() : game::GetCamera().X + 50;
	ushort YMax = GetYSize() < game::GetCamera().Y + 30 ? GetYSize() : game::GetCamera().Y + 30;

	if(game::GetSeeWholeMapCheat())
		for(ushort x = game::GetCamera().X; x < XMax; ++x)
			for(ushort y = game::GetCamera().Y; y < YMax; ++y)
			{
				if(Map[x][y]->GetLastSeen() == game::GetLOSTurns())
					Map[x][y]->UpdateMemorizedAndDraw();
				else
					Map[x][y]->DrawCheat();
			}
	else
		for(ushort x = game::GetCamera().X; x < XMax; ++x)
			for(ushort y = game::GetCamera().Y; y < YMax; ++y)
			{
				if(Map[x][y]->GetLastSeen() == game::GetLOSTurns())
					Map[x][y]->UpdateMemorizedAndDraw();
				else
					Map[x][y]->DrawMemorized();
			}
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
	vector2d Pos(rand() % XSize, rand() % YSize);

	while(Map[Pos.X][Pos.Y]->GetOverLevelTerrain()->GetIsWalkable() != Walkablility || (HasCharacter && !Map[Pos.X][Pos.Y]->GetCharacter()) || (!HasCharacter && Map[Pos.X][Pos.Y]->GetCharacter()))
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

bool level::GetOnGround() const
{
	return *LevelScript->GetOnGround();
}

vector2d level::FreeSquareSeeker(vector2d StartPos, vector2d Prohibited, uchar MaxDistance)
{
	DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize() - 1 , GetYSize() - 1,
	{
		vector2d Vector = vector2d(DoX, DoY);

		if(GetSquare(Vector)->GetOverTerrain()->GetIsWalkable() && !GetSquare(Vector)->GetCharacter() && Vector != Prohibited)
			return Vector;
	})

	if(MaxDistance)
		DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize() - 1 , GetYSize() - 1,
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

vector2d level::GetNearestFreeSquare(vector2d StartPos)
{
	if(GetSquare(StartPos)->GetOverTerrain()->GetIsWalkable() && !GetSquare(StartPos)->GetCharacter())
		return StartPos;

	DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize() - 1 , GetYSize() - 1,
	{
		vector2d Vector = vector2d(DoX, DoY);

		if(GetSquare(Vector)->GetOverTerrain()->GetIsWalkable() && !GetSquare(Vector)->GetCharacter())
			return Vector;
	})

	for(ushort c = 0; c < 20; ++c)
		DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize() - 1 , GetYSize() - 1,
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

void level::MoveCharacter(vector2d From, vector2d To)
{
	GetLevelSquare(From)->MoveCharacter(GetLevelSquare(To));
}

ushort level::GetIdealPopulation() const 
{ 
	return 10 + (game::GetCurrent() << 2); 
}
