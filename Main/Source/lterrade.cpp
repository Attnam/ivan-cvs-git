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
#include "worldmap.h"
#include "charba.h"
#include "team.h"

#include <vector>

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
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
	igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetOwnerGod() << 4, 0, 0, 0, 16, 16);
}

void altar::Load(inputfile& SaveFile)
{
	levelterrain::Load(SaveFile);

	SaveFile >> OwnerGod;
}

void altar::Save(outputfile& SaveFile) const
{
	levelterrain::Save(SaveFile);

	SaveFile << OwnerGod;
}

bool stairsup::GoUp(character* Who) const  // Try to go up
{
	if(game::GetCurrent())
	{
		if(game::GetCurrent() == 9)
			if(!game::BoolQuestion("Somehow you get the feeling you cannot return. Continue anyway? [y/N]"))
				return false;

		std::vector<character*> MonsterList;

		DO_FOR_SQUARES_AROUND(Who->GetPos().X, Who->GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
		{
			character* Char = game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter();

			if(Char)
			{
				MonsterList.push_back(Char);
				game::GetCurrentLevel()->RemoveCharacter(vector2d(DoX, DoY));
			}
		})

		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		game::GetCurrentDungeon()->SaveLevel();
		game::SetCurrent(game::GetCurrent() - 1);
		game::GetCurrentDungeon()->PrepareLevel();
		game::GetCurrentLevel()->GetSquare(game::GetCurrentLevel()->GetDownStairs())->KickAnyoneStandingHereAway();
		game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetDownStairs(), Who);

		for(std::vector<character*>::iterator c = MonsterList.begin(); c != MonsterList.end(); ++c)
			game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(game::GetCurrentLevel()->GetDownStairs()), *c);

		game::GetCurrentLevel()->Luxify();
		game::GetCurrentLevel()->UpdateLOS();
		game::UpdateCamera();
		return true;
	}
	else
	{
		if(Who == game::GetPlayer())
		{
			std::vector<character*> TempPlayerGroup;

			DO_FOR_SQUARES_AROUND(Who->GetPos().X, Who->GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
			{
				character* Char = game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter();

				if(Char)
				{
					if(Char->GetTeam()->GetRelation(Who->GetTeam()) == HOSTILE)
					{
						ADD_MESSAGE("You can't escape when there are hostile creatures nearby.");
						return false;
					}

					if(Char->GetTeam() == Who->GetTeam())
					{
						TempPlayerGroup.push_back(Char);
						game::GetCurrentLevel()->RemoveCharacter(vector2d(DoX, DoY));
					}
				}
			})

			game::GetCurrentArea()->RemoveCharacter(Who->GetPos());
			game::GetCurrentDungeon()->SaveLevel();
			game::LoadWorldMap();

			game::GetWorldMap()->GetPlayerGroup().swap(TempPlayerGroup);

			game::SetInWilderness(true);
			game::GetCurrentArea()->AddCharacter(game::GetCurrentDungeon()->GetWorldMapPos(), Who);
			game::GetCurrentArea()->UpdateLOS();
			game::UpdateCamera();
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
			ADD_MESSAGE("A great evil power seems to tremble under your feet. You feel you shouldn't wander any further.");

			if(!game::BoolQuestion("Continue anyway? [y/N]"))
				return false;

			Who->GetLevelSquareUnder()->ChangeLevelTerrain(new parquet, new empty);
		}

		std::vector<character*> MonsterList;

		DO_FOR_SQUARES_AROUND(Who->GetPos().X, Who->GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
		{
			character* Char = game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter();

			if(Char)
			{
				MonsterList.push_back(Char);
				game::GetCurrentLevel()->RemoveCharacter(vector2d(DoX, DoY));
			}
		})

		game::GetCurrentLevel()->RemoveCharacter(Who->GetPos());
		game::GetCurrentDungeon()->SaveLevel();
		game::SetCurrent(game::GetCurrent() + 1);
		game::GetCurrentDungeon()->PrepareLevel();
		game::GetCurrentLevel()->GetSquare(game::GetCurrentLevel()->GetUpStairs())->KickAnyoneStandingHereAway();
		game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetUpStairs(), Who);

		for(std::vector<character*>::iterator c = MonsterList.begin(); c != MonsterList.end(); ++c)
			game::GetCurrentLevel()->FastAddCharacter(game::GetCurrentLevel()->GetNearestFreeSquare(game::GetCurrentLevel()->GetUpStairs()), *c);

		game::GetCurrentLevel()->Luxify();
		game::ShowLevelMessage();
		game::UpdateCamera();
		game::GetCurrentLevel()->UpdateLOS();
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

void door::Save(outputfile& SaveFile) const
{
	levelterrain::Save(SaveFile);

	SaveFile << IsOpen;
}

void door::Load(inputfile& SaveFile)
{
	levelterrain::Load(SaveFile);

	SaveFile >> IsOpen;
}

void door::MakeWalkable()
{
	IsOpen = true;

	UpdatePicture();

	GetLevelSquareUnder()->SendNewDrawRequest();
	GetLevelSquareUnder()->SendMemorizedUpdateRequest();
	GetLevelSquareUnder()->SetDescriptionChanged(true);
	GetLevelSquareUnder()->ForceEmitterEmitation();

	if(GetLevelSquareUnder()->GetLastSeen() == game::GetLOSTurns())
		game::GetCurrentLevel()->UpdateLOS();
}

void door::MakeNotWalkable()
{
	GetLevelSquareUnder()->ForceEmitterNoxify();

	IsOpen = false;

	UpdatePicture();

	GetLevelSquareUnder()->SendNewDrawRequest();
	GetLevelSquareUnder()->SendMemorizedUpdateRequest();
	GetLevelSquareUnder()->SetDescriptionChanged(true);
	GetLevelSquareUnder()->ForceEmitterEmitation();

	if(GetLevelSquareUnder()->GetLastSeen() == game::GetLOSTurns())
		game::GetCurrentLevel()->UpdateLOS();
}

void altar::StepOn(character* Stepper)
{
	if(Stepper->GetIsPlayer() && OwnerGod && !game::GetGod(OwnerGod)->GetKnown())
	{
		ADD_MESSAGE("The ancient altar is covered with strange markings. You manage to decipher them.");
		ADD_MESSAGE("The altar is dedicated to %s, the %s.", game::GetGod(OwnerGod)->Name().c_str(), game::GetGod(OwnerGod)->Description().c_str());
		ADD_MESSAGE("You now know the sacred rituals that allow you to contact this deity via prayers.");
		game::GetGod(OwnerGod)->SetKnown(true);
	}
}

void throne::SitOn(character* Sitter)
{
	if(Sitter->HasPerttusNut() && Sitter->HasMaakotkaShirt() && game::GetGod(1)->GetRelation() != 1000)
	{
		ADD_MESSAGE("You have a strange vision of yourself becoming great ruler.");
		ADD_MESSAGE("The daydream fades in a whisper: \"Thou shalt be a My Champion first!\"");
		return;
	}

	if(Sitter->HasPerttusNut() && !Sitter->HasMaakotkaShirt() && game::GetGod(1)->GetRelation() == 1000)
	{
		ADD_MESSAGE("You have a strange vision of yourself becoming great ruler.");
		ADD_MESSAGE("The daydream fades in a whisper: \"Thou shalt wear My shining armor first!\"");
		return;
	}

	if(!Sitter->HasPerttusNut() && Sitter->HasMaakotkaShirt() && game::GetGod(1)->GetRelation() == 1000)
	{
		ADD_MESSAGE("You have a strange vision of yourself becoming great ruler.");
		ADD_MESSAGE("The daydream fades in a whisper: \"Thou shalt surpass thy predecessor first!\"");
		return;
	}

	if(Sitter->HasPerttusNut() && Sitter->HasMaakotkaShirt() && game::GetGod(1)->GetRelation() == 1000)
	{
		iosystem::TextScreen("A heavenly choir starts to sing Grandis Rana and a booming voice fills the air:\n\n\"Mortal! Thou hast surpassed Perttu, and pleaseth Me greatly during thine adventures!\nI hereby title thee as My new Überpriest!\"\n\nYou are victorious!");
		game::RemoveSaves();
		game::Quit();

		if(!game::GetWizardMode())
		{
			game::GetPlayer()->AddScoreEntry("ascended to Überpriesthood", 5, false);
			highscore HScore;
			HScore.Draw();
		}

		return;
	}

	ADD_MESSAGE("You feel somehow out of place.");
}

void altar::Kick(ushort, bool ShowOnScreen, uchar)
{
	// This function does not currently support AI kicking altars when they are in player's LOS

	if(ShowOnScreen)
		ADD_MESSAGE("You feel like a sinner.");

	game::GetGod(GetLevelSquareUnder()->GetDivineOwner())->PlayerKickedAltar();

	if(GetLevelSquareUnder()->GetDivineOwner() > 1)
		game::GetGod(GetLevelSquareUnder()->GetDivineOwner() - 1)->PlayerKickedFriendsAltar();

	if(GetLevelSquareUnder()->GetDivineOwner() < game::GetGodNumber())
		game::GetGod(GetLevelSquareUnder()->GetDivineOwner() + 1)->PlayerKickedFriendsAltar();
}

void altar::ReceiveVomit(character* Who)
{
	if(Who->GetIsPlayer())
		game::GetGod(GetLevelSquareUnder()->GetDivineOwner())->PlayerVomitedOnAltar();
}

std::string door::Name(uchar Case) const
{
	if(!(Case & PLURAL))
		if(!(Case & DEFINEBIT))
			return std::string(IsOpen ? "open " : "closed ") + GetMaterial(0)->Name()  + " " + NameSingular();
		else
			if(!(Case & INDEFINEBIT))
				return std::string(IsOpen ? "the open " : "the closed ") + GetMaterial(0)->Name()  + " " + NameSingular();
			else
				return std::string(IsOpen ? "an open " : "a closed ") + GetMaterial(0)->Name()  + " " + NameSingular();
	else
		if(!(Case & DEFINEBIT))
			return GetMaterial(0)->Name()  + " " + NamePlural();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + GetMaterial(0)->Name()  + " " + NamePlural();
			else
				return GetMaterial(0)->Name()  + " " + NamePlural();
}

void couch::SitOn(character*)
{
	ADD_MESSAGE("The couch is extremely soft and confortable. You relax well.");
}

void pool::SitOn(character*)
{
	ADD_MESSAGE("You sit on the pool. Oddly enough, you sink. You feel stupid.");
}

void stairsup::StepOn(character* Stepper)
{
	if(Stepper->GetIsPlayer()) 
		ADD_MESSAGE("There is stairway leading upwards here.");
}

void stairsdown::StepOn(character* Stepper)
{
	if(Stepper->GetIsPlayer()) 
		ADD_MESSAGE("There is stairway leading downwards here.");
}