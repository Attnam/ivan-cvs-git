#include <cmath>

#include "charba.h"
#include "stack.h"
#include "error.h"
#include "message.h"
#include "itemde.h"
#include "lsquare.h"
#include "lterraba.h"
#include "wsquare.h"
#include "wterraba.h"
#include "dungeon.h"
#include "whandler.h"
#include "level.h"
#include "worldmap.h"
#include "graphics.h"
#include "igraph.h"
#include "hscore.h"
#include "feio.h"
#include "god.h"
#include "list.h"

character::character(bool CreateMaterials, bool SetStats, bool CreateEquipment, bool AddToPool) : object(AddToPool), Stack(new stack), Wielded(0), RegenerationCounter(0), NP(1000), AP(0), StrengthExperience(0), EnduranceExperience(0), AgilityExperience(0), PerceptionExperience(0), Relations(0), IsPlayer(false)
{
	SetConsumingCurrently(0xFFFF);

	if(CreateMaterials || SetStats || CreateEquipment)
		ABORT("BOOO!");
}

character::~character()
{
	delete Stack;
}

void character::ReceiveSound(char* Pointer, short Success, float ScreamStrength)
{
	if(GetIsPlayer())
		ADD_MESSAGE(Pointer);

	ushort Damage = ushort(ScreamStrength * (1 + float(Success) / 100) / 20000);

	SetHP(HP - Damage);
	GetStack()->ReceiveSound(ScreamStrength);
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

ushort character::CalculateArmorModifier() const
{
	return 100;
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
			if(GetLevelSquareUnder()->CanBeSeen())
			{
				Enemy->AddHitMessage(this);

				if(game::GetWizardMode())
					ADD_MESSAGE("(damage: %d)", Damage);
			}

			SetHP(GetHP() - Damage);

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

void character::Be()
{
	if(game::GetPlayerBackup() != this)
	{
		if(game::GetPlayer() == this) 
		{
			if(!game::GetPolymorphCounter())
			{
				ChangeBackToPlayer();
				game::SetPolymorphCounter(0xFFFF);
				return;
			}
			else if(game::GetPolymorphCounter() != 0xFFFF)
				game::SetPolymorphCounter(game::GetPolymorphCounter() - 1);
		}
			
		if(!game::Flag)
		{
			ApplyExperience();
			if(GetHP() < GetEndurance()) SpillBlood(rand() % 2);
			switch(GetBurdenState())
			{
			case UNBURDENED:
				SetAP(GetAP() + 100 + (GetAgility() >> 1));
			break;
			case BURDENED:
				SetAP(GetAP() + 75 + (GetAgility() >> 1) - (GetAgility() >> 2));
			break;
			case STRESSED:
			case OVERLOADED:
				SetAP(GetAP() + 50 + (GetAgility() >> 2));
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
}


bool character::GoUp()
{
	if(GetSquareUnder()->GetOverTerrain()->GoUp(this))
	{
		SetStrengthExperience(GetStrengthExperience() + 25);
		SetNP(GetNP() - 2);
		return true;
	}
	else
		return false;
}

bool character::GoDown()
{
	if(GetSquareUnder()->GetOverTerrain()->GoDown(this))
	{
		SetAgilityExperience(GetAgilityExperience() + 25);
		SetNP(GetNP() - 1);
		return true;
	}
	else
		return false;
}

bool character::Open()
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

		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
			if(Key == game::GetMoveCommandKey(c))
				return OpenPos(GetPos() + game::GetMoveVector(c));
	}
}

bool character::Close()
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

		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
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

bool character::Drop()
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

bool character::Consume()
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

bool character::CheckBulimia() const
{
	return GetNP() > (GetSize() << 5) ? true : false;
}

void character::ReceiveBulimiaDamage()
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

void character::Move(vector2d MoveTo, bool TeleportMove)
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

void character::DrawToTileBuffer() const
{
	igraph::GetCharacterGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X, (Material[0]->GetFleshColor()) << 4, 0, 0, 16, 16);
}

bool character::Wield()
{
	ushort Index;
	if(!CanWield())
	{
		ADD_MESSAGE("This monster can not wield anything.");
		return false;
	}
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

void character::GetAICommand() // Freedom is slavery. Love is hate. War is peace.
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
	vector2d TPos = Target->GetPos();

	vector2d MoveTo[3];

	if(TPos.X < GetPos().X)
	{
		if(TPos.Y < GetPos().Y)
		{
			MoveTo[0] = vector2d(-1, -1);
			MoveTo[1] = vector2d(-1,  0);
			MoveTo[2] = vector2d( 0, -1);
		}

		if(TPos.Y == GetPos().Y)
		{
			MoveTo[0] = vector2d(-1,  0);
			MoveTo[1] = vector2d(-1, -1);
			MoveTo[2] = vector2d(-1,  1);
		}

		if(TPos.Y > GetPos().Y)
		{
			MoveTo[0] = vector2d(-1, 1);
			MoveTo[1] = vector2d(-1, 0);
			MoveTo[2] = vector2d( 0, 1);
		}
	}

	if(TPos.X == GetPos().X)
	{
		if(TPos.Y < GetPos().Y)
		{
			MoveTo[0] = vector2d( 0, -1);
			MoveTo[1] = vector2d(-1, -1);
			MoveTo[2] = vector2d( 1, -1);
		}

		if(TPos.Y == GetPos().Y)	//???
			return;

		if(TPos.Y > GetPos().Y)
		{
			MoveTo[0] = vector2d( 0, 1);
			MoveTo[1] = vector2d(-1, 1);
			MoveTo[2] = vector2d( 1, 1);
		}
	}

	if(TPos.X > GetPos().X)
	{
		if(TPos.Y < GetPos().Y)
		{
			MoveTo[0] = vector2d(1, -1);
			MoveTo[1] = vector2d(1,  0);
			MoveTo[2] = vector2d(0, -1);
		}

		if(TPos.Y == GetPos().Y)
		{
			MoveTo[0] = vector2d(1,  0);
			MoveTo[1] = vector2d(1, -1);
			MoveTo[2] = vector2d(1,  1);
		}

		if(TPos.Y > GetPos().Y)
		{
			MoveTo[0] = vector2d(1, 1);
			MoveTo[1] = vector2d(1, 0);
			MoveTo[2] = vector2d(0, 1);
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

bool character::TryMove(vector2d MoveTo)
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

bool character::ShowInventory()
{
	GetStack()->DrawContents("Character's Inventory, press ESC to exit");

	return false;
}

bool character::PickUp()
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

bool character::Quit()
{
	if(game::BoolQuestion("Thine Holy Quest is not yet compeleted! Really quit? [Y/N]"))
	{
		game::Quit();

		game::RemoveSaves();

		if(!game::GetWizardMode())
		{
			AddScoreEntry("cowardly quit the game", 0.75f);
			highscore HScore;
			HScore.Draw(FONTW, FONTB);
		}

		return true;
	}
	else
		return false;
}

void character::CreateCorpse()
{
	GetLevelSquareUnder()->GetStack()->AddItem(new corpse(GetMaterial(0)));

	SetMaterial(0, 0);
}

void character::Die()
{
	if(!Exists)
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

	//Dead = true;

	while(GetStack()->GetItems())
		GetStack()->MoveItem(0, GetLevelSquareUnder()->GetStack());
	
	GetLevelSquareUnder()->RemoveCharacter();

	CreateCorpse();

	SetExists(false);
	//game::SendToHell(this);

	if(GetIsPlayer())
	{
		game::Quit();

		if(!game::GetWizardMode())
		{
			game::DrawEverything();

			GETKEY();
		}

		iosystem::TextScreen(FONTW, "Unfortunately thee died during thine journey. The Überpriest is not happy.");

		game::RemoveSaves();

		highscore HScore;
		HScore.Draw(FONTW, FONTB);
	}
}

bool character::OpenItem()
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

bool character::WearArmor()
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

bool character::Talk()
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

			for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
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

bool character::NOP()
{
	continent* C = game::GetWorldMap()->GetContinent(1);

	SetAgilityExperience(GetAgilityExperience() - 5);

	return true;
}

void character::ApplyExperience()
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

bool character::HasHeadOfElpuri() const
{
	for(ushort c = 0; c < GetStack()->GetItems(); ++c)
		if(GetStack()->GetItem(c)->IsHeadOfElpuri())
			return true;

	return false;
}

bool character::HasPerttusNut() const
{
	for(ushort c = 0; c < GetStack()->GetItems(); ++c)
		if(GetStack()->GetItem(c)->IsPerttusNut())
			return true;

	return false;
}

bool character::HasMaakotkaShirt() const
{
	for(ushort c = 0; c < GetStack()->GetItems(); ++c)
		if(GetStack()->GetItem(c)->IsMaakotkaShirt())
			return true;

	return false;
}

bool character::Save()
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

bool character::Read()
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

bool character::Dip()
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

void character::Save(outputfile& SaveFile) const
{
	object::Save(SaveFile);

	Stack->Save(SaveFile);

	ushort Index = Wielded ? Stack->SearchItem(Wielded) : 0xFFFF;

	SaveFile << Index << Strength << Endurance << Agility << Perception << RegenerationCounter;
	SaveFile << HP << NP << AP;
	SaveFile << StrengthExperience << EnduranceExperience << AgilityExperience << PerceptionExperience;
	SaveFile << Relations << Fainted << EatingCurrently << APsToBeEaten;
}

void character::Load(inputfile& SaveFile)
{
	object::Load(SaveFile);

	if(Stack)
		delete Stack;

	Stack = new stack;
	Stack->Load(SaveFile);

	ushort Index;

	SaveFile >> Index;

	Wielded = Index != 0xFFFF ? Stack->GetItem(Index) : 0;

	SaveFile >> Strength >> Endurance >> Agility >> Perception >> RegenerationCounter;
	SaveFile >> HP >> NP >> AP;
	SaveFile >> StrengthExperience >> EnduranceExperience >> AgilityExperience >> PerceptionExperience;
	SaveFile >> Relations >> Fainted >> EatingCurrently >> APsToBeEaten;
}

bool character::WizardMode()
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

bool character::RaiseStats()
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

bool character::LowerStats()
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

bool character::GainAllItems()
{
	if(game::GetWizardMode())
		for(ushort c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
			Stack->AddItem(protocontainer<item>::GetProto(c)->Clone()); //GGG
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");

	return false;
}

bool character::SeeWholeMap()
{
	if(game::GetWizardMode())
	{
		game::SeeWholeMap();
	}
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");

	return false;
}

bool character::IncreaseGamma()
{
	//GGG graphics::Gamma(0, game::EditGamma(-5));

	return false;
}

bool character::DecreaseGamma()
{
	//GGG graphics::Gamma(0, game::EditGamma(5));

	return false;
}

bool character::IncreaseSoftGamma()
{
	game::EditSoftGamma(0.05f);

	game::GetCurrentArea()->SendNewDrawRequest();

	return false;
}

bool character::DecreaseSoftGamma()
{
	game::EditSoftGamma(-0.05f);

	game::GetCurrentArea()->SendNewDrawRequest();

	return false;
}

ushort character::GetEmitation() const
{
	ushort Emitation = 0;

	for(ushort c = 0; c < GetMaterials(); ++c)
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

bool character::WalkThroughWalls()
{
	if(game::GetWizardMode())
		game::GoThroughWalls();
	else
		ADD_MESSAGE("Activate wizardmode to use this function.");

	return false;
}

bool character::ShowKeyLayout()
{
	list List("Keyboard Layout");

	List.AddDescription("");
	List.AddDescription("Key       Description");

	for(uchar c = 1; game::GetCommand(c); ++c)
	{
		std::string Buffer;
		Buffer += game::GetCommand(c)->GetKey();
		Buffer.resize(10, ' ');
		List.AddString(Buffer + game::GetCommand(c)->GetDescription());
	}

	List.Draw(FONTW, FONTR, false);

	return false;
}

bool character::Look()
{
	int Key;
	std::string OldMemory;
	vector2d CursorPos = GetPos();
	game::DrawEverythingNoBlit();
	FONTW->Printf(DOUBLEBUFFER, 16, 514, "Press direction keys to move cursor or esc to exit from the mode.");
	DRAW_MESSAGES();
	EMPTY_MESSAGES();
	graphics::BlitDBToScreen();

	while(Key != ' ' && Key != 0x1B)
	{


		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
			if(Key == game::GetMoveCommandKey(c))
			{
				CursorPos += game::GetMoveVector(c);

				if (short(CursorPos.X) > game::GetCurrentArea()->GetXSize()-1)	CursorPos.X = 0;
				if (short(CursorPos.X) < 0)					CursorPos.X = game::GetCurrentArea()->GetXSize()-1;
				if (short(CursorPos.Y) > game::GetCurrentArea()->GetYSize()-1)	CursorPos.Y = 0;
				if (short(CursorPos.Y) < 0)					CursorPos.Y = game::GetCurrentArea()->GetYSize()-1;
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
			OldMemory = game::GetCurrentArea()->GetSquare(CursorPos)->GetMemorizedDescription();
			game::GetCurrentArea()->GetSquare(CursorPos)->UpdateMemorizedDescription();
		}

		if(game::GetCurrentArea()->GetSquare(CursorPos)->GetKnown() || game::GetSeeWholeMapCheat())
		{
			std::string Msg;

			if(game::GetCurrentArea()->GetSquare(CursorPos)->CanBeSeen() || game::GetSeeWholeMapCheat())
				Msg = "You see here ";
			else
				Msg = "You remember here ";

			Msg += game::GetCurrentArea()->GetSquare(CursorPos)->GetMemorizedDescription();

			ADD_MESSAGE("%s.", Msg.c_str());

			if(game::GetCurrentArea()->GetSquare(CursorPos)->GetCharacter() && (game::GetCurrentArea()->GetSquare(CursorPos)->CanBeSeen() || game::GetSeeWholeMapCheat()))
				ADD_MESSAGE("%s is standing here.", game::GetCurrentArea()->GetSquare(CursorPos)->GetCharacter()->CNAME(INDEFINITE));
		}
		else
			ADD_MESSAGE("You have no idea what might lie here.");

		if(game::GetSeeWholeMapCheat())
		{
			game::GetCurrentArea()->GetSquare(CursorPos)->SetMemorizedDescription(OldMemory);
			game::GetCurrentArea()->GetSquare(CursorPos)->SetDescriptionChanged(true);
		}

		game::DrawEverythingNoBlit();
		igraph::DrawCursor((CursorPos - game::GetCamera() + vector2d(0,2)) << 4);
		game::GetCurrentArea()->GetSquare(CursorPos)->SendNewDrawRequest();
		FONTW->Printf(DOUBLEBUFFER, 16, 514, "Press direction keys to move cursor or esc to exit from the mode.");
		graphics::BlitDBToScreen();
		DOUBLEBUFFER->ClearToColor((CursorPos.X - game::GetCamera().X) << 4, (CursorPos.Y - game::GetCamera().Y + 2) << 4, 16, 16);
		EMPTY_MESSAGES();

		Key = GETKEY();
	}

	return false;
}

float character::GetDifficulty() const
{
	return float(GetStrength()) * GetEndurance() * GetAgility() * GetAttackStrength() / (float(CalculateArmorModifier()) * 25000);
}

float character::GetAttackStrength() const
{
	return GetWielded() ? GetWielded()->GetWeaponStrength() : GetMeleeStrength();
}

bool character::Engrave(std::string What)
{
	game::GetCurrentLevel()->GetLevelSquare(GetPos())->Engrave(What);

	return true;
}

bool character::WhatToEngrave()
{
	game::GetCurrentLevel()->GetLevelSquare(GetPos())->Engrave(iosystem::StringQuestion(FONTW, "What do you want to engrave here?", vector2d(7,7), 0, 50));
	return false;
}

void character::MoveRandomly()
{
	ushort ToTry = rand() % 8;

	if(!game::GetCurrentLevel()->GetLevelSquare(GetPos() + game::GetMoveVector(ToTry))->GetCharacter());
		TryMove(GetPos() + game::GetMoveVector(ToTry));
}

bool character::TestForPickup(item* ToBeTested) const
{
	if(GetBurdenState(ToBeTested->GetWeight() + GetStack()->SumOfMasses()) != UNBURDENED)
		return false;
	return true;
}

bool character::OpenPos(vector2d APos)
{
	if(game::GetCurrentLevel()->GetLevelSquare(APos)->Open(this))
	{
		SetAgilityExperience(GetAgilityExperience() + 25);
		SetNP(GetNP() - 1);
		return true;
	}
	return false;
}

bool character::Pray()
{
	list Panthenon("To Whom shall thee adress thine prayers?");

	if(!GetLevelSquareUnder()->GetDivineOwner())
		for(ushort c = 1; game::GetGod(c); ++c)
			Panthenon.AddString(game::GetGod(c)->CompleteDescription());
	else
		Panthenon.AddString(game::GetGod(GetLevelSquareUnder()->GetDivineOwner())->CompleteDescription());

	ushort Select = Panthenon.Draw(FONTW, FONTR);

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
			if(game::BoolQuestion(std::string("Do you really wish to pray to ") + game::GetGod(Select+1)->Name().c_str() + "? [Y/N]"))
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

void character::NeutralAICommand()
{
	dynarray<character*> SeenCharacters;

	DO_FILLED_RECTANGLE(GetPos().X, GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, LOSRange(),
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer,YPointer))->GetCharacter())
			SeenCharacters.Add(game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer, YPointer))->GetCharacter());
	});

	for(ushort c = 0; c < SeenCharacters.Length(); ++c)
	{
		if(!SeenCharacters.Access(c)->GetRelations() && SeenCharacters.Access(c)->GetLevelSquareUnder()->CanBeSeenFrom(GetPos()))
		{
			Charge(SeenCharacters.Access(c));
			return;
		}
	}

	DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	if(game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->Open(this))
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->CanBeSeen())
		{
			if(GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s opens the door.", CNAME(DEFINITE));
			else
				ADD_MESSAGE("Something opens the door.");
		}

		return;
	})

	for(c = 0; c < GetLevelSquareUnder()->GetStack()->GetItems(); ++c)
	{
		if(GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeaponStrength() > GetAttackStrength() && GetBurdenState(GetStack()->SumOfMasses() + GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeight()) == UNBURDENED && CanWield())
		{
			item* ToWield = GetStack()->GetItem(GetLevelSquareUnder()->GetStack()->MoveItem(c, GetStack()));

			if(GetWielded())
				GetStack()->MoveItem(GetStack()->SearchItem(GetWielded()), GetLevelSquareUnder()->GetStack());

			SetWielded(ToWield);

			if(SquareUnder->CanBeSeen())
				ADD_MESSAGE("%s picks up and wields %s.", CNAME(DEFINITE), ToWield->CNAME(DEFINITE));

			return;
		}

		if(GetLevelSquareUnder()->GetStack()->GetItem(c)->GetArmorValue() > CalculateArmorModifier() && GetBurdenState(GetStack()->SumOfMasses() + GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeight()) == UNBURDENED && CanWear())
		{
			item* ToWear = GetStack()->GetItem(GetLevelSquareUnder()->GetStack()->MoveItem(c, GetStack()));

			if(GetTorsoArmor())
				GetStack()->MoveItem(GetStack()->SearchItem(GetTorsoArmor()), GetLevelSquareUnder()->GetStack());

			SetTorsoArmor(ToWear);

			if(SquareUnder->CanBeSeen())
				ADD_MESSAGE("%s picks up and wears %s.", CNAME(DEFINITE), ToWear->CNAME(DEFINITE));

			return;
		}

		if(GetLevelSquareUnder()->GetStack()->GetItem(c)->Consumable(this))
		{
			item* ToEat = GetLevelSquareUnder()->GetStack()->RemoveItem(c);

			if(SquareUnder->CanBeSeen())
				ADD_MESSAGE("%s eats %s.", CNAME(DEFINITE), ToEat->CNAME(DEFINITE));

			delete ToEat; //This should take time also.

			return;
		}
	}

	MoveRandomly();
}

void character::FriendAICommand()
{
}

void character::HostileAICommand()
{
	dynarray<character*> SeenCharacters;

	DO_FILLED_RECTANGLE(GetPos().X, GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, LOSRange(),
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer,YPointer))->GetCharacter())
			SeenCharacters.Add(game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer, YPointer))->GetCharacter());
	});

	ushort NumberFor;

	if((NumberFor = SeenCharacters.Search(game::GetPlayer())) != 0xFFFF && GetLevelSquareUnder()->RetrieveFlag())
	{
		Charge(SeenCharacters.Access(NumberFor));
		return;
	}
	else
	{
		for(ushort c = 0; c < SeenCharacters.Length(); ++c)
		{
			if(SeenCharacters.Access(c)->GetRelations() > 0 && SeenCharacters.Access(c)->GetLevelSquareUnder()->CanBeSeenFrom(GetPos()))
			{
				Charge(SeenCharacters.Access(c));
				return;
			}
		}
	}

	DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	if(game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->Open(this))
	{
		if(game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->CanBeSeen())
		{
			if(GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s opens the door.", CNAME(DEFINITE));
			else
				ADD_MESSAGE("Something opens the door.");
		}

	return;
	})

	for(ushort c = 0; c < GetLevelSquareUnder()->GetStack()->GetItems(); ++c)
	{
		if(GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeaponStrength() > GetAttackStrength() && GetBurdenState(GetStack()->SumOfMasses() + GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeight()) == UNBURDENED && CanWield())
		{
			item* ToWield = GetStack()->GetItem(GetLevelSquareUnder()->GetStack()->MoveItem(c, GetStack()));

			if(GetWielded())
				GetStack()->MoveItem(GetStack()->SearchItem(GetWielded()), GetLevelSquareUnder()->GetStack());

			SetWielded(ToWield);

			if(SquareUnder->CanBeSeen())
				ADD_MESSAGE("%s picks up and wields %s.", CNAME(DEFINITE), ToWield->CNAME(DEFINITE));

			return;
		}

		if(GetLevelSquareUnder()->GetStack()->GetItem(c)->GetArmorValue() > CalculateArmorModifier() && GetBurdenState(GetStack()->SumOfMasses() + GetLevelSquareUnder()->GetStack()->GetItem(c)->GetWeight()) == UNBURDENED && CanWear())
		{
			item* ToWear = GetStack()->GetItem(GetLevelSquareUnder()->GetStack()->MoveItem(c, GetStack()));

			if(GetTorsoArmor())
				GetStack()->MoveItem(GetStack()->SearchItem(GetTorsoArmor()), GetLevelSquareUnder()->GetStack());

			SetTorsoArmor(ToWear);

			if(SquareUnder->CanBeSeen())
				ADD_MESSAGE("%s picks up and wears %s.", CNAME(DEFINITE), ToWear->CNAME(DEFINITE));

			return;
		}

		if(GetLevelSquareUnder()->GetStack()->GetItem(c)->Consumable(this))
		{
			item* ToEat = GetLevelSquareUnder()->GetStack()->RemoveItem(c);

			if(SquareUnder->CanBeSeen())
				ADD_MESSAGE("%s eats %s.", CNAME(DEFINITE), ToEat->CNAME(DEFINITE));

			delete ToEat; //This should take time also.

			return;
		}
	}

	MoveRandomly();
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

bool character::Kick()
{
	uchar Direction;
	if(!CanKick())
	{
		ADD_MESSAGE("This monster type can not kick.");
		return false;
	}
	ADD_MESSAGE("In what direction do you wish to kick?");
	game::DrawEverything();
	vector2d KickPos = game::AskForDirectionVector();
	if(KickPos != vector2d(0,0))
	{
		for(uchar c = 0; c < 8; ++c)
		{
			if(KickPos == game::GetMoveVector(c))
				Direction = c;
		}
		
		return game::GetCurrentLevel()->GetLevelSquare(GetPos() + KickPos)->Kick(GetStrength(), Direction,this);
		
	}

	return false;
}

bool character::ScreenShot()
{
	bitmap TempDB(XRES, YRES);

	DOUBLEBUFFER->Blit(&TempDB, 0, 0, 0, 0, XRES, YRES);

	TempDB.Save("Scrshot.bmp");

	return false;
}

bool character::Offer()
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

long character::Score() const
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

bool character::DrawMessageHistory()
{
	globalmessagingsystem::DrawMessageHistory();

	return false;
}

bool character::Throw()
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

bool character::ThrowItem(uchar Direction, item* ToBeThrown) // Note to C++ people. This is throw in the GAME not the throw in ++c!
{
	if(Direction > 7)
		ABORT("Throw in TOO odd direction...");

	return ToBeThrown->Fly(Direction, GetStrength(), GetStack(), GetIsPlayer());
}

void character::HasBeenHitByItem(item* Thingy, float Speed)
{
	ushort Damage = ushort(Thingy->GetWeaponStrength() * Thingy->GetWeight() * CalculateArmorModifier() * sqrt(Speed) / 2000000000) + (rand() % 5 ? 1 : 0);

	SetHP(GetHP() - Damage);

	if(SquareUnder->CanBeSeen())
	{
		if(GetIsPlayer())
			ADD_MESSAGE("%s hits you.", Thingy->CNAME(DEFINITE));
		else
			ADD_MESSAGE("%s hits %s.", Thingy->CNAME(DEFINITE), CNAME(DEFINITE));

		if(game::GetWizardMode())
			ADD_MESSAGE("(damage: %d) (speed: %f)", Damage, Speed);
	}
	Thingy->ImpactDamage(Speed, GetSquareUnder()->CanBeSeen(), GetLevelSquareUnder()->GetStack());
	SpillBlood(1 + rand() % 1);
	CheckDeath(std::string("died by thrown ") + Thingy->CNAME(INDEFINITE) );
}

bool character::DodgesFlyingItem(item*, float Speed)
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

void character::GetPlayerCommand()
{
	bool HasActed = false;

	while(!HasActed)
	{
		game::DrawEverything();

		int Key = GETKEY();

		bool ValidKeyPressed = false;

		{
		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
			if(Key == game::GetMoveCommandKey(c))
			{
				HasActed = TryMove(GetPos() + game::GetMoveVector(c));
				ValidKeyPressed = true;
			}
		}

		for(uchar c = 1; game::GetCommand(c); ++c)
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

void character::ContinueEating()
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

void character::StopEating()
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

bool character::Apply()
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

vector2d character::GetPos() const
{
	return SquareUnder->GetPos();
}

bool character::ForceVomit()
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

bool character::Zap()
{
	ushort Index;
	if((Index = GetStack()->DrawContents("What do you want to zap with?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to zap with.");
		return false;
	}

	if(Index < GetStack()->GetItems())
	{
		if(!GetStack()->GetItem(Index)->CanBeZapped())
		{
			ADD_MESSAGE("You can't zap that!");
			return false;
		}
		uchar Answer = game::DirectionQuestion("In what direction do you wish to zap? Press . to zap yourself.", 8, false, true);
		if(Answer == 0xFF)
			return false;
		return GetStack()->GetItem(Index)->Zap(GetPos(), Answer);
	}
	else
		return false;

	return true;	
}

bool character::Polymorph()
{
	if(GetIsPlayer() && game::GetPolymorphCounter() != 0xFFFF)
	{
		ADD_MESSAGE("You shudder.");
		return true;
	}

	character* NewForm = protosystem::BalancedCreateMonster(2);

	GetSquareUnder()->AddCharacter(NewForm);

	while(GetStack()->GetItems())
		GetStack()->MoveItem(0, NewForm->GetStack());

	if(NewForm->CanWield())
		NewForm->SetWielded(GetWielded());

	if(NewForm->CanWear())
		NewForm->SetTorsoArmor(GetTorsoArmor());

	if(GetIsPlayer())
	{
		game::SetPlayerBackup(this);
		game::SetPlayer(NewForm);
		game::SetPolymorphCounter(1000);
		NewForm->SetRelations(FRIEND);
	}
	else
		SetExists(false);

	return true;
}

void character::ChangeBackToPlayer()
{
	SetExists(false);

	GetSquareUnder()->AddCharacter(game::GetPlayerBackup());

	while(GetStack()->GetItems())
		GetStack()->MoveItem(0, game::GetPlayerBackup()->GetStack());

	game::SetPlayer(game::GetPlayerBackup());
	game::SetPlayerBackup(0);	
}

levelsquare* character::GetLevelSquareUnder() const
{
	return (levelsquare*)SquareUnder;
}

worldmapsquare* character::GetWorldMapSquareUnder() const
{
	return (worldmapsquare*)SquareUnder;
}

ulong character::GetBloodColor() const
{
	return MAKE_RGB(75,0,0);
}

void character::BeKicked(ushort KickStrength, bool ShowOnScreen, uchar Direction, character*)
{
	if(rand() % 10 +  rand() % 3 * KickStrength / 2 > GetAgility())
	{
		if(KickStrength > 8 + rand() % 4 - rand() % 4)
		{
			FallTo(GetPos() + game::GetMoveVector(Direction), ShowOnScreen);
			if(ShowOnScreen)
			{
				if(GetIsPlayer())
					ADD_MESSAGE("The kick throws you off balance.");
				else
					ADD_MESSAGE("The kick throws %s off balance.", Name(DEFINITE).c_str());
			}
			SetHP(GetHP() - rand() % (KickStrength / 5));
			CheckDeath("kicked to death");		
		}
		else
			if(ShowOnScreen)
			{
				if(GetIsPlayer())
					ADD_MESSAGE("The kick hits you, but you remain standing.");
				else
					ADD_MESSAGE("The kick hits %s.", Name(DEFINITE).c_str());
				SetHP(GetHP() - rand() % (KickStrength / 7));
				CheckDeath("kicked to death");
			}
	}
	else
		if(ShowOnScreen)
		{
			if(GetIsPlayer())
				ADD_MESSAGE("The kick misses you.");
			else
				ADD_MESSAGE("The kick misses %s.", Name(DEFINITE).c_str());
		}
	
}

void character::FallTo(vector2d Where, bool OnScreen)
{
	SetAP(GetAP() - 500);
	if(game::GetCurrentLevel()->GetLevelSquare(Where)->GetOverLevelTerrain()->GetIsWalkable() && !game::GetCurrentLevel()->GetLevelSquare(Where)->GetCharacter())
	{
		Move(Where, true);
	}



	if(!game::GetCurrentLevel()->GetLevelSquare(Where)->GetOverLevelTerrain()->GetIsWalkable())
	{
		if(OnScreen)
		{
			if(GetIsPlayer()) 
				ADD_MESSAGE("You hit your head on the wall.");
			else
				ADD_MESSAGE("%s hits %s head on the wall.", Name(DEFINITE).c_str(), game::PossessivePronoun(GetSex()));
		}
		SetHP(GetHP() - rand() % 2);
		CheckDeath("killed by hitting a wall");
	}	
}
