#include <cmath>

#include "char.h"
#include "error.h"
#include "god.h"
#include "list.h"
#include "igraph.h"
#include "stack.h"
#include "lterrain.h"
#include "femath.h"
#include "hscore.h"
#include "strover.h"
#include "whandler.h"
#include "item.h"
#include "lsquare.h"
#include "level.h"
#include "worldmap.h"
#include "proto.h"
#include "message.h"

character::character(bool CreateMaterials, bool SetStats, bool CreateEquipment, bool AddToPool) : object(AddToPool), Stack(new stack), Wielded(0), RegenerationCounter(0), NP(1000), AP(0), StrengthExperience(0), EnduranceExperience(0), AgilityExperience(0), PerceptionExperience(0), Relations(0), Dead(false), IsPlayer(false)
{
	SetConsumingCurrently(0xFFFF);

	if(CreateMaterials || SetStats || CreateEquipment)
		ABORT("BOOO!");
}

character::~character(void)
{
	delete Stack;
}

void perttu::CreateInitialEquipment(void)
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new valpurijustifier)));
	WearItem(GetStack()->GetItem(GetStack()->FastAddItem(new platemail(new valpurium(4000)))));
}

void oree::CreateInitialEquipment(void)
{
	GetStack()->FastAddItem(new maakotkashirt);

	for(ushort c = 0; c < 6; c++)
	{
		item* Can = new can(false);
		Can->InitMaterials(2, new iron(10), new pepsi(330));
		GetStack()->FastAddItem(Can);
	}
}

void swatcommando::CreateInitialEquipment(void)
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new curvedtwohandedsword)));
}

void fallenvalpurist::CreateInitialEquipment(void)
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new pickaxe)));
}

void froggoblin::CreateInitialEquipment(void)
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new spear)));
}

void character::ReceiveSound(char* Pointer, short Success, float ScreamStrength)
{
	if(GetIsPlayer())
		ADD_MESSAGE(Pointer);

	ushort Damage = ushort(ScreamStrength * (1 + float(Success) / 100) / 20000);

	SetHP(HP - Damage);

	CheckDeath("killed by an Enner Beast's scream");
}

void character::Hunger(ushort Turns) 
{
	ulong BNP = GetNP();

	switch(GetBurdenState())
	{
	case UNBURDENED:
		SetNP(GetNP() - Turns);
		break;
	case BURDENED:
		SetNP(GetNP() - 2 * Turns);
		SetStrengthExperience(GetStrengthExperience() + 25 * Turns);
		SetAgilityExperience(GetAgilityExperience() - 10 * Turns);
		break;
	case STRESSED:
	case OVERLOADED:
		SetNP(GetNP() - 4 * Turns);
		SetStrengthExperience(GetStrengthExperience() + 50 * Turns);
		SetAgilityExperience(GetAgilityExperience() - 25 * Turns);
		break;
	}

	if(GetNP() < HUNGERLEVEL)
		SetStrengthExperience(GetStrengthExperience() - 10 * Turns);

	if(GetNP() < HUNGERLEVEL && BNP >= HUNGERLEVEL) if(GetIsPlayer()) ADD_MESSAGE("You are getting hungry.");
	if(GetNP() < CRITICALHUNGERLEVEL && BNP >= CRITICALHUNGERLEVEL) if(GetIsPlayer()) ADD_MESSAGE("You are getting very hungry.");
	if(GetNP() < 1)
	{
		if(!game::GetWizardMode())
			AddScoreEntry("starved to death");

		Die();
	}
}

bool character::Hit(character* Enemy)
{
	if(Enemy->GetRelations() != 0)
	{
	if(GetIsPlayer())
		if(!game::BoolQuestion("This might cause a hostile reaction. Are you sure? [Y/N]"))
			return false;
	}
	
	ushort Speed = GetWielded() ? ushort(sqrt((ulong(GetAgility() << 2) + GetStrength()) * 20000 / Wielded->GetWeight())) : ulong(GetAgility() << 2) + GetStrength();
	short Success = rand() % 26 - rand() % 26;
	float WeaponStrength = GetWielded() ? GetWielded()->GetWeaponStrength() : GetMeleeStrength();

	switch(Enemy->TakeHit(Speed, Success, WeaponStrength, this)) //there's no breaks and there shouldn't be any
	{
	case HAS_HIT:
	case HAS_BLOCKED:
		if(GetWielded())
			GetWielded()->ReceiveHitEffect(Enemy, this);
	case HAS_DIED:
		SetStrengthExperience(GetStrengthExperience() + 50);
	case HAS_DODGED:
		SetAgilityExperience(GetAgilityExperience() + 25);
	}

	SetNP(GetNP() - 4);

	return true;
}

ushort character::CalculateArmorModifier(void) const
{
	return 100;
}

ushort humanoid::CalculateArmorModifier(void) const
{
	return Armor.Torso ? Armor.Torso->GetArmorValue() : 100;
}

uchar character::TakeHit(ushort Speed, short Success, float WeaponStrength, character* Enemy)
{
	if(Enemy->GetIsPlayer())
		SetRelations(0);
	if(GetConsumingCurrently() != 0xFFFF) StopEating();
	if(!(rand() % 20))
	{
		ushort Damage = ushort(WeaponStrength * Enemy->GetStrength() * (1 + float(Success) / 100) * CalculateArmorModifier() / 1000000) + (rand() % 5 ? 2 : 1);

		if(GetLevelSquareUnder()->CanBeSeen())
		{
			Enemy->AddHitMessage(this,true);

			if(game::GetWizardMode())
				ADD_MESSAGE("(damage: %d)", Damage);
		}

		SetHP(GetHP() - Damage);

		if(rand() % 4) SpillBlood(rand() % 3);

		if(CheckDeath(std::string("killed by ") + Enemy->Name(INDEFINITE))) { return HAS_DIED; }

		return HAS_HIT;
	}

	if((Success + Speed + GetSize() - GetAgility()) > 0 && rand() % (Success + Speed + GetSize() - GetAgility()) > 50)
	{
		ushort Damage = ushort(WeaponStrength * Enemy->GetStrength() * (1 + float(Success) / 100) * CalculateArmorModifier() / 2000000) + (rand() % 5 ? 1 : 0);

		if(!Damage)
		{
			if(GetLevelSquareUnder()->CanBeSeen()) Enemy->AddBlockMessage(this);

			SetStrengthExperience(GetStrengthExperience() + 25);
			SetEnduranceExperience(GetEnduranceExperience() + 25);

			return HAS_BLOCKED;
		}
		else
		{
			if(GetLevelSquareUnder()->CanBeSeen()) Enemy->AddHitMessage(this);

			SetHP(GetHP() - Damage);

			if(game::GetWizardMode())
				ADD_MESSAGE("(damage: %d)", Damage);

			if(CheckDeath(std::string("killed by ") + Enemy->Name(INDEFINITE))) { return HAS_DIED; }

			return HAS_HIT;
		}
	}
	else
	{
		if(GetLevelSquareUnder()->CanBeSeen()) Enemy->AddDodgeMessage(this);

		SetAgilityExperience(GetAgilityExperience() + 100);

		return HAS_DODGED;
	}
}

bool ennerbeast::Hit(character*)
{
	char Message[256] ;
	if(rand() % 2) sprintf(Message,"The Enner Beast yells: UGH UGHAaaa!");
	else sprintf(Message, "The Enner Beast yells: Uga Ugar Ugade Ugat!");

	DO_FILLED_RECTANGLE(GetPos().X, GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, 100,
	                    {
					character* Char = game::GetCurrentLevel()->GetLevelSquare(vector(XPointer, YPointer))->GetCharacter();

					if(Char && Char != this)
						Char->ReceiveSound(Message, rand() % 26 - rand() % 26, GetMeleeStrength() * GetStrength() / GetHypotSquare<float>(float(GetPos().X) - XPointer, float(GetPos().Y) - YPointer));
	                    });

	SetStrengthExperience(GetStrengthExperience() + 100);

	return true;
}

void character::Act(void)
{
	if(!GetHasActed())
	{
		if(!game::Flag)
		{
			ApplyExperience();
			if(GetHP() < GetEndurance()) SpillBlood(rand() % 2);
			switch(GetBurdenState())
			{
			case UNBURDENED:
				SetAP(GetAP() + 100 + (GetAgility() - 10) / 2);
			break;
			case BURDENED:
				SetAP(GetAP() + 50 + (GetAgility() - 10) / 4);
			break;
			case STRESSED:
			case OVERLOADED:
				SetAP(GetAP() + 33 + (GetAgility() - 10) / 6);
			break;
			}
			if(GetConsumingCurrently() != 0xFFFF) ContinueEating();

			if(GetNP() < CRITICALHUNGERLEVEL && !(rand() % 50) && !GetFainted() && GetIsPlayer())
			{
				ADD_MESSAGE("You faint.");
				SetAP(GetAP() - 7000 + rand() % 10000);
				SetStrengthExperience(GetStrengthExperience() - 100);
				SetFainted(true);
			}
		}
		else
			game::Flag = false;

		if(GetAP() >= 1000)
		{
			SetConsumingCurrently(0xFFFF);
			if(GetFainted()) SetFainted(false);

			if(GetIsPlayer())
			{
				static ushort Timer = 0;
				if(Timer++ == 20)
				{
					game::Save(game::GetAutoSaveFileName().c_str());
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

			SetAP(GetAP() - 1000);
		}
	}

	SetHasActed(true);
}


bool character::GoUp(void)
{
	if(GetLevelSquareUnder()->GetOverLevelTerrain()->GoUp(this))
	{
		SetStrengthExperience(GetStrengthExperience() + 25);
		SetNP(GetNP() - 2);
		return true;
	}
	else
		return false;
}

bool character::GoDown(void)
{
	if(!game::GetInWilderness())
	{
		if(GetLevelSquareUnder()->GetOverLevelTerrain()->GoDown(this))
		{
			SetAgilityExperience(GetAgilityExperience() + 25);
			SetNP(GetNP() - 1);
			return true;
		}
		else
			return false;
	}
	else	//gum solution
	{
		game::GetCurrentArea()->RemoveCharacter(GetPos());
		game::SetInWilderness(false);
		game::SetCurrent(0);
		delete game::GetWorldMap();
		game::SetWorldMap(0);
		game::LoadLevel();
		game::GetCurrentLevel()->PutPlayerAround(game::GetCurrentLevel()->GetUpStairs());
		game::GetCurrentLevel()->Luxify();
		game::GetCurrentLevel()->UpdateLOS();
		return true;	
	}
}

bool character::Open(void)
{
	ADD_MESSAGE("Where is this famous door you wish to open? Press i for inventory.");

	DRAW_MESSAGES();

	EMPTY_MESSAGES();

	graphics::BlitDBToScreen();

	while(true)
	{
		int Key = GETKEY();

		if(Key == 'i') 
			return OpenItem();

		if(Key == 0x1B)
			return false;

		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::GetMoveCommandKey(c))
				return OpenPos(GetPos() + game::GetMoveVector(c));
	}
}

bool character::Close(void)
{
	ADD_MESSAGE("Where is this door you wish to close?");

	DRAW_MESSAGES();

	EMPTY_MESSAGES();

	graphics::BlitDBToScreen();

	while(true)
	{
		int Key = GETKEY();

		if(Key == 0x1B)
			return false;

		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::GetMoveCommandKey(c))
				if(game::GetCurrentLevel()->GetLevelSquare(GetPos() + game::GetMoveVector(c))->Close(this))
				{
					SetAgilityExperience(GetAgilityExperience() + 25);
					SetNP(GetNP() - 1);
					return true;
				}
				else
					return false;
	}
}

bool character::Drop(void)
{
	ushort Index = GetStack()->DrawContents("What do you want to drop?");

	if(Index < GetStack()->GetItems() && GetStack()->GetItem(Index))
		if(GetStack()->GetItem(Index) == GetWielded())
			ADD_MESSAGE("You can't drop something you wield!");
		else
		{
			GetStack()->MoveItem(Index, GetLevelSquareUnder()->GetStack());

			return true;
		}

        return false;
}

bool humanoid::Drop(void)
{
	ushort Index = GetStack()->DrawContents("What do you want to drop?");

	if(Index < GetStack()->GetItems() && GetStack()->GetItem(Index))
		if(GetStack()->GetItem(Index) == GetWielded())
			ADD_MESSAGE("You can't drop something you wield!");
		else if(GetStack()->GetItem(Index) == Armor.Torso)
			ADD_MESSAGE("You can't drop something you wear!");
		else
		{
			GetStack()->MoveItem(Index, GetLevelSquareUnder()->GetStack());

			return true;
		}

	return false;
}

bool character::Consume(void)
{
	if(!game::GetInWilderness() && GetLevelSquareUnder()->GetStack()->ConsumableItems(this) && game::BoolQuestion("Do you wish to consume one of the items lying on the ground? [Y/N]"))
	{
		ushort Index = GetLevelSquareUnder()->GetStack()->DrawConsumableContents("What do you wish to consume?", this);
		if(Index < GetLevelSquareUnder()->GetStack()->GetItems())
		{
			if(CheckBulimia())
				if(!game::BoolQuestion("You think your stomach will burst if you eat anything more. Force it down? (Y/N)"))
					return false;

			if(ConsumeItem(Index, GetLevelSquareUnder()->GetStack()))
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

	if(GetStack()->ConsumableItems(this))
	{
		ushort Index = GetStack()->DrawConsumableContents("What do you wish to consume?", this);
		if(Index < GetStack()->GetItems())
		{
			if(!CheckIfConsumable(Index))
				return false;

			if(CheckBulimia())
				if(!game::BoolQuestion("You think your stomach will burst if you eat anything more. Force it down? (Y/N)"))
					return false;


			if(ConsumeItem(GetStack()->MoveItem(Index, GetLevelSquareUnder()->GetStack()), GetLevelSquareUnder()->GetStack()))
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

bool character::CheckBulimia(void) const
{
	return GetNP() > (GetSize() << 5) ? true : false;
}

void character::ReceiveBulimiaDamage(void)
{
	if((GetNP() - (GetSize() << 5)) / 50 > 0)
	{
		ADD_MESSAGE("Urgh... Your stomach hurts.");

		SetHP(GetHP() - (GetNP() - (GetSize() << 5)) / 50);

		CheckDeath("died of bulimia");
	}
}

bool character::CheckIfConsumable(ushort Index) const
{
	return (GetTorsoArmor() != GetStack()->GetItem(Index) && GetWielded() != GetStack()->GetItem(Index));
}

bool character::ConsumeItem(int ToBeEaten, stack* ItemsStack)
{
	ushort ItemNumber;
	if(ConsumeItemType(ItemsStack->GetItem(ToBeEaten)->GetConsumeType()))
	{
		if((ItemNumber = ItemsStack->GetItem(ToBeEaten)->PrepareForConsuming(this, ItemsStack)) != 0xFFFF)
		{
			SetConsumingCurrently(ItemsStack->MoveItem(ItemNumber, GetLevelSquareUnder()->GetStack()));
			APsToBeEaten = ItemsStack->GetItem(ToBeEaten)->GetWeight();
			SetAP(1000 - APsToBeEaten);
			return true;
		}
	}

	if(GetIsPlayer())
		ADD_MESSAGE("You can't consume this.");

	return false;

}

bool character::ConsumeItemType(uchar Type) const
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
		game::GetCurrentArea()->RemoveCharacter(GetPos());

		game::GetCurrentArea()->AddCharacter(MoveTo, this);

		if(GetIsPlayer())
		{
			if(GetPos().X < game::GetCamera().X + 2 || GetPos().X > game::GetCamera().X + 48)
				game::UpDateCameraX();

			if(GetPos().Y < game::GetCamera().Y + 2 || GetPos().Y > game::GetCamera().Y + 27)
				game::UpDateCameraY();

			game::GetCurrentArea()->UpdateLOS();

			if(!game::GetInWilderness() && game::GetCurrentLevel()->GetLevelSquare(GetPos())->GetEngraved() != "")
				ADD_MESSAGE("Something has been engraved here: \"%s\"", game::GetCurrentLevel()->GetLevelSquare(GetPos())->GetEngraved().c_str());
		}

		SetNP(GetNP() - 1);
		SetAgilityExperience(GetAgilityExperience() + 10);

		if(!game::GetInWilderness() && GetIsPlayer() && GetLevelSquareUnder()->GetStack()->GetItems() > 0)
		{
			if (GetLevelSquareUnder()->GetStack()->GetItems() > 1)
			ADD_MESSAGE("Several items are lying here.");
			else	ADD_MESSAGE("%s is lying here.", GetLevelSquareUnder()->GetStack()->GetItem(0)->CNAME(INDEFINITE));
		}
	}
}

void character::DrawToTileBuffer(void) const
{
	igraph::GetCharacterGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X, (Material[0]->GetFleshColor()) << 4, 0, 0, 16, 16);

	if(GetIsPlayer())
		igraph::GetCharacterGraphic()->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

void golem::DrawToTileBuffer(void) const
{
	igraph::GetCharacterGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X, (Material[0]->GetItemColor()) << 4, 0, 0, 16, 16);

	if(GetIsPlayer())
		igraph::GetCharacterGraphic()->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

void humanoid::DrawToTileBuffer(void) const
{	
	vector InHandsPic, ArmPos, HeadPos;

	if(GetArmType() > 16)
	{
		ArmPos.X = 80;
		ArmPos.Y = (GetArmType() - 16) * 16;
	}
	else
	{
		ArmPos.X = 64;
		ArmPos.Y = GetArmType() * 16;
	}
	if(GetHeadType() > 16)
	{
		HeadPos.X = 112;
		HeadPos.Y = (GetHeadType() - 16) * 16;
	}
	else
	{
		HeadPos.X = 96;
		HeadPos.Y = GetHeadType() * 16;
	}


	if(GetWielded() != 0) InHandsPic = GetWielded()->GetInHandsPic();

	igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16); // Legs
	igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), 32, 0, 0, 0, 16, 16); // Torso
	igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), ArmPos.X, ArmPos.Y, 0, 0, 16, 16); // Arms
	igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), HeadPos.X, HeadPos.Y, 0, 0, 16, 16); // Head

	if(GetWielded() != 0)
		if(InHandsPic.X != 0 || InHandsPic.Y != 0) igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), InHandsPic.X , InHandsPic.Y, 0, 0, 16, 16); // Wielded

	if(GetIsPlayer())
		igraph::GetCharacterGraphic()->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

bool character::Wield(void)
{
	ushort Index;
	if((Index = GetStack()->DrawContents("What do you want to wield? or press '-' for hands")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to wield.");
		return false;
	}
	if(Index == 0xFFFE)
		SetWielded(0);
	else
		if(Index < GetStack()->GetItems())
			SetWielded(GetStack()->GetItem(Index));
		else
			return false;

	return true;
}

bool humanoid::Wield(void)
{
	ushort Index;
	if((Index = GetStack()->DrawContents("What do you want to wield? or press '-' for hands")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to wield.");
		return false;
	}

	if(Index == 0xFFFE)
		SetWielded(0);
	else
		if(Index < GetStack()->GetItems())
	{
	if(GetStack()->GetItem(Index) != Armor.Torso)
		SetWielded(GetStack()->GetItem(Index));
	else ADD_MESSAGE("You can't wield something that you wear!");
	}

		else
			return false;

	return true;
	
}

void character::GetAICommand(void) // Freedom is slavery. Love is hate. War is peace.
				   // Shouldn't it be "Ignorance is strength", not "Love is hate"?
{
	switch(GetRelations())
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
	vector TPos = Target->GetPos();

	vector MoveTo[3];

	if(TPos.X < GetPos().X)
	{
		if(TPos.Y < GetPos().Y)
		{
			MoveTo[0] = vector(-1, -1);
			MoveTo[1] = vector(-1,  0);
			MoveTo[2] = vector( 0, -1);
		}

		if(TPos.Y == GetPos().Y)
		{
			MoveTo[0] = vector(-1,  0);
			MoveTo[1] = vector(-1, -1);
			MoveTo[2] = vector(-1,  1);
		}

		if(TPos.Y > GetPos().Y)
		{
			MoveTo[0] = vector(-1, 1);
			MoveTo[1] = vector(-1, 0);
			MoveTo[2] = vector( 0, 1);
		}
	}

	if(TPos.X == GetPos().X)
	{
		if(TPos.Y < GetPos().Y)
		{
			MoveTo[0] = vector( 0, -1);
			MoveTo[1] = vector(-1, -1);
			MoveTo[2] = vector( 1, -1);
		}

		if(TPos.Y == GetPos().Y)	//???
			return;

		if(TPos.Y > GetPos().Y)
		{
			MoveTo[0] = vector( 0, 1);
			MoveTo[1] = vector(-1, 1);
			MoveTo[2] = vector( 1, 1);
		}
	}

	if(TPos.X > GetPos().X)
	{
		if(TPos.Y < GetPos().Y)
		{
			MoveTo[0] = vector(1, -1);
			MoveTo[1] = vector(1,  0);
			MoveTo[2] = vector(0, -1);
		}

		if(TPos.Y == GetPos().Y)
		{
			MoveTo[0] = vector(1,  0);
			MoveTo[1] = vector(1, -1);
			MoveTo[2] = vector(1,  1);
		}

		if(TPos.Y > GetPos().Y)
		{
			MoveTo[0] = vector(1, 1);
			MoveTo[1] = vector(1, 0);
			MoveTo[2] = vector(0, 1);
		}
	}

	if(TryMove(GetPos() + MoveTo[0])) return;

	if(rand() % 2)
	{
		if(TryMove(GetPos() + MoveTo[1])) return;
		if(TryMove(GetPos() + MoveTo[2])) return;
	}
	else
	{
		if(TryMove(GetPos() + MoveTo[2])) return;
		if(TryMove(GetPos() + MoveTo[1])) return;
	}
}

bool character::TryMove(vector MoveTo)
{
	if(!game::GetInWilderness())
		if(MoveTo.X < game::GetCurrentLevel()->GetXSize() && MoveTo.Y < game::GetCurrentLevel()->GetYSize())
			if(game::GetCurrentLevel()->GetLevelSquare(MoveTo)->GetCharacter())
				if(GetIsPlayer() || (!GetRelations() && game::GetCurrentLevel()->GetLevelSquare(MoveTo)->GetCharacter()->GetRelations() > 0) || (GetRelations() > 0 && !game::GetCurrentLevel()->GetLevelSquare(MoveTo)->GetCharacter()->GetRelations()))
					return Hit(game::GetCurrentLevel()->GetLevelSquare(MoveTo)->GetCharacter());
				else
					return false;
			else
				if(game::GetCurrentLevel()->GetLevelSquare(MoveTo)->GetOverLevelTerrain()->GetIsWalkable() || (game::GetGoThroughWallsCheat() && GetIsPlayer()))
				{
					Move(MoveTo);
					return true;
				}
				else
					if(GetIsPlayer() && game::GetCurrentLevel()->GetLevelSquare(MoveTo)->GetOverLevelTerrain()->CanBeOpened())
						if(game::BoolQuestion("Do you want to open this door? [Y/N]", false, game::GetMoveCommandKey(game::GetPlayer()->GetPos(), MoveTo)))
						{
							OpenPos(MoveTo);
							return true;
						}
						else
							return false;
					else
						return false;
		else
			return false;
	else
		if(MoveTo.X < game::GetWorldMap()->GetXSize() && MoveTo.Y < game::GetWorldMap()->GetYSize())
			if(true || (game::GetGoThroughWallsCheat() && GetIsPlayer())) //GGG
			{
				Move(MoveTo);
				return true;
			}
			else
				return false;
		else
			return false;
	
}

bool character::ShowInventory(void)
{
	GetStack()->DrawContents("Character's Inventory, press ESC to exit");

	return false;
}

bool character::PickUp(void)
{
	bool ToBeReturned = false;

	if (GetLevelSquareUnder()->GetStack()->GetItems() > 0)
	{
		if (GetLevelSquareUnder()->GetStack()->GetItems() > 1)
		{
			ushort Index;

			bitmap Backup(800, 600);

			DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, 800, 600);

			for(;;)
			{
				Index = GetLevelSquareUnder()->GetStack()->DrawContents("What do you want to pick up?");

				if(Index < GetLevelSquareUnder()->GetStack()->GetItems())
					if(GetLevelSquareUnder()->GetStack()->GetItem(Index))
					{
						ADD_MESSAGE("%s picked up.", GetLevelSquareUnder()->GetStack()->GetItem(Index)->CNAME(INDEFINITE));
						GetLevelSquareUnder()->GetStack()->MoveItem(Index, GetStack());
						ToBeReturned = true;
					}

				if(Index == 0xFFFD || !GetLevelSquareUnder()->GetStack()->GetItems())
					break;

				Backup.Blit(DOUBLEBUFFER, 0, 0, 0, 0, 800, 600);
			}
		}
		else
		{
			ADD_MESSAGE("%s picked up.", GetLevelSquareUnder()->GetStack()->GetItem(0)->CNAME(INDEFINITE));
			GetLevelSquareUnder()->GetStack()->MoveItem(0, GetStack());
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

		if(!game::GetWizardMode())
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

void character::CreateCorpse(void)
{
	GetLevelSquareUnder()->GetStack()->AddItem(new corpse(GetMaterial(0)));

	SetMaterial(0, 0);
}

void character::Die(void)
{
	if(Dead)
		return;

	if(GetIsPlayer())
	{
		ADD_MESSAGE("You die.");

		if(game::GetWizardMode())
		{
			game::DrawEverything(false);

			if(!game::BoolQuestion("Do you want to do this, cheater? [Y/N]", 2, 'y'))
			{
				SetHP(Endurance << 1);
				SetNP(1000);
				return;
			}
		}
	}
	else
		if(GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE(DeathMessage().c_str());

	Dead = true;

	while(GetStack()->GetItems())
		GetStack()->MoveItem(0, GetLevelSquareUnder()->GetStack());
	
	GetLevelSquareUnder()->RemoveCharacter();

	CreateCorpse();

	game::SendToHell(this);

	if(GetIsPlayer())
	{
		game::Quit();

		if(!game::GetWizardMode())
		{
			game::DrawEverything();

			GETKEY();
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

	if(Index < GetStack()->GetItems())
		if(GetStack()->GetItem(Index)->TryToOpen(Stack) != 0xFFFF)
		{
			SetAgilityExperience(GetAgilityExperience() + 25);
			SetNP(GetNP() - 1);
			return true;
		}
		else
		{
			ADD_MESSAGE("You can't open %s.", GetStack()->GetItem(Index)->CNAME(DEFINITE));
			return false;
		}

	return false;
}

void character::Regenerate(ushort Turns)
{
	SetRegenerationCounter(CRegenerationCounter() + GetEndurance() * Turns);

	while(CRegenerationCounter() > 100)
	{
		if(GetHP() < (GetEndurance() << 1))
		{
			SetHP(GetHP() + 1);
			SetEnduranceExperience(GetEnduranceExperience() + 100);
		}

		SetRegenerationCounter(CRegenerationCounter() - 100);
	}
}

void fallenvalpurist::CreateCorpse(void)
{
	ushort Amount = 2 + rand() % 4;

	for(ushort c = 0; c < Amount; c++)
		GetLevelSquareUnder()->GetStack()->AddItem(new abone);
}

void elpuri::CreateCorpse(void)
{
	character::CreateCorpse();

	GetLevelSquareUnder()->GetStack()->AddItem(new headofelpuri);
}

void perttu::CreateCorpse(void)
{
	character::CreateCorpse();

	GetLevelSquareUnder()->GetStack()->AddItem(new leftnutofperttu);
}

bool humanoid::WearArmor(void)
{
	ushort Index;

	if((Index = GetStack()->DrawContents("What do you want to wear? or press '-' for nothing")) == 0xFFFF)
		return false;

	if(Index == 0xFFFE)
	{
		Armor.Torso = 0;
		return true;
	}
	else
		if(Index < GetStack()->GetItems())		// Other Armor types should be coded...
		{
			if(GetStack()->GetItem(Index)->CanBeWorn())
			{
				if(GetStack()->GetItem(Index) != GetWielded())
				{
					Armor.Torso = GetStack()->GetItem(Index);
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

void character::AddBlockMessage(character* Enemy) const
{
	if(Enemy->GetIsPlayer())
		ADD_MESSAGE("You block %s!", CNAME(DEFINITE));
	else
		if(GetIsPlayer())
			ADD_MESSAGE("%s blocks you!", Enemy->CNAME(DEFINITE));
		else
			if(GetLevelSquareUnder()->RetrieveFlag()) ADD_MESSAGE("%s blocks %s!", Enemy->CNAME(DEFINITE), CNAME(DEFINITE));
}

void character::AddDodgeMessage(character* Enemy) const
{
	if(Enemy->GetIsPlayer())
		ADD_MESSAGE("You dodge %s!", CNAME(DEFINITE));
	else
		if(GetIsPlayer())
			ADD_MESSAGE("%s dodges you!", Enemy->CNAME(DEFINITE));
		else
			if(GetLevelSquareUnder()->CanBeSeen()) ADD_MESSAGE("%s dodges %s!", Enemy->CNAME(DEFINITE), CNAME(DEFINITE));
}

void character::AddHitMessage(character* Enemy, const bool Critical) const
{
	if(Enemy->GetIsPlayer())
		if(GetWielded())
			ADD_MESSAGE("%s %s you with %s %s!", CNAME(DEFINITE), ThirdPersonWeaponHitVerb(Critical).c_str(), game::PossessivePronoun(GetSex()), Wielded->CNAME(0));
		else
			ADD_MESSAGE("%s %s you!", CNAME(DEFINITE), ThirdPersonMeleeHitVerb(Critical).c_str());
	else
		if(GetIsPlayer())
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

		if(!game::GetWizardMode())
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
		if(game::GetGod(1)->GetRelation() >= 500 && Talker->GetDifficulty() >= 2500 && game::BoolQuestion("Perttu smiles. \"Thou areth indeed a great Champion of the Great Frog! Elpuri is not a foe worthy for thee. Dost thou wish to stay in the dungeon for a while more and complete another quest for me?\" (Y/n)", 'y'))
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

		if(!game::GetWizardMode())
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

		DRAW_MESSAGES();

		EMPTY_MESSAGES();

		graphics::BlitDBToScreen();

		bool CorrectKey = false;

		while(!CorrectKey)
		{
			k = GETKEY();

			if(k == 0x1B)
				CorrectKey = true;

			for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
				if(k == game::GetMoveCommandKey(c) || (k ^ 32) == game::GetMoveCommandKey(c))
				{
					if(game::GetCurrentLevel()->GetLevelSquare(GetPos() + game::GetMoveVector(c))->GetCharacter())
					{
						game::GetCurrentLevel()->GetLevelSquare(GetPos() + game::GetMoveVector(c))->GetCharacter()->BeTalkedTo(this);

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

#include <iostream>

bool character::NOP(void)
{
	SetAgilityExperience(GetAgilityExperience() - 5);

	std::cerr << "Kalle!" << std::endl;

	return true;
}

void character::ApplyExperience(void)
{
	if(GetStrengthExperience() > pow(1.18, long(GetStrength())) * 193)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You feel you could lift Bill with one hand!");

		SetStrength(GetStrength() + 1);

		SetStrengthExperience(0);
	}

	if(GetStrengthExperience() < -pow(1.18, long(100 - GetStrength())) * 193)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You collapse under your load.");

		SetStrength(GetStrength() - 1);

		SetStrengthExperience(0);
	}

	if(GetEnduranceExperience() > pow(1.18, long(GetEndurance())) * 193)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You feel Valpuri's toughness around you!");

		SetEndurance(GetEndurance() + 1);

		SetEnduranceExperience(0);
	}

	if(GetEnduranceExperience() < -pow(1.18, long(100 - GetEndurance())) * 193)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You seem as tough as Jari.");

		SetEndurance(GetEndurance() - 1);

		SetEnduranceExperience(0);
	}

	if(GetAgilityExperience() > pow(1.18, long(GetAgility())) * 193)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("Your agility challenges even the Valpuri's angels!");

		SetAgility(GetAgility() + 1);

		SetAgilityExperience(0);
	}

	if(GetAgilityExperience() < -pow(1.18, long(100 - GetAgility())) * 193)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You seem as fast as a flat mommo.");

		SetAgility(GetAgility() - 1);

		SetAgilityExperience(0);
	}

	if(GetPerceptionExperience() > pow(1.18, long(GetPerception())) * 193)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("Your sight seem to sharpen. This is bad. Very bad.");

		SetPerception(GetPerception() + 1);

		SetPerceptionExperience(0);
	}

	if(GetPerceptionExperience() < -pow(1.18, long(100 - GetPerception())) * 193)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You feel very guru.");

		SetPerception(GetPerception() - 1);

		SetPerceptionExperience(0);

		game::GetGod(1)->AdjustRelation(100);
	}
}

bool character::HasHeadOfElpuri(void) const
{
	for(ushort c = 0; c < GetStack()->GetItems(); c++)
		if(GetStack()->GetItem(c)->IsHeadOfElpuri())
			return true;

	return false;
}

bool character::HasPerttusNut(void) const
{
	for(ushort c = 0; c < GetStack()->GetItems(); c++)
		if(GetStack()->GetItem(c)->IsPerttusNut())
			return true;

	return false;
}

bool character::HasMaakotkaShirt(void) const
{
	for(ushort c = 0; c < GetStack()->GetItems(); c++)
		if(GetStack()->GetItem(c)->IsMaakotkaShirt())
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
	ushort Index = GetStack()->DrawContents("What do you want to read?");

	if(Index < GetStack()->GetItems())
		return ReadItem(Index, GetStack());
	else
		return false;
}

bool character::ReadItem(int ToBeRead, stack* ItemsStack)
{
	if(ItemsStack->GetItem(ToBeRead)->CanBeRead(this))
	{
		if(ItemsStack->GetItem(ToBeRead)->Read(this))
			ItemsStack->RemoveItem(ToBeRead);

		return true;
	}
	else
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You can't read this.");

		return false;
	}
}

// 2500 * S Burdened
// 5000 * S Stressed
// 7000 * S Overload

uchar character::GetBurdenState(ulong Mass) const
{
	ulong SumOfMasses;
	if(!Mass)
		SumOfMasses = GetStack()->SumOfMasses();
	else
		SumOfMasses = Mass;
	if(SumOfMasses > ulong(7000 * GetStrength()))
		return OVERLOADED;
	if(SumOfMasses > ulong(5000 * GetStrength()))
		return STRESSED;
	if(SumOfMasses > ulong(2500 * GetStrength()))
		return BURDENED;
	return UNBURDENED;
}

bool character::Dip(void)
{
	ushort What = GetStack()->DrawContents("What do you want to dip?");

	if(What < GetStack()->GetItems() && GetStack()->GetItem(What)->CanBeDipped())
	{
		game::DrawEverything();
		ushort To = GetStack()->DrawContents("In what do you wish to dip it into?");
		if(To < GetStack()->GetItems() && GetStack()->GetItem(To) && GetStack()->GetItem(What) != GetStack()->GetItem(To))
		{
			if(GetStack()->GetItem(To)->CanBeDippedInto(GetStack()->GetItem(What)))
			{
				GetStack()->GetItem(What)->DipInto(GetStack()->GetItem(To));
				return true;
			}
		}
	}
	ADD_MESSAGE("Invalid selection!");
        return false;
}

/*void character::Save(std::ofstream& SaveFile) const
{
	object::Save(SaveFile);

	SaveFile << Stack;
	SaveFile << ushort(Wielded ? Stack->SearchItem(Wielded) : 0xFFFF);
	SaveFile << Strength << Endurance << Agility << Perception;
	SaveFile << RegenerationCounter;
	SaveFile << HP << NP << AP;
	SaveFile << StrengthExperience << EnduranceExperience << AgilityExperience << PerceptionExperience;
	SaveFile << HasActed;
	SaveFile << Relations;
	SaveFile << Fainted, EatingCurrently, APsToBeEaten, Dead;
}

void character::Load(std::ifstream& SaveFile)
{
	object::Load(SaveFile);

	SaveFile >> Stack;

	ushort Index;
	SaveFile >> Index;
	Wielded = Index != 0xFFFF ? Stack->GetItem(Index) : 0;

	SaveFile >> Strength >> Endurance >> Agility >> Perception;
	SaveFile >> RegenerationCounter;
	SaveFile >> HP >> NP >> AP;
	SaveFile >> StrengthExperience >> EnduranceExperience >> AgilityExperience >> PerceptionExperience;
	SaveFile >> HasActed;
	SaveFile >> Relations;
	SaveFile >> Fainted, EatingCurrently, APsToBeEaten, Dead;
}

void humanoid::Save(std::ofstream& SaveFile) const
{
	character::Save(SaveFile);

	SaveFile << ushort(Armor.Torso ? Stack->SearchItem(Armor.Torso) : 0xFFFF);
	SaveFile << ArmType << HeadType << LegType << TorsoType;
}

void humanoid::Load(std::ifstream& SaveFile)
{
	character::Load(SaveFile);

	ushort Index;
	SaveFile >> Index;
	Armor.Torso = Index != 0xFFFF ? Stack->GetItem(Index) : 0;

	SaveFile >> ArmType >> HeadType >> LegType >> TorsoType;
}*/

void character::Save(std::ofstream& SaveFile) const
{
	object::Save(SaveFile);

	Stack->Save(SaveFile);

	ushort Index = Wielded ? Stack->SearchItem(Wielded) : 0xFFFF;

	SaveFile.write((char*)&Index, sizeof(Index));

	SaveFile.write((char*)&Strength, sizeof(Strength));
	SaveFile.write((char*)&Endurance, sizeof(Endurance));
	SaveFile.write((char*)&Agility, sizeof(Agility));
	SaveFile.write((char*)&Perception, sizeof(Perception));
	SaveFile.write((char*)&RegenerationCounter, sizeof(RegenerationCounter));
	SaveFile.write((char*)&HP, sizeof(HP));
	SaveFile.write((char*)&NP, sizeof(NP));
	SaveFile.write((char*)&AP, sizeof(AP));
	SaveFile.write((char*)&StrengthExperience, sizeof(StrengthExperience));
	SaveFile.write((char*)&EnduranceExperience, sizeof(EnduranceExperience));
	SaveFile.write((char*)&AgilityExperience, sizeof(AgilityExperience));
	SaveFile.write((char*)&PerceptionExperience, sizeof(PerceptionExperience));
	SaveFile.write((char*)&HasActed, sizeof(HasActed));
	SaveFile.write((char*)&Relations, sizeof(Relations));
	SaveFile.write((char*)&Fainted, sizeof(Fainted));
	SaveFile.write((char*)&EatingCurrently, sizeof(EatingCurrently));
	SaveFile.write((char*)&APsToBeEaten, sizeof(APsToBeEaten));
	SaveFile.write((char*)&Dead, sizeof(Dead)); // ?
}

void character::Load(std::ifstream& SaveFile)
{
	object::Load(SaveFile);

	Stack = new stack;
	Stack->Load(SaveFile);

	ushort Index;

	SaveFile.read((char*)&Index, sizeof(Index));

	Wielded = Index != 0xFFFF ? Stack->GetItem(Index) : 0;

	SaveFile.read((char*)&Strength, sizeof(Strength));
	SaveFile.read((char*)&Endurance, sizeof(Endurance));
	SaveFile.read((char*)&Agility, sizeof(Agility));
	SaveFile.read((char*)&Perception, sizeof(Perception));
	SaveFile.read((char*)&RegenerationCounter, sizeof(RegenerationCounter));
	SaveFile.read((char*)&HP, sizeof(HP));
	SaveFile.read((char*)&NP, sizeof(NP));
	SaveFile.read((char*)&AP, sizeof(AP));
	SaveFile.read((char*)&StrengthExperience, sizeof(StrengthExperience));
	SaveFile.read((char*)&EnduranceExperience, sizeof(EnduranceExperience));
	SaveFile.read((char*)&AgilityExperience, sizeof(AgilityExperience));
	SaveFile.read((char*)&PerceptionExperience, sizeof(PerceptionExperience));
	SaveFile.read((char*)&HasActed, sizeof(HasActed));
	SaveFile.read((char*)&Relations, sizeof(Relations));
	SaveFile.read((char*)&Fainted, sizeof(Fainted));
	SaveFile.read((char*)&EatingCurrently, sizeof(EatingCurrently));
	SaveFile.read((char*)&APsToBeEaten, sizeof(APsToBeEaten));
	SaveFile.read((char*)&Dead, sizeof(Dead)); // ?
}

void humanoid::Save(std::ofstream& SaveFile) const
{
	character::Save(SaveFile);

	ushort Index = Armor.Torso ? Stack->SearchItem(Armor.Torso) : 0xFFFF;

	SaveFile.write((char*)&Index, sizeof(Index));

	SaveFile.write((char*)&ArmType, sizeof(ArmType));
	SaveFile.write((char*)&HeadType, sizeof(HeadType));
	SaveFile.write((char*)&LegType, sizeof(LegType));
	SaveFile.write((char*)&TorsoType, sizeof(TorsoType));
}

void humanoid::Load(std::ifstream& SaveFile)
{
	character::Load(SaveFile);

	ushort Index;

	SaveFile.read((char*)&Index, sizeof(Index));

	Armor.Torso = Index != 0xFFFF ? Stack->GetItem(Index) : 0;

	SaveFile.read((char*)&ArmType, sizeof(ArmType));
	SaveFile.read((char*)&HeadType, sizeof(HeadType));
	SaveFile.read((char*)&LegType, sizeof(LegType));
	SaveFile.read((char*)&TorsoType, sizeof(TorsoType));
}

bool character::WizardMode(void)
{
	if(!game::GetWizardMode())
	{
		if(game::BoolQuestion("Do you want to cheat, cheater? This action cannot be undone. [Y/N]"))
		{
			game::EnableWizardMode();

			for(ushort x = 0; x < 5; x++)
				GetStack()->AddItem(new scrollofwishing);
		}
	}
	else
		ADD_MESSAGE("You have already activated Wizard Mode. It cannot be toggled off.");
	return false;

}

bool character::RaiseStats(void)
{
	if(game::GetWizardMode())
	{
	Strength += 10;   // I won't touch these just in case we'd do something
	Endurance += 10;  // really odd with GetStrength() etc.
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
	if(game::GetWizardMode())
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
	if(game::GetWizardMode())
		for(ushort c = item::GetProtoIndexBegin(); c < item::GetProtoIndexEnd(); c++)
			Stack->AddItem(GetProtoType<item>(c)->Clone()); //GGG
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");

	return false;
}

bool character::SeeWholeMap(void)
{
	if(game::GetWizardMode())
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

ushort character::GetEmitation(void) const
{
	ushort Emitation = 0;

	for(ushort c = 0; c < GetMaterials(); c++)
		if(Material[c])
			if(Material[c]->GetEmitation() > Emitation)
				Emitation = Material[c]->GetEmitation();

	if(GetStack()->GetEmitation() > Emitation)
		Emitation = GetStack()->GetEmitation();

	return Emitation;
}

void character::SetSquareUnder(square* Square)
{
	typeable::SetSquareUnder(Square);
	Stack->SetSquareUnder(Square);
}

bool character::WalkThroughWalls(void)
{
	if(game::GetWizardMode())
		game::GoThroughWalls();
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");

	return false;
}

bool character::ShowKeyLayout(void)
{
	list List("Keyboard Layout");

	List.AddDescription("");
	List.AddDescription("Key       Description");

	for(uchar c = 1; game::GetCommand(c); c++)
	{
		std::string Buffer;
		Buffer += game::GetCommand(c)->GetKey();
		Buffer.resize(10, ' ');
		List.AddString(Buffer + game::GetCommand(c)->GetDescription());
	}

	List.Draw(false);

	return false;
}

bool character::Look(void)
{
	int Key;
	std::string OldMemory;
	vector CursorPos = GetPos();
	game::DrawEverythingNoBlit();
	FONTW->PrintfToDB(16, 514, "Press direction keys to move cursor or esc to exit from the mode.");
	DRAW_MESSAGES();
	EMPTY_MESSAGES();
	graphics::BlitDBToScreen();

	while(Key != ' ' && Key != 0x1B)
	{


		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::GetMoveCommandKey(c))
				{
				CursorPos += game::GetMoveVector(c);

				if (short(CursorPos.X) > game::GetCurrentLevel()->GetXSize()-1)	CursorPos.X = 0;
				if (short(CursorPos.X) < 0)					CursorPos.X = game::GetCurrentLevel()->GetXSize()-1;
				if (short(CursorPos.Y) > game::GetCurrentLevel()->GetYSize()-1)	CursorPos.Y = 0;
				if (short(CursorPos.Y) < 0)					CursorPos.Y = game::GetCurrentLevel()->GetYSize()-1;
				}

			if(GetIsPlayer())
			{
				if(CursorPos.X < game::GetCamera().X + 2 || CursorPos.X > game::GetCamera().X + 48)
					game::UpdateCameraXWithPos(CursorPos.X);

				if(CursorPos.Y < game::GetCamera().Y + 2 || CursorPos.Y > game::GetCamera().Y + 27)
					game::UpdateCameraYWithPos(CursorPos.Y);

			}

		if(game::GetSeeWholeMapCheat())
		{
			OldMemory = game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetRememberedItems();
			game::GetCurrentLevel()->GetLevelSquare(CursorPos)->UpdateItemMemory();
		}

		if(game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetKnown() || game::GetSeeWholeMapCheat())
		{
			std::string Msg;

			if(game::GetCurrentLevel()->GetLevelSquare(CursorPos)->CanBeSeen() || game::GetSeeWholeMapCheat())
				Msg = "You see here ";
			else
				Msg = "You remember here ";

			bool Anything = false;

			if(game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetOverLevelTerrain()->Name(INDEFINITE) != "an air atmosphere")
			{
				Msg += game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetOverLevelTerrain()->Name(INDEFINITE);
	
				Anything = true;
			}

			if((game::GetSeeWholeMapCheat() || game::GetCurrentLevel()->GetLevelSquare(CursorPos)->CanBeSeen()) && game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetCharacter())
			{
				if(Anything)
					if(game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetRememberedItems() != "")
						Msg += std::string(", ");
					else
						Msg += std::string(" and ");

				Msg += std::string(game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetCharacter()->Name(INDEFINITE));

				Anything = true;
			}

			if(game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetRememberedItems() != "")
			{
				if(Anything)
					Msg += std::string(" and ");

				Msg += std::string(game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetRememberedItems()) + " on " + game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetGroundLevelTerrain()->Name(INDEFINITE);
			}
			else
				if(Anything)
					Msg += std::string(" on ") + game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetGroundLevelTerrain()->Name(INDEFINITE);
				else
					Msg += std::string(game::GetCurrentLevel()->GetLevelSquare(CursorPos)->GetGroundLevelTerrain()->Name(INDEFINITE));

			ADD_MESSAGE("%s.", Msg.c_str());
		}
		else
			ADD_MESSAGE("You have no idea what might lie here.");

		if(game::GetSeeWholeMapCheat())
			game::GetCurrentLevel()->GetLevelSquare(CursorPos)->SetRememberedItems(OldMemory);

		game::DrawEverythingNoBlit();
		igraph::GetCharacterGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, (CursorPos.X - game::GetCamera().X) << 4, (CursorPos.Y - game::GetCamera().Y + 2) << 4, 16, 16);
		FONTW->PrintfToDB(16, 514, "Press direction keys to move cursor or esc to exit from the mode.");
		graphics::BlitDBToScreen();
		EMPTY_MESSAGES();

		Key = GETKEY();
	}
	return false;
}

float golem::GetMeleeStrength(void) const
{
	return 75 * GetMaterial(0)->GetHitValue();
}

float character::GetDifficulty(void) const
{
	return float(GetStrength()) * GetEndurance() * GetAgility() * GetAttackStrength() / (float(CalculateArmorModifier()) * 25000);
}

float character::GetAttackStrength(void) const
{
	return GetWielded() ? GetWielded()->GetWeaponStrength() : GetMeleeStrength();
}

bool character::Engrave(std::string What)
{
	game::GetCurrentLevel()->GetLevelSquare(GetPos())->Engrave(What);

	return true;
}

bool character::WhatToEngrave(void)
{
	game::GetCurrentLevel()->GetLevelSquare(GetPos())->Engrave(game::StringQuestion("What do you want to engrave here?", 50));
	return false;
}

void character::MoveRandomly(void)
{
	ushort ToTry = rand() % 8;

	if(!game::GetCurrentLevel()->GetLevelSquare(GetPos() + game::GetMoveVector(ToTry))->GetCharacter());
		TryMove(GetPos() + game::GetMoveVector(ToTry));
}

ushort golem::CalculateArmorModifier(void) const
{
	if(GetMaterial(0)->GetArmorValue() / 2 > 90)
		return 10;
	else
		return 100 - GetMaterial(0)->GetArmorValue() / 2;
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
	if(!game::GetCurrentLevel()->GetLevelSquare(GetPos() + game::GetMoveVector(ToTry))->GetCharacter())
		TryMove(GetPos() + game::GetMoveVector(ToTry));
}


bool character::TestForPickup(item* ToBeTested) const
{
	if(GetBurdenState(ToBeTested->GetWeight() + GetStack()->SumOfMasses()) != UNBURDENED)
		return false;
	return true;
}

bool character::OpenPos(vector APos)
{
	if(game::GetCurrentLevel()->GetLevelSquare(APos)->Open(this))
	{
		SetAgilityExperience(GetAgilityExperience() + 25);
		SetNP(GetNP() - 1);
		return true;
	}
	return false;
}

bool character::Pray(void)
{
	list Panthenon("To Whom shall thee adress thine prayers?");

	if(!GetLevelSquareUnder()->GetDivineOwner())
		for(ushort c = 1; game::GetGod(c); c++)
			Panthenon.AddString(game::GetGod(c)->CompleteDescription());
	else
		Panthenon.AddString(game::GetGod(GetLevelSquareUnder()->GetDivineOwner())->CompleteDescription());

	ushort Select = Panthenon.Draw();

	if(Select & 0x8000)
		return false;
	else
	{
		if(GetLevelSquareUnder()->GetDivineOwner())
		{
			if(!Select)
			{
				if(game::BoolQuestion("Do you really wish to pray? [Y/N]"))
					game::GetGod(GetLevelSquareUnder()->GetDivineOwner())->Pray();
				else
					return false;
			}
			else
				return false;
		}
		else
		{
			if(game::BoolQuestion("Do you really wish to pray? [Y/N]"))
				game::GetGod(Select+1)->Pray();
			else
				return false;
		}

		return true;
	}
}

void character::SpillBlood(uchar HowMuch)
{
	GetLevelSquareUnder()->SpillFluid(HowMuch, GetBloodColor(),5,40);
}

void character::NeutralAICommand(void)
{
	dynarray<character*> SeenCharacters;

	DO_FILLED_RECTANGLE(GetPos().X, GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, LOSRange(),
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector(XPointer,YPointer))->GetCharacter())
			SeenCharacters.Add(game::GetCurrentLevel()->GetLevelSquare(vector(XPointer, YPointer))->GetCharacter());
	});

	for(ushort c = 0; c < SeenCharacters.Length(); c++)
	{
		if(!SeenCharacters.Access(c)->GetRelations() && SeenCharacters.Access(c)->GetLevelSquareUnder()->CanBeSeenFrom(GetPos()))
		{
			Charge(SeenCharacters.Access(c));
			SetHasActed(true);
			return;
		}
	}

	DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	if(game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->Open(this))
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->CanBeSeen())
		{
			if(GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s opens the door.", CNAME(DEFINITE));
			else
				ADD_MESSAGE("Something opens the door.");
		}

	SetHasActed(true);
	return;
	})

	if(GetLevelSquareUnder()->GetStack()->GetItems())
	{
		ushort ItemToTry = rand() % GetLevelSquareUnder()->GetStack()->GetItems();
		if(TestForPickup(GetLevelSquareUnder()->GetStack()->GetItem(ItemToTry)))
		{
			GetLevelSquareUnder()->GetStack()->MoveItem(ItemToTry, GetStack());
			SetHasActed(true);
		}
		for(ushort c = 0; c < GetStack()->GetItems(); c++)
		{
			if(CanWield())
				if(GetStack()->GetItem(c)->GetWeaponStrength() > GetAttackStrength())
				{
					SetWielded(GetStack()->GetItem(c));
					break;
				}
			if(CanWear() && GetStack()->GetItem(c)->CanBeWorn())
				if(GetStack()->GetItem(c)->GetArmorValue() < CalculateArmorModifier())
				{
					WearItem(GetStack()->GetItem(c));
					break;
				}
			if(GetStack()->GetItem(c)->Consumable(this))
			{
				item* Temp = GetStack()->GetItem(c);
				GetStack()->RemoveItem(c);
				delete Temp;
			}

			for(ushort c = 0; c < GetLevelSquareUnder()->GetStack()->GetItems(); c++)
			{
				if(GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeaponStrength() > GetAttackStrength() && GetBurdenState(GetStack()->SumOfMasses() + GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeight()) == UNBURDENED && CanWield())
				{
					if(GetWielded())
						GetStack()->MoveItem(GetStack()->SearchItem(GetWielded()), GetLevelSquareUnder()->GetStack());

					SetWielded(GetStack()->GetItem(GetLevelSquareUnder()->GetStack()->MoveItem(c, GetStack())));
				}
				if(GetLevelSquareUnder()->GetStack()->GetItem(c)->GetArmorValue() > CalculateArmorModifier() && GetBurdenState(GetStack()->SumOfMasses() + GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeight()) == UNBURDENED && CanWear())
				{
					if(GetTorsoArmor())
						GetStack()->MoveItem(GetStack()->SearchItem(GetTorsoArmor()), GetLevelSquareUnder()->GetStack());

					WearItem(GetStack()->GetItem(GetLevelSquareUnder()->GetStack()->MoveItem(c, GetStack())));
				}
				if(GetLevelSquareUnder()->GetStack()->GetItem(c)->Consumable(this))
				{
					item* Temp = GetLevelSquareUnder()->GetStack()->GetItem(c);
					GetLevelSquareUnder()->GetStack()->RemoveItem(c);
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

	DO_FILLED_RECTANGLE(GetPos().X, GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, LOSRange(),
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector(XPointer,YPointer))->GetCharacter())
			SeenCharacters.Add(game::GetCurrentLevel()->GetLevelSquare(vector(XPointer, YPointer))->GetCharacter());
	});

	ushort NumberFor;

	if((NumberFor = SeenCharacters.Search(game::GetPlayer())) != 0xFFFF && GetLevelSquareUnder()->RetrieveFlag())
	{
		Charge(SeenCharacters.Access(NumberFor));
		SetHasActed(true);
		return;
	}
	else
	{
		for(ushort c = 0; c < SeenCharacters.Length(); c++)
		{
			if(SeenCharacters.Access(c)->GetRelations() > 0 && SeenCharacters.Access(c)->GetLevelSquareUnder()->CanBeSeenFrom(GetPos()))
			{
				Charge(SeenCharacters.Access(c));
				SetHasActed(true);
				return;
			}
		}
	}

	DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	if(game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->Open(this))
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->CanBeSeen())
		{
			if(GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s opens the door.", CNAME(DEFINITE));
			else
				ADD_MESSAGE("Something opens the door.");
		}

	SetHasActed(true);
	return;
	})

	if(GetLevelSquareUnder()->GetStack()->GetItems())
	{
		for(ushort c = 0; c < GetStack()->GetItems(); c++)
		{
			if(CanWield())
				if(GetStack()->GetItem(c)->GetWeaponStrength() > GetAttackStrength())
				{
					SetWielded(GetStack()->GetItem(c));
					return;
				}
			if(CanWear() && GetStack()->GetItem(c)->CanBeWorn())
				if(GetStack()->GetItem(c)->GetArmorValue() < CalculateArmorModifier())
				{
					WearItem(GetStack()->GetItem(c));
					return;
				}
			if(GetStack()->GetItem(c)->Consumable(this))
			{
				item* Temp = GetStack()->GetItem(c);
				GetStack()->RemoveItem(c);
				delete Temp;
				return;
			}

			for(ushort c = 0; c < GetLevelSquareUnder()->GetStack()->GetItems(); c++)
			{
				if(GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeaponStrength() > GetAttackStrength() && GetBurdenState(GetStack()->SumOfMasses() + GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeight()) && CanWield())
				{
					if(GetWielded())
						GetStack()->MoveItem(GetStack()->SearchItem(GetWielded()), GetLevelSquareUnder()->GetStack());

					SetWielded(GetStack()->GetItem(GetLevelSquareUnder()->GetStack()->MoveItem(c, GetStack())));
					return;
				}
				if(GetLevelSquareUnder()->GetStack()->GetItem(c)->GetArmorValue() > CalculateArmorModifier() && GetBurdenState(GetStack()->SumOfMasses() + GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeight()) && CanWear())
				{
					if(GetTorsoArmor())
						GetStack()->MoveItem(GetStack()->SearchItem(GetTorsoArmor()), GetLevelSquareUnder()->GetStack());

					WearItem(GetStack()->GetItem(GetLevelSquareUnder()->GetStack()->MoveItem(c, GetStack())));
					return;
				}
				if(GetLevelSquareUnder()->GetStack()->GetItem(c)->Consumable(this))
				{
					item* Temp = GetLevelSquareUnder()->GetStack()->GetItem(c);
					GetLevelSquareUnder()->GetStack()->RemoveItem(c);
					delete Temp;
					return;
				}
			}
		}
		ushort ItemToTry = rand() % GetLevelSquareUnder()->GetStack()->GetItems();
		if(TestForPickup(GetLevelSquareUnder()->GetStack()->GetItem(ItemToTry)))
		{
			GetLevelSquareUnder()->GetStack()->MoveItem(ItemToTry, GetStack());
			SetHasActed(true);
			return;
		}
	}

	MoveRandomly();
}

void ennerbeast::HostileAICommand(void)
{
	if(GetLevelSquareUnder()->RetrieveFlag() && (float(GetPos().X) - game::GetPlayer()->GetPos().X) * (float(GetPos().X) - game::GetPlayer()->GetPos().X) + (float(GetPos().Y) - game::GetPlayer()->GetPos().Y) * (float(GetPos().Y) - game::GetPlayer()->GetPos().Y) <= LOSRange())
	{
		Charge(game::GetPlayer());

		SetHasActed(true);

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
	SetHealTimer(GetHealTimer() + 1);
	DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	if(game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->GetCharacter())
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->GetCharacter() == game::GetPlayer())
		{
			if(game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->GetCharacter()->GetHP() < (game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->GetCharacter()->GetEndurance() << 1) / 3 && game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->GetCharacter() == game::GetPlayer() && GetHealTimer() > 100)
				HealFully(game::GetPlayer());
		}
		else
			Hit(game::GetCurrentLevel()->GetLevelSquare(vector(DoX, DoY))->GetCharacter());
	})
}

void perttu::HealFully(character* ToBeHealed)
{
	SetHealTimer(0);
	ToBeHealed->SetHP(ToBeHealed->GetEndurance() << 1);
	if(ToBeHealed->GetIsPlayer())
		ADD_MESSAGE("%s heals you fully.", CNAME(DEFINITE));
}

void character::ReceiveSchoolFoodEffect(long)
{
	if(GetIsPlayer())
		ADD_MESSAGE("Yuck! This stuff tastes like vomit and old mousepads.");

	SetHP(GetHP() - 1 - rand() % 5);
	Vomit(2);
	CheckDeath("was poisoned by school food");

	if(!(rand() % 10))
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You gain a little bit of toughness for surviving this stuff.");

		SetEndurance(GetEndurance() + 1 + rand() % 5);
	}
}

void character::ReceiveNutrition(long SizeOfEffect)
{
	SetNP(GetNP() + SizeOfEffect / 1000);
}

void character::ReceiveOmleUrineEffect(long)
{
	if(GetIsPlayer())
		ADD_MESSAGE("You feel a primitive Force coursing through your veins.");

	SetStrength(GetStrength() + 1 + rand() % 5);
	SetHP(GetHP() + 2);
}

void character::ReceivePepsiEffect(long SizeOfEffect)
{
	SetHP(GetHP() - short(sqrt(SizeOfEffect / 20)));
	CheckDeath("was poisoned by pepsi");
	if(short(GetPerception() - short(sqrt(SizeOfEffect / 20))) > 0)
		SetPerception(GetPerception() - short(sqrt(SizeOfEffect / 20)));
	else SetPerception(1);
}

void character::Darkness(long SizeOfEffect)
{
	ushort x = 30 + rand() % SizeOfEffect;
	if(GetIsPlayer())
		ADD_MESSAGE("Arg. You feel the fate of a navastater placed upon you...");
	if(GetStrength() - x / 30 > 1) SetStrength(GetStrength() - x / 30); // Old comment was about eating... This
	else SetStrength(1);                                        // can happen with drinkin, hitting etc.
	if(GetEndurance() - x / 30 > 1) SetEndurance(GetEndurance() - x / 30);
	else SetEndurance(1);
	if(GetAgility() - x / 30 > 1) SetAgility(GetAgility() - x / 30);
	else SetAgility(1);
	if(GetHP() - x / 10 > 1) SetHP(GetHP() - x / 10);
	else SetHP(1);
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
			if(KickPos == game::GetMoveVector(c))
				Direction = c;
		}
		game::GetCurrentLevel()->GetLevelSquare(GetPos() + KickPos)->Kick(GetStrength(), Direction);
		return true;
	}

	return false;
}

void perttu::Save(std::ofstream& SaveFile) const
{
	humanoid::Save(SaveFile);

	SaveFile.write((char*)&HealTimer, sizeof(HealTimer));
}

void perttu::Load(std::ifstream& SaveFile)
{
	humanoid::Load(SaveFile);

	SaveFile.read((char*)&HealTimer, sizeof(HealTimer));
}

/*void perttu::Save(std::ofstream& SaveFile) const
{
	humanoid::Save(SaveFile);

	SaveFile << HealTimer;
}

void perttu::Load(std::ifstream& SaveFile)
{
	humanoid::Load(SaveFile);

	SaveFile >> HealTimer;
}*/

bool character::ScreenShot(void)
{
	bitmap TempDB(800, 600);

	TempDB.ReadFromDB(0,0);

	TempDB.Save("Scrshot.bmp");

	return false;
}

bool character::Offer(void)
{
	if(GetLevelSquareUnder()->GetOverLevelTerrain()->CanBeOffered())
	{
		ushort Index = GetStack()->DrawContents("What do you want to offer?");
		if(Index < GetStack()->GetItems())
		{
			if(GetStack()->GetItem(Index) == GetWielded())
			{
				ADD_MESSAGE("You can't offer something that you wield.");
				return false;
			}
			if(GetStack()->GetItem(Index) == GetTorsoArmor())
			{
				ADD_MESSAGE("You can't offer something that you wear.");
				return false;
			}

			if(game::GetGod(GetLevelSquareUnder()->GetOverLevelTerrain()->GetOwnerGod())->ReceiveOffer(GetStack()->GetItem(Index)))
			{
				item* Temp = GetStack()->GetItem(Index);
				GetStack()->RemoveItem(Index);
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

long character::Score(void) const
{
	long Score = GetHP() * 5 + GetNP() / 10 + GetEndurance() * 30 + (GetStrength() + GetAgility() + GetPerception()) * 40;

	Score += Stack->Score();

	Score += game::GodScore();

	return Score;
}

long character::AddScoreEntry(std::string Description, float Multiplier) const
{
	highscore HScore;

	HScore.Add(long((Score() - game::GetBaseScore()) * Multiplier), game::GetPlayerName() + ", " + Description);

	HScore.Save();

	return 0; //??????????
}

bool character::CheckDeath(std::string Msg)
{
	if(GetHP() < 1)
	{
		if(GetIsPlayer() && !game::GetWizardMode())
			AddScoreEntry(Msg);

		Die();

		return true;
	}
	else
		return false;
}

ulong golem::Danger(void) const
{
	return ulong(GetMeleeStrength() * 100 / (CalculateArmorModifier() * 15));
}

bool character::DrawMessageHistory(void)
{
	globalmessagingsystem::DrawMessageHistory();

	return false;
}

bool character::Throw(void)
{
	ushort Index;
	if((Index = GetStack()->DrawContents("What do you want to throw?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to throw.");
		return false;
	}

	if(Index < GetStack()->GetItems())
	{
		if(GetStack()->GetItem(Index) == GetWielded())
		{
			ADD_MESSAGE("You can't throw something that you wield.");
			return false;
		}
		uchar Answer = game::DirectionQuestion("In what direction do you wish to throw?", 8, false);
		if(Answer == 0xFF)
			return false;
		ThrowItem(Answer, GetStack()->GetItem(Index));

	}
	else
		return false;

	return true;
}

bool humanoid::Throw(void)
{
	ushort Index;
	if((Index = GetStack()->DrawContents("What do you want to throw?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to throw.");
		return false;
	}


	if(Index < GetStack()->GetItems())
	{
		if(GetStack()->GetItem(Index) == GetWielded())
		{
			ADD_MESSAGE("You can't throw something that you wield.");
			return false;
		}
		if(GetStack()->GetItem(Index) == GetTorsoArmor())
		{
			ADD_MESSAGE("You can't throw something that you wear.");
			return false;
		}
		uchar Answer = game::DirectionQuestion("In what direction do you wish to throw it?", 8, false);
		if(Answer == 0xFF)
			return false;
		ThrowItem(Answer, GetStack()->GetItem(Index));
	}
	else
		return false;

	return true;
}

bool character::ThrowItem(uchar Direction, item* ToBeThrown) // Note to C++ people. This is throw in the GAME not the throw in c++!
{
	if(Direction > 7)
		ABORT("Throw in TOO odd direction...");
	return ToBeThrown->Fly(Direction, GetStrength(), GetStack(), GetIsPlayer());
}

void character::HasBeenHitByItem(item* Thingy, float Speed, bool CanBeSeen)
{
	ushort Damage = ushort(Thingy->GetWeaponStrength() * Thingy->GetWeight() * CalculateArmorModifier() * sqrt(Speed) / 2000000000) + (rand() % 5 ? 1 : 0);

	SetHP(GetHP() - Damage);

	if(CanBeSeen && !GetIsPlayer())
	{
		ADD_MESSAGE("%s hits %s.", Thingy->CNAME(DEFINITE), CNAME(DEFINITE));

		if(game::GetWizardMode())
			ADD_MESSAGE("(damage: %d) (speed: %f)", Damage, Speed);
	}

	if(GetIsPlayer())
		ADD_MESSAGE("%s hits you.", Thingy->CNAME(DEFINITE));

	SpillBlood(1 + rand() % 1);
	CheckDeath(std::string("died by thrown ") + Thingy->CNAME(INDEFINITE) );
}

bool dog::Catches(item* Thingy, float, bool CanBeSeen)
{
	if(Thingy->DogWillCatchAndConsume())
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CNAME(DEFINITE));
		else
			if(CanBeSeen) ADD_MESSAGE("%s catches %s and eats it.", CNAME(DEFINITE), Thingy->CNAME(DEFINITE));
		ConsumeItem(GetLevelSquareUnder()->GetStack()->SearchItem(Thingy), GetLevelSquareUnder()->GetStack());
		return true;
	}
	return false;
}

bool dog::ConsumeItemType(uchar Type) const     // We need a better system for this... Writing this to every F***ing character that needs one
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
	if(!(rand() % 20))
		return rand() % 2 ? true : false;

	if(rand() % ulong(sqrt(Speed) * GetSize() / GetAgility() * 10 + 1) > 40)
		return false;
	else
		return true;
}

void character::ReceiveFireDamage(long SizeOfEffect)
{
	if(GetIsPlayer())
		ADD_MESSAGE("You burn.");

	SetHP(GetHP() - (rand() % SizeOfEffect + SizeOfEffect));
}

void character::GetPlayerCommand(void)
{
	while(!GetHasActed())
	{
		game::DrawEverything();

		int Key = GETKEY();

		bool ValidKeyPressed = false;

		{
		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::GetMoveCommandKey(c))
			{
				HasActed = TryMove(GetPos() + game::GetMoveVector(c));
				ValidKeyPressed = true;
			}
		}

		for(uchar c = 1; game::GetCommand(c); c++)
			if(Key == game::GetCommand(c)->GetKey())
			{
				if(game::GetInWilderness() && !game::GetCommand(c)->GetCanBeUsedInWilderness())
					ADD_MESSAGE("This function cannot be used while in wilderness.");
				else
					HasActed = (this->*game::GetCommand(c)->GetLinkedFunction())();

				ValidKeyPressed = true;
			}

		if (!ValidKeyPressed)
			ADD_MESSAGE("Unknown key, you %s. Press '?' for a list of commands.", game::Insult());
	}
}

void character::ContinueEating(void)
{
	if(GetAPsToBeEaten() + GetAP() > 50000)
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You have eaten for a long time now...");

		StopEating();
	}
	else if(GetAP() >= 1000)
	{
		if(GetLevelSquareUnder()->GetStack()->GetItem(GetConsumingCurrently())->Consume(this))
			delete GetLevelSquareUnder()->GetStack()->RemoveItem(GetConsumingCurrently());

		SetConsumingCurrently(0xFFFF);

		Hunger(ushort(GetAPsToBeEaten() / 1000));
		Regenerate(ushort(GetAPsToBeEaten() / 1000));
		game::Turn(ushort(GetAPsToBeEaten() / 1000));
		game::ApplyDivineTick(ushort(GetAPsToBeEaten() / 1000));
	}
}

void character::StopEating(void)
{
	float NowEaten = GetAPsToBeEaten() + GetAP();
	float Temp = (NowEaten / GetAPsToBeEaten()) * 100;
	SetAP(1000);
	if(GetStack()->GetLevelSquareUnder()->GetStack()->GetItem(GetConsumingCurrently())->Consume(this, Temp))
		delete GetLevelSquareUnder()->GetStack()->RemoveItem(GetConsumingCurrently());
	Hunger(ushort(NowEaten / 1000));
	Regenerate(ushort(NowEaten / 1000));
	game::Turn(ushort(NowEaten / 1000));
	game::ApplyDivineTick(ushort(NowEaten / 1000));
	SetConsumingCurrently(0xFFFF);
}

void character::Vomit(ushort HowMuch)
{
	if(GetIsPlayer())
		ADD_MESSAGE("You vomit.");
	else
		if(GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s vomits.", CNAME(DEFINITE));
	SetNP(GetNP() - 100);
	GetLevelSquareUnder()->SpillFluid(HowMuch, MAKE_RGB(10,230,10),3,50);
}

bool character::Apply(void)
{
	ushort Index;
	if((Index = GetStack()->DrawContents("What do you want to apply?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to apply.");
		return false;
	}

	if(Index < GetStack()->GetItems())
	{
		if(GetStack()->GetItem(Index) == GetWielded())
		{
			ADD_MESSAGE("You can't throw something that you wield.");
			return false;
		}

		if(!GetStack()->GetItem(Index)->Apply(this))
			return false;
	}
	else
		return false;

	return true;
}


bool humanoid::Apply(void)
{
	ushort Index;
	if((Index = GetStack()->DrawContents("What do you want to apply?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to apply.");
		return false;
	}


	if(Index < GetStack()->GetItems())
	{
		if(GetStack()->GetItem(Index) == GetWielded())
		{
			ADD_MESSAGE("You can't apply something that you wield.");
			return false;
		}
		if(GetStack()->GetItem(Index) == GetTorsoArmor())
		{
			ADD_MESSAGE("You can't apply something that you wear.");
			return false;
		}
		
		if(!GetStack()->GetItem(Index)->Apply(this))
			return false;
	}
	else
		return false;

	return true;
}

vector character::GetPos(void) const
{
	return SquareUnder->GetPos();
}

bool character::ForceVomit(void)
{
	ushort Amount = rand() % 3;
	ADD_MESSAGE("You push your fingers down to your throat.");
	if(Amount) 
		Vomit(rand() % 3);
	else
		ADD_MESSAGE("You are not able to vomit.");
	SetAP(GetAP() - 100);
	return true;
}

bool character::Zap(void)
{
	ushort Index;
	if((Index = GetStack()->DrawContents("What do you want to zap with?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to zap with.");
		return false;
	}

	if(Index < GetStack()->GetItems())
	{
		uchar Answer = game::DirectionQuestion("In what direction do you wish to zap?", 8, false);
		if(Answer == 0xFF)
			return false;
		return GetStack()->GetItem(Index)->Zap(GetPos(), Answer);
	}
	else
		return false;

	return true;	
}

bool character::Polymorph(void)
{
	GetSquareUnder()->AddCharacter(prototypesystem::BalancedCreateMonster());

	delete this;
	return true;
}
