#include "worldmap.h"
#include "wsquare.h"
#include "wterrain.h"
#include "char.h"
#include "allocate.h"
#include "level.h"
#include "material.h"
#include "proto.h"
//#include "save.h"

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
	short** Data;
	short** Buffer;
	Data = Alloc2D<short>(XSize, YSize);
	Buffer = Alloc2D<short>(XSize, YSize);
   

	int placex, placey,change;
	for(int x = 0; x < XSize; x++)
		for(int y = 0; y < YSize; y++)
			Data[x][y] = rand() % 1000 - rand() % 1000;
	for(int timesgone = 0; timesgone < 3; timesgone++)
	{
		for(int c1 = 0; c1 < rand() % 10; c1++)
		{
			placex = 5+rand() % (XSize-10);
			placey = 5+rand() % (YSize-10);
			change = rand() % 10000 - rand() % 10000;
			for(int c2 = 0; c2 < rand() % 50; c2++)
			{
				Data[(placex + rand() % 5 - rand() % 5)][(placey + rand() % 5 - rand() % 5)] += change;
			}
		}
		for(int x = 0; x < XSize; x++)
			for(int y = 0; y < YSize; y++)
			{
				ulong HeightNear = 0;
				for(uchar c = 0; c < 8; c++)
				{
					vector Temp(vector(x,y) + game::GetMoveVector(c));
						if(short(Temp.X) < XSize && short(Temp.Y) < YSize && short(Temp.X) > 0 && short(Temp.Y) > 0) 
							HeightNear += Data[Temp.X][Temp.Y];
					Buffer[x][y] = HeightNear >> 3;
				}
			}

		for(x = 0; x < XSize; x++)
			for(int y = 0; y < YSize; y++)
			{
				Data[x][y] = Buffer[x][y];

			}
	}
	for(x = 0; x < XSize; x++)
		for(int y = 0; y < YSize; y++)
		{
			Map[x][y]->SAltitude(Data[x][y]);
		}


	delete [] Buffer;
	delete [] Data;
	GenerateTerrain();
}

#define CLIMATE_RANDOMNESS		0.8		//1 = mathematic, 0 = random
#define MAX_TEMPERATURE			27		//increase for more tropical world
#define LATITUDE_EFFECT			0.3		//increase for more effect
#define ALTITUDE_EFFECT			0.03

#define COLD					10
#define NORMAL					13
#define WARM					15
#define HOT						18

#define BORDER_OF_DARKNESS		-100		//in what depth will DARK_WATER be shown

#define CLIMATE_SMOOTHING		20

void worldmap::GenerateTerrain(void)
{
	char rainfall = 0;

	short temperature;

	float altsum = 0, distance_from_equator;

	for(unsigned short x = 0; x < XSize; x++)
		for(unsigned short y = 0; y < YSize; y++)
		{
			distance_from_equator = (float)abs(y - (YSize / 2));

			if (distance_from_equator / YSize * 2 <= 0.125)
				if (rand() %10 < 10 * CLIMATE_RANDOMNESS) rainfall = 1; else rainfall = 0;

			if (distance_from_equator / YSize * 2 > 0.125 && distance_from_equator / YSize <= 0.375)
				if (rand() %10 < 10 * CLIMATE_RANDOMNESS) rainfall = 0; else rainfall = 1;

			if (distance_from_equator / YSize * 2 > 0.375 && distance_from_equator / YSize <= 0.625)
				if (rand() %10 < 10 * CLIMATE_RANDOMNESS) rainfall = 1; else rainfall = 0;

			if (distance_from_equator / YSize * 2 > 0.625)
				if (rand() %10 < 10 * CLIMATE_RANDOMNESS) rainfall = 0; else rainfall = 1;

			unsigned char WaterNear = 0;
			
			for(char c = 0; c < 8; c++)
			{
				DO_FOR_SQUARES_AROUND(x, y, XSize, YSize, 
				{
					if(Map[DoX][DoY]->CAltitude() < 0) WaterNear++;
				});
				if(Map[x][y]->CAltitude() < 0) WaterNear++;
			}

			if (WaterNear) rainfall = 1;

			temperature = (short)(MAX_TEMPERATURE - distance_from_equator * LATITUDE_EFFECT - Map[x][y]->CAltitude() * ALTITUDE_EFFECT);

			if (Map[x][y]->CAltitude() <= 0)
			{
/*				if (Data[x][y].Height <= BORDER_OF_DARKNESS)
					Data[x][y].Terrain = DARK_OCEAN;
				else
					Data[x][y].Terrain = LIGHT_OCEAN;*/
					Map[x][y]->ChangeWorldMapTerrain(new ocean, new atmosphere);

		 	}
			else
			{
				if (temperature <= COLD && rainfall == 0)
					Map[x][y]->ChangeWorldMapTerrain(new glacier, new atmosphere);

				if (temperature <= COLD && rainfall == 1)
					Map[x][y]->ChangeWorldMapTerrain(new snow, new atmosphere);

				if (temperature > COLD && temperature <= NORMAL && rainfall == 0)
					Map[x][y]->ChangeWorldMapTerrain(new snow, new atmosphere);

				if (temperature > COLD && temperature <= NORMAL && rainfall == 1)
					Map[x][y]->ChangeWorldMapTerrain(new evergreenforest, new atmosphere);

				if (temperature > NORMAL && temperature <= WARM)
					Map[x][y]->ChangeWorldMapTerrain(new leafyforest, new atmosphere);

				if (temperature > WARM && temperature <= HOT && rainfall == 0)
					Map[x][y]->ChangeWorldMapTerrain(new steppe, new atmosphere);

				if (temperature > WARM && temperature <= HOT && rainfall == 1)
					Map[x][y]->ChangeWorldMapTerrain(new leafyforest, new atmosphere);

				if (temperature > HOT && rainfall == 0)
					Map[x][y]->ChangeWorldMapTerrain(new desert, new atmosphere);

				if (temperature > HOT && rainfall == 1)
					Map[x][y]->ChangeWorldMapTerrain(new jungle, new atmosphere);
	     	}
		}
}

