/*
 *
 *  Iter Vehemens ad Necem
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with
 *  this file for more details
 *
 */

/* Compiled through levelset.cpp */

#define FORBIDDEN 1
#define ON_POSSIBLE_ROUTE 2
#define STILL_ON_POSSIBLE_ROUTE 4
#define PREFERRED 8
#define ICE_TERRAIN 16
#define STONE_TERRAIN 32

level::level() : Room(1, static_cast<room*>(0)), GlobalRainLiquid(0), SunLightEmitation(0), AmbientLuminance(0), SquareStack(0), NightAmbientLuminance(0) { }
void level::SetRoom(int I, room* What) { Room[I] = What; }
void level::AddToAttachQueue(vector2d Pos) { AttachQueue.push_back(Pos); }

ulong level::NextExplosionID = 1;

node*** node::NodeMap;
int node::RequiredWalkability;
const character* node::SpecialMover;
vector2d node::To;
uchar** node::WalkabilityMap;
int node::XSize, node::YSize;
nodequeue* node::NodeQueue;

level::~level()
{
  ulong c;

  for(c = 0; c < XSizeTimesYSize; ++c)
    delete NodeMap[0][c];

  for(c = 0; c < Room.size(); ++c)
    delete Room[c];

  delete [] NodeMap;
  delete [] WalkabilityMap;
  delete GlobalRainLiquid;
  delete [] SquareStack;
  game::SetGlobalRainLiquid(0);
}

void level::ExpandPossibleRoute(int OrigoX, int OrigoY, int TargetX, int TargetY, bool XMode)
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

void level::ExpandStillPossibleRoute(int OrigoX, int OrigoY, int TargetX, int TargetY, bool XMode)
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

void level::GenerateTunnel(int FromX, int FromY, int TargetX, int TargetY, bool XMode)
{
  FlagMap[FromX][FromY] |= ON_POSSIBLE_ROUTE;
  ExpandPossibleRoute(FromX, FromY, TargetX, TargetY, XMode);
  const contentscript<glterrain>* GTerrain = LevelScript->GetTunnelSquare()->GetGTerrain();
  const contentscript<olterrain>* OTerrain = LevelScript->GetTunnelSquare()->GetOTerrain();

  if(FlagMap[TargetX][TargetY] & ON_POSSIBLE_ROUTE)
    for(int x = 0; x < XSize; ++x)
      for(int y = 0; y < YSize; ++y)
	if((FlagMap[x][y] & (ON_POSSIBLE_ROUTE|PREFERRED)) == ON_POSSIBLE_ROUTE
	&& !(x == FromX && y == FromY) && !(x == TargetX && y == TargetY))
	  {
	    FlagMap[x][y] &= ~ON_POSSIBLE_ROUTE;
	    FlagMap[FromX][FromY] |= STILL_ON_POSSIBLE_ROUTE;
	    ExpandStillPossibleRoute(FromX, FromY, TargetX, TargetY, XMode);

	    if(!(FlagMap[TargetX][TargetY] & STILL_ON_POSSIBLE_ROUTE))
	      {
		FlagMap[x][y] |= ON_POSSIBLE_ROUTE|PREFERRED;
		Map[x][y]->ChangeGLTerrain(GTerrain->Instantiate());
		Map[x][y]->ChangeOLTerrain(OTerrain->Instantiate());
	      }

	    for(int X = 0; X < XSize; ++X)
	      for(int Y = 0; Y < YSize; ++Y)
		FlagMap[X][Y] &= ~STILL_ON_POSSIBLE_ROUTE;
	  }

  for(int x = 1; x < XSize - 1; ++x)
    for(int y = 1; y < YSize - 1; ++y)
      FlagMap[x][y] &= ~ON_POSSIBLE_ROUTE;
}

void level::Generate(int Index)
{
  game::BusyAnimation();
  Initialize(LevelScript->GetSize()->X, LevelScript->GetSize()->Y);
  game::SetCurrentArea(this);
  game::SetCurrentLevel(this);
  Alloc2D(NodeMap, XSize, YSize);
  Alloc2D(WalkabilityMap, XSize, YSize);
  Map = reinterpret_cast<lsquare***>(area::Map);
  SquareStack = new lsquare*[XSizeTimesYSize];

  if((Index == 0 && GetDungeon()->GetIndex() == NEW_ATTNAM)
  || (Index == 0 && GetDungeon()->GetIndex() == ATTNAM))
    NightAmbientLuminance = MakeRGB24(95, 95, 95);

  int x, y;

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	NodeMap[x][y] = new node(x, y, Map[x][y]);
      }

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      Map[x][y]->CalculateNeighbourLSquares();

  int Type = LevelScript->GetType() ? *LevelScript->GetType() : 0;

  switch(Type)
    {
    case 0:
      GenerateDungeon(Index);
      break;
    case DESERT:
      GenerateDesert();
      break;
    case JUNGLE:
      GenerateJungle();
      break;
    case STEPPE:
      GenerateSteppe();
      break;
    case LEAFY_FOREST:
      GenerateLeafyForest();
      break;
    case EVERGREEN_FOREST:
      GenerateEvergreenForest();
      break;
    case TUNDRA:
      GenerateTundra();
      break;
    case GLACIER:
      GenerateGlacier();
      break;
    default:
      ABORT("You are a terrorist. Please stop creating wterrains that are stupid.");
    }
}

void level::ApplyLSquareScript(const squarescript* Script)
{
  const interval* ScriptTimes = Script->GetTimes();
  int Times = ScriptTimes ? ScriptTimes->Randomize() : 1;

  for(int c = 0; c < Times; ++c)
    {
      vector2d Pos;

      if(Script->GetPosition()->GetRandom())
	Pos = GetRandomSquare(0, Script->GetPosition()->GetFlags(), Script->GetPosition()->GetBorders());
      else
	Pos = Script->GetPosition()->GetVector();

      Map[Pos.X][Pos.Y]->ApplyScript(Script, 0);
    }
}

void level::AttachPos(int WhatX, int WhatY)
{
  int PosX = 1 + RAND() % (XSize - 2);
  int PosY = 1 + RAND() % (YSize - 2);

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

void level::CreateItems(int Amount)
{
  if(Amount)
    {
      long MinPrice = *LevelScript->GetItemMinPriceBase() + *LevelScript->GetItemMinPriceDelta() * Index;

      for(int x = 0; x < Amount; ++x)
	{
	  vector2d Pos = GetRandomSquare();
	  item* Item = protosystem::BalancedCreateItem(MinPrice, MAX_PRICE, ANY_CATEGORY, 0, IGNORE_BROKEN_PRICE);
	  Item->CalculateEnchantment();
	  Map[Pos.X][Pos.Y]->Stack->AddItem(Item);
	  Item->SpecialGenerationHandler();
	}
    }
}

bool level::MakeRoom(const roomscript* RoomScript)
{
  game::BusyAnimation();
  vector2d Pos = RoomScript->GetPos()->Randomize();
  vector2d Size = RoomScript->GetSize()->Randomize();
  int x, y;

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
      int Owner = 1 + RAND() % GODS;
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

      for(int x = 0; x < CharacterMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < CharacterMap->GetSize()->Y; ++y)
	    if(IsValidScript(CharacterScript = CharacterMap->GetContentScript(x, y)))
	      {
		character* Char = CharacterScript->Instantiate();
		Char->SetGenerationDanger(Difficulty);

		if(!Char->GetTeam())
		  Char->SetTeam(game::GetTeam(*LevelScript->GetTeamDefault()));

		if(CharacterScript->GetFlags() & IS_LEADER)
		  Char->GetTeam()->SetLeader(Char);

		Char->PutTo(CharPos + vector2d(x, y));
		Char->CreateHomeData();

		if(CharacterScript->GetFlags() & IS_MASTER)
		  RoomClass->SetMasterID(Char->GetID());
	      }
	}
    }

  const itemcontentmap* ItemMap = RoomScript->GetItemMap();

  if(ItemMap)
    {
      vector2d ItemPos(Pos + *ItemMap->GetPos());
      const fearray<contentscript<item> >* ItemScript;

      for(int x = 0; x < ItemMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < ItemMap->GetSize()->Y; ++y)
	    if(IsValidScript(ItemScript = ItemMap->GetContentScript(x, y)))
	      for(uint c1 = 0; c1 < ItemScript->Size; ++c1)
		{
		  const interval* TimesPtr = ItemScript->Data[c1].GetTimes();
		  int Times = TimesPtr ? TimesPtr->Randomize() : 1;

		  for(int c2 = 0; c2 < Times; ++c2)
		    {
		      item* Item = ItemScript->Data[c1].Instantiate();

		      if(Item)
			{
			  int SquarePosition = ItemScript->Data[c1].GetSquarePosition();

			  if(SquarePosition != CENTER)
			    Item->SignalSquarePositionChange(SquarePosition);

			  Map[ItemPos.X + x][ItemPos.Y + y]->GetStack()->AddItem(Item);
			  Item->SpecialGenerationHandler();
			}
		    }
		}
	}
    }

  const glterraincontentmap* GTerrainMap = RoomScript->GetGTerrainMap();

  if(GTerrainMap)
    {
      vector2d GTerrainPos(Pos + *GTerrainMap->GetPos());
      const contentscript<glterrain>* GTerrainScript;

      for(int x = 0; x < GTerrainMap->GetSize()->X; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < GTerrainMap->GetSize()->Y; ++y)
	    if(IsValidScript(GTerrainScript = GTerrainMap->GetContentScript(x, y)))
	      {
		lsquare* Square = Map[GTerrainPos.X + x][GTerrainPos.Y + y];
		Square->ChangeGLTerrain(GTerrainScript->Instantiate());

		if(GTerrainScript->IsInside())
		  if(*GTerrainScript->IsInside())
		    Square->Flags |= INSIDE;
		  else
		    Square->Flags &= ~INSIDE;
	      }
	}
    }

  const olterraincontentmap* OTerrainMap = RoomScript->GetOTerrainMap();

  if(OTerrainMap)
    {
      vector2d OTerrainPos(Pos + *OTerrainMap->GetPos());
      const contentscript<olterrain>* OTerrainScript;

      for(int x = 0; x < OTerrainMap->GetSize()->X; ++x)
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
      int Times = ScriptTimes ? ScriptTimes->Randomize() : 1;

      for(int t = 0; t < Times; ++t)
	{
	  vector2d SquarePos;

	  if(Script->GetPosition()->GetRandom())
	    {
	      const rect* ScriptBorders = Script->GetPosition()->GetBorders();
	      rect Borders = ScriptBorders ? *ScriptBorders + Pos : rect(Pos, Pos + Size - vector2d(1, 1));
	      SquarePos = GetRandomSquare(0, Script->GetPosition()->GetFlags(), &Borders);
	    }
	  else
	    SquarePos = Pos + Script->GetPosition()->GetVector();

	  Map[SquarePos.X][SquarePos.Y]->ApplyScript(Script, RoomClass);
	}
    }

  return true;
}

bool level::GenerateLanterns(int X, int Y, int SquarePos) const
{
  if(!(RAND() % 7))
    {
      lantern* Lantern = new lantern;
      Lantern->SignalSquarePositionChange(SquarePos);
      Map[X][Y]->GetStack()->AddItem(Lantern);
      return true;
    }

  return false;
}

void level::CreateRoomSquare(glterrain* GLTerrain, olterrain* OLTerrain, int X, int Y, int Room, int Flags) const
{
  Map[X][Y]->ChangeLTerrain(GLTerrain, OLTerrain);
  FlagMap[X][Y] |= FORBIDDEN;
  Map[X][Y]->SetRoomIndex(Room);
  Map[X][Y]->AddFlags(Flags);
}

void level::GenerateMonsters()
{
  if(*LevelScript->GenerateMonsters()
  && game::GetTeam(MONSTER_TEAM)->GetEnabledMembers() < IdealPopulation
  && (MonsterGenerationInterval <= 1 || !(RAND() % MonsterGenerationInterval)))
    GenerateNewMonsters(1);
}

void level::Save(outputfile& SaveFile) const
{
  area::Save(SaveFile);
  SaveFile << Room << GlobalRainLiquid << GlobalRainSpeed;

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->Save(SaveFile);

  SaveFile << Door << LevelMessage << IdealPopulation << MonsterGenerationInterval << Difficulty;
  SaveFile << SunLightEmitation << SunLightDirection << AmbientLuminance << NightAmbientLuminance;
}

void level::Load(inputfile& SaveFile)
{
  game::SetIsGenerating(true);
  area::Load(SaveFile);
  Map = reinterpret_cast<lsquare***>(area::Map);
  SaveFile >> Room;
  GlobalRainLiquid = static_cast<liquid*>(ReadType<material*>(SaveFile));
  SaveFile >> GlobalRainSpeed;

  if(GlobalRainLiquid)
    GlobalRainLiquid->SetVolumeNoSignals(0);

  game::SetGlobalRainLiquid(GlobalRainLiquid);
  game::SetGlobalRainSpeed(GlobalRainSpeed);
  int x, y;

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      Map[x][y] = new lsquare(this, vector2d(x, y));

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      {
	game::SetSquareInLoad(Map[x][y]);
	Map[x][y]->Load(SaveFile);
	Map[x][y]->CalculateNeighbourLSquares();
      }

  SaveFile >> Door >> LevelMessage >> IdealPopulation >> MonsterGenerationInterval >> Difficulty;
  SaveFile >> SunLightEmitation >> SunLightDirection >> AmbientLuminance >> NightAmbientLuminance;
  Alloc2D(NodeMap, XSize, YSize);
  Alloc2D(WalkabilityMap, XSize, YSize);

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      {
	if(!Map[x][y]->IsInside())
	  Map[x][y]->AmbientLuminance = AmbientLuminance;

	NodeMap[x][y] = new node(x, y, Map[x][y]);
	WalkabilityMap[x][y] = Map[x][y]->GetTheoreticalWalkability();
	Map[x][y]->CalculateBorderPartners();
      }

  SquareStack = new lsquare*[XSizeTimesYSize];
  game::SetIsGenerating(false);
}

void level::FiatLux()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	Map[x][y]->CalculateEmitation();
	Map[x][y]->Emitate();
	Map[x][y]->CalculateLuminance();
      }

  CheckSunLight();
}

void level::GenerateNewMonsters(int HowMany, bool ConsiderPlayer)
{
  vector2d Pos;

  for(int c1 = 0; c1 < HowMany; ++c1)
    {
      Pos = vector2d(0, 0);
      character* Char = protosystem::BalancedCreateMonster();
      Char->CalculateEnchantments();

      for(int c2 = 0; c2 < 30; ++c2)
	{
	  Pos = GetRandomSquare(Char);

	  if(!ConsiderPlayer
	  || abs(int(Pos.X) - PLAYER->GetPos().X) > 6
	  || abs(int(Pos.Y) - PLAYER->GetPos().Y) > 6)
	    break;
	}

      if(Pos != ERROR_VECTOR)
	{
	  Char->PutTo(Pos);
	  Char->SetGenerationDanger(Difficulty);
	  Char->SignalGeneration();
	  Char->SignalNaturalGeneration();
	  ivantime Time;
	  game::GetTime(Time);

	  if(Time.Day > 7)
	    Char->EditAllAttributes((Time.Day - 6) >> 1);
	}
      else
	delete Char;
    }
}

/* Example of the usage: GetRandomSquare() gives out a random walkable square */

vector2d level::GetRandomSquare(const character* Char, int Flags, const rect* Borders) const
{
  rect LocalBorder;

  if(Borders)
    {
      LocalBorder = *Borders;
      Borders = &LocalBorder;
      LimitRef(LocalBorder.X1, 0, XSize - 1);
      LimitRef(LocalBorder.X2, 0, XSize - 1);
      LimitRef(LocalBorder.Y1, 0, YSize - 1);
      LimitRef(LocalBorder.Y2, 0, YSize - 1);
    }

  lsquare* LSquare;

  for(int c = 0;; ++c)
    {
      if(c == 50)
	Char = 0;

      if(c == 500)
	return ERROR_VECTOR;

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

      LSquare = Map[Pos.X][Pos.Y];

      if(((Char ? Char->CanMoveOn(LSquare) : (LSquare->GetWalkability() & WALK)) != !(Flags & NOT_WALKABLE))
      || ((Char ? Char->IsFreeForMe(LSquare) : !LSquare->GetCharacter()) != !(Flags & HAS_CHARACTER))
      || ((Flags & ATTACHABLE) && (FlagMap[Pos.X][Pos.Y] & FORBIDDEN)))
	continue;

      int RoomFlags = Flags & (IN_ROOM|NOT_IN_ROOM);

      if((RoomFlags == IN_ROOM && !LSquare->GetRoomIndex())
      || (RoomFlags == NOT_IN_ROOM && LSquare->GetRoomIndex()))
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

int level::GetLOSModifier() const
{
  return *LevelScript->GetLOSModifier();
}

void level::AddRoom(room* NewRoom)
{
  NewRoom->SetIndex(Room.size());
  Room.push_back(NewRoom);
}

room* level::GetRoom(int I) const
{
  if(!I)
    ABORT("Access to room zero denied!");

  return Room[I];
}

void level::Explosion(character* Terrorist, const festring& DeathMsg, vector2d Pos, int Strength, bool HurtNeutrals)
{
  static int StrengthLimit[6] = { 500, 250, 100, 50, 25, 10 };
  uint c;
  int Size = 6;

  for(c = 0; c < 6; ++c)
    if(Strength >= StrengthLimit[c])
      {
        Size = c;
        break;
      }

  PlayerHurt.resize(PlayerHurt.size() + 1);
  explosion* Exp = new explosion;
  Exp->Terrorist = Terrorist;
  Exp->DeathMsg = DeathMsg;
  Exp->Pos = Pos;
  Exp->ID = NextExplosionID++;
  Exp->Strength = Strength;
  Exp->RadiusSquare = (8 - Size) * (8 - Size);
  Exp->Size = Size;
  Exp->HurtNeutrals = HurtNeutrals;
  ExplosionQueue.push_back(Exp);

  if(ExplosionQueue.size() == 1)
    {
      uint Explosions = 0;

      while(Explosions != ExplosionQueue.size())
	{
	  for(c = Explosions; c != ExplosionQueue.size(); c = TriggerExplosions(c));
	  uint NewExplosions = c;

	  for(c = Explosions; c < NewExplosions; ++c)
	    if(PlayerHurt[c] && PLAYER->IsEnabled())
	      PLAYER->GetHitByExplosion(ExplosionQueue[c], ExplosionQueue[c]->Strength / ((PLAYER->GetPos() - ExplosionQueue[c]->Pos).GetLengthSquare() + 1));

	  Explosions = NewExplosions;
	}

      for(uint c = 0; c < ExplosionQueue.size(); ++c)
	delete ExplosionQueue[c];

      ExplosionQueue.clear();
      PlayerHurt.clear();
      NextExplosionID = 1;

      for(int x = 0; x < XSize; ++x)
	for(int y = 0; y < YSize; ++y)
	  Map[x][y]->LastExplosionID = 0;
    }
}

bool level::DrawExplosion(const explosion* Explosion) const
{
  static vector2d StrengthPicPos[7] = { vector2d(176, 176), vector2d(0, 144), vector2d(256, 32), vector2d(144, 32), vector2d(64, 32), vector2d(16, 32),vector2d(0, 32) };
  vector2d BPos = game::CalculateScreenCoordinates(Explosion->Pos) - vector2d((6 - Explosion->Size) << 4, (6 - Explosion->Size) << 4);
  vector2d SizeVect(16 + ((6 - Explosion->Size) << 5), 16 + ((6 - Explosion->Size) << 5));
  vector2d OldSizeVect = SizeVect;
  vector2d PicPos = StrengthPicPos[Explosion->Size];

  if(BPos.X < 0)
    if(BPos.X + SizeVect.X <= 0)
      return false;
    else
      {
	PicPos.X -= BPos.X;
	SizeVect.X += BPos.X;
	BPos.X = 0;
      }

  if(BPos.Y < 0)
    if(BPos.Y + SizeVect.Y <= 0)
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

  int Flags = RAND() & 7;

  if(!Flags || SizeVect != OldSizeVect)
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(DOUBLE_BUFFER, PicPos, BPos, SizeVect, ivanconfig::GetContrastLuminance());
  else
    {
      /* Cache these */
      bitmap ExplosionPic(SizeVect.X, SizeVect.Y);
      ExplosionPic.ActivateFastFlag();
      igraph::GetSymbolGraphic()->NormalBlit(&ExplosionPic, PicPos, 0, 0, SizeVect, Flags);
      ExplosionPic.LuminanceMaskedBlit(DOUBLE_BUFFER, 0, 0, BPos, SizeVect, ivanconfig::GetContrastLuminance());
    }

  return true;
}

struct explosioncontroller
{
  static bool Handler(int x, int y)
  {
    lsquare* Square = Map[x][y];
    Square->GetHitByExplosion(CurrentExplosion);
    return Square->IsFlyable();
  }
  static lsquare*** Map;
  static explosion* CurrentExplosion;
};

lsquare*** explosioncontroller::Map;
explosion* explosioncontroller::CurrentExplosion;

int level::TriggerExplosions(int MinIndex)
{
  int LastExplosion = ExplosionQueue.size();
  int NotSeen = 0;
  int c;

  for(c = MinIndex; c < LastExplosion; ++c)
    {
      int EmitChange = Min(50 + ExplosionQueue[c]->Strength, 255);
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
      if(DrawExplosion(ExplosionQueue[c]))
	Drawn = true;
    }

  if(Drawn)
    {
      graphics::BlitDBToScreen();
      game::GetCurrentArea()->SendNewDrawRequest();
      clock_t StartTime = clock();
      while(clock() - StartTime < 0.3 * CLOCKS_PER_SEC);
    }

  for(c = MinIndex; c < LastExplosion; ++c)
    {
      explosion* Explosion = ExplosionQueue[c];
      int Radius = 8 - Explosion->Size;
      game::SetPlayerWasHurtByExplosion(false);
      explosioncontroller::Map = Map;
      explosioncontroller::CurrentExplosion = Explosion;

      rect Rect;
      femath::CalculateEnvironmentRectangle(Rect, GetBorder(), Explosion->Pos, Radius);

      for(int x = Rect.X1; x <= Rect.X2; ++x)
	{
	  mapmath<explosioncontroller>::DoLine(Explosion->Pos.X, Explosion->Pos.Y, x, Rect.Y1);
	  mapmath<explosioncontroller>::DoLine(Explosion->Pos.X, Explosion->Pos.Y, x, Rect.Y2);
	}

      for(int y = Rect.Y1 + 1; y < Rect.Y2; ++y)
	{
	  mapmath<explosioncontroller>::DoLine(Explosion->Pos.X, Explosion->Pos.Y, Rect.X1, y);
	  mapmath<explosioncontroller>::DoLine(Explosion->Pos.X, Explosion->Pos.Y, Rect.X2, y);
	}

      PlayerHurt[c] = game::PlayerWasHurtByExplosion();

      if(GetLSquare(Explosion->Pos)->IsFlyable())
	GetLSquare(Explosion->Pos)->AddSmoke(new gas(SMOKE, 1000));
    }

  for(c = MinIndex; c < LastExplosion; ++c)
    GetLSquare(ExplosionQueue[c]->Pos)->SetTemporaryEmitation(0);

  return LastExplosion;
}

bool level::CollectCreatures(charactervector& CharacterArray, character* Leader, bool AllowHostiles)
{
  int c;

  if(!AllowHostiles)
    for(c = 0; c < game::GetTeams(); ++c)
      if(Leader->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
	for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	  if((*i)->IsEnabled() && Leader->CanBeSeenBy(*i)
	  && Leader->SquareUnderCanBeSeenBy(*i, true) && (*i)->CanFollow())
	    {
	      ADD_MESSAGE("You can't escape when there are hostile creatures nearby.");
	      return false;
	    }

  bool TakeAll = true;

  for(c = 0; c < game::GetTeams(); ++c)
    if(game::GetTeam(c)->GetEnabledMembers()
    && Leader->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      {
	TakeAll = false;
	break;
      }

  for(c = 0; c < game::GetTeams(); ++c)
    if(game::GetTeam(c) == Leader->GetTeam() || Leader->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled() && *i != Leader
	&& (TakeAll
	 || (Leader->CanBeSeenBy(*i)
	  && Leader->SquareUnderCanBeSeenBy(*i, true)))
	&& (*i)->CanFollow()
	&& (*i)->GetCommandFlags() & FOLLOW_LEADER)
	  {
	    if((*i)->GetAction() && (*i)->GetAction()->IsVoluntary())
	      (*i)->GetAction()->Terminate(false);

	    if(!(*i)->GetAction())
	      {
		ADD_MESSAGE("%s follows you.", (*i)->CHAR_NAME(DEFINITE));
		CharacterArray.push_back(*i);
		(*i)->Remove();
	      }
	  }

  return true;
}

void level::Draw(bool AnimationDraw) const
{
  const int XMin = Max(game::GetCamera().X, 0);
  const int YMin = Max(game::GetCamera().Y, 0);
  const int XMax = Min(XSize, game::GetCamera().X + game::GetScreenXSize());
  const int YMax = Min(YSize, game::GetCamera().Y + game::GetScreenYSize());
  const ulong LOSTick = game::GetLOSTick();

  if(!game::GetSeeWholeMapCheatMode())
    {
      if(!AnimationDraw)
	{
	  for(int x = XMin; x < XMax; ++x)
	    {
	      lsquare** Square = &Map[x][YMin];

	      for(int y = YMin; y < YMax; ++y, ++Square)
		{
		  ulong LastSeen = (*Square)->LastSeen;

		  if(LastSeen == LOSTick)
		    (*Square)->Draw();
		  else if((*Square)->Flags & STRONG_BIT || LastSeen == LOSTick - 2)
		    (*Square)->DrawMemorized();
		}
	    }
	}
      else
	{
	  for(int x = XMin; x < XMax; ++x)
	    {
	      lsquare** Square = &Map[x][YMin];

	      for(int y = YMin; y < YMax; ++y, ++Square)
		if((*Square)->LastSeen == LOSTick)
		  (*Square)->Draw();
		else
		  (*Square)->DrawMemorizedCharacter();
	    }
	}
    }
  else
    {
      for(int x = XMin; x < XMax; ++x)
	{
	  lsquare** Square = &Map[x][YMin];

	  for(int y = YMin; y < YMax; ++y, ++Square)
	    (*Square)->Draw();
	}
    }
}

vector2d level::GetEntryPos(const character* Char, int I) const
{
  if(I == FOUNTAIN)
    {
      std::vector<vector2d> Fountains;
      for(int x = 0; x < XSize; ++x)
	for(int y = 0; y < YSize; ++y)
	  {
	    if(GetLSquare(x,y)->GetOLTerrain() && GetLSquare(x,y)->GetOLTerrain()->IsFountainWithWater())
	      Fountains.push_back(vector2d(x,y));
	  }

      if(Fountains.empty())
	return GetRandomSquare();
      
      return Fountains[RAND_N(Fountains.size())];
    }
  std::map<int, vector2d>::const_iterator i = EntryMap.find(I);
  return i == EntryMap.end() ? GetRandomSquare(Char) : i->second;
}

void level::GenerateRectangularRoom(std::vector<vector2d>& OKForDoor, std::vector<vector2d>& Inside, std::vector<vector2d>& Border, const roomscript* RoomScript, room* RoomClass, vector2d Pos, vector2d Size)
{
  const contentscript<glterrain>* GTerrain;
  const contentscript<olterrain>* OTerrain;

  if(*RoomScript->UseFillSquareWalls())
    {
      GTerrain = LevelScript->GetFillSquare()->GetGTerrain();
      OTerrain = LevelScript->GetFillSquare()->GetOTerrain();
    }
  else
    {
      GTerrain = RoomScript->GetWallSquare()->GetGTerrain();
      OTerrain = RoomScript->GetWallSquare()->GetOTerrain();
    }

  int Room = RoomClass->GetIndex();
  long Counter = 0;
  bool AllowLanterns = *RoomScript->GenerateLanterns();
  bool AllowWindows = *RoomScript->GenerateWindows();
  int x, y;
  int Shape = *RoomScript->GetShape();
  int Flags = (GTerrain->IsInside() ? *GTerrain->IsInside() : *RoomScript->IsInside()) ? INSIDE : 0;

  if(Shape == ROUND_CORNERS && (Size.X < 5 || Size.Y < 5)) /* No weird shapes this way. */
    Shape = RECTANGLE;

  for(x = Pos.X; x < Pos.X + Size.X; ++x, Counter += 2)
    {
      if(Shape == ROUND_CORNERS)
	{
	  if(x == Pos.X)
	    {
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x + 1, Pos.Y + 1, Room, Flags);
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x + 1, Pos.Y + Size.Y - 2, Room, Flags);
	      Border.push_back(vector2d(x + 1, Pos.Y + 1));
	      Border.push_back(vector2d(x + 1, Pos.Y + Size.Y - 2));
	      continue;
	    }
	  else if(x == Pos.X + Size.X - 1)
	    {
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x - 1, Pos.Y + 1, Room, Flags);
	      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x - 1, Pos.Y + Size.Y - 2, Room, Flags);
	      Border.push_back(vector2d(x - 1, Pos.Y + 1));
	      Border.push_back(vector2d(x - 1, Pos.Y + Size.Y - 2));
	      continue;
	    }
	}

      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, Pos.Y, Room, Flags);
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, Pos.Y + Size.Y - 1, Room, Flags);

      if((Shape == RECTANGLE && x != Pos.X && x != Pos.X + Size.X - 1)
      || (Shape == ROUND_CORNERS && x > Pos.X + 1 && x < Pos.X + Size.X - 2))
	{
	  OKForDoor.push_back(vector2d(x, Pos.Y));
	  OKForDoor.push_back(vector2d(x, Pos.Y + Size.Y - 1));

	  if((!AllowLanterns || !GenerateLanterns(x, Pos.Y, DOWN)) && AllowWindows)
	    GenerateWindows(x, Pos.Y);

	  if((!AllowLanterns || !GenerateLanterns(x, Pos.Y + Size.Y - 1, UP)) && AllowWindows)
	    GenerateWindows(x, Pos.Y + Size.Y - 1);
	}

      Border.push_back(vector2d(x, Pos.Y));
      Border.push_back(vector2d(x, Pos.Y + Size.Y - 1));
    }

  game::BusyAnimation();

  for(y = Pos.Y + 1; y < Pos.Y + Size.Y - 1; ++y, Counter += 2)
    {
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), Pos.X, y, Room, Flags);
      CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), Pos.X + Size.X - 1, y, Room, Flags);

      if(Shape == RECTANGLE || (Shape == ROUND_CORNERS && y != Pos.Y + 1 && y != Pos.Y + Size.Y - 2))
	{
	  OKForDoor.push_back(vector2d(Pos.X, y));
	  OKForDoor.push_back(vector2d(Pos.X + Size.X - 1, y));

	  if((!AllowLanterns || !GenerateLanterns(Pos.X, y, RIGHT)) && AllowWindows)
	    GenerateWindows(Pos.X, y);

	  if((!AllowLanterns || !GenerateLanterns(Pos.X + Size.X - 1, y, LEFT)) && AllowWindows)
	    GenerateWindows(Pos.X + Size.X - 1, y);
	}

      Border.push_back(vector2d(Pos.X, y));
      Border.push_back(vector2d(Pos.X + Size.X - 1, y));
    }

  GTerrain = RoomScript->GetFloorSquare()->GetGTerrain();
  OTerrain = RoomScript->GetFloorSquare()->GetOTerrain();
  Counter = 0;
  Flags = (GTerrain->IsInside() ? *GTerrain->IsInside() : *RoomScript->IsInside()) ? INSIDE : 0;

  for(x = Pos.X + 1; x < Pos.X + Size.X - 1; ++x)
    for(y = Pos.Y + 1; y < Pos.Y + Size.Y - 1; ++y, ++Counter)
      {
	/* if not in the corner */

	if(!(Shape == ROUND_CORNERS && (x == Pos.X + 1 || x == Pos.X + Size.X - 2) && (y == Pos.Y + 1 || y == Pos.Y + Size.Y - 2)))
	  {
	    CreateRoomSquare(GTerrain->Instantiate(), OTerrain->Instantiate(), x, y, Room, Flags);
	    Inside.push_back(vector2d(x,y));
	  }
      }
}

void level::Reveal()
{
  ulong Tick = game::GetLOSTick();

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->Reveal(Tick);
}

void level::ParticleBeam(beamdata& Beam)
{
  vector2d CurrentPos = Beam.StartPos;

  if(Beam.Direction != YOURSELF)
    {
      for(int Length = 0; Length < Beam.Range; ++Length)
	{
	  CurrentPos += game::GetMoveVector(Beam.Direction);

	  if(!IsValidPos(CurrentPos))
	    break;

	  lsquare* CurrentSquare = GetLSquare(CurrentPos);

	  if(!CurrentSquare->IsFlyable())
	    {
	      (CurrentSquare->*lsquare::GetBeamEffect(Beam.BeamEffect))(Beam);
	      break;
	    }
	  else
	    {
	      CurrentSquare->DrawParticles(Beam.BeamColor);

	      if((CurrentSquare->*lsquare::GetBeamEffect(Beam.BeamEffect))(Beam))
		break;
	    }
	}
    }
  else
    {
      lsquare* Where = GetLSquare(CurrentPos);
      Where->DrawParticles(Beam.BeamColor);
      (Where->*lsquare::GetBeamEffect(Beam.BeamEffect))(Beam);
    }
}

/* Note: You will most likely need some help from supernatural entities to comprehend this code. Sorry. */

void level::LightningBeam(beamdata& Beam)
{
  vector2d CurrentPos = Beam.StartPos;

  if(Beam.Direction == YOURSELF)
    {
      lsquare* Where = GetLSquare(CurrentPos);

      for(int c = 0; c < 4; ++c)
	Where->DrawLightning(vector2d(8, 8), Beam.BeamColor, YOURSELF);

      (Where->*lsquare::GetBeamEffect(Beam.BeamEffect))(Beam);
      return;
    }

  vector2d StartPos;

  switch(Beam.Direction)
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

  for(int Length = 0; Length < Beam.Range; ++Length)
    {
      CurrentPos += game::GetMoveVector(Beam.Direction);

      if(!IsValidPos(CurrentPos))
	break;

      lsquare* CurrentSquare = GetLSquare(CurrentPos);

      if(!CurrentSquare->IsFlyable())
	{
	  if((CurrentSquare->*lsquare::GetBeamEffect(Beam.BeamEffect))(Beam))
	    break;

	  bool W1, W2;

	  switch(Beam.Direction)
	    {
	    case 0:
	      W1 = GetLSquare(CurrentPos + vector2d(1, 0))->IsFlyable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, 1))->IsFlyable();

	      if(W1 == W2)
		Beam.Direction = 7;
	      else if(W1)
		{
		  ++CurrentPos.Y;
		  Beam.Direction = 2;
		}
	      else
		{
		  ++CurrentPos.X;
		  Beam.Direction = 5;
		}

	      break;
	    case 1: Beam.Direction = 6; StartPos.Y = 0; break;
	    case 2:
	      W1 = GetLSquare(CurrentPos + vector2d(-1, 0))->IsFlyable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, 1))->IsFlyable();

	      if(W1 == W2)
		Beam.Direction = 5;
	      else if(W1)
		{
		  ++CurrentPos.Y;
		  Beam.Direction = 0;
		}
	      else
		{
		  --CurrentPos.X;
		  Beam.Direction = 7;
		}

	      break;
	    case 3: Beam.Direction = 4; StartPos.X = 0; break;
	    case 4: Beam.Direction = 3; StartPos.X = 15; break;
	    case 5:
	      W1 = GetLSquare(CurrentPos + vector2d(1, 0))->IsFlyable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, -1))->IsFlyable();

	      if(W1 == W2)
		Beam.Direction = 2;
	      else if(W1)
		{
		  --CurrentPos.Y;
		  Beam.Direction = 7;
		}
	      else
		{
		  ++CurrentPos.X;
		  Beam.Direction = 0;
		}

	      break;
	    case 6: Beam.Direction = 1; StartPos.Y = 15; break;
	    case 7:
	      W1 = GetLSquare(CurrentPos + vector2d(-1, 0))->IsFlyable();
	      W2 = GetLSquare(CurrentPos + vector2d(0, -1))->IsFlyable();

	      if(W1 == W2)
		Beam.Direction = 0;
	      else if(W1)
		{
		  --CurrentPos.Y;
		  Beam.Direction = 5;
		}
	      else
		{
		  --CurrentPos.X;
		  Beam.Direction = 2;
		}

	      break;
	    }

	  switch(Beam.Direction)
	    {
	    case 0: StartPos = vector2d(15, 15); break;
	    case 2: StartPos = vector2d(0, 15); break;
	    case 5: StartPos = vector2d(15, 0); break;
	    case 7: StartPos = vector2d(0, 0); break;
	    }
	}
      else
	{
	  StartPos = CurrentSquare->DrawLightning(StartPos, Beam.BeamColor, Beam.Direction);

	  if((CurrentSquare->*lsquare::GetBeamEffect(Beam.BeamEffect))(Beam))
	    break;
	}
    }
}

void level::ShieldBeam(beamdata& Beam)
{
  vector2d Pos[3];

  switch(Beam.Direction)
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
      GetLSquare(Beam.StartPos)->DrawParticles(Beam.BeamColor);
      (GetLSquare(Beam.StartPos)->*lsquare::GetBeamEffect(Beam.BeamEffect))(Beam);
      return;
    }

  for(int c = 0; c < 3; ++c)
    if(IsValidPos(Beam.StartPos + Pos[c]))
      {
	GetLSquare(Beam.StartPos + Pos[c])->DrawParticles(Beam.BeamColor);
	(GetLSquare(Beam.StartPos + Pos[c])->*lsquare::GetBeamEffect(Beam.BeamEffect))(Beam);
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

void (level::*Beam[BEAM_STYLES])(beamdata&) =
{
  &level::ParticleBeam,
  &level::LightningBeam,
  &level::ShieldBeam
};

void (level::*level::GetBeam(int I))(beamdata&)
{
  return Beam[I];
}

vector2d level::FreeSquareSeeker(const character* Char, vector2d StartPos, vector2d Prohibited, int MaxDistance, bool AllowStartPos) const
{
  int c;

  for(c = 0; c < 8; ++c)
    {
      vector2d Pos = StartPos + game::GetMoveVector(c);

      if(IsValidPos(Pos) && Char->CanMoveOn(GetLSquare(Pos)) && Char->IsFreeForMe(GetLSquare(Pos)) && Pos != Prohibited && (AllowStartPos || !Char->PlaceIsIllegal(Pos, Prohibited)))
	return Pos;
    }

  if(MaxDistance)
    for(c = 0; c < 8; ++c)
      {
	vector2d Pos = StartPos + game::GetMoveVector(c);

	if(IsValidPos(Pos))
	  {
	    if(Char->CanMoveOn(GetLSquare(Pos)) && Pos != Prohibited)
	      {
		Pos = FreeSquareSeeker(Char, Pos, Prohibited, MaxDistance - 1, AllowStartPos);

		if(Pos != ERROR_VECTOR)
		  return Pos;
	      }
	  }
      }

  return ERROR_VECTOR;
}

/* Returns ERROR_VECTOR if no free square was found */

vector2d level::GetNearestFreeSquare(const character* Char, vector2d StartPos, bool AllowStartPos) const
{
  if(AllowStartPos && Char->CanMoveOn(GetLSquare(StartPos)) && Char->IsFreeForMe(GetLSquare(StartPos)))
    return StartPos;

  int c;

  for(c = 0; c < 8; ++c)
    {
      vector2d Pos = StartPos + game::GetMoveVector(c);

      if(IsValidPos(Pos) && Char->CanMoveOn(GetLSquare(Pos)) && Char->IsFreeForMe(GetLSquare(Pos)) && (AllowStartPos || !Char->PlaceIsIllegal(Pos, StartPos)))
	return Pos;
    }

  for(int Dist = 0; Dist < 5; ++Dist)
    for(c = 0; c < 8; ++c)
      {
	vector2d Pos = StartPos + game::GetMoveVector(c);

	if(IsValidPos(Pos) && Char->CanMoveOn(GetLSquare(Pos)))
	  {
	    Pos = FreeSquareSeeker(Char, Pos, StartPos, Dist, AllowStartPos);

	    if(Pos != ERROR_VECTOR)
	      return Pos;
	  }
      }

  return ERROR_VECTOR;
}

vector2d level::GetFreeAdjacentSquare(const character* Char, vector2d StartPos, bool AllowCharacter) const
{
  int PossibleDir[8];
  int Index = 0;
  lsquare* Origo = GetLSquare(StartPos);

  for(int d = 0; d < 8; ++d)
    {
      lsquare* Square = Origo->GetNeighbourLSquare(d);

      if(Square && Char->CanMoveOn(Square) && (AllowCharacter || Char->IsFreeForMe(Square)))
	PossibleDir[Index++] = d;
    }

  return Index ? StartPos + game::GetMoveVector(PossibleDir[RAND() % Index]) : ERROR_VECTOR;
}

void (level::*level::GetBeamEffectVisualizer(int I))(const fearray<lsquare*>&, color16) const
{
  static void (level::*Visualizer[BEAM_STYLES])(const fearray<lsquare*>&, color16) const = { &level::ParticleVisualizer, &level::LightningVisualizer, &level::ParticleVisualizer };
  return Visualizer[I];
}

void level::ParticleVisualizer(const fearray<lsquare*>& Stack, color16 BeamColor) const
{
  clock_t StartTime = clock();
  game::DrawEverythingNoBlit();

  for(fearray<lsquare*>::sizetype c = 0; c < Stack.Size; ++c)
    Stack[c]->DrawParticles(BeamColor, false);

  graphics::BlitDBToScreen();
  while(clock() - StartTime < 0.05 * CLOCKS_PER_SEC);
}

void level::LightningVisualizer(const fearray<lsquare*>& Stack, color16 BeamColor) const
{
  clock_t StartTime = clock();
  game::DrawEverythingNoBlit();

  for(fearray<lsquare*>::sizetype c = 0; c < Stack.Size; ++c)
    Stack[c]->DrawLightning(vector2d(8, 8), BeamColor, YOURSELF, false);

  graphics::BlitDBToScreen();
  while(clock() - StartTime < 0.05 * CLOCKS_PER_SEC);
}

bool level::PreProcessForBone()
{
  if(!*LevelScript->CanGenerateBone())
    return false;

  /* Gum solution */

  game::SetQuestMonstersFound(0);

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->PreProcessForBone();

  int DungeonIndex = GetDungeon()->GetIndex();

  return !(DungeonIndex == ELPURI_CAVE && Index == IVAN_LEVEL && game::GetQuestMonstersFound() < 5)
      &&  (game::GetQuestMonstersFound()
      || ((DungeonIndex != UNDER_WATER_TUNNEL || Index != VESANA_LEVEL)
      &&  (DungeonIndex != ELPURI_CAVE || (Index != ENNER_BEAST_LEVEL && Index != DARK_LEVEL))));
}

bool level::PostProcessForBone()
{
  game::SetTooGreatDangerFound(false);
  double DangerSum = 0;
  int Enemies = 0;

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->PostProcessForBone(DangerSum, Enemies);

  if(game::TooGreatDangerFound() || (Enemies && DangerSum / Enemies > Difficulty * 10))
    return false;

  return true;
}

void level::FinalProcessForBone()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->FinalProcessForBone();

  for(int c = 1; c < Room.size(); ++c)
    Room[c]->FinalProcessForBone();
}

void level::GenerateDungeon(int Index)
{
  const festring* Msg = LevelScript->GetLevelMessage();

  if(Msg)
    LevelMessage = *Msg;

  if(*LevelScript->GenerateMonsters())
    {
      MonsterGenerationInterval = *LevelScript->GetMonsterGenerationIntervalBase() + *LevelScript->GetMonsterGenerationIntervalDelta() * Index;
      IdealPopulation = *LevelScript->GetMonsterAmountBase() + *LevelScript->GetMonsterAmountDelta() * Index;
    }

  Difficulty = 0.001 * (*LevelScript->GetDifficultyBase() + *LevelScript->GetDifficultyDelta() * Index);
  EnchantmentMinusChance = *LevelScript->GetEnchantmentMinusChanceBase() + *LevelScript->GetEnchantmentMinusChanceDelta() * Index;
  EnchantmentPlusChance = *LevelScript->GetEnchantmentPlusChanceBase() + *LevelScript->GetEnchantmentPlusChanceDelta() * Index;
  const contentscript<glterrain>* GTerrain = LevelScript->GetFillSquare()->GetGTerrain();
  const contentscript<olterrain>* OTerrain = LevelScript->GetFillSquare()->GetOTerrain();
  long Counter = 0;
  int x;
  game::BusyAnimation();

  for(x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y, ++Counter)
      Map[x][y]->SetLTerrain(GTerrain->Instantiate(), OTerrain->Instantiate());

  uint c;
  uint Rooms = LevelScript->GetRooms()->Randomize();
  const std::list<roomscript>& RoomList = LevelScript->GetRoom();
  std::list<roomscript>::const_iterator Iterator = RoomList.begin();

  for(c = 0; c < Rooms; ++c)
    {
      game::BusyAnimation();

      if(c < RoomList.size())
	{
	  int i;

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

	  for(int i = 0; i < 50; ++i)
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

  for(c = 0; c < AttachQueue.size(); ++c)
    AttachPos(AttachQueue[c].X, AttachQueue[c].Y);

  for(x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->CalculateBorderPartners();

  AttachQueue.clear();
  CreateItems(LevelScript->GetItems()->Randomize());
}

void level::GenerateJungle()
{
  int x,y;

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	Map[x][y]->SetLTerrain(new solidterrain(GRASS_TERRAIN), 0);
      }

  for(;;)
    {
      CreateTunnelNetwork(1, 4, 20, 120, vector2d(0, YSize / 2));
      CreateTunnelNetwork(1, 4, 20, 120, vector2d(XSize - 1, YSize / 2));

      for(int c = 0; c < 25; ++c)
	{
	  vector2d StartPos;

	  switch(RAND_N(5))
	    {
	    case 0:
	      StartPos = vector2d(RAND_N(XSize), 0);
	      break;
	    case 1:
	      StartPos = vector2d(RAND_N(XSize), YSize - 1);
	      break;
	    case 2:
	      StartPos = vector2d(0, RAND_N(YSize));
	      break;
	    case 3:
	      StartPos = vector2d(XSize - 1, RAND_N(YSize));
	      break;
	    case 4:
	      StartPos = vector2d(RAND_N(XSize), RAND_N(YSize));
	    }

	  CreateTunnelNetwork(1,4,20, 120, StartPos);
	}

      for(x = 0; x < XSize; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < YSize; ++y)
	    {
	      if(FlagMap[x][y] != PREFERRED)
		Map[x][y]->ChangeOLTerrain(new wall(BRICK_PROPAGANDA));
	      else if(RAND_2)
		Map[x][y]->ChangeOLTerrain(new decoration(PALM));
	    }
	}
    }
}

void level::CreateTunnelNetwork(int MinLength, int MaxLength, int MinNodes, int MaxNodes, vector2d StartPos)
{
  vector2d Pos = StartPos, Direction;
  int Length;
  game::BusyAnimation();
  FlagMap[Pos.X][Pos.Y] = PREFERRED;

  for(int c1 = 0; c1 < MaxNodes; ++c1)
    {
      Direction = game::GetBasicMoveVector(RAND() % 4);
      Length = MinLength + RAND_N(MaxLength - MinLength + 1);

      for(int c2 = 0; c2 < Length; ++c2)
	{
	  if(IsValidPos(Direction + Pos))
	    {
	      Pos += Direction;
	      FlagMap[Pos.X][Pos.Y] = PREFERRED;
	    }
	  else
	    {
	      if(c1 >= MinNodes)
		return;

	      break;
	    }
	}
    }
}

void level::GenerateDesert()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	Map[x][y]->SetLTerrain(new solidterrain(SAND_TERRAIN), 0);
      }

  game::BusyAnimation();
  int AmountOfCactuses = RAND_N(10);
  int c;

  for(c = 0; c < AmountOfCactuses; ++c)
    Map[RAND_N(XSize)][RAND_N(YSize)]->ChangeOLTerrain(new decoration(CACTUS));

  int AmountOfBoulders = RAND_N(10);
  
  for(c = 0; c < AmountOfBoulders; ++c)
    Map[RAND_N(XSize)][RAND_N(YSize)]->ChangeOLTerrain(new boulder(1 + RAND_2));
}

void level::GenerateSteppe()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	Map[x][y]->SetLTerrain(new solidterrain(GRASS_TERRAIN), 0);
      }

  game::BusyAnimation();
  int c;
  
  int AmountOfBoulders = RAND_N(20) + 5;
  
  for(c = 0; c < AmountOfBoulders; ++c)
    Map[RAND_N(XSize)][RAND_N(YSize)]->ChangeOLTerrain(new boulder(1 + RAND_2));
}

void level::GenerateLeafyForest()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	olterrain* OLTerrain;

	switch(RAND_4)
	  {
	  case 0:
	    if(RAND_8)
	      OLTerrain = new decoration(OAK);
	    else
	      OLTerrain = new decoration(TEAK);
	    break;
	  case 1:
	    OLTerrain = new decoration(BIRCH);
	    break;
	  case 2:
	    OLTerrain = 0;
	    if(!RAND_4)
	      OLTerrain = new boulder(1 + RAND_2);

	    if(!RAND_4)
	      OLTerrain = new boulder(3);
	    break;
	  default:
	    OLTerrain = 0;
	  }

	Map[x][y]->SetLTerrain(new solidterrain(GRASS_TERRAIN), OLTerrain);
      }
}

void level::GenerateEvergreenForest()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	olterrain* OLTerrain = 0;

	switch(RAND_4)
	  {
	  case 0:
	    if(RAND_2)
	      OLTerrain = new decoration(PINE);
	    break;
	  case 1:
	    OLTerrain = new decoration(FIR);
	    break;
	  case 2:
	    if(!RAND_4)
	      OLTerrain = new boulder(1 + RAND_2);

	    if(!RAND_4)
	      OLTerrain = new boulder(3);
	    break;
	  }

	Map[x][y]->SetLTerrain(new solidterrain(GRASS_TERRAIN), OLTerrain);
      }
}

void level::GenerateTundra()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	Map[x][y]->SetLTerrain(new solidterrain(SNOW_TERRAIN), 0);
      }

  game::BusyAnimation();
  int c;
  int AmountOfBoulders = RAND_N(20) + 8;

  for(c = 0; c < AmountOfBoulders; ++c)
    Map[RAND_N(XSize)][RAND_N(YSize)]->ChangeOLTerrain(new boulder(SNOW_BOULDER));

  int AmountOfDwarfBirches = RAND_N(10);

  for(c = 0; c < AmountOfDwarfBirches; ++c)
    Map[RAND_N(XSize)][RAND_N(YSize)]->ChangeOLTerrain(new decoration(DWARF_BIRCH));
}

void level::GenerateGlacier()
{
  int x,y;

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      {
	Map[x][y] = new lsquare(this, vector2d(x, y));
	Map[x][y]->SetLTerrain(new solidterrain(SNOW_TERRAIN), 0);
      }

  int AmountOfBoulders = RAND_N(20) + 5;

  for(int c = 0; c < AmountOfBoulders; ++c)
    Map[RAND_N(XSize)][RAND_N(YSize)]->ChangeOLTerrain(new boulder(SNOW_BOULDER));

  for(;;)
    {
      CreateTunnelNetwork(1,4,20, 120, vector2d(0,YSize / 2));
      CreateTunnelNetwork(1,4,20, 120, vector2d(XSize - 1,YSize / 2));

      for(int c = 0; c < 20; ++c)
	{
	  vector2d StartPos;

	  switch(RAND_N(5))
	    {
	    case 0:
	      StartPos = vector2d(RAND_N(XSize), 0);
	      break;
	    case 1:
	      StartPos = vector2d(RAND_N(XSize), YSize - 1);
	      break;
	    case 2:
	      StartPos = vector2d(0, RAND_N(YSize));
	      break;
	    case 3:
	      StartPos = vector2d(XSize - 1, RAND_N(YSize));
	      break;
	    case 4:
	      StartPos = vector2d(RAND_N(XSize), RAND_N(YSize));
	    }

	  CreateTunnelNetwork(1,4,20, 120, StartPos);
	}

      for(x = 0; x < XSize; ++x)
	for(y = 0; y < YSize; ++y)
	  if(FlagMap[x][y] != PREFERRED)
	    FlagMap[x][y] |= RAND_2 ? ICE_TERRAIN : STONE_TERRAIN;

      for(x = 0; x < XSize; ++x)
	{
	  game::BusyAnimation();

	  for(y = 0; y < YSize; ++y)
	    {
	      if(!(FlagMap[x][y] & PREFERRED)) 
		{
		  int SquaresAround = 0;
		  int IceAround = 0;

		  for(int d = 0; d < 8; ++d)
		    {
		      vector2d Pos = vector2d(x,y) + game::GetMoveVector(d);
		      if(IsValidPos(Pos) && !(FlagMap[Pos.X][Pos.Y] & PREFERRED))
			{
			  ++SquaresAround;
			  if(FlagMap[Pos.X][Pos.Y] & ICE_TERRAIN)
			    ++IceAround;
			}
		    }

		  if(IceAround > SquaresAround / 2)
		    FlagMap[x][y] = ICE_TERRAIN;
		  else
		    FlagMap[x][y] = STONE_TERRAIN;
		}
	    }
	}

      for(x = 0; x < XSize; ++x)
	for(y = 0; y < YSize; ++y)
	  if(!(FlagMap[x][y] & PREFERRED))
	    {
	      if(FlagMap[x][y] & ICE_TERRAIN)
		GetLSquare(x,y)->ChangeOLTerrain(new wall(ICE_WALL));
	      else
		GetLSquare(x,y)->ChangeOLTerrain(new wall(STONE_WALL));
	    }

      break; // Doesn't yet check path in any way 
    }
}

bool nodepointerstorer::operator<(const nodepointerstorer& N) const
{
  /* In the non-euclidean geometry of IVAN, certain very curved paths are as long as straight ones.
     However, they are so ugly that it is best to prefer routes with as few diagonal moves as
     possible without lengthening the travel. */

  if(Node->TotalDistanceEstimate != N.Node->TotalDistanceEstimate)
    return Node->TotalDistanceEstimate > N.Node->TotalDistanceEstimate;
  else
    return Node->Diagonals > N.Node->Diagonals;
}

void node::CalculateNextNodes()
{
  static int TryOrder[8] = { 1, 3, 4, 6, 0, 2, 5, 7 };

  for(int d = 0; d < 8; ++d)
    {
      vector2d NodePos = Pos + game::GetMoveVector(TryOrder[d]);

      if(NodePos.X >= 0 && NodePos.Y >= 0 && NodePos.X < XSize && NodePos.Y < YSize)
	{
	  node* Node = NodeMap[NodePos.X][NodePos.Y];

	  if(!Node->Processed && ((!SpecialMover && RequiredWalkability & WalkabilityMap[NodePos.X][NodePos.Y]) || (SpecialMover && SpecialMover->CanTheoreticallyMoveOn(Node->Square)) || NodePos == To))
	    {
	      Node->Processed = true;
	      Node->Distance = Distance + 1;
	      Node->Diagonals = Diagonals;

	      if(d >= 4)
		++Node->Diagonals;

	      Node->Last = this;

	      /* We use the heuristic max(abs(distance.x), abs(distance.y)) here,
	         which is exact in the current geometry if the path is open */

	      long Remaining = To.X - NodePos.X;

	      if(Remaining < NodePos.X - To.X)
		Remaining = NodePos.X - To.X;

	      if(Remaining < NodePos.Y - To.Y)
		Remaining = NodePos.Y - To.Y;

	      if(Remaining < To.Y - NodePos.Y)
		Remaining = To.Y - NodePos.Y;

	      Node->Remaining = Remaining;
	      Node->TotalDistanceEstimate = Node->Distance + Node->Remaining;
	      NodeQueue->push(nodepointerstorer(Node));
	    }
	}
    }
}

/* Finds the shortest (but possibly not the shortest-looking) path between From and To
   if such exists. Returns a pointer to the node associated with the last square or zero if
   a route can't be found. Calling FindRoute again may invalidate the node, so you must
   store the path in another format ASAP. */

node* level::FindRoute(vector2d From, vector2d To, const std::set<vector2d>& Illegal, int RequiredWalkability, const character* SpecialMover)
{
  node::NodeMap = NodeMap;
  node::RequiredWalkability = RequiredWalkability;
  node::SpecialMover = SpecialMover;
  node::To = To;
  node::WalkabilityMap = WalkabilityMap;
  node::XSize = XSize;
  node::YSize = YSize;

  if(!Illegal.empty() && Illegal.find(To) != Illegal.end())
    return 0;

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      NodeMap[x][y]->Processed = false;

  node* Node = NodeMap[From.X][From.Y]; 
  Node->Last = 0; 
  Node->Processed = true;
  Node->Distance = 0;
  Node->Diagonals = 0;
  nodequeue NodeQueue;
  NodeQueue.push(nodepointerstorer(Node));
  node::NodeQueue = &NodeQueue;

  while(!NodeQueue.empty())
    {
      Node = NodeQueue.top().Node;
      NodeQueue.pop();

      if(Node->Pos == To)
	return Node;

      if(Illegal.empty() || Illegal.find(Node->Pos) == Illegal.end())
	Node->CalculateNextNodes();
    }

  return 0;
}

/* All items on ground are moved to the IVector and all characters to CVector */

void level::CollectEverything(itemvector& IVector, charactervector& CVector)
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	lsquare* LS = Map[x][y];
	LS->GetStack()->MoveItemsTo(IVector, CENTER);
	character* C = LS->GetCharacter();

	if(C && !C->IsPlayer())
	  {
	    C->Remove();
	    CVector.push_back(C);
	  }
      }
}

void level::CreateGlobalRain(liquid* Liquid, vector2d Speed)
{
  GlobalRainLiquid = Liquid;
  GlobalRainSpeed = Speed;

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      if(!Map[x][y]->IsInside())
	Map[x][y]->AddRain(Liquid, Speed, MONSTER_TEAM, false);
}

void level::CheckSunLight()
{
  if(Index == 0 && GetDungeon()->GetIndex() == NEW_ATTNAM)
    {
      double Cos = cos(FPI * (game::GetTick() % 48000) / 24000.);

      if(Cos > 0.01)
	{
	  int E = int(100 + Cos * 30);
	  SunLightEmitation = MakeRGB24(E, E, E);
	  AmbientLuminance = MakeRGB24(E - 6, E - 6, E - 6);
	}
      else
	{
	  SunLightEmitation = 0;
	  AmbientLuminance = NightAmbientLuminance;
	}
    }
  else if(Index == 0 && GetDungeon()->GetIndex() == ATTNAM)
    {
      double Cos = cos(FPI * (game::GetTick() % 48000) / 24000.);

      if(Cos > 0.41)
	{
	  int E = int(100 + (Cos - 0.40) * 40);
	  SunLightEmitation = MakeRGB24(E, E, E);
	  AmbientLuminance = MakeRGB24(E - 8, E - 8, E - 8);
	}
      else
	{
	  SunLightEmitation = 0;
	  AmbientLuminance = NightAmbientLuminance;
	}
    }
  else
    return;

  SunLightDirection = game::GetSunLightDirectionVector();
  ChangeSunLight();
}

void level::ChangeSunLight()
{
  bool SunSet = game::IsDark(SunLightEmitation);
  ulong c;

  for(c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->RemoveSunLight();

  if(!SunSet)
    EmitSunBeams();

  for(c = 0; c < XSizeTimesYSize; ++c)
    {
      lsquare* Square = Map[0][c];

      if(Square->Flags & IS_TRANSPARENT)
	Square->CalculateSunLightLuminance(EMITTER_SQUARE_PART_BITS);

      if(!Square->IsInside())
	Square->AmbientLuminance = AmbientLuminance;

      Square->SendSunLightSignals();
    }

  for(c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->CheckIfIsSecondarySunLightEmitter();
}

void level::InitSquarePartEmitationTicks()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->SquarePartEmitationTick = 0;
}

bool level::GenerateWindows(int X, int Y) const
{
  olterrain* Terrain = Map[X][Y]->GetOLTerrain();

  if(Terrain && Terrain->CreateWindowConfigurations() && !(RAND() % 6))
    {
      Terrain->SetConfig(Terrain->GetConfig() | WINDOW);
      Map[X][Y]->CalculateIsTransparent();
      return true;
    }

  return false;
}

struct sunbeamcontroller : public stackcontroller
{
  static bool Handler(int, int);
  static void ProcessStack();
  static ulong ID;
  static int SunLightBlockHeight;
  static vector2d SunLightBlockPos;
  static bool ReSunEmitation;
};

ulong sunbeamcontroller::ID;
int sunbeamcontroller::SunLightBlockHeight;
vector2d sunbeamcontroller::SunLightBlockPos;
bool sunbeamcontroller::ReSunEmitation;

void level::ForceEmitterNoxify(const emittervector& Emitter) const
{
  for(emittervector::const_iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    {
      ulong ID = i->ID;
      lsquare* Square = GetLSquare(ExtractPosFromEmitterID(ID));

      if(ID & SECONDARY_SUN_LIGHT)
	Square->Noxify(Square->SecondarySunLightEmitation, SECONDARY_SUN_LIGHT);
      else
	Square->Noxify(Square->Emitation);
    }
}

void level::ForceEmitterEmitation(const emittervector& Emitter, const sunemittervector& SunEmitter, ulong IDFlags) const
{
  for(emittervector::const_iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    {
      ulong ID = i->ID;
      lsquare* Square = GetLSquare(ExtractPosFromEmitterID(ID));

      if(ID & SECONDARY_SUN_LIGHT)
	Square->Emitate(Square->SecondarySunLightEmitation, SECONDARY_SUN_LIGHT|IDFlags);
      else
	Square->Emitate(Square->Emitation, IDFlags);
    }

  {
  stackcontroller::Map = Map;
  stackcontroller::Stack = SquareStack;
  stackcontroller::StackIndex = 0;
  stackcontroller::LevelXSize = XSize;
  stackcontroller::LevelYSize = YSize;
  sunbeamcontroller::ReSunEmitation = true;

  for(sunemittervector::const_iterator i = SunEmitter.begin(); i != SunEmitter.end(); ++i)
    {
      ulong ID = *i & ~(EMITTER_SHADOW_BITS|EMITTER_SQUARE_PART_BITS) | RE_SUN_EMITATED, SourceFlags;
      int X, Y;

      if(ID & ID_X_COORDINATE)
	{
	  X = (ID & EMITTER_IDENTIFIER_BITS) - (XSize << 3);
	  Y = ID & ID_BEGIN ? -1 : YSize;
	  SourceFlags = ID & ID_BEGIN ? SP_BOTTOM : SP_TOP;
	}
      else
	{
	  X = ID & ID_BEGIN ? -1 : XSize;
	  Y = (ID & EMITTER_IDENTIFIER_BITS) - (YSize << 3);
	  SourceFlags = ID & ID_BEGIN ? SP_RIGHT : SP_LEFT;
	}

      EmitSunBeam(vector2d(X, Y), ID, SourceFlags);
    }

  sunbeamcontroller::ProcessStack();
  }
}

struct loscontroller : public tickcontroller, public stackcontroller
{
  static int Handler(int x, int y)
  {
    lsquare* Square = Map[x >> 1][y >> 1];
    const ulong SquareFlags = Square->Flags;

    if(SquareFlags & PERFECTLY_QUADRI_HANDLED)
      return true;

    if(!(SquareFlags & IN_SQUARE_STACK))
      {
	Square->Flags |= IN_SQUARE_STACK;
	Stack[StackIndex++] = Square;
      }

    if(SquareFlags & IS_TRANSPARENT)
      {
	Square->Flags |= PERFECTLY_QUADRI_HANDLED;
	return true;
      }

    const int SquarePartIndex = (x & 1) + ((y & 1) << 1);
    Square->SquarePartLastSeen = Square->SquarePartLastSeen
			       & ~SquarePartTickMask[SquarePartIndex]
			       | ShiftedTick[SquarePartIndex];
    return false;
  }
  static ulong& GetTickReference(int X, int Y)
  {
    return Map[X][Y]->SquarePartLastSeen;
  }
  static void ProcessStack()
  {
    for(long c = 0; c < StackIndex; ++c)
      Stack[c]->SignalSeen(Tick);
  }
};

void level::UpdateLOS()
{
  game::RemoveLOSUpdateRequest();
  stackcontroller::Map = Map;
  stackcontroller::Stack = SquareStack;
  stackcontroller::StackIndex = 0;
  tickcontroller::Tick = game::IncreaseLOSTick();
  tickcontroller::PrepareShiftedTick();
  int Radius = PLAYER->GetLOSRange();

  for(int c = 0; c < PLAYER->GetSquaresUnder(); ++c)
    mapmath<loscontroller>::DoQuadriArea(PLAYER->GetPos(c).X, PLAYER->GetPos(c).Y,
					 Radius * Radius, XSize, YSize);

  loscontroller::ProcessStack();

  if(PLAYER->StateIsActivated(INFRA_VISION))
    for(int c = 0; c < game::GetTeams(); ++c)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
	  (*i)->SendNewDrawRequest();
}

void level::EnableGlobalRain()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->EnableGlobalRain();
}

void level::DisableGlobalRain()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->DisableGlobalRain();
}

void level::InitLastSeen()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->InitLastSeen();
}

void level::EmitSunBeams()
{
  stackcontroller::Map = Map;
  stackcontroller::LevelXSize = XSize;
  stackcontroller::LevelYSize = YSize;
  sunbeamcontroller::ReSunEmitation = false;
  vector2d Dir = SunLightDirection;
  int x, y, X = 0, Y = 0, SourceFlags;
  ulong IDFlags;

  /* Do not try to understand the logic behind the starting points of
     sunbeams. I determined the formulas by trial and error since all
     understandable loops produced strange shapes for the shadows of
     either small of large objects probably due to rounding errors
     made during line calculations. */

  if(!Dir.X || (Dir.Y && abs(Dir.Y) < abs(Dir.X)))
    {
      if(Dir.Y > 0)
	{
	  Y = -1;
	  SourceFlags = SP_BOTTOM;
	  IDFlags = ID_X_COORDINATE|ID_BEGIN;
	}
      else
	{
	  Y = YSize;
	  SourceFlags = SP_TOP;
	  IDFlags = ID_X_COORDINATE;
	}
    }
  else
    {
      if(Dir.X > 0)
	{
	  X = -1;
	  SourceFlags = SP_RIGHT;
	  IDFlags = ID_BEGIN;
	}
      else
	{
	  X = XSize;
	  SourceFlags = SP_LEFT;
	  IDFlags = 0;
	}
    }

  if(!Dir.X)
    {
      int Index = XSize << 3;

      for(x = 0; x < XSize; ++x, ++Index)
	EmitSunBeam(vector2d(x, Y), Index | IDFlags, SourceFlags);
    }
  else if(!Dir.Y)
    {
      int Index = YSize << 3;

      for(y = 0; y < YSize; ++y, ++Index)
	EmitSunBeam(vector2d(X, y), Index | IDFlags, SourceFlags);
    }
  else if(abs(Dir.Y) < abs(Dir.X))
    {
      int Index = Dir.X > 0 ? 0 : XSize << 3;
      int StartX = Dir.X > 0 ? -XSize << 3 : 0;
      int EndX = Dir.X > 0 ? XSize : (XSize << 3) + XSize;

      for(x = StartX; x < EndX; ++x, ++Index)
	EmitSunBeam(vector2d(x, Y), Index | IDFlags, SourceFlags);
    }
  else
    {
      int Index = Dir.Y > 0 ? 0 : YSize << 3;
      int StartY = Dir.Y > 0 ? -YSize << 3 : 0;
      int EndY = Dir.Y > 0 ? YSize : (YSize << 3) + YSize;

      for(y = StartY; y < EndY; ++y, ++Index)
	EmitSunBeam(vector2d(X, y), Index | IDFlags, SourceFlags);
    }
}

void level::EmitSunBeam(vector2d S, ulong ID, int SourceFlags) const
{
  S <<= 1;
  vector2d D = S + SunLightDirection;
  sunbeamcontroller::ID = ID;

  if(SourceFlags & SP_TOP_LEFT)
    {
      sunbeamcontroller::SunLightBlockHeight = 0;
      mapmath<sunbeamcontroller>::DoLine(S.X,     S.Y,     D.X,     D.Y, SKIP_FIRST);
    }

  if(SourceFlags & SP_TOP_RIGHT)
    {
      sunbeamcontroller::SunLightBlockHeight = 0;
      mapmath<sunbeamcontroller>::DoLine(S.X + 1, S.Y,     D.X + 1, D.Y, SKIP_FIRST);
    }

  if(SourceFlags & SP_BOTTOM_LEFT)
    {
      sunbeamcontroller::SunLightBlockHeight = 0;
      mapmath<sunbeamcontroller>::DoLine(S.X,     S.Y + 1, D.X,     D.Y + 1, SKIP_FIRST);
    }

  if(SourceFlags & SP_BOTTOM_RIGHT)
    {
      sunbeamcontroller::SunLightBlockHeight = 0;
      mapmath<sunbeamcontroller>::DoLine(S.X + 1, S.Y + 1, D.X + 1, D.Y + 1, SKIP_FIRST);
    }
}

bool sunbeamcontroller::Handler(int x, int y)
{
  int X = x >> 1, Y = y >> 1;

  if(X < 0 || Y < 0 || X >= LevelXSize || Y >= LevelYSize)
    return (X >= -1 && X <= LevelXSize) || (Y >= -1 && Y <= LevelYSize);

  lsquare* Square = Map[X][Y];
  int SquarePartIndex = (x & 1) + ((y & 1) << 1);

  if(SunLightBlockHeight && !Square->IsInside()
  && HypotSquare(x - SunLightBlockPos.X, y - SunLightBlockPos.Y) > SunLightBlockHeight)
    SunLightBlockHeight = 0;

  if(!SunLightBlockHeight)
    {
      ulong Flag = 1 << EMITTER_SQUARE_PART_SHIFT << SquarePartIndex;
      Square->AddSunLightEmitter(ID | Flag);
    }
  else
    {
      ulong Flags = ((1 << EMITTER_SQUARE_PART_SHIFT)
		   | (1 << EMITTER_SHADOW_SHIFT))
		  << SquarePartIndex;

      Square->AddSunLightEmitter(ID | Flags);
    }

  if(ReSunEmitation)
    {
      if(!(Square->Flags & IN_SQUARE_STACK))
	Stack[StackIndex++] = Square;

      Square->Flags |= IN_SQUARE_STACK|CHECK_SUN_LIGHT_NEEDED;

      for(int d = 0; d < 8; ++d)
	{
	  lsquare* Neighbour = Square->GetNeighbourLSquare(d);

	  if(Neighbour && !(Neighbour->Flags & IN_SQUARE_STACK))
	    {
	      Neighbour->Flags |= IN_SQUARE_STACK;
	      Stack[StackIndex++] = Neighbour;
	    }
	}
    }

  if(!(Square->Flags & IS_TRANSPARENT) || (SunLightBlockHeight && Square->IsInside()))
    {
      /* This should depend on the square */
      SunLightBlockHeight = 81;
      SunLightBlockPos = vector2d(x, y);
    }

  return true;
}

void sunbeamcontroller::ProcessStack()
{
  long c;

  for(c = 0; c < StackIndex; ++c)
    {
      lsquare* Square = Stack[c];

      if(Square->Flags & CHECK_SUN_LIGHT_NEEDED)
	{
	  if(Square->Flags & IS_TRANSPARENT)
	    Square->CalculateSunLightLuminance(EMITTER_SQUARE_PART_BITS);

	  Square->SendSunLightSignals();
	  Square->ZeroReSunEmitatedFlags();
	}

      Square->Flags &= ~(IN_SQUARE_STACK|CHECK_SUN_LIGHT_NEEDED);
    }

  for(c = 0; c < StackIndex; ++c)
    Stack[c]->CheckIfIsSecondarySunLightEmitter();
}

int level::DetectMaterial(const material* Material)
{
  ulong Tick = game::IncreaseLOSTick();
  int Squares = 0;

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	lsquare* Square = Map[x][y];

	if(Square->DetectMaterial(Material))
	  {
	    Square->Reveal(Tick, true);
	    ++Squares;
	  }
      }

  return Squares;
}

void level::BlurMemory()
{
  int x, y, SquareStackSize = 0;

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      {
	lsquare* Square = Map[x][y];

	if(Square->HasNoBorderPartners())
	  SquareStack[SquareStackSize++] = Square;
      }

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      {
	lsquare* Square = Map[x][y];
	Square->Flags |= STRONG_NEW_DRAW_REQUEST
		       | MEMORIZED_UPDATE_REQUEST
		       | DESCRIPTION_CHANGE;

	if(Square->HasNoBorderPartners()
	&& RAND() & 1
	&& SquareStackSize)
	  Square->SwapMemorized(SquareStack[RAND() % SquareStackSize]);
	else if(RAND() & 1)
	  Square->DestroyMemorized();
      }
}

void level::CalculateLuminances()
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	lsquare* Square = Map[x][y];
	Square->CalculateLuminance();
	Square->Flags |= MEMORIZED_UPDATE_REQUEST
		       | DESCRIPTION_CHANGE;
      }
}

struct areacontroller : public stackcontroller
{
  static bool Handler(int x, int y)
  {
    if(x >= 0 && y >= 0 && x < LevelXSize && y < LevelYSize
    && HypotSquare(x - Center.X, y - Center.Y) <= RadiusSquare)
      {
	lsquare* Square = Map[x][y];

	if(!(Square->Flags & IN_SQUARE_STACK))
	  {
	    Stack[StackIndex++] = Square;
	    Square->Flags |= IN_SQUARE_STACK;
	    return Square->IsFlyable();
	  }
      }

    return false;
  }
  static int GetStartX(int) { return Center.X; }
  static int GetStartY(int) { return Center.Y; }
  static long RadiusSquare;
};

long areacontroller::RadiusSquare;

int level::AddRadiusToSquareStack(vector2d Center, long RadiusSquare) const
{
  stackcontroller::Map = Map;
  stackcontroller::Stack = SquareStack;
  SquareStack[0] = GetLSquare(Center);
  stackcontroller::StackIndex = 1;
  stackcontroller::LevelXSize = XSize;
  stackcontroller::LevelYSize = YSize;
  stackcontroller::Center = Center;
  areacontroller::RadiusSquare = RadiusSquare;
  mapmath<areacontroller>::DoArea();
  return stackcontroller::StackIndex;
}

/* Any fountain is good that is not dry and is NOT Except */ 

olterrain* level::GetRandomFountainWithWater(olterrain* Except) const
{
  std::vector<olterrain*> Found;
  olterrain* OLTerrain;
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	OLTerrain = GetLSquare(x,y)->GetOLTerrain();
	if(OLTerrain && OLTerrain != Except && OLTerrain->IsFountainWithWater())
	  Found.push_back(OLTerrain);
      }

  if(Found.empty())
    return 0;

  return Found[RAND_N(Found.size())];
}

void level::Amnesia(int Percentile)
{
  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	lsquare* Square = Map[x][y];

	if(Square->HasNoBorderPartners() && RAND_N(100) < Percentile)
	  {
	    Square->Flags |= STRONG_NEW_DRAW_REQUEST
			   | MEMORIZED_UPDATE_REQUEST
			   | DESCRIPTION_CHANGE;

	    Square->DestroyMemorized();
	  }
      }
}
