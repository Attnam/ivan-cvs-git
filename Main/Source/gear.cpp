/* Compiled through itemset.cpp */

void meleeweapon::SetSecondaryMaterial(material* What, ushort SpecialFlags) { SetMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
void meleeweapon::ChangeSecondaryMaterial(material* What, ushort SpecialFlags) { ChangeMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
void meleeweapon::InitMaterials(material* M1, material* M2, bool CUP) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), SecondaryMaterial, M2, GetDefaultSecondaryVolume(), CUP); }
ushort meleeweapon::GetEffectBonus() const { return 100 + 5 * Enchantment; }
ushort meleeweapon::GetAPBonus() const { return 2000 / (20 + Enchantment); }
ushort meleeweapon::GetBonus() const { return 100 + 5 * Enchantment; }
ushort meleeweapon::GetDripColor() const { return (*Fluid[0].begin())->GetLiquid()->GetColor(); }
bool meleeweapon::IsDippable(const character*) const { return !HasFluids; }

ushort justifier::GetOutlineColor(ushort) const { return MakeRGB16(0, 255, 0); }

ushort neercseulb::GetOutlineColor(ushort) const { return MakeRGB16(255, 0, 0); }

ushort flamingsword::GetSpecialFlags() const { return meleeweapon::GetSpecialFlags()|ST_FLAME; }

ushort gorovitshammer::GetOutlineColor(ushort) const { return MakeRGB16(255, 0, 0); }

ushort gorovitssickle::GetOutlineColor(ushort) const { return MakeRGB16(255, 0, 0); }

ushort thunderhammer::GetSpecialFlags() const { return !IsBroken() ? meleeweapon::GetSpecialFlags()|ST_LIGHTNING : meleeweapon::GetSpecialFlags(); }

short armor::GetCarryingBonus() const { return Enchantment << 1; }

ulong bodyarmor::GetPrice() const { return (armor::GetPrice() << 3) + GetEnchantedPrice(Enchantment); }
bool bodyarmor::IsInCorrectSlot(ushort Index) const { return Index == BODY_ARMOR_INDEX; }
const festring& bodyarmor::GetNameSingular() const { return GetMainMaterial()->GetFlexibility() >= 5 ? item::GetFlexibleNameSingular() : item::GetNameSingular(); }
const char* bodyarmor::GetBreakVerb() const { return GetMainMaterial()->GetFlexibility() >= 5 ? "is torn apart" : "breaks"; }

ushort goldeneagleshirt::GetOutlineColor(ushort) const { return MakeRGB16(0, 255, 255); }

ushort shield::GetBonus() const { return 100 + 10 * Enchantment; }

ulong cloak::GetPrice() const { return armor::GetPrice() * 10 + GetEnchantedPrice(Enchantment); }
bool cloak::IsInCorrectSlot(ushort Index) const { return Index == CLOAK_INDEX; }
ushort cloak::GetMaterialColorB(ushort) const { return MakeRGB16(111, 64, 37); }
const char* cloak::GetBreakVerb() const { return GetMainMaterial()->GetFlexibility() >= 5 ? "is torn apart" : "breaks"; }
bool cloak::ReceiveDamage(character* Damager, ushort Damage, ushort Type, uchar Dir) { return armor::ReceiveDamage(Damager, Damage >> 1, Type, Dir); }
ushort cloak::GetSpecialFlags() const { return ST_CLOAK; }

ulong boot::GetPrice() const { return armor::GetPrice() / 5 + GetEnchantedPrice(Enchantment); }
bool boot::IsInCorrectSlot(ushort Index) const { return Index == RIGHT_BOOT_INDEX || Index == LEFT_BOOT_INDEX; }

ulong gauntlet::GetPrice() const { return armor::GetPrice() / 3 + GetEnchantedPrice(Enchantment); }
bool gauntlet::IsInCorrectSlot(ushort Index) const { return Index == RIGHT_GAUNTLET_INDEX || Index == LEFT_GAUNTLET_INDEX; }

ulong belt::GetPrice() const { return armor::GetPrice() * 5 + GetEnchantedPrice(Enchantment); }
bool belt::IsInCorrectSlot(ushort Index) const { return Index == BELT_INDEX; }

bool ring::IsInCorrectSlot(ushort Index) const { return Index == RIGHT_RING_INDEX || Index == LEFT_RING_INDEX; }
ushort ring::GetMaterialColorB(ushort) const { return MakeRGB16(200, 200, 200); }

bool amulet::IsInCorrectSlot(ushort Index) const { return Index == AMULET_INDEX; }
ushort amulet::GetMaterialColorB(ushort) const { return MakeRGB16(111, 64, 37); }

bool helmet::IsGorovitsFamilyRelic() const { return GetConfig() == GOROVITS_FAMILY_GAS_MASK; }
ulong helmet::GetPrice() const { return armor::GetPrice() + GetEnchantedPrice(Enchantment); }
bool helmet::IsInCorrectSlot(ushort Index) const { return Index == HELMET_INDEX; }
ushort helmet::GetMaterialColorB(ushort) const { return GetConfig() != GOROVITS_FAMILY_GAS_MASK ? MakeRGB16(140, 70, 70) : MakeRGB16(0, 40, 0); }
ushort helmet::GetMaterialColorC(ushort) const { return MakeRGB16(180, 200, 180); }

ushort wondersmellstaff::GetClassAnimationFrames() const { return !IsBroken() ? 128 : 1; }

bool meleeweapon::HitEffect(character* Enemy, character*, vector2d, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour && HasFluids)
    {
      bool Success = false;
      fluidvector FluidVector;
      FillFluidVector(FluidVector);

      for(ushort c = 0; c < FluidVector.size(); ++c)
	if(FluidVector[c]->Exists())
	  {
	    if(FluidVector[c]->GetLiquid()->HitEffect(Enemy))
	      Success = true;

	    if(!FluidVector[c]->GetLiquid()->GetVolume())
	      RemoveFluid(FluidVector[c]);
	  }

      return Success;
    }
  else
    return false;
}

void meleeweapon::DipInto(liquid* Liquid, character* Dipper)
{
  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now covered with %s.", CHAR_NAME(DEFINITE), Liquid->GetName(false, false).CStr());

  SpillFluid(Dipper, Liquid);
  Dipper->DexterityAction(10);
}

bool pickaxe::Apply(character* User)
{
  if(IsBroken())
    {
      ADD_MESSAGE("%s is totally broken.",CHAR_NAME(DEFINITE));
      return false;
    }

  uchar Dir = game::DirectionQuestion(CONST_S("What direction do you want to dig? [press a direction key]"), false);

  vector2d Temp = game::GetMoveVector(Dir);
  vector2d ToBeDug = User->GetPos() + Temp;
  if(Dir == DIR_ERROR || !GetArea()->IsValidPos(ToBeDug))
    return false;

  lsquare* Square = GetNearLSquare(ToBeDug);
  olterrain* Terrain = Square->GetOLTerrain();

  if(!Terrain)
    {
        ADD_MESSAGE("Nothing to dig there!");
	return false;
    }

  if(Square->CanBeDug())
    {
      if(Terrain->CanBeDestroyed())
	if(Terrain->GetMainMaterial()->CanBeDug(GetMainMaterial()))
	  {
	    uchar RoomNumber = Square->GetRoomIndex();

	    if(!RoomNumber || Square->GetLevel()->GetRoom(RoomNumber)->CheckDestroyTerrain(User))
	      {
		User->SwitchToDig(this, ToBeDug);
		User->DexterityAction(5);
		return true;
	      }
	    else
	      return false;
	  }
	else
	  ADD_MESSAGE("%s is too hard to dig with %s.", Square->GetOLTerrain()->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
      else
	ADD_MESSAGE(Terrain->GetDigMessage().CStr());
    }

  return false;
}

ulong meleeweapon::GetPrice() const
{
  float WeaponStrengthModifier = GetFormModifier() * GetMainMaterial()->GetStrengthValue() * (20 + Enchantment) / 20;
  WeaponStrengthModifier *= WeaponStrengthModifier;
  WeaponStrengthModifier *= GetMainMaterial()->GetWeight();
  return ulong(WeaponStrengthModifier / (20000000.0f * sqrt(GetWeight()))) + GetEnchantedPrice(Enchantment);
}

ushort whip::GetFormModifier() const
{
  return item::GetFormModifier() * GetMainMaterial()->GetFlexibility();
}

bool pickaxe::IsAppliable(const character* Who) const
{
  return Who->CanWield();
}

void meleeweapon::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Enchantment;
  SaveFile << SecondaryMaterial;
}

void meleeweapon::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Enchantment;
  LoadMaterial(SaveFile, SecondaryMaterial);
}

material* meleeweapon::GetMaterial(ushort Index) const
{
  return !Index ? MainMaterial : SecondaryMaterial;
}

ushort meleeweapon::GetMaterialColorB(ushort) const
{
  return SecondaryMaterial->GetColor();
}

uchar meleeweapon::GetAlphaB(ushort) const
{
  return SecondaryMaterial->GetAlpha();
}

bool flamingsword::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(Enemy->IsEnabled() && RAND() & 1)
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s sword burns %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

      return Enemy->ReceiveBodyPartDamage(Hitter, 3 + (RAND() & 3), FIRE, BodyPartIndex, Direction) != 0 || BaseSuccess;
    }
  else
    return BaseSuccess;
}

bool mjolak::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 3))
    {
      if(Hitter->IsPlayer())
	game::DoEvilDeed(10);

      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("A burst of %s Mjolak's unholy energy fries %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

      return Enemy->ReceiveBodyPartDamage(Hitter, 5 + (RAND() % 6), ENERGY, BodyPartIndex, Direction) != 0 || BaseSuccess;
    }
  else
    return BaseSuccess;
}

bool vermis::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("%s Vermis sends %s on a sudden journey.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

      Enemy->TeleportRandomly();
      return true;
    }
  else
    return BaseSuccess;
}

bool turox::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s smash%s %s with the full force of Turox.", Hitter->CHAR_PERSONAL_PRONOUN, Hitter->IsPlayer() ? "" : "es", Enemy->CHAR_DESCRIPTION(DEFINITE));

      festring DeathMSG;

      if(Hitter->IsPlayer())
	DeathMSG = CONST_S("burned by ") + Hitter->GetPossessivePronoun(false) + CONST_S(" own Turox's explosion");
      else
        DeathMSG = CONST_S("killed by ") + Hitter->GetKillName();

      Enemy->GetLevel()->Explosion(Hitter, DeathMSG, HitPos, 10 + RAND() % 100);
      return true;
    }
  else
    return BaseSuccess;
}

bool whipofthievery::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(Enemy->IsEnabled() && CleptiaHelps(Enemy, Hitter))
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s whip asks for the help of Cleptia as it steals %s %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_POSSESSIVE_PRONOUN, Enemy->GetMainWielded()->CHAR_NAME(UNARTICLED));

      if(Hitter->IsPlayer())
	{
	  game::DoEvilDeed(10);
	  game::GetGod(10)->AdjustRelation(10);
	}

      Enemy->GetMainWielded()->MoveTo(Hitter->GetStackUnder());
      return true;
    }
  else
    return BaseSuccess;
}

meleeweapon::~meleeweapon()
{
  delete SecondaryMaterial;
}

meleeweapon::meleeweapon(const meleeweapon& MW) : item(MW), Enchantment(MW.Enchantment)
{
  CopyMaterial(MW.SecondaryMaterial, SecondaryMaterial);
}

extern character::prototype petrus_ProtoType;

bool whipofthievery::CleptiaHelps(const character* Enemy, const character* Hitter) const
{
  /* TERRIBLE gum solution! */

  if(Enemy->GetType() == petrus_ProtoType.GetIndex())
    return false;

  if(!Enemy->GetMainWielded() || GetMainMaterial()->GetFlexibility() <= 5)
    return false;

  if(Hitter->IsPlayer())
    {
      if(game::GetGod(10)->GetRelation() < 0)
	return false;
      else
	return !(RAND() % (10 - game::GetGod(10)->GetRelation() / 200));
    }
  else
    return !(RAND() % 10);
}

void meleeweapon::AddInventoryEntry(const character* Viewer, festring& Entry, ushort, bool ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
    {
      Entry << " [" << GetWeight() << "g, DAM " << GetBaseMinDamage() << '-' << GetBaseMaxDamage() << ", " << GetBaseToHitValueDescription();

      if(!IsBroken() && !IsWhip())
	Entry << ", " << GetStrengthValueDescription();

      uchar CWeaponSkillLevel = Viewer->GetCWeaponSkillLevel(this);
      uchar SWeaponSkillLevel = Viewer->GetSWeaponSkillLevel(this);

      if(CWeaponSkillLevel || SWeaponSkillLevel)
	Entry << ", skill " << CWeaponSkillLevel << '/' << SWeaponSkillLevel;

      Entry << ']';
    }
}

void meleeweapon::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  item::SignalSpoil(Material); // this should spill potential poison liquid to the ground!
}

void meleeweapon::AddPostFix(festring& String) const
{
  item::AddPostFix(String);

  if(HasFluids)
    {
      String << " covered with ";
      fluid::AddFluidInfo(Fluid[0], String);
    }

  if(Enchantment > 0)
    String << " +" << short(Enchantment);
  else if(Enchantment < 0)
    String << ' ' << short(Enchantment);
}

bool meleeweapon::CanBePiledWith(const item* Item, const character* Viewer) const
{
  if(!item::CanBePiledWith(Item, Viewer))
    return false;

  const meleeweapon* Weapon = static_cast<const meleeweapon*>(Item);

  return Enchantment == Weapon->Enchantment
      && SecondaryMaterial->IsSameAs(Weapon->SecondaryMaterial)
      && SecondaryMaterial->GetSpoilLevel() == Weapon->SecondaryMaterial->GetSpoilLevel();
}

void meleeweapon::Be()
{
  MainMaterial->Be();

  if(Exists())
    SecondaryMaterial->Be();
}

ulong whipofthievery::GetPrice() const
{
  /* If intact but not flexible enough to work, special thievery bonus must be removed */

  return GetMainMaterial()->GetFlexibility() > 5 || IsBroken() ? whip::GetPrice() : whip::GetPrice() - item::GetPrice();
}

bool meleeweapon::IsSparkling(ushort ColorIndex) const
{
  return (ColorIndex == 0 && MainMaterial->IsSparkling()) || (ColorIndex == 1 && SecondaryMaterial->IsSparkling());
}

void meleeweapon::SetEnchantment(char Amount)
{
  Enchantment = Amount;
  SignalEnchantmentChange();
}

void meleeweapon::EditEnchantment(char Amount)
{
  Enchantment += Amount;
  SignalEnchantmentChange();
}

float meleeweapon::GetWeaponStrength() const
{
  return item::GetWeaponStrength() * (20 + Enchantment) / 20;
}

ushort meleeweapon::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() * (20 + Enchantment) / 40000;
}

void meleeweapon::VirtualConstructor(bool Load)
{
  if(!Load)
    Enchantment = GetDefaultEnchantment();
}

uchar meleeweapon::GetSpoilLevel() const
{
  return Max<uchar>(MainMaterial->GetSpoilLevel(), SecondaryMaterial->GetSpoilLevel());
}

bool neercseulb::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
    {
      if(Hitter->IsPlayer())
	game::DoEvilDeed(10);

      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s Neerc Se-ulb's life-draining energies swallow %s!", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

      return Enemy->ReceiveBodyPartDamage(Hitter, 10 + (RAND() % 11), DRAIN, BodyPartIndex, Direction) != 0 || BaseSuccess;
    }
  else
    return BaseSuccess;
}

bool thunderhammer::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s hammer shoots a lightning bolt at %s!", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

      festring DeathMSG;

      if(Hitter->IsPlayer())
	DeathMSG = CONST_S("electrocuted ") + Hitter->GetObjectPronoun(false) + CONST_S("self with a thunder hammer");
      else
        DeathMSG = CONST_S("killed by ") + Hitter->GetKillName();

      GetLevel()->LightningBeam(Hitter, DeathMSG, Hitter->GetPos(), WHITE, BEAM_LIGHTNING, Direction, 4);
      return true;
    }
  else
     return BaseSuccess;
}

bool thunderhammer::ReceiveDamage(character* Damager, ushort Damage, ushort Type, uchar Dir)
{
  return Type & ELECTRICITY ? false : meleeweapon::ReceiveDamage(Damager, Damage, Type, Dir);
}

ulong armor::GetPrice() const
{
  float StrengthValue = GetStrengthValue();
  return ulong(StrengthValue * StrengthValue * StrengthValue * 20 / sqrt(GetWeight()));
}

ushort belt::GetFormModifier() const
{
  return item::GetFormModifier() * GetMainMaterial()->GetFlexibility();
}

void armor::AddInventoryEntry(const character*, festring& Entry, ushort Amount, bool ShowSpecialInfo) const
{
  if(Amount == 1)
    AddName(Entry, INDEFINITE);
  else
    {
      Entry << Amount << ' ';
      AddName(Entry, PLURAL);
    }

  if(ShowSpecialInfo)
    Entry << " [" << GetWeight() * Amount << "g, AV " << GetStrengthValue() << ']';
}

void shield::AddInventoryEntry(const character* Viewer, festring& Entry, ushort, bool ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
    {
      Entry << " [" << GetWeight() << "g, "  << GetBaseBlockValueDescription();

      if(!IsBroken())
	Entry << ", " << GetStrengthValueDescription();

      uchar CWeaponSkillLevel = Viewer->GetCWeaponSkillLevel(this);
      uchar SWeaponSkillLevel = Viewer->GetSWeaponSkillLevel(this);

      if(CWeaponSkillLevel || SWeaponSkillLevel)
	Entry << ", skill " << CWeaponSkillLevel << '/' << SWeaponSkillLevel;

      Entry << ']';
    }
}

bool armor::CanBePiledWith(const item* Item, const character* Viewer) const
{
  return item::CanBePiledWith(Item, Viewer) && Enchantment == static_cast<const armor*>(Item)->Enchantment;
}

ulong shield::GetPrice() const /* temporary... */
{
  float StrengthValue = GetStrengthValue();
  return ulong(sqrt(GetBaseBlockValue()) * StrengthValue * StrengthValue) + item::GetPrice();
}

void armor::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Enchantment;
}

void armor::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Enchantment;
}

void armor::AddPostFix(festring& String) const
{
  item::AddPostFix(String);

  if(HasFluids)
    {
      String << " covered with ";
      fluid::AddFluidInfo(Fluid[0], String);
    }

  if(Enchantment > 0)
    String << " +" << short(Enchantment);
  else if(Enchantment < 0)
    String << ' ' << short(Enchantment);
}

void armor::SetEnchantment(char Amount)
{
  Enchantment = Amount;
  SignalEnchantmentChange();
}

void armor::EditEnchantment(char Amount)
{
  Enchantment += Amount;
  SignalEnchantmentChange();
}

ushort armor::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() * (10 + Enchantment) / 20000;
}

void armor::VirtualConstructor(bool Load)
{
  if(!Load)
    Enchantment = GetDefaultEnchantment();
}

ushort armor::GetInElasticityPenalty(ushort Attribute) const
{
  return Attribute * GetInElasticityPenaltyModifier() / (GetMainMaterial()->GetFlexibility() * 100);
}

void meleeweapon::GenerateMaterials()
{
  ushort Chosen = RandomizeMaterialConfiguration();
  InitChosenMaterial(MainMaterial, GetMainMaterialConfig(), GetDefaultMainVolume(), Chosen);
  InitChosenMaterial(SecondaryMaterial, GetSecondaryMaterialConfig(), GetDefaultSecondaryVolume(), Chosen);
}

bool chameleonwhip::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && ScabiesHelps(Enemy, Hitter))
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
      ADD_MESSAGE("%s whip asks for the help of Scabies as it polymorphs %s.", Hitter->CHAR_PERSONAL_PRONOUN, Hitter->CHAR_DESCRIPTION(DEFINITE));

      if(Hitter->IsPlayer())
	{
	  game::DoEvilDeed(20);
	  game::GetGod(10)->AdjustRelation(10);
	}

      ushort CurrentDanger = ushort(Enemy->GetRelativeDanger(PLAYER) * 100);
      Enemy->PolymorphRandomly(CurrentDanger / 4, Min<ushort>(CurrentDanger, 9999), 100 + RAND() % 400);
      return true;
    }
  else
    return BaseSuccess;
}

bool chameleonwhip::ScabiesHelps(const character* Enemy, const character* Hitter) const
{
  if(!Enemy->IsPolymorphable())
    return false;

  if(Hitter->IsPlayer())
    {
      if(game::GetGod(SCABIES)->GetRelation() < 0)
	return false;
      else
	return !(RAND() % (20 - game::GetGod(SCABIES)->GetRelation() / 150));
    }
  else
    return !(RAND() % 20);
} 

uchar justifier::GetOutlineAlpha(ushort Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

uchar neercseulb::GetOutlineAlpha(ushort Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

uchar gorovitshammer::GetOutlineAlpha(ushort Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

uchar gorovitssickle::GetOutlineAlpha(ushort Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

uchar goldeneagleshirt::GetOutlineAlpha(ushort Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

uchar wondersmellstaff::GetOutlineAlpha(ushort Frame) const
{
  if(!IsBroken())
    {
      Frame &= 31;
      return Frame * (31 - Frame) >> 1;
    }
  else
    return 255;
}

ushort wondersmellstaff::GetOutlineColor(ushort Frame) const
{
  if(!IsBroken())
    switch((Frame&127) >> 5)
      {
      case 0: return BLUE;
      case 1: return GREEN;
      case 2: return RED;
      case 3: return YELLOW;
      }

  return TRANSPARENT_COLOR;
}

bool wondersmellstaff::HitEffect(character* Enemy, character* Hitter, vector2d HitPos, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
    {
      if(RAND() & 3)
	{
	  bool Seen = false;
	  ushort Amount = 250 / Enemy->GetSquaresUnder();

	  for(ushort c = 0; c < Enemy->GetSquaresUnder(); ++c)
	    {
	      lsquare* Square = Enemy->GetLSquareUnder(c);

	      if(Square->IsFlyable())
		{
		  Square->AddSmoke(new gas(EVIL_WONDER_STAFF_VAPOUR, Amount));

		  if(!Seen && Square->CanBeSeenByPlayer())
		    Seen = true;
		}
	    }

	  if(Seen)
	    ADD_MESSAGE("Strange red smoke billows out of %s staff.", Hitter->CHAR_POSSESSIVE_PRONOUN);
	}
      else
	{
	  /* Can a multitiled creature ever be the hitter? */

	  lsquare* Square = Hitter->GetLSquareUnder();

	  if(Square->IsFlyable())
	    {
	      if(Square->CanBeSeenByPlayer())
		ADD_MESSAGE("Strange blue smoke billows out of %s staff.", Hitter->CHAR_POSSESSIVE_PRONOUN);

	      Square->AddSmoke(new gas(GOOD_WONDER_STAFF_VAPOUR, 100));
	    }
	}

      return true;
    }
  else
    return BaseSuccess;
}

bool bodyarmor::AddAdjective(festring& String, bool Articled) const
{
  if(IsBroken())
    {
      if(Articled)
	String << "a ";

      if(GetMainMaterial()->GetFlexibility() >= 5)
	String << "torn";
      else
	String << "broken";

      String << ' ';
      return true;
    }
  else
    return false;
}

bool cloak::AddAdjective(festring& String, bool Articled) const
{
  if(IsBroken())
    {
      if(Articled)
	String << "a ";

      if(GetMainMaterial()->GetFlexibility() >= 5)
	String << "torn";
      else
	String << "broken";

      String << ' ';
      return true;
    }
  else
    return false;
}

uchar meleeweapon::GetRustDataB() const
{
  return SecondaryMaterial->GetRustData();
}

void meleeweapon::TryToRust(ulong LiquidModifier)
{
  item::TryToRust(LiquidModifier);

  if(SecondaryMaterial->TryToRust(LiquidModifier))
    SecondaryMaterial->SetRustLevel(SecondaryMaterial->GetRustLevel() + 1);
}
