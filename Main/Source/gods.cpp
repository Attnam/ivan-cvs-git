/* Compiled through godset.cpp */

const char* valpurus::GetName() const { return "Valpurus"; }
const char* valpurus::GetDescription() const { return "King of Gods"; }
uchar valpurus::GetAlignment() const { return ALPP; }
uchar valpurus::GetBasicAlignment() const { return GOOD; }
ushort valpurus::GetColor() const { return MakeRGB16(160, 160, 0); }

const char* legifer::GetName() const { return "Legifer"; }
const char* legifer::GetDescription() const { return "god of law and order"; }
uchar legifer::GetAlignment() const { return ALP; }
uchar legifer::GetBasicAlignment() const { return GOOD; }
ushort legifer::GetColor() const { return MakeRGB16(160, 160, 0); }

const char* atavus::GetName() const { return "Atavus"; }
const char* atavus::GetDescription() const { return "god of charity and munificence"; }
uchar atavus::GetAlignment() const { return ALP; }
uchar atavus::GetBasicAlignment() const { return GOOD; }
ushort atavus::GetColor() const { return MakeRGB16(160, 160, 0); }

const char* dulcis::GetName() const { return "Dulcis"; }
const char* dulcis::GetDescription() const { return "goddess of love and art"; }
uchar dulcis::GetAlignment() const { return AL; }
uchar dulcis::GetBasicAlignment() const { return GOOD; }
ushort dulcis::GetColor() const { return MakeRGB16(160, 160, 0); }

const char* seges::GetName() const { return "Seges"; }
const char* seges::GetDescription() const { return "goddess of health and nutrition"; }
uchar seges::GetAlignment() const { return AL; }
uchar seges::GetBasicAlignment() const { return GOOD; }
ushort seges::GetColor() const { return MakeRGB16(160, 160, 0); }

const char* sophos::GetName() const { return "Sophos"; }
const char* sophos::GetDescription() const { return "god of knowledge and magic"; }
uchar sophos::GetAlignment() const { return ALM; }
uchar sophos::GetBasicAlignment() const { return GOOD; }
ushort sophos::GetColor() const { return MakeRGB16(160, 160, 0); }

const char* silva::GetName() const { return "Silva"; }
const char* silva::GetDescription() const { return "goddess of nature"; }
uchar silva::GetAlignment() const { return ANP; }
uchar silva::GetBasicAlignment() const { return NEUTRAL; }
ushort silva::GetColor() const { return MakeRGB16(20, 120, 200); }

const char* loricatus::GetName() const { return "Loricatus"; }
const char* loricatus::GetDescription() const { return "god of fire, machines and weaponry"; }
uchar loricatus::GetAlignment() const { return AN; }
uchar loricatus::GetBasicAlignment() const { return NEUTRAL; }
ushort loricatus::GetColor() const { return MakeRGB16(20, 120, 200); }

const char* mellis::GetName() const { return "Mellis"; }
const char* mellis::GetDescription() const { return "god of money, trade and politics"; }
uchar mellis::GetAlignment() const { return ANM; }
uchar mellis::GetBasicAlignment() const { return NEUTRAL; }
ushort mellis::GetColor() const { return MakeRGB16(20, 120, 200); }

const char* cleptia::GetName() const { return "Cleptia"; }
const char* cleptia::GetDescription() const { return "goddess of assassins and thieves"; }
uchar cleptia::GetAlignment() const { return ACP; }
uchar cleptia::GetBasicAlignment() const { return EVIL; }
ushort cleptia::GetColor() const { return MakeRGB16(200, 0, 0); }

const char* nefas::GetName() const { return "Nefas"; }
const char* nefas::GetDescription() const { return "goddess of forbidden pleasures"; }
uchar nefas::GetAlignment() const { return AC; }
uchar nefas::GetBasicAlignment() const { return EVIL; }
ushort nefas::GetColor() const { return MakeRGB16(200, 0, 0); }

const char* scabies::GetName() const { return "Scabies"; }
const char* scabies::GetDescription() const { return "goddess of mutations, disease and famine"; }
uchar scabies::GetAlignment() const { return AC; }
uchar scabies::GetBasicAlignment() const { return EVIL; }
ushort scabies::GetColor() const { return MakeRGB16(200, 0, 0); }

const char* infuscor::GetName() const { return "Infuscor"; }
const char* infuscor::GetDescription() const { return "goddess of wrong knowledge and vile magic"; }
uchar infuscor::GetAlignment() const { return ACM; }
uchar infuscor::GetBasicAlignment() const { return EVIL; }
ushort infuscor::GetColor() const { return MakeRGB16(200, 0, 0); }

const char* cruentus::GetName() const { return "Cruentus"; }
const char* cruentus::GetDescription() const { return "god of war and blood"; }
uchar cruentus::GetAlignment() const { return ACM; }
uchar cruentus::GetBasicAlignment() const { return EVIL; }
ushort cruentus::GetColor() const { return MakeRGB16(200, 0, 0); }

const char* mortifer::GetName() const { return "Mortifer"; }
const char* mortifer::GetDescription() const { return "Destroyer of Worlds"; }
uchar mortifer::GetAlignment() const { return ACMM; }
uchar mortifer::GetBasicAlignment() const { return EVIL; }
ushort mortifer::GetColor() const { return MakeRGB16(200, 0, 0); }

void sophos::PrayGoodEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement! You teleport away!");
  PLAYER->Move(game::GetCurrentLevel()->GetRandomSquare(PLAYER), true);
}

void sophos::PrayBadEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement!");
  PLAYER->TeleportSomePartsAway(1 + (RAND() & 1));
  PLAYER->CheckDeath(std::string("shattered to pieces by the wrath of ") + GetName(), 0);
}

void valpurus::PrayGoodEffect()
{
  if(RAND() & 1)
    {
      ADD_MESSAGE("You hear booming voice: \"THIS WILL PROTECT THEE FROM MORTIFER, MY PALADIN!\" A shield glittering with holy might appears from nothing.");
      shield* Shield = new shield;
      Shield->InitMaterials(MAKE_MATERIAL(VALPURIUM));
      PLAYER->GetGiftStack()->AddItem(Shield);
    }
  else
    {
      ADD_MESSAGE("You hear booming voice: \"DEFEAT MORTIFER WITH THIS, MY PALADIN!\" A sword glittering with holy might appears from nothing.");
      meleeweapon* Weapon = new meleeweapon(TWO_HANDED_SWORD);
      Weapon->InitMaterials(MAKE_MATERIAL(VALPURIUM), MAKE_MATERIAL(VALPURIUM), 0);
      PLAYER->GetGiftStack()->AddItem(Weapon);
    }
}

void valpurus::PrayBadEffect()
{
  ADD_MESSAGE("Valpurus smites you with a small hammer.");
  PLAYER->ReceiveDamage(0, 10, PHYSICAL_DAMAGE, HEAD, RAND() & 7);
  PLAYER->CheckDeath(std::string("faced the hammer of Justice from the hand of ") + GetName(), 0);
}

void legifer::PrayGoodEffect()
{
  ADD_MESSAGE("A booming voice echoes: \"Xunil! Xunil! Save us!\" A huge firestorm engulfs everything around you.");
  game::GetCurrentLevel()->Explosion(PLAYER, std::string("killed accidentally by ") + GetName(), PLAYER->GetPos(), 40, false);
}

void legifer::PrayBadEffect()
{
  ADD_MESSAGE("%s casts a beam of horrible, yet righteous, fire on you.", GetName());
  PLAYER->ReceiveDamage(0, 20 + RAND() % 20, FIRE, ALL);
  PLAYER->CheckDeath(std::string("burned to death by the wrath of ") + GetName(), 0);
}

void dulcis::PrayGoodEffect()
{
  ADD_MESSAGE("A beautiful melody echoes around you.");

  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = PLAYER->GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char)
	    if(Char->IsCharmable())
	      if(PLAYER->GetRelativeDanger(Char) > 2.0f)
		{
		  if(Char->GetTeam() == PLAYER->GetTeam())
		    ADD_MESSAGE("%s seems to be very happy.", Char->CHAR_NAME(DEFINITE));
		  else if(Char->GetRelation(PLAYER) == HOSTILE)
		    ADD_MESSAGE("%s stops fighting.", Char->CHAR_NAME(DEFINITE));
		  else
		    ADD_MESSAGE("%s seems to be very friendly towards you.", Char->CHAR_NAME(DEFINITE));

		  Char->ChangeTeam(PLAYER->GetTeam());
		}
	      else
		ADD_MESSAGE("%s resists its charming call.", Char->CHAR_NAME(DEFINITE));
	    else
	      ADD_MESSAGE("%s seems not affected.", Char->CHAR_NAME(DEFINITE));
	}
    }
}

void dulcis::PrayBadEffect()
{
  ADD_MESSAGE("%s plays a horrible tune that rots your brain.", GetName());
  PLAYER->ReceiveDamage(0, 1 + RAND() % 9, SOUND, HEAD);
  PLAYER->CheckDeath(std::string("became insane by listening ") + GetName() + " too much", 0);
}

void seges::PrayGoodEffect()
{
  if(!PLAYER->HasAllBodyParts())
    {
      bodypart* OldBodyPart = PLAYER->FindRandomOwnBodyPart();

      if(OldBodyPart != 0)
	{
	  OldBodyPart->RemoveFromSlot();
	  PLAYER->AttachBodyPart(OldBodyPart);
	  OldBodyPart->SetHP(OldBodyPart->GetMaxHP());
	  ADD_MESSAGE("%s attaches your old %s back and heals it.", GetName(), OldBodyPart->GetBodyPartName().c_str());
	}
      else
	{
	  bodypart* NewBodyPart = PLAYER->GenerateRandomBodyPart();
	  NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
	  ADD_MESSAGE("You grow a new %s.", NewBodyPart->GetBodyPartName().c_str()); 
	}

      return;
    }

  if(PLAYER->IsInBadCondition())
    {
      ADD_MESSAGE("%s cures your wounds.", GetName());
      PLAYER->RestoreLivingHP();
      return;
    }

  if(PLAYER->TemporaryStateIsActivated(POISONED))
    {
      ADD_MESSAGE("%s removes the foul liquid in your veins.", GetName());
      PLAYER->DeActivateTemporaryState(POISONED);
      return;
    }

  if(PLAYER->GetNP() < SATIATED_LEVEL)
    {
      ADD_MESSAGE("Your stomach feels full again.");

      if(PLAYER->GetNP() < SATIATED_LEVEL)
	PLAYER->SetNP(SATIATED_LEVEL);
    }
}

void seges::PrayBadEffect()
{
  if(PLAYER->UsesNutrition())
    {
      ADD_MESSAGE("You feel Seges altering the contents of your stomach in an eerie way.");
      PLAYER->EditNP(-10000);
      PLAYER->CheckStarvationDeath(std::string("starved by ") + GetName());
    }
  else
    ADD_MESSAGE("Seges tries to alter the contents of your stomach, but fails.");
}

void atavus::PrayGoodEffect()
{
  if(!Timer && Relation > 500 + RAND_N(500))
    {
      item* Reward = new bodyarmor(PLATE_MAIL, NO_MATERIALS);
      Reward->InitMaterials(MAKE_MATERIAL(ARCANITE));
      ADD_MESSAGE("%s materializes before you.", Reward->CHAR_NAME(INDEFINITE));
      PLAYER->GetGiftStack()->AddItem(Reward);
      AdjustTimer(45000);
      AdjustRelation(-300);
    }
  else if(!PLAYER->HasAllBodyParts())
    {
      bodypart* NewBodyPart = PLAYER->GenerateRandomBodyPart();
      NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
      ADD_MESSAGE("You gives you a new %s as a gift between friends.", NewBodyPart->GetBodyPartName().c_str());
    }
  else
    ADD_MESSAGE("Nothing happens.");
}

void atavus::PrayBadEffect()
{
  ADD_MESSAGE("You have not been good the whole year.");

  if(PLAYER->GetStack()->GetItems())
    {
      ushort ToBeDeleted = RAND() % PLAYER->GetStack()->GetItems();
      item* Disappearing = PLAYER->GetStack()->GetItem(ToBeDeleted);

      if(Disappearing->IsDestroyable())
	{
	  ADD_MESSAGE("Your %s disappears.", Disappearing->CHAR_NAME(UNARTICLED));
	  Disappearing->RemoveFromSlot();
	  Disappearing->SendToHell();
	}
      else
	{
	  ADD_MESSAGE("%s tries to remove your %s, but fails. You feel you are not so gifted anymore.", GetName(), Disappearing->CHAR_NAME(UNARTICLED));
	  PLAYER->EditAttribute(AGILITY, -1);
	  PLAYER->EditAttribute(ARM_STRENGTH, -1);
	  PLAYER->EditAttribute(ENDURANCE, -1);
	}
    }
  else
    {
      ADD_MESSAGE("You feel you are not so gifted anymore.");
      PLAYER->EditAttribute(AGILITY, -1);
      PLAYER->EditAttribute(ARM_STRENGTH, -1);
      PLAYER->EditAttribute(ENDURANCE, -1);
    }
}

void silva::PrayGoodEffect()
{
  if(!PLAYER->HasAllBodyParts())
    {
      bodypart* OldBodyPart = PLAYER->FindRandomOwnBodyPart();

      if(OldBodyPart != 0)
	{
	  OldBodyPart->RemoveFromSlot();
	  PLAYER->AttachBodyPart(OldBodyPart);
	  OldBodyPart->SetHP(1);
	  ADD_MESSAGE("%s attaches your old %s back.", GetName(), OldBodyPart->GetBodyPartName().c_str());
	}
      else
	{
	  bodypart* NewBodyPart = PLAYER->GenerateRandomBodyPart();
	  NewBodyPart->SetHP(1);
	  ADD_MESSAGE("You grow a new %s.", NewBodyPart->GetBodyPartName().c_str()); 
	}

      return;
    }

  if(PLAYER->GetNP() < HUNGER_LEVEL)
    {
      ADD_MESSAGE("Your stomach feels full again.");
      PLAYER->SetNP(SATIATED_LEVEL);
    }

  if(!game::GetCurrentLevel()->IsOnGround())
    {
      ADD_MESSAGE("Suddenly a horrible earthquake shakes the level.");
      ushort c, Tunnels = 2 + RAND() % 3;

      for(c = 0; c < Tunnels; ++c)
	game::GetCurrentLevel()->AttachPos(game::GetCurrentLevel()->GetRandomSquare(0, NOT_WALKABLE|ATTACHABLE));

      uchar ToEmpty = 10 + RAND() % 11;

      for(c = 0; c < ToEmpty; ++c)
	for(ushort i = 0; i < 50; ++i)
	  {
	    vector2d Pos = game::GetCurrentLevel()->GetRandomSquare(0, NOT_WALKABLE);
	    bool Correct = false;

	    for(ushort d = 0; d < 8; ++d)
	      {
		lsquare* Square = game::GetCurrentLevel()->GetNeighbourLSquare(Pos, d);

		if(Square && Square->IsFlyable())
		  {
		    Correct = true;
		    break;
		  }
	      }

	    if(Correct)
	      {
		game::GetCurrentLevel()->GetLSquare(Pos)->ChangeOLTerrainAndUpdateLights(0);

		for(ushort p = 0; p < 4; ++p)
		  game::GetCurrentLevel()->GetLSquare(Pos)->GetSideStack(p)->Clean();

		break;
	      }
	  }

      ushort ToGround = 20 + RAND() % 21;

      for(c = 0; c < ToGround; ++c)
	for(ushort i = 0; i < 50; ++i)
	  {
	    vector2d Pos = game::GetCurrentLevel()->GetRandomSquare(0, RAND() & 1 ? 0 : HAS_CHARACTER);
	    lsquare* Square = game::GetCurrentLevel()->GetLSquare(Pos);
	    character* Char = Square->GetCharacter();

	    if(Square->GetOLTerrain() || (Char && (Char->IsPlayer() || PLAYER->GetRelation(Char) != HOSTILE)))
	      continue;

	    ushort Walkables = 0;

	    for(ushort d = 0; d < 8; ++d)
	      {
		lsquare* NearSquare = game::GetCurrentLevel()->GetNeighbourLSquare(Pos, d);

		if(NearSquare && NearSquare->IsFlyable())
		  ++Walkables;
	      }

	    if(Walkables > 6)
	      {
		Square->ChangeOLTerrainAndUpdateLights(new wall(EARTH));

		if(Char)
		  {
		    if(Char->CanBeSeenByPlayer())
		      ADD_MESSAGE("%s is hit by a brick of earth falling from the roof!", Char->CHAR_NAME(DEFINITE));

		    Char->ReceiveDamage(0, 20 + RAND() % 21, PHYSICAL_DAMAGE, HEAD|TORSO, 8, true);
		    Char->CheckDeath("killed by an earthquake", 0);
		  }

		Square->KickAnyoneStandingHereAway();

		ushort p;

		for(p = 0; p < 4; ++p)
		  Square->GetSideStack(p)->Clean();

		Square->GetStack()->ReceiveDamage(0, 10 + RAND() % 41, PHYSICAL_DAMAGE);
		break;
	      }
	  }

      // Generate a few boulders in the level

      ushort BoulderNumber = 10 + RAND() % 10;

      for(c = 0; c < BoulderNumber; ++c)
	{
	    vector2d Pos = game::GetCurrentLevel()->GetRandomSquare();
	    lsquare* Square = game::GetCurrentLevel()->GetLSquare(Pos);
	    character* MonsterHere = Square->GetCharacter();

	    if(!MonsterHere || MonsterHere->GetRelation(PLAYER) == HOSTILE)
	      {
		Square->ChangeOLTerrainAndUpdateLights(new boulder(1 + (RAND() & 1)));

		if(MonsterHere)
		  MonsterHere->ReceiveDamage(0, 10 + RAND() % 10, PHYSICAL_DAMAGE, HEAD|TORSO, 8, true);

		Square->GetStack()->ReceiveDamage(0, 10 + RAND() % 10, PHYSICAL_DAMAGE);
	      }
	}

      // Impact damage to items in the level

      for(ushort x = 0; x < game::GetCurrentLevel()->GetXSize(); ++x)
	for(ushort y = 0; y < game::GetCurrentLevel()->GetYSize(); ++y)
	  game::GetCurrentLevel()->GetLSquare(x, y)->ReceiveEarthQuakeDamage();
    }
  else
    {
      ushort TryToCreate = 1 + RAND() % 7;
      ushort Created = 0;

      for(; Created < TryToCreate; ++Created)
	{
	  wolf* Wolf = new wolf;
	  vector2d Pos = game::GetCurrentLevel()->GetNearestFreeSquare(Wolf, PLAYER->GetPos());

	  if(Pos == ERROR_VECTOR)
	    {
	      delete Wolf;
	      break;
	    }

	  Wolf->SetTeam(PLAYER->GetTeam());
	  game::GetCurrentLevel()->AddCharacter(Pos, Wolf);
	}

      if(!Created)
	ADD_MESSAGE("You hear a sad howling of a wolf imprisoned in the earth.");

      if(Created == 1)
	ADD_MESSAGE("Suddenly a tame wolf materializes beside you.");

      if(Created > 1)
	ADD_MESSAGE("Suddenly some tame wolves materialize around you.");
    }
}

void silva::PrayBadEffect()
{
  switch(RAND() % 3)
    {
    case 0:
      PLAYER->Polymorph(new largerat, 1000 + RAND() % 1000);
      break;
    case 1:
      PLAYER->Polymorph(new ass, 1000 + RAND() % 1000);
      break;
    case 2:
      PLAYER->Polymorph(new jackal, 1000 + RAND() % 1000);
      break;
    }
}

void loricatus::PrayGoodEffect()
{
  if(!PLAYER->HasAllBodyParts())
    {
      bodypart* NewBodyPart = PLAYER->GenerateRandomBodyPart();
      NewBodyPart->ChangeMainMaterial(MAKE_MATERIAL(STEEL));
      NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
      ADD_MESSAGE("You grow a new %s that is made of steel.", NewBodyPart->GetBodyPartName().c_str());
      return;
    }

  for(ushort c = 0; c < PLAYER->GetEquipmentSlots(); ++c)
    if(PLAYER->GetEquipment(c) && PLAYER->GetEquipment(c)->IsBroken())
      {
	ADD_MESSAGE("%s fixes your %s.", GetName(), PLAYER->GetEquipment(c)->CHAR_NAME(UNARTICLED));
	PLAYER->GetEquipment(c)->Fix();
	return;
      }

  item* MainWielded = PLAYER->GetMainWielded();
	
  if(MainWielded)
    {
      if(MainWielded->IsMaterialChangeable())
	{
	  ushort StrengthValue = protocontainer<material>::GetProto(1)->GetConfig().find(STEEL)->second.StrengthValue;

	  if(StrengthValue > MainWielded->GetMainMaterial()->GetStrengthValue())
	    {
	      std::string Desc;
	      item* SecondaryWielded;

	      if(MainWielded->HandleInPairs() && (SecondaryWielded = PLAYER->GetSecondaryWielded()) && SecondaryWielded->CanBePiledWith(MainWielded, PLAYER))
		{
		  MainWielded->AddName(Desc, PLURAL);
		  Desc << " glow and sparkle like they were";
		  SecondaryWielded->ChangeMainMaterial(MAKE_MATERIAL(STEEL));
		}
	      else
		{
		  MainWielded->AddName(Desc, UNARTICLED);
		  Desc << " glows and sparkles like it was";
		}

	      MainWielded->ChangeMainMaterial(MAKE_MATERIAL(STEEL));
	      ADD_MESSAGE("Your %s reforged by invisible hands.", Desc.c_str());
	    }
	  else
	    {
	      if(!(RAND() % 10))
		{
		  item* Scroll = new scrollofrepair;
		  ADD_MESSAGE("%s gives you %s.", GetName(), Scroll->CHAR_NAME(INDEFINITE));
		  PLAYER->GetGiftStack()->AddItem(Scroll);
		}
	      else
		ADD_MESSAGE("\"Mortal, thou art always my valiant knight!\"");  
	    }
	  return;
	}
      else
	ADD_MESSAGE("%s emits strange light but remains unchanged.", MainWielded->CHAR_NAME(DEFINITE));
    }
  else
    ADD_MESSAGE("You feel a slight tingling in your hands.");
}

void loricatus::PrayBadEffect()
{
  item* MainWielded = PLAYER->GetMainWielded();

  if(MainWielded)
    if(MainWielded->IsMaterialChangeable())
      {
	std::string Desc;
	item* SecondaryWielded;

	if(MainWielded->HandleInPairs() && (SecondaryWielded = PLAYER->GetSecondaryWielded()) && SecondaryWielded->CanBePiledWith(MainWielded, PLAYER))
	  {
	    MainWielded->AddName(Desc, PLURAL);
	    Desc << " vibrate and soften";
	    SecondaryWielded->ChangeMainMaterial(MAKE_MATERIAL(BANANA_FLESH));
	  }
	else
	  {
	    MainWielded->AddName(Desc, UNARTICLED);
	    Desc << " vibrates and softens";
	  }

	MainWielded->ChangeMainMaterial(MAKE_MATERIAL(BANANA_FLESH));
	ADD_MESSAGE("Your %s.", Desc.c_str());
      }
    else
      ADD_MESSAGE("%s emits strange light but remain unchanged.", MainWielded->CHAR_NAME(DEFINITE));
  else
    ADD_MESSAGE("You feel a slight tingling in your hands.");
}

void cleptia::PrayGoodEffect()
{
  if(!PLAYER->StateIsActivated(HASTE))
    {
      ADD_MESSAGE("%s gives you the talent for speed.", GetName());
      PLAYER->BeginTemporaryState(HASTE, 250);
      return;
    }

  if(!PLAYER->StateIsActivated(INVISIBLE))
    {
      ADD_MESSAGE("%s helps you to avoid your enemies by making you invisible.", GetName());
      PLAYER->BeginTemporaryState(INVISIBLE, 250);
      return;
    }

  ADD_MESSAGE("Cleptia helps you, but you really don't know how.");  
  ushort StateToActivate = RAND() & 1 ? HASTE : INVISIBLE;
  PLAYER->BeginTemporaryState(StateToActivate, 250);
}

void cleptia::PrayBadEffect()
{
  ADD_MESSAGE("%s slows you down.", GetName());
  PLAYER->BeginTemporaryState(SLOW, 250);
}

void mortifer::PrayGoodEffect()
{
  ADD_MESSAGE("The air vibrates violently around you. A terrible undead voice echoes through the caverns: \"SlAvE! ThOu HaSt PlAeSeD mE! lIfT tHy ReWaRd, ChAmPiOn!\" A heavy weapon of pure corruption materializes before you.");
  PLAYER->GetGiftStack()->AddItem(new neercseulb);
}

void mortifer::PrayBadEffect()
{
  ADD_MESSAGE("A dark, booming voice shakes the area: \"PuNy MoRtAl! ThOu ArT nOt WoRtHy! I sHaLl dEsTrOy ThEe LiKe EvErYoNe ElSe!\" A bolt of black energy hits you.");
  PLAYER->ReceiveDamage(0, 1 + RAND() % 20, ENERGY, ALL);
  PLAYER->EditAttribute(AGILITY, -1);
  PLAYER->EditAttribute(ARM_STRENGTH, -1);
  PLAYER->EditAttribute(ENDURANCE, -1);
  PLAYER->CheckDeath(std::string("obliterated by the unholy power of ") + GetName(), 0);
}

void mellis::PrayGoodEffect()
{
  bool Success = false;
  itemvector OKItems;

  for(stackiterator i = PLAYER->GetStack()->GetBottom(); i.HasItem(); ++i)
    {
      if(!i->HasBetterVersion())
	continue;
	  
      OKItems.push_back(*i);
      Success = true;
    }

  item* NewVersion;

  for(ushort c = 0; !OKItems.empty() && c < 5; ++c)
    {
      item* ToBeDeleted = OKItems[RAND() % OKItems.size()];
      NewVersion = ToBeDeleted->BetterVersion();
      ADD_MESSAGE("%s manages to trade %s into %s.", GetName(), ToBeDeleted->CHAR_NAME(DEFINITE), NewVersion->CHAR_NAME(INDEFINITE));
      PLAYER->GetStack()->AddItem(NewVersion);
      ToBeDeleted->RemoveFromSlot();
      ToBeDeleted->SendToHell();
      OKItems.erase(std::find(OKItems.begin(), OKItems.end(), ToBeDeleted));
    }

  if((Success && !(RAND() % 5)) || (!Success && !(RAND() % 3)))
    {
      uchar Possible[GODS];
      ushort PossibleSize = 0;

      for(uchar c = 1; c <= GODS; ++c)
	if(!game::GetGod(c)->IsKnown())
	  Possible[PossibleSize++] = c;

      if(PossibleSize)
	{
	  uchar NewKnownGod = Possible[RAND() % PossibleSize];
	  game::GetGod(NewKnownGod)->SetIsKnown(true);
	  ADD_MESSAGE("%s shares his knowledge of %s, the %s.", GetName(), game::GetGod(NewKnownGod)->GetName(), game::GetGod(NewKnownGod)->GetDescription());
	  return;
	}
    }

  if(!Success)
    ADD_MESSAGE("Nothing happens.");
}

void mellis::PrayBadEffect()
{
  for(ushort c = 1; c <= GODS; ++c)
    if(c != GetType())
      game::GetGod(c)->AdjustRelation(-100);

  ADD_MESSAGE("%s spreads bad rumours about you to other gods.", GetName());
}

void valpurus::Pray()
{
  if(!Timer && Relation == 1000)
    {
      ADD_MESSAGE("You feel %s is very pleased.", GetName());
      PrayGoodEffect();
      AdjustTimer(100000);
      AdjustRelation(-500);
      game::ApplyDivineAlignmentBonuses(this, true);
      PLAYER->EditExperience(WISDOM, 1000);

      if(Relation > 500 && !(RAND() % 5))
	{
	  character* Angel = CreateAngel();

	  if(Angel)
	    {
	      Angel->SetTeam(PLAYER->GetTeam());
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
	    }
	}
    }
  else
    {
      ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GetName());
      PrayBadEffect();
      AdjustTimer(50000);
      AdjustRelation(-100);
      game::ApplyDivineAlignmentBonuses(this, false);
      PLAYER->EditExperience(WISDOM, -250);

      if(Relation < -500 && !(RAND() % 25))
	{
	  character* Angel = CreateAngel();

	  if(Angel)
	    {
	      Angel->SetTeam(game::GetTeam(4));
	      ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
	    }
	}
    }
}

void mortifer::Pray()
{
  if(!Timer && Relation == 1000)
    {
      ADD_MESSAGE("You feel %s is very pleased.", GetName());
      PrayGoodEffect();
      AdjustTimer(100000);
      AdjustRelation(-500);
      game::ApplyDivineAlignmentBonuses(this, true);
      PLAYER->EditExperience(WISDOM, 1000);

      if(Relation > 500 && !(RAND() % 5))
	{
	  character* Angel = CreateAngel();

	  if(Angel)
	    {
	      Angel->SetTeam(PLAYER->GetTeam());
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_NAME(DEFINITE));
	    }
	}
    }
  else
    {
      ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GetName());
      PrayBadEffect();
      AdjustTimer(50000);
      AdjustRelation(-100);
      game::ApplyDivineAlignmentBonuses(this, false);
      PLAYER->EditExperience(WISDOM, -250);

      if(Relation < -500 && !(RAND() % 5))
	{
	  character* Angel = CreateAngel();

	  if(Angel)
	    {
	      Angel->SetTeam(game::GetTeam(4));
	      ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_NAME(DEFINITE));
	    }
	}
    }
}

void infuscor::PrayBadEffect()
{
  ADD_MESSAGE("Vile and evil knowledge pulps into your brain. It's too much for it to handle; you faint.");
  PLAYER->Faint(1000 + RAND() % 1000);
}

void nefas::PrayGoodEffect()
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, game::GetCurrentLevel()->GetBorder(), PLAYER->GetPos(), 10);
  bool AudiencePresent = false;

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    {
      for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
	{
	  character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

	  if(Audience && Audience->CanBeSeenByPlayer() && !Audience->TemporaryStateIsActivated(CONFUSED) && Audience->CanBeConfused() && PLAYER->GetRelation(Audience) == HOSTILE)
	    {
	      AudiencePresent = true;
	      break;
	    }
	}

      if(AudiencePresent)
	break;
    }

  if(AudiencePresent)
    {
      for(ushort x = Rect.X1; x <= Rect.X2; ++x)
	for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
	  {
	    character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

	    if(Audience && !Audience->TemporaryStateIsActivated(CONFUSED) && Audience->CanBeConfused() && PLAYER->GetRelation(Audience) == HOSTILE)
	      {
		if(Audience->CanBeSeenByPlayer())
		  ADD_MESSAGE("%s confuses %s with her sweet lies.", GetName(), Audience->CHAR_NAME(DEFINITE));

		Audience->BeginTemporaryState(CONFUSED, 500 + RAND() % 500);
	      }
	  }
    }
  else if(RAND() % 5)
    {
      mistress* Mistress = new mistress(RAND() & 7 ? 0 : TORTURING_CHIEF);
      vector2d Where = game::GetCurrentLevel()->GetNearestFreeSquare(Mistress, PLAYER->GetPos());

      if(Where == ERROR_VECTOR)
	{
	  ADD_MESSAGE("You hear a strange scream from somewhere beneath.");
	  delete Mistress;
	}
      else
	{
	  Mistress->SetTeam(PLAYER->GetTeam());
	  game::GetCurrentLevel()->AddCharacter(Where, Mistress);
	  ADD_MESSAGE("You hear a sweet voice inside your head: \"Have fun, mortal!\"");
	}
    }
  else
    {
      ADD_MESSAGE("%s wishes you to have fun with this potion.", GetName());
      potion* Reward = new potion(0, NO_MATERIALS);
      Reward->InitMaterials(MAKE_MATERIAL(GLASS), MAKE_MATERIAL(OMMEL_URINE));
      PLAYER->GetGiftStack()->AddItem(Reward);
      ADD_MESSAGE("%s drops on the ground.", Reward->CHAR_NAME(DEFINITE));
    }
}

void nefas::PrayBadEffect()
{
  ADD_MESSAGE("A potion drops on your head and shatters into small bits.");
  PLAYER->ReceiveDamage(0, 2 + RAND() % 7, PHYSICAL_DAMAGE, HEAD);
  PLAYER->GetStackUnder()->AddItem(new brokenbottle);
  PLAYER->CheckDeath(std::string("killed while enjoying the company of ") + GetName(), 0);
}

void scabies::PrayGoodEffect()
{
  if(!PLAYER->HasAllBodyParts())
    {
      bodypart* OldBodyPart = PLAYER->FindRandomOwnBodyPart();

      if(OldBodyPart != 0)
	{
	  OldBodyPart->RemoveFromSlot();
	  PLAYER->AttachBodyPart(OldBodyPart);
	  OldBodyPart->SetHP(1);
	  OldBodyPart->Mutate();
	  ADD_MESSAGE("%s attaches your old %s back. But it seems somehow different...", GetName(), OldBodyPart->GetBodyPartName().c_str());
	}
      else
	{
	  bodypart* NewBodyPart = PLAYER->GenerateRandomBodyPart();
	  NewBodyPart->SetHP(1);
	  NewBodyPart->Mutate();
	  ADD_MESSAGE("You grow a new %s, which seems to be a bit strange.", NewBodyPart->GetBodyPartName().c_str()); 
	}

      return;
    }

  if(!(RAND() % 50))
    {
      ADD_MESSAGE("Five cans full of school food drop from somewhere above!");

      for(ushort c = 0; c < 5; ++c)
	{
	  can* Reward = new can(0, NO_MATERIALS);
	  Reward->InitMaterials(MAKE_MATERIAL(IRON), MAKE_MATERIAL(SCHOOL_FOOD));
	  PLAYER->GetGiftStack()->AddItem(Reward);
	}

      return;
    }

  bool Success = false;

  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = PLAYER->GetNeighbourLSquare(d);

      if(Square && Square->GetCharacter() && Square->GetCharacter()->GetRelation(PLAYER) == HOSTILE)
	{
	  ADD_MESSAGE("%s throws poison on %s!", GetName(), Square->GetCharacter()->CHAR_NAME(DEFINITE));
	  Square->SpillFluid(PLAYER, MAKE_MATERIAL(POISON_LIQUID, 500), 100);
	  Success = true;
	}
    }

  if(!Success)
    PLAYER->PolymorphRandomly(250, 1000, 1000 + RAND() % 1000);
}

void scabies::PrayBadEffect()
{
  if(!(RAND() % 50))
    {
      ADD_MESSAGE("%s makes you eat a LOT of school food.", GetName());
      material* SchoolFood = MAKE_MATERIAL(SCHOOL_FOOD, 1000);
      SchoolFood->EatEffect(PLAYER, 1000);
      delete SchoolFood;
      ADD_MESSAGE("You feel your muscles softening terribly...");
      PLAYER->EditAttribute(ARM_STRENGTH, -1);
      PLAYER->EditAttribute(DEXTERITY, -1);
    }
  else
    {
      ADD_MESSAGE("%s unleashes all her fury upon you!", GetName());
      PLAYER->BeginTemporaryState(POISONED, 600 + RAND() % 400);
    }
}

void infuscor::PrayGoodEffect()
{
  ADD_MESSAGE("%s helps you.", GetName());

  if(!PLAYER->StateIsActivated(ESP))
    {
      PLAYER->BeginTemporaryState(ESP, 10000 + RAND() % 10000);
      return;
    }

  if(!PLAYER->StateIsActivated(TELEPORT_CONTROL))
    {
      PLAYER->BeginTemporaryState(TELEPORT_CONTROL, 10000 + RAND() % 10000);
      return;
    }

  if(!PLAYER->StateIsActivated(POLYMORPH_CONTROL))
    {
      PLAYER->BeginTemporaryState(POLYMORPH_CONTROL, 10000 + RAND() % 10000);
      return;
    }    

  ADD_MESSAGE("Suddenly 5 scrolls appear almost under your feet.");

  for(ushort c = 0; c < 5; ++c)
    PLAYER->GetGiftStack()->AddItem(new scrollofteleportation);
}

void cruentus::PrayGoodEffect()
{
  if(!PLAYER->HasAllBodyParts())
    {
      bodypart* NewBodyPart = PLAYER->GenerateRandomBodyPart();
      NewBodyPart->ChangeMainMaterial(MAKE_MATERIAL(IRON));
      NewBodyPart->SetHP(NewBodyPart->GetMaxHP());
      ADD_MESSAGE("You grow a new %s, which seems to be made of iron.", NewBodyPart->GetBodyPartName().c_str()); 
      return;
    }

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, game::GetCurrentLevel()->GetBorder(), PLAYER->GetPos(), 10);
  bool AudiencePresent = false;

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    {
      for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
	{
	  character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

	  if(Audience && Audience->CanBeSeenByPlayer() && !Audience->TemporaryStateIsActivated(PANIC) && PLAYER->GetRelation(Audience) == HOSTILE)
	    {
	      AudiencePresent = true;
	      break;
	    }
	}

      if(AudiencePresent)
	break;
    }

  if(AudiencePresent)
    {
      ADD_MESSAGE("The thundering voice of a godly battle drum shakes everything around you.");

      for(ushort x = Rect.X1; x <= Rect.X2; ++x)
	for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
	  {
	    character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

	    if(Audience && !Audience->TemporaryStateIsActivated(PANIC) && PLAYER->GetRelation(Audience) == HOSTILE && Audience->GetPanicLevel() >= RAND() % 100)
	      Audience->BeginTemporaryState(PANIC, 500 + RAND() % 500);
	  }

      return;
    }

  item* Weapon = PLAYER->GetMainWielded();

  if(!Weapon || !Weapon->IsWeapon(PLAYER))
    Weapon = PLAYER->GetSecondaryWielded();

  if(Weapon && Weapon->IsWeapon(PLAYER) && Weapon->CanBeEnchanted() && Weapon->GetEnchantment() < 5 && !(RAND() % 10))
    {
      ADD_MESSAGE("Your %s glows briefly red. It feels very warm now.", Weapon->CHAR_NAME(UNARTICLED));
      Weapon->EditEnchantment(1);
    }
  else if(RAND() & 3)
    {
      potion* Bottle = new potion(0, NO_MATERIALS);
      Bottle->InitMaterials(MAKE_MATERIAL(GLASS), MAKE_MATERIAL(TROLL_BLOOD));
      PLAYER->GetGiftStack()->AddItem(Bottle);
      ADD_MESSAGE("%s drops from nowhere.", Bottle->CHAR_DESCRIPTION(DEFINITE));
    }
  else
    {
      ADD_MESSAGE("Cruentus recommends you to his master, Mortifer.");
      game::GetGod(MORTIFER)->AdjustRelation(100);
    }
}

void cruentus::PrayBadEffect()
{
  item* ToBe = PLAYER->GetMainWielded();

  if(ToBe)
    {
      if(!ToBe->IsDestroyable())
	{
	  ToBe = PLAYER->GetSecondaryWielded();

	  if(!ToBe || !ToBe->IsDestroyable())
	    ADD_MESSAGE("%s tries to destroy your %s, but fails.", GetName(), PLAYER->GetMainWielded()->CHAR_NAME(UNARTICLED));
	}
    }
  else
    {
      ToBe = PLAYER->GetSecondaryWielded();

      if(ToBe && !ToBe->IsDestroyable())
	ADD_MESSAGE("%s tries to destroy your %s, but fails.", GetName(), ToBe->CHAR_NAME(UNARTICLED));
    }

  if(ToBe && ToBe->IsDestroyable())
    {
      ADD_MESSAGE("%s destroys your weapon.", GetName());
      ToBe->RemoveFromSlot();
      ToBe->SendToHell();
    }
  else
    {
      ADD_MESSAGE("%s gets mad and hits you!", GetName());
      PLAYER->ReceiveDamage(0, 1 + RAND() % 30, PHYSICAL_DAMAGE, ALL, RAND() & 7);
      PLAYER->CheckDeath(std::string("destroyed by ") + GetName(), 0);
    }
}

void scabies::PlayerVomitedOnAltar()
{
  ADD_MESSAGE("%s feels that you are indeed her follower.", GetName()); 
  AdjustRelation(1);
}

