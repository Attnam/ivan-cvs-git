#include "materde.h"
#include "charba.h"
#include "message.h"
#include "lsquare.h"
#include "rand.h"

void schoolfood::EatEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->ReceiveSchoolFoodEffect(Volume > Amount ? Amount : Volume);
	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

void schoolfood::HitEffect(character* Enemy)
{
	Enemy->ReceiveSchoolFoodEffect(Volume);
}

void frogflesh::EatEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->Darkness(Volume > Amount ? Amount : Volume);
	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

void frogflesh::HitEffect(character* Enemy)
{
	Enemy->Darkness(Volume);
}

void omleurine::EatEffect(character* Eater, float Amount, float NPModifier)
{
	if(Amount >= Volume)
		Eater->ReceiveOmleUrineEffect(Volume > Amount ? Amount : Volume);

	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

void omleurine::HitEffect(character* Enemy)
{
	Enemy->ReceiveOmleUrineEffect(Volume);
}

void pepsi::EatEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->ReceivePepsiEffect(Volume > Amount ? Amount : Volume);
	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

void pepsi::HitEffect(character* Enemy)
{
	Enemy->ReceivePepsiEffect(Volume);
}

void bone::EatEffect(character* Eater, float Amount, float NPModifier)
{
	if(Amount >= Volume)
	{
		if(Eater == game::GetPlayer())
			ADD_MESSAGE("You feel like a hippie.");
		else
			if(GetMotherObject()->GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s barks happily.", Eater->CNAME(DEFINITE));
	}

	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

ushort goodleather::GetColor() const { return MAKE_RGB(160, 160, 0); }
ushort neutralleather::GetColor() const { return MAKE_RGB(20, 120, 200); }
ushort evilleather::GetColor() const { return MAKE_RGB(200, 0, 0); }
