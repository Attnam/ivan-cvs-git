/* Compiled through charset.cpp */

command::command(bool (*LinkedFunction)(character*), const std::string& Description, char Key, bool UsableInWilderness, bool WizardModeFunction) : LinkedFunction(LinkedFunction), Description(Description), Key(Key), UsableInWilderness(UsableInWilderness), WizardModeFunction(WizardModeFunction) { }

command* commandsystem::Command[] =
{
  0,

  /* Sort according to description */

  new command(&Apply, "apply", 'a', false),
  new command(&Talk, "chat", 'C', false),
  new command(&Close, "close", 'c', false),
  new command(&Dip, "dip", '!', true),
  new command(&Drink, "drink", 'D', true),
  new command(&Drop, "drop", 'd', false),
  new command(&Eat, "eat", 'e', true),
  new command(&WhatToEngrave, "engrave", 'G', false),
  new command(&EquipmentScreen, "equipment menu", 'E', true),
  new command(&Go, "go", 'g', false),
  new command(&GoDown, "go down", '>', true),
  new command(&GoUp, "go up", '<', true),
  new command(&Kick, "kick", 'k', false),
  new command(&Look, "look", 'l', true),
  new command(&AssignName, "name", 'n', false),
  new command(&Offer, "offer", 'O', false),
  new command(&Open, "open", 'o', false),
  new command(&OutlineCharacters, "outline characters", 'u', true),
  new command(&OutlineItems, "outline items", 'U', true),
  new command(&PickUp, "pick up", ',', false),
  new command(&Pray, "pray", 'p', false),
  new command(&Quit, "quit", 'Q', true),
  new command(&Read, "read", 'r', false),
  new command(&RestUntilHealed, "rest/heal", 'h', true),
  new command(&Save, "save game", 'S', true),
  new command(&ScrollMessagesDown, "scroll messages down", '+', true),
  new command(&ScrollMessagesUp, "scroll messages up", '-', true),
  new command(&ShowConfigScreen, "show config screen", '\\', true),
  new command(&ShowInventory, "show inventory", 'i', true),
  new command(&ShowKeyLayout, "show key layout", '?', true),
  new command(&DrawMessageHistory, "show message history", 'M', true),
  new command(&ShowWeaponSkills, "show weapon skills", '@', true),
  new command(&Search, "search", 's', false),
  new command(&Sit, "sit", '_', false),
  new command(&Throw, "throw", 't', false),
  new command(&ForceVomit, "vomit", 'V', false),
  new command(&NOP, "wait", '.', true),
  new command(&WieldInRightArm, "wield in right arm", 'w', true),
  new command(&WieldInLeftArm, "wield in left arm", 'W', true),
  new command(&WizardMode, "wizard mode activation", 'X', true),
  new command(&Zap, "zap", 'z', false),

  /* Sort according to key */

  new command(&RaiseStats, "raise stats", '1', true, true),
  new command(&LowerStats, "lower stats", '2', true, true),
  new command(&SeeWholeMap, "see whole map", '3', true, true),
  new command(&WalkThroughWalls, "toggle walk through walls mode", '4', true, true),
  new command(&RaiseGodRelations, "raise your relations to the gods", '5', true, true),
  new command(&LowerGodRelations, "lower your relations to the gods", '6', true, true),
  new command(&GainDivineKnowledge, "gain knowledge of all gods", '7', true, true),
  new command(&GainAllItems, "gain all items", '8', true, true),
  new command(&SecretKnowledge, "reveal secret knowledge", '9', true, true),
  new command(&DetachBodyPart, "detach a limb", '0', true, true),
  new command(&ReloadDatafiles, "reload datafiles", 'R', true, true),
  new command(&ShowBattleInfo, "show battle info", '%', true, true),
  new command(&SummonMonster, "summon monster", '&', false, true),
  new command(&LevelTeleport, "level teleport", '|', false, true),
  0
};

bool commandsystem::GoUp(character* Char)
{
  if(Char->IsStuck())
    {
      ADD_MESSAGE("You unable to go up with %s stuck to you.", Char->GetStuckTo()->CHAR_NAME(INDEFINITE));
      return false;
    }

  if(!game::IsInWilderness() && game::WizardModeIsActive() && game::GetCurrentLevelIndex() >= 1)
    if(game::TryTravel(game::GetCurrentDungeonIndex(), game::GetCurrentLevelIndex() - 1, RANDOM, true))
      return true;

  oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

  if(!Terrain)
    {
      if(game::IsInWilderness())
	{
	  if(!Char->CanFly())
	    ADD_MESSAGE("You jump into the air. For some reason you don't get too far above.");
	  else
	    ADD_MESSAGE("You fly around for some time.");
	}
      else
	ADD_MESSAGE("You can't go up.");

      return false;
    }

  if(Terrain->Enter(true))
    {
      Char->EditExperience(LEG_STRENGTH, 50);
      Char->EditNP(-20);
      Char->EditAP(-100000 / APBonus(Char->GetAttribute(AGILITY)));
      return true;
    }
  else
    return false;
}

bool commandsystem::GoDown(character* Char)
{
  if(Char->IsStuck())
    {
      ADD_MESSAGE("You unable to go down with %s stuck to you.", Char->GetStuckTo()->CHAR_NAME(INDEFINITE));
      return false;
    }

  if(!game::IsInWilderness() && game::WizardModeIsActive() && game::GetCurrentLevelIndex() < game::GetLevels() - 1)
    if(game::TryTravel(game::GetCurrentDungeonIndex(), game::GetCurrentLevelIndex() + 1, RANDOM, true))
      return true;

  oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

  if(!Terrain)
    {
      if(game::IsInWilderness())
	ADD_MESSAGE("There seems to be nothing of interest here.");
      else
	ADD_MESSAGE("You can't go down.");

      return false;
    }

  if(Terrain->Enter(false))
    {
      Char->EditExperience(AGILITY, 25);
      Char->EditNP(-10);
      Char->EditAP(-100000 / APBonus(Char->GetAttribute(AGILITY)));
      return true;
    }
  else
    return false;
}

bool commandsystem::Open(character* Char)
{
  if(Char->CanOpen())
    {
      int Key;
      bool OpenableItems = Char->GetStack()->SortedItems(Char, &item::OpenableSorter);

      if(OpenableItems)
	Key = game::AskForKeyPress("What do you wish to open?  [press a direction key, space or i]");
      else
	Key = game::AskForKeyPress("What do you wish to open?  [press a direction key or space]");

      if(Key == 'i' && OpenableItems)
	{
	  item* Item = Char->GetStack()->DrawContents(Char, "What do you want to open?", 0, &item::OpenableSorter);
	  return Item && Item->Open(Char);
	}

      vector2d DirVect = game::GetDirectionVectorForKey(Key);

      if(DirVect != ERROR_VECTOR && Char->GetArea()->IsValidPos(Char->GetPos() + DirVect))
	return Char->OpenPos(Char->GetPos() + DirVect);
    }
  else
    ADD_MESSAGE("This monster type cannot open anything.");

  return false;
}

bool commandsystem::Close(character* Char)
{
  if(Char->CanOpen())
    {
      uchar Dir = game::DirectionQuestion("What do you wish to close?  [press a direction key]", false);

      if(Dir != DIR_ERROR && Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
	return Char->ClosePos(Char->GetPos() + game::GetMoveVector(Dir));
    }
  else
    ADD_MESSAGE("This monster type cannot close anything.");

  return false;
}

bool commandsystem::Drop(character* Char)
{
  if(!Char->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to drop!");
      return false;
    }

  bool Success = false;
  stack::SetSelected(0);

  for(;;)
    {
      itemvector ToDrop;
      game::DrawEverythingNoBlit();
      Char->GetStack()->DrawContents(ToDrop, Char, "What do you want to drop?", REMEMBER_SELECTED);

      if(ToDrop.empty())
	break;

      if(!Char->GetRoom() || Char->GetRoom()->DropItem(Char, ToDrop[0], ToDrop.size()))
	{
	  for(ushort c = 0; c < ToDrop.size(); ++c)
	    ToDrop[c]->MoveTo(Char->GetStackUnder());

	  ADD_MESSAGE("%s dropped.", ToDrop[0]->GetName(INDEFINITE, ToDrop.size()).c_str());
	  Success = true;
	}
    }

  if(Success)
    {
      Char->DexterityAction(2);
      return true;
    }

  return false;
}

bool commandsystem::Eat(character* Char)
{
  if(!Char->UsesNutrition())
    {
      ADD_MESSAGE("In this form you can't and don't need to eat.");
      return true;
    }

  lsquare* Square = Char->GetLSquareUnder();

  if(!game::IsInWilderness() && Square->GetOLTerrain() && Square->GetOLTerrain()->HasEatEffect())
    {
      if(Square->GetOLTerrain()->Eat(Char))
	return true;
    }

  stack* Inventory = Char->GetStack();
  stack* StackUnder = Square->GetStack();

  if((game::IsInWilderness() || !StackUnder->SortedItems(Char, &item::EatableSorter)) && !Inventory->SortedItems(Char, &item::EatableSorter))
    {
      ADD_MESSAGE("You have nothing to eat!");
      return false;
    }

  itemvector Item;

  if(!game::IsInWilderness() && StackUnder->SortedItems(Char, &item::EatableSorter))
    Inventory->DrawContents(Item, StackUnder, Char, "What do you wish to eat?", "Items in your inventory", "Items on the ground", NO_MULTI_SELECT, &item::EatableSorter);
  else
    Inventory->DrawContents(Item, Char, "What do you wish to eat?", NO_MULTI_SELECT, &item::EatableSorter);

  return !Item.empty() ? Char->ConsumeItem(Item[0]) : false;
}

bool commandsystem::Drink(character* Char)
{
  if(!Char->UsesNutrition())
    {
      ADD_MESSAGE("In this form you can't and don't need to drink.");
      return true;
    }

  lsquare* Square = Char->GetLSquareUnder();

  if(!game::IsInWilderness() && Square->GetOLTerrain() && Square->GetOLTerrain()->HasDrinkEffect())
    {
      if(Square->GetOLTerrain()->Drink(Char))
	return true;
    }

  stack* Inventory = Char->GetStack();
  stack* StackUnder = Square->GetStack();

  if((game::IsInWilderness() || !StackUnder->SortedItems(Char, &item::DrinkableSorter)) && !Inventory->SortedItems(Char, &item::DrinkableSorter))
    {
      ADD_MESSAGE("You have nothing to drink!");
      return false;
    }

  itemvector Item;

  if(!game::IsInWilderness() && StackUnder->SortedItems(Char, &item::DrinkableSorter))
    Inventory->DrawContents(Item, StackUnder, Char, "What do you wish to drink?", "Items in your inventory", "Items on the ground", NO_MULTI_SELECT, &item::DrinkableSorter);
  else
    Inventory->DrawContents(Item, Char, "What do you wish to drink?", NO_MULTI_SELECT, &item::DrinkableSorter);

  return !Item.empty() ? Char->ConsumeItem(Item[0]) : false;
}

bool commandsystem::ShowInventory(character* Char)
{
  Char->GetStack()->DrawContents(Char, "Your inventory", NO_SELECT);
  return false;
}

bool commandsystem::PickUp(character* Char)
{
  ushort VisibleItemsOnGround = Char->GetStackUnder()->GetVisibleItems(Char);

  if(!VisibleItemsOnGround)
    {
      ADD_MESSAGE("There is nothing here to pick up, %s!", game::Insult());
      return false;
    }

  std::vector<itemvector> PileVector;
  Char->GetStackUnder()->Pile(PileVector, Char);

  if(PileVector.size() == 1)
    {
      ushort Amount = PileVector[0].size();

      if(Amount > 1)
	Amount = game::ScrollBarQuestion("How many " + PileVector[0][0]->GetName(PLURAL) + '?', vector2d(16, 6), Amount, 1, 0, Amount, 0, WHITE, LIGHT_GRAY, DARK_GRAY);

      if(!Amount)
	return false;

      if((!Char->GetRoom() || Char->GetRoom()->PickupItem(Char, PileVector[0][0], Amount)) && PileVector[0][0]->CheckPickUpEffect(Char))
	{
	  for(ushort c = 0; c < Amount; ++c)
	    PileVector[0][c]->MoveTo(Char->GetStack());

	  ADD_MESSAGE("%s picked up.", PileVector[0][0]->GetName(INDEFINITE, Amount).c_str());
	  Char->DexterityAction(2);
	  return true;
	}
      else
	return false;
    }

  bool Success = false;
  stack::SetSelected(0);

  for(;;)
    {
      itemvector ToPickup;
      game::DrawEverythingNoBlit();
      Char->GetStackUnder()->DrawContents(ToPickup, Char, "What do you want to pick up?", REMEMBER_SELECTED);

      if(ToPickup.empty())
	break;

      if((!Char->GetRoom() || Char->GetRoom()->PickupItem(Char, ToPickup[0], ToPickup.size())) && ToPickup[0]->CheckPickUpEffect(Char))
	{
	  for(ushort c = 0; c < ToPickup.size(); ++c)
	    ToPickup[c]->MoveTo(Char->GetStack());

	  ADD_MESSAGE("%s picked up.", ToPickup[0]->GetName(INDEFINITE, ToPickup.size()).c_str());
	  Success = true;
	}
    }

  if(Success)
    {
      Char->DexterityAction(2);
      return true;
    }

  return false;
}

bool commandsystem::Quit(character* Char)
{
  if(game::BoolQuestion("Your quest is not yet compeleted! Really quit? [y/N]"))
    {
      Char->ShowAdventureInfo();
      Char->AddScoreEntry("cowardly quit the game", 0.75f);
      game::End(!game::WizardModeIsActive() || game::BoolQuestion("Remove saves? [y/N]"));
      return true;
    }
  else
    return false;
}

bool commandsystem::Talk(character* Char)
{
  if(!Char->CheckTalk())
    return false;

  character* ToTalk = 0;
  ushort Characters = 0;

  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = Char->GetNeighbourLSquare(d);

      if(Square)
	{
	  character* Dude = Square->GetCharacter();

	  if(Dude)
	    {
	      ToTalk = Dude;
	      ++Characters;
	    }
	}
    }

  if(!Characters)
    {
      ADD_MESSAGE("Find yourself someone to talk to first!");
      return false;
    }
  else if(Characters == 1)
    {
      ToTalk->BeTalkedTo();
      Char->EditExperience(CHARISMA, 10);
      Char->EditAP(-1000);
      return true;
    }
  else
    {
      uchar Dir = game::DirectionQuestion("To whom do you wish to talk to? [press a direction key]", false, true);

      if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
	return false;

      if(Dir == YOURSELF)
	{
	  ADD_MESSAGE("You talk to yourself for some time.");
	  Char->EditExperience(WISDOM, -10);
	  Char->EditAP(-1000);
	  return true;
	}

      character* Dude = Char->GetNearSquare(Char->GetPos() + game::GetMoveVector(Dir))->GetCharacter();

      if(Dude)
	{
	  Dude->BeTalkedTo();
	  Char->EditExperience(CHARISMA, 10);
	  Char->EditAP(-1000);
	  return true;
	}
      else
	ADD_MESSAGE("You get no response.");
    }

  return false;
}

bool commandsystem::NOP(character* Char)
{
  Char->EditExperience(DEXTERITY, -10);
  Char->EditExperience(AGILITY, -10);
  Char->EditAP(-Char->GetStateAPGain(1000));
  return true;
}

bool commandsystem::Save(character*)
{
  if(game::BoolQuestion("Do you truly wish to save and flee? [y/N]"))
    {
      game::Save();
      game::End(false);
      return true;
    }
  else
    return false;
}

bool commandsystem::Read(character* Char)
{
  if(!Char->CanRead() && !game::SeeWholeMapCheatIsActive())
    {
      ADD_MESSAGE("You can't read.");
      return false;
    }

  if(!Char->GetStack()->SortedItems(Char, &item::ReadableSorter))
    {
      ADD_MESSAGE("You have nothing to read!");
      return false;
    }

  if(Char->GetLSquareUnder()->IsDark() && !game::SeeWholeMapCheatIsActive())
    {
      ADD_MESSAGE("It is too dark to read.");
      return false;
    }

  item* Item = Char->GetStack()->DrawContents(Char, "What do you want to read?", 0, &item::ReadableSorter);
  return Item && Char->ReadItem(Item);
}

bool commandsystem::Dip(character* Char)
{
  if(!Char->GetStack()->SortedItems(Char, &item::DippableSorter) && !Char->EquipsSomething(&item::DippableSorter))
    {
      ADD_MESSAGE("You have nothing to dip!");
      return false;
    }

  bool HasDipDestination = Char->GetStack()->SortedItems(Char, &item::DipDestinationSorter) || Char->EquipsSomething(&item::DippableSorter);
  bool DipDestinationNear = false;

  for(ushort d = 0; d < 9; ++d)
    {
      lsquare* Square = Char->GetNeighbourLSquare(d);

      if(Square && Square->IsDipDestination())
	DipDestinationNear = true;
    }

  if(!HasDipDestination && !DipDestinationNear)
    {
      ADD_MESSAGE("There is nothing to dip anything into.");
      return false;
    }

  item* Item = Char->SelectFromPossessions("What do you want to dip?", &item::DippableSorter);

  if(Item)
    {
      if(!HasDipDestination || (DipDestinationNear && game::BoolQuestion("Do you wish to dip in a nearby square? [y/N]")))
	{
	  uchar Dir = game::DirectionQuestion("Where do you want to dip " + Item->GetName(DEFINITE) + "? [press a direction key or '.']", false, true);
	  vector2d Pos = Char->GetPos() + game::GetMoveVector(Dir);

	  if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Pos) || !Char->GetNearLSquare(Pos)->IsDipDestination())
	    return false;
	  
	  return Char->GetNearLSquare(Pos)->DipInto(Item, Char);
	}
      else
	{
	  item* DipTo = Char->SelectFromPossessions("Into what do you wish to dip it?", &item::DipDestinationSorter);

	  if(DipTo)
	    {
	      if(Item == DipTo)
		{
		  ADD_MESSAGE("Very funny...");
		  return false;
		}

	      Item->DipInto(DipTo->CreateDipMaterial(), Char);
	      return true;
	    }
	}
    }

  return false;
}

bool commandsystem::WizardMode(character* Char)
{
  if(!game::WizardModeIsActive())
    {
      if(game::BoolQuestion("Do you want to cheat, cheater? This action cannot be undone. [y/N]"))
	{
	  game::ActivateWizardMode();
	  ADD_MESSAGE("Wizard mode activated.");

	  for(ushort x = 0; x < 5; ++x)
	    Char->GetStack()->AddItem(new scrollofwishing);

	  if(game::IsInWilderness())
	    {
	      vector2d ElpuriCavePos = game::GetWorldMap()->GetEntryPos(0, ELPURI_CAVE);
	      game::GetWorldMap()->GetWSquare(ElpuriCavePos)->ChangeOWTerrain(new elpuricave);
	      game::GetWorldMap()->RevealEnvironment(ElpuriCavePos, 1);
	      game::GetWorldMap()->SendNewDrawRequest();
	    }
	  else
	    {
	      game::LoadWorldMap();
	      vector2d ElpuriCavePos = game::GetWorldMap()->GetEntryPos(0, ELPURI_CAVE);
	      game::GetWorldMap()->GetWSquare(ElpuriCavePos)->ChangeOWTerrain(new elpuricave);
	      game::GetWorldMap()->RevealEnvironment(ElpuriCavePos, 1);
	      game::SaveWorldMap();
	    }

	  game::Save();
	  game::Save(game::GetAutoSaveFileName());

	}
    }
  else
    {
      ADD_MESSAGE("Got some scrolls of wishing.");

      for(ushort x = 0; x < 5; ++x)
	Char->GetStack()->AddItem(new scrollofwishing);
    }

  return false;

}

bool commandsystem::RaiseStats(character* Char)
{
  Char->RaiseStats();
  return false;
}

bool commandsystem::LowerStats(character* Char)
{
  Char->LowerStats();
  return false;
}

bool commandsystem::GainAllItems(character* Char)
{
  itemvector AllItems;
  protosystem::CreateEveryItem(AllItems);
  stack* Stack = game::IsInWilderness() ? Char->GetStack() : Char->GetStackUnder();

  for(ushort c = 0; c < AllItems.size(); ++c)
    Stack->AddItem(AllItems[c]);

  return false;
}

bool commandsystem::SeeWholeMap(character*)
{
  game::SeeWholeMap();
  return false;
}

bool commandsystem::WalkThroughWalls(character*)
{
  game::GoThroughWalls();
  return false;
}

bool commandsystem::ShowKeyLayout(character*)
{
  felist List("Keyboard Layout");

  List.AddDescription("");
  List.AddDescription("Key       Description");
  std::string Buffer;

  for(ushort c = 1; GetCommand(c); ++c)
    if(!GetCommand(c)->IsWizardModeFunction())
      {
	Buffer.resize(0);
	Buffer += GetCommand(c)->GetKey();
	Buffer.resize(10, ' ');
	List.AddEntry(Buffer + GetCommand(c)->GetDescription(), LIGHT_GRAY);
      }

  if(game::WizardModeIsActive())
    {
      List.AddEntry("", WHITE);
      List.AddEntry("Wizard mode functions:", WHITE);
      List.AddEntry("", WHITE);

      for(ushort c = 1; GetCommand(c); ++c)
	if(GetCommand(c)->IsWizardModeFunction())
	  {
	    Buffer.resize(0);
	    Buffer += GetCommand(c)->GetKey();
	    Buffer.resize(10, ' ');
	    List.AddEntry(Buffer + GetCommand(c)->GetDescription(), LIGHT_GRAY);
	  }
    }

  game::SetStandardListAttributes(List);
  List.Draw();
  return false;
}

bool commandsystem::Look(character* Char)
{
  std::string Msg;

  if(!game::IsInWilderness())
    Msg = "Direction keys move cursor, ESC exits, 'i' examines items, 'c' examines a character.";
  else
    Msg = "Direction keys move cursor, ESC exits, 'c' examines a character.";

  game::PositionQuestion(Msg, Char->GetPos(), &game::LookHandler, &game::LookKeyHandler, configuration::GetLookZoom());
  Char->EditExperience(PERCEPTION, 1);
  Char->EditAP(-100);
  return true;
}

bool commandsystem::WhatToEngrave(character* Char)
{
  if(!Char->CanRead())
    {
      ADD_MESSAGE("You can't even read.");
      return false;
    }

  Char->GetNearLSquare(Char->GetPos())->Engrave(game::StringQuestion("What do you want to engrave here?", vector2d(16, 6), WHITE, 0, 80, true));
  return false;
}

bool commandsystem::Pray(character* Char)
{
  felist Panthenon("To Whom you want to address your prayers?");
  std::vector<ushort> KnownIndex;

  if(!Char->GetLSquareUnder()->GetDivineMaster())
    {
      for(ushort c = 1; c <= GODS; ++c)
	if(game::GetGod(c)->IsKnown())
	  {
	    igraph::GetSymbolGraphic()->Blit(igraph::GetTileBuffer(), c << 4, 0, 0, 0, 16, 16);
	    Panthenon.AddEntry(game::GetGod(c)->GetCompleteDescription(), LIGHT_GRAY, 20, igraph::GetTileBuffer());
	    KnownIndex.push_back(c);
	  }
    }
  else
    if(game::GetGod(Char->GetLSquareUnder()->GetDivineMaster())->IsKnown())
      {
	igraph::GetSymbolGraphic()->Blit(igraph::GetTileBuffer(), Char->GetLSquareUnder()->GetDivineMaster() << 4, 0, 0, 0, 16, 16);
	Panthenon.AddEntry(game::GetGod(Char->GetLSquareUnder()->GetDivineMaster())->GetCompleteDescription(), LIGHT_GRAY, 20, igraph::GetTileBuffer());
	KnownIndex.push_back(Char->GetLSquareUnder()->GetDivineMaster());
      }
    else
      {
	ADD_MESSAGE("Somehow you feel that no deity you know can hear your prayers from this place.");
	return false;
      }

  game::SetStandardListAttributes(Panthenon);
  Panthenon.AddFlags(SELECTABLE);
  ushort Select = Panthenon.Draw();

  if(Select == LIST_WAS_EMPTY)
    {
      ADD_MESSAGE("You do not know any gods.");
      return false;
    }

  if(Select & FELIST_ERROR_BIT)
    return false;
  else
    {
      if(Char->GetLSquareUnder()->GetDivineMaster())
	{
	  if(!Select)
	    {
	      if(game::BoolQuestion("Do you really wish to pray to " + game::GetGod(Char->GetLSquareUnder()->GetDivineMaster())->GetName() + "? [y/N]"))
		game::GetGod(Char->GetLSquareUnder()->GetDivineMaster())->Pray();
	      else
		return false;
	    }
	  else
	    return false;
	}
      else
	{
	  if(game::BoolQuestion("Do you really wish to pray to " + game::GetGod(KnownIndex[Select])->GetName() + "? [y/N]"))
	    game::GetGod(KnownIndex[Select])->Pray();
	  else
	    return false;
	}

      Char->EditAP(-1000);
      return true;
    }
}

bool commandsystem::Kick(character* Char)
{
  if(!Char->CheckKick())
    return false;

  if(Char->GetBurdenState() == OVER_LOADED)
    {
      ADD_MESSAGE("You try to kick, but you collapse under your load.");
      Char->EditAP(-100000 / APBonus(Char->GetAttribute(AGILITY)));
      return true;
    }

  uchar Dir = game::DirectionQuestion("In what direction do you wish to kick? [press a direction key]", false);

  if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
    return false;

  lsquare* Square = Char->GetNearLSquare(Char->GetPos() + game::GetMoveVector(Dir));

  if(!Square->CheckKick(Char))
    return false;

  if(Square->GetCharacter() && Char->GetRelation(Square->GetCharacter()) != HOSTILE)
    if(!game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
      return false;
    else
      Char->Hostility(Square->GetCharacter());

  Char->Kick(Square);
  return true;
}

bool commandsystem::Offer(character* Char)
{
  if(!Char->CheckOffer())
    return false;

  lsquare* Square = Char->GetLSquareUnder();

  if(Square->GetOLTerrain() && Square->GetOLTerrain()->AcceptsOffers())
    {
      if(!Char->GetStack()->GetItems())
	{
	  ADD_MESSAGE("You have nothing to offer!");
	  return false;
	}

      item* Item = Char->GetStack()->DrawContents(Char, "What do you want to offer?");

      if(Item)
	{
	  if(game::GetGod(Square->GetDivineMaster())->ReceiveOffer(Item))
	    {
	      Item->RemoveFromSlot();
	      Item->SendToHell();
	      Char->DexterityAction(5);
	      return true;
	    }
	  else
	    return false;
	}
      else
	return false;
    }
  else
    ADD_MESSAGE("There isn't any altar here!");

  return false;
}

bool commandsystem::DrawMessageHistory(character*)
{
  msgsystem::DrawMessageHistory();
  return false;
}

bool commandsystem::Throw(character* Char)
{
  if(!Char->CheckThrow())
    return false;

  if(!Char->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to throw!");
      return false;
    }

  item* Item = Char->GetStack()->DrawContents(Char, "What do you want to throw?");

  if(Item)
    {
      uchar Answer = game::DirectionQuestion("In what direction do you wish to throw?  [press a direction key]", false);

      if(Answer == DIR_ERROR)
	return false;

      Char->ThrowItem(Answer, Item);
      Char->EditExperience(ARM_STRENGTH, 25);
      Char->EditExperience(PERCEPTION, 25);
      Char->EditNP(-50);
      Char->DexterityAction(5);
      return true;
    }
  else
    return false;
}

bool commandsystem::Apply(character* Char)
{
  if(!Char->GetStack()->SortedItems(Char, &item::AppliableSorter) && !Char->EquipsSomething(&item::AppliableSorter))
    {
      ADD_MESSAGE("You have nothing to apply!");
      return false;
    }

  item* Item = Char->SelectFromPossessions("What do you want to apply?", &item::AppliableSorter);
  return Item && Item->Apply(Char);
}

bool commandsystem::ForceVomit(character* Char)
{
  if(Char->IsAlive())
    {
      ADD_MESSAGE("You push your fingers down to your throat and vomit.");
      Char->Vomit(2 + RAND() % 3);
      Char->EditAP(-1000);
      return true;
    }
  else
    {
      ADD_MESSAGE("You can't vomit.");
      return false;
    }
}

bool commandsystem::Zap(character* Char)
{
  if(!Char->CheckZap())
    return false;

  if(!Char->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to zap with.");
      return false;
    }

  item* Item = Char->GetStack()->DrawContents(Char, "What do you want to zap with?", 0, &item::ZappableSorter);

  if(Item)
    {
      uchar Answer = game::DirectionQuestion("In what direction do you wish to zap?  [press a direction key or '.']", false, true);

      if(Answer == DIR_ERROR)
	return false;

      if(Item->Zap(Char, Char->GetPos(), Answer))
	{
	  Char->EditAP(-100000 / APBonus(Char->GetAttribute(PERCEPTION)));
	  return true;
	}
      else
	return false;
    }
  else
    return false;
}

bool commandsystem::RestUntilHealed(character* Char)
{
  if(Char->GetHP() == Char->GetMaxHP())
    {
      ADD_MESSAGE("You HP is already at its maximum.");
      return false;
    }

  if(!Char->CanHeal())
    {
      ADD_MESSAGE("You cannot heal.");
      return false;
    }

  long HPToRest = game::ScrollBarQuestion("How many hit points you desire?", vector2d(16, 6), Char->GetMaxHP(), 1, 0, Char->GetMaxHP(), 0, WHITE, LIGHT_GRAY, DARK_GRAY);

  if(HPToRest <= Char->GetHP())
    {
      ADD_MESSAGE("Your HP is already %d.", Char->GetHP());
      return false;
    }

  oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

  if(Terrain)
    Terrain->ShowRestMessage(Char);

  rest* Rest = new rest(Char);
  Rest->SetGoalHP(HPToRest);
  Char->SetAction(Rest);
  return true;
}

bool commandsystem::OutlineCharacters(character* Char)
{
  configuration::SetOutlineCharacters(!configuration::GetOutlineCharacters());
  configuration::Save();
  Char->GetArea()->SendNewDrawRequest();
  return false;
}

bool commandsystem::OutlineItems(character* Char)
{
  configuration::SetOutlineItems(!configuration::GetOutlineItems());
  configuration::Save();
  Char->GetArea()->SendNewDrawRequest();
  return false;
}

bool commandsystem::RaiseGodRelations(character*)
{
  for(ushort c = 1; c <= GODS; ++c)
    game::GetGod(c)->AdjustRelation(50);

  return false;
}

bool commandsystem::LowerGodRelations(character*)
{
  for(ushort c = 1; c <= GODS; ++c)
    game::GetGod(c)->AdjustRelation(-50);

  return false;
}

bool commandsystem::GainDivineKnowledge(character*)
{
  for(ushort c = 1; c <= GODS; ++c)
    game::GetGod(c)->SetIsKnown(true);

  return false;
}

bool commandsystem::Sit(character* Char)
{
  lsquare* Square = Char->GetLSquareUnder();
  return (Square->GetOLTerrain() && Square->GetOLTerrain()->SitOn(Char)) || Square->GetGLTerrain()->SitOn(Char);
}

bool commandsystem::Go(character* Char)
{
  if(Char->StateIsActivated(CONFUSED))
    {
      ADD_MESSAGE("In this mental state you would forget your destination immediately.");
      return false;
    }

  uchar Dir = game::DirectionQuestion("In what direction do you want to go? [press a direction key]", false);

  if(Dir == DIR_ERROR)
    return false;

  go* Go = new go(Char);
  Go->SetDirection(Dir);
  uchar OKDirectionsCounter = 0;

  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = Char->GetNeighbourLSquare(d);

      if(Square && Square->IsWalkable(Char))
	++OKDirectionsCounter;
    }

  Go->SetWalkingInOpen(OKDirectionsCounter > 2);
  Char->SetAction(Go);
  Go->Handle();
  return Char->GetAction() != 0;
}

bool commandsystem::ShowConfigScreen(character*)
{
  configuration::ShowConfigScreen();
  return false;
}

bool commandsystem::SecretKnowledge(character* Char)
{
  felist List("Knowledge of the ancients");
  List.AddEntry("Character attributes", LIGHT_GRAY);
  List.AddEntry("Character attack info", LIGHT_GRAY);
  List.AddEntry("Character defence info", LIGHT_GRAY);
  List.AddEntry("Character danger values", LIGHT_GRAY);
  List.AddEntry("Item attack info", LIGHT_GRAY);
  List.AddEntry("Miscellaneous item info", LIGHT_GRAY);
  List.AddEntry("Material info", LIGHT_GRAY);
  game::SetStandardListAttributes(List);
  List.AddFlags(SELECTABLE);
  ushort Chosen = List.Draw();
  ushort c, PageLength = 20;
  std::string Entry;

  if(Chosen & FELIST_ERROR_BIT)
    return false;

  List.Empty();

  if(Chosen < 4)
    {
      std::vector<character*> Character;
      protosystem::CreateEveryCharacter(Character);

      switch(Chosen)
	{
	case 0:
	  List.AddDescription("                                                AS LS DX AG EN PE IN WI CH MA");

	  for(c = 0; c < Character.size(); ++c)
	    {
	      Entry.resize(0);
	      Character[c]->AddName(Entry, UNARTICLED);
	      std::vector<bitmap*> Picture;
	      Character[c]->DrawBodyPartVector(Picture);
	      Character[c]->AddAttributeInfo(Entry);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, Picture);
	    }

	  PageLength = 15;
	  break;
	case 1:
	  List.AddDescription("                                                  BD        THV       APC");

	  for(c = 0; c < Character.size(); ++c)
	    {
	      Entry.resize(0);
	      Character[c]->AddName(Entry, UNARTICLED);
	      std::vector<bitmap*> Picture;
	      Character[c]->DrawBodyPartVector(Picture);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, Picture);
	      Character[c]->AddAttackInfo(List);
	    }

	  PageLength = 20;
	  break;
	case 2:
	  List.AddDescription("                                                  DV/BV     HP        AV/BS");

	  for(c = 0; c < Character.size(); ++c)
	    {
	      Entry.resize(0);
	      Character[c]->AddName(Entry, UNARTICLED);
	      Entry.resize(47, ' ');
	      Entry << int(Character[c]->GetDodgeValue());
	      Entry.resize(57, ' ');
	      Entry << Character[c]->GetMaxHP();
	      std::vector<bitmap*> Picture;
	      Character[c]->DrawBodyPartVector(Picture);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, Picture);
	      Character[c]->AddDefenceInfo(List);
	    }

	  PageLength = 25;
	  break;
	case 3:
	  List.AddDescription("                                                  Danger    NGM       EGM");

	  for(c = 0; c < Character.size(); ++c)
	    {
	      Entry.resize(0);
	      Character[c]->AddName(Entry, UNARTICLED);
	      Entry.resize(47, ' ');
	      Entry << int(Character[c]->GetRelativeDanger(Char, true) * 100);
	      Entry.resize(57, ' ');

	      if(Character[c]->CanBeGenerated())
		{
		  const dangerid& DI = game::GetDangerMap().find(configid(Character[c]->GetType(), Character[c]->GetConfig()))->second;
		  Entry << int(DI.NakedDanger * 100);
		  Entry.resize(67, ' ');
		  Entry << int(DI.EquippedDanger * 100);
		}
	      else
		Entry << "-         -";

	      std::vector<bitmap*> Picture;
	      Character[c]->DrawBodyPartVector(Picture);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, Picture);
	    }

	  PageLength = 15;
	  break;
	}

      for(c = 0; c < Character.size(); ++c)
	delete Character[c];
    }
  else if(Chosen < 6)
    {
      itemvector Item;
      protosystem::CreateEveryItem(Item);
      PageLength = 20;

      switch(Chosen)
	{
	case 4:
	  List.AddDescription("                                        Weight    Size      SR        DAM");

	  for(c = 0; c < Item.size(); ++c)
	    {
	      Entry.resize(0);
	      Item[c]->AddName(Entry, UNARTICLED);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, Item[c]->GetPicture(), true, Item[c]->AllowAlphaEverywhere());
	      Item[c]->AddAttackInfo(List);
	    }

	  break;
	case 5:
	  List.AddDescription("                                        \177              OV             NV");

	  for(c = 0; c < Item.size(); ++c)
	    {
	      Entry.resize(0);
	      Item[c]->AddName(Entry, UNARTICLED);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, Item[c]->GetPicture(), true, Item[c]->AllowAlphaEverywhere());
	      Item[c]->AddMiscellaneousInfo(List);
	    }

	  break;
	}

      for(c = 0; c < Item.size(); ++c)
	delete Item[c];
    }
  else if(Chosen < 7)
    {
      std::vector<material*> Material;
      protosystem::CreateEveryMaterial(Material);
      PageLength = 30;
      List.AddDescription("                                        Strength       Flexibility   Density");

      for(c = 0; c < Material.size(); ++c)
	{
	  Entry.resize(0);
	  Material[c]->AddName(Entry, false, false);
	  Entry.resize(40, ' ');
	  Entry << int(Material[c]->GetStrengthValue());
	  Entry.resize(55, ' ');
	  Entry << int(Material[c]->GetFlexibility());
	  Entry.resize(70, ' ');
	  Entry << int(Material[c]->GetDensity());
	  List.AddEntry(Entry, Material[c]->GetColor());
	}

      for(c = 0; c < Material.size(); ++c)
	delete Material[c];
    }

  game::SetStandardListAttributes(List);
  List.SetPageLength(PageLength);
  List.Draw();
  List.PrintToFile(game::GetHomeDir() + "secret" + Chosen + ".txt");
  ADD_MESSAGE("Info written also to %ssecret%d.txt.", game::GetHomeDir().c_str(), Chosen);
  return false;
}

bool commandsystem::AssignName(character* Char)
{
  game::PositionQuestion("What do you want to name? [press 'n' over a tame creature or ESC to exit]", Char->GetPos(), 0, &game::NameKeyHandler);
  return false;
}

bool commandsystem::EquipmentScreen(character* Char)
{
  if(!Char->CanUseEquipment())
    {
      ADD_MESSAGE("You cannot use equipment.");
      return false;
    }

  ushort Chosen = 0;
  bool EquipmentChanged = false;
  felist List("Equipment menu");
  std::string Entry;

  while(true)
    {
      List.Empty();

      for(ushort c = 0; c < Char->GetEquipmentSlots(); ++c)
	{
	  Entry = Char->GetEquipmentName(c) + ":";
	  Entry.resize(20, ' ');
	  item* Equipment = Char->GetEquipment(c);

	  if(Equipment)
	    {
	      Equipment->AddInventoryEntry(Char, Entry, 1, true);
	      Char->AddSpecialEquipmentInfo(Entry, c);
	      List.AddEntry(Entry, LIGHT_GRAY, 20, Equipment->GetPicture(), true, Equipment->AllowAlphaEverywhere());
	    }
	  else
	    {
	      Entry += Char->GetBodyPartOfEquipment(c) ? "-" : "can't use";
	      List.AddEntry(Entry, LIGHT_GRAY, 20, igraph::GetTransparentTile());
	    }
	}

      game::DrawEverythingNoBlit();
      game::SetStandardListAttributes(List);
      List.SetFlags(SELECTABLE|DRAW_BACKGROUND_AFTERWARDS);
      Chosen = List.Draw();

      if(Chosen >= Char->GetEquipmentSlots())
	break;

      EquipmentChanged = Char->TryToChangeEquipment(Chosen);
    }

  if(EquipmentChanged)
    Char->DexterityAction(5);

  return EquipmentChanged;
}

bool commandsystem::ScrollMessagesDown(character*)
{
  msgsystem::ScrollDown();
  return false;
}

bool commandsystem::ScrollMessagesUp(character*)
{
  msgsystem::ScrollUp();
  return false;
}

bool commandsystem::ShowWeaponSkills(character* Char)
{
  felist List("Your experience in weapon categories");
  List.AddDescription("");
  List.AddDescription("Category name                 Level     Points    Needed    Battle bonus");
  bool Something = false;

  for(ushort c = 0; c < Char->GetAllowedWeaponSkillCategories(); ++c)
    {
      cweaponskill* Skill = Char->GetCWeaponSkill(c);

      if(Skill->GetHits())
	{
	  std::string Buffer = Skill->GetName();
	  Buffer.resize(30, ' ');
	  Buffer << Skill->GetLevel();
	  Buffer.resize(40, ' ');
	  Buffer << Skill->GetHits();
	  Buffer.resize(50, ' ');

	  if(Skill->GetLevel() != 10)
	    Buffer << (Skill->GetLevelMap(Skill->GetLevel() + 1) - Skill->GetHits());
	  else
	    Buffer << '-';

	  Buffer.resize(60, ' ');
	  Buffer << '+' << int(Skill->GetBonus() - 100) << '%';
	  List.AddEntry(Buffer, LIGHT_GRAY);
	  Something = true;
	}
    }

  if(Char->AddSpecialSkillInfo(List))
    Something = true;

  if(Something)
    {
      game::SetStandardListAttributes(List);
      List.Draw();
    }
  else
    ADD_MESSAGE("You are not experienced in any weapon skill yet!");

  return false;
}

bool commandsystem::DetachBodyPart(character* Char)
{
  Char->DetachBodyPart();
  return false;
}

bool commandsystem::ReloadDatafiles(character*)
{
  databasesystem::Initialize();
  return false;
}

bool commandsystem::SummonMonster(character* Char)
{
  character* Summoned = 0;

  while(!Summoned)
    Summoned = protosystem::CreateMonster(game::StringQuestion("Summon which monster?", vector2d(16, 6), WHITE, 0, 80, false));

  Summoned->SetTeam(game::GetTeam(MONSTER_TEAM));
  vector2d Where = Char->GetLevel()->GetNearestFreeSquare(Summoned, Char->GetPos());

  if(Where == ERROR_VECTOR)
    Where = Char->GetLevel()->GetRandomSquare(Summoned);

  Char->GetNearLSquare(Where)->AddCharacter(Summoned);
  return false;
}

bool commandsystem::LevelTeleport(character*)
{
  long Level = game::NumberQuestion("To which level?", vector2d(16, 6), WHITE);

  if(Level <= 0 || Level > game::GetLevels())
    {
      ADD_MESSAGE("There is no level %d in this dungeon, %s!", Level, game::Insult());
      return false;
    }

  if(Level == game::GetCurrentLevelIndex() + 1)
    {
      ADD_MESSAGE("You are already here, %s!", game::Insult());
      return false;
    }

  return game::TryTravel(game::GetCurrentDungeonIndex(), Level - 1, RANDOM, true);
}

bool commandsystem::WieldInRightArm(character* Char)
{
  if(!Char->CanUseEquipment())
    ADD_MESSAGE("You cannot wield anything.");
  else if(Char->TryToChangeEquipment(RIGHT_WIELDED_INDEX))
    {
      Char->DexterityAction(5);
      return true;
    }

  return false;
}

bool commandsystem::WieldInLeftArm(character* Char)
{
  if(!Char->CanUseEquipment())
    ADD_MESSAGE("You cannot wield anything.");
  else if(Char->TryToChangeEquipment(LEFT_WIELDED_INDEX))
    {
      Char->DexterityAction(5);
      return true;
    }

  return false;
}

bool commandsystem::ShowBattleInfo(character* Char)
{
  Char->ShowBattleInfo();
  return false;
}

bool commandsystem::Search(character* Char)
{
  Char->Search(Char->GetAttribute(PERCEPTION) << 2);
  return true;
}
