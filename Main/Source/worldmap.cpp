/* Compiled through wmapset.cpp */

#define MAX_TEMPERATURE			27		//increase for more tropical world
#define LATITUDE_EFFECT			40		//increase for more effect
#define ALTITUDE_EFFECT			0.02

#define COLD				10
#define MEDIUM				12
#define WARM				17
#define HOT				19

int DirX[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int DirY[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

worldmap::worldmap() { }
continent* worldmap::GetContinentUnder(vector2d Pos) const { return Continent[ContinentBuffer[Pos.X][Pos.Y]]; }
vector2d worldmap::GetEntryPos(const character*, int I) const { return EntryMap.find(I)->second; }
continent* worldmap::GetContinent(int I) const { return Continent[I]; }
int worldmap::GetAltitude(vector2d Pos) { return AltitudeBuffer[Pos.X][Pos.Y]; }
charactervector& worldmap::GetPlayerGroup() { return PlayerGroup; }
character* worldmap::GetPlayerGroupMember(int c) { return PlayerGroup[c]; }

worldmap::worldmap(int XSize, int YSize) : area(XSize, YSize)
{
  Map = reinterpret_cast<wsquare***>(area::Map);

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      {
	Map[x][y] = new wsquare(this, vector2d(x, y));
	Map[x][y]->SetGWTerrain(new ocean);
      }

  Alloc2D(TypeBuffer, XSize, YSize);
  Alloc2D(AltitudeBuffer, XSize, YSize);
  Alloc2D(ContinentBuffer, XSize, YSize);
  continent::TypeBuffer = TypeBuffer;
  continent::AltitudeBuffer = AltitudeBuffer;
  continent::ContinentBuffer = ContinentBuffer;
}

worldmap::~worldmap()
{
  delete [] TypeBuffer;
  delete [] AltitudeBuffer;
  delete [] ContinentBuffer;

  uint c;

  for(c = 1; c < Continent.size(); ++c)
    delete Continent[c];

  for(c = 0; c < PlayerGroup.size(); ++c)
    delete PlayerGroup[c];
}

void worldmap::Save(outputfile& SaveFile) const
{
  area::Save(SaveFile);
  SaveFile.Write(reinterpret_cast<char*>(TypeBuffer[0]), XSizeTimesYSize * sizeof(uchar));
  SaveFile.Write(reinterpret_cast<char*>(AltitudeBuffer[0]), XSizeTimesYSize * sizeof(short));
  SaveFile.Write(reinterpret_cast<char*>(ContinentBuffer[0]), XSizeTimesYSize * sizeof(uchar));

  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->Save(SaveFile);

  SaveFile << Continent << PlayerGroup;
}

void worldmap::Load(inputfile& SaveFile)
{
  area::Load(SaveFile);
  Map = reinterpret_cast<wsquare***>(area::Map);
  Alloc2D(TypeBuffer, XSize, YSize);
  Alloc2D(AltitudeBuffer, XSize, YSize);
  Alloc2D(ContinentBuffer, XSize, YSize);
  SaveFile.Read(reinterpret_cast<char*>(TypeBuffer[0]), XSizeTimesYSize * sizeof(uchar));
  SaveFile.Read(reinterpret_cast<char*>(AltitudeBuffer[0]), XSizeTimesYSize * sizeof(short));
  SaveFile.Read(reinterpret_cast<char*>(ContinentBuffer[0]), XSizeTimesYSize * sizeof(uchar));
  continent::TypeBuffer = TypeBuffer;
  continent::AltitudeBuffer = AltitudeBuffer;
  continent::ContinentBuffer = ContinentBuffer;
  int x, y;

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      Map[x][y] = new wsquare(this, vector2d(x, y));

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      {
	game::SetSquareInLoad(Map[x][y]);
	Map[x][y]->Load(SaveFile);
      }

  CalculateNeighbourBitmapPoses();
  SaveFile >> Continent >> PlayerGroup;
}

void worldmap::Generate()
{
  Alloc2D(OldAltitudeBuffer, XSize, YSize);
  Alloc2D(OldTypeBuffer, XSize, YSize);

  for(;;)
    {
      RandomizeAltitude();
      SmoothAltitude();
      GenerateClimate();
      SmoothClimate();
      CalculateContinents();
      std::vector<continent*> PerfectForAttnam, PerfectForNewAttnam;

      for(uint c = 1; c < Continent.size(); ++c)
	if(Continent[c]->GetSize() > 25 && Continent[c]->GetSize() < 1000 && Continent[c]->GetGTerrainAmount(evergreenforest::StaticType()) && Continent[c]->GetGTerrainAmount(snow::StaticType()))
	  PerfectForAttnam.push_back(Continent[c]);

      if(!PerfectForAttnam.size())
	continue;

      vector2d AttnamPos, ElpuriCavePos, NewAttnamPos, TunnelEntry, TunnelExit;
      bool Correct = false;
      continent* PetrusLikes;

      for(int c1 = 0; c1 < 25; ++c1)
	{
	  game::BusyAnimation();
	  PetrusLikes = PerfectForAttnam[RAND() % PerfectForAttnam.size()];
	  AttnamPos = PetrusLikes->GetRandomMember(evergreenforest::StaticType());
	  ElpuriCavePos = PetrusLikes->GetRandomMember(snow::StaticType());

	  for(int c2 = 1; c2 < 50; ++c2)
	    {
	      TunnelExit = PetrusLikes->GetMember(RAND() % PetrusLikes->GetSize());

	      if(AttnamPos != TunnelExit && ElpuriCavePos != TunnelExit)
		{
		  for(int d1 = 0; d1 < 8; ++d1)
		    {
		      vector2d Pos = TunnelExit + game::GetMoveVector(d1);

		      if(IsValidPos(Pos) && AltitudeBuffer[Pos.X][Pos.Y] <= 0)
			{
			  int Distance = 3 + (RAND() & 3);
			  bool Error = false;
			  TunnelEntry = Pos;

			  for(int c2 = 0; c2 < Distance; ++c2)
			    {
			      TunnelEntry += game::GetMoveVector(d1);

			      if(!IsValidPos(TunnelEntry) || AltitudeBuffer[TunnelEntry.X][TunnelEntry.Y] > 0)
				{
				  Error = true;
				  break;
				}
			    }

			  if(Error)
			    continue;

			  int x, y;
			  int Counter = 0;

			  for(x = TunnelEntry.X - 3; x <= TunnelEntry.X + 3; ++x)
			    {
			      for(y = TunnelEntry.Y - 3; y <= TunnelEntry.Y + 3; ++y, ++Counter)
				if(Counter != 0 && Counter != 6 && Counter != 42 && Counter != 48
				&&(!IsValidPos(x, y) || AltitudeBuffer[x][y] > 0 || AltitudeBuffer[x][y] < -350))
				  {
				    Error = true;
				    break;
				  }

			      if(Error)
				break;
			    }

			  if(Error)
			    continue;

			  Error = true;

			  for(x = 0; x < XSize; ++x)
			    if(TypeBuffer[x][TunnelEntry.Y] == jungle::StaticType())
			      {
				Error = false;
				break;
			      }

			  if(Error)
			    continue;

			  Counter = 0;

			  for(x = TunnelEntry.X - 2; x <= TunnelEntry.X + 2; ++x)
			    for(y = TunnelEntry.Y - 2; y <= TunnelEntry.Y + 2; ++y, ++Counter)
			      if(Counter != 0 && Counter != 4 && Counter != 20 && Counter != 24)
				AltitudeBuffer[x][y] /= 2;

			  AltitudeBuffer[TunnelEntry.X][TunnelEntry.Y] = 1 + RAND() % 50;
			  TypeBuffer[TunnelEntry.X][TunnelEntry.Y] = jungle::StaticType();
			  GetWSquare(TunnelEntry)->ChangeGWTerrain(new jungle);
			  int NewAttnamIndex;

			  for(NewAttnamIndex = RAND() & 7; NewAttnamIndex == 7 - d1; NewAttnamIndex = RAND() & 7);

			  NewAttnamPos = TunnelEntry + game::GetMoveVector(NewAttnamIndex);
			  static int DiagonalDir[4] = { 0, 2, 5, 7 };
			  static int NotDiagonalDir[4] = { 1, 3, 4, 6 };
			  static int AdjacentDir[4][2] = { { 0, 1 }, { 0, 2 }, { 1, 3 }, { 2, 3 } };
			  bool Raised[] = { false, false, false, false };
			  int d2;

			  for(d2 = 0; d2 < 4; ++d2)
			    if(NotDiagonalDir[d2] != 7 - d1 && (NotDiagonalDir[d2] == NewAttnamIndex || !(RAND() & 2)))
			      {
				vector2d Pos = TunnelEntry + game::GetMoveVector(NotDiagonalDir[d2]);
				AltitudeBuffer[Pos.X][Pos.Y] = 1 + RAND() % 50;
				TypeBuffer[Pos.X][Pos.Y] = jungle::StaticType();
				GetWSquare(Pos)->ChangeGWTerrain(new jungle);
				Raised[d2] = true;
			      }

			  for(d2 = 0; d2 < 4; ++d2)
			    if(DiagonalDir[d2] != 7 - d1 && (DiagonalDir[d2] == NewAttnamIndex || (Raised[AdjacentDir[d2][0]] && Raised[AdjacentDir[d2][1]] && !(RAND() & 2))))
			      {
				vector2d Pos = TunnelEntry + game::GetMoveVector(DiagonalDir[d2]);
				AltitudeBuffer[Pos.X][Pos.Y] = 1 + RAND() % 50;
				TypeBuffer[Pos.X][Pos.Y] = jungle::StaticType();
				GetWSquare(Pos)->ChangeGWTerrain(new jungle);
			      }

			  Correct = true;
			  break;
			}
		    }

		  if(Correct)
		    break;
		}
	    }

	  if(Correct)
	    break;
	}

      if(!Correct)
	continue;

      GetWSquare(AttnamPos)->ChangeOWTerrain(new attnam);
      SetEntryPos(ATTNAM, AttnamPos);
      RevealEnvironment(AttnamPos, 1);
      GetWSquare(NewAttnamPos)->ChangeOWTerrain(new newattnam);
      SetEntryPos(NEW_ATTNAM, NewAttnamPos);
      SetEntryPos(ELPURI_CAVE, ElpuriCavePos);
      GetWSquare(TunnelEntry)->ChangeOWTerrain(new underwatertunnel);
      SetEntryPos(UNDER_WATER_TUNNEL, TunnelEntry);
      GetWSquare(TunnelExit)->ChangeOWTerrain(new underwatertunnelexit);
      SetEntryPos(UNDER_WATER_TUNNEL_EXIT, TunnelExit);
      PLAYER->PutTo(NewAttnamPos);
      CalculateLuminances();
      CalculateNeighbourBitmapPoses();
      break;
    }

  delete [] OldAltitudeBuffer;
  delete [] OldTypeBuffer;
}

void worldmap::RandomizeAltitude()
{
  game::BusyAnimation();

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      AltitudeBuffer[x][y] = 4000 - RAND() % 8000;
}

void worldmap::SmoothAltitude()
{
  for(int c = 0; c < 10; ++c)
    {
      game::BusyAnimation();
      int x, y;

      for(y = 0; y < YSize; ++y)
	SafeSmooth(0, y);

      for(x = 1; x < XSize - 1; ++x)
	{
	  SafeSmooth(x, 0);

	  for(y = 1; y < YSize - 1; ++y)
	    FastSmooth(x, y);

	  SafeSmooth(x, YSize - 1);
	}

      for(y = 0; y < YSize; ++y)
	SafeSmooth(XSize - 1, y);
    }
}

void worldmap::FastSmooth(int x, int y)
{
  long HeightNear = 0;
  int d;

  for(d = 0; d < 4; ++d)
    HeightNear += OldAltitudeBuffer[x + DirX[d]][y + DirY[d]];

  for(d = 4; d < 8; ++d)
    HeightNear += AltitudeBuffer[x + DirX[d]][y + DirY[d]];

  OldAltitudeBuffer[x][y] = AltitudeBuffer[x][y];
  AltitudeBuffer[x][y] = HeightNear >> 3;
}

void worldmap::SafeSmooth(int x, int y)
{
  long HeightNear = 0;
  int d, SquaresNear = 0;

  for(d = 0; d < 4; ++d)
    {
      int X = x + DirX[d];
      int Y = y + DirY[d];

      if(IsValidPos(X, Y))
	{
	  HeightNear += OldAltitudeBuffer[X][Y];
	  ++SquaresNear;
	}
    }

  for(d = 4; d < 8; ++d)
    {
      int X = x + DirX[d];
      int Y = y + DirY[d];

      if(IsValidPos(X, Y))
	{
	  HeightNear += AltitudeBuffer[X][Y];
	  ++SquaresNear;
	}
    }

  OldAltitudeBuffer[x][y] = AltitudeBuffer[x][y];
  AltitudeBuffer[x][y] = HeightNear / SquaresNear;
}

void worldmap::GenerateClimate()
{
  game::BusyAnimation();

  for(int y = 0; y < YSize; ++y)
    {
      double DistanceFromEquator = fabs(double(y) / YSize - 0.5);
      bool LatitudeRainy = DistanceFromEquator <= 0.05 || (DistanceFromEquator > 0.25 && DistanceFromEquator <= 0.45);

      for(int x = 0; x < XSize; ++x)
	{
	  if(AltitudeBuffer[x][y] <= 0)
	    {
	      TypeBuffer[x][y] = ocean::StaticType();
	      continue;
	    }

	  bool Rainy = LatitudeRainy;

	  if(!Rainy)
	    for(int d = 0; d < 8; ++d)
	      {
		vector2d Pos = vector2d(x, y) + game::GetMoveVector(d);

		if(IsValidPos(Pos) && AltitudeBuffer[Pos.X][Pos.Y] <= 0)
		  {
		    Rainy = true;
		    break;
		  }
	      }

	  int Temperature = int(MAX_TEMPERATURE - DistanceFromEquator * LATITUDE_EFFECT - AltitudeBuffer[x][y] * ALTITUDE_EFFECT);
	  int Type = 0;

	  if(Temperature <= COLD)
	    if(Rainy)
	      Type = snow::StaticType();
	    else
	      Type = glacier::StaticType();

	  if(Temperature > COLD && Temperature <= MEDIUM)
	    if(Rainy)
	      Type = evergreenforest::StaticType();
	    else
	      Type = snow::StaticType();

	  if(Temperature > MEDIUM && Temperature <= WARM)
	    if(Rainy)
	      Type = leafyforest::StaticType();
	    else
	      Type = steppe::StaticType();

	  if(Temperature > WARM && Temperature <= HOT)
	    if(Rainy)
	      Type = leafyforest::StaticType();
	    else
	      Type = desert::StaticType();

	  if(Temperature > HOT)
	    if(Rainy)
	      Type = jungle::StaticType();
	    else
	      Type = desert::StaticType();

	  TypeBuffer[x][y] = Type;
	}
    }
}

void worldmap::SmoothClimate()
{
  for(int c = 0; c < 3; ++c)
    {
      game::BusyAnimation();

      for(int x = 0; x < XSize; ++x)
	for(int y = 0; y < YSize; ++y)
	  if((OldTypeBuffer[x][y] = TypeBuffer[x][y]) != ocean::StaticType())
	    TypeBuffer[x][y] = WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(x, y);
    }

  game::BusyAnimation();

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->ChangeGWTerrain(protocontainer<gwterrain>::GetProto(TypeBuffer[x][y])->Clone());
}

/* Evil... */

#define ANALYZE_TYPE(type)\
{\
  int T = type;\
  \
  for(c = 0; c < u; ++c)\
    if(T == UsedType[c])\
      {\
	++TypeAmount[c];\
	break;\
      }\
  \
  if(c == u)\
    {\
      UsedType[u] = T;\
      TypeAmount[u++] = 1;\
    }\
}

int worldmap::WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(int x, int y)
{
  int UsedType[9];
  int TypeAmount[9];
  int c, d, u = 1;

  UsedType[0] = TypeBuffer[x][y];
  TypeAmount[0] = 1;

  for(d = 0; d < 4; ++d)
    {
      int X = x + DirX[d];
      int Y = y + DirY[d];

      if(IsValidPos(X, Y))
	ANALYZE_TYPE(OldTypeBuffer[X][Y]);
    }

  for(d = 4; d < 8; ++d)
    {
      int X = x + DirX[d];
      int Y = y + DirY[d];

      if(IsValidPos(X, Y))
	ANALYZE_TYPE(TypeBuffer[X][Y]);
    }

  int MostCommon = 0;

  for(c = 1; c < u; ++c)
    if(TypeAmount[c] > TypeAmount[MostCommon] && UsedType[c] != ocean::StaticType())
      MostCommon = c;

  return UsedType[MostCommon];
}

void worldmap::CalculateContinents()
{
  uint c;

  for(c = 1; c < Continent.size(); ++c)
    delete Continent[c];

  Continent.resize(1, 0);
  memset(ContinentBuffer[0], 0, XSizeTimesYSize * sizeof(uchar));
  game::BusyAnimation();

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      if(AltitudeBuffer[x][y] > 0)
	{
	  bool Attached = false;

	  for(int d = 0; d < 8; ++d)
	    {
	      vector2d Pos = vector2d(x, y) + game::GetMoveVector(d);

	      if(IsValidPos(Pos) && ContinentBuffer[Pos.X][Pos.Y])
		{
		  if(ContinentBuffer[x][y])
		    {
		      if(ContinentBuffer[x][y] != ContinentBuffer[Pos.X][Pos.Y])
			if(Continent[ContinentBuffer[x][y]]->GetSize() < Continent[ContinentBuffer[Pos.X][Pos.Y]]->GetSize())
			  Continent[ContinentBuffer[x][y]]->AttachTo(Continent[ContinentBuffer[Pos.X][Pos.Y]]);
			else
			  Continent[ContinentBuffer[Pos.X][Pos.Y]]->AttachTo(Continent[ContinentBuffer[x][y]]);
		    }
		  else
		    Continent[ContinentBuffer[Pos.X][Pos.Y]]->Add(vector2d(x, y));

		  Attached = true;
		}
	    }

	  if(!Attached)
	    {
	      if(Continent.size() == 255)
		{
		  RemoveEmptyContinents();

		  if(Continent.size() == 255)
		    ABORT("Valpurus shall not carry more continents!");
		}

	      continent* NewContinent = new continent(Continent.size());
	      NewContinent->Add(vector2d(x, y));
	      Continent.push_back(NewContinent);
	    }
	}

  RemoveEmptyContinents();

  for(c = 1; c < Continent.size(); ++c)
    Continent[c]->GenerateInfo();
}

void worldmap::RemoveEmptyContinents()
{
  for(uint c = 1; c < Continent.size(); ++c)
    if(!Continent[c]->GetSize())
      for(uint i = Continent.size() - 1; i >= c; i--)
	if(Continent[i]->GetSize())
	  {
	    Continent[i]->AttachTo(Continent[c]);
	    delete Continent[i];
	    Continent.pop_back();
	    break;
	  }
	else
	  {
	    delete Continent[i];
	    Continent.pop_back();
	  }
}

void worldmap::Draw(bool) const
{
  const int XMin = game::GetCamera().X;
  const int YMin = game::GetCamera().Y;
  const int XMax = Min(XSize, game::GetCamera().X + game::GetScreenXSize());
  const int YMax = Min(YSize, game::GetCamera().Y + game::GetScreenYSize());

  if(!game::GetSeeWholeMapCheatMode())
    {
      for(int x = XMin; x < XMax; ++x)
	{
	  wsquare** Square = &Map[x][YMin];

	  for(int y = YMin; y < YMax; ++y, ++Square)
	    if((*Square)->LastSeen)
	      (*Square)->Draw();
	}
    }
  else
    {
      for(int x = XMin; x < XMax; ++x)
	{
	  wsquare** Square = &Map[x][YMin];

	  for(int y = YMin; y < YMax; ++y, ++Square)
	    (*Square)->Draw();
	}
    }
}

void worldmap::CalculateLuminances()
{
  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->CalculateLuminance();
}

void worldmap::CalculateNeighbourBitmapPoses()
{
  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->GetGWTerrain()->CalculateNeighbourBitmapPoses();
}

wsquare* worldmap::GetNeighbourWSquare(vector2d Pos, int I) const
{
  Pos += game::GetMoveVector(I);

  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize)
    return Map[Pos.X][Pos.Y];
  else
    return 0;
}

void worldmap::RevealEnvironment(vector2d Pos, int Radius)
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, Border, Pos, Radius);

  for(int x = Rect.X1; x <= Rect.X2; ++x)
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
      Map[x][y]->SignalSeen();
}

outputfile& operator<<(outputfile& SaveFile, const worldmap* WorldMap)
{
  WorldMap->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, worldmap*& WorldMap)
{
  WorldMap = new worldmap;
  WorldMap->Load(SaveFile);
  return SaveFile;
}

void worldmap::UpdateLOS()
{
  int Radius = PLAYER->GetLOSRange();
  long RadiusSquare = Radius * Radius;
  vector2d Pos = PLAYER->GetPos();
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, Border, Pos, Radius);

  for(int x = Rect.X1; x <= Rect.X2; ++x)
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
      if(long(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	Map[x][y]->SignalSeen();

  game::RemoveLOSUpdateRequest();
}
