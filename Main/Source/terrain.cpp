#include <cstdio>

#include "char.h"
#include "bitmap.h"
#include "graphics.h"
#include "game.h"
#include "igraph.h"
#include "terrain.h"
#include "god.h"
#include "hscore.h"
#include "level.h"
#include "lsquare.h"

bool overterrain::GoUp(character* Who) // Try to go up
{
	if(game::CCurrent() != 0 && game::CCurrent() != 9 && game::GetWizardMode())
	{
		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		vector Pos = Who->GetPos();
		game::SaveLevel();
		game::SetCurrent(game::CCurrent() - 1);
		game::LoadLevel();
		game::GetCurrentLevel()->AddCharacter(Pos, Who);
		game::GetCurrentLevel()->Luxify();
		game::GetCurrentLevel()->UpdateLOS();
		return true;
	}
	else
	{
	if(Who == game::GetPlayer())
		ADD_MESSAGE("You can't go up.");
	return false;
	}
}

bool overterrain::GoDown(character* Who) // Try to go down
{
	if(game::CCurrent() < game::GetLevels() - 2 && game::GetWizardMode())
	{
		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		vector Pos = Who->GetPos();
		game::SaveLevel();
		game::SetCurrent(game::CCurrent() + 1);
		game::LoadLevel();
		game::GetCurrentLevel()->AddCharacter(Pos, Who);
		game::GetCurrentLevel()->Luxify();
		game::GetCurrentLevel()->UpdateLOS();
		return true;
	}
	else
	{
	if(Who == game::GetPlayer())
		ADD_MESSAGE("You can't go down.");
	return false;
	}
}

void terrain::Save(std::ofstream* SaveFile) const
{
	object::Save(SaveFile);

	SaveFile->write((char*)&VisualFlags, sizeof(VisualFlags));
}

void terrain::Load(std::ifstream* SaveFile)
{
	object::Load(SaveFile);

	SaveFile->read((char*)&VisualFlags, sizeof(VisualFlags));
}

void overterrain::Save(std::ofstream* SaveFile) const
{
	terrain::Save(SaveFile);

	SaveFile->write((char*)&IsWalkable, sizeof(IsWalkable));
}

void overterrain::Load(std::ifstream* SaveFile)
{
	terrain::Load(SaveFile);

	SaveFile->read((char*)&IsWalkable, sizeof(IsWalkable));
}

void overterrain::DrawToTileBuffer(void) const
{
	igraph::GetTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (CMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16, GetVisualFlags());
}

void groundterrain::DrawToTileBuffer(void) const
{
	igraph::GetTerrainGraphic()->Blit(igraph::GetTileBuffer(), GetBitmapPos().X + (CMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
}

bool stairsup::GoUp(character* Who)  // Try to go up
{
	if(game::CCurrent())
	{
		if(game::CCurrent() == 9)
			if(!game::BoolQuestion("Somehow you get the feeling you cannot return. Continue anyway? [y/N]"))
				return false;

		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		vector Pos = Who->GetPos();
		game::SaveLevel();
		game::SetCurrent(game::CCurrent() - 1);
		game::LoadLevel();
		game::GetCurrentLevel()->AddCharacter(Pos, Who);
		game::GetCurrentLevel()->Luxify();
		game::GetCurrentLevel()->UpdateLOS();
		return true;
	}
	else
	{
		if(Who == game::GetPlayer())
			if(Who->HasPerttusNut())
			{
				if(Who->HasMaakotkaShirt() && game::GetGod(1)->GetRelation() == 1000)
				{
					game::StoryScreen("A heavenly choir starts to sing Grandis Rana and a booming voice fills the air:\n\n\"Mortal! Thou hast surpassed Perttu, and pleaseth Me greatly during thine adventures!\nI hereby title thee as My new Überpriest!\"\n\nYou are victorious!");
					game::RemoveSaves();
					game::Quit();

					if(!game::GetWizardMode())
					{
						game::GetPlayer()->AddScoreEntry("ascended to Überpriesthood", 5);
						highscore HScore;
						HScore.Draw();
					}
					return true;
				}
				else
				{
					game::StoryScreen("As you ascend, an undead and sinister voice greets you:\n\n\"MoRtAl! ThOu HaSt SlAuGtHeReD pErTtU aNd PlEaSeD mE!\nfRoM tHiS dAy On, YoU aRe ThE dEaReSt SeRvAnT oF aLl EvIl!\"\n\nYou are victorious!");
					game::RemoveSaves();
					game::Quit();

					if(!game::GetWizardMode())
					{
						game::GetPlayer()->AddScoreEntry("killed Perttu and became the Avatar of Chaos", 4);
						highscore HScore;
						HScore.Draw();
					}
					return true;
				}
			}
			else
			{
				game::StoryScreen("Thou escaped from the dungeon and were executed for not obeying the Überpriest!\n\nGame Over.");
				game::RemoveSaves();
				game::Quit();

				if(!game::GetWizardMode())
				{
					game::GetPlayer()->AddScoreEntry("escaped from the dungeon and was executed for cowardice");
					highscore HScore;
					HScore.Draw();
				}
				return true;
			}

		return false;
	}
}

bool stairsdown::GoDown(character* Who)  // Try to go down
{
	if(game::CCurrent() != game::GetLevels() - 1)
	{
		if(game::CCurrent() == 8)
		{
			if(!game::BoolQuestion("Something with ultimate sinister power seems to tremble under your feet. You feel you shouldn't wander any further. Continue anyway? [y/N]"))
				return false;

			Who->GetLevelSquareUnder()->ChangeTerrain(new parquet, new empty);
		}

		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		vector Pos = Who->GetPos();
		game::SaveLevel();
		game::SetCurrent(game::CCurrent() + 1);
		game::LoadLevel();
		game::GetCurrentLevel()->AddCharacter(Pos, Who);
		game::GetCurrentLevel()->Luxify();
		game::GetCurrentLevel()->UpdateLOS();
		game::ShowLevelMessage();
		return true;
	}
	else
	{
		if(Who == game::GetPlayer())
			ADD_MESSAGE("You are at the bottom.");

		return false;
	}
}

bool door::Open(character* Opener)
{

	if(!GetIsWalkable())
	{
		if(Opener == game::GetPlayer())
			ADD_MESSAGE("You open the door.");
	}
	else
	{
		if(Opener == game::GetPlayer()) ADD_MESSAGE("The door is already open, %s.", game::Insult());

		return false;
	}

	MakeWalkable();

	return true;
}

bool door::Close(character* Closer)
{
	if(Closer == game::GetPlayer())
		if(GetIsWalkable())
			ADD_MESSAGE("You close the door.");
		else
		{
			ADD_MESSAGE("The door is already closed, %s.", game::Insult());

			return false;
		}

	MakeNotWalkable();

	return true;
}

bool terrain::Open(character* Opener)
{
	if(Opener == game::GetPlayer())
		ADD_MESSAGE("There isn't anything to open, %s.", game::Insult());

	return false;
}

bool terrain::Close(character* Closer)
{
	if(Closer == game::GetPlayer())
		ADD_MESSAGE("There isn't anything to close, %s.", game::Insult());

	return false;
}

void overterrain::MakeWalkable(void)
{
	IsWalkable = true;

	GetLevelSquareUnder()->ForceEmitterEmitation();

	if(GetLevelSquareUnder()->RetrieveFlag())
		game::GetCurrentLevel()->UpdateLOS();
}

void overterrain::MakeNotWalkable(void)
{
	GetLevelSquareUnder()->ForceEmitterNoxify();

	IsWalkable = false;

	GetLevelSquareUnder()->ForceEmitterEmitation();

	if(GetLevelSquareUnder()->RetrieveFlag())
		game::GetCurrentLevel()->UpdateLOS();
}

vector terrain::GetPos(void) const
{
	return GetLevelSquareUnder()->GetPos();
}

std::string altar::Name(uchar Case) const
{
	if(!(Case & PLURAL))
		if(!(Case & DEFINEBIT))
			return CMaterial(0)->Name()  + " " + NameSingular() + " of " + game::GetGod(OwnerGod)->Name();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + CMaterial(0)->Name()  + " " + NameSingular() + " of " + game::GetGod(OwnerGod)->Name();
			else
				return CMaterial(0)->Name(INDEFINITE)  + " " + NameSingular() + " of " + game::GetGod(OwnerGod)->Name();
	else
		if(!(Case & DEFINEBIT))
			return CMaterial(0)->Name()  + " " + NamePlural() + " of " + game::GetGod(OwnerGod)->Name();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + CMaterial(0)->Name()  + " " + NamePlural() + " of " + game::GetGod(OwnerGod)->Name();
			else
				return CMaterial(0)->Name()  + " " + NamePlural() + " of " + game::GetGod(OwnerGod)->Name();
}

void terrain::SetSquareUnder(square* Square)
{
	SquareUnder = Square;
}

void altar::DrawToTileBuffer(void) const
{
	igraph::GetTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (CMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
	igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetOwnerGod() << 4, 0, 0, 0, 16, 16);
}

void altar::Load(std::ifstream* SaveFile)
{
	overterrain::Load(SaveFile);

	SaveFile->read((char*)&OwnerGod, sizeof(OwnerGod));
}

void altar::Save(std::ofstream* SaveFile) const
{
	overterrain::Save(SaveFile);

	SaveFile->write((char*)&OwnerGod, sizeof(OwnerGod));
}

void terrain::HandleVisualEffects(void)
{
	uchar Flags = 0, AcceptedFlags = OKVisualEffects();

	for(uchar c = 0; c < 8; c++)
		if((AcceptedFlags & (1 << c)) && (rand() % 2))
			Flags |= 1 << c;

	SetVisualFlags(Flags);
}
