/* Compiled through levelset.cpp */

#define FORBIDDEN 1
#define ON_POSSIBLE_ROUTE 2
#define STILL_ON_POSSIBLE_ROUTE 4
#define PREFERRED 8

level::level() : Room(1, static_cast<room*>(0)) { }

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
  std::string* Msg = LevelScript->GetLevelMessage();

  if(Msg)
    LevelMessage = *Msg;

  if(*LevelScript->GenerateMonsters())
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
  inputfile ScriptFile(game::GetGameDir() + "Script/dungeon.dat", &game::GetGlobalValueMap());

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

	      if(*RoomScript->ReCalculate())
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

	      if(*RoomScript->ReCalculate())
		RoomScript->ReadFrom(ScriptFile, true);

	      if(MakeRoom(RoomScript))
		break;
	    }
	}
    }

  game::BusyAnimation();

  if(!*LevelScript->IgnoreDefaultSpecialSquares())
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
  uchar* ScriptTimes = Script->GetTimes();
  uchar Times = ScriptTimes ? *ScriptTimes : 1;

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

  if(*RoomScript->GenerateFountains() && !(RAND() % 10))
    GetLSquare(Inside[RAND() % Inside.size()])->ChangeOLTerrain(new fountain);

  if(*RoomScript->AltarPossible() && !(RAND() % 5))
    {
      uchar Owner = 1 + RAND() % GODS;
      GetLSquare(Inside[RAND() % Inside.size()])->ChangeOLTerrain(new altar(Owner));

      for(c = 0; c < Inside.size(); ++c)
	GetLSquare(Inside[c])->SetDivineMaster(Owner);

      for(c = 0; c < Border.size(); ++c)
	GetLSquare(Border[c])->SetDivineMaster(Owner);
    }

  if(*RoomScript->GenerateTunnel() && !Door.empty())
    {
      game::BusyAnimation();

      vector2d OutsideDoorPos = Door[RAND() % Door.size()]; // An other room

      if(OKForDoor.empty())
	ABORT("The Doors - You are strange.");

      vector2d InsideDoorPos = OKForDoor[RAND() % OKForDoor.size()]; // this door

      olterrain* Door = RoomScript->GetDoorSquare()->GetOTerrain()->Instantiate(); //Bug! Wrong room!

      if(!(RAND() % 5) && *RoomScript->AllowLockedDoors())
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

      if(!(RAND() % 5) && *RoomScript->AllowLockedDoors())
	{
	  if(*RoomScript->AllowBoobyTrappedDoors() && !(RAND() % 5))
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

  charactercontentmap* CharacterMap = RoomScript->GetCharacterMap();

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
		Char->CreateHomeData();
		bool* IsMaster = CharacterScript->IsMaster();

		if(IsMaster && *IsMaster)
		  RoomClass->SetMasterID(Char->GetID());
	      }
	}
    }

  itemcontentmap* ItemMap = RoomScript->GetItemMap();

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
		      uchar* SideStackIndex = (*ItemScript)[c].GetSideStackIndex();

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

  glterraincontentmap* GTerrainMap = RoomScript->GetGTerrainMap();

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

  olterraincontentmap* OTerrainMap = RoomScript->GetOTerrainMap();

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
	      }
	}
    }

  for(c = 0; c < RoomScript->GetSquare().size(); ++c)
    {
      game::BusyAnimation();
      squarescript* Script = RoomScript->GetSquare()[c];
      uchar* ScriptTimes = Script->GetTimes();
      uchar Times = ScriptTimes ? *ScriptTimes : 1;

      for(ushort t = 0; t < Times; ++t)
	{
	  vector2d Pos;

	  if(Script->GetPosition()->GetRandom())
	    {
	      rect* ScriptBorders = Script->GetPosition()->GetBorders();
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

/* example of the usage: GetRandomSquare() gives out a random walkable square */

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
  // NEEDS SOME WORK!
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

void level::Explosion(character* Terrorist, const std::string& DeathMsg, vector2d Pos, ushort Strength, bool HurtNeutrals)
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

  ExplosionQueue.resize(ExplosionQueue.size() + 1);
  PlayerHurt.resize(PlayerHurt.size() + 1);
  explosion& E = ExplosionQueue.back();
  E.Terrorist = Terrorist;
  E.DeathMsg = DeathMsg;
  E.Pos = Pos;
  E.ID = game::CreateNewExplosionID();
  E.Strength = Strength;
  E.RadiusSquare = (8 - Size) * (8 - Size);
  E.Size = Size;
  E.HurtNeutrals = HurtNeutrals;

  if(ExplosionQueue.size() == 1)
    {
      ushort Explosions = 0;

      while(Explosions != ExplosionQueue.size())
	{
	  for(c = Explosions; c != ExplosionQueue.size(); c = TriggerExplosions(c));
	  ushort NewExplosions = c;

	  for(c = Explosions; c < NewExplosions; ++c)
	    if(PlayerHurt[c])
	      PLAYER->GetHitByExplosion(ExplosionQueue[c], ExplosionQueue[c].Strength / ((PLAYER->GetPos() - ExplosionQueue[c].Pos).GetLengthSquare() + 1));

	  Explosions = NewExplosions;
	}

      ExplosionQueue.clear();
      PlayerHurt.clear();
    }
}

bool level::DrawExplosion(const explosion& Explosion) const
{
  static vector2d StrengthPicPos[7] = { vector2d(176, 176), vector2d(0, 144), vector2d(256, 32), vector2d(144, 32), vector2d(64, 32), vector2d(16, 32),vector2d(0, 32) };
  vector2d BPos = game::CalculateScreenCoordinates(Explosion.Pos) - vector2d((6 - Explosion.Size) << 4, (6 - Explosion.Size) << 4);
  vector2d SizeVect(16 + ((6 - Explosion.Size) << 5), 16 + ((6 - Explosion.Size) << 5));
  vector2d OldSizeVect = SizeVect;
  vector2d PicPos = StrengthPicPos[Explosion.Size];

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

  if(BPos.X >= RES.X || BPos.Y >= RES.Y)
    return false;

  if(BPos.X + SizeVect.X > RES.X)
    SizeVect.X = RES.X - BPos.X;

  if(BPos.Y + SizeVect.Y > RES.Y)
    SizeVect.Y = RES.Y - BPos.Y;

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
      ushort EmitChange = Min(150 + ExplosionQueue[c].Strength, 255);
      GetLSquare(ExplosionQueue[c].Pos)->SetTemporaryEmitation(MakeRGB24(EmitChange, EmitChange, EmitChange));

      if(!GetSquare(ExplosionQueue[c].Pos)->CanBeSeenByPlayer())
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
      GetLSquare(ExplosionQueue[c].Pos)->SetTemporaryEmitation(0);

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
      explosion Explosion(ExplosionQueue[c]);
      ushort Radius = 8 - Explosion.Size;
      game::SetCurrentExplosion(&Explosion);
      game::SetPlayerWasHurtByExplosion(false);

      rect Rect;
      femath::CalculateEnvironmentRectangle(Rect, GetBorder(), Explosion.Pos, Radius);

      for(ushort x = Rect.X1; x <= Rect.X2; ++x)
	{
	  femath::DoLine(Explosion.Pos.X, Explosion.Pos.Y, x, Rect.Y1, game::ExplosionHandler);
	  femath::DoLine(Explosion.Pos.X, Explosion.Pos.Y, x, Rect.Y2, game::ExplosionHandler);
	}

      for(ushort y = Rect.Y1 + 1; y < Rect.Y2; ++y)
	{
	  femath::DoLine(Explosion.Pos.X, Explosion.Pos.Y, Rect.X1, y, game::ExplosionHandler);
	  femath::DoLine(Explosion.Pos.X, Explosion.Pos.Y, Rect.X2, y, game::ExplosionHandler);
	}

      PlayerHurt[c] = game::PlayerWasHurtByExplosion();
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
  uchar* ScriptDivineMaster = RoomScript->GetDivineMaster();
  uchar DivineMaster = ScriptDivineMaster ? *ScriptDivineMaster : 0;
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

void level::ParticleBeam(character* BeamOwner, const std::string& DeathMsg, vector2d CurrentPos, ulong BeamColor, uchar BeamEffect, uchar Direction, uchar Range)
{
  if(Direction != YOURSELF)
    {
      for(ushort Length = 0; Length < Range; ++Length)
	{
	  CurrentPos += game::GetMoveVector(Direction);

	  if(!IsValidPos(CurrentPos))
	    break;

	  lsquare* CurrentSquare = GetLSquare(CurrentPos);

	  if(!CurrentSquare->GetOLTerrain()->IsWalkable())
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

void level::LightningBeam(character* BeamOwner, const std::string& DeathMsg, vector2d CurrentPos, ulong BeamColor, uchar BeamEffect, uchar Direction, uchar Range)
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

      if(!CurrentSquare->GetOLTerrain()->IsWalkable())
	{
	  if((CurrentSquare->*lsquare::GetBeamEffect(BeamEffect))(BeamOwner, DeathMsg, Direction))
	    break;

	  bool W1, W2;

	  switch(Direction)
	    {
	    case 0:
	      W1 = GetLSquare(CurrentPos + vector2d(1, 0))->GetOLTerrain()->IsWalkable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, 1))->GetOLTerrain()->IsWalkable();

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
	      W1 = GetLSquare(CurrentPos + vector2d(-1, 0))->GetOLTerrain()->IsWalkable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, 1))->GetOLTerrain()->IsWalkable();

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
	      W1 = GetLSquare(CurrentPos + vector2d(1, 0))->GetOLTerrain()->IsWalkable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, -1))->GetOLTerrain()->IsWalkable();

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
	      W1 = GetLSquare(CurrentPos + vector2d(-1, 0))->GetOLTerrain()->IsWalkable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, -1))->GetOLTerrain()->IsWalkable();

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

void level::ShieldBeam(character* BeamOwner, const std::string& DeathMsg, vector2d CurrentPos, ulong BeamColor, uchar BeamEffect, uchar Direction, uchar)
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

outputfile& operator<<(outputfile& SaveFile, level* Level)
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

void (level::*level::GetBeam(ushort Index))(character*, const std::string&, vector2d, ulong, uchar, uchar, uchar)
{
  static void (level::*Beam[BEAM_STYLES])(character*, const std::string&, vector2d, ulong, uchar, uchar, uchar) = { &level::ParticleBeam, &level::LightningBeam, &level::ShieldBeam };
  return Beam[Index];
}
