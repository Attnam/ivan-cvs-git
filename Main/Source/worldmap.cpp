#include <cmath>
#include <cstring>

#include "worldmap.h"
#include "wsquare.h"
#include "wterrade.h"
#include "charba.h"
#include "allocate.h"
#include "error.h"
#include "dungeon.h"
#include "femath.h"
#include "proto.h"
#include "save.h"

#define MAX_TEMPERATURE			27		//increase for more tropical world
#define LATITUDE_EFFECT			40		//increase for more effect
#define ALTITUDE_EFFECT			0.02

#define COLD				10
#define MEDIUM				12
#define WARM				17
#define HOT				19

worldmap::worldmap(ushort XSize, ushort YSize) : area(XSize, YSize)
{
  Map = reinterpret_cast<wsquare***>(area::Map);

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      {
	Map[x][y] = new wsquare(this, vector2d(x, y));
	Map[x][y]->SetWTerrain(new ocean, new atmosphere);
      }

  TypeBuffer = Alloc2D<ushort>(XSize, YSize);
  AltitudeBuffer = Alloc2D<short>(XSize, YSize);
  ContinentBuffer = Alloc2D<uchar>(XSize, YSize);
  continent::TypeBuffer = TypeBuffer;
  continent::AltitudeBuffer = AltitudeBuffer;
  continent::ContinentBuffer = ContinentBuffer;
}

worldmap::~worldmap()
{
  delete [] TypeBuffer;
  delete [] AltitudeBuffer;
  delete [] ContinentBuffer;

  ushort c;

  for(c = 1; c < Continent.size(); ++c)
    delete Continent[c];

  for(c = 0; c < PlayerGroup.size(); ++c)
    delete PlayerGroup[c];
}

void worldmap::Save(outputfile& SaveFile) const
{
  area::Save(SaveFile);
  SaveFile.Write(reinterpret_cast<char*>(TypeBuffer[0]), sizeof(ushort) * XSizeTimesYSize);
  SaveFile.Write(reinterpret_cast<char*>(AltitudeBuffer[0]), sizeof(short) * XSizeTimesYSize);
  SaveFile.Write(reinterpret_cast<char*>(ContinentBuffer[0]), sizeof(uchar) * XSizeTimesYSize);

  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->Save(SaveFile);

  SaveFile << Continent << PlayerGroup;
}

void worldmap::Load(inputfile& SaveFile)
{
  area::Load(SaveFile);
  Map = reinterpret_cast<wsquare***>(area::Map);
  TypeBuffer = Alloc2D<ushort>(XSize, YSize);
  AltitudeBuffer = Alloc2D<short>(XSize, YSize);
  ContinentBuffer = Alloc2D<uchar>(XSize, YSize);
  SaveFile.Read(reinterpret_cast<char*>(TypeBuffer[0]), sizeof(ushort) * XSizeTimesYSize);
  SaveFile.Read(reinterpret_cast<char*>(AltitudeBuffer[0]), sizeof(short) * XSizeTimesYSize);
  SaveFile.Read(reinterpret_cast<char*>(ContinentBuffer[0]), sizeof(uchar) * XSizeTimesYSize);
  continent::TypeBuffer = TypeBuffer;
  continent::AltitudeBuffer = AltitudeBuffer;
  continent::ContinentBuffer = ContinentBuffer;

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      {
	Map[x][y] = new wsquare(this, vector2d(x, y));
	game::SetSquareInLoad(Map[x][y]);
	Map[x][y]->Load(SaveFile);
      }

  CalculateNeighbourBitmapPoses();
  SaveFile >> Continent >> PlayerGroup;
}

void worldmap::Generate()
{
  OldAltitudeBuffer = Alloc2D<short>(XSize, YSize);
  OldTypeBuffer = Alloc2D<ushort>(XSize, YSize);

  while(true)
    {
      RandomizeAltitude();
      SmoothAltitude();
      GenerateClimate();
      SmoothClimate();
      CalculateContinents();
      std::vector<continent*> PerfectForAttnam, PerfectForNewAttnam;

      for(ushort c = 1; c < Continent.size(); ++c)
	if(Continent[c]->GetSize() > 25 && Continent[c]->GetSize() < 1000 && Continent[c]->GetGTerrainAmount(evergreenforest::StaticType()) && Continent[c]->GetGTerrainAmount(snow::StaticType()))
	  PerfectForAttnam.push_back(Continent[c]);

      if(!PerfectForAttnam.size())
	continue;

      vector2d AttnamPos, ElpuriCavePos, NewAttnamPos, TunnelEntry, TunnelExit;
      bool Correct = false;
      continent* PetrusLikes;

      for(ushort c1 = 0; c1 < 25; ++c1)
	{
	  game::BusyAnimation();
	  PetrusLikes = PerfectForAttnam[RAND() % PerfectForAttnam.size()];
	  AttnamPos = PetrusLikes->GetRandomMember(evergreenforest::StaticType());
	  ElpuriCavePos = PetrusLikes->GetRandomMember(snow::StaticType());

	  for(ushort c2 = 1; c2 < 50; ++c2)
	    {
	      TunnelExit = PetrusLikes->GetMember(RAND() % PetrusLikes->GetSize());

	      if(AttnamPos != TunnelExit && ElpuriCavePos != TunnelExit)
		{
		  for(ushort d1 = 0; d1 < 8; ++d1)
		    {
		      vector2d Pos = TunnelExit + game::GetMoveVector(d1);

		      if(IsValidPos(Pos) && AltitudeBuffer[Pos.X][Pos.Y] <= 0)
			{
			  ushort Distance = 3 + (RAND() & 3);
			  bool Error = false;
			  TunnelEntry = Pos;

			  for(ushort c2 = 0; c2 < Distance; ++c2)
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

			  short x, y;
			  ushort Counter = 0;

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
			  ushort NewAttnamIndex;

			  for(NewAttnamIndex = RAND() & 7; NewAttnamIndex == 7 - d1; NewAttnamIndex = RAND() & 7);

			  NewAttnamPos = TunnelEntry + game::GetMoveVector(NewAttnamIndex);
			  static ushort DiagonalDir[4] = { 0, 2, 5, 7 };
			  static ushort NotDiagonalDir[4] = { 1, 3, 4, 6 };
			  static ushort AdjacentDir[4][2] = { { 0, 1 }, { 0, 2 }, { 1, 3 }, { 2, 3 } };
			  bool Raised[] = { false, false, false, false };
			  ushort d2;

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

      vector2d MondedrPos;

      do
	MondedrPos = PetrusLikes->GetMember(RAND() % PetrusLikes->GetSize());
      while(MondedrPos == AttnamPos || MondedrPos == ElpuriCavePos || MondedrPos == TunnelExit);

      GetWSquare(MondedrPos)->ChangeOWTerrain(new mondedr);
      SetEntryPos(MONDEDR, MondedrPos);

      GetWSquare(AttnamPos)->ChangeOWTerrain(new attnam);
      SetEntryPos(ATTNAM, AttnamPos);
      RevealEnvironment(AttnamPos, 1);
      SetEntryPos(ELPURI_CAVE, ElpuriCavePos);
      GetWSquare(NewAttnamPos)->ChangeOWTerrain(new newattnam);
      SetEntryPos(NEW_ATTNAM, NewAttnamPos);
      GetWSquare(TunnelEntry)->ChangeOWTerrain(new underwatertunnel);
      SetEntryPos(UNDER_WATER_TUNNEL, TunnelEntry);
      GetWSquare(TunnelExit)->ChangeOWTerrain(new underwatertunnelexit);
      SetEntryPos(UNDER_WATER_TUNNEL_EXIT, TunnelExit);
      GetWSquare(NewAttnamPos)->AddCharacter(game::GetPlayer());
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

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      AltitudeBuffer[x][y] = 4000 - RAND() % 8000;
}

void worldmap::SmoothAltitude()
{
  for(ushort c = 0; c < 10; ++c)
    {
      game::BusyAnimation();
      ushort x, y;

      for(x = 0; x < XSize; ++x)
	SafeSmooth(x, 0);

      for(y = 1; y < YSize - 1; ++y)
	{
	  SafeSmooth(0, y);

	  for(x = 1; x < XSize - 1; ++x)
	    FastSmooth(x, y);

	  SafeSmooth(XSize - 1, y);
	}

      for(x = 0; x < XSize; ++x)
	SafeSmooth(x, YSize - 1);
    }
}

void worldmap::FastSmooth(ushort x, ushort y)
{
  long HeightNear = 0;
  ushort d;

  for(d = 0; d < 4; ++d)
    {
      vector2d Vect = game::GetMoveVector(d);
      HeightNear += OldAltitudeBuffer[x + Vect.X][y + Vect.Y];
    }

  for(d = 4; d < 8; ++d)
    {
      vector2d Vect = game::GetMoveVector(d);
      HeightNear += AltitudeBuffer[x + Vect.X][y + Vect.Y];
    }

  OldAltitudeBuffer[x][y] = AltitudeBuffer[x][y];
  AltitudeBuffer[x][y] = HeightNear >> 3;
}

void worldmap::SafeSmooth(ushort x, ushort y)
{
  long HeightNear = 0;
  ushort d, SquaresNear = 0;

  for(d = 0; d < 4; ++d)
    {
      vector2d Vect = game::GetMoveVector(d);
      ushort X = x + Vect.X;
      ushort Y = y + Vect.Y;

      if(IsValidPos(X, Y))
	{
	  HeightNear += OldAltitudeBuffer[X][Y];
	  ++SquaresNear;
	}
    }

  for(d = 4; d < 8; ++d)
    {
      vector2d Vect = game::GetMoveVector(d);
      ushort X = x + Vect.X;
      ushort Y = y + Vect.Y;

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
  for(ushort y = 0; y < YSize; ++y)
    {
      game::BusyAnimation();
      float DistanceFromEquator = fabs(float(y) / YSize - 0.5f);
      bool LatitudeRainy = DistanceFromEquator <= 0.05 || (DistanceFromEquator > 0.25 && DistanceFromEquator <= 0.45);

      for(ushort x = 0; x < XSize; ++x)
	{
	  if(AltitudeBuffer[x][y] <= 0)
	    {
	      TypeBuffer[x][y] = ocean::StaticType();
	      continue;
	    }

	  bool Rainy = LatitudeRainy;

	  if(!Rainy)
	    for(ushort d = 0; d < 8; ++d)
	      {
		vector2d Pos = vector2d(x, y) + game::GetMoveVector(d);

		if(IsValidPos(Pos) && AltitudeBuffer[Pos.X][Pos.Y] <= 0)
		  {
		    Rainy = true;
		    break;
		  }
	      }

	  char Temperature = char(MAX_TEMPERATURE - DistanceFromEquator * LATITUDE_EFFECT - AltitudeBuffer[x][y] * ALTITUDE_EFFECT);

	  if(Temperature <= COLD)
	    if(Rainy)
	      TypeBuffer[x][y] = snow::StaticType();
	    else
	      TypeBuffer[x][y] = glacier::StaticType();

	  if(Temperature > COLD && Temperature <= MEDIUM)
	    if(Rainy)
	      TypeBuffer[x][y] = evergreenforest::StaticType();
	    else
	      TypeBuffer[x][y] = snow::StaticType();

	  if(Temperature > MEDIUM && Temperature <= WARM)
	    if(Rainy)
	      TypeBuffer[x][y] = leafyforest::StaticType();
	    else
	      TypeBuffer[x][y] = steppe::StaticType();

	  if(Temperature > WARM && Temperature <= HOT)
	    if(Rainy)
	      TypeBuffer[x][y] = leafyforest::StaticType();
	    else
	      TypeBuffer[x][y] = desert::StaticType();

	  if(Temperature > HOT)
	    if(Rainy)
	      TypeBuffer[x][y] = jungle::StaticType();
	    else
	      TypeBuffer[x][y] = desert::StaticType();
	}
    }
}

void worldmap::SmoothClimate()
{
  for(ushort c = 0; c < 3; ++c)
    {
      game::BusyAnimation();

      for(ushort y = 0; y < YSize; ++y)
	for(ushort x = 0; x < XSize; ++x)
	  if((OldTypeBuffer[x][y] = TypeBuffer[x][y]) != ocean::StaticType())
	    TypeBuffer[x][y] = WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(x, y);
    }

  game::BusyAnimation();

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      Map[x][y]->ChangeWTerrain(protocontainer<gwterrain>::GetProto(TypeBuffer[x][y])->Clone(), new atmosphere);
}

/* Evil... */

#define ANALYZE_TYPE(type)\
{\
  ushort T = type;\
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

ushort worldmap::WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(ushort x, ushort y)
{
  ushort UsedType[9];
  ushort TypeAmount[9];
  ushort c, d, u = 1;

  UsedType[0] = TypeBuffer[x][y];
  TypeAmount[0] = 1;

  for(d = 0; d < 4; ++d)
    {
      vector2d Vect = game::GetMoveVector(d);
      ushort X = x + Vect.X;
      ushort Y = y + Vect.Y;

      if(IsValidPos(X, Y))
	ANALYZE_TYPE(OldTypeBuffer[X][Y]);
    }

  for(d = 4; d < 8; ++d)
    {
      vector2d Vect = game::GetMoveVector(d);
      ushort X = x + Vect.X;
      ushort Y = y + Vect.Y;

      if(IsValidPos(X, Y))
	ANALYZE_TYPE(TypeBuffer[X][Y]);
    }

  ushort MostCommon = 0;

  for(c = 1; c < u; ++c)
    if(TypeAmount[c] > TypeAmount[MostCommon] && UsedType[c] != ocean::StaticType())
      MostCommon = c;

  return UsedType[MostCommon];
}

void worldmap::CalculateContinents()
{
  ushort c;

  for(c = 1; c < Continent.size(); ++c)
    delete Continent[c];

  Continent.resize(1, 0);
  memset(ContinentBuffer[0], 0, XSize * YSize * sizeof(uchar));

  for(ushort x = 0; x < XSize; ++x)
    {
      game::BusyAnimation();

	for(ushort y = 0; y < YSize; ++y)
	  if(AltitudeBuffer[x][y] > 0)
	    {
	      bool Attached = false;

	      for(ushort d = 0; d < 8; ++d)
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
    }

  RemoveEmptyContinents();

  for(c = 1; c < Continent.size(); ++c)
    Continent[c]->GenerateInfo();
}

void worldmap::RemoveEmptyContinents()
{
  for(ushort c = 1; c < Continent.size(); ++c)
    if(!Continent[c]->GetSize())
      for(ushort i = Continent.size() - 1; i >= c; i--)
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

void worldmap::Draw() const
{
  if(!game::SeeWholeMapCheatIsActive())
    {
      for(ushort x = game::GetCamera().X; x < XSize && x < game::GetCamera().X + game::GetScreenSize().X; ++x)
	for(ushort y = game::GetCamera().Y; y < YSize && y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
	  if(Map[x][y]->GetLastSeen())
	    Map[x][y]->Draw();
    }
  else
    {
      for(ushort x = game::GetCamera().X; x < XSize && x < game::GetCamera().X + game::GetScreenSize().X; ++x)
	for(ushort y = game::GetCamera().Y; y < YSize && y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
	  Map[x][y]->Draw();
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

wsquare* worldmap::GetNeighbourWSquare(vector2d Pos, ushort Index) const
{
  Pos += game::GetMoveVector(Index);

  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize)
    return Map[Pos.X][Pos.Y];
  else
    return 0;
}

void worldmap::RevealEnvironment(vector2d Pos, ushort Radius)
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, Border, Pos, Radius);
  ulong LastSeen = Max<ulong>(game::GetLOSTurns(), 1);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      Map[x][y]->SetLastSeen(LastSeen);
}
