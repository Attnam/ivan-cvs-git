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
#include "wskill.h"
#include "team.h"
#include "rand.h"
item::item(bool CreateMaterials, bool SetStats, bool AddToPool) : object(AddToPool)
{
	if(CreateMaterials || SetStats)
		ABORT("Boo!");
}

void item::PositionedDrawToTileBuffer(uchar) const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
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
		if(!game::IsValidPos(Pos + game::GetMoveVector(Direction)))
			break;

		if(!game::GetCurrentLevel()->GetLevelSquare(Pos + game::GetMoveVector(Direction))->GetOverLevelTerrain()->GetIsWalkable())
		{
			Breaks = true;
			break;
		}
		else
		{
			Pos += game::GetMoveVector(Direction);
			Speed *= 0.7f;
			if(Speed < 0.5)
				break;
			Start->MoveItem(Start->SearchItem(this), game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack());
			clock_t StartTime = clock();
			game::DrawEverything(false);
			Start = game::GetCurrentLevel()->GetLevelSquare(Pos)->GetStack();

			if(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter())
			{
				if(Hostile)
					game::GetPlayer()->Hostility(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter());

				if(HitCharacter(game::GetCurrentLevel()->GetLevelSquare(Pos)->GetCharacter(), Speed))
				{
					Breaks = true;
					break;
				}
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
		if(Dude->GetIsPlayer())
			ADD_MESSAGE("%s misses you.", CNAME(DEFINITE));
		else
			if(Dude->GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s misses %s.", CNAME(DEFINITE), Dude->CNAME(DEFINITE));

		return false;
	}

	Dude->HasBeenHitByItem(this, Speed);

	return true;
}

item* item::PrepareForConsuming(character*, stack* Stack)
{
	return this;
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
	return protocontainer<item>::GetProto(Type())->Clone();
}

bool item::Apply(character*, stack*)
{
	ADD_MESSAGE("You can't apply this!");

	return false;
}

bool item::Zap(character*, vector2d, uchar)
{
	 return false; 
}

bool item::Polymorph(stack* CurrentStack)
{
	if(RAND() % 3)
		CurrentStack->AddItem(protosystem::BalancedCreateItem());

	SetExists(false);
	return true;
}

void item::ChangeMainMaterial(material* NewMaterial)
{
	ChangeMaterial(0, NewMaterial);
}

uchar item::GetWeaponCategory() const
{
	return UNCATEGORIZED;
}

void item::StruckByWandOfStriking(stack* What) 
{ 
	ImpactDamage(10, What->GetLevelSquareUnder()->CanBeSeen(), What); 
}

bool item::Consume(character* Eater, float Amount)
{
	GetMaterial(0)->EatEffect(Eater, Amount, NPModifier());

	if(Eater->GetIsPlayer() && Eater->CheckCannibalism(GetMaterial(0)->GetType()))
	{
		game::DoEvilDeed(10);
		ADD_MESSAGE("You feel that this was an evil deed.");
	}

	return GetMaterial(0)->GetVolume() ? false : true;
}

bool item::IsBadFoodForAI(character* Eater) const
{
	if(Eater->CheckCannibalism(GetMaterial(0)->GetType()))
		return true;
	else
		return GetMaterial(GetConsumeMaterial())->GetIsBadFoodForAI();
}
