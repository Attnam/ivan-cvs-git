#include "charba.h"
#include "square.h"
#include "lterraba.h"
#include "igraph.h"
#include "level.h"
#include "proto.h"
#include "materba.h"
#include "strover.h"
#include "save.h"

square::square(area* AreaUnder, vector2d Pos) : AreaUnder(AreaUnder), Character(0), Known(false), Pos(Pos), NewDrawRequested(true), Memorized(0)
{
}

square::~square()
{
	delete Character;
	delete Memorized;
}

void square::Save(outputfile& SaveFile) const
{
	SaveFile << Character << Known;

	if(Known)
		GetMemorized()->Save(SaveFile);

	SaveFile << Flag << MemorizedDescription;
}

void square::Load(inputfile& SaveFile)
{
	game::SetSquareInLoad(this);

	SaveFile >> Character >> Known;

	if(Known)
	{
		Memorized = new bitmap(16, 16);
		GetMemorized()->Load(SaveFile);
	}

	SaveFile >> Flag >> MemorizedDescription;
}

void square::AddCharacter(character* Guy)
{
	Character = Guy;
	Guy->SetSquareUnder(this);
	NewDrawRequested = true;
}

void square::DrawMemorized() const
{
	if(GetKnown())
	{
		GetMemorized()->Blit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
		igraph::BlitTileBuffer(vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4));
	}
}

void square::RemoveCharacter()
{
	SetCharacter(0);
	NewDrawRequested = true;
}

bool square::CanBeSeen() const
{
	if(!game::GetPlayer()->GetSquareUnder())
		return false;

	float xDist = (float(GetPos().X) - game::GetPlayer()->GetPos().X), yDist = (float(GetPos().Y) - game::GetPlayer()->GetPos().Y);

	if((RetrieveFlag() && xDist * xDist + yDist * yDist <= game::GetPlayer()->LOSRangeLevelSquare()) || game::GetSeeWholeMapCheat())
		return true;
	else
		return false;
}

bool square::CanBeSeenFrom(vector2d FromPos) const
{
	return game::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, game::EyeHandler);
}
