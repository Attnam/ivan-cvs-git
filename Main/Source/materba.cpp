#include "materde.h"
#include "charba.h"
#include "error.h"
#include "femath.h"
#include "save.h"
#include "proto.h"

std::string material::Name(bool Articled, bool Adjective) const
{
  std::string Name = Articled ? GetArticle() + " " : "";
  Name += Adjective ? GetAdjectiveStem() : GetNameStem();
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
  SaveFile << GetType();
  SaveFile << Volume << Config;
}

void material::Load(inputfile& SaveFile)
{
  SaveFile >> Volume >> Config;
  InstallDataBase();
}

void material::Effect(character* Eater, long Amount)
{
  switch(GetEffect())
    {
    case ENOTHING: Eater->ReceivePoison(Amount); break;
    case EDARKNESS: Eater->ReceiveDarkness(Amount); break;
    case EOMLEURINE: Eater->ReceiveOmleUrine(Amount); break;
    case EPEPSI: Eater->ReceivePepsi(Amount); break;
    case EKOBOLDFLESH: Eater->ReceiveKoboldFlesh(Amount); break;
    case EHEAL: Eater->ReceiveHeal(Amount); break;
    }
}

void material::EatEffect(character* Eater, long Amount, float NPModifier)
{
  Amount = Volume > Amount ? Amount : Volume;
  Effect(Eater, Amount);
  Eater->ReceiveNutrition(long(GetNutritionValue() * Amount * GetDensity() * NPModifier / 50000));
  Volume -= Amount;
}

void material::HitEffect(character* Enemy)
{
  Effect(Enemy, Volume); // should be less than Volume!

  switch(GetHitMessage())
    {
    case HM_SCHOOLFOOD: Enemy->AddSchoolFoodHitMessage(); break;
    case HM_FROGFLESH: Enemy->AddFrogFleshConsumeEndMessage(); break;
    case HM_OMLEURINE: Enemy->AddOmleUrineConsumeEndMessage(); break;
    case HM_PEPSI: Enemy->AddPepsiConsumeEndMessage(); break;
    case HM_KOBOLDFLESH: Enemy->AddKoboldFleshHitMessage(); break;
    case HM_HEALINGLIQUID: Enemy->AddHealingLiquidConsumeEndMessage(); break;
    }
}

void material::AddConsumeEndMessage(character* Eater) const
{
  switch(GetConsumeEndMessage())
    {
    case CEM_SCHOOLFOOD: Eater->AddSchoolFoodConsumeEndMessage(); break;
    case CEM_BONE: Eater->AddBoneConsumeEndMessage(); break;
    case CEM_FROGFLESH: Eater->AddFrogFleshConsumeEndMessage(); break;
    case CEM_OMLEURINE: Eater->AddOmleUrineConsumeEndMessage(); break;
    case CEM_PEPSI: Eater->AddPepsiConsumeEndMessage(); break;
    case CEM_KOBOLDFLESH: Eater->AddKoboldFleshConsumeEndMessage(); break;
    case CEM_HEALINGLIQUID: Eater->AddHealingLiquidConsumeEndMessage(); break;
    }
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

material* materialprototype::Clone(ushort Config, ulong Volume) const
{
  material* Material = Clone(Config);
  Material->SetVolume(Volume);
  return Material;
}

material* materialprototype::CloneAndLoad(inputfile& SaveFile) const
{
  material* Material = Clone(0);
  Material->Load(SaveFile);
  return Material;
}

materialprototype::materialprototype(materialprototype* Base) : Base(Base)
{
  Index = protocontainer<material>::Add(this);
}

void material::InstallDataBase()
{
  const material::databasemap& Configs = GetProtoType()->GetConfig();
  material::databasemap::const_iterator i = Configs.find(Config);

  if(i != Configs.end())
    DataBase = &i->second;
  else
    ABORT("Undefined material configuration #%d sought!", Config);
}

material* material::MakeMaterial(ushort Config)
{
  switch(Config >> 12)
    {
    case 1: return new material(Config);
    case 2: return new organicsubstance(Config);
    case 3: return new gas(Config);
    case 4: return new liquid(Config);
    case 5: return new flesh(Config);
    default:
      ABORT("Odd material configuration number %d requested!", Config);
      return 0;
    }
  /*if(Config )
    return new material(Config);
  else if(Config < ORGANIC_SEPARATOR)
    return new organicsubstance(Config);
  else if(Config < GAS_SEPARATOR)
    return new gas(Config);
  else if(Config < LIQUID_SEPARATOR)
    return new liquid(Config);
  else if(Config < FLESH_SEPARATOR)
    return new flesh(Config);
  else
    */
}

material* material::MakeMaterial(ushort Config, ulong Volume)
{
  switch(Config >> 12)
    {
    case 1: return new material(Config);
    case 2: return new organicsubstance(Config);
    case 3: return new gas(Config);
    case 4: return new liquid(Config);
    case 5: return new flesh(Config);
    default:
      ABORT("Odd material configuration number %d of volume %d requested!", Config, Volume);
      return 0;
    }

  /*if(Config < MATERIAL_SEPARATOR)
    return new material(Config, Volume);
  else if(Config < ORGANIC_SEPARATOR)
    return new organicsubstance(Config, Volume);
  else if(Config < GAS_SEPARATOR)
    return new gas(Config, Volume);
  else if(Config < LIQUID_SEPARATOR)
    return new liquid(Config, Volume);
  else if(Config < FLESH_SEPARATOR)
    return new flesh(Config, Volume);
  else
    {
      ABORT("Odd material configuration number %d of volume %d requested!", Config, Volume);
      return 0;
    }*/
}
