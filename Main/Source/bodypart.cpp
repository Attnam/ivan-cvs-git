/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

/* Compiled through itemset.cpp */

int bodypart::GetGraphicsContainerIndex() const { return GR_HUMANOID; }
long bodypart::GetTruePrice() const { return MainMaterial->GetRawPrice(); }
int bodypart::GetArticleMode() const { return Unique ? DEFINITE_ARTICLE : NORMAL_ARTICLE; }
bool bodypart::IsAlive() const { return MainMaterial->IsAlive(); }
int bodypart::GetSpecialFlags() const { return SpecialFlags|ST_OTHER_BODYPART; }
color16 bodypart::GetMaterialColorA(int) const { return GetMainMaterial()->GetSkinColor(); }
bool bodypart::IsWarm() const { return MainMaterial->IsWarm(); }
bool bodypart::UseMaterialAttributes() const { return MainMaterial->UseMaterialAttributes(); }
bool bodypart::CanRegenerate() const { return MainMaterial->CanRegenerate(); }
square* bodypart::GetSquareUnder(int I) const { return Master ? Slot[0]->GetSquareUnder(I) : Slot[I]->GetSquareUnder(); }
lsquare* bodypart::GetLSquareUnder(int I) const { return static_cast<lsquare*>(Master ? Slot[0]->GetSquareUnder(I) : Slot[I]->GetSquareUnder()); }
item* bodypart::GetExternalBodyArmor() const { return GetHumanoidMaster()->GetBodyArmor(); }
item* bodypart::GetExternalCloak() const { return GetHumanoidMaster()->GetCloak(); }
bool bodypart::AllowFluidBe() const { return !Master || !Master->IsPolymorphed(); }

int head::GetBodyPartIndex() const { return HEAD_INDEX; }
int head::GetBiteMinDamage() const { return int(BiteDamage * 0.75); }
int head::GetBiteMaxDamage() const { return int(BiteDamage * 1.25 + 1); }

int torso::GetBodyPartIndex() const { return TORSO_INDEX; }

int normaltorso::GetGraphicsContainerIndex() const { return GR_CHARACTER; }

int arm::GetMinDamage() const { return int(Damage * 0.75); }
int arm::GetMaxDamage() const { return int(Damage * 1.25 + 1); }
double arm::GetBlockValue() const { return GetToHitValue() * GetWielded()->GetBlockModifier() / 10000; }

int rightarm::GetBodyPartIndex() const { return RIGHT_ARM_INDEX; }
int rightarm::GetSpecialFlags() const { return SpecialFlags|ST_RIGHT_ARM; }

int leftarm::GetBodyPartIndex() const { return  LEFT_ARM_INDEX; }
int leftarm::GetSpecialFlags() const { return SpecialFlags|ST_LEFT_ARM; }

int groin::GetBodyPartIndex() const { return GROIN_INDEX; }
int groin::GetSpecialFlags() const { return SpecialFlags|ST_GROIN; }

int leg::GetKickMinDamage() const { return int(KickDamage * 0.75); }
int leg::GetKickMaxDamage() const { return int(KickDamage * 1.25 + 1); }

int rightleg::GetBodyPartIndex() const { return RIGHT_LEG_INDEX; }
int rightleg::GetSpecialFlags() const { return SpecialFlags|ST_RIGHT_LEG; }

int leftleg::GetBodyPartIndex() const { return LEFT_LEG_INDEX; }
int leftleg::GetSpecialFlags() const { return SpecialFlags|ST_LEFT_LEG; }

vector2d eddytorso::GetBitmapPos(int Frame) const { return torso::GetBitmapPos(Frame) + vector2d((Frame&0x6) << 3, 0); }

head* corpse::Behead() { return Deceased->Behead(); }
bool corpse::CanBeCloned() const { return GetDeceased()->CanBeCloned(); }
int corpse::GetAttachedGod() const { return GetDeceased()->GetTorso()->GetAttachedGod(); }

vector2d ennerhead::GetBitmapPos(int Frame) const { return Frame & 16 ? head::GetBitmapPos(Frame) : head::GetBitmapPos(Frame) + vector2d(16, 0); }

alpha blinkdogtorso::GetAlphaA(int Frame) const { return (Frame & 31) != 31 ? 255 : 0; }

void bodypart::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << BitmapPos << ColorB << ColorC << ColorD << (int)SpecialFlags << HP;
  SaveFile << OwnerDescription << Unique << BloodMaterial;
  SaveFile << IsSparklingB << IsSparklingC << IsSparklingD;
}

void bodypart::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> BitmapPos >> ColorB >> ColorC >> ColorD >> (int&)SpecialFlags >> HP;
  SaveFile >> OwnerDescription >> Unique >> BloodMaterial;
  SaveFile >> IsSparklingB >> IsSparklingC >> IsSparklingD;
}

int bodypart::GetStrengthValue() const
{
  if(Master && !UseMaterialAttributes())
    return long(GetStrengthModifier()) * Master->GetAttribute(ENDURANCE) / 2000;
  else
    return long(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 2000;
}

int head::GetTotalResistance(int Type) const
{
  if(Master)
    {
      int Resistance = GetResistance(Type) + Master->GlobalResistance(Type);

      if(GetHelmet())
	Resistance += GetHelmet()->GetResistance(Type);

      if(GetExternalBodyArmor())
	Resistance += GetExternalBodyArmor()->GetResistance(Type) >> 2;

      return Resistance;
    }
  else
    return GetResistance(Type);
}

int normaltorso::GetTotalResistance(int Type) const
{
  if(Master)
    return GetResistance(Type) + Master->GlobalResistance(Type);
  else
    return GetResistance(Type);
}

int humanoidtorso::GetTotalResistance(int Type) const
{
  if(Master)
    {
      int Resistance = GetResistance(Type) + Master->GlobalResistance(Type);

      if(GetBodyArmor())
	Resistance += GetBodyArmor()->GetResistance(Type);

      if(GetBelt())
	Resistance += GetBelt()->GetResistance(Type);

      return Resistance;
    }
  else
    return GetResistance(Type);
}

int arm::GetTotalResistance(int Type) const
{
  if(Master)
    {
      int Resistance = GetResistance(Type) + Master->GlobalResistance(Type);

      if(GetExternalBodyArmor())
	Resistance += GetExternalBodyArmor()->GetResistance(Type) >> 1;

      if(GetGauntlet())
	Resistance += GetGauntlet()->GetResistance(Type);

      return Resistance;
    }
  else
    return GetResistance(Type);
}

int groin::GetTotalResistance(int Type) const
{
  if(Master)
    {
      int Resistance = GetResistance(Type) + Master->GlobalResistance(Type);

      if(GetExternalBodyArmor())
	Resistance += GetExternalBodyArmor()->GetResistance(Type);

      if(GetHumanoidMaster()->GetBelt())
	Resistance += GetHumanoidMaster()->GetBelt()->GetResistance(Type);

      return Resistance;
    }
  else
    return GetResistance(Type);
}

int leg::GetTotalResistance(int Type) const
{
  if(Master)
    {
      int Resistance = GetResistance(Type) + Master->GlobalResistance(Type);

      if(GetExternalBodyArmor())
	Resistance += GetExternalBodyArmor()->GetResistance(Type) >> 1;

      if(GetBoot())
	Resistance += GetBoot()->GetResistance(Type);

      return Resistance;
    }
  else
    return GetResistance(Type);
}

void head::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << (int)BaseBiteStrength;
  SaveFile << HelmetSlot << AmuletSlot;
}

void head::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> (int&)BaseBiteStrength;
  SaveFile >> HelmetSlot >> AmuletSlot;
}

void humanoidtorso::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BodyArmorSlot << CloakSlot << BeltSlot;
}

void humanoidtorso::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BodyArmorSlot >> CloakSlot >> BeltSlot;
}

void arm::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << (int)BaseUnarmedStrength;
  SaveFile << StrengthExperience << DexterityExperience;
  SaveFile << WieldedSlot << GauntletSlot << RingSlot;
  SaveFile << WieldedGraphicData;
}

void arm::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> (int&)BaseUnarmedStrength;
  SaveFile >> StrengthExperience >> DexterityExperience;
  SaveFile >> WieldedSlot >> GauntletSlot >> RingSlot;
  SaveFile >> WieldedGraphicData;
}

void leg::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BaseKickStrength << StrengthExperience << AgilityExperience;
  SaveFile << BootSlot;
}

void leg::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BaseKickStrength >> StrengthExperience >> AgilityExperience;
  SaveFile >> BootSlot;
}

bool bodypart::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(Master)
    {
      if(Type & POISON && !IsAlive())
	return false;

      int BHP = HP;

      if(HP <= Damage && CannotBeSevered(Type))
	Damage = GetHP() - 1;

      if(!Damage)
	return false;

      EditHP(-Damage);

      if(Type & DRAIN && IsAlive())
	for(int c = 0; c < Damage; ++c)
	  Damager->HealHitPoint();

      if(HP <= 0)
	return true;
      else if(Master->IsPlayer())
	if(HP == 1 && BHP > 1)
	  {
	    if(IsAlive())
	      ADD_MESSAGE("Your %s bleeds very badly.", GetBodyPartName().CStr());
	    else
	      ADD_MESSAGE("Your %s is in very bad condition.", GetBodyPartName().CStr());
	  }
	else if(IsInBadCondition() && !IsInBadCondition(BHP))
	  {
	    if(IsAlive())
	      ADD_MESSAGE("Your %s bleeds.", GetBodyPartName().CStr());
	    else
	      ADD_MESSAGE("Your %s is in bad condition.", GetBodyPartName().CStr());
	  }
    }

  return false;
}

bool bodypart::CannotBeSevered(int Type)
{
  return (Master->BodyPartIsVital(GetBodyPartIndex()) && ((HP == MaxHP && HP != 1) || (Master->GetTorso()->HP == Master->GetTorso()->MaxHP && Master->GetTorso()->MaxHP != 1))) || (Type & (POISON|SOUND) && GetBodyPartIndex() != TORSO_INDEX);
}

double arm::GetUnarmedDamage() const
{
  double WeaponStrength = GetBaseUnarmedStrength() * GetBaseUnarmedStrength();
  item* Gauntlet = GetGauntlet();

  if(Gauntlet)
    WeaponStrength += Gauntlet->GetWeaponStrength();

  double Damage = sqrt(5e-12 * GetAttribute(ARM_STRENGTH) * WeaponStrength) * GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetBonus();

  if(Gauntlet)
    Damage += Gauntlet->GetDamageBonus();

  return Damage;
}

double arm::GetUnarmedToHitValue() const
{
  return GetAttribute(DEXTERITY) * sqrt(2.5 * Master->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetBonus() * Master->GetMoveEase() / 500000;
}

long arm::GetUnarmedAPCost() const
{
  return long(10000000000. / (APBonus(GetAttribute(DEXTERITY)) * Master->GetMoveEase() * Master->GetCWeaponSkill(UNARMED)->GetBonus()));
}

void arm::CalculateDamage()
{
  if(!Master)
    return;

  if(GetWielded())
    Damage = GetWieldedDamage();
  else if(PairArmAllowsMelee())
    Damage = GetUnarmedDamage();
  else
    Damage = 0;
}

void arm::CalculateToHitValue()
{
  if(!Master)
    return;

  if(GetWielded())
    ToHitValue = GetWieldedToHitValue();
  else if(PairArmAllowsMelee())
    ToHitValue = GetUnarmedToHitValue();
  else
    ToHitValue = 0;
}

void arm::CalculateAPCost()
{
  if(!Master)
    return;

  if(GetWielded())
    APCost = GetWieldedAPCost();
  else if(PairArmAllowsMelee())
    APCost = GetUnarmedAPCost();
  else return;

  if(APCost < 100)
    APCost = 100;
}

bool arm::PairArmAllowsMelee() const
{
  const arm* PairArm = GetPairArm();
  return !PairArm || !PairArm->GetWielded() || PairArm->GetWielded()->IsShield(Master);
}

double arm::GetWieldedDamage() const
{
  const item* Wielded = GetWielded();

  if(Wielded->IsShield(Master))
    return 0;

  int HitStrength = GetAttribute(ARM_STRENGTH);
  int Requirement = Wielded->GetStrengthRequirement();

  if(TwoHandWieldIsActive())
    {
      HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
      Requirement >>= 2;
    }

  if(HitStrength > Requirement)
    return Wielded->GetDamageBonus() + sqrt(5e-18 * HitStrength * Wielded->GetWeaponStrength()) * GetCurrentSWeaponSkill()->GetBonus() * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus();
  else
    return 0;
}

double arm::GetWieldedToHitValue() const
{
  int HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  const item* Wielded = GetWielded();
  double Base = 2e-11 * Min(HitStrength, 10) * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus() * GetCurrentSWeaponSkill()->GetBonus() * Master->GetMoveEase() * (10000. / (1000 + Wielded->GetWeight()) + Wielded->GetTHVBonus());
  double ThisToHit = GetAttribute(DEXTERITY) * sqrt(2.5 * Master->GetAttribute(PERCEPTION));
  const arm* PairArm = GetPairArm();

  if(PairArm)
    {
      const item* PairWielded = PairArm->GetWielded();

      if(!PairWielded)
	{
	  if(Wielded->IsTwoHanded() && !Wielded->IsShield(Master))
	    return Base * (ThisToHit + PairArm->GetAttribute(DEXTERITY) * sqrt(2.5 * Master->GetAttribute(PERCEPTION))) / 2;
	}
      else if(!Wielded->IsShield(Master) && !PairWielded->IsShield(Master))
	return Base * ThisToHit / (1.0 + (500.0 + PairWielded->GetWeight()) / (1000.0 + (Wielded->GetWeight() << 1)));
    }

  return Base * ThisToHit;
}

long arm::GetWieldedAPCost() const
{
  const item* Wielded = GetWielded();

  if(Wielded->IsShield(Master))
    return 0;

  int HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  return long(1 / (1e-14 * APBonus(GetAttribute(DEXTERITY)) * Master->GetMoveEase() * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus() * GetCurrentSWeaponSkill()->GetBonus() * Min(HitStrength, 10)));
}

void head::CalculateDamage()
{
  if(!Master)
    return;

  BiteDamage = 7.07e-6 * GetBaseBiteStrength() * GetHumanoidMaster()->GetCWeaponSkill(BITE)->GetBonus();
}

void head::CalculateToHitValue()
{
  if(!Master)
    return;

  BiteToHitValue = Master->GetAttribute(AGILITY) * sqrt(2.5 * Master->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus() * Master->GetMoveEase() / 1000000;
}

void head::CalculateAPCost()
{
  if(!Master)
    return;

  BiteAPCost = Max(long(10000000000. / (APBonus(Master->GetAttribute(AGILITY)) * Master->GetMoveEase() * Master->GetCWeaponSkill(BITE)->GetBonus())), 100L);
}

void leg::CalculateDamage()
{
  if(!Master)
    return;

  double WeaponStrength = GetBaseKickStrength() * GetBaseKickStrength();
  item* Boot = GetBoot();

  if(Boot)
    WeaponStrength += Boot->GetWeaponStrength();

  KickDamage = sqrt(5e-12 * GetAttribute(LEG_STRENGTH) * WeaponStrength) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus();

  if(Boot)
    KickDamage += Boot->GetDamageBonus();
}

void leg::CalculateToHitValue()
{
  if(!Master)
    return;

  KickToHitValue = GetAttribute(AGILITY) * sqrt(2.5 * Master->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus() * Master->GetMoveEase() / 1000000;
}

void leg::CalculateAPCost()
{
  if(!Master)
    return;

  KickAPCost = Max(long(20000000000. / (APBonus(GetAttribute(AGILITY)) * Master->GetMoveEase() * Master->GetCWeaponSkill(KICK)->GetBonus())), 100L);
}

humanoid* bodypart::GetHumanoidMaster() const
{
  return static_cast<humanoid*>(Master);
}

void corpse::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Deceased;
}

void corpse::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Deceased;
  Deceased->SetMotherEntity(this);
  Enable();
}

void corpse::AddPostFix(festring& String) const
{
  String << " of ";
  GetDeceased()->AddName(String, INDEFINITE);
}

int corpse::GetOfferValue(int Receiver) const
{
  int OfferValue = 0;

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart)
        OfferValue += BodyPart->GetOfferValue(Receiver);
    }

  return OfferValue;
}

double corpse::GetWeaponStrength() const
{
  return GetFormModifier() * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() * sqrt(GetDeceased()->GetTorso()->GetMainMaterial()->GetWeight());
}

bool corpse::CanBeEatenByAI(const character* Eater) const
{
  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart && !BodyPart->CanBeEatenByAI(Eater))
	return false;
    }

  return true;
}

int corpse::GetStrengthValue() const
{
  return long(GetStrengthModifier()) * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() / 2000;
}

corpse::~corpse()
{
  delete Deceased;
}

color16 corpse::GetMaterialColorA(int) const
{
  return GetDeceased()->GetTorso()->GetMainMaterial()->GetColor();
}

alpha corpse::GetAlphaA(int) const
{
  return GetDeceased()->GetTorso()->GetMainMaterial()->GetAlpha();
}

color16 corpse::GetMaterialColorB(int) const
{
  return GetDeceased()->GetTorso()->IsAlive() ? material::GetDataBase(GetDeceased()->GetBloodMaterial())->Color : GetDeceased()->GetTorso()->GetMainMaterial()->GetColor();
}

alpha corpse::GetAlphaB(int) const
{
  return GetDeceased()->GetTorso()->IsAlive() ? 175 : GetDeceased()->GetTorso()->GetMainMaterial()->GetAlpha();
}

bool corpse::IsSparkling(int I) const
{
  if(!I)
    return GetDeceased()->GetTorso()->GetMainMaterial()->IsSparkling();
  else
    return GetDeceased()->GetTorso()->IsAlive() ? false : GetDeceased()->GetTorso()->GetMainMaterial()->IsSparkling();
}

vector2d corpse::GetBitmapPos(int) const
{
  if(GetDeceased()->GetSize() < 50)
    return vector2d(32, 64);
  else if(GetDeceased()->GetSize() < 150)
    return vector2d(16, 192);
  else
    return vector2d(16, 272);
}

int corpse::GetSize() const
{
  return GetDeceased()->GetSize();
}

void corpse::SetDeceased(character* What)
{
  Deceased = What;
  Deceased->SetMotherEntity(this);
  SignalVolumeAndWeightChange();
  SignalEmitationIncrease(Deceased->GetEmitation());
  UpdatePictures();
  Enable();
}

void head::DropEquipment(stack* Stack)
{
  if(Stack)
    {
      if(GetHelmet())
	GetHelmet()->MoveTo(Stack);

      if(GetAmulet())
	GetAmulet()->MoveTo(Stack);
    }
  else
    {
      if(GetHelmet())
	GetSlot()->AddFriendItem(GetHelmet());

      if(GetAmulet())
	GetSlot()->AddFriendItem(GetAmulet());
    }
}

void humanoidtorso::DropEquipment(stack* Stack)
{
  if(Stack)
    {
      if(GetBodyArmor())
	GetBodyArmor()->MoveTo(Stack);

      if(GetCloak())
	GetCloak()->MoveTo(Stack);

      if(GetBelt())
	GetBelt()->MoveTo(Stack);
    }
  else
    {
      if(GetBodyArmor())
	GetSlot()->AddFriendItem(GetBodyArmor());

      if(GetCloak())
	GetSlot()->AddFriendItem(GetCloak());

      if(GetBelt())
	GetSlot()->AddFriendItem(GetBelt());
    }
}

void arm::DropEquipment(stack* Stack)
{
  if(Stack)
    {
      if(GetWielded())
	GetWielded()->MoveTo(Stack);

      if(GetGauntlet())
	GetGauntlet()->MoveTo(Stack);

      if(GetRing())
	GetRing()->MoveTo(Stack);
    }
  else
    {
      if(GetWielded())
	GetSlot()->AddFriendItem(GetWielded());

      if(GetGauntlet())
	GetSlot()->AddFriendItem(GetGauntlet());

      if(GetRing())
	GetSlot()->AddFriendItem(GetRing());
    }
}

void leg::DropEquipment(stack* Stack)
{
  if(Stack)
    {
      if(GetBoot())
	GetBoot()->MoveTo(Stack);
    }
  else
    {
      if(GetBoot())
	GetSlot()->AddFriendItem(GetBoot());
    }
}

head::~head()
{
  delete GetHelmet();
  delete GetAmulet();
}

humanoidtorso::~humanoidtorso()
{
  delete GetBodyArmor();
  delete GetCloak();
  delete GetBelt();
}

arm::~arm()
{
  delete GetWielded();
  delete GetGauntlet();
  delete GetRing();
}

leg::~leg()
{
  delete GetBoot();
}

bool corpse::IsDestroyable(const character* Char) const
{
  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart && !BodyPart->IsDestroyable(Char))
	return false;
    }

  return true;
}

long corpse::GetTruePrice() const
{
  long Price = 0;

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart)
	Price += BodyPart->GetTruePrice();
    }

  return Price;
}

material* corpse::GetMaterial(int I) const
{
  return GetDeceased()->GetTorso()->GetMaterial(I);
}

bool bodypart::IsSparkling(int I) const
{
  switch(I)
    {
      case 0: return GetMainMaterial()->SkinColorIsSparkling();
      case 1: return IsSparklingB;
      case 2: return IsSparklingC;
      case 3: return IsSparklingD;
    }

  return false;
}

bool corpse::RaiseTheDead(character* Summoner)
{
  if(Summoner && Summoner->IsPlayer())
    game::DoEvilDeed(50);

  GetDeceased()->Enable();

  if(GetDeceased()->TryToRiseFromTheDead())
    {
      vector2d Pos = GetPos();
      RemoveFromSlot();
      GetDeceased()->SetMotherEntity(0);

      if(Summoner && GetDeceased()->IsCharmable() && !GetDeceased()->IsPlayer())
	GetDeceased()->ChangeTeam(Summoner->GetTeam());

      GetDeceased()->PutToOrNear(Pos);
      GetDeceased()->SignalStepFrom(0);
      Deceased = 0;
      SendToHell();
      return true;
    }
  else
    {
      GetDeceased()->Disable();
      return false;
    }
}

void bodypart::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  Unique = false;
  Master = 0;
}

void head::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);
  HelmetSlot.Init(this, HELMET_INDEX);
  AmuletSlot.Init(this, AMULET_INDEX);
  //BaseBiteStrength = 0;
}

void humanoidtorso::VirtualConstructor(bool Load)
{
  torso::VirtualConstructor(Load);
  BodyArmorSlot.Init(this, BODY_ARMOR_INDEX);
  CloakSlot.Init(this, CLOAK_INDEX);
  BeltSlot.Init(this, BELT_INDEX);
}

void arm::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);
  //BaseUnarmedStrength = 0;

  if(!Load)
    StrengthBonus = DexterityBonus = 0;
}

void rightarm::VirtualConstructor(bool Load)
{
  arm::VirtualConstructor(Load);
  WieldedSlot.Init(this, RIGHT_WIELDED_INDEX);
  GauntletSlot.Init(this, RIGHT_GAUNTLET_INDEX);
  RingSlot.Init(this, RIGHT_RING_INDEX);
}

void leftarm::VirtualConstructor(bool Load)
{
  arm::VirtualConstructor(Load);
  WieldedSlot.Init(this, LEFT_WIELDED_INDEX);
  GauntletSlot.Init(this, LEFT_GAUNTLET_INDEX);
  RingSlot.Init(this, LEFT_RING_INDEX);
}

void leg::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);

  if(!Load)
    StrengthBonus = AgilityBonus = 0;
}

void rightleg::VirtualConstructor(bool Load)
{
  leg::VirtualConstructor(Load);
  BootSlot.Init(this, RIGHT_BOOT_INDEX);
}

void leftleg::VirtualConstructor(bool Load)
{
  leg::VirtualConstructor(Load);
  BootSlot.Init(this, LEFT_BOOT_INDEX);
}

void arm::Hit(character* Enemy, vector2d HitPos, int Direction, bool ForceHit)
{
  long StrExp = 50, DexExp = 50;
  bool THW = false;
  item* Wielded = GetWielded();

  if(Wielded)
    {
      long Weight = Wielded->GetWeight();
      StrExp = Limit(15 * Weight / 200L, 75L, 300L);
      DexExp = Weight ? Limit(75000L / Weight, 75L, 300L) : 300;
      THW = TwoHandWieldIsActive();
    }

  switch(Enemy->TakeHit(Master, Wielded ? Wielded : GetGauntlet(), this, HitPos, GetTypeDamage(Enemy), GetToHitValue(), RAND() % 26 - RAND() % 26, Wielded ? WEAPON_ATTACK : UNARMED_ATTACK, Direction, !(RAND() % Master->GetCriticalModifier()), ForceHit))
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case HAS_DIED:
    case DID_NO_DAMAGE:
      EditExperience(ARM_STRENGTH, StrExp, 1 << 9);

      if(THW && GetPairArm())
	GetPairArm()->EditExperience(ARM_STRENGTH, StrExp, 1 << 9);

    case HAS_DODGED:
      EditExperience(DEXTERITY, DexExp, 1 << 9);

      if(THW && GetPairArm())
	GetPairArm()->EditExperience(DEXTERITY, DexExp, 1 << 9);
    }
}

int arm::GetAttribute(int Identifier) const
{
  if(Identifier == ARM_STRENGTH)
    {
      if(!UseMaterialAttributes())
	return Max(int(StrengthExperience * EXP_DIVISOR) + StrengthBonus, 1);
      else
	return Max(GetMainMaterial()->GetStrengthValue() + StrengthBonus, 1);
    }
  else if(Identifier == DEXTERITY)
    {
      if(!UseMaterialAttributes())
	return Max(int(DexterityExperience * EXP_DIVISOR) + DexterityBonus, 1);
      else
	return Max((GetMainMaterial()->GetFlexibility() << 2) + DexterityBonus, 1);
    }
  else
    {
      ABORT("Illegal arm attribute %d request!", Identifier);
      return 0xACCA;
    }
}

bool arm::EditAttribute(int Identifier, int Value)
{
  if(!Master)
    return false;

  if(Identifier == ARM_STRENGTH)
    {
      if(!UseMaterialAttributes()
      && Master->RawEditAttribute(StrengthExperience, Value))
	{
	  Master->CalculateBattleInfo();
	  return true;
	}
    }
  else if(Identifier == DEXTERITY)
    if(!UseMaterialAttributes()
    && Master->RawEditAttribute(DexterityExperience, Value))
      {
	Master->CalculateBattleInfo();
	return true;
      }

  return false;
}

void arm::EditExperience(int Identifier, double Value, double Speed)
{
  if(!Master)
    return;

  if(Identifier == ARM_STRENGTH)
    {
      if(!UseMaterialAttributes())
	{
	  int Change = Master->RawEditExperience(StrengthExperience,
						 Master->GetNaturalExperience(ARM_STRENGTH),
						 Value, Speed);

	  if(Change)
	    {
	      const char* Adj = Change > 0 ? "stronger" : "weaker";

	      if(Master->IsPlayer())
		ADD_MESSAGE("Your %s feels %s!", GetBodyPartName().CStr(), Adj);
	      else if(Master->IsPet() && Master->CanBeSeenByPlayer())
		ADD_MESSAGE("Suddenly %s looks %s.", Master->CHAR_NAME(DEFINITE), Adj);

	      Master->CalculateBattleInfo();
	    }
	}
    }
  else if(Identifier == DEXTERITY)
    {
      if(!UseMaterialAttributes())
	{
	  int Change = Master->RawEditExperience(DexterityExperience,
						 Master->GetNaturalExperience(DEXTERITY),
						 Value, Speed);

	  if(Change)
	    {
	      const char* Adj = Change > 0 ? "quite dextrous" : "clumsy";

	      if(Master->IsPlayer())
		ADD_MESSAGE("Your %s feels %s!", GetBodyPartName().CStr(), Adj);
	      else if(Master->IsPet() && Master->CanBeSeenByPlayer())
		ADD_MESSAGE("Suddenly %s looks %s.", Master->CHAR_NAME(DEFINITE), Adj);

	      Master->CalculateBattleInfo();
	    }
	}
    }
  else
    ABORT("Illegal arm attribute %d experience edit request!", Identifier);
}

int leg::GetAttribute(int Identifier) const
{
  if(Identifier == LEG_STRENGTH)
    {
      if(!UseMaterialAttributes())
	return Max(int(StrengthExperience * EXP_DIVISOR) + StrengthBonus, 1);
      else
	return Max(GetMainMaterial()->GetStrengthValue() + StrengthBonus, 1);
    }
  else if(Identifier == AGILITY)
    {
      if(!UseMaterialAttributes())
	return Max(int(AgilityExperience * EXP_DIVISOR) + AgilityBonus, 1);
      else
	return Max((GetMainMaterial()->GetFlexibility() << 2) + AgilityBonus, 1);
    }
  else
    {
      ABORT("Illegal leg attribute %d request!", Identifier);
      return 0xECCE;
    }
}

bool leg::EditAttribute(int Identifier, int Value)
{
  if(!Master)
    return false;

  if(Identifier == LEG_STRENGTH)
    {
      if(!UseMaterialAttributes()
      && Master->RawEditAttribute(StrengthExperience, Value))
	{
	  Master->CalculateBurdenState();
	  Master->CalculateBattleInfo();
	  return true;
	}
    }
  else if(Identifier == AGILITY)
    if(!UseMaterialAttributes()
    && Master->RawEditAttribute(AgilityExperience, Value))
      {
	Master->CalculateBattleInfo();
	return true;
      }

  return false;
}

void leg::EditExperience(int Identifier, double Value, double Speed)
{
  if(!Master)
    return;

  if(Identifier == LEG_STRENGTH)
    {
      if(!UseMaterialAttributes())
	{
	  int Change = Master->RawEditExperience(StrengthExperience,
						 Master->GetNaturalExperience(LEG_STRENGTH),
						 Value, Speed);

	  if(Change)
	    {
	      const char* Adj = Change > 0 ? "stronger" : "weaker";

	      if(Master->IsPlayer())
		ADD_MESSAGE("Your %s feels %s!", GetBodyPartName().CStr(), Adj);
	      else if(Master->IsPet() && Master->CanBeSeenByPlayer())
		ADD_MESSAGE("Suddenly %s looks %s.", Master->CHAR_NAME(DEFINITE), Adj);

	      Master->CalculateBurdenState();
	      Master->CalculateBattleInfo();
	    }
	}
    }
  else if(Identifier == AGILITY)
    {
      if(!UseMaterialAttributes())
	{
	  int Change = Master->RawEditExperience(AgilityExperience,
						 Master->GetNaturalExperience(AGILITY),
						 Value, Speed);

	  if(Change)
	    {
	      const char* Adj = Change > 0 ? "very agile" : "slower";

	      if(Master->IsPlayer())
		ADD_MESSAGE("Your %s feels %s!", GetBodyPartName().CStr(), Adj);
	      else if(Master->IsPet() && Master->CanBeSeenByPlayer())
		ADD_MESSAGE("Suddenly %s looks %s.", Master->CHAR_NAME(DEFINITE), Adj);

	      Master->CalculateBattleInfo();
	    }
	}
    }
  else
    ABORT("Illegal leg attribute %d experience edit request!", Identifier);
}

void head::InitSpecialAttributes()
{
  BaseBiteStrength = Master->GetBaseBiteStrength();
}

void arm::InitSpecialAttributes()
{
  if(!Master->IsHuman() || Master->IsInitializing())
    {
      StrengthExperience = Master->GetNaturalExperience(ARM_STRENGTH);
      DexterityExperience = Master->GetNaturalExperience(DEXTERITY);
    }
  else
    {
      StrengthExperience = game::GetAveragePlayerArmStrengthExperience();
      DexterityExperience = game::GetAveragePlayerDexterityExperience();
    }

  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(DexterityExperience, MIN_EXP, MAX_EXP);
  BaseUnarmedStrength = Master->GetBaseUnarmedStrength();
}

void leg::InitSpecialAttributes()
{
  if(!Master->IsHuman() || Master->IsInitializing())
    {
      StrengthExperience = Master->GetNaturalExperience(LEG_STRENGTH);
      AgilityExperience = Master->GetNaturalExperience(AGILITY);
    }
  else
    {
      StrengthExperience = game::GetAveragePlayerLegStrengthExperience();
      AgilityExperience = game::GetAveragePlayerAgilityExperience();
    }

  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(AgilityExperience, MIN_EXP, MAX_EXP);
  BaseKickStrength = Master->GetBaseKickStrength();
}

void bodypart::SignalEquipmentAdd(gearslot* Slot)
{
  if(Master)
    Master->SignalEquipmentAdd(Slot->GetEquipmentIndex());
}

void bodypart::SignalEquipmentRemoval(gearslot* Slot)
{
  if(Master)
    Master->SignalEquipmentRemoval(Slot->GetEquipmentIndex());
}

void bodypart::Mutate()
{
  GetMainMaterial()->SetVolume(long(GetVolume() * (1.5 - (RAND() & 1023) / 1023.)));
}

void arm::Mutate()
{
  bodypart::Mutate();
  StrengthExperience = StrengthExperience * (1.5 - (RAND() & 1023) / 1023.);
  DexterityExperience = DexterityExperience * (1.5 - (RAND() & 1023) / 1023.);
  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(DexterityExperience, MIN_EXP, MAX_EXP);
}

void leg::Mutate()
{
  bodypart::Mutate();
  StrengthExperience = StrengthExperience * (1.5 - (RAND() & 1023) / 1023.);
  AgilityExperience = AgilityExperience * (1.5 - (RAND() & 1023) / 1023.);
  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(AgilityExperience, MIN_EXP, MAX_EXP);
}

arm* rightarm::GetPairArm() const
{
  if(GetHumanoidMaster())
    return GetHumanoidMaster()->GetLeftArm();
  else
    return 0;
}

arm* leftarm::GetPairArm() const
{
  if(GetHumanoidMaster())
    return GetHumanoidMaster()->GetRightArm();
  else
    return 0;
}

sweaponskill*& rightarm::GetCurrentSWeaponSkill() const
{
  return GetHumanoidMaster()->CurrentRightSWeaponSkill;
}

sweaponskill*& leftarm::GetCurrentSWeaponSkill() const
{
  return GetHumanoidMaster()->CurrentLeftSWeaponSkill;
}

alpha bodypart::GetMaxAlpha() const
{
  if(Master && Master->StateIsActivated(INVISIBLE))
    return 150;
  else
    return 255;
}

void bodypart::AddPostFix(festring& String) const
{
  if(OwnerDescription.GetSize())
    String << ' ' << OwnerDescription;
}

void corpse::CalculateVolumeAndWeight()
{
  Volume = Deceased->GetVolume();
  Weight = Deceased->GetWeight();
}

item* head::GetEquipment(int I) const
{
  switch(I)
    {
    case 0: return GetHelmet();
    case 1: return GetAmulet();
    }

  return 0;
}

item* humanoidtorso::GetEquipment(int I) const
{
  switch(I)
    {
    case 0: return GetBodyArmor();
    case 1: return GetCloak();
    case 2: return GetBelt();
    }

  return 0;
}

item* arm::GetEquipment(int I) const
{
  switch(I)
    {
    case 0: return GetWielded();
    case 1: return GetGauntlet();
    case 2: return GetRing();
    }

  return 0;
}

item* leg::GetEquipment(int I) const
{
  return !I ? GetBoot() : 0;
}

void bodypart::CalculateVolumeAndWeight()
{
  item::CalculateVolumeAndWeight();
  CarriedWeight = 0;
  BodyPartVolume = Volume;

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	{
	  Volume += Equipment->GetVolume();
	  CarriedWeight += Equipment->GetWeight();
	}
    }

  Weight += CarriedWeight;
}

void corpse::CalculateEmitation()
{
  Emitation = Deceased->GetEmitation();
}

void bodypart::CalculateEmitation()
{
  item::CalculateEmitation();

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
	game::CombineLights(Emitation, Equipment->GetEmitation());
    }
}

void bodypart::CalculateMaxHP(bool MayChangeHPs)
{
  int HPDelta = MaxHP - HP;
  MaxHP = 0;

  if(Master)
    {
      if(!UseMaterialAttributes())
	{
	  long Endurance = Master->GetAttribute(ENDURANCE);
	  MaxHP = GetBodyPartVolume() * Endurance * Endurance / 200000;
	}
      else
	{
	  long SV = GetMainMaterial()->GetStrengthValue();
	  MaxHP = (GetBodyPartVolume() * SV >> 4) * SV / 250000;
	}

      if(MaxHP < 1)
	MaxHP = 1;

      if(MayChangeHPs)
	HP = Max(MaxHP - HPDelta, 1);
    }
}

void bodypart::SignalVolumeAndWeightChange()
{
  item::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
    {
      CalculateMaxHP();
      Master->CalculateMaxHP();
      Master->SignalBodyPartVolumeAndWeightChange();
      square* SquareUnder = GetSquareUnder();

      if(SquareUnder && UpdateArmorPictures())
	SquareUnder->SendNewDrawRequest();
    }
}

void bodypart::SetHP(int What)
{
  HP = What;

  if(Master)
    Master->CalculateHP();
}

void bodypart::EditHP(int What)
{
  HP += What;

  if(Master)
    Master->CalculateHP();
}

void arm::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
    {
      GetHumanoidMaster()->EnsureCurrentSWeaponSkillIsCorrect(GetCurrentSWeaponSkill(), GetWielded());
      CalculateAttributeBonuses();
      CalculateAttackInfo();
      UpdateWieldedPicture();

      if(GetSquareUnder())
	GetSquareUnder()->SendNewDrawRequest();
    }
}

void leg::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
    {
      CalculateAttributeBonuses();
      CalculateAttackInfo();
    }
}

void humanoidtorso::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
    {
      humanoid* HumanoidMaster = GetHumanoidMaster();

      if(HumanoidMaster->GetRightArm())
	HumanoidMaster->GetRightArm()->CalculateAttributeBonuses();

      if(HumanoidMaster->GetLeftArm())
	HumanoidMaster->GetLeftArm()->CalculateAttributeBonuses();

      if(HumanoidMaster->GetRightLeg())
	HumanoidMaster->GetRightLeg()->CalculateAttributeBonuses();

      if(HumanoidMaster->GetLeftLeg())
	HumanoidMaster->GetLeftLeg()->CalculateAttributeBonuses();
    }
}

void bodypart::CalculateAttackInfo()
{
  CalculateDamage();
  CalculateToHitValue();
  CalculateAPCost();
}

bool arm::TwoHandWieldIsActive() const
{
  const item* Wielded = GetWielded();

  if(Wielded->IsTwoHanded() && !Wielded->IsShield(Master))
    {
      arm* PairArm = GetPairArm();
      return PairArm && !PairArm->GetWielded();
    }
  else
    return false;
}

double bodypart::GetTimeToDie(int Damage, double ToHitValue, double DodgeValue, bool AttackIsBlockable, bool UseMaxHP) const
{
  double Durability;
  int TotalResistance = GetTotalResistance(PHYSICAL_DAMAGE);
  int Damage3 = (Damage << 1) + Damage;
  int Damage5 = (Damage << 2) + Damage;
  int TrueDamage = (19 * (Max((Damage3 >> 2) - TotalResistance, 0)
		       +  Max((Damage5 >> 2) + 1 - (TotalResistance >> 1), 0))
		       + (Max((Damage3 >> 1) - TotalResistance, 0)
		       +  Max((Damage5 >> 1) + 3 - (TotalResistance >> 1), 0))) / 40;

  int HP = UseMaxHP ? GetMaxHP() : GetHP();

  if(TrueDamage > 0)
    {
      double AverageDamage;

      if(AttackIsBlockable)
	{
	  blockvector Block;
	  Master->CreateBlockPossibilityVector(Block, ToHitValue);

	  if(Block.size())
	    {
	      double ChanceForNoBlock = 1.0;
	      AverageDamage = 0;

	      for(uint c = 0; c < Block.size(); ++c)
		{
		  ChanceForNoBlock -= Block[c].first;

		  if(TrueDamage - Block[c].second > 0)
		    AverageDamage += Block[c].first * (TrueDamage - Block[c].second);
		}

	      AverageDamage += ChanceForNoBlock * TrueDamage;
	    }
	  else
	    AverageDamage = TrueDamage;
	}
      else
	AverageDamage = TrueDamage;

      Durability = HP / (AverageDamage * GetRoughChanceToHit(ToHitValue, DodgeValue));

      if(Durability < 1)
	Durability = 1;

      if(Durability > 1000)
	Durability = 1000;
    }
  else
    Durability = 1000;

  return Durability;
}

double bodypart::GetRoughChanceToHit(double ToHitValue, double DodgeValue) const
{
  return GLOBAL_WEAK_BODYPART_HIT_MODIFIER * ToHitValue * GetBodyPartVolume() / ((DodgeValue / ToHitValue + 1) * DodgeValue * Master->GetBodyVolume() * 100);
}

double torso::GetRoughChanceToHit(double ToHitValue, double DodgeValue) const
{
  return 1 / (DodgeValue / ToHitValue + 1);
}

void bodypart::RandomizePosition()
{
  SpecialFlags |= 1 + RAND() % 7;
  UpdatePictures();
}

double arm::GetBlockChance(double EnemyToHitValue) const
{
  const item* Wielded = GetWielded();
  return Wielded ? Min(1.0 / (1 + EnemyToHitValue / (GetToHitValue() * Wielded->GetBlockModifier()) * 10000), 1.0) : 0;
}

int arm::GetBlockCapability() const
{
  const item* Wielded = GetWielded();

  if(!Wielded)
    return 0;

  int HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  return Min(HitStrength, 10) * Wielded->GetStrengthValue() * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus() * GetCurrentSWeaponSkill()->GetBonus() / 10000000;
}

void arm::WieldedSkillHit(int Hits)
{
  item* Wielded = GetWielded();

  if(Master->GetCWeaponSkill(Wielded->GetWeaponCategory())->AddHit(Hits))
    {
      CalculateAttackInfo();

      if(Master->IsPlayer())
	{
	  int Category = Wielded->GetWeaponCategory();
	  GetHumanoidMaster()->GetCWeaponSkill(Category)->AddLevelUpMessage(Category);
	}
    }

  if(GetCurrentSWeaponSkill()->AddHit(Hits))
    {
      CalculateAttackInfo();

      if(Master->IsPlayer())
	GetCurrentSWeaponSkill()->AddLevelUpMessage(Wielded->CHAR_NAME(UNARTICLED));
    }
}

head::head(const head& Head) : bodypart(Head), BaseBiteStrength(Head.BaseBiteStrength)
{
  HelmetSlot.Init(this, HELMET_INDEX);
  AmuletSlot.Init(this, AMULET_INDEX);
}

humanoidtorso::humanoidtorso(const humanoidtorso& Torso) : torso(Torso)
{
  BodyArmorSlot.Init(this, BODY_ARMOR_INDEX);
  CloakSlot.Init(this, CLOAK_INDEX);
  BeltSlot.Init(this, BELT_INDEX);
}

arm::arm(const arm& Arm) : bodypart(Arm), StrengthExperience(Arm.StrengthExperience), DexterityExperience(Arm.DexterityExperience), BaseUnarmedStrength(Arm.BaseUnarmedStrength)
{
}

rightarm::rightarm(const rightarm& Arm) : arm(Arm)
{
  WieldedSlot.Init(this, RIGHT_WIELDED_INDEX);
  GauntletSlot.Init(this, RIGHT_GAUNTLET_INDEX);
  RingSlot.Init(this, RIGHT_RING_INDEX);
}

leftarm::leftarm(const leftarm& Arm) : arm(Arm)
{
  WieldedSlot.Init(this, LEFT_WIELDED_INDEX);
  GauntletSlot.Init(this, LEFT_GAUNTLET_INDEX);
  RingSlot.Init(this, LEFT_RING_INDEX);
}

leg::leg(const leg& Leg) : bodypart(Leg), StrengthExperience(Leg.StrengthExperience), AgilityExperience(Leg.AgilityExperience), BaseKickStrength(Leg.BaseKickStrength)
{
}

rightleg::rightleg(const rightleg& Leg) : leg(Leg)
{
  BootSlot.Init(this, RIGHT_BOOT_INDEX);
}

leftleg::leftleg(const leftleg& Leg) : leg(Leg)
{
  BootSlot.Init(this, LEFT_BOOT_INDEX);
}

corpse::corpse(const corpse& Corpse) : item(Corpse)
{
  Deceased = Corpse.Deceased->Duplicate();
  Deceased->SetMotherEntity(this);
}

void bodypart::SignalSpoil(material* Material)
{
  if(Master)
    Master->SignalSpoil();
  else
    item::SignalSpoil(Material);
}

void corpse::SignalSpoil(material*)
{
  GetDeceased()->Disappear(this, "spoil", &item::IsVeryCloseToSpoiling);
}

void corpse::SignalDisappearance()
{
  GetDeceased()->Disappear(this, "disappear", &item::IsVeryCloseToDisappearance);
}

bool bodypart::CanBePiledWith(const item* Item, const character* Viewer) const
{
  return item::CanBePiledWith(Item, Viewer)
      && OwnerDescription == static_cast<const bodypart*>(Item)->OwnerDescription;
}

bool corpse::CanBePiledWith(const item* Item, const character* Viewer) const
{
  if(GetType() != Item->GetType()
  || GetConfig() != Item->GetConfig()
  || GetWeight() != Item->GetWeight()
  || Viewer->GetCWeaponSkillLevel(this) != Viewer->GetCWeaponSkillLevel(Item)
  || Viewer->GetSWeaponSkillLevel(this) != Viewer->GetSWeaponSkillLevel(Item))
    return false;

  const corpse* Corpse = static_cast<const corpse*>(Item);

  if(Deceased->GetBodyParts() != Corpse->Deceased->GetBodyParts())
    return false;

  for(int c = 0; c < Deceased->GetBodyParts(); ++c)
    {
      bodypart* BodyPart1 = Deceased->GetBodyPart(c);
      bodypart* BodyPart2 = Corpse->Deceased->GetBodyPart(c);

      if(!BodyPart1 && !BodyPart2)
	continue;

      if(!BodyPart1 || !BodyPart2 || !BodyPart1->CanBePiledWith(BodyPart2, Viewer))
	return false;
    }

  if(Deceased->GetName(UNARTICLED) != Corpse->Deceased->GetName(UNARTICLED))
    return false;

  return true;
}

void bodypart::Be()
{
  if(Master)
    {
      if(HP < MaxHP && ++SpillBloodCounter >= 4)
	{
	  if(Master->IsEnabled())
	    {
	      if(IsInBadCondition() && !Master->IsPolymorphed() && !(RAND() & 3))
		SpillBlood(1);
	    }
	  else if(!Master->IsPolymorphed() && !(RAND() & 3))
	    {
	      SpillBlood(1);
	      HP += Max(((MaxHP - HP) >> 2), 1);
	    }

	  SpillBloodCounter = 0;
	}

      if(Master->AllowSpoil() || !Master->IsEnabled())
	MainMaterial->Be();

      if(Exists() && LifeExpectancy)
	if(LifeExpectancy == 1)
	  Master->SignalDisappearance();
	else
	  --LifeExpectancy;
    }
  else
    {
      if(HP < MaxHP && ++SpillBloodCounter >= 4)
	{
	  if(!(RAND() & 3))
	    {
	      SpillBlood(1);
	      HP += Max(((MaxHP - HP) >> 2), 1);
	    }

	  SpillBloodCounter = 0;
	}

      item::Be();
    }
}

void bodypart::SpillBlood(int HowMuch, vector2d Pos)
{
  if(HowMuch && (!Master || Master->SpillsBlood()) && (IsAlive() || MainMaterial->IsLiquid()) && !game::IsInWilderness()) 
    GetNearLSquare(Pos)->SpillFluid(0, CreateBlood(long(HowMuch * sqrt(BodyPartVolume) / 2)), false, false);
}

void bodypart::SpillBlood(int HowMuch)
{
  if(HowMuch && (!Master || Master->SpillsBlood()) && (IsAlive() || MainMaterial->IsLiquid()) && !game::IsInWilderness())
    for(int c = 0; c < GetSquaresUnder(); ++c)
      if(GetLSquareUnder(c))
	GetLSquareUnder(c)->SpillFluid(0, CreateBlood(long(HowMuch * sqrt(BodyPartVolume) / 2)), false, false);
}

void bodypart::SignalEnchantmentChange()
{
  if(Master && !Master->IsInitializing())
    {
      Master->CalculateAttributeBonuses();
      Master->CalculateBattleInfo();
    }
}

void arm::SignalEquipmentAdd(gearslot* Slot)
{
  int EquipmentIndex = Slot->GetEquipmentIndex();

  if(Master && !Master->IsInitializing())
    {
      item* Equipment = Slot->GetItem();

      if(Equipment->IsInCorrectSlot(EquipmentIndex))
	ApplyEquipmentAttributeBonuses(Equipment);

      if(EquipmentIndex == RIGHT_GAUNTLET_INDEX || EquipmentIndex == LEFT_GAUNTLET_INDEX)
	ApplyDexterityPenalty(Equipment);

      if(EquipmentIndex == RIGHT_WIELDED_INDEX || EquipmentIndex == LEFT_WIELDED_INDEX)
	{
	  UpdateWieldedPicture();
	  GetSquareUnder()->SendNewDrawRequest();
	}
    }

  if(Master)
    Master->SignalEquipmentAdd(EquipmentIndex);
}

void arm::SignalEquipmentRemoval(gearslot* Slot)
{
  int EquipmentIndex = Slot->GetEquipmentIndex();

  if(Master && !Master->IsInitializing())
    if(EquipmentIndex == RIGHT_WIELDED_INDEX || EquipmentIndex == LEFT_WIELDED_INDEX)
      {
	UpdateWieldedPicture();
	square* Square = GetSquareUnder();

	if(Square)
	  Square->SendNewDrawRequest();
      }

  if(Master)
    Master->SignalEquipmentRemoval(EquipmentIndex);
}

void leg::SignalEquipmentAdd(gearslot* Slot)
{
  int EquipmentIndex = Slot->GetEquipmentIndex();

  if(Master && !Master->IsInitializing())
    {
      item* Equipment = Slot->GetItem();

      if(Equipment->IsInCorrectSlot(EquipmentIndex))
	ApplyEquipmentAttributeBonuses(Equipment);

      if(EquipmentIndex == RIGHT_BOOT_INDEX || EquipmentIndex == LEFT_BOOT_INDEX)
	ApplyAgilityPenalty(Equipment);
    }

  if(Master)
    Master->SignalEquipmentAdd(EquipmentIndex);
}

void arm::ApplyEquipmentAttributeBonuses(item* Item)
{
  if(Item->AffectsArmStrength())
    StrengthBonus += Item->GetEnchantment();

  if(Item->AffectsDexterity())
    DexterityBonus += Item->GetEnchantment();
}

void leg::ApplyEquipmentAttributeBonuses(item* Item)
{
  if(Item->AffectsLegStrength())
    {
      StrengthBonus += Item->GetEnchantment();

      if(Master)
	Master->CalculateBurdenState();
    }

  if(Item->AffectsAgility())
    AgilityBonus += Item->GetEnchantment();
}

void arm::CalculateAttributeBonuses()
{
  StrengthBonus = DexterityBonus = 0;

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment->IsInCorrectSlot())
	ApplyEquipmentAttributeBonuses(Equipment);
    }

  ApplyDexterityPenalty(GetGauntlet());

  if(Master)
    {
      ApplyDexterityPenalty(GetExternalCloak());
      ApplyDexterityPenalty(GetExternalBodyArmor());
    }
}

void leg::CalculateAttributeBonuses()
{
  StrengthBonus = AgilityBonus = 0;

  for(int c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment->IsInCorrectSlot())
	ApplyEquipmentAttributeBonuses(Equipment);
    }

  ApplyAgilityPenalty(GetBoot());

  if(Master)
    {
      ApplyAgilityPenalty(GetExternalCloak());
      ApplyAgilityPenalty(GetExternalBodyArmor());
    }
}

void humanoidtorso::SignalEquipmentAdd(gearslot* Slot)
{
  if(!Master)
    return;

  humanoid* Master = GetHumanoidMaster();
  int EquipmentIndex = Slot->GetEquipmentIndex();

  if(!Master->IsInitializing() && EquipmentIndex == CLOAK_INDEX || EquipmentIndex == BODY_ARMOR_INDEX)
    {
      item* Item = Slot->GetItem();

      if(Master->GetRightArm())
	Master->GetRightArm()->ApplyDexterityPenalty(Item);

      if(Master->GetLeftArm())
	Master->GetLeftArm()->ApplyDexterityPenalty(Item);

      if(Master->GetRightLeg())
	Master->GetRightLeg()->ApplyAgilityPenalty(Item);

      if(Master->GetLeftLeg())
	Master->GetLeftLeg()->ApplyAgilityPenalty(Item);
    }

  Master->SignalEquipmentAdd(EquipmentIndex);
}

int arm::GetWieldedHitStrength() const
{
  int HitStrength = GetAttribute(ARM_STRENGTH);
  int Requirement = GetWielded()->GetStrengthRequirement();

  if(TwoHandWieldIsActive())
    {
      HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
      Requirement >>= 2;
    }

  return HitStrength - Requirement;
}

void arm::ApplyDexterityPenalty(item* Item)
{
  if(Item)
    DexterityBonus -= Item->GetInElasticityPenalty(!UseMaterialAttributes() ? GetAttribute(DEXTERITY) : GetMainMaterial()->GetFlexibility());
}

void leg::ApplyAgilityPenalty(item* Item)
{
  if(Item)
    AgilityBonus -= Item->GetInElasticityPenalty(!UseMaterialAttributes() ? GetAttribute(AGILITY) : GetMainMaterial()->GetFlexibility());
}

int corpse::GetSpoilLevel() const
{
  int FlyAmount = 0;

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart && FlyAmount < BodyPart->GetSpoilLevel())
	FlyAmount = BodyPart->GetSpoilLevel();
    }

  return FlyAmount;
}

void bodypart::SignalSpoilLevelChange(material* Material)
{
  if(Master)
    Master->SignalSpoilLevelChange();
  else
    item::SignalSpoilLevelChange(Material);
}

bool head::DamageArmor(character* Damager, int Damage, int Type)
{
  long AV[3] = { 0, 0, 0 }, AVSum = 0;
  item* Armor[3];

  if((Armor[0] = GetHelmet()))
    AVSum += AV[0] = Max(Armor[0]->GetStrengthValue(), 1);

  if((Armor[1] = GetExternalBodyArmor()))
    AVSum += AV[1] = Max(Armor[1]->GetStrengthValue() >> 2, 1);

  if((Armor[2] = GetExternalCloak()))
    AVSum += AV[2] = Max(Armor[2]->GetStrengthValue(), 1);

  return AVSum ? Armor[femath::WeightedRand(AV, AVSum)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool humanoidtorso::DamageArmor(character* Damager, int Damage, int Type)
{
  long AV[3] = { 0, 0, 0 }, AVSum = 0;
  item* Armor[3];

  if((Armor[0] = GetBodyArmor()))
    AVSum += AV[0] = Max(Armor[0]->GetStrengthValue(), 1);

  if((Armor[1] = GetBelt()))
    AVSum += AV[1] = Max(Armor[1]->GetStrengthValue(), 1);

  if((Armor[2] = GetCloak()))
    AVSum += AV[2] = Max(Armor[2]->GetStrengthValue(), 1);

  return AVSum ? Armor[femath::WeightedRand(AV, AVSum)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool arm::DamageArmor(character* Damager, int Damage, int Type)
{
  long AV[3] = { 0, 0, 0 }, AVSum = 0;
  item* Armor[3];

  if((Armor[0] = GetGauntlet()))
    AVSum += AV[0] = Max(Armor[0]->GetStrengthValue(), 1);

  if((Armor[1] = GetExternalBodyArmor()))
    AVSum += AV[1] = Max(Armor[1]->GetStrengthValue() >> 1, 1);

  if((Armor[2] = GetExternalCloak()))
    AVSum += AV[2] = Max(Armor[2]->GetStrengthValue(), 1);

  return AVSum ? Armor[femath::WeightedRand(AV, AVSum)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool groin::DamageArmor(character* Damager, int Damage, int Type)
{
  return Master->GetTorso()->DamageArmor(Damager, Damage, Type);
}

bool leg::DamageArmor(character* Damager, int Damage, int Type)
{
  long AV[3] = { 0, 0, 0 }, AVSum = 0;
  item* Armor[3];

  if((Armor[0] = GetBoot()))
    AVSum += AV[0] = Max(Armor[0]->GetStrengthValue(), 1);

  if((Armor[1] = GetExternalBodyArmor()))
    AVSum += AV[1] = Max(Armor[1]->GetStrengthValue() >> 1, 1);

  if((Armor[2] = GetExternalCloak()))
    AVSum += AV[2] = Max(Armor[2]->GetStrengthValue(), 1);

  return AVSum ? Armor[femath::WeightedRand(AV, AVSum)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool bodypart::CanBeEatenByAI(const character* Who) const
{
  return item::CanBeEatenByAI(Who) && !(Who->IsPet() && PLAYER->HasHadBodyPart(this));
}

int bodypart::GetConditionColorIndex() const
{
  if(HP <= 1 && MaxHP > 1)
    return 0;
  else if((HP << 1) + HP < MaxHP)
    return 1;
  else if((HP << 1) + HP < MaxHP << 1)
    return 2;
  else if(HP < MaxHP)
    return 3;
  else
    return 4;
}

bool arm::CheckIfWeaponTooHeavy(const char* WeaponDescription) const
{
  int HitStrength = GetAttribute(ARM_STRENGTH);
  int Requirement = GetWielded()->GetStrengthRequirement();

  if(TwoHandWieldIsActive())
    {
      HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
      Requirement >>= 2;

      if(HitStrength - Requirement < 10)
	{
	  if(HitStrength <= Requirement)
	    ADD_MESSAGE("You cannot use %s. Wielding it with two hands requires %d strength.", WeaponDescription, (Requirement >> 1) + 1);
	  else if(HitStrength - Requirement < 4)
	    ADD_MESSAGE("Using %s even with two hands is extremely difficult.", WeaponDescription);
	  else if(HitStrength - Requirement < 7)
	    ADD_MESSAGE("You have much trouble using %s even with two hands.", WeaponDescription);
	  else
	    ADD_MESSAGE("It is somewhat difficult to use %s even with two hands.", WeaponDescription);

	  return !game::BoolQuestion(CONST_S("Continue anyway? [y/N]"));
	}
    }
  else
    {
      if(HitStrength - Requirement < 10)
	{
	  const char* HandInfo = "";

	  if(GetWielded()->IsTwoHanded())
	    HandInfo = " with one hand";

	  if(HitStrength <= Requirement)
	    ADD_MESSAGE("You cannot use %s. Wielding it%s requires %d strength.", WeaponDescription, HandInfo, Requirement + 1);
	  else if(HitStrength - Requirement < 4)
	    ADD_MESSAGE("Using %s%s is extremely difficult.", WeaponDescription, HandInfo);
	  else if(HitStrength - Requirement < 7)
	    ADD_MESSAGE("You have much trouble using %s%s.", WeaponDescription, HandInfo);
	  else
	    ADD_MESSAGE("It is somewhat difficult to use %s%s.", WeaponDescription, HandInfo);

	  return !game::BoolQuestion(CONST_S("Continue anyway? [y/N]"));
	}
    }

  return false;
}

int corpse::GetArticleMode() const
{
  return Deceased->LeftOversAreUnique() ? DEFINITE_ARTICLE : NORMAL_ARTICLE;
}

head* head::Behead()
{
  RemoveFromSlot();
  return this;
}

bool arm::EditAllAttributes(int Amount)
{
  LimitRef(StrengthExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  LimitRef(DexterityExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  return (Amount < 0
       && (StrengthExperience != MIN_EXP || DexterityExperience != MIN_EXP))
      || (Amount > 0
       && (StrengthExperience != MAX_EXP || DexterityExperience != MAX_EXP));
}

bool leg::EditAllAttributes(int Amount)
{
  LimitRef(StrengthExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  LimitRef(AgilityExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  return (Amount < 0
       && (StrengthExperience != MIN_EXP || AgilityExperience != MIN_EXP))
      || (Amount > 0
       && (StrengthExperience != MAX_EXP || AgilityExperience != MAX_EXP));
}

#ifdef WIZARD

void arm::AddAttackInfo(felist& List) const
{
  if(GetDamage())
    {
      festring Entry = CONST_S("   ");

      if(GetWielded())
	{
	  GetWielded()->AddName(Entry, UNARTICLED);

	  if(TwoHandWieldIsActive())
	    Entry << " (b)";
	}
      else
	Entry << "melee attack";

      Entry.Resize(50);
      Entry << GetMinDamage() << '-' << GetMaxDamage();
      Entry.Resize(60);
      Entry << int(GetToHitValue());
      Entry.Resize(70);
      Entry << GetAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

void arm::AddDefenceInfo(felist& List) const
{
  if(GetWielded())
    {
      festring Entry = CONST_S("   ");
      GetWielded()->AddName(Entry, UNARTICLED);
      Entry.Resize(50);
      Entry << int(GetBlockValue());
      Entry.Resize(70);
      Entry << GetBlockCapability();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

#else

void arm::AddAttackInfo(felist&) const { }
void arm::AddDefenceInfo(felist&) const { }

#endif

void arm::UpdateWieldedPicture()
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
    {
      item* Wielded = GetWielded();

      if(Wielded && Master)
	{
	  int SpecialFlags = (IsRightArm() ? 0 : MIRROR)|ST_WIELDED|(Wielded->GetSpecialFlags()&~0x3F);
	  Wielded->UpdatePictures(WieldedGraphicData,
				  Master->GetWieldedPosition(),
				  SpecialFlags,
				  GetMaxAlpha(),
				  GR_HUMANOID,
				  static_cast<bposretriever>(&item::GetWieldedBitmapPos));

	  if(ShowFluids())
	    Wielded->CheckFluidGearPictures(Wielded->GetWieldedBitmapPos(0), SpecialFlags, false);
	}
      else
	WieldedGraphicData.Retire();
    }
}

void arm::DrawWielded(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  DrawEquipment(WieldedGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(ShowFluids() && GetWielded())
    GetWielded()->DrawFluidGearPictures(Bitmap, Pos, Luminance, IsRightArm() ? 0 : MIRROR, AllowAnimate);
}

void arm::UpdatePictures()
{
  bodypart::UpdatePictures();
  UpdateWieldedPicture();
}

void bodypart::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance, int SquareIndex, bool AllowAnimate, bool AllowAlpha) const
{
  const int AF = GraphicData.AnimationFrames;
  const bitmap* P = GraphicData.Picture[!AllowAnimate || AF == 1 ? 0 : GET_TICK() % AF];

  if(AllowAlpha)
    P->AlphaPriorityBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
  else
    P->MaskedPriorityBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);

  if(Fluid && ShowFluids())
    DrawFluids(Bitmap, Pos, Luminance, SquareIndex, AllowAnimate);

  DrawArmor(Bitmap, Pos, Luminance, AllowAnimate);
}

void leg::AddAttackInfo(felist& List) const
{
  festring Entry = CONST_S("   kick attack");
  Entry.Resize(50);
  Entry << GetKickMinDamage() << '-' << GetKickMaxDamage();
  Entry.Resize(60);
  Entry << int(GetKickToHitValue());
  Entry.Resize(70);
  Entry << GetKickAPCost();
  List.AddEntry(Entry, LIGHT_GRAY);
}

void corpse::PreProcessForBone()
{
  item::PreProcessForBone();

  if(!Deceased->PreProcessForBone())
    {
      RemoveFromSlot();
      SendToHell();
    }
}

void corpse::PostProcessForBone()
{
  item::PostProcessForBone();

  if(!Deceased->PostProcessForBone())
    {
      RemoveFromSlot();
      SendToHell();
    }
}

void corpse::FinalProcessForBone()
{
  item::FinalProcessForBone();
  Deceased->FinalProcessForBone();
}

bool bodypart::IsRepairable() const
{
  return !CanRegenerate() && GetHP() < GetMaxHP();
}

bool corpse::SuckSoul(character* Soul, character* Summoner)
{
  vector2d Pos = Soul->GetPos();

  if(Deceased->SuckSoul(Soul))
    {
      Soul->Remove();
      character* Deceased = GetDeceased();

      if(RaiseTheDead(Summoner))
	{
	  Soul->SendToHell();
	  return true;
	}
      else
	{
	  Deceased->SetSoulID(Soul->GetID());
	  Soul->PutTo(Pos);
	  return false;
	}
    }
  else
    return false;
}

double arm::GetTypeDamage(const character* Enemy) const 
{
  if(!GetWielded() || !GetWielded()->IsGoodWithPlants() || !Enemy->IsPlant())
    return Damage;
  else
    return Damage * 1.5;
}

void largetorso::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance, int SquareIndex, bool AllowAnimate, bool AllowAlpha) const
{
  LargeDraw(Bitmap, Pos, Luminance, SquareIndex, AllowAnimate, AllowAlpha);
}

void largecorpse::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance, int SquareIndex, bool AllowAnimate, bool AllowAlpha) const
{
  LargeDraw(Bitmap, Pos, Luminance, SquareIndex, AllowAnimate, AllowAlpha);
}

void largetorso::SignalStackAdd(stackslot* StackSlot, void (stack::*AddHandler)(item*))
{
  if(!Slot[0])
    {
      Slot[0] = StackSlot;
      vector2d Pos = GetPos();
      level* Level = GetLevel();

      for(int c = 1; c < 4; ++c)
	(Level->GetLSquare(Pos + game::GetLargeMoveVector(12 + c))->GetStack()->*AddHandler)(this);
    }
  else
    for(int c = 1; c < 4; ++c)
      if(!Slot[c])
	{
	  Slot[c] = StackSlot;
	  return;
	}
}

int largetorso::GetSquareIndex(vector2d Pos) const
{
  vector2d RelativePos = Pos - GetPos();
  return RelativePos.X + (RelativePos.Y << 1);
}

void largecorpse::SignalStackAdd(stackslot* StackSlot, void (stack::*AddHandler)(item*))
{
  if(!Slot[0])
    {
      Slot[0] = StackSlot;
      vector2d Pos = GetPos();
      level* Level = GetLevel();

      for(int c = 1; c < 4; ++c)
	(Level->GetLSquare(Pos + game::GetLargeMoveVector(12 + c))->GetStack()->*AddHandler)(this);
    }
  else
    for(int c = 1; c < 4; ++c)
      if(!Slot[c])
	{
	  Slot[c] = StackSlot;
	  return;
	}
}

int largecorpse::GetSquareIndex(vector2d Pos) const
{
  vector2d RelativePos = Pos - GetPos();
  return RelativePos.X + (RelativePos.Y << 1);
}

character* corpse::TryNecromancy(character* Summoner) 
{
  if(Summoner && Summoner->IsPlayer())
    game::DoEvilDeed(50);

  character* Zombie = GetDeceased()->CreateZombie();

  if(Zombie)
    {
      vector2d Pos = GetPos();
      RemoveFromSlot();
      Zombie->ChangeTeam(Summoner ? Summoner->GetTeam() : game::GetTeam(MONSTER_TEAM));
      Zombie->PutToOrNear(Pos);
      Zombie->SignalStepFrom(0);
      SendToHell();
      return Zombie;
    }

  return 0;
}

item* head::GetArmorToReceiveFluid(bool) const
{
  item* Helmet = GetHelmet();

  if(Helmet && Helmet->GetCoverPercentile() > RAND() % 100)
    return Helmet;
  else
    return 0;
}

item* humanoidtorso::GetArmorToReceiveFluid(bool) const
{
  item* Cloak = GetCloak();

  if(Cloak && !(RAND() % 3))
    return Cloak;

  item* Belt = GetBelt();

  if(Belt && !(RAND() % 10))
    return Belt;

  item* BodyArmor = GetBodyArmor();
  return BodyArmor ? BodyArmor : 0;
}

item* arm::GetArmorToReceiveFluid(bool) const
{
  item* Cloak = GetExternalCloak();

  if(Cloak && !(RAND() % 3))
    return Cloak;

  item* Wielded = GetWielded();

  if(Wielded && !(RAND() % 3))
    return Wielded;

  item* Gauntlet = GetGauntlet();

  if(Gauntlet && RAND() & 1)
    return Gauntlet;

  item* BodyArmor = GetExternalBodyArmor();
  return BodyArmor ? BodyArmor : 0;
}

item* groin::GetArmorToReceiveFluid(bool) const
{
  item* Cloak = GetExternalCloak();

  if(Cloak && !(RAND() % 3))
    return Cloak;

  item* BodyArmor = GetExternalBodyArmor();
  return BodyArmor ? BodyArmor : 0;
}

item* leg::GetArmorToReceiveFluid(bool SteppedOn) const
{
  if(SteppedOn)
    {
      item* Boot = GetBoot();
      return Boot ? Boot : 0;
    }
  else
    {
      item* Cloak = GetExternalCloak();

      if(Cloak && !(RAND() % 3))
	return Cloak;

      item* Boot = GetBoot();

      if(Boot && RAND() & 1)
	return Boot;

      item* BodyArmor = GetExternalBodyArmor();
      return BodyArmor ? BodyArmor : 0;
    }
}

void bodypart::SpillFluid(character* Spiller, liquid* Liquid, int SquareIndex)
{
  if(Master)
    {
      item* Armor = GetArmorToReceiveFluid(false);

      if(Armor)
	Armor->SpillFluid(Spiller, Liquid);
      else if(GetMaster())
	{
	  if(Liquid->GetVolume())
	    AddFluid(Liquid, "", SquareIndex, false);
	  else
	    delete Liquid;
	}
    }
  else
    item::SpillFluid(Spiller, Liquid, SquareIndex);
}

void bodypart::StayOn(liquid* Liquid)
{
  item* Armor = GetArmorToReceiveFluid(true);

  if(Armor)
    Liquid->TouchEffect(Armor, CONST_S(""));
  else if(GetMaster())
    Liquid->TouchEffect(GetMaster(), GetBodyPartIndex());
}

liquid* bodypart::CreateBlood(long Volume) const
{
  return new liquid(GetBloodMaterial(), Volume);
}

int corpse::GetRustDataA() const
{
  return Deceased->GetTorso()->GetMainMaterial()->GetRustData();
}

void bodypart::UpdateArmorPicture(graphicdata& GData, item* Armor, int SpecialFlags, vector2d (item::*Retriever)(int) const, bool BodyArmor) const
{
  if(Armor && Master)
    {
      Armor->UpdatePictures(GData,
			    vector2d(0, 0),
			    SpecialFlags|Armor->GetSpecialFlags(),
			    GetMaxAlpha(),
			    GR_HUMANOID,
			    static_cast<bposretriever>(Retriever));
      Armor->CheckFluidGearPictures((Armor->*Retriever)(0), SpecialFlags, BodyArmor);
    }
  else
    GData.Retire();
}

bool playerkindhead::UpdateArmorPictures()
{
  UpdateHeadArmorPictures(HelmetGraphicData);
  return true;
}

bool playerkindtorso::UpdateArmorPictures()
{
  UpdateTorsoArmorPictures(TorsoArmorGraphicData,
			   CloakGraphicData,
			   BeltGraphicData);
  return true;
}

bool playerkindrightarm::UpdateArmorPictures()
{
  UpdateArmArmorPictures(ArmArmorGraphicData,
			 GauntletGraphicData,
			 ST_RIGHT_ARM);
  return true;
}

bool playerkindleftarm::UpdateArmorPictures()
{
  UpdateArmArmorPictures(ArmArmorGraphicData,
			 GauntletGraphicData,
			 ST_LEFT_ARM);
  return true;
}

bool playerkindgroin::UpdateArmorPictures()
{
  UpdateGroinArmorPictures(GroinArmorGraphicData);
  return true;
}

bool playerkindrightleg::UpdateArmorPictures()
{
  UpdateLegArmorPictures(LegArmorGraphicData,
			 BootGraphicData,
			 ST_RIGHT_LEG);
  return true;
}

bool playerkindleftleg::UpdateArmorPictures()
{
  UpdateLegArmorPictures(LegArmorGraphicData,
			 BootGraphicData,
			 ST_LEFT_LEG);
  return true;
}

void head::UpdateHeadArmorPictures(graphicdata& HelmetGraphicData) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
    {
      UpdateArmorPicture(HelmetGraphicData,
			 GetHelmet(),
			 ST_OTHER_BODYPART,
			 &item::GetHelmetBitmapPos);
    }
}

void humanoidtorso::UpdateTorsoArmorPictures(graphicdata& TorsoArmorGraphicData, graphicdata& CloakGraphicData, graphicdata& BeltGraphicData) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
    {
      UpdateArmorPicture(TorsoArmorGraphicData,
			 GetBodyArmor(),
			 ST_OTHER_BODYPART,
			 &item::GetTorsoArmorBitmapPos,
			 true);
      UpdateArmorPicture(CloakGraphicData,
			 GetCloak(),
			 ST_OTHER_BODYPART,
			 &item::GetCloakBitmapPos);
      UpdateArmorPicture(BeltGraphicData,
			 GetBelt(),
			 ST_OTHER_BODYPART,
			 &item::GetBeltBitmapPos);
    }
}

void arm::UpdateArmArmorPictures(graphicdata& ArmArmorGraphicData, graphicdata& GauntletGraphicData, int SpecialFlags) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
    {
      UpdateArmorPicture(ArmArmorGraphicData,
			 Master ? GetExternalBodyArmor() : 0,
			 SpecialFlags,
			 &item::GetArmArmorBitmapPos,
			 true);
      UpdateArmorPicture(GauntletGraphicData,
			 GetGauntlet(),
			 SpecialFlags,
			 &item::GetGauntletBitmapPos);
    }
}

void groin::UpdateGroinArmorPictures(graphicdata& GroinArmorGraphicData) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
    {
      UpdateArmorPicture(GroinArmorGraphicData,
			 Master ? GetExternalBodyArmor() : 0,
			 ST_GROIN,
			 &item::GetLegArmorBitmapPos,
			 true);
    }
}

void leg::UpdateLegArmorPictures(graphicdata& LegArmorGraphicData, graphicdata& BootGraphicData, int SpecialFlags) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
    {
      UpdateArmorPicture(LegArmorGraphicData,
			 Master ? GetExternalBodyArmor() : 0,
			 SpecialFlags,
			 &item::GetLegArmorBitmapPos,
			 true);
      UpdateArmorPicture(BootGraphicData,
			 GetBoot(),
			 SpecialFlags,
			 &item::GetBootBitmapPos);
    }
}

void bodypart::DrawEquipment(const graphicdata& GraphicData, bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  int EAF = GraphicData.AnimationFrames;

  if(EAF)
    GraphicData.Picture[!AllowAnimate || EAF == 1 ? 0 : GET_TICK() % EAF]->AlphaPriorityBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
}

void playerkindhead::DrawArmor(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  DrawEquipment(HelmetGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(GetHelmet())
    GetHelmet()->DrawFluidGearPictures(Bitmap, Pos, Luminance, 0, AllowAnimate);
}

void playerkindtorso::DrawArmor(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  DrawEquipment(TorsoArmorGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(GetBodyArmor())
    GetBodyArmor()->DrawFluidGearPictures(Bitmap, Pos, Luminance, 0, AllowAnimate);

  DrawEquipment(CloakGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(GetCloak())
    GetCloak()->DrawFluidGearPictures(Bitmap, Pos, Luminance, 0, AllowAnimate);

  DrawEquipment(BeltGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(GetBelt())
    GetBelt()->DrawFluidGearPictures(Bitmap, Pos, Luminance, 0, AllowAnimate);
}

void playerkindrightarm::DrawArmor(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  DrawEquipment(ArmArmorGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(Bitmap, Pos, Luminance, ST_RIGHT_ARM, AllowAnimate);

  DrawEquipment(GauntletGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(GetGauntlet())
    GetGauntlet()->DrawFluidGearPictures(Bitmap, Pos, Luminance, 0, AllowAnimate);
}

void playerkindleftarm::DrawArmor(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  DrawEquipment(ArmArmorGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(Bitmap, Pos, Luminance, ST_LEFT_ARM, AllowAnimate);

  DrawEquipment(GauntletGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(GetGauntlet())
    GetGauntlet()->DrawFluidGearPictures(Bitmap, Pos, Luminance, 0, AllowAnimate);
}

void playerkindgroin::DrawArmor(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  DrawEquipment(GroinArmorGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(Bitmap, Pos, Luminance, ST_GROIN, AllowAnimate);
}

void playerkindrightleg::DrawArmor(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  DrawEquipment(LegArmorGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(Bitmap, Pos, Luminance, ST_RIGHT_LEG, AllowAnimate);

  DrawEquipment(BootGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(GetBoot())
    GetBoot()->DrawFluidGearPictures(Bitmap, Pos, Luminance, 0, AllowAnimate);
}

void playerkindleftleg::DrawArmor(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  DrawEquipment(LegArmorGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(Bitmap, Pos, Luminance, ST_LEFT_LEG, AllowAnimate);

  DrawEquipment(BootGraphicData, Bitmap, Pos, Luminance, AllowAnimate);

  if(GetBoot())
    GetBoot()->DrawFluidGearPictures(Bitmap, Pos, Luminance, 0, AllowAnimate);
}

void playerkindhead::Save(outputfile& SaveFile) const
{
  head::Save(SaveFile);
  SaveFile << HelmetGraphicData;
}

void playerkindhead::Load(inputfile& SaveFile)
{
  head::Load(SaveFile);
  SaveFile >> HelmetGraphicData;
}

void playerkindtorso::Save(outputfile& SaveFile) const
{
  humanoidtorso::Save(SaveFile);
  SaveFile << TorsoArmorGraphicData << CloakGraphicData << BeltGraphicData;
}

void playerkindtorso::Load(inputfile& SaveFile)
{
  humanoidtorso::Load(SaveFile);
  SaveFile >> TorsoArmorGraphicData >> CloakGraphicData >> BeltGraphicData;
}

void playerkindrightarm::Save(outputfile& SaveFile) const
{
  rightarm::Save(SaveFile);
  SaveFile << ArmArmorGraphicData << GauntletGraphicData;
}

void playerkindrightarm::Load(inputfile& SaveFile)
{
  rightarm::Load(SaveFile);
  SaveFile >> ArmArmorGraphicData >> GauntletGraphicData;
}

void playerkindleftarm::Save(outputfile& SaveFile) const
{
  leftarm::Save(SaveFile);
  SaveFile << ArmArmorGraphicData << GauntletGraphicData;
}

void playerkindleftarm::Load(inputfile& SaveFile)
{
  leftarm::Load(SaveFile);
  SaveFile >> ArmArmorGraphicData >> GauntletGraphicData;
}

void playerkindgroin::Save(outputfile& SaveFile) const
{
  groin::Save(SaveFile);
  SaveFile << GroinArmorGraphicData;
}

void playerkindgroin::Load(inputfile& SaveFile)
{
  groin::Load(SaveFile);
  SaveFile >> GroinArmorGraphicData;
}

void playerkindrightleg::Save(outputfile& SaveFile) const
{
  rightleg::Save(SaveFile);
  SaveFile << LegArmorGraphicData << BootGraphicData;
}

void playerkindrightleg::Load(inputfile& SaveFile)
{
  rightleg::Load(SaveFile);
  SaveFile >> LegArmorGraphicData >> BootGraphicData;
}

void playerkindleftleg::Save(outputfile& SaveFile) const
{
  leftleg::Save(SaveFile);
  SaveFile << LegArmorGraphicData << BootGraphicData;
}

void playerkindleftleg::Load(inputfile& SaveFile)
{
  leftleg::Load(SaveFile);
  SaveFile >> LegArmorGraphicData >> BootGraphicData;
}

void bodypart::UpdatePictures()
{
  item::UpdatePictures();
  UpdateArmorPictures();
}

void playerkindtorso::SignalVolumeAndWeightChange()
{
  humanoidtorso::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
    Master->UpdatePictures();
}

void bodypart::ReceiveAcid(material* Material, const festring& LocationName, long Modifier)
{
  if(Master && !MainMaterial->IsImmuneToAcid())
    {
      long Tries = Modifier / 1000;
      Modifier -= Tries * 1000;
      int Damage = 0;

      for(long c = 0; c < Tries; ++c)
	if(!(RAND() % 100))
	  ++Damage;

      if(Modifier && !(RAND() % 100000 / Modifier))
	++Damage;

      if(Damage)
	{
	  ulong Minute = game::GetTotalMinutes();
	  character* Master = this->Master;

	  if(Master->GetLastAcidMsgMin() != Minute && (Master->CanBeSeenByPlayer() || Master->IsPlayer()))
	    {
	      Master->SetLastAcidMsgMin(Minute);
	      const char* MName = Material->GetName(false, false).CStr();

	      if(Master->IsPlayer())
		{
		  const char* TName = LocationName.IsEmpty() ? GetBodyPartName().CStr() : LocationName.CStr();
		  ADD_MESSAGE("Acidous %s dissolves your %s.", MName, TName);
		}
	      else
		ADD_MESSAGE("Acidous %s dissolves %s.", MName, Master->CHAR_NAME(DEFINITE));
	    }

	  Master->ReceiveBodyPartDamage(0, Damage, ACID, GetBodyPartIndex(), YOURSELF, false, false, false);
	  Master->CheckDeath(CONST_S("died of deadly acid damage"), 0);
	}
    }
}

void bodypart::TryToRust(long LiquidModifier)
{
  if(MainMaterial->TryToRust(LiquidModifier << 4))
    {
      const char* MoreMsg = MainMaterial->GetRustLevel() == NOT_RUSTED ? "" : " more";

      if(Master)
	{
	  if(Master->IsPlayer())
	    ADD_MESSAGE("Your %s rusts%s.", GetBodyPartName().CStr(), MoreMsg);
	  else if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The %s of %s rusts%s.", GetBodyPartName().CStr(), Master->CHAR_NAME(DEFINITE), MoreMsg);
	}
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s rusts%s.", CHAR_NAME(DEFINITE), MoreMsg);

      MainMaterial->SetRustLevel(MainMaterial->GetRustLevel() + 1);
    }
}

material* corpse::GetConsumeMaterial(const character* Consumer, materialpredicate Predicate) const
{
  for(int c = GetDeceased()->GetBodyParts() - 1; c; --c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart)
	{
	  material* CM = BodyPart->GetConsumeMaterial(Consumer, Predicate);

	  if(CM)
	    return CM;
	}
    }

  return GetDeceased()->GetTorso()->GetConsumeMaterial(Consumer, Predicate);
}

void corpse::Cannibalize()
{
  item::Cannibalize();

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart)
	BodyPart->Cannibalize();
    }
}

material* bodypart::RemoveMaterial(material* Material)
{
  if(Master && GetBodyPartIndex() == TORSO_INDEX)
    return Master->GetMotherEntity()->RemoveMaterial(Material); // gum
  else
    return item::RemoveMaterial(Material);
}

void arm::CopyAttributes(const bodypart* BodyPart)
{
  const arm* Arm = static_cast<const arm*>(BodyPart);
  StrengthExperience = Arm->StrengthExperience;
  DexterityExperience = Arm->DexterityExperience;
}

void leg::CopyAttributes(const bodypart* BodyPart)
{
  const leg* Leg = static_cast<const leg*>(BodyPart);
  StrengthExperience = Leg->StrengthExperience;
  AgilityExperience = Leg->AgilityExperience;
}

bool corpse::DetectMaterial(const material* Material) const
{
  return GetDeceased()->DetectMaterial(Material);
}

void bodypart::DestroyBodyPart(stack* Stack)
{
  int Lumps = 1 + RAND() % 3;
  long LumpVolume = Volume / Lumps >> 2;

  if(LumpVolume >= 10)
    for(int c = 0; c < Lumps; ++c)
      {
	item* Lump = GetMainMaterial()->CreateNaturalForm(LumpVolume + RAND() % LumpVolume);
	Stack->AddItem(Lump);
      }

  SendToHell();
}

vector2d magicmushroomtorso::GetBitmapPos(int Frame) const
{
  vector2d BasePos = torso::GetBitmapPos(Frame);
  Frame &= 0x3F;

  if(!(Frame & 0x30))
    {
      if(Frame <= 8)
	return vector2d(BasePos.X + 64 - (abs(Frame - 4) << 4), BasePos.Y);
      else
	return vector2d(BasePos.X + 64 - (abs(Frame - 12) << 4), BasePos.Y + 16);
    }
  else
    return BasePos;
}

vector2d dogtorso::GetBitmapPos(int Frame) const
{
  vector2d BasePos = torso::GetBitmapPos(Frame);

  if(Frame >= GraphicData.AnimationFrames >> 1)
    BasePos.X += 32;

  return vector2d(BasePos.X + ((Frame & 4) << 2), BasePos.Y);
}

void dogtorso::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance, int, bool AllowAnimate, bool AllowAlpha) const
{
  const int AF = GraphicData.AnimationFrames >> 1;
  int Index = !AllowAnimate || AF == 1 ? 0 : GET_TICK() % AF;

  if(GetHP() << 1 <= GetMaxHP())
    Index += AF;

  const bitmap* P = GraphicData.Picture[Index];

  if(AllowAlpha)
    P->AlphaPriorityBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
  else
    P->MaskedPriorityBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
}

void corpse::SetLifeExpectancy(int Base, int RandPlus)
{
  //LifeExpectancy = RandPlus ? Base + RAND_N(RandPlus) : Base;
  Deceased->SetLifeExpectancy(Base, RandPlus);
}

void corpse::Be()
{
  for(int c = 0; c < Deceased->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = Deceased->GetBodyPart(c);

      if(BodyPart)
	BodyPart->Be();
    }
}

void bodypart::SetLifeExpectancy(int Base, int RandPlus)
{
  LifeExpectancy = RandPlus ? Base + RAND_N(RandPlus) : Base;

  if(!Master)
    Enable();
}

void bodypart::SpecialEatEffect(character* Eater, int Amount)
{
  Amount >>= 6;

  if(Amount && (!Master || Master->SpillsBlood()) && (IsAlive() || MainMaterial->IsLiquid()) && !game::IsInWilderness())
    {
      if(Eater->GetVirtualHead())
	Eater->GetVirtualHead()->SpillFluid(Eater, CreateBlood(Amount));

      Eater->GetTorso()->SpillFluid(Eater, CreateBlood(Amount));
    }
}

bool corpse::IsValuable() const
{
  for(int c = 0; c < Deceased->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = Deceased->GetBodyPart(c);

      if(BodyPart && BodyPart->IsValuable())
	return true;
    }

  return false;
}

bool corpse::Necromancy(character* Necromancer)
{
  if(Necromancer && Necromancer->IsPlayer())
    game::DoEvilDeed(50);

  character* Zombie = GetDeceased()->CreateZombie();

  if(Zombie)
    {
      Zombie->ChangeTeam(Necromancer ? Necromancer->GetTeam() : game::GetTeam(MONSTER_TEAM));
      Zombie->PutToOrNear(GetPos());
      RemoveFromSlot();
      SendToHell();

      if(Zombie->CanBeSeenByPlayer())
	ADD_MESSAGE("%s rises back to cursed undead life.", Zombie->CHAR_DESCRIPTION(INDEFINITE));

      Zombie->SignalStepFrom(0);
      return true;
    }
  else
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s vibrates for some time.", CHAR_NAME(DEFINITE));

      return false;
    }
}
