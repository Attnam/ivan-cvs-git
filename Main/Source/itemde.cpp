#define __FILE_OF_STATIC_ITEM_PROTOTYPE_DECLARATIONS__

#include "proto.h"
#include "itemba.h"

std::vector<item::prototype*> protocontainer<item>::ProtoData;
valuemap protocontainer<item>::CodeNameMap;

ITEM_PROTOTYPE(item, 0);

#include "femath.h"
#include "itemde.h"

#undef __FILE_OF_STATIC_ITEM_PROTOTYPE_DECLARATIONS__

#include <cmath>

#include "message.h"
#include "stack.h"
#include "charde.h"
#include "level.h"
#include "lsquare.h"
#include "lterraba.h"
#include "config.h"
#include "godba.h"
#include "stdover.h"
#include "whandler.h"
#include "lterrade.h"
#include "actionba.h"
#include "felist.h"
#include "save.h"
#include "team.h"
#include "error.h"

bool can::Open(character* Opener)
{
  /*if(Opener->GetAttribute(ARMSTRENGTH) > RAND() % 30)
    {
      item* Item = new lump(GetContainedMaterial());
      DonateSlotTo(Item);

      if(!game::IsInWilderness() && configuration::GetAutoDropLeftOvers())
	  Opener->GetLSquareUnder()->GetStack()->AddItem(this);
      else
	  Item->GetSlot()->AddFriendItem(this);

      SetContainedMaterial(0);
      return Item;
    }
  else
    {
      if(Opener->IsPlayer())
	ADD_MESSAGE("The can is shut tight.");

      return 0;
    }*/
  return false;
}

void banana::GenerateLeftOvers(character* Eater)
{
  item* Peals = new bananapeals(0, false);
  Peals->InitMaterials(GetMainMaterial());

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    Eater->GetLSquareUnder()->GetStack()->AddItem(Peals);
  else
    Eater->GetStack()->AddItem(Peals);

  RemoveFromSlot();
  SendToHell();
}

void potion::GenerateLeftOvers(character* Eater)
{
  /*ushort Emit = GetEmitation();

  if(!GetConsumeMaterial()->GetVolume())
    ChangeMaterial(GetConsumeMaterialIndex(), 0);

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();

      if(!game::IsInWilderness())
	GetLSquareUnder()->SignalEmitationDecrease(Emit);

      GetSquareUnder()->SendNewDrawRequest();
      GetSquareUnder()->SendMemorizedUpdateRequest();

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorized();
    }*/

  ChangeConsumeMaterial(0);

  if(!game::IsInWilderness() && (!Eater->IsPlayer() || configuration::GetAutoDropLeftOvers()))
    MoveTo(Eater->GetLSquareUnder()->GetStack());
  else
    MoveTo(Eater->GetStack());
}

void lantern::PositionedDrawToTileBuffer(uchar LSquarePosition, bool Animate) const
{
  bitmap* Bitmap = !Animate || AnimationFrames == 1 ? Picture[0] : Picture[globalwindowhandler::GetTick() % AnimationFrames];

  switch(LSquarePosition)
    {
    case CENTER:
    case DOWN:
      Bitmap->AlphaBlit(igraph::GetTileBuffer());
      break;
    case LEFT:
      Bitmap->AlphaBlit(igraph::GetTileBuffer(), uchar(ROTATE));
      break;
    case UP:
      Bitmap->AlphaBlit(igraph::GetTileBuffer(), uchar(FLIP));
      break;
    case RIGHT:
      Bitmap->AlphaBlit(igraph::GetTileBuffer(), uchar(ROTATE | MIRROR));
      break;
    }
}

bool scroll::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::GetSeeWholeMapCheat();
}

bool scrollofcreatemonster::Read(character* Reader)
{
  vector2d TryToCreate;

  for(ushort c = 0; c < 100; ++c)
    {
      TryToCreate = Reader->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

      character* Monster = protosystem::CreateMonster();

      if(game::IsValidPos(TryToCreate) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->IsWalkable(Monster) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetCharacter() == 0)
	{
	  game::GetCurrentLevel()->GetLSquare(TryToCreate)->AddCharacter(Monster);

	  if(Reader->IsPlayer())
	    ADD_MESSAGE("As you read the scroll a monster appears.");
	  else
	    if(Reader->GetSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("The %s reads %s. A monster appears!", Reader->CHARNAME(DEFINITE), CHARNAME(DEFINITE));

	  return true;
	}
      else
	delete Monster;
    }

  ADD_MESSAGE("You feel a lost soul fly by you.");
  return true;
}

bool scrollofteleportation::Read(character* Reader)
{
  if(Reader->IsPlayer())
    ADD_MESSAGE("After you have read the scroll you realize that you have teleported.");
  else
    if(Reader->GetSquareUnder()->CanBeSeen())
      ADD_MESSAGE("The %s reads %s and disappears!", Reader->CHARNAME(DEFINITE), CHARNAME(DEFINITE));

  Reader->TeleportRandomly();
  return true;
}

void lump::ReceiveHitEffect(character* Enemy, character*)
{
  if(RAND() % 2)
    {
      if(Enemy->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("The %s touches %s.", GetMainMaterial()->CHARNAME(UNARTICLED), Enemy->CHARNAME(DEFINITE));

      GetMainMaterial()->HitEffect(Enemy);
    }
}

void meleeweapon::ReceiveHitEffect(character* Enemy, character*)
{
  if(GetContainedMaterial())
    {
      if(Enemy->IsPlayer())
	ADD_MESSAGE("The %s reacts with you!", GetContainedMaterial()->CHARNAME(UNARTICLED));
      else
	if(Enemy->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("The %s reacts with %s.", GetContainedMaterial()->CHARNAME(UNARTICLED), Enemy->CHARNAME(DEFINITE));

      GetContainedMaterial()->HitEffect(Enemy);
    }
}

void meleeweapon::DipInto(material* Material, character* Dipper)
{
  ChangeContainedMaterial(Material);

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now covered with %s.", CHARNAME(DEFINITE), Material->CHARNAME(UNARTICLED));
}

material* lump::CreateDipMaterial()
{
  return GetMainMaterial()->Clone(GetMainMaterial()->TakeDipVolumeAway());
}

item* can::PrepareForConsuming(character* Consumer)
{
  /*  if(!Consumer->IsPlayer() || game::BoolQuestion("Do you want to open " + GetName(DEFINITE) + " before eating it? [Y/n]", YES))
    return TryToOpen(Consumer);
    else*/
    return 0;
}

bool pickaxe::Apply(character* User)
{
  uchar Dir = game::DirectionQuestion("What direction do you want to dig? [press a direction key]", false);

  if(Dir == DIR_ERROR)
    return false;

  vector2d Temp = game::GetMoveVector(Dir);
	
  if(game::IsValidPos(User->GetPos() + Temp))
    {
      lsquare* Square = game::GetCurrentLevel()->GetLSquare(User->GetPos() + Temp);

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
		  return true;
		}
	      else
		return false;
	    }
	  else
	    ADD_MESSAGE("%s is too hard to dig.", Square->GetOLTerrain()->CHARNAME(DEFINITE));
	else
	  ADD_MESSAGE(Square->GetOLTerrain()->DigMessage().c_str());
    }

  return false;
}

bool wand::Apply(character* Terrorist)
{
  if(Terrorist->IsPlayer())
    ADD_MESSAGE("%s breaks in two and then explodes!", CHARNAME(DEFINITE));
  else
    if(Terrorist->GetSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s breaks %s in two. It explodes!", Terrorist->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

  RemoveFromSlot();
  SendToHell();

  std::string DeathMsg;

  if(Terrorist->IsPlayer())
    DeathMsg = "exploded himself by breaking a wand";
  else
    DeathMsg = "kamikazed by " + Terrorist->GetName(INDEFINITE);

  Terrorist->GetLSquareUnder()->GetLevelUnder()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), 40);
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
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
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
  game::DrawEverythingNoBlit();
  std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);

  item* TempItem = protosystem::CreateItem(Temp, Reader->IsPlayer());

  if(TempItem)
    {
      Reader->GetStack()->AddItem(TempItem);
      ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CHARNAME(INDEFINITE));
      Reader->EditAP(-2500); // very difficult to read
      return true;
    }

  return false;
}

bool lantern::ReceiveDamage(character*, short Damage, uchar)
{
  if(!(RAND() % 75) && Damage > 10 + RAND() % 10)
    {
      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s shatters to pieces.", CHARNAME(DEFINITE));

      brokenlantern* Lantern = new brokenlantern(0, false);
      Lantern->InitMaterials(GetMainMaterial());
      Lantern->SignalSquarePositionChange(OnWall);
      DonateSlotTo(Lantern);
      SendToHell();
      return true;
    }

  return false;
}

bool potion::ReceiveDamage(character*, short Damage, uchar)
{
  if(!(RAND() % 75) && Damage > 10 + RAND() % 10)
    {
      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s shatters to pieces.", CHARNAME(DEFINITE));

      if(GetContainedMaterial()) 
	GetLSquareUnder()->SpillFluid(5, GetContainedMaterial()->GetColor());

      item* Remains = new brokenbottle(0, false);
      Remains->InitMaterials(GetMainMaterial());
      DonateSlotTo(Remains);
      SendToHell();
      return true;
    }

  return false;
}

bool scrollofchangematerial::Read(character* Reader)
{
  if(!Reader->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to change.");
      return false;
    }

  item* Item = Reader->GetStack()->DrawContents(Reader, "What item do you wish to change?");

  if(!Item)
    return false;

  if(Item == this)
    {
      ADD_MESSAGE("That would be rather insane.");
      return false;
    }

  if(!Item->IsMaterialChangeable())
    {
      ADD_MESSAGE("Your magic is not powerful enough to affect %s .", Item->CHARNAME(DEFINITE));
      return false;
    }

  game::DrawEverythingNoBlit();
  std::string Temp = game::StringQuestion("What material do you want to wish for?", vector2d(16, 6), WHITE, 0, 80, false);

  material* TempMaterial = protosystem::CreateMaterial(Temp, Item->GetMainMaterial()->GetVolume());
	
  if(TempMaterial)
    Item->ChangeMainMaterial(TempMaterial);
  else
    {
      return false;
    }

  return true;
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
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
  return true;
}

bool platemail::ReceiveDamage(character*, short Damage, uchar)
{
  if(Damage > 2500.0f / GetStrengthValue() + RAND() % 11 - RAND() % 11)
    {
      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s is damaged.", CHARNAME(DEFINITE));

      item* Plate = new brokenplatemail(0, false);
      Plate->InitMaterials(GetMainMaterial());
      DonateSlotTo(Plate);
      SendToHell();
      return true;
    }

  return false;
}

bool brokenbottle::GetStepOnEffect(character* Stepper)
{
  if(!(RAND() % 10))
    {
      if(Stepper->ReceiveDamage(0, 1 + RAND() % 2, PHYSICALDAMAGE, LEGS))
	{
	  if(Stepper->IsPlayer())
	    ADD_MESSAGE("Auch. You step on sharp glass splinters.");
	  else
	    if(Stepper->GetSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s steps on sharp glass splinters and is hurt.", Stepper->CHARNAME(DEFINITE));
	}
      else
	{
	  if(Stepper->IsPlayer())
	    ADD_MESSAGE("Some glass splinters are shatteRED under your feet.");
	  else
	    if(Stepper->GetSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("Some glass splinters are shatteRED under %s's feet.", Stepper->CHARNAME(DEFINITE));
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

void lantern::SignalSquarePositionChange(bool NewPosOnWall)
{
  if(OnWall == NewPosOnWall)
    return;

  OnWall = NewPosOnWall;
  UpdatePictures();
}

void lantern::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << OnWall;
}

void lantern::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> OnWall;
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

      if(RAND() % 2)
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
  if(GetMainMaterial()->IsFlexible())
    return 1000;
  else
    return 70;
}

bool backpack::Apply(character* Terrorist)
{
  if(GetContainedMaterial() && GetContainedMaterial()->IsExplosive())
    {
      if(Terrorist->IsPlayer())
	ADD_MESSAGE("You light your %s. It explodes!", CHARNAME(UNARTICLED));
      else
	if(Terrorist->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s lights %s. It explodes!", Terrorist->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

      RemoveFromSlot();
      SendToHell();

      std::string DeathMsg;

      if(Terrorist->IsPlayer())
	DeathMsg = "exploded himself with " + GetName(INDEFINITE);
      else
	DeathMsg = "kamikazed by " + Terrorist->GetName(INDEFINITE);

      Terrorist->GetLSquareUnder()->GetLevelUnder()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }
  else
    if(Terrorist->IsPlayer())
      ADD_MESSAGE("You can't apply this!");	

  return false;
}

void holybook::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> DivineMaster;
}

void holybook::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << DivineMaster;
}

bool holybook::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::GetSeeWholeMapCheat();
}

bool holybook::Read(character* Reader)
{
  if(Reader->IsPlayer())
    {
      if(game::GetGod(GetDivineMaster())->GetKnown())
	{
	  ADD_MESSAGE("The book reveals many divine secrets of %s to you.", game::GetGod(DivineMaster)->GOD_NAME);
	  game::GetGod(DivineMaster)->AdjustRelation(75);
	  game::ApplyDivineAlignmentBonuses(game::GetGod(DivineMaster), true);

	  if(RAND() % 3)
	    return false;
	  else
	    {
	      ADD_MESSAGE("But then it disappears.");
	      return true;
	    }
	}

      game::GetGod(GetDivineMaster())->SetKnown(true);
      ADD_MESSAGE("You read the book through and feel that you master the magical rites of %s.", game::GetGod(DivineMaster)->GOD_NAME);
    }

  return false;
}

bool wand::ReceiveDamage(character* Damager, short, uchar Type)
{
  if((Type == FIRE && !(RAND() % 10)) || (Type == ENERGY && !(RAND() % 10)))
    {
      std::string DeathMsg = "explosion of ";

      if(Damager)
	DeathMsg += GetName(INDEFINITE) + " caused by " + Damager->GetName(INDEFINITE);
      else
	DeathMsg += GetName(INDEFINITE);

      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      level* LevelUnder = GetLSquareUnder()->GetLevelUnder();
      RemoveFromSlot();
      SendToHell();
      LevelUnder->Explosion(Damager, DeathMsg, GetLSquareUnder()->GetPos(), 40);
      return true;
    }

  return false;
}

bool backpack::ReceiveDamage(character* Damager, short, uchar Type)
{
  if((Type == FIRE && !(RAND() % 3)) || (Type == ENERGY && RAND() % 3))
    {
      std::string DeathMsg = "explosion of ";

      if(Damager)
	DeathMsg += GetName(INDEFINITE) + " caused by " + Damager->GetName(INDEFINITE);
      else
	DeathMsg += GetName(INDEFINITE);

      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      level* LevelUnder = GetLSquareUnder()->GetLevelUnder();
      RemoveFromSlot();
      SendToHell();
      LevelUnder->Explosion(Damager, DeathMsg, GetLSquareUnder()->GetPos(), GetContainedMaterial()->GetTotalExplosivePower());
      return true;
    }

  return false;
}

std::string wand::GetPostFix() const
{
  if(!TimesUsed)
    return item::GetPostFix();
  else if(TimesUsed == 1)
    return item::GetPostFix() + " (used 1 time)";
  else
    return item::GetPostFix() + " (used " + TimesUsed + " times)";
}

bool scroll::ReceiveDamage(character*, short, uchar Type)
{
  if(Type == FIRE && !(RAND() % 10) && GetMainMaterial()->IsFlammable())
    {
      if(GetLSquareUnder()->CanBeSeen())
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
	if(!game::GetCurrentLevel()->IsValid(CurrentPos + game::GetMoveVector(Direction)))
	  break;

	lsquare* CurrentSquare = game::GetCurrentLevel()->GetLSquare(CurrentPos + game::GetMoveVector(Direction));

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

	    if(CurrentSquare->CanBeSeen(true))
	      CurrentSquare->DrawParticles(GetBeamColor(), Direction);
	  }
      }
  else
    {
      lsquare* Where = Zapper->GetLSquareUnder();
      BeamEffect(Zapper, DeathMsg, Direction, Where);

      if(Where->CanBeSeen(true))
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

bool holybook::ReceiveDamage(character*, short, uchar Type)
{
  if(Type == FIRE && !(RAND() % 2) && GetMainMaterial()->IsFlammable())
    {
      if(GetSquareUnder()->CanBeSeen())
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

	  if(game::IsValidPos(TryToCreate) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->IsWalkable(Genie) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetCharacter() == 0)
	    {
	      game::GetCurrentLevel()->GetLSquare(TryToCreate)->AddCharacter(Genie);
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

			game::GetCurrentLevel()->RemoveCharacter(TryToCreate);
			delete Genie;
			return true;
		    }
		}
	    }

	    item* Sword = new curvedtwohandedsword;
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
      {
	ADD_MESSAGE("Nothing happens.");
      }

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

void holybook::SetDivineMaster(uchar NewDivineMaster)
{
  DivineMaster = NewDivineMaster;
  UpdatePictures();
}

ushort holybook::GetMaterialColor0(ushort) const
{
  return game::GetGod(DivineMaster)->GetColor();
}

bool scrollofcharging::Read(character* Reader)
{
  if(Reader->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to charge.");
      return false;
    }

  item* Item = Reader->GetStack()->DrawContents(Reader, "What item do you wish to charge?", &item::ChargeableSorter);

  if(!Item)
    return false;

  if(Item == this)
    {
      ADD_MESSAGE("This would cause a rift to appear in the space-time continuum which wouldn't be nice.");
      return false;
    }

  game::DrawEverythingNoBlit();
  Item->ChargeFully(Reader);
  ADD_MESSAGE("You charge %s and the scroll burns.", Item->CHARNAME(DEFINITE));
  return true;
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

bool banana::Zap(character* Zapper, vector2d, uchar)
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

  Zapper->EditAP(500);
  return true;
}

bool bananapeals::GetStepOnEffect(character* Stepper)
{
  if(!(RAND() % 3))
    {
      if(Stepper->IsPlayer())
	ADD_MESSAGE("Auch. Your feet slip on %s and you fall down.", CHARNAME(INDEFINITE));
      else
	if(Stepper->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s steps on %s and falls down.", Stepper->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));
      Stepper->EditAP(1000);
      /* Do damage against any random bodypart except legs */
      Stepper->ReceiveDamage(0, 1 + RAND() % 2, PHYSICALDAMAGE, ALL&~LEGS);
      Stepper->CheckDeath("stepped on a banana peal.");
      Stepper->EditAP(-500);
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
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
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
  // First find all characters in the squares around Reader
  std::vector<character*> CharactersNearBy;

  for(ushort c = 0; c < 8; ++c)
    {
      vector2d Test = Reader->GetPos() + game::GetMoveVector(c);
      if(game::IsValidPos(Test))
	{
	  character* CharacterInSquare = game::GetCurrentLevel()->GetLSquare(Test)->GetCharacter();
	  if(CharacterInSquare && CharacterInSquare->IsCharmable() && CharacterInSquare->GetTeam() != Reader->GetTeam())
	    CharactersNearBy.push_back(CharacterInSquare);
	}
    }
  
  // Then pick one of the characters and set it to the same team as Reader
  if(CharactersNearBy.empty())
    {
      ADD_MESSAGE("The scroll burns, but nothing happens.");
    }
  else
    {
      character* ToBeTamed = CharactersNearBy[RAND() % CharactersNearBy.size()];
      ToBeTamed->ChangeTeam(Reader->GetTeam());
      ADD_MESSAGE("The scroll burns and suddenly %s looks friendly.", ToBeTamed->CHARNAME(DEFINITE));
    }

  return true;
}

void bodypart::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << BitmapPos << Color1 << Color2 << Color3 << HP << OwnerDescription << Unique << RegenerationCounter;
}

void bodypart::Load(inputfile& SaveFile)
{
  materialcontainer::Load(SaveFile);
  SaveFile >> BitmapPos >> Color1 >> Color2 >> Color3 >> HP >> OwnerDescription >> Unique >> RegenerationCounter;
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
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
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

short bodypart::GetMaxHP() const
{
  if(GetMaster())
    {
      short HP = 0;

      if(GetMainMaterial()->IsAlive())
	HP = GetVolume() * GetMaster()->GetAttribute(ENDURANCE) / 10000;
      else
	HP = GetVolume() * GetMainMaterial()->GetStrengthValue() / 10000;

      if(HP < 1)
	HP = 1;

      return HP;
    }
  else
    return 0;
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

void bodypart::LoadGearSlot(inputfile& SaveFile, gearslot& GearSlot)
{
  SaveFile >> GearSlot;
  GearSlot.SetBodyPart(this);

  if(*GearSlot)
    {
      EditVolume(GearSlot->GetVolume());
      EditWeight(GearSlot->GetWeight());
    }
}

void head::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BiteStrength;
  SaveFile << HelmetSlot;
  SaveFile << AmuletSlot;
}

void head::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BiteStrength;
  LoadGearSlot(SaveFile, HelmetSlot);
  LoadGearSlot(SaveFile, AmuletSlot);
}

void humanoidtorso::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BodyArmorSlot;
  SaveFile << CloakSlot;
  SaveFile << BeltSlot;
}

void humanoidtorso::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  LoadGearSlot(SaveFile, BodyArmorSlot);
  LoadGearSlot(SaveFile, CloakSlot);
  LoadGearSlot(SaveFile, BeltSlot);
}

void arm::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << SingleWeaponSkill << UnarmedStrength;
  SaveFile << Strength << StrengthExperience << Dexterity << DexterityExperience;
  SaveFile << WieldedSlot;
  SaveFile << GauntletSlot;
  SaveFile << RingSlot;
}

void arm::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> SingleWeaponSkill >> UnarmedStrength;
  SaveFile >> Strength >> StrengthExperience >> Dexterity >> DexterityExperience;
  LoadGearSlot(SaveFile, WieldedSlot);
  LoadGearSlot(SaveFile, GauntletSlot);
  LoadGearSlot(SaveFile, RingSlot);
  SetCurrentSingleWeaponSkill(0);

  if(GetWielded())
    for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
      if((*i)->GetID() == GetWielded()->GetID())
	{
	  SetCurrentSingleWeaponSkill(*i);
	  break;
	}
}

void leg::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << KickStrength << Strength << StrengthExperience << Agility << AgilityExperience;
  SaveFile << BootSlot;
}

void leg::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> KickStrength >> Strength >> StrengthExperience >> Agility >> AgilityExperience;
  LoadGearSlot(SaveFile, BootSlot);
}

bool torso::ReceiveDamage(character* Damager, short Damage, uchar Type)
{
  if(GetMaster())
    GetMaster()->GetStack()->ReceiveDamage(Damager, Damage, Type);

  return bodypart::ReceiveDamage(Damager, Damage, Type);
}

bool bodypart::ReceiveDamage(character*, short Damage, uchar)
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
  SaveFile >> Charged;
}

void mine::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << Charged;
}

bool mine::ReceiveDamage(character* Damager, short, uchar Type)
{
  if((Type == FIRE && !(RAND() % 2)) || (Type == ENERGY && !(RAND() % 2)))
    {
      std::string DeathMsg = "explosion of ";

      if(Damager)
	DeathMsg += GetName(INDEFINITE) + " caused by " + Damager->GetName(INDEFINITE);
      else
	DeathMsg += GetName(INDEFINITE);

      if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      level* LevelUnder = GetLSquareUnder()->GetLevelUnder();
      RemoveFromSlot();
      SendToHell();
      LevelUnder->Explosion(Damager, DeathMsg, GetLSquareUnder()->GetPos(), 30);
      return true;
    }

  return false;
}

bool mine::GetStepOnEffect(character* Stepper)
{
  if(GetCharged())
    {
      if(Stepper->IsPlayer())
	ADD_MESSAGE("You hear a faint thumb. You look down. You see %s. It explodes.", CHARNAME(INDEFINITE));
      else if(Stepper->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s steps on %s.", Stepper->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));
      Stepper->GetLSquareUnder()->GetLevelUnder()->Explosion(0, "killed by a land mine", Stepper->GetPos(), 30);
    }

  return false;
}

bool wandofhaste::BeamEffect(character*, const std::string&, uchar, lsquare* LSquare)
{
  character* Dude = LSquare->GetCharacter();
  if(Dude)
    {
      Dude->Haste(500 + RAND() % 1000);
      return true;
    }
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
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
  return true;
}

bool wandofslow::BeamEffect(character*, const std::string&, uchar, lsquare* LSquare)
{
  character* Dude = LSquare->GetCharacter();
  if(Dude)
    {
      Dude->Slow(500 + RAND() % 1000);
      return true;
    }
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
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
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

      if(game::IsValidPos(ApplyPos))
	game::GetCurrentLevel()->GetLSquare(ApplyPos)->TryKey(this, User);
      else
	return false;

      User->EditAP(500);
    }

  return true;
}

void arm::SignalGearUpdate()
{
  if(!GetWielded() && GetCurrentSingleWeaponSkill())
    {
      if(!GetCurrentSingleWeaponSkill()->GetHits())
	for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
	  if(*i == GetCurrentSingleWeaponSkill())
	    {
	      delete *i;
	      SingleWeaponSkill.erase(i);
	      break;
	    }

      SetCurrentSingleWeaponSkill(0);
    }
}

void arm::SetWielded(item* Item)
{
  WieldedSlot.PutInItem(Item);

  if(Item)
    {
      for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
	if((*i)->GetID() == Item->GetID())
	  {
	    SetCurrentSingleWeaponSkill(*i);
	    break;
	  }

      if(!GetCurrentSingleWeaponSkill())
	{
	  SetCurrentSingleWeaponSkill(new sweaponskill);
	  GetCurrentSingleWeaponSkill()->SetID(Item->GetID());
	  SingleWeaponSkill.push_back(GetCurrentSingleWeaponSkill());
	}
    }

  if(GetSquareUnder())
    GetSquareUnder()->SendNewDrawRequest();
}

void arm::Be()
{
  for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end();)
    {
      if((*i)->Turn(1) && Slot->IsCharacterSlot())
	{
	  character* Master = ((characterslot*)Slot)->GetMaster();

	  if(Master->IsPlayer())
	    for(stackiterator j = Master->GetStack()->GetBottomSlot(); j != Master->GetStack()->GetSlotAboveTop(); ++j)
	      if((*i)->GetID() == (**j)->GetID())
		{
		  (*i)->AddLevelDownMessage((**j)->GetName(UNARTICLED));
		  break;
		}
	}

      if(!(*i)->GetHits() && *i != GetCurrentSingleWeaponSkill())
	{
	  SingleWeaponSkill.erase(i);
	  i = SingleWeaponSkill.begin();
	  continue;
	}

      ++i;
    }
}

float arm::CalculateWieldedStrength(bool OneHanded) const
{
  if(GetWielded()->IsShield(GetMaster()))
    return 0;

  float Strength = GetWielded()->GetWeaponStrength() * this->Strength * GetHumanoidMaster()->GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus();

  if(OneHanded)
    Strength *= float(100 - GetWielded()->GetOneHandedStrengthPenalty(GetMaster())) / 100;

  return Strength;
}

float arm::CalculateUnarmedStrength() const
{
  return GetUnarmedStrength() * Strength * GetHumanoidMaster()->GetCategoryWeaponSkill(UNARMED)->GetBonus();
}

float leg::CalculateKickStrength() const
{
  return GetKickStrength() * Strength * GetHumanoidMaster()->GetCategoryWeaponSkill(KICK)->GetBonus();
}

float head::CalculateBiteStrength() const
{
  return GetBiteStrength() * 10 * GetHumanoidMaster()->GetCategoryWeaponSkill(BITE)->GetBonus();
}

float arm::CalculateWieldedToHitValue(bool OneHanded) const
{
  if(!OneHanded && GetWielded()->IsShield(GetMaster()))
    OneHanded = true;

  float ToHit = GetMaster()->GetBattleAttributeModifier() * ((Dexterity << 2) + Strength + GetMaster()->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus() / sqrt(GetWielded()->GetWeight() > 400 ? GetWielded()->GetWeight() : 400) * 10;

  if(OneHanded)
    ToHit *= float(100 - GetWielded()->GetOneHandedToHitPenalty(GetMaster())) / 100;

  return ToHit;
}

float arm::CalculateUnarmedToHitValue() const
{
  return GetMaster()->GetBattleAttributeModifier() * ((Dexterity << 2) + Strength + GetMaster()->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCategoryWeaponSkill(UNARMED)->GetBonus() / 2;
}

float leg::CalculateKickToHitValue() const
{
  return GetMaster()->GetBattleAttributeModifier() * ((Agility << 2) + Strength + GetMaster()->GetAttribute(PERCEPTION)) * GetHumanoidMaster()->GetCategoryWeaponSkill(UNARMED)->GetBonus() / 4;
}

humanoid* bodypart::GetHumanoidMaster() const
{
  return (humanoid*)GetMaster();
}

ushort belt::GetFormModifier() const
{
  if(GetMainMaterial()->IsFlexible())
    return 800;
  else
    return 60;
}

character* bodypart::GetMaster() const
{
  if(Slot->IsCharacterSlot())
    return GetCharacterSlot()->GetMaster();
  else
    return 0;
}

characterslot* bodypart::GetCharacterSlot() const
{
  return (characterslot*)GetSlot();
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

/*void helmet::GenerateMaterials()
{
  static ushort Possibility[] = { 250, 50, 100, 150, 50, 10 };

  switch(femath::WeightedRand(6, Possibility))
    {
    case 0: InitMaterials(MAKE_MATERIAL(LEATHER)); break;
    case 1: InitMaterials(MAKE_MATERIAL(BONE)); break;
    case 2: InitMaterials(MAKE_MATERIAL(COPPER)); break;
    case 3: InitMaterials(MAKE_MATERIAL(BRONZE)); break;
    case 4: InitMaterials(MAKE_MATERIAL(IRON)); break;
    case 5: InitMaterials(MAKE_MATERIAL(MITHRIL)); break;
    }
}*/

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
  EditVolume(Deceased->GetVolume());
  EditWeight(Deceased->GetWeight());
}

std::string corpse::GetPostFix() const
{
  return "of " + GetDeceased()->GetName(INDEFINITE);
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

ushort corpse::GetEmitation() const
{
  return GetDeceased()->GetEmitation();
}

bool corpse::IsConsumable(const character* Eater) const
{
  for(ushort c = 0; c < GetDeceased()->BodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c) && GetDeceased()->GetBodyPart(c)->IsConsumable(Eater))
      return true;

  return false;
}

short corpse::CalculateOfferValue(char GodAlignment) const
{
  short OfferValue = 0;

  for(ushort c = 0; c < GetDeceased()->BodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      OfferValue += GetDeceased()->GetBodyPart(c)->CalculateOfferValue(GodAlignment);

  return OfferValue;
}

float corpse::GetWeaponStrength() const
{
  return sqrt(float(GetFormModifier()) * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() * GetWeight());
}

bool corpse::IsBadFoodForAI(character* Eater) const
{
  for(ushort c = 0; c < GetDeceased()->BodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c) && GetDeceased()->GetBodyPart(c)->IsBadFoodForAI(Eater))
      return true;

  return false;
}

ushort corpse::GetStrengthValue() const
{
  return ulong(GetStrengthModifier()) * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() / 1000;
}

/*ulong corpse::GetVolume() const
{
  ulong Volume = 0;

  for(ushort c = 0; c < GetDeceased()->BodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      Volume += GetDeceased()->GetBodyPart(c)->GetVolume();

  return Volume;
}*/

corpse::~corpse()
{
  if(GetDeceased())
    GetDeceased()->SendToHell();
}

void corpse::SetMainMaterial(material* NewMaterial)
{
  GetDeceased()->SetMainMaterial(NewMaterial);
}

void corpse::ChangeMainMaterial(material* NewMaterial)
{
  GetDeceased()->ChangeMainMaterial(NewMaterial);
}

void corpse::SetContainedMaterial(material* NewMaterial)
{
  GetDeceased()->SetContainedMaterial(NewMaterial);
}

void corpse::ChangeContainedMaterial(material* NewMaterial)
{
  GetDeceased()->ChangeContainedMaterial(NewMaterial);
}

ushort corpse::GetMaterialColor0(ushort) const
{
  return GetDeceased()->GetTorso()->GetMainMaterial()->GetColor();
}

uchar corpse::GetAlpha0(ushort) const
{
  return GetDeceased()->GetTorso()->GetMainMaterial()->GetAlpha();
}

ushort corpse::GetMaterialColor1(ushort) const
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
  EditVolume(Deceased->GetVolume());
  EditWeight(Deceased->GetWeight());
  UpdatePictures();
}

void bodypart::Regenerate(ushort Turns)
{
  if(GetMainMaterial()->IsAlive())
    {
      ulong RegenerationBonus = ulong(GetMaxHP()) * GetMaster()->GetAttribute(ENDURANCE) * Turns;

      if(GetMaster()->GetAction() && GetMaster()->GetAction()->GetRestRegenerationBonus())
	RegenerationBonus *= GetMaster()->GetSquareUnder()->RestModifier();

      EditRegenerationCounter(RegenerationBonus);

      while(GetRegenerationCounter() > 100000)
	{
	  if(GetHP() < GetMaxHP())
	    {
	      EditHP(1);
	      GetMaster()->EditExperience(ENDURANCE, 100);
	    }

	  EditRegenerationCounter(-100000);
	}
    }
}

ushort head::DangerWeight() const
{
  return 100;
}

ushort torso::DangerWeight() const
{
  return 100;
}

ushort arm::DangerWeight() const
{
  return 50;
}

ushort groin::DangerWeight() const
{
  return 100;
}

ushort leg::DangerWeight() const
{
  return 33;
}

ushort bodypart::Danger(ulong DangerEstimate, bool MaxDanger) const
{
  short HP = MaxDanger ? GetMaxHP() : GetHP();

  ushort Damage = 100; //Temporary

  if(GetResistance(PHYSICALDAMAGE) >= Damage)
    Damage = GetResistance(PHYSICALDAMAGE) + 1;

  return HP / (Damage - GetResistance(PHYSICALDAMAGE));
}

/*ulong head::GetTotalWeight() const
{
  ulong Weight = GetWeight();

  if(GetHelmet())
    Weight += GetHelmet()->GetWeight();

  if(GetAmulet())
    Weight += GetAmulet()->GetWeight();

  return Weight;
}

ulong humanoidtorso::GetTotalWeight() const
{
  ulong Weight = GetWeight();

  if(GetBodyArmor())
    Weight += GetBodyArmor()->GetWeight();

  if(GetCloak())
    Weight += GetCloak()->GetWeight();

  if(GetBelt())
    Weight += GetBelt()->GetWeight();

  return Weight;
}

ulong arm::GetTotalWeight() const
{
  ulong Weight = GetWeight();

  if(GetWielded())
    Weight += GetWielded()->GetWeight();

  if(GetGauntlet())
    Weight += GetGauntlet()->GetWeight();

  if(GetRing())
    Weight += GetRing()->GetWeight();

  return Weight;
}

ulong leg::GetTotalWeight() const
{
  ulong Weight = GetWeight();

  if(GetBoot())
    Weight += GetBoot()->GetWeight();

  return Weight;
}*/

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

void arm::AddCurrentSingleWeaponSkillInfo(felist& List)
{
  if(CurrentSingleWeaponSkill)
    {
      List.AddEntry("", RED);

      std::string Buffer = "current " + GetName(UNARTICLED) + " single weapon skill:  ";

      Buffer += CurrentSingleWeaponSkill->GetLevel();
      Buffer.resize(40, ' ');

      Buffer += CurrentSingleWeaponSkill->GetHits();
      Buffer.resize(50, ' ');

      if(CurrentSingleWeaponSkill->GetLevel() != 10)
	List.AddEntry(Buffer + (CurrentSingleWeaponSkill->GetLevelMap(CurrentSingleWeaponSkill->GetLevel() + 1) - CurrentSingleWeaponSkill->GetHits()), RED);
      else
	List.AddEntry(Buffer + '-', RED);
    }
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

  for(ushort c = 0; c < GetDeceased()->BodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      Score += GetDeceased()->GetBodyPart(c)->GetScore();

  return Score;
}

bool corpse::IsDestroyable() const
{
  for(ushort c = 0; c < GetDeceased()->BodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c) && !GetDeceased()->GetBodyPart(c)->IsDestroyable())
      return false;

  return true;
}

ulong corpse::Price() const
{
  ulong Price = 0;

  for(ushort c = 0; c < GetDeceased()->BodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      Price += GetDeceased()->GetBodyPart(c)->GetPrice();

  return Price;
}

/*ulong corpse::GetWeight() const
{
  ulong Weight = 0;

  for(ushort c = 0; c < GetDeceased()->BodyParts(); ++c)
    if(GetDeceased()->GetBodyPart(c))
      Weight += GetDeceased()->GetBodyPart(c)->GetWeight();

  return Weight;
}*/

item* corpse::PrepareForConsuming(character*)
{
  for(ushort c = GetDeceased()->BodyParts() - 1; c != 0; --c)
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
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
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

ushort materialcontainer::GetMaterialColor1(ushort Frame) const
{
  if(GetContainedMaterial())
    return GetContainedMaterial()->GetColor();
  else
    return GetMaterialColor0(Frame);
}

uchar materialcontainer::GetAlpha1(ushort Frame) const
{
  if(GetContainedMaterial() && GetContainedMaterial()->GetAlpha() > GetAlpha0(Frame))
    return GetContainedMaterial()->GetAlpha();
  else
    return GetAlpha0(Frame);
}

ushort meleeweapon::GetMaterialColor1(ushort) const
{
  if(GetSecondaryMaterial())
    return GetSecondaryMaterial()->GetColor();
  else
    return 0;
}

uchar meleeweapon::GetAlpha1(ushort) const
{
  if(GetSecondaryMaterial())
    return GetSecondaryMaterial()->GetAlpha();
  else
    return 0;
}

ushort bodypart::GetMaterialColor0(ushort Frame) const
{
  if(GetMainMaterial())
    return GetMainMaterial()->GetSkinColor(Frame);
  else
    return 0;
}

ushort banana::GetMaterialColor0(ushort Frame) const
{
  ushort Color = object::GetMaterialColor0(Frame);

  if(!Frame)
    return Color;
  else
    return MAKE_RGB(GET_RED(Color) * Frame / 20, GET_GREEN(Color) * Frame / 20, GET_BLUE(Color) * Frame / 20);
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
  Zapper->EditExperience(PERCEPTION, 50);
  Zapper->EditAP(500);
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
  SetOnWall(false);
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

void holybook::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);

  /* Don't use SetDivineMaster, since it calls UpdatePictures()! */

  if(!Load)
    DivineMaster = 1 + RAND() % (game::GetGods() - 1);
}

void oillamp::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  
  if(!Load)
    SetInhabitedByGenie(!(RAND() % 2));
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
    SetCharged(RAND() % 5 ? true : false);
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
}

void head::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);
  HelmetSlot.Init(this);
  AmuletSlot.Init(this);
}

void humanoidtorso::VirtualConstructor(bool Load)
{
  torso::VirtualConstructor(Load);
  BodyArmorSlot.Init(this);
  CloakSlot.Init(this);
  BeltSlot.Init(this);
}

void arm::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);
  WieldedSlot.Init(this);
  GauntletSlot.Init(this);
  RingSlot.Init(this);
  SetCurrentSingleWeaponSkill(0);
  SetHasBe(true);

  if(!Load)
    StrengthExperience = DexterityExperience = 0;
}

void leg::VirtualConstructor(bool Load)
{
  bodypart::VirtualConstructor(Load);
  BootSlot.Init(this);

  if(!Load)
    StrengthExperience = AgilityExperience = 0;
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
    SetCharges(1 + RAND() % 2);
}

std::string platemail::GetNameSingular() const
{
  if(GetMainMaterial() && GetMainMaterial()->IsFlexible()) 
    return "armor";
  else
    return "plate mail"; 
}

bool whistle::Apply(character* Whistler) 
{
  BlowEffect(Whistler);
  return true;
} 

void whistle::BlowEffect(character* Whistler)
{
  if(Whistler->IsPlayer())
    ADD_MESSAGE("You produce an interesting sound.");
  else if(Whistler->GetSquareUnder()->CanBeSeen())
    ADD_MESSAGE("%s blows to his whistle and produces an odd sound.", Whistler->CHARNAME(DEFINITE));
  else 
    ADD_MESSAGE("You hear a whistle.");
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
      SetIsLocked(RAND() % 3 ? true : false);
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
	    ADD_MESSAGE("You unlock %s.", CHARNAME(DEFINITE));
	  else if(Applier->GetSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s unlocks %s.", Applier->CHARNAME(DEFINITE), CHARNAME(DEFINITE));
	}
      else
	{
	  if(Applier->IsPlayer())
	    ADD_MESSAGE("You lock %s.", CHARNAME(DEFINITE));
	  else if(Applier->GetSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s locks %s.", Applier->CHARNAME(DEFINITE), CHARNAME(DEFINITE));
	}

      SetIsLocked(!IsLocked());
    }
  else
    {
      if(Applier->IsPlayer())
	ADD_MESSAGE("%s doesn't fit in the lock.", Key->CHARNAME(DEFINITE));
      else if(Applier->GetSquareUnder()->CanBeSeen())
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

void arm::ApplyExperience()
{
  if(GetMaster()->CheckForAttributeIncrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels stronger!", CHARNAME(UNARTICLED));
      else
	if(game::IsInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks stronger.", GetMaster()->CHARNAME(DEFINITE));
    }
  else if(GetMaster()->CheckForAttributeDecrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels weaker.", CHARNAME(UNARTICLED));
      else
	if(game::IsInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks weaker.", GetMaster()->CHARNAME(DEFINITE));
    }

  if(GetMaster()->CheckForAttributeIncrease(Dexterity, DexterityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels quite dextrous.", CHARNAME(UNARTICLED));
      else
	if(game::IsInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks quite dextrous.", GetMaster()->CHARNAME(DEFINITE));
    }
  else if(GetMaster()->CheckForAttributeDecrease(Dexterity, DexterityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels clumsy.", CHARNAME(UNARTICLED));
      else
	if(game::IsInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks clumsy.", GetMaster()->CHARNAME(DEFINITE));
    }
}

void leg::ApplyExperience()
{
  if(GetMaster()->CheckForAttributeIncrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels stronger!", CHARNAME(UNARTICLED));
      else
	if(game::IsInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks stronger.", GetMaster()->CHARNAME(DEFINITE));
    }
  else if(GetMaster()->CheckForAttributeDecrease(Strength, StrengthExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels weaker.", CHARNAME(UNARTICLED));
      else
	if(game::IsInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks weaker.", GetMaster()->CHARNAME(DEFINITE));
    }

  if(GetMaster()->CheckForAttributeIncrease(Agility, AgilityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels very agile!", CHARNAME(UNARTICLED));
      else
	if(game::IsInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks very agile.", GetMaster()->CHARNAME(DEFINITE));
    }
  else if(GetMaster()->CheckForAttributeDecrease(Agility, AgilityExperience))
    {
      if(GetMaster()->IsPlayer())
	ADD_MESSAGE("Your %s feels slower.", CHARNAME(UNARTICLED));
      else
	if(game::IsInWilderness() || GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("Suddenly %s looks sluggish.", GetMaster()->CHARNAME(DEFINITE));
    }
}

void arm::Hit(character* Enemy, float AttackStrength, float ToHitValue)
{
  switch(Enemy->TakeHit(GetMaster(), GetWielded(), AttackStrength, ToHitValue, RAND() % 26 - RAND() % 26, GetWielded() ? WEAPONATTACK : UNARMEDATTACK, !(RAND() % GetMaster()->GetCriticalModifier())))
    {
    case HASHIT:
    case HASBLOCKED:
      if(GetWielded())
	GetWielded()->ReceiveHitEffect(Enemy, GetMaster());
    case HASDIED:
      EditExperience(ARMSTRENGTH, 50);

      if(GetHumanoidMaster()->GetCategoryWeaponSkill(GetWielded() ? GetWielded()->GetWeaponCategory() : UNARMED)->AddHit() && GetMaster()->IsPlayer())
	GetHumanoidMaster()->GetCategoryWeaponSkill(GetWielded() ? GetWielded()->GetWeaponCategory() : UNARMED)->AddLevelUpMessage();

      if(GetWielded())
	{
	  if(GetCurrentSingleWeaponSkill()->AddHit() && GetMaster()->IsPlayer())
	    GetCurrentSingleWeaponSkill()->AddLevelUpMessage(GetWielded()->GetName(UNARTICLED));

	  GetWielded()->ReceiveDamage(GetMaster(), GetAttribute(ARMSTRENGTH), PHYSICALDAMAGE);
	}
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
	return GetMainMaterial()->IsFlexible() ? 10 : 1;
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
	return GetMainMaterial()->IsFlexible() ? 10 : 1;
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

  std::string Question = "Do you want to (t)ake something from or (p) something in this chest? [t,p]";

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

  if(ToBeTaken && (!RoomNumber || GetLSquareUnder()->GetLevelUnder()->GetRoom(RoomNumber)->PickupItem(Opener,this)))
    {
      ToBeTaken->MoveTo(Opener->GetStack());
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

      if((RoomNumber == 0 || GetLSquareUnder()->GetLevelUnder()->GetRoom(RoomNumber)->DropItem(Opener, this)))
	{
	  if(FitsIn(ToBePut))
	    {
	      ToBePut->MoveTo(GetContained());
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

/*ulong chest::GetWeight() const
{
  return item::GetWeight() + GetContained()->GetWeight();
}*/

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

std::string key::GetAdjective() const
{
  return game::GetLockDescription(LockType);
}

ushort head::GetEmitation() const
{
  ushort Emitation = item::GetEmitation();

  if(GetHelmet() && GetHelmet()->GetEmitation() > Emitation)
    Emitation = GetHelmet()->GetEmitation();

  if(GetAmulet() && GetAmulet()->GetEmitation() > Emitation)
    Emitation = GetAmulet()->GetEmitation();

  return Emitation;
}

ushort humanoidtorso::GetEmitation() const
{
  ushort Emitation = item::GetEmitation();

  if(GetBodyArmor() && GetBodyArmor()->GetEmitation() > Emitation)
    Emitation = GetBodyArmor()->GetEmitation();

  if(GetCloak() && GetCloak()->GetEmitation() > Emitation)
    Emitation = GetCloak()->GetEmitation();

  if(GetBelt() && GetBelt()->GetEmitation() > Emitation)
    Emitation = GetBelt()->GetEmitation();

  return Emitation;
}

ushort arm::GetEmitation() const
{
  ushort Emitation = item::GetEmitation();

  if(GetWielded() && GetWielded()->GetEmitation() > Emitation)
    Emitation = GetWielded()->GetEmitation();

  if(GetGauntlet() && GetGauntlet()->GetEmitation() > Emitation)
    Emitation = GetGauntlet()->GetEmitation();

  if(GetRing() && GetRing()->GetEmitation() > Emitation)
    Emitation = GetRing()->GetEmitation();

  return Emitation;
}

ushort leg::GetEmitation() const
{
  ushort Emitation = item::GetEmitation();

  if(GetBoot() && GetBoot()->GetEmitation() > Emitation)
    Emitation = GetBoot()->GetEmitation();

  return Emitation;
}

void head::InitSpecialAttributes()
{
  SetBiteStrength(GetMaster()->GetBiteStrength());
}

void arm::InitSpecialAttributes()
{
  SetStrength(GetMaster()->GetDefaultArmStrength());
  SetDexterity(GetMaster()->GetDefaultDexterity());
  SetUnarmedStrength(GetMaster()->GetUnarmedStrength());
}

void leg::InitSpecialAttributes()
{
  SetStrength(GetMaster()->GetDefaultLegStrength());
  SetAgility(GetMaster()->GetDefaultAgility());
  SetKickStrength(GetMaster()->GetKickStrength());
}

std::string corpse::GetConsumeVerb() const
{
  return GetDeceased()->GetTorso()->GetConsumeVerb();
}

void chest::SetSquareUnder(square* Square)
{
  item::SetSquareUnder(Square);
  GetContained()->SetSquareUnder(Square);
}

/* Victim is of course the stuck person, Bodypart is the index of the bodypart that the trap is stuck to and the last vector2d is just a direction vector that may - or may not - be used in the future. This function returns true if the character manages to escape */
bool beartrap::TryToUnstuck(character* Victim, ushort BodyPart, vector2d)
{
  if(!(RAND() % 5))
    {

      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to hurt your %s even more.", Victim->GetBodyPart(BodyPart)->CHARNAME(UNARTICLED));
      else if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s hurts %s %s more with %s", Victim->CHARNAME(DEFINITE), Victim->PossessivePronoun().c_str(), Victim->GetBodyPart(BodyPart)->CHARNAME(DEFINITE), CHARNAME(DEFINITE));
      Victim->ReceiveBodyPartDamage(0,RAND() % 3 + 1, PHYSICALDAMAGE, BodyPart);
      std::string DeathMessage = "died while trying to escape from " + GetName(DEFINITE) + ".";
      Victim->CheckDeath(DeathMessage);
      return false;
    }
  if(!(RAND() % 3))
    {
      Victim->SetStuckTo(0);
      Victim->SetStuckToBodyPart(NONEINDEX);
      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to free yourself from %s", CHARNAME(DEFINITE));
      else if(Victim->GetLSquareUnder()->CanBeSeen())
	{
	  std::string msg = Victim->GetName(DEFINITE) + " manages to free " + Victim->PersonalPronoun() + "self from " + GetName(DEFINITE) + ".";
	  ADD_MESSAGE("%s", msg.c_str());
	}
      return true;
    }
  ADD_MESSAGE("You are unable to escape from %s", CHARNAME(DEFINITE));
  return false;
}

void beartrap::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> IsActivated;
}

void beartrap::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << IsActivated;
}

void beartrap::VirtualConstructor(bool Load)
{
  item::VirtualConstructor(Load);
  IsActivated = RAND() % 2;
}

bool beartrap::GetStepOnEffect(character* Stepper)
{
  if(IsActivated)
    {
      ushort StepperBodyPart = Stepper->GetRandomStepperBodyPart();

      if(StepperBodyPart == NONEINDEX)
	return false;

      Stepper->SetStuckTo(this);
      Stepper->SetStuckToBodyPart(StepperBodyPart);
      if(Stepper->IsPlayer())
	ADD_MESSAGE("You step in %s and it traps your %s. ", CHARNAME(INDEFINITE), Stepper->GetBodyPart(Stepper->GetStuckToBodyPart())->CHARNAME(UNARTICLED));
      else if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s is trapped in %s", Stepper->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

      Stepper->ReceiveBodyPartDamage(0,RAND() % 3 + 1, PHYSICALDAMAGE, Stepper->GetStuckToBodyPart());
      Stepper->CheckDeath("died stepping to " + GetName(INDEFINITE) + ".");
      IsActivated = false;
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
	ADD_MESSAGE("%s is tightly stuck to %s", CHARNAME(DEFINITE), Picker->GetBodyPart(Picker->GetStuckToBodyPart())->CHARNAME(UNARTICLED));
      return false;
    }
  IsActivated = false; /* Just in case something wierd is going on */
  return true;
}

/*bool wandofdoorcreation::BeamEffect(character* Who, const std::string&, uchar, lsquare* Where) 
{ 
  return Where->LockEverything(Who); 
}*/

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
    if(game::IsValidPos(Zapper->GetPos() + Pos[c]))
      {
	lsquare* Square = GetLSquareUnder()->GetLevelUnder()->GetLSquare(Zapper->GetPos() + Pos[c]);

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

bool beartrap::IsPickable(character* Picker) const
{
  return Picker->GetStuckTo() != this;
}
