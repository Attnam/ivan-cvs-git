#include "char.h"
#include "square.h"
#include "lterrain.h"
#include "igraph.h"
#include "level.h"
#include "proto.h"
#include "material.h"
#include "strover.h"

square::square(area* AreaUnder, vector Pos) : AreaUnder(AreaUnder), Rider(0), Character(0), Flyer(0), Known(false), Pos(Pos)
{
}

square::~square(void)
{
	delete Rider;
	delete Character;
	delete Flyer;
}

void square::Save(std::ofstream& SaveFile) const
{
	//SaveFile << GroundTerrain << OverTerrain;

	SaveFile << Character;

	SaveFile.write((char*)&Known, sizeof(Known));

	if(Known)
		GetAreaUnder()->GetMemorized()->Save(SaveFile, Pos.X << 4, Pos.Y << 4, 16, 16);

	SaveFile.write((char*)&Flag, sizeof(Flag));

	SaveFile << MemorizedDescription;
}

void square::Load(std::ifstream& SaveFile)
{
	game::SetSquareInLoad(this);

	//SaveFile >> GroundTerrain >> OverTerrain;

	SaveFile >> Character;

	SaveFile.read((char*)&Known, sizeof(Known));

	if(Known)
		GetAreaUnder()->GetMemorized()->Load(SaveFile, Pos.X << 4, Pos.Y << 4, 16, 16);

	SaveFile.read((char*)&Flag, sizeof(Flag));

	SaveFile >> MemorizedDescription;
}

void square::DrawCheat(void) const
{
	DrawToTileBuffer();

	if(GetCharacter())
		GetCharacter()->DrawToTileBuffer();

	igraph::BlitTileBuffer(vector((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4));
}

void square::DrawMemorized(void) const
{
	if(GetKnown())
	{
		AreaUnder->GetMemorized()->Blit(igraph::GetTileBuffer(), Pos.X << 4, Pos.Y << 4, 0, 0, 16, 16);
		igraph::BlitTileBuffer(vector((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4));
	}
}

void square::AddCharacter(character* Guy)
{
	Character = Guy;
	Guy->SetSquareUnder(this);
}	//lisätkää aborttiii!!!

void square::RemoveCharacter(void)
{
	SetCharacter(0);
}

bool square::CanBeSeen(void) const
{
	float xDist = (float(GetPos().X) - game::GetPlayer()->GetPos().X), yDist = (float(GetPos().Y) - game::GetPlayer()->GetPos().Y);

	if(RetrieveFlag() && xDist * xDist + yDist * yDist <= game::GetPlayer()->LOSRangeLevelSquare())
		return true;
	else
		return false;
}

