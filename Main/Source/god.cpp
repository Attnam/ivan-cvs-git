#include <cstdio>
#include <cstdlib>

#include "game.h"
#include "god.h"
#include "level.h"
#include "lsquare.h"
#include "char.h"
#include "item.h"
#include "stack.h"

void god::Pray(void)
{
	if(!Timer)
		if(Relation > -(rand() % 501))
		{
			ADD_MESSAGE("You feel %s is pleased.", GOD_NAME);
			PrayGoodEffect();
			AdjustTimer(500);
			AdjustRelation(50);
			game::ApplyDivineAlignmentBonuses(this, true);
		}
		else
		{
			ADD_MESSAGE("You feel %s is displeased today.", GOD_NAME);
			PrayBadEffect();
			AdjustTimer(1000);
			game::ApplyDivineAlignmentBonuses(this, false);
		}
	else
		if(Relation >= rand() % 501)
		{
			ADD_MESSAGE("You feel %s is displeased, but helps you anyway.", GOD_NAME);
			PrayGoodEffect();
			AdjustTimer(2500);
			AdjustRelation(-50);
			game::ApplyDivineAlignmentBonuses(this, false);
		}
		else
		{
			ADD_MESSAGE("You feel %s is angry.", GOD_NAME);
			PrayBadEffect();
			AdjustTimer(5000);
			AdjustRelation(-100);
			game::ApplyDivineAlignmentBonuses(this, false);
		}
}

std::string god::CompleteDescription(void) const
{
	std::string Desc = game::CAlignment(Alignment());

	Desc.resize(4, ' ');

	Desc += Name();

	Desc.resize(30, ' ');

	if(game::CWizardMode())
	{
		char Buffer[256];

		sprintf(Buffer, "%d - %d", int(Timer), int(Relation));

		return Desc + Buffer;
	}

	return Desc + "the " + Description();
}

void god::PrayGoodEffect(void)
{
	ADD_MESSAGE("%s doesn't know what good he could do to you for now.", GOD_NAME);
}

void god::PrayBadEffect(void)
{
	ADD_MESSAGE("%s doesn't know how to punish at the moment.", GOD_NAME);
}

void god::AdjustRelation(god* Competitor, bool Good)
{
	short Adjustment = 50 - abs((signed char)(Alignment()) - Competitor->Alignment()) * 25;

	if(!Good && Adjustment > 0)
		Adjustment = -Adjustment;

	AdjustRelation(Adjustment);
}

void god::AdjustRelation(short Amount)
{
	Relation += Amount;

	if(Relation < -1000)
		Relation = -1000;

	if(Relation > 1000)
		Relation = 1000;
}

void god::AdjustTimer(long Amount)
{
	Timer += Amount;

	if(Timer < 0)
		Timer = 0;

	if(Timer > 1000000000)
		Timer = 1000000000;
}

void consummo::PrayGoodEffect(void)
{
	ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturaly powerful quantum displacement! You teleport away!");

	vector Pos;

	for(;;)
	{
		Pos = game::CCurrentLevel()->RandomSquare(true);

		if(game::CCurrentLevel()->CLevelSquare(Pos)->CCharacter() == 0)
			break;
	}

	game::CPlayer()->Move(Pos, true);
}

void consummo::PrayBadEffect(void)
{
	ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturaly powerful quantum displacement! Some parts of you teleport away!");

	game::CPlayer()->SHP(game::CPlayer()->CHP() - rand() % (game::CPlayer()->CEndurance() << 1));

	game::CPlayer()->CheckDeath(std::string("shattered to pieces by the wrath of ") + Name());
}

void valpuri::PrayGoodEffect(void)
{
	ADD_MESSAGE("A valpurium curved two-handed sword drops on to the ground.");
	item* Reward = new curvedtwohandedsword(175, false);
	Reward->InitMaterials(3, new valpurium(5500), new iron(250), 0);
	game::CPlayer()->CLevelSquareUnder()->CStack()->AddItem(Reward);
}

void valpuri::PrayBadEffect(void)
{
	ADD_MESSAGE("Valpuri smites you with a small hammer.");
	game::CPlayer()->SHP(game::CPlayer()->CHP() - 5);
	game::CPlayer()->CheckDeath(std::string("faced the hammer of Justice from the hand of ") + Name());
}

void venius::PrayGoodEffect(void)
{
	ADD_MESSAGE("A booming voice echoes: \"Xunil! Xunil! Save us!\" A huge divine fire sweeps the surrounding area.");
	DO_FOR_SQUARES_AROUND(game::CPlayer()->CPos().X, game::CPlayer()->CPos().Y, game::CCurrentLevel()->CXSize(), game::CCurrentLevel()->CYSize(),
	if(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter())
	{
		game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter()->ReceiveFireDamage(20);
		game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter()->CheckDeath(std::string("burned to death by the wrath of ") + Name());
	})
}

void venius::PrayBadEffect(void)
{
	ADD_MESSAGE("%s casts a beam of horrible, yet righteous, fire on you.", GOD_NAME);
	game::CPlayer()->ReceiveFireDamage(20);
	game::CPlayer()->CheckDeath(std::string("burned to death by the wrath of ") + Name());
}

void dulcis::PrayGoodEffect(void)
{
	ADD_MESSAGE("A beatiful melody echoes through the dungeon.");
	DO_FOR_SQUARES_AROUND(game::CPlayer()->CPos().X, game::CPlayer()->CPos().Y, game::CCurrentLevel()->CXSize(), game::CCurrentLevel()->CYSize(),
	character* Char = game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter();
	if(Char && Char->CRelations() == 0)
	{
		if(Char->Charmable())
		{
			Char->SRelations(1);
			ADD_MESSAGE("%s stops fighting.", game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter()->CNAME(DEFINITE));
		}
		else
			ADD_MESSAGE("%s seems not affected.", game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter()->CNAME(DEFINITE));
	})
}

void dulcis::PrayBadEffect(void)
{
	ADD_MESSAGE("%s plays a horrible tune that rots your brain.", GOD_NAME);
	game::CPlayer()->SHP(game::CPlayer()->CHP() - rand() % 9 + 1);
	game::CPlayer()->CheckDeath(std::string("became insane by listening ") + Name() + " too much");
}

void inasnum::PrayGoodEffect(void)
{
	ADD_MESSAGE("%s gives you a hint.", GOD_NAME);
	switch(rand() % 7)
	{
	case 0:
		ADD_MESSAGE("His eye was found in a mug.");
	break;
	case 1:
		ADD_MESSAGE("Master Evil, the Pepsi Daemon King, has escabeth not upwards but downwards in to the pits of dev/null with a great foe of the sword.");
	break;
	case 2:
		ADD_MESSAGE("Thou shalt not hit thine lesser brethren with a biggest thing you can find in this universe.");
	break;
	case 3:
		ADD_MESSAGE("If thy hast fought but not conquered thine foe thy must lick thine wounds in peace.");
	break;
	case 4:
		ADD_MESSAGE("School... No, no...its right behind me!!! Nooooo... Why is it coming into my dreams?");
	break;
	case 5:
		ADD_MESSAGE("Thou shalt not hurry in thine killing.");
	break;
	case 6:
		ADD_MESSAGE("If thy shall eat frogs, thy will be foul.");
	break;
	default:
		ADD_MESSAGE("He created me from the fire to destroy all gods who oppose Him.");
	}
}

void inasnum::PrayBadEffect(void)
{
	ADD_MESSAGE("%s gives you a hint.", GOD_NAME);
	switch(rand() % 3)
	{
	case 0:
		ADD_MESSAGE("Dancing in front of Bill's SWAT professional will calm him down.");
	break;
	case 1:
		ADD_MESSAGE("Writing \"Elbereth\" to the ground will make you safe.");
	break;
	default:
		ADD_MESSAGE("Engraving \"29392172387Fda3419080418012838\" will give thou a bag of money.");
	}
}

void seges::PrayGoodEffect(void)
{
	ADD_MESSAGE("Your stomach feels full again.");

	if(game::CPlayer()->CNP() < 1000)
		game::CPlayer()->SNP(1000);
}

void seges::PrayBadEffect(void)
{
	ADD_MESSAGE("You suddenly feel more hungry.");
	game::CPlayer()->SNP(game::CPlayer()->CNP() - 400);
	if(game::CPlayer()->CNP() < 1)
	{
		ADD_MESSAGE("In fact you feel extremely hungry.");
		game::CPlayer()->CheckDeath(std::string("starved by ") + Name());
	}
}

void atavus::PrayGoodEffect(void)
{
	ADD_MESSAGE("A mithril platemail drops on the ground.");
	item* Reward = new platemail(75, false);
	Reward->InitMaterials(new mithril(4000));
	game::CPlayer()->CLevelSquareUnder()->CStack()->AddItem(Reward);
}

void atavus::PrayBadEffect(void)
{
	ADD_MESSAGE("You have not been good the whole year.");

	if(game::CPlayer()->CStack()->CItems())
	{
		ushort ToBeDeleted = rand() % game::CPlayer()->CStack()->CItems();
		item* Disappearing = game::CPlayer()->CStack()->CItem()[ToBeDeleted];
		if(Disappearing->Destroyable())
		{
			ADD_MESSAGE("Your %s disappears.", Disappearing->CNAME(INDEFINITE));
			game::CPlayer()->CStack()->RemoveItem(ToBeDeleted);
			if(game::CPlayer()->CWielded() == Disappearing) game::CPlayer()->SWielded(0);
			if(game::CPlayer()->CTorsoArmor() == Disappearing) game::CPlayer()->WearItem(0);
			delete Disappearing;
		}
		else
		{
			ADD_MESSAGE("%s tries to remove your %s, but fails.", GOD_NAME, Disappearing->CNAME(INDEFINITE));
			ADD_MESSAGE("You feel you are not so gifted anymore.");
			game::CPlayer()->SAgility(game::CPlayer()->CAgility() - 1);
			game::CPlayer()->SStrength(game::CPlayer()->CStrength() - 1);
			game::CPlayer()->SEndurance(game::CPlayer()->CEndurance() - 1);
		}
	}
	else
	{
		ADD_MESSAGE("You feel you are not so gifted anymore.");
		game::CPlayer()->SAgility(game::CPlayer()->CAgility() - 1);
		game::CPlayer()->SStrength(game::CPlayer()->CStrength() - 1);
		game::CPlayer()->SEndurance(game::CPlayer()->CEndurance() - 1);
	}
}

void loricatus::PrayGoodEffect(void)
{
	std::string OldName;
	if(game::CPlayer()->CWielded())
	{
		OldName = game::CPlayer()->CWielded()->Name(INDEFINITE);
		game::CPlayer()->CWielded()->SMaterial(0, new mithril(game::CPlayer()->CWielded()->CMaterial(0)->CVolume()));
		ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::CPlayer()->CWielded()->CNAME(DEFINITE));
	}
}

void loricatus::PrayBadEffect(void)
{
	std::string OldName;
	if(game::CPlayer()->CWielded())
	{
		OldName = game::CPlayer()->CWielded()->Name(INDEFINITE);
		game::CPlayer()->CWielded()->SMaterial(0, new bananaflesh(game::CPlayer()->CWielded()->CMaterial(0)->CVolume()));
		ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::CPlayer()->CWielded()->CNAME(DEFINITE));
	}
	else
		ADD_MESSAGE("You feel a loss.");
}


void calamus::PrayGoodEffect(void)
{
	ADD_MESSAGE("%s gives you the talent for speed.", GOD_NAME);
	game::CPlayer()->SAgility(game::CPlayer()->CAgility() + 1);
}

void calamus::PrayBadEffect(void)
{
	if(game::CPlayer()->CAgility() > 5)
	{
		ADD_MESSAGE("%s slows you down.", GOD_NAME);
		game::CPlayer()->SAgility(game::CPlayer()->CAgility() - 1);
	}
	else
		ADD_MESSAGE("Suprisingly you feel nothing.");
}

void god::Save(std::ofstream* SaveFile)
{
	SaveFile->write((char*)&Relation, sizeof(Relation));
	SaveFile->write((char*)&Timer, sizeof(Timer));
}

void god::Load(std::ifstream* SaveFile)
{
	SaveFile->read((char*)&Relation, sizeof(Relation));
	SaveFile->read((char*)&Timer, sizeof(Timer));
}

void erado::PrayGoodEffect(void)
{
	ADD_MESSAGE("The air vibrates violently around you and a terrible undead voice echoes through the caverns: \"SlAvE! ThOu HaSt PlAeSeD mE! lIfT tHiNe ReWaRd, ChAmPiOn!\" A heavy weapon of pure corruption falls on the ground.");

	game::CPlayer()->CLevelSquareUnder()->CStack()->AddItem(new htaedfoneercseulb);
}

void erado::PrayBadEffect(void)
{
	ADD_MESSAGE("A dark, booming voice shakes the walls: \"PuNy MoRtAl! YoU aRe NoT wOrThY! i ShAlL DeStRoY yOu LiKe EvErYoNe ElSe!\" A bolt of black energy hits you.");

	game::CPlayer()->SHP(game::CPlayer()->CHP() - (game::CPlayer()->CEndurance() << 1) + 1);
	game::CPlayer()->SAgility(game::CPlayer()->CAgility() - 1);
	game::CPlayer()->SStrength(game::CPlayer()->CStrength() - 1);
	game::CPlayer()->SEndurance(game::CPlayer()->CEndurance() - 1);
	game::CPlayer()->CheckDeath(std::string("obliterated by the unholy power of ") + Name());
}

void mellis::PrayGoodEffect(void)
{
	bool Success = false;
	ushort JustCreated;
	if(game::CPlayer()->CStack()->CItems())
	{
		ADD_MESSAGE("%s tries to trade some of your items into better ones.", GOD_NAME);
		bool Cont = true;
		while(Cont)
		{
		Cont = false;
		for(ushort c = 0; c < game::CPlayer()->CStack()->CItems(); c++)
		{
			if(game::CPlayer()->CStack()->CItem()[c] && game::CPlayer()->CStack()->CItem()[c]->BetterVersion())
			{
				item* ToBeDeleted = game::CPlayer()->CStack()->CItem()[c];
				game::CPlayer()->CStack()->RemoveItem(c);
				JustCreated = game::CPlayer()->CStack()->AddItem(ToBeDeleted->BetterVersion());
				Success = true;
				ADD_MESSAGE("%s manages to trade %s into %s.", GOD_NAME, ToBeDeleted->CNAME(DEFINITE), game::CPlayer()->CStack()->CItem()[JustCreated]->CNAME(DEFINITE));
				if(ToBeDeleted == game::CPlayer()->CWielded()) game::CPlayer()->SWielded(0);
				if(ToBeDeleted == game::CPlayer()->CTorsoArmor()) game::CPlayer()->WearItem(0);
				delete ToBeDeleted;
				Cont = true;
				break;
			}
		}
		}
	}
	if(!Success) ADD_MESSAGE("You have no good items for trade.");
}

void mellis::PrayBadEffect(void)
{
	for(ushort c = 1;; c++)
	{
		if(game::CGod(c))
		{
			if(game::CGod(c) != this)
				game::CGod(c)->AdjustRelation(-100);
		}
		else
			break;
	}
	ADD_MESSAGE("%s spreads bad rumours about you to other gods.", GOD_NAME);
}

void pestifer::PrayGoodEffect(void)
{
	ADD_MESSAGE("%s increases your endurance by howling horrible sounds into your ear.", GOD_NAME);
	game::CPlayer()->SEndurance(game::CPlayer()->CEndurance() + 1);
}

void pestifer::PrayBadEffect(void)
{
	game::CCurrentLevel()->CLevelSquare(game::CCurrentLevel()->RandomSquare(true))->AddCharacter(new ennerbeast);
	ADD_MESSAGE("You hear the shouts of a new enner beast!");
}

void valpuri::Pray(void)
{
	if(!Timer && Relation == 1000)
	{
		ADD_MESSAGE("You feel %s is very pleased.", GOD_NAME);
		PrayGoodEffect();
		AdjustTimer(10000);
		AdjustRelation(-500);
		game::ApplyDivineAlignmentBonuses(this, true);
	}
	else
	{
		ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GOD_NAME);
		PrayBadEffect();
		AdjustTimer(5000);
		AdjustRelation(-100);
		game::ApplyDivineAlignmentBonuses(this, false);
	}
}

void atavus::Pray(void)
{
	if(!Timer && Relation > 500 + rand() % 500)
	{
		ADD_MESSAGE("You feel %s is pleased.", GOD_NAME);
		PrayGoodEffect();
		AdjustTimer(5000);
		AdjustRelation(-250);
		game::ApplyDivineAlignmentBonuses(this, true);
	}
	else
	{
		ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GOD_NAME);
		PrayBadEffect();
		AdjustTimer(5000);
		AdjustRelation(-100);
		game::ApplyDivineAlignmentBonuses(this, false);
	}
}

void erado::Pray(void)
{
	if(!Timer && Relation == 1000)
	{
		ADD_MESSAGE("You feel %s is very pleased.", GOD_NAME);
		PrayGoodEffect();
		AdjustTimer(10000);
		AdjustRelation(-500);
		game::ApplyDivineAlignmentBonuses(this, true);
	}
	else
	{
		ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GOD_NAME);
		PrayBadEffect();
		AdjustTimer(5000);
		AdjustRelation(-100);
		game::ApplyDivineAlignmentBonuses(this, false);
	}
}

void infuscor::PrayBadEffect(void)
{
	ADD_MESSAGE("Vile and evil knowledge pulps into your brain. It's too much for it to handle; you faint.");

	game::CPlayer()->SAP(-10000);
	game::CPlayer()->SNP(game::CPlayer()->CNP() - 200);
}

void macellarius::PrayGoodEffect(void)
{
	ADD_MESSAGE("%s wishes you to have fun with this potion.", GOD_NAME);
	item* Reward = new potion(30, false);
	Reward->InitMaterials(2, new glass(50), new omleurine(1500));
	game::CPlayer()->CLevelSquareUnder()->CStack()->AddItem(Reward);
	ADD_MESSAGE("%s drops on the ground.", Reward->CNAME(DEFINITE));
}

void macellarius::PrayBadEffect(void)
{
	ADD_MESSAGE("A potion drops on your head and shatters into small bits.");
	game::CPlayer()->SHP(game::CPlayer()->CHP() - rand() % 7);
	game::CPlayer()->CLevelSquareUnder()->CStack()->AddItem(new brokenbottle);
	game::CPlayer()->CheckDeath(std::string("killed while enjoying the company of ") + Name());
}

void scabies::PrayGoodEffect(void)
{
	ADD_MESSAGE("5 cans full of school food drop to on the ground.");
	for(uchar c = 0; c < 5; c++)
	{
		item* Reward = new can(10, false);
		Reward->InitMaterials(2, new iron(50), new schoolfood(600));
		game::CPlayer()->CLevelSquareUnder()->CStack()->AddItem(Reward);
	}
}

void scabies::PrayBadEffect(void)
{
	ADD_MESSAGE("%s makes you eat a LOT of school food.", GOD_NAME);

	for(uchar c = 0; c < 5; c++)
	{
		material* SchoolFood = new schoolfood(600);
		SchoolFood->EatEffect(game::CPlayer());
		delete SchoolFood;
	}

	ADD_MESSAGE("You feel your muscles softening terribly...");

	game::CPlayer()->SStrength(game::CPlayer()->CStrength() - 1);
	game::CPlayer()->SAgility(game::CPlayer()->CAgility() - 1);
}

void infuscor::PrayGoodEffect(void)
{
	ADD_MESSAGE("5 scrolls drop to on the ground.");
	for(uchar c = 0; c < 5; c++)
	{
		game::CPlayer()->CLevelSquareUnder()->CStack()->AddItem(new scrollofteleport);
	}
}

void cruentus::PrayGoodEffect(void)
{
	ADD_MESSAGE("Cruentus recommends you to its master, Erado.");
	game::CGod(16)->AdjustRelation(100);
}

void cruentus::PrayBadEffect(void)
{
	item* ToBe;

	ToBe = game::CPlayer()->CWielded();

	if(ToBe && !ToBe->Destroyable())
		ADD_MESSAGE("%s tries to destroy your weapon, but fails.", GOD_NAME);

	if(ToBe && ToBe->Destroyable())
	{
		ADD_MESSAGE("%s destroys your weapon.", GOD_NAME);

		game::CPlayer()->CStack()->RemoveItem(game::CPlayer()->CStack()->SearchItem(ToBe));
		game::CPlayer()->SWielded(0);
		delete ToBe;
	}
	else
	{
		ADD_MESSAGE("%s gets mad and hits you!", GOD_NAME);
		game::CPlayer()->SHP(game::CPlayer()->CHP() - rand() % 20);
		game::CPlayer()->CheckDeath(std::string("destroyed by ") + Name());
	}

}

void cruentus::Pray(void)
{
	if(!Timer && Relation > 500 + rand() % 500)
	{
		ADD_MESSAGE("You feel %s is pleased.", GOD_NAME);
		PrayGoodEffect();
		AdjustTimer(5000);
		AdjustRelation(-250);
		game::ApplyDivineAlignmentBonuses(this, true);
	}
	else
	{
		ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GOD_NAME);
		PrayBadEffect();
		AdjustTimer(5000);
		AdjustRelation(-100);
		game::ApplyDivineAlignmentBonuses(this, false);
	}
}

bool god::ReceiveOffer(item* Sacrifice)
{
	short OfferValue = Sacrifice->CalculateOfferValue(BasicAlignment());

	if(abs(OfferValue) > 5)
	{
		if(!Sacrifice->Destroyable())
		{
			ADD_MESSAGE("%s is too important for you to be sacrificed.", Sacrifice->CNAME(DEFINITE));

			return false;
		}

		AdjustRelation(OfferValue);
		game::ApplyDivineAlignmentBonuses(this, OfferValue > 0 ? true : false);

		if(OfferValue > 0)
			ADD_MESSAGE("%s thanks you for your gift.", GOD_NAME);
		else
			ADD_MESSAGE("%s seems not to appreciate your gift at all.", GOD_NAME);

		PrintRelation();

		return true;
	}
	else
	{
		ADD_MESSAGE("Nothing happens.");
		return false;
	}
}

void god::PrintRelation(void)
{
	std::string VerbalRelation;
	if(CRelation() == 1000)
		VerbalRelation = "greets you as a Champion of the Cause!";
	else if(CRelation() > 750)
		VerbalRelation = "is extremely pleased.";
	else if(CRelation() > 250)
		VerbalRelation = "is very pleased.";
	else if(CRelation() > 50)
		VerbalRelation = "is pleased.";
	else if(CRelation() > -50)
		VerbalRelation = "is content.";
	else if(CRelation() > -250)
		VerbalRelation = "is angry.";
	else if(CRelation() > -750)
		VerbalRelation = "is very angry.";
	else if(CRelation() > -1000)
		VerbalRelation = "is extremely angry.";
	else VerbalRelation = "hates you more than any other mortal.";

	ADD_MESSAGE("%s %s", GOD_NAME, VerbalRelation.c_str());
}
