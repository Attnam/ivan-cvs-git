#include "god.h"
#include "level.h"
#include "lsquare.h"
#include "charde.h"
#include "itemde.h"
#include "stack.h"
#include "message.h"
#include "proto.h"
#include "save.h"

void god::Pray()
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

std::string god::CompleteDescription() const
{
	std::string Desc = game::CAlignment(Alignment());

	Desc.resize(4, ' ');

	Desc += Name();

	Desc.resize(30, ' ');

	if(game::GetWizardMode())
	{
		char Buffer[256];

		sprintf(Buffer, "%d - %d", int(Timer), int(Relation));

		return Desc + Buffer;
	}

	return Desc + "the " + Description();
}

void god::PrayGoodEffect()
{
	ADD_MESSAGE("%s doesn't know what good he could do to you for now.", GOD_NAME);
}

void god::PrayBadEffect()
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

void consummo::PrayGoodEffect()
{
	ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturaly powerful quantum displacement! You teleport away!");

	vector2d Pos;

	for(;;)
	{
		Pos = game::GetCurrentLevel()->RandomSquare(true);

		if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter() == 0)
			break;
	}

	game::GetPlayer()->Move(Pos, true);
}

void consummo::PrayBadEffect()
{
	ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturaly powerful quantum displacement! Some parts of you teleport away!");

	game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - rand() % (game::GetPlayer()->GetEndurance() << 1));

	game::GetPlayer()->CheckDeath(std::string("shattered to pieces by the wrath of ") + Name());
}

void valpuri::PrayGoodEffect()
{
	ADD_MESSAGE("A valpurium curved two-handed sword drops on to the ground.");
	item* Reward = new curvedtwohandedsword(false);
	Reward->InitMaterials(3, new valpurium(5500), new iron(250), 0);
	game::GetPlayer()->GetLevelSquareUnder()->GetStack()->AddItem(Reward);
}

void valpuri::PrayBadEffect()
{
	ADD_MESSAGE("Valpuri smites you with a small hammer.");
	game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - 5);
	game::GetPlayer()->CheckDeath(std::string("faced the hammer of Justice from the hand of ") + Name());
}

void venius::PrayGoodEffect()
{
	ADD_MESSAGE("A booming voice echoes: \"Xunil! Xunil! Save us!\" A huge divine fire sweeps the surrounding area.");
	DO_FOR_SQUARES_AROUND(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	if(game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter())
	{
		game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter()->ReceiveFireDamage(20);
		game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter()->CheckDeath(std::string("burned to death by the wrath of ") + Name());
	})
}

void venius::PrayBadEffect()
{
	ADD_MESSAGE("%s casts a beam of horrible, yet righteous, fire on you.", GOD_NAME);
	game::GetPlayer()->ReceiveFireDamage(20);
	game::GetPlayer()->CheckDeath(std::string("burned to death by the wrath of ") + Name());
}

void dulcis::PrayGoodEffect()
{
	ADD_MESSAGE("A beatiful melody echoes through the dungeon.");
	DO_FOR_SQUARES_AROUND(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	character* Char = game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter();
	if(Char && Char->GetRelations() == 0)
	{
		if(Char->Charmable())
		{
			Char->SetRelations(1);
			ADD_MESSAGE("%s stops fighting.", game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter()->CNAME(DEFINITE));
		}
		else
			ADD_MESSAGE("%s seems not affected.", game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter()->CNAME(DEFINITE));
	})
}

void dulcis::PrayBadEffect()
{
	ADD_MESSAGE("%s plays a horrible tune that rots your brain.", GOD_NAME);
	game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - rand() % 9 + 1);
	game::GetPlayer()->CheckDeath(std::string("became insane by listening ") + Name() + " too much");
}

void inasnum::PrayGoodEffect()
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

void inasnum::PrayBadEffect()
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

void seges::PrayGoodEffect()
{
	ADD_MESSAGE("Your stomach feels full again.");

	if(game::GetPlayer()->GetNP() < 1000)
		game::GetPlayer()->SetNP(1000);
}

void seges::PrayBadEffect()
{
	ADD_MESSAGE("You suddenly feel more hungry.");
	game::GetPlayer()->SetNP(game::GetPlayer()->GetNP() - 400);
	if(game::GetPlayer()->GetNP() < 1)
	{
		ADD_MESSAGE("In fact you feel extremely hungry.");
		game::GetPlayer()->CheckDeath(std::string("starved by ") + Name());
	}
}

void atavus::PrayGoodEffect()
{
	ADD_MESSAGE("A mithril platemail drops on the ground.");
	item* Reward = new platemail(false);
	Reward->InitMaterials(new mithril(4000));
	game::GetPlayer()->GetLevelSquareUnder()->GetStack()->AddItem(Reward);
}

void atavus::PrayBadEffect()
{
	ADD_MESSAGE("You have not been good the whole year.");

	if(game::GetPlayer()->GetStack()->GetItems())
	{
		ushort ToBeDeleted = rand() % game::GetPlayer()->GetStack()->GetItems();
		item* Disappearing = game::GetPlayer()->GetStack()->GetItem(ToBeDeleted);
		if(Disappearing->Destroyable())
		{
			ADD_MESSAGE("Your %s disappears.", Disappearing->CNAME(UNARTICLED));
			game::GetPlayer()->GetStack()->RemoveItem(ToBeDeleted);
			if(game::GetPlayer()->GetWielded() == Disappearing) game::GetPlayer()->SetWielded(0);
			if(game::GetPlayer()->GetTorsoArmor() == Disappearing) game::GetPlayer()->SetTorsoArmor(0);
			delete Disappearing;
		}
		else
		{
			ADD_MESSAGE("%s tries to remove your %s, but fails.", GOD_NAME, Disappearing->CNAME(UNARTICLED));
			ADD_MESSAGE("You feel you are not so gifted anymore.");
			game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
			game::GetPlayer()->SetStrength(game::GetPlayer()->GetStrength() - 1);
			game::GetPlayer()->SetEndurance(game::GetPlayer()->GetEndurance() - 1);
		}
	}
	else
	{
		ADD_MESSAGE("You feel you are not so gifted anymore.");
		game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
		game::GetPlayer()->SetStrength(game::GetPlayer()->GetStrength() - 1);
		game::GetPlayer()->SetEndurance(game::GetPlayer()->GetEndurance() - 1);
	}
}

void loricatus::PrayGoodEffect()
{
	std::string OldName;
	if(game::GetPlayer()->GetWielded())
	{
		OldName = game::GetPlayer()->GetWielded()->Name(INDEFINITE);
		game::GetPlayer()->GetWielded()->SetMaterial(0, new mithril(game::GetPlayer()->GetWielded()->GetMaterial(0)->GetVolume()));
		ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::GetPlayer()->GetWielded()->CNAME(DEFINITE));
	}
}

void loricatus::PrayBadEffect()
{
	std::string OldName;
	if(game::GetPlayer()->GetWielded())
	{
		OldName = game::GetPlayer()->GetWielded()->Name(INDEFINITE);
		game::GetPlayer()->GetWielded()->SetMaterial(0, new bananaflesh(game::GetPlayer()->GetWielded()->GetMaterial(0)->GetVolume()));
		ADD_MESSAGE("Your %s changes into %s.", OldName.c_str(), game::GetPlayer()->GetWielded()->CNAME(DEFINITE));
	}
	else
		ADD_MESSAGE("You feel a loss.");
}


void calamus::PrayGoodEffect()
{
	ADD_MESSAGE("%s gives you the talent for speed.", GOD_NAME);
	game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() + 1);
}

void calamus::PrayBadEffect()
{
	if(game::GetPlayer()->GetAgility() > 5)
	{
		ADD_MESSAGE("%s slows you down.", GOD_NAME);
		game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
	}
	else
		ADD_MESSAGE("Suprisingly you feel nothing.");
}

void god::Save(outputfile& SaveFile) const
{
	SaveFile << Relation << Timer;
}

void god::Load(inputfile& SaveFile)
{
	SaveFile >> Relation >> Timer;
}

void erado::PrayGoodEffect()
{
	ADD_MESSAGE("The air vibrates violently around you and a terrible undead voice echoes through the caverns: \"SlAvE! ThOu HaSt PlAeSeD mE! lIfT tHiNe ReWaRd, ChAmPiOn!\" A heavy weapon of pure corruption falls on the ground.");

	game::GetPlayer()->GetLevelSquareUnder()->GetStack()->AddItem(new htaedfoneercseulb);
}

void erado::PrayBadEffect()
{
	ADD_MESSAGE("A dark, booming voice shakes the walls: \"PuNy MoRtAl! YoU aRe NoT wOrThY! i ShAlL DeStRoY yOu LiKe EvErYoNe ElSe!\" A bolt of black energy hits you.");

	game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - (game::GetPlayer()->GetEndurance() << 1) + 1);
	game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
	game::GetPlayer()->SetStrength(game::GetPlayer()->GetStrength() - 1);
	game::GetPlayer()->SetEndurance(game::GetPlayer()->GetEndurance() - 1);
	game::GetPlayer()->CheckDeath(std::string("obliterated by the unholy power of ") + Name());
}

void mellis::PrayGoodEffect()
{
	bool Success = false;
	ushort JustCreated;

	if(game::GetPlayer()->GetStack()->GetItems())
	{
		ADD_MESSAGE("%s tries to trade some of your items into better ones.", GOD_NAME);
		bool Cont = true;
		while(Cont)
		{
			Cont = false;
			for(ushort c = 0; c < game::GetPlayer()->GetStack()->GetItems(); ++c)
			{
				if(game::GetPlayer()->GetStack()->GetItem(c) && game::GetPlayer()->GetStack()->GetItem(c)->BetterVersion())
				{
					item* ToBeDeleted = game::GetPlayer()->GetStack()->GetItem(c);
					game::GetPlayer()->GetStack()->RemoveItem(c);
					JustCreated = game::GetPlayer()->GetStack()->AddItem(ToBeDeleted->BetterVersion());
					Success = true;
					ADD_MESSAGE("%s manages to trade %s into %s.", GOD_NAME, ToBeDeleted->CNAME(DEFINITE), game::GetPlayer()->GetStack()->GetItem(JustCreated)->CNAME(INDEFINITE));
					if(ToBeDeleted == game::GetPlayer()->GetWielded()) game::GetPlayer()->SetWielded(0);
					if(ToBeDeleted == game::GetPlayer()->GetTorsoArmor()) game::GetPlayer()->SetTorsoArmor(0);
					delete ToBeDeleted;
					Cont = true;
					break;
				}
			}
		}
	}
	if(!Success) ADD_MESSAGE("You have no good items for trade.");
}

void mellis::PrayBadEffect()
{
	for(ushort c = 1;; ++c)
		if(game::GetGod(c))
		{
			if(game::GetGod(c) != this)
				game::GetGod(c)->AdjustRelation(-100);
		}
		else
			break;

	ADD_MESSAGE("%s spreads bad rumours about you to other gods.", GOD_NAME);
}

void pestifer::PrayGoodEffect()
{
	ADD_MESSAGE("Suddenly a very ugly head appears beside you, groaning horribly into your ear!");
	game::GetPlayer()->GetLevelSquareUnder()->GetStack()->AddItem(new headofennerbeast);
	game::GetPlayer()->SetEndurance(game::GetPlayer()->GetEndurance() + 1);
}

void pestifer::PrayBadEffect()
{
	game::GetCurrentLevel()->GetLevelSquare(game::GetCurrentLevel()->RandomSquare(true))->AddCharacter(new ennerbeast);
	ADD_MESSAGE("You hear the shouts of a new enner beast!");
}

void valpuri::Pray()
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

void atavus::Pray()
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

void erado::Pray()
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

void infuscor::PrayBadEffect()
{
	ADD_MESSAGE("Vile and evil knowledge pulps into your brain. It's too much for it to handle; you faint.");

	game::GetPlayer()->SetAP(-10000);
	game::GetPlayer()->SetNP(game::GetPlayer()->GetNP() - 200);
}

void macellarius::PrayGoodEffect()
{
	ADD_MESSAGE("%s wishes you to have fun with this potion.", GOD_NAME);
	item* Reward = new potion(false);
	Reward->InitMaterials(2, new glass(50), new omleurine(1500));
	game::GetPlayer()->GetLevelSquareUnder()->GetStack()->AddItem(Reward);
	ADD_MESSAGE("%s drops on the ground.", Reward->CNAME(DEFINITE));
}

void macellarius::PrayBadEffect()
{
	ADD_MESSAGE("A potion drops on your head and shatters into small bits.");
	game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - rand() % 7);
	game::GetPlayer()->GetLevelSquareUnder()->GetStack()->AddItem(new brokenbottle);
	game::GetPlayer()->CheckDeath(std::string("killed while enjoying the company of ") + Name());
}

void scabies::PrayGoodEffect()
{
	ADD_MESSAGE("5 cans full of school food drop to on the ground.");
	for(uchar c = 0; c < 5; ++c)
	{
		item* Reward = new can(false);
		Reward->InitMaterials(2, new iron(50), new schoolfood(600));
		game::GetPlayer()->GetLevelSquareUnder()->GetStack()->AddItem(Reward);
	}
}

void scabies::PrayBadEffect()
{
	ADD_MESSAGE("%s makes you eat a LOT of school food.", GOD_NAME);

	for(uchar c = 0; c < 5; ++c)
	{
		material* SchoolFood = new schoolfood(600);
		SchoolFood->EatEffect(game::GetPlayer());
		delete SchoolFood;
	}

	ADD_MESSAGE("You feel your muscles softening terribly...");

	game::GetPlayer()->SetStrength(game::GetPlayer()->GetStrength() - 1);
	game::GetPlayer()->SetAgility(game::GetPlayer()->GetAgility() - 1);
}

void infuscor::PrayGoodEffect()
{
	ADD_MESSAGE("5 scrolls drop to on the ground.");
	for(uchar c = 0; c < 5; ++c)
	{
		game::GetPlayer()->GetLevelSquareUnder()->GetStack()->AddItem(new scrollofteleport);
	}
}

void cruentus::PrayGoodEffect()
{
	ADD_MESSAGE("Cruentus recommends you to its master, Erado.");
	game::GetGod(16)->AdjustRelation(100);
}

void cruentus::PrayBadEffect()
{
	item* ToBe;

	ToBe = game::GetPlayer()->GetWielded();

	if(ToBe && !ToBe->Destroyable())
		ADD_MESSAGE("%s tries to destroy your weapon, but fails.", GOD_NAME);

	if(ToBe && ToBe->Destroyable())
	{
		ADD_MESSAGE("%s destroys your weapon.", GOD_NAME);

		game::GetPlayer()->GetStack()->RemoveItem(game::GetPlayer()->GetStack()->SearchItem(ToBe));
		game::GetPlayer()->SetWielded(0);
		delete ToBe;
	}
	else
	{
		ADD_MESSAGE("%s gets mad and hits you!", GOD_NAME);
		game::GetPlayer()->SetHP(game::GetPlayer()->GetHP() - rand() % 20);
		game::GetPlayer()->CheckDeath(std::string("destroyed by ") + Name());
	}

}

void cruentus::Pray()
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

void god::PrintRelation() const
{
	std::string VerbalRelation;

	if(GetRelation() == 1000)
		VerbalRelation = "greets you as a Champion of the Cause!";
	else if(GetRelation() > 750)
		VerbalRelation = "is extremely pleased.";
	else if(GetRelation() > 250)
		VerbalRelation = "is very pleased.";
	else if(GetRelation() > 50)
		VerbalRelation = "is pleased.";
	else if(GetRelation() > -50)
		VerbalRelation = "is content.";
	else if(GetRelation() > -250)
		VerbalRelation = "is angry.";
	else if(GetRelation() > -750)
		VerbalRelation = "is very angry.";
	else if(GetRelation() > -1000)
		VerbalRelation = "is extremely angry.";
	else VerbalRelation = "hates you more than any other mortal.";

	ADD_MESSAGE("%s %s", GOD_NAME, VerbalRelation.c_str());
}

