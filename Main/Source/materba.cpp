#include "materba.h"
#include "charba.h"
#include "save.h"
#include "error.h"
#include "rand.h"

std::string material::Name(uchar Case) const
{
	return Case & INDEFINEBIT ? Article() + " " + NameStem() : NameStem();
}

ushort material::TakeDipVolumeAway()
{
	ulong Amount = RAND() % 100;

	if(GetVolume() < Amount)
		Amount = RAND() % GetVolume();

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


void material::EatEffect(character*, float, float)
{ 
	ABORT("Calling material that does not have eat effect!");
}
