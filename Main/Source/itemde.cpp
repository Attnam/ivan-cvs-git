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

item* can::TryToOpen(character* Opener, stack* Stack)
{
  if(Opener->GetStrength() > RAND() % 30)
    {
      item* Item = new lump(GetMaterial(1));
      Stack->AddItem(Item);
      SetMaterial(1,0);
      UpdatePicture();

      if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
	{
	  ushort Index = Opener->GetStack()->SearchItem(this);

	  if(Index != 0xFFFF)
	    Opener->GetStack()->MoveItem(Index, Opener->GetLSquareUnder()->GetStack());
	}

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
  GetMaterial(1)->EatEffect(Eater, Amount, NPModifier());

  if(!Cannibalised && Eater->GetIsPlayer() && Eater->CheckCannibalism(GetMaterial(1)->GetType()))
    {
      game::DoEvilDeed(25);
      ADD_MESSAGE("You feel that this was an evil deed.");
      Cannibalised = true;
    }

  if(!GetMaterial(1)->GetVolume())
    {
      item* Peals = new bananapeals(false);
      Peals->InitMaterials(GetMaterial(0));
      PreserveMaterial(0);

      if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
	Eater->GetLSquareUnder()->GetStack()->AddItem(Peals);
      else
	Eater->GetStack()->AddItem(Peals);
    }

  return GetMaterial(1)->GetVolume() ? false : true;
}

bool potion::Consume(character* Eater, float Amount)
{
  GetMaterial(1)->EatEffect(Eater, Amount, NPModifier());

  if(!Cannibalised && Eater->GetIsPlayer() && Eater->CheckCannibalism(GetMaterial(1)->GetType()))
    {
      game::DoEvilDeed(25);
      ADD_MESSAGE("You feel that this was an evil deed.");
      Cannibalised = true;
    }

  ushort Emit = GetEmitation();

  if(!GetMaterial(1)->GetVolume())
    ChangeMaterial(1,0);

  if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
    {
      ushort Index = Eater->GetStack()->SearchItem(this);

      if(Index != 0xFFFF)
	Eater->GetStack()->MoveItem(Index, Eater->GetLSquareUnder()->GetStack());
    }

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
	      ADD_MESSAGE("The %s reads %s. A monster appears!", Reader->CNAME(DEFINITE), CNAME(DEFINITE));

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
      ADD_MESSAGE("The %s reads %s and disappears!", Reader->CNAME(DEFINITE), CNAME(DEFINITE));

  Reader->Teleport();
  return true;
}

void lump::ReceiveHitEffect(character* Enemy, character*)
{
  if(RAND() % 2)
    {
      if(Enemy->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("The %s touches %s.", GetMaterial(0)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

      GetMaterial(0)->HitEffect(Enemy);
    }
}

void meleeweapon::ReceiveHitEffect(character* Enemy, character*)
{
  if(GetMaterial(2))
    {
      if(Enemy->GetIsPlayer())
	ADD_MESSAGE("The %s reacts with you!", GetMaterial(2)->CNAME(UNARTICLED));
      else
	if(Enemy->GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("The %s reacts with %s.", GetMaterial(2)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

      GetMaterial(2)->HitEffect(Enemy);
    }
}

void meleeweapon::DipInto(item* DipTo)
{
  ChangeMaterial(2, DipTo->BeDippedInto());
  ADD_MESSAGE("%s is now covered with %s.", CNAME(DEFINITE), GetMaterial(GetMaterials() - 1)->CNAME(UNARTICLED));
}

material* lump::BeDippedInto()
{
  return GetMaterial(0)->Clone(GetMaterial(0)->TakeDipVolumeAway());
}

bool potion::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
  item* Remains = new brokenbottle(false);
  if(GetMaterial(1)) 
    GetLSquareUnder()->SpillFluid(5, GetMaterial(1)->GetColor());
  Remains->InitMaterials(GetMaterial(0));
  SetMaterial(0,0);
  ItemStack->AddItem(Remains);
  ItemStack->RemoveItem(ItemStack->SearchItem(this));
  if (IsShown) ADD_MESSAGE("The potion shatters to pieces.");
  SetExists(false);
  return true;
}

void potion::PositionedDrawToTileBuffer(uchar) const
{
  Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

item* can::PrepareForConsuming(character* Consumer, stack* Stack)
{
  if(!Consumer->GetIsPlayer() || game::BoolQuestion(std::string("Do you want to open ") + CNAME(DEFINITE) + " before eating it? [Y/n]", 'y'))
    return TryToOpen(Consumer, Stack);
  else
    return 0;
}

item* leftnutofpetrus::CreateWishedItem() const
{
  return new cheapcopyofleftnutofpetrus;
}
/*
  
  Few Examples:
  
  (moraine - iron)
  TimeToBeUsed = 4 * (50 - 20) / (1 - 0.2) = 150
  TimeToBeUsed = 4 * (50 - 25) / (1 - 0.2) = 125
  TimeToBeUsed = 4 * (50 - 49) / (1 - 0.2) = 5
  
  (wood - iron) (0.3)
  TimeToBeUsed = 4 * (50 - 20) / (1 - 0.3) = 171
  TimeToBeUsed = 142
  TimeToBeUsed = 5.7

  (morain - wood)
  TimeToBeUsed = 4 * (50 - 20) / (1 - 0.2 / 0.3) = 180
  
  (moraine - valpurium)
  TimeToBeUsed = 4 * (50 - 20) / (1 - 20 / 400) = 126
*/


bool pickaxe::Apply(character* User, stack*)
{
  vector2d Temp;
	
  if((Temp = game::AskForDirectionVector("What direction do you want to dig?")) != vector2d(0,0))
    {
      lsquare* Square = game::GetCurrentLevel()->GetLSquare(User->GetPos() + Temp);

      if(Square->CanBeDigged(User, this))
	if(Square->GetOLTerrain()->GetMaterial(0)->CanBeDigged(GetMaterial(0)))
	  {
	    User->SetSquareBeingDigged(User->GetPos() + Temp);
	    User->SetOldWieldedItem(User->GetWielded());
	    User->SetWielded(this);
	    User->ActivateState(DIGGING);
	    User->SetStateCounter(DIGGING, User->GetStrength() < 50 ? 4 * (50 - User->GetStrength()) / (1 - Square->GetOLTerrain()->GetMaterial(0)->StrengthValue() / GetMaterial(0)->StrengthValue()) : 3);
	    return true;
	  }
	else
	  ADD_MESSAGE("%s is too hard to dig.", Square->GetOLTerrain()->CNAME(DEFINITE));
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

bool wand::Apply(character* Terrorist, stack* MotherStack)
{
  if(Terrorist->GetIsPlayer())
    ADD_MESSAGE("%s breaks in two and then explodes!", CNAME(DEFINITE));
  else
    if(Terrorist->GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s breaks %s in two. It explodes!", Terrorist->CNAME(DEFINITE), CNAME(INDEFINITE));

  MotherStack->RemoveItem(MotherStack->SearchItem(this));
  SetExists(false);

  std::string DeathMsg;

  if(Terrorist->GetIsPlayer())
    DeathMsg = "exploded himself by breaking a wand";
  else
    DeathMsg = "kamikazed by " + Terrorist->Name(INDEFINITE);

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
      ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CNAME(INDEFINITE));
      Reader->EditAP(-2500); // very difficult to read
      return true;
    }

  return false;
}

bool lantern::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
  brokenlantern* Lantern = new brokenlantern(false);
  Lantern->InitMaterials(GetMaterial(0));
  PreserveMaterial(0);
  Lantern->SignalSquarePositionChange(OnWall);
  ItemStack->AddItem(Lantern);
  ItemStack->RemoveItem(ItemStack->SearchItem(this));
  if (IsShown) ADD_MESSAGE("The lantern shatters to pieces.");
  SetExists(false);
  return true;
}

bool lantern::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
  if(!(RAND() % 75) && Strength > 10 + RAND() % 10)
    {
      ImpactDamage(ushort(Strength), Shown, ItemsStack);
      return true;
    }

  return false;	
}

bool potion::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
  if(!(RAND() % 75) && Strength > 10 + RAND() % 10)
    {
      ImpactDamage(ushort(Strength), Shown, ItemsStack);
      if(Shown)
	ADD_MESSAGE("The potion is destroyed by the sound.");
      return true;
    }

  return false;	
}

bool scrollofchangematerial::Read(character* Reader)
{
  ushort Index;

  if((Index = Reader->GetStack()->DrawContents(Reader, "What item do you wish to change?")) == 0xFFFF)
    {
      ADD_MESSAGE("You have nothing to change.");
      return false;
    }

  if(Index == 0xFFFE)
    return false;

  else
    if(!(Index < Reader->GetStack()->GetItems()))
      return false;

  if(Reader->GetStack()->GetItem(Index) == this)
    {
      ADD_MESSAGE("That would be rather insane.");
      return false;
    }

  if(!Reader->GetStack()->GetItem(Index)->IsMaterialChangeable())
    {
      ADD_MESSAGE("Your magic is not powerful enough to affect %s .", Reader->GetStack()->GetItem(Index)->CNAME(DEFINITE));
      return false;
    }

  EMPTY_MESSAGES();
  game::DrawEverythingNoBlit();
  std::string Temp = game::StringQuestion("What material do you want to wish for?", vector2d(7,7), WHITE, 0, 80, false);

  material* TempMaterial = protosystem::CreateMaterial(Temp, Reader->GetStack()->GetItem(Index)->GetMaterial(0)->GetVolume());
	
  if(TempMaterial)
    Reader->GetStack()->GetItem(Index)->ChangeMainMaterial(TempMaterial);
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

bool platemail::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
  if(Strength > 20000 + RAND() % 40000)
    {
      ImpactDamage(ushort(Strength), false, ItemsStack);

      if(Shown)
	ADD_MESSAGE("The plate mail is damaged by the loud sound.");

      return true;
    }

  return false;	
}

bool platemail::ImpactDamage(ushort Strength, bool IsShown, stack* ItemStack)
{
  /*if(Strength > 2500.0f / StrengthValue() + RAND() % 11 - RAND() % 11)
    {
      if (IsShown)
	ADD_MESSAGE("%s is damaged.", CNAME(DEFINITE));

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
      if(Stepper->GetIsPlayer())
	ADD_MESSAGE("Auch. You step on sharp glass splinters.");
      else
	if(Stepper->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s steps on sharp glass splinters and is hurt.", Stepper->CNAME(DEFINITE));

      Stepper->SetHP(Stepper->GetHP() - RAND() % 2 - 1);
      Stepper->CheckDeath("stepped on a broken bottle");
    }

  return false;
}

material* corpse::BeDippedInto()
{
  return GetMaterial(0)->Clone(GetMaterial(0)->TakeDipVolumeAway());
}

/*void potion::ColorChangeSpeciality(uchar Index, bool EmptyMaterial)
{
  if(!Index)
    {
      for(ushort c = 1; c < 4 && c < Material.size(); ++c)
	if(!Material[c])
	  GraphicId.Color[c] = GraphicId.Color[0];
    }
  else
    if(EmptyMaterial)
      GraphicId.Color[Index] = GraphicId.Color[0];
}*/

ulong meleeweapon::Price() const
{
  return ulong(GetWeaponStrength() * GetWeaponStrength() * GetWeaponStrength() / (float(GetWeight()) * 1000000));
}

ulong torsoarmor::Price() const
{
  float ArmorModifier = StrengthValue() / 10;

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
  if(!GetMaterial(1))
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
  if(!GetMaterial(1))
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
  if(GetMaterial(0)->IsFlexible())
    return 1000;
  else
    return 70;
}

bool backpack::Apply(character* Terrorist, stack* MotherStack)
{
  if(GetMaterial(1) && GetMaterial(1)->IsExplosive())
    {
      if(Terrorist->GetIsPlayer())
	ADD_MESSAGE("You light your %s. It explodes!", CNAME(UNARTICLED));
      else
	if(Terrorist->GetLSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s lights %s. It explodes!", Terrorist->CNAME(DEFINITE), CNAME(INDEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
      SetExists(false);

      std::string DeathMsg;

      if(Terrorist->GetIsPlayer())
	DeathMsg = std::string("exploded himself with ") + Name(INDEFINITE);
      else
	DeathMsg = std::string("kamikazed by ") + Terrorist->Name(INDEFINITE);

      Terrorist->GetLSquareUnder()->GetLevelUnder()->Explosion(Terrorist, DeathMsg, Terrorist->GetLSquareUnder()->GetPos(), GetMaterial(1)->ExplosivePower());
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
	ADD_MESSAGE("%s catches fire and explodes!", CNAME(DEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
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
	ADD_MESSAGE("%s explodes in the heat!", CNAME(DEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
      SetExists(false);
      MotherStack->GetLSquareUnder()->GetLevelUnder()->Explosion(Burner, DeathMsg, MotherStack->GetLSquareUnder()->GetPos(), GetMaterial(1)->ExplosivePower());
      return true;
    }
  else
    return false;
}

std::string wand::Name(uchar Case) const 
{ 
  if(!TimesUsed)
    return NameWithMaterial(Case);
  else if(TimesUsed == 1)
    return NameWithMaterial(Case) + " (used 1 time)"; 
  else
    return NameWithMaterial(Case) + " (used " + TimesUsed + " times)";
}

bool scroll::ReceiveFireDamage(character*, std::string, stack* MotherStack, long)
{
  if(!(RAND() % 10) && GetMaterial(0)->IsFlammable())
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s catches fire!", CNAME(DEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
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
  if(!(RAND() % 2) && GetMaterial(0)->IsFlammable())
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s catches fire!", CNAME(DEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
      SetExists(false);
      return true;
    }
  else
    return false;
}

bool wand::StruckByWandOfStriking(character* Striker, std::string DeathMsg, stack* MotherStack) 
{ 
  if(!(RAND() % 10))
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CNAME(DEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
      SetExists(false);
      MotherStack->GetLSquareUnder()->GetLevelUnder()->Explosion(Striker, DeathMsg, MotherStack->GetLSquareUnder()->GetPos(), 40);
      return true;
    }
  else
    return false;
}

bool backpack::StruckByWandOfStriking(character* Striker, std::string DeathMsg, stack* MotherStack) 
{ 
  if(RAND() % 3)
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CNAME(DEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
      SetExists(false);
      MotherStack->GetLSquareUnder()->GetLevelUnder()->Explosion(Striker, DeathMsg, MotherStack->GetLSquareUnder()->GetPos(), GetMaterial(1)->ExplosivePower());
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

bool oillamp::Apply(character* Applier, stack*)
{
  if(Applier->GetIsPlayer())
    ADD_MESSAGE("You rub %s.", CNAME(DEFINITE));

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
	  ADD_MESSAGE("You see a puff of smoke, and %s appears.", Genie->CNAME(INDEFINITE));
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
			      ADD_MESSAGE("%s appears from nothing and the genie flies happily away!", TempItem->CNAME(INDEFINITE));
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
	    ADD_MESSAGE("%s wishes for %s.", Genie->CNAME(DEFINITE), Sword->CNAME(INDEFINITE));
	    ADD_MESSAGE("%s appears from nothing.", Sword->CNAME(INDEFINITE));
	    ADD_MESSAGE("%s wields %s.", Genie->CNAME(DEFINITE), Sword->CNAME(DEFINITE));
	    Genie->SetWielded(Sword);
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
  ushort Index;

  if((Index = Reader->GetStack()->DrawContents(Reader, "What item do you wish to charge?")) == 0xFFFF)
    {
      ADD_MESSAGE("You have nothing to charge.");
      return false;
    }

  if(Index == 0xFFFE)
    return false;
  else
    if(!(Index < Reader->GetStack()->GetItems()))
      return false;

  if(Reader->GetStack()->GetItem(Index) == this)
    {
      ADD_MESSAGE("This would cause a rift to appear in the space-time continuum and that wouldn't be nice.");
      return false;
    }

  if(!Reader->GetStack()->GetItem(Index)->IsChargable())
    {
      ADD_MESSAGE("You can't charge %s.", Reader->GetStack()->GetItem(Index)->CNAME(INDEFINITE));
      return false;
    }

  EMPTY_MESSAGES();
  game::DrawEverythingNoBlit();
  Reader->GetStack()->GetItem(Index)->ChargeFully(Reader);
  ADD_MESSAGE("You charge %s and the scroll burns.", Reader->GetStack()->GetItem(Index)->CNAME(DEFINITE));
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
      ADD_MESSAGE("BANG! You zap the banana!");
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
	ADD_MESSAGE("Auch. Your feet slip on %s and you fall down.", CNAME(INDEFINITE));
      else
	if(Stepper->GetSquareUnder()->CanBeSeen())
	  ADD_MESSAGE("%s steps on %s and falls down.", Stepper->CNAME(DEFINITE), CNAME(INDEFINITE));
      Stepper->EditAP(1000);
      Stepper->SetHP(Stepper->GetHP() - RAND() % 2);
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
      ADD_MESSAGE("The scroll burns and suddenly %s looks friendly.", ToBeTamed->CNAME(DEFINITE));
    }

  return true;
}

void bodypart::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << BitmapPos << Color[0] << Color[1] << Color[2] << Color[3];
}

void bodypart::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> BitmapPos >> Color[0] >> Color[1] >> Color[2] >> Color[3];
  SetMaster(0);
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

ushort bodypart::StrengthValue() const
{
  if(GetMaster() && GetMaterial(SurfaceMaterial())->IsAlive())
    return ulong(StrengthModifier()) * GetMaster()->GetEndurance() / 1000;
  else
    return ulong(StrengthModifier()) * GetMaterial(SurfaceMaterial())->StrengthValue() / 1000;
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
	ADD_MESSAGE("%s activates and explodes!", CNAME(DEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
      SetExists(false);
      MotherStack->GetLSquareUnder()->GetLevelUnder()->Explosion(Burner, DeathMsg, MotherStack->GetLSquareUnder()->GetPos(), 30);
      return true;
    }
  else
    return false;
}

bool mine::StruckByWandOfStriking(character* Striker, std::string DeathMsg, stack* MotherStack) 
{ 
  if(!(RAND() % 2))
    {
      if(MotherStack->GetLSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s explodes!", CNAME(DEFINITE));

      MotherStack->RemoveItem(MotherStack->SearchItem(this));
      SetExists(false);
      MotherStack->GetLSquareUnder()->GetLevelUnder()->Explosion(Striker, DeathMsg, MotherStack->GetLSquareUnder()->GetPos(), 30);
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
	ADD_MESSAGE("You hear a faint thumb. You look down. You see %s. It explodes.", CNAME(INDEFINITE));
      else if(Stepper->GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s steps on %s.", Stepper->CNAME(DEFINITE), CNAME(INDEFINITE));
      Stepper->GetLSquareUnder()->GetLevelUnder()->Explosion(0, "killed by a land mine", Stepper->GetPos(), 30);
    }

  return false;
}

void potion::GeneratePotionMaterials()
{ 
  switch(RAND() % 2) 
    {
    case 0: InitMaterials(2, new glass, new omleurine); break;
    case 1: InitMaterials(2, new glass, new healingliquid); break;
    }
}

bool wandofhaste::BeamEffect(character* Zapper, std::string, uchar, lsquare* LSquare)
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

bool wandofslow::BeamEffect(character* Zapper, std::string, uchar, lsquare* LSquare)
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

bool key::Apply(character* User, stack*)
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
}

bool potion::HasBeenDippedInFountain(character* Dipper,fountain* Fountain)
{
  if(Dipper->GetIsPlayer())
      ADD_MESSAGE("You dip %s in %s and it fills with water.", CNAME(DEFINITE), Fountain->CNAME(DEFINITE));

  ChangeMaterial(1, new water);
  return true;
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
