#include "char.h"
#include "material.h"

ushort material::TakeDipVolumeAway(void)
{
	ulong Amount = rand() % 100;

	if(GetVolume() < Amount)
		Amount = rand() % GetVolume();

	SetVolume(GetVolume() - Amount);

	return Amount;
}

void material::Save(std::ofstream* SaveFile) const
{
	ushort TypeVar = Type();
	SaveFile->write((char*)&TypeVar, sizeof(TypeVar));
	SaveFile->write((char*)&Volume, sizeof(Volume));
}

void material::Load(std::ifstream* SaveFile)
{
	SaveFile->read((char*)&Volume, sizeof(Volume));
}

void material::NormalFoodEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->ReceiveNutrition(long(NutritionValue() * GetWeight() * NPModifier) * Amount / 100);
}

void schoolfood::EatEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->ReceiveSchoolFoodEffect(Volume * Amount / 100);
	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

void schoolfood::HitEffect(character* Enemy)
{
	Enemy->ReceiveSchoolFoodEffect(Volume);
}

void darkfrogflesh::EatEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->Darkness(Volume * Amount / 100);
	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

void darkfrogflesh::HitEffect(character* Enemy)
{
	Enemy->Darkness(Volume);
}

void omleurine::EatEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->ReceiveOmleUrineEffect(Volume * Amount / 100);
	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

void omleurine::HitEffect(character* Enemy)
{
	Enemy->ReceiveOmleUrineEffect(Volume);
}

void pepsi::EatEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->ReceivePepsiEffect(Volume * Amount / 100);
	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}

void pepsi::HitEffect(character* Enemy)
{
	Enemy->ReceivePepsiEffect(Volume);
}

void bone::EatEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->SetRelations(NEUTRAL);
	if(Eater == game::GetPlayer())
		ADD_MESSAGE("You feel like a hippie.");
	else
		ADD_MESSAGE("%s doesn't seem to care about you anymore.", Eater->CNAME(DEFINITE));
	NormalFoodEffect(Eater, Amount, NPModifier);
	MinusAmount(Amount);
}
