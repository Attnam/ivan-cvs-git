#include "strover.h"
#include "wsquare.h"
#include "igraph.h"
#include "charba.h"
#include "worldmap.h"
#include "wterraba.h"
#include "proto.h"
#include "materba.h"
#include "bitmap.h"
#include "save.h"

worldmapsquare::worldmapsquare(worldmap* WorldMapUnder, vector2d Pos) : square(WorldMapUnder, Pos), OverWorldMapTerrain(0), GroundWorldMapTerrain(0), DescriptionChanged(true)
{
}

worldmapsquare::~worldmapsquare()
{
	delete GetGroundWorldMapTerrain();
	delete GetOverWorldMapTerrain();
}

void worldmapsquare::Save(outputfile& SaveFile) const
{
	square::Save(SaveFile);

	SaveFile << GroundWorldMapTerrain << OverWorldMapTerrain;
}

void worldmapsquare::Load(inputfile& SaveFile)
{
	square::Load(SaveFile);

	SaveFile >> GroundWorldMapTerrain >> OverWorldMapTerrain;
}

void worldmapsquare::DrawToTileBuffer() const
{
	GetGroundWorldMapTerrain()->DrawToTileBuffer();
	GetOverWorldMapTerrain()->DrawToTileBuffer();
}

void worldmapsquare::UpdateMemorizedAndDraw()
{
	if(NewDrawRequested)
	{
		if(!GetKnown())
		{
			Memorized = new bitmap(16, 16);

			SetKnown(true);
		}

		DrawToTileBuffer();

		ushort Luminance = 256 - (abs(GetWorldMapUnder()->GetAltitude(Pos)) >> 3);

		igraph::GetTileBuffer()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16, Luminance);
		igraph::GetFOWGraphic()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16);

		if(GetCharacter())
			GetCharacter()->DrawToTileBuffer();

		igraph::BlitTileBuffer(vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4), Luminance);

		NewDrawRequested = false;
	}
}

void worldmapsquare::ChangeWorldMapTerrain(groundworldmapterrain* NewGround, overworldmapterrain* NewOver)
{
	ChangeGroundWorldMapTerrain(NewGround);
	ChangeOverWorldMapTerrain(NewOver);
}

void worldmapsquare::ChangeGroundWorldMapTerrain(groundworldmapterrain* NewGround)
{
	delete GetGroundWorldMapTerrain();
	SetGroundWorldMapTerrain(NewGround);
	DescriptionChanged = NewDrawRequested = true;
}

void worldmapsquare::ChangeOverWorldMapTerrain(overworldmapterrain* NewOver)
{
	delete GetOverWorldMapTerrain();
	SetOverWorldMapTerrain(NewOver);
	DescriptionChanged = NewDrawRequested = true;
}

void worldmapsquare::SetGroundWorldMapTerrain(groundworldmapterrain* What)
{
	GroundWorldMapTerrain = What;

	if(What)
	{
		What->SetAreaUnder(GetAreaUnder());
		What->SetSquareUnder(this);
	}
}

void worldmapsquare::SetOverWorldMapTerrain(overworldmapterrain* What)
{
	OverWorldMapTerrain = What;

	if(What)
	{
		What->SetAreaUnder(GetAreaUnder());
		What->SetSquareUnder(this);
	}
}

void worldmapsquare::UpdateMemorizedDescription()
{
	if(DescriptionChanged)
	{
		std::string Continent = GetWorldMapUnder()->GetContinentUnder(Pos) ? " of continent " + GetWorldMapUnder()->GetContinentUnder(Pos)->GetName() : "";

		if(GetOverWorldMapTerrain()->Name(UNARTICLED) != "atmosphere")
			SetMemorizedDescription(GetOverWorldMapTerrain()->Name(INDEFINITE) + " on " + GetGroundWorldMapTerrain()->Name(INDEFINITE) + Continent + ", height: " + GetWorldMapUnder()->GetAltitude(Pos) + " meters");
		else
			SetMemorizedDescription(GetGroundWorldMapTerrain()->Name(INDEFINITE) + Continent + ", height: " + GetWorldMapUnder()->GetAltitude(Pos) + " meters");

		DescriptionChanged = false;
	}
}

groundterrain* worldmapsquare::GetGroundTerrain() const
{
	return GroundWorldMapTerrain;
}

overterrain* worldmapsquare::GetOverTerrain() const
{
	return OverWorldMapTerrain;
}

void worldmapsquare::DrawCheat()
{
	if(NewDrawRequested)
	{
		DrawToTileBuffer();

		if(GetCharacter())
			GetCharacter()->DrawToTileBuffer();

		ushort Luminance = 256 - (abs(GetWorldMapUnder()->GetAltitude(Pos)) >> 3);

		igraph::BlitTileBuffer(vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4), Luminance);

		NewDrawRequested = false;
	}
}
