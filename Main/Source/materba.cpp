#include "materba.h"
#include "charba.h"
#include "save.h"

std::string material::Name(uchar Case) const
{
	return Case & INDEFINEBIT ? Article() + " " + NameStem() : NameStem();
}

ushort material::TakeDipVolumeAway()
{
	ulong Amount = rand() % 100;

	if(GetVolume() < Amount)
		Amount = rand() % GetVolume();

	SetVolume(GetVolume() - Amount);

	return Amount;
}

void material::Save(outputfile& SaveFile) const
{
	typeable::Save(SaveFile);
	SaveFile << Volume;
}

void material::Load(inputfile& SaveFile)
{
	typeable::Load(SaveFile);
	SaveFile >> Volume;
}

void material::NormalFoodEffect(character* Eater, float Amount, float NPModifier)
{
	Eater->ReceiveNutrition(long(NutritionValue() * (Volume > Amount ? Amount : Volume) * GetDensity() * NPModifier / 1000000));
}

material* material::CreateWishedMaterial(ulong OldVolume) const
{
	return protocontainer<material>::GetProto(Type())->Clone(OldVolume);
}
