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
	  CALL_EXPAND(Origo.X - 1, Origo.Y);

      if(Target.X > Origo.X)
	if(CHECK(Origo.X + 1, Origo.Y))
	  CALL_EXPAND(Origo.X + 1, Origo.Y);

      if(Target.Y < Origo.Y)
	if(CHECK(Origo.X, Origo.Y - 1))
	  CALL_EXPAND(Origo.X, Origo.Y - 1);

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
	  CALL_EXPAND(Origo.X, Origo.Y - 1);

      if(Target.Y > Origo.Y)
	if(CHECK(Origo.X, Origo.Y + 1))
	  CALL_EXPAND(Origo.X, Origo.Y + 1);

      if(Target.X < Origo.X)
	if(CHECK(Origo.X - 1, Origo.Y))
	  CALL_EXPAND(Origo.X - 1, Origo.Y);

      if(Target.X > Origo.X)
	if(CHECK(Origo.X + 1, Origo.Y))
	  CALL_EXPAND(Origo.X + 1, Origo.Y);

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
	  CALL_EXPAND(Origo.X - 1, Origo.Y);

      if(Target.X > Origo.X)
	if(CHECK(Origo.X + 1, Origo.Y))
	  CALL_EXPAND(Origo.X + 1, Origo.Y);

      if(Target.Y < Origo.Y)
	if(CHECK(Origo.X, Origo.Y - 1))
	  CALL_EXPAND(Origo.X, Origo.Y - 1);

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
	  CALL_EXPAND(Origo.X, Origo.Y - 1);

      if(Target.Y > Origo.Y)
	if(CHECK(Origo.X, Origo.Y + 1))
	  CALL_EXPAND(Origo.X, Origo.Y + 1);

      if(Target.X < Origo.X)
	if(CHECK(Origo.X - 1, Origo.Y))
	  CALL_EXPAND(Origo.X - 1, Origo.Y);

      if(Target.X > Origo.X)
	if(CHECK(Origo.X + 1, Origo.Y))
	  CALL_EXPAND(Origo.X + 1, Origo.Y);

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

void level::Generate(ushort Index)
{
  game::BusyAnimation();
  Initialize(LevelScript->GetSize()->X, LevelScript->GetSize()->Y);
  Map = reinterpret_cast<lsquare***>(area::Map);
  std::string* Msg = LevelScript->GetLevelMessage(false);

  if(Msg)
    LevelMessage = *Msg;

  if(*LevelScript->GetGenerateMonsters())
    {
      MonsterGenerationInterval = *LevelScript->GetMonsterGenerationIntervalBase() + *LevelScript->GetMonsterGenerationIntervalDelta() * Index;
      IdealPopulation = *LevelScript->GetMonsterAmountBase() + *LevelScript->GetMonsterAmountDelta() * Index;
      Difficulty = *LevelScript->GetDifficultyBase() + *LevelScript->GetDifficultyDelta() * Index;
    }

  contentscript<glterrain>* GTerrain = LevelScript->GetFillSquare()->GetGTerrain();
  contentscript<olterrain>* OTerrain = LevelScript->GetFillSquare()->GetOTerrain();
  ulong Counter = 0;

  for(ushort x = 0; x < XSize; ++x)
    {
      game::BusyAnimation();

      for(ushort y = 0; y < YSize; ++y, ++Counter)
	{
	  Map[x][y] = new lsquare(this, vector2d(x, y));
	  Map[x][y]->SetLTerrain(GTerrain->Instantiate(), OTerrain->Instantiate());
	}
    }

  ushort c;
  inputfile ScriptFile(GAME_DIR + "Script/dungeon.dat");

  for(c = 0; c < *LevelScript->GetRooms(); ++c)
    {
      game::BusyAnimation();
      std::map<uchar, roomscript*>::const_iterator RoomIterator = LevelScript->GetRoom().find(c);
      roomscript* RoomScript;

      if(RoomIterator != LevelScript->GetRoom().end())
	{
	  ushort i;

	  for(i = 0; i < 1000; ++i)
	    {
	      RoomScript = RoomIterator->second;

	      if(*RoomScript->GetReCalculate())
		RoomScript->ReadFrom(ScriptFile, true);

	      if(MakeRoom(RoomScript))
		break;
	    }

	  if(i == 1000)
	    ABORT("Failed to place room #%d!", c);
	}
      else
	{
	  for(ushort i = 0; i < 10; ++i)
	    {
	      RoomScript = LevelScript->GetRoomDefault();

	      if(*RoomScript->GetReCalculate())
		RoomScript->ReadFrom(ScriptFile, true);

	      if(MakeRoom(RoomScript))
		break;
	    }
	}
    }

  game::BusyAnimation();

  if(!*LevelScript->GetIgnoreDefaultSpecialSquares())
    {
      /* Gum solution */

      levelscript* LevelBase = static_cast<levelscript*>(LevelScript->GetBase());

      if(LevelBase)
	for(c = 0; c < LevelBase->GetSquare().size(); ++c)
	  {
	    game::BusyAnimation();
	    squarescript* Script = LevelBase->GetSquare()[c];
	    ApplyLSquareScript(Script);
	  }
    }

  for(c = 0; c < LevelScript->GetSquare().size(); ++c)
    {
      game::BusyAnimation();
      squarescript* Script = LevelScript->GetSquare()[c];
      ApplyLSquareScript(Script);
    }

  CreateItems(*LevelScript->GetItems());
}

void level::ApplyLSquareScript(squarescript* Script)
{
  uchar* ScriptTimes = Script->GetTimes(false);
  uchar Times = ScriptTimes ? *ScriptTimes : 1;

  for(ushort c = 0; c < Times; ++c)
    {
      vector2d Pos;

      if(Script->GetPosition()->GetRandom())
	Pos = GetRandomSquare(0, *Script->GetPosition()->GetFlags(), Script->GetPosition()->GetBorders(false));
      else
	Pos = *Script->GetPosition()->GetVector();

      Map[Pos.X][Pos.Y]->ApplyScript(Script, 0);
    }
}

void level::AttachPos(vector2d What)
{
  vector2d Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

  while(!(FlagMap[Pos.X][Pos.Y] & PREFERRED))
    Pos = vector2d(1 + RAND() % (XSize - 2), 1 + RAND() % (YSize - 2));

  FlagMap[What.X][What.Y] &= ~FORBIDDEN;
  FlagMap[What.X][What.Y] |= PREFERRED;

  GenerateTunnel(What, Pos, RAND() & 1);

  FlagMap[What.X][What.Y] |= FORBIDDEN;
  FlagMap[What.X][What.Y] &= ~PREFERRED;
}

void level::CreateItems(ushort Amount)
{
  for(ushort x = 0; x < Amount; ++x)
    {
      vector2d Pos = GetRandomSquare();
      item* Item = protosystem::BalancedCreateItem();
      Map[Pos.X][Pos.Y]->Stack->AddItem(Item);
      Item->SpecialGenerationHandler();
    }
}

bool level::MakeRoom(roomscript* RoomScript)
{
  game::BusyAnimation();
  ushort XPos = RoomScript->GetPos()->X, YPos = RoomScript->GetPos()->Y, Width = RoomScript->GetSize()->X, Height = RoomScript->GetSize()->Y;
  ushort x, y, c;

  if(XPos + Width > XSize - 2)
    return false;

  if(YPos + Height > YSize - 2)
    return false;

  for(x = XPos - 1; x <= XPos + Width; ++x)
    for(y = YPos - 1; y <= YPos + Height; ++y)
      if(FlagMap[x][y] & FORBIDDEN || FlagMap[x][y] & PREFERRED)
	return false;

  room* RoomClass = protocontainer<room>::GetProto(*RoomScript->GetType())->Clone();
  RoomClass->SetPos(vector2d(XPos, YPos));
  RoomClass->SetSize(vector2d(Width, Height));
  AddRoom(RoomClass);
  RoomClass->SetDivineMaster(*RoomScript->GetDivineMaster());
  game::BusyAnimation();
  std::vector<vector2d> OKForDoor, Inside, Border;

  GenerateRectangularRoom(OKForDoor, Inside, Border, RoomScript, RoomClass, vector2d(XPos, YPos), vector2d(Width, Height));
  game::BusyAnimation();

  if(*RoomScript->GetGenerateFountains() && !(RAND() % 10))
    GetLSquare(Inside[RAND() % Inside.size()])->ChangeOLTerrain(new fountain);

  if(*RoomScript->GetAltarPossible() && !(RAND() % 5))
    {
      uchar Owner = 1 + RAND() % (game::GetGods() - 1);
      GetLSquare(Inside[RAND() % Inside.size()])->ChangeOLTerrain(new altar(Owner));

      for(c = 0; c < Inside.size(); ++c)
	GetLSquare(Inside[c])->SetDivineMaster(Owner);

      for(c = 0; c < Border.size(); ++c)
	GetLSquare(Border[c])->SetDivineMaster(Owner);
    }

  if(*RoomScript->GetGenerateTunnel() && !Door.empty())
    {
      game::BusyAnimation();

      vector2d OutsideDoorPos = Door[RAND() % Door.size()]; // An other room

      if(OKForDoor.empty())
	ABORT("The Doors - You are strange.");

      vector2d InsideDoorPos = OKForDoor[RAND() % OKForDoor.size()]; // this door

      olterrain* Door = RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate(); //Bug! Wrong room!

      if(!(RAND() % 5) && *RoomScript->GetAllowLockedDoors())
	{
	  if(*RoomScript->GetAllowBoobyTrappedDoors() && !(RAND() % 5))
	    Door->CreateBoobyTrap();

	  Door->Lock();
	}

      Map[OutsideDoorPos.X][OutsideDoorPos.Y]->ChangeLTerrain(RoomScript->GetDoorSquare()->GetGTerrain()->Instantiate(), Door);
      Map[OutsideDoorPos.X][OutsideDoorPos.Y]->Clean();
      FlagMap[OutsideDoorPos.X][OutsideDoorPos.Y] &= ~FORBIDDEN;
      FlagMap[OutsideDoorPos.X][OutsideDoorPos.Y] |= PREFERRED;
      FlagMap[InsideDoorPos.X][InsideDoorPos.Y] &= ~FORBIDDEN;
      FlagMap[InsideDoorPos.X][InsideDoorPos.Y] |= PREFERRED;
      Door = RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate();

      if(!(RAND() % 5) && *RoomScript->GetAllowLockedDoors())
	{
	  if(*RoomScript->GetAllowBoobyTrappedDoors() && !(RAND() % 5))
	    Door->CreateBoobyTrap();

	  Door->Lock();
	}

      Map[InsideDoorPos.X][InsideDoorPos.Y]->ChangeLTerrain(RoomScript->GetDoorSquare()->GetGTerrain()->Instantiate(), Door);
      Map[InsideDoorPos.X][InsideDoorPos.Y]->Clean();
      GenerateTunnel(vector2d(InsideDoorPos.X, InsideDoorPos.Y), vector2d(OutsideDoorPos.X, OutsideDoorPos.Y), RAND() & 1);
      FlagMap[OutsideDoorPos.X][OutsideDoorPos.Y] |= FORBIDDEN;
      FlagMap[OutsideDoorPos.X][OutsideDoorPos.Y] &= ~PREFERRED;
      FlagMap[InsideDoorPos.X][InsideDoorPos.Y] |= FORBIDDEN;
      FlagMap[InsideDoorPos.X][InsideDoorPos.Y] &= ~PREFERRED;
    }

  if(*RoomScript->GetGenerateDoor())
    {
      game::BusyAnimation();
      vector2d DoorPos;

      if(OKForDoor.empty())
	ABORT("The Doors - This thing has been broken.");

      DoorPos = OKForDoor[RAND() % OKForDoor.size()];
      Door.push_back(DoorPos);

      if(!*RoomScript->GetGenerateTunnel())
	{
	  Map[DoorPos.X][DoorPos.Y]->ChangeLTerrain(RoomScript->GetDoorSquare()->GetGTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate());
	  Map[DoorPos.X][DoorPos.Y]->Clean();
	}
    }

  charactercontentmap* CharacterMap = RoomScript->GetCharacterMap(false);

  if(CharacterMap)
    {
      vector2d CharPos(XPos + CharacterMap->GetPos()->X, YPos + CharacterMap->GetPos()->Y);
      const contentscript<character>* CharacterScript;

      for(ushort x = 0; x < CharacterMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < CharacterMap->GetSize()->Y; ++y)
	    if((CharacterScript = CharacterMap->GetContentScript(x, y)))
	      {
		character* Char = CharacterScript->Instantiate();

		if(!Char->GetTeam())
		  Char->SetTeam(game::GetTeam(*LevelScript->GetTeamDefault()));

		Map[CharPos.X + x][CharPos.Y + y]->AddCharacter(Char);
		Char->SetHomePos(vector2d(CharPos.X + x, CharPos.Y + y));
		RoomClass->HandleInstantiatedCharacter(Char);
		bool* IsMaster = CharacterScript->GetIsMaster(false);

		if(IsMaster && *IsMaster)
		  RoomClass->SetMaster(Char);
	      }
	}
    }

  itemcontentmap* ItemMap = RoomScript->GetItemMap(false);

  if(ItemMap)
    {
      vector2d ItemPos(XPos + ItemMap->GetPos()->X, YPos + ItemMap->GetPos()->Y);
      const std::vector<contentscript<item> >* ItemScript;

      for(ushort x = 0; x < ItemMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < ItemMap->GetSize()->Y; ++y)
	    if((ItemScript = ItemMap->GetContentScript(x, y)))
	      for(c = 0; c < ItemScript->size(); ++c)
		{
		  stack* Stack;
		  item* Item = (*ItemScript)[c].Instantiate();

		  if(Item)
		    {
		      uchar* SideStackIndex = (*ItemScript)[c].GetSideStackIndex(false);

		      if(!SideStackIndex)
			Stack = Map[ItemPos.X + x][ItemPos.Y + y]->GetStack();
		      else
			{
			  Item->SignalSquarePositionChange(*SideStackIndex);
			  Stack = Map[ItemPos.X + x][ItemPos.Y + y]->GetSideStack(*SideStackIndex);
			}

		      Stack->AddItem(Item);
		    }
		}
	}
    }

  glterraincontentmap* GTerrainMap = RoomScript->GetGTerrainMap(false);

  if(GTerrainMap)
    {
      vector2d GTerrainPos(XPos + GTerrainMap->GetPos()->X, YPos + GTerrainMap->GetPos()->Y);
      const contentscript<glterrain>* GTerrainScript;

      for(ushort x = 0; x < GTerrainMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < GTerrainMap->GetSize()->Y; ++y)
	    if((GTerrainScript = GTerrainMap->GetContentScript(x, y)))
	      Map[GTerrainPos.X + x][GTerrainPos.Y + y]->ChangeGLTerrain(GTerrainScript->Instantiate());
	}
    }

  olterraincontentmap* OTerrainMap = RoomScript->GetOTerrainMap(false);

  if(OTerrainMap)
    {
      vector2d OTerrainPos(XPos + OTerrainMap->GetPos()->X, YPos + OTerrainMap->GetPos()->Y);
      const contentscript<olterrain>* OTerrainScript;

      for(ushort x = 0; x < OTerrainMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < OTerrainMap->GetSize()->Y; ++y)
	    if((OTerrainScript = OTerrainMap->GetContentScript(x, y)))
	      {
		olterrain* Terrain = OTerrainScript->Instantiate();
		Map[OTerrainPos.X + x][OTerrainPos.Y + y]->ChangeOLTerrain(Terrain);
		RoomClass->HandleInstantiatedOLTerrain(Terrain);
	      }
	}
    }

  for(c = 0; c < RoomScript->GetSquare().size(); ++c)
    {
      game::BusyAnimation();
      squarescript* Script = RoomScript->GetSquare()[c];
      uchar* ScriptTimes = Script->GetTimes(false);
      uchar Times = ScriptTimes ? *ScriptTimes : 1;

      for(ushort t = 0; t < Times; ++t)
	{
	  vector2d Pos;

	  if(Script->GetPosition()->GetRandom())
	    {
	      rect* ScriptBorders = Script->GetPosition()->GetBorders(false);
	      rect Borders = ScriptBorders ? *ScriptBorders + vector2d(XPos, YPos) : rect(XPos, YPos, XPos + Width - 1, YPos + Height - 1);
	      Pos = GetRandomSquare(0, *Script->GetPosition()->GetFlags(), &Borders);
	    }
	  else
	    Pos = vector2d(XPos, YPos) + *Script->GetPosition()->GetVector();

	  Map[Pos.X][Pos.Y]->ApplyScript(Script, RoomClass);
	}
    }

  return true;
}

void level::GenerateLanterns(ushort X, ushort Y, uchar SquarePos) const
{
  if(!(RAND() % 7))
    {
      lantern* Lantern = new lantern;
      Lantern->SignalSquarePositionChange(SquarePos);
      Map[X][Y]->GetSideStack(SquarePos)->AddItem(Lantern);
    }
}

void level::CreateRoomSquare(glterrain* GLTerrain, olterrain* OLTerrain, ushort X, ushort Y, uchar Room, uchar DivineMaster) const
{
  Map[X][Y]->ChangeLTerrain(GLTerrain, OLTerrain);
  FlagMap[X][Y] |= FORBIDDEN;

  if(DivineMaster)
    Map[X][Y]->SetDivineMaster(DivineMaster);

  Map[X][Y]->SetRoom(Room);
}

void level::GenerateMonsters()
{
  if(*LevelScript->GetGenerateMonsters()
  && game::GetTeam(MONSTER_TEAM)->GetEnabledMembers() < IdealPopulation
  && !(RAND() % MonsterGenerationInterval))
    GenerateNewMonsters(1);
}

void level::Save(outputfile& SaveFile) const
{
  area::Save(SaveFile);
  SaveFile << Room;

  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->Save(SaveFile);

  SaveFile << Door << LevelMessage << IdealPopulation << MonsterGenerationInterval << Difficulty;
}

void level::Load(inputfile& SaveFile)
{
  area::Load(SaveFile);
  Map = reinterpret_cast<lsquare***>(area::Map);
  SaveFile >> Room;

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	game::SetSquareInLoad(Map[x][y]);
	Map[x][y]->Load(SaveFile);
      }

  SaveFile >> Door >> LevelMessage >> IdealPopulation >> MonsterGenerationInterval >> Difficulty;
}

void level::FiatLux()
{
  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      {
	Map[x][y]->CalculateEmitation();
	Map[x][y]->Emitate();
	Map[x][y]->CalculateLuminance();
      }
}

#include <fstream>
#include <iostream>

void level::GenerateNewMonsters(ushort HowMany, bool ConsiderPlayer)
{
  vector2d Pos;

  for(ushort c = 0; c < HowMany; ++c)
    {
      Pos = vector2d(0,0);
      character* Char = protosystem::BalancedCreateMonster();

      for(ushort cc = 0; cc < 30; ++cc)
	{
	  Pos = GetRandomSquare(Char);
			
	  if(!ConsiderPlayer || abs(short(Pos.X) - game::GetPlayer()->GetPos().X) > 6 || abs(short(Pos.Y) - game::GetPlayer()->GetPos().Y) > 6)
	    break;
	}

      if(!(Pos.X == 0 && Pos.Y == 0))
	Map[Pos.X][Pos.Y]->AddCharacter(Char);
    }
}

/* example of the usage: GetRandomSquare() gives out a random walkable square* */
vector2d level::GetRandomSquare(const character* Char, uchar Flags, const rect* Borders) const
{
  for(ushort c = 0;; ++c)
    {
      if(c == 100)
	Char = 0;

      if(c == 10000)
	ABORT("GetRandomSquare request failed!");

      vector2d Pos;

      if(Borders)
	{
	  Pos.X = Borders->X1 + RAND() % (Borders->X2 - Borders->X1 + 1);
	  Pos.Y = Borders->Y1 + RAND() % (Borders->Y2 - Borders->Y1 + 1);
	}
      else
	{
	  Pos.X = 1 + RAND() % (XSize - 2);
	  Pos.Y = 1 + RAND() % (YSize - 2);
	}

      if((!Map[Pos.X][Pos.Y]->IsWalkable(Char) != (Flags & NOT_WALKABLE))
	 || (!Map[Pos.X][Pos.Y]->GetCharacter() != !(Flags & HAS_CHARACTER))
	 || ((Flags & ATTACHABLE) && (FlagMap[Pos.X][Pos.Y] & FORBIDDEN)))
	continue;

      uchar RoomFlags = Flags & (IN_ROOM|NOT_IN_ROOM);

      if((RoomFlags == IN_ROOM && !Map[Pos.X][Pos.Y]->GetRoom())
	 || (RoomFlags == NOT_IN_ROOM && Map[Pos.X][Pos.Y]->GetRoom()))
	continue;

      return Pos;
    }
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

ushort level::GetLOSModifier() const
{
  return *LevelScript->GetLOSModifier();
}

ushort level::CalculateMinimumEmitationRadius(ulong E) const
{
  ulong A = *LevelScript->GetAmbientLight();
  ushort RedRadius = ushort(sqrt(float(GetRed24(E) << 7) / Max<uchar>(GetRed24(A), LIGHT_BORDER)));
  ushort GreenRadius = ushort(sqrt(float(GetGreen24(E) << 7) / Max<uchar>(GetGreen24(A), LIGHT_BORDER)));
  ushort BlueRadius = ushort(sqrt(float(GetBlue24(E) << 7) / Max<uchar>(GetBlue24(A), LIGHT_BORDER)));
  return Max(RedRadius, GreenRadius, BlueRadius);
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
  if(!GetSquare(Pos)->CanBeSeenByPlayer())
    ADD_MESSAGE("You hear an explosion.");

  ushort EmitChange = Min(150 + Strength, 255);
  GetLSquare(Pos)->SetTemporaryEmitation(MakeRGB24(EmitChange, EmitChange, EmitChange));
  static ushort StrengthLimit[6] = { 500, 250, 100, 50, 25, 10 };
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
	igraph::GetSymbolGraphic()->MaskedBlit(DOUBLE_BUFFER, PicPos, BPos, SizeVect, configuration::GetContrastLuminance());
      else
	{
	  bitmap ExplosionPic(SizeVect.X, SizeVect.Y);
	  igraph::GetSymbolGraphic()->Blit(&ExplosionPic, PicPos, 0, 0, SizeVect, Flags);
	  ExplosionPic.MaskedBlit(DOUBLE_BUFFER, 0, 0, BPos, SizeVect, configuration::GetContrastLuminance());
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

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetBorder(), Pos, Radius);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      {
	ushort DistanceSquare = HypotSquare(Pos.X - x, Pos.Y - y);

	if(DistanceSquare <= RadiusSquare)
	  {
	    lsquare* Square = GetLSquare(x, y);
	    character* Char = Square->GetCharacter();
	    ushort Damage = Strength / (DistanceSquare + 1);
	    uchar DamageDirection = vector2d(x, y) == Pos ? RANDOM_DIR : game::CalculateRoughDirection(vector2d(x, y) - Pos);

	    if(Char && (HurtNeutrals || (Terrorist && Char->GetRelation(Terrorist) == HOSTILE)))
	      if(Char->IsPlayer())
		{
		  PlayerDamage = Damage;
		  PlayerHurt = true;
		}
	      else
		{
		  if(Terrorist)
		    Terrorist->GetTeam()->Hostility(Char->GetTeam());

		  Char->GetTorso()->SpillBlood((8 - Size + RAND() % (8 - Size)) / 2);
		  vector2d SpillDirection = vector2d(x, y) + game::GetMoveVector(DamageDirection);

		  if(IsValidPos(SpillDirection))
		    Char->GetTorso()->SpillBlood((8 - Size + RAND() % (8 - Size)) / 2, SpillDirection);

		  if(Char->CanBeSeenByPlayer())
		    ADD_MESSAGE("%s is hit by the explosion.", Char->CHAR_NAME(DEFINITE));

		  Char->ReceiveDamage(Terrorist, Damage / 2, FIRE, ALL, DamageDirection, true, false, false, false);
		  Char->ReceiveDamage(Terrorist, Damage / 2, PHYSICAL_DAMAGE, ALL, DamageDirection, true, false, false, false);
		  Char->CheckDeath(DeathMsg, Terrorist);
		}

	    Square->GetStack()->ReceiveDamage(Terrorist, Damage / 2, FIRE);
	    Square->GetStack()->ReceiveDamage(Terrorist, Damage / 2, PHYSICAL_DAMAGE);

	    if(Damage >= 20 && Square->GetOLTerrain()->CanBeDug() && Square->GetOLTerrain()->GetMainMaterial()->GetStrengthValue() < 100)
	      {
		room* Room = Square->GetRoomClass();

		if(Room)
		  Room->DestroyTerrain(Terrorist, Square->GetOLTerrain());

		Square->GetOLTerrain()->Break();
	      }
	  }
      }

  if(PlayerHurt)
    {
      uchar DamageDirection = game::GetPlayer()->GetPos() == Pos ? RANDOM_DIR : game::CalculateRoughDirection(game::GetPlayer()->GetPos() - Pos);

      if(Terrorist)
	Terrorist->GetTeam()->Hostility(game::GetPlayer()->GetTeam());

      game::GetPlayer()->GetTorso()->SpillBlood((8 - Size + RAND() % (8 - Size)) / 2);

      if(IsValidPos(game::GetPlayer()->GetPos() + game::GetMoveVector(DamageDirection)))
	game::GetPlayer()->GetTorso()->SpillBlood((8 - Size + RAND() % (8 - Size)) / 2, game::GetPlayer()->GetPos() + game::GetMoveVector(DamageDirection));

      ADD_MESSAGE("You are hit by the explosion!");
      game::GetPlayer()->ReceiveDamage(Terrorist, PlayerDamage / 2, FIRE, ALL, DamageDirection, true, false, false, false);
      game::GetPlayer()->ReceiveDamage(Terrorist, PlayerDamage / 2, PHYSICAL_DAMAGE, ALL, DamageDirection, true, false, false, false);
      game::GetPlayer()->CheckDeath(DeathMsg, Terrorist);
    }
}

bool level::CollectCreatures(std::vector<character*>& CharacterArray, character* Leader, bool AllowHostiles)
{
  if(!AllowHostiles)
    for(ushort c = 0; c < game::GetTeams(); ++c)
      if(Leader->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
	for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	  if((*i)->IsEnabled() && Leader->CanBeSeenBy(*i) && Leader->GetSquareUnder()->CanBeSeenBy(*i, true))
	    {
	      ADD_MESSAGE("You can't escape when there are hostile creatures nearby.");
	      return false;
	    }

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(game::GetTeam(c) == Leader->GetTeam() || Leader->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled() && *i != Leader && Leader->CanBeSeenBy(*i) && Leader->GetSquareUnder()->CanBeSeenBy(*i, true))
	  {
	    if((*i)->GetAction() && (*i)->GetAction()->IsVoluntary())
	      (*i)->GetAction()->Terminate(false);

	    if(!(*i)->GetAction())
	      {
		ADD_MESSAGE("%s follows you.", (*i)->CHAR_NAME(DEFINITE));
		CharacterArray.push_back(*i);
		Leader->GetLevelUnder()->RemoveCharacter((*i)->GetPos());
	      }
	  }

  return true;
}

void level::Draw() const
{
  if(!game::SeeWholeMapCheatIsActive())
    {
      for(ushort x = game::GetCamera().X; x < XSize && x < game::GetCamera().X + game::GetScreenSize().X; ++x)
	for(ushort y = game::GetCamera().Y; y < YSize && y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
	  if(Map[x][y]->GetLastSeen() == game::GetLOSTurns())
	    Map[x][y]->Draw();
	  else
	    Map[x][y]->DrawMemorized();
    }
  else
    {
      for(ushort x = game::GetCamera().X; x < XSize && x < game::GetCamera().X + game::GetScreenSize().X; ++x)
	for(ushort y = game::GetCamera().Y; y < YSize && y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
	  Map[x][y]->Draw();
    }
}

lsquare* level::GetNeighbourLSquare(vector2d Pos, ushort Index) const
{
  Pos += game::GetMoveVector(Index);

  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize)
    return Map[Pos.X][Pos.Y];
  else
    return 0;
}

vector2d level::GetEntryPos(const character* Char, uchar Index) const
{
  std::map<uchar, vector2d>::const_iterator i = EntryMap.find(Index);
  return i == EntryMap.end() ? GetRandomSquare(Char) : i->second;
}

void level::GenerateRectangularRoom(std::vector<vector2d>& OKForDoor, std::vector<vector2d>& Inside, std::vector<vector2d>& Border, roomscript* RoomScript, room* RoomClass, vector2d Pos, vector2d Size)
{
  contentscript<glterrain>* GTerrain = RoomScript->GetWallSquare()->GetGTerrain();
  contentscript<olterrain>* OTerrain = RoomScript->GetWallSquare()->GetOTerrain();
  uchar Room = RoomClass->GetIndex();
  ulong Counter = 0;
  uchar* ScriptDivineMaster = RoomScript->GetDivineMaster(false);
  uchar DivineMaster = ScriptDivineMaster ? *ScriptDivineMaster : 0;
  bool AllowLanterns = *RoomScript->GetGenerateLanterns();
  ushort x, y;
  uchar Shape = *RoomScript->GetShape();

  if(Shape == ROUND_CORNERS && (Size.X < 5 || Size.Y < 5)) /* No weird shapes this way. */
    Shape = RECTANGLE;

  for(x = Pos.X; x < Pos.X + Size.X; ++x, Counter += 2)
    {
      if(Shape == ROUND_CORNERS)
	{
	  if(x == Pos.X)
	    {
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x + 1, Pos.Y + 1, Room, DivineMaster);
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x + 1, Pos.Y + Size.Y - 2, Room, DivineMaster);
	      Border.push_back(vector2d(x + 1, Pos.Y + 1));
	      Border.push_back(vector2d(x + 1, Pos.Y + Size.Y - 2));
	      continue;
	    }
	  else if(x == Pos.X + Size.X - 1)
	    {
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x - 1, Pos.Y + 1, Room, DivineMaster);
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x - 1, Pos.Y + Size.Y - 2, Room, DivineMaster);
	      Border.push_back(vector2d(x - 1, Pos.Y + 1));
	      Border.push_back(vector2d(x - 1, Pos.Y + Size.Y - 2));
	      continue;
	    }
	}

      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, Pos.Y, Room, DivineMaster);
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, Pos.Y + Size.Y - 1, Room, DivineMaster);

      if((Shape == RECTANGLE && x != Pos.X && x != Pos.X + Size.X - 1)
      || (Shape == ROUND_CORNERS && x > Pos.X + 1 && x < Pos.X + Size.X - 2))
	{
	  OKForDoor.push_back(vector2d(x,Pos.Y));
	  OKForDoor.push_back(vector2d(x,Pos.Y + Size.Y - 1));
	  if(AllowLanterns)
	    {
	      GenerateLanterns(x, Pos.Y, DOWN);
	      GenerateLanterns(x, Pos.Y + Size.Y - 1, UP);
	    }
	}
      Border.push_back(vector2d(x, Pos.Y));
      Border.push_back(vector2d(x, Pos.Y + Size.Y - 1));
    }

  game::BusyAnimation();

  for(y = Pos.Y + 1; y < Pos.Y + Size.Y - 1; ++y, Counter += 2)
    {
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), Pos.X, y, Room, DivineMaster);
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), Pos.X + Size.X - 1, y, Room, DivineMaster);

      if(Shape == ROUND_CORNERS && y > Pos.Y + 1 && y < Pos.Y + Size.Y - 2)
	{
	  OKForDoor.push_back(vector2d(Pos.X, y));
	  OKForDoor.push_back(vector2d(Pos.X + Size.X - 1, y));
	}

      Border.push_back(vector2d(Pos.X, y));
      Border.push_back(vector2d(Pos.X + Size.X - 1, y));

      if(AllowLanterns)
	{
	  GenerateLanterns(Pos.X, y, RIGHT);
	  GenerateLanterns(Pos.X + Size.X - 1, y, LEFT);
	}
    }

  GTerrain = RoomScript->GetFloorSquare()->GetGTerrain();
  OTerrain = RoomScript->GetFloorSquare()->GetOTerrain();
  Counter = 0;

  for(x = Pos.X + 1; x < Pos.X + Size.X - 1; ++x)
    for(y = Pos.Y + 1; y < Pos.Y + Size.Y - 1; ++y, ++Counter)
      {
	/* if not in the corner */

	if(!(Shape == ROUND_CORNERS && (x == Pos.X + 1 || x == Pos.X + Size.X - 2) && (y == Pos.Y + 1 || y == Pos.Y + Size.Y - 2)))
	  {
	    CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, y, Room, DivineMaster);
	    Inside.push_back(vector2d(x,y));
	  }
      }
}

void level::Reveal()
{
  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      {
	if(!Map[x][y]->LastSeen)
	  {
	    Map[x][y]->Memorized = new bitmap(16, 16);
	    Map[x][y]->LastSeen = 1;
	  }

	Map[x][y]->MemorizedUpdateRequested = true;
	Map[x][y]->DescriptionChanged = true;
	Map[x][y]->UpdateMemorized();
	Map[x][y]->UpdateMemorizedDescription();
      }
}
