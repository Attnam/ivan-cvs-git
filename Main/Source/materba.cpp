#include "materba.h"
#include "charba.h"
#include "error.h"
#include "femath.h"
#include "save.h"

std::string material::Name(bool Articled, bool Adjective) const
{
  std::string Name = Articled ? Article() + " " : "";
  Name += Adjective ? AdjectiveStem() : NameStem();
  return Name;
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
  SaveFile << Type();
  SaveFile << Volume;
}

void material::Load(inputfile& SaveFile)
{
  SaveFile >> Volume;
}

void material::NormalFoodEffect(character* Eater, float Amount, float NPModifier)
{
  Eater->ReceiveNutrition(long(GetNutritionValue() * (Volume > Amount ? Amount : Volume) * GetDensity() * NPModifier / 50000));
}

void material::EatEffect(character*, float, float)
{ 
  ABORT("Calling material that does not have eat effect!");
}

bool material::CanBeDug(material* ShovelMaterial) const
{
  return ShovelMaterial->GetStrengthValue() > GetStrengthValue();
}

long material::CalculateOfferValue(char GodAlignment) const
{
  long Value = 0;

  if(GetAlignment() == EVIL)
    {
      if(GodAlignment == EVIL || GodAlignment == NEUTRAL)
	Value += GetVolume() * GetOfferValue();
      else
	if(GodAlignment == GOOD)
	  Value -= GetVolume() * GetOfferValue();
    }
  else if(GetAlignment() == GOOD)
    {
      if(GodAlignment == GOOD || GodAlignment == NEUTRAL)
	Value += GetVolume() * GetOfferValue();
      else
	if(GodAlignment == EVIL)
	  Value -= GetVolume() * GetOfferValue();
    }
  else
    Value += GetVolume() * GetOfferValue();

  return Value;
}
