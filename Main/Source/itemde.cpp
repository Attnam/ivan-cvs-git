#define __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "itemba.h"

std::vector<item::prototype*> protocontainer<item>::ProtoData;
valuemap protocontainer<item>::CodeNameMap;

ITEM_PROTOTYPE(item, 0);

#include "femath.h"
#include "itemde.h"

#undef __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__

#include <cmath>

#include "message.h"
#include "stack.h"
#include "charde.h"
#include "level.h"
#include "lsquare.h"
#include "lterraba.h"
#include "config.h"
#include "godba.h"
#include "festring.h"
#include "whandler.h"
#include "lterrade.h"
#include "actionba.h"
#include "felist.h"
#include "save.h"
#include "team.h"
#include "error.h"

void banana::GenerateLeftOvers(character* Eater)
{
  item* Peel = new bananapeels(0, NO_MATERIALS);
  Peel->InitMaterials(GetMainMaterial());

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    {
      Eater->GetStackUnder()->AddItem(Peel);
      Eater->DexterityAction(1);
    }
  else
    Eater->GetStack()->AddItem(Peel);

  RemoveFromSlot();
  SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  SendToHell();
}

void potion::GenerateLeftOvers(character* Eater)
{
  ChangeConsumeMaterial(0);

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    {
      MoveTo(Eater->GetStackUnder());
      Eater->DexterityAction(1);
    }
  else
    MoveTo(Eater->GetStack());
}

bool scroll::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::SeeWholeMapCheatIsActive();
}

bool scrollofcreatemonster::Read(character* Reader)
{
  Reader->StartReading(this, 200);
  return true;
}

void scrollofcreatemonster::FinishReading(character* Reader)
{
  vector2d TryToCreate;

  for(ushort c = 0; c < 100; ++c)
    {
      TryToCreate = Reader->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);
      character* Monster = protosystem::CreateMonster();

      if(GetArea()->IsValidPos(TryToCreate) && GetNearSquare(TryToCreate)->IsWalkable(Monster) && GetNearSquare(TryToCreate)->GetCharacter() == 0)
	{
	  GetNearLSquare(TryToCreate)->AddCharacter(Monster);

	  if(Reader->IsPlayer())
	    {
	      if(Monster->CanBeSeenByPlayer())
		ADD_MESSAGE("%s appears.", Monster->CHAR_NAME(INDEFINITE));
	      else
		ADD_MESSAGE("You notice no effect.");
	    }
	  else if(Reader->CanBeSeenByPlayer())
	    {
	      if(Monster->CanBeSeenByPlayer())
		ADD_MESSAGE("%s summons %s!", Reader->CHAR_NAME(DEFINITE), Monster->CHAR_NAME(INDEFINITE));
	    }
	  else if(Monster->CanBeSeenByPlayer())
	    ADD_MESSAGE("Suddenly %s appears.", Monster->CHAR_NAME(INDEFINITE));

	  RemoveFromSlot();
	  SendToHell();
	  return;
	}
      else
	delete Monster;
    }

  ADD_MESSAGE("You feel a lost soul fly by you.");
  RemoveFromSlot();
  SendToHell();
}

bool scrollofteleportation::Read(character* Reader)
{
  Reader->StartReading(this, 200);
  return true;
}

void scrollofteleportation::FinishReading(character* Reader)
{
  if(Reader->IsPlayer())
    ADD_MESSAGE("Suddenly you realize you have teleported.");
  else if(Reader->CanBeSeenByPlayer())
    ADD_MESSAGE("%s disappears!", Reader->CHAR_NAME(DEFINITE));

  Reader->TeleportRandomly();
  RemoveFromSlot();
  SendToHell();
}

bool lump::HitEffect(character* Enemy, character*, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour && RAND() & 1)
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("The %s touches %s!", GetMainMaterial()->GetName(false, false).c_str(), Enemy->CHAR_DESCRIPTION(DEFINITE));

      bool Success = GetMainMaterial()->HitEffect(Enemy);

      if(!GetMainMaterial()->GetVolume())
	{
	  RemoveFromSlot();
	  SendToHell();
	}

      return Success;
    }
  else
    return false;
}

bool meleeweapon::HitEffect(character* Enemy, character*, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour && GetContainedMaterial())
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("The %s reacts with %s!", GetContainedMaterial()->GetName(false, false).c_str(), Enemy->CHAR_DESCRIPTION(DEFINITE));

      bool Success = GetContainedMaterial()->HitEffect(Enemy);

      if(!GetContainedMaterial()->GetVolume())
	Empty();

      return Success;
    }
  else
    return false;
}

void meleeweapon::DipInto(material* Material, character* Dipper)
{
  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now covered with %s.", CHAR_NAME(DEFINITE), Material->GetName(false, false).c_str());

  ChangeContainedMaterial(Material);
  Dipper->DexterityAction(10);
}

material* lump::CreateDipMaterial()
{
  material* Material = GetMainMaterial()->Clone(GetMainMaterial()->TakeDipVolumeAway());

  if(!GetMainMaterial()->GetVolume())
    {
      RemoveFromSlot();
      SendToHell();
    }

  return Material;
}

bool pickaxe::Apply(character* User)
{
  if(IsBroken())
    {
      ADD_MESSAGE("%s is totally broken.",CHAR_NAME(DEFINITE));
      return false;
    }

  uchar Dir = game::DirectionQuestion("What direction do you want to dig? [press a direction key]", false);

  vector2d Temp = game::GetMoveVector(Dir);

  if(Dir == DIR_ERROR || !GetArea()->IsValidPos(User->GetPos() + Temp))
    return false;

  lsquare* Square = GetNearLSquare(User->GetPos() + Temp);

  if(Square->CanBeDug())
    if(Square->GetOLTerrain()->CanBeDestroyed())
      if(Square->GetOLTerrain()->GetMainMaterial()->CanBeDug(GetMainMaterial()))
	{
	  uchar RoomNumber = Square->GetRoomIndex();

	  if(!RoomNumber || Square->GetLevel()->GetRoom(RoomNumber)->CheckDestroyTerrain(User, Square->GetOLTerrain()))
	    {
	      User->SwitchToDig(this, User->GetPos() + Temp);
	      User->DexterityAction(5);
	      return true;
	    }
	  else
	    return false;
	}
      else
	ADD_MESSAGE("%s is too hard to dig with %s.", Square->GetOLTerrain()->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
    else
      ADD_MESSAGE(Square->GetOLTerrain()->GetDigMessage().c_str());

  return false;
}

bool wand::Apply(character* Terrorist)
{
  if(Terrorist->IsPlayer() && !game::BoolQuestion("Are you sure you want to break " + GetName(DEFINITE) + "? [y/N]")) 
    return false;

  if(Terrorist->IsPlayer())
    ADD_MESSAGE("%s breaks in two and then explodes!", CHAR_NAME(DEFINITE));
  else if(Terrorist->CanBeSeenByPlayer())
    ADD_MESSAGE("%s breaks %s in two. It explodes!", Terrorist->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
  else if(GetSquareUnder()->CanBeSeenByPlayer())
    ADD_MESSAGE("Something explodes!");

  RemoveFromSlot();
  SendToHell();
  std::string DeathMsg;

  if(Terrorist->IsPlayer())
    DeathMsg = "exploded himself by breaking a wand";
  else
    DeathMsg = "kamikazed by " + Terrorist->GetKillName();

  Terrorist->GetLevel()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), 50);
  Terrorist->DexterityAction(5);
  return true;
}

void wand::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << TimesUsed << Charges;
}

void wand::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> TimesUsed >> Charges;
}

bool scrollofwishing::Read(character* Reader)
{
  Reader->StartReading(this, 2500);
  return true;
}

void scrollofwishing::FinishReading(character* Reader)
{
  std::string Temp;

  while(true)
    {
      Temp = game::StringQuestion("What do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
      item* TempItem = protosystem::CreateItem(Temp, Reader->IsPlayer());

      if(TempItem)
	{
	  Reader->GetStack()->AddItem(TempItem);
	  TempItem->SpecialGenerationHandler();

	  if(TempItem->HandleInPairs())
	    ADD_MESSAGE("Two %s appear from nothing and the scroll burns!", TempItem->CHAR_NAME(PLURAL));
	  else
	    ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CHAR_NAME(INDEFINITE));

	  RemoveFromSlot();
	  SendToHell();
	  return;
	}
    }
}

bool scrollofchangematerial::Read(character* Reader)
{
  Reader->StartReading(this, 1000);
  return true;
}

void scrollofchangematerial::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->GetItems() && !Reader->EquipsSomething())
    ADD_MESSAGE("You notice you have lost anything you wished to alter.");
  else
    {
      while(true)
	{
	  std::vector<item*> Item;
	  Reader->SelectFromPossessions(Item, "What item do you wish to change?", NO_MULTI_SELECT|SELECT_PAIR);

	  if(!Item.empty())
	    {
	      if(!Item[0]->IsMaterialChangeable())
		{
		  ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
		  break;
		}

	      if(Item[0]->HandleInPairs() && Item.size() == 1)
		{
		  ADD_MESSAGE("Only one %s will be altered.", Item[0]->CHAR_NAME(UNARTICLED));

		  if(!game::BoolQuestion("Still continue? [y/N]"))
		    continue;
		}

	      std::string Temp = game::StringQuestion("What material do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
	      material* TempMaterial = protosystem::CreateMaterial(Temp);

	      if(TempMaterial)
		{
		  material* MainMaterial = Item[0]->GetMainMaterial();
		  material* SecondaryMaterial = Item[0]->GetSecondaryMaterial();

		  if(Item.size() == 1)
		    {
		      if(!MainMaterial->IsSameAs(TempMaterial))
			{
			  ADD_MESSAGE("Suddenly your %s is consumed in roaring magical flames. As you lift it again it looks greatly altered.", Item[0]->CHAR_NAME(UNARTICLED));

			  if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
			    Item[0]->ChangeSecondaryMaterial(TempMaterial->Clone());

			  Item[0]->ChangeMainMaterial(TempMaterial);
			}
		      else
			ADD_MESSAGE("Suddenly your %s is consumed in roaring magical flames. As you lift it again it looks unchanged.", Item[0]->CHAR_NAME(UNARTICLED));
		    }
		  else
		    {
		      if(!MainMaterial->IsSameAs(TempMaterial))
			{
			  ADD_MESSAGE("Suddenly your %s are consumed in roaring magical flames. As you lift them again they look greatly altered.", Item[0]->CHAR_NAME(PLURAL));

			  if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
			    for(ushort c = 0; c < Item.size(); ++c)
			      Item[c]->ChangeSecondaryMaterial(TempMaterial->Clone());

			  Item[0]->ChangeMainMaterial(TempMaterial);

			  for(ushort c = 1; c < Item.size(); ++c)
			    Item[c]->ChangeMainMaterial(TempMaterial->Clone());
			}
		      else
			ADD_MESSAGE("Suddenly your %s are consumed in roaring magical flames. As you lift them again they look unchanged.", Item[0]->CHAR_NAME(PLURAL));
		    }

		  break;
		}
	      else
		game::DrawEverythingNoBlit();
	    }
	  else if(game::BoolQuestion("Really cancel read? [y/N]"))
	    return;
	}

      RemoveFromSlot();
      SendToHell();
    }
}

item* brokenbottle::BetterVersion() const
{
  material* Stuff;

  if(RAND() % 5)
    Stuff = MAKE_MATERIAL(BANANA_FLESH);
  else
    Stuff = MAKE_MATERIAL(OMMEL_URINE);

  potion* P = new potion(0, NO_MATERIALS);
  P->InitMaterials(MAKE_MATERIAL(GLASS), Stuff);
  return P;
}

void brokenbottle::StepOnEffect(character* Stepper)
{
  if(Stepper->HasFeet() && !(RAND() % 5))
    {
      if(Stepper->IsPlayer())
	ADD_MESSAGE("Auch. You step on sharp glass splinters.");
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s steps on sharp glass splinters.", Stepper->CHAR_NAME(DEFINITE));

      Stepper->ReceiveDamage(0, 1 + RAND() % 3, PHYSICAL_DAMAGE, LEGS);
      Stepper->CheckDeath("stepped on a broken bottle", 0);
    }
}

material* can::CreateDipMaterial()
{
  material* Material = GetContainedMaterial()->Clone(GetContainedMaterial()->TakeDipVolumeAway());

  if(!GetContainedMaterial()->GetVolume())
    Empty();

  return Material;
}

material* potion::CreateDipMaterial()
{
  material* Material = GetContainedMaterial()->Clone(GetContainedMaterial()->TakeDipVolumeAway());

  if(!GetContainedMaterial()->GetVolume())
    Empty();

  return Material;
}

void potion::DipInto(material* Material, character* Dipper)
{
  /* Add alchemy */

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now filled with %s.", CHAR_NAME(DEFINITE), Material->GetName(false, false).c_str());

  ChangeContainedMaterial(Material);
  Dipper->DexterityAction(10);
}

ulong meleeweapon::GetPrice() const
{
  float WeaponStrengthModifier = GetFormModifier() * GetMainMaterial()->GetStrengthValue() * (20 + Enchantment) / 20;
  WeaponStrengthModifier *= WeaponStrengthModifier;
  WeaponStrengthModifier *= GetMainMaterial()->GetWeight();
  return ulong(WeaponStrengthModifier / (20000000.0f * sqrt(GetWeight()))) + GetEnchantedPrice(Enchantment);
}

ulong armor::GetPrice() const
{
  float StrengthValue = GetStrengthValue();
  return ulong(StrengthValue * StrengthValue * StrengthValue * 20 / sqrt(GetWeight()));
}

void lantern::SignalSquarePositionChange(uchar NewSquarePosition)
{
  if(SquarePosition != NewSquarePosition)
    {
      SquarePosition = NewSquarePosition;
      UpdatePictures();
    }
}

void lantern::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << SquarePosition;
}

void lantern::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> SquarePosition;
}

item* potion::BetterVersion() const
{
  if(!GetContainedMaterial())
    {
      material* Stuff;

      if(RAND() % 5)
	Stuff = MAKE_MATERIAL(BANANA_FLESH);
      else
	Stuff = MAKE_MATERIAL(OMMEL_URINE);

      potion* P = new potion(0, NO_MATERIALS); 
      P->InitMaterials(MAKE_MATERIAL(GLASS), Stuff);
      return P;
    }
  else
    return 0;
}

item* can::BetterVersion() const
{
  if(!GetContainedMaterial())
    {
      material* Stuff;

      if(RAND() & 1)
	Stuff = MAKE_MATERIAL(SCHOOL_FOOD);
      else
	Stuff = MAKE_MATERIAL(BANANA_FLESH);

      can* P = new can(0, NO_MATERIALS); 
      P->InitMaterials(MAKE_MATERIAL(IRON), Stuff); 
      return P;
    }
  else
    return 0;
}

ushort whip::GetFormModifier() const
{
  return item::GetFormModifier() * GetMainMaterial()->GetFlexibility();
}

bool backpack::Apply(character* Terrorist)
{
  if(IsExplosive())
    {
      if(Terrorist->IsPlayer())
	ADD_MESSAGE("You light your %s. It explodes!", CHAR_NAME(UNARTICLED));
      else if(Terrorist->CanBeSeenByPlayer())
	ADD_MESSAGE("%s lights %s. It explodes!", Terrorist->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
      else if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("Something explodes!");

      RemoveFromSlot();
      SendToHell();

      std::string DeathMsg;

      if(Terrorist->IsPlayer())
	DeathMsg = "exploded himself with " + GetName(INDEFINITE);
      else
	DeathMsg = "kamikazed by " + Terrorist->GetKillName();

      Terrorist->DexterityAction(5);
      Terrorist->GetLevel()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }
  else if(Terrorist->IsPlayer())
    ADD_MESSAGE("You are not able to explode yourself with this crummy %s!", CHAR_NAME(UNARTICLED));

  return false;
}

bool holybook::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::SeeWholeMapCheatIsActive();
}

bool holybook::Read(character* Reader)
{
  Reader->StartReading(this, 1000);
  return true;
}

void holybook::FinishReading(character* Reader)
{
  if(Reader->IsPlayer())
    {
      if(GetMasterGod()->GetKnown())
	{
	  ADD_MESSAGE("The book reveals many divine secrets of %s to you.", GetMasterGod()->GOD_NAME);
	  GetMasterGod()->AdjustRelation(75);
	  game::ApplyDivineAlignmentBonuses(GetMasterGod(), true);
	  game::GetPlayer()->EditExperience(WISDOM, 250);

	  if(!(RAND() % 3))
	    {
	      ADD_MESSAGE("But then it disappears.");
	      RemoveFromSlot();
	      SendToHell();
	    }
	}
      else
	{
	  GetMasterGod()->SetKnown(true);
	  ADD_MESSAGE("You feel you master the magical rites of %s.", GetMasterGod()->GOD_NAME);
	}
    }
}

bool wand::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
{
  if((Type == FIRE || Type == ENERGY) && Damage && (Damage > 125 || !(RAND() % (250 / Damage))))
    {
      std::string DeathMsg = "killed by an explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), 50);
      return true;
    }

  return false;
}

bool backpack::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
{
  if((Type == FIRE || Type == ENERGY) && Damage && IsExplosive() && (Damage > 50 || !(RAND() % (100 / Damage))))
    {
      std::string DeathMsg = "killed by an explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

bool scroll::ReceiveDamage(character*, ushort Damage, uchar Type)
{
  if(Type == FIRE && Damage && GetMainMaterial()->IsFlammable() && (Damage > 125 || !(RAND() % (250 / Damage))))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s catches fire!", CHAR_NAME(DEFINITE));

      RemoveFromSlot();
      SendToHell();
      return true;
    }

  return false;
}

bool holybook::ReceiveDamage(character*, ushort Damage, uchar Type)
{
  if(Type == FIRE && Damage && GetMainMaterial()->IsFlammable() && (Damage > 125 || !(RAND() % (250 / Damage))))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s catches fire!", CHAR_NAME(DEFINITE));

      RemoveFromSlot();
      SendToHell();
      return true;
    }

  return false;
}

bool oillamp::Apply(character* Applier)
{
  if(Applier->IsPlayer())
    ADD_MESSAGE("You rub %s.", CHAR_NAME(DEFINITE));

  if(GetInhabitedByGenie())
    {
      genie* Genie = new genie;
      vector2d TryToCreate;
      bool FoundPlace = false;

      /* First try to create a genie nearby (10 tries - if all of them fail then stop trying) */

      for(ushort c = 0; c < 10 && !FoundPlace; ++c)
	{	  
	  TryToCreate = Applier->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

	  if(GetArea()->IsValidPos(TryToCreate) && GetNearSquare(TryToCreate)->IsWalkable(Genie) && GetNearSquare(TryToCreate)->GetCharacter() == 0)
	    {
	      GetNearSquare(TryToCreate)->AddCharacter(Genie);
	      FoundPlace = true;
	      SetInhabitedByGenie(false);
	      break;
	    }
	}

      if(FoundPlace)
	{
	  if(!(RAND() % 5))
	    {
	      Genie->SetTeam(game::GetTeam(MONSTER_TEAM));
	      ADD_MESSAGE("You see a puff of smoke, and %s appears. \"For centuries I have been imprisoned in this lamp. But at last you have freed me! As a reward, I will kill you.\"", Genie->CHAR_NAME(INDEFINITE));
	    }
	  else
	    {
	      if(Applier->IsPlayer())
		{
		  Genie->SetTeam(Applier->GetTeam());
		  ADD_MESSAGE("You see a puff of smoke, and %s appears. \"For centuries I have been imprisoned in this lamp. But at last you have freed me! I am deeply grateful. You deserve a generous reward. I may serve you for 1001 nights or grant you a wish. It's your choice.\"", Genie->CHAR_NAME(INDEFINITE));

		  if(game::BoolQuestion("Do you want to wish? [Y/n]", YES))
		    {
		      ADD_MESSAGE("You may wish for an item.");

		      while(true)
			{
			  std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
			  item* TempItem = protosystem::CreateItem(Temp, Applier->IsPlayer());

			  if(TempItem)
			    {
			      Applier->GetStack()->AddItem(TempItem);
			      TempItem->SpecialGenerationHandler();

			      if(TempItem->HandleInPairs())
				ADD_MESSAGE("Two %s appear from nothing and the genie flies happily away!", TempItem->CHAR_NAME(PLURAL));
			      else
				ADD_MESSAGE("%s appears from nothing and the genie flies happily away!", TempItem->CHAR_NAME(INDEFINITE));

			      break;
			    }
			}

		      GetLevel()->RemoveCharacter(TryToCreate);
		      delete Genie;
		      Applier->EditAP(-1000);
		      return true;
		    }
		}
	    }

	  ADD_MESSAGE("%s wishes for a two-handed scimitar. Suddenly %s appears from nothing and %s wields it.", Genie->CHAR_NAME(DEFINITE), Genie->GetMainWielded()->CHAR_NAME(INDEFINITE), Genie->CHAR_NAME(DEFINITE));
	}
      else
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You feel that it is warmer.");

	  delete Genie;
	}
    }
  else
    if(Applier->IsPlayer())
      ADD_MESSAGE("Nothing happens.");

  Applier->EditAP(-1000);
  return true;
}

void oillamp::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << InhabitedByGenie;
}

void oillamp::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> InhabitedByGenie;
}

ushort holybook::GetMaterialColorA(ushort) const
{
  return GetMasterGod()->GetColor();
}

bool scrollofcharging::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::ChargeableSorter) && !Reader->EquipsSomething(&item::ChargeableSorter))
    {
      ADD_MESSAGE("You have nothing to charge.");
      return false;
    }
  else
    {
      Reader->StartReading(this, 500);
      return true;
    }
}

void scrollofcharging::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::ChargeableSorter) && !Reader->EquipsSomething(&item::ChargeableSorter))
    ADD_MESSAGE("You have lost whatever you wished to charge.");
  else
    while(true)
      {
	std::vector<item*> Item;
	Reader->SelectFromPossessions(Item, "Which item do you wish to charge?", NO_MULTI_SELECT|SELECT_PAIR, &item::ChargeableSorter);

	if(!Item.empty())
	  {
	    if(Item[0]->HandleInPairs() && Item.size() == 1)
	      {
		ADD_MESSAGE("Only one %s will be charged.", Item[0]->CHAR_NAME(UNARTICLED));

		if(!game::BoolQuestion("Still continue? [y/N]"))
		  continue;
	      }

	    for(ushort c = 0; c < Item.size(); ++c)
	      Item[c]->ChargeFully(Reader);

	    ADD_MESSAGE("You charge %s and the scroll burns.", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
	    break;
	  }
	else if(game::BoolQuestion("Really cancel read? [y/N]"))
	  return;
      }

  RemoveFromSlot();
  SendToHell();
}

void banana::Save(outputfile& SaveFile) const
{
  meleeweapon::Save(SaveFile);
  SaveFile << Charges;
}

void banana::Load(inputfile& SaveFile)
{
  meleeweapon::Load(SaveFile);
  SaveFile >> Charges;
}

bool banana::Zap(character*, vector2d, uchar)
{
  if(Charges)
    {
      ADD_MESSAGE("BANG! You zap %s!", CHAR_NAME(DEFINITE));
      --Charges;
    }
  else
    ADD_MESSAGE("Click!");

  return true;
}

void bananapeels::StepOnEffect(character* Stepper)
{
  if(Stepper->HasFeet() && !(RAND() % 5))
    {
      if(Stepper->IsPlayer())
	ADD_MESSAGE("Auch. Your feet slip on %s and you fall down.", CHAR_NAME(INDEFINITE));
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s steps on %s and falls down.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));

      /* Do damage against any random bodypart except legs */

      Stepper->ReceiveDamage(0, 1 + (RAND() & 3), PHYSICAL_DAMAGE, ALL&~LEGS);
      Stepper->CheckDeath("slipped on a banana peel.", 0);
      Stepper->EditAP(-500);
    }
}

bool scrolloftaming::Read(character* Reader)
{
  Reader->StartReading(this, 100);
  return true;
}

void scrolloftaming::FinishReading(character* Reader)
{
  // First find all characters in the squares around Reader

  std::vector<character*> CharactersNearBy;

  for(ushort c = 0; c < 8; ++c)
    {
      vector2d Test = Reader->GetPos() + game::GetMoveVector(c);

      if(GetArea()->IsValidPos(Test))
	{
	  character* CharacterInSquare = GetNearSquare(Test)->GetCharacter();

	  if(CharacterInSquare && CharacterInSquare->IsCharmable() && CharacterInSquare->GetTeam() != Reader->GetTeam())
	    CharactersNearBy.push_back(CharacterInSquare);
	}
    }
  
  // Then pick one of the characters and set it to the same team as Reader

  if(CharactersNearBy.empty())
    {
      if(Reader->IsPlayer() || Reader->CanBeSeenByPlayer())
	ADD_MESSAGE("The scroll burns, but nothing happens.");
    }
  else
    {
      character* ToBeTamed = CharactersNearBy[RAND() % CharactersNearBy.size()];
      ToBeTamed->ChangeTeam(Reader->GetTeam());

      if(Reader->IsPlayer())
	{
	  if(ToBeTamed->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s looks much friendlier.", ToBeTamed->CHAR_NAME(INDEFINITE));
	  else
	    ADD_MESSAGE("You notice no effect.");
	}
      else if(Reader->CanBeSeenByPlayer())
	{
	  if(ToBeTamed->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s seems to like %s far more.", ToBeTamed->CHAR_NAME(INDEFINITE), Reader->CHAR_NAME(DEFINITE));
	}
      else if(ToBeTamed->CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks like having fallen in love with something nearby.", ToBeTamed->CHAR_NAME(INDEFINITE));
    }

  RemoveFromSlot();
  SendToHell();
}

void bodypart::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << BitmapPos << ColorB << ColorC << ColorD << SpecialFlags << HP << OwnerDescription << Unique << BloodColor;
}

void bodypart::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> BitmapPos >> ColorB >> ColorC >> ColorD >> SpecialFlags  >> HP >> OwnerDescription >> Unique >> BloodColor;
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
	    game::Beep();

	    if(IsAlive())
	      ADD_MESSAGE("Your %s bleeds very badly.", GetBodyPartName().c_str());
	    else
	      ADD_MESSAGE("Your %s is in very bad condition.", GetBodyPartName().c_str());
	  }
	else if(IsInBadCondition() && !IsInBadCondition(BHP))
	  {
	    game::Beep();

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

void mine::Load(inputfile& SaveFile)
{
  materialcontainer::Load(SaveFile);
  SaveFile >> Active >> Team;
}

void mine::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << Active << Team;
}

bool mine::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
{
  if(((Type == FIRE || Type == ENERGY) && Damage && (Damage > 50 || !(RAND() % (100 / Damage)))) || ((Type == PHYSICAL_DAMAGE || Type == SOUND) && WillExplode(0)))
    {
      std::string DeathMsg = "killed by an explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

void mine::StepOnEffect(character* Stepper)
{
  if(!WillExplode(Stepper))
    return;

  if(Stepper->IsPlayer())
    ADD_MESSAGE("You hear a faint thumb. You look down. You see %s.", CHAR_NAME(INDEFINITE));
  else if(Stepper->CanBeSeenByPlayer())
    ADD_MESSAGE("%s steps on %s.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
  else if(GetSquareUnder()->CanBeSeenByPlayer())
    ADD_MESSAGE("Something explodes!");

  SetIsActive(false);
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  GetLSquareUnder()->SendNewDrawRequest();

  if(Stepper->IsPlayer())
    game::AskForKeyPress("Trap activated! [press any key to continue]");

  lsquare* Square = GetLSquareUnder();
  RemoveFromSlot();
  SendToHell();
  Square->GetLevel()->Explosion(0, "killed by a land mine", Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
}

bool key::Apply(character* User)
{
  if(User->IsPlayer())
    {
      uchar Dir = game::DirectionQuestion("What door do you wish to lock or unlock? [press a direction key]", false, true);

      if(Dir == DIR_ERROR)
	return false;

      vector2d ApplyPos = User->GetPos() + game::GetMoveVector(Dir);

      if(GetArea()->IsValidPos(ApplyPos))
	{
	  GetNearLSquare(ApplyPos)->TryKey(this, User);
	  User->DexterityAction(5);
	}
      else
	return false;
    }

  return true;
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

ushort belt::GetFormModifier() const
{
  return item::GetFormModifier() * GetMainMaterial()->GetFlexibility();
}

bool pickaxe::IsAppliable(const character* Who) const
{
  return Who->CanWield();
}

void key::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LockType;
}

void key::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LockType;
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

void materialcontainer::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << ContainedMaterial;
}

void materialcontainer::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  LoadMaterial(SaveFile, ContainedMaterial);
}

void meleeweapon::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Enchantment;
  SaveFile << SecondaryMaterial;
  SaveFile << ContainedMaterial;
}

void meleeweapon::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Enchantment;
  LoadMaterial(SaveFile, SecondaryMaterial);
  LoadMaterial(SaveFile, ContainedMaterial);
}

material*& materialcontainer::GetMaterialReference(ushort Index)
{
  switch(Index)
    {
    case 0: return MainMaterial;
    default: return ContainedMaterial;
    }
}

material*& meleeweapon::GetMaterialReference(ushort Index)
{
  switch(Index)
    {
    case 0: return MainMaterial;
    case 1: return SecondaryMaterial;
    default: return ContainedMaterial;
    }
}

material*& corpse::GetMaterialReference(ushort Index)
{
  return GetDeceased()->GetTorso()->GetMaterialReference(Index);
}

ushort materialcontainer::GetMaterialColorB(ushort Frame) const
{
  if(GetContainedMaterial())
    return GetContainedMaterial()->GetColor();
  else
    return GetMaterialColorA(Frame);
}

uchar materialcontainer::GetAlphaB(ushort Frame) const
{
  if(GetContainedMaterial() && GetContainedMaterial()->GetAlpha() > GetAlphaA(Frame))
    return GetContainedMaterial()->GetAlpha();
  else
    return GetAlphaA(Frame);
}

ushort meleeweapon::GetMaterialColorB(ushort) const
{
  if(GetSecondaryMaterial())
    return GetSecondaryMaterial()->GetColor();
  else
    return 0;
}

uchar meleeweapon::GetAlphaB(ushort) const
{
  if(GetSecondaryMaterial())
    return GetSecondaryMaterial()->GetAlpha();
  else
    return 0;
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

void banana::VirtualConstructor(bool Load)
{
  meleeweapon::VirtualConstructor(Load);

  if(!Load)
    SetCharges(GetMinCharges() + RAND() % (GetMaxCharges() - GetMinCharges() + 1));
}

void lantern::VirtualConstructor(bool)
{
  SetSquarePosition(CENTER);
}

void wand::VirtualConstructor(bool Load)
{
  if(!Load)
    {
      SetCharges(GetMinCharges() + RAND() % (GetMaxCharges() - GetMinCharges() + 1));
      SetTimesUsed(0);
    }
}

void oillamp::VirtualConstructor(bool Load)
{
  if(!Load)
    SetInhabitedByGenie(RAND() & 1);
}

void mine::VirtualConstructor(bool)
{
  Active = false; /* this must be false */
}

void key::VirtualConstructor(bool Load)
{ 
  if(!Load)
    SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
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

bool whistle::Apply(character* Whistler) 
{
  BlowEffect(Whistler);
  Whistler->EditAP(-1000);
  return true;
} 

void whistle::BlowEffect(character* Whistler)
{
  if(Whistler->IsPlayer())
    ADD_MESSAGE("You produce an interesting sound.");
  else if(Whistler->CanBeSeenByPlayer())
    ADD_MESSAGE("%s blows %s and produces an interesting sound.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
  else 
    ADD_MESSAGE("You hear a whistle playing.");

  game::CallForAttention(GetPos(), GetRange());
}

void magicalwhistle::BlowEffect(character* Whistler)
{
  if(LastUsed != 0 && game::GetTicks() - LastUsed < 2500)
    {
      whistle::BlowEffect(Whistler);
      return;
    }
  else 
    LastUsed = game::GetTicks();

  if(Whistler->IsPlayer())
    ADD_MESSAGE("You produce a peculiar sound.");
  else if(Whistler->CanBeSeenByPlayer())
    ADD_MESSAGE("%s blows %s and produces a peculiar sound.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
  else 
    ADD_MESSAGE("You hear a strange tune playing.");

  const std::list<character*>& Member = Whistler->GetTeam()->GetMember();

  for(std::list<character*>::const_iterator i = Member.begin(); i != Member.end(); ++i)
    if((*i)->IsEnabled() && Whistler != *i)
      (*i)->TeleportNear(Whistler);

  game::CallForAttention(GetPos(), GetRange());
}

void itemcontainer::VirtualConstructor(bool Load)
{
  Contained = new stack(0, this, HIDDEN, true);

  if(!Load)
    {
      SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
      SetIsLocked(RAND() % 3 != 0);
      ulong ItemNumber = RAND() % (GetMaxGeneratedContainedItems() + 1);

      for(ushort c = 0; c < ItemNumber; ++c)
	{
	  item* NewItem = protosystem::BalancedCreateItem();
	  ulong Volume = NewItem->GetVolume();

	  if(NewItem->HandleInPairs())
	    Volume <<= 1;

	  if(NewItem->CanBeGeneratedInContainer() && (GetStorageVolume() - GetContained()->GetVolume()) >= Volume)
	    {
	      GetContained()->AddItem(NewItem);
	      NewItem->SpecialGenerationHandler();
	    }
	  else
	    delete NewItem;
	}
    }
}

bool itemcontainer::TryKey(item* Key, character* Applier)
{
  if(GetLockType() == DAMAGED)
    {
      ADD_MESSAGE("The lock is broken.");
      return true;
    }

  if(Key->CanOpenLockType(GetLockType()))
    {
      if(IsLocked())
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You unlock %s.", CHAR_DESCRIPTION(DEFINITE));
	  else if(Applier->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s unlocks %s.", Applier->CHAR_NAME(DEFINITE), CHAR_DESCRIPTION(DEFINITE));
	}
      else
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You lock %s.", CHAR_DESCRIPTION(DEFINITE));
	  else if(Applier->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s locks %s.", Applier->CHAR_NAME(DEFINITE), CHAR_DESCRIPTION(DEFINITE));
	}

      SetIsLocked(!IsLocked());
    }
  else
    {
      if(Applier->IsPlayer())
	ADD_MESSAGE("%s doesn't fit in the lock.", Key->CHAR_NAME(DEFINITE));
      else if(Applier->CanBeSeenByPlayer())
	ADD_MESSAGE("%s tries to fit %s in the lock, but fails.", Applier->CHAR_NAME(DEFINITE), Key->CHAR_NAME(DEFINITE));
    }

  return true;
}

void materialcontainer::GenerateMaterials()
{
  ushort Chosen = RandomizeMaterialConfiguration();
  InitChosenMaterial(MainMaterial, GetMainMaterialConfig(), GetDefaultMainVolume(), Chosen);
  InitChosenMaterial(ContainedMaterial, GetContainedMaterialConfig(), GetDefaultContainedVolume(), Chosen);
}

void meleeweapon::GenerateMaterials()
{
  ushort Chosen = RandomizeMaterialConfiguration();
  InitChosenMaterial(MainMaterial, GetMainMaterialConfig(), GetDefaultMainVolume(), Chosen);
  InitChosenMaterial(SecondaryMaterial, GetSecondaryMaterialConfig(), GetDefaultSecondaryVolume(), Chosen);
  InitChosenMaterial(ContainedMaterial, GetContainedMaterialConfig(), GetDefaultContainedVolume(), Chosen);
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

/* Returns true if container opens fine else false */

bool itemcontainer::Open(character* Opener)
{
  if(IsLocked())
    {
      ADD_MESSAGE("%s seems to be locked.", CHAR_NAME(DEFINITE));
      return false;
    }

  std::string Question = "Do you want to (t)ake something from or (p)ut something in this container? [t,p]";
  bool Success;

  switch(game::KeyQuestion(Question, KEY_ESC, 3, 't', 'p', KEY_ESC))
    {
    case 't':
      Success = GetContained()->TakeSomethingFrom(Opener, GetName(DEFINITE));
      break;
    case 'p':
      Success = GetContained()->PutSomethingIn(Opener, GetName(DEFINITE), GetStorageVolume(), GetID());
      break;
    default:
      return false;
    }

  if(Success)
    Opener->DexterityAction(Opener->OpenMultiplier() * 5);

  return Success;
}

void itemcontainer::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  Contained->Save(SaveFile);
  SaveFile << LockType << Locked;
}

void itemcontainer::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  Contained->Load(SaveFile);
  SaveFile >> LockType >> Locked;
}

bool itemcontainer::Polymorph(stack* CurrentStack)
{
  GetContained()->MoveItemsTo(CurrentStack);
  item::Polymorph(CurrentStack);
  return true;
}

itemcontainer::~itemcontainer()
{
  delete Contained;
}

const std::string& key::GetAdjective() const
{
  return game::GetLockDescription(LockType);
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

/* Victim is of course the stuck person, Bodypart is the index of the bodypart that the trap is stuck to and the last vector2d is just a direction vector that may - or may not - be used in the future. This function returns true if the character manages to escape */

bool beartrap::TryToUnstuck(character* Victim, ushort BodyPart, vector2d)
{
  if(IsBroken())
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONE_INDEX);
      return true;
    }

  if(!(RAND() % 3))
    {
      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to hurt your %s even more.", Victim->GetBodyPartName(BodyPart).c_str());
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s hurts %s %s more with %s.", Victim->CHAR_NAME(DEFINITE), Victim->GetPossessivePronoun().c_str(), Victim->GetBodyPartName(BodyPart).c_str(), CHAR_NAME(DEFINITE));

      Victim->ReceiveBodyPartDamage(0, 2 + (RAND() & 1), PHYSICAL_DAMAGE, BodyPart, YOURSELF, false, false, false);
      Victim->CheckDeath("died while trying to escape from " + GetName(DEFINITE), 0);
      return false;
    }

  if(!(RAND() % 5))
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONE_INDEX);

      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to free yourself from %s.", CHAR_NAME(DEFINITE));
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s manages to free %sself from %s.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN, CHAR_NAME(DEFINITE));

      return true;
    }

  if(!(RAND() % 10))
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONE_INDEX);
      Break();

      if(Victim->IsPlayer())
	ADD_MESSAGE("You are freed.");
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is freed.", Victim->CHAR_NAME(DEFINITE));

      return true;
    }

  if(Victim->IsPlayer())
    ADD_MESSAGE("You are unable to escape from %s.", CHAR_NAME(DEFINITE));

  return false;
}

void beartrap::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Active >> Team;
}

void beartrap::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Active << Team;
}

void beartrap::VirtualConstructor(bool)
{
  Active = false; /* this must be false */
}

void beartrap::StepOnEffect(character* Stepper)
{
  if(IsActive() && !Stepper->IsStuck() && !IsBroken())
    {
      ushort StepperBodyPart = Stepper->GetRandomStepperBodyPart();

      if(StepperBodyPart == NONE_INDEX)
	return;

      Stepper->SetStuckTo(this);
      Stepper->SetStuckToBodyPart(StepperBodyPart);

      if(Stepper->IsPlayer())
	ADD_MESSAGE("You step in %s and it traps your %s.", CHAR_NAME(INDEFINITE), Stepper->GetBodyPartName(StepperBodyPart).c_str());
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is trapped in %s.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));

      SetIsActive(false);
      GetLSquareUnder()->SendMemorizedUpdateRequest();
      GetLSquareUnder()->SendNewDrawRequest();

      if(Stepper->IsPlayer())
	game::AskForKeyPress("Trap activated! [press any key to continue]");

      Stepper->ReceiveBodyPartDamage(0, 3 + RAND() % 3, PHYSICAL_DAMAGE, Stepper->GetStuckToBodyPart(), YOURSELF, false, false, false);
      Stepper->CheckDeath("died by stepping to " + GetName(INDEFINITE), 0);
    }
}

bool beartrap::CheckPickUpEffect(character* Picker)
{
  if(Picker->IsStuck() && Picker->GetStuckTo()->GetID() == GetID())
    {
      if(Picker->IsPlayer())
	ADD_MESSAGE("%s is tightly stuck to your %s.", CHAR_NAME(DEFINITE), Picker->GetBodyPartName(Picker->GetStuckToBodyPart()).c_str());

      return false;
    }

  SetIsActive(false);
  return true;
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

bool beartrap::IsPickable(character* Picker) const
{
  return !IsActive() && (!Picker->GetStuckTo() || Picker->GetStuckTo()->GetID() != GetID());
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

void can::GenerateLeftOvers(character* Eater)
{
  ChangeConsumeMaterial(0);

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    {
      MoveTo(Eater->GetStackUnder());
      Eater->DexterityAction(1);
    }
  else
    MoveTo(Eater->GetStack());
}

uchar bodypart::GetMaxAlpha(ushort) const
{
  if(GetMaster() && GetMaster()->StateIsActivated(INVISIBLE))
    return 150;
  else
    return 255;
}

uchar lantern::GetSpecialFlags() const
{
  switch(SquarePosition)
    {
    case RIGHT:
      return ROTATE;
    case DOWN:
      return FLIP;
    case LEFT:
      return ROTATE|MIRROR;
    default:
      return 0;
    }
}

void bodypart::AddPostFix(std::string& String) const
{
  if(OwnerDescription.length())
    String << ' ' << OwnerDescription;
}

bool stethoscope::Apply(character* Doctor) 
{
  if(!Doctor->CanUseStethoscope(true))
    return false;

  if(!Doctor->IsPlayer())
    ABORT("Doctor is not here, man, but these pills taste just as good anyway.");

  uchar Dir = game::DirectionQuestion("What do you want to inspect? [press a direction key]", false,true);  

  if(Dir == DIR_ERROR)
    return false;

  Doctor->DexterityAction(2);
  character* Char = GetNearSquare(GetPos() + game::GetMoveVector(Dir))->GetCharacter();

  if(!Char)
    {
      ADD_MESSAGE("There's no-one here.");
      return false;
    }

  Char->DisplayStethoscopeInfo(Doctor);
  return true;
} 

void itemcontainer::CalculateVolumeAndWeight()
{
  item::CalculateVolumeAndWeight();
  Volume += Contained->GetVolume();
  Weight += Contained->GetWeight();
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

bool flamingsword::HitEffect(character* Enemy, character* Hitter, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, BodyPartIndex, Direction, BlockedByArmour);

  if(Enemy->IsEnabled() && RAND() & 1)
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s sword burns %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

      return Enemy->ReceiveBodyPartDamage(Hitter, 2 + (RAND() & 3), FIRE, BodyPartIndex, Direction) != 0 || BaseSuccess;
    }
  else
    return BaseSuccess;
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

materialcontainer::~materialcontainer()
{
  delete ContainedMaterial;
}

meleeweapon::~meleeweapon()
{
  delete SecondaryMaterial;
  delete ContainedMaterial;
}

bool mjolak::HitEffect(character* Enemy, character* Hitter, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, BodyPartIndex, Direction, BlockedByArmour);

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

bool vermis::HitEffect(character* Enemy, character* Hitter, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, BodyPartIndex, Direction, BlockedByArmour);

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

bool turox::HitEffect(character* Enemy, character* Hitter, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s smash%s %s with the full force of Turox.", Hitter->CHAR_PERSONAL_PRONOUN, Hitter->IsPlayer() ? "" : "es", Enemy->CHAR_DESCRIPTION(DEFINITE));

      std::string DeathMSG = "killed by " + Hitter->GetKillName(); 
      Enemy->GetLevel()->Explosion(Hitter, DeathMSG, Enemy->GetPos(), 80 + RAND() % 20 - RAND() % 20);
      return true;
    }
  else
    return BaseSuccess;
}

bool whipofthievery::HitEffect(character* Enemy, character* Hitter, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && CleptiaHelps(Enemy, Hitter))
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

void bodypart::RandomizePosition()
{
  SpecialFlags |= 1 + RAND() % 7;
  UpdatePictures();
}

bool itemcontainer::ContentsCanBeSeenBy(const character* Viewer) const
{
  return GetMainMaterial()->IsTransparent() && CanBeSeenBy(Viewer);
}

bool mine::CanBeSeenBy(const character* Viewer) const 
{ 
  return (!IsActive() || (Viewer->GetTeam()->GetID() == Team)) && materialcontainer::CanBeSeenBy(Viewer); 
}

bool beartrap::CanBeSeenBy(const character* Viewer) const 
{ 
  return (!IsActive() || (Viewer->GetTeam()->GetID() == Team)) && item::CanBeSeenBy(Viewer); 
}

bool mine::Apply(character* User)
{
  if(User->IsPlayer())
    ADD_MESSAGE("%s is now %sactive.", CHAR_NAME(DEFINITE), IsActive() ? "in" : "");

  SetIsActive(!IsActive());
  User->DexterityAction(10);

  if(IsActive())
    {
      Team = User->GetTeam()->GetID();
      RemoveFromSlot();
      User->GetStackUnder()->AddItem(this);
    }

  return true;
}

bool beartrap::Apply(character* User)
{
  if(IsBroken())
    {
      if(User->IsPlayer())
	ADD_MESSAGE("%s is useless.", CHAR_NAME(DEFINITE));

      return false;
    }

  if(User->IsPlayer())
    ADD_MESSAGE("%s is now %sactive.", CHAR_NAME(DEFINITE), IsActive() ? "in" : "");

  SetIsActive(!IsActive());
  User->DexterityAction(10);

  if(IsActive())
    {
      Team = User->GetTeam()->GetID();
      RemoveFromSlot();
      User->GetStackUnder()->AddItem(this);
    }

  return true;
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

vector2d beartrap::GetBitmapPos(ushort Frame) const
{
  if(!IsBroken())
    return IsActive() ? vector2d(32, 304) : vector2d(32, 320);
  else
    return item::GetBitmapPos(Frame);
}

bool mine::WillExplode(const character* Stepper) const
{
  return IsActive() && GetContainedMaterial() && GetContainedMaterial()->IsExplosive() && (!Stepper || Stepper->GetWeight() > 5000);
}

materialcontainer::materialcontainer(const materialcontainer& MC) : item(MC)
{
  CopyMaterial(MC.ContainedMaterial, ContainedMaterial);
}

meleeweapon::meleeweapon(const meleeweapon& MW) : item(MW), Enchantment(MW.Enchantment)
{
  CopyMaterial(MW.SecondaryMaterial, SecondaryMaterial);
  CopyMaterial(MW.ContainedMaterial, ContainedMaterial);
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

itemcontainer::itemcontainer(const itemcontainer& Container) : item(Container), LockType(Container.LockType), Locked(Container.Locked)
{
  Contained = new stack(0, this, HIDDEN, true);
}

oillamp::oillamp(const oillamp& Lamp) : item(Lamp), InhabitedByGenie(false)
{
}

bool whipofthievery::CleptiaHelps(const character* Enemy, const character* Hitter) const
{
  /* Gum solution! */

  if(game::GetPetrus() == Enemy)
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

bool wand::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Zapper->EditExperience(PERCEPTION, 250);
  std::string DeathMSG = "killed by " + GetName(INDEFINITE);
  (GetLevel()->*level::GetBeam(GetBeamStyle()))(Zapper, DeathMSG, Zapper->GetPos(), GetBeamColor(), GetBeamEffect(), Direction, GetBeamRange());
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

void meleeweapon::AddInventoryEntry(const character* Viewer, std::string& Entry, ushort, bool ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
    {
      Entry << " [" << GetWeight() << "g, DAM " << GetBaseMinDamage() << "-" << GetBaseMaxDamage() << ", " << GetBaseToHitValueDescription();

      if(!IsBroken() && !IsWhip())
	Entry << ", " << GetStrengthValueDescription();

      uchar CWeaponSkillLevel = Viewer->GetCWeaponSkillLevel(this);
      uchar SWeaponSkillLevel = Viewer->GetSWeaponSkillLevel(this);

      if(CWeaponSkillLevel || SWeaponSkillLevel)
	Entry << ", skill " << CWeaponSkillLevel << '/' << SWeaponSkillLevel;

      Entry << "]";
    }
}

void banana::AddInventoryEntry(const character* Viewer, std::string& Entry, ushort Amount, bool ShowSpecialInfo) const
{
  item::AddInventoryEntry(Viewer, Entry, Amount, ShowSpecialInfo);
}

void armor::AddInventoryEntry(const character*, std::string& Entry, ushort Amount, bool ShowSpecialInfo) const
{
  if(Amount == 1)
    AddName(Entry, INDEFINITE);
  else
    {
      Entry << Amount << " ";
      AddName(Entry, PLURAL);
    }

  if(ShowSpecialInfo)
    Entry << " [" << GetWeight() * Amount << "g, AV " << GetStrengthValue() << "]";
}

void shield::AddInventoryEntry(const character* Viewer, std::string& Entry, ushort, bool ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
    {
      Entry << " [" << GetWeight() << "g, "  << GetBaseToHitValueDescription();

      if(!IsBroken())
	Entry << ", " << GetStrengthValueDescription();

      uchar CWeaponSkillLevel = Viewer->GetCWeaponSkillLevel(this);
      uchar SWeaponSkillLevel = Viewer->GetSWeaponSkillLevel(this);

      if(CWeaponSkillLevel || SWeaponSkillLevel)
	Entry << ", skill " << CWeaponSkillLevel << '/' << SWeaponSkillLevel;

      Entry << "]";
    }
}

void wand::AddInventoryEntry(const character*, std::string& Entry, ushort, bool ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
    {
      Entry << " [" << GetWeight();

      if(TimesUsed == 1)
	Entry << "g, used 1 time]";
      else if(TimesUsed)
	Entry << "g, used " << TimesUsed << " times]";
      else
	Entry << "g]";
    }
}

void materialcontainer::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == GetMainMaterial())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s becomes so spoiled that it cannot hold its contents anymore.", CHAR_NAME(DEFINITE));

      if(GetContainedMaterial()->IsLiquid())
	{
	  if(!game::IsInWilderness())
	    GetLSquareUnder()->SpillFluid(5, GetContainedMaterial()->GetColor());

	  RemoveFromSlot();
	}
      else
	{
	  lump* Lump = new lump(0, NO_MATERIALS);
	  Lump->InitMaterials(GetContainedMaterial());
	  DonateSlotTo(Lump);
	  SetContainedMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
	}

      SendToHell();
    }
  else if(Material == GetContainedMaterial())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The contents of %s spoil completely.", CHAR_NAME(DEFINITE));

      Empty();
    }
}

void banana::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == GetSecondaryMaterial() && !GetMainMaterial()->IsVeryCloseToSpoiling())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The inside of %s spoils completely.", CHAR_NAME(DEFINITE));

      item* Peel = new bananapeels(0, NO_MATERIALS);
      Peel->InitMaterials(GetMainMaterial());
      DonateSlotTo(Peel);
      SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
      SendToHell();
    }
  else
    item::SignalSpoil(Material); // this should spill potential poison liquid to the ground!
}

void meleeweapon::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == GetContainedMaterial())
    {
      Empty();

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s seems cleaner now.", CHAR_NAME(DEFINITE));
    }
  else
    item::SignalSpoil(Material); // this should spill potential poison liquid to the ground!
}

void meleeweapon::AddPostFix(std::string& String) const
{
  item::AddPostFix(String);

  if(GetContainedMaterial())
    {
      String << " covered with ";
      GetContainedMaterial()->AddName(String, false, false);
    }

  if(Enchantment > 0)
    String << " +" << short(Enchantment);
  else if(Enchantment < 0)
    String << ' ' << short(Enchantment);
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

bool materialcontainer::CanBePiledWith(const item* Item, const character* Viewer) const
{
  if(!item::CanBePiledWith(Item, Viewer))
    return false;

  const materialcontainer* Weapon = static_cast<const materialcontainer*>(Item);

  if(ContainedMaterial == 0 && Weapon->ContainedMaterial == 0)
    return true;

  return ContainedMaterial != 0
      && Weapon->ContainedMaterial != 0
      && ContainedMaterial->IsSameAs(Weapon->ContainedMaterial)
      && ContainedMaterial->GetSpoilLevel() == Weapon->ContainedMaterial->GetSpoilLevel();
}

bool meleeweapon::CanBePiledWith(const item* Item, const character* Viewer) const
{
  if(!item::CanBePiledWith(Item, Viewer))
    return false;

  const meleeweapon* Weapon = static_cast<const meleeweapon*>(Item);

  if(Enchantment != Weapon->Enchantment || !SecondaryMaterial->IsSameAs(Weapon->SecondaryMaterial) || SecondaryMaterial->GetSpoilLevel() != Weapon->SecondaryMaterial->GetSpoilLevel())
    return false;

  if(ContainedMaterial == 0 && Weapon->ContainedMaterial == 0)
    return true;

  return ContainedMaterial != 0
      && Weapon->ContainedMaterial != 0
      && ContainedMaterial->IsSameAs(Weapon->ContainedMaterial)
      && ContainedMaterial->GetSpoilLevel() == Weapon->ContainedMaterial->GetSpoilLevel();
}

bool armor::CanBePiledWith(const item* Item, const character* Viewer) const
{
  return item::CanBePiledWith(Item, Viewer) && Enchantment == static_cast<const armor*>(Item)->Enchantment;
}

bool key::CanBePiledWith(const item* Item, const character* Viewer) const
{
  return item::CanBePiledWith(Item, Viewer) && LockType == static_cast<const key*>(Item)->LockType;
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

ulong mine::GetPrice() const
{
  if(GetContainedMaterial())
    {
      if(GetContainedMaterial()->IsExplosive())
	return (GetContainedMaterial()->GetRawPrice() << 1) + item::GetPrice();
      else
	return GetContainedMaterial()->GetRawPrice() + item::GetPrice();
    }
  else
    return item::GetPrice();
}

ulong itemcontainer::GetPrice() const
{
  return GetContained()->GetPrice() + item::GetPrice();
}

void potion::Break()
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s shatters to pieces.", CHAR_NAME(DEFINITE));

  item* Remains = new brokenbottle(0, NO_MATERIALS);
  Remains->InitMaterials(GetMainMaterial()->Clone());
  Remains->SetID(BackupID);
  Remains->SetBackupID(ID);
  DonateSlotTo(Remains);
  SendToHell();

  if(GetContainedMaterial()) 
    Remains->GetLSquareUnder()->SpillFluid(0, GetContainedMaterial()->Clone(), 70);

  if(game::GetPlayer()->Equips(Remains))
    game::AskForKeyPress("Equipment broken! [press any key to continue]");
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

void materialcontainer::Be()
{
  MainMaterial->Be();

  if(Exists() && ContainedMaterial)
    ContainedMaterial->Be();
}

void meleeweapon::Be()
{
  MainMaterial->Be();

  if(Exists())
    SecondaryMaterial->Be();

  if(Exists() && ContainedMaterial)
    ContainedMaterial->Be();
}

void bodypart::InitSpecialAttributes()
{
  BloodColor = Master->GetBloodColor();
}

ulong shield::GetPrice() const /* temporary... */
{
  float StrengthValue = GetStrengthValue();
  return ulong(sqrt(GetBaseBlockValue()) * StrengthValue * StrengthValue) + item::GetPrice();
}

ulong whipofthievery::GetPrice() const
{
  /* If not broken but not flexible enough to work, special thievery bonus must be removed */

  return GetMainMaterial()->GetFlexibility() > 5 || IsBroken() ? whip::GetPrice() : whip::GetPrice() - item::GetPrice();
}

bool materialcontainer::IsSparkling(ushort ColorIndex) const
{
  return (ColorIndex == 0 && MainMaterial->IsSparkling()) || (ColorIndex == 1 && ContainedMaterial && ContainedMaterial->IsSparkling());
}

bool meleeweapon::IsSparkling(ushort ColorIndex) const
{
  return (ColorIndex == 0 && MainMaterial->IsSparkling()) || (ColorIndex == 1 && SecondaryMaterial->IsSparkling()) || (ColorIndex == 2 && ContainedMaterial && ContainedMaterial->IsSparkling());
}

ushort justifier::GetOutlineColor(ushort Frame) const
{
  Frame &= 31;
  return MakeRGB16(0, 135 + (Frame * (31 - Frame) >> 1), 0);
}

ushort neercseulb::GetOutlineColor(ushort Frame) const
{
  Frame &= 31;
  return MakeRGB16(135 + (Frame * (31 - Frame) >> 1), 0, 0);
}

ushort goldeneagleshirt::GetOutlineColor(ushort Frame) const
{
  Frame &= 31;
  ushort Element = 135 + (Frame * (31 - Frame) >> 2);
  return MakeRGB16(0, Element, Element);
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

void armor::AddPostFix(std::string& String) const
{
  item::AddPostFix(String);

  if(Enchantment > 0)
    String << " +" << short(Enchantment);
  else if(Enchantment < 0)
    String << ' ' << short(Enchantment);
}

bool scrollofenchantweapon::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::WeaponSorter) && !Reader->EquipsSomething(&item::WeaponSorter))
    {
      ADD_MESSAGE("You have nothing to enchant.");
      return false;
    }
  else
    {
      Reader->StartReading(this, 1000);
      return true;
    }
}

void scrollofenchantweapon::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::WeaponSorter) && !Reader->EquipsSomething(&item::WeaponSorter))
    ADD_MESSAGE("You notice you have lost anything you wished to enchant.");
  else
    {
      while(true)
	{
	  std::vector<item*> Item;
	  Reader->SelectFromPossessions(Item, "Choose a weapon to enchant:", NO_MULTI_SELECT|SELECT_PAIR, &item::WeaponSorter);

	  if(!Item.empty())
	    {
	      if(!Item[0]->CanBeEnchanted())
		{
		  ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
		  break;
		}

	      if(Item[0]->HandleInPairs() && Item.size() == 1)
		{
		  ADD_MESSAGE("Only one %s will be enchanted.", Item[0]->CHAR_NAME(UNARTICLED));

		  if(!game::BoolQuestion("Still continue? [y/N]"))
		    continue;
		}

	      if(Item[0]->GetEnchantment() >= 5 && RAND() % (Item[0]->GetEnchantment() - 3))
		{
		  if(Item.size() == 1)
		    ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant it further!", Item[0]->CHAR_NAME(DEFINITE));
		  else
		    ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant them further!", Item[0]->CHAR_NAME(DEFINITE|PLURAL));

		  break;
		}

	      if(Item.size() == 1)
		ADD_MESSAGE("Your %s glows briefly red. It feels very warm now.", Item[0]->CHAR_NAME(UNARTICLED));
	      else
		ADD_MESSAGE("Your %s glow briefly red. They feel very warm now.", Item[0]->CHAR_NAME(PLURAL));

	      for(ushort c = 0; c < Item.size(); ++c)
		Item[c]->EditEnchantment(1);

	      break;
	    }
	  else if(game::BoolQuestion("Really cancel read? [y/N]"))
	    return;
	}

      RemoveFromSlot();
      SendToHell();
    }
}

bool scrollofenchantarmor::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::ArmorSorter) && !Reader->EquipsSomething(&item::ArmorSorter))
    {
      ADD_MESSAGE("You have nothing to enchant.");
      return false;
    }
  else
    {
      Reader->StartReading(this, 1000);
      return true;
    }
}

void scrollofenchantarmor::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::ArmorSorter) && !Reader->EquipsSomething(&item::ArmorSorter))
    ADD_MESSAGE("You notice you have lost anything you wished to enchant.");
  else
    {
      while(true)
	{
	  std::vector<item*> Item;
	  Reader->SelectFromPossessions(Item, "Choose an armor to enchant:", NO_MULTI_SELECT|SELECT_PAIR, &item::ArmorSorter);

	  if(!Item.empty())
	    {
	      if(!Item[0]->CanBeEnchanted())
		{
		  ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
		  break;
		}

	      if(Item[0]->HandleInPairs() && Item.size() == 1)
		{
		  ADD_MESSAGE("Only one %s will be enchanted.", Item[0]->CHAR_NAME(UNARTICLED));

		  if(!game::BoolQuestion("Still continue? [y/N]"))
		    continue;
		}

	      if(Item[0]->GetEnchantment() >= 5 && RAND() % (Item[0]->GetEnchantment() - 3))
		{
		  if(Item.size() == 1)
		    ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant it further!", Item[0]->CHAR_NAME(DEFINITE));
		  else
		    ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant them further!", Item[0]->CHAR_NAME(DEFINITE|PLURAL));

		  break;
		}

	      if(Item.size() == 1)
		ADD_MESSAGE("Your %s glows briefly blue. It feels very warm now.", Item[0]->CHAR_NAME(UNARTICLED));
	      else
		ADD_MESSAGE("Your %s glow briefly blue. They feel very warm now.", Item[0]->CHAR_NAME(PLURAL));

	      for(ushort c = 0; c < Item.size(); ++c)
		Item[c]->EditEnchantment(1);

	      break;
	    }
	  else if(game::BoolQuestion("Really cancel read? [y/N]"))
	    return;
	}

      RemoveFromSlot();
      SendToHell();
    }
}

void meleeweapon::SetEnchantment(char Amount)
{
  Enchantment = Amount;
  SignalEnchantmentChange();
}

void armor::SetEnchantment(char Amount)
{
  Enchantment = Amount;
  SignalEnchantmentChange();
}

void meleeweapon::EditEnchantment(char Amount)
{
  Enchantment += Amount;
  SignalEnchantmentChange();
}

void armor::EditEnchantment(char Amount)
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

ushort armor::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() * (10 + Enchantment) / 20000;
}

void bodypart::SignalEnchantmentChange()
{
  if(GetMaster() && !GetMaster()->IsInitializing())
    {
      GetMaster()->CalculateAttributeBonuses();
      GetMaster()->CalculateBattleInfo();
    }
}

bool itemcontainer::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
{
  if(Type == PHYSICAL_DAMAGE || Type == SOUND)
    {
      Contained->ReceiveDamage(Damager, Damage / GetDamageDivider(), Type);
      ushort SV = Max<ushort>(GetStrengthValue(), 1);

      if(IsLocked() && Damage > SV && RAND() % (100 * Damage / SV) >= 100)
	{
	  SetIsLocked(false);
	  SetLockType(DAMAGED);

	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("The %s's lock shatters to pieces.", GetNameSingular().c_str());

	  return true;
	}
      else if(Damager && Damager->IsPlayer())
	ADD_MESSAGE("THUMP!");
    }

  return false;
}

void itemcontainer::DrawContents(const character* Char)
{
  std::string Topic = "Contents of your " + GetName(UNARTICLED);
  GetContained()->DrawContents(Char, Topic, NO_SELECT);
}

void backpack::ReceiveFluidSpill(material* Liquid)
{
  if(!Liquid->IsExplosive())
    {
      ulong Amount = Liquid->GetVolume() * (30 + (RAND() % 71)) / 100;
      GetContainedMaterial()->SetWetness(Amount);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s gets wet.", CHAR_NAME(DEFINITE));
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

void meleeweapon::VirtualConstructor(bool Load)
{
  if(!Load)
    Enchantment = GetDefaultEnchantment();
}

void armor::VirtualConstructor(bool Load)
{
  if(!Load)
    Enchantment = GetDefaultEnchantment();
}

void magicalwhistle::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LastUsed;
}

void magicalwhistle::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LastUsed;
}

void magicalwhistle::VirtualConstructor(bool Load)
{
  whistle::VirtualConstructor(Load);
  LastUsed = 0;
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

ushort armor::GetInElasticityPenalty(ushort Attribute) const
{
  return Attribute * GetInElasticityPenaltyModifier() / (GetMainMaterial()->GetFlexibility() * 100);
}

uchar materialcontainer::GetFlyAmount() const
{
  return Max<uchar>(MainMaterial->GetSpoilLevel(), ContainedMaterial ? ContainedMaterial->GetSpoilLevel() : 0);
}

uchar meleeweapon::GetFlyAmount() const
{
  return Max<uchar>(MainMaterial->GetSpoilLevel(), SecondaryMaterial->GetSpoilLevel(), ContainedMaterial ? ContainedMaterial->GetSpoilLevel() : 0);
}

uchar corpse::GetFlyAmount() const
{
  uchar FlyAmount = 0;

  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    {
      bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

      if(BodyPart && FlyAmount < BodyPart->GetFlyAmount())
	FlyAmount = BodyPart->GetFlyAmount();
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

void itemcontainer::SetItemsInside(const std::vector<contentscript<item> >& ItemVector, ushort SpecialFlags)
{
  GetContained()->Clean();

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c].IsValid())
      {
	item* Item = ItemVector[c].Instantiate(SpecialFlags);

	if(Item)
	  {
	    Contained->AddItem(Item);
	    Item->SpecialGenerationHandler();
	  }
      }
}

bool head::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  std::vector<long> AV(3, 0);
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

  return AnyArmor ? Armor[femath::WeightedRand(AV)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool humanoidtorso::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  std::vector<long> AV(3, 0);
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

  return AnyArmor ? Armor[femath::WeightedRand(AV)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool arm::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  std::vector<long> AV(3, 0);
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

  return AnyArmor ? Armor[femath::WeightedRand(AV)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool groin::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  return GetMaster()->GetTorso()->DamageArmor(Damager, Damage, Type);
}

bool leg::DamageArmor(character* Damager, ushort Damage, uchar Type)
{
  std::vector<long> AV(3, 0);
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

  return AnyArmor ? Armor[femath::WeightedRand(AV)]->ReceiveDamage(Damager, Damage, Type) : false;
}

bool bodypart::CanBeEatenByAI(const character* Who) const
{
  return item::CanBeEatenByAI(Who) && !(Who->GetTeam()->GetID() == game::GetPlayer()->GetTeam()->GetID() && game::GetPlayer()->HasHadBodyPart(this));
}

bool mine::CheckPickUpEffect(character*)
{
  if(WillExplode(0))
    {
      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(0, "tried to pick up an active land mine", Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return false;
    }

  return true;
}

void itemcontainer::GenerateLeftOvers(character*)
{
  Contained->MoveItemsTo(GetSlot());
}

ushort gorovitshammer::GetOutlineColor(ushort Frame) const
{
  Frame &= 31;
  return MakeRGB16(135 + (Frame * (31 - Frame) >> 1), 0, 0);
}

ushort gorovitssickle::GetOutlineColor(ushort Frame) const
{
  Frame &= 31;
  return MakeRGB16(135 + (Frame * (31 - Frame) >> 1), 0, 0);
}

bool scrollofrepair::Read(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::BrokenSorter) && !Reader->EquipsSomething(&item::BrokenSorter))
    {
      ADD_MESSAGE("You have nothing to repair.");
      return false;
    }

  Reader->StartReading(this, 1000);
  return true;
}

void scrollofrepair::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->SortedItems(Reader, &item::BrokenSorter) && !Reader->EquipsSomething(&item::BrokenSorter))
    ADD_MESSAGE("You have lost whatever you wished to repair.");
  else
    while(true)
      {
	std::vector<item*> Item;
	Reader->SelectFromPossessions(Item, "Which item do you wish to repair?", NO_MULTI_SELECT|SELECT_PAIR, &item::BrokenSorter);

	if(!Item.empty())
	  {
	    if(Item[0]->HandleInPairs() && Item.size() == 1)
	      {
		ADD_MESSAGE("Only one %s will be enchanted.", Item[0]->CHAR_NAME(UNARTICLED));

		if(!game::BoolQuestion("Still continue? [y/N]"))
		  continue;
	      }

	    if(Item.size() == 1)
	      ADD_MESSAGE("As you read the scroll, %s glows green and fixes itself.", Item[0]->CHAR_NAME(DEFINITE));
	    else
	      ADD_MESSAGE("As you read the scroll, %s glow green and fix themselves.", Item[0]->CHAR_NAME(PLURAL));

	    for(ushort c = 0; c < Item.size(); ++c)
	      Item[c]->Fix();

	    break;
	  }
	else if(game::BoolQuestion("Really cancel read? [y/N]"))
	  return;
      }

  RemoveFromSlot();
  SendToHell();  
}

item* brokenbottle::Fix()
{
  potion* Potion = new potion(0, NO_MATERIALS);
  Potion->InitMaterials(GetMainMaterial(), 0);
  Potion->SetID(BackupID);
  Potion->SetBackupID(ID);
  DonateSlotTo(Potion);
  SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  SendToHell();
  return Potion;
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

bool encryptedscroll::Read(character*)
{
  ADD_MESSAGE("You could never hope to decipher this top secret message. It is meant for Petrus's eyes only.");
  return false;
}

long itemcontainer::GetScore() const
{
  return item::GetScore() + GetContained()->GetScore();
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

bool horn::Apply(character* Blower) 
{
  if(LastUsed == 0 || game::GetTicks() - LastUsed >= 2500)
    {
      LastUsed = game::GetTicks();
      const char* SoundDescription = Config == BRAVERY ? "loud but calming" : "frightening, almost scream-like";

      if(Blower->IsPlayer())
	ADD_MESSAGE("You produce a %s sound.", SoundDescription);
      else if(Blower->CanBeSeenByPlayer())
	ADD_MESSAGE("%s blows %s and produces a %s sound.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE), SoundDescription);
      else 
	ADD_MESSAGE("You hear a %s sound echoing everywhere.", SoundDescription);

      rect Rect;
      femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), GetPos(), 10);

      for(ushort x = Rect.X1; x <= Rect.X2; ++x)
	for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
	  {
	    character* Audience = GetNearSquare(x, y)->GetCharacter();

	    if(Audience)
	      {
		if(Config == BRAVERY && Audience->TemporaryStateIsActivated(PANIC) && Blower->GetTeam()->GetID() == Audience->GetTeam()->GetID() && Audience->GetPanicLevel() <= RAND() % 100)
		  {
		    if(Audience->IsPlayer())
		      ADD_MESSAGE("You calm down.");
		    else if(CanBeSeenByPlayer())
		      ADD_MESSAGE("%s calms down.", Audience->CHAR_NAME(DEFINITE));

		    Audience->DeActivateTemporaryState(PANIC);
		  }
		else if(Config == FEAR && !Audience->TemporaryStateIsActivated(PANIC) && Blower->GetRelation(Audience) == HOSTILE && Audience->GetPanicLevel() >= RAND() % 100)
		  Audience->BeginTemporaryState(PANIC, 500 + RAND() % 500);
	      }
	  }

    }
  else
    {
      if(Blower->IsPlayer())
	ADD_MESSAGE("You produce a mighty sound.");
      else if(Blower->CanBeSeenByPlayer())
	ADD_MESSAGE("%s blows %s and produces a mighty sound.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
      else 
	ADD_MESSAGE("You hear a horn being blown.");
    }

  game::CallForAttention(GetPos(), 30 * 30);
  Blower->EditAP(-1000);
  return true;
}

void horn::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LastUsed;
}

void horn::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LastUsed;
}

void horn::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  LastUsed = 0;
}

bool potion::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
{
  if(Type == FIRE && Damage && IsExplosive() && (Damage > 50 || !(RAND() % (100 / Damage))))
    {
      std::string DeathMsg = "killed by an explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetKillName();

      if(GetSquareUnder()->CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHAR_DESCRIPTION(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }
  else
    return item::ReceiveDamage(Damager, Damage, Type);
}

uchar corpse::GetArticleMode() const
{
  return Deceased->LeftOversAreUnique() ? DEFINITE_ARTICLE : NORMAL_ARTICLE;
}

bool neercseulb::HitEffect(character* Enemy, character* Hitter, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, BodyPartIndex, Direction, BlockedByArmour);

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

bool thunderhammer::HitEffect(character* Enemy, character* Hitter, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  bool BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s hammer shoots a lightning bolt at %s!", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

      std::string DeathMSG = "killed by " + Hitter->GetKillName();
      GetLevel()->LightningBeam(Hitter, DeathMSG, GetPos(), WHITE, BEAM_LIGHTNING, Direction, 10);
      return true;
    }
  else
     return BaseSuccess;
}

bool thunderhammer::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
{
  return Type != ELECTRICITY ? meleeweapon::ReceiveDamage(Damager, Damage, Type) : false;
}
