#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "command.h"
#include "char.h"
#include "error.h"
#include "bitmap.h"
#include "graphics.h"
#include "game.h"
#include "god.h"
#include "list.h"
#include "igraph.h"
#include "stack.h"
#include "terrain.h"
#include "femath.h"
#include "hscore.h"
#include "strover.h"
#include "whandler.h"
#include "item.h"
#include "lsquare.h"
#include "level.h"

character::character(bool CreateMaterials, bool SetStats, bool CreateEquipment) : Stack(new stack), Wielded(0), RegenerationCounter(0), NP(1000), AP(0), StrengthExperience(0), EnduranceExperience(0), AgilityExperience(0), PerceptionExperience(0), Relations(0), Dead(false)
{
	SConsumingCurrently(0xFFFF);

	if(CreateMaterials || SetStats || CreateEquipment)
		ABORT("BOOO!");
}

character::~character(void)
{
	delete Stack;
}

void perttu::CreateInitialEquipment(void)
{
	SWielded(CStack()->CItem(CStack()->FastAddItem(new valpurijustifier)));
	item* VPlate = new platemail(false);
	VPlate->InitMaterials(new valpurium(4000));
	WearItem(CStack()->CItem(CStack()->FastAddItem(VPlate)));
}

void oree::CreateInitialEquipment(void)
{
	CStack()->FastAddItem(new maakotkashirt);

	for(ushort c = 0; c < 6; c++)
	{
		item* Can = new can(false);
		Can->InitMaterials(2, new iron(10), new pepsi(330));
		CStack()->FastAddItem(Can);
	}
}

void swatcommando::CreateInitialEquipment(void)
{
	SWielded(CStack()->CItem(CStack()->FastAddItem(new curvedtwohandedsword)));
}

void fallenvalpurist::CreateInitialEquipment(void)
{
	SWielded(CStack()->CItem(CStack()->FastAddItem(new pickaxe)));
}

void froggoblin::CreateInitialEquipment(void)
{
	SWielded(CStack()->CItem(CStack()->FastAddItem(new spear)));
}

void character::ReceiveSound(char* Pointer, short Success, float ScreamStrength)
{
	if(this == game::CPlayer())
		ADD_MESSAGE(Pointer);

	ushort Damage = ushort(ScreamStrength * (1 + float(Success) / 100) / 20000);

	//if(game::CWizardMode())
	//	ADD_MESSAGE("(damage: %d)", Damage);

	SHP(HP - Damage);

	CheckDeath("killed by an Enner Beast's scream");
}

void character::Hunger(ushort Turns) 
{
	ulong BNP = CNP();

	switch(GetBurdenState())
	{
	case UNBURDENED:
		SNP(CNP() - Turns);
		break;
	case BURDENED:
		SNP(CNP() - 2 * Turns);
		SStrengthExperience(CStrengthExperience() + 25 * Turns);
		SAgilityExperience(CAgilityExperience() - 10 * Turns);
		break;
	case STRESSED:
	case OVERLOADED:
		SNP(CNP() - 4 * Turns);
		SStrengthExperience(CStrengthExperience() + 50 * Turns);
		SAgilityExperience(CAgilityExperience() - 25 * Turns);
		break;
	}

	if(CNP() < HUNGERLEVEL)
		SStrengthExperience(CStrengthExperience() - 10 * Turns);

	if(CNP() < HUNGERLEVEL && BNP >= HUNGERLEVEL) if(game::CPlayer() == this) ADD_MESSAGE("You are getting hungry.");
	if(CNP() < CRITICALHUNGERLEVEL && BNP >= CRITICALHUNGERLEVEL) if(game::CPlayer() == this) ADD_MESSAGE("You are getting very hungry.");
	if(CNP() < 1)
	{
		if(!game::CWizardMode())
			AddScoreEntry("starved to death");

		Die();
	}
}

bool character::Hit(character* Enemy)
{
	if(Enemy->CRelations() != 0)
	{
	if(game::CPlayer() == this)
		if(!game::BoolQuestion("This might cause a hostile reaction. Are you sure? [Y/N]"))
			return false;
	}
	
	ushort Speed = CWielded() ? ushort(sqrt((ulong(CAgility() << 2) + CStrength()) * 20000 / Wielded->CWeight())) : ulong(CAgility() << 2) + CStrength();
	short Success = rand() % 26 - rand() % 26;
	float WeaponStrength = CWielded() ? CWielded()->GetWeaponStrength() : GetMeleeStrength();

	switch(Enemy->TakeHit(Speed, Success, WeaponStrength, this)) //there's no breaks and there shouldn't be any
	{
	case HAS_HIT:
	case HAS_BLOCKED:
		if(CWielded())
			CWielded()->ReceiveHitEffect(Enemy, this);
	case HAS_DIED:
		SStrengthExperience(CStrengthExperience() + 50);
	case HAS_DODGED:
		SAgilityExperience(CAgilityExperience() + 25);
	}

	SNP(CNP() - 4);

	return true;
}

ushort character::CalculateArmorModifier(void)
{
	return 100;
}

ushort humanoid::CalculateArmorModifier(void)
{
	return Armor.Torso ? Armor.Torso->GetArmorValue() : 100;
}

uchar character::TakeHit(ushort Speed, short Success, float WeaponStrength, character* Enemy)
{
	if(Enemy == game::CPlayer())
		SRelations(0);
	if(CConsumingCurrently() != 0xFFFF) StopEating();
	if(!(rand() % 20))
	{
		ushort Damage = ushort(WeaponStrength * Enemy->CStrength() * (1 + float(Success) / 100) * CalculateArmorModifier() / 1000000) + (rand() % 5 ? 2 : 1);

		if(CLevelSquareUnder()->CanBeSeen())
		{
			Enemy->AddHitMessage(this,true);

			if(game::CWizardMode())
				ADD_MESSAGE("(damage: %d)", Damage);
		}

		SHP(CHP() - Damage);
		//if(Enemy->CWielded())
		//	Enemy->CWielded()->ReactWith(this);
		if(rand() % 4) SpillBlood(rand() % 3);

		if(CheckDeath(std::string("killed by ") + Enemy->Name(INDEFINITE))) { return HAS_DIED; }

		return HAS_HIT;
	}

	if((Success + Speed + CSize() - CAgility()) > 0 && rand() % (Success + Speed + CSize() - CAgility()) > 50)
	{
		ushort Damage = ushort(WeaponStrength * Enemy->CStrength() * (1 + float(Success) / 100) * CalculateArmorModifier() / 2000000) + (rand() % 5 ? 1 : 0);

		if(!Damage)
		{
			if(CLevelSquareUnder()->CanBeSeen()) Enemy->AddBlockMessage(this);

			SStrengthExperience(CStrengthExperience() + 25);
			SEnduranceExperience(CEnduranceExperience() + 25);

			return HAS_BLOCKED;
		}
		else
		{
			if(CLevelSquareUnder()->CanBeSeen()) Enemy->AddHitMessage(this);
			//if(Enemy->CWielded())
			//	Enemy->CWielded()->ReactWith(this);
			SHP(CHP() - Damage);

			if(game::CWizardMode())
				ADD_MESSAGE("(damage: %d)", Damage);

			if(CheckDeath(std::string("killed by ") + Enemy->Name(INDEFINITE))) { return HAS_DIED; }

			return HAS_HIT;
		}
	}
	else
	{
		if(CLevelSquareUnder()->CanBeSeen()) Enemy->AddDodgeMessage(this);

		SAgilityExperience(CAgilityExperience() + 100);

		return HAS_DODGED;
	}
}

bool ennerbeast::Hit(character*)
{
	char Message[256] ;
	if(rand() % 2) sprintf(Message,"The Enner Beast yells: UGH UGHAaaa!");
	else sprintf(Message, "The Enner Beast yells: Uga Ugar Ugade Ugat!");

	DO_FILLED_RECTANGLE(CPos().X, CPos().Y, 0, 0, game::CCurrentLevel()->CXSize() - 1, game::CCurrentLevel()->CYSize() - 1, 100,
	                    {
					character* Char = game::CCurrentLevel()->CLevelSquare(vector(XPointer, YPointer))->CCharacter();

					if(Char && Char != this)
						Char->ReceiveSound(Message, rand() % 26 - rand() % 26, GetMeleeStrength() * CStrength() / GetHypotSquare<float>(float(CPos().X) - XPointer, float(CPos().Y) - YPointer));
	                    });

	SStrengthExperience(CStrengthExperience() + 100);

	return true;
}

void character::Act(void)
{
	if(!CHasActed())
	{
		if(!game::Flag)
		{
			ApplyExperience();
			if(CHP() < CEndurance()) SpillBlood(rand() % 2);
			switch(GetBurdenState())
			{
			case UNBURDENED:
				SAP(CAP() + 100 + (CAgility() - 10) / 2);
			break;
			case BURDENED:
				SAP(CAP() + 50 + (CAgility() - 10) / 4);
			break;
			case STRESSED:
			case OVERLOADED:
				SAP(CAP() + 33 + (CAgility() - 10) / 6);
			break;
			}
			if(CConsumingCurrently() != 0xFFFF) ContinueEating();

			if(CNP() < CRITICALHUNGERLEVEL && !(rand() % 50) && !CFainted() && this == game::CPlayer())
			{
				ADD_MESSAGE("You faint.");
				SAP(CAP() - 7000 + rand() % 10000);
				SStrengthExperience(CStrengthExperience() - 100);
				SFainted(true);
			}
		}
		else
			game::Flag = false;

		if(CAP() >= 1000)
		{
			SConsumingCurrently(0xFFFF);
			if(CFainted()) SFainted(false);

			if(this == game::CPlayer())
			{
				static ushort Timer = 0;
				if(Timer++ == 20)
				{
					game::Save(game::CAutoSaveFileName().c_str());
					Timer = 0;
				}
				GetPlayerCommand();
				Hunger();
				Regenerate();
				game::Turn();
				game::ApplyDivineTick();
			}
			else
			{
				GetAICommand(); //This should handle AI suicide also...
				Regenerate();
			}

			SAP(CAP() - 1000);
		}
	}

	SHasActed(true);
}


bool character::GoUp(void)
{
	if(CSquareUnder()->COverTerrain()->GoUp(this))
	{
		SStrengthExperience(CStrengthExperience() + 25);
		SNP(CNP() - 2);
		return true;
	}
	else
		return false;
}

bool character::GoDown(void)
{
	if(CSquareUnder()->COverTerrain()->GoDown(this))
	{
		SAgilityExperience(CAgilityExperience() + 25);
		SNP(CNP() - 1);
		return true;
	}
	else
		return false;
}

bool character::Open(void)
{
	ADD_MESSAGE("Where is this famous door you wish to open? Press i for inventory.");

	DRAW_MESSAGES;

	EMPTY_MESSAGES;

	graphics::BlitDBToScreen();

	while(true)
	{
		int Key = game::GetKey();

		if(Key == 'i') 
			return OpenItem();

		if(Key == 0x1B)
			return false;

		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::CMoveCommandKey()[c])
				return OpenPos(CPos() + game::CMoveVector()[c]);
	}
}

bool character::Close(void)
{
	ADD_MESSAGE("Where is this door you wish to close?");

	DRAW_MESSAGES;

	EMPTY_MESSAGES;

	graphics::BlitDBToScreen();
	//GGG DOUBLEBUFFER->Blit(SCREEN, 0, 0, 0, 0, 800, 16);

	while(true)
	{
		int Key = game::GetKey();

		if(Key == 0x1B)
			return false;

		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::CMoveCommandKey()[c])
				if(game::CCurrentLevel()->CLevelSquare(CPos() + game::CMoveVector()[c])->Close(this))
				{
					SAgilityExperience(CAgilityExperience() + 25);
					SNP(CNP() - 1);
					return true;
				}
				else
					return false;
	}
}

bool character::Drop(void)
{
	ushort Index = CStack()->DrawContents("What do you want to drop?");

	if(Index < CStack()->CItems() && CStack()->CItem()[Index])
		if(CStack()->CItem()[Index] == CWielded())
			ADD_MESSAGE("You can't drop something you wield!");
		else
		{
			CStack()->MoveItem(Index, CLevelSquareUnder()->CStack());

			return true;
		}

        return false;
}

bool humanoid::Drop(void)
{
	ushort Index = CStack()->DrawContents("What do you want to drop?");

	if(Index < CStack()->CItems() && CStack()->CItem()[Index])
		if(CStack()->CItem()[Index] == CWielded())
			ADD_MESSAGE("You can't drop something you wield!");
		else if(CStack()->CItem()[Index] == Armor.Torso)
			ADD_MESSAGE("You can't drop something you wear!");
		else
		{
			CStack()->MoveItem(Index, CLevelSquareUnder()->CStack());

			return true;
		}

	return false;
}

bool character::Consume(void)
{
	if(CLevelSquareUnder()->CStack()->ConsumableItems(this) && game::BoolQuestion("Do you wish to consume one of the items lying on the ground? [Y/N]"))
	{
		ushort Index = CLevelSquareUnder()->CStack()->DrawConsumableContents("What do you wish to consume?", this);
		if(Index < CLevelSquareUnder()->CStack()->CItems())
		{
			if(CheckBulimia())
				if(!game::BoolQuestion("You think your stomach will burst if you eat anything more. Force it down? (Y/N)"))
					return false;

			if(ConsumeItem(Index, CLevelSquareUnder()->CStack()))
			{
				ReceiveBulimiaDamage();
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	if(CStack()->ConsumableItems(this))
	{
		ushort Index = CStack()->DrawConsumableContents("What do you wish to consume?", this);
		if(Index < CStack()->CItems())
		{
			if(!CheckIfConsumable(Index))
				return false;

			if(CheckBulimia())
				if(!game::BoolQuestion("You think your stomach will burst if you eat anything more. Force it down? (Y/N)"))
					return false;


			if(ConsumeItem(CStack()->MoveItem(Index, CLevelSquareUnder()->CStack()), CLevelSquareUnder()->CStack()))
			{
				ReceiveBulimiaDamage();
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	ADD_MESSAGE("You have nothing to eat.");
	return false;
}

bool character::CheckBulimia(void)
{
	return CNP() > (CSize() << 5) ? true : false;
}

void character::ReceiveBulimiaDamage(void)
{
	if((CNP() - (CSize() << 5)) / 50 > 0)
	{
		ADD_MESSAGE("Urgh... Your stomach hurts.");

		SHP(CHP() - (CNP() - (CSize() << 5)) / 50);

		CheckDeath("died of bulimia");
	}
}

bool character::CheckIfConsumable(ushort Index)
{
	return (CTorsoArmor() != CStack()->CItem()[Index] && CWielded() != CStack()->CItem()[Index]);
}

bool character::ConsumeItem(int ToBeEaten, stack* ItemsStack)
{
	ushort ItemNumber;
	if(ConsumeItemType(ItemsStack->CItem()[ToBeEaten]->GetConsumeType()))
	{
		if((ItemNumber = ItemsStack->CItem(ToBeEaten)->PrepareForConsuming(this, ItemsStack)) != 0xFFFF)
		{
			SConsumingCurrently(ItemsStack->MoveItem(ItemNumber, CLevelSquareUnder()->CStack()));
			APsToBeEaten = ItemsStack->CItem()[ToBeEaten]->CWeight();
			SAP(1000 - APsToBeEaten);
			return true;
		}
	}
	if(this == game::CPlayer()) ADD_MESSAGE("You can't consume this.");

	return false;

}

bool character::ConsumeItemType(uchar Type)
{
	switch(Type)
	{
	case LIQUID:
		return true;
	break;
		
	case ODD:
		return false;
	break;
	case FRUIT:
		return true;
	break;
	case MEAT:
		return true;
	break;
	case SPOILED:
		return true;
	break;
	case HARD:
		return false;
	break;
	case SCHOOLFOOD:
		return true;
	break;
	case CONTAINER:
		return false;
	break;
	case BONE:
		return false;
	break;

	default:
		ADD_MESSAGE("ERRRRORRRRRR in Consumeitemtype."); //All hail SpykoS! He is the author of this file, and his might is over that of PMGR!!!
	}
		
	return false;
}

void character::Move(vector MoveTo, bool TeleportMove)
{
	if(GetBurdenState() || TeleportMove)
	{
	game::CCurrentLevel()->RemoveCharacter(CPos());

	game::CCurrentLevel()->AddCharacter(MoveTo, this);

	if(this == game::CPlayer())
	{
		if(CPos().X < game::CCamera().X + 2 || CPos().X > game::CCamera().X + 48)
			game::UpDateCameraX();

		if(CPos().Y < game::CCamera().Y + 2 || CPos().Y > game::CCamera().Y + 27)
			game::UpDateCameraY();

		game::CCurrentLevel()->UpdateLOS();
		if(game::CCurrentLevel()->CLevelSquare(CPos())->CEngraved() != "") ADD_MESSAGE("Something has been engraved here: \"%s\"", game::CCurrentLevel()->CLevelSquare(CPos())->CEngraved().c_str());
	}
	SNP(CNP() - 1);
	SAgilityExperience(CAgilityExperience() + 10);
	if (this == game::CPlayer() && CLevelSquareUnder()->CStack()->CItems() > 0)
	{
		if (CLevelSquareUnder()->CStack()->CItems() > 1)
		ADD_MESSAGE("Several items are lying here.");
		else	ADD_MESSAGE("%s is lying here.", CLevelSquareUnder()->CStack()->CItem()[0]->CNAME(INDEFINITE));
	}
	}
}

void character::DrawToTileBuffer(void)
{
	igraph::CCharacterGraphic()->MaskedBlit(igraph::CTileBuffer(), CBitmapPos().X, (Material[0]->CFleshColor()) << 4, 0, 0, 16, 16);

	if(this == game::CPlayer())
		igraph::CCharacterGraphic()->MaskedBlit(igraph::CTileBuffer(), 0, 0, 0, 0, 16, 16);
}

void golem::DrawToTileBuffer(void)
{
	igraph::CCharacterGraphic()->MaskedBlit(igraph::CTileBuffer(), CBitmapPos().X, (Material[0]->CItemColor()) << 4, 0, 0, 16, 16);

	if(this == game::CPlayer())
		igraph::CCharacterGraphic()->MaskedBlit(igraph::CTileBuffer(), 0, 0, 0, 0, 16, 16);
}

void humanoid::DrawToTileBuffer(void)
{	
	vector InHandsPic, ArmPos, HeadPos;

	if(CArmType() > 16)
	{
		ArmPos.X = 80;
		ArmPos.Y = (CArmType() - 16) * 16;
	}
	else
	{
		ArmPos.X = 64;
		ArmPos.Y = CArmType() * 16;
	}
	if(CHeadType() > 16)
	{
		HeadPos.X = 112;
		HeadPos.Y = (CHeadType() - 16) * 16;
	}
	else
	{
		HeadPos.X = 96;
		HeadPos.Y = CHeadType() * 16;
	}


	if(CWielded() != 0) InHandsPic = CWielded()->GetInHandsPic();

	igraph::CHumanGraphic()->MaskedBlit(igraph::CTileBuffer(), 0, 0, 0, 0, 16, 16); // Legs
	igraph::CHumanGraphic()->MaskedBlit(igraph::CTileBuffer(), 32, 0, 0, 0, 16, 16); // Torso
	igraph::CHumanGraphic()->MaskedBlit(igraph::CTileBuffer(), ArmPos.X, ArmPos.Y, 0, 0, 16, 16); // Arms
	igraph::CHumanGraphic()->MaskedBlit(igraph::CTileBuffer(), HeadPos.X, HeadPos.Y, 0, 0, 16, 16); // Head

	if(CWielded() != 0)
		if(InHandsPic.X != 0 || InHandsPic.Y != 0) igraph::CHumanGraphic()->MaskedBlit(igraph::CTileBuffer(), InHandsPic.X , InHandsPic.Y, 0, 0, 16, 16); // Wielded

	if (this == game::CPlayer())
		igraph::CCharacterGraphic()->MaskedBlit(igraph::CTileBuffer(), 0, 0, 0, 0, 16, 16);
}

bool character::Wield(void)
{
	ushort Index;
	if((Index = CStack()->DrawContents("What do you want to wield? or press '-' for hands")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to wield.");
		return false;
	}
	if(Index == 0xFFFE)
		SWielded(0);
	else
		if(Index < CStack()->CItems())
			SWielded(CStack()->CItem()[Index]);
		else
			return false;

	return true;
}

bool humanoid::Wield(void)
{
	ushort Index;
	if((Index = CStack()->DrawContents("What do you want to wield? or press '-' for hands")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to wield.");
		return false;
	}

	if(Index == 0xFFFE)
		SWielded(0);
	else
		if(Index < CStack()->CItems())
	{
	if(CStack()->CItem()[Index] != Armor.Torso)
		SWielded(CStack()->CItem()[Index]);
	else ADD_MESSAGE("You can't wield something that you wear!");
	}

		else
			return false;

	return true;
	
}

void character::GetAICommand(void) // Freedom is slavery. Love is hate. War is peace.
{
	switch(CRelations())
	{
	case HOSTILE:
		HostileAICommand();
		break;
	case NEUTRAL:
		NeutralAICommand();
		break;
	case FRIEND:
		FriendAICommand();
		break;
	default:
		ABORT("Illegal relationship(s) encountered");
	}

}

void character::Charge(character* Target)
{
	vector TPos = Target->CPos();

	vector MoveTo[3];

	if(TPos.X < CPos().X)
	{
		if(TPos.Y < CPos().Y)
		{
			MoveTo[0] = vector(-1, -1);
			MoveTo[1] = vector(-1,  0);
			MoveTo[2] = vector( 0, -1);
		}

		if(TPos.Y == CPos().Y)
		{
			MoveTo[0] = vector(-1,  0);
			MoveTo[1] = vector(-1, -1);
			MoveTo[2] = vector(-1,  1);
		}

		if(TPos.Y > CPos().Y)
		{
			MoveTo[0] = vector(-1, 1);
			MoveTo[1] = vector(-1, 0);
			MoveTo[2] = vector( 0, 1);
		}
	}

	if(TPos.X == CPos().X)
	{
		if(TPos.Y < CPos().Y)
		{
			MoveTo[0] = vector( 0, -1);
			MoveTo[1] = vector(-1, -1);
			MoveTo[2] = vector( 1, -1);
		}

		if(TPos.Y == CPos().Y)	//???
			return;

		if(TPos.Y > CPos().Y)
		{
			MoveTo[0] = vector( 0, 1);
			MoveTo[1] = vector(-1, 1);
			MoveTo[2] = vector( 1, 1);
		}
	}

	if(TPos.X > CPos().X)
	{
		if(TPos.Y < CPos().Y)
		{
			MoveTo[0] = vector(1, -1);
			MoveTo[1] = vector(1,  0);
			MoveTo[2] = vector(0, -1);
		}

		if(TPos.Y == CPos().Y)
		{
			MoveTo[0] = vector(1,  0);
			MoveTo[1] = vector(1, -1);
			MoveTo[2] = vector(1,  1);
		}

		if(TPos.Y > CPos().Y)
		{
			MoveTo[0] = vector(1, 1);
			MoveTo[1] = vector(1, 0);
			MoveTo[2] = vector(0, 1);
		}
	}

	if(TryMove(CPos() + MoveTo[0])) return;

	if(rand() % 2)
	{
		if(TryMove(CPos() + MoveTo[1])) return;
		if(TryMove(CPos() + MoveTo[2])) return;
	}
	else
	{
		if(TryMove(CPos() + MoveTo[2])) return;
		if(TryMove(CPos() + MoveTo[1])) return;
	}
}

bool character::TryMove(vector MoveTo)
{
	if(MoveTo.X < game::CCurrentLevel()->CXSize() && MoveTo.Y < game::CCurrentLevel()->CYSize())
	{
		if(game::CCurrentLevel()->CLevelSquare(MoveTo)->CCharacter())
			if(this == game::CPlayer() || (!CRelations() && game::CCurrentLevel()->CLevelSquare(MoveTo)->CCharacter()->CRelations() > 0) || (CRelations() > 0 && !game::CCurrentLevel()->CLevelSquare(MoveTo)->CCharacter()->CRelations()))
				return Hit(game::CCurrentLevel()->CLevelSquare(MoveTo)->CCharacter());
			else
				return false;
		else
			if(game::CCurrentLevel()->CLevelSquare(MoveTo)->COverTerrain()->CIsWalkable() || (game::CGoThroughWallsCheat() && game::CPlayer() == this))
			{
				Move(MoveTo);

				return true;
			}
			else if(game::CPlayer() == this && game::CCurrentLevel()->CLevelSquare(MoveTo)->COverTerrain()->CanBeOpened())
			{
				if(game::BoolQuestion("Do you want to open this door? [Y/N]", false, game::GetMoveCommandKey(game::CPlayer()->CPos(), MoveTo)))
				{
					OpenPos(MoveTo);
					return true;
				}
				else
					return false;
			}
			else
				return false;
	}
	else
		return false;
}

bool character::ShowInventory(void)
{
	CStack()->DrawContents("Character's Inventory, press ESC to exit");

	return false;
}

bool character::PickUp(void)
{
	bool ToBeReturned = false;

	if (CLevelSquareUnder()->CStack()->CItems() > 0)
	{
		if (CLevelSquareUnder()->CStack()->CItems() > 1)
		{
			ushort Index;

			bitmap Backup(800, 600);

			DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, 800, 600);

			for(;;)
			{
				Index = CLevelSquareUnder()->CStack()->DrawContents("What do you want to pick up?");

				if(Index < CLevelSquareUnder()->CStack()->CItems())
					if(CLevelSquareUnder()->CStack()->CItem()[Index])
					{
						ADD_MESSAGE("%s picked up.", CLevelSquareUnder()->CStack()->CItem()[Index]->CNAME(INDEFINITE));
						CLevelSquareUnder()->CStack()->MoveItem(Index, CStack());
						ToBeReturned = true;
					}

				if(Index == 0xFFFD || !CLevelSquareUnder()->CStack()->CItems())
					break;

				Backup.Blit(DOUBLEBUFFER, 0, 0, 0, 0, 800, 600);
			}
		}
		else
		{
			ADD_MESSAGE("%s picked up.", CLevelSquareUnder()->CStack()->CItem()[0]->CNAME(INDEFINITE));
			CLevelSquareUnder()->CStack()->MoveItem(0, CStack());
			return true;
		}
	}
	else
		ADD_MESSAGE("There is nothing here to pick up, %s!", game::Insult());

	return ToBeReturned;
}

bool character::Quit(void)
{
	if(game::BoolQuestion("Thine Holy Quest is not yet compeleted! Really quit? [Y/N]"))
	{
		game::Quit();

		game::RemoveSaves();

		if(!game::CWizardMode())
		{
			AddScoreEntry("cowardly quit the game", 0.75f);
			highscore HScore;
			HScore.Draw();
		}

		return true;
	}
	else
		return false;
}

void character::Die(void)
{
	if(Dead)
		return;

	if(game::CPlayer() == this)
	{
		ADD_MESSAGE("You die.");

		if(game::CWizardMode())
		{
			game::DrawEverything(false);

			if(!game::BoolQuestion("Do you want to do this, cheater? [Y/N]", 2, 'y', game::MonsGetKey))
			{
				SHP(Endurance << 1);
				SNP(1000);
				return;
			}
		}
	}
	else
		if(CLevelSquareUnder()->CanBeSeen()) ADD_MESSAGE("%s dies screaming.", CNAME(DEFINITE));

	Dead = true;

	while(CStack()->CItems())
		CStack()->MoveItem(0, CLevelSquareUnder()->CStack());
	
	CLevelSquareUnder()->RemoveCharacter();

	item* Corpse = new corpse(false);

	Corpse->InitMaterials(CMaterial(0));

	CLevelSquareUnder()->CStack()->AddItem(Corpse);

	SMaterial(0, 0);

	game::SendToHell(this);

	if(game::CPlayer() == this)
	{
		game::Quit();

		if(!game::CWizardMode())
		{
			game::DrawEverything();

			globalwindowhandler::GetKey();
		}

		game::StoryScreen("Unfortunately thee died during thine journey. The Überpriest is not happy.");

		game::RemoveSaves();

		highscore HScore;
		HScore.Draw();
	}
}

bool character::OpenItem(void)
{
	ushort Index = Stack->DrawContents("What do you want to open?");

	if(Index < CStack()->CItems())
		if(CStack()->CItem()[Index]->TryToOpen(Stack) != 0xFFFF)
		{
			SAgilityExperience(CAgilityExperience() + 25);
			SNP(CNP() - 1);
			return true;
		}
		else
		{
			ADD_MESSAGE("You can't open %s.", CStack()->CItem()[Index]->CNAME(DEFINITE));
			return false;
		}

	return false;
}

void character::Regenerate(ushort Turns)
{
	SRegenerationCounter(CRegenerationCounter() + CEndurance() * Turns);

	while(CRegenerationCounter() > 100)
	{
		if(CHP() < (CEndurance() << 1))
		{
			SHP(CHP() + 1);
			SEnduranceExperience(CEnduranceExperience() + 100);
		}

		SRegenerationCounter(CRegenerationCounter() - 100);
	}
}

void fallenvalpurist::Die(void)
{
	if(Dead)
		return;

	if(game::CPlayer() == this) 
	{
		ADD_MESSAGE("You die.");

		game::DrawEverything();

		globalwindowhandler::GetKey();

		game::StoryScreen("Unfortunately thee died during thine journey. The Überpriest is not happy.");

		game::RemoveSaves();

		game::Quit();
	}
	else
		if(CLevelSquareUnder()->CanBeSeen()) ADD_MESSAGE("%s is transformed into a crumpled heap of bones.", CNAME(DEFINITE));

	Dead = true;

	while(CStack()->CItems())
		CStack()->MoveItem(0, CLevelSquareUnder()->CStack());

	ushort Amount = 2 + rand() % 4;

	for(ushort c = 0; c < Amount; c++)
		CLevelSquareUnder()->CStack()->AddItem(new abone);

	CLevelSquareUnder()->RemoveCharacter();

	game::SendToHell(this);
}

void elpuri::Die(void)
{
	if(Dead)
		return;

	if(game::CPlayer() == this) 
	{
		ADD_MESSAGE("You die and your head drops off.");

		game::DrawEverything();

		globalwindowhandler::GetKey();

		game::StoryScreen("Unfortunately thee died during thine journey. The Überpriest is not happy.");

		game::RemoveSaves();

		game::Quit();
	}
	else
		if(CLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s groans horribly and drops %s head.", CNAME(DEFINITE), game::PossessivePronoun(GetSex()));

	Dead = true;

	while(CStack()->CItems())
		CStack()->MoveItem(0, CLevelSquareUnder()->CStack());

	item* Corpse = new corpse(false);

	Corpse->InitMaterials(CMaterial(0));

	CLevelSquareUnder()->CStack()->AddItem(Corpse);

	SMaterial(0,0);

	CLevelSquareUnder()->CStack()->AddItem(new headofelpuri);
	CLevelSquareUnder()->RemoveCharacter();

	game::SendToHell(this);
}

void perttu::Die(void)
{
	if(Dead)
		return;

	if(game::CPlayer() == this) 
	{
		ADD_MESSAGE("You die and your left nut falls off.");

		game::DrawEverything();

		globalwindowhandler::GetKey();

		game::StoryScreen("Unfortunately thee died during thine journey. Thee are not happy.");

		game::RemoveSaves();

		game::Quit();
	}
	else
		if(CLevelSquareUnder()->CanBeSeen()) ADD_MESSAGE("%s disappears in a bright light and his left nut is left behind.", CNAME(DEFINITE));

	Dead = true;

	while(CStack()->CItems())
		CStack()->MoveItem(0, CLevelSquareUnder()->CStack());

	item* Corpse = new corpse(false);

	Corpse->InitMaterials(CMaterial(0));

	CLevelSquareUnder()->CStack()->AddItem(Corpse);

	SMaterial(0, 0);

	CLevelSquareUnder()->CStack()->AddItem(new leftnutofperttu);

	CLevelSquareUnder()->RemoveCharacter();

	game::SendToHell(this);
}

void billswill::Die(void)
{
	if(Dead)
		return;

	if(game::CPlayer() == this) 
	{
		ADD_MESSAGE("You die and disappear in a whisp of smoke."); // Shouldn't this smoke be poison?

		game::DrawEverything();

		globalwindowhandler::GetKey();

		game::StoryScreen("Unfortunately thee died during thine journey. The Überpriest is not happy.");

		game::RemoveSaves();

		game::Quit();
	}
	else
		if(CLevelSquareUnder()->CanBeSeen()) ADD_MESSAGE("%s vanishes from existence.", CNAME(DEFINITE));

	Dead = true;

	while(CStack()->CItems())
		CStack()->MoveItem(0, CLevelSquareUnder()->CStack());

	CLevelSquareUnder()->RemoveCharacter();

	game::SendToHell(this);
}

bool humanoid::WearArmor(void)
{
	ushort Index;
	if((Index = CStack()->DrawContents("What do you want to wear? or press '-' for nothing")) == 0xFFFF)
	{
		//ADD_MESSAGE("You have nothing to wear.");
		return false;
	}


	if(Index == 0xFFFE)
	{
		Armor.Torso = 0;
		return true;
	}
	else
		if(Index < CStack()->CItems())		// Other Armor types should be coded...
		{
			if(CStack()->CItem()[Index]->CanBeWorn())
			{
				if(CStack()->CItem()[Index] != CWielded())
				{
					Armor.Torso = CStack()->CItem()[Index];
					return true;
				}
				else
					ADD_MESSAGE("You can't wear something that you wield!");
			}
			else
				ADD_MESSAGE("You can't wear THAT!");
		}

	return false;
}

bool character::WearArmor(void)
{
	ADD_MESSAGE("This monster type doesn't have armor.");

	return false;
}

void character::AddBlockMessage(character* Enemy)
{
	if(Enemy == game::CPlayer())
		ADD_MESSAGE("You block %s!", CNAME(DEFINITE));
	else
		if(this == game::CPlayer())
			ADD_MESSAGE("%s blocks you!", Enemy->CNAME(DEFINITE));
		else
			if(CLevelSquareUnder()->RetrieveFlag()) ADD_MESSAGE("%s blocks %s!", Enemy->CNAME(DEFINITE), CNAME(DEFINITE));
}

void character::AddDodgeMessage(character* Enemy)
{
	if(Enemy == game::CPlayer())
		ADD_MESSAGE("You dodge %s!", CNAME(DEFINITE));
	else
		if(this == game::CPlayer())
			ADD_MESSAGE("%s dodges you!", Enemy->CNAME(DEFINITE));
		else
			if(CLevelSquareUnder()->CanBeSeen()) ADD_MESSAGE("%s dodges %s!", Enemy->CNAME(DEFINITE), CNAME(DEFINITE));
}

void character::AddHitMessage(character* Enemy, const bool Critical)
{
	if(Enemy == game::CPlayer())
		if(CWielded())
			ADD_MESSAGE("%s %s you with %s %s!", CNAME(DEFINITE), ThirdPersonWeaponHitVerb(Critical).c_str(), game::PossessivePronoun(GetSex()), Wielded->CNAME(0));
		else
			ADD_MESSAGE("%s %s you!", CNAME(DEFINITE), ThirdPersonMeleeHitVerb(Critical).c_str());
	else
		if(this == game::CPlayer())
			ADD_MESSAGE("You %s %s!", FirstPersonHitVerb(Enemy, Critical).c_str(), Enemy->CNAME(DEFINITE));
		else
			ADD_MESSAGE("%s %s %s!", CNAME(DEFINITE), AICombatHitVerb(Enemy, Critical).c_str(), Enemy->CNAME(DEFINITE));
}

void character::BeTalkedTo(character*)
{
	ADD_MESSAGE("The monster grunts.");
}

void perttu::BeTalkedTo(character* Talker)
{
	static bool Triggered = false;

	if(Talker->HasMaakotkaShirt())
	{
		game::StoryScreen("Thou hast slain the Pepsi Daemon King, and Perttu is happy!\n\nYou are victorious!");
		game::RemoveSaves();
		game::Quit();

		if(!game::CWizardMode())
		{
			AddScoreEntry("retrieved the Holy Maakotka Shirt and was titled as the Avatar of Law", 3);
			highscore HScore;
			HScore.Draw();
		}
	}
	else
		if(Triggered)
			ADD_MESSAGE("Perttu says: \"Bring me the Maakotka shirt and we'll talk.\"");

	if(Talker->HasHeadOfElpuri() && !Triggered)
	{
		if(game::CGod(1)->CRelation() >= 500 && Talker->CDifficulty() >= 2500 && game::BoolQuestion("Perttu smiles. \"Thou areth indeed a great Champion of the Great Frog! Elpuri is not a foe worthy for thee. Dost thou wish to stay in the dungeon for a while more and complete another quest for me?\" (Y/n)", 'y'))
		{
			game::StoryScreen("Champion of Law!\n\nSeek out the Master Evil: Oree the Pepsi Daemon King,\nwho hast stolenth one of the most powerful of all of my artifacts:\nthe Holy Maakotka Shirt! Return with it and immortal glory shall be thine!");

			game::TriggerQuestForMaakotkaShirt();

			Triggered = true;
		}
		else
		{
		game::StoryScreen("Thou hast slain Elpuri, and Perttu is happy!\n\nYou are victorious!");
		game::RemoveSaves();
		game::Quit();

		if(!game::CWizardMode())
		{
			AddScoreEntry("defeated Elpuri and continued to further adventures", 2);
			highscore HScore;
			HScore.Draw();
		}
		}
	}
	else
		if(!Triggered)
			ADD_MESSAGE("Perttu says: \"Bring me the head of Elpuri and we'll talk.\"");
}

bool character::Talk(void)
{
                int k;
		ADD_MESSAGE("To whom do you wish to talk to?");

		DRAW_MESSAGES;

		EMPTY_MESSAGES;

		graphics::BlitDBToScreen();

		bool CorrectKey = false;

		while(!CorrectKey)
		{
			k = game::GetKey();

			if(k == 0x1B)
				CorrectKey = true;

			for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
				if(k == game::CMoveCommandKey()[c] || (k ^ 32) == game::CMoveCommandKey()[c])
				{
					if(game::CCurrentLevel()->CLevelSquare(CPos() + game::CMoveVector()[c])->CCharacter())
					{
						game::CCurrentLevel()->CLevelSquare(CPos() + game::CMoveVector()[c])->CCharacter()->BeTalkedTo(this);

						return true;
					}
					else
					{
						ADD_MESSAGE("There is no one in that square.");

						return false;
					}

					CorrectKey = true;
				}
		}

                return false;

}

bool character::NOP(void)
{
	SAgilityExperience(CAgilityExperience() - 5);

	return true;
}

void character::ApplyExperience(void)
{
	if(CStrengthExperience() > pow(1.18, long(CStrength())) * 193)
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("You feel you could lift Bill with one hand!");

		SStrength(CStrength() + 1);

		SStrengthExperience(0);
	}

	if(CStrengthExperience() < -pow(1.18, long(100 - CStrength())) * 193)
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("You collapse under your load.");

		SStrength(CStrength() - 1);

		SStrengthExperience(0);
	}

	if(CEnduranceExperience() > pow(1.18, long(CEndurance())) * 193)
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("You feel Valpuri's toughness around you!");

		SEndurance(CEndurance() + 1);

		SEnduranceExperience(0);
	}

	if(CEnduranceExperience() < -pow(1.18, long(100 - CEndurance())) * 193)
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("You seem as tough as Jari.");

		SEndurance(CEndurance() - 1);

		SEnduranceExperience(0);
	}

	if(CAgilityExperience() > pow(1.18, long(CAgility())) * 193)
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("Your agility challenges even the Valpuri's angels!");

		SAgility(CAgility() + 1);

		SAgilityExperience(0);
	}

	if(CAgilityExperience() < -pow(1.18, long(100 - CAgility())) * 193)
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("You seem as fast as a flat mommo.");

		SAgility(CAgility() - 1);

		SAgilityExperience(0);
	}

	if(CPerceptionExperience() > pow(1.18, long(CPerception())) * 193)
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("Your sight seem to sharpen. This is bad. Very bad.");

		SPerception(CPerception() + 1);

		SPerceptionExperience(0);
	}

	if(CPerceptionExperience() < -pow(1.18, long(100 - CPerception())) * 193)
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("You feel very guru.");

		SPerception(CPerception() - 1);

		SPerceptionExperience(0);

		game::CGod(1)->AdjustRelation(100);
	}
}

bool character::HasHeadOfElpuri(void)
{
	for(ushort c = 0; c < CStack()->CItems(); c++)
		if(CStack()->CItem(c)->IsHeadOfElpuri())
			return true;

	return false;
}

bool character::HasPerttusNut(void)
{
	for(ushort c = 0; c < CStack()->CItems(); c++)
		if(CStack()->CItem(c)->IsPerttusNut())
			return true;

	return false;
}

bool character::HasMaakotkaShirt(void)
{
	for(ushort c = 0; c < CStack()->CItems(); c++)
		if(CStack()->CItem(c)->IsMaakotkaShirt())
			return true;

	return false;
}

bool character::Save(void)
{
	if(game::BoolQuestion("Dost thee truly wish to save and flee? [Y/N]"))
	{
		game::Save();

		game::Quit();

		return true;
	}
        else
		return false;
}

bool character::Read(void)
{
	ushort Index = CStack()->DrawContents("What do you want to read?");

	if(Index < CStack()->CItems())
		return ReadItem(Index, CStack());
	else
		return false;
}

bool character::ReadItem(int ToBeRead, stack* ItemsStack)
{
	if(ItemsStack->CItem()[ToBeRead]->CanBeRead(this))
	{
		if(ItemsStack->CItem()[ToBeRead]->Read(this))
			ItemsStack->RemoveItem(ToBeRead);

		return true;
	}
	else
	{
		if(this == game::CPlayer()) ADD_MESSAGE("You can't read this.");

		return false;
	}
}

// 2500 * S Burdened
// 5000 * S Stressed
// 7000 * S Overload

uchar character::GetBurdenState(ulong Mass)
{
	ulong SumOfMasses;
	if(!Mass)
		SumOfMasses = CStack()->SumOfMasses();
	else
		SumOfMasses = Mass;
	if(SumOfMasses > ulong(7000 * CStrength()))
		return OVERLOADED;
	if(SumOfMasses > ulong(5000 * CStrength()))
		return STRESSED;
	if(SumOfMasses > ulong(2500 * CStrength()))
		return BURDENED;
	return UNBURDENED;
}

bool character::Dip(void)
{
	ushort What = CStack()->DrawContents("What do you want to dip?");

	if(What < CStack()->CItems() && CStack()->CItem()[What]->CanBeDipped())
	{
		game::DrawEverything();
		ushort To = CStack()->DrawContents("In what do you wish to dip it into?");
		if(To < CStack()->CItems() && CStack()->CItem()[To] && CStack()->CItem()[What] != CStack()->CItem()[To])
		{
			if(CStack()->CItem()[To]->CanBeDippedInto(CStack()->CItem()[What]))
			{
				CStack()->CItem()[What]->DipInto(CStack()->CItem()[To]);
				return true;
			}
		}
	}
	ADD_MESSAGE("Invalid selection!");
        return false;
}

void character::Save(std::ofstream* SaveFile)
{
	object::Save(SaveFile);

	Stack->Save(SaveFile);

	ushort Index = Wielded ? Stack->SearchItem(Wielded) : 0xFFFF;

	SaveFile->write((char*)&Index, sizeof(Index));

	SaveFile->write((char*)&Strength, sizeof(Strength));
	SaveFile->write((char*)&Endurance, sizeof(Endurance));
	SaveFile->write((char*)&Agility, sizeof(Agility));
	SaveFile->write((char*)&Perception, sizeof(Perception));
	SaveFile->write((char*)&RegenerationCounter, sizeof(RegenerationCounter));
	SaveFile->write((char*)&HP, sizeof(HP));
	SaveFile->write((char*)&NP, sizeof(NP));
	SaveFile->write((char*)&AP, sizeof(AP));
	SaveFile->write((char*)&StrengthExperience, sizeof(StrengthExperience));
	SaveFile->write((char*)&EnduranceExperience, sizeof(EnduranceExperience));
	SaveFile->write((char*)&AgilityExperience, sizeof(AgilityExperience));
	SaveFile->write((char*)&PerceptionExperience, sizeof(PerceptionExperience));
	SaveFile->write((char*)&HasActed, sizeof(HasActed));
	SaveFile->write((char*)&Relations, sizeof(Relations));
	SaveFile->write((char*)&Fainted, sizeof(Fainted));
	SaveFile->write((char*)&EatingCurrently, sizeof(EatingCurrently));
	SaveFile->write((char*)&APsToBeEaten, sizeof(APsToBeEaten));
	SaveFile->write((char*)&Dead, sizeof(Dead)); // ?
}

character::character(std::ifstream* SaveFile) : object(SaveFile)
{
	Stack = new stack(SaveFile);

	ushort Index;

	SaveFile->read((char*)&Index, sizeof(Index));

	Wielded = Index != 0xFFFF ? Stack->CItem(Index) : 0;

	SaveFile->read((char*)&Strength, sizeof(Strength));
	SaveFile->read((char*)&Endurance, sizeof(Endurance));
	SaveFile->read((char*)&Agility, sizeof(Agility));
	SaveFile->read((char*)&Perception, sizeof(Perception));
	SaveFile->read((char*)&RegenerationCounter, sizeof(RegenerationCounter));
	SaveFile->read((char*)&HP, sizeof(HP));
	SaveFile->read((char*)&NP, sizeof(NP));
	SaveFile->read((char*)&AP, sizeof(AP));
	SaveFile->read((char*)&StrengthExperience, sizeof(StrengthExperience));
	SaveFile->read((char*)&EnduranceExperience, sizeof(EnduranceExperience));
	SaveFile->read((char*)&AgilityExperience, sizeof(AgilityExperience));
	SaveFile->read((char*)&PerceptionExperience, sizeof(PerceptionExperience));
	SaveFile->read((char*)&HasActed, sizeof(HasActed));
	SaveFile->read((char*)&Relations, sizeof(Relations));
	SaveFile->read((char*)&Fainted, sizeof(Fainted));
	SaveFile->read((char*)&EatingCurrently, sizeof(EatingCurrently));
	SaveFile->read((char*)&APsToBeEaten, sizeof(APsToBeEaten));
	SaveFile->read((char*)&Dead, sizeof(Dead)); // ?
}

void humanoid::Save(std::ofstream* SaveFile)
{
	character::Save(SaveFile);

	ushort Index = Armor.Torso ? Stack->SearchItem(Armor.Torso) : 0xFFFF;

	SaveFile->write((char*)&Index, sizeof(Index));

	SaveFile->write((char*)&ArmType, sizeof(ArmType));
	SaveFile->write((char*)&HeadType, sizeof(HeadType));
	SaveFile->write((char*)&LegType, sizeof(LegType));
	SaveFile->write((char*)&TorsoType, sizeof(TorsoType));
}

/*void humanoid::LoadSpecialVariables(std::ifstream* SaveFile)
{
	
}*/

bool character::WizardMode(void)
{
	if(!game::CWizardMode())
	{
		if(game::BoolQuestion("Do you want to cheat, cheater? This action cannot be undone. [Y/N]"))
		{
			game::EnableWizardMode();

			for(ushort x = 0; x < 5; x++)
				CStack()->AddItem(new scrollofwishing);
		}
	}
	else
		ADD_MESSAGE("You have already activated Wizard Mode. It cannot be toggled off.");
	return false;

}

bool character::RaiseStats(void)
{
	if(game::CWizardMode())
	{
	Strength += 10;   // I won't touch these just in case we'd do something
	Endurance += 10;  // really odd with CStrength() etc.
	Agility += 10;
	Perception += 10;
	HP = Endurance << 1;
	}
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");

	return false;
}

bool character::LowerStats(void)
{
	if(game::CWizardMode())
	{
		Strength -= 10;
		Endurance -= 10;
		Agility -= 10;
		Perception -= 10;
		HP = Endurance << 1;
	}
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");

	return false;
}

bool character::GainAllItems(void)
{
	if(game::CWizardMode())
		for(ushort c = 1; game::CItemPrototype(c); c++)
			Stack->AddItem(game::CItemPrototype(c)->Clone());
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");

	return false;
}

bool character::SeeWholeMap(void)
{
	if(game::CWizardMode())
	{
		game::SeeWholeMap();
	}
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");
	return false;
}

bool character::IncreaseGamma(void)
{
	//GGG graphics::Gamma(0, game::EditGamma(-5));

	return false;
}

bool character::DecreaseGamma(void)
{
	//GGG graphics::Gamma(0, game::EditGamma(5));

	return false;
}

bool character::IncreaseSoftGamma(void)
{
	game::EditSoftGamma(0.05f);

	return false;
}

bool character::DecreaseSoftGamma(void)
{
	game::EditSoftGamma(-0.05f);

	return false;
}

ushort character::CEmitation(void)
{
	ushort Emitation = 0;

	for(ushort c = 0; c < CMaterials(); c++)
		if(Material[c])
			if(Material[c]->CEmitation() > Emitation)
				Emitation = Material[c]->CEmitation();

	if(CStack()->CEmitation() > Emitation)
		Emitation = CStack()->CEmitation();

	return Emitation;
}

void character::SSquareUnder(square* Square)
{
	SquareUnder = Square; Stack->SSquareUnder(Square);
}

bool character::WalkThroughWalls(void)
{
	if(game::CWizardMode())
		game::GoThroughWalls();
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");
	return false;
}


float character::CWeaponStrength(void)
{
	return CWielded() ? CWielded()->GetWeaponStrength() : GetMeleeStrength();
}


bool character::ShowKeyLayout(void)
{
	list List("Keyboard Layout");

	List.AddDescription("");
	List.AddDescription("Key       Description");

	for(uchar c = 1; game::CCommand()[c]; c++)
	{
		std::string Buffer;
		Buffer += game::CCommand()[c]->CKey();
		Buffer.resize(10, ' ');
		List.AddString(Buffer + game::CCommand()[c]->CDescription());
	}

	List.Draw(false);

	return false;
}

bool character::Look(void)
{
	int Key;
	std::string OldMemory;
	vector CursorPos = CPos();
	game::DrawEverythingNoBlit();
	FONTW->PrintfToDB(16, 514, "Press direction keys to move cursor or esc to exit from the mode.");
	DRAW_MESSAGES;
	EMPTY_MESSAGES;
	graphics::BlitDBToScreen();

	while(Key != ' ' && Key != 0x1B)
	{


		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::CMoveCommandKey()[c])
				{
				CursorPos += game::CMoveVector()[c];

				if (short(CursorPos.X) > game::CCurrentLevel()->CXSize()-1)	CursorPos.X = 0;
				if (short(CursorPos.X) < 0)					CursorPos.X = game::CCurrentLevel()->CXSize()-1;
				if (short(CursorPos.Y) > game::CCurrentLevel()->CYSize()-1)	CursorPos.Y = 0;
				if (short(CursorPos.Y) < 0)					CursorPos.Y = game::CCurrentLevel()->CYSize()-1;
				}
			if(this == game::CPlayer())
			{
				if(CursorPos.X < game::CCamera().X + 2 || CursorPos.X > game::CCamera().X + 48)
					game::UpdateCameraXWithPos(CursorPos.X);

				if(CursorPos.Y < game::CCamera().Y + 2 || CursorPos.Y > game::CCamera().Y + 27)
					game::UpdateCameraYWithPos(CursorPos.Y);

			}

		if(game::CSeeWholeMapCheat())
		{
			OldMemory = game::CCurrentLevel()->CLevelSquare(CursorPos)->CRememberedItems();
			game::CCurrentLevel()->CLevelSquare(CursorPos)->UpdateItemMemory();
		}

		if(game::CCurrentLevel()->CLevelSquare(CursorPos)->CKnown() || game::CSeeWholeMapCheat())
		{
			std::string Msg;

			if(game::CCurrentLevel()->CLevelSquare(CursorPos)->CanBeSeen() || game::CSeeWholeMapCheat())
				Msg = "You see here ";
			else
				Msg = "You remember here ";

			bool Anything = false;

			if(game::CCurrentLevel()->CLevelSquare(CursorPos)->COverTerrain()->Name(INDEFINITE) != "an air atmosphere")
			{
				Msg += game::CCurrentLevel()->CLevelSquare(CursorPos)->COverTerrain()->Name(INDEFINITE);
	
				Anything = true;
			}

			if((game::CSeeWholeMapCheat() || game::CCurrentLevel()->CLevelSquare(CursorPos)->CanBeSeen()) && game::CCurrentLevel()->CLevelSquare(CursorPos)->CCharacter())
			{
				if(Anything)
					if(game::CCurrentLevel()->CLevelSquare(CursorPos)->CRememberedItems() != "")
						Msg += std::string(", ");
					else
						Msg += std::string(" and ");

				Msg += std::string(game::CCurrentLevel()->CLevelSquare(CursorPos)->CCharacter()->Name(INDEFINITE));

				Anything = true;
			}

			if(game::CCurrentLevel()->CLevelSquare(CursorPos)->CRememberedItems() != "")
			{
				if(Anything)
					Msg += std::string(" and ");

				Msg += std::string(game::CCurrentLevel()->CLevelSquare(CursorPos)->CRememberedItems()) + " on " + game::CCurrentLevel()->CLevelSquare(CursorPos)->CGroundTerrain()->Name(INDEFINITE);
			}
			else
				if(Anything)
					Msg += std::string(" on ") + game::CCurrentLevel()->CLevelSquare(CursorPos)->CGroundTerrain()->Name(INDEFINITE);
				else
					Msg += std::string(game::CCurrentLevel()->CLevelSquare(CursorPos)->CGroundTerrain()->Name(INDEFINITE));

			ADD_MESSAGE("%s.", Msg.c_str());
		}
		else
			ADD_MESSAGE("You have no idea what might lie here.");

		if(game::CSeeWholeMapCheat())
			game::CCurrentLevel()->CLevelSquare(CursorPos)->SRememberedItems(OldMemory);

		game::DrawEverythingNoBlit();
		igraph::CCharacterGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (CursorPos.X - game::CCamera().X) << 4, (CursorPos.Y - game::CCamera().Y + 2) << 4, 16, 16);
		FONTW->PrintfToDB(16, 514, "Press direction keys to move cursor or esc to exit from the mode.");
		graphics::BlitDBToScreen();
		EMPTY_MESSAGES;

		Key = game::GetKey();
	}
	return false;
}

std::string golem::NameSingular(void) const
{
	return Material[0]->Name() + " golem";
}

void golem::Die(void)
{
	if(Dead)
		return;

	if(game::CPlayer() == this) 
	{
		ADD_MESSAGE("Your Holy Words fly out of your nose and you collapse and die.");

		game::DrawEverything();

		globalwindowhandler::GetKey();

		game::StoryScreen("Unfortunately thee died during thine journey. The Überpriest is not happy.");

		game::RemoveSaves();

		game::Quit();
	}
	else
		if(CLevelSquareUnder()->CanBeSeen()) ADD_MESSAGE("The Holy Words of the golem fly away and the golem collapses.");

	Dead = true;

	while(CStack()->CItems())
		CStack()->MoveItem(0, CLevelSquareUnder()->CStack());

	item* Corpse = new corpse(false);

	Corpse->InitMaterials(CMaterial(0));

	CLevelSquareUnder()->CStack()->AddItem(Corpse);

	SMaterial(0, 0);

	CLevelSquareUnder()->RemoveCharacter();

	game::SendToHell(this);
}

float golem::GetMeleeStrength(void)
{
	return 75 * CMaterial(0)->GetHitValue();
}

float character::CDifficulty(void)
{
	return float(CStrength()) * CEndurance() * CAgility() * CWeaponStrength() / (float(CalculateArmorModifier()) * 25000);
}

float character::CAttackStrength(void)
{
	return CWielded() ? CWielded()->GetWeaponStrength() : GetMeleeStrength();
}

bool character::Engrave(std::string What)
{
	game::CCurrentLevel()->CLevelSquare(CPos())->Engrave(What);

	return true;
}

bool character::WhatToEngrave(void)
{
	game::CCurrentLevel()->CLevelSquare(CPos())->Engrave(game::StringQuestion("What do you want to engrave here?", 50));
	return false;
}

void character::MoveRandomly(void)
{
	ushort ToTry = rand() % 8;

	if(!game::CCurrentLevel()->CLevelSquare(CPos() + game::CMoveVector()[ToTry])->CCharacter());
		TryMove(CPos() + game::CMoveVector()[ToTry]);
}

ushort golem::CalculateArmorModifier(void)
{
	if(CMaterial(0)->CArmorValue() / 2 > 90)
		return 10;
	else
		return 100 - CMaterial(0)->CArmorValue() / 2;
}

void golem::MoveRandomly(void)
{
	ushort ToTry = rand() % 9;
	if(ToTry == 8)
	{
		if(!(rand () % 5))
			Engrave("Golem Needs Master");
	}
	else
	if(!game::CCurrentLevel()->CLevelSquare(CPos() + game::CMoveVector()[ToTry])->CCharacter())
		TryMove(CPos() + game::CMoveVector()[ToTry]);
}


bool character::TestForPickup(item* ToBeTested)
{
	if(GetBurdenState(ToBeTested->CWeight() + CStack()->SumOfMasses()) != UNBURDENED)
		return false;
	return true;
}

//humanoid::humanoid(ushort Size, ushort Agility, ushort Strength, ushort Endurance, ushort Perception, uchar ArmType, uchar HeadType, uchar LegType, uchar TorsoType, uchar Relations, bool CreateMaterials)  : character(Size, Agility, Strength, Endurance, Perception, Relations, CreateMaterials), ArmType(ArmType), HeadType(HeadType), LegType(LegType), TorsoType(TorsoType) {}

bool character::OpenPos(vector APos)
{
	if(game::CCurrentLevel()->CLevelSquare(APos)->Open(this))
	{
		SAgilityExperience(CAgilityExperience() + 25);
		SNP(CNP() - 1);
		return true;
	}
	return false;
}

bool character::Pray(void)
{
	list Panthenon("To Whom shall thee adress thine prayers?");

	if(!CLevelSquareUnder()->CDivineOwner())
		for(ushort c = 1; game::CGod(c); c++)
			Panthenon.AddString(game::CGod(c)->CompleteDescription());
	else
		Panthenon.AddString(game::CGod(CLevelSquareUnder()->CDivineOwner())->CompleteDescription());

	ushort Select = Panthenon.Draw();

	if(Select & 0x8000)
		return false;
	else
	{
		if(CLevelSquareUnder()->CDivineOwner())
		{
			if(!Select)
			{
				if(game::BoolQuestion("Do you really wish to pray? [Y/N]"))
					game::CGod(CLevelSquareUnder()->CDivineOwner())->Pray();
				else
					return false;
			}
			else
				return false;
		}
		else
		{
			if(game::BoolQuestion("Do you really wish to pray? [Y/N]"))
				game::CGod(Select+1)->Pray();
			else
				return false;
		}

		return true;
	}
}

void character::SpillBlood(uchar HowMuch)
{
	CLevelSquareUnder()->SpillFluid(HowMuch, CBloodColor(),5,40);
}

void character::NeutralAICommand(void)
{
	dynarray<character*> SeenCharacters;

	DO_FILLED_RECTANGLE(CPos().X, CPos().Y, 0, 0, game::CCurrentLevel()->CXSize() - 1, game::CCurrentLevel()->CYSize() - 1, LOSRange(),
	{
		if(game::CCurrentLevel()->CLevelSquare(vector(XPointer,YPointer))->CCharacter())
			SeenCharacters.Add(game::CCurrentLevel()->CLevelSquare(vector(XPointer, YPointer))->CCharacter());
	});

	for(ushort c = 0; c < SeenCharacters.Length(); c++)
	{
		if(!SeenCharacters.Access(c)->CRelations() && SeenCharacters.Access(c)->CLevelSquareUnder()->CanBeSeenFrom(CPos()))
		{
			Charge(SeenCharacters.Access(c));
			SHasActed(true);
			return;
		}
	}

	DO_FOR_SQUARES_AROUND(CPos().X, CPos().Y, game::CCurrentLevel()->CXSize(), game::CCurrentLevel()->CYSize(),
	if(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->Open(this))
	{
		if(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CanBeSeen())
		{
			if(CLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s opens the door.", CNAME(DEFINITE));
			else
				ADD_MESSAGE("Something opens the door.");
		}

	SHasActed(true);
	return;
	})

	if(CLevelSquareUnder()->CStack()->CItems())
	{
		ushort ItemToTry = rand() % CLevelSquareUnder()->CStack()->CItems();
		if(TestForPickup(CLevelSquareUnder()->CStack()->CItem()[ItemToTry]))
		{
			CLevelSquareUnder()->CStack()->MoveItem(ItemToTry, CStack());
			SHasActed(true);
		}
		for(ushort c = 0; c < CStack()->CItems(); c++)
		{
			if(CanWield())
				if(CStack()->CItem()[c]->GetWeaponStrength() > CWeaponStrength())
				{
					SWielded(CStack()->CItem()[c]);
					break;
				}
			if(CanWear() && CStack()->CItem()[c]->CanBeWorn())
				if(CStack()->CItem()[c]->GetArmorValue() < CalculateArmorModifier())
				{
					WearItem(CStack()->CItem()[c]);
					break;
				}
			if(CStack()->CItem()[c]->Consumable(this))
			{
				item* Temp = CStack()->CItem()[c];
				CStack()->RemoveItem(c);
				delete Temp;
			}

			for(ushort c = 0; c < CLevelSquareUnder()->CStack()->CItems(); c++)
			{
				if(CLevelSquareUnder()->CStack()->CItem()[c]->GetWeaponStrength() > CWeaponStrength() && GetBurdenState(CStack()->SumOfMasses() + CLevelSquareUnder()->CStack()->CItem()[c]->CWeight()) == UNBURDENED && CanWield())
				{
					if(CWielded())
						CStack()->MoveItem(CStack()->SearchItem(CWielded()), CLevelSquareUnder()->CStack());

					SWielded(CStack()->CItem()[CLevelSquareUnder()->CStack()->MoveItem(c, CStack())]);
				}
				if(CLevelSquareUnder()->CStack()->CItem()[c]->GetArmorValue() > CalculateArmorModifier() && GetBurdenState(CStack()->SumOfMasses() + CLevelSquareUnder()->CStack()->CItem()[c]->CWeight()) == UNBURDENED && CanWear())
				{
					if(CTorsoArmor())
						CStack()->MoveItem(CStack()->SearchItem(CTorsoArmor()), CLevelSquareUnder()->CStack());
					WearItem(CStack()->CItem()[CLevelSquareUnder()->CStack()->MoveItem(c, CStack())]);
				}
				if(CLevelSquareUnder()->CStack()->CItem()[c]->Consumable(this))
				{
					item* Temp = CLevelSquareUnder()->CStack()->CItem()[c];
					CLevelSquareUnder()->CStack()->RemoveItem(c);
					delete Temp;
				}
			}
		}
		return;
	}

	MoveRandomly();
}

void character::FriendAICommand(void)
{

}

void character::HostileAICommand(void)
{
	dynarray<character*> SeenCharacters;

	DO_FILLED_RECTANGLE(CPos().X, CPos().Y, 0, 0, game::CCurrentLevel()->CXSize() - 1, game::CCurrentLevel()->CYSize() - 1, LOSRange(),
	{
		if(game::CCurrentLevel()->CLevelSquare(vector(XPointer,YPointer))->CCharacter())
			SeenCharacters.Add(game::CCurrentLevel()->CLevelSquare(vector(XPointer, YPointer))->CCharacter());
	});

	ushort NumberFor;

	if((NumberFor = SeenCharacters.Search(game::CPlayer())) != 0xFFFF && CLevelSquareUnder()->RetrieveFlag())
	{
		Charge(SeenCharacters.Access(NumberFor));
		SHasActed(true);
		return;
	}
	else
	{
		for(ushort c = 0; c < SeenCharacters.Length(); c++)
		{
			if(SeenCharacters.Access(c)->CRelations() > 0 && SeenCharacters.Access(c)->CLevelSquareUnder()->CanBeSeenFrom(CPos()))
			{
				Charge(SeenCharacters.Access(c));
				SHasActed(true);
				return;
			}
		}
	}

	DO_FOR_SQUARES_AROUND(CPos().X, CPos().Y, game::CCurrentLevel()->CXSize(), game::CCurrentLevel()->CYSize(),
	if(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->Open(this))
	{
		if(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CanBeSeen())
		{
			if(CLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s opens the door.", CNAME(DEFINITE));
			else
				ADD_MESSAGE("Something opens the door.");
		}

	SHasActed(true);
	return;
	})

	if(CLevelSquareUnder()->CStack()->CItems())
	{
		for(ushort c = 0; c < CStack()->CItems(); c++)
		{
			if(CanWield())
				if(CStack()->CItem()[c]->GetWeaponStrength() > CWeaponStrength())
				{
					SWielded(CStack()->CItem()[c]);
					return;
				}
			if(CanWear() && CStack()->CItem()[c]->CanBeWorn())
				if(CStack()->CItem()[c]->GetArmorValue() < CalculateArmorModifier())
				{
					WearItem(CStack()->CItem()[c]);
					return;
				}
			if(CStack()->CItem()[c]->Consumable(this))
			{
				item* Temp = CStack()->CItem()[c];
				CStack()->RemoveItem(c);
				delete Temp;
				return;
			}

			for(ushort c = 0; c < CLevelSquareUnder()->CStack()->CItems(); c++)
			{
				if(CLevelSquareUnder()->CStack()->CItem()[c]->GetWeaponStrength() > CWeaponStrength() && GetBurdenState(CStack()->SumOfMasses() + CLevelSquareUnder()->CStack()->CItem()[c]->CWeight()) && CanWield())
				{
					if(CWielded())
						CStack()->MoveItem(CStack()->SearchItem(CWielded()), CLevelSquareUnder()->CStack());

					SWielded(CStack()->CItem(CLevelSquareUnder()->CStack()->MoveItem(c, CStack())));
					return;
				}
				if(CLevelSquareUnder()->CStack()->CItem()[c]->GetArmorValue() > CalculateArmorModifier() && GetBurdenState(CStack()->SumOfMasses() + CLevelSquareUnder()->CStack()->CItem()[c]->CWeight()) && CanWear())
				{
					if(CTorsoArmor())
						CStack()->MoveItem(CStack()->SearchItem(CTorsoArmor()), CLevelSquareUnder()->CStack());

					WearItem(CStack()->CItem(CLevelSquareUnder()->CStack()->MoveItem(c, CStack())));
					return;
				}
				if(CLevelSquareUnder()->CStack()->CItem()[c]->Consumable(this))
				{
					item* Temp = CLevelSquareUnder()->CStack()->CItem()[c];
					CLevelSquareUnder()->CStack()->RemoveItem(c);
					delete Temp;
					return;
				}
			}
		}
		ushort ItemToTry = rand() % CLevelSquareUnder()->CStack()->CItems();
		if(TestForPickup(CLevelSquareUnder()->CStack()->CItem()[ItemToTry]))
		{
			CLevelSquareUnder()->CStack()->MoveItem(ItemToTry, CStack());
			SHasActed(true);
			return;
		}
	}

	MoveRandomly();
}

void ennerbeast::HostileAICommand(void)
{
	if(CLevelSquareUnder()->RetrieveFlag() && (float(CPos().X) - game::CPlayer()->CPos().X) * (float(CPos().X) - game::CPlayer()->CPos().X) + (float(CPos().Y) - game::CPlayer()->CPos().Y) * (float(CPos().Y) - game::CPlayer()->CPos().Y) <= LOSRange())
	{
		Charge(game::CPlayer());

		SHasActed(true);

		return;
	}
	else
	{
		if(rand() % 3)
			Hit(0);
		else
			MoveRandomly();
	}
}

void perttu::NeutralAICommand(void)
{
	SHealTimer(CHealTimer() + 1);
	DO_FOR_SQUARES_AROUND(CPos().X, CPos().Y, game::CCurrentLevel()->CXSize(), game::CCurrentLevel()->CYSize(),
	if(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter())
	{
		if(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter() == game::CPlayer())
		{
			if(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter()->CHP() < (game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter()->CEndurance() << 1) / 3 && game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter() == game::CPlayer() && CHealTimer() > 100)
				HealFully(game::CPlayer());
		}
		else
			Hit(game::CCurrentLevel()->CLevelSquare(vector(DoX, DoY))->CCharacter());
	})
}

void perttu::HealFully(character* ToBeHealed)
{
	SHealTimer(0);
	ToBeHealed->SHP(ToBeHealed->CEndurance() << 1);
	if(ToBeHealed == game::CPlayer())
		ADD_MESSAGE("%s heals you fully.", CNAME(DEFINITE));
}

void character::ReceiveSchoolFoodEffect(long)
{
	if(this == game::CPlayer())
		ADD_MESSAGE("Yuck! This stuff tastes like vomit and old mousepads.");
	//else
	//	if(CLevelSquareUnder()->CanBeSeen())
	//		ADD_MESSAGE("%s vomits.", CNAME(DEFINITE));

	SHP(CHP() - 1 - rand() % 5);
	Vomit(2);
	CheckDeath("was poisoned by school food");

	if(!(rand() % 10))
	{
		if(this == game::CPlayer())
			ADD_MESSAGE("You gain a little bit of toughness for surviving this stuff.");

		SEndurance(CEndurance() + 1 + rand() % 5);
	}
}

void character::ReceiveNutrition(long SizeOfEffect)
{
	SNP(CNP() + SizeOfEffect / 1000);
}

void character::ReceiveOmleUrineEffect(long)
{
	if(this == game::CPlayer()) ADD_MESSAGE("You feel a primitive Force coursing through your veins.");
	SStrength(CStrength() + 1 + rand() % 5);
	SHP(CHP() + 2);
}

void character::ReceivePepsiEffect(long SizeOfEffect)
{
	SHP(CHP() - short(sqrt(SizeOfEffect / 20)));
	CheckDeath("was poisoned by pepsi");
	if(short(CPerception() - short(sqrt(SizeOfEffect / 20))) > 0)
		SPerception(CPerception() - short(sqrt(SizeOfEffect / 20)));
	else SPerception(1);
}

void character::Darkness(long SizeOfEffect)
{
	if(SizeOfEffect == 0) SizeOfEffect++; // Gum solution... It appears that while eating Elpuries corpse this *might* result a division by zero error
	ushort x = 30 + rand() % SizeOfEffect;
	if(this == game::CPlayer()) ADD_MESSAGE("Arg. You feel the fate of a navastater placed upon you...");
	if(CStrength() - x / 30 > 1) SStrength(CStrength() - x / 30); // Old comment was about eating... This
	else SStrength(1);                                        // can happen with drinkin, hitting etc.
	if(CEndurance() - x / 30 > 1) SEndurance(CEndurance() - x / 30);
	else SEndurance(1);
	if(CAgility() - x / 30 > 1) SAgility(CAgility() - x / 30);
	else SAgility(1);
	if(CHP() - x / 10 > 1) SHP(CHP() - x / 10);
	else SHP(1);
}

bool character::Kick(void)
{
	uchar Direction;
	ADD_MESSAGE("In what direction do you wish to kick?");
	game::DrawEverything();
	vector KickPos = game::AskForDirectionVector();
	if(KickPos != vector(0,0))
	{
		for(uchar c = 0; c < 8; c++)
		{
			if(KickPos == game::CMoveVector()[c])
				Direction = c;
		}
		game::CCurrentLevel()->CLevelSquare(CPos() + KickPos)->Kick(CStrength(), Direction);
		return true;
	}

	return false;
}


void perttu::Save(std::ofstream* SaveFile)
{
	humanoid::Save(SaveFile);

	SaveFile->write((char*)&HealTimer, sizeof(HealTimer));
}



bool character::ScreenShot(void)
{
	bitmap TempDB(800, 600);

	TempDB.ReadFromDB(0,0);

	TempDB.Save("Scrshot.bmp");

	return false;
}

bool character::Offer(void)
{
	if(CLevelSquareUnder()->COverTerrain()->CanBeOffered())
	{
		ushort Index = CStack()->DrawContents("What do you want to offer?");
		if(Index < CStack()->CItems())
		{
			if(CStack()->CItem()[Index] == CWielded())
			{
				ADD_MESSAGE("You can't offer something that you wield.");
				return false;
			}
			if(CStack()->CItem()[Index] == CTorsoArmor())
			{
				ADD_MESSAGE("You can't offer something that you wear.");
				return false;
			}

			if(game::CGod(CLevelSquareUnder()->COverTerrain()->COwnerGod())->ReceiveOffer(CStack()->CItem(Index)))
			{
				item* Temp = CStack()->CItem()[Index];
				CStack()->RemoveItem(Index);
				delete Temp;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		ADD_MESSAGE("There isn't any altar here!");

	return false;
}

long character::Score(void)
{
	long Score = CHP() * 5 + CNP() / 10 + CEndurance() * 30 + (CStrength() + CAgility() + CPerception()) * 40;

	Score += Stack->Score();

	Score += game::GodScore();

	return Score;
}

long character::AddScoreEntry(std::string Description, float Multiplier)
{
	highscore HScore;

	HScore.Add(long((Score() - game::CBaseScore()) * Multiplier), game::CPlayerName() + ", " + Description);

	HScore.Save();

	return 0; //??????????
}

bool character::CheckDeath(std::string Msg)
{
	if(CHP() < 1)
	{
		if(this == game::CPlayer() && !game::CWizardMode())
			AddScoreEntry(Msg);

		Die();

		return true;
	}
	else
		return false;
}

ulong golem::Danger(void)
{
	return ulong(GetMeleeStrength() * 100 / (CalculateArmorModifier() * 15));
}

bool character::DrawMessageHistory(void)
{
	game::GlobalMessagingSystem.DrawMessageHistory();
	return true;
}

bool character::Throw(void)
{
	ushort Index;
	if((Index = CStack()->DrawContents("What do you want to throw?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to throw.");
		return false;
	}

	if(Index < CStack()->CItems())
	{
		if(CStack()->CItem()[Index] == CWielded())
		{
			ADD_MESSAGE("You can't throw something that you wield.");
			return false;
		}
		uchar Answer = game::DirectionQuestion("In what direction do you wish to throw?", 8, false);
		if(Answer == 0xFF)
			return false;
		ThrowItem(Answer, CStack()->CItem()[Index]);

	}
	else
		return false;

	return true;
}

bool humanoid::Throw(void)
{
	ushort Index;
	if((Index = CStack()->DrawContents("What do you want to throw?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to throw.");
		return false;
	}


	if(Index < CStack()->CItems())
	{
		if(CStack()->CItem()[Index] == CWielded())
		{
			ADD_MESSAGE("You can't throw something that you wield.");
			return false;
		}
		if(CStack()->CItem()[Index] == CTorsoArmor())
		{
			ADD_MESSAGE("You can't throw something that you wear.");
			return false;
		}
		uchar Answer = game::DirectionQuestion("In what direction do you wish to throw it?", 8, false);
		if(Answer == 0xFF)
			return false;
		ThrowItem(Answer, CStack()->CItem(Index));
	}
	else
		return false;

	return true;
}

bool character::ThrowItem(uchar Direction, item* ToBeThrown) // Note to C++ people. This is throw in the GAME not the throw in c++!
{
	if(Direction > 7)
		ABORT("Throw in TOO odd direction...");
	return ToBeThrown->Fly(Direction, CStrength(), CStack(), game::CPlayer() == this);
}

void character::HasBeenHitByItem(item* Thingy, float Speed, bool CanBeSeen)
{
	ushort Damage = ushort(Thingy->GetWeaponStrength() * Thingy->CWeight() * CalculateArmorModifier() * sqrt(Speed) / 2000000000) + (rand() % 5 ? 1 : 0);

	SHP(CHP() - Damage);

	if(CanBeSeen && game::CPlayer() != this)
	{
		ADD_MESSAGE("%s hits %s.", Thingy->CNAME(DEFINITE), CNAME(DEFINITE));

		if(game::CWizardMode())
			ADD_MESSAGE("(damage: %d) (speed: %f)", Damage, Speed);
	}

	if(game::CPlayer() == this)
		ADD_MESSAGE("%s hits you.", Thingy->CNAME(DEFINITE));

	SpillBlood(1 + rand() % 1);
	CheckDeath(std::string("died by thrown ") + Thingy->CNAME(INDEFINITE) );
}

bool dog::Catches(item* Thingy, float, bool CanBeSeen)
{
	if(Thingy->DogWillCatchAndConsume())
	{
		if(game::CPlayer() == this)
			ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CNAME(DEFINITE));
		else
			if(CanBeSeen) ADD_MESSAGE("%s catches %s and eats it.", CNAME(DEFINITE), Thingy->CNAME(DEFINITE));
		ConsumeItem(CLevelSquareUnder()->CStack()->SearchItem(Thingy), CLevelSquareUnder()->CStack());
		return true;
	}
	return false;
}

bool dog::ConsumeItemType(uchar Type)     // We need a better system for this... Writing this to every F***ing character that needs one
{					  // is Stoo-bit
	switch(Type)
	{
	case LIQUID:
		return true;
	break;
		
	case ODD:
		return false;
	break;
	case FRUIT:
		return true;
	break;
	case MEAT:
		return true;
	break;
	case SPOILED:
		return true;
	break;
	case HARD:
		return false;
	break;
	case SCHOOLFOOD:
		return true;
	break;
	case CONTAINER:
		return false;
	break;
	case BONE: // Bone is an odd item, cause it actually can be opened, but not always...
		return true;
	break;


	default:
		ADD_MESSAGE("ERRRRORRRRRR in dog::Consumeitemtype."); //All hail SpykoS! He is the author of this file, and his might is over that of PMGR!!!
	}
		
	return false;
}

bool character::DodgesFlyingItem(item*, float Speed, bool)
{			// Formula requires a little bit a tweaking...
	//if(Speed * 25 - (rand() % CAgility()) * 100 + CSize() * 3< 0)
	//	return true;

	if(!(rand() % 20))
		return rand() % 2 ? true : false;

	if(rand() % ulong(sqrt(Speed) * CSize() / CAgility() * 10 + 1) > 40)
		return false;
	else
		return true;
}

void character::ReceiveFireDamage(long SizeOfEffect)
{
	if(game::CPlayer() == this) ADD_MESSAGE("You burn.");
	SHP(CHP() - (rand() % SizeOfEffect + SizeOfEffect));
}

void character::GetPlayerCommand(void)
{

	while(!CHasActed())
	{
		game::DrawEverythingWithDebug();

		clock_t Tim = clock();

		int Key = game::GetKey();
		bool ValidKeyPressed = false;

		{
		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::CMoveCommandKey()[c])
			{
				HasActed = TryMove(CPos() + game::CMoveVector()[c]);
				ValidKeyPressed = true;
			}
		}

		for(uchar c = 1; game::CCommand()[c]; c++)
			if(Key == game::CCommand()[c]->CKey())
			{
				HasActed = (this->*game::CCommand()[c]->CLinkedFunction())();
				ValidKeyPressed = true;
			}

		game::HandlePlayTim += clock() - Tim;
		if (!ValidKeyPressed) ADD_MESSAGE("Unknown key, you %s. Press '?' for a list of commands.", game::Insult());
	}
}

void character::ContinueEating(void)
{
	if(CAPsToBeEaten() + CAP() > 50000)
	{
		if(this == game::CPlayer()) ADD_MESSAGE("You have eaten for a long time now...");
		StopEating();
	}
	else if(CAP() >= 1000)
	{
		if(CLevelSquareUnder()->CStack()->CItem(CConsumingCurrently())->Consume(this))
			delete CLevelSquareUnder()->CStack()->RemoveItem(CConsumingCurrently());

		SConsumingCurrently(0xFFFF);

		Hunger(ushort(CAPsToBeEaten() / 1000));
		Regenerate(ushort(CAPsToBeEaten() / 1000));
		game::Turn(ushort(CAPsToBeEaten() / 1000));
		game::ApplyDivineTick(ushort(CAPsToBeEaten() / 1000));
	}
}

void character::StopEating(void)
{
	float NowEaten = CAPsToBeEaten() + CAP();
	float Temp = (NowEaten / CAPsToBeEaten()) * 100;
	SAP(1000);
	if(CStack()->CLevelSquareUnder()->CStack()->CItem(CConsumingCurrently())->Consume(this, Temp))
		delete CLevelSquareUnder()->CStack()->RemoveItem(CConsumingCurrently());
	Hunger(ushort(NowEaten / 1000));
	Regenerate(ushort(NowEaten / 1000));
	game::Turn(ushort(NowEaten / 1000));
	game::ApplyDivineTick(ushort(NowEaten / 1000));
	SConsumingCurrently(0xFFFF);
}

void character::Vomit(ushort HowMuch)
{
	if(this == game::CPlayer())
		ADD_MESSAGE("You vomit.");
	else
		if(CLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s vomits.", CNAME(DEFINITE));
	SNP(CNP() - 100);
	CLevelSquareUnder()->SpillFluid(HowMuch, RGB(10,230,10),3,50);
}

bool character::Apply(void)
{
	ushort Index;
	if((Index = CStack()->DrawContents("What do you want to apply?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to apply.");
		return false;
	}

	if(Index < CStack()->CItems())
	{
		if(CStack()->CItem()[Index] == CWielded())
		{
			ADD_MESSAGE("You can't throw something that you wield.");
			return false;
		}

		if(!CStack()->CItem()[Index]->Apply(this))
			return false;
	}
	else
		return false;

	return true;
}


bool humanoid::Apply(void)
{
	ushort Index;
	if((Index = CStack()->DrawContents("What do you want to apply?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to apply.");
		return false;
	}


	if(Index < CStack()->CItems())
	{
		if(CStack()->CItem()[Index] == CWielded())
		{
			ADD_MESSAGE("You can't apply something that you wield.");
			return false;
		}
		if(CStack()->CItem()[Index] == CTorsoArmor())
		{
			ADD_MESSAGE("You can't apply something that you wear.");
			return false;
		}
		
		if(!CStack()->CItem()[Index]->Apply(this))
			return false;
	}
	else
		return false;

	return true;
}

vector character::CPos(void)
{
	return SquareUnder->CPos();
}

bool character::ForceVomit(void)
{
	ushort Amount = rand() % 3;
	ADD_MESSAGE("You push you fingers down to your throat.");
	if(Amount) 
		Vomit(rand() % 3);
	else
		ADD_MESSAGE("You are not able to vomit.");
	SAP(CAP() - 100);
	return true;
}
