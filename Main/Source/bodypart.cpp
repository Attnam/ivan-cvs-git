/* Compiled through itemset.cpp */

uchar bodypart::GetGraphicsContainerIndex() const { return GR_HUMANOID; }
void bodypart::SetConsumeMaterial(material* NewMaterial, ushort SpecialFlags) { SetMainMaterial(NewMaterial, SpecialFlags); }
void bodypart::ChangeConsumeMaterial(material* NewMaterial, ushort SpecialFlags) { ChangeMainMaterial(NewMaterial, SpecialFlags); }
ulong bodypart::GetPrice() const { return GetMainMaterial()->GetRawPrice() / 100 + item::GetPrice(); }
uchar bodypart::GetArticleMode() const { return Unique ? DEFINITE_ARTICLE : NORMAL_ARTICLE; }
bool bodypart::IsAlive() const { return MainMaterial->IsAlive(); }

uchar head::GetBodyPartIndex() const { return HEAD_INDEX; }
ushort head::GetBiteMinDamage() const { return ushort(BiteDamage * 0.75f); }
ushort head::GetBiteMaxDamage() const { return ushort(BiteDamage * 1.25f + 1); }

uchar torso::GetBodyPartIndex() const { return TORSO_INDEX; }

uchar normaltorso::GetGraphicsContainerIndex() const { return GR_CHARACTER; }

ushort arm::GetMinDamage() const { return ushort(Damage * 0.75f); }
ushort arm::GetMaxDamage() const { return ushort(Damage * 1.25f + 1); }
float arm::GetBlockValue() const { return GetToHitValue() * GetWielded()->GetBlockModifier() / 10000; }

uchar rightarm::GetBodyPartIndex() const { return RIGHT_ARM_INDEX; }
uchar rightarm::GetSpecialFlags() const { return SpecialFlags|ST_RIGHT_ARM; }

uchar leftarm::GetBodyPartIndex() const { return  LEFT_ARM_INDEX; }
uchar leftarm::GetSpecialFlags() const { return SpecialFlags|ST_LEFT_ARM; }

uchar groin::GetBodyPartIndex() const { return GROIN_INDEX; }
uchar groin::GetSpecialFlags() const { return SpecialFlags|ST_GROIN; }

ushort leg::GetKickMinDamage() const { return ushort(KickDamage * 0.75f); }
ushort leg::GetKickMaxDamage() const { return ushort(KickDamage * 1.25f + 1); }

uchar rightleg::GetBodyPartIndex() const { return RIGHT_LEG_INDEX; }
uchar rightleg::GetSpecialFlags() const { return SpecialFlags|ST_RIGHT_LEG; }

uchar leftleg::GetBodyPartIndex() const { return LEFT_LEG_INDEX; }
uchar leftleg::GetSpecialFlags() const { return SpecialFlags|ST_LEFT_LEG; }

vector2d eddytorso::GetBitmapPos(ushort Frame) const { return vector2d(64 + ((Frame&6) << 3), 32); }

void bodypart::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << BitmapPos << ColorB << ColorC << ColorD << SpecialFlags << HP << OwnerDescription << Unique << BloodColor;
}

void bodypart::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> BitmapPos >> ColorB >> ColorC >> ColorD >> SpecialFlags >> HP >> OwnerDescription >> Unique >> BloodColor;
}

ushort bodypart::GetStrengthValue() const
{
  if(GetMaster() && IsAlive())
    return ulong(GetStrengthModifier()) * GetMaster()->GetAttribute(ENDURANCE) / 2000;
  else
    return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 2000;
}

ushort head::GetTotalResistance(uchar Type) const
{
  if(GetMaster())
    {
      ushort Resistance = GetResistance(Type) + GetMaster()->GlobalResistance(Type);

      if(GetHelmet())
	Resistance += GetHelmet()->GetResistance(Type);

      if(GetHumanoidMaster()->GetBodyArmor())
	Resistance += GetHumanoidMaster()->GetBodyArmor()->GetResistance(Type) >> 2;

      return Resistance;
    }
  else
    return GetResistance(Type);
}

ushort normaltorso::GetTotalResistance(uchar Type) const
{
  if(GetMaster())
    return GetResistance(Type) + GetMaster()->GlobalResistance(Type);
  else
    return GetResistance(Type);
}

ushort humanoidtorso::GetTotalResistance(uchar Type) const
{
  if(GetMaster())
    {
      ushort Resistance = GetResistance(Type) + GetMaster()->GlobalResistance(Type);

      if(GetBodyArmor())
	Resistance += GetBodyArmor()->GetResistance(Type);

      if(GetBelt())
	Resistance += GetBelt()->GetResistance(Type);

      return Resistance;
    }
  else
    return GetResistance(Type);
}

ushort arm::GetTotalResistance(uchar Type) const
{
  if(GetMaster())
    {
      ushort Resistance = GetResistance(Type) + GetMaster()->GlobalResistance(Type);

      if(GetHumanoidMaster()->GetBodyArmor())
	Resistance += GetHumanoidMaster()->GetBodyArmor()->GetResistance(Type) >> 1;

      if(GetGauntlet())
	Resistance += GetGauntlet()->GetResistance(Type);

      return Resistance;
    }
  else
    return GetResistance(Type);
}

ushort groin::GetTotalResistance(uchar Type) const
{
  if(GetMaster())
    {
      ushort Resistance = GetResistance(Type) + GetMaster()->GlobalResistance(Type);

      if(GetHumanoidMaster()->GetBodyArmor())
	Resistance += GetHumanoidMaster()->GetBodyArmor()->GetResistance(Type);

      if(GetHumanoidMaster()->GetBelt())
	Resistance += GetHumanoidMaster()->GetBelt()->GetResistance(Type);

      return Resistance;
    }
  else
    return GetResistance(Type);
}

ushort leg::GetTotalResistance(uchar Type) const
{
  if(GetMaster())
    {
      ushort Resistance = GetResistance(Type) + GetMaster()->GlobalResistance(Type);

      if(GetHumanoidMaster()->GetBodyArmor())
	Resistance += GetHumanoidMaster()->GetBodyArmor()->GetResistance(Type) >> 1;

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
  SaveFile << BaseBiteStrength;
  SaveFile << HelmetSlot << AmuletSlot;
}

void head::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BaseBiteStrength;
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
  SaveFile << BaseUnarmedStrength;
  SaveFile << Strength << StrengthExperience << Dexterity << DexterityExperience;
  SaveFile << WieldedSlot << GauntletSlot << RingSlot;
}

void arm::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BaseUnarmedStrength;
  SaveFile >> Strength >> StrengthExperience >> Dexterity >> DexterityExperience;
  SaveFile >> WieldedSlot >> GauntletSlot >> RingSlot;
}

void leg::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BaseKickStrength << Strength << StrengthExperience << Agility << AgilityExperience;
  SaveFile << BootSlot;
}

void leg::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BaseKickStrength >> Strength >> StrengthExperience >> Agility >> AgilityExperience;
  SaveFile >> BootSlot;
}

bool bodypart::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
{
  if(Master)
    {
      if(!IsAlive() && Type == POISON)
	return false;

      ushort BHP = HP;

      if(HP <= Damage && CannotBeSevered(Type))
	Damage = GetHP() - 1;

      if(!Damage)
	return false;

      EditHP(-Damage);

      if(Type == DRAIN && IsAlive())
	for(ushort c = 0; c < Damage; ++c)
	  Damager->HealHitPoint();

      if(HP <= 0)
	return true;
      else if(Master->IsPlayer())
	if(HP == 1 && BHP > 1)
	  {
	    if(IsAlive())
	      ADD_MESSAGE("Your %s bleeds very badly.", GetBodyPartName().c_str());
	    else
	      ADD_MESSAGE("Your %s is in very bad condition.", GetBodyPartName().c_str());
	  }
	else if(IsInBadCondition() && !IsInBadCondition(BHP))
	  {
	    if(IsAlive())
	      ADD_MESSAGE("Your %s bleeds.", GetBodyPartName().c_str());
	    else
	      ADD_MESSAGE("Your %s is in bad condition.", GetBodyPartName().c_str());
	  }
    }

  return false;
}

bool bodypart::CannotBeSevered(uchar Type)
{
  return (Master->BodyPartIsVital(GetBodyPartIndex()) && ((HP == MaxHP && HP != 1) || (!Master->GetTorso()->IsInBadCondition() && Master->GetTorso()->MaxHP > 3))) || ((Type == POISON || Type == SOUND) && GetBodyPartIndex() != TORSO_INDEX);
}

float arm::GetUnarmedDamage() const
{
  float WeaponStrength = GetBaseUnarmedStrength() * GetBaseUnarmedStrength();

  if(GetGauntlet())
    WeaponStrength += GetGauntlet()->GetWeaponStrength();

  return sqrt(5e-10f * GetAttribute(ARM_STRENGTH) * WeaponStrength) * GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetBonus();
}

float arm::GetUnarmedToHitValue() const
{
  return GetAttribute(DEXTERITY) * sqrt(2.5f * Master->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetBonus() * Master->GetMoveEase() / 50000;
}

long arm::GetUnarmedAPCost() const
{
  return 1000000000 / (APBonus(GetAttribute(DEXTERITY)) * Master->GetMoveEase() * Master->GetCWeaponSkill(UNARMED)->GetBonus());
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
  arm* PairArm = GetPairArm();
  return !PairArm || !PairArm->GetWielded() || PairArm->GetWielded()->IsShield(Master);
}

float arm::GetWieldedDamage() const
{
  const item* Wielded = GetWielded();

  if(Wielded->IsShield(Master))
    return 0;

  ushort HitStrength = GetAttribute(ARM_STRENGTH);
  ushort Requirement = Wielded->GetStrengthRequirement();

  if(TwoHandWieldIsActive())
    {
      HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
      Requirement >>= 2;
    }

  if(HitStrength > Requirement)
    return sqrt(5e-14f * HitStrength * Wielded->GetWeaponStrength()) * GetCurrentSWeaponSkill()->GetBonus() * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus();
  else
    return 0;
}

float arm::GetWieldedToHitValue() const
{
  short HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  const item* Wielded = GetWielded();
  float Base = 2e-7f * Min<short>(HitStrength, 10) * Wielded->GetBonus() * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus() * GetCurrentSWeaponSkill()->GetBonus() * Master->GetMoveEase() / (1000 + Wielded->GetWeight());
  float ThisToHit = GetAttribute(DEXTERITY) * sqrt(2.5f * Master->GetAttribute(PERCEPTION));
  const arm* PairArm = GetPairArm();

  if(PairArm)
    {
      const item* PairWielded = PairArm->GetWielded();

      if(!PairWielded)
	{
	  if(Wielded->IsTwoHanded() && !Wielded->IsShield(Master))
	    return Base * (ThisToHit + PairArm->GetAttribute(DEXTERITY) * sqrt(2.5f * Master->GetAttribute(PERCEPTION))) / 2;
	}
      else if(!Wielded->IsShield(Master) && !PairWielded->IsShield(Master))
	return Base * ThisToHit / (1.0f + (500.0f + PairWielded->GetWeight()) / (1000.0f + (Wielded->GetWeight() << 1)));
    }

  return Base * ThisToHit;
}

long arm::GetWieldedAPCost() const
{
  const item* Wielded = GetWielded();

  if(Wielded->IsShield(Master))
    return 0;

  short HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  return long(1 / (1e-14f * APBonus(GetAttribute(DEXTERITY)) * Master->GetMoveEase() * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus() * GetCurrentSWeaponSkill()->GetBonus() * Wielded->GetBonus() * Min<short>(HitStrength, 10)));
}

void head::CalculateDamage()
{
  if(!Master)
    return;

  BiteDamage = 7.07e-5f * GetBaseBiteStrength() * GetHumanoidMaster()->GetCWeaponSkill(BITE)->GetBonus();
}

void head::CalculateToHitValue()
{
  if(!Master)
    return;

  BiteToHitValue = Master->GetAttribute(AGILITY) * sqrt(2.5f * Master->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus() * Master->GetMoveEase() / 100000;
}

void head::CalculateAPCost()
{
  if(!Master)
    return;

  BiteAPCost = Max<long>(1000000000 / (APBonus(Master->GetAttribute(AGILITY)) * Master->GetMoveEase() * Master->GetCWeaponSkill(BITE)->GetBonus()), 100);
}

void leg::CalculateDamage()
{
  if(!Master)
    return;

  float WeaponStrength = GetBaseKickStrength() * GetBaseKickStrength();

  if(GetBoot())
    WeaponStrength += GetBoot()->GetWeaponStrength();

  KickDamage = sqrt(5e-10f * GetAttribute(LEG_STRENGTH) * WeaponStrength) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus();
}

void leg::CalculateToHitValue()
{
  if(!Master)
    return;

  KickToHitValue = GetAttribute(AGILITY) * sqrt(2.5f * Master->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus() * Master->GetMoveEase() / 100000;
}

void leg::CalculateAPCost()
{
  if(!Master)
    return;

  KickAPCost = Max<long>(2000000000 / (APBonus(GetAttribute(AGILITY)) * Master->GetMoveEase() * Master->GetCWeaponSkill(KICK)->GetBonus()), 100);
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
}

void corpse::AddPostFix(std::string& String) const
{
  String << " of ";
  GetDeceased()->AddName(String, INDEFINITE);
}

void corpse::GenerateLeftOvers(character* Eater)
{
  GetDeceased()->GetTorso()->GenerateLeftOvers(Eater);
  RemoveFromSlot();
  SendToHell();
}

bool corpse::IsConsumable(const character* Eater) const
{
  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c) && GetDeceased()->GetBodyPart(c)->IsConsumable(Eater))
      return true;

  return false;
}

short corpse::GetOfferValue(char GodAlignment) const
{
  short OfferValue = 0;

  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      OfferValue += GetDeceased()->GetBodyPart(c)->GetOfferValue(GodAlignment);

  return OfferValue;
}

float corpse::GetWeaponStrength() const
{
  return GetFormModifier() * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() * sqrt(GetDeceased()->GetTorso()->GetMainMaterial()->GetWeight());
}

bool corpse::CanBeEatenByAI(const character* Eater) const
{
  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c) && !GetDeceased()->GetBodyPart(c)->CanBeEatenByAI(Eater))
      return false;

  return true;
}

ushort corpse::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() / 2000;
}

corpse::~corpse()
{
  delete Deceased;
}

ushort corpse::GetMaterialColorA(ushort) const
{
  return GetDeceased()->GetTorso()->GetMainMaterial()->GetColor();
}

uchar corpse::GetAlphaA(ushort) const
{
  return GetDeceased()->GetTorso()->GetMainMaterial()->GetAlpha();
}

ushort corpse::GetMaterialColorB(ushort) const
{
  return GetDeceased()->GetTorso()->IsAlive() ? GetDeceased()->GetBloodColor() : GetDeceased()->GetTorso()->GetMainMaterial()->GetColor();
}

vector2d corpse::GetBitmapPos(ushort) const
{
  if(GetDeceased()->GetSize() < 50)
    return vector2d(32, 64);
  else if(GetDeceased()->GetSize() < 150)
    return vector2d(16, 192);
  else
    return vector2d(16, 272);
}

ushort corpse::GetSize() const
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
}

void head::DropEquipment()
{
  if(GetHelmet())
    GetSlot()->AddFriendItem(GetHelmet());

  if(GetAmulet())
    GetSlot()->AddFriendItem(GetAmulet());
}

void humanoidtorso::DropEquipment()
{
  if(GetBodyArmor())
    GetSlot()->AddFriendItem(GetBodyArmor());

  if(GetCloak())
    GetSlot()->AddFriendItem(GetCloak());

  if(GetBelt())
    GetSlot()->AddFriendItem(GetBelt());
}

void arm::DropEquipment()
{
  if(GetWielded())
    GetSlot()->AddFriendItem(GetWielded());

  if(GetGauntlet())
    GetSlot()->AddFriendItem(GetGauntlet());

  if(GetRing())
    GetSlot()->AddFriendItem(GetRing());
}

void leg::DropEquipment()
{
  if(GetBoot())
    GetSlot()->AddFriendItem(GetBoot());
}

void corpse::AddConsumeEndMessage(character* Eater) const
{
  GetDeceased()->GetTorso()->AddConsumeEndMessage(Eater);
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

long corpse::GetScore() const
{
  long Score = 0;

  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      Score += GetDeceased()->GetBodyPart(c)->GetScore();

  return Score;
}

bool corpse::IsDestroyable() const
{
  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c) && !GetDeceased()->GetBodyPart(c)->IsDestroyable())
      return false;

  return true;
}

ulong corpse::GetPrice() const
{
  ulong Price = 0;

  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      Price += GetDeceased()->GetBodyPart(c)->GetPrice();

  return Price;
}

bool corpse::Consume(character* Eater, long Amount)
{
  for(ushort c = GetDeceased()->GetBodyParts() - 1; c != 0; --c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart)
	{
	  if(BodyPart->Consume(Eater, Amount))
	    BodyPart->GenerateLeftOvers(Eater);

	  return false;
	}
    }

  return GetDeceased()->GetTorso()->Consume(Eater, Amount);
}

material* corpse::GetMaterial(ushort Index) const
{
  return GetDeceased()->GetTorso()->GetMaterial(Index);
}

ushort bodypart::GetMaterialColorA(ushort) const
{
  if(GetMainMaterial())
    return GetMainMaterial()->GetSkinColor();
  else
    return 0;
}

bool corpse::RaiseTheDead(character* Summoner)
{
  if(Summoner->IsPlayer())
    game::DoEvilDeed(50);

  GetLSquareUnder()->AddCharacter(GetDeceased());
  RemoveFromSlot();
  GetDeceased()->Enable();
  GetDeceased()->SetMotherEntity(0);
  GetDeceased()->CompleteRiseFromTheDead();
  Deceased = 0;
  SendToHell();
  return true;
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

  if(!Load)
    StrengthBonus = DexterityBonus = StrengthExperience = DexterityExperience = 0;
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
    StrengthBonus = AgilityBonus = StrengthExperience = AgilityExperience = 0;
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

bool arm::ApplyExperience()
{
  if(!IsAlive())
    return false;

  bool Edited = false;

  if(GetMaster()->CheckForAttributeIncrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels stronger!", GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks stronger.", GetMaster()->CHAR_NAME(DEFINITE));

      Edited = true;
    }
  else if(GetMaster()->CheckForAttributeDecrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels weaker.", GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks weaker.", GetMaster()->CHAR_NAME(DEFINITE));

      Edited = true;
    }

  if(GetMaster()->CheckForAttributeIncrease(Dexterity, DexterityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels quite dextrous.", GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks quite dextrous.", GetMaster()->CHAR_NAME(DEFINITE));

      Edited = true;
    }
  else if(GetMaster()->CheckForAttributeDecrease(Dexterity, DexterityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels clumsy.", GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks clumsy.", GetMaster()->CHAR_NAME(DEFINITE));

      Edited = true;
    }

  return Edited;
}

bool leg::ApplyExperience()
{
  if(!IsAlive())
    return false;

  bool Edited = false;

  if(GetMaster()->CheckForAttributeIncrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels stronger!", GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks stronger.", GetMaster()->CHAR_NAME(DEFINITE));

      GetMaster()->CalculateBurdenState();
      Edited = true;
    }
  else if(GetMaster()->CheckForAttributeDecrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels weaker.", GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks weaker.", GetMaster()->CHAR_NAME(DEFINITE));

      GetMaster()->CalculateBurdenState();
      Edited = true;
    }

  if(GetMaster()->CheckForAttributeIncrease(Agility, AgilityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels very agile!", GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks very agile.", GetMaster()->CHAR_NAME(DEFINITE));

      Edited = true;
    }
  else if(GetMaster()->CheckForAttributeDecrease(Agility, AgilityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels slower.", GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks sluggish.", GetMaster()->CHAR_NAME(DEFINITE));

      Edited = true;
    }

  return Edited;
}

void arm::Hit(character* Enemy, bool ForceHit)
{
  switch(Enemy->TakeHit(GetMaster(), GetWielded() ? GetWielded() : GetGauntlet(), GetDamage(), GetToHitValue(), RAND() % 26 - RAND() % 26, GetWielded() ? WEAPON_ATTACK : UNARMED_ATTACK, !(RAND() % GetMaster()->GetCriticalModifier()), ForceHit))
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case HAS_DIED:
    case DID_NO_DAMAGE:
      EditExperience(ARM_STRENGTH, 50);
    case HAS_DODGED:
      EditExperience(DEXTERITY, 25);
    }
}

ushort arm::GetAttribute(ushort Identifier) const
{
  if(Identifier == ARM_STRENGTH)
    {
      if(IsAlive())
	return Max(Strength + StrengthBonus, 1);
      else
	return Max(GetMainMaterial()->GetStrengthValue() + StrengthBonus, 1);
    }
  else if(Identifier == DEXTERITY)
    {
      if(IsAlive())
	return Max(Dexterity + DexterityBonus, 1);
      else
	return Max((GetMainMaterial()->GetFlexibility() << 2) + DexterityBonus, 1);
    }
  else
    {
      ABORT("Illegal arm attribute %d request!", Identifier);
      return 0xACCA;
    }
}

bool arm::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier == ARM_STRENGTH)
    return IsAlive() && GetMaster()->RawEditAttribute(Strength, Value);
  else if(Identifier == DEXTERITY)
    return IsAlive() && GetMaster()->RawEditAttribute(Dexterity, Value);
  else
    {
      ABORT("Illegal arm attribute %d edit request!", Identifier);
      return false;
    }
}

void arm::EditExperience(ushort Identifier, long Value)
{
  if(Identifier == ARM_STRENGTH)
    {
      if(IsAlive())
	StrengthExperience += Value << 1;
    }
  else if(Identifier == DEXTERITY)
    {
      if(IsAlive())
	DexterityExperience += Value << 1;
    }
  else
    ABORT("Illegal arm attribute %d experience edit request!", Identifier);
}

ushort leg::GetAttribute(ushort Identifier) const
{
  if(Identifier == LEG_STRENGTH)
    {
      if(IsAlive())
	return Max(Strength + StrengthBonus, 1);
      else
	return Max(GetMainMaterial()->GetStrengthValue() + StrengthBonus, 1);
    }
  else if(Identifier == AGILITY)
    {
      if(IsAlive())
	return Max(Agility + AgilityBonus, 1);
      else
	return Max((GetMainMaterial()->GetFlexibility() << 2) + AgilityBonus, 1);
    }
  else
    {
      ABORT("Illegal leg attribute %d request!", Identifier);
      return 0xECCE;
    }
}

bool leg::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier == LEG_STRENGTH)
    return IsAlive() && GetMaster()->RawEditAttribute(Strength, Value);
  else if(Identifier == AGILITY)
    return IsAlive() && GetMaster()->RawEditAttribute(Agility, Value);
  else
    {
      ABORT("Illegal leg attribute %d edit request!", Identifier);
      return false;
    }
}

void leg::EditExperience(ushort Identifier, long Value)
{
  if(Identifier == LEG_STRENGTH)
    {
      if(IsAlive())
	StrengthExperience += Value << 1;
    }
  else if(Identifier == AGILITY)
    {
      if(IsAlive())
	AgilityExperience += Value << 1;
    }
  else
    ABORT("Illegal leg attribute %d experience edit request!", Identifier);
}

void arm::RaiseStats()
{
  Strength += 10;
  Dexterity += 10;
}

void arm::LowerStats()
{
  Strength -= 10;
  Dexterity -= 10;
}

void leg::RaiseStats()
{
  Strength += 10;
  Agility += 10;
}

void leg::LowerStats()
{
  Strength -= 10;
  Agility -= 10;
}

void head::InitSpecialAttributes()
{
  bodypart::InitSpecialAttributes();
  BaseBiteStrength = Master->GetBaseBiteStrength();
}

void arm::InitSpecialAttributes()
{
  bodypart::InitSpecialAttributes();

  if(!Master->IsPlayer() || Master->IsInitializing())
    {
      Strength = Master->GetDefaultArmStrength() * (100 + Master->GetAttributeBonus()) / 100;
      Dexterity = Master->GetDefaultDexterity() * (100 + Master->GetAttributeBonus()) / 100;
    }
  else
    {
      Strength = ushort(game::GetAveragePlayerArmStrength());
      Dexterity = ushort(game::GetAveragePlayerDexterity());
    }

  BaseUnarmedStrength = Master->GetBaseUnarmedStrength();
}

void leg::InitSpecialAttributes()
{
  bodypart::InitSpecialAttributes();

  if(!Master->IsPlayer() || Master->IsInitializing())
    {
      Strength = Master->GetDefaultLegStrength() * (100 + Master->GetAttributeBonus()) / 100;
      Agility = Master->GetDefaultAgility() * (100 + Master->GetAttributeBonus()) / 100;
    }
  else
    {
      Strength = ushort(game::GetAveragePlayerLegStrength());
      Agility = ushort(game::GetAveragePlayerAgility());
    }

  BaseKickStrength = Master->GetBaseKickStrength();
}

std::string corpse::GetConsumeVerb() const
{
  return GetDeceased()->GetTorso()->GetConsumeVerb();
}

void bodypart::SignalEquipmentAdd(gearslot* Slot)
{
  if(GetMaster())
    GetMaster()->SignalEquipmentAdd(Slot->GetEquipmentIndex());
}

void bodypart::SignalEquipmentRemoval(gearslot* Slot)
{
  if(GetMaster())
    GetMaster()->SignalEquipmentRemoval(Slot->GetEquipmentIndex());
}

void bodypart::Mutate()
{
  GetMainMaterial()->SetVolume(ulong(GetVolume() * (1.5f - float(RAND() % 1000) / 1000)));
}

void leg::Mutate()
{
  bodypart::Mutate();
  SetAgility(ushort(GetAgility() * (1.5f - float(RAND() % 1000) / 1000)));
  SetStrength(ushort(GetStrength() * (1.5f - float(RAND() % 1000) / 1000)));
}

void arm::Mutate()
{
  bodypart::Mutate();
  SetStrength(ulong(GetStrength() * (1.5f - float(RAND() % 1000) / 1000)));
  SetDexterity(ulong(GetDexterity() * (1.5f - float(RAND() % 1000) / 1000)));
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

sweaponskill* rightarm::GetCurrentSWeaponSkill() const
{
  return GetHumanoidMaster()->GetCurrentRightSWeaponSkill();
}

sweaponskill* leftarm::GetCurrentSWeaponSkill() const
{
  return GetHumanoidMaster()->GetCurrentLeftSWeaponSkill();
}

uchar bodypart::GetMaxAlpha(ushort) const
{
  if(GetMaster() && GetMaster()->StateIsActivated(INVISIBLE))
    return 150;
  else
    return 255;
}

void bodypart::AddPostFix(std::string& String) const
{
  if(OwnerDescription.length())
    String << ' ' << OwnerDescription;
}

void corpse::CalculateVolumeAndWeight()
{
  Volume = Deceased->GetVolume();
  Weight = Deceased->GetWeight();
}

item* head::GetEquipment(ushort Index) const
{
  switch(Index)
    {
    case 0: return GetHelmet();
    case 1: return GetAmulet();
    default: return 0;
    }
}

item* humanoidtorso::GetEquipment(ushort Index) const
{
  switch(Index)
    {
    case 0: return GetBodyArmor();
    case 1: return GetCloak();
    case 2: return GetBelt();
    default: return 0;
    }
}

item* arm::GetEquipment(ushort Index) const
{
  switch(Index)
    {
    case 0: return GetWielded();
    case 1: return GetGauntlet();
    case 2: return GetRing();
    default: return 0;
    }
}

item* leg::GetEquipment(ushort Index) const
{
  if(!Index)
    return GetBoot();
  else
    return 0;
}

void bodypart::CalculateVolumeAndWeight()
{
  item::CalculateVolumeAndWeight();
  CarriedWeight = 0;
  BodyPartVolume = Volume;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c))
      {
	Volume += GetEquipment(c)->GetVolume();
	CarriedWeight += GetEquipment(c)->GetWeight();
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

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    if(GetEquipment(c))
      game::CombineLights(Emitation, GetEquipment(c)->GetEmitation());
}

void bodypart::CalculateMaxHP(bool MayChangeHPs)
{
  short HPDelta = MaxHP - HP;
  MaxHP = 0;

  if(GetMaster())
    {
      if(IsAlive())
	{
	  long Endurance = GetMaster()->GetAttribute(ENDURANCE);
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

  if(GetMaster() && !GetMaster()->IsInitializing())
    {
      CalculateMaxHP();
      GetMaster()->CalculateMaxHP();
    }
}

void bodypart::SetHP(short What)
{
  HP = What;

  if(GetMaster())
    GetMaster()->CalculateHP();
}

void bodypart::EditHP(short What)
{
  HP += What;

  if(GetMaster())
    GetMaster()->CalculateHP();
}

void arm::AddAttackInfo(felist& List) const
{
  if(GetDamage())
    {
      std::string Entry = "   ";

      if(GetWielded())
	{
	  GetWielded()->AddName(Entry, UNARTICLED);

	  if(TwoHandWieldIsActive())
	    Entry << " (b)";
	}
      else
	Entry << "melee attack";

      Entry.resize(50, ' ');
      Entry << GetMinDamage() << '-' << GetMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetToHitValue());
      Entry.resize(70, ' ');
      Entry << GetAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

void arm::AddDefenceInfo(felist& List) const
{
  if(GetWielded())
    {
      std::string Entry = "   ";
      GetWielded()->AddName(Entry, UNARTICLED);
      Entry.resize(50, ' ');
      Entry << int(GetBlockValue());
      Entry.resize(70, ' ');
      Entry << GetBlockCapability();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

void arm::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(GetMaster() && !GetMaster()->IsInitializing())
    {
      CalculateAttributeBonuses();
      CalculateAttackInfo();
    }
}

void leg::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(GetMaster() && !GetMaster()->IsInitializing())
    {
      CalculateAttributeBonuses();
      CalculateAttackInfo();
    }
}


void humanoidtorso::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(GetMaster() && !GetMaster()->IsInitializing())
    {
      if(GetHumanoidMaster()->GetRightArm())
	GetHumanoidMaster()->GetRightArm()->CalculateAttributeBonuses();

      if(GetHumanoidMaster()->GetLeftArm())
	GetHumanoidMaster()->GetLeftArm()->CalculateAttributeBonuses();

      if(GetHumanoidMaster()->GetRightLeg())
	GetHumanoidMaster()->GetRightLeg()->CalculateAttributeBonuses();

      if(GetHumanoidMaster()->GetLeftLeg())
	GetHumanoidMaster()->GetLeftLeg()->CalculateAttributeBonuses();
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
  if(GetWielded()->IsTwoHanded() && !GetWielded()->IsShield(Master))
    {
      arm* PairArm = GetPairArm();
      return PairArm && !PairArm->GetWielded();
    }
  else
    return false;
}

float bodypart::GetTimeToDie(ushort Damage, float ToHitValue, float DodgeValue, bool AttackIsBlockable, bool UseMaxHP) const
{
  float Durability;
  short TrueDamage = (19 * (Max((Damage * 3 >> 2) - GetTotalResistance(PHYSICAL_DAMAGE), 0)
			 +  Max((Damage * 5 >> 2) + 1 - (GetTotalResistance(PHYSICAL_DAMAGE) >> 1), 0))
			 + (Max((Damage * 3 >> 1) - GetTotalResistance(PHYSICAL_DAMAGE), 0)
			 +  Max((Damage * 5 >> 1) + 3 - (GetTotalResistance(PHYSICAL_DAMAGE) >> 1), 0))) / 40;

  short HP = UseMaxHP ? GetMaxHP() : GetHP();

  if(TrueDamage > 0)
    {
      float AverageDamage;

      if(AttackIsBlockable)
	{
	  blockvector Block;
	  Master->CreateBlockPossibilityVector(Block, ToHitValue);

	  if(Block.size())
	    {
	      float ChanceForNoBlock = 1.0f;
	      AverageDamage = 0;

	      for(ushort c = 0; c < Block.size(); ++c)
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

float bodypart::GetRoughChanceToHit(float ToHitValue, float DodgeValue) const
{
  return GLOBAL_WEAK_BODYPART_HIT_MODIFIER * ToHitValue * GetBodyPartVolume() / ((DodgeValue / ToHitValue + 1) * DodgeValue * Master->GetBodyVolume() * 100);
}

float torso::GetRoughChanceToHit(float ToHitValue, float DodgeValue) const
{
  return 1 / (DodgeValue / ToHitValue + 1);
}

void bodypart::RandomizePosition()
{
  SpecialFlags |= 1 + RAND() % 7;
  UpdatePictures();
}

float arm::GetBlockChance(float EnemyToHitValue) const
{
  return GetWielded() ? Min(1.0f / (1 + EnemyToHitValue / (GetToHitValue() * GetWielded()->GetBlockModifier()) * 10000), 1.0f) : 0;
}

ushort arm::GetBlockCapability() const
{
  if(!GetWielded())
    return 0;

  short HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  return Min<short>(HitStrength, 10) * GetWielded()->GetStrengthValue() * GetHumanoidMaster()->GetCWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSWeaponSkill()->GetBonus() / 100000;
}

void arm::WieldedSkillHit()
{
  if(Master->GetCWeaponSkill(GetWielded()->GetWeaponCategory())->AddHit())
    {
      CalculateAttackInfo();

      if(Master->IsPlayer())
	GetHumanoidMaster()->GetCWeaponSkill(GetWielded()->GetWeaponCategory())->AddLevelUpMessage();
    }

  if(GetCurrentSWeaponSkill()->AddHit())
    {
      CalculateAttackInfo();

      if(Master->IsPlayer())
	GetCurrentSWeaponSkill()->AddLevelUpMessage(GetWielded()->GetName(UNARTICLED));
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

arm::arm(const arm& Arm) : bodypart(Arm), Strength(Arm.Strength), Dexterity(Arm.Dexterity), StrengthExperience(Arm.StrengthExperience), DexterityExperience(Arm.DexterityExperience), BaseUnarmedStrength(Arm.BaseUnarmedStrength)
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

leg::leg(const leg& Leg) : bodypart(Leg), Strength(Leg.Strength), Agility(Leg.Agility), StrengthExperience(Leg.StrengthExperience), AgilityExperience(Leg.AgilityExperience), BaseKickStrength(Leg.BaseKickStrength)
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

bool bodypart::IsVeryCloseToSpoiling() const
{
  return MainMaterial->IsVeryCloseToSpoiling();
}

void corpse::SignalSpoil(material*)
{
  bool TorsoSpoiled = false;

  if(GetDeceased()->GetTorso()->IsVeryCloseToSpoiling())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s spoils.", CHAR_NAME(DEFINITE));

      TorsoSpoiled = true;
    }

  for(ushort c = 1; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart)
	if(BodyPart->IsVeryCloseToSpoiling())
	  {
	    if(!TorsoSpoiled && CanBeSeenByPlayer())
	      ADD_MESSAGE("The %s of %s spoils.", GetDeceased()->GetBodyPartName(c).c_str(), GetDeceased()->CHAR_NAME(DEFINITE));

	    GetDeceased()->SevereBodyPart(c)->SendToHell();
	  }
	else if(TorsoSpoiled)
	  GetSlot()->AddFriendItem(GetDeceased()->SevereBodyPart(c));
    }

  if(TorsoSpoiled)
    {
      RemoveFromSlot();
      SendToHell();
    }
}

bool bodypart::CanBePiledWith(const item* Item, const character* Viewer) const
{
  return item::CanBePiledWith(Item, Viewer) && OwnerDescription == static_cast<const bodypart*>(Item)->OwnerDescription;
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

  for(ushort c = 0; c < Deceased->GetBodyParts(); ++c)
    {
      bodypart* BodyPart1 = Deceased->GetBodyPart(c);
      bodypart* BodyPart2 = Corpse->Deceased->GetBodyPart(c);

      if(BodyPart1 == 0 && BodyPart2 == 0)
	continue;

      if(BodyPart1 == 0 || BodyPart2 == 0 || !BodyPart1->CanBePiledWith(BodyPart2, Viewer))
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
      if(Master->IsPolymorphed())
	return;

      if(Master->IsEnabled())
	{
	  if(IsInBadCondition() && !(RAND() & 0xF))
	    SpillBlood(1);

	  return;
	}
    }

  if(HP < MaxHP && !(RAND() & 0xF))
    {
      SpillBlood(1);
      HP += Max(((MaxHP - HP) >> 2), 1);
    }

  item::Be();
}

void bodypart::SpillBlood(ushort HowMuch, vector2d GetPos)
{
  if(!HowMuch || (Master && !Master->SpillsBlood()) || !IsAlive())
    return;

  if(!game::IsInWilderness()) 
    GetNearLSquare(GetPos)->SpillFluid(HowMuch, GetBloodColor(), 5, 60);
}

void bodypart::SpillBlood(ushort HowMuch)
{
  if(!HowMuch || (Master && !Master->SpillsBlood()) || !IsAlive())
    return;

  if(!game::IsInWilderness()) 
    GetLSquareUnder()->SpillFluid(HowMuch, GetBloodColor(), 5, 60);
}

void bodypart::InitSpecialAttributes()
{
  BloodColor = Master->GetBloodColor();
}

void bodypart::SignalEnchantmentChange()
{
  if(GetMaster() && !GetMaster()->IsInitializing())
    {
      GetMaster()->CalculateAttributeBonuses();
      GetMaster()->CalculateBattleInfo();
    }
}

void arm::SignalEquipmentAdd(gearslot* Slot)
{
  if(GetMaster())
    {
      ushort EquipmentIndex = Slot->GetEquipmentIndex();
      item* Equipment = Slot->GetItem();

      if(Equipment->IsInCorrectSlot(EquipmentIndex))
	ApplyEquipmentAttributeBonuses(Equipment);

      if(EquipmentIndex == RIGHT_GAUNTLET_INDEX || EquipmentIndex == LEFT_GAUNTLET_INDEX)
	ApplyDexterityPenalty(Equipment);

      GetMaster()->SignalEquipmentAdd(EquipmentIndex);
    }
}

void leg::SignalEquipmentAdd(gearslot* Slot)
{
  if(GetMaster())
    {
      ushort EquipmentIndex = Slot->GetEquipmentIndex();
      item* Equipment = Slot->GetItem();

      if(Equipment->IsInCorrectSlot(EquipmentIndex))
	ApplyEquipmentAttributeBonuses(Equipment);

      if(EquipmentIndex == RIGHT_BOOT_INDEX || EquipmentIndex == LEFT_BOOT_INDEX)
	ApplyAgilityPenalty(Equipment);

      GetMaster()->SignalEquipmentAdd(EquipmentIndex);
    }
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

      if(GetMaster())
	GetMaster()->CalculateBurdenState();
    }

  if(Item->AffectsAgility())
    AgilityBonus += Item->GetEnchantment();
}

void arm::CalculateAttributeBonuses()
{
  StrengthBonus = DexterityBonus = 0;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment->IsInCorrectSlot())
	ApplyEquipmentAttributeBonuses(Equipment);
    }

  ApplyDexterityPenalty(GetGauntlet());

  if(GetMaster())
    {
      ApplyDexterityPenalty(GetHumanoidMaster()->GetCloak());
      ApplyDexterityPenalty(GetHumanoidMaster()->GetBodyArmor());
    }
}

void leg::CalculateAttributeBonuses()
{
  StrengthBonus = AgilityBonus = 0;

  for(ushort c = 0; c < GetEquipmentSlots(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment->IsInCorrectSlot())
	ApplyEquipmentAttributeBonuses(Equipment);
    }

  ApplyAgilityPenalty(GetBoot());

  if(GetMaster())
    {
      ApplyAgilityPenalty(GetHumanoidMaster()->GetCloak());
      ApplyAgilityPenalty(GetHumanoidMaster()->GetBodyArmor());
    }
}

void humanoidtorso::SignalEquipmentAdd(gearslot* Slot)
{
  if(!GetMaster())
    return;

  humanoid* Master = GetHumanoidMaster();
  ushort EquipmentIndex = Slot->GetEquipmentIndex();

  if(EquipmentIndex == CLOAK_INDEX || EquipmentIndex == BODY_ARMOR_INDEX)
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

short arm::GetWieldedHitStrength() const
{
  ushort HitStrength = GetAttribute(ARM_STRENGTH);
  ushort Requirement = GetWielded()->GetStrengthRequirement();

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
    DexterityBonus -= Item->GetInElasticityPenalty(IsAlive() ? Dexterity : GetMainMaterial()->GetFlexibility());
}

void leg::ApplyAgilityPenalty(item* Item)
{
  if(Item)
    AgilityBonus -= Item->GetInElasticityPenalty(IsAlive() ? Agility : GetMainMaterial()->GetFlexibility());
}

uchar corpse::GetSpoilLevel() const
{
  uchar FlyAmount = 0;

  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart && FlyAmount < BodyPart->GetSpoilLevel())
	FlyAmount = BodyPart->GetSpoilLevel();
    }

  return FlyAmount;
}

void bodypart::SignalSpoilLevelChange(material* Material)
{
  if(GetMaster())
    GetMaster()->SignalSpoilLevelChange();
  else
    item::SignalSpoilLevelChange(Material);
}

void arm::ShowAttackInfo() const
{
  if(GetWielded())
    ShowWieldedAttackInfo();
  else
    ShowUnarmedInfo();
}

void arm::ShowWieldedAttackInfo() const
{
  felist Info(festring::CapitalizeCopy(GetBodyPartName()) + " attack info:");
  game::SetStandardListAttributes(Info);
  ushort HitStrength = GetAttribute(ARM_STRENGTH);
  ushort Requirement = GetWielded()->GetStrengthRequirement();
  short Bonus;

  if(TwoHandWieldIsActive())
    {
      Info.AddEntry("Wielded: " + GetWielded()->GetName(INDEFINITE) + " (in both hands)", MakeRGB16(220, 220, 220), 0, GetWielded()->GetPicture());
      HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
      Requirement >>= 2;

      if(HitStrength <= Requirement)
	{
	  Info.AddEntry(std::string("You cannot use this weapon. Wielding it with two hands requires ") + ((Requirement >> 1) + 1) + " strength.", RED);
	  Info.AddEntry("", LIGHT_GRAY);
	  Info.Draw();
	  return;
	}
    }
  else
    {
      Info.AddEntry("Wielded: " + GetWielded()->GetName(INDEFINITE), MakeRGB16(220, 220, 220), 0, GetWielded()->GetPicture());

      if(HitStrength <= Requirement)
	{
	  Info.AddEntry(std::string("You cannot use this weapon. Wielding it with one hand requires ") + (Requirement + 1) + " strength.", RED);
	  Info.AddEntry("", LIGHT_GRAY);
	  Info.Draw();
	  return;
	}
    }

  bool Shown = false;
  Bonus = GetHumanoidMaster()->GetCWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus();

  if(Bonus > 100)
    {
      Info.AddEntry("", LIGHT_GRAY);
      Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
      Info.AddEntry(std::string("Category weapon skill bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
      Shown = true;
    }

  Bonus = GetCurrentSWeaponSkill()->GetBonus();

  if(Bonus > 100)
    {
      if(!Shown)
	{
	  Info.AddEntry("", LIGHT_GRAY);
	  Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
	}

      Info.AddEntry(std::string("Accustomization bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
    }

  Info.AddEntry("", LIGHT_GRAY);

  /* Damage */

  Info.AddEntry("Damage: how much you can hurt an enemy", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry(std::string("Base: ") + GetWielded()->GetBaseMinDamage() + '-' + GetWielded()->GetBaseMaxDamage(), LIGHT_GRAY);

  if(HitStrength > 10)
    Info.AddEntry(std::string("Strength bonus: ") + '+' + int(sqrt(1000 * HitStrength) - 100) + '%', LIGHT_GRAY);
  else if(HitStrength < 10)
    Info.AddEntry(std::string("Strength penalty: ") + int(sqrt(1000 * HitStrength) - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real damage: ") + GetMinDamage() + '-' + GetMaxDamage(), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* To Hit Value */

  Info.AddEntry("To hit value: determines your chance to hit", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry(std::string("Base: ") + GetWielded()->GetBaseToHitValue(), LIGHT_GRAY);

  if(HitStrength - Requirement < 10)
    Info.AddEntry(std::string("Strength penalty: ") + ((HitStrength - Requirement) * 10 - 100) + '%', LIGHT_GRAY);

  Bonus = GetAttribute(DEXTERITY) * 10;

  if(TwoHandWieldIsActive())
    Bonus = (Bonus >> 1) + GetPairArm()->GetAttribute(DEXTERITY) * 5;

  if(Bonus > 100)
    Info.AddEntry(std::string("Dexterity bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Dexterity penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  Bonus = short(sqrt(GetMaster()->GetAttribute(PERCEPTION) * 1000));

  if(Bonus > 100)
    Info.AddEntry(std::string("Perception bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Perception penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(!GetWielded()->IsShield(GetMaster()) && GetPairArm() && GetPairArm()->GetWielded() && !GetPairArm()->GetWielded()->IsShield(GetMaster()))
    {
      Bonus = short(-100.0f / (1.0f + (1000.0f + (GetWielded()->GetWeight() << 1)) / (500.0f + GetPairArm()->GetWielded()->GetWeight())));

      if(Bonus < 0)
	Info.AddEntry(std::string("Penalty for using a weapon in another hand: ") + Bonus + '%', LIGHT_GRAY);
    }

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real to hit value: ") + int(GetToHitValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Speed */

  Info.AddEntry("Speed: how often you are able to strike", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry(std::string("Base: ") + GetWielded()->GetBonus() / 10, LIGHT_GRAY);

  if(HitStrength - Requirement < 10)
    Info.AddEntry(std::string("Strength penalty: ") + (10 * (HitStrength - Requirement) - 100) + '%', LIGHT_GRAY);

  Bonus = APBonus(GetAttribute(DEXTERITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Dexterity bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Dexterity penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real speed: ") + 10000 / GetAPCost(), MakeRGB16(220, 220, 220));
  Info.Draw();
}

void arm::ShowDefenceInfo() const
{
  felist Info(festring::CapitalizeCopy(GetBodyPartName()) + " defence info:");
  game::SetStandardListAttributes(Info);
  ushort HitStrength = GetAttribute(ARM_STRENGTH);
  ushort Requirement = GetWielded()->GetStrengthRequirement();
  short Bonus;

  if(TwoHandWieldIsActive())
    {
      Info.AddEntry("Wielded: " + GetWielded()->GetName(INDEFINITE) + " (in both hands)", MakeRGB16(220, 220, 220), 0, GetWielded()->GetPicture());
      HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
      Requirement >>= 2;

      if(HitStrength <= Requirement)
	{
	  Info.AddEntry(std::string("You cannot use this weapon. Wielding it with two hands requires ") + ((Requirement >> 1) + 1) + " strength.", RED);
	  Info.AddEntry("", LIGHT_GRAY);
	  Info.Draw();
	  return;
	}
    }
  else
    {
      Info.AddEntry("Wielded: " + GetWielded()->GetName(INDEFINITE), MakeRGB16(220, 220, 220), 0, GetWielded()->GetPicture());

      if(HitStrength <= Requirement)
	{
	  Info.AddEntry(std::string("You cannot use this weapon. Wielding it with one hand requires ") + (Requirement + 1) + " strength.", RED);
	  Info.AddEntry("", LIGHT_GRAY);
	  Info.Draw();
	  return;
	}
    }

  bool Shown = false;
  Bonus = GetHumanoidMaster()->GetCWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus();

  if(Bonus > 100)
    {
      Info.AddEntry("", LIGHT_GRAY);
      Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
      Info.AddEntry(std::string("Category weapon skill bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
      Shown = true;
    }

  Bonus = GetCurrentSWeaponSkill()->GetBonus();

  if(Bonus > 100)
    {
      if(!Shown)
	{
	  Info.AddEntry("", LIGHT_GRAY);
	  Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
	}

      Info.AddEntry(std::string("Accustomization bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
    }

  Info.AddEntry("", LIGHT_GRAY);

  /* Block capability */

  Info.AddEntry("Block capability: how much damage you are able to block", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry(std::string("Base: ") + GetWielded()->GetStrengthValue(), LIGHT_GRAY);

  if(HitStrength - Requirement < 10)
    Info.AddEntry(std::string("Strength penalty: ") + (10 * (HitStrength - Requirement) - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real block capability: ") + GetBlockCapability(), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Block value */

  Info.AddEntry("Block value: determines your chance to block an attack", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry(std::string("Base: ") + GetWielded()->GetBaseBlockValue(), LIGHT_GRAY);

  if(HitStrength - Requirement < 10)
    Info.AddEntry(std::string("Strength penalty: ") + ((HitStrength - Requirement) * 10 - 100) + '%', LIGHT_GRAY);

  Bonus = GetAttribute(DEXTERITY) * 10;

  if(TwoHandWieldIsActive())
    Bonus = (Bonus >> 1) + GetPairArm()->GetAttribute(DEXTERITY) * 5;

  if(Bonus > 100)
    Info.AddEntry(std::string("Dexterity bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Dexterity penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  Bonus = short(sqrt(GetMaster()->GetAttribute(PERCEPTION) * 1000));

  if(Bonus > 100)
    Info.AddEntry(std::string("Perception bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Perception penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(!GetWielded()->IsShield(GetMaster()) && GetPairArm() && GetPairArm()->GetWielded() && !GetPairArm()->GetWielded()->IsShield(GetMaster()))
    {
      Bonus = short(-100.0f / (1.0f + (1000.0f + (GetWielded()->GetWeight() << 1)) / (500.0f + GetPairArm()->GetWielded()->GetWeight())));

      if(Bonus < 0)
	Info.AddEntry(std::string("Penalty for using a weapon in another hand: ") + Bonus + '%', LIGHT_GRAY);
    }

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real block value: ") + int(GetBlockValue()), MakeRGB16(220, 220, 220));
  Info.Draw();
}

void arm::ShowUnarmedInfo() const
{
  felist Info(festring::CapitalizeCopy(GetBodyPartName()) + " unarmed attack info:");
  game::SetStandardListAttributes(Info);
  float WeaponStrength = GetBaseUnarmedStrength() * GetBaseUnarmedStrength();
  short Bonus;

  if(GetGauntlet())
    {
      Info.AddEntry("Worn: " + GetGauntlet()->GetName(INDEFINITE), MakeRGB16(220, 220, 220), 0, GetGauntlet()->GetPicture());
      WeaponStrength += GetGauntlet()->GetWeaponStrength();
      Info.AddEntry("", LIGHT_GRAY);
    }

  Bonus = GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetBonus();

  if(Bonus > 100)
    {
      Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
      Info.AddEntry("", LIGHT_GRAY);
      Info.AddEntry(std::string("Category weapon skill bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
    }

  /* Damage */

  Info.AddEntry("Damage: how much you can hurt an enemy", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  float Damage = sqrt(WeaponStrength / 20000);
  Info.AddEntry(std::string("Base: ") + int(Damage * 0.75f) + '-' + int(Damage * 1.25f + 1), LIGHT_GRAY);

  if(GetAttribute(ARM_STRENGTH) > 10)
    Info.AddEntry(std::string("Strength bonus: ") + '+' + int(sqrt(1000 * GetAttribute(ARM_STRENGTH)) - 100) + '%', LIGHT_GRAY);
  else if(GetAttribute(ARM_STRENGTH) < 10)
    Info.AddEntry(std::string("Strength penalty: ") + int(sqrt(1000 * GetAttribute(ARM_STRENGTH)) - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real damage: ") + GetMinDamage() + '-' + GetMaxDamage(), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* To hit value */

  Info.AddEntry("To hit value: determines your chance to hit", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 10", LIGHT_GRAY);
  Bonus = GetAttribute(DEXTERITY) * 10;

  if(Bonus > 100)
    Info.AddEntry(std::string("Dexterity bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Dexterity penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  Bonus = short(sqrt(GetMaster()->GetAttribute(PERCEPTION) * 1000));

  if(Bonus > 100)
    Info.AddEntry(std::string("Perception bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Perception penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real to hit value: ") + int(GetUnarmedToHitValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Speed */

  Info.AddEntry("Speed: how often you are able to strike", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 10", LIGHT_GRAY);
  Bonus = APBonus(GetAttribute(DEXTERITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Dexterity bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Dexterity penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real speed: ") + 10000 / GetUnarmedAPCost(), MakeRGB16(220, 220, 220));
  Info.Draw();
}

void leg::ShowKickInfo() const
{
  felist Info("Kick attack info (" + GetBodyPartName() + "):");
  game::SetStandardListAttributes(Info);
  float WeaponStrength = GetBaseKickStrength() * GetBaseKickStrength();
  short Bonus;

  if(GetBoot())
    {
      Info.AddEntry("Worn: " + GetBoot()->GetName(INDEFINITE), MakeRGB16(220, 220, 220), 0, GetBoot()->GetPicture());
      WeaponStrength += GetBoot()->GetWeaponStrength();
      Info.AddEntry("", LIGHT_GRAY);
    }

  Bonus = GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus();

  if(Bonus > 100)
    {
      Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
      Info.AddEntry("", LIGHT_GRAY);
      Info.AddEntry(std::string("Category weapon skill bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
    }

  /* Damage */

  Info.AddEntry("Damage: how much you can hurt an enemy", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  float Damage = sqrt(WeaponStrength / 20000);
  Info.AddEntry(std::string("Base: ") + int(Damage * 0.75f) + '-' + int(Damage * 1.25f + 1), LIGHT_GRAY);

  if(GetAttribute(LEG_STRENGTH) > 10)
    Info.AddEntry(std::string("Strength bonus: ") + '+' + int(sqrt(1000 * GetAttribute(LEG_STRENGTH)) - 100) + '%', LIGHT_GRAY);
  else if(GetAttribute(LEG_STRENGTH) < 10)
    Info.AddEntry(std::string("Strength penalty: ") + int(sqrt(1000 * GetAttribute(LEG_STRENGTH)) - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real damage: ") + GetKickMinDamage() + '-' + GetKickMaxDamage(), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* To hit value */

  Info.AddEntry("To hit value: determines your chance to hit", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 5", LIGHT_GRAY);
  Bonus = GetAttribute(AGILITY) * 10;

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  Bonus = short(sqrt(GetMaster()->GetAttribute(PERCEPTION) * 1000));

  if(Bonus > 100)
    Info.AddEntry(std::string("Perception bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Perception penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real to hit value: ") + int(GetKickToHitValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Speed */

  Info.AddEntry("Speed: how often you are able to strike", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 5", LIGHT_GRAY);
  Bonus = APBonus(GetAttribute(AGILITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real speed: ") + 10000 / GetKickAPCost(), MakeRGB16(220, 220, 220));
  Info.Draw();
}

void head::ShowBiteInfo() const
{
  felist Info("Bite attack info:");
  game::SetStandardListAttributes(Info);
  short Bonus = GetHumanoidMaster()->GetCWeaponSkill(BITE)->GetBonus();

  if(Bonus > 100)
    {
      Info.AddEntry("Bonuses common to all values:", LIGHT_GRAY);
      Info.AddEntry(std::string("Category weapon skill bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
      Info.AddEntry("", LIGHT_GRAY);
    }

  /* Damage */

  Info.AddEntry("Damage: how much you can hurt an enemy", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  float Damage = 7.07e-3f * GetBaseBiteStrength();
  Info.AddEntry(std::string("Base: ") + int(Damage * 0.75f) + '-' + int(Damage * 1.25f + 1), LIGHT_GRAY);
  Info.AddEntry(std::string("Real damage: ") + GetBiteMinDamage() + '-' + GetBiteMaxDamage(), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* To hit value */

  Info.AddEntry("To hit value: determines your chance to hit", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 5", LIGHT_GRAY);
  Bonus = GetMaster()->GetAttribute(AGILITY) * 10;

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  Bonus = short(sqrt(GetMaster()->GetAttribute(PERCEPTION) * 1000));

  if(Bonus > 100)
    Info.AddEntry(std::string("Perception bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Perception penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real to hit value: ") + int(GetBiteToHitValue()), MakeRGB16(220, 220, 220));
  Info.AddEntry("", LIGHT_GRAY);

  /* Speed */

  Info.AddEntry("Speed: how often you are able to strike", LIGHT_GRAY);
  Info.AddEntry("", LIGHT_GRAY);
  Info.AddEntry("Base: 10", LIGHT_GRAY);
  Bonus = APBonus(GetMaster()->GetAttribute(AGILITY));

  if(Bonus > 100)
    Info.AddEntry(std::string("Agility bonus: ") + '+' + (Bonus - 100) + '%', LIGHT_GRAY);
  else if(Bonus < 100)
    Info.AddEntry(std::string("Agility penalty: ") + (Bonus - 100) + '%', LIGHT_GRAY);

  if(GetMaster()->GetMoveEase() < 100)
    Info.AddEntry(std::string("Penalty for carrying too much: ") + (GetMaster()->GetMoveEase() - 100) + '%', LIGHT_GRAY);

  Info.AddEntry(std::string("Real speed: ") + 10000 / GetBiteAPCost(), MakeRGB16(220, 220, 220));
  Info.Draw();
}

bool head::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  long AV[3] = { 0, 0, 0 };
  item* Armor[3];
  bool AnyArmor = false;

  if((Armor[0] = GetHelmet()))
    {
      AV[0] = Max<ushort>(Armor[0]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  if((Armor[1] = GetHumanoidMaster()->GetBodyArmor()))
    {
      AV[1] = Max<ushort>(Armor[1]->GetStrengthValue() >> 2, 1);
      AnyArmor = true;
    }

  if((Armor[2] = GetHumanoidMaster()->GetCloak()))
    {
      AV[2] = Max<ushort>(Armor[2]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  return AnyArmor ? Armor[femath::WeightedRand(AV, 3)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool humanoidtorso::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  long AV[3] = { 0, 0, 0 };
  item* Armor[3];
  bool AnyArmor = false;

  if((Armor[0] = GetBodyArmor()))
    {
      AV[0] = Max<ushort>(Armor[0]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  if((Armor[1] = GetBelt()))
    {
      AV[1] = Max<ushort>(Armor[1]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  if((Armor[2] = GetCloak()))
    {
      AV[2] = Max<ushort>(Armor[2]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  return AnyArmor ? Armor[femath::WeightedRand(AV, 3)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool arm::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  long AV[3] = { 0, 0, 0 };
  item* Armor[3];
  bool AnyArmor = false;

  if((Armor[0] = GetGauntlet()))
    {
      AV[0] = Max<ushort>(Armor[0]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  if((Armor[1] = GetHumanoidMaster()->GetBodyArmor()))
    {
      AV[1] = Max<ushort>(Armor[1]->GetStrengthValue() >> 1, 1);
      AnyArmor = true;
    }

  if((Armor[2] = GetHumanoidMaster()->GetCloak()))
    {
      AV[2] = Max<ushort>(Armor[2]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  return AnyArmor ? Armor[femath::WeightedRand(AV, 3)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool groin::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  return GetMaster()->GetTorso()->DamageArmor(Damager, Damage, Type);
}

bool leg::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  long AV[3] = { 0, 0, 0 };
  item* Armor[3];
  bool AnyArmor = false;

  if((Armor[0] = GetBoot()))
    {
      AV[0] = Max<ushort>(Armor[0]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  if((Armor[1] = GetHumanoidMaster()->GetBodyArmor()))
    {
      AV[1] = Max<ushort>(Armor[1]->GetStrengthValue() >> 1, 1);
      AnyArmor = true;
    }

  if((Armor[2] = GetHumanoidMaster()->GetCloak()))
    {
      AV[2] = Max<ushort>(Armor[2]->GetStrengthValue(), 1);
      AnyArmor = true;
    }

  return AnyArmor ? Armor[femath::WeightedRand(AV, 3)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool bodypart::CanBeEatenByAI(const character* Who) const
{
  return item::CanBeEatenByAI(Who) && !(Who->GetTeam()->GetID() == PLAYER->GetTeam()->GetID() && PLAYER->HasHadBodyPart(this));
}

ushort bodypart::GetConditionColor() const
{
  if(HP <= 1 && MaxHP > 1)
    return MakeRGB16(32, 32, 32);
  else if(HP * 3 < MaxHP)
    return MakeRGB16(120, 0, 0);
  else if(HP * 3 < MaxHP << 1)
    return MakeRGB16(180, 0, 0);
  else if(HP < MaxHP)
    return MakeRGB16(180, 120, 120);
  else
    return LIGHT_GRAY;
}

bool arm::CheckIfWeaponTooHeavy(const std::string& WeaponDescription) const
{
  ushort HitStrength = GetAttribute(ARM_STRENGTH);
  ushort Requirement = GetWielded()->GetStrengthRequirement();

  if(TwoHandWieldIsActive())
    {
      HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
      Requirement >>= 2;

      if(HitStrength - Requirement < 10)
	{
	  if(HitStrength <= Requirement)
	    ADD_MESSAGE("You cannot use %s. Wielding it with two hands requires %d strength.", WeaponDescription.c_str(), (Requirement >> 1) + 1);
	  else if(HitStrength - Requirement < 4)
	    ADD_MESSAGE("Using %s even with two hands is extremely difficult.", WeaponDescription.c_str());
	  else if(HitStrength - Requirement < 7)
	    ADD_MESSAGE("You have much trouble using %s even with two hands.", WeaponDescription.c_str());
	  else
	    ADD_MESSAGE("It is somewhat difficult to use %s even with two hands.", WeaponDescription.c_str());

	  return !game::BoolQuestion("Continue anyway? [y/N]");
	}
    }
  else
    {
      if(HitStrength - Requirement < 10)
	{
	  std::string HandInfo;

	  if(GetWielded()->IsTwoHanded())
	    HandInfo = " with one hand";

	  if(HitStrength <= Requirement)
	    ADD_MESSAGE("You cannot use %s. Wielding it%s requires %d strength.", WeaponDescription.c_str(), HandInfo.c_str(), Requirement + 1);
	  else if(HitStrength - Requirement < 4)
	    ADD_MESSAGE("Using %s%s is extremely difficult.", WeaponDescription.c_str(), HandInfo.c_str());
	  else if(HitStrength - Requirement < 7)
	    ADD_MESSAGE("You have much trouble using %s%s.", WeaponDescription.c_str(), HandInfo.c_str());
	  else
	    ADD_MESSAGE("It is somewhat difficult to use %s%s.", WeaponDescription.c_str(), HandInfo.c_str());

	  return !game::BoolQuestion("Continue anyway? [y/N]");
	}
    }

  return false;
}

uchar corpse::GetArticleMode() const
{
  return Deceased->LeftOversAreUnique() ? DEFINITE_ARTICLE : NORMAL_ARTICLE;
}

