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
  item* Peals = new bananapeals(0, false);
  Peals->InitMaterials(GetMainMaterial());
  SetMainMaterial(0);

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    Eater->GetStackUnder()->AddItem(Peals);
  else
    Eater->GetStack()->AddItem(Peals);

  RemoveFromSlot();
  SendToHell();
}

void potion::GenerateLeftOvers(character* Eater)
{
  ChangeConsumeMaterial(0);

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    MoveTo(Eater->GetStackUnder());
  else
    MoveTo(Eater->GetStack());
}

bool scroll::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::GetSeeWholeMapCheat();
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

      if(GetAreaUnder()->IsValidPos(TryToCreate) && GetNearSquare(TryToCreate)->IsWalkable(Monster) && GetNearSquare(TryToCreate)->GetCharacter() == 0)
	{
	  GetNearLSquare(TryToCreate)->AddCharacter(Monster);

	  if(Reader->IsPlayer())
	    {
	      if(Monster->CanBeSeenByPlayer())
		ADD_MESSAGE("%s appears.", Monster->CHARNAME(INDEFINITE));
	      else
		ADD_MESSAGE("You notice no effect.");
	    }
	  else if(Reader->CanBeSeenByPlayer())
	    {
	      if(Monster->CanBeSeenByPlayer())
		ADD_MESSAGE("%s summons %s!", Reader->CHARNAME(DEFINITE), Monster->CHARNAME(INDEFINITE));
	    }
	  else if(Monster->CanBeSeenByPlayer())
	    ADD_MESSAGE("Suddenly %s appears.", Monster->CHARNAME(INDEFINITE));

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
    ADD_MESSAGE("%s disappears!", Reader->CHARNAME(DEFINITE));

  Reader->TeleportRandomly();
  RemoveFromSlot();
  SendToHell();
}

bool lump::HitEffect(character* Enemy, character*, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour && RAND() & 1)
    {
      if(Enemy->IsPlayer())
	ADD_MESSAGE("The %s touches you!", GetMainMaterial()->CHARNAME(UNARTICLED), Enemy->CHARNAME(DEFINITE));
      else if(Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("The %s touches %s.", GetMainMaterial()->CHARNAME(UNARTICLED), Enemy->CHARNAME(DEFINITE));

      return GetMainMaterial()->HitEffect(Enemy);
    }
  else
    return false;
}

bool meleeweapon::HitEffect(character* Enemy, character*, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour && GetContainedMaterial())
    {
      if(Enemy->IsPlayer())
	ADD_MESSAGE("The %s reacts with you!", GetContainedMaterial()->CHARNAME(UNARTICLED));
      else if(Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("The %s reacts with %s.", GetContainedMaterial()->CHARNAME(UNARTICLED), Enemy->CHARNAME(DEFINITE));

      return GetContainedMaterial()->HitEffect(Enemy);
    }
  else
    return false;
}

void meleeweapon::DipInto(material* Material, character* Dipper)
{
  ChangeContainedMaterial(Material);

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now covered with %s.", CHARNAME(DEFINITE), Material->CHARNAME(UNARTICLED));

  Dipper->DexterityAction(10);
}

material* lump::CreateDipMaterial()
{
  return GetMainMaterial()->Clone(GetMainMaterial()->TakeDipVolumeAway());
}

bool pickaxe::Apply(character* User)
{
  uchar Dir = game::DirectionQuestion("What direction do you want to dig? [press a direction key]", false);

  vector2d Temp = game::GetMoveVector(Dir);

  if(Dir == DIR_ERROR || !GetAreaUnder()->IsValidPos(User->GetPos() + Temp))
    return false;

  lsquare* Square = GetNearLSquare(User->GetPos() + Temp);

  if(Square->GetCharacter())
    if(User->Hit(Square->GetCharacter()))
      return true;

  if(Square->CanBeDug())
    if(Square->GetOLTerrain()->CanBeDug())
      if(Square->GetOLTerrain()->GetMainMaterial()->CanBeDug(GetMainMaterial()))
	{
	  uchar RoomNumber = Square->GetRoom();

	  if(!RoomNumber || Square->GetLevelUnder()->GetRoom(RoomNumber)->DestroyTerrain(User, Square->GetOLTerrain()))
	    {
	      User->SwitchToDig(this, User->GetPos() + Temp);
	      User->DexterityAction(10);
	      return true;
	    }
	  else
	    return false;
	}
      else
	ADD_MESSAGE("%s is too hard to dig with %s.", Square->GetOLTerrain()->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));
    else
      ADD_MESSAGE(Square->GetOLTerrain()->GetDigMessage().c_str());

  return false;
}

bool wand::Apply(character* Terrorist)
{
  if(Terrorist->IsPlayer())
    ADD_MESSAGE("%s breaks in two and then explodes!", CHARNAME(DEFINITE));
  else
    if(Terrorist->CanBeSeenByPlayer())
      ADD_MESSAGE("%s breaks %s in two. It explodes!", Terrorist->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

  RemoveFromSlot();
  SendToHell();

  std::string DeathMsg;

  if(Terrorist->IsPlayer())
    DeathMsg = "exploded himself by breaking a wand";
  else
    DeathMsg = "kamikazed by " + Terrorist->GetName(INDEFINITE);

  Terrorist->GetLevelUnder()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), 40);
  Terrorist->DexterityAction(5);
  return true;
}

bool wandofpolymorph::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a bug in the polymorph code", Direction, 5);
  SetTimesUsed(GetTimesUsed() + 1);
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
  while(true)
    {
      std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
      item* TempItem = protosystem::CreateItem(Temp, Reader->IsPlayer());

      if(TempItem)
	{
	  Reader->GetStack()->AddItem(TempItem);
	  ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CHARNAME(INDEFINITE));
	  RemoveFromSlot();
	  SendToHell();
	  return;
	}
    }
}

bool lantern::ReceiveDamage(character*, ushort Damage, uchar)
{
  if(!(RAND() % 75) && Damage > 10 + RAND() % 10)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s shatters to pieces.", CHARNAME(DEFINITE));

      brokenlantern* Lantern = new brokenlantern(0, false);
      Lantern->InitMaterials(GetMainMaterial());
      SetMainMaterial(0);
      Lantern->SignalSquarePositionChange(SquarePosition);
      GetSlot()->DonateTo(Lantern);
      SendToHell();
      return true;
    }

  return false;
}

bool potion::ReceiveDamage(character*, ushort Damage, uchar)
{
  if(!(RAND() % 75) && Damage > 10 + RAND() % 10)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s shatters to pieces.", CHARNAME(DEFINITE));

      if(GetContainedMaterial()) 
	GetLSquareUnder()->SpillFluid(5, GetContainedMaterial()->GetColor());

      item* Remains = new brokenbottle(0, false);
      Remains->InitMaterials(GetMainMaterial());
      GetSlot()->DonateTo(Remains);
      SendToHell();
      return true;
    }

  return false;
}

bool scrollofchangematerial::Read(character* Reader)
{
  if(Reader->GetStack()->GetItems() <= 1) // that 1 is the scroll itself
    {
      ADD_MESSAGE("You have nothing to change.");
      return false;
    }
  else
    {
      Reader->StartReading(this, 1000);
      return true;
    }
}

void scrollofchangematerial::FinishReading(character* Reader)
{
  if(!Reader->GetStack()->GetItems())
    ADD_MESSAGE("You notice you have lost anything you wished to alter.");
  else
    while(true)
      {
	item* Item = Reader->GetStack()->DrawContents(Reader, "What item do you wish to change?");

	if(Item)
	  {
	    if(!Item->IsMaterialChangeable())
	      {
		ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item->CHARNAME(DEFINITE));
		break;
	      }

	    std::string Temp = game::StringQuestion("What material do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);
	    material* TempMaterial = protosystem::CreateMaterial(Temp, Item->GetMainMaterial()->GetVolume());

	    if(TempMaterial)
	      {
		Item->ChangeMainMaterial(TempMaterial);
		break;
	      }
	  }
      }

  RemoveFromSlot();
  SendToHell();
}

item* brokenbottle::BetterVersion() const
{
  material* Stuff;

  if(RAND() % 5)
    Stuff = MAKE_MATERIAL(BANANAFLESH);
  else
    Stuff = MAKE_MATERIAL(OMLEURINE);

  potion* P = new potion(0, false); 
  P->InitMaterials(MAKE_MATERIAL(GLASS), Stuff); 

  return P;
}

bool wandofstriking::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a wand of striking", Direction, 15);
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

bool platemail::ReceiveDamage(character*, ushort Damage, uchar)
{
  if(Damage > GetStrengthValue() * 2 + RAND() % 11 - RAND() % 11)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s is damaged.", CHARNAME(DEFINITE));

      item* Plate = new brokenplatemail(0, false);
      Plate->InitMaterials(GetMainMaterial());
      SetMainMaterial(0);
      GetSlot()->DonateTo(Plate);
      SendToHell();
      return true;
    }

  return false;
}

bool brokenbottle::StepOnEffect(character* Stepper)
{
  if(!(RAND() % 10))
    {
      if(Stepper->ReceiveDamage(0, 1 + (RAND() & 1), PHYSICALDAMAGE, LEGS))
	{
	  if(Stepper->IsPlayer())
	    ADD_MESSAGE("Auch. You step on sharp glass splinters.");
	  else if(Stepper->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s steps on sharp glass splinters and is hurt.", Stepper->CHARNAME(DEFINITE));
	}
      else
	{
	  if(Stepper->IsPlayer())
	    ADD_MESSAGE("Some glass splinters are shattered under your feet.");
	  else if(Stepper->CanBeSeenByPlayer())
	    ADD_MESSAGE("Some glass splinters are shattered under %s's feet.", Stepper->CHARNAME(DEFINITE));
	}

      //Stepper->SetHP(Stepper->GetHP() - RAND() % 2 - 1);
      Stepper->CheckDeath("stepped on a broken bottle");
    }

  return false;
}

material* potion::CreateDipMaterial()
{
  return GetContainedMaterial()->Clone(GetContainedMaterial()->TakeDipVolumeAway());
}

void potion::DipInto(material* Material, character* Dipper)
{
  /* Add alchemy */

  ChangeContainedMaterial(Material);

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now filled with %s.", CHARNAME(DEFINITE), Material->CHARNAME(UNARTICLED));

  Dipper->DexterityAction(10);
}

ulong meleeweapon::Price() const
{
  return ulong(GetWeaponStrength() * GetWeaponStrength() * GetWeaponStrength() / (float(GetWeight()) * 1000000));
}

ulong bodyarmor::Price() const
{
  float ArmorModifier = GetStrengthValue() / 10;

  return ulong(ArmorModifier * ArmorModifier * ArmorModifier * 200);
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
	Stuff = MAKE_MATERIAL(BANANAFLESH);
      else
	Stuff = MAKE_MATERIAL(OMLEURINE);

      potion* P = new potion(0, false); 
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
	Stuff = MAKE_MATERIAL(SCHOOLFOOD);
      else
	Stuff = MAKE_MATERIAL(BANANAFLESH);

      can* P = new can(0, false); 
      P->InitMaterials(MAKE_MATERIAL(IRON), Stuff); 

      return P;
    }
  else
    return 0;
}

ushort whip::GetFormModifier() const
{
  return 75 * GetMainMaterial()->GetFlexibility();
}

bool backpack::Apply(character* Terrorist)
{
  if(GetContainedMaterial() && GetContainedMaterial()->IsExplosive())
    {
      if(Terrorist->IsPlayer())
	ADD_MESSAGE("You light your %s. It explodes!", CHARNAME(UNARTICLED));
      else if(Terrorist->CanBeSeenByPlayer())
	ADD_MESSAGE("%s lights %s. It explodes!", Terrorist->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

      RemoveFromSlot();
      SendToHell();

      std::string DeathMsg;

      if(Terrorist->IsPlayer())
	DeathMsg = "exploded himself with " + GetName(INDEFINITE);
      else
	DeathMsg = "kamikazed by " + Terrorist->GetName(INDEFINITE);

      Terrorist->DexterityAction(5);
      Terrorist->GetLevelUnder()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }
  else
    if(Terrorist->IsPlayer())
      ADD_MESSAGE("You can't apply this!");	

  return false;
}

bool holybook::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::GetSeeWholeMapCheat();
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

bool wand::ReceiveDamage(character* Damager, ushort, uchar Type)
{
  if((Type == FIRE && !(RAND() % 10)) || (Type == ENERGY && !(RAND() % 10)))
    {
      std::string DeathMsg = "explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetName(INDEFINITE);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevelUnder()->Explosion(Damager, DeathMsg, Square->GetPos(), 40);
      return true;
    }

  return false;
}

bool backpack::ReceiveDamage(character* Damager, ushort, uchar Type)
{
  if((Type == FIRE && !(RAND() % 3)) || (Type == ENERGY && RAND() % 3))
    {
      std::string DeathMsg = "explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetName(INDEFINITE);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevelUnder()->Explosion(Damager, DeathMsg, Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

void wand::AddPostFix(std::string& String) const
{
  String << " " << item::GetPostFix();

  if(TimesUsed == 1)
    String += " (used 1 time)";
  else if(TimesUsed)
    String += std::string(" (used ") + TimesUsed + " times)";
}

bool scroll::ReceiveDamage(character*, ushort, uchar Type)
{
  if(Type == FIRE && !(RAND() % 10) && GetMainMaterial()->IsFlammable())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s catches fire!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SendToHell();
      return true;
    }

  return false;
}

void wand::Beam(character* Zapper, const std::string& DeathMsg, uchar Direction, uchar Range)
{
  vector2d CurrentPos = Zapper->GetPos();

  if(Direction != YOURSELF)
    for(ushort Length = 0; Length < Range; ++Length)
      {
	if(!GetAreaUnder()->IsValidPos(CurrentPos + game::GetMoveVector(Direction)))
	  break;

	lsquare* CurrentSquare = GetNearLSquare(CurrentPos + game::GetMoveVector(Direction));

	if(!(CurrentSquare->GetOLTerrain()->IsWalkable()))
	  {
	    BeamEffect(Zapper, DeathMsg, Direction, CurrentSquare);
	    break;
	  }
	else
	  {	
	    CurrentPos += game::GetMoveVector(Direction);
	    if(BeamEffect(Zapper, DeathMsg, Direction, CurrentSquare))
	      break;

	    if(CurrentSquare->CanBeSeenByPlayer(true))
	      CurrentSquare->DrawParticles(GetBeamColor(), Direction);
	  }
      }
  else
    {
      lsquare* Where = Zapper->GetLSquareUnder();
      BeamEffect(Zapper, DeathMsg, Direction, Where);

      if(Where->CanBeSeenByPlayer(true))
	Where->DrawParticles(GetBeamColor(), Direction);
    }
}

bool wandofpolymorph::BeamEffect(character* Zapper, const std::string&, uchar, lsquare* LSquare)
{
  LSquare->PolymorphEverything(Zapper);
  return false;
}

bool wandofstriking::BeamEffect(character* Who, const std::string& DeathMsg, uchar Dir, lsquare* Where) 
{ 
  Where->StrikeEverything(Who, DeathMsg, 20, Dir); 
  return false;
}

bool holybook::ReceiveDamage(character*, ushort, uchar Type)
{
  if(Type == FIRE && RAND() & 1 && GetMainMaterial()->IsFlammable())
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s catches fire!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SendToHell();
      return true;
    }

  return false;
}

bool oillamp::Apply(character* Applier)
{
  if(Applier->IsPlayer())
    ADD_MESSAGE("You rub %s.", CHARNAME(DEFINITE));

  if(GetInhabitedByGenie())
    {
      character* Genie = new genie;
      vector2d TryToCreate;
      bool FoundPlace = false;

      /* First try to create a genie nearby (10 tries - if all of them fail then stop trying) */

      for(ushort c = 0; c < 10 && !FoundPlace; ++c)
	{	  
	  TryToCreate = Applier->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

	  if(GetAreaUnder()->IsValidPos(TryToCreate) && GetNearSquare(TryToCreate)->IsWalkable(Genie) && GetNearSquare(TryToCreate)->GetCharacter() == 0)
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
	      Genie->SetTeam(game::GetTeam(1));
	      ADD_MESSAGE("You see a puff of smoke, and %s appears. \"For centuries I have been imprisoned in this lamp. But at last you have freed me! As a reward, I will kill you.\"", Genie->CHARNAME(INDEFINITE));
	    }
	  else
	    {
	      if(Applier->IsPlayer())
		{
		  Genie->SetTeam(Applier->GetTeam());
		  ADD_MESSAGE("You see a puff of smoke, and %s appears. \"For centuries I have been imprisoned in this lamp. But at last you have freed me! I am deeply grateful. You deserve a generous reward. I may serve you for 1001 nights or grant you a wish. It's your choice.\"", Genie->CHARNAME(INDEFINITE));
		  game::DrawEverything();
		  GETKEY();

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
			      ADD_MESSAGE("%s appears from nothing and the genie flies happily away!", TempItem->CHARNAME(INDEFINITE));
			      break;
			    }
			}

		      GetLevelUnder()->RemoveCharacter(TryToCreate);
		      delete Genie;
		      return true;
		    }
		}
	    }

	    item* Sword = new meleeweapon(CURVEDTWOHANDEDSWORD);
	    ADD_MESSAGE("%s wishes for %s. Suddenly %s appears from nothing and %s wields it.", Genie->CHARNAME(DEFINITE), Sword->CHARNAME(INDEFINITE), Sword->CHARNAME(INDEFINITE), Genie->CHARNAME(DEFINITE));
	    Genie->SetMainWielded(Sword);
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
  if(Reader->GetStack()->GetItems() > 1) // that 1 is the scroll itself
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
  if(!Reader->GetStack()->SortedItems(Reader, &item::ChargeableSorter))
    ADD_MESSAGE("You have lost whatever you wished to charge.");
  else
    while(true)
      {
	item* Item = Reader->GetStack()->DrawContents(Reader, "What item do you wish to charge?", &item::ChargeableSorter);

	if(Item)
	  {
	    game::DrawEverythingNoBlit();
	    Item->ChargeFully(Reader);
	    ADD_MESSAGE("You charge %s and the scroll burns.", Item->CHARNAME(DEFINITE));
	    break;
	  }
      }

  RemoveFromSlot();
  SendToHell();
}

void banana::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << Charges;
}

void banana::Load(inputfile& SaveFile)
{
  materialcontainer::Load(SaveFile);
  SaveFile >> Charges;
}

bool banana::Zap(character*, vector2d, uchar)
{
  if(Charges)
    {
      ADD_MESSAGE("BANG! You zap %s!", CHARNAME(DEFINITE));
      --Charges;
    }
  else
    {
      ADD_MESSAGE("Click!");
    }

  return true;
}

bool bananapeals::StepOnEffect(character* Stepper)
{
  if(Stepper->HasFeet() && !(RAND() % 3))
    {
      if(Stepper->IsPlayer())
	ADD_MESSAGE("Auch. Your feet slip on %s and you fall down.", CHARNAME(INDEFINITE));
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s steps on %s and falls down.", Stepper->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

      /* Do damage against any random bodypart except legs */
      Stepper->ReceiveDamage(0, 1 + (RAND() & 1), PHYSICALDAMAGE, ALL&~LEGS);
      Stepper->CheckDeath("slipped on a banana peal.");
      Stepper->EditAP(-1000);
    }
  
  return false;
}

bool wandoffireballs::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a wand of fireballs", Direction, 200);
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

bool wandoffireballs::BeamEffect(character* Who, const std::string& DeathMsg, uchar, lsquare* Where) 
{ 
  if(!Where->GetOTerrain()->IsWalkable() || Where->GetCharacter())
    {
      Where->GetLevelUnder()->Explosion(Who, DeathMsg, Where->GetPos(), 20 + RAND() % 5 - RAND() % 5);
      return true;
    }

  return false;
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

      if(GetAreaUnder()->IsValidPos(Test))
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
	    ADD_MESSAGE("%s looks much friendlier.", ToBeTamed->CHARNAME(INDEFINITE));
	  else
	    ADD_MESSAGE("You notice no effect.");
	}
      else if(Reader->CanBeSeenByPlayer())
	{
	  if(ToBeTamed->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s seems to like %s far more.", ToBeTamed->CHARNAME(INDEFINITE), Reader->CHARNAME(DEFINITE));
	}
      else if(ToBeTamed->CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks like having fallen in love with something nearby.", ToBeTamed->CHARNAME(INDEFINITE));
    }

  RemoveFromSlot();
  SendToHell();
}

void bodypart::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << BitmapPos << ColorB << ColorC << ColorD << SpecialFlags << HP << OwnerDescription << Unique << RegenerationCounter;
}

void bodypart::Load(inputfile& SaveFile)
{
  materialcontainer::Load(SaveFile);
  SaveFile >> BitmapPos >> ColorB >> ColorC >> ColorD >> SpecialFlags  >> HP >> OwnerDescription >> Unique >> RegenerationCounter;
}

bool wandofteleportation::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a bug in the teleportation code", Direction, 5);
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

bool wandofteleportation::BeamEffect(character* Who, const std::string&, uchar, lsquare* Where) 
{ 
  Where->TeleportEverything(Who);
  return false;
}

ushort bodypart::GetStrengthValue() const
{
  if(GetMaster() && GetMainMaterial()->IsAlive())
    return ulong(GetStrengthModifier()) * GetMaster()->GetAttribute(ENDURANCE) / 1000;
  else
    return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 1000;
}

ushort head::GetTotalResistance(uchar Type) const
{
  if(GetMaster())
    {
      ushort Resistance = GetResistance(Type) + GetMaster()->GlobalResistance(Type);

      if(GetHelmet())
	Resistance += GetHelmet()->GetResistance(Type);

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

      if(GetMaster()->GetBodyArmor())
	Resistance += GetMaster()->GetBodyArmor()->GetResistance(Type) / 2;

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

      if(GetMaster()->GetBodyArmor())
	Resistance += GetMaster()->GetBodyArmor()->GetResistance(Type);

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

      if(GetMaster()->GetBodyArmor())
	Resistance += GetMaster()->GetBodyArmor()->GetResistance(Type) / 2;

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

bool torso::ReceiveDamage(character* Damager, ushort Damage, uchar Type)
{
  if(GetMaster())
    GetMaster()->GetStack()->ReceiveDamage(Damager, Damage, Type);

  return bodypart::ReceiveDamage(Damager, Damage, Type);
}

bool bodypart::ReceiveDamage(character*, ushort Damage, uchar)
{
  if(GetMaster())
    {
      ushort BHP = GetHP();

      if(GetHP() <= Damage)
	if(GetHP() == GetMaxHP() && GetHP() != 1 && GetMaster()->BodyPartVital(GetBodyPartIndex()))
	  Damage = GetHP() - 1;

      EditHP(-Damage);

      if(GetHP() <= 0)
	return true;
      else
	if(GetMaster()->IsPlayer())
	  if(GetHP() == 1 && BHP >= 2)
	    {
	      game::Beep();
	      ADD_MESSAGE("Your %s bleeds very badly.", CHARNAME(UNARTICLED));
	    }
	  else if(GetHP() < GetMaxHP() / 3 && BHP >= GetMaxHP() / 3)
	    {
	      game::Beep();
	      ADD_MESSAGE("Your %s bleeds.", CHARNAME(UNARTICLED));
	    }
    }

  return false;
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

bool mine::ReceiveDamage(character* Damager, ushort, uchar Type)
{
  if((Type == FIRE && RAND() & 1) || (Type == ENERGY && RAND() & 1) || (Type == PHYSICALDAMAGE))
    {
      std::string DeathMsg = "explosion of ";
      AddName(DeathMsg, INDEFINITE);

      if(Damager)
	DeathMsg << " caused by " << Damager->GetName(INDEFINITE);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      SendToHell();
      Square->GetLevelUnder()->Explosion(Damager, DeathMsg, Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

bool mine::StepOnEffect(character* Stepper)
{
  if(!WillExplode(Stepper))
    return false;

  if(Stepper->IsPlayer())
    ADD_MESSAGE("You hear a faint thumb. You look down. You see %s. It explodes.", CHARNAME(INDEFINITE));
  else if(Stepper->CanBeSeenByPlayer())
    ADD_MESSAGE("%s steps on %s.", Stepper->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

  lsquare* Square = GetLSquareUnder();
  RemoveFromSlot();
  SendToHell();
  Square->GetLevelUnder()->Explosion(0, "killed by a land mine", Square->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
  return true;
}

bool wandofhaste::BeamEffect(character*, const std::string&, uchar, lsquare* LSquare)
{
  character* Dude = LSquare->GetCharacter();

  if(Dude)
    {
      Dude->BeginTemporaryState(HASTE, 500 + RAND() % 1000);
      return true;
    }
  else
    return false;
}

bool wandofhaste::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a bug in the haste code", Direction, 5);
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

bool wandofslow::BeamEffect(character*, const std::string&, uchar, lsquare* LSquare)
{
  character* Dude = LSquare->GetCharacter();

  if(Dude)
    {
      Dude->BeginTemporaryState(SLOW, 500 + RAND() % 1000);
      return true;
    }
  else
    return false;
}

bool wandofslow::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a bug in the slow code", Direction, 5);
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

bool key::Apply(character* User)
{
  if(User->IsPlayer())
    {
      uchar Dir = game::DirectionQuestion("What door do you wish to lock or unlock? [press a direction key]", true);

      if(Dir == DIR_ERROR)
	return false;

      vector2d ApplyPos = User->GetPos() + game::GetMoveVector(Dir);

      if(GetAreaUnder()->IsValidPos(ApplyPos))
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
  float WeaponStrength = GetBaseUnarmedStrength();

  if(GetGauntlet())
    WeaponStrength += GetGauntlet()->GetWeaponStrength();

  return WeaponStrength * GetAttribute(ARMSTRENGTH) * GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetEffectBonus() / 5000000;
}

float arm::GetUnarmedToHitValue() const
{
  return float((GetAttribute(DEXTERITY) << 2) + GetMaster()->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetEffectBonus() * GetMaster()->GetMoveEase() / 10000;
}

long arm::GetUnarmedAPCost() const
{
  return long(float(GetMaster()->GetCWeaponSkill(UNARMED)->GetAPBonus()) * (200 - GetAttribute(DEXTERITY)) * 5 / GetMaster()->GetMoveEase());
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
  else
    APCost = 100;

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
  if(GetWielded()->IsShield(Master))
    return 0;

  float HitStrength = GetAttribute(ARMSTRENGTH);

  if(TwoHandWieldIsActive())
    HitStrength += GetPairArm()->GetAttribute(ARMSTRENGTH);

  return HitStrength * GetWielded()->GetWeaponStrength() * GetCurrentSWeaponSkill()->GetEffectBonus() * GetHumanoidMaster()->GetCWeaponSkill(GetWielded()->GetWeaponCategory())->GetEffectBonus() / 500000000;
}

float arm::GetWieldedToHitValue() const
{
  arm* PairArm = GetPairArm();
  float Weight = GetWielded()->GetWeight();

  if(Weight < 500)
    Weight = 500;

  float Bonus = float(GetHumanoidMaster()->GetCWeaponSkill(GetWielded()->GetWeaponCategory())->GetEffectBonus()) * GetCurrentSWeaponSkill()->GetEffectBonus() * GetMaster()->GetMoveEase() / (1000000 + (Weight - 500) * 400);
  float ThisToHit = ((GetAttribute(DEXTERITY) << 2) + GetMaster()->GetAttribute(PERCEPTION));

  if(PairArm)
    {
      if(!PairArm->GetWielded())
	{
	  if(GetWielded()->IsTwoHanded())
	    return Bonus * (ThisToHit + ((PairArm->GetAttribute(DEXTERITY) << 2) + GetMaster()->GetAttribute(PERCEPTION))) / 2;
	}
      else
	{
	  float Weight = GetPairArm()->GetWielded()->GetWeight();

	  if(Weight < 500)
	    Weight = 500;

	  return Bonus * ThisToHit / (1 + (Weight - 500) / 10000);
	}
    }

  return Bonus * ThisToHit;
}

long arm::GetWieldedAPCost() const
{
  arm* PairArm = GetPairArm();
  float SkillPenalty = 0.01f * GetHumanoidMaster()->GetCWeaponSkill(GetWielded()->GetWeaponCategory())->GetAPBonus() * GetCurrentSWeaponSkill()->GetAPBonus() / GetMaster()->GetMoveEase();

  if(PairArm)
    {
      if(!PairArm->GetWielded())
	{
	  if(GetWielded()->IsTwoHanded())
	    return long(SkillPenalty * ((400 - (GetAttribute(DEXTERITY) + PairArm->GetAttribute(DEXTERITY))) * 2.5f + GetWielded()->GetWeight() / 50));
	}
      else
	return long(SkillPenalty * ((200 - GetAttribute(DEXTERITY)) * 5 + (GetWielded()->GetWeight() + GetPairArm()->GetWielded()->GetWeight()) / 50));
    }

  return long(SkillPenalty * ((200 - GetAttribute(DEXTERITY)) * 5 + GetWielded()->GetWeight() / 50));
}

void head::CalculateDamage()
{
  if(!Master)
    return;

  BiteDamage = float(GetBaseBiteStrength()) * GetHumanoidMaster()->GetCWeaponSkill(BITE)->GetEffectBonus() / 500000;
}

void head::CalculateToHitValue()
{
  if(!Master)
    return;

  BiteToHitValue = float((Master->GetAttribute(AGILITY) << 2) + GetMaster()->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetEffectBonus() * GetMaster()->GetMoveEase() / 20000;
}

void head::CalculateAPCost()
{
  if(!Master)
    return;

  BiteAPCost = long(float(GetMaster()->GetCWeaponSkill(BITE)->GetAPBonus()) * (200 - GetMaster()->GetAttribute(AGILITY)) * 5 / GetMaster()->GetMoveEase());

  if(BiteAPCost < 100)
    BiteAPCost = 100;
}

void leg::CalculateDamage()
{
  if(!Master)
    return;

  float WeaponStrength = GetBaseKickStrength();

  if(GetBoot())
    WeaponStrength += GetBoot()->GetWeaponStrength();

  KickDamage = WeaponStrength * GetAttribute(LEGSTRENGTH) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetEffectBonus() / 5000000;
}

void leg::CalculateToHitValue()
{
  if(!Master)
    return;

  KickToHitValue = float((GetAttribute(AGILITY) << 2) + GetMaster()->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetEffectBonus() * GetMaster()->GetMoveEase() / 20000;
}

void leg::CalculateAPCost()
{
  if(!Master)
    return;

  KickAPCost = long(float(GetMaster()->GetCWeaponSkill(KICK)->GetAPBonus()) * (200 - GetAttribute(AGILITY)) * 10 / GetMaster()->GetMoveEase());

  if(KickAPCost < 100)
    KickAPCost = 100;
}

humanoid* bodypart::GetHumanoidMaster() const
{
  return static_cast<humanoid*>(Master);
}

ushort belt::GetFormModifier() const
{
  return 50 * GetMainMaterial()->GetFlexibility();
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

bool corpse::Consume(character* Eater, long Amount)
{
  return GetDeceased()->GetTorso()->Consume(Eater, Amount);
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
  return sqrt(float(GetFormModifier()) * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() * GetWeight());
}

bool corpse::IsBadFoodForAI(character* Eater) const
{
  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c) && GetDeceased()->GetBodyPart(c)->IsBadFoodForAI(Eater))
      return true;

  return false;
}

ushort corpse::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() / 1000;
}

corpse::~corpse()
{
  if(GetDeceased())
    GetDeceased()->SendToHell();
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
  return GetDeceased()->GetBloodColor();
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
  UpdatePictures();
}

void bodypart::Regenerate()
{
  if(HP == MaxHP)
    return;

  if(MainMaterial->IsAlive())
    {
      ulong RegenerationBonus = ulong(MaxHP) * Master->GetAttribute(ENDURANCE);

      if(Master->GetAction() && GetMaster()->GetAction()->GetRestRegenerationBonus())
	RegenerationBonus *= Master->GetSquareUnder()->GetRestModifier();

      RegenerationCounter += RegenerationBonus;
      bool Increased = false;

      while(RegenerationCounter > 100000)
	{
	  if(HP < MaxHP)
	    {
	      ++HP;
	      Master->EditExperience(ENDURANCE, 100);
	      Increased = true;
	    }

	  RegenerationCounter -= 100000;
	}

      if(Increased)
	Master->CalculateHP();
    }
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
  if(GetHelmet())
    GetHelmet()->SendToHell();

  if(GetAmulet())
    GetAmulet()->SendToHell();
}

humanoidtorso::~humanoidtorso()
{
  if(GetBodyArmor())
    GetBodyArmor()->SendToHell();

  if(GetCloak())
    GetCloak()->SendToHell();

  if(GetBelt())
    GetBelt()->SendToHell();
}

arm::~arm()
{
  if(GetWielded())
    GetWielded()->SendToHell();

  if(GetGauntlet())
    GetGauntlet()->SendToHell();

  if(GetRing())
    GetRing()->SendToHell();
}

leg::~leg()
{
  if(GetBoot())
    GetBoot()->SendToHell();
}

long corpse::Score() const
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

ulong corpse::Price() const
{
  ulong Price = 0;

  for(ushort c = 0; c < GetDeceased()->GetBodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      Price += GetDeceased()->GetBodyPart(c)->GetPrice();

  return Price;
}

item* corpse::PrepareForConsuming(character*)
{
  for(ushort c = GetDeceased()->GetBodyParts() - 1; c != 0; --c)
    {
      bodypart* BPart = GetDeceased()->GetBodyPart(c);

      if(BPart)
	{
	  GetDeceased()->SevereBodyPart(c);
	  GetSlot()->AddFriendItem(BPart);
	  return BPart;
	}
    }

  return this;
}

bool wandoflocking::BeamEffect(character* Who, const std::string&, uchar, lsquare* Where) 
{ 
  return Where->LockEverything(Who); 
}

bool wandoflocking::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a bug in the wand locking code", Direction, 10);
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
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
  SaveFile << SecondaryMaterial;
  SaveFile << ContainedMaterial;
}

void meleeweapon::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  LoadMaterial(SaveFile, SecondaryMaterial);
  LoadMaterial(SaveFile, ContainedMaterial);
}

material* materialcontainer::GetMaterial(ushort Index) const
{
  switch(Index)
    {
    case 0: return MainMaterial;
    case 1: return ContainedMaterial;
    default: return 0;
    }
}

material* meleeweapon::GetMaterial(ushort Index) const
{
  switch(Index)
    {
    case 0: return MainMaterial;
    case 1: return SecondaryMaterial;
    case 2: return ContainedMaterial;
    default: return 0;
    }
}

material* corpse::GetMaterial(ushort Index) const
{
  switch(Index)
    {
    case 0: return GetDeceased()->GetTorso()->GetMainMaterial();
    case 1: return GetDeceased()->GetTorso()->GetContainedMaterial();
    default: return 0;
    }
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

ushort bodypart::GetMaterialColorA(ushort Frame) const
{
  if(GetMainMaterial())
    return GetMainMaterial()->GetSkinColor(Frame);
  else
    return 0;
}

ushort banana::GetMaterialColorA(ushort Frame) const
{
  ushort Color = object::GetMaterialColorA(Frame);

  if(!Frame)
    return Color;
  else
    return MakeRGB(GetRed(Color) * Frame / 20, GetGreen(Color) * Frame / 20, GetBlue(Color) * Frame / 20);
}

bool wandofresurrection::BeamEffect(character* Zapper, const std::string&, uchar, lsquare* LSquare)
{
  return LSquare->RaiseTheDead(Zapper);
}

bool wandofresurrection::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a wand of resurrection", Direction, 15);
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

bool corpse::RaiseTheDead(character* Summoner)
{
  RemoveFromSlot();

  if(Summoner->IsPlayer())
    game::DoEvilDeed(50);

  GetLSquareUnder()->AddCharacter(GetDeceased());
  GetDeceased()->SetHasBe(true);
  GetDeceased()->CompleteRiseFromTheDead();
  Deceased = 0;
  SendToHell();
  return true;
}

void banana::VirtualConstructor(bool Load)
{
  materialcontainer::VirtualConstructor(Load);
  SetAnimationFrames(20);
  SetCharges(6);
}

void lantern::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  SetSquarePosition(CENTER);
}

void wand::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  SetTimesUsed(0);
}

void wandofpolymorph::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);

  if(!Load)
    SetCharges(2 + RAND() % 5);
}

void wandofstriking::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);
  
  if(!Load)
    SetCharges(2 + RAND() % 5);
}

void oillamp::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  
  if(!Load)
    SetInhabitedByGenie(RAND() & 1);
}

void wandoffireballs::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);
  
  if(!Load)
    SetCharges(1 + RAND() % 3);
}

void wandofteleportation::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);
  
  if(!Load)
    SetCharges(2 + RAND() % 5);
}

void mine::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  
  if(!Load)
    {
      Team = MONSTER_TEAM;
      Active = false;
    }
}

void wandofhaste::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);
  
  if(!Load)
    SetCharges(2 + RAND() % 5);
}

void wandofslow::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);
  
  if(!Load)
    SetCharges(2 + RAND() % 5);
}

void key::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  
  if(!Load)
    SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
}

void bodypart::VirtualConstructor(bool Load)
{
  materialcontainer::VirtualConstructor(Load);
  SetUnique(false);
  SetRegenerationCounter(0);
  SetMaster(0);
}

void head::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);
  HelmetSlot.Init(this, HELMETINDEX);
  AmuletSlot.Init(this, AMULETINDEX);
}

void humanoidtorso::VirtualConstructor(bool Load)
{
  torso::VirtualConstructor(Load);
  BodyArmorSlot.Init(this, BODYARMORINDEX);
  CloakSlot.Init(this, CLOAKINDEX);
  BeltSlot.Init(this, BELTINDEX);
}

void arm::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);

  if(!Load)
    StrengthExperience = DexterityExperience = 0;
}

void rightarm::VirtualConstructor(bool Load)
{
  arm::VirtualConstructor(Load);
  WieldedSlot.Init(this, RIGHTWIELDEDINDEX);
  GauntletSlot.Init(this, RIGHTGAUNTLETINDEX);
  RingSlot.Init(this, RIGHTRINGINDEX);
}

void leftarm::VirtualConstructor(bool Load)
{
  arm::VirtualConstructor(Load);
  WieldedSlot.Init(this, LEFTWIELDEDINDEX);
  GauntletSlot.Init(this, LEFTGAUNTLETINDEX);
  RingSlot.Init(this, LEFTRINGINDEX);
}

void leg::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);

  if(!Load)
    StrengthExperience = AgilityExperience = 0;
}

void rightleg::VirtualConstructor(bool Load)
{
  leg::VirtualConstructor(Load);
  BootSlot.Init(this, RIGHTBOOTINDEX);
}

void leftleg::VirtualConstructor(bool Load)
{
  leg::VirtualConstructor(Load);
  BootSlot.Init(this, LEFTBOOTINDEX);
}

void wandoflocking::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);
  
  if(!Load)
    SetCharges(2 + RAND() % 5);
}

void wandofresurrection::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);
  
  if(!Load)
    SetCharges(1 + (RAND() & 1));
}

const std::string& platemail::GetNameSingular() const
{
  /* Fast, but really unelegant... */

  if(GetMainMaterial() && GetMainMaterial()->GetFlexibility() > 5)
    {
      static std::string Armor = "armor";
      return Armor;
    }
  else
    {
      static std::string PlateMail = "plate mail";
      return PlateMail;
    }
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
    ADD_MESSAGE("%s blows to his whistle and produces an odd sound.", Whistler->CHARNAME(DEFINITE));
  else 
    ADD_MESSAGE("You hear a whistle playing.");
}

void magicalwhistle::BlowEffect(character* Whistler)
{
  const std::list<character*>& Member = Whistler->GetTeam()->GetMember();

  for(std::list<character*>::const_iterator i = Member.begin(); i != Member.end(); ++i)
    if(Whistler != *i)
      (*i)->TeleportNear(Whistler);
}

void chest::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  Contained = new stack(0, this, HIDDEN);

  if(!Load)
    {
      StorageVolume = 1000;
      SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
      SetIsLocked(RAND() % 3 != 0);
      uchar ItemNumber = RAND() % 5;

      for(ushort c = 0; c < ItemNumber; ++c)
	{
	  item* NewItem = protosystem::BalancedCreateItem();

	  if(NewItem->CanBeGeneratedInContainer() && FitsIn(NewItem))
	    GetContained()->AddItem(NewItem);
	  else
	    delete NewItem;
	}
    }
}

bool chest::TryKey(item* Key, character* Applier)
{
  if(Key->CanOpenLockType(GetLockType()))
    {
      if(IsLocked())
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You unlock %s.", CHARDESCRIPTION(DEFINITE));
	  else if(Applier->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s unlocks %s.", Applier->CHARNAME(DEFINITE), CHARDESCRIPTION(DEFINITE));
	}
      else
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You lock %s.", CHARDESCRIPTION(DEFINITE));
	  else if(Applier->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s locks %s.", Applier->CHARNAME(DEFINITE), CHARDESCRIPTION(DEFINITE));
	}

      SetIsLocked(!IsLocked());
    }
  else
    {
      if(Applier->IsPlayer())
	ADD_MESSAGE("%s doesn't fit in the lock.", Key->CHARNAME(DEFINITE));
      else if(Applier->CanBeSeenByPlayer())
	ADD_MESSAGE("%s tries to fit %s in the lock, but fails.", Applier->CHARNAME(DEFINITE), Key->CHARNAME(DEFINITE));
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
  bool Edited = false;

  if(GetMaster()->CheckForAttributeIncrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels stronger!", CHARNAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks stronger.", GetMaster()->CHARNAME(DEFINITE));

      Edited = true;
    }
  else if(GetMaster()->CheckForAttributeDecrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels weaker.", CHARNAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks weaker.", GetMaster()->CHARNAME(DEFINITE));

      Edited = true;
    }

  if(GetMaster()->CheckForAttributeIncrease(Dexterity, DexterityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels quite dextrous.", CHARNAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks quite dextrous.", GetMaster()->CHARNAME(DEFINITE));

      Edited = true;
    }
  else if(GetMaster()->CheckForAttributeDecrease(Dexterity, DexterityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels clumsy.", CHARNAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks clumsy.", GetMaster()->CHARNAME(DEFINITE));

      Edited = true;
    }

  return Edited;
}

bool leg::ApplyExperience()
{
  bool Edited = false;

  if(GetMaster()->CheckForAttributeIncrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels stronger!", CHARNAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks stronger.", GetMaster()->CHARNAME(DEFINITE));

      GetMaster()->CalculateBurdenState();
      Edited = true;
    }
  else if(GetMaster()->CheckForAttributeDecrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels weaker.", CHARNAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks weaker.", GetMaster()->CHARNAME(DEFINITE));

      GetMaster()->CalculateBurdenState();
      Edited = true;
    }

  if(GetMaster()->CheckForAttributeIncrease(Agility, AgilityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels very agile!", CHARNAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks very agile.", GetMaster()->CHARNAME(DEFINITE));

      Edited = true;
    }
  else if(GetMaster()->CheckForAttributeDecrease(Agility, AgilityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels slower.", CHARNAME(UNARTICLED));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks sluggish.", GetMaster()->CHARNAME(DEFINITE));

      Edited = true;
    }

  return Edited;
}

void arm::Hit(character* Enemy)
{
  switch(Enemy->TakeHit(GetMaster(), GetWielded() ? GetWielded() : GetGauntlet(), GetDamage(), GetToHitValue(), RAND() % 26 - RAND() % 26, GetWielded() ? WEAPONATTACK : UNARMEDATTACK, !(RAND() % GetMaster()->GetCriticalModifier())))
    {
    case HASHIT:
    case HASBLOCKED:
    case HASDIED:
    case DIDNODAMAGE:
      EditExperience(ARMSTRENGTH, 50);
    case HASDODGED:
      EditExperience(DEXTERITY, 25);
    }
}

ushort arm::GetAttribute(ushort Identifier) const
{
  if(Identifier == ARMSTRENGTH)
    {
      if(GetMainMaterial()->IsAlive())
	return Strength;
      else
	return GetMainMaterial()->GetStrengthValue();
    }
  else if(Identifier == DEXTERITY)
    {
      if(GetMainMaterial()->IsAlive())
	return Dexterity;
      else
	return GetMainMaterial()->GetFlexibility();
    }
  else
    {
      ABORT("Illegal arm attribute %d request!", Identifier);
      return 0xACCA;
    }
}

bool arm::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier == ARMSTRENGTH)
    return GetMaster()->RawEditAttribute(Strength, Value);
  else if(Identifier == DEXTERITY)
    return GetMaster()->RawEditAttribute(Dexterity, Value);
  else
    {
      ABORT("Illegal arm attribute %d edit request!", Identifier);
      return false;
    }
}

void arm::EditExperience(ushort Identifier, long Value)
{
  if(Identifier == ARMSTRENGTH)
    StrengthExperience += Value;
  else if(Identifier == DEXTERITY)
    DexterityExperience += Value;
  else
    ABORT("Illegal arm attribute %d experience edit request!", Identifier);
}

ushort leg::GetAttribute(ushort Identifier) const
{
  if(Identifier == LEGSTRENGTH)
    {
      if(GetMainMaterial()->IsAlive())
	return Strength;
      else
	return GetMainMaterial()->GetStrengthValue();
    }
  else if(Identifier == AGILITY)
    {
      if(GetMainMaterial()->IsAlive())
	return Agility;
      else
	return GetMainMaterial()->GetFlexibility();
    }
  else
    {
      ABORT("Illegal leg attribute %d request!", Identifier);
      return 0xECCE;
    }
}

bool leg::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier == LEGSTRENGTH)
    return GetMaster()->RawEditAttribute(Strength, Value);
  else if(Identifier == AGILITY)
    return GetMaster()->RawEditAttribute(Agility, Value);
  else
    {
      ABORT("Illegal leg attribute %d edit request!", Identifier);
      return false;
    }
}

void leg::EditExperience(ushort Identifier, long Value)
{
  if(Identifier == LEGSTRENGTH)
    StrengthExperience += Value;
  else if(Identifier == AGILITY)
    AgilityExperience += Value;
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

/* Returns true if chest opens fine else false */

bool chest::Open(character* Opener)
{
  if(IsLocked())
    {
      ADD_MESSAGE("The chest seems to be locked.");
      return false;
    }

  std::string Question = "Do you want to (t)ake something from or (p)ut something in this chest? [t,p]";

  switch(game::KeyQuestion(Question, KEYESC, 3, 't', 'p', KEYESC))
    {
    case 't':
      return TakeSomethingFrom(Opener);
    case 'p':
      return PutSomethingIn(Opener);
    default:
      return false;
    }
}

bool chest::TakeSomethingFrom(character* Opener)
{
  if(GetContained()->GetItems() == 0)
    {
      ADD_MESSAGE("There is nothing in %s.", CHARNAME(DEFINITE));
      return false;
    }

  item* ToBeTaken = GetContained()->DrawContents(Opener, "What do you want take?");
  uchar RoomNumber = GetLSquareUnder()->GetRoom();

  if(ToBeTaken && (!RoomNumber || GetLevelUnder()->GetRoom(RoomNumber)->PickupItem(Opener,this)))
    {
      ToBeTaken->MoveTo(Opener->GetStack());
      Opener->DexterityAction(Opener->OpenMultiplier() * 5);
      return true;
    }
  else
    return false;
}

bool chest::PutSomethingIn(character* Opener)
{
  if(Opener->GetStack()->GetItems() == 0)
    {
      ADD_MESSAGE("You have nothing to put in %s.", CHARNAME(DEFINITE));
      return false;
    }

  std::string Message = "What do you want to put in " + GetName(DEFINITE) + "?";
  item* ToBePut = Opener->GetStack()->DrawContents(Opener, Message);

  if(ToBePut)
    {
      uchar RoomNumber = GetLSquareUnder()->GetRoom();

      if((RoomNumber == 0 || GetLevelUnder()->GetRoom(RoomNumber)->DropItem(Opener, this)))
	{
	  if(FitsIn(ToBePut))
	    {
	      ToBePut->MoveTo(GetContained());
	      Opener->DexterityAction(Opener->OpenMultiplier() * 5);
	      return true;
	    }
	  else
	    {
	      ADD_MESSAGE("%s doesn't fit in %s.", ToBePut->CHARNAME(DEFINITE), CHARNAME(DEFINITE));
	      return false;
	    }
	}
    }

  return false;
}

void chest::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  GetContained()->Save(SaveFile);
  SaveFile << StorageVolume << LockType << Locked;
}

void chest::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  GetContained()->Load(SaveFile);
  SaveFile >> StorageVolume >> LockType >> Locked;
}

bool chest::Polymorph(stack* CurrentStack)
{
  GetContained()->MoveAll(CurrentStack);
  item::Polymorph(CurrentStack);
  return true;
}

bool chest::FitsIn(item* ToBePut) const
{
  return GetContained()->GetVolume() + ToBePut->GetVolume() <= GetStorageVolume();
}

chest::~chest()
{
  delete Contained;
}

const std::string& key::GetAdjective() const
{
  return game::GetLockDescription(LockType);
}

void head::InitSpecialAttributes()
{
  SetBaseBiteStrength(GetMaster()->GetBaseBiteStrength());
}

void arm::InitSpecialAttributes()
{
  SetStrength(GetMaster()->GetDefaultArmStrength() * (100 + GetMaster()->GetAttributeBonus()) / 100);
  SetDexterity(GetMaster()->GetDefaultDexterity() * (100 + GetMaster()->GetAttributeBonus()) / 100);
  SetBaseUnarmedStrength(GetMaster()->GetBaseUnarmedStrength());
}

void leg::InitSpecialAttributes()
{
  SetStrength(GetMaster()->GetDefaultLegStrength() * (100 + GetMaster()->GetAttributeBonus()) / 100);
  SetAgility(GetMaster()->GetDefaultAgility() * (100 + GetMaster()->GetAttributeBonus()) / 100);
  SetBaseKickStrength(GetMaster()->GetBaseKickStrength());
}

std::string corpse::GetConsumeVerb() const
{
  return GetDeceased()->GetTorso()->GetConsumeVerb();
}

/* Victim is of course the stuck person, Bodypart is the index of the bodypart that the trap is stuck to and the last vector2d is just a direction vector that may - or may not - be used in the future. This function returns true if the character manages to escape */
bool beartrap::TryToUnstuck(character* Victim, ushort BodyPart, vector2d)
{
  if(!(RAND() % 5))
    {
      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to hurt your %s even more.", Victim->GetBodyPart(BodyPart)->CHARNAME(UNARTICLED));
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s hurts %s %s more with %s.", Victim->CHARNAME(DEFINITE), Victim->GetPossessivePronoun().c_str(), Victim->GetBodyPart(BodyPart)->CHARNAME(DEFINITE), CHARNAME(DEFINITE));

      Victim->ReceiveBodyPartDamage(0, RAND() % 3 + 1, PHYSICALDAMAGE, BodyPart);
      std::string DeathMessage = "died while trying to escape from " + GetName(DEFINITE) + ".";
      Victim->CheckDeath(DeathMessage);
      return false;
    }
  if(!(RAND() % 3))
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONEINDEX);

      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to free yourself from %s.", CHARNAME(DEFINITE));
      else if(Victim->CanBeSeenByPlayer())
	{
	  std::string msg = Victim->GetName(DEFINITE) + " manages to free " + Victim->GetPersonalPronoun() + "self from " + GetName(DEFINITE) + ".";
	  ADD_MESSAGE("%s", msg.c_str());
	}

      return true;
    }
  if(Victim->IsPlayer())
    ADD_MESSAGE("You are unable to escape from %s.", CHARNAME(DEFINITE));
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

void beartrap::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);

  if(!Load)
    {
      Active = false;
      Team = MONSTER_TEAM;
    }
}

bool beartrap::StepOnEffect(character* Stepper)
{
  if(IsActive())
    {
      ushort StepperBodyPart = Stepper->GetRandomStepperBodyPart();

      if(StepperBodyPart == NONEINDEX)
	return false;

      Stepper->SetStuckTo(this);
      Stepper->SetStuckToBodyPart(StepperBodyPart);
      if(Stepper->IsPlayer())
	ADD_MESSAGE("You step in %s and it traps your %s.", CHARNAME(INDEFINITE), Stepper->GetBodyPart(Stepper->GetStuckToBodyPart())->CHARNAME(UNARTICLED));
      else if(Stepper->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is trapped in %s.", Stepper->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

      Stepper->ReceiveBodyPartDamage(0, RAND() % 3 + 1, PHYSICALDAMAGE, Stepper->GetStuckToBodyPart());
      Stepper->CheckDeath("died stepping to " + GetName(INDEFINITE) + ".");
      SetIsActive(false);
    }
  else
    {
      if(Stepper->IsPlayer())
	ADD_MESSAGE("You step on %s but luckily it isn't active.", CHARNAME(INDEFINITE));
      /* My English seems to be a bit rusty. It might be good to choose some other word than "active" */
    }

  return false;
}

bool beartrap::CheckPickUpEffect(character* Picker)
{
  if(Picker->GetStuckTo() == this)
    {
      if(Picker->IsPlayer())
	ADD_MESSAGE("%s is tightly stuck to %s.", CHARNAME(DEFINITE), Picker->GetBodyPart(Picker->GetStuckToBodyPart())->CHARNAME(UNARTICLED));
      return false;
    }

  SetIsActive(false); /* Just in case something wierd is going on */
  return true;
}

bool wandofdoorcreation::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  vector2d Pos[3];

  Pos[0] = game::GetMoveVector(Direction);

  switch(Direction)
    {
    case 0:
      Pos[1] = vector2d(-1, 0);
      Pos[2] = vector2d(0, -1);
      break;
    case 1:
      Pos[1] = vector2d(-1, -1);
      Pos[2] = vector2d(1, -1);
      break;
    case 2:
      Pos[1] = vector2d(0, -1);
      Pos[2] = vector2d(1, 0);
      break;
    case 3:
      Pos[1] = vector2d(-1, -1);
      Pos[2] = vector2d(-1, 1);
      break;
    case 4:
      Pos[1] = vector2d(1, -1);
      Pos[2] = vector2d(1, 1);
      break;
    case 5:
      Pos[1] = vector2d(-1, 0);
      Pos[2] = vector2d(0, 1);
      break;
    case 6:
      Pos[1] = vector2d(-1, 1);
      Pos[2] = vector2d(1, 1);
      break;
    case 7:
      Pos[1] = vector2d(0, 1);
      Pos[2] = vector2d(1, 0);
      break;
    case 8:
      ADD_MESSAGE("You suddenly feel like a door.");
      return false;
    }

  bool Succeeded = false;

  for(ushort c = 0; c < 3; ++c)
    if(GetAreaUnder()->IsValidPos(Zapper->GetPos() + Pos[c]))
      {
	lsquare* Square = GetNearLSquare(Zapper->GetPos() + Pos[c]);

	if(Square->GetOLTerrain()->IsSafeToDestroy() && !Square->GetCharacter())
	  {
	    door* Door = new door(0, false);
	    Door->InitMaterials(MAKE_MATERIAL(IRON));
	    Door->Lock();
	    Square->ChangeOLTerrainAndUpdateLights(Door);
	    Succeeded = true;
	  }
      }

  if(!Succeeded)
    {
      ADD_MESSAGE("The spell is wasted.");
      return true;
    }

  SetTimesUsed(GetTimesUsed() + 1);
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
  return true;
}

void wandofdoorcreation::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);
  
  if(!Load)
    SetCharges(2 + RAND() % 5);
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
  return Picker->GetStuckTo() != (item*)this;
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
    MoveTo(Eater->GetStackUnder());
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

void wandofinvisibility::VirtualConstructor(bool Load)
{
  wand::VirtualConstructor(Load);

  if(!Load)
    SetCharges(2 + RAND() % 5);
}

bool wandofinvisibility::Zap(character* Zapper, vector2d, uchar Direction)
{
  if(GetCharges() <= GetTimesUsed())
    {
      ADD_MESSAGE("Nothing happens.");
      return true;
    }

  Beam(Zapper, "killed by a bug in the invisibility code", Direction, 10);
  SetTimesUsed(GetTimesUsed() + 1);
  return true;
}

bool wandofinvisibility::BeamEffect(character*, const std::string&, uchar, lsquare* Where) 
{
  if(Where->GetCharacter())
    Where->GetCharacter()->BeginTemporaryState(INVISIBLE, 1000 + RAND() % 1001);

  return false;
}

uchar lantern::GetSpecialFlags(ushort) const
{
  switch(SquarePosition)
    {
    case LEFT:
      return ROTATE;
    case UP:
      return FLIP;
    case RIGHT:
      return ROTATE|MIRROR;
    default:
      return 0;
    }
}

void bodypart::AddPostFix(std::string& String) const
{
  if(OwnerDescription.length())
    String << " " << OwnerDescription;
}

bool stethoscope::Apply(character* Doctor) 
{
  if(!Doctor->IsPlayer())
    ABORT("Doctor is not here, man, but these pills taste just as good anyway.");

  uchar Dir = game::DirectionQuestion("What do you want to inspect? [press a direction key]", false,true);  
  if(Dir == DIR_ERROR)
    return false;
  Doctor->DexterityAction(2);
  return ListenTo(GetNearLSquare(GetPos() + game::GetMoveVector(Dir)), Doctor);
} 

/* Returns true if successful else false */
bool stethoscope::ListenTo(lsquare* Square, character* Listener)
{
  if(!Listener->CanUseStethoscope(true))
    return false;

  character* Char = Square->GetCharacter();
  if(!Char)
    {
      if(Listener->IsPlayer())
	ADD_MESSAGE("There's no-one here.");
      return false;
    }


  Char->DisplayStethoscopeInfo(Listener);
  return true;
}

void chest::CalculateVolumeAndWeight()
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
    if(GetEquipment(c) && GetEquipment(c)->GetEmitation() > Emitation)
      Emitation = GetEquipment(c)->GetEmitation();
}

void bodypart::CalculateMaxHP()
{
  MaxHP = 0;

  if(GetMaster())
    {
      if(GetMainMaterial()->IsAlive())
	MaxHP = GetBodyPartVolume() * GetMaster()->GetAttribute(ENDURANCE) / 10000;
      else
	MaxHP = GetBodyPartVolume() * GetMainMaterial()->GetStrengthValue() / 10000;

      if(MaxHP < 1)
	MaxHP = 1;
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
	    Entry << " (in both hands)";
	}
      else
	Entry << "melee attack";

      Entry.resize(50, ' ');
      Entry << GetMinDamage() << '-' << GetMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetToHitValue());
      Entry.resize(70, ' ');
      Entry << GetAPCost();
      List.AddEntry(Entry, LIGHTGRAY);
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
      Entry << GetBlockStrength();
      List.AddEntry(Entry, LIGHTGRAY);
    }
}

void arm::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(GetMaster() && !GetMaster()->IsInitializing())
    CalculateAttackInfo();
}

void bodypart::CalculateAttackInfo()
{
  CalculateDamage();
  CalculateToHitValue();
  CalculateAPCost();
}

bool flamingsword::HitEffect(character* Enemy, character* Hitter, uchar BodyPartIndex, uchar Direction, bool BlockedByArmour)
{
  if(RAND() & 1)
    {
      ADD_MESSAGE("BURN BURN BURN! FIX FIX FIX!");
      return Enemy->ReceiveBodyPartDamage(Hitter, 2 + (RAND() & 1), FIRE, BodyPartIndex, Direction);
    }
  else
    return false;
}

void flamingsword::VirtualConstructor(bool Load)
{
  meleeweapon::VirtualConstructor(Load);
  SetAnimationFrames(16);
}

bool arm::TwoHandWieldIsActive() const
{
  if(GetWielded()->IsTwoHanded())
    {
      arm* PairArm = GetPairArm();
      return PairArm && !PairArm->GetWielded();
    }
  else
    return false;
}

float bodypart::GetTimeToDie(ushort Damage, float ToHitValue, bool AttackIsBlockable, bool UseMaxHP) const
{
  float Durability;
  short TrueDamage = Damage - GetTotalResistance(PHYSICALDAMAGE);
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

      Durability = HP / (AverageDamage * GetRoughChanceToHit(ToHitValue));

      if(Durability < 1)
	Durability = 1;

      if(Durability > 1000)
	Durability = 1000;
    }
  else
    Durability = 1000;

  return Durability;
}

float bodypart::GetRoughChanceToHit(float ToHitValue) const
{
  return GLOBAL_WEAK_BODYPART_HIT_MODIFIER * ToHitValue * GetBodyPartVolume() / ((Master->GetDodgeValue() / ToHitValue + 1) * Master->GetDodgeValue() * Master->GetBodyVolume() * 100);
}

float torso::GetRoughChanceToHit(float ToHitValue) const
{
  return 1 / (Master->GetDodgeValue() / ToHitValue + 1);
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
  if(!(RAND() % 5))
    {
      if(Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("A burst of %s Mjolak's unholy energy fries %s.", Hitter->CHARPOSSESSIVEPRONOUN, Enemy->CHARNAME(DEFINITE));

      return Enemy->ReceiveBodyPartDamage(Hitter, 2 + (RAND() & 1), ENERGY, BodyPartIndex, Direction);
    }
  else
    return false;
}

bool vermis::HitEffect(character* Enemy, character* Hitter, uchar, uchar, bool)
{
  if(RAND() & 1)
    {
      if(Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s Vermis sends %s on a sudden journey.", Hitter->CHARPOSSESSIVEPRONOUN, Enemy->CHARNAME(DEFINITE));
      Enemy->TeleportRandomly();
      return true;
    }
  else
    return false;
}

bool turox::HitEffect(character* Enemy, character* Hitter, uchar, uchar, bool)
{
  if(!(RAND() % 5))
    {
      if(Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s smashes %s with the full force of Turox.", Hitter->CHARPOSSESSIVEPRONOUN, Enemy->CHARNAME(DEFINITE));
      square* Square = Enemy->GetSquareUnder();
      std::string DeathMSG = "Killed by " + Enemy->GetName(DEFINITE); 
      Enemy->GetLevelUnder()->Explosion(Hitter, DeathMSG, Square->GetPos(), 20);
      return true;
    }
  else
    return false;
}

bool whipofcalamus::HitEffect(character* Enemy, character* Hitter, uchar, uchar, bool)
{
  if(CalamusHelps(Enemy, Hitter))
    {
      if(Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s whip asks for the help of Calamus as it steals %s %s.", Hitter->CHARPOSSESSIVEPRONOUN, Enemy->CHARPOSSESSIVEPRONOUN, Enemy->GetMainWielded()->CHARNAME(UNARTICLED));
      if(Hitter->IsPlayer())
	game::GetGod(10)->AdjustRelation(10);
      Enemy->GetMainWielded()->MoveTo(Hitter->GetStackUnder());
      return true;
    }
  else
    return false;
}

void bodypart::RandomizePosition()
{
  for(ushort c = 0; c < SpecialFlags.size(); ++c)
    SpecialFlags[c] |= 1 + RAND() % 7;

  UpdatePictures();
}

void bodypart::ResetPosition()
{
  for(ushort c = 0; c < SpecialFlags.size(); ++c)
    SpecialFlags[c] &= ~0x7;

  UpdatePictures();
}

bool chest::ContentsCanBeSeenBy(const character* Viewer) const
{
  return GetMainMaterial()->IsTransparent() && CanBeSeenBy(Viewer);
}

bool mine::CanBeSeenBy(const character* Viewer) const 
{ 
  return ((Viewer->GetTeam()->GetID() == Team) || !IsActive()) && materialcontainer::CanBeSeenBy(Viewer); 
}

bool beartrap::CanBeSeenBy(const character* Viewer) const 
{ 
  return ((Viewer->GetTeam()->GetID() == Team) || !IsActive()) && item::CanBeSeenBy(Viewer); 
}

bool mine::Apply(character* User)
{
  Team = User->GetTeam()->GetID();
  if(IsActive())
    SetIsActive(false);
  else
    SetIsActive(true);

  User->DexterityAction(10);
  return true;
}

bool beartrap::Apply(character* User)
{
  Team = User->GetTeam()->GetID();
  if(IsActive())
    SetIsActive(false);
  else
    SetIsActive(true);

  User->DexterityAction(10);
  return true;
}

float arm::GetBlockChance(float EnemyToHitValue) const
{
  return GetWielded() ? Min(1.0f / (1 + EnemyToHitValue / (GetToHitValue() * GetWielded()->GetBlockModifier(Master)) * 10000), 1.0f) : 0;
}

ushort arm::GetBlockCapability() const
{
  return GetWielded() ? GetWielded()->GetStrengthValue() : 0;
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

vector2d beartrap::GetBitmapPos(ushort) const
{
  if(IsActive())
    return vector2d(32,304);
  else
    return vector2d(32,320);
}

bool mine::WillExplode(const character* Stepper) const
{
  return GetContainedMaterial()->GetTotalExplosivePower() != 0 && Stepper->GetWeight() > 500;
}

bool whipofcalamus::CalamusHelps(const character* Enemy, const character* Hitter) const
{
  if(!Hitter->GetMainWielded() || !Enemy->GetMainWielded())
    return false;
  if(Hitter->IsPlayer())
    {
      if(game::GetGod(10)->GetRelation() < 0)
	return false;
      else
	{
	  return (RAND() % (game::GetGod(10)->GetRelation() / 200 + 1));
	}
    }
  else
    return !(RAND() % 5);
}
