/* Compiled through charsset.cpp */

ushort humanoid::DrawOrder[] = { TORSO_INDEX, HEAD_INDEX, GROIN_INDEX, RIGHT_LEG_INDEX, LEFT_LEG_INDEX, RIGHT_ARM_INDEX, LEFT_ARM_INDEX };

bool humanoid::BodyPartIsVital(ushort Index) const { return Index == TORSO_INDEX || Index == HEAD_INDEX || Index == GROIN_INDEX; }
bool humanoid::BodyPartCanBeSevered(ushort Index) const { return Index != TORSO_INDEX && Index != GROIN_INDEX; }
uchar humanoid::OpenMultiplier() const { return GetRightArm() || GetLeftArm() ? 1 : 3; }
uchar humanoid::CloseMultiplier() const { return GetRightArm() || GetLeftArm() ? 1 : 2; }
ushort humanoid::GetCarryingStrength() const { return Max<ushort>(GetAttribute(LEG_STRENGTH), 1) + CarryingBonus; }
void humanoid::CalculateBodyParts() { BodyParts = HUMANOID_BODYPARTS; }
void humanoid::CalculateAllowedWeaponSkillCategories() { AllowedWeaponSkillCategories = WEAPON_SKILL_CATEGORIES; }
bool humanoid::HasFeet() const { return GetLeftLeg() || GetRightLeg(); }

vector2d farmer::GetHeadBitmapPos() const { return vector2d(96, (4 + (RAND() & 1)) << 4); }
vector2d farmer::GetRightArmBitmapPos() const { return vector2d(64, (RAND() & 1) << 4); }

void guard::SetWayPoints(const std::vector<vector2d>& What) { WayPoints = What; }

const char* oree::FirstPersonBiteVerb() const { return "vomit acidous blood at"; }
const char* oree::FirstPersonCriticalBiteVerb() const { return "vomit very acidous blood at"; }
const char* oree::ThirdPersonBiteVerb() const { return "vomits acidous blood at"; }
const char* oree::ThirdPersonCriticalBiteVerb() const { return "vomits very acidous blood at"; }
const char* oree::BiteNoun() const { return "liquid"; }

bool skeleton::BodyPartIsVital(ushort Index) const { return Index == GROIN_INDEX || Index == TORSO_INDEX; }

bool communist::ShowClassDescription() const { return GetAssignedName() != "Ivan"; }

vector2d housewife::GetHeadBitmapPos() const { return vector2d(112, (RAND() % 6) << 4); }

bool zombie::BodyPartIsVital(ushort Index) const { return Index == GROIN_INDEX || Index == TORSO_INDEX; }

bool angel::BodyPartIsVital(ushort Index) const { return Index == TORSO_INDEX || Index == HEAD_INDEX; }

bool genie::BodyPartIsVital(ushort Index) const { return Index == TORSO_INDEX || Index == HEAD_INDEX; }

material* golem::CreateBodyPartMaterial(ushort, ulong Volume) const { return MAKE_MATERIAL(GetConfig(), Volume); }

petrus::~petrus()
{
  game::SetPetrus(0);
}

bool ennerbeast::Hit(character*, vector2d, uchar, bool)
{
  if(RAND() & 1)
    ADD_MESSAGE("%s yells: UGH UGHAaaa!", CHAR_DESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s yells: Uga Ugar Ugade Ugat!", CHAR_DESCRIPTION(DEFINITE));

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), GetPos(), 30);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      {
	ushort ScreamStrength = ushort(70 / (hypot(GetPos().X - x, GetPos().Y - y) + 1));

	if(ScreamStrength)
	  {
	    character* Char = GetNearSquare(x, y)->GetCharacter();

	    if(Char && Char != this)
	      {
		msgsystem::EnterBigMessageMode();

		if(Char->IsPlayer())
		  ADD_MESSAGE("You are hit by the horrible waves of high sound.");
		else if(Char->CanBeSeenByPlayer())
		  ADD_MESSAGE("%s is hit by the horrible waves of high sound.", Char->CHAR_NAME(DEFINITE));

		Char->ReceiveDamage(this, ScreamStrength, SOUND, ALL, YOURSELF, true);
		festring DeathMsg = CONST_S("killed by ") + GetName(INDEFINITE) + "'s scream";
		Char->CheckDeath(DeathMsg, this);
		msgsystem::LeaveBigMessageMode();
	      }

	    GetNearLSquare(x, y)->GetStack()->ReceiveDamage(this, ScreamStrength, SOUND);

	    for(ushort c = 0; c < 4; ++c)
	      GetNearLSquare(x, y)->GetSideStack(c)->ReceiveDamage(this, ScreamStrength, SOUND);
	  }
      }

  EditNP(-100);
  EditAP(-100000 / GetCWeaponSkill(BITE)->GetBonus());
  return true;
}

void skeleton::CreateCorpse(lsquare* Square)
{
  if(GetHead())
    {
      item* Skull = SevereBodyPart(HEAD_INDEX);
      Square->AddItem(Skull);
    }

  ushort Amount = 2 + (RAND() & 3);

  for(ushort c = 0; c < Amount; ++c)
    Square->AddItem(new bone);

  SendToHell();
}

void petrus::CreateCorpse(lsquare* Square)
{
  if(game::GetStoryState() == 2)
    game::GetTeam(ATTNAM_TEAM)->SetRelation(game::GetTeam(PLAYER_TEAM), FRIEND);

  Square->AddItem(new leftnutofpetrus);
  SendToHell();
}

void humanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << SWeaponSkill;
}

void humanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> SWeaponSkill;

  if(GetRightWielded())
    for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
      if((*i)->IsSkillOf(GetRightWielded()))
	{
	  SetCurrentRightSWeaponSkill(*i);
	  break;
	}

  if(GetLeftWielded())
    for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
      if((*i)->IsSkillOf(GetLeftWielded()))
	{
	  SetCurrentLeftSWeaponSkill(*i);
	  break;
	}
}

bool golem::MoveRandomly()
{
  if(!(RAND() % 500))
    {
      Engrave("Golem Needs Master");
      EditAP(-1000);
      return true;
    }
  else
    return character::MoveRandomly();
}

void ennerbeast::GetAICommand()
{
  SeekLeader();

  if(StateIsActivated(PANIC) || !(RAND() % 3))
    Hit(0, vector2d(0, 0), YOURSELF);

  if(CheckForEnemies(false, false))
    return;

  if(FollowLeader())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void petrus::GetAICommand()
{
  if(HP << 1 < MaxHP && (GetPos() - vector2d(28, 20)).GetLengthSquare() > 25)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

      GetLevel()->GetLSquare(28, 20)->KickAnyoneStandingHereAway();
      Move(vector2d(28, 20), true);
      EditAP(-1000);
      return;
    }

  if(!LastHealed || game::GetTicks() - LastHealed > 16384)
    for(ushort d = 0; d < GetNeighbourSquares(); ++d)
      {
	square* Square = GetNeighbourSquare(d);

	if(Square)
	  {
	    character* Char = Square->GetCharacter();

	    if(Char && GetRelation(Char) == FRIEND && HealFully(Char))
	      return;
	  }
      }

  StandIdleAI();
}

bool petrus::HealFully(character* ToBeHealed)
{
  bool DidSomething = false;

  for(ushort c = 0; c < ToBeHealed->GetBodyParts(); ++c)
    if(!ToBeHealed->GetBodyPart(c))
      {
	bodypart* BodyPart = 0;

	for(std::list<ulong>::const_iterator i = ToBeHealed->GetOriginalBodyPartID(c).begin(); i != ToBeHealed->GetOriginalBodyPartID(c).end(); ++i)
	  {
	    BodyPart = static_cast<bodypart*>(ToBeHealed->SearchForItem(*i));

	    if(BodyPart)
	      break;
	  }

	if(!BodyPart || !BodyPart->CanRegenerate())
	  continue;

	BodyPart->RemoveFromSlot();
	ToBeHealed->AttachBodyPart(BodyPart);
	BodyPart->SetHP(BodyPart->GetMaxHP());
	DidSomething = true;

	if(ToBeHealed->IsPlayer())
	  ADD_MESSAGE("%s attaches your old %s back and heals it.", CHAR_NAME(DEFINITE), BodyPart->GetBodyPartName().CStr());
	else if(CanBeSeenByPlayer())
	  ADD_MESSAGE("%s attaches the old %s of %s back and heals it.", CHAR_NAME(DEFINITE), BodyPart->GetBodyPartName().CStr(), ToBeHealed->CHAR_DESCRIPTION(DEFINITE));
      }

  if(ToBeHealed->IsInBadCondition())
    {
      ToBeHealed->RestoreLivingHP();
      DidSomething = true;

      if(ToBeHealed->IsPlayer())
	ADD_MESSAGE("%s heals you fully.", CHAR_DESCRIPTION(DEFINITE));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s heals %s fully.", CHAR_DESCRIPTION(DEFINITE), ToBeHealed->CHAR_DESCRIPTION(DEFINITE));
    }

  if(ToBeHealed->TemporaryStateIsActivated(POISONED))
    {
      ToBeHealed->DeActivateTemporaryState(POISONED);
      DidSomething = true;

      if(ToBeHealed->IsPlayer())
	ADD_MESSAGE("%s removes the foul poison in your body.", CHAR_DESCRIPTION(DEFINITE));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s removes the foul poison in %s's body.", CHAR_DESCRIPTION(DEFINITE), ToBeHealed->CHAR_DESCRIPTION(DEFINITE));
    }

  if(DidSomething)
    {
      LastHealed = game::GetTicks();
      DexterityAction(5);
      return true;
    }
  else
    return false;
}

void petrus::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << LastHealed;
}

void petrus::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> LastHealed;
  game::SetPetrus(this);
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

bool humanoid::Hit(character* Enemy, vector2d HitPos, uchar Direction, bool ForceHit)
{
  if(IsPlayer() && GetRelation(Enemy) != HOSTILE && !game::BoolQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
    return false;

  if(!IsPlayer() && GetAttribute(WISDOM) >= Enemy->GetAttackWisdomLimit())
    return false;

  if(GetBurdenState() == OVER_LOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return false;
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
      if((GetRightArm() && GetRightArm()->GetDamage()) || (GetLeftArm() && GetLeftArm()->GetDamage()))
	{
	  msgsystem::EnterBigMessageMode();
	  Hostility(Enemy);
	  long FirstAPCost = 0, SecondAPCost = 0;
	  arm* FirstArm, * SecondArm;

	  if(RAND() & 1)
	    {
	      FirstArm = GetRightArm();
	      SecondArm = GetLeftArm();
	    }
	  else
	    {
	      FirstArm = GetLeftArm();
	      SecondArm = GetRightArm();
	    }
	  
	  if(FirstArm && FirstArm->GetDamage())
	    {
	      FirstAPCost = FirstArm->GetAPCost();
	      FirstArm->Hit(Enemy, HitPos, Direction, ForceHit);
	    }

	  if(!GetAction() && IsEnabled() && Enemy->IsEnabled() && SecondArm && SecondArm->GetDamage())
	    {
	      SecondAPCost = SecondArm->GetAPCost();
	      SecondArm->Hit(Enemy, HitPos, Direction, ForceHit);
	    } 

	  EditNP(-50);
	  EditAP(-Max(FirstAPCost, SecondAPCost));
	  msgsystem::LeaveBigMessageMode();
	  return true;
	}
    case USE_LEGS:
      if(GetRightLeg() && GetLeftLeg())
	{
	  msgsystem::EnterBigMessageMode();
	  Hostility(Enemy);
	  Kick(GetNearLSquare(HitPos), Direction, ForceHit);
	  msgsystem::LeaveBigMessageMode();
	  return true;
	}
    case USE_HEAD:
      if(GetHead())
	{
	  msgsystem::EnterBigMessageMode();
	  Hostility(Enemy);
          Bite(Enemy, HitPos, Direction, ForceHit);
	  msgsystem::LeaveBigMessageMode();
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

  if(CurrentRightSWeaponSkill && CurrentRightSWeaponSkill->GetHits())
    {
      List.AddEntry(CONST_S(""), LIGHT_GRAY);
      festring Buffer = CONST_S("right accustomization");
      Buffer.Resize(30);
      Buffer << CurrentRightSWeaponSkill->GetLevel();
      Buffer.Resize(40);
      Buffer << CurrentRightSWeaponSkill->GetHits();
      Buffer.Resize(50);

      if(CurrentRightSWeaponSkill->GetLevel() != 10)
	Buffer << (CurrentRightSWeaponSkill->GetLevelMap(CurrentRightSWeaponSkill->GetLevel() + 1) - CurrentRightSWeaponSkill->GetHits());
      else
	Buffer << '-';

      Buffer.Resize(60);
      Buffer << '+' << int(CurrentRightSWeaponSkill->GetBonus() - 100) << '%';
      List.AddEntry(Buffer, LIGHT_GRAY);
      Something = true;
    }

  if(CurrentLeftSWeaponSkill && CurrentLeftSWeaponSkill->GetHits())
    {
      if(!Something)
	List.AddEntry(CONST_S(""), LIGHT_GRAY);

      festring Buffer = CONST_S("left accustomization");
      Buffer.Resize(30);
      Buffer << CurrentLeftSWeaponSkill->GetLevel();
      Buffer.Resize(40);
      Buffer << CurrentLeftSWeaponSkill->GetHits();
      Buffer.Resize(50);

      if(CurrentLeftSWeaponSkill->GetLevel() != 10)
	Buffer << (CurrentLeftSWeaponSkill->GetLevelMap(CurrentLeftSWeaponSkill->GetLevel() + 1) - CurrentLeftSWeaponSkill->GetHits());
      else
	Buffer << '-';

      Buffer.Resize(60);
      Buffer << '+' << int(CurrentLeftSWeaponSkill->GetBonus() - 100) << '%';
      List.AddEntry(Buffer, LIGHT_GRAY);
      Something = true;
    }

  return Something;
}

void petrus::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("Heretic! Dev/null is a place not worthy to receive thee!");
      return;
    }

  if(PLAYER->HasGoldenEagleShirt())
    {
      ADD_MESSAGE("Petrus smiles. \"Thou hast defeated Oree! Mayst thou be blessed by Valpurus for the rest of thy life! And thou possess the Shirt of the Golden Eagle, the symbol of Our status! Return it now, please.\"");

      if(game::BoolQuestion(CONST_S("Will you give the Shirt of the Golden Eagle to Petrus? [y/n]"), REQUIRES_ANSWER))
	{
	  game::TextScreen(CONST_S( "The Holy Shirt is returned to its old owner and you kneel down to receive your reward.\n"
				    "Petrus taps your shoulder with the Justifier and raises you to nobility. Later you\n"
				    "receive a small dukedom in the middle of tundra where you rule with justice till\n"
				    "the end of your content life.\n\nYou are victorious!"));

	  game::GetCurrentArea()->SendNewDrawRequest();
	  game::DrawEverything();
	  PLAYER->ShowAdventureInfo();
	  AddScoreEntry(CONST_S("retrieved the Shirt of the Golden Eagle and was raised to nobility"), 4, false);
	  game::End();
	  return;
	}
      else
	{
	  ADD_MESSAGE("Petrus's face turns red. \"I see. Thy greed hath overcome thy wisdom. Then, we shall fight for the shiny shirt. May Valpurus bless him who is better.\"");

	  /* And now we actually make his face change color ;-) */

	  GetHead()->GetMainMaterial()->SetSkinColor(MakeRGB16(255, 75, 50));
	  GetHead()->UpdatePictures();
	  SendNewDrawRequest();
	  game::AskForKeyPress(CONST_S("You are attacked! [press any key to continue]"));
	  PLAYER->GetTeam()->Hostility(GetTeam());
	  game::SetStoryState(2);
	  return;
	}
    }

  if(PLAYER->HasHeadOfElpuri())
    {
      game::TextScreen(CONST_S(	"You have slain Elpuri, and Petrus grants you the freedom you desire.\n"
				"You spend the next months in Attnam as an honored hero and when the\n"
				"sea finally melts, you board the first ship, leaving your past forever\n"
				"behind.\n\nYou are victorious!"));

      game::GetCurrentArea()->SendNewDrawRequest();
      game::DrawEverything();
      PLAYER->ShowAdventureInfo();
      AddScoreEntry(CONST_S("defeated Elpuri and continued to further adventures"), 2, false);
      game::End();
    }
  else
    {
      if(!game::GetStoryState())
	{
	  if(PLAYER->RemoveEncryptedScroll())
	    {
	      game::TextScreen(CONST_S(	"You kneel down and bow before the high priest and hand him the encrypted scroll.\n"
					"Petrus raises his arm, the scroll glows yellow, and lo! The letters are clear and\n"
					"readable. Petrus asks you to voice them aloud. The first two thousand words praise\n"
					"Valpurus the Creator and all His manifestations and are followed by a canticle of\n"
					"Saint Petrus the Lion-Hearted lasting roughly three thousand words. Finally there\n"
					"are some sentences actually concerning your mission:\n\n"
					"\"Alas, I fear dirty tongues have spread lies to my Lord's ears. I assure all tales\n"
					"of treasures here in New Attnam are but mythic legends. There is nothing of value here.\n"
					"The taxes are already an unbearable burden and I can't possibly pay more. However I do\n"
					"not question the wisdom of the government's decisions. I will contribute what I can:\n"
					"the ostriches will deliver an extra 10000 bananas to the capital and additionally the\n"
					"slave that brought the message will henceforth be at Your disposal. I am certain this\n"
					"satisfies the crown's needs.\"\n\n"
					"\"Yours sincerely,\n"
					"Richel Decos, the viceroy of New Attnam\""));

	      game::TextScreen(CONST_S(	"You almost expected the last bit. Petrus seems to be deep in his thoughts and you\n"
					"wonder what shape your destiny is taking in his mind. Suddenly he seems to return\n"
					"to this reality and talks to you.\n\n"
					"\"Oh, thou art still here. We were just discussing telepathically with Sir Galladon.\n"
					"We started doubting Decos's alleged poverty a while back when he bought a couple of\n"
					"medium-sized castles nearby. Thy brethren from New Attnam have also told Us about\n"
					"vast riches seized from them. Our law says all such stolen valuables belong to \n"
					"the Cathedral's treasury, so this is a severe claim. However, proof is needed,\n"
					"and even if such was provided, We couldn't send soldiers over the snow fields\n"
					"ere spring.\""));

	      game::TextScreen(CONST_S(	"\"However, since thou now servest Us, We ought to find thee something to do. Sir\n"
					"Galladon hath told Us his agents witnessed thou leaving the dreaded underwater tunnel.\n"
					"This means thou most likely hast defeated genetrix vesana and art a talented warrior.\n"
					"We happen to have a task perfect for such a person. An evil dark frog named Elpuri who\n"
					"hates Valpurus and Attnam more than anything hath taken control over an abandoned mine\n"
					"nearby. It is pestering our fine city in many ways and reconnaissance has reported an\n"
					"army of monsters gathering in the cave. Our guards are not trained to fight underground\n"
					"and We dare not send them. To make things worse, someone hath recently stolen Us the\n"
					"greatest armor in existence - the Shirt of the Golden Eagle. Elpuri cannot wear\n"
					"it but he who can is now nearly immortal.\"\n\n"
					"\"We have marked the location of the gloomy cave on thy world map. We want you to dive\n"
					"into it and slay the vile frog. Bring Us its head and We reward thee with freedom.\n"
					"Shouldst thou also find the Shirt, We'll knight thee. Good luck, and return when\n"
					"thou hast succeeded.\""));

	      game::LoadWorldMap();
	      vector2d ElpuriCavePos = game::GetWorldMap()->GetEntryPos(0, ELPURI_CAVE);
	      game::GetWorldMap()->GetWSquare(ElpuriCavePos)->ChangeOWTerrain(new elpuricave);
	      game::GetWorldMap()->RevealEnvironment(ElpuriCavePos, 1);
	      game::SaveWorldMap();
	      GetArea()->SendNewDrawRequest();
	      ADD_MESSAGE("\"And by the way, visit the librarian. He might have advice for thee.\"");
	      game::SetStoryState(1);
	    }
	  else
	    ADD_MESSAGE("\"Yes, citizen? We are quite busy now, thou shalt not disturb Us without proper cause.\"");
	}
      else /* StoryState == 1 */
	ADD_MESSAGE("Petrus says: \"Bring me the head of Elpuri and we'll talk.\"");
    }
}

void priest::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("\"Sinner! My hands shall pour Dinive Wrath upon thee!\"");
      return;
    }

  if(PLAYER->TemporaryStateIsActivated(POISONED))
    {
      if(PLAYER->GetMoney() >= 25)
	{
	  ADD_MESSAGE("\"You seem to be rather ill. I could give you a small dose of antidote for 25 gold pieces.\"");

	  if(game::BoolQuestion(CONST_S("Do you agree? [y/N]")))
	    {
	      ADD_MESSAGE("You feel better.");
	      PLAYER->DeActivateTemporaryState(POISONED);
	      PLAYER->SetMoney(PLAYER->GetMoney() - 25);
	      SetMoney(GetMoney() + 25);
	      return;
	    }
	}
      else
	ADD_MESSAGE("\"You seem to be rather ill. Get 25 gold pieces and I'll fix that.\"");
    }

  for(ushort c = 0; c < PLAYER->GetBodyParts(); ++c)
    if(!PLAYER->GetBodyPart(c) && PLAYER->CanCreateBodyPart(c))
      {
	bool HasOld = false;

	for(std::list<ulong>::const_iterator i = PLAYER->GetOriginalBodyPartID(c).begin(); i != PLAYER->GetOriginalBodyPartID(c).end(); ++i)
	  {
	    bodypart* OldBodyPart = static_cast<bodypart*>(PLAYER->SearchForItem(*i));

	    if(OldBodyPart)
	      {
		HasOld = true;

		if(PLAYER->GetMoney() >= PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR)
		  {
		    if(!OldBodyPart->CanRegenerate())
		      ADD_MESSAGE("\"Sorry, I cannot put back bodyparts made of %s, not even your severed %s.\"", OldBodyPart->GetMainMaterial()->GetName(false, false).CStr(), PLAYER->GetBodyPartName(c).CStr());
		    else
		      {
			ADD_MESSAGE("\"I could put your old %s back in exchange for %d gold.\"", PLAYER->GetBodyPartName(c).CStr(), PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR);

			if(game::BoolQuestion(CONST_S("Do you agree? [y/N]")))
			  {
			    OldBodyPart->SetHP(1);
			    PLAYER->SetMoney(PLAYER->GetMoney() - PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR);
			    SetMoney(GetMoney() + PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR);
			    OldBodyPart->RemoveFromSlot();
			    PLAYER->AttachBodyPart(OldBodyPart);
			    return;
			  }
		      }
		  }
		else
		  ADD_MESSAGE("\"You %s is severed. Help yourself and get %dgp and I'll help you too.\"", PLAYER->GetBodyPartName(c).CStr(), PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR);
	      }
	  }

	if(PLAYER->GetMoney() >= PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR)
	  {
	    if(HasOld)
	      ADD_MESSAGE("\"I could still summon up a new one for %d gold.\"", PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);
	    else
	      ADD_MESSAGE("\"Since you don't seem to have your original %s with you, I could summon up a new one for %d gold.\"", PLAYER->GetBodyPartName(c).CStr(), PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);

	    if(game::BoolQuestion(CONST_S("Agreed? [y/N]")))
	      {
		PLAYER->SetMoney(PLAYER->GetMoney() - PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);
		SetMoney(GetMoney() + PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);
		PLAYER->CreateBodyPart(c);
		PLAYER->GetBodyPart(c)->SetHP(1);
		return;
	      }
	  }
	else if(!HasOld)
	  ADD_MESSAGE("\"You don't have your orginal %s with you. I could create you a new one, but my Divine Employer is not a communist and you need %dgp first.\"", PLAYER->GetBodyPartName(c).CStr(), PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);
      }

  if(!GetHomeRoom())
    ADD_MESSAGE("\"Receive my blessings, child.\"");
  else
    ADD_MESSAGE("%s talks to you: \"Valpurus the Great Frog is the highest of all gods. The Wise know that the world is really a square pancake which He carries on His back. This is why this Cathedral and the whole city of Attnam is dedicated to His worship.\" \"In thy prayers thou must understand He is a busy god who knows His importance. He will not help newbies. Thou shouldst only pray Him when He hath called thee a Champion!\"", CHAR_DESCRIPTION(DEFINITE));
}

void skeleton::BeTalkedTo()
{
  if(GetHead())
    character::BeTalkedTo();
  else
    ADD_MESSAGE("The headless %s remains silent.", PLAYER->CHAR_DESCRIPTION(UNARTICLED));
}

void communist::BeTalkedTo()
{
  if(GetRelation(PLAYER) != HOSTILE && GetTeam() != PLAYER->GetTeam() && PLAYER->GetRelativeDanger(this, true) > 0.5f)
    {
      ADD_MESSAGE("%s seems to be very friendly. \"%s make good communist. %s go with %s!\"", CHAR_DESCRIPTION(DEFINITE), PLAYER->GetAssignedName().CStr(), CHAR_NAME(UNARTICLED), PLAYER->GetAssignedName().CStr());

      for(std::list<character*>::const_iterator i = GetTeam()->GetMember().begin(); i != GetTeam()->GetMember().end();)
	if(*i != this)
	  {
	    character* Char = *i++;
	    Char->ChangeTeam(PLAYER->GetTeam());
	  }
	else
	  ++i;

      ChangeTeam(PLAYER->GetTeam());
    }
  else if(GetTeam() != PLAYER->GetTeam() && !(RAND() % 5))
    ADD_MESSAGE("You weak. Learn killing and come back.");
  else
    character::BeTalkedTo();
}

void hunter::BeTalkedTo()
{
  if(GetRelation(PLAYER) != HOSTILE && GetMainWielded() && !(RAND() % 10))
    ADD_MESSAGE("\"This is my %s. There are many like it but this one is mine. My %s is my best friend.\"", GetMainWielded()->CHAR_NAME(UNARTICLED), GetMainWielded()->CHAR_NAME(UNARTICLED));
  else
    character::BeTalkedTo();
}

void slave::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("\"Yikes!\"");
      return;
    }

  room* Room = GetHomeRoom();

  if(Room && Room->MasterIsActive())
    {
      character* Master = Room->GetMaster();

      if(PLAYER->GetMoney() >= 50)
	{
	  ADD_MESSAGE("%s talks: \"Do you want to buy me? 50 gold pieces. I work very hard.\"", CHAR_DESCRIPTION(DEFINITE));

	  if(game::BoolQuestion(CONST_S("Do you want to buy him? [y/N]")))
	    {
	      PLAYER->SetMoney(PLAYER->GetMoney() - 50);
	      Master->SetMoney(Master->GetMoney() + 50);
	      ChangeTeam(PLAYER->GetTeam());
	      RemoveHomeData();
	    }
	}
      else
	ADD_MESSAGE("\"Don't touch me! Master doesn't want people to touch sale items. I'm worth 50 gold pieces, you know!\"");

      return;
    }

  if(GetTeam() == PLAYER->GetTeam())
    {
      if((PLAYER->GetMainWielded() && PLAYER->GetMainWielded()->IsWhip()) || (PLAYER->GetSecondaryWielded() && PLAYER->GetSecondaryWielded()->IsWhip()))
	ADD_MESSAGE("\"Don't hit me! I work! I obey! I don't think!\"");
      else
	character::BeTalkedTo();
    }
  else
    ADD_MESSAGE("\"I'm free! Rejoice!\"");
}

void slave::GetAICommand()
{
  SeekLeader();

  if(CheckForEnemies(true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(CheckForDoors())
    return;

  if(!GetHomeRoom() || !GetHomeRoom()->MasterIsActive())
    {
      RemoveHomeData();

      if(MoveRandomly())
	return;
    }
  else if(MoveTowardsHomePos())
    return;

  EditAP(-1000);
}

void librarian::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("\"The pen is mightier than the sword! Fall, unlearned one!\"");
      return;
    }

  static ulong Said;

  switch(RandomizeReply(Said, 12))
    {
    case 0:
      if(game::GetPetrus() && !game::GetStoryState())
	ADD_MESSAGE("\"Thou shouldst visit Petrus if thou art in need of further adventures.\"");
      else
	ADD_MESSAGE("\"They say a wand of polymorph hath dozens of uses.\"");

      break;
    case 1:
      if(game::GetPetrus() && game::GetStoryState() == 1)
	ADD_MESSAGE("\"Thou art going to fight Elpuri? Beware! It is a powerful enemy. Other monsters are very vulnerable if surrounded by thy party, but not that beast, for it may slay a horde of thy friends at once with its horrendous tail attack.\"");
      else
	ADD_MESSAGE("\"Thou shalt remember: Scientia est potentia.\"");

      break;
    case 2:
      if(game::GetPetrus() && game::GetStoryState() == 1)
	ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");
      else
	ADD_MESSAGE("\"Shh! Thou shalt be silent in the library.\"");

      break;
    case 3:
      if(game::GetPetrus() && game::GetStoryState() == 1)
	ADD_MESSAGE("\"Elpuri's attacks are so strong that they may shatter many of thy precious items.\"");
      else
	ADD_MESSAGE("\"Dost thou not smell all the knowledge floating around here?\"");

      break;
    case 4:
      ADD_MESSAGE("\"It is said that Loricatus, the god of smithing, can upgrade thy weapons' materials.\"");
      break;
    case 5:
      if(game::GetPetrus() && game::GetStoryState() == 1)
	ADD_MESSAGE("\"The Shirt of the Golden Eagle is a legendary artifact. Thou canst not find a better armor.\"");
      else
	ADD_MESSAGE("\"In this book they talk about Mortifer, the great chaos god. He hates us mortals more than anything and will respond only to Champions of Evil.\"");

      break;
    case 6:
      ADD_MESSAGE("\"Attnam is traditionally ruled by the high priest of the Great Frog. He holds the Shirt of the Golden Eagle and has always killed his predecessor.\"");
      break;
    case 7:
      ADD_MESSAGE("\"They say thou shouldst keep all the artifacts thou findst. They shall make thee famous after thy retirement.\"");
      break;
    case 8:
      ADD_MESSAGE("\"If thou wilt ever encounter an enner beast, know this: It is a horrible foe. It may shatter thy items and armor with its scream that penetrates iron and stone. Thou shouldst not engage it in melee but rather kill it from afar.\"");
      break;
    case 9:
      if(game::GetPetrus() && game::GetStoryState() == 1)
	ADD_MESSAGE("\"Thou art not alone in thy attempt to defeat Elpuri. A brave adventurer called Ivan also diveth into its cave not long ago.\"");
      else
	ADD_MESSAGE("\"It is said that chaotic gods offer great power to their followers. But thou must remember: unlike lawfuls, they shall not help thee when things go bad.\"");

      break;
    case 10:
      ADD_MESSAGE("\"If a man cannot choose, he ceases to be a man.\"");
      break;
    case 11:
      ADD_MESSAGE("%s sighs: \"The censorship laws in this town are really too strict...\"", CHAR_DESCRIPTION(DEFINITE));
      break;
    }
}

bool communist::MoveRandomly()
{
  switch(RAND() % 1000)
    {
    case 0:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

      Engrave("The bourgeois is a bourgeois -- for the benefit of the working class.");
      return true;
    case 1:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

      Engrave("Proletarians of all countries, unite!");
      return true;
    case 2:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

      Engrave("Capital is therefore not only personal; it is a social power.");
      return true;
    default:
      return character::MoveRandomly();
    }
}

void zombie::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE && PLAYER->GetAttribute(INTELLIGENCE) > 5)
    {
      if(RAND() % 5)
	{
	  if(GetHead())
	    ADD_MESSAGE("\"Need brain!!\"");
	  else
	    ADD_MESSAGE("\"Need head with brain!!\"");
	}
      else
	ADD_MESSAGE("\"Redrum! Redrum! Redrum!\"");
    }
  else
    ADD_MESSAGE("\"Need brain, but not your brain.\"");
}

void angel::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << LastHealed;
}

void angel::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> LastHealed;
}

void angel::CreateInitialEquipment(ushort SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  GetStack()->AddItem(new holybook(GetConfig(), SpecialFlags));
  armor* Equipment;
  meleeweapon* Weapon;

  switch(GetMasterGod()->GetBasicAlignment())
    {
    case GOOD:
      Equipment = new bodyarmor(PLATE_MAIL, SpecialFlags|NO_MATERIALS);
      Equipment->InitMaterials(MAKE_MATERIAL(ANGEL_HAIR), !(SpecialFlags & NO_PIC_UPDATE));
      Equipment->SetEnchantment(1);
      SetBodyArmor(Equipment);
      Weapon = new meleeweapon(LONG_SWORD, SpecialFlags|NO_MATERIALS);
      Weapon->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(MITHRIL), !(SpecialFlags & NO_PIC_UPDATE));
      Weapon->SetEnchantment(2);
      SetRightWielded(Weapon);
      Equipment = new shield(0, SpecialFlags|NO_MATERIALS);
      Equipment->InitMaterials(MAKE_MATERIAL(MITHRIL), !(SpecialFlags & NO_PIC_UPDATE));
      Equipment->SetEnchantment(2);
      SetLeftWielded(Equipment);
      GetCWeaponSkill(LARGE_SWORDS)->AddHit(200);
      GetCWeaponSkill(SHIELDS)->AddHit(500);
      GetCurrentRightSWeaponSkill()->AddHit(200);
      GetCurrentLeftSWeaponSkill()->AddHit(200);
      GetRightArm()->SetDexterity(40);
      GetLeftArm()->SetDexterity(40);
      break;
    case NEUTRAL:
      Equipment = new cloak(0, SpecialFlags|NO_MATERIALS);
      Equipment->InitMaterials(MAKE_MATERIAL(ANGEL_HAIR), !(SpecialFlags & NO_PIC_UPDATE));
      Equipment->SetEnchantment(1);
      SetCloak(Equipment);
      Weapon = new meleeweapon(WAR_HAMMER, SpecialFlags|NO_MATERIALS);
      Weapon->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(TEAK_WOOD), !(SpecialFlags & NO_PIC_UPDATE));
      Weapon->SetEnchantment(2);
      SetRightWielded(Weapon);
      Weapon = new meleeweapon(WAR_HAMMER, SpecialFlags|NO_MATERIALS);
      Weapon->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(TEAK_WOOD), !(SpecialFlags & NO_PIC_UPDATE));
      Weapon->SetEnchantment(2);
      SetLeftWielded(Weapon);
      GetCWeaponSkill(BLUNT_WEAPONS)->AddHit(500);
      GetCurrentRightSWeaponSkill()->AddHit(200);
      GetCurrentLeftSWeaponSkill()->AddHit(200);
      SetEndurance(40);
      break;
    case EVIL:
      Weapon = new meleeweapon(HALBERD, SpecialFlags|NO_MATERIALS);
      Weapon->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(EBONY_WOOD), !(SpecialFlags & NO_PIC_UPDATE));
      Weapon->SetEnchantment(2);
      SetRightWielded(Weapon);
      Equipment = new gauntlet(0, SpecialFlags|NO_MATERIALS);
      Equipment->InitMaterials(MAKE_MATERIAL(ANGEL_HAIR), !(SpecialFlags & NO_PIC_UPDATE));
      Equipment->SetEnchantment(1);
      SetRightGauntlet(Equipment);
      Equipment = new gauntlet(0, SpecialFlags|NO_MATERIALS);
      Equipment->InitMaterials(MAKE_MATERIAL(ANGEL_HAIR), !(SpecialFlags & NO_PIC_UPDATE));
      Equipment->SetEnchantment(1);
      SetLeftGauntlet(Equipment);
      GetCWeaponSkill(POLE_ARMS)->AddHit(1000);
      GetCurrentRightSWeaponSkill()->AddHit(1000);
      GetRightArm()->SetStrength(40);
      GetLeftArm()->SetStrength(40);
    }
}

void kamikazedwarf::CreateInitialEquipment(ushort SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  SetRightWielded(new holybook(GetConfig(), SpecialFlags));
  GetCWeaponSkill(UNCATEGORIZED)->AddHit(100);
  GetCurrentRightSWeaponSkill()->AddHit(100);
}

bool kamikazedwarf::Hit(character* Enemy, vector2d HitPos, uchar Direction, bool ForceHit)
{
  if(!IsPlayer())
    for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
      if(i->IsExplosive())
	{
	  if(RAND() & 1)
	    ADD_MESSAGE("%s shouts: \"For %s!\"", CHAR_DESCRIPTION(DEFINITE), GetMasterGod()->GetName());
	  else
	    ADD_MESSAGE("%s screams: \"%s, here I come!\"", CHAR_DESCRIPTION(DEFINITE), GetMasterGod()->GetName());

	  if(i->Apply(this))
	    return true;
	}

  return humanoid::Hit(Enemy, HitPos, Direction, ForceHit);
}

void kamikazedwarf::GetAICommand()
{
  if(GetHomeRoom())
    StandIdleAI();
  else
    character::GetAICommand();
}

ushort humanoid::GetSize() const
{
  ushort Size = 0;

  if(GetHead())
    Size += GetHead()->GetSize();

  if(GetTorso())
    Size += GetTorso()->GetSize();

  rightleg* RightLeg = GetRightLeg();
  leftleg* LeftLeg = GetLeftLeg();

  if(LeftLeg && RightLeg)
    Size += Max(LeftLeg->GetSize(), RightLeg->GetSize());
  else if(LeftLeg)
    Size += LeftLeg->GetSize();
  else if(RightLeg)
    Size += RightLeg->GetSize();

  return Size;
}

ulong humanoid::GetBodyPartSize(ushort Index, ushort TotalSize) const
{
  switch(Index)
    {
    case HEAD_INDEX: return 20;
    case TORSO_INDEX: return ((TotalSize - 20) << 1) / 5;
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return (TotalSize - 20) * 3 / 5;
    case GROIN_INDEX: return (TotalSize - 20) / 3;
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return (TotalSize - 20) * 3 / 5;
    }

  ABORT("Illegal humanoid bodypart size request!");
  return 0;
}

ulong humanoid::GetBodyPartVolume(ushort Index) const
{
  switch(Index)
    {
    case HEAD_INDEX: return 4000;
    case TORSO_INDEX: return (GetTotalVolume() - 4000) * 13 / 30;
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return (GetTotalVolume() - 4000) / 10;
    case GROIN_INDEX: return (GetTotalVolume() - 4000) / 10;
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return ((GetTotalVolume() - 4000) << 1) / 15;
    }

  ABORT("Illegal humanoid bodypart volume request!");
  return 0;
}

bodypart* humanoid::MakeBodyPart(ushort Index) const
{
  switch(Index)
    {
    case TORSO_INDEX: return new humanoidtorso(0, NO_MATERIALS);
    case HEAD_INDEX: return new head(0, NO_MATERIALS);
    case RIGHT_ARM_INDEX: return new rightarm(0, NO_MATERIALS);
    case LEFT_ARM_INDEX: return new leftarm(0, NO_MATERIALS);
    case GROIN_INDEX: return new groin(0, NO_MATERIALS);
    case RIGHT_LEG_INDEX: return new rightleg(0, NO_MATERIALS);
    case LEFT_LEG_INDEX: return new leftleg(0, NO_MATERIALS);
    }

  ABORT("Weird bodypart to make for a humanoid. It must be your fault!");
  return 0;
}

bool humanoid::ReceiveDamage(character* Damager, ushort Damage, ushort Type, uchar TargetFlags, uchar Direction, bool Divide, bool PenetrateArmor, bool Critical, bool ShowMsg)
{
  uchar ChooseFrom[MAX_BODYPARTS];
  ushort BodyParts = 0;

  if(TargetFlags & RIGHT_ARM && GetRightArm())
    ChooseFrom[BodyParts++] = 2;

  if(TargetFlags & LEFT_ARM && GetLeftArm())
    ChooseFrom[BodyParts++] = 3;

  if(TargetFlags & RIGHT_LEG && GetRightLeg())
    ChooseFrom[BodyParts++] = 5;

  if(TargetFlags & LEFT_LEG && GetLeftLeg())
    ChooseFrom[BodyParts++] = 6;

  if(TargetFlags & HEAD && GetHead())
    ChooseFrom[BodyParts++] = 1;

  if(TargetFlags & TORSO && GetTorso())
    ChooseFrom[BodyParts++] = 0;

  if(TargetFlags & GROIN && GetGroin())
    ChooseFrom[BodyParts++] = 4;

  if(!BodyParts)
    return false;

  bool Affected = false;

  if(Divide)
    {
      ushort c;
      ulong TotalVolume = 0;

      for(c = 0; c < BodyParts; ++c)
	TotalVolume += GetBodyPart(ChooseFrom[c])->GetBodyPartVolume();

      for(c = 0; c < BodyParts; ++c)
	if(ReceiveBodyPartDamage(Damager, ulong(Damage) * GetBodyPart(ChooseFrom[c])->GetBodyPartVolume() / TotalVolume, Type, ChooseFrom[c], Direction, PenetrateArmor, Critical, false))
	  Affected = true;
    }
  else
    {
      long Possibility[MAX_BODYPARTS];
      ushort Possibilities = 0;

      for(ushort c = 0; c < BodyParts; ++c)
	Possibility[Possibilities++] = GetBodyPart(ChooseFrom[c])->GetBodyPartVolume();

      Affected = ReceiveBodyPartDamage(Damager, Damage, Type, ChooseFrom[femath::WeightedRand(Possibility, Possibilities)], Direction, PenetrateArmor, Critical, false) != 0;
    }

  if(!Affected && ShowMsg)
    {
      if(IsPlayer())
	ADD_MESSAGE("You are not hurt.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s is not hurt.", GetPersonalPronoun().CStr());
    }

  if(DamageTypeAffectsInventory(Type))
    {
      for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	{
	  item* Equipment = GetEquipment(c);

	  if(Equipment)
	    Equipment->ReceiveDamage(Damager, Damage, Type);
	}

      GetStack()->ReceiveDamage(Damager, Damage, Type);
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

const char* humanoid::GetEquipmentName(ushort Index) const // convert to array
{
  switch(Index)
    {
    case HELMET_INDEX: return "helmet";
    case AMULET_INDEX: return "amulet";
    case CLOAK_INDEX: return "cloak";
    case BODY_ARMOR_INDEX: return "body armor";
    case BELT_INDEX: return "belt";
    case RIGHT_WIELDED_INDEX: return "right hand wielded";
    case LEFT_WIELDED_INDEX: return "left hand wielded";
    case RIGHT_RING_INDEX: return "right ring";
    case LEFT_RING_INDEX: return "left ring";
    case RIGHT_GAUNTLET_INDEX: return "right gauntlet";
    case LEFT_GAUNTLET_INDEX: return "left gauntlet";
    case RIGHT_BOOT_INDEX: return "right boot";
    case LEFT_BOOT_INDEX: return "left boot";
    }

  return "forbidden piece of cloth";
}

bool (*humanoid::EquipmentSorter(ushort Index) const)(const item*, const character*)
{
  switch(Index)
    {
    case HELMET_INDEX: return &item::HelmetSorter;
    case AMULET_INDEX: return &item::AmuletSorter;
    case CLOAK_INDEX: return &item::CloakSorter;
    case BODY_ARMOR_INDEX: return &item::BodyArmorSorter;
    case BELT_INDEX: return &item::BeltSorter;
    case RIGHT_WIELDED_INDEX:
    case LEFT_WIELDED_INDEX: return 0;
    case RIGHT_RING_INDEX:
    case LEFT_RING_INDEX: return &item::RingSorter;
    case RIGHT_GAUNTLET_INDEX:
    case LEFT_GAUNTLET_INDEX: return &item::GauntletSorter;
    case RIGHT_BOOT_INDEX:
    case LEFT_BOOT_INDEX: return &item::BootSorter;
    }

  return 0;
}

bodypart* humanoid::GetBodyPartOfEquipment(ushort Index) const
{
  switch(Index)
    {
    case HELMET_INDEX:
    case AMULET_INDEX:
      return GetHead();
    case CLOAK_INDEX:
    case BODY_ARMOR_INDEX:
    case BELT_INDEX:
      return GetTorso();
    case RIGHT_WIELDED_INDEX:
    case RIGHT_RING_INDEX:
    case RIGHT_GAUNTLET_INDEX:
      return GetRightArm();
    case LEFT_WIELDED_INDEX:
    case LEFT_RING_INDEX:
    case LEFT_GAUNTLET_INDEX:
      return GetLeftArm();
    case RIGHT_BOOT_INDEX:
      return GetRightLeg();
    case LEFT_BOOT_INDEX:
      return GetLeftLeg();
    }

  return 0;
}

item* humanoid::GetEquipment(ushort Index) const
{
  switch(Index)
    {
    case HELMET_INDEX: return GetHelmet();
    case AMULET_INDEX: return GetAmulet();
    case CLOAK_INDEX: return GetCloak();
    case BODY_ARMOR_INDEX: return GetBodyArmor();
    case BELT_INDEX: return GetBelt();
    case RIGHT_WIELDED_INDEX: return GetRightWielded();
    case LEFT_WIELDED_INDEX: return GetLeftWielded();
    case RIGHT_RING_INDEX: return GetRightRing();
    case LEFT_RING_INDEX: return GetLeftRing();
    case RIGHT_GAUNTLET_INDEX: return GetRightGauntlet();
    case LEFT_GAUNTLET_INDEX: return GetLeftGauntlet();
    case RIGHT_BOOT_INDEX: return GetRightBoot();
    case LEFT_BOOT_INDEX: return GetLeftBoot();
    }

  return 0;
}

void humanoid::SetEquipment(ushort Index, item* What)
{
  switch(Index)
    {
    case HELMET_INDEX: SetHelmet(What); break;
    case AMULET_INDEX: SetAmulet(What); break;
    case CLOAK_INDEX: SetCloak(What); break;
    case BODY_ARMOR_INDEX: SetBodyArmor(What); break;
    case BELT_INDEX: SetBelt(What); break;
    case RIGHT_WIELDED_INDEX: SetRightWielded(What); break;
    case LEFT_WIELDED_INDEX: SetLeftWielded(What); break;
    case RIGHT_RING_INDEX: SetRightRing(What); break;
    case LEFT_RING_INDEX: SetLeftRing(What); break;
    case RIGHT_GAUNTLET_INDEX: SetRightGauntlet(What); break;
    case LEFT_GAUNTLET_INDEX: SetLeftGauntlet(What); break;
    case RIGHT_BOOT_INDEX: SetRightBoot(What); break;
    case LEFT_BOOT_INDEX: SetLeftBoot(What); break;
    }
}

void humanoid::SwitchToDig(item* DigItem, vector2d Square)
{
  dig* Dig = new dig(this);

  if(GetRightArm())
    {
      item* Item = GetRightArm()->GetWielded();

      if(Item && Item != DigItem)
	{
	  Dig->SetRightBackupID(GetRightArm()->GetWielded()->GetID());
	  GetRightArm()->GetWielded()->MoveTo(GetStack());
	}
    }

  if(GetLeftArm())
    {
      item* Item = GetLeftArm()->GetWielded();

      if(Item && Item != DigItem)
	{
	  Dig->SetLeftBackupID(GetLeftArm()->GetWielded()->GetID());
	  GetLeftArm()->GetWielded()->MoveTo(GetStack());
	}
    }

  if(GetMainWielded() != DigItem)
    {
      Dig->SetMoveDigger(true);
      DigItem->RemoveFromSlot();
      SetMainWielded(DigItem);
    }
  else
    Dig->SetMoveDigger(false);

  Dig->SetSquareDug(Square);
  SetAction(Dig);
}

bool humanoid::CheckKick() const
{
  if(!CanKick())
    {
      if(IsPlayer())
	ADD_MESSAGE("This race can't kick.");

      return false;
    }

  if(GetLegs() < 2)
    {
      if(IsPlayer())
	ADD_MESSAGE("How are you you going to do that with %s?", GetLegs() ? "one leg" : "no legs");

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
  if(!character::CheckThrow())
    return false;

  if(GetRightArm() || GetLeftArm())
    return true;
  else
    {
      ADD_MESSAGE("You don't have an arm to do that!");
      return false;
    }
}

bool humanoid::CheckOffer() const
{
  if(GetRightArm() || GetLeftArm())
    return true;
  else
    {
      ADD_MESSAGE("You need an arm to offer.");
      return false;
    }
}

vector2d humanoid::GetEquipmentPanelPos(ushort Index) const // convert to array
{
  switch(Index)
    {
    case HELMET_INDEX: return vector2d(34, -22);
    case AMULET_INDEX: return vector2d(14, -22);
    case CLOAK_INDEX: return vector2d(-6, -22);
    case BODY_ARMOR_INDEX: return vector2d(54, -22);
    case BELT_INDEX: return vector2d(24, 70);
    case RIGHT_WIELDED_INDEX: return vector2d(-14, 4);
    case LEFT_WIELDED_INDEX: return vector2d(62, 4);
    case RIGHT_RING_INDEX: return vector2d(-14, 44);
    case LEFT_RING_INDEX: return vector2d(62, 44);
    case RIGHT_GAUNTLET_INDEX: return vector2d(-14, 24);
    case LEFT_GAUNTLET_INDEX: return vector2d(62, 24);
    case RIGHT_BOOT_INDEX: return vector2d(4, 70);
    case LEFT_BOOT_INDEX: return vector2d(44, 70);
    }

  return vector2d(24, 12);
}

void humanoid::DrawSilhouette(bitmap* ToBitmap, vector2d Where, bool AnimationDraw) const
{
  ushort c;

  for(c = 0; c < GetEquipmentSlots(); ++c)
    if(CanUseEquipment(c))
      {
	vector2d Pos = Where + GetEquipmentPanelPos(c);

	if(!AnimationDraw)
	  DOUBLE_BUFFER->DrawRectangle(Pos + vector2d(-1, -1), Pos + vector2d(16, 16), DARK_GRAY);

	item* Equipment = GetEquipment(c);

	if(Equipment)
	  {
	    DOUBLE_BUFFER->Fill(Pos, 16, 16, BLACK);
	    Equipment->Draw(DOUBLE_BUFFER, Pos, ivanconfig::GetContrastLuminance(), 0, true, Equipment->AllowAlphaEverywhere());
	  }
      }

  if(AnimationDraw)
    return;

  ushort Color[4] = { 0, 0, 0, 0 };

  if(GetHead())
    Color[0] = GetHead()->GetConditionColor();

  if(GetRightArm())
    Color[1] = GetRightArm()->GetConditionColor();

  if(GetLeftArm())
    Color[2] = GetLeftArm()->GetConditionColor();

  if(GetTorso())
    Color[3] = GetTorso()->GetConditionColor();

  igraph::GetCharacterRawGraphic()->MaskedBlit(ToBitmap, 0, 64, Where.X, Where.Y, SILHOUETTE_X_SIZE, SILHOUETTE_Y_SIZE, Color);

  for(c = 0; c < 4; ++c)
    Color[c] = 0;

  if(GetGroin())
    Color[1] = GetGroin()->GetConditionColor();

  if(GetRightLeg())
    Color[2] = GetRightLeg()->GetConditionColor();

  if(GetLeftLeg())
    Color[3] = GetLeftLeg()->GetConditionColor();

  igraph::GetCharacterRawGraphic()->MaskedBlit(ToBitmap, 64, 64, Where, SILHOUETTE_X_SIZE, SILHOUETTE_Y_SIZE, Color);
}

ushort humanoid::GlobalResistance(ushort Type) const
{
  ushort Resistance = GetResistance(Type);

  if(GetCloak())
    Resistance += GetCloak()->GetResistance(Type);

  if(!(Type & PHYSICAL_DAMAGE))
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

bool humanoid::TryToRiseFromTheDead()
{
  ushort c;

  for(c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c))
      {
	/* Let's search for the original bodypart */

	for(stackiterator i1 = GetStackUnder()->GetBottom(); i1.HasItem(); ++i1)
	  {
	    for(std::list<ulong>::iterator i2 = OriginalBodyPartID[c].begin(); i2 != OriginalBodyPartID[c].end(); ++i2)
	      if(i1->GetID() == *i2)
		{
		  item* Item = *i1;
		  Item->RemoveFromSlot();
		  AttachBodyPart(static_cast<bodypart*>(Item));
		  GetBodyPart(c)->SetHP(1);
		  break;
		}

	    if(GetBodyPart(c))
	      break;
	  }
      }

  for(c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPartIsVital(c) && !BodyPart)
	if(!HandleNoBodyPart(c))
	  return false;

      if(BodyPart)
	{
	  BodyPart->ResetSpoiling();

	  if(BodyPart->CanRegenerate() || BodyPart->GetHP() < 1)
	    BodyPart->SetHP(1);
	}
    }

  ResetStates();
  return true;
}

bool humanoid::HandleNoBodyPart(ushort Index)
{
  switch(Index)
    {
    case HEAD_INDEX:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The headless body of %s vibrates violently.", CHAR_NAME(DEFINITE));

      return false;
    case GROIN_INDEX:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The groinless body of %s vibrates violently.", CHAR_NAME(DEFINITE));

      return false;
    case TORSO_INDEX:
      ABORT("The corpse does not have a torso.");
    default:
      return true;
    }
}

vector2d humanoid::GetBodyPartBitmapPos(ushort Index, bool) const
{
  switch(Index)
    {
    case TORSO_INDEX: return GetTorsoBitmapPos();
    case HEAD_INDEX: return GetHeadBitmapPos();
    case RIGHT_ARM_INDEX: return GetRightArmBitmapPos();
    case LEFT_ARM_INDEX: return GetLeftArmBitmapPos();
    case GROIN_INDEX: return GetGroinBitmapPos();
    case RIGHT_LEG_INDEX: return GetRightLegBitmapPos();
    case LEFT_LEG_INDEX: return GetLeftLegBitmapPos();
    }

  ABORT("Weird bodypart BitmapPos request for a humanoid!");
  return vector2d();
}

ushort humanoid::GetBodyPartColorB(ushort Index, bool) const
{
  switch(Index)
    {
    case TORSO_INDEX: return GetTorsoMainColor();
    case HEAD_INDEX: return GetCapColor();
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return GetArmMainColor();
    case GROIN_INDEX:
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return GetLegMainColor();
    }

  ABORT("Weird bodypart color B request for a humanoid!");
  return 0;
}

ushort humanoid::GetBodyPartColorC(ushort Index, bool) const
{
  switch(Index)
    {
    case TORSO_INDEX: return GetBeltColor();
    case HEAD_INDEX: return GetHairColor();
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return GetGauntletColor();
    case GROIN_INDEX:
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return GetBootColor();
    }

  ABORT("Weird bodypart color C request for a humanoid!");
  return 0;
}

ushort humanoid::GetBodyPartColorD(ushort Index, bool) const
{
  switch(Index)
    {
    case TORSO_INDEX: return GetTorsoSpecialColor();
    case HEAD_INDEX: return GetEyeColor();
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return GetArmSpecialColor();
    case GROIN_INDEX:
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return GetLegSpecialColor();
    }

  ABORT("Weird bodypart color D request for a humanoid!");
  return 0;
}

bool humanoid::BodyPartColorBIsSparkling(ushort Index, bool) const
{
  switch(Index)
    {
    case TORSO_INDEX: return TorsoMainColorIsSparkling();
    case HEAD_INDEX: return CapColorIsSparkling();
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return ArmMainColorIsSparkling();
    case GROIN_INDEX:
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return LegMainColorIsSparkling();
    }

  ABORT("Weird bodypart color B request for a humanoid!");
  return 0;
}

bool humanoid::BodyPartColorCIsSparkling(ushort Index, bool) const
{
  switch(Index)
    {
    case TORSO_INDEX: return BeltColorIsSparkling();
    case HEAD_INDEX: return HairColorIsSparkling();
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return GauntletColorIsSparkling();
    case GROIN_INDEX:
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return BootColorIsSparkling();
    }

  ABORT("Weird bodypart color C request for a humanoid!");
  return 0;
}

bool humanoid::BodyPartColorDIsSparkling(ushort Index, bool) const
{
  switch(Index)
    {
    case TORSO_INDEX: return TorsoSpecialColorIsSparkling();
    case HEAD_INDEX: return EyeColorIsSparkling();
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return ArmSpecialColorIsSparkling();
    case GROIN_INDEX:
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return LegSpecialColorIsSparkling();
    }

  ABORT("Weird bodypart color D request for a humanoid!");
  return 0;
}

void playerkind::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);

  if(!Load)
    {
      SoulID = 0;
      IsBonePlayer = false;
      IsClone = false;
      EditAttribute(ARM_STRENGTH, (RAND() & 1) - (RAND() & 1));
      EditAttribute(DEXTERITY, (RAND() & 1) - (RAND() & 1));
      EditAttribute(LEG_STRENGTH, (RAND() & 1) - (RAND() & 1));
      EditAttribute(AGILITY, (RAND() & 1) - (RAND() & 1));
      EditAttribute(PERCEPTION, (RAND() & 1) - (RAND() & 1));
      EditAttribute(INTELLIGENCE, (RAND() & 1) - (RAND() & 1));
      EditAttribute(WISDOM, (RAND() & 1) - (RAND() & 1));
      EditAttribute(CHARISMA, (RAND() & 1) - (RAND() & 1));
      EditAttribute(MANA, (RAND() & 1) - (RAND() & 1));

      if(GetMoney())
	SetMoney(GetMoney() + RAND() % 101);
    }
}

void petrus::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  game::SetPetrus(this);
  LastHealed = 0;
}

void shopkeeper::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);

  if(!Load)
    SetMoney(GetMoney() + RAND() % 2001);
}

void humanoid::Bite(character* Enemy, vector2d HitPos, uchar Direction, bool ForceHit)
{
  /* This function ought not to be called without a head */

  EditNP(-50);
  EditAP(-GetHead()->GetBiteAPCost());
  EditExperience(AGILITY, 30);
  Enemy->TakeHit(this, 0, GetHead(), HitPos, GetHead()->GetBiteDamage(), GetHead()->GetBiteToHitValue(), RAND() % 26 - RAND() % 26, BITE_ATTACK, Direction, !(RAND() % GetCriticalModifier()), ForceHit);
}

void humanoid::Kick(lsquare* Square, uchar Direction, bool ForceHit)
{
  leg* KickLeg = GetRandomLeg();
  EditNP(-50);
  EditAP(-KickLeg->GetKickAPCost());

  if(Square->BeKicked(this, 0, KickLeg, KickLeg->GetKickDamage(), KickLeg->GetKickToHitValue(), RAND() % 26 - RAND() % 26, Direction, !(RAND() % GetCriticalModifier()), ForceHit))
    {
      KickLeg->EditExperience(LEG_STRENGTH, 40);
      KickLeg->EditExperience(AGILITY, 20);
    }
}

/* Returns the average number of APs required to kill Enemy */

float humanoid::GetTimeToKill(const character* Enemy, bool UseMaxHP) const
{
  float Effectivity = 0;
  ushort AttackStyles = 0;

  if(IsUsingArms())
    {
      rightarm* RightArm = GetRightArm();

      if(RightArm)
	{
	  float Damage = RightArm->GetDamage();

	  if(Damage)
	    Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(Damage) + 1, RightArm->GetToHitValue(), AttackIsBlockable(GetRightWielded() ? WEAPON_ATTACK : UNARMED_ATTACK), UseMaxHP) * RightArm->GetAPCost());
	}

      leftarm* LeftArm = GetLeftArm();

      if(LeftArm)
	{
	  float Damage = LeftArm->GetDamage();

	  if(Damage)
	    Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(Damage) + 1, LeftArm->GetToHitValue(), AttackIsBlockable(GetLeftWielded() ? WEAPON_ATTACK : UNARMED_ATTACK), UseMaxHP) * LeftArm->GetAPCost());
	}

      ++AttackStyles;
    }

  if(IsUsingLegs())
    {
      leg* RightLeg = GetRightLeg();
      leg* LeftLeg = GetLeftLeg();
      float TimeToDie = Enemy->GetTimeToDie(this, ushort(RightLeg->GetKickDamage()) + 1, RightLeg->GetKickToHitValue(), AttackIsBlockable(KICK_ATTACK), UseMaxHP) * RightLeg->GetKickAPCost()
		      + Enemy->GetTimeToDie(this, ushort(LeftLeg->GetKickDamage()) + 1, LeftLeg->GetKickToHitValue(), AttackIsBlockable(KICK_ATTACK), UseMaxHP) * LeftLeg->GetKickAPCost();
      Effectivity += 2 / TimeToDie;
      ++AttackStyles;
    }

  if(IsUsingHead())
    {
      head* Head = GetHead();
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(Head->GetBiteDamage()) + 1, Head->GetBiteToHitValue(), AttackIsBlockable(BITE_ATTACK), UseMaxHP) * Head->GetBiteAPCost());
      ++AttackStyles;
    }

  if(StateIsActivated(HASTE))
    Effectivity *= 2;

  if(StateIsActivated(SLOW))
    Effectivity /= 2;

  return AttackStyles ? AttackStyles / Effectivity : 10000000;
}

ushort humanoid::GetAttribute(ushort Identifier) const
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::GetAttribute(Identifier);
  else
    {
      ushort Attrib = 0;

       if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
	{
	  rightarm* RightArm = GetRightArm();

	  if(RightArm)
	    Attrib += RightArm->GetAttribute(Identifier);

	  leftarm* LeftArm = GetLeftArm();

	  if(LeftArm)
	    Attrib += LeftArm->GetAttribute(Identifier);
	}
      else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
	{
	  rightleg* RightLeg = GetRightLeg();

	  if(RightLeg)
	    Attrib += RightLeg->GetAttribute(Identifier);

	  leftleg* LeftLeg = GetLeftLeg();

	  if(LeftLeg)
	    Attrib += LeftLeg->GetAttribute(Identifier);
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
  if(Identifier < BASE_ATTRIBUTES)
    return character::EditAttribute(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
    {
      bool Success = false;

      if(GetRightArm() && GetRightArm()->EditAttribute(Identifier, Value))
	Success = true;

      if(GetLeftArm() && GetLeftArm()->EditAttribute(Identifier, Value))
	Success = true;

      return Success;
    }
  else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
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
  if(Identifier < BASE_ATTRIBUTES)
    character::EditExperience(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
    {
      if(!IsPlayer())
	Value <<= 1;

      if(GetRightArm())
	GetRightArm()->EditExperience(Identifier, Value, false);

      if(GetLeftArm())
	GetLeftArm()->EditExperience(Identifier, Value, false);
    }
  else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
    {
      if(!IsPlayer())
	Value <<= 1;

      if(GetRightLeg())
	GetRightLeg()->EditExperience(Identifier, Value, false);

      if(GetLeftLeg())
	GetLeftLeg()->EditExperience(Identifier, Value, false);
    }
  else
    ABORT("Illegal humanoid attribute %d experience edit request!", Identifier);
}

ushort humanoid::DrawStats(bool AnimationDraw) const
{
  DrawSilhouette(DOUBLE_BUFFER, vector2d(RES_X - SILHOUETTE_X_SIZE - 23, 53), AnimationDraw);

  if(AnimationDraw)
    return 15;

  ushort PanelPosX = RES_X - 96, PanelPosY = 15;

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "ArmStr %d", GetAttribute(ARM_STRENGTH));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "LegStr %d", GetAttribute(LEG_STRENGTH));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Dex %d", GetAttribute(DEXTERITY));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, PanelPosY++ * 10, WHITE, "Agi %d", GetAttribute(AGILITY));
  return PanelPosY;
}

ushort humanoid::GetRandomStepperBodyPart() const
{
  uchar Possible = 0, PossibleArray[3];

  if(GetRightLeg())
    PossibleArray[Possible++] = RIGHT_LEG_INDEX;

  if(GetLeftLeg())
    PossibleArray[Possible++] = LEFT_LEG_INDEX;

  if(Possible)
    return PossibleArray[RAND() % Possible];

  if(GetHead())
    PossibleArray[Possible++] = HEAD_INDEX;

  if(GetGroin())
    PossibleArray[Possible++] = GROIN_INDEX;

  PossibleArray[Possible++] = TORSO_INDEX;
  return PossibleArray[RAND() % Possible];
}

ushort humanoid::CheckForBlock(character* Enemy, item* Weapon, float ToHitValue, ushort Damage, short Success, uchar Type)
{
  if(GetAction())
    return Damage;

  if(GetRightWielded())
    Damage = CheckForBlockWithArm(Enemy, Weapon, GetRightArm(), ToHitValue, Damage, Success, Type);

  if(Damage && GetLeftWielded() && (!Weapon || Weapon->Exists()))
    Damage = CheckForBlockWithArm(Enemy, Weapon, GetLeftArm(), ToHitValue, Damage, Success, Type);

  return Damage;
}

bool humanoid::CanWield() const
{
  return CanUseEquipment(RIGHT_WIELDED_INDEX) || CanUseEquipment(LEFT_WIELDED_INDEX);
}

/* return true if still in balance */

bool humanoid::CheckBalance(float KickDamage)
{
  return !(GetMoveType() & WALK) || !KickDamage || IsStuck() || (GetLegs() != 1 && KickDamage * 10 < RAND() % GetSize());
}

long humanoid::GetMoveAPRequirement(uchar Difficulty) const
{
  if(GetMoveType() & FLY)
    return (!StateIsActivated(PANIC) ? 10000000 : 8000000) * Difficulty / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());

  switch(GetLegs())
    {
    case 0:
      return (!StateIsActivated(PANIC) ? 20000000 : 16000000) * Difficulty / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
    case 1:
      return (!StateIsActivated(PANIC) ? 13333333 : 10666667) * Difficulty / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
    case 2:
      return (!StateIsActivated(PANIC) ? 10000000 : 8000000) * Difficulty / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
    }

  ABORT("A %d legged humanoid invaded the dungeon!", GetLegs());
  return 0;
}

void hunter::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < BodyParts; ++c) 
    if(c != LEFT_ARM_INDEX)
      CreateBodyPart(c, SpecialFlags);
    else
      SetBodyPart(LEFT_ARM_INDEX, 0);
}

bool humanoid::EquipmentEasilyRecognized(ushort Index) const
{
  if(IsPet())
    return true;

  switch(Index)
    {
    case AMULET_INDEX:
    case RIGHT_RING_INDEX:
    case LEFT_RING_INDEX:
    case BELT_INDEX:
      return false;
    }

  return true;
}

void humanoid::VirtualConstructor(bool)
{
  SetCurrentRightSWeaponSkill(0);
  SetCurrentLeftSWeaponSkill(0);
}

void humanoid::SignalEquipmentAdd(ushort EquipmentIndex)
{
  character::SignalEquipmentAdd(EquipmentIndex);

  if(EquipmentIndex == RIGHT_WIELDED_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentRightSWeaponSkill, GetRightWielded());
  else if(EquipmentIndex == LEFT_WIELDED_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentLeftSWeaponSkill, GetLeftWielded());

  if(!Initializing)
    CalculateBattleInfo();
}

void humanoid::SignalEquipmentRemoval(ushort EquipmentIndex)
{
  character::SignalEquipmentRemoval(EquipmentIndex);

  if(EquipmentIndex == RIGHT_WIELDED_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentRightSWeaponSkill, 0);
  else if(EquipmentIndex == LEFT_WIELDED_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentLeftSWeaponSkill, 0);

  if(!Initializing)
    CalculateBattleInfo();
}

void humanoid::SWeaponSkillTick()
{
  for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end();)
    {
      if((*i)->Tick() && IsPlayer())
	{
	  item* Item = SearchForItem(*i);

	  if(Item)
	    (*i)->AddLevelDownMessage(Item->CHAR_NAME(UNARTICLED));
	}

      if(!(*i)->GetHits() && *i != GetCurrentRightSWeaponSkill() && *i != GetCurrentLeftSWeaponSkill())
	{
	  std::list<sweaponskill*>::iterator Dirt = i++;
	  SWeaponSkill.erase(Dirt);
	}
      else
	++i;
    }
}

void angel::GetAICommand()
{
  if((LastHealed || game::GetTicks() - LastHealed > 10000) && AttachBodyPartsOfFriendsNear())
    return;

  humanoid::GetAICommand();
}

/* Returns true if the angel finds somebody near to heal else false */

bool angel::AttachBodyPartsOfFriendsNear()
{
  character* HurtOne = 0;
  bodypart* SeveredOne = 0;

  for(ushort d = 0; d < GetNeighbourSquares(); ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char && (!HurtOne || Char->IsPlayer()) && GetRelation(Char) == FRIEND && !Char->HasAllBodyParts())
	    {
	      bodypart* BodyPart = Char->FindRandomOwnBodyPart();

	      if(BodyPart)
		{
		  HurtOne = Char;
		  SeveredOne = BodyPart;
		}
	    }
	}
    }

  if(HurtOne)
    {
      if(HurtOne->IsPlayer())
	ADD_MESSAGE("%s puts your %s back to its place.", CHAR_DESCRIPTION(DEFINITE), SeveredOne->GetBodyPartName().CStr());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s helps %s by putting %s %s in its old place.", CHAR_DESCRIPTION(DEFINITE), HurtOne->CHAR_DESCRIPTION(DEFINITE), HurtOne->GetPossessivePronoun().CStr(), SeveredOne->GetBodyPartName().CStr());
	
      SeveredOne->SetHP(1);
      SeveredOne->RemoveFromSlot();
      HurtOne->AttachBodyPart(SeveredOne);
      LastHealed = game::GetTicks();
      DexterityAction(5);
      return true;
    }
  else
    return false;
}

void angel::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  LastHealed = 0;
}

void humanoid::DrawBodyParts(bitmap* Bitmap, vector2d Pos, ulong Luminance, ushort, bool AllowAnimate, bool AllowAlpha) const
{
  bitmap* TileBuffer = igraph::GetTileBuffer();
  Bitmap->Blit(TileBuffer, Pos, 0, 0, 16, 16);
  TileBuffer->FillPriority(0);

  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(DrawOrder[c]);

      if(BodyPart)
	BodyPart->Draw(TileBuffer, GetDrawDisplacement(c), Luminance, 0, AllowAnimate, AllowAlpha);
    }

  if(GetLeftArm())
    GetLeftArm()->DrawWielded(TileBuffer, vector2d(0, 0), Luminance, AllowAnimate);

  if(GetRightArm())
    GetRightArm()->DrawWielded(TileBuffer, vector2d(0, 0), Luminance, AllowAnimate);

  TileBuffer->Blit(Bitmap, 0, 0, Pos, 16, 16);
}

vector2d kamikazedwarf::GetDrawDisplacement(ushort Index) const
{
  static vector2d DrawDisplacement[] = { vector2d(0, 0), vector2d(0, 1), vector2d(0, -1), vector2d(0, -1), vector2d(0, -1), vector2d(0, 0), vector2d(0, 0) };
  return DrawDisplacement[Index];
}

ushort angel::GetTorsoMainColor() const
{
  return GetMasterGod()->GetColor();
}

ushort angel::GetArmMainColor() const
{
  return GetMasterGod()->GetColor();
}

ushort kamikazedwarf::GetTorsoMainColor() const
{
  return GetMasterGod()->GetColor();
}

ushort kamikazedwarf::GetGauntletColor() const
{
  return GetMasterGod()->GetColor();
}

ushort kamikazedwarf::GetLegMainColor() const
{
  return GetMasterGod()->GetColor();
}

ushort housewife::GetHairColor() const
{
  static ushort HouseWifeHairColor[] = { MakeRGB16(48, 40, 8), MakeRGB16(60, 48, 24),  MakeRGB16(200, 0, 0) };
  return HouseWifeHairColor[RAND() % 3];
}

ushort angel::GetAttribute(ushort Identifier) const // temporary until wings are bodyparts
{
  if(Identifier == LEG_STRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return GetDefaultAgility();
  else
    return humanoid::GetAttribute(Identifier);
}

ushort genie::GetAttribute(ushort Identifier) const // temporary until someone invents a better way of doing this
{
  if(Identifier == LEG_STRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return GetDefaultAgility();
  else
    return humanoid::GetAttribute(Identifier);
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
  return GetAttackStyle() & USE_ARMS && ((GetRightArm() && GetRightArm()->GetDamage()) || (GetLeftArm() && GetLeftArm()->GetDamage()));
}

bool humanoid::IsUsingLegs() const
{
  return (GetAttackStyle() & USE_LEGS || (GetAttackStyle() & USE_ARMS && (!GetRightArm() || !GetRightArm()->GetDamage()) && (!GetLeftArm() || !GetLeftArm()->GetDamage()))) && GetRightLeg() && GetLeftLeg();
}

bool humanoid::IsUsingHead() const
{
  return (GetAttackStyle() & USE_HEAD || ((GetAttackStyle() & USE_LEGS || (GetAttackStyle() & USE_ARMS && (!GetRightArm() || !GetRightArm()->GetDamage()) && (!GetLeftArm() || !GetLeftArm()->GetDamage()))) && (!GetRightLeg() || !GetLeftLeg()))) && GetHead();
}

void humanoid::CalculateBattleInfo()
{
  CalculateDodgeValue();

  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
	BodyPart->CalculateAttackInfo();
    }
}

leg* humanoid::GetRandomLeg() const
{
  if(RAND() & 1)
    return GetRightLeg();
  else
    return GetLeftLeg();
}

item* skeleton::SevereBodyPart(ushort BodyPartIndex)
{
  item* Bone;

  if(BodyPartIndex == HEAD_INDEX)
    Bone = new skull(0, NO_MATERIALS);
  else
    Bone = new bone(0, NO_MATERIALS);

  item* BodyPart = GetBodyPart(BodyPartIndex);
  Bone->InitMaterials(BodyPart->GetMainMaterial());
  BodyPart->DropEquipment();
  BodyPart->RemoveFromSlot();
  BodyPart->SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  BodyPart->SendToHell();
  CalculateAttributeBonuses();
  CalculateBattleInfo();

  if(StuckToBodyPart == BodyPartIndex)
    {
      StuckToBodyPart = NONE_INDEX;
      StuckTo = 0;
    }

  return Bone;  
}

void zombie::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < BodyParts; ++c) 
    if(BodyPartIsVital(c) || RAND() % 3)
      {
	bodypart* BodyPart = CreateBodyPart(c, SpecialFlags|NO_PIC_UPDATE);
	BodyPart->GetMainMaterial()->SetSpoilCounter(2000 + RAND() % 1000);
      }
}

void humanoid::AddSpecialEquipmentInfo(festring& String, ushort Index) const
{
  if((Index == RIGHT_WIELDED_INDEX && GetRightArm()->TwoHandWieldIsActive()) || (Index == LEFT_WIELDED_INDEX && GetLeftArm()->TwoHandWieldIsActive()))
    String << " (in both hands)";
}

/* Yes, this is evil. */

#define INSTANTIATE(name) if(DataBase->name.IsValid() && (Item = DataBase->name.Instantiate(SpecialFlags))) Set##name(Item);

void humanoid::CreateInitialEquipment(ushort SpecialFlags)
{
  character::CreateInitialEquipment(SpecialFlags);
  item* Item;

  INSTANTIATE(Helmet);
  INSTANTIATE(Amulet);
  INSTANTIATE(Cloak);
  INSTANTIATE(BodyArmor);
  INSTANTIATE(Belt);
  INSTANTIATE(RightWielded);
  INSTANTIATE(LeftWielded);
  INSTANTIATE(RightRing);
  INSTANTIATE(LeftRing);
  INSTANTIATE(RightGauntlet);
  INSTANTIATE(LeftGauntlet);
  INSTANTIATE(RightBoot);
  INSTANTIATE(LeftBoot);

  if(CurrentRightSWeaponSkill)
    CurrentRightSWeaponSkill->AddHit(GetRightSWeaponSkillHits());

  if(CurrentLeftSWeaponSkill)
    CurrentLeftSWeaponSkill->AddHit(GetLeftSWeaponSkillHits());
}

festring humanoid::GetBodyPartName(ushort Index, bool Articled) const
{
  festring Article;

  if(Articled)
    Article << 'a';

  switch(Index)
    {
    case HEAD_INDEX: return Article + "head";
    case TORSO_INDEX: return Article + "torso";
    case RIGHT_ARM_INDEX: return Article + "right arm";
    case LEFT_ARM_INDEX: return Article + "left arm";
    case GROIN_INDEX: return Article + "groin";
    case RIGHT_LEG_INDEX: return Article + "right leg";
    case LEFT_LEG_INDEX: return Article + "left leg";
    }

  ABORT("Illegal humanoid bodypart name request!");
  return "";
}

void humanoid::CreateBlockPossibilityVector(blockvector& Vector, float ToHitValue) const
{
  float RightBlockChance = 0;
  ushort RightBlockCapability = 0;
  float LeftBlockChance = 0;
  ushort LeftBlockCapability = 0;
  rightarm* RightArm = GetRightArm();
  leftarm* LeftArm = GetLeftArm();

  if(RightArm)
    {
      RightBlockChance = RightArm->GetBlockChance(ToHitValue);
      RightBlockCapability = RightArm->GetBlockCapability();
    }

  if(LeftArm)
    {
      LeftBlockChance = LeftArm->GetBlockChance(ToHitValue);
      LeftBlockCapability = LeftArm->GetBlockCapability();
    }

  /* Double block */

  if(RightBlockCapability + LeftBlockCapability)
    Vector.push_back(std::pair<float, ushort>(RightBlockChance * LeftBlockChance, RightBlockCapability + LeftBlockCapability));

  /* Right block */

  if(RightBlockCapability)
    Vector.push_back(std::pair<float, ushort>(RightBlockChance * (1 - LeftBlockChance), RightBlockCapability));

  /* Left block */

  if(LeftBlockCapability)
    Vector.push_back(std::pair<float, ushort>(LeftBlockChance * (1 - RightBlockChance), LeftBlockCapability));
}

item* humanoid::SevereBodyPart(ushort BodyPartIndex)
{
  if(BodyPartIndex == RIGHT_ARM_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentRightSWeaponSkill, 0);
  else if(BodyPartIndex == LEFT_ARM_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentLeftSWeaponSkill, 0);

  return character::SevereBodyPart(BodyPartIndex);
}

humanoid::humanoid(const humanoid& Humanoid) : character(Humanoid), CurrentRightSWeaponSkill(0), CurrentLeftSWeaponSkill(0)
{
  SWeaponSkill.resize(Humanoid.SWeaponSkill.size());
  std::list<sweaponskill*>::iterator i1 = SWeaponSkill.begin();
  std::list<sweaponskill*>::const_iterator i2 = Humanoid.SWeaponSkill.begin();

  for(; i1 != SWeaponSkill.end(); ++i1, ++i2)
    *i1 = new sweaponskill(**i2);
}

const festring& humanoid::GetDeathMessage() const
{
  static festring HeadlessDeathMsg = CONST_S("@Dd dies without a sound.");
  return GetHead() || character::GetDeathMessage() != "@Dd dies screaming." ? character::GetDeathMessage() : HeadlessDeathMsg;
}

uchar humanoid::GetSWeaponSkillLevel(const item* Item) const
{
  std::list<sweaponskill*>::const_iterator i;

  for(i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
    if((*i)->IsSkillOf(Item))
      return (*i)->GetLevel();

  const std::vector<ulong>& CMID = Item->GetCloneMotherID();

  for(long c = CMID.size() - 1; c >= 0; --c)
    for(i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
      if((*i)->IsSkillOfCloneMother(Item, CMID[c]))
	return (*i)->GetLevel();

  return 0;
}

bool humanoid::UseMaterialAttributes() const
{
  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && !BodyPart->UseMaterialAttributes())
	return false;
    }

  return true;
}

ulong angel::GetBaseEmitation() const
{
  switch(GetMasterGod()->GetBasicAlignment())
    {
    case GOOD: return MakeRGB24(150, 150, 150);
    case NEUTRAL: return MakeRGB24(120, 120, 150);
    case EVIL: return MakeRGB24(150, 110, 110);
    }

  return 0;
}

void bananagrower::BeTalkedTo()
{
  static ulong Said;

  if(GetRelation(PLAYER) == HOSTILE)
    ProcessAndAddMessage(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().size())]);
  else if(!game::TweraifIsFree())
    {
      if(GetRelation(PLAYER) != HOSTILE && !Profession && !(RAND() % 7))
	ADD_MESSAGE("\"I'm glad Petrus spared my life even though I was the president.\"");

      ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, 6)]);
    }
  else
    ProcessAndAddMessage(GetFriendlyReplies()[6 + RandomizeReply(Said, 3)]);
}

festring bananagrower::GetProfessionDescription() const
{
  festring String;

  switch(Profession)
    {
    case 0:
      String = CONST_S("the president of Tweraif");
      break;
    case 1:
      String = CONST_S("a diplomat");
      break;
    case 2:
      String = CONST_S("a teacher");
      break;
    case 3:
      String = CONST_S("a philosopher");
      break;
    case 4:
      String = CONST_S("a journalist");
      break;
    case 5:
      String = CONST_S("an alchemist");
      break;
    case 6:
      String = CONST_S("a renown mathematician");
      break;
    case 7:
      String = CONST_S("a priest of Silva");
      break;
    }

  return String;
}

void bananagrower::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);

  if(!Load)
    {
      Profession = RAND() & 7;
      HasBeenOnLandingSite = false;
    }
}

void bananagrower::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << Profession << HasBeenOnLandingSite;
}

void bananagrower::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> Profession >> HasBeenOnLandingSite;
}

void smith::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("\"You talkin' to me? You talkin' to me? You talkin' to me? Then who the hell else are you talkin' to? You talkin' to me? Well I'm the only one here. Who do you think you're talking to? Oh yeah? Huh? Ok.\"");
      return;
    }

  if(!GetMainWielded() || !GetMainWielded()->CanBeUsedBySmith())
    {
      ADD_MESSAGE("\"Sorry, I need an intact hammer to practise the art of smithing.\"");
      return;
    }

  for(ushort c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = PLAYER->GetBodyPart(c);

      if(BodyPart)
	{
	  if(!BodyPart->GetMainMaterial()->IsMetal())
	    continue;
	  
	  if(BodyPart->GetHP() >= BodyPart->GetMaxHP())
	    continue;

	  ADD_MESSAGE("Your %s seems to be hurt. I could fix it for the modest sum of 25 gold pieces.", BodyPart->GetBodyPartName().CStr()); 
	  
	  if(game::BoolQuestion(CONST_S("Do you accept this deal? [y/N]")))
	    {
	      BodyPart->RestoreHP();
	      PLAYER->EditMoney(-25);
	    }	
	}
    }

  if(PLAYER->GetStack()->SortedItems(this, &item::FixableBySmithSorter))
    {
      item* Item = PLAYER->GetStack()->DrawContents(this, CONST_S("\"What do you want me to fix?\""), 0, &item::FixableBySmithSorter);

      if(!Item)
	return;

      if(!Item->GetMainMaterial()->IsMetal())
	{
	  ADD_MESSAGE("\"I only fix items made of metal.\"");
	  return;
	}

      if(PLAYER->GetMoney() < Item->GetFixPrice())
	{
	  ADD_MESSAGE("\"Getting that fixed costs you %d gold pieces. Get the money and we'll talk.\"", Item->GetFixPrice());
	  return;
	}

      ADD_MESSAGE("\"I can fix your %s, but it'll cost you %d gold pieces.\"", Item->CHAR_NAME(UNARTICLED), Item->GetFixPrice());

      if(game::BoolQuestion(CONST_S("Do you accept this deal? [y/N]")))
	{
	  Item->Fix();
	  PLAYER->EditMoney(-Item->GetFixPrice());
	  ADD_MESSAGE("%s fixes %s in no time.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(DEFINITE));
	}
    }
  else
    ADD_MESSAGE("\"Come back when you have some weapons or armor I can fix.\"");
}

void humanoid::CalculateDodgeValue()
{
  DodgeValue = 0.05f * GetMoveEase() * GetAttribute(AGILITY) / sqrt(GetSize());

  if(GetMoveType() & FLY)
    DodgeValue *= 2;
  else
    {
      if(!GetRightLeg() && !GetRightLeg())
	DodgeValue *= 0.50f;
      else if(!GetRightLeg() || !GetRightLeg())
	DodgeValue *= 0.75f;
    }

  if(DodgeValue < 1)
    DodgeValue = 1;
}

bool humanoid::CheckZap()
{
  if(GetArms() == 0)
    {
      ADD_MESSAGE("You need at least one arm to zap."); 
      return false;
    }
  else
    return character::CheckZap();
}

void bananagrower::GetAICommand()
{
  if(game::TweraifIsFree())
    {
      humanoid::GetAICommand();
      return;
    }

  if(CheckForEnemies(false, false))
    return;

  if(GetPos() == vector2d(45, 45))
    {
      itemvector ItemVector;
      GetStack()->FillItemVector(ItemVector);
      ushort BananasDropped = 0;

      for(ushort c = 0; c < ItemVector.size(); ++c)
	if(ItemVector[c]->IsBanana())
	  {
	    ItemVector[c]->MoveTo(GetStackUnder());
	    ++BananasDropped;
	  }

      if(BananasDropped)
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s drops %s.", CHAR_NAME(DEFINITE), BananasDropped == 1 ? "a banana" : "some bananas");

	  return;
	}

      HasBeenOnLandingSite = true;
    }

  if(!HasBeenOnLandingSite)
    {
      SetGoingTo(vector2d(45, 45));

      if(MoveTowardsTarget())
	return;
    }
  else if(GetPos().X == 54)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s leaves the town to gather more bananas.", CHAR_NAME(DEFINITE));

      for(ushort c = 0; c < 10; ++c)
	GetStack()->AddItem(new banana);

      vector2d Where = GetLevel()->GetNearestFreeSquare(this, vector2d(0, 45));

      if(Where == ERROR_VECTOR)
	Where = GetLevel()->GetRandomSquare(this, NOT_IN_ROOM); // this is odd but at least it doesn't crash

      Move(Where, true);
      Profession = RAND() & 7;
      RestoreBodyParts();
      RestoreHP();
      ResetStates();
      TemporaryState = 0;

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s enters the town.", CHAR_NAME(INDEFINITE));

      HasBeenOnLandingSite = false;
    }
  else
    {
      SetGoingTo(vector2d(54, 45));

      if(MoveTowardsTarget())
	return;
    }

  EditAP(-1000);
}

bool humanoid::CheckTalk()
{
  if(!character::CheckTalk())
    return false;

  if(!GetHead())
    {
      ADD_MESSAGE("You need a head to talk.");
      return false;
    }

  return true;
}

bool angel::CanCreateBodyPart(ushort C) const
{
  return C == TORSO_INDEX || C == HEAD_INDEX || C == RIGHT_ARM_INDEX || C == LEFT_ARM_INDEX;
}

bool genie::CanCreateBodyPart(ushort C) const
{
  return C == TORSO_INDEX || C == HEAD_INDEX || C == RIGHT_ARM_INDEX || C == LEFT_ARM_INDEX;
}

bool bananagrower::HandleCharacterBlockingTheWay(character* Char, vector2d Pos, uchar Dir)
{
  return Char->GetPos() == vector2d(45, 45) && (Displace(Char, true) || Hit(Char, Pos, Dir));
}

festring& bananagrower::ProcessMessage(festring& Msg) const
{
  character::ProcessMessage(Msg);
  SEARCH_N_REPLACE("@pd", GetProfessionDescription());
  SEARCH_N_REPLACE("@Pd", GetProfessionDescription().CapitalizeCopy());
  return Msg;
}

void elder::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < BodyParts; ++c) 
    if(c != LEFT_LEG_INDEX)
      CreateBodyPart(c, SpecialFlags);
    else
      SetBodyPart(LEFT_LEG_INDEX, 0);
}

void encourager::GetAICommand()
{
  if(CheckForEnemies(true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(CheckForDoors())
    return;

  if(game::GetTicks() - LastHit > 200)
    {
      static ushort NotDiagonal[] = { 1, 3, 4, 6 };

      for(ushort d = 0; d < 4; ++d)
	{
	  square* Square = GetNeighbourSquare(NotDiagonal[d]);

	  if(Square)
	    {
	      character* Char = Square->GetCharacter();

	      if(Char && Char->IsBananaGrower() && Hit(Char, Square->GetPos(), NotDiagonal[d], true))
		{
		  LastHit = game::GetTicks();
		  TerminateGoingTo();
		  return;
		}
	    }
	}
    }

  if(MoveTowardsHomePos())
    return;

  EditAP(-1000);
}

void encourager::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << LastHit;
}

void encourager::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> LastHit;
}

void encourager::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  LastHit = 0;
}

ulong skeleton::GetBodyPartVolume(ushort Index) const
{
  switch(Index)
    {
    case HEAD_INDEX: return 600;
    case TORSO_INDEX: return (GetTotalVolume() - 600) * 13 / 30;
    case RIGHT_ARM_INDEX:
    case LEFT_ARM_INDEX: return (GetTotalVolume() - 600) / 10;
    case GROIN_INDEX: return (GetTotalVolume() - 600) / 10;
    case RIGHT_LEG_INDEX:
    case LEFT_LEG_INDEX: return ((GetTotalVolume() - 600) << 1) / 15;
    }

  ABORT("Illegal humanoid bodypart volume request!");
  return 0;
}

bool humanoid::CheckIfEquipmentIsNotUsable(ushort Index) const
{
  return (Index == RIGHT_WIELDED_INDEX && GetRightArm()->CheckIfWeaponTooHeavy("this item"))
      || (Index == LEFT_WIELDED_INDEX && GetLeftArm()->CheckIfWeaponTooHeavy("this item"))
      || (Index == RIGHT_WIELDED_INDEX && GetLeftWielded() && GetLeftWielded()->IsTwoHanded() && GetLeftArm()->CheckIfWeaponTooHeavy("your other wielded item"))
      || (Index == LEFT_WIELDED_INDEX && GetRightWielded() && GetRightWielded()->IsTwoHanded() && GetRightArm()->CheckIfWeaponTooHeavy("your other wielded item"));
}

ushort mistress::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, vector2d HitPos, float Damage, float ToHitValue, short Success, uchar Type, uchar Direction, bool Critical, bool ForceHit)
{
  ushort Return = humanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage, ToHitValue, Success, Type, Direction, Critical, ForceHit);

  if(Return == HAS_HIT && Critical)
    {
      if(IsPlayer())
	ADD_MESSAGE("Aahhh. The pain feels unbelievably good.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s screams: \"Oh the delightful pain!\"", CHAR_NAME(DEFINITE));
      else
	ADD_MESSAGE("You hear someone screaming: \"Oh the delightful pain!\"");
    }

  return Return;
}

bool petrusswife::SpecialEnemySightedReaction(character* Char)
{
  item* Weapon = Char->GetMainWielded();

  if(Weapon && Weapon->IsWeapon(Char) && !(RAND() % 20))
    ADD_MESSAGE("%s screams: \"Oh my Frog, %s's got %s %s!\"", CHAR_DESCRIPTION(DEFINITE), Char->CHAR_PERSONAL_PRONOUN_THIRD_PERSON_VIEW, Weapon->GetArticle().CStr(), Weapon->GetNameSingular().CStr());

  return false;
}

bool housewife::SpecialEnemySightedReaction(character* Char)
{
  item* Weapon = Char->GetMainWielded();

  if(Weapon && Weapon->IsWeapon(Char) && !(RAND() % 5))
    ADD_MESSAGE("%s screams: \"Oh my Frog, %s's got %s %s!\"", CHAR_DESCRIPTION(DEFINITE), Char->CHAR_PERSONAL_PRONOUN_THIRD_PERSON_VIEW, Weapon->GetArticle().CStr(), Weapon->GetNameSingular().CStr());

  return false;
}

void guard::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << WayPoints << NextWayPoint;
}

void guard::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> WayPoints >> NextWayPoint;
}

void guard::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  NextWayPoint = 0;
}

void guard::GetAICommand()
{
  if(GetConfig() == MASTER && HP << 1 < MaxHP && (GetPos() - vector2d(30, 17)).GetLengthSquare() > 9)
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

      GetLevel()->GetLSquare(30, 16)->KickAnyoneStandingHereAway();
      Move(vector2d(30, 16), true);
      EditAP(-1000);
      return;
    }

  if(WayPoints.size() && !IsGoingSomeWhere())
    {
      if(GetPos() == WayPoints[NextWayPoint])
	if(NextWayPoint < WayPoints.size() - 1)
	  ++NextWayPoint;
	else
	  NextWayPoint = 0;

      GoingTo = WayPoints[NextWayPoint];
    }

  StandIdleAI();
}

bool mistress::ReceiveDamage(character* Damager, ushort Damage, ushort Type, uchar TargetFlags, uchar Direction, bool Divide, bool PenetrateArmor, bool Critical, bool ShowMsg)
{
  bool Success = humanoid::ReceiveDamage(Damager, Damage, Type, TargetFlags, Direction, Divide, PenetrateArmor, Critical, ShowMsg);

  if(Type & SOUND && Success && !(RAND() & 7))
    {
      if(IsPlayer())
	ADD_MESSAGE("Aahhh. The pain feels unbelievably good.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s screams: \"Oh the delightful pain!\"", CHAR_NAME(DEFINITE));
      else
	ADD_MESSAGE("You hear someone screaming: \"Oh the delightful pain!\"");
    }

  return Success;
}

void humanoid::AddSpecialStethoscopeInfo(felist& Info) const
{
  Info.AddEntry(CONST_S("Arm strength: ") + GetAttribute(ARM_STRENGTH), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Leg strength: ") + GetAttribute(LEG_STRENGTH), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Dexterity: ") + GetAttribute(DEXTERITY), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Agility: ") + GetAttribute(AGILITY), LIGHT_GRAY);
}

item* humanoid::GetPairEquipment(ushort Index) const
{
  switch(Index)
    {
    case RIGHT_WIELDED_INDEX: return GetLeftWielded();
    case LEFT_WIELDED_INDEX: return GetRightWielded();
    case RIGHT_GAUNTLET_INDEX: return GetLeftGauntlet();
    case LEFT_GAUNTLET_INDEX: return GetRightGauntlet();
    case RIGHT_BOOT_INDEX: return GetLeftBoot();
    case LEFT_BOOT_INDEX: return GetRightBoot();
    }

  return 0;
}

const festring& humanoid::GetStandVerb() const
{
  static festring HasntFeet = CONST_S("crawling");
  return HasFeet() ? character::GetStandVerb() : HasntFeet;
}

void darkmage::GetAICommand()
{
  SeekLeader();

  if(FollowLeader())
    return;

  character* NearestEnemy = 0;
  ulong NearestEnemyDistance = 0xFFFFFFFF;
  character* RandomFriend = 0;
  std::vector<character*> Friend;
  vector2d Pos = GetPos();

  for(ushort c = 0; c < game::GetTeams(); ++c)
    {
      if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
	{
	  for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	    if((*i)->IsEnabled())
	      {
		ulong ThisDistance = Max<ulong>(abs((*i)->GetPos().X - Pos.X), abs((*i)->GetPos().Y - Pos.Y));

		if((ThisDistance < NearestEnemyDistance || (ThisDistance == NearestEnemyDistance && !(RAND() % 3))) && (*i)->CanBeSeenBy(this))
		  {
		    NearestEnemy = *i;
		    NearestEnemyDistance = ThisDistance;
		  }
	      }
	}
      else if(GetTeam()->GetRelation(game::GetTeam(c)) == FRIEND)
	{
	  for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	    if((*i)->IsEnabled() && (*i)->CanBeSeenBy(this))
	      Friend.push_back(*i);
	}
    }

  if(NearestEnemy && ((GetConfig() != APPRENTICE && NearestEnemyDistance < 10) || StateIsActivated(PANIC)) && RAND() & 3)
    {
      SetGoingTo((Pos << 1) - NearestEnemy->GetPos());

      if(MoveTowardsTarget())
	return;
    }

  if(NearestEnemy && NearestEnemy->GetPos().IsAdjacent(Pos))
    {
      if(NearestEnemy->IsSmall() && GetAttribute(WISDOM) < NearestEnemy->GetAttackWisdomLimit() && !(RAND() % 5) && Hit(NearestEnemy, NearestEnemy->GetPos(), game::GetDirectionForVector(NearestEnemy->GetPos() - GetPos())))
	return;
      else if((GetConfig() == ARCH_MAGE && RAND() & 1) || (GetConfig() == ELDER && !(RAND() & 3)))
	{
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s invokes a spell and disappears.", CHAR_NAME(DEFINITE));

	  TeleportRandomly();
	  EditAP(-4000 / GetConfig());
	  return;
	}
    }

  if(Friend.size() && !(RAND() & 3))
    {
      RandomFriend = Friend[RAND() % Friend.size()];
      NearestEnemy = 0;
    }

  festring DeathMsg = CONST_S("killed by the spells of ") + GetName(INDEFINITE);

  if(NearestEnemy)
    {
      lsquare* Square = NearestEnemy->GetLSquareUnder();
      EditAP(-4000 / GetConfig());

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s invokes a spell!", CHAR_NAME(DEFINITE));

      switch(GetConfig())
	{
	case APPRENTICE:
	  Square->DrawLightning(vector2d(8, 8), WHITE, YOURSELF);
	  Square->Lightning(this, DeathMsg, YOURSELF);
	  break;
	case BATTLE_MAGE:
	  if(RAND() % 20)
	    {
	      Square->DrawLightning(vector2d(8, 8), WHITE, YOURSELF);
	      Square->Lightning(this, DeathMsg, YOURSELF);
	    }
	  else
	    {
	      Square->DrawParticles(RED);
	      Square->LowerEnchantment(this, DeathMsg, YOURSELF);
	    }

	  break;
	case ELDER:
	  switch(RAND() % 20)
	    {
	    case 0:
	    case 1:
	    case 2: Square->DrawParticles(RED); Square->Strike(this, DeathMsg, YOURSELF); break;
	    case 3: Square->DrawParticles(RED); Square->FireBall(this, DeathMsg, YOURSELF); break;
	    case 4:
	    case 5:
	    case 6: Square->DrawParticles(RED); Square->Slow(this, DeathMsg, YOURSELF); break;
	    case 7: Square->DrawParticles(RED); Square->Teleport(this, DeathMsg, YOURSELF); break;
	    case 8:
	    case 9:
	    case 10: Square->DrawParticles(RED); Square->LowerEnchantment(this, DeathMsg, YOURSELF); break;
	    default: Square->DrawLightning(vector2d(8, 8), WHITE, YOURSELF); Square->Lightning(this, DeathMsg, YOURSELF); break;
	    }

	  break;
	case ARCH_MAGE:
	  switch(RAND() % 20)
	    {
	    case 0:
	    case 1:
	    case 2: Square->DrawParticles(RED); Square->FireBall(this, DeathMsg, YOURSELF); break;
	    case 3:
	    case 4:
	    case 5: Square->DrawParticles(RED); Square->Slow(this, DeathMsg, YOURSELF); break;
	    case 6: Square->DrawParticles(RED); Square->Teleport(this, DeathMsg, YOURSELF); break;
	    case 7:
	    case 8:
	    case 9: Square->DrawParticles(RED); Square->LowerEnchantment(this, DeathMsg, YOURSELF); break;
	    case 10:
	      {
		golem* Golem = new golem(RAND() % 3 ? ARCANITE : OCTIRON);
		vector2d Where = GetLevel()->GetNearestFreeSquare(Golem, Square->GetPos());

		if(Where == ERROR_VECTOR)
		  {
		    if(CanBeSeenByPlayer())
		      ADD_MESSAGE("Nothing happens.");

		    delete Golem;
		  }
		else
		  {
		    Golem->SetTeam(GetTeam());
		    Golem->PutTo(Where);

		    if(Golem->CanBeSeenByPlayer())
		      ADD_MESSAGE("Suddenly %s materializes!", Golem->CHAR_NAME(DEFINITE));

		    Golem->GetLSquareUnder()->DrawParticles(RED); 
		  }

		break;
	      }
	    default: Square->DrawParticles(RED); Square->Strike(this, DeathMsg, YOURSELF); break;
	    }

	  break;
	}

      if(CanBeSeenByPlayer())
	NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell of ") + GetName(DEFINITE) + CONST_S(" interrupts you."));
      else
	NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell interrupts you."));

      return;
    }

  if(RandomFriend)
    {
      lsquare* Square = RandomFriend->GetLSquareUnder();
      EditAP(-4000 / GetConfig());
      Square->DrawParticles(RED);

      switch(GetConfig())
	{
	case APPRENTICE:
	case BATTLE_MAGE:
	  Square->Haste(this, DeathMsg, YOURSELF);
	  break;
	case ARCH_MAGE:
	  if(!(RAND() & 31))
	    {
	      RandomFriend->CloneToNearestSquare(this);
	      return;
	    }
	case ELDER:
	  if(RAND() & 1)
	    Square->Invisibility(this, DeathMsg, YOURSELF);
	  else
	    Square->Haste(this, DeathMsg, YOURSELF);

	  break;
	}

      return;
    }

  if(CheckForDoors())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void zombie::GetAICommand()
{
  if(!GetHead())
    {
      for(stackiterator i = GetLSquareUnder()->GetStack()->GetBottom(); i.HasItem(); ++i)
	{
	  head* Head = i->Behead();

	  if(Head)
	    {
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s takes %s and attaches it to its torso.", CHAR_NAME(DEFINITE), Head->CHAR_NAME(INDEFINITE));

	      Head->RemoveFromSlot();
	      AttachBodyPart(Head);
	      Head->SetHP(1);
	      DexterityAction(10);
	      return;
	    }
	}
    }

  humanoid::GetAICommand();
}

head* humanoid::Behead()
{
  head* Head = GetHead();

  if(Head)
    SevereBodyPart(HEAD_INDEX);

  return Head;
}

bool communist::BoundToUse(const item* Item, ushort Index) const
{
  return Item && Item->IsGorovitsFamilyRelic() && Item->IsInCorrectSlot(Index);
}

festring werewolfwolf::GetKillName() const
{
  if(GetPolymorphBackup() && GetPolymorphBackup()->GetType() == werewolfhuman_ProtoType.GetIndex())
    return GetName(INDEFINITE);

  return humanoid::GetKillName();
}

ushort humanoid::GetRandomApplyBodyPart() const
{
  if(GetRightArm() && GetLeftArm())
    {
      if(RAND_2)
	return RIGHT_ARM_INDEX;
      else
	return LEFT_ARM_INDEX;
    }

  if(!GetRightArm() && GetLeftArm())
    return LEFT_ARM_INDEX;

  if(!GetLeftArm() && GetRightArm())
    return RIGHT_ARM_INDEX;

  if(GetHead())
    return HEAD_INDEX;

  return TORSO_INDEX;
}

void golem::BeTalkedTo()
{
  static ulong Said;

  if(GetRelation(PLAYER) == HOSTILE)
    Engrave(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().size())]);
  else
    Engrave(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().size())]);

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s engraves something.", CHAR_NAME(DEFINITE));
}

#ifdef WIZARD

void humanoid::AddAttributeInfo(festring& Entry) const
{
  Entry.Resize(45);
  Entry << GetAttribute(ARM_STRENGTH);
  Entry.Resize(48);
  Entry << GetAttribute(LEG_STRENGTH);
  Entry.Resize(51);
  Entry << GetAttribute(DEXTERITY);
  Entry.Resize(54);
  Entry << GetAttribute(AGILITY);
  character::AddAttributeInfo(Entry);
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

  festring Entry;

  if(IsUsingLegs())
    {
      GetRightLeg()->AddAttackInfo(List);
      GetLeftLeg()->AddAttackInfo(List);
    }

  if(IsUsingHead())
    {
      Entry = CONST_S("   bite attack");
      Entry.Resize(50);
      Entry << GetHead()->GetBiteMinDamage() << '-' << GetHead()->GetBiteMaxDamage();
      Entry.Resize(60);
      Entry << int(GetHead()->GetBiteToHitValue());
      Entry.Resize(70);
      Entry << GetHead()->GetBiteAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

void humanoid::AddDefenceInfo(felist& List) const
{
  character::AddDefenceInfo(List);

  if(GetRightArm())
    GetRightArm()->AddDefenceInfo(List);

  if(GetLeftArm())
    GetLeftArm()->AddDefenceInfo(List);
}

void humanoid::DetachBodyPart()
{
  uchar ToBeDetached;

  switch(game::KeyQuestion(CONST_S("What limb? (l)eft arm, (r)ight arm, (L)eft leg, (R)ight leg, (h)ead?"), KEY_ESC, 5, 'l','r','L','R', 'h'))
    {
    case 'l':
      ToBeDetached = LEFT_ARM_INDEX;
      break;
    case 'r':
      ToBeDetached = RIGHT_ARM_INDEX;
      break;
    case 'L':
      ToBeDetached = LEFT_LEG_INDEX;
      break;
    case 'R':
      ToBeDetached = RIGHT_LEG_INDEX;
      break;
    case 'h':
      ToBeDetached = HEAD_INDEX;
      break;
    default:
      return;
    }

  if(GetBodyPart(ToBeDetached))
    {
      item* ToDrop = SevereBodyPart(ToBeDetached);
      SendNewDrawRequest();

      if(ToDrop)
	{
	  GetStack()->AddItem(ToDrop);
	  ToDrop->DropEquipment();
	}

      ADD_MESSAGE("Bodypart detached!");  
    }
  else
    ADD_MESSAGE("That bodypart has already been detached.");

  CheckDeath(CONST_S("removed one of his vital bodyparts"), 0);
}

#else

void humanoid::AddAttributeInfo(festring&) const { }
void humanoid::AddAttackInfo(felist&) const { }
void humanoid::AddDefenceInfo(felist&) const { }
void humanoid::DetachBodyPart() { }

#endif

bool ennerbeast::MustBeRemovedFromBone() const
{
  return !IsEnabled() || GetTeam()->GetID() != MONSTER_TEAM || GetDungeon()->GetIndex() != ELPURI_CAVE || GetLevel()->GetIndex() != ENNER_BEAST_LEVEL;
}

bool communist::MustBeRemovedFromBone() const
{
  return !IsEnabled() || GetTeam()->GetID() != IVAN_TEAM || GetDungeon()->GetIndex() != ELPURI_CAVE|| GetLevel()->GetIndex() != IVAN_LEVEL;
}

bool humanoid::PreProcessForBone()
{
  for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
    (*i)->PreProcessForBone();

  return character::PreProcessForBone();
}

void humanoid::FinalProcessForBone()
{
  character::FinalProcessForBone();

  for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end();)
    {
      boneidmap::iterator BI = game::GetBoneItemIDMap().find(-(*i)->GetID());

      if(BI == game::GetBoneItemIDMap().end())
	{
	  std::list<sweaponskill*>::iterator Dirt = i++;
	  SWeaponSkill.erase(Dirt);
	}
      else
	{
	  (*i)->SetID(BI->second);
	  ++i;
	}
    }
}

void petrus::FinalProcessForBone()
{
  humanoid::FinalProcessForBone();
  LastHealed = 0;
}

void angel::FinalProcessForBone()
{
  humanoid::FinalProcessForBone();
  LastHealed = 0;
}

void encourager::FinalProcessForBone()
{
  humanoid::FinalProcessForBone();
  LastHit = 0;
}

void playerkind::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << SoulID << IsBonePlayer << IsClone;
}

void playerkind::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> SoulID >> IsBonePlayer >> IsClone;
}

void playerkind::SetSoulID(ulong What)
{
  SoulID = What;

  if(GetPolymorphBackup())
    GetPolymorphBackup()->SetSoulID(What);
}

bool playerkind::SuckSoul(character* Soul)
{
  if(Soul->GetID() == SoulID)
    {
      SoulID = 0;
      return true;
    }

  return false;
}

bool playerkind::TryToRiseFromTheDead()
{
  if(humanoid::TryToRiseFromTheDead())
    {
      if(IsEnabled() && SoulID)
	{
	  ADD_MESSAGE("The soulless body of %s wobbles for a moment.", CHAR_NAME(DEFINITE));
	  return false;
	}

      return true;
    }
  else
    return false;
}

void playerkind::FinalProcessForBone()
{
  humanoid::FinalProcessForBone();
  IsBonePlayer = true;

  if(SoulID)
    {
      boneidmap::iterator BI = game::GetBoneCharacterIDMap().find(SoulID);

      if(BI != game::GetBoneItemIDMap().end())
	SoulID = BI->second;
    }
}

playerkind::playerkind(const playerkind& Char) : humanoid(Char), SoulID(Char.SoulID), IsBonePlayer(Char.IsBonePlayer), IsClone(true)
{
}

void playerkind::BeTalkedTo()
{
  if(IsClone && IsBonePlayer)
    {
      if(GetRelation(PLAYER) == HOSTILE)
	{
	  ADD_MESSAGE("Oh no, you too! Why does everyone bully me!", CHAR_DESCRIPTION(DEFINITE));
	  return;
	}

      static ulong Said;

      switch(RandomizeReply(Said, 4))
	{
	case 0:
	  ADD_MESSAGE("\"I'd like to write a memoir, but alas I doubt anyone would believe it.\"");
	  break;
	case 1:
	  ADD_MESSAGE("\"Then that damned clone appeared, took all my equipment and claimed I was his slave...\"");
	  break;
	case 2:
	  ADD_MESSAGE("\"The level was a catastrophe for the party, but luckily you saved the day.\"");
	  break;
	case 3:
	  ADD_MESSAGE("\"Oh, how I hate bananas. I Hate Them! I HATE THEM SO MUCH!!!\"");
	  break;
	}
    }
  else if(IsClone)
    {
      if(GetRelation(PLAYER) == HOSTILE)
	{
	  ADD_MESSAGE("%s seems extremely irritated. \"Vanish, you foul mirror image!\"", CHAR_DESCRIPTION(DEFINITE));
	  return;
	}

      static ulong Said;

      switch(RandomizeReply(Said, 4))
	{
	case 0:
	  ADD_MESSAGE("\"Hey, those clothes are mine! Give them back!\"");
	  break;
	case 1:
	  ADD_MESSAGE("\"What, you summoned me? What a coincidence, I remember summoning you, too.\"");
	  break;
	case 2:
	  ADD_MESSAGE("\"I'm leading this party, not you, Mr. copy guy!\"");
	  break;
	case 3:
	  ADD_MESSAGE("\"Oh, how I hate bananas. I Hate Them! I HATE THEM SO MUCH!!!\"");
	  break;
	}
    }
  else
    {
      if(GetRelation(PLAYER) == HOSTILE)
	{
	  ADD_MESSAGE("Let's finish what my ghost failed to do!", CHAR_DESCRIPTION(DEFINITE));
	  return;
	}

      static ulong Said;

      switch(RandomizeReply(Said, 4))
	{
	case 0:
	  ADD_MESSAGE("\"What was it like? Death, you mean? Well, just like New Attnam. Very hot and whips everywhere.\"");
	  break;
	case 1:
	  ADD_MESSAGE("\"Stop it already! I *don't* want to know how my corpse smelled!\"");
	  break;
	case 2:
	  ADD_MESSAGE("\"I'm sorry about that ghost thing. That YASD was just a bit too much to handle, so I lost myself.\"");
	  break;
	case 3:
	  ADD_MESSAGE("\"Oh, how I hate bananas. I Hate Them! I HATE THEM SO MUCH!!!\"");
	  break;
	}
    }
}

void humanoid::EnsureCurrentSWeaponSkillIsCorrect(sweaponskill*& Skill, const item* Wielded)
{
  if(Wielded)
    {
      if(!Skill || !Skill->IsSkillOf(Wielded))
	{
	  if(Skill)
	    EnsureCurrentSWeaponSkillIsCorrect(Skill, 0);

	  std::list<sweaponskill*>::iterator i;

	  for(i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
	    if((*i)->IsSkillOf(Wielded))
	      {
		Skill = *i;
		return;
	      }

	  const std::vector<ulong>& CMID = Wielded->GetCloneMotherID();

	  for(long c = CMID.size() - 1; c >= 0; --c)
	    for(i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
	      if((*i)->IsSkillOfCloneMother(Wielded, CMID[c]))
		{
		  Skill = new sweaponskill(**i);
		  Skill->SetID(Wielded->GetID());
		  SWeaponSkill.push_back(Skill);
		  return;
		}

	  Skill = new sweaponskill(Wielded);
	  SWeaponSkill.push_back(Skill);
	}
    }
  else if(Skill)
    {
      if(!Skill->GetHits() && (CurrentRightSWeaponSkill != Skill || CurrentLeftSWeaponSkill != Skill))
	for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
	  if(*i == Skill)
	    {
	      delete *i;
	      SWeaponSkill.erase(i);
	      break;
	    }

      Skill = 0;
    }
}

humanoid::~humanoid()
{
  for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
    delete *i;
}

void guard::TeleportRandomly()
{
  if(GetConfig() != MASTER)
    humanoid::TeleportRandomly();
}

bool petrus::MoveTowardsHomePos()
{
  if(GetPos() != vector2d(28, 20))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

      GetLevel()->GetLSquare(28, 20)->KickAnyoneStandingHereAway();
      Move(vector2d(28, 20), true);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s appears.", CHAR_NAME(DEFINITE));

      EditAP(-1000);
      return true;
    }
  else
    return false;
}

bool guard::MoveTowardsHomePos()
{
  if(GetConfig() == MASTER && GetPos() != vector2d(30, 16))
    {
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

      GetLevel()->GetLSquare(30, 16)->KickAnyoneStandingHereAway();
      Move(vector2d(30, 16), true);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s appears.", CHAR_NAME(DEFINITE));

      EditAP(-1000);
      return true;
    }
  else
    return humanoid::MoveTowardsHomePos();
}

bodypart* ennerbeast::MakeBodyPart(ushort Index) const
{
  if(Index == HEAD_INDEX)
    return new ennerhead(0, NO_MATERIALS);
  else
    return humanoid::MakeBodyPart(Index);
}

ulong humanoid::GetSumOfAttributes() const
{
  return GetAttribute(ENDURANCE) + GetAttribute(PERCEPTION) + GetAttribute(INTELLIGENCE) + GetAttribute(WISDOM) + GetAttribute(CHARISMA) + GetAttribute(ARM_STRENGTH) + GetAttribute(AGILITY) + GetAttribute(LEG_STRENGTH) + GetAttribute(DEXTERITY) ;
}

bool humanoid::CheckConsume(const festring& Verb) const
{
  if(!HasHead())
    {
      if(IsPlayer())
	ADD_MESSAGE("You need a head to %s.", Verb.CStr());

      return false;
    }

  return character::CheckConsume(Verb);
}

bool humanoid::CanConsume(material* Material) const
{
  return HasHead() && character::CanConsume(Material);
}

void femaleslave::BeTalkedTo()
{
  static ulong Said;

  if(GetConfig() != NEW_ATTNAM || GetRelation(PLAYER) == HOSTILE)
    humanoid::BeTalkedTo();
  else if(!game::TweraifIsFree())
    ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, 4)]);
  else
    ProcessAndAddMessage(GetFriendlyReplies()[4 + RandomizeReply(Said, 3)]);
}

void necromancer::GetAICommand()
{
  character* NearestEnemy = 0;
  ulong NearestEnemyDistance = 0xFFFFFFFF;
  character* RandomFriend = 0;
  vector2d Pos = GetPos();

  /* Look for zombie candidates */

  if(SpellsLeft && RaiseDeadAsZombies())
    {
      --SpellsLeft;
      return;
    }

  /* if no zombies can be created just rise some skeletons or just walk */

  switch(RAND() % 10)
    {
    case 0:
      if(SpellsLeft)
	{
	  character* BoneMan = RaiseSkeleton();
	  if(BoneMan)
	    {
	      --SpellsLeft;	  
	      if(BoneMan->CanBeSeenByPlayer())
		{
		  ADD_MESSAGE("The ground shakes and %s emerges from it.", BoneMan->CHAR_NAME(DEFINITE));
		}
	      return;
	    }
	}

    case 1:
      if(GetConfig() == MASTER_NECROMANCER)
	{
	  character* NearestEnemy = 0;
	  ulong NearestEnemyDistance = 0xFFFFFFFF;
	  for(ushort c = 0; c < game::GetTeams(); ++c)
	    {
	      if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
		{
		  for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
		    if((*i)->IsEnabled())
		      {
			ulong ThisDistance = Max<ulong>(abs((*i)->GetPos().X - Pos.X), abs((*i)->GetPos().Y - Pos.Y));

			if((ThisDistance < NearestEnemyDistance || (ThisDistance == NearestEnemyDistance && !(RAND() % 3))) && (*i)->CanBeSeenBy(this))
			  {
			    NearestEnemy = *i;
			    NearestEnemyDistance = ThisDistance;
			  }
		      }
		}
	    }
	  if(NearestEnemyDistance > 7)
	    {
	      SetGoingTo(NearestEnemy->GetPos());
	      MoveTowardsTarget();
	    }
	  else
	    {

	      lsquare* Square = NearestEnemy->GetLSquareUnder();
	      Square->DrawParticles(RED); 
	      if(CanBeSeenByPlayer())
		ADD_MESSAGE("%s invokes a spell.", CHAR_NAME(DEFINITE));

	      Square->Strike(this, CONST_S("killed by the spells of ") + GetName(INDEFINITE), YOURSELF);
	    }
	}
    case 2:
      if(CheckForDoors())
	return;
      
      if(MoveRandomly())
	return;
    }
  
}



bool necromancer::RaiseDeadAsZombies()
{
  for(ushort d = 0; d < GetNeighbourSquares(); ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square)
	{
	  for(stackiterator si = Square->GetStack()->GetBottom(); si.HasItem(); ++si)
	    {
	      character* Risen;
	      if((Risen = si->TryNecromancy(this)) && Risen->CanBeSeenByPlayer())
		{
		  Risen->SetTeam(GetTeam());
		  ADD_MESSAGE("%s calls %s back to life.", CHAR_NAME(DEFINITE), Risen->CHAR_NAME(DEFINITE));
		  EditAP(-500);
		  return true;
		}
	    }

	}
    }
  return false;
}


character* necromancer::RaiseSkeleton()
{
  for(ushort c = 0; c < 100; ++c)
    {
      lsquare* Square = GetNeighbourLSquare(RAND() % DIRECTION_COMMAND_KEYS);

      if(Square && !Square->GetCharacter())
	{
	  character* Monster;

	  switch(RAND() % 10)
	    {
	    case 0:
	      Monster = new skeleton(WARRIOR);
	      break;
	    default:
	      Monster = new skeleton;
	    }

	  Monster->SetTeam(GetTeam());
	  if(Monster->CanMoveOn(Square) && Monster->IsFreeForMe(Square))
	    {
	      Monster->PutTo(Square);
	      EditAP(-4000 / GetConfig());
	      return Monster;
	    }
	  else
	    delete Monster; 
	}
    }

  return 0;
}

character* humanoid::TryToRiseFromTheDeadAsZombie()
{
  return new zombie;

  // zombie shouldn't have bodyparts this has... Correct please! 
}


void necromancer::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << SpellsLeft;
}

void necromancer::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> SpellsLeft;
}

void necromancer::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  if(!Load)
    {
      if(GetConfig() == MASTER_NECROMANCER)
	SpellsLeft = RAND_N(31) + 20;
      else
	SpellsLeft = RAND_N(16) + 10;
    }
}

void humanoid::StayOn(liquid* Liquid)
{
  bool Standing = false;

  if(GetRightLeg())
    {
      GetRightLeg()->StayOn(Liquid);
      Standing = true;
    }

  if(IsEnabled() && GetLeftLeg())
    {
      GetLeftLeg()->StayOn(Liquid);
      Standing = true;
    }

  if(!Standing)
    {
      bodypart* BodyPart[MAX_BODYPARTS];
      ushort Index = 0;

      for(ushort c = 0; c < GetBodyParts(); ++c)
	if(GetBodyPart(c))
	  BodyPart[Index++] = GetBodyPart(c);

      BodyPart[RAND() % Index]->StayOn(Liquid);
    }
}

bodypart* playerkind::MakeBodyPart(ushort Index) const
{
  switch(Index)
    {
    case TORSO_INDEX: return new playerkindtorso(0, NO_MATERIALS);
    case HEAD_INDEX: return new playerkindhead(0, NO_MATERIALS);
    case RIGHT_ARM_INDEX: return new playerkindrightarm(0, NO_MATERIALS);
    case LEFT_ARM_INDEX: return new playerkindleftarm(0, NO_MATERIALS);
    case GROIN_INDEX: return new playerkindgroin(0, NO_MATERIALS);
    case RIGHT_LEG_INDEX: return new playerkindrightleg(0, NO_MATERIALS);
    case LEFT_LEG_INDEX: return new playerkindleftleg(0, NO_MATERIALS);
    }

  ABORT("Weird bodypart to make for a playerkind. It must be your fault!");
  return 0;
}

bool golem::AddAdjective(festring& String, bool Articled) const
{
  ushort TotalRustLevel = 0;

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      TotalRustLevel += GetBodyPart(c)->GetMainMaterial()->GetRustLevel();

  if(!TotalRustLevel)
    return humanoid::AddAdjective(String, Articled);
  else
    {
      if(Articled)
	String << 'a';

      if(TotalRustLevel <= GetBodyParts())
	String << "slightly rusted ";
      else if(TotalRustLevel <= GetBodyParts() << 1)
	String << "rusted ";
      else
	String << "very rusted ";

      String << GetAdjective() << ' ';
      return true;
    }
}

void oree::Bite(character* Enemy, vector2d HitPos, uchar, bool)
{
  if(IsPlayer())
    ADD_MESSAGE("You vomit acidous blood at %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
  else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    ADD_MESSAGE("%s vomits acidous blood at %s.", CHAR_DESCRIPTION(DEFINITE), Enemy->CHAR_DESCRIPTION(DEFINITE));

  Vomit(HitPos, 500 + RAND() % 500, false);
}
