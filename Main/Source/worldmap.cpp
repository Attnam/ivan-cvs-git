#include <cmath>

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
  Map = (wsquare***)area::Map;

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
  Map = (wsquare***)area::Map;
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
  while(true)
    {
      RandomizeAltitude();
      SmoothAltitude();
      GenerateClimate();
      SmoothClimate();
      CalculateContinents();
      std::vector<continent*> PerfectForAttnam;

      for(ushort c = 1; c < Continent.size(); ++c)
	if(Continent[c]->Size() > 50 && Continent[c]->Size() < 200 && Continent[c]->GetGTerrainAmount(evergreenforest::StaticType()) && Continent[c]->GetGTerrainAmount(snow::StaticType()))
	  PerfectForAttnam.push_back(Continent[c]);

      if(!PerfectForAttnam.size())
	continue;

      vector2d AttnamPos, ElpuriCavePos;
      ushort CounterOne;

      for(CounterOne = 0;;)
	{
	  continent* PetrusLikes = PerfectForAttnam[RAND() % PerfectForAttnam.size()];
	  AttnamPos = PetrusLikes->GetRandomMember(evergreenforest::StaticType());
	  ushort CounterTwo = 0;

	  for(ElpuriCavePos = PetrusLikes->GetRandomMember(snow::StaticType());; ElpuriCavePos = PetrusLikes->GetRandomMember(snow::StaticType()))
	    {
	      if(ElpuriCavePos != AttnamPos && (ElpuriCavePos.X - AttnamPos.X) * (ElpuriCavePos.X - AttnamPos.X) + (ElpuriCavePos.Y - AttnamPos.Y) * (ElpuriCavePos.Y - AttnamPos.Y) < 50)
		break;

	      if(++CounterTwo == 50)
		break;
	    }

	  if(CounterTwo != 50)
	    break;

	  if(++CounterOne == 50)
	    break;
	}

      if(CounterOne == 50)
	continue;

      GetWSquare(AttnamPos)->ChangeOWTerrain(new attnam);
      SetEntryPos(ATTNAM, AttnamPos);
      GetWSquare(ElpuriCavePos)->ChangeOWTerrain(new elpuricave);
      SetEntryPos(ELPURICAVE, ElpuriCavePos);
      GetWSquare(AttnamPos)->AddCharacter(game::GetPlayer());
      CalculateLuminances();
      CalculateNeighbourBitmapPoses();
      break;
    }
}

void worldmap::RandomizeAltitude()
{
  game::BusyAnimation();

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      AltitudeBuffer[x][y] = RAND() % 5001 - RAND() % 5000;
}

void worldmap::SmoothAltitude()
{
  short** OldAltitudeBuffer = Alloc2D<short>(XSize, YSize);

  for(ushort c = 0; c < 10; ++c)
    {
      game::BusyAnimation();

      if(c < 8)
	{
	  for(ushort c1 = 0; c1 < RAND() % 20; ++c1)
	    {
	      ushort PlaceX = 5 + RAND() % (XSize-10), PlaceY = 5 + RAND() % (YSize-10);
	      short Change = RAND() % 10000 - RAND() % 10000;

	      for(int c2 = 0; c2 < RAND() % 50; ++c2)
		AltitudeBuffer[(PlaceX + RAND() % 5 - RAND() % 5)][(PlaceY + RAND() % 5 - RAND() % 5)] += Change;
	    }
	}

      for(ushort y = 0; y < YSize; ++y)
	for(ushort x = 0; x < XSize; ++x)
	  {
	    long HeightNear = 0;
	    uchar SquaresNear = 0;
	    OldAltitudeBuffer[x][y] = AltitudeBuffer[x][y];
	    ushort d;

	    for(d = 0; d < 4; ++d)
	      {
		vector2d Pos = vector2d(x, y) + game::GetMoveVector(d);

		if(IsValidPos(Pos))
		  {
		    HeightNear += OldAltitudeBuffer[Pos.X][Pos.Y];
		    ++SquaresNear;
		  }
	      }

	    for(d = 4; d < 8; ++d)
	      {
		vector2d Pos = vector2d(x, y) + game::GetMoveVector(d);

		if(IsValidPos(Pos))
		  {
		    HeightNear += AltitudeBuffer[Pos.X][Pos.Y];
		    ++SquaresNear;
		  }
	      }

	    AltitudeBuffer[x][y] = HeightNear / SquaresNear;

	    if(c < 8)
	      AltitudeBuffer[x][y] += RAND() % 100 - RAND() % 100;
	  }
    }

  delete [] OldAltitudeBuffer;
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
  OldTypeBuffer = Alloc2D<ushort>(XSize, YSize);

  for(ushort c = 0; c < 3; ++c)
    {
      game::BusyAnimation();

      for(ushort y = 0; y < YSize; ++y)
	for(ushort x = 0, NewType; x < XSize; ++x)
	  if((OldTypeBuffer[x][y] = TypeBuffer[x][y]) != ocean::StaticType() && (NewType = WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(x, y)))
	    TypeBuffer[x][y] = NewType;
    }

  game::BusyAnimation();

  for(ushort x = 0; x < XSize; ++x)
    for(ushort y = 0; y < YSize; ++y)
      Map[x][y]->ChangeWTerrain(protocontainer<gwterrain>::GetProto(TypeBuffer[x][y])->Clone(), new atmosphere);

  delete [] OldTypeBuffer;
}

ushort worldmap::WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(ushort x, ushort y)
{
  static ushort Types = protocontainer<gwterrain>::GetProtoAmount() + 1;
  static uchar* Type = new uchar[Types];

  for(ushort n = 0; n < Types; ++n)
    Type[n] = 0;

  ushort d;

  for(d = 0; d < 4; ++d)
    {
      vector2d Pos = vector2d(x, y) + game::GetMoveVector(d);

      if(IsValidPos(Pos))
	++Type[OldTypeBuffer[Pos.X][Pos.Y]];
    }

  for(d = 4; d < 8; ++d)
    {
      vector2d Pos = vector2d(x, y) + game::GetMoveVector(d);

      if(IsValidPos(Pos))
	++Type[TypeBuffer[Pos.X][Pos.Y]];
    }

  ++Type[TypeBuffer[x][y]];
  uchar MostCommon = 0;

  for(ushort c = 1; c < Types; ++c)
    if(Type[c] > Type[MostCommon] && c != ocean::StaticType())
      MostCommon = c;

  return MostCommon;
}

void worldmap::CalculateContinents()
{
  ushort c;

  for(c = 1; c < Continent.size(); ++c)
    delete Continent[c];

  Continent.resize(1, 0);
  memset(ContinentBuffer[0], 0, XSize * YSize);

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
			    if(Continent[ContinentBuffer[x][y]]->Size() < Continent[ContinentBuffer[Pos.X][Pos.Y]]->Size())
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
    if(!Continent[c]->Size())
      for(ushort i = Continent.size() - 1; i >= c; i--)
	if(Continent[i]->Size())
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
  if(!game::GetSeeWholeMapCheat())
    {
      for(ushort x = game::GetCamera().X; x < game::GetCamera().X + game::GetScreenSize().X; ++x)
	for(ushort y = game::GetCamera().Y; y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
	  if(Map[x][y]->GetLastSeen())
	    Map[x][y]->Draw();
    }
  else
    {
      for(ushort x = game::GetCamera().X; x < game::GetCamera().X + game::GetScreenSize().X; ++x)
	for(ushort y = game::GetCamera().Y; y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
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
