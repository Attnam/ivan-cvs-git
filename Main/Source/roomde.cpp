#include "roomde.h"
#include "charba.h"
#include "square.h"
#include "message.h"
#include "itemba.h"
#include "lterrade.h"
#include "god.h"

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
			if(Customer->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
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
	if(!Customer->GetIsPlayer())
		return false;

	if(Master && Customer->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
	{
		if(Customer->GetMoney() >= ForSale->Price())
		{
			ADD_MESSAGE("\"Ah! That %s costs %d Attnamian Perttubucks.", ForSale->CNAME(UNARTICLED), ForSale->Price());
			ADD_MESSAGE("No haggling, please.\"");

			if(game::BoolQuestion("Do you want to buy this item? [y/N]"))
			{
				Customer->SetMoney(Customer->GetMoney() - ForSale->Price());
				return true;
			}
			else
				return false;
		}
		else
		{
			ADD_MESSAGE("\"Don't touch that, beggar! It is worth %d Attnamian Perttubucks!\"", ForSale->Price());

			return false;
		}
	}
	else
		return true;
}

bool shop::DropItem(character* Customer, item* ForSale)
{
	if(!Customer->GetIsPlayer())
		return false;

	if(Master && Customer->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
	{
		ADD_MESSAGE("\"Sorry, I don't buy any of your junk.\"");

		return false;
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
			if(Pilgrim->GetSquareUnder()->CanBeSeenFrom(Master->GetSquareUnder()->GetPos(), Master->LOSRangeSquare()))
				if(Master->GetSquareUnder()->CanBeSeenFrom(Pilgrim->GetSquareUnder()->GetPos(), Pilgrim->LOSRangeSquare()))
					ADD_MESSAGE("%s opens %s mouth: \"Welcome to the shrine of %s!\"", Master->CNAME(DEFINITE), game::PossessivePronoun(Master->GetSex()), game::GetGod(DivineOwner)->Name().c_str());
				else
					ADD_MESSAGE("You hear a voice say: \"Welcome to the shrine of %s!\"", game::GetGod(DivineOwner)->Name().c_str());
		}
		else
			ADD_MESSAGE("The temple appears to be deserted.");
}
