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

item::item(ushort Size2, bool CreateMaterials)
{
	Size = Size2;

	if(CreateMaterials)
		ABORT("Boo!");
}

void item::PositionedDrawToTileBuffer(uchar)
{
	igraph::CItemGraphic()->MaskedBlit(igraph::CTileBuffer(), CBitmapPos().X + (CMaterial(0)->CItemColor() << 4), CBitmapPos().Y, 0, 0, 16, 16);
}

void can::PositionedDrawToTileBuffer(uchar)
{
	igraph::CItemGraphic()->MaskedBlit(igraph::CTileBuffer(), CBitmapPos().X + (CMaterial(0)->CItemColor() << 4), CBitmapPos().Y, 0, 0, 16, 16);
}

ushort can::TryToOpen(stack* Stack)
{
	ADD_MESSAGE("You succeed in opening the can!");

	item* Lump = new lump(123, false);
	Lump->InitMaterials(CMaterial(1));

	ushort x = Stack->AddItem(Lump);

	SMaterial(1,0);

	return x;
}

ulong item::CWeight(void)
{
	ulong TotalWeight = 0;

	for(uchar c = 0; c < CMaterials(); c++)
		if(CMaterial(c))
			TotalWeight += CMaterial(c)->CWeight();

	return TotalWeight;
}

bool corpse::Consume(character* Eater, float Amount)
{
	if(Amount > 100) Amount = 100;
	if(Eater == game::CPlayer())
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
	if(Eater == game::CPlayer())
	{
		if(Amount == 100)
			ADD_MESSAGE("You eat %s.", CNAME(DEFINITE));
		else
			ADD_MESSAGE("You eat part of %s.", CNAME(DEFINITE));
	}
	CMaterial(1)->EatEffect(Eater, Amount);
	game::CCurrentLevel()->CLevelSquare(Eater->CPos())->CStack()->AddItem(new bananapeals);
	return (Amount > 99);
}

bool lump::Consume(character* Eater, float Amount)
{
	if(Amount > 100) Amount = 100;
	if(Eater == game::CPlayer())
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
	if(Eater == game::CPlayer())
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
		SMaterial(1,0);
	//	Eater->CLevelSquareUnder()->CStack()->AddItem(this);
	}	
	return false;
}

void lamp::PositionedDrawToTileBuffer(uchar LevelSquarePosition)
{
	igraph::CItemGraphic()->MaskedBlit(igraph::CTileBuffer(), CBitmapPos().X + (CMaterial(0)->CItemColor() << 4), (CBitmapPos().Y + (LevelSquarePosition << 4)), 0, 0, 16, 16);
}

bool item::CanBeRead(character*)
{
	return false;
}

bool item::Read(character*)
{
	return false;
}

bool scroll::CanBeRead(character* Reader)
{
	return Reader->CanRead();
}

bool scrollofcreatemonster::Read(character* Reader)
{
	vector TryToCreate;
	for(;;) // Bug bug bug!
	{
		TryToCreate = (Reader->CPos() + game::CMoveVector()[rand() % DIRECTION_COMMAND_KEYS]);
		if(game::CCurrentLevel()->CLevelSquare(TryToCreate)->COverTerrain()->CIsWalkable())
			break;
	}

	if(game::CCurrentLevel()->CLevelSquare(TryToCreate)->CCharacter() == 0)
	{
		game::CCurrentLevel()->CLevelSquare(TryToCreate)->AddCharacter(game::BalancedCreateMonster());
		if(Reader == game::CPlayer()) ADD_MESSAGE("As you read the scroll a monster appears.");
	}
	else
	{
		if(Reader == game::CPlayer()) ADD_MESSAGE("You feel a lost soul fly by you.");
	}
	return true;
}

bool scrollofteleport::Read(character* Reader)
{
	vector Pos;
	for(;;)
	{
		Pos = game::CCurrentLevel()->RandomSquare(true);
		if(game::CCurrentLevel()->CLevelSquare(Pos)->CCharacter() == 0)
			break;
	}
	if(Reader == game::CPlayer())
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
		if(Enemy == game::CPlayer())
			ADD_MESSAGE("The %s reacts with you!", CMaterial(2)->CNAME(UNARTICLED));
		else
			ADD_MESSAGE("The %s reacts with %s.", CMaterial(2)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

		CMaterial(2)->HitEffect(Enemy);
	}
}

void meleeweapon::DipInto(item* DipTo)
{
	SMaterial(2, DipTo->BeDippedInto());
	ADD_MESSAGE("%s is now covered with %s.", CNAME(DEFINITE), CMaterial(2)->CNAME(UNARTICLED));
}

material* lump::BeDippedInto(void)
{
	return game::CreateMaterial(CMaterial(0)->Type(), CMaterial(0)->TakeDipVolumeAway());
}

bool item::Consumable(character* Eater)
{
	return Eater->ConsumeItemType(GetConsumeType());
}

ushort item::CEmitation(void)
{
	ushort Emitation = 0;

	for(ushort c = 0; c < CMaterials(); c++)
		if(CMaterial(c))
			if(CMaterial(c)->CEmitation() > Emitation)
				Emitation = CMaterial(c)->CEmitation();

	return Emitation;
}

void potion::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
game::CCurrentLevel()->CLevelSquare(ItemStack->CPos())->CStack()->AddItem(new brokenbottle);
ItemStack->RemoveItem(ItemStack->FindItem(this));
if (IsShown) ADD_MESSAGE("The potion shatters to pieces.");
delete this;
}


void potion::PositionedDrawToTileBuffer(uchar)
{
	igraph::CItemGraphic()->MaskedBlit(igraph::CTileBuffer(), CBitmapPos().X + (Material[0]->CItemColor() << 4), CBitmapPos().Y+16, 0, 0, 16, 16);

	if(Material[1])
		igraph::CItemGraphic()->MaskedBlit(igraph::CTileBuffer(), CBitmapPos().X + (Material[1]->CItemColor() << 4), CBitmapPos().Y, 0, 0, 16, 16);
}

bool loaf::Consume(character* Eater, float Amount)
{
	if(Amount > 100) Amount = 100;
	if(Eater == game::CPlayer())
	{
		if(Amount == 100)
			ADD_MESSAGE("You eat %s.", CNAME(DEFINITE));
		else
			ADD_MESSAGE("You eat part of %s.", CNAME(DEFINITE));
	}

	CMaterial(0)->EatEffect(Eater, Amount, 0.25f);
	return (Amount > 99);
}

short item::CalculateOfferValue(char GodAlignment)
{
	float OfferValue = 0;
	for(ushort c = 0; c < CMaterials(); c++)
	{
		if(CMaterial(c))
		{
		if(CMaterial(c)->Alignment() == EVIL)
		{
			if(GodAlignment == EVIL || GodAlignment == NEUTRAL)
				OfferValue += Material[c]->CVolume() * Material[c]->OfferValue();
			else
			if(GodAlignment == GOOD)
				OfferValue -= Material[c]->CVolume() * Material[c]->OfferValue();
		}
		else if(CMaterial(c)->Alignment() == GOOD)
		{
			if(GodAlignment == GOOD || GodAlignment == NEUTRAL)
				OfferValue += Material[c]->CVolume() * Material[c]->OfferValue();
			else
			if(GodAlignment == EVIL)
				OfferValue -= Material[c]->CVolume() * Material[c]->OfferValue();
		}
		else
			OfferValue += Material[c]->CVolume() * Material[c]->OfferValue();
		}
	}
	return short(OfferValue * (OfferModifier() / 250));
}

bool item::Destroyable(void)
{
	if(IsHeadOfElpuri() || IsPerttusNut() || IsMaakotkaShirt())
		return false;
	else
		return true;
}

bool item::Fly(uchar Direction, ushort Force, stack* Start, bool Hostile)
{
	vector StartingPos = Start->CPos();
	vector Pos = Start->CPos();
	bool Breaks = false;
	float Speed = float(Force) / CWeight() * 1500;
	for(;;)
	{
		if(!game::CCurrentLevel()->CLevelSquare(Pos + game::CMoveVector()[Direction])->COverTerrain()->CIsWalkable())
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
			Start->MoveItem(Start->SearchItem(this), game::CCurrentLevel()->CLevelSquare(Pos)->CStack());
			clock_t StartTime = clock();
			game::CCurrentLevel()->CLevelSquare(Pos)->ReEmitate();
			game::CCurrentLevel()->CLevelSquare(OldPos)->ReEmitate();
			game::DrawEverything(false);
			Start = game::CCurrentLevel()->CLevelSquare(Pos)->CStack();

			if(game::CCurrentLevel()->CLevelSquare(Pos)->CCharacter())
			{
				if(Hostile)
					game::CCurrentLevel()->CLevelSquare(Pos)->CCharacter()->SRelations(HOSTILE);
				if(HitCharacter(game::CCurrentLevel()->CLevelSquare(Pos)->CCharacter(), Speed, game::CCurrentLevel()->CLevelSquare(Pos)->CanBeSeen()))
					break;
			}
			while(clock() - StartTime < 0.2 * CLOCKS_PER_SEC)
			{

			}

		}
	}
	Start->MoveItem(Start->SearchItem(this), game::CCurrentLevel()->CLevelSquare(Pos)->CStack());
	if(Breaks)
		ImpactDamage(Speed, game::CCurrentLevel()->CLevelSquare(Pos)->CanBeSeen(), game::CCurrentLevel()->CLevelSquare(Pos)->CStack());
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
	if(Consumer == game::CPlayer())
		ADD_MESSAGE("You consume %s.", CNAME(DEFINITE));
	else if(Consumer->CLevelSquareUnder()->CanBeSeen())
		ADD_MESSAGE("%s consumes %s.", Consumer->CNAME(DEFINITE), CNAME(DEFINITE));
	CMaterial(0)->EatEffect(Consumer, Amount);
	return (Amount > 99);
}

/*void item::ReactWith(character* Dude)
{
	if(CanBeDipped())
		if(CMaterial(GetDipMaterialNumber()))
			CMaterial(GetDipMaterialNumber())->HitEffect(Dude);
}*/

ushort can::PrepareForConsuming(character* Consumer, stack* Stack)
{
	if(Consumer != game::CPlayer() || game::BoolQuestion(std::string("Do you want to open ") + CNAME(DEFINITE) + " before eating it? [Y/N]"))
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

float item::GetWeaponStrength(void)
{
	return sqrt(float(CFormModifier()) * Material[0]->GetHitValue() * CWeight());
}

bool scrollofwishing::Read(character* Reader)
{
	std::string Temp = game::StringQuestion("What do you want to wish for?", 256);
	item* TempItem = game::CreateItem(Temp);

	if(TempItem)
	{
		Reader->CLevelSquareUnder()->CStack()->AddItem(TempItem);
		ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CNAME(INDEFINITE));
		return true;
	}
	else
		ADD_MESSAGE("There is no such item.");

	return false;
}

item* leftnutofperttu::CreateWishedItem(void)
{
	return new cheapcopyofleftnutofperttu;
}

void item::DrawToTileBuffer(void)
{
	PositionedDrawToTileBuffer(CENTER);
}

bool pickaxe::Apply(character* User)
{
	vector Temp;
	
	if((Temp = game::AskForDirectionVector("What direction do you want to dig?")) != vector(0,0))
	{

		if(game::CCurrentLevel()->CLevelSquare(User->CPos() + Temp)->Dig(User, this))
		{
			User->SAP(ushort(User->CAP() + float(User->CStrength() * 2 - 200)));
			User->SStrengthExperience(User->CStrengthExperience() + 50);
			return true;
		}
	}	
	return false;
}