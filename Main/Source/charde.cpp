#define __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "charba.h"

std::vector<character::prototype*> protocontainer<character>::ProtoData;
valuemap protocontainer<character>::CodeNameMap;

CHARACTER_PROTOTYPE(character, 0);

#include "femath.h"
#include "itemde.h"
#include "charde.h"

#undef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__

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
#include "stdover.h"
#include "team.h"
#include "lterraba.h"
#include "error.h"
#include "config.h"
#include "slot.h"
#include "actionde.h"
#include "colorbit.h"
#include "save.h"
#include "game.h"

petrus::~petrus()
{
  game::SetPetrus(0);
}

void petrus::CreateInitialEquipment()
{
  SetMainWielded(new valpurusjustifier);
  SetBodyArmor(new platemail(MAKE_MATERIAL(VALPURIUM)));
}

void priest::CreateInitialEquipment()
{
  SetMainWielded(new meleeweapon(SPIKEDMACE, MAKE_MATERIAL(MITHRIL)));
  SetBodyArmor(new bodyarmor(CHAINMAIL, MAKE_MATERIAL(MITHRIL)));
}

void oree::CreateInitialEquipment()
{
  SetBodyArmor(new goldeneagleshirt);
  can* Can = new can(0, false);
  Can->InitMaterials(MAKE_MATERIAL(IRON, 10), MAKE_MATERIAL(PEPSI, 330));
  GetStack()->FastAddItem(Can);
}

void darkknight::CreateInitialEquipment()
{
  if(RAND() % 20)
    SetMainWielded(RAND() % 5 ? (item*)(new meleeweapon(LONGSWORD)) : RAND() % 20 ? (item*)(new meleeweapon(TWOHANDEDSWORD)) : (item*)(new meleeweapon(CURVEDTWOHANDEDSWORD)));
  else
    {
      meleeweapon* DoomsDay = new meleeweapon(LONGSWORD, false);
      DoomsDay->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(IRON), MAKE_MATERIAL(FROGFLESH));
      SetMainWielded(DoomsDay);
    }

  SetBodyArmor(new bodyarmor(CHAINMAIL, RAND() % 5 ? MAKE_MATERIAL(IRON) : MAKE_MATERIAL(MITHRIL)));
}

void skeleton::CreateInitialEquipment()
{
  SetMainWielded(RAND() % 10 ? RAND() % 3 ? (item*)new meleeweapon(AXE) : (item*)new pickaxe : (item*)new meleeweapon(SPIKEDMACE));
}

void goblin::CreateInitialEquipment()
{
  SetMainWielded(RAND() % 3 ? (item*)new meleeweapon(SPEAR) : (item*)new meleeweapon(POLEAXE));
}

void guard::CreateInitialEquipment()
{
  SetMainWielded(new meleeweapon(LONGSWORD, MAKE_MATERIAL(IRON)));
  SetBodyArmor(new bodyarmor(CHAINMAIL));

  GetCategoryWeaponSkill(LARGE_SWORDS)->AddHit(500);
  //GetMainArm()->GetCurrentSingleWeaponSkill()->AddHit(500);
}

bool ennerbeast::Hit(character*)
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevelUnder()->GetBorder(), GetPos(), 30);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      {
	character* Char = GetNearSquare(x, y)->GetCharacter();
	ushort ScreamStrength = 1000000 / GetHypotSquare(GetPos().X - x, GetPos().Y - y);

	if(Char && Char != this)
	  {
	    if(Char->IsPlayer())
	      if(RAND() & 1)
		ADD_MESSAGE("%s yells: UGH UGHAaaa!", CHARDESCRIPTION(DEFINITE));
	      else
		ADD_MESSAGE("%s yells: Uga Ugar Ugade Ugat!", CHARDESCRIPTION(DEFINITE));

	    Char->ReceiveDamage(this, ScreamStrength, SOUND, ALL, 8, true);
	    Char->CheckDeath("killed by " + GetName(INDEFINITE) + "'s scream");
	  }

	GetNearLSquare(x, y)->GetStack()->ReceiveDamage(this, ScreamStrength, SOUND);

	for(ushort x = 0; x < 4; ++x)
	  GetNearLSquare(x, y)->GetSideStack(x)->ReceiveDamage(this, ScreamStrength, SOUND);
      }

  EditNP(-100);
  EditAP(long(-1000 * GetCategoryWeaponSkill(BITE)->GetAPBonus()));
  return true;
}

void skeleton::CreateCorpse()
{
  ushort Amount = 2 + RAND() % 4;

  for(ushort c = 0; c < Amount; ++c)
    GetStackUnder()->AddItem(new bone);

  SendToHell();
}

void petrus::CreateCorpse()
{
  GetStackUnder()->AddItem(new leftnutofpetrus);
  SendToHell();
}

void elpuri::CreateCorpse()
{
  character::CreateCorpse();
  GetStackUnder()->AddItem(new headofelpuri);
}

void humanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << SingleWeaponSkill;
}

void humanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> SingleWeaponSkill;

  if(GetRightWielded())
    for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
      if((*i)->GetID() == GetRightWielded()->GetID())
	{
	  SetCurrentRightSingleWeaponSkill(*i);
	  break;
	}

  if(GetLeftWielded())
    for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
      if((*i)->GetID() == GetLeftWielded()->GetID())
	{
	  SetCurrentLeftSingleWeaponSkill(*i);
	  break;
	}
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
  SeekLeader();

  if(RAND() % 3)
    Hit(0);

  if(CheckForEnemies(false))
    return;

  if(FollowLeader())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void petrus::GetAICommand()
{
  SetHealTimer(GetHealTimer() + 1);

  if(GetHealTimer() > 100)
    for(ushort d = 0; d < 8; ++d)
      {
	square* Square = GetNeighbourSquare(d);

	if(Square)
	  {
	    character* Char = Square->GetCharacter();

	    if(Char && GetTeam()->GetRelation(Char->GetTeam()) == FRIEND && Char->GetHP() < Char->GetMaxHP() / 3)
	      {
		HealFully(Char);
		return;
	      }
	  }
      }

  StandIdleAI();
}

void petrus::HealFully(character* ToBeHealed)
{
  SetHealTimer(0);

  ToBeHealed->RestoreHP();

  if(ToBeHealed->IsPlayer())
    ADD_MESSAGE("%s heals you fully.", CHARDESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s heals %s fully.", CHARDESCRIPTION(DEFINITE), ToBeHealed->CHARDESCRIPTION(DEFINITE));

  if(!ToBeHealed->HasAllBodyParts())
    {
      stackiterator OldOwnBodyPartIterator = game::GetPlayer()->FindRandomOwnBodyPart();

      if(OldOwnBodyPartIterator != 0)
	{
	  bodypart* OldOwnBodyPart = dynamic_cast<bodypart*>(***OldOwnBodyPartIterator);
	  if(!OldOwnBodyPart)
	    ABORT("character::FindRandomOwnBodyPart seems to be returning odd iterators...");
	  
	  game::GetPlayer()->AttachBodyPart(OldOwnBodyPart, OldOwnBodyPart->GetBodyPartIndex());
	  game::GetPlayer()->GetStack()->RemoveItem(OldOwnBodyPartIterator);
	  OldOwnBodyPart->SetHP(OldOwnBodyPart->GetMaxHP());
	  ADD_MESSAGE("%s attaches your old %s back and heals it.", CHARNAME(DEFINITE), OldOwnBodyPart->GetNameSingular().c_str());
	}
    }
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
      if(ConsumeItem(Thingy))
	if(IsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHARNAME(DEFINITE));
	else
	  {
	    if(CanBeSeenByPlayer())
	      ADD_MESSAGE("%s catches %s and eats it.", CHARNAME(DEFINITE), Thingy->CHARNAME(DEFINITE));

	    ChangeTeam(game::GetPlayer()->GetTeam());
	  }
      else
	if(IsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHARNAME(DEFINITE));
	else
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s catches %s.", CHARNAME(DEFINITE), Thingy->CHARNAME(DEFINITE));

      return true;
    }
  else
    return false;
}

item* humanoid::GetMainWielded() const
{
  if(GetMainArm())
    if(GetMainArm()->GetWielded())
      return GetMainArm()->GetWielded();
    else
      if(GetSecondaryArm())
	return GetSecondaryArm()->GetWielded();
      else
	return 0;
  else
    if(GetSecondaryArm())
      return GetSecondaryArm()->GetWielded();
    else
      return 0;
}

item* humanoid::GetSecondaryWielded() const
{
  if(GetMainArm() && GetMainArm()->GetWielded() && GetSecondaryArm())
    return GetSecondaryArm()->GetWielded();
  else
    return 0;
}

bool humanoid::Hit(character* Enemy)
{
  if(IsPlayer() && GetTeam()->GetRelation(Enemy->GetTeam()) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  Hostility(Enemy);

  if(GetBurdenState() == OVERLOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return true;
    }

  ushort c, AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c))
      ++AttackStyles;

  ushort Chosen = RAND() % AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c) && AttackStyles++ == Chosen)
      {
	Chosen = 1 << c;
	break;
      }

  switch(Chosen)
    {
    case USE_ARMS:
      if((GetRightArm() && GetRightArm()->GetAttackStrength()) || (GetLeftArm() && GetLeftArm()->GetAttackStrength()))
	{
	  long RightAPCost = 0, LeftAPCost = 0;

	  if(GetRightArm() && GetRightArm()->GetAttackStrength())
	    {
	      RightAPCost = GetRightArm()->GetAPCost();
	      GetRightArm()->Hit(Enemy);
	    }

	  if(IsEnabled() && Enemy->IsEnabled() && GetLeftArm() && GetLeftArm()->GetAttackStrength())
	    {
	      LeftAPCost = GetLeftArm()->GetAPCost();
	      GetLeftArm()->Hit(Enemy);
	    } 

	  EditNP(-50);
	  EditAP(Min(RightAPCost, LeftAPCost));
	  return true;
	}
    case USE_LEGS:
      if(GetRightLeg() && GetLeftLeg())
	{
	  Kick(Enemy->GetLSquareUnder());
	  return true;
	}
    case USE_HEAD:
      if(GetHead())
	{
          Bite(Enemy);
	  return true;
	}
    default:
      if(IsPlayer())
	ADD_MESSAGE("You are currently quite unable to damage anything.");

      return false;
    }
}

bool humanoid::AddSpecialSkillInfo(felist& List) const
{
  bool Something = false;

  if(CurrentRightSingleWeaponSkill && CurrentRightSingleWeaponSkill->GetHits())
    {
      List.AddEntry("", LIGHTGRAY);
      std::string Buffer = "right single weapon skill:  ";
      Buffer.resize(30, ' ');
      Buffer += CurrentRightSingleWeaponSkill->GetLevel();
      Buffer.resize(40, ' ');
      Buffer += CurrentRightSingleWeaponSkill->GetHits();
      Buffer.resize(50, ' ');

      if(CurrentRightSingleWeaponSkill->GetLevel() != 10)
	Buffer += (CurrentRightSingleWeaponSkill->GetLevelMap(CurrentRightSingleWeaponSkill->GetLevel() + 1) - CurrentRightSingleWeaponSkill->GetHits());
      else
	Buffer += '-';

      Buffer.resize(60, ' ');
      Buffer += int(CurrentRightSingleWeaponSkill->GetEffectBonus() * 100 - 100);
      Buffer += '%';
      Buffer.resize(70, ' ');
      Buffer += int(CurrentRightSingleWeaponSkill->GetAPBonus() * 100 - 100);
      Buffer += '%';
      List.AddEntry(Buffer, LIGHTGRAY);
      Something = true;
    }

  if(CurrentLeftSingleWeaponSkill && CurrentLeftSingleWeaponSkill->GetHits())
    {
      List.AddEntry("", LIGHTGRAY);
      std::string Buffer = "left single weapon skill:  ";
      Buffer.resize(30, ' ');
      Buffer += CurrentLeftSingleWeaponSkill->GetLevel();
      Buffer.resize(40, ' ');
      Buffer += CurrentLeftSingleWeaponSkill->GetHits();
      Buffer.resize(50, ' ');

      if(CurrentLeftSingleWeaponSkill->GetLevel() != 10)
	Buffer += (CurrentLeftSingleWeaponSkill->GetLevelMap(CurrentLeftSingleWeaponSkill->GetLevel() + 1) - CurrentLeftSingleWeaponSkill->GetHits());
      else
	Buffer += '-';

      Buffer.resize(60, ' ');
      Buffer += int(CurrentLeftSingleWeaponSkill->GetEffectBonus() * 100 - 100);
      Buffer += '%';
      Buffer.resize(70, ' ');
      Buffer += int(CurrentLeftSingleWeaponSkill->GetAPBonus() * 100 - 100);
      Buffer += '%';
      List.AddEntry(Buffer, LIGHTGRAY);
      Something = true;
    }

  return Something;
}

void shopkeeper::CreateInitialEquipment()
{
  SetMainWielded(new pickaxe(MAKE_MATERIAL(MITHRIL)));
  SetBodyArmor(new bodyarmor(CHAINMAIL, MAKE_MATERIAL(MITHRIL)));

  GetCategoryWeaponSkill(AXES)->AddHit(5000);
  GetMainArm()->GetCurrentSingleWeaponSkill()->AddHit(5000);
}

void farmer::CreateInitialEquipment()
{
  SetMainWielded(new meleeweapon(AXE));
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
      ADD_MESSAGE("Petrus smiles. \"Thou hast defeated Oree! Valpurus shall bless thee for the rest of thine life! And thou possess the Shirt of the Golden Eagle, the symbol of my status! Return it now, please.\"");

      if(game::BoolQuestion("Will you give the Shirt of the Golden Eagle to Petrus? [y/n]", REQUIRES_ANSWER))
	{
	  game::TextScreen("Thou hast slain the Blood Daemon King, and Petrus is happy!\n\nYou are victorious!");
	  AddScoreEntry("retrieved the Shirt of the Golden Eagle and became the Avatar of Law", 4, false);
	  game::End();
	  return;
	}
      else
	{
	  ADD_MESSAGE("Petrus's face turns red. \"I see. Thine greed hast overcome thine wisdom. Then, we shall fight for the shiny shirt. May Valpurus bless him who is better.\"");

	  /* And now we actually make his face change color ;-) */

	  if(GetHead()->GetMainMaterial()->IsFlesh())
	    {
	      std::vector<ushort>& SkinColor = ((flesh*)GetHead()->GetMainMaterial())->GetSkinColorVector();
	      SkinColor.clear();
	      SkinColor.push_back(MAKE_RGB(255, 75, 50));
	    }

	  GetHead()->UpdatePictures();
	  GetSquareUnder()->SendNewDrawRequest();
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
	  ADD_MESSAGE("Petrus smiles. \"Thou areth indeed a great Champion of the Great Frog! Elpuri is not a foe worthy for thee.");

	  if(game::BoolQuestion("Dost thou wish to stay on duty for a while more and complete another quest for me?\" [y/n]", REQUIRES_ANSWER))
	    {
	      game::TextScreen(	"Champion of Law!\n\n"
				"Return to the foul cave of Elpuri and seek out the Master Evil:\n"
				"Oree the Blood Daemon King, who hast stolenth one of the most powerful of all of my artifacts:\n"
				"the Shirt of the Golden Eagle! Return with it and immortal glory shall be thine!");

	      GetAreaUnder()->SendNewDrawRequest();
	      game::TriggerQuestForGoldenEagleShirt();
	      StoryState = 2;
	      return;
	    }
	}

      game::TextScreen("Thou hast slain Elpuri, and Petrus is happy!\n\nYou are victorious!");
      AddScoreEntry("defeated Elpuri and continued to further adventures", 2, false);
      game::End();
    }
  else
    if(!StoryState)
      {
	game::TextScreen(	"Petrus raises his hand as a salutation, and talks:\n"
				"\"Fare thee well, adventurer! Should thou seek glory, I have a task for thee!\n"
				"An evil dark frog named Elpuri is pestering our fine city in many ways.\n"
				"Valpurus hast told that this vile beast can be found in a nearby cave.\n"
				"Slay it and bring me its head as proof. Return when thou hast succeeded.\"");

	GetAreaUnder()->SendNewDrawRequest();
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
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s says gravely: \"You don't have a life. Get it in the army.\"", CHARDESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s says gravely: \"You don't have a life. Get it as shop guard.\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 1:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of breaking rules.\"", CHARDESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of stealing anything.\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 2:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s shouts excited: \"Attnam victoor!\"", CHARDESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("\"Yes, this is a dangerous place to work, but our boss pays us well.\"");
      break;
    case 3:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"The High Priest is my idol. I would want a sword as big as his!\"");
      else
        {
	  ADD_MESSAGE("\"Attnam's guards can barely wield a sword. But we are trained by the laws of the dungeon, so don't do anything suspicious here.\"");
        }
      break;
    case 4:
      if(GetLevelUnder()->GetOnGround())
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
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s sighs: \"If only I hadn't chosen a city in the middle of nowhere...\"", CHARDESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s sighs: \"I wonder why I have so few customers these days...\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 1:
      ADD_MESSAGE("%s sighs: \"Mutant mushrooms ate the last caravan, and the one before it ran into an enner beast. It must be all Elpuri's doings!\"", CHARDESCRIPTION(DEFINITE));
      break;
    case 2:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"You truly can't find better prices in this city! Indeed, you can't find ANY prices, since my store is a monopoly.\"");
      else
	ADD_MESSAGE("\"The topmost reason why I work here is that monsters devour tax collectors.\"");
      break;
    case 3:
      if(GetLevelUnder()->GetOnGround())
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

  for(ushort c = 0; c < Talker->GetBodyParts(); ++c)
    if(!Talker->GetBodyPart(c))
      {
	ushort OriginalID = Talker->GetOriginalBodyPartID(c);
	for(stackiterator ii = Talker->GetStack()->GetBottomSlot(); ii != Talker->GetStack()->GetSlotAboveTop(); ++ii)
	  if(OriginalID == (**ii)->GetID())
	    {
	      /* Now attach the old BodyPart (**ii) to slot c */
	      if(Talker->GetMoney() >= PRICE_TO_ATTACH_OLD_LIMB_IN_ALTAR)
		{
		  if(game::BoolQuestion("Would you like me to put your old " + (**ii)->GetNameSingular() + " back in exchange for " + PRICE_TO_ATTACH_OLD_LIMB_IN_ALTAR + " gold? [y,N]"))
		    {
		      ((bodypart*)***ii)->SetHP(1);
		      Talker->SetMoney(Talker->GetMoney() - PRICE_TO_ATTACH_OLD_LIMB_IN_ALTAR);
		      SetMoney(GetMoney() + PRICE_TO_ATTACH_OLD_LIMB_IN_ALTAR);
		      bodypart* OldBodyPart = dynamic_cast<bodypart*>(***ii);
		      if(!OldBodyPart)
			ABORT("Priest is asked to but odd object in place of BodyPart");
		      if(OldBodyPart->GetMainMaterial()->IsSameAs(Talker->GetTorso()->GetMainMaterial()))
			{
			  Talker->AttachBodyPart(OldBodyPart,c);
			  Talker->GetStack()->RemoveItem(ii);
			}
		      else
			{
			  ADD_MESSAGE("Sorry I cannot help you, because your old %s doesn't seem to fit anymore.", (***ii)->GetNameSingular().c_str());   
			}
		    }
		  else
		    ADD_MESSAGE("\"Help yourself and get some money and we'll help you too.\"");
		}
	      return;
	    }
	bodypart* NewBodyPart = Talker->MakeBodyPart(c);
	NewBodyPart->SetHP(1);
	ADD_MESSAGE("You seem to be missing %s %s.", NewBodyPart->GetArticle().c_str(), NewBodyPart->GetNameSingular().c_str());
	if(Talker->GetMoney() >= PRICE_TO_ATTACH_OLD_LIMB_IN_ALTAR)
	  {
	    ADD_MESSAGE("Since you don't seem to have your orginal %s with you, I could summon up a new one.", NewBodyPart->GetNameSingular().c_str());
	    if(game::BoolQuestion(std::string("And this would cost you only ") + PRICE_TO_ATTACH_NEW_LIMB_IN_ALTAR +std::string(" gold. Agreed? [y,N]")))
	      {
		Talker->SetMoney(Talker->GetMoney() - PRICE_TO_ATTACH_NEW_LIMB_IN_ALTAR);
		SetMoney(GetMoney() + PRICE_TO_ATTACH_NEW_LIMB_IN_ALTAR);
		Talker->AttachBodyPart(NewBodyPart, c);
	      }
	    else
	      {
		ADD_MESSAGE("Since you don't have your original %s with you, I could summon up %s %s, but I am not a communist and you haven't got %d gold.", NewBodyPart->GetNameSingular().c_str(), NewBodyPart->GetArticle().c_str(), NewBodyPart->GetNameSingular().c_str(), PRICE_TO_ATTACH_NEW_LIMB_IN_ALTAR);
		delete NewBodyPart;
		return;
	      }
	  }


      }

  if(!HomeRoom)
    ADD_MESSAGE("\"Receive my blessings, child.\"");
  else
    ADD_MESSAGE("%s talks to you: %s", CHARDESCRIPTION(DEFINITE), game::GetGod(GetLevelUnder()->GetRoom(HomeRoom)->GetDivineMaster())->GetPriestMessage().c_str());
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

void humanoid::AddSpecialItemInfo(std::string& Description, item* Item)
{
  Description.resize(62, ' ');
  Description += GetCategoryWeaponSkill(Item->GetWeaponCategory())->GetLevel();
  Description.resize(66, ' ');
  bool Added = false;

  for(ushort c = 0; c < GetSingleWeaponSkills(); ++c)
    if(Item->GetID() == GetSingleWeaponSkill(c)->GetID())
      {
	Description += GetSingleWeaponSkill(c)->GetLevel();
	Added = true;
	break;
      }

  if(!Added)
    Description += 0;
}

void humanoid::AddSpecialItemInfoDescription(std::string& Description)
{
  Description.resize(68, ' ');
  Description += "GS";
  Description.resize(72, ' ');
  Description += "SS";
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
  SetMainWielded(new meleeweapon(SPIKEDMACE, MAKE_MATERIAL(IRON)));
  SetBodyArmor(new platemail(0, MAKE_MATERIAL(IRON)));
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
      ADD_MESSAGE("\"I saw a communist visiting the city a few days past. I'm now organising a party to seek and hunt him down.\"");
      break;
    }
}

void hunter::CreateInitialEquipment()
{
  SetMainWielded(new meleeweapon(SPEAR));
}

void slave::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Yikes!\"");
      return;
    }

  character* Master;

  if(HomeRoom && (Master = GetLevelUnder()->GetRoom(HomeRoom)->GetMaster()))
    {
      if(Talker->GetMoney() >= 50)
	{
	  ADD_MESSAGE("%s talks: \"Do you want to buy me? 50 gold pieces. I work very hard.\"", CHARDESCRIPTION(DEFINITE));

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
	  ADD_MESSAGE("\"Don't touch me! Master doesn't want people to touch sale items. I'm worth 50 gold pieces, you know!\"");
	}

      return;
    }

  if(GetTeam() == Talker->GetTeam())
    {
      /* What if the whip is in the off hand? */

      if(Talker->GetMainWielded() && Talker->GetMainWielded()->IsWhip())
	{
	  ADD_MESSAGE("\"Don't hit me! I work! I obey! I not think!\"");
	}
      else
	{
	  static bool Said[5];

	  switch(RandomizeReply(5, Said))
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

  if(!HomeRoom || !GetLevelUnder()->GetRoom(HomeRoom)->GetMaster())
    {
      HomeRoom = 0;

      if(MoveRandomly())
	return;
    }

  EditAP(-1000);
}

bool elpuri::Hit(character* Enemy)
{
  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square)
	{
	  character* ByStander = Square->GetCharacter();

	  if(ByStander && (ByStander == Enemy || ByStander->GetTeam()->GetRelation(GetTeam()) == HOSTILE))
	    nonhumanoid::Hit(ByStander);

	  Square->GetStack()->ReceiveDamage(this, GetAttribute(ARMSTRENGTH), PHYSICALDAMAGE);

	  for(ushort c = 0; c < 4; ++c)
	    if(Square->GetSideStack(c)->GetSquareTrulyUnder() == GetSquareUnder())
	      Square->GetSideStack(c)->ReceiveDamage(this, GetAttribute(ARMSTRENGTH), PHYSICALDAMAGE);
	}
    }

  SetAP(-1000);
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
      ADD_MESSAGE("\"Being one of six wives is a dream job. Pay is good and you only have to work about one night a week!\"");
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
      ADD_MESSAGE("\"Can you help me find my husband? He is hiding somewhere. He's that farmer who's just been mugged with a frying pan.\"");
      break;
    case 1:
      ADD_MESSAGE("\"Yesterday a bear rushed through my kitchen wall and ruined all my pies. Animals are truly annoying. Why can't we just burn the whole forest down?\"");
      break;
    case 2:
      ADD_MESSAGE("\"Wolves ate my seventh daughter last week. Damn. It'll take eight years to produce an equally good replacement worker.\"");
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
	  ADD_MESSAGE("\"Thou are going to fight Elpuri? Beware! It is a powerful enemy. Other monsters are very vulnerable if surrounded by thine party, but not that beast, for it may slay a horde of thine friends at once with its horrendous tail attack.\"");
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
	  ADD_MESSAGE("\"Thou wish to confront the Blood Daemon King? Heed my advice: He is a cunning enemy and will try to ambush thee. A powerful party and a means of quick escape at hand would help thee greatly.\"");
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
	  ADD_MESSAGE("\"The Shirt of the Golden Eagle is a legendary artifact. Thou canst not find a better armor.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"In this book they talk about Erado, the great chaos god. He hates us mortals more than anything and will respond only to Champions of Evil.\"");
	  break;
	}
    case 6:
      ADD_MESSAGE("\"Attnam is traditionally ruled by the High Priest of the Great Frog. He holds the Shirt of the Golden Eagle and has always killed his predecessor.\"");
      break;
    case 7:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 3)
	{
	  ADD_MESSAGE("\"Remember, the Shirt of the Golden Eagle is the armor the High Priests. Things will get *very* rough if one denies it from Petrus.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"They say thou should keep all the artifacts thou find. They shalt make thee famous after thine retirement.\"");
	  break;
	}
    case 8:
      ADD_MESSAGE("\"If thou shalt ever encounter an enner beast, know this: It is a horrible foe. It may shatter thine items and armor with its scream that penetrates iron and stone. Thou should not engage it in melee. Kill it from afar.\"");
      break;
    case 9:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Thou areth not alone in thine attempth to defeat Elpuri. A brave adventurer called Ivan also dived into its cave not long ago.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that chaotic gods offer great power to their followers. But thou must remember: unlike lawfuls, they shalt not help thee when things go bad.\"");
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
    case 0:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engraves something to the ground.", CHARNAME(UNARTICLED));

      Engrave("The bourgeois is a bourgeois -- for the benefit of the working class.");
      return true;
    case 1:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engraves something to the ground.", CHARNAME(UNARTICLED));

      Engrave("Proletarians of all countries, unite!");
      return true;
    case 2:
      if(CanBeSeenByPlayer())
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

  if(!game::IsInWilderness()) 
    {
      GetNearLSquare(GetPos)->SpillFluid(HowMuch, GetBloodColor(), 5, 60);

      if(!(RAND() % 10)) 
	GetNearLSquare(GetPos)->GetStack()->AddItem(new lump(MAKE_MATERIAL(HUMANFLESH, 1000)));
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

void kobold::CreateInitialEquipment()
{
  SetMainWielded(new meleeweapon(SPEAR));
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
  SaveFile << HealTimer;
}

void angel::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> HealTimer;
}

void angel::CreateInitialEquipment()
{
  GetStack()->FastAddItem(new holybook(GetConfig()));

  switch(GetMasterGod()->BasicAlignment())
    {
    case GOOD:
      SetMainWielded(new meleeweapon(LONGSWORD, MAKE_MATERIAL(DIAMOND)));
      SetBodyArmor(new bodyarmor(CHAINMAIL, MAKE_MATERIAL(DIAMOND)));
      break;
    case NEUTRAL:
      SetMainWielded(new meleeweapon(POLEAXE, MAKE_MATERIAL(SAPPHIRE)));
      SetBodyArmor(new bodyarmor(CHAINMAIL, MAKE_MATERIAL(SAPPHIRE)));
      break;
    case EVIL:
      {
	meleeweapon* SpikedMace = new meleeweapon(SPIKEDMACE, false);
	SpikedMace->InitMaterials(MAKE_MATERIAL(RUBY), MAKE_MATERIAL(IRON), MAKE_MATERIAL(FROGFLESH));
	SetMainWielded(SpikedMace);
	SetBodyArmor(new brokenplatemail(MAKE_MATERIAL(RUBY)));
	break;
      }
    }
}

void angel::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    ADD_MESSAGE("\"With the power of %s, I shall slay thee, sinner!\"", GetMasterGod()->Name().c_str());
  else
    ADD_MESSAGE("\"%s be with you, mortal.\"", GetMasterGod()->Name().c_str());
}

void kamikazedwarf::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Heaven awaits me in the house of %s after I bomb you, heretic!\"", GetMasterGod()->Name().c_str());
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
      ADD_MESSAGE("%s praises %s with numerous hymns. %s is obviously a very devoted follower.", CHARDESCRIPTION(DEFINITE), GetMasterGod()->Name().c_str(), CapitalizeCopy(GetPersonalPronoun()).c_str());
      break;
    case 3:
      ADD_MESSAGE("\"One day, Holy War will break out and I shall sacrifice my life with joy.\"");
      break;
    }
}

void kamikazedwarf::CreateInitialEquipment()
{
  SetMainWielded(new holybook(GetConfig()));
  GetStack()->FastAddItem(new backpack);
}

bool kamikazedwarf::Hit(character* Enemy)
{
  if(IsPlayer())
    return humanoid::Hit(Enemy);
  else
    {
      for(stackiterator i = GetStack()->GetBottomSlot(); i != GetStack()->GetSlotAboveTop(); ++i)
	if((**i)->IsExplosive())
	  {
	    if(RAND() & 1)
	      ADD_MESSAGE("%s shouts: \"For %s!\"", CHARDESCRIPTION(DEFINITE), GetMasterGod()->Name().c_str());
	    else
	      ADD_MESSAGE("%s screams: \"%s, here I come!\"", CHARDESCRIPTION(DEFINITE), GetMasterGod()->Name().c_str());

	    if((**i)->Apply(this))
	      return true;
	  }

      return humanoid::Hit(Enemy);
    }
}

bool largecat::Catches(item* Thingy, float)
{
  if(Thingy->CatWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy))
	if(IsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHARNAME(DEFINITE));
	else
	  {
	    if(CanBeSeenByPlayer())
	      ADD_MESSAGE("%s catches %s and eats it.", CHARNAME(DEFINITE), Thingy->CHARNAME(DEFINITE));

	    ChangeTeam(game::GetPlayer()->GetTeam());
	  }
      else
	if(IsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHARNAME(DEFINITE));
	else
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s catches %s.", CHARNAME(DEFINITE), Thingy->CHARNAME(DEFINITE));

      return true;
    }
  else
    return false;
}

material* unicorn::CreateBodyPartFlesh(ushort, ulong Volume) const
{
  switch(Alignment)
    {
    case GOOD:
      return MAKE_MATERIAL(WHITEUNICORNFLESH, Volume);
    case NEUTRAL:
      return MAKE_MATERIAL(GRAYUNICORNFLESH, Volume);
    default:
      return MAKE_MATERIAL(BLACKUNICORNFLESH, Volume);
    }
}

void unicorn::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << Alignment;
}

void unicorn::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> Alignment;
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
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears!", CHARNAME(DEFINITE));

    Move(GetLevelUnder()->RandomSquare(this, true), true);

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s appears from nothing!", CHARNAME(INDEFINITE));

    return true;
  }

  if(!(RAND() % 3) && MoveRandomly())
    return true;

  return false;
}

void unicorn::CreateInitialEquipment()
{
  if(RAND() & 1)
    GetStack()->FastAddItem(new stone);

  if(RAND() & 1)
    GetStack()->FastAddItem(new stone);
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

ulong humanoid::GetBodyPartSize(ushort Index, ushort TotalSize)
{
  switch(Index)
    {
    case HEADINDEX: return 20;
    case TORSOINDEX: return (TotalSize - 20) * 2 / 5;
    case RIGHTARMINDEX:
    case LEFTARMINDEX: return (TotalSize - 20) * 3 / 5;
    case GROININDEX: return (TotalSize - 20) / 3;
    case RIGHTLEGINDEX:
    case LEFTLEGINDEX: return (TotalSize - 20) * 3 / 5;
    default:
      ABORT("Illegal humanoid bodypart size request!");
      return 0;
    }
}

ulong humanoid::GetBodyPartVolume(ushort Index)
{
  switch(Index)
    {
    case HEADINDEX: return 4000;
    case TORSOINDEX: return (GetTotalVolume() - 4000) * 13 / 30;
    case RIGHTARMINDEX:
    case LEFTARMINDEX: return (GetTotalVolume() - 4000) / 10;
    case GROININDEX: return (GetTotalVolume() - 4000) / 10;
    case RIGHTLEGINDEX:
    case LEFTLEGINDEX: return (GetTotalVolume() - 4000) * 2 / 15;
    default:
      ABORT("Illegal humanoid bodypart volume request!");
      return 0;
    }
}

bodypart* humanoid::MakeBodyPart(ushort Index)
{
  switch(Index)
    {
    case TORSOINDEX: return new humanoidtorso(0, false);
    case HEADINDEX: return new head(0, false);
    case RIGHTARMINDEX: return new rightarm(0, false);
    case LEFTARMINDEX: return new leftarm(0, false);
    case GROININDEX: return new groin(0, false);
    case RIGHTLEGINDEX: return new rightleg(0, false);
    case LEFTLEGINDEX: return new leftleg(0, false);
    default:
      ABORT("Weird bodypart to make for a humanoid. It must be your fault!");
      return 0;
    }
}

uchar humanoid::GetBodyPartBonePercentile(ushort Index)
{
  switch(Index)
    {
    case TORSOINDEX: return GetTorsoBonePercentile();
    case HEADINDEX: return GetHeadBonePercentile();
    case RIGHTARMINDEX: return GetRightArmBonePercentile();
    case LEFTARMINDEX: return GetLeftArmBonePercentile();
    case GROININDEX: return GetGroinBonePercentile();
    case RIGHTLEGINDEX: return GetRightLegBonePercentile();
    case LEFTLEGINDEX: return GetLeftLegBonePercentile();
    default:
      ABORT("Weird bodypart bone percentile request for a humanoid. It must be your fault!");
      return 0;
    }
}

bool humanoid::ReceiveDamage(character* Damager, short Amount, uchar Type, uchar TargetFlags, uchar Direction, bool Divide, bool PenetrateArmor, bool Critical)
{
  std::vector<uchar> ChooseFrom;

  if(TargetFlags & RIGHTARM && GetRightArm())
    ChooseFrom.push_back(2);

  if(TargetFlags & LEFTARM && GetLeftArm())
    ChooseFrom.push_back(3);

  if(TargetFlags & RIGHTLEG && GetRightLeg())
    ChooseFrom.push_back(5);

  if(TargetFlags & LEFTLEG && GetLeftLeg())
    ChooseFrom.push_back(6);

  if(TargetFlags & HEAD && GetHead())
    ChooseFrom.push_back(1);

  if(TargetFlags & TORSO && GetTorso())
    ChooseFrom.push_back(0);

  if(TargetFlags & GROIN && GetGroin())
    ChooseFrom.push_back(4);

  if(!ChooseFrom.size())
    return false;

  ulong TotalVolume = 0;
  ushort c;

  for(c = 0; c < ChooseFrom.size(); ++c)
    TotalVolume += GetBodyPart(ChooseFrom[c])->GetBodyPartVolume();

  bool Affected = false;

  if(Divide)
    {
      for(c = 0; c < ChooseFrom.size(); ++c)
	if(ReceiveBodyPartDamage(Damager, long(Amount) * GetBodyPart(ChooseFrom[c])->GetBodyPartVolume() / TotalVolume, Type, ChooseFrom[c], Direction, PenetrateArmor, Critical))
	  Affected = true;
    }
  else
    Affected = ReceiveBodyPartDamage(Damager, Amount, Type, ChooseFrom[RAND() % ChooseFrom.size()], Direction, PenetrateArmor, Critical);

  if(!Affected)
    {
      if(IsPlayer())
	ADD_MESSAGE("You are not hurt.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s is not hurt.", GetPersonalPronoun().c_str());
    }

  if(DamageTypeAffectsInventory(Type))
    {
      for(ushort c = 0; c < EquipmentSlots(); ++c)
	if(GetEquipment(c))
	  GetEquipment(c)->ReceiveDamage(Damager, Amount, Type);

      GetStack()->ReceiveDamage(Damager, Amount, Type);
    }

  return Affected;
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
  if(GetRightArm())
    return GetLeftArm();
  else
    return 0;
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

std::string humanoid::EquipmentName(ushort Index) const
{
  switch(Index)
    {
    case HELMETINDEX: return "helmet";
    case AMULETINDEX: return "amulet";
    case CLOAKINDEX: return "cloak";
    case BODYARMORINDEX: return "body armor";
    case BELTINDEX: return "belt";
    case RIGHTWIELDEDINDEX: return "right hand wielded";
    case LEFTWIELDEDINDEX: return "left hand wielded";
    case RIGHTRINGINDEX: return "right ring";
    case LEFTRINGINDEX: return "left ring";
    case RIGHTGAUNTLETINDEX: return "right gauntlet";
    case LEFTGAUNTLETINDEX: return "left gauntlet";
    case RIGHTBOOTINDEX: return "right boot";
    case LEFTBOOTINDEX: return "left boot";
    default: return "forbidden piece of cloth";
    }
}

bool (*humanoid::EquipmentSorter(ushort Index) const)(item*, character*)
{
  switch(Index)
    {
    case HELMETINDEX: return &item::HelmetSorter;
    case AMULETINDEX: return &item::AmuletSorter;
    case CLOAKINDEX: return &item::CloakSorter;
    case BODYARMORINDEX: return &item::BodyArmorSorter;
    case BELTINDEX: return &item::BeltSorter;
    case RIGHTWIELDEDINDEX:
    case LEFTWIELDEDINDEX: return 0;
    case RIGHTRINGINDEX:
    case LEFTRINGINDEX: return &item::RingSorter;
    case RIGHTGAUNTLETINDEX:
    case LEFTGAUNTLETINDEX: return &item::GauntletSorter;
    case RIGHTBOOTINDEX:
    case LEFTBOOTINDEX: return &item::BootSorter;
    default: return 0;
    }
}

bodypart* humanoid::GetBodyPartOfEquipment(ushort Index) const
{
  switch(Index)
    {
    case HELMETINDEX:
    case AMULETINDEX:
      return GetHead();
    case CLOAKINDEX:
    case BODYARMORINDEX:
    case BELTINDEX:
      return GetTorso();
    case RIGHTWIELDEDINDEX:
    case RIGHTRINGINDEX:
    case RIGHTGAUNTLETINDEX:
      return GetRightArm();
    case LEFTWIELDEDINDEX:
    case LEFTRINGINDEX:
    case LEFTGAUNTLETINDEX:
      return GetLeftArm();
    case RIGHTBOOTINDEX:
      return GetRightLeg();
    case LEFTBOOTINDEX:
      return GetLeftLeg();
    default:
      return 0;
    }
}

item* humanoid::GetEquipment(ushort Index) const
{
  switch(Index)
    {
    case HELMETINDEX: return GetHelmet();
    case AMULETINDEX: return GetAmulet();
    case CLOAKINDEX: return GetCloak();
    case BODYARMORINDEX: return GetBodyArmor();
    case BELTINDEX: return GetBelt();
    case RIGHTWIELDEDINDEX: return GetRightWielded();
    case LEFTWIELDEDINDEX: return GetLeftWielded();
    case RIGHTRINGINDEX: return GetRightRing();
    case LEFTRINGINDEX: return GetLeftRing();
    case RIGHTGAUNTLETINDEX: return GetRightGauntlet();
    case LEFTGAUNTLETINDEX: return GetLeftGauntlet();
    case RIGHTBOOTINDEX: return GetRightBoot();
    case LEFTBOOTINDEX: return GetLeftBoot();
    default: return 0;
    }
}

void humanoid::SetEquipment(ushort Index, item* What)
{
  switch(Index)
    {
    case HELMETINDEX: SetHelmet(What); break;
    case AMULETINDEX: SetAmulet(What); break;
    case CLOAKINDEX: SetCloak(What); break;
    case BODYARMORINDEX: SetBodyArmor(What); break;
    case BELTINDEX: SetBelt(What); break;
    case RIGHTWIELDEDINDEX: SetRightWielded(What); break;
    case LEFTWIELDEDINDEX: SetLeftWielded(What); break;
    case RIGHTRINGINDEX: SetRightRing(What); break;
    case LEFTRINGINDEX: SetLeftRing(What); break;
    case RIGHTGAUNTLETINDEX: SetRightGauntlet(What); break;
    case LEFTGAUNTLETINDEX: SetLeftGauntlet(What); break;
    case RIGHTBOOTINDEX: SetRightBoot(What); break;
    case LEFTBOOTINDEX: SetLeftBoot(What); break;
    }
}

void humanoid::SwitchToDig(item* DigItem, vector2d Square)
{
  dig* Dig = new dig(this);

  if(GetRightArm())
    Dig->SetRightBackup(GetRightArm()->GetWielded()); // slot cleared automatically

  if(GetLeftArm())
    Dig->SetRightBackup(GetLeftArm()->GetWielded()); // slot cleared automatically

  DigItem->RemoveFromSlot();
  SetMainWielded(DigItem); // bug, this may change arm?
  Dig->SetSquareDug(Square);
  SetAction(Dig);
}

bool humanoid::CheckKick() const
{
  if(!CanKick())
    {
      ADD_MESSAGE("This race can't kick.");
      return false;
    }

  if(GetLegs() < 2)
    {
      std::string LegNumber;

      switch(GetLegs())
	{
	case 0: 
	  LegNumber = "no legs";
	  break;
	case 1:
	  LegNumber = "one leg";
	  break;
	}

      ADD_MESSAGE("How are you you going to do that with %s?", LegNumber.c_str());
      return false;
    }
  else
    return true;
}

uchar humanoid::GetLegs() const
{
  ushort Legs = 0;

  if(GetRightLeg())
    ++Legs;

  if(GetLeftLeg())
    ++Legs;

  return Legs;
}

uchar humanoid::GetArms() const
{
  ushort Arms = 0;

  if(GetRightArm())
    ++Arms;

  if(GetLeftArm())
    ++Arms;

  return Arms;
}

bool humanoid::CheckThrow() const
{
  switch(GetArms())
    {
    case 0:
      ADD_MESSAGE("You don't have an arm to do that!");
      return false;
    default:
      return true;
    }
}

bool humanoid::CheckOffer() const
{
  switch(GetArms())
    {
    case 0:
      ADD_MESSAGE("You need an arm to offer.");
      return false; 
    default:
      return true;
    }
}

void carnivorousplant::GetAICommand()
{
  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char && GetTeam()->GetRelation(Char->GetTeam()) == HOSTILE)
	    {
	      Hit(Char);
	      return;
	    }
	}
    }

  EditAP(-1000);
}

vector2d humanoid::GetEquipmentPanelPos(ushort Index) const
{
  switch(Index)
    {
    case HELMETINDEX: return vector2d(34, -23);
    case AMULETINDEX: return vector2d(14, -23);
    case CLOAKINDEX: return vector2d(-6, -23);
    case BODYARMORINDEX: return vector2d(54, -23);
    case BELTINDEX: return vector2d(24, 70);
    case RIGHTWIELDEDINDEX: return vector2d(-14, 4);
    case LEFTWIELDEDINDEX: return vector2d(62, 4);
    case RIGHTRINGINDEX: return vector2d(-14, 44);
    case LEFTRINGINDEX: return vector2d(62, 44);
    case RIGHTGAUNTLETINDEX: return vector2d(-14, 24);
    case LEFTGAUNTLETINDEX: return vector2d(62, 24);
    case RIGHTBOOTINDEX: return vector2d(4, 70);
    case LEFTBOOTINDEX: return vector2d(44, 70);
    default: return vector2d(24, 12);
    }
}

void humanoid::DrawSilhouette(bitmap* ToBitmap, vector2d Where, bool AnimationDraw) const
{
  ushort c;

  for(c = 0; c < EquipmentSlots(); ++c)
    if(CanUseEquipment(c))
      {
	vector2d Pos = Where + GetEquipmentPanelPos(c);

	if(!AnimationDraw)
	  DOUBLEBUFFER->DrawRectangle(Pos + vector2d(-1, -1), Pos + vector2d(17, 17), DARKGRAY);

	if(GetEquipment(c))
	  {
	    DOUBLEBUFFER->Fill(Pos, 16, 16, BLACK);
	    GetEquipment(c)->Draw(DOUBLEBUFFER, Pos, configuration::GetContrastLuminance(), false, true);
	  }
      }

  if(AnimationDraw)
    return;

  ushort Color[4] = { 0, 0, 0, 0 };

  if(GetHead())
    Color[0] = GetHead()->GetHP() * 3 < GetHead()->GetMaxHP() ? MAKE_RGB(128,0,0) : LIGHTGRAY;

  if(GetRightArm())
    Color[1] = GetRightArm()->GetHP() * 3 < GetRightArm()->GetMaxHP() ? MAKE_RGB(128,0,0) : LIGHTGRAY;

  if(GetLeftArm())
    Color[2] = GetLeftArm()->GetHP() * 3 < GetLeftArm()->GetMaxHP() ? MAKE_RGB(128,0,0) : LIGHTGRAY;

  if(GetTorso())
    Color[3] = GetTorso()->GetHP() * 3 < GetTorso()->GetMaxHP() ? MAKE_RGB(128,0,0) : LIGHTGRAY;

  igraph::GetCharacterRawGraphic()->MaskedBlit(ToBitmap, 0, 64, Where.X, Where.Y, SILHOUETTE_X_SIZE, SILHOUETTE_Y_SIZE, Color);

  for(c = 0; c < 4; ++c)
    Color[c] = 0;

  if(GetGroin())
    Color[1] = GetGroin()->GetHP() * 3 < GetGroin()->GetMaxHP() ? MAKE_RGB(128,0,0) : LIGHTGRAY;

  if(GetRightLeg())
    Color[2] = GetRightLeg()->GetHP() * 3 < GetRightLeg()->GetMaxHP() ? MAKE_RGB(128,0,0) : LIGHTGRAY;

  if(GetLeftLeg())
    Color[3] = GetLeftLeg()->GetHP() * 3 < GetLeftLeg()->GetMaxHP() ? MAKE_RGB(128,0,0) : LIGHTGRAY;

  igraph::GetCharacterRawGraphic()->MaskedBlit(ToBitmap, 64, 64, Where, SILHOUETTE_X_SIZE, SILHOUETTE_Y_SIZE, Color);
}

ushort humanoid::GlobalResistance(uchar Type) const
{
  ushort Resistance = GetResistance(Type);

  if(GetCloak())
    Resistance += GetCloak()->GetResistance(Type);

  if(Type != PHYSICALDAMAGE)
    {
      if(GetAmulet())
	Resistance += GetAmulet()->GetResistance(Type);

      if(GetRightRing())
	Resistance += GetRightRing()->GetResistance(Type);

      if(GetLeftRing())
	Resistance += GetLeftRing()->GetResistance(Type);
    }

  return Resistance;
}

void humanoid::AddInfo(felist& Info) const
{
  if(!game::WizardModeActivated())
    return;

  if(GetRightArm())
    {
      Info.AddEntry(std::string("Right arm strength: ") + GetRightArm()->GetAttribute(ARMSTRENGTH), LIGHTGRAY);
      Info.AddEntry(std::string("Right arm dexterity: ") + GetRightArm()->GetAttribute(DEXTERITY), LIGHTGRAY);
    }

  if(GetLeftArm())
    {
      Info.AddEntry(std::string("Left arm strength: ") + GetLeftArm()->GetAttribute(ARMSTRENGTH), LIGHTGRAY);
      Info.AddEntry(std::string("Left arm dexterity: ") + GetLeftArm()->GetAttribute(DEXTERITY), LIGHTGRAY);
    }

  if(GetRightLeg())
    {
      Info.AddEntry(std::string("Right leg strength: ") + GetRightLeg()->GetAttribute(LEGSTRENGTH), LIGHTGRAY);
      Info.AddEntry(std::string("Right leg agility: ") + GetRightLeg()->GetAttribute(AGILITY), LIGHTGRAY);
    }

  if(GetLeftLeg())
    {
      Info.AddEntry(std::string("Left leg strength: ") + GetLeftLeg()->GetAttribute(LEGSTRENGTH), LIGHTGRAY);
      Info.AddEntry(std::string("Left leg agility: ") + GetLeftLeg()->GetAttribute(AGILITY), LIGHTGRAY);
    }

  if(GetRightArm())
    {
      Info.AddEntry(std::string("Right attack strength: ") + ulong(GetRightArm()->GetAttackStrength() / 1000), LIGHTGRAY);
      Info.AddEntry(std::string("Right to hit value: ") + ulong(GetRightArm()->GetToHitValue()), LIGHTGRAY);
    }

  if(GetLeftArm())
    {
      Info.AddEntry(std::string("Left attack strength: ") + ulong(GetLeftArm()->GetAttackStrength() / 1000), LIGHTGRAY);
      Info.AddEntry(std::string("Left to hit value: ") + ulong(GetLeftArm()->GetToHitValue()), LIGHTGRAY);
    }

  if(GetRightLeg())
    {
      Info.AddEntry(std::string("Right kick strength: ") + ulong(GetRightLeg()->GetKickStrength() / 1000), LIGHTGRAY);
      Info.AddEntry(std::string("Right kick to hit value: ") + ulong(GetRightLeg()->GetKickToHitValue()), LIGHTGRAY);
    }

  if(GetLeftLeg())
    {
      Info.AddEntry(std::string("Left kick strength: ") + ulong(GetLeftLeg()->GetKickStrength() / 1000), LIGHTGRAY);
      Info.AddEntry(std::string("Left kick to hit value: ") + ulong(GetLeftLeg()->GetKickToHitValue()), LIGHTGRAY);
    }

  if(GetHead())
    {
      Info.AddEntry(std::string("Bite strength: ") + ulong(GetHead()->GetBiteStrength() / 1000), LIGHTGRAY);
      Info.AddEntry(std::string("Bite to hit value: ") + ulong(GetHead()->GetBiteToHitValue()), LIGHTGRAY);
    }
}

void humanoid::CompleteRiseFromTheDead()
{
  ushort c;

  for(c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c))
      {
	stack* Stack = GetStackUnder();

	/* Let's search for the original bodypart */

	for(stackiterator i = Stack->GetBottomSlot(); i != Stack->GetSlotAboveTop(); ++i)
	  {
	    if((**i)->GetID() == GetOriginalBodyPartID(c))
	      {
		item* Item = ***i;
		Item->RemoveFromSlot();
		SetBodyPart(c, (bodypart*)Item);
		break;
	      }
	  }

	/* If not found, take the first that fits */

	if(!GetBodyPart(c))
	  for(stackiterator i = Stack->GetBottomSlot(); i != Stack->GetSlotAboveTop(); ++i)
	    {
	      if((**i)->GetBodyPartIndex() == c)
		{
		  item* Item = ***i;
		  Item->RemoveFromSlot();
		  SetBodyPart(c, (bodypart*)Item);
		  break;
		}
	    }
      }

  for(c = 0; c < GetBodyParts(); ++c)
    {
      if(BodyPartVital(c) && !GetBodyPart(c))
	if(!HandleNoBodyPart(c))
	  return;

      if(GetBodyPart(c))
	GetBodyPart(c)->SetHP(1);
    }
}

bool humanoid::HandleNoBodyPart(ushort Index)
{
  switch(Index)
    {
    case HEADINDEX:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The headless body of %s vibrates violently.", CHARNAME(DEFINITE));

      Die();
      return false;
    case GROININDEX:
      CreateBodyPart(GROININDEX);
      return true;
    case TORSOINDEX:
      ABORT("The corpse does not have a torso.");
    default:
      return false;
    }
}

vector2d humanoid::GetBodyPartBitmapPos(ushort Index, ushort Frame)
{
  switch(Index)
    {
    case TORSOINDEX: return GetTorsoBitmapPos(Frame);
    case HEADINDEX: return GetHeadBitmapPos(Frame);
    case RIGHTARMINDEX: return GetRightArmBitmapPos(Frame);
    case LEFTARMINDEX: return GetLeftArmBitmapPos(Frame);
    case GROININDEX: return GetGroinBitmapPos(Frame);
    case RIGHTLEGINDEX: return GetRightLegBitmapPos(Frame);
    case LEFTLEGINDEX: return GetLeftLegBitmapPos(Frame);
    default:
      ABORT("Weird bodypart BitmapPos request for a humanoid!");
      return vector2d();
    }
}

ushort humanoid::GetBodyPartColorB(ushort Index, ushort Frame)
{
  switch(Index)
    {
    case TORSOINDEX: return GetTorsoMainColor(Frame);
    case HEADINDEX: return GetCapColor(Frame);
    case RIGHTARMINDEX:
    case LEFTARMINDEX: return GetArmMainColor(Frame);
    case GROININDEX:
    case RIGHTLEGINDEX:
    case LEFTLEGINDEX: return GetLegMainColor(Frame);
    default:
      ABORT("Weird bodypart color B request for a humanoid!");
      return 0;
    }
}

ushort humanoid::GetBodyPartColorC(ushort Index, ushort Frame)
{
  switch(Index)
    {
    case TORSOINDEX: return GetBeltColor(Frame);
    case HEADINDEX: return GetHairColor(Frame);
    case RIGHTARMINDEX:
    case LEFTARMINDEX:
    case GROININDEX:
    case RIGHTLEGINDEX:
    case LEFTLEGINDEX: return 0; // reserved for future use
    default:
      ABORT("Weird bodypart color C request for a humanoid!");
      return 0;
    }
}

ushort humanoid::GetBodyPartColorD(ushort Index, ushort Frame)
{
  switch(Index)
    {
    case TORSOINDEX: return GetTorsoSpecialColor(Frame);
    case HEADINDEX: return GetEyeColor(Frame);
    case RIGHTARMINDEX:
    case LEFTARMINDEX: return GetArmSpecialColor(Frame);
    case GROININDEX:
    case RIGHTLEGINDEX:
    case LEFTLEGINDEX: return GetLegSpecialColor(Frame);
    default:
      ABORT("Weird bodypart color D request for a humanoid!");
      return 0;
    }
}

void human::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);

  if(!Load)
    {
      EditAttribute(ARMSTRENGTH, (RAND() & 1) - (RAND() & 1));
      EditAttribute(DEXTERITY, (RAND() & 1) - (RAND() & 1));
      EditAttribute(LEGSTRENGTH, (RAND() & 1) - (RAND() & 1));
      EditAttribute(AGILITY, (RAND() & 1) - (RAND() & 1));
      EditAttribute(ENDURANCE, (RAND() & 1) - (RAND() & 1));
      EditAttribute(PERCEPTION, (RAND() & 1) - (RAND() & 1));
      EditAttribute(INTELLIGENCE, (RAND() & 1) - (RAND() & 1));
      EditAttribute(WISDOM, (RAND() & 1) - (RAND() & 1));
      EditAttribute(CHARISMA, (RAND() & 1) - (RAND() & 1));
      EditAttribute(MANA, (RAND() & 1) - (RAND() & 1));
      SetMoney(GetMoney() + RAND() % 101);
      SetSize(character::GetTotalSize() + RAND() % 51);
    }
}

void petrus::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  game::SetPetrus(this);

  if(!Load)
    {
      SetHealTimer(100);
      SetStoryState(0);
      SetAssignedName("Petrus");
    }
}

void shopkeeper::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);

  if(!Load)
    SetMoney(GetMoney() + RAND() % 2001);
}

void oree::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  
  if(!Load)
    SetAssignedName("Oree");
}

void elpuri::VirtualConstructor(bool Load)
{
  character::VirtualConstructor(Load);
  
  if(!Load)
    SetAssignedName("Elpuri");
}

void communist::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  
  if(!Load)
    SetAssignedName("Ivan");
}

void unicorn::CreateBodyParts()
{
  SetAlignment(RAND() % 3);
  character::CreateBodyParts();
}

void human::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << TotalSize;
}

void human::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> TotalSize;
}

void nonhumanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << Strength << Agility << StrengthExperience << AgilityExperience;
}

void nonhumanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> Strength >> Agility >> StrengthExperience >> AgilityExperience;
}

void nonhumanoid::CalculateUnarmedStrength()
{
  UnarmedStrength = GetBaseUnarmedStrength() * GetAttribute(ARMSTRENGTH) * GetCategoryWeaponSkill(UNARMED)->GetEffectBonus();
}

void nonhumanoid::CalculateUnarmedToHitValue()
{
  UnarmedToHitValue = ((GetAttribute(DEXTERITY) << 2) + GetAttribute(PERCEPTION)) * GetCategoryWeaponSkill(UNARMED)->GetEffectBonus() * GetMoveEase() / 100;
}

void nonhumanoid::CalculateUnarmedAPCost()
{
  UnarmedAPCost = long(GetCategoryWeaponSkill(UNARMED)->GetAPBonus() * (float(GetAttribute(DEXTERITY)) - 200) * 500 / GetMoveEase());

  if(UnarmedAPCost > -100)
    UnarmedAPCost = -100;
}

void nonhumanoid::CalculateKickStrength()
{
  KickStrength = GetBaseKickStrength() * GetAttribute(LEGSTRENGTH) * GetCategoryWeaponSkill(KICK)->GetEffectBonus();
}

void nonhumanoid::CalculateKickToHitValue()
{
  KickToHitValue = ((GetAttribute(AGILITY) << 2) + GetAttribute(PERCEPTION)) * GetCategoryWeaponSkill(KICK)->GetEffectBonus() * GetMoveEase() / 200;
}

void nonhumanoid::CalculateKickAPCost()
{
  KickAPCost = long(GetCategoryWeaponSkill(KICK)->GetAPBonus() * (float(GetAttribute(AGILITY)) - 200) * 1000 / GetMoveEase());

  if(KickAPCost > -100)
    KickAPCost = -100;
}

void nonhumanoid::CalculateBiteStrength()
{
  BiteStrength = GetBaseBiteStrength() * GetAttribute(ARMSTRENGTH) * GetCategoryWeaponSkill(BITE)->GetEffectBonus();
}

void nonhumanoid::CalculateBiteToHitValue()
{
  BiteToHitValue = ((GetAttribute(DEXTERITY) << 2) + GetAttribute(PERCEPTION)) * GetCategoryWeaponSkill(BITE)->GetEffectBonus() * GetMoveEase() / 200;
}

void nonhumanoid::CalculateBiteAPCost()
{
  BiteAPCost = long(GetCategoryWeaponSkill(BITE)->GetAPBonus() * (float(GetAttribute(DEXTERITY)) - 200) * 500 / GetMoveEase());

  if(BiteAPCost > -100)
    BiteAPCost = -100;
}

void nonhumanoid::InitSpecialAttributes()
{
  Strength = GetDefaultArmStrength() << 1;
  Agility = GetDefaultAgility() << 1;
  StrengthExperience = AgilityExperience = 0;
}

void humanoid::Bite(character* Enemy)
{
  /* This function ought not to be called without a head */

  EditNP(-50);
  EditAP(GetHead()->GetBiteAPCost());

  switch(Enemy->TakeHit(this, 0, GetHead()->GetBiteStrength(), GetHead()->GetBiteToHitValue(), RAND() % 26 - RAND() % 26, BITEATTACK, !(RAND() % GetCriticalModifier())))
    {
    case HASHIT:
    case HASBLOCKED:
      SpecialBiteEffect(Enemy);
    case HASDIED:
      if(GetCategoryWeaponSkill(BITE)->AddHit() && IsPlayer())
	{
	  GetHead()->CalculateAttackInfo();
	  GetCategoryWeaponSkill(BITE)->AddLevelUpMessage();
	}
    case HASDODGED:
      EditExperience(AGILITY, 50);
    }
}

void nonhumanoid::Bite(character* Enemy)
{
  EditNP(-50);
  EditAP(GetBiteAPCost());

  switch(Enemy->TakeHit(this, 0, GetBiteStrength(), GetBiteToHitValue(), RAND() % 26 - RAND() % 26, BITEATTACK, !(RAND() % GetCriticalModifier())))
    {
    case HASHIT:
    case HASBLOCKED:
      SpecialBiteEffect(Enemy);
    case HASDIED:
      if(GetCategoryWeaponSkill(BITE)->AddHit() && IsPlayer())
	{
	  CalculateBiteAttackInfo();
	  GetCategoryWeaponSkill(BITE)->AddLevelUpMessage();
	}
    case HASDODGED:
      EditExperience(AGILITY, 50);
    }
}

void humanoid::Kick(lsquare* Square)
{
  leg* KickLeg = GetKickLeg();
  EditAP(KickLeg->GetKickAPCost());
  Square->BeKicked(this, KickLeg->GetKickStrength(), KickLeg->GetKickToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % GetCriticalModifier()));

  /* Leg might be destroyed in the process, check needed */

  if(KickLeg == GetKickLeg())
    {
      KickLeg->EditExperience(LEGSTRENGTH, 25);
      KickLeg->EditExperience(AGILITY, 50);
    }

  EditNP(-50);

  if(GetCategoryWeaponSkill(KICK)->AddHit() && IsPlayer())
    {
      GetKickLeg()->CalculateAttackInfo(); // check existence
      GetCategoryWeaponSkill(KICK)->AddLevelUpMessage();
    }
}

void nonhumanoid::Kick(lsquare* Square)
{
  Square->BeKicked(this, GetKickStrength(), GetKickToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % GetCriticalModifier()));
  EditExperience(LEGSTRENGTH, 25);
  EditExperience(AGILITY, 50);
  EditNP(-50);
  EditAP(GetKickAPCost());

  if(GetCategoryWeaponSkill(KICK)->AddHit() && IsPlayer())
    {
      CalculateKickAttackInfo();
      GetCategoryWeaponSkill(KICK)->AddLevelUpMessage();
    }
}

bool nonhumanoid::Hit(character* Enemy)
{
  if(IsPlayer() && GetTeam()->GetRelation(Enemy->GetTeam()) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  Hostility(Enemy);

  if(GetBurdenState() == OVERLOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return true;
    }

  ushort c, AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c))
      ++AttackStyles;

  ushort Chosen = RAND() % AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c) && AttackStyles++ == Chosen)
      {
	Chosen = 1 << c;
	break;
      }

  switch(Chosen)
    {
    case USE_ARMS:
      UnarmedHit(Enemy);
      return true;
    case USE_LEGS:
      Kick(Enemy->GetLSquareUnder());
      return true;
    case USE_HEAD:
      Bite(Enemy);
      return true;
    default:
      ADD_MESSAGE("Strange alien attack style requested!");
      return false;
    }
}

void nonhumanoid::AddInfo(felist& Info) const
{
  if(!game::WizardModeActivated())
    return;

  Info.AddEntry(std::string("Strength: ") + GetAttribute(ARMSTRENGTH), LIGHTGRAY);
  Info.AddEntry(std::string("Agility: ") + GetAttribute(AGILITY), LIGHTGRAY);
  Info.AddEntry(std::string("Unarmed attack strength: ") + ulong(GetUnarmedStrength() / 1000), LIGHTGRAY);
  Info.AddEntry(std::string("Unarmed to hit value: ") + ulong(GetUnarmedToHitValue()), LIGHTGRAY);
  Info.AddEntry(std::string("Kick attack strength: ") + ulong(GetKickStrength() / 1000), LIGHTGRAY);
  Info.AddEntry(std::string("Kick to hit value: ") + ulong(GetKickToHitValue()), LIGHTGRAY);
  Info.AddEntry(std::string("Bite attack strength: ") + ulong(GetBiteStrength() / 1000), LIGHTGRAY);
  Info.AddEntry(std::string("Bite to hit value: ") + ulong(GetBiteToHitValue()), LIGHTGRAY);
}

void nonhumanoid::UnarmedHit(character* Enemy)
{
  switch(Enemy->TakeHit(this, 0, GetUnarmedStrength(), GetUnarmedToHitValue(), RAND() % 26 - RAND() % 26, UNARMEDATTACK, !(RAND() % GetCriticalModifier())))
    {
    case HASHIT:
    case HASBLOCKED:
      SpecialBiteEffect(Enemy);
    case HASDIED:
      EditExperience(ARMSTRENGTH, 50);

      if(GetCategoryWeaponSkill(BITE)->AddHit() && IsPlayer())
	{
	  CalculateUnarmedAttackInfo();
	  GetCategoryWeaponSkill(BITE)->AddLevelUpMessage();
	}
    case HASDODGED:
      EditExperience(DEXTERITY, 25);
    }

  EditNP(-50);
  EditAP(GetUnarmedAPCost());
}

/* Returns the average number of APs required to kill Enemy */

float humanoid::GetEffectivityAgainst(const character* Enemy, bool UseMaxHP) const
{
  float Effectivity = 0;
  ushort AttackStyles = 0;

  if(IsUsingArms())
    {
      float RightEffectivity, LeftEffectivity;
      bool RightArmUsed = false, LeftArmUsed = false;

      if(GetRightArm() && GetRightArm()->GetAttackStrength())
	{
	  Effectivity = RightEffectivity = Enemy->GetDurability(short(GetRightArm()->GetAttackStrength() / 50000), GetRightArm()->GetToHitValue(), UseMaxHP) * -GetRightArm()->GetAPCost();
	  RightArmUsed = true;
	}

      if(GetLeftArm() && GetLeftArm()->GetAttackStrength())
	{
	  Effectivity = LeftEffectivity = Enemy->GetDurability(short(GetLeftArm()->GetAttackStrength() / 50000), GetLeftArm()->GetToHitValue(), UseMaxHP) * -GetLeftArm()->GetAPCost();
	  LeftArmUsed = true;
	}

      if(RightArmUsed && LeftArmUsed)
	Effectivity = 1 / (1 / RightEffectivity + 1 / LeftEffectivity);

      ++AttackStyles;
    }

  if(IsUsingLegs())
    {
      leg* KickLeg = GetKickLeg();
      Effectivity += Enemy->GetDurability(short(KickLeg->GetKickStrength() / 50000), KickLeg->GetKickToHitValue(), UseMaxHP) * -KickLeg->GetKickAPCost();
      ++AttackStyles;
    }

  if(IsUsingHead())
    {
      Effectivity += Enemy->GetDurability(short(GetHead()->GetBiteStrength() / 50000), GetHead()->GetBiteToHitValue(), UseMaxHP) * -GetHead()->GetBiteAPCost();
      ++AttackStyles;
    }

  return AttackStyles ? Effectivity / AttackStyles : 1000;
}

/* Returns the average number of APs required to kill Enemy */

float nonhumanoid::GetEffectivityAgainst(const character* Enemy, bool UseMaxHP) const
{
  float Effectivity = 0;
  ushort AttackStyles = 0;

  if(IsUsingArms())
    {
      Effectivity += Enemy->GetDurability(short(GetUnarmedStrength() / 50000), GetUnarmedToHitValue(), UseMaxHP) * -GetUnarmedAPCost();
      ++AttackStyles;
    }

  if(IsUsingLegs())
    {
      Effectivity += Enemy->GetDurability(short(GetKickStrength() / 50000), GetKickToHitValue(), UseMaxHP) * -GetKickAPCost();
      ++AttackStyles;
    }

  if(IsUsingHead())
    {
      Effectivity += Enemy->GetDurability(short(GetBiteStrength() / 50000), GetBiteToHitValue(), UseMaxHP) * -GetBiteAPCost();
      ++AttackStyles;
    }

  return Effectivity / AttackStyles;
}

void nonhumanoid::ApplyExperience(bool Edited)
{
  if(CheckForAttributeIncrease(Strength, StrengthExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel stronger!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks stronger.", CHARNAME(DEFINITE));

      CalculateBurdenState();
      Edited = true;
    }
  else if(CheckForAttributeDecrease(Strength, StrengthExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel weaker.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks weaker.", CHARNAME(DEFINITE));

      CalculateBurdenState();
      Edited = true;
    }

  if(CheckForAttributeIncrease(Agility, AgilityExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel very agile!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks very agile.", CHARNAME(DEFINITE));

      Edited = true;
    }
  else if(CheckForAttributeDecrease(Agility, AgilityExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel slower.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks sluggish.", CHARNAME(DEFINITE));

      Edited = true;
    }

  character::ApplyExperience(Edited);
}

ushort humanoid::GetAttribute(ushort Identifier) const
{
  if(Identifier < BASEATTRIBUTES)
    return character::GetAttribute(Identifier);
  else
    {
      ushort Attrib = 0;

       if(Identifier == ARMSTRENGTH || Identifier == DEXTERITY)
	{
	  if(GetRightArm())
	    Attrib += GetRightArm()->GetAttribute(Identifier);

	  if(GetLeftArm())
	    Attrib += GetLeftArm()->GetAttribute(Identifier);
	}
      else if(Identifier == LEGSTRENGTH || Identifier == AGILITY)
	{
	  if(GetRightLeg())
	    Attrib += GetRightLeg()->GetAttribute(Identifier);

	  if(GetLeftLeg())
	    Attrib += GetLeftLeg()->GetAttribute(Identifier);
	}
      else
	{
	  ABORT("Illegal humanoid attribute %d request!", Identifier);
	  return 0xEBBA;
	}

      return Attrib >> 1;
    }
}

bool humanoid::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier < BASEATTRIBUTES)
    return character::EditAttribute(Identifier, Value);
  else if(Identifier == ARMSTRENGTH || Identifier == DEXTERITY)
    {
      bool Success = false;

      if(GetRightArm() && GetRightArm()->EditAttribute(Identifier, Value))
	Success = true;

      if(GetLeftArm() && GetLeftArm()->EditAttribute(Identifier, Value))
	Success = true;

      return Success;
    }
  else if(Identifier == LEGSTRENGTH || Identifier == AGILITY)
    {
      bool Success = false;

      if(GetRightLeg() && GetRightLeg()->EditAttribute(Identifier, Value))
	Success = true;

      if(GetLeftLeg() && GetLeftLeg()->EditAttribute(Identifier, Value))
	Success = true;

      return Success;
    }
  else
    {
      ABORT("Illegal humanoid attribute %d edit request!", Identifier);
      return false;
    }
}

void humanoid::EditExperience(ushort Identifier, long Value)
{
  if(Identifier < BASEATTRIBUTES)
    character::EditExperience(Identifier, Value);
  else if(Identifier == ARMSTRENGTH || Identifier == DEXTERITY)
    {
      /* If the humanoid has only one arm, should the experience be doubled? */

      if(GetRightArm())
	GetRightArm()->EditExperience(Identifier, Value);

      if(GetLeftArm())
	GetLeftArm()->EditExperience(Identifier, Value);
    }
  else if(Identifier == LEGSTRENGTH || Identifier == AGILITY)
    {
      /* If the humanoid has only one leg, should the experience be doubled? */

      if(GetRightLeg())
	GetRightLeg()->EditExperience(Identifier, Value);

      if(GetLeftLeg())
	GetLeftLeg()->EditExperience(Identifier, Value);
    }
  else
    ABORT("Illegal humanoid attribute %d experience edit request!", Identifier);
}

ushort nonhumanoid::GetAttribute(ushort Identifier) const
{
  if(Identifier < BASEATTRIBUTES)
    return character::GetAttribute(Identifier);
  else if(Identifier == ARMSTRENGTH || Identifier == LEGSTRENGTH)
    {
      if(GetTorso()->GetMainMaterial()->IsAlive())
	return Strength >> 1;
      else
	return GetTorso()->GetMainMaterial()->GetStrengthValue();
    }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    {
      if(GetTorso()->GetMainMaterial()->IsAlive())
	return Agility >> 1;
      else
	return GetTorso()->GetMainMaterial()->GetFlexibility();
    }
  else
    {
      ABORT("Illegal nonhumanoid attribute %d request!", Identifier);
      return 0xABBE;
    }
}

bool nonhumanoid::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier < BASEATTRIBUTES)
    return character::EditAttribute(Identifier, Value);
  else if(Identifier == ARMSTRENGTH || Identifier == LEGSTRENGTH)
    return RawEditAttribute(Strength, Value, true);
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    return RawEditAttribute(Agility, Value, true);
  else
    {
      ABORT("Illegal nonhumanoid attribute %d edit request!", Identifier);
      return false;
    }
}

void nonhumanoid::EditExperience(ushort Identifier, long Value)
{
  if(Identifier < BASEATTRIBUTES)
    character::EditExperience(Identifier, Value);
  else if(Identifier == ARMSTRENGTH || Identifier == LEGSTRENGTH)
    StrengthExperience += Value;
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    AgilityExperience += Value;
  else
    ABORT("Illegal nonhumanoid attribute %d experience edit request!", Identifier);
}

bool nonhumanoid::RaiseStats()
{
  Strength += 20;
  Agility += 20;
  return character::RaiseStats();
}

bool nonhumanoid::LowerStats()
{
  Strength -= 20;
  Agility -= 20;
  return character::LowerStats();
}

ushort humanoid::DrawStats(bool AnimationDraw) const
{
  DrawSilhouette(DOUBLEBUFFER, vector2d(RES.X - SILHOUETTE_X_SIZE - 23, 53), AnimationDraw);

  if(AnimationDraw)
    return 15;

  ushort PanelPosX = RES.X - 96, PanelPosY = 15;

  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "ArmStr %d", GetAttribute(ARMSTRENGTH));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LegStr %d", GetAttribute(LEGSTRENGTH));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Dex %d", GetAttribute(DEXTERITY));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Agi %d", GetAttribute(AGILITY));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "End %d", GetAttribute(ENDURANCE));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Per %d", GetAttribute(PERCEPTION));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Int %d", GetAttribute(INTELLIGENCE));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Wis %d", GetAttribute(WISDOM));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Cha %d", GetAttribute(CHARISMA));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Man %d", GetAttribute(MANA));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Siz %d", GetSize());
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, GetHP() < GetMaxHP() / 3 ? RED : WHITE, "HP %d/%d", GetHP(), GetMaxHP());
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Gold: %d", GetMoney());

  ++PanelPosY;

  if(GetAttackStyle() & USE_ARMS)
    {
      if(GetRightArm())
	if(GetRightWielded() && GetRightWielded()->IsShield(this))
	  {
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RDS %d", GetRightWielded()->GetStrengthValue());
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RDV %.0f", GetRightArm()->GetToHitValue() * GetRightWielded()->GetBlockModifier(this) / 10000);
	  }
	else
	  {
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RAS %.0f", GetRightArm()->GetAttackStrength() / 1000);
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RTHV %.0f", GetRightArm()->GetToHitValue());
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RAPC %d", -GetRightArm()->GetAPCost());
	  }

      if(GetLeftArm())
	if(GetLeftWielded() && GetLeftWielded()->IsShield(this))
	  {
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LDS %d", GetLeftWielded()->GetStrengthValue());
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LDV %.0f", GetLeftArm()->GetToHitValue() * GetLeftWielded()->GetBlockModifier(this) / 10000);
	  }
	else
	  {
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LAS %.0f", GetLeftArm()->GetAttackStrength() / 1000);
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LTHV %.0f", GetLeftArm()->GetToHitValue());
	    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LAPC %d", -GetLeftArm()->GetAPCost());
	  }
    }

  if(IsUsingLegs())
    {
      leg* Leg = GetKickLeg();
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KAS %.0f", Leg->GetKickStrength() / 1000);
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KTHV %.0f", Leg->GetKickToHitValue());
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KAPC %d", -Leg->GetKickAPCost());
    }

  if(IsUsingHead())
    {
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BAS %.0f", GetHead()->GetBiteStrength() / 1000);
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BTHV %.0f", GetHead()->GetBiteToHitValue());
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BAPC %d", -GetHead()->GetBiteAPCost());
    }

  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "DV: %.0f", GetDodgeValue());

  if(game::WizardModeActivated())
    {
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Da: %d", CurrentDanger());
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "NP: %d", GetNP());
    }
  else
    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "DL: %d", DangerLevel());

  return PanelPosY;
}

ushort nonhumanoid::DrawStats(bool AnimationDraw) const
{
  if(AnimationDraw)
    return 3;

  ushort PanelPosX = RES.X - 96, PanelPosY = 3;

  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Str %d", GetAttribute(ARMSTRENGTH));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Agi %d", GetAttribute(AGILITY));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "End %d", GetAttribute(ENDURANCE));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Per %d", GetAttribute(PERCEPTION));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Int %d", GetAttribute(INTELLIGENCE));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Wis %d", GetAttribute(WISDOM));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Cha %d", GetAttribute(CHARISMA));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Man %d", GetAttribute(MANA));
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Siz %d", GetSize());
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, GetHP() < GetMaxHP() / 3 ? RED : WHITE, "HP %d/%d", GetHP(), GetMaxHP());
  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Gold: %d", GetMoney());

  ++PanelPosY;

  if(GetAttackStyle() & USE_ARMS)
    {
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "AS %.0f", GetUnarmedStrength() / 1000);
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "THV %.0f", GetUnarmedToHitValue());
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "APC %d", -GetUnarmedAPCost());
    }

  if(GetAttackStyle() & USE_LEGS)
    {
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KAS %.0f", GetKickStrength() / 1000);
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KTHV %.0f", GetKickToHitValue());
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KAPC %d", -GetKickAPCost());
    }

  if(GetAttackStyle() & USE_HEAD)
    {
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BAS %.0f", GetBiteStrength() / 1000);
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BTHV %.0f", GetBiteToHitValue());
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BAPC %d", -GetBiteAPCost());
    }

  FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "DV: %.0f", GetDodgeValue());

  if(game::WizardModeActivated())
    {
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Da: %d", CurrentDanger());
      FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "NP: %d", GetNP());
    }
  else
    FONT->Printf(DOUBLEBUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "DL: %d", DangerLevel());

  return PanelPosY;
}

ushort humanoid::GetRandomStepperBodyPart() const
{
  std::vector<ushort> Possible;
  if(GetBodyPart(RIGHTLEGINDEX))
    Possible.push_back(RIGHTLEGINDEX);
  if(GetBodyPart(LEFTLEGINDEX))
    Possible.push_back(LEFTLEGINDEX);
  if(Possible.size())
    return Possible[RAND() % Possible.size()];
  else
    switch(RAND() % 3)
      {
      case 0:
	return GROININDEX;
      case 1:
	return TORSOINDEX;
      default:
	return HEADINDEX;
      }
}

ushort humanoid::CheckForBlock(character* Enemy, item* Weapon, float ToHitValue, ushort Damage, short Success, uchar Type)
{
  if(GetRightWielded())
    Damage = CheckForBlockWithItem(Enemy, Weapon, GetRightWielded(), ToHitValue, GetRightArm()->GetToHitValue(), Damage, Success, Type);

  if(Damage && GetLeftWielded())
    Damage = CheckForBlockWithItem(Enemy, Weapon, GetLeftWielded(), ToHitValue, GetLeftArm()->GetToHitValue(), Damage, Success, Type);

  return Damage;
}

bool humanoid::CanWield() const
{
  return CanUseEquipment(RIGHTWIELDEDINDEX) || CanUseEquipment(LEFTWIELDEDINDEX);
}

bool humanoid::CheckBalance(float KickStrength)
{
  if(GetLegs() == 1)
    return true;
  else
    return KickStrength / 1000 >= RAND() % GetSize();
}

long humanoid::GetMoveAPRequirement(uchar Difficulty) const
{
  if(CanFly())
    return (long(GetAttribute(AGILITY)) - 200) * Difficulty * GetMoveEase() / 20;

  switch(GetLegs())
    {
    case 0:
      return (long(GetAttribute(AGILITY)) - 200) * Difficulty * GetMoveEase() / 2;
    case 1:
      return (long(GetAttribute(AGILITY)) - 200) * Difficulty * GetMoveEase() * 3 / 20;
    case 2:
      return (long(GetAttribute(AGILITY)) - 200) * Difficulty * GetMoveEase() / 20;
    default:
      ABORT("A %d legged humanoid invaded the dungeon!", GetLegs());
      return 0;
    }
}

bool humanoid::EquipmentHasNoPairProblems(ushort Index) const
{
  switch(Index)
    {
    case RIGHTGAUNTLETINDEX:
      return GetLeftGauntlet() && GetLeftGauntlet()->IsSimiliarTo(GetRightGauntlet());
    case LEFTGAUNTLETINDEX:
      return GetRightGauntlet() && GetRightGauntlet()->IsSimiliarTo(GetLeftGauntlet());
    case RIGHTBOOTINDEX:
      return GetLeftBoot() && GetLeftBoot()->IsSimiliarTo(GetRightBoot());
    case LEFTBOOTINDEX:
      return GetRightBoot() && GetRightBoot()->IsSimiliarTo(GetLeftBoot());
    }

  return true;
}

void hunter::CreateBodyPart(ushort Index)
{
  if(Index != LEFTARMINDEX)
    character::CreateBodyPart(Index);
  else
    SetLeftArm(0);
}

bool humanoid::EquipmentEasilyRecognized(ushort Index) const
{
  switch(Index)
    {
    case AMULETINDEX:
    case RIGHTRINGINDEX:
    case LEFTRINGINDEX:
    case BELTINDEX:
      return false;
    }

  return true;
}

void humanoid::VirtualConstructor(bool)
{
  SetCurrentRightSingleWeaponSkill(0);
  SetCurrentLeftSingleWeaponSkill(0);
}

void humanoid::SignalEquipmentAdd(ushort EquipmentIndex)
{
  character::SignalEquipmentAdd(EquipmentIndex);

  if(EquipmentIndex == RIGHTWIELDEDINDEX)
    {
      for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
	if((*i)->GetID() == GetRightWielded()->GetID())
	  {
	    SetCurrentRightSingleWeaponSkill(*i);
	    break;
	  }

      if(!GetCurrentRightSingleWeaponSkill())
	{
	  SetCurrentRightSingleWeaponSkill(new sweaponskill);
	  GetCurrentRightSingleWeaponSkill()->SetID(GetRightWielded()->GetID());
	  SingleWeaponSkill.push_back(GetCurrentRightSingleWeaponSkill());
	}
    }
  else if(EquipmentIndex == LEFTWIELDEDINDEX)
    {
      for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
	if((*i)->GetID() == GetLeftWielded()->GetID())
	  {
	    SetCurrentLeftSingleWeaponSkill(*i);
	    break;
	  }

      if(!GetCurrentLeftSingleWeaponSkill())
	{
	  SetCurrentLeftSingleWeaponSkill(new sweaponskill);
	  GetCurrentLeftSingleWeaponSkill()->SetID(GetLeftWielded()->GetID());
	  SingleWeaponSkill.push_back(GetCurrentLeftSingleWeaponSkill());
	}
    }

  CalculateBattleInfo();
}

void humanoid::SignalEquipmentRemoval(ushort EquipmentIndex)
{
  character::SignalEquipmentRemoval(EquipmentIndex);

  if(EquipmentIndex == RIGHTWIELDEDINDEX)
    {
      if(!GetCurrentRightSingleWeaponSkill()->GetHits())
	for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
	  if(*i == GetCurrentRightSingleWeaponSkill())
	    {
	      delete *i;
	      SingleWeaponSkill.erase(i);
	      break;
	    }

      SetCurrentRightSingleWeaponSkill(0);
    }
  else if(EquipmentIndex == LEFTWIELDEDINDEX)
    {
      if(!GetCurrentLeftSingleWeaponSkill()->GetHits())
	for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end(); ++i)
	  if(*i == GetCurrentLeftSingleWeaponSkill())
	    {
	      delete *i;
	      SingleWeaponSkill.erase(i);
	      break;
	    }

      SetCurrentLeftSingleWeaponSkill(0);
    }

  CalculateBattleInfo();
}

void humanoid::CharacterSpeciality()
{
  for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end();)
    {
      if((*i)->Tick() && IsPlayer())
	{
	  for(stackiterator j = GetStack()->GetBottomSlot(); j != GetStack()->GetSlotAboveTop(); ++j)
	    if((*i)->GetID() == (**j)->GetID())
	      {
		(*i)->AddLevelDownMessage((**j)->GetName(UNARTICLED));
		break;
	      }

	  for(ushort c = 0; c < EquipmentSlots(); ++c)
	    if(GetEquipment(c) && GetEquipment(c)->GetID() == (*i)->GetID())
	      {
		(*i)->AddLevelDownMessage(GetEquipment(c)->GetName(UNARTICLED));
		break;
	      }
	}

      if(!(*i)->GetHits() && *i != GetCurrentRightSingleWeaponSkill() && *i != GetCurrentLeftSingleWeaponSkill())
	{
	  std::vector<sweaponskill*>::iterator Dirt = i++;
	  SingleWeaponSkill.erase(Dirt);
	  continue;
	}
      else
	++i;
    }
}

bool humanoid::DetachBodyPart()
{
  uchar ToBeDetached;
  switch(game::KeyQuestion("What limb do you wish to detach? (l)eft arm, (r)ight arm, (L)eft leg, (R)ight leg?", KEYESC, 4, 'l','r','L','R'))
    {
    case 'l':
      ToBeDetached = LEFTARMINDEX;
      break;
    case 'r':
      ToBeDetached = RIGHTARMINDEX;
      break;
    case 'L':
      ToBeDetached = LEFTLEGINDEX;
      break;
    case 'R':
      ToBeDetached = RIGHTLEGINDEX;
      break;
    default:
      return false;
    }

  bodypart* BodyPart = GetBodyPart(ToBeDetached);

  if(BodyPart)
    {
      item* ToDrop = SevereBodyPart(ToBeDetached);
      GetSquareUnder()->SendNewDrawRequest();
      GetStack()->AddItem(ToDrop);
      ToDrop->DropEquipment();
      ADD_MESSAGE("Bodypart detached!");  
      return true;
    }
  else
    {
      ADD_MESSAGE("That bodypart has already been detached.");
      return false;
    }
}

void angel::GetAICommand()
{
  SetHealTimer(GetHealTimer() + 1);

  if(GetHealTimer() > LENGTH_OF_ANGELS_HEAL_COUNTER_LOOP && AttachBodyPartsOfFriendsNear())
    {
      SetHealTimer(0);
      return;
    }

  humanoid::GetAICommand();
}

/* Returns true if the angel finds somebody near to heal else false */
bool angel::AttachBodyPartsOfFriendsNear()
{
  std::vector<character*> HurtFriends = GetFriendsAround();
  std::vector<character*> Temp;

  /* 
     Now analyze HurtFriendsAround for really hurt friends and delete unhurt ones and 
     check whether the HurtFriends are curable (ie. they have one of their bodyparts in their stack)
   */

  std::vector<character*>::iterator i;

  for(i = HurtFriends.begin(); i != HurtFriends.end(); ++i)
    if(!(*i)->HasAllBodyParts() && (*i)->FindRandomOwnBodyPart() != 0)
      Temp.push_back(*i);

  HurtFriends.swap(Temp);
 
  /* Now if there are no friends left return false */
  if(HurtFriends.empty())
    return false;

  /* Check whether the player is in the list */
  character* TheOneWhoWillBeCured = 0;

  for(i = HurtFriends.begin(); i != HurtFriends.end(); ++i)
    if(!(*i)->IsPlayer())
      {
	TheOneWhoWillBeCured = *i;
	break;
      }

  /* If no player found, take one friend at random */
  if(!TheOneWhoWillBeCured)
    TheOneWhoWillBeCured = HurtFriends[RAND() % HurtFriends.size()];

  bodypart* JustCreated = TheOneWhoWillBeCured->TryAttachRandomOldBodyPart();
  if(JustCreated)
    {
      if(TheOneWhoWillBeCured->IsPlayer())
	ADD_MESSAGE("%s puts %s back to its place.", CHARNAME(DEFINITE), JustCreated->CHARNAME(DEFINITE));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s helps %s by putting %s in its old place.", CHARNAME(DEFINITE), TheOneWhoWillBeCured->CHARNAME(DEFINITE), JustCreated->CHARNAME(DEFINITE));
	
      JustCreated->SetHP(1);

      return true;
    }
  else
    {
      ABORT("Dark forces attack angel. Too Bad. Sorry.");
      return false;
    }
}

void angel::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  SetHealTimer(LENGTH_OF_ANGELS_HEAL_COUNTER_LOOP);
}

void humanoid::DrawBodyParts(bitmap* Bitmap, vector2d Pos, ushort Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  /* Order is important: Don't use a loop. */

  if(GetGroin())
    GetGroin()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetRightLeg())
    GetRightLeg()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetLeftLeg())
    GetLeftLeg()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetTorso())
    GetTorso()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
  
  if(GetRightArm())
    GetRightArm()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetLeftArm())
    GetLeftArm()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetHead())
    GetHead()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
}

void dwarf::DrawBodyParts(bitmap* Bitmap, vector2d Pos, ushort Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  if(GetGroin())
    GetGroin()->Draw(Bitmap, Pos + vector2d(0, -1), Luminance, AllowAlpha, AllowAnimate);

  if(GetRightLeg())
    GetRightLeg()->Draw(Bitmap, Pos + vector2d(0, -1), Luminance, AllowAlpha, AllowAnimate);

  if(GetLeftLeg())
    GetLeftLeg()->Draw(Bitmap, Pos + vector2d(0, -1), Luminance, AllowAlpha, AllowAnimate);

  if(GetTorso())
    GetTorso()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
  
  if(GetRightArm())
    GetRightArm()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetLeftArm())
    GetLeftArm()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetHead())
    GetHead()->Draw(Bitmap, Pos + vector2d(0, 1), Luminance, AllowAlpha, AllowAnimate);
}


void snake::SpecialBiteEffect(character* Char)
{
  Char->BeginTemporaryState(POISONED, 500);
}

ushort angel::GetTorsoMainColor(ushort) const
{
  return GetMasterGod()->GetColor();
}

ushort angel::GetArmMainColor(ushort) const
{
  return GetMasterGod()->GetColor();
}

void angel::CreateBodyPart(ushort Index)
{
  if(Index == GROININDEX || Index == RIGHTLEGINDEX || Index == LEFTLEGINDEX)
    SetBodyPart(Index, 0);
  else
    character::CreateBodyPart(Index);
}

void genie::CreateBodyPart(ushort Index)
{
  if(Index == GROININDEX || Index == RIGHTLEGINDEX || Index == LEFTLEGINDEX)
    SetBodyPart(Index, 0);
  else
    character::CreateBodyPart(Index);
}

ushort housewife::GetHairColor(ushort) const
{
  static ushort HouseWifeHairColor[] = { MAKE_RGB(48, 40, 8), MAKE_RGB(60, 48, 24),  MAKE_RGB(200, 0, 0) };
  return HouseWifeHairColor[RAND() % 3];
}

ushort angel::GetAttribute(ushort Identifier) const // temporary until wings are bodyparts
{
  if(Identifier == LEGSTRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return GetDefaultAgility();
  else
    return humanoid::GetAttribute(Identifier);
}

ushort genie::GetAttribute(ushort Identifier) const // temporary until someone invents a better way of doing this
{
  if(Identifier == LEGSTRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return 50; /* Air's flexibility */
  else
    return humanoid::GetAttribute(Identifier);
}

float billswill::GetUnarmedStrength() const
{
  return GetBaseUnarmedStrength() * 5 * GetCategoryWeaponSkill(UNARMED)->GetEffectBonus();
}

void spider::SpecialBiteEffect(character* Char)
{
  Char->BeginTemporaryState(POISONED, 50);
}

bool humanoid::CanUseStethoscope(bool PrintReason) const
{
  if(GetArms() == 0)
    {
      if(PrintReason)
	ADD_MESSAGE("You need an arm to use a stethoscope.");
      return false;
    }

  if(GetHead() == 0)
    {
      if(PrintReason)
	ADD_MESSAGE("You need a head to use stethoscope.");
      return false;
    }
  return true;
}

bool humanoid::IsUsingArms() const
{
  return GetAttackStyle() & USE_ARMS && ((GetRightArm() && GetRightArm()->GetAttackStrength()) || (GetLeftArm() && GetLeftArm()->GetAttackStrength()));
}

bool humanoid::IsUsingLegs() const
{
  return (GetAttackStyle() & USE_LEGS || (GetAttackStyle() & USE_ARMS && (!GetRightArm() || !GetRightArm()->GetAttackStrength()) && (!GetLeftArm() || !GetLeftArm()->GetAttackStrength()))) && GetRightLeg() && GetLeftLeg();
}

bool humanoid::IsUsingHead() const
{
  return (GetAttackStyle() & USE_HEAD || ((GetAttackStyle() & USE_LEGS || (GetAttackStyle() & USE_ARMS && (!GetRightArm() || !GetRightArm()->GetAttackStrength()) && (!GetLeftArm() || !GetLeftArm()->GetAttackStrength()))) && (!GetRightLeg() || !GetLeftLeg()))) && GetHead();
}

void humanoid::AddAttackInfo(felist& List) const
{
  if(GetAttackStyle() & USE_ARMS)
    {
      if(GetRightArm())
	GetRightArm()->AddAttackInfo(List);

      if(GetLeftArm())
	GetLeftArm()->AddAttackInfo(List);
    }

  if(IsUsingLegs())
    {
      leg* KickLeg = GetKickLeg();

      std::string Entry = "   kick attack";
      Entry.resize(50, ' ');
      Entry << int(KickLeg->GetKickStrength() / 50000);
      Entry.resize(60, ' ');
      Entry << int(KickLeg->GetKickToHitValue());
      Entry.resize(70, ' ');
      Entry << -KickLeg->GetKickAPCost();
      List.AddEntry(Entry, LIGHTGRAY);
    }

  if(IsUsingHead())
    {
      std::string Entry = "   bite attack";
      Entry.resize(50, ' ');
      Entry << int(GetHead()->GetBiteStrength() / 50000);
      Entry.resize(60, ' ');
      Entry << int(GetHead()->GetBiteToHitValue());
      Entry.resize(70, ' ');
      Entry << -GetHead()->GetBiteAPCost();
      List.AddEntry(Entry, LIGHTGRAY);
    }
}

void nonhumanoid::AddAttackInfo(felist& List) const
{
  if(IsUsingArms())
    {
      std::string Entry = "   unarmed attack";
      Entry.resize(50, ' ');
      Entry << int(GetUnarmedStrength() / 50000);
      Entry.resize(60, ' ');
      Entry << int(GetUnarmedToHitValue());
      Entry.resize(70, ' ');
      Entry << -GetUnarmedAPCost();
      List.AddEntry(Entry, LIGHTGRAY);
    }

  if(IsUsingLegs())
    {
      std::string Entry = "   kick attack";
      Entry.resize(50, ' ');
      Entry << int(GetKickStrength() / 50000);
      Entry.resize(60, ' ');
      Entry << int(GetKickToHitValue());
      Entry.resize(70, ' ');
      Entry << -GetKickAPCost();
      List.AddEntry(Entry, LIGHTGRAY);
    }

  if(IsUsingHead())
    {
      std::string Entry = "   bite attack";
      Entry.resize(50, ' ');
      Entry << int(GetBiteStrength() / 50000);
      Entry.resize(60, ' ');
      Entry << int(GetBiteToHitValue());
      Entry.resize(70, ' ');
      Entry << -GetBiteAPCost();
      List.AddEntry(Entry, LIGHTGRAY);
    }
}

void humanoid::CalculateBattleInfo()
{
  CalculateDodgeValue();

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      {
	GetBodyPart(c)->CalculateAttackStrength();
	GetBodyPart(c)->CalculateToHitValue();
	GetBodyPart(c)->CalculateAPCost();
      }
}

void nonhumanoid::CalculateBattleInfo()
{
  CalculateDodgeValue();
  CalculateUnarmedAttackInfo();
  CalculateKickAttackInfo();
  CalculateBiteAttackInfo();
}

void nonhumanoid::CalculateUnarmedAttackInfo()
{
  CalculateUnarmedStrength();
  CalculateUnarmedToHitValue();
  CalculateUnarmedAPCost();
}

void nonhumanoid::CalculateKickAttackInfo()
{
  CalculateKickStrength();
  CalculateKickToHitValue();
  CalculateKickAPCost();
}

void nonhumanoid::CalculateBiteAttackInfo()
{
  CalculateBiteStrength();
  CalculateBiteToHitValue();
  CalculateBiteAPCost();
}

leg* humanoid::GetKickLeg() const
{
  return GetRightLeg()->GetKickStrength() >= GetLeftLeg()->GetKickStrength() ? static_cast<leg*>(GetRightLeg()) : static_cast<leg*>(GetLeftLeg());
}

material* humanoid::CreateBodyPartFlesh(ushort, ulong Volume) const
{
  if(!CreateSolidMaterialConfigurations())
    return MAKE_MATERIAL(HUMANFLESH, Volume);
  else
    return MAKE_MATERIAL(Config, Volume);
}

item* skeleton::SevereBodyPart(ushort BodyPartIndex)
{
  bone* Bone = new bone(0, false);
  item* BodyPart = GetBodyPart(BodyPartIndex);
  Bone->InitMaterials(BodyPart->GetContainedMaterial());
  BodyPart->SetContainedMaterial(0);
  BodyPart->SendToHell();
  BodyPart->DropEquipment();
  BodyPart->RemoveFromSlot();
  return Bone;  
}
