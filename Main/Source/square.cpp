#include "char.h"
#include "square.h"
#include "bitmap.h"
#include "terrain.h"
#include "game.h"
#include "igraph.h"
#include "area.h"
#include "level.h"

square::square(area* MotherArea, vector Pos) : MotherArea(MotherArea), GroundTerrain(0), OverTerrain(0), Rider(0), Character(0), Flyer(0), Known(false), Pos(Pos)
{
}

square::~square(void)
{
	delete GroundTerrain;
	delete OverTerrain;
	delete Rider;
	delete Character;
	delete Flyer;
}

void square::Save(std::ofstream* SaveFile) const
{
	GroundTerrain->Save(SaveFile);
	OverTerrain->Save(SaveFile);

	if(Character)
		Character->Save(SaveFile);
	else
	{
		ushort Type = 0;

		SaveFile->write((char*)&Type, sizeof(Type));
	}

	SaveFile->write((char*)&Known, sizeof(Known));

	if(Known)
		CMotherArea()->CMemorized()->Save(SaveFile, Pos.X << 4, Pos.Y << 4, 16, 16);
}

square::square(area* MotherArea, std::ifstream* SaveFile, vector Pos) : MotherArea(MotherArea), Rider(0), Flyer(0), Pos(Pos)
{
	GroundTerrain = game::LoadGroundTerrain(SaveFile);
	GroundTerrain->SSquareUnder(this);
	OverTerrain = game::LoadOverTerrain(SaveFile);
	OverTerrain->SSquareUnder(this);

	Character = game::LoadCharacter(SaveFile);
	if(Character) Character->SSquareUnder(this);

	SaveFile->read((char*)&Known, sizeof(Known));

	if(Known)
		CMotherArea()->CMemorized()->Load(SaveFile, Pos.X << 4, Pos.Y << 4, 16, 16);
}

void square::DrawCheat(void) const
{
	DrawToTileBuffer();

	if(CCharacter())
		CCharacter()->DrawToTileBuffer();

	igraph::BlitTileBuffer(vector((CPos().X - game::CCamera().X) << 4, (CPos().Y - game::CCamera().Y + 2) << 4));
}

void square::DrawMemorized(void) const
{
	if(CKnown())
	{
		game::CCurrentLevel()->CMemorized()->Blit(igraph::CTileBuffer(), Pos.X << 4, Pos.Y << 4, 0, 0, 16, 16);
		igraph::BlitTileBuffer(vector((CPos().X - game::CCamera().X) << 4, (CPos().Y - game::CCamera().Y + 2) << 4));
	}
}

void square::AddCharacter(character* Guy)
{
	Character = Guy;
	Guy->SSquareUnder(this);
}	//lisätkää aborttiii!!!

void square::RemoveCharacter(void)
{
	SCharacter(0);
}

void square::ChangeTerrain(groundterrain* NewGround, overterrain* NewOver)
{
	delete GroundTerrain;
	SGroundTerrain(NewGround);
	CGroundTerrain()->SSquareUnder(this);
	delete OverTerrain;
	SOverTerrain(NewOver);
	COverTerrain()->SSquareUnder(this);
}

