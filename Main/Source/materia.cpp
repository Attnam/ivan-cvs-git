/* Compiled through materset.cpp */

materialprototype::materialprototype(materialprototype* Base, material* (*Cloner)(ushort, ulong, bool), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId) { Index = protocontainer<material>::Add(this); }
const materialdatabase& materialprototype::ChooseBaseForConfig(ushort) { return Config.begin()->second; }

void material::InstallDataBase() { ::database<material>::InstallDataBase(this); }
ulong material::GetRawPrice() const { return GetPriceModifier() * GetWeight() / 10000; }
bool material::CanBeDug(material* ShovelMaterial) const { return ShovelMaterial->GetStrengthValue() > GetStrengthValue(); }
ulong material::GetTotalExplosivePower() const { return ulong(float(Volume) * GetExplosivePower() / 1000000); }
std::string material::GetConsumeVerb() const { return "eating"; }

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
  ulong Amount = Min(500UL, GetVolume());
  EditVolume(-Amount);
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

bool material::Effect(character* Eater, ulong Amount)
{
  /* Receivexxx should return bool! */

  Amount = Amount * GetEffectStrength() / 100;

  if(!Amount)
    return false;

  switch(GetEffect())
    {
    case EFFECT_POISON: Eater->BeginTemporaryState(POISONED, Amount); return true;
    case EFFECT_DARKNESS: Eater->ReceiveDarkness(Amount); return true;
    case EFFECT_OMMEL_URINE: Eater->ReceiveOmmelUrine(Amount); return true;
    case EFFECT_PEPSI: Eater->ReceivePepsi(Amount); return true;
    case EFFECT_KOBOLD_FLESH: Eater->ReceiveKoboldFlesh(Amount); return true;
    case EFFECT_HEAL: Eater->ReceiveHeal(Amount); return true;
    case EFFECT_LYCANTHROPY: Eater->BeginTemporaryState(LYCANTHROPY, Amount); return true;
    case EFFECT_SCHOOL_FOOD: Eater->ReceiveSchoolFood(Amount); return true;
    case EFFECT_ANTIDOTE: Eater->ReceiveAntidote(Amount); return true;
    case EFFECT_CONFUSE: Eater->BeginTemporaryState(CONFUSED, Amount); return true;
    case EFFECT_POLYMORPH: Eater->BeginTemporaryState(POLYMORPH, Amount); return true;
    case EFFECT_ESP: Eater->BeginTemporaryState(ESP, Amount); return true;
    case EFFECT_SKUNK_SMELL: Eater->BeginTemporaryState(POISONED, Amount); return true;
    default: return false;
    }
}

/* NPModifier / 100 = percent */

void material::EatEffect(character* Eater, ulong Amount, ulong NPModifier)
{
  Amount = Volume > Amount ? Amount : Volume;
  Effect(Eater, Amount);
  Eater->ReceiveNutrition(ulonglong(GetNutritionValue()) * Amount * NPModifier * 15 / 1000000);
  EditVolume(-Amount);
}

bool material::HitEffect(character* Enemy)
{
  switch(GetHitMessage())
    {
    case HM_SCHOOL_FOOD: Enemy->AddSchoolFoodHitMessage(); break;
    case HM_FROG_FLESH: Enemy->AddFrogFleshConsumeEndMessage(); break;
    case HM_OMMEL_URINE: Enemy->AddOmmelUrineConsumeEndMessage(); break;
    case HM_PEPSI: Enemy->AddPepsiConsumeEndMessage(); break;
    case HM_KOBOLD_FLESH: Enemy->AddKoboldFleshHitMessage(); break;
    case HM_HEALING_LIQUID: Enemy->AddHealingLiquidConsumeEndMessage(); break;
    case HM_ANTIDOTE: Enemy->AddAntidoteConsumeEndMessage(); break;
    case HM_CONFUSE: Enemy->AddConfuseHitMessage();
    }

  ulong Amount = Min(100UL, GetVolume());
  EditVolume(-Amount);
  return Effect(Enemy, Amount);
}

void material::AddConsumeEndMessage(character* Eater) const
{
  switch(GetConsumeEndMessage())
    {
    case CEM_SCHOOL_FOOD: Eater->AddSchoolFoodConsumeEndMessage(); break;
    case CEM_BONE: Eater->AddBoneConsumeEndMessage(); break;
    case CEM_FROG_FLESH: Eater->AddFrogFleshConsumeEndMessage(); break;
    case CEM_OMMEL_URINE: Eater->AddOmmelUrineConsumeEndMessage(); break;
    case CEM_PEPSI: Eater->AddPepsiConsumeEndMessage(); break;
    case CEM_KOBOLD_FLESH: Eater->AddKoboldFleshConsumeEndMessage(); break;
    case CEM_HEALING_LIQUID: Eater->AddHealingLiquidConsumeEndMessage(); break;
    case CEM_ANTIDOTE: Eater->AddAntidoteConsumeEndMessage(); break;
    case CEM_ESP: Eater->AddESPConsumeMessage(); break;
    }
}

material* materialprototype::CloneAndLoad(inputfile& SaveFile) const
{
  material* Material = Cloner(0, 0, true);
  Material->Load(SaveFile);
  return Material;
}

material* material::MakeMaterial(ushort Config)
{
  if(!Config)
    return 0;

  switch(Config >> 12)
    {
    case MATERIAL_ID >> 12: return new material(Config, 0);
    case ORGANIC_SUBSTANCE_ID >> 12: return new organicsubstance(Config, 0);
    case GAS_ID >> 12: return new gas(Config, 0);
    case LIQUID_ID >> 12: return new liquid(Config, 0);
    case FLESH_ID >> 12: return new flesh(Config, 0);
    case POWDER_ID >> 12: return new powder(Config, 0);
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
    case MATERIAL_ID >> 12: return new material(Config, Volume);
    case ORGANIC_SUBSTANCE_ID >> 12: return new organicsubstance(Config, Volume);
    case GAS_ID >> 12: return new gas(Config, Volume);
    case LIQUID_ID >> 12: return new liquid(Config, Volume);
    case FLESH_ID >> 12: return new flesh(Config, Volume);
    case POWDER_ID >> 12: return new powder(Config, Volume);
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
  return ulonglong(GetNutritionValue()) * What->GetNPModifier() * GetVolume() * 15 / 1000000;
}

bool material::CanBeEatenByAI(const character* Eater) const
{
  return Eater->GetAttribute(WISDOM) < GetConsumeWisdomLimit() && !GetSpoilLevel();
}

bool material::IsStupidToConsume()
{
  return GetConsumeWisdomLimit() != NO_LIMIT;
}

bool material::BreatheEffect(character* Enemy)
{
  return Effect(Enemy, GetVolume() / 10);
}
