#include <cmath>

#include "itemde.h"
#include "igraph.h"
#include "message.h"
#include "bitmap.h"
#include "stack.h"
#include "game.h"
#include "charba.h"
#include "level.h"
#include "lsquare.h"
#include "lterraba.h"
#include "save.h"
#include "feio.h"

void can::PositionedDrawToTileBuffer(uchar) const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

ushort can::TryToOpen(stack* Stack)
{
	ADD_MESSAGE("You succeed in opening the can!");

	ushort x = Stack->AddItem(new lump(GetMaterial(1)));

	SetMaterial(1,0);

	return x;
}

bool corpse::Consume(character* Eater, float Amount)
{
	if(Amount > 100) Amount = 100;
	if(Eater == game::GetPlayer())
	{
		if(Amount == 100)
			ADD_MESSAGE("You eat %s.", CNAME(DEFINITE));
		else
			ADD_MESSAGE("You eat part of %s.", CNAME(DEFINITE));
	}

	GetMaterial(0)->EatEffect(Eater, Amount, 0.10f);
	if(Eater->CheckCannibalism(GetMaterial(0)->GetType()))
	{
		game::DoEvilDeed(10);
		ADD_MESSAGE("You feel that this was a evil deed.");
	}

	return (Amount > 99);
}

bool banana::Consume(character* Eater, float Amount)
{
	if(Amount > 100) Amount = 100;
	if(Eater == game::GetPlayer())
	{
		if(Amount == 100)
			ADD_MESSAGE("You eat %s.", CNAME(DEFINITE));
		else
			ADD_MESSAGE("You eat part of %s.", CNAME(DEFINITE));
	}
	GetMaterial(1)->EatEffect(Eater, Amount);
	Eater->GetStack()->AddItem(new bananapeals);
	return (Amount > 99);
}

bool lump::Consume(character* Eater, float Amount)
{
	if(Amount > 100) Amount = 100;
	if(Eater == game::GetPlayer())
	{
		if(Amount == 100)
			ADD_MESSAGE("You eat %s.", CNAME(DEFINITE));
		else
			ADD_MESSAGE("You eat part of %s.", CNAME(DEFINITE));
	}
	GetMaterial(0)->EatEffect(Eater, Amount);
	return (Amount > 99);
}

bool potion::Consume(character* Eater, float Amount)
{
	if(Amount > 100) Amount = 100;
	if(Eater == game::GetPlayer())
	{
		if(Amount == 100)
			ADD_MESSAGE("You drink %s.", CNAME(DEFINITE));
		else
			ADD_MESSAGE("You drink part of %s.", CNAME(DEFINITE));
	}

	GetMaterial(1)->EatEffect(Eater, Amount);
	if(Amount == 100)
	{
		delete Material[1];
		SetMaterial(1,0);
	}	
	return false;
}

void lamp::PositionedDrawToTileBuffer(uchar LevelSquarePosition) const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

bool scroll::CanBeRead(character* Reader) const
{
	return Reader->CanRead();
}

bool scrollofcreatemonster::Read(character* Reader)
{
	vector2d TryToCreate;

	for(;;) // Bug bug bug! This can cause an infinite loop if there's no walkable squares around.
	{
		TryToCreate = (Reader->GetPos() + game::GetMoveVector(rand() % DIRECTION_COMMAND_KEYS));
		if(game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->GetOverLevelTerrain()->GetIsWalkable())
			break;
	}

	if(game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->GetCharacter() == 0)
	{
		game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->AddCharacter(protosystem::BalancedCreateMonster());

		if(Reader == game::GetPlayer())
			ADD_MESSAGE("As you read the scroll a monster appears.");
	}
	else
		if(Reader == game::GetPlayer())
			ADD_MESSAGE("You feel a lost soul fly by you.");

	return true;
}

bool scrollofteleport::Read(character* Reader)
{
	vector2d Pos;
	for(;;)
	{
		Pos = game::GetCurrentLevel()->RandomSquare(true);
		if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter() == 0)
			break;
	}
	if(Reader == game::GetPlayer())
		ADD_MESSAGE("After you have read the scroll you realise that you have teleported.");
	Reader->Move(Pos, true);
	return true;
}

void lump::ReceiveHitEffect(character* Enemy, character*)
{
	if(rand() % 2)
	{
	ADD_MESSAGE("The %s touches %s.", GetMaterial(0)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));
	GetMaterial(0)->HitEffect(Enemy);
	}
}

void meleeweapon::ReceiveHitEffect(character* Enemy, character*)
{
	if(GetMaterial(2))
	{
		if(Enemy == game::GetPlayer())
			ADD_MESSAGE("The %s reacts with you!", GetMaterial(2)->CNAME(UNARTICLED));
		else
			ADD_MESSAGE("The %s reacts with %s.", GetMaterial(2)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

		GetMaterial(2)->HitEffect(Enemy);
	}
}

void meleeweapon::DipInto(item* DipTo)
{
	SetMaterial(2, DipTo->BeDippedInto());
	ADD_MESSAGE("%s is now covered with %s.", CNAME(DEFINITE), GetMaterial(2)->CNAME(UNARTICLED));
}

material* lump::BeDippedInto()
{
	return GetMaterial(0)->Clone(GetMaterial(0)->TakeDipVolumeAway());
}

void potion::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
	game::GetCurrentLevel()->GetLevelSquare(ItemStack->GetPos())->GetStack()->AddItem(new brokenbottle);
	ItemStack->RemoveItem(ItemStack->SearchItem(this));
	if (IsShown) ADD_MESSAGE("The potion shatters to pieces.");
	SetExists(false);
}


void potion::PositionedDrawToTileBuffer(uchar) const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

bool loaf::Consume(character* Eater, float Amount)
{
	if(Amount > 100) Amount = 100;
	if(Eater == game::GetPlayer())
	{
		if(Amount == 100)
			ADD_MESSAGE("You eat %s.", CNAME(DEFINITE));
		else
			ADD_MESSAGE("You eat part of %s.", CNAME(DEFINITE));
	}

	GetMaterial(0)->EatEffect(Eater, Amount, 0.25f);
	return (Amount > 99);
}

bool abone::Consume(character* Consumer, float Amount)
{
	if(Consumer == game::GetPlayer())
		ADD_MESSAGE("You consume %s.", CNAME(DEFINITE));
	else if(Consumer->GetLevelSquareUnder()->CanBeSeen())
		ADD_MESSAGE("%s consumes %s.", Consumer->CNAME(DEFINITE), CNAME(DEFINITE));
	GetMaterial(0)->EatEffect(Consumer, Amount);
	return (Amount > 99);
}

ushort can::PrepareForConsuming(character* Consumer, stack* Stack)
{
	if(Consumer != game::GetPlayer() || game::BoolQuestion(std::string("Do you want to open ") + CNAME(DEFINITE) + " before eating it? [Y/N]"))
		return TryToOpen(Stack);
	else
		return 0xFFFF;
}

item* leftnutofperttu::CreateWishedItem() const
{
	return new cheapcopyofleftnutofperttu;
}

bool pickaxe::Apply(character* User)
{
	vector2d Temp;
	
	if((Temp = game::AskForDirectionVector("What direction do you want to dig?")) != vector2d(0,0))
	{

		if(game::GetCurrentLevel()->GetLevelSquare(User->GetPos() + Temp)->Dig(User, this))
		{
			User->SetAP(ushort(User->GetAP() + float(User->GetStrength() * 2 - 200)));
			User->SetStrengthExperience(User->GetStrengthExperience() + 50);
			return true;
		}
	}	

	return false;
}

ushort platemail::GetArmorValue() const
{
	float Base = 80 - sqrt(Material[0]->GetHitValue()) * 3;

	if(Base < 1)
		Base = 1;

	if(Base > 100)
		Base = 100;

	return ushort(Base);
}

ushort chainmail::GetArmorValue() const
{
	float Base = 90 - sqrt(Material[0]->GetHitValue()) * 2;

	if(Base < 1)
		Base = 1;

	if(Base > 100)
		Base = 100;

	return ushort(Base);
}

bool wand::Apply(character* StupidPerson)
{
	if(StupidPerson == game::GetPlayer()) ADD_MESSAGE("The wand brakes in two and then explodes.");

	DO_FOR_SQUARES_AROUND(StupidPerson->GetPos().X, StupidPerson->GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),

	if(game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter())
	{
		game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter()->ReceiveFireDamage(5);
		game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter()->CheckDeath(std::string("killed by ") + Name(INDEFINITE) + std::string(" exploding nearby."));
	})

	StupidPerson->ReceiveFireDamage(10);
	StupidPerson->CheckDeath(std::string("killed by ") + Name(INDEFINITE) + std::string(" exploding."));
	
	return true;
}


bool wandofpolymorph::Zap(vector2d Pos, uchar Direction)
{
	vector2d CurrentPos = Pos;

	if(!GetCharge())
	{
		ADD_MESSAGE("Nothing happens.");
		return false;
	}

	if(Direction != '.')
		for(ushort Length = 0;Length < 5;Length++)
		{
			if(!game::GetCurrentLevel()->GetLevelSquare(CurrentPos + game::GetMoveVector(Direction))->GetOverLevelTerrain()->GetIsWalkable())
				break;
			else
			{
				
				CurrentPos += game::GetMoveVector(Direction);			

				if(game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetCharacter())
					game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetCharacter()->Polymorph();
				
				if(game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetStack()->GetItems())
					game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetStack()->Polymorph();
			}
		}
	else
	{
		if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter())
			game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter()->Polymorph();
		
		if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack()->GetItems())
			game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack()->Polymorph();
	}

	SetCharge(GetCharge() - 1);

	return true;
}

void wand::Save(outputfile& SaveFile) const
{
	item::Save(SaveFile);

	SaveFile << Charge;
}

void wand::Load(inputfile& SaveFile)
{
	item::Load(SaveFile);

	SaveFile >> Charge;
}

bool scrollofwishing::Read(character* Reader)
{
	std::string Temp = iosystem::StringQuestion(FONTW, "What do you want to wish for?", vector2d(7,7), 0, 256);

	item* TempItem = protosystem::CreateItem(Temp);

	if(TempItem)
	{
		Reader->GetStack()->AddItem(TempItem);
		ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CNAME(INDEFINITE));
		return true;
	}
	else
		ADD_MESSAGE("There is no such item.");

	return false;
}

void lamp::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
	ItemStack->AddItem(new brokenlamp);
	ItemStack->RemoveItem(ItemStack->SearchItem(this));
	if (IsShown) ADD_MESSAGE("The lamp shatters to pieces.");
	SetExists(false);
}

vector2d can::GetBitmapPos() const
{
	if(GetMaterial(1)) 
		RETV(144,288)	
	else
		RETV(144,304)
}

bool lamp::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
	if(!(rand() % 75) && Strength > 10 + rand() % 10)
	{
		ImpactDamage(ushort(Strength), Shown, ItemsStack);
		if(Shown)
			ADD_MESSAGE("The lamp is destroyed by the sound.");
		return true;
	}
	return false;
	
}

bool potion::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
	if(!(rand() % 75) && Strength > 10 + rand() % 10)
	{
		ImpactDamage(ushort(Strength), Shown, ItemsStack);
		if(Shown)
			ADD_MESSAGE("The potion is destroyed by the sound.");
		return true;
	}
	return false;
	
}

bool scrollofchangematerial::Read(character* Reader)
{
	ushort Index;
	if(!Reader->CanRead())
	{
		ADD_MESSAGE("This monster can not read anything.");
		return false;
	}
	if((Index = Reader->GetStack()->DrawContents("What item do you wish to change?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to change.");
		return false;
	}
	if(Index == 0xFFFE)
		return false;
	else
		if(!(Index < Reader->GetStack()->GetItems()))
			return false;
	if(Reader->GetStack()->GetItem(Index) == this || !Reader->GetStack()->GetItem(Index)->IsMaterialChangeable())
	{
		ADD_MESSAGE("You can't change that!");
		return false;
	}
	
	std::string Temp = iosystem::StringQuestion(FONTW, "What material do you want to wish for?", vector2d(7,7), 0, 256);

	material* TempMaterial = protosystem::CreateMaterial(Temp, Reader->GetStack()->GetItem(Index)->GetMaterial(0)->GetVolume());
	
	if(TempMaterial)
		Reader->GetStack()->GetItem(Index)->ChangeMainMaterial(TempMaterial);
	else
	{
		ADD_MESSAGE("There is no such material.");
		return false;
	}

	return true;
}
