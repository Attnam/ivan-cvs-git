/* Compiled through levelset.cpp */

#define FORBIDDEN 1
#define ON_POSSIBLE_ROUTE 2
#define STILL_ON_POSSIBLE_ROUTE 4
#define PREFERRED 8

level::level() : Room(1, static_cast<room*>(0)) { }
void level::SetRoom(ushort Index, room* What) { Room[Index] = What; }

level::~level()
{
  for(ushort c = 0; c < Room.size(); ++c)
    delete Room[c];
}

void level::ExpandPossibleRoute(ushort OrigoX, ushort OrigoY, ushort TargetX, ushort TargetY, bool XMode)
{
#define CHECK(x, y) !(FlagMap[x][y] & (ON_POSSIBLE_ROUTE|FORBIDDEN))

#define CALL_EXPAND(x, y)\
  {\
    ExpandPossibleRoute(x, y, TargetX, TargetY, XMode);\
    \
    if(FlagMap[TargetX][TargetY] & ON_POSSIBLE_ROUTE)\
      return;\
  }

  FlagMap[OrigoX][OrigoY] |= ON_POSSIBLE_ROUTE;

  if(XMode)
    {
      if(TargetX < OrigoX)
	if(CHECK(OrigoX - 1, OrigoY))
	  CALL_EXPAND(OrigoX - 1, OrigoY);

      if(TargetX > OrigoX)
	if(CHECK(OrigoX + 1, OrigoY))
	  CALL_EXPAND(OrigoX + 1, OrigoY);

      if(TargetY < OrigoY)
	if(CHECK(OrigoX, OrigoY - 1))
	  CALL_EXPAND(OrigoX, OrigoY - 1);

      if(TargetY > OrigoY)
	if(CHECK(OrigoX, OrigoY + 1))
	  CALL_EXPAND(OrigoX, OrigoY + 1);

      if(TargetX <= OrigoX)
	if(OrigoX < XSize - 2 && CHECK(OrigoX + 1, OrigoY))
	  CALL_EXPAND(OrigoX + 1, OrigoY);

      if(TargetX >= OrigoX)
	if(OrigoX > 1 && CHECK(OrigoX - 1, OrigoY))
	  CALL_EXPAND(OrigoX - 1, OrigoY);

      if(TargetY <= OrigoY)
	if(OrigoY < YSize - 2 && CHECK(OrigoX, OrigoY + 1))
	  CALL_EXPAND(OrigoX, OrigoY + 1);

      if(TargetY >= OrigoY)
	if(OrigoY > 1 && CHECK(OrigoX, OrigoY - 1))
	  CALL_EXPAND(OrigoX, OrigoY - 1);
    }
  else
    {
      if(TargetY < OrigoY)
	if(CHECK(OrigoX, OrigoY - 1))
	  CALL_EXPAND(OrigoX, OrigoY - 1);

      if(TargetY > OrigoY)
	if(CHECK(OrigoX, OrigoY + 1))
	  CALL_EXPAND(OrigoX, OrigoY + 1);

      if(TargetX < OrigoX)
	if(CHECK(OrigoX - 1, OrigoY))
	  CALL_EXPAND(OrigoX - 1, OrigoY);

      if(TargetX > OrigoX)
	if(CHECK(OrigoX + 1, OrigoY))
	  CALL_EXPAND(OrigoX + 1, OrigoY);

      if(TargetY <= OrigoY)
	if(OrigoY < YSize - 2 && CHECK(OrigoX, OrigoY + 1))
	  CALL_EXPAND(OrigoX, OrigoY + 1);

      if(TargetY >= OrigoY)
	if(OrigoY > 1 && CHECK(OrigoX, OrigoY - 1))
	  CALL_EXPAND(OrigoX, OrigoY - 1);

      if(TargetX <= OrigoX)
	if(OrigoX < XSize - 2 && CHECK(OrigoX + 1, OrigoY))
	  CALL_EXPAND(OrigoX + 1, OrigoY);

      if(TargetX >= OrigoX)
	if(OrigoX > 1 && CHECK(OrigoX - 1, OrigoY))
	  CALL_EXPAND(OrigoX - 1, OrigoY);
    }

#undef CHECK

#undef CALL_EXPAND
}

void level::ExpandStillPossibleRoute(ushort OrigoX, ushort OrigoY, ushort TargetX, ushort TargetY, bool XMode)
{
#define CHECK(x, y) (FlagMap[x][y] & (STILL_ON_POSSIBLE_ROUTE|ON_POSSIBLE_ROUTE)) == ON_POSSIBLE_ROUTE

#define CALL_EXPAND(x, y) \
  {\
    ExpandStillPossibleRoute(x, y, TargetX, TargetY, XMode);\
    \
    if(FlagMap[TargetX][TargetY] & STILL_ON_POSSIBLE_ROUTE) \
      return;\
  }

  FlagMap[OrigoX][OrigoY] |= STILL_ON_POSSIBLE_ROUTE;

  if(XMode)
    {
      if(TargetX < OrigoX)
	if(CHECK(OrigoX - 1, OrigoY))
	  CALL_EXPAND(OrigoX - 1, OrigoY);

      if(TargetX > OrigoX)
	if(CHECK(OrigoX + 1, OrigoY))
	  CALL_EXPAND(OrigoX + 1, OrigoY);

      if(TargetY < OrigoY)
	if(CHECK(OrigoX, OrigoY - 1))
	  CALL_EXPAND(OrigoX, OrigoY - 1);

      if(TargetY > OrigoY)
	if(CHECK(OrigoX, OrigoY + 1))
	  CALL_EXPAND(OrigoX, OrigoY + 1);

      if(TargetX <= OrigoX)
	if(OrigoX < XSize - 2 && CHECK(OrigoX + 1, OrigoY))
	  CALL_EXPAND(OrigoX + 1, OrigoY);

      if(TargetX >= OrigoX)
	if(OrigoX > 1 && CHECK(OrigoX - 1, OrigoY))
	  CALL_EXPAND(OrigoX - 1, OrigoY);

      if(TargetY <= OrigoY)
	if(OrigoY < YSize - 2 && CHECK(OrigoX, OrigoY + 1))
	  CALL_EXPAND(OrigoX, OrigoY + 1);

      if(TargetY >= OrigoY)
	if(OrigoY > 1 && CHECK(OrigoX, OrigoY - 1))
	  CALL_EXPAND(OrigoX, OrigoY - 1);
    }
  else
    {
      if(TargetY < OrigoY)
	if(CHECK(OrigoX, OrigoY - 1))
	  CALL_EXPAND(OrigoX, OrigoY - 1);

      if(TargetY > OrigoY)
	if(CHECK(OrigoX, OrigoY + 1))
	  CALL_EXPAND(OrigoX, OrigoY + 1);

      if(TargetX < OrigoX)
	if(CHECK(OrigoX - 1, OrigoY))
	  CALL_EXPAND(OrigoX - 1, OrigoY);

      if(TargetX > OrigoX)
	if(CHECK(OrigoX + 1, OrigoY))
	  CALL_EXPAND(OrigoX + 1, OrigoY);

      if(TargetY <= OrigoY)
	if(OrigoY < YSize - 2 && CHECK(OrigoX, OrigoY + 1))
	  CALL_EXPAND(OrigoX, OrigoY + 1);

      if(TargetY >= OrigoY)
	if(OrigoY > 1 && CHECK(OrigoX, OrigoY - 1))
	  CALL_EXPAND(OrigoX, OrigoY - 1);

      if(TargetX <= OrigoX)
	if(OrigoX < XSize - 2 && CHECK(OrigoX + 1, OrigoY))
	  CALL_EXPAND(OrigoX + 1, OrigoY);

      if(TargetX >= OrigoX)
	if(OrigoX > 1 && CHECK(OrigoX - 1, OrigoY))
	  CALL_EXPAND(OrigoX - 1, OrigoY);
    }

#undef CHECK

#undef CALL_EXPAND
}

void level::GenerateTunnel(ushort FromX, ushort FromY, ushort TargetX, ushort TargetY, bool XMode)
{
  FlagMap[FromX][FromY] |= ON_POSSIBLE_ROUTE;
  ExpandPossibleRoute(FromX, FromY, TargetX, TargetY, XMode);

  if(FlagMap[TargetX][TargetY] & ON_POSSIBLE_ROUTE)
    for(ushort x = 0; x < XSize; ++x)
      for(ushort y = 0; y < YSize; ++y)
	if((FlagMap[x][y] & (ON_POSSIBLE_ROUTE|PREFERRED)) == ON_POSSIBLE_ROUTE && !(x == FromX && y == FromY) && !(x == TargetX && y == TargetY))
	  {
	    FlagMap[x][y] &= ~ON_POSSIBLE_ROUTE;
	    FlagMap[FromX][FromY] |= STILL_ON_POSSIBLE_ROUTE;
	    ExpandStillPossibleRoute(FromX, FromY, TargetX, TargetY, XMode);

	    if(!(FlagMap[TargetX][TargetY] & STILL_ON_POSSIBLE_ROUTE))
	      {
		FlagMap[x][y] |= ON_POSSIBLE_ROUTE|PREFERRED;
		Map[x][y]->ChangeOLTerrain(0);
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
  const festring* Msg = LevelScript->GetLevelMessage();

  if(Msg)
    LevelMessage = *Msg;

  if(*LevelScript->GenerateMonsters())
    {
      MonsterGenerationInterval = *LevelScript->GetMonsterGenerationIntervalBase() + *LevelScript->GetMonsterGenerationIntervalDelta() * Index;
      IdealPopulation = *LevelScript->GetMonsterAmountBase() + *LevelScript->GetMonsterAmountDelta() * Index;
      Difficulty = *LevelScript->GetDifficultyBase() + *LevelScript->GetDifficultyDelta() * Index;
    }

  const contentscript<glterrain>* GTerrain = LevelScript->GetFillSquare()->GetGTerrain();
  const contentscript<olterrain>* OTerrain = LevelScript->GetFillSquare()->GetOTerrain();
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
  ushort Rooms = LevelScript->GetRooms()->Randomize();
  const std::list<roomscript>& RoomList = LevelScript->GetRoom();
  std::list<roomscript>::const_iterator Iterator = RoomList.begin();

  for(c = 0; c < Rooms; ++c)
    {
      game::BusyAnimation();

      if(c < RoomList.size())
	{
	  ushort i;

	  for(i = 0; i < 1000; ++i)
	    if(MakeRoom(&*Iterator))
	      break;

	  if(i == 1000)
	    ABORT("Failed to place special room #%d!", c);

	  ++Iterator;
	}
      else
	{
	  const roomscript* RoomScript = LevelScript->GetRoomDefault();

	  for(ushort i = 0; i < 50; ++i)
	    if(MakeRoom(RoomScript))
	      break;
	}
    }

  game::BusyAnimation();

  if(!*LevelScript->IgnoreDefaultSpecialSquares())
    {
      /* Gum solution */

      const levelscript* LevelBase = static_cast<const levelscript*>(LevelScript->GetBase());

      if(LevelBase)
	{
	  const std::list<squarescript>& Square = LevelBase->GetSquare();

	  for(std::list<squarescript>::const_iterator i = Square.begin(); i != Square.end(); ++i)
	    {
	      game::BusyAnimation();
	      ApplyLSquareScript(&*i);
	    }
	}
    }

  const std::list<squarescript>& Square = LevelScript->GetSquare();

  for(std::list<squarescript>::const_iterator i = Square.begin(); i != Square.end(); ++i)
    {
      game::BusyAnimation();
      ApplyLSquareScript(&*i);
    }

  CreateItems(LevelScript->GetItems()->Randomize());
}

void level::ApplyLSquareScript(const squarescript* Script)
{
  const interval* ScriptTimes = Script->GetTimes();
  ushort Times = ScriptTimes ? ScriptTimes->Randomize() : 1;

  for(ushort c = 0; c < Times; ++c)
    {
      vector2d Pos;

      if(Script->GetPosition()->GetRandom())
	Pos = GetRandomSquare(0, *Script->GetPosition()->GetFlags(), Script->GetPosition()->GetBorders());
      else
	Pos = *Script->GetPosition()->GetVector();

      Map[Pos.X][Pos.Y]->ApplyScript(Script, 0);
    }
}

void level::AttachPos(ushort WhatX, ushort WhatY)
{
  ushort PosX = 1 + RAND() % (XSize - 2);
  ushort PosY = 1 + RAND() % (YSize - 2);

  while(!(FlagMap[PosX][PosY] & PREFERRED))
    {
      PosX = 1 + RAND() % (XSize - 2);
      PosY = 1 + RAND() % (YSize - 2);
    }

  FlagMap[WhatX][WhatY] &= ~FORBIDDEN;
  FlagMap[WhatX][WhatY] |= PREFERRED;
  GenerateTunnel(WhatX, WhatY, PosX, PosY, RAND() & 1);
  FlagMap[WhatX][WhatY] |= FORBIDDEN;
  FlagMap[WhatX][WhatY] &= ~PREFERRED;
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

bool level::MakeRoom(const roomscript* RoomScript)
{
  game::BusyAnimation();
  vector2d Pos = RoomScript->GetPos()->Randomize();
  vector2d Size = RoomScript->GetSize()->Randomize();
  ushort x, y;

  if(Pos.X + Size.X > XSize - 2)
    return false;

  if(Pos.Y + Size.Y > YSize - 2)
    return false;

  for(x = Pos.X - 1; x <= Pos.X + Size.X; ++x)
    for(y = Pos.Y - 1; y <= Pos.Y + Size.Y; ++y)
      if(FlagMap[x][y] & FORBIDDEN || FlagMap[x][y] & PREFERRED)
	return false;

  room* RoomClass = protocontainer<room>::GetProto(*RoomScript->GetType())->Clone();
  RoomClass->SetPos(Pos);
  RoomClass->SetSize(Size);
  AddRoom(RoomClass);
  RoomClass->SetDivineMaster(*RoomScript->GetDivineMaster());
  game::BusyAnimation();
  std::vector<vector2d> OKForDoor, Inside, Border;

  GenerateRectangularRoom(OKForDoor, Inside, Border, RoomScript, RoomClass, Pos, Size);
  game::BusyAnimation();

  if(*RoomScript->GenerateFountains() && !(RAND() % 10))
    GetLSquare(Inside[RAND() % Inside.size()])->ChangeOLTerrain(new fountain);

  if(*RoomScript->AltarPossible() && !(RAND() % 5))
    {
      uchar Owner = 1 + RAND() % GODS;
      GetLSquare(Inside[RAND() % Inside.size()])->ChangeOLTerrain(new altar(Owner));
      RoomClass->SetDivineMaster(Owner);
    }

  if(*RoomScript->GenerateTunnel() && !Door.empty())
    {
      game::BusyAnimation();
      vector2d OutsideDoorPos = Door[RAND() % Door.size()]; // An other room

      if(OKForDoor.empty())
	ABORT("The Doors - You are strange.");

      vector2d InsideDoorPos = OKForDoor[RAND() % OKForDoor.size()]; // this door
      olterrain* Door = RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate(); //Bug! Wrong room!

      if(Door && !(RAND() % 5) && *RoomScript->AllowLockedDoors())
	{
	  if(*RoomScript->AllowBoobyTrappedDoors() && !(RAND() % 5))
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

      if(Door && !(RAND() % 5) && *RoomScript->AllowLockedDoors())
	{
	  if(*RoomScript->AllowBoobyTrappedDoors() && !(RAND() % 5))
	    Door->CreateBoobyTrap();

	  Door->Lock();
	}

      Map[InsideDoorPos.X][InsideDoorPos.Y]->ChangeLTerrain(RoomScript->GetDoorSquare()->GetGTerrain()->Instantiate(), Door);
      Map[InsideDoorPos.X][InsideDoorPos.Y]->Clean();
      GenerateTunnel(InsideDoorPos.X, InsideDoorPos.Y, OutsideDoorPos.X, OutsideDoorPos.Y, RAND() & 1);
      FlagMap[OutsideDoorPos.X][OutsideDoorPos.Y] |= FORBIDDEN;
      FlagMap[OutsideDoorPos.X][OutsideDoorPos.Y] &= ~PREFERRED;
      FlagMap[InsideDoorPos.X][InsideDoorPos.Y] |= FORBIDDEN;
      FlagMap[InsideDoorPos.X][InsideDoorPos.Y] &= ~PREFERRED;
    }

  if(*RoomScript->GenerateDoor())
    {
      game::BusyAnimation();
      vector2d DoorPos;

      if(OKForDoor.empty())
	ABORT("The Doors - This thing has been broken.");

      DoorPos = OKForDoor[RAND() % OKForDoor.size()];
      Door.push_back(DoorPos);

      if(!*RoomScript->GenerateTunnel())
	{
	  Map[DoorPos.X][DoorPos.Y]->ChangeLTerrain(RoomScript->GetDoorSquare()->GetGTerrain()->Instantiate(), RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate());
	  Map[DoorPos.X][DoorPos.Y]->Clean();
	}
    }

  const charactercontentmap* CharacterMap = RoomScript->GetCharacterMap();

  if(CharacterMap)
    {
      vector2d CharPos(Pos + *CharacterMap->GetPos());
      const contentscript<character>* CharacterScript;

      for(ushort x = 0; x < CharacterMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < CharacterMap->GetSize()->Y; ++y)
	    if(IsValidScript(CharacterScript = CharacterMap->GetContentScript(x, y)))
	      {
		character* Char = CharacterScript->Instantiate();

		if(!Char->GetTeam())
		  Char->SetTeam(game::GetTeam(*LevelScript->GetTeamDefault()));

		Map[CharPos.X + x][CharPos.Y + y]->AddCharacter(Char);
		Char->CreateHomeData();
		const bool* IsMaster = CharacterScript->IsMaster();

		if(IsMaster && *IsMaster)
		  RoomClass->SetMasterID(Char->GetID());
	      }
	}
    }

  const itemcontentmap* ItemMap = RoomScript->GetItemMap();

  if(ItemMap)
    {
      vector2d ItemPos(Pos + *ItemMap->GetPos());
      const std::list<contentscript<item> >* ItemScript;

      for(ushort x = 0; x < ItemMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < ItemMap->GetSize()->Y; ++y)
	    if(IsValidScript(ItemScript = ItemMap->GetContentScript(x, y)))
	      for(std::list<contentscript<item> >::const_iterator i = ItemScript->begin(); i != ItemScript->end(); ++i)
		{
		  stack* Stack;
		  item* Item = i->Instantiate();

		  if(Item)
		    {
		      const uchar* SideStackIndex = i->GetSideStackIndex();

		      if(!SideStackIndex)
			Stack = Map[ItemPos.X + x][ItemPos.Y + y]->GetStack();
		      else
			{
			  Item->SignalSquarePositionChange(*SideStackIndex);
			  Stack = Map[ItemPos.X + x][ItemPos.Y + y]->GetSideStack(*SideStackIndex);
			}

		      Stack->AddItem(Item);
		      Item->SpecialGenerationHandler();
		    }
		}
	}
    }

  const glterraincontentmap* GTerrainMap = RoomScript->GetGTerrainMap();

  if(GTerrainMap)
    {
      vector2d GTerrainPos(Pos + *GTerrainMap->GetPos());
      const contentscript<glterrain>* GTerrainScript;

      for(ushort x = 0; x < GTerrainMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < GTerrainMap->GetSize()->Y; ++y)
	    if(IsValidScript(GTerrainScript = GTerrainMap->GetContentScript(x, y)))
	      Map[GTerrainPos.X + x][GTerrainPos.Y + y]->ChangeGLTerrain(GTerrainScript->Instantiate());
	}
    }

  const olterraincontentmap* OTerrainMap = RoomScript->GetOTerrainMap();

  if(OTerrainMap)
    {
      vector2d OTerrainPos(Pos + *OTerrainMap->GetPos());
      const contentscript<olterrain>* OTerrainScript;

      for(ushort x = 0; x < OTerrainMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < OTerrainMap->GetSize()->Y; ++y)
	    if(IsValidScript(OTerrainScript = OTerrainMap->GetContentScript(x, y)))
	      {
		olterrain* Terrain = OTerrainScript->Instantiate();
		Map[OTerrainPos.X + x][OTerrainPos.Y + y]->ChangeOLTerrain(Terrain);
	      }
	}
    }

  const std::list<squarescript> Square = RoomScript->GetSquare();

  for(std::list<squarescript>::const_iterator i = Square.begin(); i != Square.end(); ++i)
    {
      game::BusyAnimation();
      const squarescript* Script = &*i;
      const interval* ScriptTimes = Script->GetTimes();
      ushort Times = ScriptTimes ? ScriptTimes->Randomize() : 1;

      for(ushort t = 0; t < Times; ++t)
	{
	  vector2d SquarePos;

	  if(Script->GetPosition()->GetRandom())
	    {
	      const rect* ScriptBorders = Script->GetPosition()->GetBorders();
	      rect Borders = ScriptBorders ? *ScriptBorders + Pos : rect(Pos, Pos + Size - vector2d(1, 1));
	      SquarePos = GetRandomSquare(0, *Script->GetPosition()->GetFlags(), &Borders);
	    }
	  else
	    SquarePos = Pos + *Script->GetPosition()->GetVector();

	  Map[SquarePos.X][SquarePos.Y]->ApplyScript(Script, RoomClass);
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

void level::CreateRoomSquare(glterrain* GLTerrain, olterrain* OLTerrain, ushort X, ushort Y, uchar Room) const
{
  Map[X][Y]->ChangeLTerrain(GLTerrain, OLTerrain);
  FlagMap[X][Y] |= FORBIDDEN;
  Map[X][Y]->SetRoomIndex(Room);
}

void level::GenerateMonsters()
{
  if(*LevelScript->GenerateMonsters()
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
			
	  if(!ConsiderPlayer || abs(short(Pos.X) - PLAYER->GetPos().X) > 6 || abs(short(Pos.Y) - PLAYER->GetPos().Y) > 6)
	    break;
	}

      if(!(Pos.X == 0 && Pos.Y == 0))
	Map[Pos.X][Pos.Y]->AddCharacter(Char);
    }
}

/* Example of the usage: GetRandomSquare() gives out a random walkable square */

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

      if((RoomFlags == IN_ROOM && !Map[Pos.X][Pos.Y]->GetRoomIndex())
	 || (RoomFlags == NOT_IN_ROOM && Map[Pos.X][Pos.Y]->GetRoomIndex()))
	continue;

      return Pos;
    }
}

void level::ParticleTrail(vector2d StartPos, vector2d EndPos)
{
  if(StartPos.X != EndPos.X && StartPos.Y != EndPos.Y)
    ABORT("666th rule of thermodynamics - Particles don't move the way you want them to move.");
}

bool level::IsOnGround() const
{
  return *LevelScript->IsOnGround();
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

void level::Explosion(character* Terrorist, const festring& DeathMsg, vector2d Pos, ushort Strength, bool HurtNeutrals)
{
  static ushort StrengthLimit[6] = { 500, 250, 100, 50, 25, 10 };
  ushort c;
  uchar Size = 6;

  for(c = 0; c < 6; ++c)
    if(Strength >= StrengthLimit[c])
      {
        Size = c;
        break;
      }

  PlayerHurt.resize(PlayerHurt.size() + 1);
  explosion* E = new explosion;
  E->Terrorist = Terrorist;
  E->DeathMsg = DeathMsg;
  E->Pos = Pos;
  E->ID = game::CreateNewExplosionID();
  E->Strength = Strength;
  E->RadiusSquare = (8 - Size) * (8 - Size);
  E->Size = Size;
  E->HurtNeutrals = HurtNeutrals;

  ExplosionQueue.push_back(E);

  if(ExplosionQueue.size() == 1)
    {
      ushort Explosions = 0;

      while(Explosions != ExplosionQueue.size())
	{
	  for(c = Explosions; c != ExplosionQueue.size(); c = TriggerExplosions(c));
	  ushort NewExplosions = c;

	  for(c = Explosions; c < NewExplosions; ++c)
	    if(PlayerHurt[c])
	      PLAYER->GetHitByExplosion(ExplosionQueue[c], ExplosionQueue[c]->Strength / ((PLAYER->GetPos() - ExplosionQueue[c]->Pos).GetLengthSquare() + 1));

	  Explosions = NewExplosions;
	}

      for(ushort c = 0; c < ExplosionQueue.size(); ++c)
	delete ExplosionQueue[c];

      ExplosionQueue.clear();
      PlayerHurt.clear();
    }
}

bool level::DrawExplosion(const explosion* Explosion) const
{
  static vector2d StrengthPicPos[7] = { vector2d(176, 176), vector2d(0, 144), vector2d(256, 32), vector2d(144, 32), vector2d(64, 32), vector2d(16, 32),vector2d(0, 32) };
  vector2d BPos = game::CalculateScreenCoordinates(Explosion->Pos) - vector2d((6 - Explosion->Size) << 4, (6 - Explosion->Size) << 4);
  vector2d SizeVect(16 + ((6 - Explosion->Size) << 5), 16 + ((6 - Explosion->Size) << 5));
  vector2d OldSizeVect = SizeVect;
  vector2d PicPos = StrengthPicPos[Explosion->Size];

  if(short(BPos.X) < 0)
    if(short(BPos.X) + SizeVect.X <= 0)
      return false;
    else
      {
	PicPos.X -= BPos.X;
	SizeVect.X += BPos.X;
	BPos.X = 0;
      }

  if(short(BPos.Y) < 0)
    if(short(BPos.Y) + SizeVect.Y <= 0)
      return false;
    else
      {
	PicPos.Y -= BPos.Y;
	SizeVect.Y += BPos.Y;
	BPos.Y = 0;
      }

  if(BPos.X >= RES_X || BPos.Y >= RES_Y)
    return false;

  if(BPos.X + SizeVect.X > RES_X)
    SizeVect.X = RES_X - BPos.X;

  if(BPos.Y + SizeVect.Y > RES_Y)
    SizeVect.Y = RES_Y - BPos.Y;

  uchar Flags = RAND() & 7;

  if(!Flags || SizeVect != OldSizeVect)
    igraph::GetSymbolGraphic()->MaskedBlit(DOUBLE_BUFFER, PicPos, BPos, SizeVect, configuration::GetContrastLuminance());
  else
    {
      bitmap ExplosionPic(SizeVect.X, SizeVect.Y);
      igraph::GetSymbolGraphic()->Blit(&ExplosionPic, PicPos, 0, 0, SizeVect, Flags);
      ExplosionPic.MaskedBlit(DOUBLE_BUFFER, 0, 0, BPos, SizeVect, configuration::GetContrastLuminance());
    }

  return true;
}

ushort level::TriggerExplosions(ushort MinIndex)
{
  ushort LastExplosion = ExplosionQueue.size();
  ushort NotSeen = 0;
  ushort c;

  for(c = MinIndex; c < LastExplosion; ++c)
    {
      ushort EmitChange = Min(150 + ExplosionQueue[c]->Strength, 255);
      GetLSquare(ExplosionQueue[c]->Pos)->SetTemporaryEmitation(MakeRGB24(EmitChange, EmitChange, EmitChange));

      if(!GetSquare(ExplosionQueue[c]->Pos)->CanBeSeenByPlayer())
	++NotSeen;
    }

  if(NotSeen)
    if(NotSeen == 1)
      ADD_MESSAGE("You hear an explosion.");
    else
      ADD_MESSAGE("You hear explosions.");

  game::DrawEverythingNoBlit();
  bool Drawn = false;

  for(c = MinIndex; c < LastExplosion; ++c)
    {
      GetLSquare(ExplosionQueue[c]->Pos)->SetTemporaryEmitation(0);

      if(DrawExplosion(ExplosionQueue[c]))
	Drawn = true;
    }

  if(Drawn)
    {
      graphics::BlitDBToScreen();
      game::GetCurrentArea()->SendNewDrawRequest();
      clock_t StartTime = clock();
      while(clock() - StartTime < 0.3f * CLOCKS_PER_SEC);
    }

  for(c = MinIndex; c < LastExplosion; ++c)
    {
      explosion* Explosion = ExplosionQueue[c];
      ushort Radius = 8 - Explosion->Size;
      game::SetCurrentExplosion(Explosion);
      game::SetPlayerWasHurtByExplosion(false);

      rect Rect;
      femath::CalculateEnvironmentRectangle(Rect, GetBorder(), Explosion->Pos, Radius);

      for(ushort x = Rect.X1; x <= Rect.X2; ++x)
	{
	  femath::DoLine(Explosion->Pos.X, Explosion->Pos.Y, x, Rect.Y1, game::ExplosionHandler);
	  femath::DoLine(Explosion->Pos.X, Explosion->Pos.Y, x, Rect.Y2, game::ExplosionHandler);
	}

      for(ushort y = Rect.Y1 + 1; y < Rect.Y2; ++y)
	{
	  femath::DoLine(Explosion->Pos.X, Explosion->Pos.Y, Rect.X1, y, game::ExplosionHandler);
	  femath::DoLine(Explosion->Pos.X, Explosion->Pos.Y, Rect.X2, y, game::ExplosionHandler);
	}

      PlayerHurt[c] = game::PlayerWasHurtByExplosion();

      if(GetLSquare(Explosion->Pos)->IsFlyable())
	GetLSquare(Explosion->Pos)->AddSmoke(new gas(SMOKE, 1000));
    }

  return LastExplosion;
}

bool level::CollectCreatures(std::vector<character*>& CharacterArray, character* Leader, bool AllowHostiles)
{
  if(!AllowHostiles)
    for(ushort c = 0; c < game::GetTeams(); ++c)
      if(Leader->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
	for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	  if((*i)->IsEnabled() && Leader->CanBeSeenBy(*i) && Leader->GetSquareUnder()->CanBeSeenBy(*i, true) && (*i)->CanFollow())
	    {
	      ADD_MESSAGE("You can't escape when there are hostile creatures nearby.");
	      return false;
	    }

  for(ushort c = 0; c < game::GetTeams(); ++c)
    if(game::GetTeam(c) == Leader->GetTeam() || Leader->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled() && *i != Leader && Leader->CanBeSeenBy(*i) && Leader->GetSquareUnder()->CanBeSeenBy(*i, true) && (*i)->CanFollow())
	  {
	    if((*i)->GetAction() && (*i)->GetAction()->IsVoluntary())
	      (*i)->GetAction()->Terminate(false);

	    if(!(*i)->GetAction())
	      {
		ADD_MESSAGE("%s follows you.", (*i)->CHAR_NAME(DEFINITE));
		CharacterArray.push_back(*i);
		Leader->GetLevel()->RemoveCharacter((*i)->GetPos());
	      }
	  }

  return true;
}

void level::Draw(bool AnimationDraw) const
{
  ushort XMin = game::GetCamera().X;
  ushort YMin = game::GetCamera().Y;
  ushort XMax = Min<ushort>(XSize, game::GetCamera().X + game::GetScreenXSize());
  ushort YMax = Min<ushort>(YSize, game::GetCamera().Y + game::GetScreenYSize());

  if(!game::GetSeeWholeMapCheatMode())
    {
      if(!AnimationDraw)
	{
	  for(ushort x = XMin; x < XMax; ++x)
	    {
	      lsquare** Square = &Map[x][YMin];

	      for(ushort y = YMin; y < YMax; ++y, ++Square)
		{
		  ulong LastSeen = (*Square)->LastSeen;

		  if(LastSeen == game::GetLOSTurns())
		    (*Square)->Draw();
		  else if((*Square)->NewDrawRequested || LastSeen == game::GetLOSTurns() - 1)
		    (*Square)->DrawMemorized();
		}
	    }
	}
      else
	{
	  for(ushort x = XMin; x < XMax; ++x)
	    {
	      lsquare** Square = &Map[x][YMin];

	      for(ushort y = YMin; y < YMax; ++y, ++Square)
		if((*Square)->LastSeen == game::GetLOSTurns())
		  (*Square)->Draw();
		else
		  (*Square)->DrawMemorizedCharacter();
	    }
	}
    }
  else
    {
      for(ushort x = XMin; x < XMax; ++x)
	{
	  lsquare** Square = &Map[x][YMin];

	  for(ushort y = YMin; y < YMax; ++y, ++Square)
	    (*Square)->Draw();
	}
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

void level::GenerateRectangularRoom(std::vector<vector2d>& OKForDoor, std::vector<vector2d>& Inside, std::vector<vector2d>& Border, const roomscript* RoomScript, room* RoomClass, vector2d Pos, vector2d Size)
{
  const contentscript<glterrain>* GTerrain = RoomScript->GetWallSquare()->GetGTerrain();
  const contentscript<olterrain>* OTerrain = RoomScript->GetWallSquare()->GetOTerrain();
  uchar Room = RoomClass->GetIndex();
  ulong Counter = 0;
  bool AllowLanterns = *RoomScript->GenerateLanterns();
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
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x + 1, Pos.Y + 1, Room);
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x + 1, Pos.Y + Size.Y - 2, Room);
	      Border.push_back(vector2d(x + 1, Pos.Y + 1));
	      Border.push_back(vector2d(x + 1, Pos.Y + Size.Y - 2));
	      continue;
	    }
	  else if(x == Pos.X + Size.X - 1)
	    {
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x - 1, Pos.Y + 1, Room);
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x - 1, Pos.Y + Size.Y - 2, Room);
	      Border.push_back(vector2d(x - 1, Pos.Y + 1));
	      Border.push_back(vector2d(x - 1, Pos.Y + Size.Y - 2));
	      continue;
	    }
	}

      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, Pos.Y, Room);
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, Pos.Y + Size.Y - 1, Room);

      if((Shape == RECTANGLE && x != Pos.X && x != Pos.X + Size.X - 1)
      || (Shape == ROUND_CORNERS && x > Pos.X + 1 && x < Pos.X + Size.X - 2))
	{
	  OKForDoor.push_back(vector2d(x, Pos.Y));
	  OKForDoor.push_back(vector2d(x, Pos.Y + Size.Y - 1));

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
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), Pos.X, y, Room);
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), Pos.X + Size.X - 1, y, Room);

      if(Shape == RECTANGLE || (Shape == ROUND_CORNERS && y != Pos.Y + 1 && y != Pos.Y + Size.Y - 2))
	{
	  OKForDoor.push_back(vector2d(Pos.X, y));
	  OKForDoor.push_back(vector2d(Pos.X + Size.X - 1, y));

	  if(AllowLanterns)
	    {
	      GenerateLanterns(Pos.X, y, RIGHT);
	      GenerateLanterns(Pos.X + Size.X - 1, y, LEFT);
	    }
	}

      Border.push_back(vector2d(Pos.X, y));
      Border.push_back(vector2d(Pos.X + Size.X - 1, y));
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
	    CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, y, Room);
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

void level::ParticleBeam(character* BeamOwner, const festring& DeathMsg, vector2d CurrentPos, ulong BeamColor, uchar BeamEffect, uchar Direction, uchar Range)
{
  if(Direction != YOURSELF)
    {
      for(ushort Length = 0; Length < Range; ++Length)
	{
	  CurrentPos += game::GetMoveVector(Direction);

	  if(!IsValidPos(CurrentPos))
	    break;

	  lsquare* CurrentSquare = GetLSquare(CurrentPos);

	  if(!CurrentSquare->IsFlyable())
	    {
	      (CurrentSquare->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction);
	      break;
	    }
	  else
	    {
	      CurrentSquare->DrawParticles(BeamColor);

	      if((CurrentSquare->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction))
		break;
	    }
	}
    }
  else
    {
      lsquare* Where = GetLSquare(CurrentPos);
      Where->DrawParticles(BeamColor);
      (Where->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction);
    }
}

/* Note: You will most likely need some help from supernatural entities to comprehend this code. Sorry. */

void level::LightningBeam(character* BeamOwner, const festring& DeathMsg, vector2d CurrentPos, ulong BeamColor, uchar BeamEffect, uchar Direction, uchar Range)
{
  if(Direction == YOURSELF)
    {
      lsquare* Where = GetLSquare(CurrentPos);

      for(ushort c = 0; c < 4; ++c)
	Where->DrawLightning(vector2d(8, 8), BeamColor, YOURSELF);

      (Where->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction);
      return;
    }

  vector2d StartPos;

  switch(Direction)
    {
    case 0: StartPos = vector2d(15, 15); break;
    case 1: StartPos = vector2d(RAND() & 15, 15); break;
    case 2: StartPos = vector2d(0, 15); break;
    case 3: StartPos = vector2d(15, RAND() & 15); break;
    case 4: StartPos = vector2d(0, RAND() & 15); break;
    case 5: StartPos = vector2d(15, 0); break;
    case 6: StartPos = vector2d(RAND() & 15, 0); break;
    case 7: StartPos = vector2d(0, 0); break;
    }

  for(ushort Length = 0; Length < Range; ++Length)
    {
      CurrentPos += game::GetMoveVector(Direction);

      if(!IsValidPos(CurrentPos))
	break;

      lsquare* CurrentSquare = GetLSquare(CurrentPos);

      if(!CurrentSquare->IsFlyable())
	{
	  if((CurrentSquare->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction))
	    break;

	  bool W1, W2;

	  switch(Direction)
	    {
	    case 0:
	      W1 = GetLSquare(CurrentPos + vector2d(1, 0))->IsFlyable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, 1))->IsFlyable();

	      if(W1 == W2)
		Direction = 7;
	      else if(W1)
		{
		  ++CurrentPos.Y;
		  Direction = 2;
		}
	      else
		{
		  ++CurrentPos.X;
		  Direction = 5;
		}

	      break;
	    case 1: Direction = 6; StartPos.Y = 0; break;
	    case 2:
	      W1 = GetLSquare(CurrentPos + vector2d(-1, 0))->IsFlyable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, 1))->IsFlyable();

	      if(W1 == W2)
		Direction = 5;
	      else if(W1)
		{
		  ++CurrentPos.Y;
		  Direction = 0;
		}
	      else
		{
		  --CurrentPos.X;
		  Direction = 7;
		}

	      break;
	    case 3: Direction = 4; StartPos.X = 0; break;
	    case 4: Direction = 3; StartPos.X = 15; break;
	    case 5:
	      W1 = GetLSquare(CurrentPos + vector2d(1, 0))->IsFlyable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, -1))->IsFlyable();

	      if(W1 == W2)
		Direction = 2;
	      else if(W1)
		{
		  --CurrentPos.Y;
		  Direction = 7;
		}
	      else
		{
		  ++CurrentPos.X;
		  Direction = 0;
		}

	      break;
	    case 6: Direction = 1; StartPos.Y = 15; break;
	    case 7:
	      W1 = GetLSquare(CurrentPos + vector2d(-1, 0))->IsFlyable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, -1))->IsFlyable();

	      if(W1 == W2)
		Direction = 0;
	      else if(W1)
		{
		  --CurrentPos.Y;
		  Direction = 5;
		}
	      else
		{
		  --CurrentPos.X;
		  Direction = 2;
		}

	      break;
	    }

	  switch(Direction)
	    {
	    case 0: StartPos = vector2d(15, 15); break;
	    case 2: StartPos = vector2d(0, 15); break;
	    case 5: StartPos = vector2d(15, 0); break;
	    case 7: StartPos = vector2d(0, 0); break;
	    }
	}
      else
	{
	  StartPos = CurrentSquare->DrawLightning(StartPos, BeamColor, Direction);

	  if((CurrentSquare->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction))
	    break;
	}
    }
}

void level::ShieldBeam(character* BeamOwner, const festring& DeathMsg, vector2d CurrentPos, ulong BeamColor, uchar BeamEffect, uchar Direction, uchar)
{
  vector2d Pos[3];

  switch(Direction)
    {
    case 0:
      Pos[0] = vector2d(-1, 0);
      Pos[1] = vector2d(-1, -1);
      Pos[2] = vector2d(0, -1);
      break;
    case 1:
      Pos[0] = vector2d(-1, -1);
      Pos[1] = vector2d(0, -1);
      Pos[2] = vector2d(1, -1);
      break;
    case 2:
      Pos[0] = vector2d(0, -1);
      Pos[1] = vector2d(1, -1);
      Pos[2] = vector2d(1, 0);
      break;
    case 3:
      Pos[0] = vector2d(-1, 1);
      Pos[1] = vector2d(-1, 0);
      Pos[2] = vector2d(-1, -1);
      break;
    case 4:
      Pos[0] = vector2d(1, -1);
      Pos[1] = vector2d(1, 0);
      Pos[2] = vector2d(1, 1);
      break;
    case 5:
      Pos[0] = vector2d(0, 1);
      Pos[1] = vector2d(-1, 1);
      Pos[2] = vector2d(-1, 0);
      break;
    case 6:
      Pos[0] = vector2d(1, 1);
      Pos[1] = vector2d(0, 1);
      Pos[2] = vector2d(-1, 1);
      break;
    case 7:
      Pos[0] = vector2d(1, 0);
      Pos[1] = vector2d(1, 1);
      Pos[2] = vector2d(0, 1);
      break;
    case 8:
      GetLSquare(CurrentPos)->DrawParticles(BeamColor);
      (GetLSquare(CurrentPos)->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction);
      return;
    }

  for(ushort c = 0; c < 3; ++c)
    if(IsValidPos(CurrentPos + Pos[c]))
      {
	GetLSquare(CurrentPos + Pos[c])->DrawParticles(BeamColor);
	(GetLSquare(CurrentPos + Pos[c])->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction);
      }
}

outputfile& operator<<(outputfile& SaveFile, const level* Level)
{
  Level->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, level*& Level)
{
  Level = new level;
  Level->Load(SaveFile);
  return SaveFile;
}

void (level::*level::GetBeam(ushort Index))(character*, const festring&, vector2d, ulong, uchar, uchar, uchar)
{
  static void (level::*Beam[BEAM_STYLES])(character*, const festring&, vector2d, ulong, uchar, uchar, uchar) = { &level::ParticleBeam, &level::LightningBeam, &level::ShieldBeam };
  return Beam[Index];
}

vector2d level::FreeSquareSeeker(const character* Char, vector2d StartPos, vector2d Prohibited, uchar MaxDistance) const
{
  ushort c;

  for(c = 0; c < 8; ++c)
    {
      vector2d Pos = StartPos + game::GetMoveVector(c);

      if(IsValidPos(Pos) && GetLSquare(Pos)->IsWalkable(Char) && !GetLSquare(Pos)->GetCharacter() && Pos != Prohibited)
	return Pos;
    }

  if(MaxDistance)
    for(c = 0; c < 8; ++c)
      {
	vector2d Pos = StartPos + game::GetMoveVector(c);

	if(IsValidPos(Pos))
	  {
	    if(GetLSquare(Pos)->IsWalkable(Char) && Pos != Prohibited)
	      {
		Pos = FreeSquareSeeker(Char, Pos, StartPos, MaxDistance - 1);

		if(Pos != ERROR_VECTOR)
		  return Pos;
	      }
	  }
      }

  return ERROR_VECTOR;
}

/* Returns ERROR_VECTOR if no free square was found */

vector2d level::GetNearestFreeSquare(const character* Char, vector2d StartPos) const
{
  if(GetLSquare(StartPos)->IsWalkable(Char) && !GetLSquare(StartPos)->GetCharacter())
    return StartPos;

  ushort c;

  for(c = 0; c < 8; ++c)
    {
      vector2d Pos = StartPos + game::GetMoveVector(c);

      if(IsValidPos(Pos) && GetLSquare(Pos)->IsWalkable(Char) && !GetLSquare(Pos)->GetCharacter())
	return Pos;
    }

  for(ushort Dist = 0; Dist < 5; ++Dist)
    for(c = 0; c < 8; ++c)
      {
	vector2d Pos = StartPos + game::GetMoveVector(c);

	if(IsValidPos(Pos) && GetLSquare(Pos)->IsWalkable(Char))
	  {
	    Pos = FreeSquareSeeker(Char, Pos, StartPos, Dist);

	    if(Pos != ERROR_VECTOR)
	      return Pos;
	  }
      }

  return ERROR_VECTOR;
}

vector2d level::GetFreeAdjacentSquare(const character* Char, vector2d StartPos, bool AllowCharacter) const
{
  ushort PossibleDir[8];
  ushort Index = 0;
  lsquare* Origo = GetLSquare(StartPos);

  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = Origo->GetNeighbourLSquare(d);

      if(Square && Square->IsWalkable(Char) && (AllowCharacter || !Square->GetCharacter()))
	PossibleDir[Index++] = d;
    }

  return Index ? StartPos + game::GetMoveVector(PossibleDir[RAND() % Index]) : ERROR_VECTOR;
}

void (level::*level::GetBeamEffectVisualizer(ushort Index))(const rect&, ushort) const
{
  static void (level::*Visualizer[BEAM_STYLES])(const rect&, ushort) const = { &level::ParticleVisualizer, &level::LightningVisualizer, &level::ParticleVisualizer };
  return Visualizer[Index];
}

void level::ParticleVisualizer(const rect& Rect, ushort BeamColor) const
{
  clock_t StartTime = clock();
  game::DrawEverythingNoBlit();

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      Map[x][y]->DrawParticles(BeamColor, false);

  graphics::BlitDBToScreen();
  while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
}

void level::LightningVisualizer(const rect& Rect, ushort BeamColor) const
{
  clock_t StartTime = clock();
  game::DrawEverythingNoBlit();

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      Map[x][y]->DrawLightning(vector2d(8, 8), BeamColor, YOURSELF, false);

  graphics::BlitDBToScreen();
  while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
}

bool level::PreProcessForBone()
{
  if(!*LevelScript->CanGenerateBone())
    return false;

  /* Gum solution */

  game::SetQuestMonsterFound(false);

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      Map[x][y]->PreProcessForBone();

  uchar DungeonIndex = GetDungeon()->GetIndex();

  return   game::QuestMonsterFound()
      || ((DungeonIndex != UNDER_WATER_TUNNEL || Index != VESANA_LEVEL)
      &&  (DungeonIndex != ELPURI_CAVE || (Index != ENNER_BEAST_LEVEL && Index != IVAN_LEVEL && Index != DARK_LEVEL)));
}

bool level::PostProcessForBone()
{
  game::SetTooGreatDangerFound(false);
  float DangerSum = 0;
  ushort Enemies = 0;

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      Map[x][y]->PostProcessForBone(DangerSum, Enemies);

  if(game::TooGreatDangerFound() || (Enemies && DangerSum / Enemies > float(Difficulty) / 100))
    return false;

  return true;
}

void level::FinalProcessForBone()
{
  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      Map[x][y]->FinalProcessForBone();
}
