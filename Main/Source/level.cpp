#include <cmath>
#include <ctime>

#include "level.h"
#include "charba.h"
#include "itemde.h"
#include "lsquare.h"
#include "stack.h"
#include "lterrade.h"
#include "proto.h"
#include "script.h"
#include "team.h"
#include "config.h"
#include "femath.h"
#include "message.h"
#include "actionba.h"
#include "error.h"
#include "game.h"
#include "save.h"

#define FORBIDDEN 1
#define ON_POSSIBLE_ROUTE 2
#define STILL_ON_POSSIBLE_ROUTE 4
#define PREFERRED 8

level::~level()
{
  for(ushort c = 0; c < Room.size(); ++c)
    delete Room[c];
}

void level::ExpandPossibleRoute(vector2d Origo, vector2d Target, bool XMode)
{
  #define CHECK(x, y) (!(FlagMap[x][y] & ON_POSSIBLE_ROUTE) && !(FlagMap[x][y] & FORBIDDEN))

  #define CALL_EXPAND(x, y)\
    {\
      ExpandPossibleRoute(vector2d(x, y), Target, XMode);\
      \
      if(FlagMap[Target.X][Target.Y] & ON_POSSIBLE_ROUTE)\
	return;\
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

  #define CALL_EXPAND(x, y) \
    {\
      ExpandStillPossibleRoute(vector2d(x, y), Target, XMode);\
      \
      if(FlagMap[Target.X][Target.Y] & STILL_ON_POSSIBLE_ROUTE) \
	return;\
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
		Map[x][y]->ChangeOLTerrain(new empty);
	      }

	    for(ushort X = 0; X < XSize; ++X)
	      for(ushort Y = 0; Y < YSize; ++Y)
		FlagMap[X][Y] &= ~STILL_ON_POSSIBLE_ROUTE;
	  }

  for(ushort x = 1; x < XSize - 1; ++x)
    for(ushort y = 1; y < YSize - 1; ++y)
      FlagMap[x][y] &= ~ON_POSSIBLE_ROUTE;
}

void level::Generate()
{
  game::BusyAnimation();
  Initialize(LevelScript->GetSize()->X, LevelScript->GetSize()->Y);
  Map = (lsquare***)area::Map;

  if(LevelScript->GetLevelMessage(false))
    LevelMessage = *LevelScript->GetLevelMessage();

  for(ushort x = 0; x < XSize; ++x)
    {
      game::BusyAnimation();

      for(ulong y = 0; y < YSize; ++y)
	{
	  Map[x][y] = new lsquare(this, vector2d(x, y));
	  Map[x][y]->SetLTerrain(LevelScript->GetFillSquare()->GetGTerrain()->Instantiate(), LevelScript->GetFillSquare()->GetOTerrain()->Instantiate());
	}
    }

  ushort c;

  for(c = 0; c < *LevelScript->GetRooms(); ++c)
    {
      game::BusyAnimation();
      std::map<uchar, roomscript*>::const_iterator RoomIterator = LevelScript->GetRoom().find(c);
      roomscript* RoomScript;

      if(RoomIterator != LevelScript->GetRoom().end())
	{
	  while(true)
	    {
	      RoomScript = RoomIterator->second;

	      if(*RoomScript->GetReCalculate())
		{
		  inputfile ScriptFile(GAME_DIR + "Script/dungeon.dat");
		  RoomScript->ReadFrom(ScriptFile, true);
		}

	      if(MakeRoom(RoomScript))
		break;
	    }
	}
      else
	{
	  for(ushort i = 0; i < 10; ++i)
	    {
	      RoomScript = LevelScript->GetRoomDefault();

	      if(*RoomScript->GetReCalculate())
		{
		  inputfile ScriptFile(GAME_DIR + "Script/dungeon.dat");
		  RoomScript->ReadFrom(ScriptFile, true);
		}

	      if(MakeRoom(RoomScript))
		break;
	    }
	}
    }

  game::BusyAnimation();

  if(*LevelScript->GetGenerateUpStairs())
    CreateStairs(true);

  if(*LevelScript->GetGenerateDownStairs())
    CreateStairs(false);

  CreateItems(*LevelScript->GetItems());

  for(c = 0; c < LevelScript->GetSquare().size(); ++c)
    {
      game::BusyAnimation();
      squarescript* Square = LevelScript->GetSquare()[c];
      ApplyLSquareScript(Square);
    }

  if(LevelScript->GetBase())
    for(c = 0; c < LevelScript->GetBase()->GetSquare().size(); ++c)
      {
	game::BusyAnimation();
	squarescript* Square = LevelScript->GetBase()->GetSquare()[c];
	ApplyLSquareScript(Square);
      }
}

void level::ApplyLSquareScript(squarescript* Square)
{
  uchar Times = Square->GetTimes(false) ? *Square->GetTimes() : 1;

  for(ushort c = 0; c < Times; ++c)
    {
      vector2d Pos;

      if(Square->GetPosition()->GetRandom())
	{
	  if(Square->GetPosition()->GetInRoom(false))
	    for(Pos = RandomSquare(0, *Square->GetPosition()->GetWalkable());; Pos = RandomSquare(0, *Square->GetPosition()->GetWalkable()))
	      {
		if((!GetLSquare(Pos)->GetRoom() && !*Square->GetPosition()->GetInRoom()) || (GetLSquare(Pos)->GetRoom() && *Square->GetPosition()->GetInRoom()))
		  break;
	      }
	  else
	    Pos = RandomSquare(0, *Square->GetPosition()->GetWalkable());
	}
      else
	Pos = *Square->GetPosition()->GetVector();

      Map[Pos.X][Pos.Y]->ApplyScript(Square, 0);
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
  for(ushort x = 0; x < Amount; ++x)
    {
      vector2d Pos = RandomSquare(0, true);
      Map[Pos.X][Pos.Y]->Stack->FastAddItem(protosystem::BalancedCreateItem());
    }
}

void level::CreateStairs(bool Up)
{
  vector2d Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

  while((FlagMap[Pos.X][Pos.Y] & PREFERRED) || (FlagMap[Pos.X][Pos.Y] & FORBIDDEN))
    Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

  Map[Pos.X][Pos.Y]->ChangeOLTerrain(Up ? (olterrain*)(new stairsup) : (olterrain*)(new stairsdown));

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
  game::BusyAnimation();
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

  RoomClass->SetDivineMaster(*RoomScript->GetDivineMaster());

  game::BusyAnimation();

  {
    for(ushort x = XPos; x < XPos + Width; ++x)
      {
	Map[x][YPos]->ChangeLTerrain(RoomScript->GetWallSquare()->GetGTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOTerrain()->Instantiate());
	FlagMap[x][YPos] |= FORBIDDEN;
	Map[x][YPos + Height - 1]->ChangeLTerrain(RoomScript->GetWallSquare()->GetGTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOTerrain()->Instantiate());
	FlagMap[x][YPos + Height - 1] |= FORBIDDEN;

	if(*RoomScript->GetGenerateLanterns() && !(RAND() % 7) && x != XPos && x != XPos + Width - 1)
	  {
	    lantern* Lantern = new lantern;
	    Lantern->SignalSquarePositionChange(true);
	    Map[x][YPos]->GetSideStack(2)->FastAddItem(Lantern);
	  }

	if(*RoomScript->GetGenerateLanterns() && !(RAND() % 7) && x != XPos && x != XPos + Width - 1)
	  {
	    lantern* Lantern = new lantern;
	    Lantern->SignalSquarePositionChange(true);
	    Map[x][YPos + Height - 1]->GetSideStack(0)->FastAddItem(Lantern);
	  }

	if(RoomScript->GetDivineMaster(false) && *RoomScript->GetDivineMaster())
	  {
	    Map[x][YPos]->SetDivineMaster(*RoomScript->GetDivineMaster());
	    Map[x][YPos + Height - 1]->SetDivineMaster(*RoomScript->GetDivineMaster());
	  }

	Map[x][YPos]->SetRoom(RoomClass->GetIndex());
	Map[x][YPos + Height - 1]->SetRoom(RoomClass->GetIndex());
      }
  }

  game::BusyAnimation();

  for(ushort y = YPos; y < YPos + Height; ++y)
    {
      Map[XPos][y]->ChangeLTerrain(RoomScript->GetWallSquare()->GetGTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOTerrain()->Instantiate());
      FlagMap[XPos][y] |= FORBIDDEN;
      Map[XPos + Width - 1][y]->ChangeLTerrain(RoomScript->GetWallSquare()->GetGTerrain()->Instantiate(), RoomScript->GetWallSquare()->GetOTerrain()->Instantiate());
      FlagMap[XPos + Width - 1][y] |= FORBIDDEN;

      if(*RoomScript->GetGenerateLanterns() && !(RAND() % 7) && y != YPos && y != YPos + Height - 1)
	{
	  lantern* Lantern = new lantern;
	  Lantern->SignalSquarePositionChange(true);
	  Map[XPos][y]->GetSideStack(1)->FastAddItem(Lantern);
	}

      if(*RoomScript->GetGenerateLanterns() && !(RAND() % 7) && y != YPos && y != YPos + Height - 1)
	{
	  lantern* Lantern = new lantern;
	  Lantern->SignalSquarePositionChange(true);
	  Map[XPos + Width - 1][y]->GetSideStack(3)->FastAddItem(Lantern);
	}

      if(RoomScript->GetDivineMaster(false) && *RoomScript->GetDivineMaster())
	{
	  Map[XPos][y]->SetDivineMaster(*RoomScript->GetDivineMaster());
	  Map[XPos + Width - 1][y]->SetDivineMaster(*RoomScript->GetDivineMaster());
	}

      Map[XPos][y]->SetRoom(RoomClass->GetIndex());
      Map[XPos + Width - 1][y]->SetRoom(RoomClass->GetIndex());
    }

  game::BusyAnimation();

  for(ushort x = XPos + 1; x < XPos + Width - 1; ++x)
    for(ushort y = YPos + 1; y < YPos + Height - 1; ++y)
      {
	Map[x][y]->ChangeLTerrain(RoomScript->GetFloorSquare()->GetGTerrain()->Instantiate(), RoomScript->GetFloorSquare()->GetOTerrain()->Instantiate());

	FlagMap[x][y] |= FORBIDDEN;

	if(RoomScript->GetDivineMaster(false) && *RoomScript->GetDivineMaster())
	  Map[x][y]->SetDivineMaster(*RoomScript->GetDivineMaster());

	Map[x][y]->SetRoom(RoomClass->GetIndex());
      }

  if(*RoomScript->GetGenerateFountains() && !(RAND() % 10))
    {
      vector2d Pos(XPos + 1 + RAND() % (Width-2), YPos + 1 + RAND() % (Height-2));
      Map[Pos.X][Pos.Y]->ChangeOLTerrain(new fountain);
    }

  if(*RoomScript->GetAltarPossible() && !(RAND() % 5))
    {
      vector2d Pos(XPos + 1 + RAND() % (Width-2), YPos + 1 + RAND() % (Height-2));
      Map[Pos.X][Pos.Y]->ChangeOLTerrain(new altar);

      uchar Owner = ((altar*)Map[Pos.X][Pos.Y]->GetOLTerrain())->GetDivineMaster();

      for(ushort x = XPos + 1; x < XPos + Width - 1; ++x)
	for(ushort y = YPos + 1; y < YPos + Height - 1; ++y)
	  Map[x][y]->SetDivineMaster(Owner);
    }

  if(*RoomScript->GetGenerateTunnel() && Door.size())
    {
      game::BusyAnimation();
      vector2d LPos = Door[RAND() % Door.size()];
      ushort LXPos = LPos.X, LYPos = LPos.Y;
      olterrain* Door = RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate(); //Bug! Wrong room!

      if(!(RAND() % 5) && *RoomScript->GetAllowLockedDoors())
	{
	  if(*RoomScript->GetAllowBoobyTrappedDoors() && !(RAND() % 5))
	    Door->CreateBoobyTrap();

	  Door->Lock();
	}      
      Map[LXPos][LYPos]->ChangeLTerrain(RoomScript->GetDoorSquare()->GetGTerrain()->Instantiate(), Door);
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

      Door = RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate();

      if(!(RAND() % 5) && *RoomScript->GetAllowLockedDoors())
	{
	  if(*RoomScript->GetAllowBoobyTrappedDoors() && !(RAND() % 5))
	    Door->CreateBoobyTrap();

	  Door->Lock();
	}

      Map[XPos][YPos]->ChangeLTerrain(RoomScript->GetDoorSquare()->GetGTerrain()->Instantiate(), Door);
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
      game::BusyAnimation();

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

      Door.push_back(vector2d(XPos, YPos));

      if(!*RoomScript->GetGenerateTunnel())
	{
	  Map[XPos][YPos]->ChangeLTerrain(RoomScript->GetDoorSquare()->GetGTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate());
	  Map[XPos][YPos]->Clean();
	}
    }

  for(ushort c = 0; c < RoomScript->GetSquare().size(); ++c)
    {
      game::BusyAnimation();
      squarescript* Square = RoomScript->GetSquare()[c];
      uchar Times = Square->GetTimes(false) ? *Square->GetTimes() : 1;

      for(ushort c = 0; c < Times; ++c)
	{
	  vector2d Pos;

	  if(Square->GetPosition()->GetRandom())
	    ABORT("Illegal command: Random square positioning not supported in roomscript!");
	  else
	    Pos = *Square->GetPosition()->GetVector();

	  Map[BXPos + Pos.X][BYPos + Pos.Y]->ApplyScript(Square, RoomClass);
	}
    }

  if(RoomScript->GetCharacterMap(false))
    {
      XPos = BXPos + RoomScript->GetCharacterMap()->GetPos()->X;
      YPos = BYPos + RoomScript->GetCharacterMap()->GetPos()->Y;
      const contentscript<character>* CharacterScript;

      for(ushort x = 0; x < RoomScript->GetCharacterMap()->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

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
    }

  if(RoomScript->GetItemMap(false))
    {
      XPos = BXPos + RoomScript->GetItemMap()->GetPos()->X;
      YPos = BYPos + RoomScript->GetItemMap()->GetPos()->Y;
      const contentscript<item>* ItemScript;

      for(ushort x = 0; x < RoomScript->GetItemMap()->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(ushort y = 0; y < RoomScript->GetItemMap()->GetSize()->Y; ++y)
	    if((ItemScript = RoomScript->GetItemMap()->GetContentScript(x, y)))
	      Map[XPos + x][YPos + y]->GetStack()->FastAddItem(ItemScript->Instantiate());
	}
    }

  if(RoomScript->GetGTerrainMap(false))
    {
      XPos = BXPos + RoomScript->GetGTerrainMap()->GetPos()->X;
      YPos = BYPos + RoomScript->GetGTerrainMap()->GetPos()->Y;
      const contentscript<glterrain>* GTerrainScript;

      for(ushort x = 0; x < RoomScript->GetGTerrainMap()->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(ushort y = 0; y < RoomScript->GetGTerrainMap()->GetSize()->Y; ++y)
	    if((GTerrainScript = RoomScript->GetGTerrainMap()->GetContentScript(x, y)))
	      Map[XPos + x][YPos + y]->ChangeGLTerrain(GTerrainScript->Instantiate());
	}
    }

  if(RoomScript->GetOTerrainMap(false))
    {
      XPos = BXPos + RoomScript->GetOTerrainMap()->GetPos()->X;
      YPos = BYPos + RoomScript->GetOTerrainMap()->GetPos()->Y;
      const contentscript<olterrain>* OTerrainScript;

      for(ushort x = 0; x < RoomScript->GetOTerrainMap()->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(ushort y = 0; y < RoomScript->GetOTerrainMap()->GetSize()->Y; ++y)
	    if((OTerrainScript = RoomScript->GetOTerrainMap()->GetContentScript(x, y)))
	      {
		olterrain* Terrain = OTerrainScript->Instantiate();
		Map[XPos + x][YPos + y]->ChangeOLTerrain(Terrain);
		RoomClass->HandleInstantiatedOLTerrain(Terrain);
	      }
	}
    }

  return true;
}

void level::GenerateMonsters()
{
  if(*LevelScript->GetGenerateMonsters())
    {
      ushort Population = 0;

      for(ushort c = 0; c < game::GetTeams(); ++c)
	{
	  Population += game::GetTeam(c)->GetMember().size();
	}

      if(Population < GetIdealPopulation())
	if(!(RAND() % (2 << (11 - game::GetCurrent()))))
	  GenerateNewMonsters(1);
    }
}

void level::Save(outputfile& SaveFile) const
{
  area::Save(SaveFile);
  SaveFile << Room;

  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->Save(SaveFile);

  SaveFile << Door << UpStairs << DownStairs << WorldMapEntry << LevelMessage;
}

void level::Load(inputfile& SaveFile)
{
  area::Load(SaveFile);
  Map = (lsquare***)area::Map;
  SaveFile >> Room;

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	game::SetSquareInLoad(Map[x][y]);
	Map[x][y]->Load(SaveFile);
      }

  SaveFile >> Door >> UpStairs >> DownStairs >> WorldMapEntry >> LevelMessage;
}

void level::Luxify()
{
  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      {
	Map[x][y]->CalculateLuminance();
	Map[x][y]->Emitate();
      }
}

void level::FastAddCharacter(vector2d Pos, character* Guy)
{
  Map[Pos.X][Pos.Y]->FastAddCharacter(Guy);
}

void level::GenerateNewMonsters(ushort HowMany, bool ConsiderPlayer)
{
  vector2d Pos;

  for(ushort c = 0; c < HowMany; ++c)
    {
      Pos = vector2d(0,0);

      character* Char = protosystem::BalancedCreateMonster();

      for(ushort cc = 0; cc < 30; ++c)
	{
	  Pos = RandomSquare(Char, true);
			
	  if(!ConsiderPlayer || (abs(short(Pos.X) - game::GetPlayer()->GetPos().X) > 6 && abs(short(Pos.Y) - game::GetPlayer()->GetPos().Y) > 6))
	    break;
	}

      if(!(Pos.X == 0 && Pos.Y == 0))
	Map[Pos.X][Pos.Y]->AddCharacter(Char);
    }
}

vector2d level::RandomSquare(character* Char, bool Walkablility, bool HasCharacter) const
{
  vector2d Pos(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

  for(ushort c = 0; (Map[Pos.X][Pos.Y]->IsWalkable(Char) != Walkablility || (HasCharacter && !Map[Pos.X][Pos.Y]->GetCharacter()) || (!HasCharacter && Map[Pos.X][Pos.Y]->GetCharacter())) && c < 1000; ++c)
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
  GetLSquare(From)->MoveCharacter(GetLSquare(To));
}

ushort level::GetIdealPopulation() const 
{ 
  return 10 + (game::GetCurrent() * 4);
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

room* level::GetRoom(ushort Index) const
{
  if(!Index)
    ABORT("Access to room zero denied!");

  return Room[Index];
}

void level::Explosion(character* Terrorist, const std::string& DeathMsg, vector2d Pos, ushort Strength, bool HurtNeutrals)
{
  ushort EmitChange = 300 + Strength * 2;

  if(EmitChange > 511)
    EmitChange = 511;

  GetLSquare(Pos)->SetTemporaryEmitation(EmitChange);

  ushort ContrastLuminance = (configuration::GetContrast() << 8) / 100;

  static ushort StrengthLimit[6] = { 150, 100, 75, 50, 25, 10 };
  static vector2d StrengthPicPos[7] = { vector2d(176, 176), vector2d(0, 144), vector2d(256, 32), vector2d(144, 32), vector2d(64, 32), vector2d(16, 32),vector2d(0, 32) };

  uchar Size = 6;

  for(ushort c = 0; c < 6; ++c)
    if(Strength >= StrengthLimit[c])
      {
        Size = c;
        break;
      }

  vector2d BPos = game::CalculateScreenCoordinates(Pos) - vector2d(16 * (6 - Size), 16 * (6 - Size));
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

      if(BPos.X >= RES.X || BPos.Y >= RES.Y)
	break;

      if(BPos.X + SizeVect.X > RES.X)
	SizeVect.X = RES.X - BPos.X;

      if(BPos.Y + SizeVect.Y > RES.Y)
	SizeVect.Y = RES.Y - BPos.Y;

      game::DrawEverythingNoBlit();

      uchar Flags = RAND() % 8;

      if(!Flags || SizeVect != OldSizeVect)
	igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, PicPos, BPos, SizeVect, ContrastLuminance);
      else
	{
	  bitmap ExplosionPic(SizeVect.X, SizeVect.Y);
	  igraph::GetSymbolGraphic()->Blit(&ExplosionPic, PicPos, 0, 0, SizeVect, Flags);
	  ExplosionPic.MaskedBlit(DOUBLEBUFFER, 0, 0, BPos, SizeVect, ContrastLuminance);
	}

      graphics::BlitDBToScreen();
      game::GetCurrentArea()->SendNewDrawRequest();
      clock_t StartTime = clock();
      while(clock() - StartTime < 0.3f * CLOCKS_PER_SEC);
      break;
    }

  GetLSquare(Pos)->SetTemporaryEmitation(0);

  ushort Radius = 8 - Size;
  ushort RadiusSquare = Radius * Radius;
  ushort PlayerDamage = 0;
  bool PlayerHurt = false;

  DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, GetXSize() - 1, GetYSize() - 1, Radius,
  {
    ushort DistanceSquare = GetHypotSquare(long(Pos.X) - XPointer, long(Pos.Y) - YPointer);

    if(DistanceSquare <= RadiusSquare)
      {
	lsquare* Square = GetLSquare(XPointer, YPointer);
	character* Char = Square->GetCharacter();
	ushort Damage = Strength / (DistanceSquare + 1);
	uchar DamageDirection = game::CalculateRoughDirection(vector2d(XPointer, YPointer) - Pos);

	if(Char && (HurtNeutrals || (Terrorist && Char->GetTeam()->GetRelation(Terrorist->GetTeam()) == HOSTILE)))
	  if(Char->IsPlayer())
	    {
	      PlayerDamage = Damage;
	      PlayerHurt = true;
	    }
	  else
	    {
	      if(Terrorist)
		Terrorist->GetTeam()->Hostility(Char->GetTeam());

	      Char->SpillBlood((8 - Size + RAND() % (8 - Size)) / 2);

	      if(game::IsValidPos(vector2d(XPointer, YPointer) + game::GetMoveVector(DamageDirection)))
		Char->SpillBlood((8 - Size + RAND() % (8 - Size)) / 2, vector2d(XPointer, YPointer) + game::GetMoveVector(DamageDirection));

	      if(Char->CanBeSeenByPlayer())
		ADD_MESSAGE("%s is hit by the explosion.", Char->CHARNAME(DEFINITE));

	      Char->ReceiveDamage(Terrorist, Damage / 2, PHYSICALDAMAGE, ALL, DamageDirection, true);
	      Char->ReceiveDamage(Terrorist, Damage / 2, FIRE, ALL, DamageDirection, true);
	      Char->CheckDeath(DeathMsg);
	    }

	Square->GetStack()->ReceiveDamage(Terrorist, Damage, FIRE);

	if(Damage >= 20 && Square->GetOLTerrain()->CanBeDug() && Square->GetOLTerrain()->GetMainMaterial()->GetStrengthValue() < 100)
	  Square->GetOLTerrain()->Break();
      }
  });

  if(PlayerHurt)
    {
      uchar DamageDirection = game::CalculateRoughDirection(game::GetPlayer()->GetPos() - Pos);

      if(Terrorist)
	Terrorist->GetTeam()->Hostility(game::GetPlayer()->GetTeam());

      game::GetPlayer()->SpillBlood((8 - Size + RAND() % (8 - Size)) / 2);

      if(game::IsValidPos(game::GetPlayer()->GetPos() + game::GetMoveVector(DamageDirection)))
	game::GetPlayer()->SpillBlood((8 - Size + RAND() % (8 - Size)) / 2, game::GetPlayer()->GetPos() + game::GetMoveVector(DamageDirection));

      ADD_MESSAGE("You are hit by the explosion!");
      game::GetPlayer()->ReceiveDamage(Terrorist, PlayerDamage / 2, PHYSICALDAMAGE, ALL, DamageDirection, true);
      game::GetPlayer()->ReceiveDamage(Terrorist, PlayerDamage / 2, FIRE, ALL, DamageDirection, true);
      game::GetPlayer()->CheckDeath(DeathMsg);
    }
}

bool level::CollectCreatures(std::vector<character*>& CharacterArray, character* Leader, bool AllowHostiles)
{
  if(!AllowHostiles)
    DO_FILLED_RECTANGLE(Leader->GetPos().X, Leader->GetPos().Y, 0, 0, GetXSize() - 1, GetYSize() - 1, Leader->LOSRange(),
    {
      character* Char = GetLSquare(XPointer, YPointer)->GetCharacter();

      if(Char)
	if(Char->GetTeam()->GetRelation(Leader->GetTeam()) == HOSTILE && Char->CanBeSeenBy(Leader))
	  {
	    ADD_MESSAGE("You can't escape when there are hostile creatures nearby.");
	    return false;
	  }
    });

  DO_FILLED_RECTANGLE(Leader->GetPos().X, Leader->GetPos().Y, 0, 0, GetXSize() - 1, GetYSize() - 1, Leader->LOSRange(),
  {
    character* Char = GetLSquare(XPointer, YPointer)->GetCharacter();

    if(Char)
      if(Char != Leader && (Char->GetTeam() == Leader->GetTeam() || Char->GetTeam()->GetRelation(Leader->GetTeam()) == HOSTILE) && Char->CanBeSeenBy(Leader))
	{
	  if(Char->GetAction() && Char->GetAction()->IsVoluntary())
	    Char->GetAction()->Terminate(false);

	  if(!Char->GetAction())
	    {
	      ADD_MESSAGE("%s follows you.", Char->CHARNAME(DEFINITE));
	      CharacterArray.push_back(Char);
	      game::GetCurrentLevel()->RemoveCharacter(Char->GetPos());
	    }
	}
  });

  return true;
}

bool level::IsValid(vector2d Vector) const
{
  if(Vector.X >= 0 && Vector.Y >= 0 && Vector.X < GetXSize() && Vector.Y < GetYSize())
    return true;
  else
    return false;
}

void level::Draw() const
{
  if(!game::GetSeeWholeMapCheat())
    for(ushort x = game::GetCamera().X; x < game::GetCamera().X + game::GetScreenSize().X; ++x)
      for(ushort y = game::GetCamera().Y; y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
	if(Map[x][y]->GetLastSeen() == game::GetLOSTurns())
	  Map[x][y]->Draw();
	else
	  Map[x][y]->DrawMemorized();
  else
    for(ushort x = game::GetCamera().X; x < game::GetCamera().X + game::GetScreenSize().X; ++x)
      for(ushort y = game::GetCamera().Y; y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
	Map[x][y]->Draw();
}
