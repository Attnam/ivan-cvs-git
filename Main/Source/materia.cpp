/* Compiled through materset.cpp */

materialprototype::materialprototype(materialprototype* Base, material* (*Cloner)(ushort, ulong, bool), const char* ClassID) : Base(Base), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<material>::Add(this); }
const materialdatabase& materialprototype::ChooseBaseForConfig(ushort) { return Config.begin()->second; }

void material::InstallDataBase(ushort Config) { databasecreator<material>::InstallDataBase(this, Config); }
ulong material::GetRawPrice() const { return GetPriceModifier() * GetWeight() / 10000; }
bool material::CanBeDug(material* ShovelMaterial) const { return ShovelMaterial->GetStrengthValue() > GetStrengthValue(); }
ulong material::GetTotalExplosivePower() const { return ulong(float(Volume) * GetExplosivePower() / 1000000); }
const char* material::GetConsumeVerb() const { return "eating"; }

materialpredicate TrueMaterialPredicate = &material::True;

void material::AddName(festring& Name, bool Articled, bool Adjective) const
{
  if(Articled)
    Name << GetArticle() << ' ';

  Name << (Adjective ? GetAdjectiveStem() : GetNameStem());
}

festring material::GetName(bool Articled, bool Adjective) const
{
  static festring Name;
  Name.Empty();
  AddName(Name, Articled, Adjective);
  return Name;
}

ushort material::TakeDipVolumeAway()
{
  ulong Amount = 100;

  if(Amount < Volume)
    EditVolume(-Amount);
  else
    {
      Amount = Volume;
      delete MotherEntity->RemoveMaterial(this);
    }

  return Amount;
}

void material::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  SaveFile << Volume;
  SaveFile << GetConfig();
}

void material::Load(inputfile& SaveFile)
{
  SaveFile >> Volume;
  InstallDataBase(ReadType<ushort>(SaveFile));
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
    case EFFECT_POISON: Eater->BeginTemporaryState(POISONED, Amount); break;
    case EFFECT_DARKNESS: Eater->ReceiveDarkness(Amount); break;
    case EFFECT_OMMEL_URINE: Eater->ReceiveOmmelUrine(Amount); break;
    case EFFECT_PEPSI: Eater->ReceivePepsi(Amount); break;
    case EFFECT_KOBOLD_FLESH: Eater->ReceiveKoboldFlesh(Amount); break;
    case EFFECT_HEAL: Eater->ReceiveHeal(Amount); break;
    case EFFECT_LYCANTHROPY: Eater->BeginTemporaryState(LYCANTHROPY, Amount); break;
    case EFFECT_SCHOOL_FOOD: Eater->ReceiveSchoolFood(Amount); break;
    case EFFECT_ANTIDOTE: Eater->ReceiveAntidote(Amount); break;
    case EFFECT_CONFUSE: Eater->BeginTemporaryState(CONFUSED, Amount); break;
    case EFFECT_POLYMORPH: Eater->BeginTemporaryState(POLYMORPH, Amount); break;
    case EFFECT_ESP: Eater->BeginTemporaryState(ESP, Amount); break;
    case EFFECT_SKUNK_SMELL: Eater->BeginTemporaryState(POISONED, Amount); break;
    case EFFECT_MAGIC_VAPOUR:
      {
	vector2d Pos = GetMotherEntity()->GetSquareUnderEntity()->GetPos();
        Eater->ActivateRandomState(SRC_MAGIC_VAPOUR, Amount, Volume % 250 + Pos.X + Pos.Y + 1);
	break;
      }
    case EFFECT_TRAIN_PERCEPTION: Eater->EditExperience(PERCEPTION, Amount); break;
    case EFFECT_HOLY_BANANA: Eater->ReceiveHolyBanana(Amount); break;
    case EFFECT_EVIL_WONDER_STAFF_VAPOUR:
      {
	vector2d Pos = GetMotherEntity()->GetSquareUnderEntity()->GetPos();
        Eater->ActivateRandomState(SRC_EVIL, Amount, Volume % 250 + Pos.X + Pos.Y + 1);
	break;
      }
    case EFFECT_GOOD_WONDER_STAFF_VAPOUR:
      {
	vector2d Pos = GetMotherEntity()->GetSquareUnderEntity()->GetPos();
        Eater->ActivateRandomState(SRC_GOOD, Amount, Volume % 250 + Pos.X + Pos.Y + 1);
	break;
      }
    default: return false;
    }

  return true;
}

material* material::EatEffect(character* Eater, ulong Amount)
{
  Amount = Volume > Amount ? Amount : Volume;
  Effect(Eater, Amount);
  Eater->ReceiveNutrition(GetNutritionValue() * Amount * 15 / 1000);

  if(Volume != Amount)
    {
      EditVolume(-Amount);
      return 0;
    }
  else
    return MotherEntity->RemoveMaterial(this);
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
    case HM_CONFUSE: Enemy->AddConfuseHitMessage(); break;
    case HM_HOLY_BANANA: Enemy->AddHolyBananaConsumeEndMessage(); break;
    }

  ulong Amount = Min(GetVolume() >> 1, 1UL);
  bool Success = Effect(Enemy, Amount);

  if(Amount != Volume)
    EditVolume(-Amount);
  else
    delete MotherEntity->RemoveMaterial(this);

  return Success;
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
    case CEM_HOLY_BANANA: Eater->AddHolyBananaConsumeEndMessage(); break;
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
    case SOLID_ID >> 12: return new solid(Config, 0);
    case ORGANIC_ID >> 12: return new organic(Config, 0);
    case GAS_ID >> 12: return new gas(Config, 0);
    case LIQUID_ID >> 12: return new liquid(Config, 0);
    case FLESH_ID >> 12: return new flesh(Config, 0);
    case POWDER_ID >> 12: return new powder(Config, 0);
    case IRON_ALLOY_ID >> 12: return new ironalloy(Config, 0);
    }

  ABORT("Odd material configuration number %d requested!", Config);
  return 0;
}

material* material::MakeMaterial(ushort Config, ulong Volume)
{
  if(!Config)
    return 0;

  switch(Config >> 12)
    {
    case SOLID_ID >> 12: return new solid(Config, Volume);
    case ORGANIC_ID >> 12: return new organic(Config, Volume);
    case GAS_ID >> 12: return new gas(Config, Volume);
    case LIQUID_ID >> 12: return new liquid(Config, Volume);
    case FLESH_ID >> 12: return new flesh(Config, Volume);
    case POWDER_ID >> 12: return new powder(Config, Volume);
    case IRON_ALLOY_ID >> 12: return new ironalloy(Config, Volume);
    }

  ABORT("Odd material configuration number %d of volume %d requested!", Config, Volume);
  return 0;
}

void material::SetVolume(ulong What)
{
  Volume = What;
  CalculateWeight();

  if(MotherEntity)
    MotherEntity->SignalVolumeAndWeightChange();
}

void material::SetVolumeNoSignals(ulong What)
{
  Volume = What;
  CalculateWeight();
}

void material::Initialize(ushort NewConfig, ulong InitVolume, bool Load)
{
  if(!Load)
    {
      InstallDataBase(NewConfig);
      Volume = InitVolume;
      CalculateWeight();
    }

  VirtualConstructor(Load);
}

ulong material::GetTotalNutritionValue() const
{ 
  return GetNutritionValue() * GetVolume() * 15 / 1000;
}

bool material::CanBeEatenByAI(const character* Eater) const
{
  return Eater->GetAttribute(WISDOM) < GetConsumeWisdomLimit() && !GetSpoilLevel() && !Eater->CheckCannibalism(this);
}

bool material::BreatheEffect(character* Enemy)
{
  return Effect(Enemy, Max<ulong>(GetVolume() / 10, 50));
}

const materialdatabase* material::GetDataBase(ushort Config)
{
  const prototype* ProtoType = 0;

  switch(Config >> 12)
    {
    case SOLID_ID >> 12: ProtoType = &solid_ProtoType; break;
    case ORGANIC_ID >> 12: ProtoType = &organic_ProtoType; break;
    case GAS_ID >> 12: ProtoType = &gas_ProtoType; break;
    case LIQUID_ID >> 12: ProtoType = &liquid_ProtoType; break;
    case FLESH_ID >> 12: ProtoType = &flesh_ProtoType; break;
    case POWDER_ID >> 12: ProtoType = &powder_ProtoType; break;
    case IRON_ALLOY_ID >> 12: ProtoType = &ironalloy_ProtoType; break;
    }

  if(ProtoType)
    {
      const databasemap& ConfigMap = ProtoType->GetConfig();
      const databasemap::const_iterator i = ConfigMap.find(Config);

      if(i != ConfigMap.end())
	return &i->second;
    }

  ABORT("Odd material configuration number %d requested!", Config);
  return 0;
}

void material::FinishConsuming(character* Consumer)
{
  if(!Consumer->IsPlayer() && GetConsumeWisdomLimit() != NO_LIMIT)
    Consumer->EditExperience(WISDOM, 500);

  AddConsumeEndMessage(Consumer);
}
