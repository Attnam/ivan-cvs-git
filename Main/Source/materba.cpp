#include "materde.h"
#include "charba.h"
#include "error.h"
#include "femath.h"
#include "save.h"
#include "proto.h"
#include "festring.h"
#include "database.h"

void material::AddName(std::string& Name, bool Articled, bool Adjective) const
{
  if(Articled)
    Name << GetArticle() << " ";

  Name << (Adjective ? GetAdjectiveStem() : GetNameStem());
}

std::string material::GetName(bool Articled, bool Adjective) const
{
  std::string Name;
  AddName(Name, Articled, Adjective);
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
  CalculateWeight();
}

bool material::Effect(character* Eater, long Amount)
{
  /* Receivexxx should return bool! */

  switch(GetEffect())
    {
    case EPOISON: Eater->BeginTemporaryState(POISON, Amount / 10); return true;
    case EDARKNESS: Eater->ReceiveDarkness(Amount); return true;
    case EOMLEURINE: Eater->ReceiveOmleUrine(Amount); return true;
    case EPEPSI: Eater->ReceivePepsi(Amount); return true;
    case EKOBOLDFLESH: Eater->ReceiveKoboldFlesh(Amount); return true;
    case EHEAL: Eater->ReceiveHeal(Amount); return true;
    case ELYCANTHROPY: Eater->BeginTemporaryState(LYCANTHROPY, Amount / 10); return true;
    case ESCHOOLFOOD: Eater->ReceiveSchoolFood(Amount); return true;
    default: return false;
    }
}

void material::EatEffect(character* Eater, ulong Amount, float NPModifier)
{
  Amount = Volume > Amount ? Amount : Volume;
  Effect(Eater, Amount);
  Eater->ReceiveNutrition(long(GetNutritionValue() * Amount * GetDensity() * NPModifier / 500000000));
  Volume -= Amount;
}

bool material::HitEffect(character* Enemy)
{
  switch(GetHitMessage())
    {
    case HM_SCHOOLFOOD: Enemy->AddSchoolFoodHitMessage(); break;
    case HM_FROGFLESH: Enemy->AddFrogFleshConsumeEndMessage(); break;
    case HM_OMLEURINE: Enemy->AddOmleUrineConsumeEndMessage(); break;
    case HM_PEPSI: Enemy->AddPepsiConsumeEndMessage(); break;
    case HM_KOBOLDFLESH: Enemy->AddKoboldFleshHitMessage(); break;
    case HM_HEALINGLIQUID: Enemy->AddHealingLiquidConsumeEndMessage(); break;
    }

  return Effect(Enemy, Volume); // should be less than Volume!
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

long material::GetOfferValue(char GodAlignment) const
{
  long Value = 0;

  if(GetAlignment() == EVIL)
    {
      if(GodAlignment == EVIL || GodAlignment == NEUTRAL)
	Value += GetVolume() * GetOfferModifier();
      else
	if(GodAlignment == GOOD)
	  Value -= GetVolume() * GetOfferModifier();
    }
  else if(GetAlignment() == GOOD)
    {
      if(GodAlignment == GOOD || GodAlignment == NEUTRAL)
	Value += GetVolume() * GetOfferModifier();
      else
	if(GodAlignment == EVIL)
	  Value -= GetVolume() * GetOfferModifier();
    }
  else
    Value += GetVolume() * GetOfferModifier();

  return Value;
}

material* materialprototype::CloneAndLoad(inputfile& SaveFile) const
{
  material* Material = Cloner(0, 0, true);
  Material->Load(SaveFile);
  return Material;
}

materialprototype::materialprototype(materialprototype* Base, material* (*Cloner)(ushort, ulong, bool), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<material>::Add(this);
}

void material::InstallDataBase()
{
  ::database<material>::InstallDataBase(this);

  /*if(!Config)
    return; // loading

  const material::databasemap& Configs = GetProtoType()->GetConfig();
  material::databasemap::const_iterator i = Configs.find(Config);

  if(i != Configs.end())
    DataBase = &i->second;
  else
    ABORT("Undefined material configuration #%d sought!", Config);*/
}

material* material::MakeMaterial(ushort Config)
{
  if(!Config)
    return 0;

  switch(Config >> 12)
    {
    case FIRSTMATERIAL >> 12: return new material(Config, 0);
    case FIRSTORGANICSUBSTANCE >> 12: return new organicsubstance(Config, 0);
    case FIRSTGAS >> 12: return new gas(Config, 0);
    case FIRSTLIQUID >> 12: return new liquid(Config, 0);
    case FIRSTFLESH >> 12: return new flesh(Config, 0);
    default:
      ABORT("Odd material configuration number %d requested!", Config);
      return 0;
    }
}

material* material::MakeMaterial(ushort Config, ulong Volume)
{
  if(!Config)
    return 0;

  switch(Config >> 12)
    {
    case FIRSTMATERIAL >> 12: return new material(Config, Volume);
    case FIRSTORGANICSUBSTANCE >> 12: return new organicsubstance(Config, Volume);
    case FIRSTGAS >> 12: return new gas(Config, Volume);
    case FIRSTLIQUID >> 12: return new liquid(Config, Volume);
    case FIRSTFLESH >> 12: return new flesh(Config, Volume);
    default:
      ABORT("Odd material configuration number %d of volume %d requested!", Config, Volume);
      return 0;
    }
}

void material::SetVolume(ulong What)
{
  Volume = What;
  CalculateWeight();

  if(MotherEntity)
    MotherEntity->SignalVolumeAndWeightChange();
}

void material::SetConfig(ushort NewConfig)
{
  Config = NewConfig;
  InstallDataBase();
  CalculateWeight();
}

void material::Initialize(ushort NewConfig, ulong InitVolume, bool Load)
{
  if(!Load)
    {
      Config = NewConfig;
      InstallDataBase();
      Volume = InitVolume;
      CalculateWeight();
    }

  VirtualConstructor(Load);
}

ulong material::GetTotalNutritionValue(const item* What) const
{ 
  return GetNutritionValue() * GetWeight() * What->GetNPModifier() / 500000000; 
}

