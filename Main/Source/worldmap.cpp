#include <cmath>

#include "worldmap.h"
#include "wsquare.h"
#include "wterrain.h"
#include "char.h"
#include "allocate.h"
#include "level.h"
#include "material.h"
#include "proto.h"

worldmap::worldmap(ushort XSize, ushort YSize) : area(XSize, YSize)
{
	Map = (worldmapsquare***)area::Map;

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
		{
			Map[x][y] = new worldmapsquare(this, vector(x, y));
			Map[x][y]->ChangeWorldMapTerrain(new ocean, new atmosphere);
		}
}

worldmap::~worldmap(void)
{
}

void worldmap::Save(std::ofstream& SaveFile) const
{
	area::Save(SaveFile);

	for(ulong c = 0; c < XSizeTimesYSize; c++)
		Map[0][c]->Save(SaveFile);
}

void worldmap::Load(std::ifstream& SaveFile)
{
	area::Load(SaveFile);

	Map = (worldmapsquare***)area::Map;

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
		{
			Map[x][y] = new worldmapsquare(this, vector(x, y));
			Map[x][y]->Load(SaveFile);
		}
}

void worldmap::Draw(void) const
{
	ushort XMax = GetXSize() < game::GetCamera().X + 50 ? GetXSize() : game::GetCamera().X + 50;
	ushort YMax = GetYSize() < game::GetCamera().Y + 30 ? GetYSize() : game::GetCamera().Y + 30;

	if(game::GetSeeWholeMapCheat())
		for(ushort x = game::GetCamera().X; x < XMax; x++)
			for(ushort y = game::GetCamera().Y; y < YMax; y++)
			{
				long xDist = long(x) - game::GetPlayer()->GetPos().X, yDist = long(y) - game::GetPlayer()->GetPos().Y;

				if(Map[x][y]->RetrieveFlag() && (xDist * xDist + yDist * yDist) <= game::GetPlayer()->LOSRangeLevelSquare())
					Map[x][y]->UpdateMemorizedAndDraw();
				else
					Map[x][y]->DrawCheat();
			}
	else
		for(ushort x = game::GetCamera().X; x < XMax; x++)
			for(ushort y = game::GetCamera().Y; y < YMax; y++)
			{
				long xDist = (long(x) - game::GetPlayer()->GetPos().X), yDist = (long(y) - game::GetPlayer()->GetPos().Y);

				if(Map[x][y]->RetrieveFlag() && (xDist * xDist + yDist * yDist) <= game::GetPlayer()->LOSRangeLevelSquare())
					Map[x][y]->UpdateMemorizedAndDraw();
				else
					Map[x][y]->DrawMemorized();
			}
}

void worldmap::Generate(void)
{
	short** Data = Alloc2D<short>(XSize, YSize);
	short** Buffer = Alloc2D<short>(XSize, YSize);

	int placex, placey, change;

	for(int x = 0; x < XSize; x++)
		for(int y = 0; y < YSize; y++)
			Data[x][y] = rand() % 1000 - rand() % 1000;

	for(int timesgone = 0; timesgone < 6; timesgone++)
	{
		if(timesgone < 4)
		{
			for(int c1 = 0; c1 < rand() % 10; c1++)
			{
				placex = 5 + rand() % (XSize-10);
				placey = 5 + rand() % (YSize-10);
				change = rand() % 10000 - rand() % 10000;

				for(int c2 = 0; c2 < rand() % 50; c2++)
					Data[(placex + rand() % 5 - rand() % 5)][(placey + rand() % 5 - rand() % 5)] += change;
			}
		}

		for(int x = 0; x < XSize; x++)
			for(int y = 0; y < YSize; y++)
			{
				long HeightNear = 0, SquaresNear = 0;

				DO_FOR_SQUARES_AROUND(x, y, XSize, YSize, HeightNear += Data[DoX][DoY]; SquaresNear++;)

				Buffer[x][y] = HeightNear / SquaresNear;
			}

		for(x = 0; x < XSize; x++)
			for(int y = 0; y < YSize; y++)
				Data[x][y] = Buffer[x][y];
	}

	for(x = 0; x < XSize; x++)
		for(int y = 0; y < YSize; y++)
			Map[x][y]->SetAltitude(Data[x][y]);

	delete [] Buffer;
	delete [] Data;

	GenerateClimate();
}

#define CLIMATE_RANDOMNESS		0.2		//0 = mathematic, 1 = random
#define MAX_TEMPERATURE			27		//increase for more tropical world
#define LATITUDE_EFFECT			0.3		//increase for more effect
#define ALTITUDE_EFFECT			0.03

#define COLD					10
#define NORMAL					13
#define WARM					15
#define HOT					19

#define BORDER_OF_DARKNESS		-100		//in what depth will DARK_WATER be shown

#define CLIMATE_SMOOTHING		20

void worldmap::GenerateClimate(void)
{
	char rainfall = 0;
	ushort** Data = Alloc2D<ushort>(XSize, YSize);
	ushort** Buffer = Alloc2D<ushort>(XSize, YSize);
	short temperature;

	float altsum = 0, distance_from_equator;

	for(ushort x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
		{
			if (Map[x][y]->GetAltitude() <= 0)
			{
				Map[x][y]->ChangeWorldMapTerrain(new ocean, new atmosphere);
				Data[x][y] = Map[x][y]->GetGroundWorldMapTerrain()->GetType() - groundworldmapterrain::GetProtoIndexBegin();
				continue;
			}

			distance_from_equator = fabs(float(y) / YSize - 0.5f);

			if(distance_from_equator <= 0.08 || (distance_from_equator > 0.25 && distance_from_equator <= 0.42))
				rainfall = 1;
			else
				rainfall = 0;

			if(rand() % 10 < 10 * CLIMATE_RANDOMNESS)
				rainfall = rand() % 2;

			if(!rainfall)
				DO_FOR_SQUARES_AROUND(x, y, XSize, YSize, if(Map[DoX][DoY]->GetAltitude() <= 0) rainfall = 1;)

			temperature = (short)(MAX_TEMPERATURE - distance_from_equator * YSize * LATITUDE_EFFECT - Map[x][y]->GetAltitude() * ALTITUDE_EFFECT);

			if (temperature <= COLD && rainfall == 0)
				Map[x][y]->ChangeWorldMapTerrain(new glacier, new atmosphere);

			if (temperature <= COLD && rainfall == 1)
				Map[x][y]->ChangeWorldMapTerrain(new snow, new atmosphere);

			if (temperature > COLD && temperature <= NORMAL)
				Map[x][y]->ChangeWorldMapTerrain(new evergreenforest, new atmosphere);

			//if (temperature > COLD && temperature <= NORMAL && rainfall == 1)
			//	Map[x][y]->ChangeWorldMapTerrain(new evergreenforest, new atmosphere);

			if (temperature > NORMAL && temperature <= WARM && rainfall == 0)
				Map[x][y]->ChangeWorldMapTerrain(new steppe, new atmosphere);

			if (temperature > NORMAL && temperature <= WARM && rainfall == 1)
				Map[x][y]->ChangeWorldMapTerrain(new leafyforest, new atmosphere);

			if (temperature > WARM && temperature <= HOT && rainfall == 0)
				Map[x][y]->ChangeWorldMapTerrain(new steppe, new atmosphere);

			if (temperature > WARM && temperature <= HOT && rainfall == 1)
				Map[x][y]->ChangeWorldMapTerrain(new leafyforest, new atmosphere);

			if (temperature > HOT && rainfall == 0)
				Map[x][y]->ChangeWorldMapTerrain(new desert, new atmosphere);

			if (temperature > HOT && rainfall == 1)
				Map[x][y]->ChangeWorldMapTerrain(new jungle, new atmosphere);

			Data[x][y] = Map[x][y]->GetGroundWorldMapTerrain()->GetType() - groundworldmapterrain::GetProtoIndexBegin();
		}

	for(ushort c = 0; c < 2; c++)
	{
		for(x = 0; x < XSize; x++)
			for(ushort y = 0; y < YSize; y++)
				if(Data[x][y])
					Buffer[x][y] = WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(Data, x, y);

		for(x = 0; x < XSize; x++)
			for(ushort y = 0; y < YSize; y++)
				if(Data[x][y])
					Data[x][y] = Buffer[x][y];
	}

	for(x = 0; x < XSize; x++)
		for(ushort y = 0; y < YSize; y++)
			Map[x][y]->ChangeWorldMapTerrain(GetProtoType<groundworldmapterrain>(Data[x][y] + groundworldmapterrain::GetProtoIndexBegin())->Clone(), new atmosphere);

	delete [] Data;
	delete [] Buffer;
}

ushort worldmap::WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(ushort** Data, ushort x, ushort y)
{
	static ushort Types = groundworldmapterrain::GetProtoAmount();											
	static uchar* Type = new uchar[Types];

	for(ushort n = 0; n < Types; n++)
		Type[n] = 0;

	DO_FOR_SQUARES_AROUND(x, y, XSize, YSize, 
	{
		Type[Data[DoX][DoY]]++;	
	});

	Type[Data[x][y]]++;

	uchar MostCommon = Type[0];

	for(ushort c = 1; c < Types; c++)
		if(Type[c] > Type[MostCommon])
			MostCommon = c;

	return MostCommon;
}
