#define __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DECLARATIONS__

#include "proto.h"

class character;

std::vector<character*>			protocontainer<character>::ProtoData;
std::map<std::string, ushort>		protocontainer<character>::CodeNameMap;

#include "femath.h"
#include "charde.h"

#undef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DECLARATIONS__

#include <cmath>

#include "stack.h"
#include "itemde.h"
#include "level.h"
#include "lsquare.h"
#include "message.h"
#include "hscore.h"
#include "godba.h"
#include "feio.h"
#include "wskill.h"
#include "felist.h"
#include "strover.h"
#include "team.h"
#include "lterraba.h"
#include "error.h"
#include "config.h"
#include "slot.h"

petrus::~petrus()
{
  game::SetPetrus(0);
}

void humanoid::VirtualConstructor()
{
  for(ushort c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    CategoryWeaponSkill[c] = new gweaponskill(c);

  //CurrentSingleWeaponSkill = 0;
}

humanoid::~humanoid()
{
  for(ushort c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    delete CategoryWeaponSkill[c];

  /* Do not delete these! */

  if(GetHead())
    GetHead()->SetExists(false);

  if(GetRightArm())
    GetRightArm()->SetExists(false);

  if(GetLeftArm())
    GetLeftArm()->SetExists(false);

  if(GetGroin())
    GetGroin()->SetExists(false);

  if(GetRightLeg())
    GetRightLeg()->SetExists(false);

  if(GetLeftLeg())
    GetLeftLeg()->SetExists(false);
}

void petrus::CreateInitialEquipment()
{
  SetMainWielded(new valpurusjustifier);
  SetBodyArmor(new platemail(new valpurium));
}

void priest::CreateInitialEquipment()
{
  SetMainWielded(new spikedmace);
  SetBodyArmor(new chainmail(new mithril));
}

void oree::CreateInitialEquipment()
{
  SetBodyArmor(new goldeneagleshirt);
  item* Can = new can(false);
  Can->InitMaterials(2, new iron(10), new pepsi(330));
  GetStack()->FastAddItem(Can);
}

void darkknight::CreateInitialEquipment()
{
  if(RAND() % 20)
    SetMainWielded(RAND() % 5 ? (item*)(new longsword) : RAND() % 20 ? (item*)(new twohandedsword) : (item*)(new curvedtwohandedsword));
  else
    {
      item* DoomsDay = new longsword;
      DoomsDay->InitMaterials(3, new mithril, new iron, new darkfrogflesh);
      SetMainWielded(DoomsDay);
    }

  SetBodyArmor(new chainmail(RAND() % 5 ? (material*)new iron : (material*)new mithril));
}

void skeleton::CreateInitialEquipment()
{
  if(RAND() % 10)
    SetMainWielded(RAND() % 3 ? (item*)new axe : (item*)new pickaxe);
  else
    {
      SetMainWielded(new spikedmace);
      SetSize(200);
      SetAgility(15);
      SetStrength(18);
      SetEndurance(15);
    }
}

void goblin::CreateInitialEquipment()
{
  SetMainWielded(RAND() % 3 ? (item*)new spear : (item*)new poleaxe);
}

void guard::CreateInitialEquipment()
{
  SetMainWielded(new longsword(new iron));
  SetBodyArmor(new chainmail);

  GetCategoryWeaponSkill(LARGE_SWORDS)->AddHit(500);
  GetMainArm()->GetCurrentSingleWeaponSkill()->AddHit(500);
}

bool ennerbeast::Hit(character*)
{
  char Message[256] ;
  if(RAND() % 2) sprintf(Message,"The Enner Beast yells: UGH UGHAaaa!");
  else sprintf(Message, "The Enner Beast yells: Uga Ugar Ugade Ugat!");

  DO_FILLED_RECTANGLE(GetPos().X, GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, 30,
  {
    character* Char = game::GetCurrentLevel()->GetLSquare(vector2d(XPointer, YPointer))->GetCharacter();

    float ScreamStrength = GetMeleeStrength() * GetStrength() / GetHypotSquare(float(GetPos().X) - XPointer, float(GetPos().Y) - YPointer);

    if(Char && Char != this)
      Char->ReceiveSound(Message, RAND() % 26 - RAND() % 26,ScreamStrength);

    game::GetCurrentLevel()->GetLSquare(vector2d(XPointer, YPointer))->GetStack()->ReceiveSound(ScreamStrength);

    for(ushort x = 0; x < 4; ++x)
      game::GetCurrentLevel()->GetLSquare(vector2d(XPointer, YPointer))->GetSideStack(x)->ReceiveSound(ScreamStrength);
  });

  EditStrengthExperience(100);
  EditNP(-100);

  return true;
}

void skeleton::CreateCorpse()
{
  ushort Amount = 2 + RAND() % 4;

  for(ushort c = 0; c < Amount; ++c)
    GetLSquareUnder()->GetStack()->AddItem(new abone);
}

void elpuri::CreateCorpse()
{
  character::CreateCorpse();
  GetLSquareUnder()->GetStack()->AddItem(new headofelpuri);
}

void petrus::CreateCorpse()
{
  GetLSquareUnder()->GetStack()->AddItem(new leftnutofpetrus);
}

void ennerbeast::CreateCorpse()
{
  character::CreateCorpse();
  GetLSquareUnder()->GetStack()->AddItem(new headofennerbeast);
}

/*bool humanoid::WearArmor()
{
  if(!GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to wear!");
      return false;
    }

  ushort Index = GetStack()->DrawContents(this, "What do you want to wear? or press '-' for nothing");

  if(Index == 0xFFFE)
    {
      Armor.Torso = 0;
      EditAP(-5000);
      return true;
    }
  else
    if(Index < GetStack()->GetItems()) // Other Armor types should be coded...
      if(GetStack()->GetItem(Index)->CanBeWorn())
	if(GetStack()->GetItem(Index) != GetWielded())
	  {
	    Armor.Torso = GetStack()->GetItem(Index);
	    EditAP(-5000);
	    return true;
	  }
	else
	  ADD_MESSAGE("You can't wear something that you wield!");
      else
	ADD_MESSAGE("You can't wear THAT!");

  return false;
}*/

void humanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);

  //ushort Index = Armor.Torso ? Stack->SearchItem(Armor.Torso) : 0xFFFF;

  //SaveFile << SingleWeaponSkill;

  for(ushort c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    SaveFile << GetCategoryWeaponSkill(c);
}

void humanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);

  /*ushort Index;

  SaveFile >> Index >> SingleWeaponSkill;

  Armor.Torso = Index != 0xFFFF ? Stack->GetItem(Index) : 0;*/

  for(ushort c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    SaveFile >> GetCategoryWeaponSkill(c);
}

float golem::GetMeleeStrength() const
{
  //return 150 * GetTorso()->GetMaterial(0)->GetHitValue();
  return 0;
}

/*ushort golem::CalculateArmorModifier() const
{
  if(((GetTorso()->GetMaterial(0)->GetStrengthValue() * 3) >> 2) > 90)
    return 10;
  else
    return 100 - ((GetTorso()->GetMaterial(0)->GetStrengthValue() * 3) >> 2);
  return 0;
}*/

bool golem::MoveRandomly()
{
  if(!(RAND() % 500))
  {
    Engrave("Golem Needs Master");
    return true;
  }
  else
    return character::MoveRandomly();
}

void ennerbeast::GetAICommand()
{
  if(RAND() % 3)
    Hit(0);

  SeekLeader();

  if(CheckForEnemies(false))
    return;

  if(FollowLeader())
    return;

  MoveRandomly();
}

void petrus::GetAICommand()
{
  SeekLeader();

  SetHealTimer(GetHealTimer() + 1);

  character* Char;

  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
			if((Char = game::GetCurrentLevel()->GetLSquare(vector2d(DoX, DoY))->GetCharacter()))
  {
    if(GetTeam()->GetRelation(Char->GetTeam()) == FRIEND && Char->GetHP() < Char->GetMaxHP() / 3 && GetHealTimer() > 100)
      {
	HealFully(Char);
	return;
      }
  })

  if(CheckForEnemies(true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  CheckForDoors();
}

void petrus::HealFully(character* ToBeHealed)
{
  SetHealTimer(0);

  ToBeHealed->RestoreHP();//SetHP(ToBeHealed->GetMaxHP());

  if(ToBeHealed->GetIsPlayer())
    ADD_MESSAGE("%s heals you fully.", CHARDESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s heals %s fully.", CHARDESCRIPTION(DEFINITE), ToBeHealed->CHARDESCRIPTION(DEFINITE));
}

void petrus::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);

  SaveFile << HealTimer << StoryState;
}

void petrus::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);

  SaveFile >> HealTimer >> StoryState;

  game::SetPetrus(this);
}

bool dog::Catches(item* Thingy, float)
{
  if(Thingy->DogWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy, GetLSquareUnder()->GetStack()))
	if(GetIsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHARNAME(DEFINITE));
	else
	  {
	    if(GetLSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s catches %s and eats it.", CHARNAME(DEFINITE), Thingy->CHARNAME(DEFINITE));

	    ChangeTeam(game::GetPlayer()->GetTeam());
	  }
      else
	if(GetIsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHARNAME(DEFINITE));
	else
	  if(GetLSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s catches %s.", CHARNAME(DEFINITE), Thingy->CHARNAME(DEFINITE));

      return true;
    }
  else
    return false;
}

bool dog::ConsumeItemType(uchar Type) const // We need a better system for this... Writing this to every F***ing character that needs one
{					  // is Stoo-bit
  switch(Type)
    {
    case LIQUID:
      return true;
      break;	
    case ODD:
      return false;
      break;
    case FRUIT:
      return true;
      break;
    case MEAT:
      return true;
      break;
    case SPOILED:
      return true;
      break;
    case HARD:
      return false;
      break;
    case SCHOOLFOOD:
      return true;
      break;
    case CONTAINER:
      return false;
      break;
    case BONE:	// Bone is an odd item, cause it actually can be opened, but not always...
      // Comment for the comment: a banana can be opened also.
      return true;
      break;
    default:
      ADD_MESSAGE("ERRRRORRRRRR in dog::Consumeitemtype.");
      return false;
    }
}

/*float humanoid::GetAttackStrength() const
{
  if(!GetMainArm() && !GetSecondaryArm())
    return 0; //add biting

  //if(!GetMainWielded() && !GetSecondaryWielded())
  //  return GetMeleeStrength() * GetCategoryWeaponSkill(UNARMED)->GetBonus();

  float AS;

  //if(GetMainWielded())
  AS = GetMainAttackStrength() * GetCategoryWeaponSkill(GetMainWeaponCategory())->GetBonus();

  if(GetSecondaryWielded())
    {
      AS *= 1 - float(GetMainWielded()->OneHandedPenalty(this)) / 100;

      AS += GetSecondaryAttackStrength() * GetCategoryWeaponSkill(GetSecondaryWeaponCategory())->GetBonus();
    }
  else
    {
      if(GetSecondaryHand())
	{
	  
	}
      else
	{
	  //if(GetMainWielded()->CanBeWieldedWithTwoHands(this))
	  AS *= 1 - float(GetMainWielded()->OneHandedPenalty(this)) / 100;
	}
    }

  return 
}*/

/*float humanoid::GetAttackStrength() const
{
  return GetMainAttackStrength() + GetSecondaryAttackStrength();
}*/

/*float humanoid::GetToHitValue() const
{
  return (GetMainToHitValue() + GetSecondaryToHitValue()) / 2;
}*/

float humanoid::GetMainAttackStrength() const
{
  if(GetMainArm())
    if(GetMainArm()->GetWielded())
      return GetMainArm()->GetWieldedStrength(!GetSecondaryArm() || GetSecondaryArm()->GetWielded());
    else
      if(GetSecondaryArm() && GetSecondaryArm()->GetWielded())
	return GetSecondaryArm()->GetWieldedStrength(false);
      else
	return GetMainArm()->GetMeleeStrength();
  else
    if(GetSecondaryArm())
      if(GetSecondaryArm()->GetWielded())
	return GetSecondaryArm()->GetWieldedStrength(true);
      else
	return GetSecondaryArm()->GetMeleeStrength();
    else
      return 0;
}

float humanoid::GetSecondaryAttackStrength() const
{
  if(GetSecondaryArm())
    if(GetSecondaryArm()->GetWielded())
      return GetSecondaryArm()->GetWieldedStrength(!GetMainArm() || GetMainArm()->GetWielded());
    else
      if(GetMainArm() && GetMainArm()->GetWielded())
	return 0;
      else
	return GetSecondaryArm()->GetMeleeStrength();
  else
    return 0;
}

float humanoid::GetMainToHitValue() const
{
  if(GetMainArm())
    if(GetMainArm()->GetWielded())
      return GetMainArm()->GetWieldedToHitValue(!GetSecondaryArm() || GetSecondaryArm()->GetWielded());
    else
      if(GetSecondaryArm() && GetSecondaryArm()->GetWielded())
	return GetSecondaryArm()->GetWieldedToHitValue(false);
      else
	return GetMainArm()->GetMeleeToHitValue();
  else
    if(GetSecondaryArm())
      if(GetSecondaryArm()->GetWielded())
	return GetSecondaryArm()->GetWieldedToHitValue(true);
      else
	return GetSecondaryArm()->GetMeleeToHitValue();
    else
      return 0;
}

float humanoid::GetSecondaryToHitValue() const
{
  if(GetSecondaryArm())
    if(GetSecondaryArm()->GetWielded())
      return GetSecondaryArm()->GetWieldedToHitValue(!GetMainArm() || GetMainArm()->GetWielded());
    else
      if(GetMainArm() && GetMainArm()->GetWielded())
	return 0;
      else
	return GetSecondaryArm()->GetMeleeToHitValue();
  else
    return 0;
}

bool humanoid::Hit(character* Enemy)
{
  if(GetIsPlayer() && GetTeam()->GetRelation(Enemy->GetTeam()) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  Hostility(Enemy);

  if(GetBurdenState() == OVERLOADED)
    {
      if(GetIsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return true;
    }

  if(GetMainAttackStrength())
    MainHit(Enemy);

  if(GetSecondaryAttackStrength())
    SecondaryHit(Enemy);

  EditNP(-50);

  return true;
}

arm* humanoid::GetMainWeaponArm() const
{
  if(!GetMainArm() || !GetMainArm()->GetWielded())
    return GetSecondaryArm();
  else
    return GetMainArm();
}

arm* humanoid::GetSecondaryWeaponArm() const
{
  return GetSecondaryArm();
}

void humanoid::MainHit(character* Enemy)
{
  switch(Enemy->TakeHit(this, GetMainWielded(), GetMainAttackStrength(), GetMainToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % Enemy->CriticalModifier()))) //there's no breaks and there shouldn't be any
    {
    case HAS_HIT:
    case HAS_BLOCKED:
      if(GetMainWielded())
	GetMainWielded()->ReceiveHitEffect(Enemy, this);
    case HAS_DIED:
      EditStrengthExperience(50);
      if(GetCategoryWeaponSkill(GetMainWielded() ? GetMainWielded()->GetWeaponCategory() : UNARMED)->AddHit() && GetIsPlayer())
	GetCategoryWeaponSkill(GetMainWielded() ? GetMainWielded()->GetWeaponCategory() : UNARMED)->AddLevelUpMessage();
      if(GetMainWielded())
	{
	  if(GetMainWeaponArm()->GetCurrentSingleWeaponSkill()->AddHit() && GetIsPlayer())
	    GetMainWeaponArm()->GetCurrentSingleWeaponSkill()->AddLevelUpMessage(GetMainWielded()->Name(UNARTICLED));

	  GetMainWielded()->ImpactDamage(GetStrength() / 2);
	    //SetWielded(GetStack()->GetItem(GetStack()->GetItems() - 1));

	}
    case HAS_DODGED:
      EditAgilityExperience(25);
    }
}

void humanoid::SecondaryHit(character* Enemy)
{
  switch(Enemy->TakeHit(this, GetSecondaryWielded(), GetSecondaryAttackStrength(), GetSecondaryToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % Enemy->CriticalModifier()))) //there's no breaks and there shouldn't be any
    {
    case HAS_HIT:
    case HAS_BLOCKED:
      if(GetSecondaryWielded())
	GetSecondaryWielded()->ReceiveHitEffect(Enemy, this);
    case HAS_DIED:
      EditStrengthExperience(50);
      if(GetCategoryWeaponSkill(GetSecondaryWeaponArm() ? GetSecondaryWeaponArm()->GetWeaponCategory() : UNARMED)->AddHit() && GetIsPlayer())
	GetCategoryWeaponSkill(GetSecondaryWeaponArm() ? GetSecondaryWeaponArm()->GetWeaponCategory() : UNARMED)->AddLevelUpMessage();
      if(GetSecondaryWielded())
	{
	  if(GetSecondaryWeaponArm()->GetCurrentSingleWeaponSkill()->AddHit() && GetIsPlayer())
	    GetSecondaryWeaponArm()->GetCurrentSingleWeaponSkill()->AddLevelUpMessage(GetMainWielded()->Name(UNARTICLED));

	  GetSecondaryWielded()->ImpactDamage(GetStrength() / 2);
	    //SetWielded(GetStack()->GetItem(GetStack()->GetItems() - 1));

	}
    case HAS_DODGED:
      EditAgilityExperience(25);
    }
}

void humanoid::CharacterSpeciality(ushort Turns)
{
  for(ushort c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    if(GetCategoryWeaponSkill(c)->Turn(Turns) && GetIsPlayer())
      GetCategoryWeaponSkill(c)->AddLevelDownMessage();

  //GetRightArm()->WeaponSkillHandler();
  //GetLeftArm()->WeaponSkillHandler();
}

bool humanoid::ShowWeaponSkills()
{
  {
    felist List("Your experience in weapon categories", 0xFFFF, 0, false);

    List.AddDescription("");
    List.AddDescription("Category name                 Level     Points    To next level");

    for(ushort c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
      {
	std::string Buffer;

	Buffer += GetCategoryWeaponSkill(c)->Name();
	Buffer.resize(30, ' ');

	Buffer += GetCategoryWeaponSkill(c)->GetLevel();
	Buffer.resize(40, ' ');

	Buffer += int(GetCategoryWeaponSkill(c)->GetHits());
	Buffer.resize(50, ' ');

	if(GetCategoryWeaponSkill(c)->GetLevel() != 10)
	  List.AddEntry(Buffer + (GetCategoryWeaponSkill(c)->GetLevelMap(GetCategoryWeaponSkill(c)->GetLevel() + 1) - GetCategoryWeaponSkill(c)->GetHits()), RED);
	else
	  List.AddEntry(Buffer + '-', RED);
      }

    /*if(CurrentSingleWeaponSkill)
      {
	List.AddEntry("", RED);

	std::string Buffer = "current right hand single weapon skill:  ";

	Buffer += CurrentSingleWeaponSkill->GetLevel();
	Buffer.resize(40, ' ');

	Buffer += int(CurrentSingleWeaponSkill->GetHits());
	Buffer.resize(50, ' ');

	if(CurrentSingleWeaponSkill->GetLevel() != 10)
	  List.AddEntry(Buffer + (CurrentSingleWeaponSkill->GetLevelMap(CurrentSingleWeaponSkill->GetLevel() + 1) - CurrentSingleWeaponSkill->GetHits()), RED);
	else
	  List.AddEntry(Buffer + '-', RED);
      }*/

    List.Draw();
  }

  return false;
}

/*void humanoid::SetWielded(item* Something)
{
  if(GetWielded() && !GetCurrentSingleWeaponSkill()->GetHits())
    for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
      if(*i == GetCurrentSingleWeaponSkill())
	{
	  delete *i;
	  SingleWeaponSkill.erase(i);
	  break;
	}

  SetCurrentSingleWeaponSkill(0);

  if((Wielded = Something))
    {
      for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
	if((*i)->GetID() == Wielded->GetID())
	  {
	    SetCurrentSingleWeaponSkill(*i);
	    break;
	  }

      if(!GetCurrentSingleWeaponSkill())
	{
	  SetCurrentSingleWeaponSkill(new sweaponskill);
	  GetCurrentSingleWeaponSkill()->SetID(Wielded->GetID());
	  SingleWeaponSkill.push_back(GetCurrentSingleWeaponSkill());
	}
    }

  if(GetSquareUnder())
    GetSquareUnder()->SendNewDrawRequest();
}*/

/*float humanoid::GetToHitValue() const
{
  if(GetWielded())
    return GetMeleeAttributeModifier() * GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus() / sqrt(GetWielded()->GetWeight() > 400 ? GetWielded()->GetWeight() : 400) * 10;
  else
    return (GetMeleeAttributeModifier() >> 1) * GetCategoryWeaponSkill(UNARMED)->GetBonus();
}*/

void shopkeeper::CreateInitialEquipment()
{
  SetMainWielded(new pickaxe(new mithril));
  SetBodyArmor(new chainmail(new mithril));

  GetCategoryWeaponSkill(AXES)->AddHit(5000);
  GetMainArm()->GetCurrentSingleWeaponSkill()->AddHit(5000);
}

void farmer::CreateInitialEquipment()
{
  SetMainWielded(new axe);
}

/*
 * This function is temporary. It prevents Petrus's hit messages becoming too long,
 * which would at present make the message history quite ugly.
 */

void petrus::AddHitMessage(character* Enemy, item* Weapon, uchar BodyPart, bool Critical) const
{
  std::string Msg;
  std::string ThisDescription = GetSquareUnder()->CanBeSeen() ? "Petrus" : "something";
  std::string BodyPartDescription = BodyPart && Enemy->GetSquareUnder()->CanBeSeen() ? std::string(" in ") + Enemy->GetBodyPart(BodyPart)->Name(DEFINITE) : "";

  if(Enemy->GetIsPlayer())
    {
      if(Weapon && GetSquareUnder()->CanBeSeen())
	Msg = ThisDescription + " " + ThirdPersonWeaponHitVerb(Critical) + " you" + BodyPartDescription + " with " + PossessivePronoun() + " " + Weapon->Name(UNARTICLED) + "!";
      else
	Msg = ThisDescription + " " + ThirdPersonMeleeHitVerb(Critical) + " you" + BodyPartDescription + "!";
    }
  else if(GetIsPlayer())
    Msg = std::string("You ") + FirstPersonHitVerb(Enemy, Critical) + " " + Enemy->Description(DEFINITE) + BodyPartDescription + "!";
  else if(GetSquareUnder()->CanBeSeen() || Enemy->GetSquareUnder()->CanBeSeen())
    Msg = ThisDescription + " " + AICombatHitVerb(Enemy, Critical) + " " + Enemy->Description(DEFINITE) + BodyPartDescription + "!";
  else
    Msg = "";

  ADD_MESSAGE("%s", Msg.c_str());
}

void petrus::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("Heretic! Dev/null is a place not worthy to receive thee!");
      return;
    }

  if(Talker->HasGoldenEagleShirt())
    {
      ADD_MESSAGE("Petrus smiles.");
      ADD_MESSAGE("\"Thou hast defeated Oree! Valpurus shall bless thee for the rest of thine life!");
      ADD_MESSAGE("And thou possess the Shirt of the Golden Eagle, the symbol of my status!");
      ADD_MESSAGE("Return it now, please.\"");

      if(game::BoolQuestion("Will you give the Shirt of the Golden Eagle to Petrus? [Y/n]", 'y'))
	{
	  iosystem::TextScreen("Thou hast slain the Blood Daemon King, and Petrus is happy!\n\nYou are victorious!");
	  game::RemoveSaves();

	  if(!game::GetWizardMode())
	    {
	      AddScoreEntry("retrieved the Shirt of the Golden Eagle and became the Avatar of Law", 4, false);
	      highscore HScore;
	      HScore.Draw();
	    }

	  game::Quit();
	  return;
	}
      else
	{
	  ADD_MESSAGE("Petrus's face turns red. \"I see. Thine greed hast overcome thine wisdom.");
	  ADD_MESSAGE("Then, we shall fight for the shiny shirt. May Valpurus bless him who is better.\"");

	  /* Perhaps we could here use our material system to actually make his face change color ;-) */

	  Talker->GetTeam()->Hostility(GetTeam());
	  return;
	}
    }
  else
    if(StoryState >= 2)
      {
	ADD_MESSAGE("Petrus says: \"Bring me the Shirt of the Golden Eagle and we'll talk.\"");
	return;
      }

  if(Talker->HasHeadOfElpuri())
    {
      if(game::GetGod(1)->GetRelation() >= 0 && Talker->MaxDanger() >= 250000)
	{
	  ADD_MESSAGE("Petrus smiles. \"Thou areth indeed a great Champion of the Great Frog!");
	  ADD_MESSAGE("Elpuri is not a foe worthy for thee.");

	  if(game::BoolQuestion("Dost thou wish to stay on duty for a while more and complete another quest for me?\" [Y/n]", 'y'))
	    {
	      iosystem::TextScreen(	"Champion of Law!\n\n"
					"Return to the foul cave of Elpuri and seek out the Master Evil:\n"
					"Oree the Blood Daemon King, who hast stolenth one of the most powerful of all of my artifacts:\n"
					"the Shirt of the Golden Eagle! Return with it and immortal glory shall be thine!");

	      game::GetCurrentArea()->SendNewDrawRequest();
	      game::TriggerQuestForGoldenEagleShirt();
	      StoryState = 2;
	      return;
	    }
	}

      iosystem::TextScreen("Thou hast slain Elpuri, and Petrus is happy!\n\nYou are victorious!");
      game::RemoveSaves();

      if(!game::GetWizardMode())
	{
	  AddScoreEntry("defeated Elpuri and continued to further adventures", 2, false);
	  highscore HScore;
	  HScore.Draw();
	}

      game::Quit();
    }
  else
    if(!StoryState)
      {
	iosystem::TextScreen(	"Petrus raises his hand as a salutation, and talks:\n"
				"\"Fare thee well, adventurer! Should thou seek glory, I have a task for thee!\n"
				"An evil dark frog named Elpuri is pestering our fine city in many ways.\n"
				"Valpurus hast told that this vile beast can be found in a nearby cave.\n"
				"Slay it and bring me its head as proof. Return when thou hast succeeded.\"");

	game::GetCurrentArea()->SendNewDrawRequest();
	ADD_MESSAGE("\"And by the way, visit the librarian. He might have advice for thee.\"");
	StoryState = 1;
      }
    else
      ADD_MESSAGE("Petrus says: \"Bring me the head of Elpuri and we'll talk.\"");
}

void farmer::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      /* This message may be little incorrect if wielded item has changed... */

      ADD_MESSAGE("\"Did you think I use this axe only to chop wood?\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Crops are so lousy around here. Perhaps because the summer lasts two weeks.\"");
      break;
    case 1:
      ADD_MESSAGE("%s seems suspicious. \"You look like one from Istour! Go away!\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 2:
      ADD_MESSAGE("%s sighs: \"Again polar bears ate my cattle...\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 3:
      ADD_MESSAGE("\"The prices are infamous here. Ivan should smack that capitalist shopkeeper hard!\"");
      break;
    }
}

void guard::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"A fair trial? Hah! Prepare to be executed!\"");
      return;
    }

  static bool Said[5];

  switch(RandomizeReply(5, Said))
    {
    case 0:
      if(GetLSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s says gravely: \"You don't have a life. Get it in the army.\"", CHARDESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s says gravely: \"You don't have a life. Get it as shop guard.\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 1:
      if(GetLSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of breaking rules.\"", CHARDESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of stealing anything.\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 2:
      if(GetLSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s shouts excited: \"Attnam victoor!\"", CHARDESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("\"Yes, this is a dangerous place to work, but our boss pays us well.\"");
      break;
    case 3:
      if(GetLSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"The High Priest is my idol. I would want a sword as big as his!\"");
      else
        {
	  ADD_MESSAGE("\"Attnam's guards can barely wield a sword.");
          ADD_MESSAGE("But we are trained by the laws of the dungeon, so don't do anything suspicious here.\"");
        }
      break;
    case 4:
      if(GetLSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"Attnam, shit, I'm still only in Attnam. Every time I think I'm gonna wake up back in the dungeon.\"");
      else
	ADD_MESSAGE("\"When I was here, I wanted to be there, when I was there all I could think of was getting back into the dungeon.\"");
      break;
    }
}

void shopkeeper::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Criminal! Mellis bless my efforts of removing you!\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      if(GetLSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s sighs: \"If only I hadn't chosen a city in the middle of nowhere...\"", CHARDESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s sighs: \"I wonder why I have so few customers these days...\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 1:
      ADD_MESSAGE("%s sighs: \"Mutant mushrooms ate the last caravan.", CHARDESCRIPTION(DEFINITE));
      ADD_MESSAGE("The one before it ran into an Enner Beast. It must be all Elpuri's doings!\"");
      break;
    case 2:
      if(GetLSquareUnder()->GetLevelUnder()->GetOnGround())
	{
	  ADD_MESSAGE("\"You truly can't find better prices in this city!\", %s smiles.", CHARDESCRIPTION(DEFINITE));
	  ADD_MESSAGE("\"Indeed, you can't find ANY prices, since my store is a monopoly.\"");
	}
      else
	ADD_MESSAGE("\"The topmost reason why I work here is that monsters devour tax collectors.\"");
      break;
    case 3:
      if(GetLSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"Don't try anything. The High Priest is a friend of mine.\"");
      else
	ADD_MESSAGE("\"The monsters don't attack me, because of our mutually profitable contract.\"");
      break;
    }
}

void priest::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Sinner! My hands shall pour Dinive Wrath upon thee!\"");
      return;
    }

  if(!HomeRoom)
    ADD_MESSAGE("\"Receive my blessings, child.\"");
  else
    {
      ADD_MESSAGE("%s talks to you:", CHARDESCRIPTION(DEFINITE));
      game::GetGod(GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetDivineOwner())->AddPriestMessage();
    }
}

void oree::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s laughs: \"No time for small talk. Time to drink blood!\"", CHARDESCRIPTION(DEFINITE));
}

void darkknight::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s yells: \"For Erado I shall slay you!\"", CHARDESCRIPTION(DEFINITE));
}

void ennerbeast::BeTalkedTo(character*)
{
  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Fishing is fun! Do you fish?\"");
      break;
    case 1:
      ADD_MESSAGE("\"And then I got that perch weighting fifty stones...\"");
      break;
    case 2:
      ADD_MESSAGE("\"What do you think of Braveheart?\"");
      break;
    case 3:
      ADD_MESSAGE("\"Can you help me with this download problem?\"");
      break;
    }
}

void elpuri::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s roars horribly: \"DiE hUmAn!!\"", CHARDESCRIPTION(DEFINITE));
}

void billswill::BeTalkedTo(character*)
{
  ADD_MESSAGE("\"Windows XP is coming. You will install it. Resistance is futile. Prepare to be assimilited.\"");
}

void skeleton::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("%s sings: \"Leg bone is connected to the hib bone, hib bone is connected to the rib bone...\"", CHARDESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s grunts: \"Bones. Need more bones.\"", CHARDESCRIPTION(DEFINITE));
}

void goblin::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("%s laughs: \"Humie friend. Many mommo we kill. Many spider we eat.\"", CHARDESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s yells goblin war cries at you.", CHARDESCRIPTION(DEFINITE));
}

void golem::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("\"Yes, master?\"");
  else
    ADD_MESSAGE("\"Yes, master. Golem kill human. Golem then return.\"");
}

long humanoid::StatScore() const
{
  long SkillScore = 0;
  ushort c;

  for(c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    SkillScore += GetCategoryWeaponSkill(c)->GetHits();

  /*
  for(c = 0; c < SingleWeaponSkill.size(); ++c)
    SkillScore += SingleWeaponSkill[c]->GetHits();*/

  return (SkillScore >> 2) + character::StatScore();
}

void humanoid::AddSpecialItemInfo(std::string& Description, item* Item)
{
  Description.resize(75, ' ');
  Description += GetCategoryWeaponSkill(Item->GetWeaponCategory())->GetLevel();
  Description.resize(80, ' ');

  if(GetRightArm())
    {
      bool Added = false;

      for(ushort c = 0; c < GetRightArm()->GetSingleWeaponSkills(); ++c)
	if(Item->GetID() == GetRightArm()->GetSingleWeaponSkill(c)->GetID())
	  {
	    Description += GetRightArm()->GetSingleWeaponSkill(c)->GetLevel();
	    Added = true;
	    break;
	  }

      if(!Added)
	Description += 0;
    }

  Description.resize(87, ' ');

  if(GetLeftArm())
    {
      bool Added = false;

      for(ushort c = 0; c < GetLeftArm()->GetSingleWeaponSkills(); ++c)
	if(Item->GetID() == GetLeftArm()->GetSingleWeaponSkill(c)->GetID())
	  {
	    Description += GetLeftArm()->GetSingleWeaponSkill(c)->GetLevel();
	    Added = true;
	    break;
	  }

      if(!Added)
	Description += 0;
    }
}

void humanoid::AddSpecialItemInfoDescription(std::string& Description)
{
  Description.resize(78, ' ');
  Description += "GS";
  Description.resize(83, ' ');
  Description += "SSR";
  Description.resize(90, ' ');
  Description += "SSL";
}

void humanoid::KickHit()
{
  if(GetCategoryWeaponSkill(UNARMED)->AddHit() && GetIsPlayer())
    GetCategoryWeaponSkill(UNARMED)->AddLevelUpMessage();
}

void communist::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"You capitalist! Lenin want %s kill capitalists!\"", CHARNAME(UNARTICLED));
      return;
    }

  if(GetTeam() == Talker->GetTeam() || Talker->MaxDanger() < 100000)
    {
      static bool Said[11];

      switch(RandomizeReply(11, Said))
	{
	case 0:
	  ADD_MESSAGE("\"Da, %s like killing.\"", CHARNAME(UNARTICLED));
	  break;
	case 1:
	  ADD_MESSAGE("\"%s ruski specialist.\"", CHARNAME(UNARTICLED));
	  break;
	case 2:
	  ADD_MESSAGE("\"%s work. Else %s get stapled.\"", CHARNAME(UNARTICLED), CHARNAME(UNARTICLED));
	  break;
	case 3:
	  ADD_MESSAGE("\"Party mean big weapons. %s like big weapons. %s kill for Party.\"", CHARNAME(UNARTICLED), CHARNAME(UNARTICLED));
	  break;
	case 4:
	  ADD_MESSAGE("\"CCCP roxxx.\"");
	  break;
	case 5:
	  ADD_MESSAGE("\"%s like throw Ladas. You want compete?\"", CHARNAME(UNARTICLED));
	  break;
	case 6:
	  ADD_MESSAGE("\"Why is AK not invented?\"");
	  break;
	case 7:
	  ADD_MESSAGE("\"%s like Attnam. Petrus communist too.\"", CHARNAME(UNARTICLED));
	  break;
	case 8:
	  ADD_MESSAGE("\"%s buy kyber eyes. %s see in dark.\"", CHARNAME(UNARTICLED), CHARNAME(UNARTICLED));
	  break;
	case 9:
	  ADD_MESSAGE("\"Uncle Lenin lives in Russia. Lenin strong guy. %s like.\"", CHARNAME(UNARTICLED));
	  break;
	case 10:
	  if(GetTeam() == Talker->GetTeam())
	    ADD_MESSAGE("\"We both good proles. We do good work.\"");
	  else
	    ADD_MESSAGE("\"Go away, you weak. Be back when learned killing.\"");
	  break;
	}
    }
  else
    {
      ADD_MESSAGE("%s seems to be very friendly. \"You make good communist. %s go with you!\"", CHARDESCRIPTION(DEFINITE), CHARNAME(UNARTICLED));
      ChangeTeam(Talker->GetTeam());
    }
}

void communist::CreateInitialEquipment()
{
  SetMainWielded(new spikedmace(new iron));
  SetBodyArmor(new platemail);
  GetStack()->FastAddItem(new fiftymillionroubles);
}

void hunter::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Your head will look fine above my fireplace!\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"A man is not a man unless he has lost his left arm in a battle against a polar bear.\"");
      break;
    case 1:
      ADD_MESSAGE("\"Bears, ogres, slaves, farmers... Ah, there's so much to hunt here!\"");
      break;
    case 2:
      if(GetMainWielded())
	ADD_MESSAGE("\"This is my %s. There are many like it but this one is mine. My %s is my best friend.\"", GetMainWielded()->CHARNAME(UNARTICLED), GetMainWielded()->CHARNAME(UNARTICLED));
      else
	ADD_MESSAGE("\"I am the Great White Hunter. Get out of My way!\"");
      break;
    case 3:
      ADD_MESSAGE("\"I saw a communist visiting the city a few days past.");
      ADD_MESSAGE("I'm now organising a party to seek and hunt him down.\"");
      break;
    }
}

void hunter::CreateInitialEquipment()
{
  SetMainWielded(new spear(new iron));
}

void slave::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Yikes!\"");
      return;
    }

  character* Master;

  if(HomeRoom && (Master = GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetMaster()))
    {
      if(Talker->GetMoney() >= 50)
	{
	  ADD_MESSAGE("%s talks: \"Do you want to buy me? 50 squirrels. I work very hard.\"", CHARDESCRIPTION(DEFINITE));

	  if(game::BoolQuestion("Do you want to buy him? [y/N]"))
	    {
	      Talker->SetMoney(Talker->GetMoney() - 50);
	      Master->SetMoney(Master->GetMoney() + 50);
	      ChangeTeam(Talker->GetTeam());
	      SetHomeRoom(0);
	    }
	}
      else
	{
	  ADD_MESSAGE("\"Don't touch me! Master doesn't want people to touch sale items.");
	  ADD_MESSAGE("I'm worth 50 squirrels, you know!\"");
	}

      return;
    }

  if(GetTeam() == Talker->GetTeam())
    {
      if(Talker->GetMainWielded() && Talker->GetMainWielded()->GetType() == whip::StaticType())
	{
	  ADD_MESSAGE("\"Don't hit me! I work! I obey! I not think!\"");
	}
      else
	{
	  static bool Said[4];

	  switch(RandomizeReply(4, Said))
	    {
	    case 0:
	      ADD_MESSAGE("\"Whatever the master wants.\"");
	      break;
	    case 1:
	      ADD_MESSAGE("\"Work work work all day long. No, that was not a complain! Don't punish!\"");
	      break;
	    case 2:
	      ADD_MESSAGE("\"I love all my masters. At least when the whip is being washed.\"");
	      break;
	    case 3:
	      ADD_MESSAGE("\"I would like to be like Ivan. Ivan is a good worker.\"");
	      break;
	    case 4:
	      ADD_MESSAGE("\"I am putting myself to the fullest possible use, which is all I think that any conscious entity can ever hope to do.\"");
	      break;
	    }
	}
    }
  else
    ADD_MESSAGE("\"I'm free! Rejoice!\"");
}

void slave::GetAICommand()
{
  SeekLeader();

  if(CheckForEnemies(true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(CheckForDoors())
    return;

  if(!HomeRoom || !GetLSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetMaster())
    {
      HomeRoom = 0;
      MoveRandomly();
    }
}

bool elpuri::Hit(character* Enemy)
{
  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
  {
    lsquare* Square = GetLSquareUnder()->GetLevelUnder()->GetLSquare(vector2d(DoX, DoY));
    character* ByStander = Square->GetCharacter();

    if(ByStander && (ByStander == Enemy || ByStander->GetTeam()->GetRelation(GetTeam()) == HOSTILE))
      {
	Hostility(ByStander);

	switch(ByStander->TakeHit(this, 0, GetAttackStrength(), GetToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % Enemy->CriticalModifier())))
	  {
	  case HAS_HIT:
	  case HAS_BLOCKED:
	  case HAS_DIED:
	    ByStander->GetStack()->ImpactDamage(GetStrength());
	    //ByStander->CheckGearExistence();
	    EditStrengthExperience(50);
	  case HAS_DODGED:
	    EditAgilityExperience(25);
	  }
      }

    Square->GetStack()->ImpactDamage(GetStrength());

    for(ushort c = 0; c < 4; ++c)
      if(Square->GetSideStack(c)->GetSquareTrulyUnder() == GetSquareUnder())
	Square->GetSideStack(c)->ImpactDamage(GetStrength());
  });

  EditNP(-50);

  return true;
}

void petrusswife::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Murderer! Just wait until Petrus finds you!\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"I'm so sick jealous to those dolphins...\"");
      break;
    case 1:
      ADD_MESSAGE("\"I'm Petrus's favorite, not she!\"");
      break;
    case 2:
      ADD_MESSAGE("\"Why must Petrus stay in this forest? There isn't even a proper hairdresser here!\"");
      break;
    case 3:
      ADD_MESSAGE("\"Being one of six wives is a dream job.");
      ADD_MESSAGE("Pay is good and you only have to work about one night a week!\"");
      break;
    }
}

void housewife::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Face my rolling pin! Graah!\"");
      return;
    }

  static bool Said[5];

  switch(RandomizeReply(5, Said))
    {
    case 0:
      ADD_MESSAGE("\"Can you help me find my husband? He is hiding somewhere.");
      ADD_MESSAGE("He's that farmer who's just been mugged with a frying pan.\"");
      break;
    case 1:
      ADD_MESSAGE("\"Yesterday a bear rushed through my kitchen wall and ruined all my pies.");
      ADD_MESSAGE("Animals are truly annoying. Why can't we just burn the whole forest down?\"");
      break;
    case 2:
      ADD_MESSAGE("\"Wolves ate my seventh daughter last week. Damn.");
      ADD_MESSAGE("It'll take eight years to produce an equally good replacement worker.\"");
      break;
    case 3:
      ADD_MESSAGE("\"Petrus's wives are so arrogant towards us working class ones. Grr...\"");
      break;
    case 4:
      ADD_MESSAGE("\"If you men only knew!\"");
    }
}

void femaleslave::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Yikes!\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Praise our lord Petrus!\"");
      break;
    case 1:
      ADD_MESSAGE("\"Does that toy frog really need us serving it?\"");
      break;
    case 2:
      ADD_MESSAGE("\"I'm not a slave. I'm a high-ranking palm branch officer with a good career history.\"");
      break;
    case 3:
      ADD_MESSAGE("\"Palm leaves are good for health. Petrus loves their smell.\"");
      break;
    }
}

void librarian::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"The pen is mightier than the sword! Fall, unlearned one!\"");
      return;
    }

  static bool Said[11];

  switch(RandomizeReply(11, Said))
    {
    case 0:
      if(game::GetPetrus() && !game::GetPetrus()->GetStoryState())
	{
	  ADD_MESSAGE("\"Thou should visit Petrus if thou areth in need of adventure.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that a wand of polymorph has dozens of uses.\"");
	  break;
	}
    case 1:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Thou are going to fight Elpuri? Beware! It is a powerful enemy.");
	  ADD_MESSAGE("Other monsters are very vulnerable if surrounded by thine party, but not that beast.");
	  ADD_MESSAGE("For it may slay a horde of thine friends at once with its horrendous tail attack.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"Remember: Scientia est potentia.\"");
	  break;
	}
    case 2:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"Shh! Be silent in the library.\"");
	  break;
	}
    case 3:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Elpuri's attacks are so strong that they may shatter many of thine precious items.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"Dost thou not smell all the knowledge floating around here?\"");
	  break;
	}
    case 4:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 2)
	{
	  ADD_MESSAGE("\"Thou wish to confront the Blood Daemon King? Heed my advice:");
	  ADD_MESSAGE("He is a cunning enemy and will try to ambush thee.");
	  ADD_MESSAGE("A powerful party and a means of quick escape at hand would help thee greatly.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that Loricatus, the god of smithing, can enchant thine weapons' material.\"");
	  break;
	}
    case 5:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 2)
	{
	  ADD_MESSAGE("\"The Shirt of the Golden Eagle is a legendary artifact.");
	  ADD_MESSAGE("Thou canst not find a better armor.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"In this book they talk about Erado, the great chaos god.");
	  ADD_MESSAGE("He hates us mortals more than anything and will respond only to Champions of Evil.\"");
	  break;
	}
    case 6:
      ADD_MESSAGE("\"Attnam is traditionally ruled by the High Priest of the Great Frog.");
      ADD_MESSAGE("He holds the Shirt of the Golden Eagle and has always killed his predecessor.\"");
      break;
    case 7:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 3)
	{
	  ADD_MESSAGE("\"Remember, the Shirt of the Golden Eagle is the armor the High Priests.");
	  ADD_MESSAGE("Things will get *very* rough if one denies it from Petrus.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"They say thou should keep all the artifacts thou find.");
	  ADD_MESSAGE("They shalt make thee famous after thine retirement.\"");
	  break;
	}
    case 8:
      ADD_MESSAGE("\"If thou shalt ever encounter an Enner Beast, know this: It is a horrible foe.");
      ADD_MESSAGE("It may shatter thine items and armor with its scream that penetrates iron and stone.");
      ADD_MESSAGE("Thou should not engage it in melee. Kill it from afar.\"");
      break;
    case 9:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Thou areth not alone in thine attempth to defeat Elpuri.");
	  ADD_MESSAGE("A brave adventurer called Ivan also dived into its cave not long ago.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that chaotic gods offer great power to their followers.");
	  ADD_MESSAGE("But thou must remember: unlike lawfuls, they shalt not help thee when things go bad.\"");
	  break;
	}
    case 10:
      ADD_MESSAGE("\"If a man cannot choose, he ceases to be a man.\"");
      break;

    }
}

void femaleslave::CreateInitialEquipment()
{
  SetMainWielded(new palmbranch);
}

bool communist::MoveRandomly()
{
  switch(RAND() % 1000)
    {
      /*		case 0:
			Engrave("The bourgeois is a bourgeois -- for the benefit of the working class.");
			break;*/
      // Currently too long
    case 1:
      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s engraves something to the ground.", CHARNAME(UNARTICLED));

      Engrave("Proletarians of all countries, unite!");
      return true;
    case 2:
      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s engraves something to the ground.", CHARNAME(UNARTICLED));

      Engrave("Capital is therefore not only personal; it is a social power.");
      return true;
    default:
      return character::MoveRandomly();
    }
}

void zombie::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    if(RAND() % 5)
      ADD_MESSAGE("\"Need brain!!\"");
    else
      ADD_MESSAGE("\"Redrum! Redrum! Redrum!\"");
  else
    ADD_MESSAGE("\"Need brain, but not your brain.\"");
}

void zombie::SpillBlood(uchar HowMuch, vector2d GetPos)
{
  if(!HowMuch)
    return;

  if(!game::GetInWilderness()) 
    {
      game::GetCurrentLevel()->GetLSquare(GetPos)->SpillFluid(HowMuch, GetBloodColor(), 5, 60);

      if(!(RAND() % 10)) 
	game::GetCurrentLevel()->GetLSquare(GetPos)->GetStack()->AddItem(new lump(new humanflesh(1000)));
    }
}

void mistress::CreateInitialEquipment()
{
  SetMainWielded(new whip);

  if(!(RAND() % 10))
    GetStack()->FastAddItem(new banana);

  if(!(RAND() % 100))
    GetStack()->FastAddItem(new holybanana);

  GetCategoryWeaponSkill(WHIPS)->AddHit(10000);
  GetMainArm()->GetCurrentSingleWeaponSkill()->AddHit(10000);
}

void werewolf::ChangeIntoHuman()
{
  SetSize(170);
  SetAgility(15);
  SetStrength(15);
  SetEndurance(15);
  SetPerception(15);
  SetIsWolf(false);
  SetStrengthExperience(0);
  SetEnduranceExperience(0);
  SetAgilityExperience(0);
  SetPerceptionExperience(0);
  RestoreHP();
  UpdateBodyPartPictures();

  if(GetSquareUnder())
    GetSquareUnder()->SendNewDrawRequest();
}

void werewolf::ChangeIntoWolf()
{
  SetSize(200);
  SetAgility(25);
  SetStrength(25);
  SetEndurance(25);
  SetPerception(24);
  SetIsWolf(true);
  SetMainWielded(0);
  SetSecondaryWielded(0);
  SetStrengthExperience(0);
  SetEnduranceExperience(0);
  SetAgilityExperience(0);
  SetPerceptionExperience(0);
  RestoreHP();
  UpdateBodyPartPictures();

  if(GetSquareUnder())
    GetSquareUnder()->SendNewDrawRequest();
}

void werewolf::Be()
{
  if(ChangeCounter++ > 2500)
    {
      SetChangeCounter(0);

      if(GetIsWolf())
	{	
	  ChangeIntoHuman();
	  if(GetIsPlayer())
	    ADD_MESSAGE("You change into a human... At least for some time.");
	  else if(GetSquareUnder()->CanBeSeen())
	    {
	      ADD_MESSAGE("%s changes into human.", CHARNAME(DEFINITE));
	    }
	}
      else
	{
	  ChangeIntoWolf();
	  if(GetIsPlayer())
	    ADD_MESSAGE("You change into a wolf... At least for some time.");
	  else if(GetSquareUnder()->CanBeSeen())
	    {
	      ADD_MESSAGE("%s changes into a wolf.", CHARNAME(DEFINITE));
	    }
	}
    }

  character::Be();
}

ulong werewolf::MaxDanger()
{
  bool BeforeThis = GetIsWolf();

  /* This should perhaps be made more elegant... */

  ChangeIntoWolf();
  ulong AsWolf = humanoid::MaxDanger();
  ChangeIntoHuman();
  ulong AsHuman = humanoid::MaxDanger();

  if(BeforeThis)
    ChangeIntoWolf();

  return (AsWolf + AsHuman) >> 1;
}

float werewolf::GetMeleeStrength() const
{
  if(GetIsWolf())
    return 10000;
  else
    return 2000;
}

void werewolf::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);

  SaveFile << IsWolf << ChangeCounter;
}

void werewolf::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);

  SaveFile >> IsWolf >> ChangeCounter;
}

void kobold::CreateInitialEquipment()
{
  SetMainWielded(new spear);
}

void mistress::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    ADD_MESSAGE("\"Come closer, little boy, I'll teach you something...\"");
  else
    ADD_MESSAGE("\"Just wait for the night, darling.\"");
}

void angel::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << Master;
}

void angel::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> Master;
}

void angel::SetMaster(uchar NewMaster)
{
  switch(game::GetGod(NewMaster)->BasicAlignment())
    {
    case GOOD:
      SetAgility(99);
      SetMainWielded(new longsword(new diamond));
      SetBodyArmor(new chainmail(new diamond));
      SetMaterial(1, new goodleather);
      RestoreHP();
      break;
    case NEUTRAL:
      SetEndurance(99);
      SetMainWielded(new poleaxe(new sapphire));
      SetBodyArmor(new chainmail(new sapphire));
      SetMaterial(1, new neutralleather);
      RestoreHP();
      break;
    case EVIL:
      {
	SetStrength(99);
	item* SpikedMace = new spikedmace(false);
	SpikedMace->InitMaterials(3, new ruby, new iron, new darkfrogflesh);
	SetMainWielded(SpikedMace);
	SetBodyArmor(new brokenplatemail(new ruby));
	SetMaterial(1, new evilleather);
	RestoreHP();
	break;
      }
    }

  Master = NewMaster;
}

void angel::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    ADD_MESSAGE("\"With the power of %s, I shall slay thee, sinner!\"", game::GetGod(Master)->Name().c_str());
  else
    ADD_MESSAGE("\"%s be with you, mortal.\"", game::GetGod(Master)->Name().c_str());
}

void kamikazedwarf::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Heaven awaits me in the house of %s after I bomb you, heretic!\"", game::GetGod(Master)->Name().c_str());
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Would you like me to teach you the best suicidal bombing tactics?\"");
      break;
    case 1:
      ADD_MESSAGE("%s shouts: \"Death to disbelievers!\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 2:
      ADD_MESSAGE("%s praises %s with numerous hymns.", CHARDESCRIPTION(DEFINITE), game::GetGod(Master)->Name().c_str());
      ADD_MESSAGE("%s is obviously a very devoted follower.", PersonalPronoun().c_str());
      break;
    case 3:
      ADD_MESSAGE("\"One day, Holy War will break out and I shall sacrifice my life with joy.\"");
      break;
    }
}

void kamikazedwarf::CreateInitialEquipment()
{
  holybook* HolyBook = new holybook;
  HolyBook->SetOwnerGod(Master);
  SetMainWielded(HolyBook);
  GetStack()->FastAddItem(new backpack);
}

bool kamikazedwarf::Hit(character* Enemy)
{
  if(GetIsPlayer())
    return humanoid::Hit(Enemy);
  else
    {
      //for(ushort c = 0; c < GetStack()->GetItems(); ++c)
      for(stackiterator i = GetStack()->GetBottomSlot(); i != GetStack()->GetSlotAboveTop(); ++i)
	if((**i)->IsExplosive())
	  {
	    if(RAND() % 2)
	      ADD_MESSAGE("%s shouts: \"For %s!\"", CHARDESCRIPTION(DEFINITE), game::GetGod(Master)->Name().c_str());
	    else
	      ADD_MESSAGE("%s screams: \"%s, here I come!\"", CHARDESCRIPTION(DEFINITE), game::GetGod(Master)->Name().c_str());

	    if((**i)->Apply(this, GetStack()))
	      return true;
	  }

      return humanoid::Hit(Enemy);
    }
}

void kamikazedwarf::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);

  SaveFile << Master;
}

void kamikazedwarf::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);

  SaveFile >> Master;
}

/*void humanoid::CheckGearExistence()
{
  character::CheckGearExistence();

  if(GetBodyArmor() && !GetBodyArmor()->GetExists())
    SetBodyArmor(0);
}*/

bool largecat::Catches(item* Thingy, float)
{
  if(Thingy->CatWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy, GetLSquareUnder()->GetStack()))
	if(GetIsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHARNAME(DEFINITE));
	else
	  {
	    if(GetLSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s catches %s and eats it.", CHARNAME(DEFINITE), Thingy->CHARNAME(DEFINITE));

	    ChangeTeam(game::GetPlayer()->GetTeam());
	  }
      else
	if(GetIsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHARNAME(DEFINITE));
	else
	  if(GetLSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s catches %s.", CHARNAME(DEFINITE), Thingy->CHARNAME(DEFINITE));

      return true;
    }
  else
    return false;
}

/*void unicorn::RandomizeFleshMaterial()
{
  SetAlignment(RAND() % 3);
  switch(GetAlignment())
    {
    case GOOD:
      InitMaterials(2, new whiteunicornflesh, new unicornhorn);
      return;
    case NEUTRAL:
      InitMaterials(2, new grayunicornflesh, new unicornhorn);
      return;
    default:
      InitMaterials(2, new blackunicornflesh, new unicornhorn);
      return;
    }
}*/

material* unicorn::CreateTorsoFlesh(ulong Volume) const
{
  switch(RAND() % 3)
    {
    case GOOD:
      return new whiteunicornflesh(Volume);
    case NEUTRAL:
      return new grayunicornflesh(Volume);
    default:
      return new blackunicornflesh(Volume);
    }
}

void unicorn::AdjustAlignment()
{
  SetAlignment(2 - GetTorso()->GetMainMaterial()->Alignment());
}

void unicorn::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << Alignment;
}

void unicorn::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> Alignment;
}

std::string unicorn::NameSingular() const
{
  switch(Alignment)
    {
    case EVIL:
      return "black unicorn";
    case NEUTRAL:
      return "gray unicorn";
    case GOOD:
      return "white unicorn";
    }

  ABORT("Unicorns do not exist.");
  return "";
}

void kamikazedwarf::GetAICommand()
{
  if(HomeRoom)
    StandIdleAI();
  else
    character::GetAICommand();
}

void genie::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    ADD_MESSAGE("\"Fall, you puny Prime Material Plane dweller!\"");
  else
    ADD_MESSAGE("\"You know, it's not fun to stay in an oil lamp for three centuries...\"");
}

bool unicorn::SpecialEnemySightedReaction(character*)
{
  if((RAND() % 3 && GetHP() < GetMaxHP() / 3) || !(RAND() % 10))
  {
    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s disappears!", CHARNAME(DEFINITE));

    Move(GetLSquareUnder()->GetLevelUnder()->RandomSquare(this, true), true);

    if(GetLSquareUnder()->CanBeSeen())
      ADD_MESSAGE("Suddenly %s appears from nothing!", CHARNAME(INDEFINITE));

    return true;
  }

  if(!(RAND() % 3) && MoveRandomly())
    return true;

  return false;
}

void unicorn::CreateInitialEquipment()
{
  if(RAND() % 2)
    GetStack()->FastAddItem(new astone);

  if(RAND() % 2)
    GetStack()->FastAddItem(new astone);
}

void humanoid::SetSize(ushort Size)
{
  if(GetHead())
    GetHead()->SetSize(HeadSize());

  if(GetTorso())
    GetTorso()->SetSize(TorsoSize());

  if(GetGroin())
    GetGroin()->SetSize(GroinSize());

  if(GetRightArm())
    GetRightArm()->SetSize(ArmSize());

  if(GetLeftArm())
    GetLeftArm()->SetSize(ArmSize());

  if(GetRightLeg())
    GetRightLeg()->SetSize(LegSize());

  if(GetLeftLeg())
    GetLeftLeg()->SetSize(LegSize());
}

ushort humanoid::GetSize() const
{
  ushort Size = 0;

  if(GetHead())
    Size += GetHead()->GetSize();

  if(GetTorso())
    Size += GetTorso()->GetSize();

  if(GetLeftLeg() && GetRightLeg())
    Size += Max(GetLeftLeg()->GetSize(), GetRightLeg()->GetSize());
  else
    if(GetLeftLeg())
      Size += GetLeftLeg()->GetSize();
    else
      if(GetRightLeg())
	Size += GetRightLeg()->GetSize();

  return Size;
}

ushort humanoid::HeadSize() const
{
  return 20;
}

ushort humanoid::TorsoSize() const
{
  return (TotalSize() - HeadSize()) * 2 / 5;
}

ushort humanoid::ArmSize() const
{
  return (TotalSize() - HeadSize()) * 3 / 5;
}

ushort humanoid::GroinSize() const
{
  return (TotalSize() - HeadSize()) / 3;
}

ushort humanoid::LegSize() const
{
  return (TotalSize() - HeadSize()) * 3 / 5;
}

ulong humanoid::HeadVolume() const
{
  return 4000;
}

ulong humanoid::TorsoVolume() const
{
  return TotalVolume() - HeadVolume() - ArmVolume() * 2 - GroinVolume() - LegVolume() * 2;
}

ulong humanoid::ArmVolume() const
{
  return (TotalVolume() - HeadVolume()) / 10;
}

ulong humanoid::GroinVolume() const
{
  return (TotalVolume() - HeadVolume()) / 10;
}

ulong humanoid::LegVolume() const
{
  return (TotalVolume() - HeadVolume()) * 2 / 15;
}

void humanoid::CreateBodyParts()
{
  CreateHead();
  CreateTorso();
  CreateRightArm();
  CreateLeftArm();
  CreateGroin();
  CreateRightLeg();
  CreateLeftLeg();
}

void humanoid::RestoreBodyParts()
{
  if(!GetHead())
    CreateHead();

  if(!GetTorso())
    CreateTorso();

  if(!GetRightArm())
    CreateRightArm();

  if(!GetLeftArm())
    CreateLeftArm();

  if(!GetGroin())
    CreateGroin();

  if(!GetRightLeg())
    CreateRightLeg();

  if(!GetLeftLeg())
    CreateLeftLeg();
}

void humanoid::UpdateBodyPartPictures(bool CallUpdatePicture)
{
  UpdateHeadPicture(CallUpdatePicture);
  UpdateTorsoPicture(CallUpdatePicture);
  UpdateRightArmPicture(CallUpdatePicture);
  UpdateLeftArmPicture(CallUpdatePicture);
  UpdateGroinPicture(CallUpdatePicture);
  UpdateRightLegPicture(CallUpdatePicture);
  UpdateLeftLegPicture(CallUpdatePicture);
}

void humanoid::CreateHead()
{
  SetHead(new head(false));
  UpdateHeadPicture(false);
  GetHead()->InitMaterials(2, CreateHeadFlesh(HeadVolume() * (100 - HeadBonePercentile()) / 100), CreateHeadBone(TorsoVolume() * HeadBonePercentile() / 100));
  /*GetHead()->SetMaster(this);
  GetHead()->SetAttached(true);*/
  GetHead()->PlaceToSlot(GetHeadSlot());
  GetHead()->SetSize(HeadSize());
}

void humanoid::UpdateHeadPicture(bool CallUpdatePicture)
{
  if(GetHead())
    {
      GetHead()->SetBitmapPos(vector2d(96 + (GetHeadType() / 16) * 16, (GetHeadType() % 16) * 16));
      GetHead()->SetColor(0, SkinColor());
      GetHead()->SetColor(1, CapColor());
      GetHead()->SetColor(2, HairColor());
      GetHead()->SetColor(3, EyeColor());

      if(CallUpdatePicture)
	GetHead()->UpdatePicture();
    }
}

void humanoid::CreateTorso()
{
  SetTorso(new humanoidtorso(false));
  UpdateTorsoPicture(false);
  GetTorso()->InitMaterials(2, CreateTorsoFlesh(TorsoVolume() * (100 - TorsoBonePercentile()) / 100), CreateTorsoBone(TorsoVolume() * TorsoBonePercentile() / 100));
  /*GetTorso()->SetMaster(this);
  GetTorso()->SetAttached(true);*/
  GetTorso()->PlaceToSlot(GetTorsoSlot());
  GetTorso()->SetSize(TorsoSize());
}

void humanoid::UpdateTorsoPicture(bool CallUpdatePicture)
{
  if(GetTorso())
    {
      GetTorso()->SetBitmapPos(vector2d(32 + (GetTorsoType() / 16) * 16, (GetTorsoType() % 16) * 16));
      GetTorso()->SetColor(0, SkinColor());
      GetTorso()->SetColor(1, TorsoMainColor());
      GetTorso()->SetColor(2, BeltColor());
      GetTorso()->SetColor(3, TorsoSpecialColor());

      if(CallUpdatePicture)
	GetTorso()->UpdatePicture();
    }
}

void humanoid::CreateRightArm()
{
  SetRightArm(new rightarm(false));
  UpdateRightArmPicture(false);
  GetRightArm()->InitMaterials(2, CreateRightArmFlesh(RightArmVolume() * (100 - RightArmBonePercentile()) / 100), CreateRightArmBone(RightArmVolume() * RightArmBonePercentile() / 100));
  /*GetRightArm()->SetMaster(this);
  GetRightArm()->SetAttached(true);*/
  GetRightArm()->PlaceToSlot(GetRightArmSlot());
  GetRightArm()->SetSize(RightArmSize());
}

void humanoid::UpdateRightArmPicture(bool CallUpdatePicture)
{
  if(GetRightArm())
    {
      GetRightArm()->SetBitmapPos(vector2d(64 + (GetRightArmType() / 16) * 16, (GetRightArmType() % 16) * 16));
      GetRightArm()->SetColor(0, SkinColor());
      GetRightArm()->SetColor(1, ArmMainColor());
      GetRightArm()->SetColor(3, ArmSpecialColor());

      if(CallUpdatePicture)
	GetRightArm()->UpdatePicture();
    }
}

void humanoid::CreateLeftArm()
{
  SetLeftArm(new leftarm(false));
  UpdateLeftArmPicture(false);
  GetLeftArm()->InitMaterials(2, CreateLeftArmFlesh(LeftArmVolume() * (100 - LeftArmBonePercentile()) / 100), CreateLeftArmBone(LeftArmVolume() * LeftArmBonePercentile() / 100));
  //GetLeftArm()->SetMaster(this);
  GetLeftArm()->PlaceToSlot(GetLeftArmSlot());
  GetLeftArm()->SetSize(LeftArmSize());
}

void humanoid::UpdateLeftArmPicture(bool CallUpdatePicture)
{
  if(GetLeftArm())
    {
      GetLeftArm()->SetBitmapPos(vector2d(64 + (GetLeftArmType() / 16) * 16, (GetLeftArmType() % 16) * 16));
      GetLeftArm()->SetColor(0, SkinColor());
      GetLeftArm()->SetColor(1, ArmMainColor());
      GetLeftArm()->SetColor(3, ArmSpecialColor());

      if(CallUpdatePicture)
	GetLeftArm()->UpdatePicture();
    }
}

void humanoid::CreateGroin()
{
  SetGroin(new groin(false));
  UpdateGroinPicture(false);
  GetGroin()->InitMaterials(2, CreateGroinFlesh(GroinVolume() * (100 - GroinBonePercentile()) / 100), CreateGroinBone(GroinVolume() * GroinBonePercentile() / 100));
  /*GetGroin()->SetMaster(this);
  GetGroin()->SetAttached(true);*/
  GetGroin()->PlaceToSlot(GetGroinSlot());
  GetGroin()->SetSize(GroinSize());
}

void humanoid::UpdateGroinPicture(bool CallUpdatePicture)
{
  if(GetGroin())
    {
      GetGroin()->SetBitmapPos(vector2d((GetGroinType() / 16) * 16, (GetGroinType() % 16) * 16));
      GetGroin()->SetColor(0, SkinColor());
      GetGroin()->SetColor(1, LegMainColor());
      GetGroin()->SetColor(3, LegSpecialColor());

      if(CallUpdatePicture)
	GetGroin()->UpdatePicture();
    }
}

void humanoid::CreateRightLeg()
{
  SetRightLeg(new rightleg(false));
  UpdateRightLegPicture(false);
  GetRightLeg()->InitMaterials(2, CreateRightLegFlesh(RightLegVolume() * (100 - RightLegBonePercentile()) / 100), CreateRightLegBone(RightLegVolume() * RightLegBonePercentile() / 100));
  /*GetRightLeg()->SetMaster(this);
  GetRightLeg()->SetAttached(true);*/
  GetRightLeg()->PlaceToSlot(GetRightLegSlot());
  GetRightLeg()->SetSize(RightLegSize());
}

void humanoid::UpdateRightLegPicture(bool CallUpdatePicture)
{
  if(GetRightLeg())
    {
      GetRightLeg()->SetBitmapPos(vector2d((GetRightLegType() / 16) * 16, (GetRightLegType() % 16) * 16));
      GetRightLeg()->SetColor(0, SkinColor());
      GetRightLeg()->SetColor(1, LegMainColor());
      GetRightLeg()->SetColor(3, LegSpecialColor());

      if(CallUpdatePicture)
	GetRightLeg()->UpdatePicture();
    }
}

void humanoid::CreateLeftLeg()
{
  SetLeftLeg(new leftleg(false));
  UpdateLeftLegPicture(false);
  GetLeftLeg()->InitMaterials(2, CreateLeftLegFlesh(LeftLegVolume() * (100 - LeftLegBonePercentile()) / 100), CreateLeftLegBone(LeftLegVolume() * LeftLegBonePercentile() / 100));
  /*GetLeftLeg()->SetMaster(this);
  GetLeftLeg()->SetAttached(true);*/
  GetLeftLeg()->PlaceToSlot(GetLeftLegSlot());
  GetLeftLeg()->SetSize(LeftLegSize());
}

void humanoid::UpdateLeftLegPicture(bool CallUpdatePicture)
{
  if(GetLeftLeg())
    {
      GetLeftLeg()->SetBitmapPos(vector2d((GetLeftLegType() / 16) * 16, (GetLeftLegType() % 16) * 16));
      GetLeftLeg()->SetColor(0, SkinColor());
      GetLeftLeg()->SetColor(1, LegMainColor());
      GetLeftLeg()->SetColor(3, LegSpecialColor());

      if(CallUpdatePicture)
	GetLeftLeg()->UpdatePicture();
    }
}

head* humanoid::GetHead() const { return (head*)GetBodyPart(1); }
void humanoid::SetHead(head* What) { SetBodyPart(1, What); }
rightarm* humanoid::GetRightArm() const { return (rightarm*)GetBodyPart(2); }
void humanoid::SetRightArm(rightarm* What) { SetBodyPart(2, What); }
leftarm* humanoid::GetLeftArm() const { return (leftarm*)GetBodyPart(3); }
void humanoid::SetLeftArm(leftarm* What) { SetBodyPart(3, What); }
groin* humanoid::GetGroin() const { return (groin*)GetBodyPart(4); }
void humanoid::SetGroin(groin* What) { SetBodyPart(4, What); }
rightleg* humanoid::GetRightLeg() const { return (rightleg*)GetBodyPart(5); }
void humanoid::SetRightLeg(rightleg* What) { SetBodyPart(5, What); }
leftleg* humanoid::GetLeftLeg() const { return (leftleg*)GetBodyPart(6); }
void humanoid::SetLeftLeg(leftleg* What) { SetBodyPart(6, What); }

humanoidtorso* humanoid::GetHumanoidTorso() const { return (humanoidtorso*)GetBodyPart(0); }
void humanoid::SetHumanoidTorso(humanoidtorso* What) { SetBodyPart(0, What); }

bool humanoid::BodyPartVital(uchar Index)
{
  if(Index == 0 || Index == 1 || Index == 4)
    return true;
  else
    return false;
}

bool humanoid::BodyPartCanBeSevered(uchar Index) const
{
  if(Index == 0 || Index == 4)
    return false;
  else
    return true;
}

void humanoid::DrawToTileBuffer() const
{
  /* Order is important, so don't use a loop */

  if(GetGroin())
    GetGroin()->DrawToTileBuffer();

  if(GetRightLeg())
    GetRightLeg()->DrawToTileBuffer();

  if(GetLeftLeg())
    GetLeftLeg()->DrawToTileBuffer();

  if(GetTorso())
    GetTorso()->DrawToTileBuffer();

  if(GetRightArm())
    GetRightArm()->DrawToTileBuffer();

  if(GetLeftArm())
    GetLeftArm()->DrawToTileBuffer();

  if(GetHead())
    GetHead()->DrawToTileBuffer();
}

void dwarf::DrawToTileBuffer() const
{
  if(GetGroin())
    GetGroin()->DrawToTileBuffer(vector2d(0, -1));

  if(GetRightLeg())
    GetRightLeg()->DrawToTileBuffer(vector2d(0, -1));

  if(GetLeftLeg())
    GetLeftLeg()->DrawToTileBuffer(vector2d(0, -1));

  if(GetTorso())
    GetTorso()->DrawToTileBuffer();
  
  if(GetRightArm())
    GetRightArm()->DrawToTileBuffer();

  if(GetLeftArm())
    GetLeftArm()->DrawToTileBuffer();

  if(GetHead())
    GetHead()->DrawToTileBuffer(vector2d(0, 1));
}

bool humanoid::ReceiveEffect(short Amount, uchar Type, uchar TargetFlags, uchar Direction, bool Divide, bool PenetrateArmor, bool Critical)
{
  std::vector<uchar> ChooseFrom;

  if(TargetFlags & TORSO && GetTorso())
    ChooseFrom.push_back(0);

  if(TargetFlags & HEAD && GetHead())
    ChooseFrom.push_back(1);

  if(TargetFlags & RIGHTARM && GetRightArm())
    ChooseFrom.push_back(2);

  if(TargetFlags & LEFTARM && GetLeftArm())
    ChooseFrom.push_back(3);

  if(TargetFlags & GROIN && GetGroin())
    ChooseFrom.push_back(4);

  if(TargetFlags & RIGHTLEG && GetRightLeg())
    ChooseFrom.push_back(5);

  if(TargetFlags & LEFTLEG && GetLeftLeg())
    ChooseFrom.push_back(6);

  if(!ChooseFrom.size())
    return false;

  if(Divide)
    {
      bool Affected = false;

      for(ushort c = 0; c < ChooseFrom.size(); ++c)
	if(ReceiveBodyPartEffect(Amount / ChooseFrom.size(), Type, ChooseFrom[c], Direction, PenetrateArmor, Critical))
	  Affected = true;

      return Affected;
    }
  else
    return ReceiveBodyPartEffect(Amount, Type, ChooseFrom[RAND() % ChooseFrom.size()], Direction, PenetrateArmor, Critical);
}

item* humanoid::GetMainWielded() const
{
  if(GetMainArm())
    return GetMainArm()->GetWielded();
  else
    return 0;
}

item* humanoid::GetSecondaryWielded() const
{
  if(GetSecondaryArm())
    return GetSecondaryArm()->GetWielded();
  else
    return 0;
}

item* humanoid::GetBodyArmor() const
{
  return GetHumanoidTorso()->GetBodyArmor();
}

arm* humanoid::GetMainArm() const
{
  if(GetRightArm())
    return GetRightArm();
  else
    return GetLeftArm();
}

arm* humanoid::GetSecondaryArm() const
{
  return GetLeftArm();
}

void humanoid::SetMainWielded(item* Item)
{
  if(GetMainArm())
    GetMainArm()->SetWielded(Item);
  else
    ABORT("Right hand in wrong place!");
}

void humanoid::SetSecondaryWielded(item* Item)
{
  if(GetSecondaryArm())
    GetSecondaryArm()->SetWielded(Item);
  else
    ABORT("Left hand in wrong place!");
}

void humanoid::SetBodyArmor(item* Item)
{
  GetHumanoidTorso()->SetBodyArmor(Item);
}

bool humanoid::CanWieldInMainHand() const
{
  return GetMainArm() ? true : false;
}

bool humanoid::CanWieldInSecondaryHand() const
{
  return GetSecondaryArm() ? true : false;
}

/*float humanoid::GetMainToHitValue() const
{
  if(GetWielded())
    return GetMeleeAttributeModifier() * GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus() / sqrt(GetWielded()->GetWeight() > 400 ? GetWielded()->GetWeight() : 400) * 10;
  else
    return (GetMeleeAttributeModifier() >> 1) * GetCategoryWeaponSkill(UNARMED)->GetBonus();
}

float humanoid::GetSecondaryToHitValue() const
{
  if(GetWielded())
    return GetMeleeAttributeModifier() * GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus() / sqrt(GetWielded()->GetWeight() > 400 ? GetWielded()->GetWeight() : 400) * 10;
  else
    return (GetMeleeAttributeModifier() >> 1) * GetCategoryWeaponSkill(UNARMED)->GetBonus();
}*/

bool humanoid::EqupmentScreen()
{
  return false;
}
