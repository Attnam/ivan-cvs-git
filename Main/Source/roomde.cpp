#include "roomde.h"
#include "charba.h"
#include "square.h"
#include "message.h"
#include "itemba.h"
#include "lterrade.h"
#include "god.h"
#include "lsquare.h"
#include "stack.h"
#include "team.h"

void shop::HandleInstantiatedCharacter(character* Character)
{
	Master = Character;
	Character->SetHomeRoom(Index);
}

void shop::Enter(character* Customer)
{
	if(Customer->GetIsPlayer())
		if(Master)
		{
			if(Master->GetTeam()->GetRelation(Customer->GetTeam()) != HOSTILE && Customer->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
				if(Master->GetSquareUnder()->CanBeSeenFrom(Customer->GetSquareUnder()->GetPos(), Customer->LOSRangeSquare()))
					ADD_MESSAGE("%s welcomes you warmly to the shop.", Master->CNAME(DEFINITE));
				else
					ADD_MESSAGE("Something welcomes you warmly to the shop.");
		}
		else
			ADD_MESSAGE("The shop appears to be deserted.");
}

bool shop::PickupItem(character* Customer, item* ForSale)
{
	if(!Master || Customer == Master || Master->GetTeam()->GetRelation(Customer->GetTeam()) == HOSTILE)
		return true;

	ulong Price = ForSale->Price();

	if(!Customer->GetIsPlayer())
		if(Customer->GetSquareUnder()->CanBeSeen() && Customer->GetMoney() >= Price)
		{
			ADD_MESSAGE("%s buys %s.", Customer->CNAME(DEFINITE), ForSale->CNAME(DEFINITE));
			Customer->SetMoney(Customer->GetMoney() - Price);
			Master->SetMoney(Master->GetMoney() + Price);
			return true;
		}
		else
			return false;

	if(Customer->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
	{
		if(ForSale->IsHeadOfElpuri() || ForSale->IsMaakotkaShirt() || ForSale->IsPerttusNut() || ForSale->IsTheAvatar())
		{
			ADD_MESSAGE("\"I think it is yours. Take it.\"");
			return true;
		}

		if(!Price)
		{
			ADD_MESSAGE("\"Thank you for cleaning that junk out of my floor.\"");
			return true;
		}

		if(Customer->GetMoney() >= Price)
		{
			ADD_MESSAGE("\"Ah! That %s costs %d Attnamian Perttubucks.", ForSale->CNAME(UNARTICLED), Price);
			ADD_MESSAGE("No haggling, please.\"");

			if(game::BoolQuestion("Do you want to buy this item? [y/N]"))
			{
				Customer->SetMoney(Customer->GetMoney() - Price);
				Master->SetMoney(Master->GetMoney() + Price);
				return true;
			}
			else
				return false;
		}
		else
		{
			ADD_MESSAGE("\"Don't touch that, beggar! It is worth %d Attnamian Perttubucks!\"", Price);
			return false;
		}
	}
	else
		if(game::BoolQuestion("Are you sure you want to steal this item? [y/N]"))
		{
			Customer->GetTeam()->Hostility(Master->GetTeam());
			return true;
		}
		else
			return false;
}

bool shop::DropItem(character* Customer, item* ForSale)
{
	if(!Master || Customer == Master || Master->GetTeam()->GetRelation(Customer->GetTeam()) == HOSTILE)
		return true;

	ulong Price = (ForSale->Price() >> 1);

	if(!Customer->GetIsPlayer())
		if(Price && Customer->GetSquareUnder()->CanBeSeen() && Master->GetMoney() >= Price)
		{
			ADD_MESSAGE("%s sells %s.", Customer->CNAME(DEFINITE), ForSale->CNAME(DEFINITE));
			Customer->SetMoney(Customer->GetMoney() + Price);
			Master->SetMoney(Master->GetMoney() - Price);
			return true;
		}
		else
			return false;

	if(Customer->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
	{
		if(ForSale->IsHeadOfElpuri() || ForSale->IsMaakotkaShirt() || ForSale->IsPerttusNut() || ForSale->IsTheAvatar())
		{
			ADD_MESSAGE("\"Oh no! You need it far more than I!\"");
			return false;
		}

		if(!Price)
		{
			ADD_MESSAGE("\"Hah! I wouldn't take that even if you paid me for it!\"");
			return false;
		}

		if(Master->GetMoney() >= Price)
		{
			ADD_MESSAGE("\"What a fine %s. I'll pay %d Attnamian Perttubucks for it.\"", ForSale->CNAME(UNARTICLED), Price);

			if(game::BoolQuestion("Do you want to sell this item? [y/N]"))
			{
				Customer->SetMoney(Customer->GetMoney() + Price);
				Master->SetMoney(Master->GetMoney() - Price);
				return true;
			}
			else
				return false;
		}
		else
		{
			ADD_MESSAGE("\"I would pay you %d Perttubucks for it, but I don't have so much. Sorry.\"", Price);
			return false;
		}
	}
	else
		return true;
}

void temple::HandleInstantiatedCharacter(character* Character)
{
	Master = Character;
	Character->SetHomeRoom(Index);
}

void temple::HandleInstantiatedOverLevelTerrain(overlevelterrain* Terrain)
{
	if(Terrain->GetType() == altar::StaticType())
		((altar*)Terrain)->SetOwnerGod(DivineOwner);
}

void temple::Enter(character* Pilgrim)
{
	if(Pilgrim->GetIsPlayer())
		if(Master)
		{
			if(Master->GetTeam()->GetRelation(Pilgrim->GetTeam()) != HOSTILE && Pilgrim->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
				if(Master->GetSquareUnder()->CanBeSeenFrom(Pilgrim->GetSquareUnder()->GetPos(), Pilgrim->LOSRangeSquare()))
					ADD_MESSAGE("%s opens %s mouth: \"Welcome to the shrine of %s!\"", Master->CNAME(DEFINITE), game::PossessivePronoun(Master->GetSex()), game::GetGod(DivineOwner)->Name().c_str());
				else
					ADD_MESSAGE("You hear a voice say: \"Welcome to the shrine of %s!\"", game::GetGod(DivineOwner)->Name().c_str());
		}
		else
			ADD_MESSAGE("The temple appears to be deserted.");
}

void shop::KickSquare(character* Infidel, levelsquare* Square)
{
	if(!Master)
		return;

	if(Square->GetStack()->GetItems() && Infidel->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
	{
		ADD_MESSAGE("\"You infidel!\"");
		Infidel->GetTeam()->Hostility(Master->GetTeam());
	}
}

bool shop::ConsumeItem(character* Customer, item*)
{
	if(!Master)
		return true;

	if(!Customer->GetIsPlayer())
		return false;

	if(Customer->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
	{
		ADD_MESSAGE("\"Buy that first, please.\"");
		return false;
	}
	else
		if(game::BoolQuestion("It's illegal to eat property of others. Are you sure you sure? [y/N]"))
		{
			Customer->GetTeam()->Hostility(Master->GetTeam());
			return true;
		}
		else
			return false;
}
