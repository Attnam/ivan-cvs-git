#include <ctime>
#include <cmath>

#include "itemba.h"
#include "error.h"
#include "charba.h"
#include "stack.h"
#include "level.h"
#include "lsquare.h"
#include "lterraba.h"
#include "message.h"
#include "wskill.h"
#include "femath.h"
#include "slot.h"
#include "game.h"
#include "proto.h"
#include "whandler.h"
#include "materba.h"
#include "save.h"
#include "database.h"
#include "felist.h"
#include "festring.h"

item::item(const item& Item) : object(Item), Slot(0), Cannibalised(false), Size(Item.Size), ID(game::CreateNewItemID()), DataBase(Item.DataBase), Volume(Item.Volume), Weight(Item.Weight)
{
}

item::item(donothing) : Slot(0), Cannibalised(false), ID(game::CreateNewItemID())
{
}

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

bool item::Fly(character* Thrower, uchar Direction, ushort Force)
{
  if(Direction == RANDOMDIR)
    Direction = RAND() % 8;

  vector2d StartingPos = GetPos();
  vector2d Pos = StartingPos;
  bool Breaks = false;
  float Speed = float(Force) / GetWeight() * 1500;

  for(;;)
    {
      if(!GetLevelUnder()->IsValidPos(Pos + game::GetMoveVector(Direction)))
	break;

      lsquare* JustHit = GetNearLSquare(Pos + game::GetMoveVector(Direction));

      if(!(JustHit->GetOLTerrain()->IsWalkable()))
	{
	  Breaks = true;
	  JustHit->HasBeenHitBy(this, Speed, Direction);
	  break;
	}
      else
	{
	  clock_t StartTime = clock();

	  Pos += game::GetMoveVector(Direction);
	  Speed *= 0.7f;

	  if(Speed < 0.5)
	    break;

	  MoveTo(GetNearLSquare(Pos)->GetStack());
	  game::DrawEverything();

	  if(GetNearSquare(Pos)->GetCharacter())
	    {
	      if(HitCharacter(Thrower, GetNearSquare(Pos)->GetCharacter(), Speed))
		{
		  Breaks = true;
		  break;
		}
	    }

	  while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
	}
    }

  //MoveTo(GetNearLSquare(Pos)->GetStack());

  if(Breaks)
    ReceiveDamage(Thrower, short(Speed), PHYSICALDAMAGE);

  if(Pos == StartingPos)
    return false;
  else
    return true;
}

bool item::HitCharacter(character* Thrower, character* Dude, float Speed)
{
  if(Dude->Catches(this, Speed))
    return true;

  if(Thrower)
    Thrower->Hostility(Dude);

  if(Dude->DodgesFlyingItem(this, Speed)) 
    {
      if(Dude->IsPlayer())
	ADD_MESSAGE("%s misses you.", CHARNAME(DEFINITE));
      else if(Dude->CanBeSeenByPlayer())
	ADD_MESSAGE("%s misses %s.", CHARNAME(DEFINITE), Dude->CHARNAME(DEFINITE));

      return false;
    }

  /* Fix! */

  //if(RAND() & 1)
    //HitEffect(Dude, Thrower);

  Dude->HasBeenHitByItem(Thrower, this, Speed);
  return true;
}

float item::GetWeaponStrength() const
{
  return sqrt(float(GetFormModifier()) * GetMainMaterial()->GetStrengthValue() * GetWeight());
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
      CurrentStack->AddItem(protosystem::BalancedCreateItem());
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

bool item::IsBadFoodForAI(character* Eater) const
{
  if(Eater->CheckCannibalism(GetConsumeMaterial()))
    return true;
  else
    return GetConsumeMaterial()->IsBadFoodForAI();
}

void item::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  object::Save(SaveFile);
  SaveFile << Cannibalised << Size << ID;
}

void item::Load(inputfile& SaveFile)
{
  object::Load(SaveFile);
  InstallDataBase();
  game::CombineLights(Emitation, GetBaseEmitation()); // what does this do?
  game::PopItemID(ID);
  SaveFile >> Cannibalised >> Size >> ID;
}

void item::TeleportRandomly()
{
  MoveTo(GetNearLSquare(GetLevelUnder()->GetRandomSquare())->GetStack());
}

ushort item::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 1000;
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

std::string item::ItemCategoryName(uchar Category)
{
  switch(Category)
    {
    case HELMET: return "Helmets";
    case AMULET: return "Amulets";
    case CLOAK: return "Cloaks";
    case BODYARMOR: return "Body armors";
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
    case PHYSICALDAMAGE: return GetStrengthValue();
    case SOUND: return GetSoundResistance();
    case ENERGY: return GetEnergyResistance();
    case ACID: return GetAcidResistance();
    case FIRE: return GetFireResistance();
    case POISON: return GetPoisonResistance();
    case BULIMIA: return GetBulimiaResistance();
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
    ADD_MESSAGE("You can't open %s.", CHARNAME(DEFINITE));

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
      Config = NewConfig;
      InstallDataBase();
      LoadDataBaseStats();
      RandomizeVisualEffects();

      if(!(SpecialFlags & NOMATERIALS))
	GenerateMaterials();
    }

  VirtualConstructor(SpecialFlags & LOAD);

  if(!(SpecialFlags & LOAD))
    {
      if(!(SpecialFlags & NOMATERIALS))
	{
	  CalculateAll();

	  if(!(SpecialFlags & NOPICUPDATE))
	    UpdatePictures();
	}
    }
}

itemprototype::itemprototype(itemprototype* Base, item* (*Cloner)(ushort, ushort), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId)
{
  Index = protocontainer<item>::Add(this);
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
  return CanBeSeenBy(game::GetPlayer());
}

bool item::CanBeSeenBy(const character* Who) const
{
  return (Who->IsPlayer() && game::GetSeeWholeMapCheat()) || (Slot && Slot->CanBeSeenBy(Who));
}

std::string item::GetDescription(uchar Case) const
{
  if(CanBeSeenByPlayer())
    return GetName(Case);
  else
    return "something";
}

void item::InstallDataBase()
{
  ::database<item>::InstallDataBase(this);
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
  if(CanBeBroken() && !(IsBroken()) && Type & (PHYSICALDAMAGE|SOUND|ENERGY))
    {
      ushort StrengthValue = GetStrengthValue();

      if(!StrengthValue)
	StrengthValue = 1;

      if(Damage > StrengthValue && RAND() % (100 * Damage / StrengthValue) >= 100)
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

void item::AddMiscellaneousInfo(felist& List) const
{
  std::string Entry(40, ' ');
  Entry << int(GetPrice());
  Entry.resize(55, ' ');
  Entry << int(GetOfferValue(NEUTRAL));
  Entry.resize(70, ' ');
  Entry << int(GetNutritionValue());
  List.AddEntry(Entry, LIGHTGRAY);
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
    ADD_MESSAGE("%s spoils completely.", CHARNAME(DEFINITE));

  RemoveFromSlot();
  SendToHell();
}

bool item::CarriedByPlayer() const
{
  return CarriedBy(game::GetPlayer());
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
      && Viewer->GetCWeaponSkillLevel(this) == Viewer->GetCWeaponSkillLevel(Item)
      && Viewer->GetSWeaponSkillLevel(this) == Viewer->GetSWeaponSkillLevel(Item);
}

void item::Break()
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s breaks.", CHARNAME(DEFINITE));

  item* Broken = RawDuplicate();
  Broken->SetConfig(GetConfig() | BROKEN);
  Broken->SetSize(Broken->GetSize() >> 1);
  GetSlot()->DonateTo(Broken);
  SendToHell();
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
  return short(sqrt(GetPrice()));
}

void item::SignalAttackInfoChange()
{
  if(Slot)
    Slot->SignalAttackInfoChange();
}

