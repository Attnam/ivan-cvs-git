#include <ctime>
#include <cmath>

#include "itemba.h"
#include "error.h"
#include "igraph.h"
#include "bitmap.h"
#include "charba.h"
#include "stack.h"
#include "level.h"
#include "lsquare.h"
#include "lterraba.h"
#include "message.h"

item::item(bool CreateMaterials, bool SetStats, bool AddToPool) : object(AddToPool)
{
	if(CreateMaterials || SetStats)
		ABORT("Boo!");
}

void item::PositionedDrawToTileBuffer(uchar) const
{
	igraph::GetItemGraphic()->MaskedBlit(igraph::GetTileBuffer(), GetBitmapPos().X + (GetMaterial(0)->GetItemColor() << 4), GetBitmapPos().Y, 0, 0, 16, 16);
}

ulong item::GetWeight() const
{
	ulong TotalWeight = 0;

	for(uchar c = 0; c < GetMaterials(); ++c)
		if(GetMaterial(c))
			TotalWeight += GetMaterial(c)->GetWeight();

	return TotalWeight;
}

bool item::Consumable(character* Eater) const
{
	return Eater->ConsumeItemType(GetConsumeType());
}

ushort item::GetEmitation() const
{
	ushort Emitation = 0;

	for(ushort c = 0; c < GetMaterials(); ++c)
		if(GetMaterial(c))
			if(GetMaterial(c)->GetEmitation() > Emitation)
				Emitation = GetMaterial(c)->GetEmitation();

	return Emitation;
}

short item::CalculateOfferValue(char GodAlignment) const
{
	float OfferValue = 0;
	for(ushort c = 0; c < GetMaterials(); ++c)
	{
		if(GetMaterial(c))
		{
		if(GetMaterial(c)->Alignment() == EVIL)
		{
			if(GodAlignment == EVIL || GodAlignment == NEUTRAL)
				OfferValue += Material[c]->GetVolume() * Material[c]->OfferValue();
			else
			if(GodAlignment == GOOD)
				OfferValue -= Material[c]->GetVolume() * Material[c]->OfferValue();
		}
		else if(GetMaterial(c)->Alignment() == GOOD)
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
	vector2d StartingPos = Start->GetPos();
	vector2d Pos = Start->GetPos();
	bool Breaks = false;
	float Speed = float(Force) / GetWeight() * 1500;

	for(;;)
	{
		if(!game::GetCurrentLevel()->GetLevelSquare(Pos + game::GetMoveVector(Direction))->GetOverLevelTerrain()->GetIsWalkable())
		{
			Breaks = true;
			break;
		}
		else
		{
			vector2d OldPos = Pos;
			Pos += game::GetMoveVector(Direction);
			Speed *= 0.7f;
			if(Speed < 0.5)
				break;
			Start->MoveItem(Start->SearchItem(this), game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack());
			clock_t StartTime = clock();
			game::GetCurrentLevel()->GetLevelSquare(Pos)->ReEmitate();
			game::GetCurrentLevel()->GetLevelSquare(OldPos)->ReEmitate();
			game::DrawEverything(false);
			Start = game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack();

			if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter())
			{
				if(Hostile)
					game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter()->SetRelations(HOSTILE);
				if(HitCharacter(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter(), Speed))
					break;
			}

			while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
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

bool item::HitCharacter(character* Dude, float Speed)
{
	if(Dude->Catches(this, Speed))
		return true;

	if(Dude->DodgesFlyingItem(this, Speed)) // Insert better formula for dodge
	{
		if(GetSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s misses %s.", CNAME(DEFINITE), Dude->CNAME(DEFINITE));

		return false;
	}

	Dude->HasBeenHitByItem(this, Speed);

	return true;
}

ushort item::PrepareForConsuming(character*, stack* Stack)
{
	return Stack->SearchItem(this);
}

float item::GetWeaponStrength() const
{
	return sqrt(float(GetFormModifier()) * Material[0]->GetHitValue() * GetWeight());
}

void item::DrawToTileBuffer() const
{
	PositionedDrawToTileBuffer(CENTER);
}

item* item::CreateWishedItem() const
{
	return protocontainer<item>::GetProto(Type())->Clone(); //GGG
}

bool item::Apply(character*)
{
	ADD_MESSAGE("You can't apply this!");

	return false;
}

bool item::Zap(vector2d, uchar)
{
	 return false; 
}

bool item::Polymorph(stack* CurrentStack)
{
	CurrentStack->AddItem(protosystem::BalancedCreateItem());
	CurrentStack->RemoveItem(CurrentStack->SearchItem(this));
	SetExists(false);
	//delete this;
	return true;
}

