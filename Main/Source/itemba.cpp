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

item::item(bool CreateMaterials, bool SetStats) : Slot(0), Cannibalised(false)
{
  ID = game::CreateNewItemID();

  if(CreateMaterials || SetStats)
    ABORT("Boo!");
}

void item::PositionedDrawToTileBuffer(uchar, bool Animate) const
{
  if(Animate)
    Picture[globalwindowhandler::GetTick() % AnimationFrames()]->MaskedBlit(igraph::GetTileBuffer());
  else
    Picture[0]->MaskedBlit(igraph::GetTileBuffer());
}

bool item::IsConsumable(character* Eater) const
{
  if(!GetConsumeMaterial())
    return false;
  else
    return Eater->CanEat(GetConsumeMaterial());
}

short item::CalculateOfferValue(char GodAlignment) const
{
  long OfferValue = 0;

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      OfferValue += GetMaterial(c)->CalculateOfferValue(GodAlignment);

  return short(OfferValue * (OfferModifier() / 250));
}

/******************************************
*This fly system seems to have been made, *
*just to handle only player               *
*kicking and throwing things...		  *
******************************************/

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
      if(!(JustHit->GetOLTerrain()->GetIsWalkable()))
	{
	  Breaks = true;
	  JustHit->HasBeenHitBy(this, Speed, Direction, JustHit->CanBeSeen());
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
      if(Dude->GetIsPlayer())
	ADD_MESSAGE("%s misses you.", CHARNAME(DEFINITE));
      else
	if(Dude->GetLSquareUnder()->CanBeSeen())
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

void item::DrawToTileBuffer(bool Animate) const
{
  PositionedDrawToTileBuffer(CENTER, Animate);
}

bool item::Apply(character* Applier)
{
  if(Applier->GetIsPlayer())
    ADD_MESSAGE("You can't apply this!");

  return false;
}

bool item::Zap(character*, vector2d, uchar)
{
  return false; 
}

bool item::Polymorph(stack* CurrentStack)
{
  CurrentStack->AddItem(protosystem::BalancedCreateItem());
  SetExists(false);
  return true;
}

/*uchar item::GetWeaponCategory() const
{
  return UNCATEGORIZED;
}*/

bool item::Consume(character* Eater, float Amount)
{
  GetConsumeMaterial()->EatEffect(Eater, Amount, NPModifier());

  if(!Cannibalised && Eater->GetIsPlayer() && Eater->CheckCannibalism(GetConsumeMaterial()->GetType()))
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
    return GetConsumeMaterial()->GetIsBadFoodForAI();
}

void item::Save(outputfile& SaveFile) const
{
  object::Save(SaveFile);
  SaveFile << Cannibalised << Size << ID;
}

void item::Load(inputfile& SaveFile)
{
  object::Load(SaveFile);
  game::PopItemID(ID);
  SaveFile >> Cannibalised >> Size >> ID;
}

void item::Teleport()
{
  /* This uses Player as the character that is used for walkability calculations, which might not be very wise. Please fix.*/

  MoveTo(game::GetCurrentLevel()->GetLSquare(game::GetCurrentLevel()->RandomSquare(game::GetPlayer(), true, false))->GetStack());
}

void item::DrawToTileBuffer(vector2d Pos, bool Animate) const
{
  vector2d From, To, BlitSize;

  if(Pos.X < 0)
    {
      From.X = -Pos.X;
      To.X = 0;
      BlitSize.X = 16 + Pos.X;
    }
  else
    {
      From.X = 0;
      To.X = Pos.X;
      BlitSize.X = 16 - Pos.X;
    }

  if(Pos.Y < 0)
    {
      From.Y = -Pos.Y;
      To.Y = 0;
      BlitSize.Y = 16 + Pos.Y;
    }
  else
    {
      From.Y = 0;
      To.Y = Pos.Y;
      BlitSize.Y = 16 - Pos.Y;
    }

  if(Animate)
    Picture[globalwindowhandler::GetTick() % AnimationFrames()]->MaskedBlit(igraph::GetTileBuffer(), From, To, BlitSize);
  else
    Picture[0]->MaskedBlit(igraph::GetTileBuffer(), From, To, BlitSize);
}

ushort item::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 1000;
}

void item::PlaceToSlot(slot* Slot)
{
  SetSlot(Slot);
  Slot->SetItem(this);
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
  GetSlot()->SetItem(Item);
  SetSlot(0);
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
    case SOUND: return SoundResistance();
    case ENERGY: return EnergyResistance();
    case ACID: return AcidResistance();
    case FIRE: return FireResistance();
    case POISON: return PoisonResistance();
    case BULIMIA: return BulimiaResistance();
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
	SetExists(false);
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
  SetExists(false);
}

item* item::TryToOpen(character* Char)
{
  if(Char->GetIsPlayer())
    ADD_MESSAGE("You can't open %s.", CHARNAME(DEFINITE));

  return 0;
}
