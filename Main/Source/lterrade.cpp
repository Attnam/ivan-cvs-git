#include "lterrade.h"
#include "message.h"
#include "god.h"
#include "igraph.h"
#include "save.h"
#include "level.h"
#include "charba.h"
#include "dungeon.h"
#include "feio.h"
#include "hscore.h"
#include "lsquare.h"

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

void altar::DrawToTileBuffer() const
{
	igraph::GetLevelTerrainGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (GetMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
	igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetOwnerGod() << 4, 0, 0, 0, 16, 16);
}

void altar::Load(inputfile& SaveFile)
{
	overlevelterrain::Load(SaveFile);

	SaveFile >> OwnerGod;
}

void altar::Save(outputfile& SaveFile) const
{
	overlevelterrain::Save(SaveFile);

	SaveFile << OwnerGod;
}

bool stairsup::GoUp(character* Who) const  // Try to go up
{
	if(game::GetCurrent())
	{
		if(game::GetCurrent() == 9)
			if(!game::BoolQuestion("Somehow you get the feeling you cannot return. Continue anyway? [y/N]"))
				return false;

		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		//vector2d Pos = Who->GetPos();
		game::GetCurrentDungeon()->SaveLevel();
		game::SetCurrent(game::GetCurrent() - 1);
		game::GetCurrentDungeon()->PrepareLevel();
		game::GetCurrentLevel()->AddCharacter(game::GetCurrentLevel()->GetDownStairs(), Who);
		game::GetCurrentLevel()->Luxify();
		game::GetCurrentLevel()->UpdateLOS();
		game::GetCurrentLevel()->SendNewDrawRequest();
		return true;
	}
	else
	{
		if(Who == game::GetPlayer())
			if(Who->HasPerttusNut())
			{
				if(Who->HasMaakotkaShirt() && game::GetGod(1)->GetRelation() == 1000)
				{
					iosystem::TextScreen(FONTW, "A heavenly choir starts to sing Grandis Rana and a booming voice fills the air:\n\n\"Mortal! Thou hast surpassed Perttu, and pleaseth Me greatly during thine adventures!\nI hereby title thee as My new Überpriest!\"\n\nYou are victorious!");
					game::RemoveSaves();
					game::Quit();

					if(!game::GetWizardMode())
					{
						game::GetPlayer()->AddScoreEntry("ascended to Überpriesthood", 5);
						highscore HScore;
						HScore.Draw(FONTW, FONTB);
					}
					return true;
				}
				else
				{
					iosystem::TextScreen(FONTW, "As you ascend, an undead and sinister voice greets you:\n\n\"MoRtAl! ThOu HaSt SlAuGtHeReD pErTtU aNd PlEaSeD mE!\nfRoM tHiS dAy On, YoU aRe ThE dEaReSt SeRvAnT oF aLl EvIl!\"\n\nYou are victorious!");
					game::RemoveSaves();
					game::Quit();

					if(!game::GetWizardMode())
					{
						game::GetPlayer()->AddScoreEntry("killed Perttu and became the Avatar of Chaos", 4);
						highscore HScore;
						HScore.Draw(FONTW, FONTB);
					}
					return true;
				}
			}
			else
			{
				iosystem::TextScreen(FONTW, "Thou escaped from the dungeon and were executed for not obeying the Überpriest!\n\nGame Over.");
				game::RemoveSaves();
				game::Quit();

				if(!game::GetWizardMode())
				{
					game::GetPlayer()->AddScoreEntry("escaped from the dungeon and was executed for cowardice");
					highscore HScore;
					HScore.Draw(FONTW, FONTB);
				}
				return true;
			}

		return false;
	}
}

bool stairsdown::GoDown(character* Who) const  // Try to go down
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
		//vector2d Pos = Who->GetPos();
		game::GetCurrentDungeon()->SaveLevel();
		game::SetCurrent(game::GetCurrent() + 1);
		game::GetCurrentDungeon()->PrepareLevel();
		game::GetCurrentLevel()->AddCharacter(game::GetCurrentLevel()->GetUpStairs(), Who);
		game::GetCurrentLevel()->Luxify();
		game::GetCurrentLevel()->UpdateLOS();
		game::GetCurrentLevel()->SendNewDrawRequest();
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


void door::Kick(ushort, bool ShowOnScreen, uchar)
{
	if(!GetIsWalkable() && ShowOnScreen) ADD_MESSAGE("The door opens.");
		MakeWalkable();
}

