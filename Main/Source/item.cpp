/* Compiled through itemset.cpp */

const std::string ToHitValueDescription[] = { "unbelievably inaccurate", "extremely inaccurate", "inaccurate", "decently accurate", "accurate", "highly accurate", "extremely accurate", "unbelievably accurate" };
const std::string StrengthValueDescription[] = { "fragile", "rather sturdy", "sturdy", "durable", "very durable", "extremely durable", "almost unbreakable" };

itemprototype::itemprototype(itemprototype* Base, item* (*Cloner)(ushort, ushort), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId) { Index = protocontainer<item>::Add(this); }

item::item(const item& Item) : object(Item), Slot(0), Cannibalised(false), Size(Item.Size), ID(game::CreateNewItemID()), BackupID(game::CreateNewItemID()), DataBase(Item.DataBase), Volume(Item.Volume), Weight(Item.Weight) { }
item::item(donothing) : Slot(0), Cannibalised(false) { }
void item::InstallDataBase() { ::database<item>::InstallDataBase(this); }
bool item::IsOnGround() const { return GetSlot()->IsOnGround(); }
bool item::IsSimiliarTo(item* Item) const { return Item->GetType() == GetType() && Item->GetConfig() == GetConfig(); }
ushort item::GetBaseMinDamage() const { return ushort(sqrt(GetWeaponStrength() / 20000.0f) * 0.75f); }
ushort item::GetBaseMaxDamage() const { return ushort(sqrt(GetWeaponStrength() / 20000.0f) * 1.25f) + 1; }
ushort item::GetBaseToHitValue() const { return 100 * GetBonus() / (1000 + GetWeight()); }
ushort item::GetBaseBlockValue() const { return ushort(GetBlockModifier() * GetBonus() / (100000 + 100 * GetWeight())); }
bool item::IsInCorrectSlot(ushort Index) const { return Index == RIGHT_WIELDED_INDEX || Index == LEFT_WIELDED_INDEX; }
bool item::IsInCorrectSlot() const { return IsInCorrectSlot(static_cast<gearslot*>(Slot)->GetEquipmentIndex()); }
ushort item::GetEquipmentIndex() const { return static_cast<gearslot*>(Slot)->GetEquipmentIndex(); }
uchar item::GetGraphicsContainerIndex() const { return GR_ITEM; }

bool item::IsConsumable(const character* Eater) const
{
  if(!GetConsumeMaterial())
    return false;
  else
    return Eater->CanConsume(GetConsumeMaterial());
}

bool item::IsEatable(const character* Eater) const
{
  return IsConsumable(Eater) && !GetConsumeMaterial()->IsLiquid();
}

bool item::IsDrinkable(const character* Eater) const
{
  return IsConsumable(Eater) && GetConsumeMaterial()->IsLiquid();
}

void item::Fly(character* Thrower, uchar Direction, ushort Force)
{
  MoveTo(GetLSquareUnder()->GetStack());
  ushort Range = Force * 25UL / Max<ulong>(ulong(sqrt(GetWeight())), 1);

  if(!Range)
    return;

  if(Direction == RANDOM_DIR)
    Direction = RAND() & 7;

  vector2d StartingPos = GetPos();
  vector2d Pos = StartingPos;
  vector2d DirVector = game::GetMoveVector(Direction);
  bool Breaks = false;
  float BaseDamage, BaseToHitValue;

  if(Thrower)
    {
      ushort Bonus = Thrower->IsHumanoid() ? Thrower->GetCWeaponSkill(GetWeaponCategory())->GetBonus() : 100;
      BaseDamage = sqrt(5e-10f * GetWeaponStrength() * Force / Range) * Bonus;
      BaseToHitValue = GetBonus() * Bonus * Thrower->GetMoveEase() / (500 + GetWeight()) * Thrower->GetAttribute(DEXTERITY) * sqrt(2.5e-8f * Thrower->GetAttribute(PERCEPTION)) / Range;
    }
  else
    {
      BaseDamage = sqrt(5e-6f * GetWeaponStrength() * Force / Range);
      BaseToHitValue = 10 * GetBonus() / (500 + GetWeight()) / Range;
    }

  ushort RangeLeft;

  for(RangeLeft = Range; RangeLeft != 0; --RangeLeft)
    {
      if(!GetLevel()->IsValidPos(Pos + DirVector))
	break;

      lsquare* JustHit = GetNearLSquare(Pos + DirVector);

      if(!JustHit->GetOLTerrain()->IsWalkable())
	{
	  Breaks = true;
	  JustHit->GetOLTerrain()->HasBeenHitByItem(Thrower, this, ushort(BaseDamage * sqrt(RangeLeft)));
	  break;
	}
      else
	{
	  clock_t StartTime = clock();
	  Pos += DirVector;
	  MoveTo(JustHit->GetStack());
	  game::DrawEverything();

	  if(JustHit->GetCharacter())
	    {
	      ushort Damage = ushort(BaseDamage * sqrt(RangeLeft));
	      float ToHitValue = BaseToHitValue * RangeLeft;
	      uchar Returned = HitCharacter(Thrower, JustHit->GetCharacter(), Damage, ToHitValue, Direction);

	      if(Returned == HIT)
		Breaks = true;

	      if(Returned != MISSED)
		break;
	    }

	  while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
	}
    }

  if(Breaks)
    ReceiveDamage(Thrower, ushort(sqrt(GetWeight() * RangeLeft) / 10), PHYSICAL_DAMAGE);
}

uchar item::HitCharacter(character* Thrower, character* Dude, ushort Damage, float ToHitValue, uchar Direction)
{
  if(Dude->Catches(this))
    return CATCHED;

  if(Thrower)
    Thrower->Hostility(Dude);

  if(Dude->DodgesFlyingItem(this, ToHitValue)) 
    {
      if(Dude->IsPlayer())
	ADD_MESSAGE("%s misses you.", CHAR_NAME(DEFINITE));
      else if(Dude->CanBeSeenByPlayer())
	ADD_MESSAGE("%s misses %s.", CHAR_NAME(DEFINITE), Dude->CHAR_NAME(DEFINITE));

      return MISSED;
    }

  Dude->HasBeenHitByItem(Thrower, this, Damage, ToHitValue, Direction);
  return HIT;
}

float item::GetWeaponStrength() const
{
  return GetFormModifier() * GetMainMaterial()->GetStrengthValue() * sqrt(GetMainMaterial()->GetWeight());
}

ushort item::GetStrengthRequirement() const
{
  float WeightTimesSize = GetWeight() * GetSize();
  return ushort(1.25e-10f * WeightTimesSize * WeightTimesSize);
}

bool item::Apply(character* Applier)
{
  if(Applier->IsPlayer())
    ADD_MESSAGE("You can't apply this!");

  return false;
}

/* returns bool that tells whether the Polymorph really happened */

bool item::Polymorph(stack* CurrentStack)
{
  if(!IsPolymorphable())
    return false;
  else
    {
      CurrentStack->AddItem(protosystem::BalancedCreateItem(0, MAX_PRICE, 0, true));
      RemoveFromSlot();
      SendToHell();
      return true;
    }
}

bool item::Consume(character* Eater, long Amount)
{
  if(!GetConsumeMaterial()) // if it's spoiled or something
    return true;

  GetConsumeMaterial()->EatEffect(Eater, Amount, GetNPModifier());

  if(!Cannibalised && Eater->IsPlayer() && Eater->CheckCannibalism(GetConsumeMaterial()))
    {
      game::DoEvilDeed(25);
      ADD_MESSAGE("You feel that this was an evil deed.");
      Cannibalised = true;
    }

  return GetConsumeMaterial()->GetVolume() ? false : true;
}

bool item::CanBeEatenByAI(const character* Eater) const
{
  return !Eater->CheckCannibalism(GetConsumeMaterial()) && GetConsumeMaterial()->CanBeEatenByAI(Eater);
}

void item::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  object::Save(SaveFile);
  SaveFile << Cannibalised << Size << ID << BackupID;
}

void item::Load(inputfile& SaveFile)
{
  object::Load(SaveFile);
  InstallDataBase();
  SaveFile >> Cannibalised >> Size >> ID >> BackupID;
}

void item::TeleportRandomly()
{
  MoveTo(GetNearLSquare(GetLevel()->GetRandomSquare())->GetStack());
}

ushort item::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 2000;
}

void item::RemoveFromSlot()
{
  if(Slot)
    {
      Slot->Empty();
      Slot = 0;
    }
}

void item::MoveTo(stack* Stack)
{
  if(Slot)
    {
      RemoveFromSlot();
      Stack->AddItem(this);
    }
  else
    Stack->AddItem(this);
}

std::string item::ItemCategoryName(ulong Category)
{
  switch(Category)
    {
    case HELMET: return "Helmets";
    case AMULET: return "Amulets";
    case CLOAK: return "Cloaks";
    case BODY_ARMOR: return "Body armors";
    case WEAPON: return "Weapons";
    case SHIELD: return "Shields";
    case RING: return "Rings";
    case GAUNTLET: return "Gauntlets";
    case BELT: return "Belts";
    case BOOT: return "Boots";
    case FOOD: return "Food";
    case POTION: return "Potions";
    case SCROLL: return "Scrolls";
    case BOOK: return "Books";
    case WAND: return "Wands";
    case TOOL: return "Tools";
    case VALUABLE: return "Valuables";
    case MISC: return "Miscellaneous items";
    default: return "Warezzz";
    }
}

ushort item::GetResistance(uchar Type) const
{
  switch(Type)
    {
    case PHYSICAL_DAMAGE: return GetStrengthValue();
    case SOUND:
    case ENERGY:
    case ACID:
    case DRAIN:
      return 0;
    case FIRE: return GetFireResistance();
    case POISON: return GetPoisonResistance();
    case ELECTRICITY: return GetElectricityResistance();
    default:
      ABORT("Resistance lack detected!");
      return 0;
    }
}

void item::AddConsumeEndMessage(character* Eater) const
{
  GetConsumeMaterial()->AddConsumeEndMessage(Eater);
}

void item::GenerateLeftOvers(character*)
{
  RemoveFromSlot();
  SendToHell();
}

bool item::Open(character* Char)
{
  if(Char->IsPlayer())
    ADD_MESSAGE("You can't open %s.", CHAR_NAME(DEFINITE));

  return false;
}

item* itemprototype::CloneAndLoad(inputfile& SaveFile) const
{
  item* Item = Cloner(0, LOAD);
  Item->Load(SaveFile);
  Item->CalculateAll();
  return Item;
}

void item::LoadDataBaseStats()
{
  SetSize(GetDefaultSize());
}

void item::Initialize(ushort NewConfig, ushort SpecialFlags)
{
  if(!(SpecialFlags & LOAD))
    {
      ID = game::CreateNewItemID();
      BackupID = game::CreateNewItemID();
      Config = NewConfig;
      InstallDataBase();
      LoadDataBaseStats();
      RandomizeVisualEffects();

      if(!(SpecialFlags & NO_MATERIALS))
	GenerateMaterials();
    }

  VirtualConstructor(SpecialFlags & LOAD);

  if(!(SpecialFlags & LOAD))
    {
      if(!(SpecialFlags & NO_MATERIALS))
	{
	  CalculateAll();

	  if(!(SpecialFlags & NO_PIC_UPDATE))
	    UpdatePictures();
	}
    }
}

bool item::ShowMaterial() const
{
  if(GetMainMaterialConfig().size() == 1)
    return GetMainMaterial()->GetConfig() != GetMainMaterialConfig()[0];
  else
    return true;
}

std::string item::GetConsumeVerb() const
{
  return GetConsumeMaterial()->GetConsumeVerb();
}

ulong item::GetBlockModifier() const
{
  if(!IsShield(0))
    return GetSize() * GetRoundness() << 1;
  else
    return GetSize() * GetRoundness() << 2;
}

bool item::CanBeSeenByPlayer() const
{
  return CanBeSeenBy(PLAYER);
}

bool item::CanBeSeenBy(const character* Who) const
{
  return (Who->IsPlayer() && game::SeeWholeMapCheatIsActive()) || (Slot && Slot->CanBeSeenBy(Who));
}

std::string item::GetDescription(uchar Case) const
{
  if(CanBeSeenByPlayer())
    return GetName(Case);
  else
    return "something";
}

void item::SignalVolumeAndWeightChange()
{
  CalculateVolumeAndWeight();

  if(Slot)
    Slot->SignalVolumeAndWeightChange();
}

void item::CalculateVolumeAndWeight()
{
  Volume = Weight = 0;

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      {
	Volume += GetMaterial(c)->GetVolume();
	Weight += GetMaterial(c)->GetWeight();
      }
}

void item::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
    {
      game::CombineLights(Emitation, EmitationUpdate);

      if(Slot)
	Slot->SignalEmitationIncrease(EmitationUpdate);
    }
}

void item::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
    {
      ulong Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation && Slot)
	Slot->SignalEmitationDecrease(EmitationUpdate);
    }
}

void item::CalculateAll()
{
  CalculateVolumeAndWeight();
  CalculateEmitation();
}

/* Temporary and buggy. */

void item::WeaponSkillHit()
{
  if(Slot)
    static_cast<arm*>(static_cast<gearslot*>(Slot)->GetBodyPart())->WieldedSkillHit();
}

/* returns 0 if item cannot be cloned */

item* item::Duplicate() const
{
  if(!CanBeCloned())
    return 0;

  item* Clone = RawDuplicate();
  Clone->UpdatePictures();
  return Clone;
}

void item::AddInventoryEntry(const character*, std::string& Entry, ushort Amount, bool ShowSpecialInfo) const
{
  if(Amount == 1)
    AddName(Entry, INDEFINITE);
  else
    {
      Entry << Amount << " ";
      AddName(Entry, PLURAL);
    }

  if(ShowSpecialInfo)
    Entry << " [" << GetWeight() * Amount << "g]";
}

const itemdatabase& itemprototype::ChooseBaseForConfig(ushort ConfigNumber)
{
  if(!(ConfigNumber & BROKEN))
    return Config.begin()->second;
  else
    {
      item::databasemap::iterator i = Config.find(ConfigNumber&~BROKEN);

      if(i != Config.end())
	return i->second;
      else
	return Config.begin()->second;
    }
}

bool item::ReceiveDamage(character*, ushort Damage, uchar Type)
{
  if(CanBeBroken() && !IsBroken() && Type & (PHYSICAL_DAMAGE|SOUND|ENERGY))
    {
      ushort StrengthValue = GetStrengthValue();

      if(!StrengthValue)
	StrengthValue = 1;

      if(Damage > StrengthValue << 2 && RAND() & 3 && RAND() % (25 * Damage / StrengthValue) >= 100)
	{
	  Break();
	  return true;
	}
    }

  return false;
}

void itemdatabase::InitDefaults(ushort Config)
{
  IsAbstract = false;

  if(Config & BROKEN)
    {
      if(Adjective.length())
	Adjective.insert(0, "broken ");
      else
	Adjective = "broken";

      FormModifier >>= 2;
      StrengthModifier >>= 1;
    }
}

void item::AddAttackInfo(felist& List) const
{
  std::string Entry(40, ' ');
  Entry << int(GetWeight());
  Entry.resize(50, ' ');
  Entry << int(GetSize());
  Entry.resize(60, ' ');
  Entry << int(GetStrengthRequirement());
  Entry.resize(70, ' ');
  Entry << int(GetBaseMinDamage()) << '-' << GetBaseMaxDamage();
  List.AddEntry(Entry, LIGHT_GRAY);
}

void item::AddMiscellaneousInfo(felist& List) const
{
  std::string Entry(40, ' ');
  Entry << int(GetTruePrice());
  Entry.resize(55, ' ');
  Entry << int(GetOfferValue(NEUTRAL));
  Entry.resize(70, ' ');
  Entry << int(GetNutritionValue());
  List.AddEntry(Entry, LIGHT_GRAY);
}

ulong item::GetNutritionValue() const
{
  return GetConsumeMaterial() ? GetConsumeMaterial()->GetTotalNutritionValue(this) : 0; 
}

void item::SignalSpoil(material*)
{
  if(!Exists())
    return;

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s spoils completely.", CHAR_NAME(DEFINITE));

  RemoveFromSlot();
  SendToHell();
}

bool item::CarriedByPlayer() const
{
  return CarriedBy(PLAYER);
}

bool item::CarriedBy(const character* Who) const
{
  return Who->SearchForItemWithID(GetID()) != 0;
}

item* item::DuplicateToStack(stack* CurrentStack)
{
  item* Duplicated = Duplicate();

  if(!Duplicated)
    return 0;

  CurrentStack->AddItem(Duplicated);
  return Duplicated;
}

bool item::CanBePiledWith(const item* Item, const character* Viewer) const
{
  return GetType() == Item->GetType()
      && Config == Item->Config
      && Weight == Item->Weight
      && MainMaterial->IsSameAs(Item->MainMaterial)
      && MainMaterial->GetSpoilLevel() == Item->MainMaterial->GetSpoilLevel()
      && Viewer->GetCWeaponSkillLevel(this) == Viewer->GetCWeaponSkillLevel(Item)
      && Viewer->GetSWeaponSkillLevel(this) == Viewer->GetSWeaponSkillLevel(Item);
}

void item::Break()
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s breaks.", CHAR_NAME(DEFINITE));

  item* Broken = RawDuplicate();
  Broken->SetConfig(GetConfig() | BROKEN);
  Broken->SetSize(Broken->GetSize() >> 1);
  Broken->SetID(BackupID);
  Broken->SetBackupID(ID);
  DonateSlotTo(Broken);
  SendToHell();

  if(PLAYER->Equips(Broken))
    game::AskForKeyPress("Equipment broken! [press any key to continue]");
}

void item::Be()
{
  MainMaterial->Be();
}

void item::Empty()
{
  ChangeContainedMaterial(0);

  if(!game::IsInWilderness())
    {
      GetLSquareUnder()->SendMemorizedUpdateRequest();
      GetLSquareUnder()->SendNewDrawRequest();
    }
}

short item::GetOfferValue(char) const
{
  /* Temporary */
  return short(sqrt(GetTruePrice()));
}

void item::SignalEnchantmentChange()
{
  if(Slot)
    Slot->SignalEnchantmentChange();
}

ulong item::GetEnchantedPrice(char Enchantment) const
{
  return !PriceIsProportionalToEnchantment() ? item::GetPrice() : Max(item::GetPrice() * Enchantment, 0UL);
}

item* item::Fix()
{
  item* Fixed = 0;

  if(IsBroken())
    {
      Fixed = RawDuplicate();
      Fixed->SetConfig(GetConfig() ^ BROKEN);
      Fixed->SetSize(Fixed->GetSize() << 1);
      Fixed->SetID(BackupID);
      Fixed->SetBackupID(ID);
      DonateSlotTo(Fixed);
      SendToHell();
    }

  return Fixed;
}

void item::DonateSlotTo(item* Item)
{
  Slot->DonateTo(Item);
  Slot = 0;
}

uchar item::GetSpoilLevel() const
{
  return MainMaterial->GetSpoilLevel();
}

void item::SignalSpoilLevelChange(material*)
{
  bool NeedToInformSlot = !IsAnimated() && GetSpoilLevel() && GetSquareUnder() && Slot->IsVisible();
  UpdatePictures();

  if(NeedToInformSlot)
    Slot->GetSquareUnder()->IncAnimatedEntities();
}

bool item::AllowSpoil() const
{
  if(IsOnGround())
    {
      lsquare* Square = GetLSquareUnder();
      uchar RoomNumber = Square->GetRoomIndex();
      return !RoomNumber || Square->GetLevel()->GetRoom(RoomNumber)->AllowSpoil(this);
    }
  else
    return true;
}

void item::ResetSpoiling()
{
  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      GetMaterial(c)->ResetSpoiling();
}

const std::string& item::GetBaseToHitValueDescription() const
{
  if(GetBaseToHitValue() < 10)
    return ToHitValueDescription[Min<ushort>(GetBaseToHitValue(), 6)];
  else
    return ToHitValueDescription[7];
}

const std::string& item::GetStrengthValueDescription() const
{
  ushort SV = GetStrengthValue();
  if(SV < 3)
    return StrengthValueDescription[0];
  if(SV < 5)
    return StrengthValueDescription[1];
  if(SV < 8)
    return StrengthValueDescription[2];
  if(SV < 11)
    return StrengthValueDescription[3];
  if(SV < 16)
    return StrengthValueDescription[4];
  if(SV < 20)
    return StrengthValueDescription[5];
  else
    return StrengthValueDescription[6];
}

void item::SpecialGenerationHandler()
{
  if(HandleInPairs())
    GetSlot()->AddFriendItem(Duplicate());
}

bool item::IsBroken() const
{
  return (Config & BROKEN) != 0;
}

void item::SortAllItems(itemvector& AllItems, const character* Character, bool (*Sorter)(const item*, const character*)) const
{
  if(Sorter == 0 || Sorter(this, Character))
    AllItems.push_back(const_cast<item*>(this));
}

uchar item::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : MainMaterial->GetAttachedGod();
}

ulong item::GetMaterialPrice() const
{
  ulong Price = 0;

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      Price += GetMaterial(c)->GetRawPrice();

  return Price;
}

ulong item::GetTruePrice() const
{
  return Max(GetPrice(), GetMaterialPrice());
}

bool item::IsSparkling() const
{
  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      {
	if(GetMaterial(c)->IsSparkling())
	  return true;
      }
  return false;
}

bool item::IsStupidToConsume() const
{
  return GetConsumeMaterial()->IsStupidToConsume();
}
