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

item::item(donothing) : Slot(0), Cannibalised(false), ID(game::CreateNewItemID()), CarriedWeight(0)
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

short item::CalculateOfferValue(char GodAlignment) const
{
  long OfferValue = 0;

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      OfferValue += GetMaterial(c)->CalculateOfferValue(GodAlignment);

  return short(OfferValue * (GetOfferModifier() / 250));
}

bool item::Fly(character* Thrower, uchar Direction, ushort Force)
{
  vector2d StartingPos = GetSquareUnder()->GetPos();
  vector2d Pos = GetSquareUnder()->GetPos();
  bool Breaks = false;
  float Speed = float(Force) / GetWeight() * 1500;

  for(;;)
    {
      if(!game::IsValidPos(Pos + game::GetMoveVector(Direction)))
	break;

      lsquare* JustHit = game::GetCurrentLevel()->GetLSquare(Pos + game::GetMoveVector(Direction));

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

	  MoveTo(game::GetCurrentLevel()->GetLSquare(Pos)->GetStack());
	  game::DrawEverything();

	  if(game::GetCurrentLevel()->GetLSquare(Pos)->GetCharacter())
	    {
	      if(HitCharacter(Thrower, game::GetCurrentLevel()->GetLSquare(Pos)->GetCharacter(), Speed))
		{
		  Breaks = true;
		  break;
		}
	    }

	  while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
	}
    }

  MoveTo(game::GetCurrentLevel()->GetLSquare(Pos)->GetStack());

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

  if(RAND() % 2)
    ReceiveHitEffect(Dude, Thrower);

  Dude->HasBeenHitByItem(Thrower, this, Speed);
  return true;
}

item* item::PrepareForConsuming(character*)
{
  return this;
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

bool item::Zap(character*, vector2d, uchar)
{
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
  GetConsumeMaterial()->EatEffect(Eater, Amount, GetNPModifier());

  if(!Cannibalised && Eater->IsPlayer() && Eater->CheckCannibalism(GetConsumeMaterial()->GetType()))
    {
      game::DoEvilDeed(25);
      ADD_MESSAGE("You feel that this was an evil deed.");
      Cannibalised = true;
    }

  return GetConsumeMaterial()->GetVolume() ? false : true;
}

bool item::IsBadFoodForAI(character* Eater) const
{
  if(Eater->CheckCannibalism(GetConsumeMaterial()->GetType()))
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

  if(GetBaseEmitation() > Emitation)
    Emitation = GetBaseEmitation();

  game::PopItemID(ID);
  SaveFile >> Cannibalised >> Size >> ID;
}

void item::TeleportRandomly()
{
  /* This uses Player as the character that is used for walkability calculations, which might not be very wise. Please fix.*/

  MoveTo(game::GetCurrentLevel()->GetLSquare(game::GetCurrentLevel()->RandomSquare(0, true, false))->GetStack());
}

ushort item::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 1000;
}

void item::PlaceToSlot(slot* Slot)
{
  Slot->PutInItem(this);
}

void item::RemoveFromSlot()
{
  if(GetSlot())
    {
      GetSlot()->Empty();
      SetSlot(0);
    }
}

void item::MoveTo(stack* Stack)
{
  if(GetSlot())
    GetSlot()->MoveItemTo(Stack);
  else
    Stack->AddItem(this);
}

void item::DonateSlotTo(item* Item)
{
  slot* Slot = GetSlot();
  RemoveFromSlot();
  Item->PlaceToSlot(Slot);
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
    default: return "Illegal items";
    }
}

bool item::IsOnGround() const
{
  return GetSlot()->IsOnGround();
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

void item::Be()
{
  ABORT("Illegal item::Be()!");

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c) && !GetMaterial(c)->Be() && RemoveMaterial(c))
      {
	RemoveFromSlot();
	SendToHell();
	break;
      }
}

bool item::RemoveMaterial(uchar)
{
  return true;
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
  item* Item = Cloner(0, false, true);
  Item->Load(SaveFile);
  return Item;
}

void item::LoadDataBaseStats()
{
  SetEmitation(GetBaseEmitation());
  SetSize(GetDefaultSize());
}

void item::Initialize(uchar NewConfig, bool CallGenerateMaterials, bool Load)
{
  if(!Load)
    {
      Config = NewConfig;
      InstallDataBase();
      LoadDataBaseStats();
      RandomizeVisualEffects();

      if(CallGenerateMaterials)
	GenerateMaterials();
    }

  VirtualConstructor(Load);

  if(CallGenerateMaterials)
    UpdatePictures();
}

itemprototype::itemprototype(itemprototype* Base, item* (*Cloner)(ushort, bool, bool), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId)
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

void item::EditVolume(long What)
{
  Volume += What;

  if(GetSlot())
    GetSlot()->EditVolume(What);
}

void item::EditWeight(long What)
{
  Weight += What;

  if(GetSlot())
    GetSlot()->EditWeight(What);
}

void item::EditCarriedWeight(long What)
{
  CarriedWeight += What;

  if(GetSlot())
    GetSlot()->EditCarriedWeight(What);
}

ulong item::GetBlockModifier(const character* User) const
{
  if(!IsShield(User))
    return GetSize() * GetRoundness();
  else
    return GetSize() * GetRoundness() << 1;
}

bool item::IsSimiliarTo(item* Item) const
{
  return Item->GetType() == GetType() && Item->GetConfig() == GetConfig();
}

bool item::CanBeSeenByPlayer() const
{
  return GetSquareUnder()->CanBeSeenByPlayer();
}

bool item::CanBeSeenBy(character* Who) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer();
  else
    return GetSquareUnder()->CanBeSeenFrom(Who->GetPos(), Who->LOSRangeSquare());
}

std::string item::Description(uchar Case) const
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

square* item::GetSquareUnder() const
{
  if(Slot)
    return Slot->GetSquareUnder();
  else
    return 0;
}

lsquare* item::GetLSquareUnder() const
{
  return static_cast<lsquare*>(Slot->GetSquareUnder());
}
