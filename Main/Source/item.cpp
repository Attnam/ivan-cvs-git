#include <cstdio>
#include <cstring>

#include "char.h"
#include "error.h"
#include "graphics.h"
#include "bitmap.h"
#include "item.h"
#include "material.h"
#include "igraph.h"
#include "strover.h"
#include "stack.h"
#include "level.h"
#include "lsquare.h"
#include "terrain.h"

item::item(bool CreateMaterials, bool SetStats)
{
	if(CreateMaterials || SetStats)
		ABORT("Boo!");
}

void item::PositionedDrawToTileBuffer(uchar) const
{
	igraph::GetItemGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (CMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
}

void can::PositionedDrawToTileBuffer(uchar) const
{
	igraph::GetItemGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (CMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
}

ushort can::TryToOpen(stack* Stack)
{
	ADD_MESSAGE("You succeed in opening the can!");

	ushort x = Stack->AddItem(new lump(CMaterial(1)));

	SetMaterial(1,0);

	return x;
}

ulong item::GetWeight(void) const
{
	ulong TotalWeight = 0;

	for(uchar c = 0; c < GetMaterials(); c++)
		if(CMaterial(c))
			TotalWeight += CMaterial(c)->GetWeight();

	return TotalWeight;
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

	CMaterial(0)->EatEffect(Eater, Amount, 0.10f);
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
	CMaterial(1)->EatEffect(Eater, Amount);
	game::GetCurrentLevel()->GetLevelSquare(Eater->GetPos())->GetStack()->AddItem(new bananapeals);
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
	CMaterial(0)->EatEffect(Eater, Amount);
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

	CMaterial(1)->EatEffect(Eater, Amount);
	if(Amount == 100)
	{
		delete Material[1];
		SetMaterial(1,0);
	}	
	return false;
}

void lamp::PositionedDrawToTileBuffer(uchar LevelSquarePosition) const
{
	igraph::GetItemGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (CMaterial(0)->GetItemColor() << 4), (GetBitmapPos().Y + (LevelSquarePosition << 4)), 0, 0, 16, 16);
}

bool scroll::CanBeRead(character* Reader) const
{
	return Reader->CanRead();
}

bool scrollofcreatemonster::Read(character* Reader)
{
	vector TryToCreate;
	for(;;) // Bug bug bug!
	{
		TryToCreate = (Reader->GetPos() + game::CMoveVector()[rand() % DIRECTION_COMMAND_KEYS]);
		if(game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->GetOverTerrain()->GetIsWalkable())
			break;
	}

	if(game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->CCharacter() == 0)
	{
		game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->AddCharacter(game::BalancedCreateMonster());
		if(Reader == game::GetPlayer()) ADD_MESSAGE("As you read the scroll a monster appears.");
	}
	else
	{
		if(Reader == game::GetPlayer()) ADD_MESSAGE("You feel a lost soul fly by you.");
	}
	return true;
}

bool scrollofteleport::Read(character* Reader)
{
	vector Pos;
	for(;;)
	{
		Pos = game::GetCurrentLevel()->RandomSquare(true);
		if(game::GetCurrentLevel()->GetLevelSquare(Pos)->CCharacter() == 0)
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
	ADD_MESSAGE("The %s touches %s.", CMaterial(0)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));
	CMaterial(0)->HitEffect(Enemy);
	}
}

void meleeweapon::ReceiveHitEffect(character* Enemy, character*)
{
	if(CMaterial(2))
	{
		if(Enemy == game::GetPlayer())
			ADD_MESSAGE("The %s reacts with you!", CMaterial(2)->CNAME(UNARTICLED));
		else
			ADD_MESSAGE("The %s reacts with %s.", CMaterial(2)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

		CMaterial(2)->HitEffect(Enemy);
	}
}

void meleeweapon::DipInto(item* DipTo)
{
	SetMaterial(2, DipTo->BeDippedInto());
	ADD_MESSAGE("%s is now covered with %s.", CNAME(DEFINITE), CMaterial(2)->CNAME(UNARTICLED));
}

material* lump::BeDippedInto(void)
{
	return CMaterial(0)->Clone(CMaterial(0)->TakeDipVolumeAway());
}

bool item::Consumable(character* Eater) const
{
	return Eater->ConsumeItemType(GetConsumeType());
}

ushort item::GetEmitation(void) const
{
	ushort Emitation = 0;

	for(ushort c = 0; c < GetMaterials(); c++)
		if(CMaterial(c))
			if(CMaterial(c)->GetEmitation() > Emitation)
				Emitation = CMaterial(c)->GetEmitation();

	return Emitation;
}

void potion::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
	game::GetCurrentLevel()->GetLevelSquare(ItemStack->GetPos())->GetStack()->AddItem(new brokenbottle);
	ItemStack->RemoveItem(ItemStack->SearchItem(this));
	if (IsShown) ADD_MESSAGE("The potion shatters to pieces.");
	delete this;
}


void potion::PositionedDrawToTileBuffer(uchar) const
{
	igraph::GetItemGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (Material[0]->GetItemColor() << 4), GetBitmapPos().Y+16, 0, 0, 16, 16);

	if(Material[1])
		igraph::GetItemGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (Material[1]->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
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

	CMaterial(0)->EatEffect(Eater, Amount, 0.25f);
	return (Amount > 99);
}

short item::CalculateOfferValue(char GodAlignment) const
{
	float OfferValue = 0;
	for(ushort c = 0; c < GetMaterials(); c++)
	{
		if(CMaterial(c))
		{
		if(CMaterial(c)->Alignment() == EVIL)
		{
			if(GodAlignment == EVIL || GodAlignment == NEUTRAL)
				OfferValue += Material[c]->GetVolume() * Material[c]->OfferValue();
			else
			if(GodAlignment == GOOD)
				OfferValue -= Material[c]->GetVolume() * Material[c]->OfferValue();
		}
		else if(CMaterial(c)->Alignment() == GOOD)
		{
			if(GodAlignment == GOOD || GodAlignment == NEUTRAL)
				OfferValue += Material[c]->GetVolume() * Material[c]->OfferValue();
			else
			if(GodAlignment == EVIL)
				OfferValue -= Material[c]->GetVolume() * Material[c]->OfferValue();
		}
		else
			OfferValue += Material[c]->GetVolume() * Material[c]->OfferValue();
		}
	}
	return short(OfferValue * (OfferModifier() / 250));
}

bool item::Fly(uchar Direction, ushort Force, stack* Start, bool Hostile)
{
	vector StartingPos = Start->GetPos();
	vector Pos = Start->GetPos();
	bool Breaks = false;
	float Speed = float(Force) / GetWeight() * 1500;
	for(;;)
	{
		if(!game::GetCurrentLevel()->GetLevelSquare(Pos + game::CMoveVector()[Direction])->GetOverTerrain()->GetIsWalkable())
		{
			Breaks = true;
			break;
		}
		else
		{
			vector OldPos = Pos;
			Pos += game::CMoveVector()[Direction];
			Speed *= 0.7f;
			if(Speed < 0.5)
				break;
			Start->MoveItem(Start->SearchItem(this), game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack());
			clock_t StartTime = clock();
			game::GetCurrentLevel()->GetLevelSquare(Pos)->ReEmitate();
			game::GetCurrentLevel()->GetLevelSquare(OldPos)->ReEmitate();
			game::DrawEverything(false);
			Start = game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack();

			if(game::GetCurrentLevel()->GetLevelSquare(Pos)->CCharacter())
			{
				if(Hostile)
					game::GetCurrentLevel()->GetLevelSquare(Pos)->CCharacter()->SetRelations(HOSTILE);
				if(HitCharacter(game::GetCurrentLevel()->GetLevelSquare(Pos)->CCharacter(), Speed, game::GetCurrentLevel()->GetLevelSquare(Pos)->CanBeSeen()))
					break;
			}
			while(clock() - StartTime < 0.2 * CLOCKS_PER_SEC)
			{

			}

		}
	}
	Start->MoveItem(Start->SearchItem(this), game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack());
	if(Breaks)
		ImpactDamage(Speed, game::GetCurrentLevel()->GetLevelSquare(Pos)->CanBeSeen(), game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack());
	if(Pos == StartingPos)
		return false;
	else
		return true;
}

bool item::HitCharacter(character* Dude, float Speed, bool CanBeSeen)
{
	if(Dude->Catches(this, Speed, CanBeSeen))
		return true;

	if(Dude->DodgesFlyingItem(this, Speed, CanBeSeen)) // Insert better formula for dodge
	{
		if(CanBeSeen) ADD_MESSAGE("%s misses %s.", CNAME(DEFINITE), Dude->CNAME(DEFINITE));
		return false;
	}
	Dude->HasBeenHitByItem(this, Speed, CanBeSeen);
	return true;
}

bool abone::Consume(character* Consumer, float Amount)
{
	if(Consumer == game::GetPlayer())
		ADD_MESSAGE("You consume %s.", CNAME(DEFINITE));
	else if(Consumer->GetLevelSquareUnder()->CanBeSeen())
		ADD_MESSAGE("%s consumes %s.", Consumer->CNAME(DEFINITE), CNAME(DEFINITE));
	CMaterial(0)->EatEffect(Consumer, Amount);
	return (Amount > 99);
}

ushort can::PrepareForConsuming(character* Consumer, stack* Stack)
{
	if(Consumer != game::GetPlayer() || game::BoolQuestion(std::string("Do you want to open ") + CNAME(DEFINITE) + " before eating it? [Y/N]"))
	{
		return TryToOpen(Stack);
	}
	else
		return 0xFFFF;
}

ushort item::PrepareForConsuming(character*, stack* Stack)
{
	return Stack->SearchItem(this);
}

float item::GetWeaponStrength(void) const
{
	return sqrt(float(GetFormModifier()) * Material[0]->GetHitValue() * GetWeight());
}

bool scrollofwishing::Read(character* Reader)
{
	std::string Temp = game::StringQuestion("What do you want to wish for?", 256);
	item* TempItem = game::CreateItem(Temp);

	if(TempItem)
	{
		Reader->GetLevelSquareUnder()->GetStack()->AddItem(TempItem);
		ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CNAME(INDEFINITE));
		return true;
	}
	else
		ADD_MESSAGE("There is no such item.");

	return false;
}

item* leftnutofperttu::CreateWishedItem(void) const
{
	return new cheapcopyofleftnutofperttu;
}

void item::DrawToTileBuffer(void) const
{
	PositionedDrawToTileBuffer(CENTER);
}

bool pickaxe::Apply(character* User)
{
	vector Temp;
	
	if((Temp = game::AskForDirectionVector("What direction do you want to dig?")) != vector(0,0))
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
