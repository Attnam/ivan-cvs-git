#include "worldmap.h"
#include "wsquare.h"
#include "wterrain.h"
#include "game.h"
#include "char.h"

worldmap::worldmap(ushort XSize, ushort YSize) : area(XSize, YSize)
{
	Map = (worldmapsquare***)area::Map;

	for(ushort x = 0; x < XSize; x++)
		for(ulong y = 0; y < YSize; y++)
		{
			Map[x][y] = new worldmapsquare(this, vector(x, y));
			Map[x][y]->ChangeWorldMapTerrain(new jungle, 0);
		}
}

worldmap::~worldmap(void)
{
}

void worldmap::Save(std::ofstream* SaveFile) const
{
	area::Save(SaveFile);

	{
	for(ulong c = 0; c < XSizeTimesYSize; c++)
		Map[0][c]->Save(SaveFile);
	}
}

worldmap::worldmap(std::ifstream* SaveFile) : area(SaveFile)
{
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
	ushort XMax = GetXSize() < game::CCamera().X + 50 ? GetXSize() : game::CCamera().X + 50;
	ushort YMax = GetYSize() < game::CCamera().Y + 30 ? GetYSize() : game::CCamera().Y + 30;

	if(game::GetSeeWholeMapCheat())
		for(ushort x = game::CCamera().X; x < XMax; x++)
			for(ushort y = game::CCamera().Y; y < YMax; y++)
			{
				long xDist = long(x) - game::GetPlayer()->GetPos().X, yDist = long(y) - game::GetPlayer()->GetPos().Y;

				if(Map[x][y]->RetrieveFlag() && (xDist * xDist + yDist * yDist) <= game::GetPlayer()->LOSRangeLevelSquare())
					Map[x][y]->UpdateMemorizedAndDraw();
				else
					Map[x][y]->DrawCheat();
			}
	else
		for(ushort x = game::CCamera().X; x < XMax; x++)
			for(ushort y = game::CCamera().Y; y < YMax; y++)
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
}
