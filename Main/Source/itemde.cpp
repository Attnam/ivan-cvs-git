#define __FILE_OF_STATIC_ITEM_PROTOTYPE_DECLARATIONS__

#include "proto.h"

class item;

std::vector<item*>			protocontainer<item>::ProtoData;
std::map<std::string, ushort>		protocontainer<item>::CodeNameMap;

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
#include "strover.h"
#include "whandler.h"
#include "lterrade.h"

void can::PositionedDrawToTileBuffer(uchar) const
{
  Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

item* can::TryToOpen(character* Opener)
{
  if(Opener->GetStrength() > RAND() % 30)
    {
      item* Item = new lump(GetContainedMaterial());
      DonateSlotTo(Item);

      if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
	  Opener->GetLSquareUnder()->GetStack()->AddItem(this);
      else
	  Item->GetSlot()->AddFriendItem(this);

      SetMaterial(GetContainedMaterialIndex(), 0);
      UpdatePicture();

      return Item;
    }
  else
    {
      if(Opener->GetIsPlayer())
	ADD_MESSAGE("The can is shut tight.");

      return 0;
    }
}

bool banana::Consume(character* Eater, float Amount)
{
  GetConsumeMaterial()->EatEffect(Eater, Amount, NPModifier());

  if(!Cannibalised && Eater->GetIsPlayer() && Eater->CheckCannibalism(GetConsumeMaterial()->GetType()))
    {
      game::DoEvilDeed(25);
      ADD_MESSAGE("You feel that this was an evil deed.");
      Cannibalised = true;
    }

  if(!GetConsumeMaterial()->GetVolume())
    {
      item* Peals = new bananapeals(false);
      Peals->InitMaterials(GetMaterial(0));
      PreserveMaterial(0);

      if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
	Eater->GetLSquareUnder()->GetStack()->AddItem(Peals);
      else
	Eater->GetStack()->AddItem(Peals);
    }

  return GetConsumeMaterial()->GetVolume() ? false : true;
}

bool potion::Consume(character* Eater, float Amount)
{
  GetConsumeMaterial()->EatEffect(Eater, Amount, NPModifier());

  if(!Cannibalised && Eater->GetIsPlayer() && Eater->CheckCannibalism(GetConsumeMaterial()->GetType()))
    {
      game::DoEvilDeed(25);
      ADD_MESSAGE("You feel that this was an evil deed.");
      Cannibalised = true;
    }

  ushort Emit = GetEmitation();

  if(!GetConsumeMaterial()->GetVolume())
    ChangeMaterial(GetConsumeMaterialIndex(), 0);

  if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
      MoveTo(Eater->GetLSquareUnder()->GetStack());

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();

      if(!game::GetInWilderness())
	GetLSquareUnder()->SignalEmitationDecrease(Emit);

      GetSquareUnder()->SendNewDrawRequest();
      GetSquareUnder()->SendMemorizedUpdateRequest();

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorized();
    }

  return false;
}

void lantern::PositionedDrawToTileBuffer(uchar LSquarePosition) const
{
  switch(LSquarePosition)
    {
    case CENTER:
    case DOWN:
      Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
      break;
    case LEFT:
      Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, uchar(ROTATE_90));
      break;
    case UP:
      Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, uchar(FLIP));
      break;
    case RIGHT:
      Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, uchar(ROTATE_90 | MIRROR));
      break;
    }
}

bool scroll::CanBeRead(character* Reader) const
{
  return Reader->CanRead();
}

bool scrollofcreatemonster::Read(character* Reader)
{
  vector2d TryToCreate;

  for(ushort c = 0; c < 100; ++c)
    {
      TryToCreate = Reader->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

      character* Monster = protosystem::CreateMonster();

      if(game::IsValidPos(TryToCreate) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetIsWalkable(Monster) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetCharacter() == 0)
	{
	  game::GetCurrentLevel()->GetLSquare(TryToCreate)->AddCharacter(Monster);

	  if(Reader->GetIsPlayer())
	    ADD_MESSAGE("As you read the scroll a monster appears.");
	  else
	    if(Reader->GetLSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("The %s reads %s. A monster appears!", Reader->CHARNAME(DEFINITE), CHARNAME(DEFINITE));

	  return true;
	}
      else
	delete Monster;
    }

  ADD_MESSAGE("You feel a lost soul fly by you.");
  return true;
}

bool scrollofteleport::Read(character* Reader)
{
  if(Reader->GetIsPlayer())
    ADD_MESSAGE("After you have read the scroll you realize that you have teleported.");
  else
    if(Reader->GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("The %s reads %s and disappears!", Reader->CHARNAME(DEFINITE), CHARNAME(DEFINITE));

  Reader->Teleport();
  return true;
}

void lump::ReceiveHitEffect(character* Enemy, character*)
{
  if(RAND() % 2)
    {
      if(Enemy->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("The %s touches %s.", GetMainMaterial()->CHARNAME(UNARTICLED), Enemy->CHARNAME(DEFINITE));

      GetMainMaterial()->HitEffect(Enemy);
    }
}

void meleeweapon::ReceiveHitEffect(character* Enemy, character*)
{
  if(GetMaterial(2))
    {
      if(Enemy->GetIsPlayer())
	ADD_MESSAGE("The %s reacts with you!", GetMaterial(2)->CHARNAME(UNARTICLED));
      else
	if(Enemy->GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("The %s reacts with %s.", GetMaterial(2)->CHARNAME(UNARTICLED), Enemy->CHARNAME(DEFINITE));

      GetMaterial(2)->HitEffect(Enemy);
    }
}

void meleeweapon::DipInto(material* Material, character* Dipper)
{
  ChangeMaterial(2, Material);

  if(Dipper->GetIsPlayer())
    ADD_MESSAGE("%s is now covered with %s.", CHARNAME(DEFINITE), Material->CHARNAME(UNARTICLED));
}

material* lump::CreateDipMaterial()
{
  return GetMainMaterial()->Clone(GetMainMaterial()->TakeDipVolumeAway());
}

bool potion::ImpactDamage(ushort)
{
  item* Remains = new brokenbottle(false);
  if(GetContainedMaterial()) 
    GetLSquareUnder()->SpillFluid(5, GetContainedMaterial()->GetColor());
  Remains->InitMaterials(GetMaterial(0));
  SetMaterial(0,0);
  DonateSlotTo(Remains);
  if(GetSquareUnder()->CanBeSeen())
    ADD_MESSAGE("The potion shatters to pieces.");
  SetExists(false);
  return true;
}

void potion::PositionedDrawToTileBuffer(uchar) const
{
  Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

item* can::PrepareForConsuming(character* Consumer)
{
  if(!Consumer->GetIsPlayer() || game::BoolQuestion(std::string("Do you want to open ") + CHARNAME(DEFINITE) + " before eating it? [Y/n]", 'y'))
    return TryToOpen(Consumer);
  else
    return 0;
}

item* leftnutofpetrus::CreateWishedItem() const
{
  return new cheapcopyofleftnutofpetrus;
}

bool pickaxe::Apply(character* User)
{
  vector2d Temp = game::AskForDirectionVector("What direction do you want to dig?");
	
  if(Temp != vector2d(0,0) && game::IsValidPos(User->GetPos() + Temp))
    {
      lsquare* Square = game::GetCurrentLevel()->GetLSquare(User->GetPos() + Temp);

      if(Square->CanBeDug())
	if(Square->GetOLTerrain()->CanBeDug())
	  if(Square->GetOLTerrain()->GetMainMaterial()->CanBeDug(GetMainMaterial()))
	    {
	      User->SwitchToDig(this, User->GetPos() + Temp);
	      return true;
	    }
	  else
	    ADD_MESSAGE("%s is too hard to dig.", Square->GetOLTerrain()->CHARNAME(DEFINITE));
	else
	  ADD_MESSAGE(Square->GetOLTerrain()->DigMessage().c_str());
    }

  return false;
}

/*ushort platemail::GetArmorValue() const
{
  float Base = 80 - sqrt(Material[0]->GetHitValue()) * 3;

  if(Base < 1)
    Base = 1;

  if(Base > 100)
    Base = 100;

  return ushort(Base);
}

ushort chainmail::GetArmorValue() const
{
  float Base = 90 - sqrt(Material[0]->GetHitValue()) * 2;

  if(Base < 1)
    Base = 1;

  if(Base > 100)
    Base = 100;

  return ushort(Base);
}

ushort brokenplatemail::GetArmorValue() const
{
  float Base = 95 - sqrt(Material[0]->GetHitValue());

  if(Base < 1)
    Base = 1;

  if(Base > 100)
    Base = 100;

  return ushort(Base);
}*/

bool wand::Apply(character* Terrorist)
{
  if(Terrorist->GetIsPlayer())
    ADD_MESSAGE("%s breaks in two and then explodes!", CHARNAME(DEFINITE));
  else
    if(Terrorist->GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s breaks %s in two. It explodes!", Terrorist->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

  RemoveFromSlot();
  SetExists(false);

  std::string DeathMsg;

  if(Terrorist->GetIsPlayer())
    DeathMsg = "exploded himself by breaking a wand";
  else
    DeathMsg = std::string("kamikazed by ") + Terrorist->Name(INDEFINITE);

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
  Zapper->EditPerceptionExperience(50);
  Zapper->EditAP(500);
  return true;
}

void wand::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << TimesUsed;
  SaveFile << Charges;
}

void wand::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> TimesUsed;
  SaveFile >> Charges;
}

bool scrollofwishing::Read(character* Reader)
{
  EMPTY_MESSAGES();
  game::DrawEverythingNoBlit();
  std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(7,7), WHITE, 0, 80, false);

  item* TempItem = protosystem::CreateItem(Temp, Reader->GetIsPlayer());

  if(TempItem)
    {
      Reader->GetStack()->AddItem(TempItem);
      ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CHARNAME(INDEFINITE));
      Reader->EditAP(-2500); // very difficult to read
      return true;
    }

  return false;
}

bool lantern::ImpactDamage(ushort)
{
  brokenlantern* Lantern = new brokenlantern(false);
  Lantern->InitMaterials(GetMaterial(0));
  PreserveMaterial(0);
  Lantern->SignalSquarePositionChange(OnWall);
  DonateSlotTo(Lantern);
  if(GetSquareUnder()->CanBeSeen())
    ADD_MESSAGE("The lantern shatters to pieces.");
  SetExists(false);
  return true;
}

bool lantern::ReceiveSound(float Strength)
{
  if(!(RAND() % 75) && Strength > 10 + RAND() % 10)
    {
      ImpactDamage(ushort(Strength));
      return true;
    }

  return false;	
}

bool potion::ReceiveSound(float Strength)
{
  if(!(RAND() % 75) && Strength > 10 + RAND() % 10)
    {
      ImpactDamage(ushort(Strength));
      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("The potion is destroyed by the sound.");
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

  EMPTY_MESSAGES();
  game::DrawEverythingNoBlit();
  std::string Temp = game::StringQuestion("What material do you want to wish for?", vector2d(7,7), WHITE, 0, 80, false);

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
    Stuff = new bananaflesh;
  else
    Stuff = new omleurine;

  item* P = new potion(false); 
  P->InitMaterials(2, new glass, Stuff); 

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
  Zapper->EditPerceptionExperience(50);
  Zapper->EditAP(500);
  return true;
}

bool platemail::ReceiveSound(float Strength)
{
  if(Strength > 20000 + RAND() % 40000)
    {
      ImpactDamage(ushort(Strength));

      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("The plate mail is damaged by the loud sound.");

      return true;
    }

  return false;	
}

bool platemail::ImpactDamage(ushort Strength)
{
  /*if(Strength > 2500.0f / GetStrengthValue() + RAND() % 11 - RAND() % 11)
    {
      if (IsShown)
	ADD_MESSAGE("%s is damaged.", CHARNAME(DEFINITE));

      ItemStack->RemoveItem(ItemStack->SearchItem(this));
      item* Plate = new brokenplatemail(false);
      Plate->InitMaterials(GetMaterial(0));
      ItemStack->AddItem(Plate);
      SetMaterial(0,0);
      SetExists(false);
      return true;
    }
  else*/
    return false;
}

bool brokenbottle::GetStepOnEffect(character* Stepper)
{
  if(!(RAND() % 10))
    {
      if(Stepper->ReceiveEffect(1 + RAND() % 2, PHYSICALDAMAGE, LEGS))
	{
	  if(Stepper->GetIsPlayer())
	    ADD_MESSAGE("Auch. You step on sharp glass splinters.");
	  else
	    if(Stepper->GetSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s steps on sharp glass splinters and is hurt.", Stepper->CHARNAME(DEFINITE));
	}
      else
	{
	  if(Stepper->GetIsPlayer())
	    ADD_MESSAGE("Some glass splinters are shattered under your feet.");
	  else
	    if(Stepper->GetSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("Some glass splinters are shattered under %s's feet.", Stepper->CHARNAME(DEFINITE));
	}

      //Stepper->SetHP(Stepper->GetHP() - RAND() % 2 - 1);
      Stepper->CheckDeath("stepped on a broken bottle");
    }

  return false;
}

material* corpse::CreateDipMaterial()
{
  return GetMainMaterial()->Clone(GetMainMaterial()->TakeDipVolumeAway());
}

material* potion::CreateDipMaterial()
{
  return GetContainedMaterial()->Clone(GetContainedMaterial()->TakeDipVolumeAway());
}

void potion::DipInto(material* Material, character* Dipper)
{
  ChangeMaterial(1, Material);

  if(Dipper->GetIsPlayer())
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
  UpdatePicture();
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

void corpse::SetBloodColor(ushort Color)
{
  /*igraph::RemoveUser(GraphicId);
  GraphicId.Color[1] = Color;
  Picture = igraph::AddUser(GraphicId).Bitmap;*/
}

item* potion::BetterVersion() const
{
  if(!GetContainedMaterial())
    {
      material* Stuff;

      if(RAND() % 5)
	Stuff = new bananaflesh;
      else
	Stuff = new omleurine;

      item* P = new potion(false); 
      P->InitMaterials(2, new glass, Stuff);

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
	Stuff = new schoolfood;
      else
	Stuff = new bananaflesh;

      item* P = new can(false); 
      P->InitMaterials(2, new iron, Stuff); 

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
      if(Terrorist->GetIsPlayer())
	ADD_MESSAGE("You light your %s. It explodes!", CHARNAME(UNARTICLED));
      else
	if(Terrorist->GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s lights %s. It explodes!", Terrorist->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));

      RemoveFromSlot();
      SetExists(false);

      std::string DeathMsg;

      if(Terrorist->GetIsPlayer())
	DeathMsg = std::string("exploded himself with ") + Name(INDEFINITE);
      else
	DeathMsg = std::string("kamikazed by ") + Terrorist->Name(INDEFINITE);

      Terrorist->GetLSquareUnder()->GetLevelUnder()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), GetContainedMaterial()->ExplosivePower());
      return true;
    }
  else
    if(Terrorist->GetIsPlayer())
      ADD_MESSAGE("You can't apply this!");	

  return false;
}

void holybook::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> OwnerGod;
}

void holybook::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << OwnerGod;
}

bool holybook::CanBeRead(character* Reader) const
{
  return Reader->CanRead();
}

bool holybook::Read(character* Reader)
{
  if(Reader->GetIsPlayer())
    {
      if(game::GetGod(GetOwnerGod())->GetKnown())
	{
	  ADD_MESSAGE("The book reveals many divine secrets of %s to you.", game::GetGod(OwnerGod)->GOD_NAME);
	  game::GetGod(OwnerGod)->AdjustRelation(75);
	  game::ApplyDivineAlignmentBonuses(game::GetGod(OwnerGod), true);

	  if(RAND() % 3)
	    return false;
	  else
	    {
	      ADD_MESSAGE("But then it disappears.");
	      return true;
	    }
	}

      game::GetGod(GetOwnerGod())->SetKnown(true);
      ADD_MESSAGE("You read the book through and feel that you master the magical rites of %s.", game::GetGod(OwnerGod)->GOD_NAME);
    }

  return false;
}

bool wand::ReceiveFireDamage(character* Burner, std::string DeathMsg, stack* MotherStack, long)
{
  if(!(RAND() % 10))
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s catches fire and explodes!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SetExists(false);
      MotherStack->GetLSquareUnder()->GetLevelUnder()->Explosion(Burner, DeathMsg, MotherStack->GetLSquareUnder()->GetPos(), 40);
      return true;
    }
  else
    return false;
}

bool backpack::ReceiveFireDamage(character* Burner, std::string DeathMsg, stack* MotherStack, long)
{
  if(!(RAND() % 3))
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes in the heat!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SetExists(false);
      MotherStack->GetLSquareUnder()->GetLevelUnder()->Explosion(Burner, DeathMsg, MotherStack->GetLSquareUnder()->GetPos(), GetContainedMaterial()->ExplosivePower());
      return true;
    }
  else
    return false;
}

std::string wand::PostFix() const
{
  if(!TimesUsed)
    return "";
  else if(TimesUsed == 1)
    return "(used 1 time)";
  else
    return std::string("(used ") + TimesUsed + " times)";
}

bool scroll::ReceiveFireDamage(character*, std::string, stack* MotherStack, long)
{
  if(!(RAND() % 10) && GetMainMaterial()->IsFlammable())
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s catches fire!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SetExists(false);
      return true;
    }
  else
    return false;
}

void wand::Beam(character* Zapper, std::string DeathMsg, uchar Direction, uchar Range)
{
  vector2d CurrentPos = Zapper->GetPos();

  if(Direction != '.')
    for(ushort Length = 0; Length < Range; ++Length)
      {
	if(!game::GetCurrentLevel()->IsValid(CurrentPos + game::GetMoveVector(Direction)))
	  break;

	lsquare* CurrentSquare = game::GetCurrentLevel()->GetLSquare(CurrentPos + game::GetMoveVector(Direction));

	if(!(CurrentSquare->GetOLTerrain()->GetIsWalkable()))
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

bool wandofpolymorph::BeamEffect(character* Zapper, std::string, uchar, lsquare* LSquare)
{
  LSquare->PolymorphEverything(Zapper);
  return false;
}

bool wandofstriking::BeamEffect(character* Who, std::string DeathMsg, uchar Dir, lsquare* Where) 
{ 
  Where->StrikeEverything(Who, DeathMsg, Dir); 
  return false;
}

bool holybook::ReceiveFireDamage(character*, std::string, stack* MotherStack, long)
{
  if(!(RAND() % 2) && GetMainMaterial()->IsFlammable())
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s catches fire!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SetExists(false);
      return true;
    }
  else
    return false;
}

bool wand::StruckByWandOfStriking(character* Striker, std::string DeathMsg) 
{ 
  if(!(RAND() % 10))
    {
      if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SetExists(false);
      GetLSquareUnder()->GetLevelUnder()->Explosion(Striker, DeathMsg, GetLSquareUnder()->GetPos(), 40);
      return true;
    }
  else
    return false;
}

bool backpack::StruckByWandOfStriking(character* Striker, std::string DeathMsg) 
{ 
  if(RAND() % 3)
    {
      if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SetExists(false);
      GetLSquareUnder()->GetLevelUnder()->Explosion(Striker, DeathMsg, GetLSquareUnder()->GetPos(), GetContainedMaterial()->ExplosivePower());
      return true;
    }
  else
    return false;
}

material* loaf::CreateLoafMaterials()
{
  switch(RAND() % 5)
    {
    case 0:
      return new beef;
    case 1:
      return new pork;
    default:
      return new bread;
    }
}

bool oillamp::Apply(character* Applier)
{
  if(Applier->GetIsPlayer())
    ADD_MESSAGE("You rub %s.", CHARNAME(DEFINITE));

  if(GetInhabitedByGenie())
    {
      character* Genie = new genie;
      vector2d TryToCreate;
      bool FoundPlace = false;

      /* First try to create a genie nearby (10 tries - if all of them fail then stop trying) */

      for(ushort c = 0; c < 10 && !FoundPlace; c++)
	{	  
	  TryToCreate = Applier->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

	  if(game::IsValidPos(TryToCreate) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetIsWalkable(Genie) && game::GetCurrentLevel()->GetLSquare(TryToCreate)->GetCharacter() == 0)
	    {
	      game::GetCurrentLevel()->GetLSquare(TryToCreate)->AddCharacter(Genie);
	      FoundPlace = true;
	      SetInhabitedByGenie(false);
	      break;
	    }
	}

      if(FoundPlace)
	{
	  ADD_MESSAGE("You see a puff of smoke, and %s appears.", Genie->CHARNAME(INDEFINITE));
	  ADD_MESSAGE("\"For centuries I have been imprisoned in this lamp. But at last you have freed me!");

	  if(!(RAND() % 5))
	    {
	      Genie->SetTeam(game::GetTeam(1));
	      ADD_MESSAGE("As a reward, I will kill you.\"");
	    }
	  else
	    {
	      if(Applier->GetIsPlayer())
		{
		  Genie->SetTeam(Applier->GetTeam());
		  ADD_MESSAGE("I am deeply grateful. You deserve a generous reward.");
		  ADD_MESSAGE("I may serve you for 1001 nights or grant you a wish. Its your choice.\"");
		  game::DrawEverything();
		  GETKEY();

		  if(game::BoolQuestion("Do you want to wish? [Y/n]", 'y'))
		    {
		      ADD_MESSAGE("You may wish for an item. - press any key -");
		      game::DrawEverything();
		      GETKEY();

		      while(true)
			{
			  std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(7,7), WHITE, 0, 80, false);
			  item* TempItem = protosystem::CreateItem(Temp, Applier->GetIsPlayer());

			  if(TempItem)
			    {
			      Applier->GetStack()->AddItem(TempItem);
			      ADD_MESSAGE("%s appears from nothing and the genie flies happily away!", TempItem->CHARNAME(INDEFINITE));
			      break;
			    }
			  else
			    {
			      ADD_MESSAGE("You may try again. - press any key -");
			      DRAW_MESSAGES();
			      game::DrawEverything();
			      GETKEY();
			    }
			}

			game::GetCurrentLevel()->RemoveCharacter(TryToCreate);
			delete Genie;
			return true;
		    }
		}
	    }

	    item* Sword = new curvedtwohandedsword;
	    ADD_MESSAGE("%s wishes for %s.", Genie->CHARNAME(DEFINITE), Sword->CHARNAME(INDEFINITE));
	    ADD_MESSAGE("%s appears from nothing.", Sword->CHARNAME(INDEFINITE));
	    ADD_MESSAGE("%s wields %s.", Genie->CHARNAME(DEFINITE), Sword->CHARNAME(DEFINITE));
	    Genie->SetMainWielded(Sword);
	}
      else
	{
	  if(Applier->GetIsPlayer())
	    ADD_MESSAGE("You feel that it is warmer.");

	  delete Genie;
	}
    }
  else
    if(Applier->GetIsPlayer())
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

void holybook::SetOwnerGod(uchar NewOwnerGod)
{
  switch(game::GetGod(NewOwnerGod)->BasicAlignment())
    {
    case GOOD:
      SetMaterial(0, new goodleather);
      break;
    case NEUTRAL:
      SetMaterial(0, new neutralleather);
      break;
    case EVIL:
      SetMaterial(0, new evilleather);
      break;
    }

  OwnerGod = NewOwnerGod;
}

void astone::GenerateStoneMaterials()
{ 
  switch(RAND() % 5) 
    {
    case 0: InitMaterials(new gold); break;
    case 1: InitMaterials(new sapphire); break;
    case 2: InitMaterials(new silver); break;
    case 3: InitMaterials(new ruby); break;
    case 4: InitMaterials(new diamond); break;
    }
}

bool scrollofcharging::Read(character* Reader)
{
  if(Reader->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to charge.");
      return false;
    }

  item* Item = Reader->GetStack()->DrawContents(Reader, "What item do you wish to charge?", item::ChargeableSorter);

  if(!Item)
    return false;

  if(Item == this)
    {
      ADD_MESSAGE("This would cause a rift to appear in the space-time continuum and that wouldn't be nice.");
      return false;
    }

  if(!Item->IsChargable())
    {
      ADD_MESSAGE("You can't charge %s.", Item->CHARNAME(INDEFINITE));
      return false;
    }

  EMPTY_MESSAGES();
  game::DrawEverythingNoBlit();
  Item->ChargeFully(Reader);
  ADD_MESSAGE("You charge %s and the scroll burns.", Item->CHARNAME(DEFINITE));
  return true;
}

void banana::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Charges;
}

void banana::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
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
      if(Stepper->GetIsPlayer())
	ADD_MESSAGE("Auch. Your feet slip on %s and you fall down.", CHARNAME(INDEFINITE));
      else
	if(Stepper->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s steps on %s and falls down.", Stepper->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));
      Stepper->EditAP(1000);
      /* Do damage against any random bodypart except legs */
      Stepper->ReceiveEffect(1 + RAND() % 2, PHYSICALDAMAGE, ALL&~LEGS);
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
  Zapper->EditPerceptionExperience(50);
  Zapper->EditAP(500);
  return true;
}

bool wandoffireballs::BeamEffect(character* Who, std::string DeathMsg, uchar, lsquare* Where) 
{ 
  if(!Where->GetOTerrain()->GetIsWalkable() || Where->GetCharacter())
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
  for(ushort c = 0; c < 8; c++)
    {
      vector2d Test = Reader->GetPos() + game::GetMoveVector(c);
      if(game::IsValidPos(Test))
	{
	  character* CharacterInSquare = game::GetCurrentLevel()->GetLSquare(Test)->GetCharacter();
	  if(CharacterInSquare && CharacterInSquare->Charmable() && CharacterInSquare->GetTeam() != Reader->GetTeam())
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
  item::Save(SaveFile);
  SaveFile << BitmapPos << Color[0] << Color[1] << Color[2] << Color[3] << HP << OwnerDescription << Unique;
}

void bodypart::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> BitmapPos >> Color[0] >> Color[1] >> Color[2] >> Color[3] >> HP >> OwnerDescription >> Unique;
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
  Zapper->EditPerceptionExperience(50);
  Zapper->EditAP(500);
  return true;
}

bool wandofteleportation::BeamEffect(character* Who, std::string DeathMsg, uchar, lsquare* Where) 
{ 
  Where->TeleportEverything(Who);
  return false;
}

ushort bodypart::GetStrengthValue() const
{
  if(GetMaster() && GetMainMaterial()->IsAlive())
    return ulong(GetStrengthModifier()) * GetMaster()->GetEndurance() / 1000;
  else
    return ulong(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 1000;
}

short bodypart::GetMaxHP() const
{
  if(GetMaster())
    {
      short HP = 0;

      if(GetMainMaterial()->IsAlive())
	for(ushort c = 0; c < GetMaterials(); ++c)
	  {
	    if(GetMaterial(c))
	      HP += ulong(GetMaterial(c)->GetVolume()) * GetMaster()->GetEndurance() / 10000;
	  }
      else
	for(ushort c = 0; c < GetMaterials(); ++c)
	  {
	    if(GetMaterial(c))
	      HP += ulong(GetMaterial(c)->GetVolume()) * GetMaterial(c)->GetStrengthValue() / 10000;
	  }

      if(HP < 1)
	HP = 1;

      return HP;
    }
  else
    return 0;
}

ushort head::GetArmoredStrengthValue() const
{
  if(GetMaster())
    {
      ushort SV = GetStrengthValue();

      if(GetMaster()->GetHumanoidTorso()->GetBodyArmor())
	SV += GetMaster()->GetHumanoidTorso()->GetBodyArmor()->GetStrengthValue();

      return SV;
    }
  else
    return GetStrengthValue();
}

ushort normaltorso::GetArmoredStrengthValue() const
{
  return GetStrengthValue();
}

ushort humanoidtorso::GetArmoredStrengthValue() const
{
  if(GetMaster())
    {
      ushort SV = GetStrengthValue();

      if(GetBodyArmor())
	SV += GetBodyArmor()->GetStrengthValue();

      return SV;
    }
  else
    return GetStrengthValue();
}

ushort arm::GetArmoredStrengthValue() const
{
  if(GetMaster())
    {
      ushort SV = GetStrengthValue();

      if(GetMaster()->GetHumanoidTorso()->GetBodyArmor())
	SV += GetMaster()->GetHumanoidTorso()->GetBodyArmor()->GetStrengthValue();

      return SV;
    }
  else
    return GetStrengthValue();
}

ushort groin::GetArmoredStrengthValue() const
{
  if(GetMaster())
    {
      ushort SV = GetStrengthValue();

      if(GetMaster()->GetHumanoidTorso()->GetBodyArmor())
	SV += GetMaster()->GetHumanoidTorso()->GetBodyArmor()->GetStrengthValue();

      return SV;
    }
  else
    return GetStrengthValue();
}

ushort leg::GetArmoredStrengthValue() const
{
  if(GetMaster())
    {
      ushort SV = GetStrengthValue();

      if(GetMaster()->GetHumanoidTorso()->GetBodyArmor())
	SV += GetMaster()->GetHumanoidTorso()->GetBodyArmor()->GetStrengthValue();

      return SV;
    }
  else
    return GetStrengthValue();
}

void head::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << HelmetSlot << AmuletSlot;
}

void head::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> HelmetSlot >> AmuletSlot;
  HelmetSlot.SetBodyPart(this);
  AmuletSlot.SetBodyPart(this);
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
  BodyArmorSlot.SetBodyPart(this);
  CloakSlot.SetBodyPart(this);
  BeltSlot.SetBodyPart(this);
}

void arm::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << WieldedSlot << GauntletSlot << RingSlot;
  SaveFile << SingleWeaponSkill;
}

void arm::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> WieldedSlot >> GauntletSlot >> RingSlot;
  WieldedSlot.SetBodyPart(this);
  GauntletSlot.SetBodyPart(this);
  RingSlot.SetBodyPart(this);
  SaveFile >> SingleWeaponSkill;
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
  SaveFile << BootSlot;
}

void leg::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BootSlot;
  BootSlot.SetBodyPart(this);
}

bool bodypart::ReceivePhysicalDamage(short Damage)
{
  if(GetMaster())
    {
      ushort BHP = GetHP();

      if(GetHP() <= Damage)
	if(GetHP() == GetMaxHP() && GetHP() != 1)
	  Damage = GetHP() - 1;

      EditHP(-Damage);

      if(GetHP() <= 0)
	return true;
      else
	if(GetMaster()->GetIsPlayer())
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
  item::Load(SaveFile);
  SaveFile >> Charged;
}

void mine::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Charged;
}

bool mine::ReceiveFireDamage(character* Burner, std::string DeathMsg, stack* MotherStack, long)
{
  if(!(RAND() % 2))
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s activates and explodes!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SetExists(false);
      MotherStack->GetLSquareUnder()->GetLevelUnder()->Explosion(Burner, DeathMsg, MotherStack->GetLSquareUnder()->GetPos(), 30);
      return true;
    }
  else
    return false;
}

bool mine::StruckByWandOfStriking(character* Striker, std::string DeathMsg) 
{ 
  if(!(RAND() % 2))
    {
      if(GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CHARNAME(DEFINITE));

      RemoveFromSlot();
      SetExists(false);
      GetLSquareUnder()->GetLevelUnder()->Explosion(Striker, DeathMsg, GetLSquareUnder()->GetPos(), 30);
      return true;
    }
  else
    return false;
}

bool mine::GetStepOnEffect(character* Stepper)
{
  if(GetCharged())
    {
      if(Stepper->GetIsPlayer())
	ADD_MESSAGE("You hear a faint thumb. You look down. You see %s. It explodes.", CHARNAME(INDEFINITE));
      else if(Stepper->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s steps on %s.", Stepper->CHARNAME(DEFINITE), CHARNAME(INDEFINITE));
      Stepper->GetLSquareUnder()->GetLevelUnder()->Explosion(0, "killed by a land mine", Stepper->GetPos(), 30);
    }

  return false;
}

void potion::GeneratePotionMaterials()
{ 
  switch(RAND() % 3) 
    {
    case 0: InitMaterials(2, new glass, 0); break;
    case 1: InitMaterials(2, new glass, new omleurine); break;
    case 2: InitMaterials(2, new glass, new healingliquid); break;
    }
}

bool wandofhaste::BeamEffect(character*, std::string, uchar, lsquare* LSquare)
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
  Zapper->EditPerceptionExperience(50);
  Zapper->EditAP(500);
  return true;
}

bool wandofslow::BeamEffect(character*, std::string, uchar, lsquare* LSquare)
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
  Zapper->EditPerceptionExperience(50);
  Zapper->EditAP(500);
  return true;
}

bool key::Apply(character* User)
{
  if(User->GetIsPlayer())
    {
      ADD_MESSAGE("What door do you wish to lock or unlock? [press a direction key or space]");
      game::DrawEverything();
      vector2d ApplyPos = game::AskForDirectionVector() + User->GetPos();
      if(game::GetCurrentLevel()->IsValid(ApplyPos))
	game::GetCurrentLevel()->GetLSquare(ApplyPos)->ReceiveApply(this, User);
      else
	{
	  if(User->GetIsPlayer())
	    ADD_MESSAGE("Can't do that, sir!");
	}
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
  WieldedSlot.SetItem(Item);

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

	  if(Master->GetIsPlayer())
	    for(stackiterator j = Master->GetStack()->GetBottomSlot(); j != Master->GetStack()->GetSlotAboveTop(); ++j)
	      if((*i)->GetID() == (**j)->GetID())
		{
		  (*i)->AddLevelDownMessage((**j)->Name(UNARTICLED));
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

float arm::GetWieldedStrength(bool OneHanded)
{
  float Strength = GetWielded()->GetWeaponStrength() * GetHumanoidMaster()->GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus();

  if(OneHanded)
    Strength *= float(100 - GetWielded()->GetOneHandedStrengthPenalty(GetMaster())) / 100;

  return Strength;
}

float arm::GetMeleeStrength()
{
  /* temporary */

  return 1000 * GetHumanoidMaster()->GetCategoryWeaponSkill(UNARMED)->GetBonus();
}

float arm::GetWieldedToHitValue(bool OneHanded)
{
  float ToHit = GetHumanoidMaster()->GetMeleeAttributeModifier() * GetHumanoidMaster()->GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus() / sqrt(GetWielded()->GetWeight() > 400 ? GetWielded()->GetWeight() : 400) * 10;

  if(OneHanded)
    ToHit *= float(100 - GetWielded()->GetOneHandedToHitPenalty(GetMaster())) / 100;

  return ToHit;
}

float arm::GetMeleeToHitValue()
{
  return (GetMaster()->GetMeleeAttributeModifier() >> 1) * GetHumanoidMaster()->GetCategoryWeaponSkill(UNARMED)->GetBonus();
}

humanoid* bodypart::GetHumanoidMaster() const
{
  return (humanoid*)GetMaster();
}

void cloak::GenerateCloakMaterials()
{ 
  switch(RAND() % 3)
    {
    case 0: InitMaterials(new expensivefabric); break;
    case 1: InitMaterials(new leather); break;
    case 2: InitMaterials(new cloth); break;
    }
}

ushort belt::GetFormModifier() const
{
  if(GetMaterial(0)->IsFlexible())
    return 800;
  else
    return 60;
}

void boot::GenerateBootMaterials()
{ 
  switch(RAND() % 2)
    {
    case 0: InitMaterials(new leather); break;
    case 1: InitMaterials(new iron); break;
    }
}

void gauntlet::GenerateGauntletMaterials()
{ 
  switch(RAND() % 2)
    {
    case 0: InitMaterials(new bronze); break;
    case 1: InitMaterials(new iron); break;
    }
}

void ring::GenerateRingMaterials()
{ 
  switch(RAND() % 3)
    {
    case 0: InitMaterials(new gold); break;
    case 1: InitMaterials(new diamond); break;
    case 2: InitMaterials(new copper); break;
    }
}

void amulet::GenerateAmuletMaterials()
{
  switch(RAND() % 3)
    {
    case 0: InitMaterials(new gold); break;
    case 1: InitMaterials(new diamond); break;
    case 2: InitMaterials(new copper); break;
    }  
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

bool pickaxe::IsAppliable(character* Who) const
{
  return Who->CanWield();
}

