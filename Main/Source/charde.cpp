#include <cmath>

#include "charde.h"
#include "stack.h"
#include "itemde.h"
#include "level.h"
#include "lsquare.h"
#include "message.h"
#include "igraph.h"
#include "bitmap.h"
#include "hscore.h"
#include "save.h"
#include "god.h"
#include "feio.h"
#include "wskill.h"
#include "felist.h"
#include "strover.h"
#include "team.h"
#include "lterraba.h"
#include "femath.h"
#include "error.h"

petrus::~petrus()
{
  game::SetPetrus(0);
}

void humanoid::VirtualConstructor()
{
  for(uchar c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    CategoryWeaponSkill[c] = new gweaponskill(c);

  CurrentSingleWeaponSkill = 0;
}

humanoid::~humanoid()
{
  for(uchar c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    delete CategoryWeaponSkill[c];
}

void petrus::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new valpurusjustifier)));
  SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new platemail(new valpurium))));
}

void priest::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new spikedmace)));
  SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new chainmail(new mithril))));
}

void oree::CreateInitialEquipment()
{
  SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new goldeneagleshirt)));
  item* Can = new can(false);
  Can->InitMaterials(2, new iron(10), new pepsi(330));
  GetStack()->FastAddItem(Can);
}

void darkknight::CreateInitialEquipment()
{
  if(RAND() % 20)
    SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(RAND() % 5 ? (item*)(new longsword) : RAND() % 20 ? (item*)(new twohandedsword) : (item*)(new curvedtwohandedsword))));
  else
    {
      item* DoomsDay = new longsword;
      DoomsDay->InitMaterials(3, new mithril, new iron, new darkfrogflesh);
      SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(DoomsDay)));
    }

  SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new chainmail(RAND() % 5 ? (material*)new iron : (material*)new mithril))));
}

void skeleton::CreateInitialEquipment()
{
  if(RAND() % 10)
    SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(RAND() % 3 ? (item*)new axe : (item*)new pickaxe)));
  else
    {
      SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new spikedmace)));
      SetSize(200);
      SetAgility(15);
      SetStrength(18);
      SetEndurance(15);
    }
}

void goblin::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(RAND() % 3 ? (item*)new spear : (item*)new poleaxe)));
}

void guard::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new longsword(new iron))));
  SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new chainmail)));

  GetCategoryWeaponSkill(LARGE_SWORDS)->AddHit(500);
  GetCurrentSingleWeaponSkill()->AddHit(500);
}

ushort humanoid::CalculateArmorModifier() const
{
  return Armor.Torso ? Armor.Torso->GetArmorValue() : 100;
}

bool ennerbeast::Hit(character*)
{
  char Message[256] ;
  if(RAND() % 2) sprintf(Message,"The Enner Beast yells: UGH UGHAaaa!");
  else sprintf(Message, "The Enner Beast yells: Uga Ugar Ugade Ugat!");

  DO_FILLED_RECTANGLE(GetPos().X, GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, 30,
  {
    character* Char = game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer, YPointer))->GetCharacter();

    float ScreamStrength = GetMeleeStrength() * GetStrength() / GetHypotSquare(float(GetPos().X) - XPointer, float(GetPos().Y) - YPointer);

    if(Char && Char != this)
      Char->ReceiveSound(Message, RAND() % 26 - RAND() % 26,ScreamStrength);

    game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer, YPointer))->GetStack()->ReceiveSound(ScreamStrength);

    for(uchar x = 0; x < 4; ++x)
      game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer, YPointer))->GetSideStack(x)->ReceiveSound(ScreamStrength);
  });

  SetStrengthExperience(GetStrengthExperience() + 100);

  return true;
}

void golem::DrawToTileBuffer() const
{
  Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

void complexhumanoid::DrawToTileBuffer() const
{	
  vector2d InHandsPic, LegPos, TorsoPos, ArmPos, HeadPos, ShieldPos;

  if(GetLegType() >= 16)
    {
      LegPos.X = 16;
      LegPos.Y = (GetLegType() - 16) << 4;
    }
  else
    {
      LegPos.X = 0;
      LegPos.Y = GetLegType() << 4;
    }

  if(GetTorsoType() >= 16)
    {
      TorsoPos.X = 48;
      TorsoPos.Y = (GetTorsoType() - 16) << 4;
    }
  else
    {
      TorsoPos.X = 32;
      TorsoPos.Y = GetTorsoType() << 4;
    }

  if(GetArmType() >= 16)
    {
      ArmPos.X = 80;
      ArmPos.Y = (GetArmType() - 16) << 4;
    }
  else
    {
      ArmPos.X = 64;
      ArmPos.Y = GetArmType() << 4;
    }

  if(GetHeadType() >= 16)
    {
      HeadPos.X = 112;
      HeadPos.Y = (GetHeadType() - 16) << 4;
    }
  else
    {
      HeadPos.X = 96;
      HeadPos.Y = GetHeadType() << 4;
    }

  if(GetShieldType() >= 16)
    {
      ShieldPos.X = 144;
      ShieldPos.Y = (GetShieldType() - 16) << 4;
    }
  else
    {
      ShieldPos.X = 128;
      ShieldPos.Y = GetShieldType() << 4;
    }

  DrawLegs(LegPos);
  DrawTorso(TorsoPos);
  DrawArms(ArmPos);
  DrawHead(HeadPos);
  DrawShield(ShieldPos);

  if(GetWielded())
    {
      InHandsPic = GetWielded()->GetInHandsPic();

      if(InHandsPic.X != 0 || InHandsPic.Y != 0)
	DrawInHandsPic(InHandsPic);
    }
}

void complexhumanoid::DrawLegs(vector2d Pos) const { igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), Pos.X, Pos.Y, 0, 0, 16, 16); }
void complexhumanoid::DrawTorso(vector2d Pos) const { igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), Pos.X, Pos.Y, 0, 0, 16, 16); }
void complexhumanoid::DrawArms(vector2d Pos) const { igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), Pos.X, Pos.Y, 0, 0, 16, 16); }
void complexhumanoid::DrawHead(vector2d Pos) const { igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), Pos.X, Pos.Y, 0, 0, 16, 16); }
void complexhumanoid::DrawShield(vector2d Pos) const { igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), Pos.X, Pos.Y, 0, 0, 16, 16); }
void complexhumanoid::DrawInHandsPic(vector2d Pos) const { igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), Pos.X, Pos.Y, 0, 0, 16, 16); }

void skeleton::CreateCorpse()
{
  ushort Amount = 2 + RAND() % 4;

  for(ushort c = 0; c < Amount; ++c)
    GetLevelSquareUnder()->GetStack()->AddItem(new abone);
}

void elpuri::CreateCorpse()
{
  character::CreateCorpse();

  GetLevelSquareUnder()->GetStack()->AddItem(new headofelpuri);
}

void petrus::CreateCorpse()
{
  GetLevelSquareUnder()->GetStack()->AddItem(new leftnutofpetrus);
}

void ennerbeast::CreateCorpse()
{
  character::CreateCorpse();

  GetLevelSquareUnder()->GetStack()->AddItem(new headofennerbeast);
}

bool humanoid::WearArmor()
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
      return true;
    }
  else
    if(Index < GetStack()->GetItems()) // Other Armor types should be coded...
      if(GetStack()->GetItem(Index)->CanBeWorn())
	if(GetStack()->GetItem(Index) != GetWielded())
	  {
	    Armor.Torso = GetStack()->GetItem(Index);
	    return true;
	  }
	else
	  ADD_MESSAGE("You can't wear something that you wield!");
      else
	ADD_MESSAGE("You can't wear THAT!");

  return false;
}

void humanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);

  ushort Index = Armor.Torso ? Stack->SearchItem(Armor.Torso) : 0xFFFF;

  SaveFile << Index << SingleWeaponSkill;

  for(uchar c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    SaveFile << GetCategoryWeaponSkill(c);
}

void humanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);

  ushort Index;

  SaveFile >> Index >> SingleWeaponSkill;

  Armor.Torso = Index != 0xFFFF ? Stack->GetItem(Index) : 0;

  for(uchar c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    SaveFile >> GetCategoryWeaponSkill(c);

  if(GetWielded())
    for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
      if((*i)->GetID() == Wielded->GetID())
	{
	  SetCurrentSingleWeaponSkill(*i);
	  break;
	}
}

void complexhumanoid::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);

  SaveFile << ArmType << HeadType << LegType << TorsoType << ShieldType;
}

void complexhumanoid::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);

  SaveFile >> ArmType >> HeadType >> LegType >> TorsoType >> ShieldType;
}

float golem::GetMeleeStrength() const
{
  return 150 * GetMaterial(0)->GetHitValue();
}

ushort golem::CalculateArmorModifier() const
{
  if(((GetMaterial(0)->GetArmorValue() * 3) >> 2) > 90)
    return 10;
  else
    return 100 - ((GetMaterial(0)->GetArmorValue() * 3) >> 2);
}

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
			if((Char = game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter()))
  {
    if(GetTeam()->GetRelation(Char->GetTeam()) == FRIEND && Char->GetHP() < Char->GetMaxHP() / 3 && GetHealTimer() > 100)
      {
	HealFully(Char);
	return;
      }
  })

    if(CheckForEnemies(true))
      return;

  if(CheckForDoors())
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  FollowLeader();
}

void petrus::HealFully(character* ToBeHealed)
{
  SetHealTimer(0);

  ToBeHealed->SetHP(ToBeHealed->GetMaxHP());

  if(ToBeHealed->GetIsPlayer())
    ADD_MESSAGE("%s heals you fully.", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
  else
    ADD_MESSAGE("%s heals %s fully.", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something", ToBeHealed->GetSquareUnder()->CanBeSeen() ? ToBeHealed->CNAME(DEFINITE) : "something");
}

void petrus::Save(outputfile& SaveFile) const
{
  complexhumanoid::Save(SaveFile);

  SaveFile << HealTimer << StoryState;
}

void petrus::Load(inputfile& SaveFile)
{
  complexhumanoid::Load(SaveFile);

  SaveFile >> HealTimer >> StoryState;

  game::SetPetrus(this);
}

bool dog::Catches(item* Thingy, float)
{
  if(Thingy->DogWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy, GetLevelSquareUnder()->GetStack()))
	if(GetIsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CNAME(DEFINITE));
	else
	  {
	    if(GetLevelSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s catches %s and eats it.", CNAME(DEFINITE), Thingy->CNAME(DEFINITE));

	    ChangeTeam(game::GetPlayer()->GetTeam());
	  }
      else
	if(GetIsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air.", Thingy->CNAME(DEFINITE));
	else
	  if(GetLevelSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s catches %s.", CNAME(DEFINITE), Thingy->CNAME(DEFINITE));

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

float humanoid::GetAttackStrength() const
{
  return GetWielded() ? GetWielded()->GetWeaponStrength() * GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus() : GetMeleeStrength() * GetCategoryWeaponSkill(UNARMED)->GetBonus();
}

bool humanoid::Hit(character* Enemy)
{
  if(GetIsPlayer() && GetTeam()->GetRelation(Enemy->GetTeam()) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  Hostility(Enemy);

  short Success = RAND() % 26 - RAND() % 26;

  switch(Enemy->TakeHit(this, Success)) //there's no breaks and there shouldn't be any
    {
    case HAS_HIT:
    case HAS_BLOCKED:
      if(GetWielded())
	GetWielded()->ReceiveHitEffect(Enemy, this);
    case HAS_DIED:
      SetStrengthExperience(GetStrengthExperience() + 50);
      if(GetCategoryWeaponSkill(GetWielded() ? GetWielded()->GetWeaponCategory() : UNARMED)->AddHit() && GetIsPlayer())
	GetCategoryWeaponSkill(GetWielded() ? GetWielded()->GetWeaponCategory() : UNARMED)->AddLevelUpMessage();
      if(GetWielded())
	{
	  if(GetCurrentSingleWeaponSkill()->AddHit() && GetIsPlayer())
	    GetCurrentSingleWeaponSkill()->AddLevelUpMessage(GetWielded()->Name(UNARTICLED));

	  if(GetWielded()->ImpactDamage(GetStrength() / 2, GetLevelSquareUnder()->CanBeSeen(), GetStack()))
	    SetWielded(GetStack()->GetItem(GetStack()->GetItems() - 1));

	}
    case HAS_DODGED:
      SetAgilityExperience(GetAgilityExperience() + 25);
    }

  SetNP(GetNP() - 4);

  return true;
}

void humanoid::CharacterSpeciality()
{
  for(uchar c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
    if(GetCategoryWeaponSkill(c)->Turn() && GetIsPlayer())
      GetCategoryWeaponSkill(c)->AddLevelDownMessage();

  for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end();)
    {
      if((*i)->Turn() && GetIsPlayer())
	for(ushort c = 0; c < GetStack()->GetItems(); ++c)
	  if((*i)->GetID() == GetStack()->GetItem(c)->GetID())
	    {
	      (*i)->AddLevelDownMessage(GetStack()->GetItem(c)->Name(UNARTICLED));
	      break;
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

bool humanoid::ShowWeaponSkills()
{
  {
    felist List("Your experience in weapon categories", 0xFFFF, 0, false);

    List.AddDescription("");
    List.AddDescription("Category name                 Level     Points    To next level");

    for(uchar c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
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

    if(CurrentSingleWeaponSkill)
      {
	List.AddEntry("", RED);

	std::string Buffer = "current single weapon skill:  ";

	Buffer += CurrentSingleWeaponSkill->GetLevel();
	Buffer.resize(40, ' ');

	Buffer += int(CurrentSingleWeaponSkill->GetHits());
	Buffer.resize(50, ' ');

	if(CurrentSingleWeaponSkill->GetLevel() != 10)
	  List.AddEntry(Buffer + (CurrentSingleWeaponSkill->GetLevelMap(CurrentSingleWeaponSkill->GetLevel() + 1) - CurrentSingleWeaponSkill->GetHits()), RED);
	else
	  List.AddEntry(Buffer + '-', RED);
      }

    List.Draw();
  }

  return false;
}

void humanoid::SetWielded(item* Something)
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
}

float humanoid::GetToHitValue() const
{
  if(GetWielded())
    return GetMeleeAttributeModifier() * GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus() / sqrt(GetWielded()->GetWeight() > 400 ? GetWielded()->GetWeight() : 400) * 10;
  else
    return (GetMeleeAttributeModifier() >> 1) * GetCategoryWeaponSkill(UNARMED)->GetBonus();
}

void shopkeeper::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new pickaxe(new mithril))));
  SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new chainmail(new mithril))));

  GetCategoryWeaponSkill(AXES)->AddHit(5000);
  GetCurrentSingleWeaponSkill()->AddHit(5000);
}

void farmer::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new axe)));
}

void petrus::AddHitMessage(character* Enemy, const bool Critical) const
{
  /*
   * This function is temporary. It prevents Petrus's hit messages becoming too long,
   * which would at present make the message history quite ugly.
   */

  std::string ThisDescription = GetLevelSquareUnder()->CanBeSeen() ? "Petrus" : "something";
  std::string EnemyDescription = Enemy->GetLevelSquareUnder()->CanBeSeen() ? Enemy->CNAME(DEFINITE) : "something";

  if(Enemy->GetIsPlayer())
    if(GetWielded() && GetLevelSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s %s you with %s %s!", ThisDescription.c_str(), ThirdPersonWeaponHitVerb(Critical).c_str(), game::PossessivePronoun(GetSex()), Wielded->CNAME(0));
    else
      ADD_MESSAGE("%s %s you!", ThisDescription.c_str(), ThirdPersonMeleeHitVerb(Critical).c_str());
  else
    if(GetIsPlayer())
      ADD_MESSAGE("You %s %s!", FirstPersonHitVerb(Enemy, Critical).c_str(), EnemyDescription.c_str());
    else
      if(GetLevelSquareUnder()->CanBeSeen() || Enemy->GetLevelSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s %s %s!", ThisDescription.c_str(), AICombatHitVerb(Enemy, Critical).c_str(), EnemyDescription.c_str());
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
      ADD_MESSAGE("%s seems suspicious. \"You look like one from Istour! Go away!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      break;
    case 2:
      ADD_MESSAGE("%s sighs: \"Again polar bears ate my cattle...\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
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

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      if(GetLevelSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s says gravely: \"You don't have a life. Get it in the army.\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      else
	ADD_MESSAGE("%s says gravely: \"You don't have a life. Get it as shop guard.\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      break;
    case 1:
      if(GetLevelSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of breaking rules.\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      else
	ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of stealing anything.\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      break;
    case 2:
      if(GetLevelSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s shouts excited: \"Attnam victoor!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      else
	ADD_MESSAGE("\"Yes, this is a dangerous place to work, but our boss pays us well.\"");
      break;
    case 3:
      if(GetLevelSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"The High Priest is my idol. I would want a sword as big as his!\"");
      else
        {
	  ADD_MESSAGE("\"Attnam's guards can barely wield a sword.");
          ADD_MESSAGE("But we are trained by the laws of the dungeon, so don't make anything suspicious here.\"");
        }
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
      if(GetLevelSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s sighs: \"If only I hadn't chosen a city in the middle of nowhere...\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      else
	ADD_MESSAGE("%s sighs: \"I wonder why I have so few customers these days...\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      break;
    case 1:
      ADD_MESSAGE("%s sighs: \"Mutant mushrooms ate the last caravan.", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      ADD_MESSAGE("The one before it ran into an Enner Beast. It must be all Elpuri's doings!\"");
      break;
    case 2:
      if(GetLevelSquareUnder()->GetLevelUnder()->GetOnGround())
	{
	  ADD_MESSAGE("\"You truly can't find better prices in this city!\", %s smiles.", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
	  ADD_MESSAGE("\"Indeed, you can't find ANY prices, since my store is a monopoly.\"");
	}
      else
	ADD_MESSAGE("\"The topmost reason why I work here is that the monsters around here devour tax collectors.\"");
      break;
    case 3:
      if(GetLevelSquareUnder()->GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"Don't try anything. The High Priest is a LAN friend of mine.\"");
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
      ADD_MESSAGE("%s talks to you:", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      game::GetGod(GetLevelSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetDivineOwner())->AddPriestMessage();
    }
}

void oree::BeTalkedTo(character*)
{
	ADD_MESSAGE("%s laughs: \"No time for small talk. Time to drink blood!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
}

void darkknight::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s yells: \"For Erado I shall slay you!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
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
  ADD_MESSAGE("%s roars horribly: \"DiE hUmAn!!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
}

void billswill::BeTalkedTo(character*)
{
  ADD_MESSAGE("\"Windows XP is coming. You will install it. Resistance is futile. Prepare to be assimilited.\"");
}

void skeleton::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("%s sings: \"Leg bone is connected to the hib bone, hib bone is connected to the rib bone...\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
  else
    ADD_MESSAGE("%s grunts: \"Bones. Need more bones.\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
}

void goblin::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("%s laughs: \"Humie friend. Many mommo we kill. Many spider we eat.\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
  else
    ADD_MESSAGE("%s yells goblin war cries at you.", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
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

  for(c = 0; c < SingleWeaponSkill.size(); ++c)
    SkillScore += SingleWeaponSkill[c]->GetHits();

  return (SkillScore >> 2) + character::StatScore();
}

void humanoid::AddSpecialItemInfo(std::string& Description, item* Item)
{
  Description.resize(75, ' ');
  Description += GetCategoryWeaponSkill(Item->GetWeaponCategory())->GetLevel();
  Description.resize(80, ' ');

  for(ushort c = 0; c < SingleWeaponSkill.size(); ++c)
    if(Item->GetID() == SingleWeaponSkill[c]->GetID())
      {
	Description += SingleWeaponSkill[c]->GetLevel();
	return;
      }

  Description += 0;
}

void humanoid::AddSpecialItemInfoDescription(std::string& Description)
{
  Description.resize(78, ' ');
  Description += "GS";
  Description.resize(83, ' ');
  Description += "SS";
}

void humanoid::KickHit()
{
  if(GetCategoryWeaponSkill(UNARMED)->AddHit() && GetIsPlayer())
    GetCategoryWeaponSkill(UNARMED)->AddLevelUpMessage();
}

void ivan::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"You capitalist! Lenin want Ivan kill capitalists!\"");
      return;
    }

  if(GetTeam() == Talker->GetTeam() || Talker->MaxDanger() < 50000)
    {
      static bool Said[11];

      switch(RandomizeReply(11, Said))
	{
	case 0:
	  ADD_MESSAGE("\"Da, Ivan like killing.\"");
	  break;
	case 1:
	  ADD_MESSAGE("\"Ivan ruski specialist.\"");
	  break;
	case 2:
	  ADD_MESSAGE("\"Ivan work. Else Ivan get stapled.\"");
	  break;
	case 3:
	  ADD_MESSAGE("\"Party mean big weapons. Ivan like big weapons. Ivan kill for Party.\"");
	  break;
	case 4:
	  ADD_MESSAGE("\"CCCP roxxx.\"");
	  break;
	case 5:
	  ADD_MESSAGE("\"Ivan like throw Ladas. You want compete?\"");
	  break;
	case 6:
	  ADD_MESSAGE("\"Why is AK not invented?\"");
	  break;
	case 7:
	  ADD_MESSAGE("\"Ivan like Attnam. Petrus communist too.\"");
	  break;
	case 8:
	  ADD_MESSAGE("\"Ivan buy kyber eyes. Ivan see in dark.\"");
	  break;
	case 9:
	  ADD_MESSAGE("\"Uncle Lenin lives in Russia. Lenin strong guy. Ivan like.\"");
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
      ADD_MESSAGE("%s seems to be very friendly. \"You make good communist. Ivan go with you!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      ChangeTeam(Talker->GetTeam());
    }
}

void ivan::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new spikedmace(new iron))));
  SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new platemail)));
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
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new spear(new iron))));
}

void slave::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Yikes!\"");
      return;
    }

  character* Master;

  if(HomeRoom && (Master = GetLevelSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetMaster()))
    {
      if(Talker->GetMoney() >= 50)
	{
	  ADD_MESSAGE("%s talks: \"Do you want to buy me? 50 squirrels. I work very hard.\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");

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
      if(Talker->GetWielded() && Talker->GetWielded()->GetType() == whip::StaticType())
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

  if(CheckForDoors())
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(!HomeRoom || !GetLevelSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetMaster())
    {
      HomeRoom = 0;
      MoveRandomly();
    }
}

bool elpuri::Hit(character* Enemy)
{
  DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
  {
    levelsquare* Square = GetLevelSquareUnder()->GetLevelUnder()->GetLevelSquare(vector2d(DoX, DoY));
    character* ByStander = Square->GetCharacter();

    if(ByStander && (ByStander == Enemy || ByStander->GetTeam()->GetRelation(GetTeam()) == HOSTILE))
      {
	Hostility(ByStander);

	short Success = RAND() % 26 - RAND() % 26;

	switch(ByStander->TakeHit(this, Success))
	  {
	  case HAS_HIT:
	  case HAS_BLOCKED:
	  case HAS_DIED:
	    ByStander->GetStack()->ImpactDamage(GetStrength(), Square->CanBeSeen());
	    ByStander->CheckGearExistence();
	    SetStrengthExperience(GetStrengthExperience() + 50);
	  case HAS_DODGED:
	    SetAgilityExperience(GetAgilityExperience() + 25);
	  }
      }

    Square->GetStack()->ImpactDamage(GetStrength(), Square->CanBeSeen());

    for(uchar c = 0; c < 4; ++c)
      if(Square->GetSideStack(c)->GetSquareTrulyUnder() == GetSquareUnder())
	Square->GetSideStack(c)->ImpactDamage(GetStrength(), Square->CanBeSeen());
  });

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

  static bool Said[4];

  switch(RandomizeReply(4, Said))
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

  static bool Said[10];

  switch(RandomizeReply(10, Said))
    {
    case 0:
      if(game::GetPetrus() && !game::GetPetrus()->GetStoryState())
	{
	  ADD_MESSAGE("\"Thou should visit Petrus if thou areth in need of adventure.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that the wand of polymorph has dozens of uses.\"");
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
	  ADD_MESSAGE("A brave adventurer named Ivan also dived into its cave not long ago.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that chaotic gods offer great power to their followers.");
	  ADD_MESSAGE("But thou must remember: unlike lawfuls, they shalt not help thee when things go bad.\"");
	  break;
	}
    }
}

void femaleslave::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new palmbranch)));
}

bool ivan::MoveRandomly()
{
  switch(RAND() % 1000)
    {
      /*		case 0:
			Engrave("The bourgeois is a bourgeois -- for the benefit of the working class.");
			break;*/
      // Currently too long
    case 1:
      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("Ivan engraves something to the ground.");

      Engrave("Proletarians of all countries, unite!");
      return true;
    case 2:
      if(GetSquareUnder()->CanBeSeen())
	ADD_MESSAGE("Ivan engraves something to the ground.");

      Engrave("Capital is therefore not only personal; it is a social power.");
      return true;
    default:
      return character::MoveRandomly();
    }
}

void zombie::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    ADD_MESSAGE("\"Need brain!!\"");
  else
    ADD_MESSAGE("\"Need brain, but not your brain.\"");
}

void zombie::SpillBlood(uchar HowMuch, vector2d GetPos)
{
  if(!HowMuch)
    return;

  if(!game::GetInWilderness()) 
    {
      game::GetCurrentLevel()->GetLevelSquare(GetPos)->SpillFluid(HowMuch, GetBloodColor(), 5, 60);

      if(!(RAND() % 10)) 
	game::GetCurrentLevel()->GetLevelSquare(GetPos)->GetStack()->AddItem(new lump(new humanflesh(1000)));
    }
}

void mistress::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new whip)));

  if(!(RAND() % 10))
    GetStack()->FastAddItem(new banana);

  if(!(RAND() % 100))
    GetStack()->FastAddItem(new holybanana);

  GetCategoryWeaponSkill(WHIPS)->AddHit(10000);
  GetCurrentSingleWeaponSkill()->AddHit(10000);
}

void werewolf::ChangeIntoHuman()
{
  SetSize(170);
  SetAgility(15);
  SetStrength(15);
  SetEndurance(15);
  SetPerception(15);
  SetLegType(11);
  SetTorsoType(15);
  SetArmType(0);
  SetHeadType(24);
  SetShieldType(0);
  SetIsWolf(false);
  SetStrengthExperience(0);
  SetEnduranceExperience(0);
  SetAgilityExperience(0);
  SetPerceptionExperience(0);
  SetHP(GetMaxHP());

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
  SetLegType(12);
  SetTorsoType(16);
  SetArmType(15);
  SetHeadType(25);
  SetShieldType(0);
  SetIsWolf(true);
  SetWielded(0);
  SetStrengthExperience(0);
  SetEnduranceExperience(0);
  SetAgilityExperience(0);
  SetPerceptionExperience(0);
  SetHP(GetMaxHP());

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
	      ADD_MESSAGE("%s changes into human.", CNAME(DEFINITE));
	    }
	}
      else
	{
	  ChangeIntoWolf();
	  if(GetIsPlayer())
	    ADD_MESSAGE("You change into a wolf... At least for some time.");
	  else if(GetSquareUnder()->CanBeSeen())
	    {
	      ADD_MESSAGE("%s changes into a wolf.", CNAME(DEFINITE));
	    }
	}
    }

  character::Be();
}

ulong werewolf::MaxDanger()
{
  bool BeforeThis = GetIsWolf();

  ChangeIntoWolf();
  ulong AsWolf = complexhumanoid::MaxDanger();
  ChangeIntoHuman();
  ulong AsHuman = complexhumanoid::MaxDanger();

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
  complexhumanoid::Save(SaveFile);

  SaveFile << IsWolf << ChangeCounter;
}

void werewolf::Load(inputfile& SaveFile)
{
  complexhumanoid::Load(SaveFile);

  SaveFile >> IsWolf >> ChangeCounter;
}

void kobold::CreateInitialEquipment()
{
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new spear)));
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
      SetStrength(30);
      SetEndurance(30);
      SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new longsword(new valpurium))));
      SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new chainmail(new valpurium))));
      SetMaterial(1, new goodleather);
      SetHP(GetMaxHP());
      break;
    case NEUTRAL:
      SetAgility(25);
      SetStrength(25);
      SetEndurance(99);
      SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new twohandedsword(new mithril))));
      SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new chainmail(new mithril))));
      SetMaterial(1, new neutralleather);
      SetHP(GetMaxHP());
      break;
    case EVIL:
      {
	SetAgility(25);
	SetStrength(99);
	SetEndurance(25);
	item* SpikedMace = new spikedmace(false);
	SpikedMace->InitMaterials(3, new mithril, new iron, new darkfrogflesh);
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(SpikedMace)));
	SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new brokenplatemail(new mithril))));
	SetMaterial(1, new evilleather);
	SetHP(GetMaxHP());
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
      ADD_MESSAGE("%s shouts: \"Death to disbelievers!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something");
      break;
    case 2:
      ADD_MESSAGE("%s praises %s with numerous hymns.", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something", game::GetGod(Master)->Name().c_str());
      ADD_MESSAGE("%s is obviously a very devoted follower.", game::PersonalPronoun(GetSex()));
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
  SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(HolyBook)));
  GetStack()->FastAddItem(new backpack);
}

bool kamikazedwarf::Hit(character* Enemy)
{
  if(GetIsPlayer())
    humanoid::Hit(Enemy);
  else
    {
      for(ushort c = 0; c < GetStack()->GetItems(); ++c)
	if(GetStack()->GetItem(c)->IsExplosive())
	  {
	    if(RAND() % 2)
	      ADD_MESSAGE("%s shouts: \"For %s!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something", game::GetGod(Master)->Name().c_str());
	    else
	      ADD_MESSAGE("%s screams: \"%s, here I come!\"", GetSquareUnder()->CanBeSeen() ? CNAME(DEFINITE) : "something", game::GetGod(Master)->Name().c_str());

	    if(GetStack()->GetItem(c)->Apply(this, GetStack()))
	      return true;
	  }

      humanoid::Hit(Enemy);
    }

  return true;
}

void kamikazedwarf::Save(outputfile& SaveFile) const
{
  complexhumanoid::Save(SaveFile);

  SaveFile << Master;
}

void kamikazedwarf::Load(inputfile& SaveFile)
{
  complexhumanoid::Load(SaveFile);

  SaveFile >> Master;
}

void humanoid::CheckGearExistence()
{
  character::CheckGearExistence();

  if(GetTorsoArmor() && !GetTorsoArmor()->GetExists())
    SetTorsoArmor(0);
}

bool largecat::Catches(item* Thingy, float)
{
  if(Thingy->CatWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy, GetLevelSquareUnder()->GetStack()))
	if(GetIsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CNAME(DEFINITE));
	else
	  {
	    if(GetLevelSquareUnder()->CanBeSeen())
	      ADD_MESSAGE("%s catches %s and eats it.", CNAME(DEFINITE), Thingy->CNAME(DEFINITE));

	    ChangeTeam(game::GetPlayer()->GetTeam());
	  }
      else
	if(GetIsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air.", Thingy->CNAME(DEFINITE));
	else
	  if(GetLevelSquareUnder()->CanBeSeen())
	    ADD_MESSAGE("%s catches %s.", CNAME(DEFINITE), Thingy->CNAME(DEFINITE));

      return true;
    }
  else
    return false;
}

void dwarf::DrawLegs(vector2d Pos) const { igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), Pos.X, Pos.Y + 1, 0, 0, 16, 15); }
void dwarf::DrawHead(vector2d Pos) const { igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), Pos.X, Pos.Y, 0, 1, 16, 15); }

void unicorn::RandomizeFleshMaterial()
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
  if((!(RAND() % 3) && GetHP() < GetMaxHP() / 3) || !(RAND() % 10))
  {
    if(GetLevelSquareUnder()->CanBeSeen())
      ADD_MESSAGE("%s disappears!", CNAME(DEFINITE));

    Move(GetLevelSquareUnder()->GetLevelUnder()->RandomSquare(this, true), true);

    if(GetLevelSquareUnder()->CanBeSeen())
      ADD_MESSAGE("Suddenly %s appears from nothing!", CNAME(INDEFINITE));

    return true;
  }

  if(!(RAND() % 3) && MoveRandomly())
    return true;

  return false;
}

void unicorn::CreateInitialEquipment()
{
  GetStack()->FastAddItem(new astone);
}
