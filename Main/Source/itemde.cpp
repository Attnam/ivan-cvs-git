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
#include "team.h"

void can::PositionedDrawToTileBuffer(uchar) const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

item* can::TryToOpen(stack* Stack)
{
	item* x = new lump(GetMaterial(1));

	Stack->AddItem(x);

	SetMaterial(1,0);

	return x;
}

bool corpse::Consume(character* Eater, float Amount)
{
	GetMaterial(0)->EatEffect(Eater, Amount, NPModifier());

	if(Eater->GetIsPlayer() && Eater->CheckCannibalism(GetMaterial(0)->GetType()))
	{
		game::DoEvilDeed(10);
		ADD_MESSAGE("You feel that this was an evil deed.");
		Eater->EndEating();
	}

	return GetMaterial(0)->GetVolume() ? false : true;
}

bool banana::Consume(character* Eater, float Amount)
{
	GetMaterial(1)->EatEffect(Eater, Amount, NPModifier());

	if(!GetMaterial(1)->GetVolume())
		Eater->GetStack()->AddItem(new bananapeals);

	return GetMaterial(1)->GetVolume() ? false : true;
}

bool lump::Consume(character* Eater, float Amount)
{
	GetMaterial(0)->EatEffect(Eater, Amount, NPModifier());

	return GetMaterial(0)->GetVolume() ? false : true;
}

bool potion::Consume(character* Eater, float Amount)
{
	GetMaterial(1)->EatEffect(Eater, Amount, NPModifier());

	if(!GetMaterial(1)->GetVolume())
	{
		Eater->EndEating();
		ChangeMaterial(1,0);
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

		if(Reader->GetIsPlayer())
			ADD_MESSAGE("As you read the scroll a monster appears.");
		else
			if(Reader->GetSquareUnder()->CanBeSeen())
				ADD_MESSAGE("The %s reads %s. A monster appears!", Reader->CNAME(DEFINITE), CNAME(DEFINITE));
	}
	else
		if(Reader->GetIsPlayer())
			ADD_MESSAGE("You feel a lost soul fly by you.");

	return true;
}

bool scrollofteleport::Read(character* Reader)
{
	for(vector2d Pos;;)
	{
		Pos = game::GetCurrentLevel()->RandomSquare(true);
		if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter() == 0)
			break;
	}
	if(Reader->GetIsPlayer())
		ADD_MESSAGE("After you have read the scroll you realize that you have teleported.");
	else
		if(Reader->GetSquareUnder()->CanBeSeen())
			ADD_MESSAGE("The %s reads %s and disappears!", Reader->CNAME(DEFINITE), CNAME(DEFINITE));

	Reader->Move(Pos, true);
	return true;
}

void lump::ReceiveHitEffect(character* Enemy, character*)
{
	if(rand() % 2)
	{
	if(Enemy->GetSquareUnder()->CanBeSeen())
		ADD_MESSAGE("The %s touches %s.", GetMaterial(0)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

	GetMaterial(0)->HitEffect(Enemy);
	}
}

void meleeweapon::ReceiveHitEffect(character* Enemy, character*)
{
	if(GetMaterial(2))
	{
		if(Enemy->GetIsPlayer())
			ADD_MESSAGE("The %s reacts with you!", GetMaterial(2)->CNAME(UNARTICLED));
		else
			if(Enemy->GetSquareUnder()->CanBeSeen())
				ADD_MESSAGE("The %s reacts with %s.", GetMaterial(2)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

		GetMaterial(2)->HitEffect(Enemy);
	}
}

void meleeweapon::DipInto(item* DipTo)
{
	ChangeMaterial(2, DipTo->BeDippedInto());
	ADD_MESSAGE("%s is now covered with %s.", CNAME(DEFINITE), GetMaterial(2)->CNAME(UNARTICLED));
}

material* lump::BeDippedInto()
{
	return GetMaterial(0)->Clone(GetMaterial(0)->TakeDipVolumeAway());
}

bool potion::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
	ushort Index = ItemStack->AddItem(new brokenbottle);
	ItemStack->RemoveItem(ItemStack->SearchItem(this));
	if (IsShown) ADD_MESSAGE("The potion shatters to pieces.");
	SetExists(false);
	return true;
}

void potion::PositionedDrawToTileBuffer(uchar) const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

bool loaf::Consume(character* Eater, float Amount)
{
	GetMaterial(0)->EatEffect(Eater, Amount, NPModifier());
	ulong p = GetMaterial(0)->GetVolume();
	return GetMaterial(0)->GetVolume() ? false : true;
}

bool abone::Consume(character* Consumer, float Amount)
{
	GetMaterial(0)->EatEffect(Consumer, Amount, NPModifier());
	return GetMaterial(0)->GetVolume() ? false : true;
}

item* can::PrepareForConsuming(character* Consumer, stack* Stack)
{
	if(!Consumer->GetIsPlayer() || game::BoolQuestion(std::string("Do you want to open ") + CNAME(DEFINITE) + " before eating it? [Y/N]"))
		return TryToOpen(Stack);
	else
		return 0;
}

item* leftnutofperttu::CreateWishedItem() const
{
	return new cheapcopyofleftnutofperttu;
}

bool pickaxe::Apply(character* User, stack*)
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

bool wand::Apply(character* StupidPerson, stack* MotherStack)
{
	if(StupidPerson->GetIsPlayer())
		ADD_MESSAGE("The wand brakes in two and then explodes.");
	else
		if(StupidPerson->GetSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s brakes a wand in two. It explodes!", StupidPerson->CNAME(DEFINITE));
	MotherStack->RemoveItem(MotherStack->SearchItem(this));
	SetExists(false);	

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
	EMPTY_MESSAGES();
	game::DrawEverythingNoBlit();
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

bool lamp::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
	ItemStack->AddItem(new brokenlamp);
	ItemStack->RemoveItem(ItemStack->SearchItem(this));
	if (IsShown) ADD_MESSAGE("The lamp shatters to pieces.");
	SetExists(false);
	return true;
}

vector2d can::GetBitmapPos() const
{
	if(GetMaterial(1)) 
		RETV(16,288)	
	else
		RETV(16,304)
}

bool lamp::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
	if(!(rand() % 75) && Strength > 10 + rand() % 10)
	{
		ImpactDamage(ushort(Strength), Shown, ItemsStack);
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
		ADD_MESSAGE("This monster can't read anything.");
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

	EMPTY_MESSAGES();
	game::DrawEverythingNoBlit();
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

void avatarofvalpuri::CheckPickUpEffect(character* Picker)
{
	if(Picker->GetIsPlayer())
		DO_FILLED_RECTANGLE(Picker->GetPos().X, Picker->GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1 , game::GetCurrentLevel()->GetYSize() - 1, 30, 
		{
			character* Temp;
			if((Temp = game::GetCurrentLevel()->GetSquare(XPointer, YPointer)->GetCharacter()) && !Temp->GetIsPlayer())
				Picker->GetTeam()->Hostility(Temp->GetTeam());
		})
}

item* brokenbottle::BetterVersion(void) const
{
	material* Stuff;

	if(rand() % 5)
		Stuff = new bananaflesh(1500);
	else
		Stuff = new omleurine(1500);

	 item* P = new potion(false); 
	 P->InitMaterials(2, new glass(50), Stuff); 

	 return P;
}


bool wandofstriking::Zap(vector2d Pos, uchar Direction)
{
	vector2d CurrentPos = Pos;

	if(!GetCharge())
	{
		ADD_MESSAGE("Nothing happens.");
		return false;
	}

	if(Direction != '.')
		for(ushort Length = 0;Length < 15;Length++)
		{
			if(!game::GetCurrentLevel()->GetLevelSquare(CurrentPos + game::GetMoveVector(Direction))->GetOverLevelTerrain()->GetIsWalkable())
				break;
			else
			{
				
				CurrentPos += game::GetMoveVector(Direction);			

				if(game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetCharacter())
					game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetCharacter()->StruckByWandOfStriking();
				
				if(game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetStack()->GetItems())
					game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetStack()->StruckByWandOfStriking();
			}
		}
	else
	{
		if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter())
			game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetCharacter()->StruckByWandOfStriking();
		
		if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack()->GetItems())
			game::GetCurrentLevel()->GetLevelSquare(CurrentPos)->GetStack()->StruckByWandOfStriking();
	}

	SetCharge(GetCharge() - 1);

	return true;
}

bool platemail::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
	if(Strength > 20000 + rand() % 40000)
	{
		character* Wearer = ItemsStack->GetSquareUnder()->GetCharacter();

		if(Wearer && Wearer->GetTorsoArmor() == this)
			Wearer->SetTorsoArmor(0);

		ImpactDamage(ushort(Strength), false, ItemsStack);

		if(Shown)
			ADD_MESSAGE("The plate mail is damaged by the loud sound.");

		return true;
	}
	return false;
	
}

bool platemail::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
	if (IsShown)
		ADD_MESSAGE("%s is damaged.", CNAME(DEFINITE));

	ItemStack->RemoveItem(ItemStack->SearchItem(this));
	item* Plate = new brokenplatemail(false);
	Plate->InitMaterials(GetMaterial(0));
	ItemStack->AddItem(Plate);
	SetMaterial(0,0);
	SetExists(false);
	return true;
}
