#include "char.h"
#include "igraph.h"
#include "lterrain.h"
#include "god.h"
#include "hscore.h"
#include "level.h"
#include "lsquare.h"
#include "message.h"
#include "proto.h"
#include "dungeon.h"

bool overlevelterrain::GoUp(character* Who) // Try to go up
{
	if(game::GetCurrent() != 0 && game::GetCurrent() != 9 && game::GetWizardMode())
	{
		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		vector Pos = Who->GetPos();
		game::GetDungeon()->SaveLevel();
		game::SetCurrent(game::GetCurrent() - 1);
		game::GetDungeon()->LoadLevel();
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

bool overlevelterrain::GoDown(character* Who) // Try to go down
{
	if(game::GetCurrent() < game::GetLevels() - 2 && game::GetWizardMode())
	{
		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		vector Pos = Who->GetPos();
		game::GetDungeon()->SaveLevel();
		game::SetCurrent(game::GetCurrent() + 1);
		game::GetDungeon()->LoadLevel();
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

void levelterrain::Save(std::ofstream& SaveFile) const
{
	object::Save(SaveFile);

	SaveFile.write((char*)&VisualFlags, sizeof(VisualFlags));
}

void levelterrain::Load(std::ifstream& SaveFile)
{
	object::Load(SaveFile);

	SaveFile.read((char*)&VisualFlags, sizeof(VisualFlags));
}

void overlevelterrain::Save(std::ofstream& SaveFile) const
{
	levelterrain::Save(SaveFile);
	overterrain::Save(SaveFile);
}

void overlevelterrain::Load(std::ifstream& SaveFile)
{
	levelterrain::Load(SaveFile);
	overterrain::Load(SaveFile);
}

void groundlevelterrain::DrawToTileBuffer(void) const
{
	igraph::GetLevelTerrainGraphic()->Blit(igraph::GetTileBuffer(), GetBitmapPos().X + (GetMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
}

void overlevelterrain::DrawToTileBuffer(void) const
{
	igraph::GetLevelTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (GetMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16, GetVisualFlags());
}

bool stairsup::GoUp(character* Who)  // Try to go up
{
	if(game::GetCurrent())
	{
		if(game::GetCurrent() == 9)
			if(!game::BoolQuestion("Somehow you get the feeling you cannot return. Continue anyway? [y/N]"))
				return false;

		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		vector Pos = Who->GetPos();
		game::GetDungeon()->SaveLevel();
		game::SetCurrent(game::GetCurrent() - 1);
		game::GetDungeon()->LoadLevel();
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
	if(game::GetCurrent() != game::GetLevels() - 1)
	{
		if(game::GetCurrent() == 8)
		{
			if(!game::BoolQuestion("Something with ultimate sinister power seems to tremble under your feet. You feel you shouldn't wander any further. Continue anyway? [y/N]"))
				return false;

			Who->GetLevelSquareUnder()->ChangeLevelTerrain(new parquet, new empty);
		}

		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		vector Pos = Who->GetPos();
		game::GetDungeon()->SaveLevel();
		game::SetCurrent(game::GetCurrent() + 1);
		game::GetDungeon()->LoadLevel();
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

bool levelterrain::Open(character* Opener)
{
	if(Opener == game::GetPlayer())
		ADD_MESSAGE("There isn't anything to open, %s.", game::Insult());

	return false;
}

bool levelterrain::Close(character* Closer)
{
	if(Closer == game::GetPlayer())
		ADD_MESSAGE("There isn't anything to close, %s.", game::Insult());

	return false;
}

void overlevelterrain::MakeWalkable(void)
{
	IsWalkable = true;

	GetLevelSquareUnder()->ForceEmitterEmitation();

	if(GetLevelSquareUnder()->RetrieveFlag())
		game::GetCurrentLevel()->UpdateLOS();
}

void overlevelterrain::MakeNotWalkable(void)
{
	GetLevelSquareUnder()->ForceEmitterNoxify();

	IsWalkable = false;

	GetLevelSquareUnder()->ForceEmitterEmitation();

	if(GetLevelSquareUnder()->RetrieveFlag())
		game::GetCurrentLevel()->UpdateLOS();
}

vector levelterrain::GetPos(void) const
{
	return GetLevelSquareUnder()->GetPos();
}

std::string altar::Name(uchar Case) const
{
	if(!(Case & PLURAL))
		if(!(Case & DEFINEBIT))
			return GetMaterial(0)->Name()  + " " + NameSingular() + " of " + game::GetGod(OwnerGod)->Name();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + GetMaterial(0)->Name()  + " " + NameSingular() + " of " + game::GetGod(OwnerGod)->Name();
			else
				return GetMaterial(0)->Name(INDEFINITE)  + " " + NameSingular() + " of " + game::GetGod(OwnerGod)->Name();
	else
		if(!(Case & DEFINEBIT))
			return GetMaterial(0)->Name()  + " " + NamePlural() + " of " + game::GetGod(OwnerGod)->Name();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + GetMaterial(0)->Name()  + " " + NamePlural() + " of " + game::GetGod(OwnerGod)->Name();
			else
				return GetMaterial(0)->Name()  + " " + NamePlural() + " of " + game::GetGod(OwnerGod)->Name();
}

void altar::DrawToTileBuffer(void) const
{
	igraph::GetLevelTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (GetMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
	igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetOwnerGod() << 4, 0, 0, 0, 16, 16);
}

void altar::Load(std::ifstream& SaveFile)
{
	overlevelterrain::Load(SaveFile);

	SaveFile.read((char*)&OwnerGod, sizeof(OwnerGod));
}

void altar::Save(std::ofstream& SaveFile) const
{
	overlevelterrain::Save(SaveFile);

	SaveFile.write((char*)&OwnerGod, sizeof(OwnerGod));
}

void levelterrain::HandleVisualEffects(void)
{
	uchar Flags = 0, AcceptedFlags = OKVisualEffects();

	for(uchar c = 0; c < 8; c++)
		if((AcceptedFlags & (1 << c)) && (rand() % 2))
			Flags |= 1 << c;

	SetVisualFlags(Flags);
}

levelsquare* levelterrain::GetLevelSquareUnder(void) const
{
	return dynamic_cast<levelsquare*>(SquareUnder);
}

