#include <cmath>

#include "charde.h"
#include "stack.h"
#include "itemde.h"
#include "level.h"
#include "lsquare.h"
#include "femath.h"
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

perttu::~perttu()
{
	game::SetPerttu(0);
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

void perttu::CreateInitialEquipment()
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new valpurijustifier)));
	SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new platemail(new valpurium))));
}

void priest::CreateInitialEquipment()
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new spikedmace)));
	SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new chainmail(new mithril))));
}

void oree::CreateInitialEquipment()
{
	GetStack()->FastAddItem(new maakotkashirt);

	for(ushort c = 0; c < 6; ++c)
	{
		item* Can = new can(false);
		Can->InitMaterials(2, new iron(10), new pepsi(330));
		GetStack()->FastAddItem(Can);
	}
}

void swatcommando::CreateInitialEquipment()
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(rand() % 20 ? (item*)(new twohandedsword) : (item*)(new curvedtwohandedsword))));
	GetStack()->FastAddItem(new lamp);
	GetStack()->FastAddItem(new chainmail(new mithril));
}

void fallenvalpurist::CreateInitialEquipment()
{
	if(rand() % 10)
		SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(rand() % 3 ? (item*)(new axe) : (item*)(new pickaxe))));
	else
	{
		SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new spikedmace)));
		SetSize(200);
		SetAgility(15);
		SetStrength(18);
		SetEndurance(15);
	}
}

void froggoblin::CreateInitialEquipment()
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(rand() % 3 ? (item*)(new spear) : (item*)(new poleaxe))));
}

void cityguard::CreateInitialEquipment()
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new poleaxe)));
	SetTorsoArmor(GetStack()->GetItem(GetStack()->FastAddItem(new chainmail)));
}

ushort humanoid::CalculateArmorModifier() const
{
	return Armor.Torso ? Armor.Torso->GetArmorValue() : 100;
}

bool ennerbeast::Hit(character*)
{
	char Message[256] ;
	if(rand() % 2) sprintf(Message,"The Enner Beast yells: UGH UGHAaaa!");
	else sprintf(Message, "The Enner Beast yells: Uga Ugar Ugade Ugat!");

	DO_FILLED_RECTANGLE(GetPos().X, GetPos().Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, 100,
	{
		character* Char = game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer, YPointer))->GetCharacter();

		float ScreamStrength = GetMeleeStrength() * GetStrength() / GetHypotSquare(float(GetPos().X) - XPointer, float(GetPos().Y) - YPointer);

		if(Char && Char != this)
			Char->ReceiveSound(Message, rand() % 26 - rand() % 26,ScreamStrength);

		game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer, YPointer))->GetStack()->ReceiveSound(ScreamStrength);

		for(uchar x = 0; x < 4; ++x)
			game::GetCurrentLevel()->GetLevelSquare(vector2d(XPointer, YPointer))->GetSideStack(x)->ReceiveSound(ScreamStrength);
	});

	SetStrengthExperience(GetStrengthExperience() + 100);

	return true;
}

bool humanoid::Drop()
{
	ushort Index = GetStack()->DrawContents(this, "What do you want to drop?");

	if(Index < GetStack()->GetItems() && GetStack()->GetItem(Index))
		if(GetStack()->GetItem(Index) == GetWielded())
			ADD_MESSAGE("You can't drop something you wield!");
		else if(GetStack()->GetItem(Index) == Armor.Torso)
			ADD_MESSAGE("You can't drop something you wear!");
		else
			if(!GetLevelSquareUnder()->GetRoom() || (GetLevelSquareUnder()->GetRoom() && GetLevelSquareUnder()->GetLevelUnder()->GetRoom(GetLevelSquareUnder()->GetRoom())->DropItem(this, GetStack()->GetItem(Index))))
			{
				GetStack()->MoveItem(Index, GetLevelSquareUnder()->GetStack());

				return true;
			}

	return false;
}

void golem::DrawToTileBuffer() const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

void humanoid::DrawToTileBuffer() const
{	
	vector2d InHandsPic, ArmPos, HeadPos;

	if(GetArmType() > 16)
	{
		ArmPos.X = 80;
		ArmPos.Y = (GetArmType() - 16) * 16;
	}
	else
	{
		ArmPos.X = 64;
		ArmPos.Y = GetArmType() * 16;
	}
	if(GetHeadType() > 16)
	{
		HeadPos.X = 112;
		HeadPos.Y = (GetHeadType() - 16) * 16;
	}
	else
	{
		HeadPos.X = 96;
		HeadPos.Y = GetHeadType() * 16;
	}

	if(GetWielded() != 0) InHandsPic = GetWielded()->GetInHandsPic();

	igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16); // Legs
	igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), 32, 0, 0, 0, 16, 16); // Torso
	igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), ArmPos.X, ArmPos.Y, 0, 0, 16, 16); // Arms
	igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), HeadPos.X, HeadPos.Y, 0, 0, 16, 16); // Head

	if(GetWielded() != 0 && (InHandsPic.X != 0 || InHandsPic.Y != 0))
		igraph::GetHumanGraphic()->MaskedBlit(igraph::GetTileBuffer(), InHandsPic.X , InHandsPic.Y, 0, 0, 16, 16); // Wielded
}

bool humanoid::Wield()
{
	ushort Index;
	if((Index = GetStack()->DrawContents(this, "What do you want to wield? or press '-' for hands")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to wield.");
		return false;
	}

	if(Index == 0xFFFE)
		SetWielded(0);
	else
		if(Index < GetStack()->GetItems())
	{
	if(GetStack()->GetItem(Index) != Armor.Torso)
		SetWielded(GetStack()->GetItem(Index));
	else ADD_MESSAGE("You can't wield something that you wear!");
	}

		else
			return false;

	return true;
	
}

void fallenvalpurist::CreateCorpse()
{
	ushort Amount = 2 + rand() % 4;

	for(ushort c = 0; c < Amount; ++c)
		GetLevelSquareUnder()->GetStack()->AddItem(new abone);
}

void elpuri::CreateCorpse()
{
	character::CreateCorpse();

	GetLevelSquareUnder()->GetStack()->AddItem(new headofelpuri);
}

void perttu::CreateCorpse()
{
	GetLevelSquareUnder()->GetStack()->AddItem(new leftnutofperttu);
}

void ennerbeast::CreateCorpse()
{
	character::CreateCorpse();

	GetLevelSquareUnder()->GetStack()->AddItem(new headofennerbeast);
}


bool humanoid::WearArmor()
{
	ushort Index;

	if((Index = GetStack()->DrawContents(this, "What do you want to wear? or press '-' for nothing")) == 0xFFFF)
		return false;

	if(Index == 0xFFFE)
	{
		Armor.Torso = 0;
		return true;
	}
	else
		if(Index < GetStack()->GetItems())		// Other Armor types should be coded...
		{
			if(GetStack()->GetItem(Index)->CanBeWorn())
			{
				if(GetStack()->GetItem(Index) != GetWielded())
				{
					Armor.Torso = GetStack()->GetItem(Index);
					return true;
				}
				else
					ADD_MESSAGE("You can't wear something that you wield!");
			}
			else
				ADD_MESSAGE("You can't wear THAT!");
		}

	return false;
}

void humanoid::Save(outputfile& SaveFile) const
{
	character::Save(SaveFile);

	ushort Index = Armor.Torso ? Stack->SearchItem(Armor.Torso) : 0xFFFF;

	SaveFile << Index << ArmType << HeadType << LegType << TorsoType << SingleWeaponSkill;

	for(uchar c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
		SaveFile << GetCategoryWeaponSkill(c);
}

void humanoid::Load(inputfile& SaveFile)
{
	character::Load(SaveFile);

	ushort Index;

	SaveFile >> Index >> ArmType >> HeadType >> LegType >> TorsoType >> SingleWeaponSkill;

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

void golem::MoveRandomly()
{
	ushort ToTry = rand() % 9;

	if(ToTry == 8)
	{
		if(!(rand () % 100))
			Engrave("Golem Needs Master");
	}
	else
		if(game::GetCurrentLevel()->IsValid(GetPos() + game::GetMoveVector(ToTry)) && !game::GetCurrentLevel()->GetLevelSquare(GetPos() + game::GetMoveVector(ToTry))->GetCharacter())
			TryMove(GetPos() + game::GetMoveVector(ToTry));
}

void ennerbeast::GetAICommand()
{
	SeekLeader();

	if(CheckForEnemies())
		return;

	if(FollowLeader())
		return;

	if(rand() % 3)
		Hit(0);
	else
		MoveRandomly();
}

void perttu::GetAICommand()
{
	SeekLeader();

	SetHealTimer(GetHealTimer() + 1);

	character* Char;

	DO_FOR_SQUARES_AROUND(GetPos().X, GetPos().Y, game::GetCurrentLevel()->GetXSize(), game::GetCurrentLevel()->GetYSize(),
	if(Char = game::GetCurrentLevel()->GetLevelSquare(vector2d(DoX, DoY))->GetCharacter())
	{
		if(GetTeam()->GetRelation(Char->GetTeam()) == FRIEND && Char->GetHP() < Char->GetMaxHP() / 3 && GetHealTimer() > 100)
		{
			HealFully(Char);
			return;
		}
	})

	if(CheckForEnemies())
		return;

	if(CheckForDoors())
		return;

	if(CheckForUsefulItemsOnGround())
		return;

	FollowLeader();
}

void perttu::HealFully(character* ToBeHealed)
{
	SetHealTimer(0);

	ToBeHealed->SetHP(ToBeHealed->GetMaxHP());

	if(ToBeHealed->GetIsPlayer())
		ADD_MESSAGE("%s heals you fully.", CNAME(DEFINITE));
	else
		ADD_MESSAGE("%s heals %s fully.", CNAME(DEFINITE), ToBeHealed->CNAME(DEFINITE));
}

void perttu::Save(outputfile& SaveFile) const
{
	humanoid::Save(SaveFile);

	SaveFile << HealTimer << StoryState;
}

void perttu::Load(inputfile& SaveFile)
{
	humanoid::Load(SaveFile);

	SaveFile >> HealTimer >> StoryState;

	game::SetPerttu(this);
}

bool humanoid::Throw()
{
	ushort Index;

	if((Index = GetStack()->DrawContents(this, "What do you want to throw?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to throw.");
		return false;
	}

	if(Index < GetStack()->GetItems())
	{
		if(GetStack()->GetItem(Index) == GetTorsoArmor())
		{
			ADD_MESSAGE("You can't throw something that you wear.");
			return false;
		}

		uchar Answer = game::DirectionQuestion("In what direction do you wish to throw it?", 8, false);

		if(Answer == 0xFF)
			return false;

		if(GetStack()->GetItem(Index) == GetWielded())
			SetWielded(0);

		ThrowItem(Answer, GetStack()->GetItem(Index));
	}
	else
		return false;

	return true;
}

bool dog::Catches(item* Thingy, float)
{
	if(Thingy->DogWillCatchAndConsume())
	{
		if(GetIsPlayer())
			ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CNAME(DEFINITE));
		else
		{
			if(GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s catches %s and eats it.", CNAME(DEFINITE), Thingy->CNAME(DEFINITE));
			SetTeam(game::GetPlayer()->GetTeam());
		}
		ConsumeItem(Thingy, GetLevelSquareUnder()->GetStack());
		return true;
	}

	return false;
}

bool dog::ConsumeItemType(uchar Type) const     // We need a better system for this... Writing this to every F***ing character that needs one
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
	case BONE: // Bone is an odd item, cause it actually can be opened, but not always...
		return true;
	break;


	default:
		ADD_MESSAGE("ERRRRORRRRRR in dog::Consumeitemtype.");
	}
		
	return false;
}

bool humanoid::Apply()
{
	ushort Index;

	if((Index = GetStack()->DrawContents(this, "What do you want to apply?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to apply.");
		return false;
	}

	if(Index < GetStack()->GetItems())
	{
		if(GetStack()->GetItem(Index) == GetTorsoArmor())
		{
			ADD_MESSAGE("You can't apply something that you wear.");
			return false;
		}
		
		if(!GetStack()->GetItem(Index)->Apply(this, GetStack()))
			return false;

		if(GetWielded() && !GetWielded()->GetExists()) 
			SetWielded(0);
	}
	else
		return false;

	return true;
}

float humanoid::GetAttackStrength() const
{
	return GetWielded() ? GetWielded()->GetWeaponStrength() * GetCategoryWeaponSkill(GetWielded()->GetWeaponCategory())->GetBonus() * GetCurrentSingleWeaponSkill()->GetBonus() : GetMeleeStrength() * GetCategoryWeaponSkill(UNARMED)->GetBonus();
}

bool humanoid::Hit(character* Enemy)
{
	if(GetTeam()->GetRelation(Enemy->GetTeam()) != HOSTILE)
		if(GetIsPlayer() && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
			return false;

	GetTeam()->Hostility(Enemy->GetTeam());

	if(GetTeam() == Enemy->GetTeam())
		Enemy->SetTeam(game::GetTeam(1));

	short Success = rand() % 26 - rand() % 26;

	switch(Enemy->TakeHit(this, Success)) //there's no breaks and there shouldn't be any
	{
	case HAS_HIT:
	case HAS_BLOCKED:
		if(GetWielded())
			GetWielded()->ReceiveHitEffect(Enemy, this);
	case HAS_DIED:
		SetStrengthExperience(GetStrengthExperience() + 50);
		if(GetCategoryWeaponSkill(GetWielded() ? GetWielded()->GetWeaponCategory() : UNARMED)->AddHit(GetIsPlayer()))
			GetCategoryWeaponSkill(GetWielded() ? GetWielded()->GetWeaponCategory() : UNARMED)->AddLevelUpMessage();
		if(GetWielded())
			if(GetCurrentSingleWeaponSkill()->AddHit(GetIsPlayer()))
				GetCurrentSingleWeaponSkill()->AddLevelUpMessage(GetWielded()->Name(UNARTICLED));
	case HAS_DODGED:
		SetAgilityExperience(GetAgilityExperience() + 25);
	}

	SetNP(GetNP() - 4);

	return true;
}

void humanoid::CharacterSpeciality()
{
	for(uchar c = 0; c < WEAPON_SKILL_GATEGORIES; ++c)
		if(GetCategoryWeaponSkill(c)->Turn())
			GetCategoryWeaponSkill(c)->AddLevelDownMessage();

	for(std::vector<sweaponskill*>::iterator i = SingleWeaponSkill.begin(); i != SingleWeaponSkill.end();)
	{
		if((*i)->Turn())
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
			std::vector<sweaponskill*>::iterator p = SingleWeaponSkill.end();
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

	/*if(SingleWeaponSkill.size())
	{
		felist List("Your experience in single weapons");

		List.AddDescription("");
		List.AddDescription("Weapon name         Level     Points    To next level");

		for(uchar c = 0; c < SingleWeaponSkill.size(); ++c)
		{
			std::string Buffer;

			Buffer += GetSingleWeaponSkill(c)->Name();
			Buffer.resize(20, ' ');

			Buffer += GetSingleWeaponSkill(c)->GetLevel();
			Buffer.resize(30, ' ');

			Buffer += int(GetSingleWeaponSkill(c)->GetHits());
			Buffer.resize(40, ' ');

			if(GetSingleWeaponSkill(c)->GetLevel() != 10)
				List.AddEntry(Buffer + (GetSingleWeaponSkill(c)->GetLevelMap(GetSingleWeaponSkill(c)->GetLevel() + 1) - GetSingleWeaponSkill(c)->GetHits()), RED);
			else
				List.AddEntry(Buffer + '-', RED);
		}

		List.Draw();
	}*/

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

	if(Wielded = Something)
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

void humanoid::ReceiveSound(char* Pointer, short Success, float ScreamStrength)
{
	character::ReceiveSound(Pointer, Success, ScreamStrength);

	if(GetTorsoArmor() && !GetTorsoArmor()->GetExists())
		SetTorsoArmor(0);
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
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new pickaxe)));
}

void farmer::CreateInitialEquipment()
{
	SetWielded(GetStack()->GetItem(GetStack()->FastAddItem(new axe)));
}

void perttu::AddHitMessage(character* Enemy, const bool Critical) const
{
	/*
	 * This function is temporary. It prevents Perttu's hit messages becoming too long,
	 * which would at present make the message history quite ugly.
	 */

	std::string ThisDescription = GetLevelSquareUnder()->CanBeSeen() ? "Perttu" : "something";
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

void perttu::BeTalkedTo(character* Talker)
{
	if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
	{
		ADD_MESSAGE("Heretic! Dev/null is a place not worthy to receive thee!");
		return;
	}

	if(Talker->HasMaakotkaShirt())
	{
		iosystem::TextScreen("Thou hast slain the Pepsi Daemon King, and Perttu is happy!\n\nYou are victorious!");
		game::RemoveSaves();
		game::Quit();

		if(!game::GetWizardMode())
		{
			AddScoreEntry("retrieved the Holy Maakotka Shirt and was titled as the Avatar of Law", 3);
			highscore HScore;
			HScore.Draw();
		}

		return;
	}
	else
		if(StoryState >= 2)
		{
			ADD_MESSAGE("Perttu says: \"Bring me the Maakotka shirt and we'll talk.\"");
			return;
		}

	if(Talker->HasHeadOfElpuri())
	{
		if(game::GetGod(1)->GetRelation() >= 0 && Talker->Danger() >= 100000)
		{
			ADD_MESSAGE("Perttu smiles. \"Thou areth indeed a great Champion of the Great Frog!");
			ADD_MESSAGE("Elpuri is not a foe worthy for thee.");

			if(game::BoolQuestion("Dost thou wish to stay on duty for a while more and complete another quest for me?\" [Y/n]", 'y'))
			{
				iosystem::TextScreen("Champion of Law!\n\nReturn to the foul cave of Elpuri and seek out the Master Evil:\nOree the Pepsi Daemon King, who hast stolenth one of the most powerful of all of my artifacts:\nthe Holy Maakotka Shirt! Return with it and immortal glory shall be thine!");
				game::GetCurrentArea()->SendNewDrawRequest();
				game::TriggerQuestForMaakotkaShirt();
				StoryState = 2;
				return;
			}
		}

		iosystem::TextScreen("Thou hast slain Elpuri, and Perttu is happy!\n\nYou are victorious!");
		game::RemoveSaves();
		game::Quit();

		if(!game::GetWizardMode())
		{
			AddScoreEntry("defeated Elpuri and continued to further adventures", 2);
			highscore HScore;
			HScore.Draw();
		}
	}
	else
		if(!StoryState)
		{
			iosystem::TextScreen(	"Perttu raises his hand as a salutation, and talks:\n"
						"\"Fare thee well, adventurer! Should thou seek glory, I have a task for thee!\n"
						"An evil dark frog named Elpuri is pestering our fine city in many ways.\n"
						"Valpuri hast told that this vile beast can be found in a nearby cave.\n"
						"Slay it and bring me its head as proof. Return when thou hast succeeded.\"");

			game::GetCurrentArea()->SendNewDrawRequest();
			ADD_MESSAGE("Perttu hands you something. \"Thou migth need this.\"");
			Talker->GetStack()->AddItem(new banana);
			StoryState = 1;
		}
		else
			ADD_MESSAGE("Perttu says: \"Bring me the head of Elpuri and we'll talk.\"");
}

void farmer::BeTalkedTo(character* Talker)
{
	if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
	{
		ADD_MESSAGE("\"Did you think I use this axe only to chop wood?\"");
		return;
	}

	switch(rand() % 4)
	{
	case 0:
		ADD_MESSAGE("\"Crops are so lousy around here. Perhaps because summer lasts two weeks.\"");
		break;
	case 1:
		ADD_MESSAGE("%s seems suspicious. \"You look like one from Istour! Go away!\"", CNAME(DEFINITE));
		break;
	case 2:
		ADD_MESSAGE("%s sighs: \"Again polar bears ate my cattle...\"", CNAME(DEFINITE));
		break;
	case 3:
		ADD_MESSAGE("\"The prices here are infamous. What a capitalist scum that shopkeeper is!\"");
		break;
	}
}

void cityguard::BeTalkedTo(character* Talker)
{
	if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
	{
		ADD_MESSAGE("\"A fair trial? Hah! Prepare to be executed!\"");
		return;
	}

	switch(rand() % 4)
	{
	case 0:
		ADD_MESSAGE("%s says gravely: \"You don't have life. Get it in the army.\"", CNAME(DEFINITE));
		break;
	case 1:
		ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of breaking rules.\"", CNAME(DEFINITE));
		break;
	case 2:
		ADD_MESSAGE("%s shouts excited: \"Attnam victoor!\"", CNAME(DEFINITE));
		break;
	case 3:
		ADD_MESSAGE("\"The Überpriest is my idol. I would want a sword as big as his!\"");
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

	switch(rand() % 4)
	{
	case 0:
		ADD_MESSAGE("%s sighs: \"If only I hadn't chosen a city in the middle of nowhere...\"", CNAME(DEFINITE));
		break;
	case 1:
		ADD_MESSAGE("%s sighs: \"Mutant school food mushrooms ate the last caravan.", CNAME(DEFINITE));
		ADD_MESSAGE("The one before it ran into an Enner Beast. It must be all Elpuri's doings!\"");
		break;
	case 2:
		ADD_MESSAGE("\"You truly can't find better prices in this city!\", %s smiles.", CNAME(DEFINITE));
		ADD_MESSAGE("\"Indeed, you can't find ANY prices, since I have a monopoly.\"");
		break;
	case 3:
		ADD_MESSAGE("\"Don't try anything. The Überpriest is a LAN friend of mine.\"");
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
		ADD_MESSAGE("%s talks to you:", CNAME(DEFINITE));
		game::GetGod(GetLevelSquareUnder()->GetLevelUnder()->GetRoom(HomeRoom)->GetDivineOwner())->AddPriestMessage();
	}
}

void oree::BeTalkedTo(character*)
{
	ADD_MESSAGE("%s laughs: \"No time for small talk. Time to drink Pepsi!\"", CNAME(DEFINITE));
}

void swatcommando::BeTalkedTo(character*)
{
	ADD_MESSAGE("%s yells: \"Microsoft powaah! Die Unix!\"", CNAME(DEFINITE));
}

void ennerbeast::BeTalkedTo(character*)
{
	switch(rand() % 4)
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
	ADD_MESSAGE("%s roars horribly: \"DiE hUmAn!!\"", CNAME(DEFINITE));
}

void billswill::BeTalkedTo(character*)
{
	ADD_MESSAGE("\"Windows XP is coming. You will install it. Resistance is futile. Prepare to be assimilited.\"", CNAME(DEFINITE));
}

void fallenvalpurist::BeTalkedTo(character* Talker)
{
	if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
		ADD_MESSAGE("%s sings: \"Leg bone is connected to the hib bone, hib bone is connected to the rib bone...\"", CNAME(DEFINITE));
	else
		ADD_MESSAGE("%s grunts: \"Bones. Need more bones.\"", CNAME(DEFINITE));
}

void froggoblin::BeTalkedTo(character* Talker)
{
	if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
		ADD_MESSAGE("%s laughs: \"Humie friend. Many mommo we kill. Many spider we eat.\"", CNAME(DEFINITE));
	else
		ADD_MESSAGE("%s yells goblin war cries at you.", CNAME(DEFINITE));
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
