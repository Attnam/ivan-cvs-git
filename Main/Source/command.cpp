/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

#include "command.h"
#include "char.h"
#include "message.h"
#include "game.h"
#include "stack.h"
#include "room.h"
#include "god.h"
#include "felist.h"
#include "iconf.h"
#include "bitmap.h"
#include "actions.h"
#include "miscitem.h"
#include "worldmap.h"
#include "wsquare.h"
#include "wterras.h"
#include "materia.h"
#include "database.h"

#ifdef WIZARD
#include "proto.h"
#endif

command::command(bool (*LinkedFunction)(character*), const char* Description, char Key1, char Key2, bool UsableInWilderness, bool WizardModeFunction) : LinkedFunction(LinkedFunction), Description(Description), Key1(Key1), Key2(Key2), UsableInWilderness(UsableInWilderness), WizardModeFunction(WizardModeFunction) { }

char command::GetKey() const { return !ivanconfig::GetUseAlternativeKeys() ? Key1 : Key2; }

command* commandsystem::Command[] =
{
  0,

  /* Sort according to description */

  new command(&Apply, "apply", 'a', 'a', false),
  new command(&Talk, "chat", 'C', 'C', false),
  new command(&Close, "close", 'c', 'c', false),
  new command(&Dip, "dip", '!', '!', true),
  new command(&Drink, "drink", 'D', 'D', true),
  new command(&Drop, "drop", 'd', 'd', true),
  new command(&Eat, "eat", 'e', 'e', true),
  new command(&WhatToEngrave, "engrave", 'G', 'G', false),
  new command(&EquipmentScreen, "equipment menu", 'E', 'E', true),
  new command(&Go, "go", 'g', 'g', false),
  new command(&GoDown, "go down/enter area", '>', '>', true),
  new command(&GoUp, "go up", '<', '<', true),
  new command(&IssueCommand, "issue command(s) to team member(s)", 'I', 'I', false),
  new command(&Kick, "kick", 'k', 'K', false),
  new command(&Look, "look", 'l', 'L', true),
  new command(&AssignName, "name", 'n', 'n', false),
  new command(&Offer, "offer", 'O', 'f', false),
  new command(&Open, "open", 'o', 'O', false),
  new command(&PickUp, "pick up", ',', ',', false),
  new command(&Pray, "pray", 'p', 'p', false),
  new command(&Quit, "quit", 'Q', 'Q', true),
  new command(&Read, "read", 'r', 'r', false),
  new command(&Rest, "rest/heal", 'h', 'h', true),
  new command(&Save, "save game", 'S', 'S', true),
  new command(&ScrollMessagesDown, "scroll messages down", '+', '+', true),
  new command(&ScrollMessagesUp, "scroll messages up", '-', '-', true),
  new command(&ShowConfigScreen, "show config screen", '\\', '\\', true),
  new command(&ShowInventory, "show inventory", 'i', 'i', true),
  new command(&ShowKeyLayout, "show key layout", '?', '?', true),
  new command(&DrawMessageHistory, "show message history", 'M', 'M', true),
  new command(&ShowWeaponSkills, "show weapon skills", '@', '@', true),
  new command(&Search, "search", 's', 's', false),
  new command(&Sit, "sit", '_', '_', false),
  new command(&Throw, "throw", 't', 't', false),
  new command(&ToggleRunning, "toggle running", 'u', 'U', true),
  new command(&ForceVomit, "vomit", 'V', 'V', false),
  new command(&NOP, "wait", '.', '.', true),
  new command(&WieldInRightArm, "wield in right arm", 'w', 'w', true),
  new command(&WieldInLeftArm, "wield in left arm", 'W', 'W', true),
#ifdef WIZARD
  new command(&WizardMode, "wizard mode activation", 'X', 'X', true),
#endif
  new command(&Zap, "zap", 'z', 'z', false),

#ifdef WIZARD

  /* Sort according to key */

  new command(&RaiseStats, "raise stats", '1', '1', true, true),
  new command(&LowerStats, "lower stats", '2', '2', true, true),
  new command(&SeeWholeMap, "see whole map", '3', '3', true, true),
  new command(&WalkThroughWalls, "toggle walk through walls mode", '4', '4', true, true),
  new command(&RaiseGodRelations, "raise your relations to the gods", '5', '5', true, true),
  new command(&LowerGodRelations, "lower your relations to the gods", '6', '6', true, true),
  new command(&GainDivineKnowledge, "gain knowledge of all gods", '\"', '\"', true, true),
  new command(&GainAllItems, "gain all items", '$', '$', true, true),
  new command(&SecretKnowledge, "reveal secret knowledge", '*', '*', true, true),
  new command(&DetachBodyPart, "detach a limb", '0', '0', true, true),
  /*new command(&ReloadDatafiles, "reload datafiles", 'R', 'R', true, true),*/
  new command(&SummonMonster, "summon monster", '&', '&', false, true),
  new command(&LevelTeleport, "level teleport", '|', '|', false, true),
  new command(&Possess, "possess creature", '{', '{', false, true),

#endif

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
	  if(!(Char->GetMoveType() & FLY))
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
      Char->EditExperience(LEG_STRENGTH, 150, 1 << 6);
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
      Char->EditExperience(AGILITY, 150, 1 << 6);
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
      bool OpenableItems = Char->GetStack()->SortedItems(Char, &item::IsOpenable);

      if(OpenableItems)
	Key = game::AskForKeyPress(CONST_S("What do you wish to open? [press a direction key, space or i]"));
      else
	Key = game::AskForKeyPress(CONST_S("What do you wish to open? [press a direction key or space]"));

      if(Key == 'i' && OpenableItems)
	{
	  item* Item = Char->GetStack()->DrawContents(Char, CONST_S("What do you want to open?"), 0, &item::IsOpenable);
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
      int Dir = game::DirectionQuestion(CONST_S("What do you wish to close?  [press a direction key]"), false);

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
      Char->GetStack()->DrawContents(ToDrop, Char, CONST_S("What do you want to drop?"), REMEMBER_SELECTED);

      if(ToDrop.empty())
	break;

      if(game::IsInWilderness())
	{
	  for(uint c = 0; c < ToDrop.size(); ++c)
	    {
	      if(game::BoolQuestion(CONST_S("Are you sure? You will never see ") + ToDrop[c]->CHAR_NAME(DEFINITE)
				    + " again! [y/N]"))
		{

		  ADD_MESSAGE("You drop %s.", ToDrop[c]->CHAR_NAME(DEFINITE));
		  ToDrop[c]->RemoveFromSlot();
		  ToDrop[c]->SendToHell();
		}
	    }
	}
      else if(!Char->GetRoom() || Char->GetRoom()->DropItem(Char, ToDrop[0], ToDrop.size()))
	{
	  for(uint c = 0; c < ToDrop.size(); ++c)
	    ToDrop[c]->MoveTo(Char->GetStackUnder());

	  ADD_MESSAGE("%s dropped.", ToDrop[0]->GetName(INDEFINITE, ToDrop.size()).CStr());
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
  if(!Char->CheckConsume(CONST_S("eat")))
    return false;

  lsquare* Square = Char->GetLSquareUnder();

  if(!game::IsInWilderness() && Square->GetOLTerrain() && Square->GetOLTerrain()->HasEatEffect())
    {
      if(Square->GetOLTerrain()->Eat(Char))
	return true;
    }

  return Consume(Char, "eat", &item::IsEatable);
}

bool commandsystem::Drink(character* Char)
{
  if(!Char->CheckConsume(CONST_S("drink")))
    return false;

  lsquare* Square = Char->GetLSquareUnder();

  if(!game::IsInWilderness() && Square->GetOLTerrain() && Square->GetOLTerrain()->HasDrinkEffect())
    {
      if(Square->GetOLTerrain()->Drink(Char))
	return true;
    }

  return Consume(Char, "drink", &item::IsDrinkable);
}

bool commandsystem::Consume(character* Char, const char* ConsumeVerb, sorter Sorter)
{
  lsquare* Square = Char->GetLSquareUnder();
  stack* Inventory = Char->GetStack();
  stack* StackUnder = Square->GetStack();

  if((game::IsInWilderness() || !StackUnder->SortedItems(Char, Sorter)) && !Inventory->SortedItems(Char, Sorter))
    {
      ADD_MESSAGE("You have nothing to %s!", ConsumeVerb);
      return false;
    }

  itemvector Item;
  festring Question = CONST_S("What do you wish to ") + ConsumeVerb + '?';

  if(!game::IsInWilderness() && StackUnder->SortedItems(Char, Sorter))
    Inventory->DrawContents(Item, StackUnder, Char, Question, CONST_S("Items in your inventory"), CONST_S("Items on the ground"), CONST_S(""), 0, NO_MULTI_SELECT, Sorter);
  else
    Inventory->DrawContents(Item, Char, Question, NO_MULTI_SELECT, Sorter);

  return !Item.empty() ? Char->ConsumeItem(Item[0], ConsumeVerb + CONST_S("ing")) : false;
}

bool commandsystem::ShowInventory(character* Char)
{
  Char->GetStack()->DrawContents(Char, CONST_S("Your inventory"), NO_SELECT);
  return false;
}

bool commandsystem::PickUp(character* Char)
{
  if(!Char->GetStackUnder()->GetVisibleItems(Char))
    {
      ADD_MESSAGE("There is nothing here to pick up!");
      return false;
    }

  std::vector<itemvector> PileVector;
  Char->GetStackUnder()->Pile(PileVector, Char, CENTER);

  for(int c = 0; c < 4; ++c)
    {
      stack* Stack = Char->GetLSquareUnder()->GetStackOfAdjacentSquare(c);

      if(Stack)
	Stack->Pile(PileVector, Char, 3 - c);
    }

  if(PileVector.size() == 1)
    if(PileVector[0][0]->CanBePickedUp())
      {
	int Amount = PileVector[0].size();

	if(Amount > 1)
	  Amount = game::ScrollBarQuestion(CONST_S("How many ") + PileVector[0][0]->GetName(PLURAL) + '?', vector2d(16, 6), Amount, 1, 0, Amount, 0, WHITE, LIGHT_GRAY, DARK_GRAY);

	if(!Amount)
	  return false;

	if((!Char->GetRoom() || Char->GetRoom()->PickupItem(Char, PileVector[0][0], Amount)) && PileVector[0][0]->CheckPickUpEffect(Char))
	  {
	    for(int c = 0; c < Amount; ++c)
	      PileVector[0][c]->MoveTo(Char->GetStack());

	    ADD_MESSAGE("%s picked up.", PileVector[0][0]->GetName(INDEFINITE, Amount).CStr());
	    Char->DexterityAction(2);
	    return true;
	  }
	else
	  return false;
      }
    else
      {
	ADD_MESSAGE("%s too large to pick up!", PileVector[0].size() == 1 ? "It is" : "They are");
	return false;
      }

  bool Success = false;
  stack::SetSelected(0);

  for(;;)
    {
      itemvector ToPickup;
      game::DrawEverythingNoBlit();
      Char->GetStackUnder()->DrawContents(ToPickup, Char, CONST_S("What do you want to pick up?"), REMEMBER_SELECTED);

      if(ToPickup.empty())
	break;

      if(ToPickup[0]->CanBePickedUp())
	{
	  if((!Char->GetRoom() || Char->GetRoom()->PickupItem(Char, ToPickup[0], ToPickup.size())) && ToPickup[0]->CheckPickUpEffect(Char))
	    {
	      for(uint c = 0; c < ToPickup.size(); ++c)
		ToPickup[c]->MoveTo(Char->GetStack());

	      ADD_MESSAGE("%s picked up.", ToPickup[0]->GetName(INDEFINITE, ToPickup.size()).CStr());
	      Success = true;
	    }
	}
      else
	ADD_MESSAGE("%s too large to pick up!", ToPickup.size() == 1 ? "It is" : "They are");
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
  if(game::BoolQuestion(CONST_S("Your quest is not yet compeleted! Really quit? [y/N]")))
    {
      Char->ShowAdventureInfo();
      Char->AddScoreEntry(CONST_S("cowardly quit the game"), 0.75);
      game::End(!game::WizardModeIsActive() || game::BoolQuestion(CONST_S("Remove saves? [y/N]")));
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
  int Characters = 0;

  for(int d = 0; d < 8; ++d)
    {
      lsquare* Square = Char->GetNaturalNeighbourLSquare(d);

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
    return ToTalk->ChatMenu();
  else
    {
      int Dir = game::DirectionQuestion(CONST_S("To whom do you wish to talk to? [press a direction key]"), false, true);

      if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
	return false;

      character* Dude = Char->GetNearSquare(Char->GetPos() + game::GetMoveVector(Dir))->GetCharacter();

      if(Dude == Char)
	{
	  ADD_MESSAGE("You talk to yourself for some time.");
	  Char->EditExperience(WISDOM, -50, 1 << 7);
	  Char->EditAP(-1000);
	  return true;
	}
      else if(Dude)
	return Dude->ChatMenu();
      else
	ADD_MESSAGE("You get no response.");
    }

  return false;
}

bool commandsystem::NOP(character* Char)
{
  /*{
    //humanoid* H = static_cast<humanoid*>(Char);

    for(int c = 0; c < Char->GetBodyParts(); ++c)
      Char->GetBodyPart(c)->EditHP(-Char->GetBodyPart(c)->GetHP() / 2);
  }*/

  //game::GetScore();
  Char->EditExperience(DEXTERITY, -25, 1 << 3);
  Char->EditExperience(AGILITY, -25, 1 << 3);
  Char->EditAP(-Char->GetStateAPGain(1000));
  return true;
}

bool commandsystem::Save(character*)
{
  if(game::BoolQuestion(CONST_S("Do you truly wish to save and flee? [y/N]")))
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
  if(!Char->CanRead() && !game::GetSeeWholeMapCheatMode())
    {
      ADD_MESSAGE("You can't read.");
      return false;
    }

  if(!Char->GetStack()->SortedItems(Char, &item::IsReadable))
    {
      ADD_MESSAGE("You have nothing to read!");
      return false;
    }

  if(Char->GetLSquareUnder()->IsDark() && !game::GetSeeWholeMapCheatMode())
    {
      ADD_MESSAGE("It is too dark to read.");
      return false;
    }

  item* Item = Char->GetStack()->DrawContents(Char, CONST_S("What do you want to read?"), 0, &item::IsReadable);
  return Item && Char->ReadItem(Item);
}

bool commandsystem::Dip(character* Char)
{
  if(!Char->GetStack()->SortedItems(Char, &item::IsDippable) && !Char->EquipsSomething(&item::IsDippable))
    {
      ADD_MESSAGE("You have nothing to dip!");
      return false;
    }

  bool HasDipDestination = Char->GetStack()->SortedItems(Char, &item::IsDipDestination) || Char->EquipsSomething(&item::IsDipDestination);
  bool DipDestinationNear = false;

  for(int d = 0; d < 9; ++d)
    {
      lsquare* Square = Char->GetNaturalNeighbourLSquare(d);

      if(Square && Square->IsDipDestination())
	DipDestinationNear = true;
    }

  if(!HasDipDestination && !DipDestinationNear)
    {
      ADD_MESSAGE("There is nothing to dip anything into.");
      return false;
    }

  item* Item = Char->SelectFromPossessions(CONST_S("What do you want to dip?"), &item::IsDippable);

  if(Item)
    {
      if(!HasDipDestination || (DipDestinationNear && game::BoolQuestion(CONST_S("Do you wish to dip in a nearby square? [y/N]"))))
	{
	  int Dir = game::DirectionQuestion(CONST_S("Where do you want to dip ") + Item->GetName(DEFINITE) + "? [press a direction key or '.']", false, true);
	  vector2d Pos = Char->GetPos() + game::GetMoveVector(Dir);

	  if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Pos) || !Char->GetNearLSquare(Pos)->IsDipDestination())
	    return false;
	  
	  return Char->GetNearLSquare(Pos)->DipInto(Item, Char);
	}
      else
	{
	  item* DipTo = Char->SelectFromPossessions(CONST_S("Into what do you wish to dip it?"), &item::IsDipDestination);

	  if(DipTo)
	    {
	      if(Item == DipTo)
		{
		  ADD_MESSAGE("Very funny...");
		  return false;
		}

	      Item->DipInto(DipTo->CreateDipLiquid(), Char);
	      return true;
	    }
	}
    }

  return false;
}

bool commandsystem::ShowKeyLayout(character*)
{
  felist List(CONST_S("Keyboard Layout"));
  List.AddDescription(CONST_S(""));
  List.AddDescription(CONST_S("Key       Description"));
  festring Buffer;

  for(int c = 1; GetCommand(c); ++c)
    if(!GetCommand(c)->IsWizardModeFunction())
      {
	Buffer.Empty();
	Buffer << GetCommand(c)->GetKey();
	Buffer.Resize(10);
	List.AddEntry(Buffer + GetCommand(c)->GetDescription(), LIGHT_GRAY);
      }

  if(game::WizardModeIsActive())
    {
      List.AddEntry(CONST_S(""), WHITE);
      List.AddEntry(CONST_S("Wizard mode functions:"), WHITE);
      List.AddEntry(CONST_S(""), WHITE);

      for(int c = 1; GetCommand(c); ++c)
	if(GetCommand(c)->IsWizardModeFunction())
	  {
	    Buffer.Empty();
	    Buffer << GetCommand(c)->GetKey();
	    Buffer.Resize(10);
	    List.AddEntry(Buffer + GetCommand(c)->GetDescription(), LIGHT_GRAY);
	  }
    }

  game::SetStandardListAttributes(List);
  List.Draw();
  return false;
}

bool commandsystem::Look(character* Char)
{
  festring Msg;

  if(!game::IsInWilderness())
    Msg = CONST_S("Direction keys move cursor, ESC exits, 'i' examines items, 'c' examines a character.");
  else
    Msg = CONST_S("Direction keys move cursor, ESC exits, 'c' examines a character.");

  game::PositionQuestion(Msg, Char->GetPos(), &game::LookHandler, &game::LookKeyHandler, ivanconfig::GetLookZoom());
  return false;
}

bool commandsystem::WhatToEngrave(character* Char)
{
  if(!Char->CanRead())
    {
      ADD_MESSAGE("You can't even read.");
      return false;
    }

  Char->GetNearLSquare(Char->GetPos())->Engrave(game::StringQuestion(CONST_S("What do you want to engrave here?"), vector2d(16, 6), WHITE, 0, 80, true));
  return false;
}

bool commandsystem::Pray(character* Char)
{
  felist Panthenon(CONST_S("To Whom you want to address your prayers?"));
  Panthenon.SetEntryDrawer(game::GodEntryDrawer);
  int Known[GODS];
  int Index = 0;
  int DivineMaster = Char->GetLSquareUnder()->GetDivineMaster();

  if(DivineMaster == ATHEIST)
    {
      ADD_MESSAGE("Somehow you feel that no god will help you here.");
      return false;
    }

  if(!DivineMaster)
    {
      for(int c = 1; c <= GODS; ++c)
	if(game::GetGod(c)->IsKnown())
	  {
	    Panthenon.AddEntry(game::GetGod(c)->GetCompleteDescription(), LIGHT_GRAY, 20, c);
	    Known[Index++] = c;
	  }
    }
  else
    if(game::GetGod(DivineMaster)->IsKnown())
      {
	Panthenon.AddEntry(game::GetGod(DivineMaster)->GetCompleteDescription(), LIGHT_GRAY, 20, DivineMaster);
	Known[0] = DivineMaster;
      }
    else
      {
	ADD_MESSAGE("Somehow you feel that no deity you know can hear your prayers from this place.");
	return false;
      }

  game::SetStandardListAttributes(Panthenon);
  Panthenon.AddFlags(SELECTABLE);
  int Select = Panthenon.Draw();

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
	      if(game::BoolQuestion(CONST_S("Do you really wish to pray to ") + game::GetGod(Char->GetLSquareUnder()->GetDivineMaster())->GetName() + "? [y/N]"))
		game::GetGod(Char->GetLSquareUnder()->GetDivineMaster())->Pray();
	      else
		return false;
	    }
	  else
	    return false;
	}
      else
	{
	  if(game::BoolQuestion(CONST_S("Do you really wish to pray to ") + game::GetGod(Known[Select])->GetName() + "? [y/N]"))
	    {
	      if(Char->StateIsActivated(CONFUSED) && !(RAND() & 7))
		{
		  int Index;
		  for(Index = 1 + RAND() % GODS;
		      Index == Known[Select];
		      Index = 1 + RAND() % GODS);

		  if(game::GetGod(Index)->IsKnown())
		    ADD_MESSAGE("You feel something went wrong in the rituals. You have accidentally prayed to %s!", game::GetGod(Index)->GetName());
		  else
		    ADD_MESSAGE("Your rituals were seriously incorrect. You have accidentally prayed to an unknown god, but have no idea how!");

		  game::GetGod(Index)->Pray();
		}
	      else
		game::GetGod(Known[Select])->Pray();
	    }
	  else
	    return false;
	}

      Char->EditAP(-1000);
      return true;
    }
}

bool commandsystem::Kick(character* Char)
{
  /** No multi-tile support */

  if(!Char->CheckKick())
    return false;

  if(Char->GetBurdenState() == OVER_LOADED)
    {
      ADD_MESSAGE("You try to kick, but you collapse under your load.");
      Char->EditAP(-100000 / APBonus(Char->GetAttribute(AGILITY)));
      return true;
    }

  int Dir = game::DirectionQuestion(CONST_S("In what direction do you wish to kick? [press a direction key]"), false);

  if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
    return false;

  lsquare* Square = Char->GetNearLSquare(Char->GetPos() + game::GetMoveVector(Dir));

  if(!Square->CheckKick(Char))
    return false;

  if(Square->GetCharacter() && Char->GetRelation(Square->GetCharacter()) != HOSTILE)
    if(!game::BoolQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
      return false;
    else
      Char->Hostility(Square->GetCharacter());

  Char->Kick(Square, Dir);
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

      item* Item = Char->GetStack()->DrawContents(Char, CONST_S("What do you want to offer?"));

      if(Item)
	{
	  if(game::GetGod(Square->GetDivineMaster())->ReceiveOffer(Item))
	    {
	      Item->RemoveFromSlot();
	      Item->SendToHell();
	      Char->DexterityAction(5); /** C **/
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

  item* Item = Char->GetStack()->DrawContents(Char, CONST_S("What do you want to throw?"));

  if(Item)
    {
      int Answer = game::DirectionQuestion(CONST_S("In what direction do you wish to throw?  [press a direction key]"), false);

      if(Answer == DIR_ERROR)
	return false;

      Char->ThrowItem(Answer, Item);
      Char->EditExperience(ARM_STRENGTH, 75, 1 << 8);
      Char->EditExperience(DEXTERITY, 75, 1 << 8);
      Char->EditExperience(PERCEPTION, 75, 1 << 8);
      Char->EditNP(-50);
      Char->DexterityAction(5);
      return true;
    }
  else
    return false;
}

bool commandsystem::Apply(character* Char)
{
  if(!Char->CanApply())
    {
      ADD_MESSAGE("This monster type cannot apply.");
      return false;
    }
  if(!Char->GetStack()->SortedItems(Char, &item::IsAppliable) && !Char->EquipsSomething(&item::IsAppliable))
    {
      ADD_MESSAGE("You have nothing to apply!");
      return false;
    }

  item* Item = Char->SelectFromPossessions(CONST_S("What do you want to apply?"), &item::IsAppliable);
  return Item && Item->Apply(Char);
}

bool commandsystem::ForceVomit(character* Char)
{
  if(Char->CanVomit())
    {
      int Dir = game::DirectionQuestion(CONST_S("Where do you wish to vomit?  [press a direction key]"), false, true);

      if(Dir != DIR_ERROR && Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
	{
	  ADD_MESSAGE("You push your fingers down to your throat and vomit.");
	  Char->Vomit(Char->GetPos() + game::GetMoveVector(Dir), 500 + RAND() % 500);
	  Char->EditAP(-1000);
	  return true;
	}
    }
  else
    ADD_MESSAGE("You can't vomit.");

  return false;
}

bool commandsystem::Zap(character* Char)
{
  if(!Char->CheckZap())
    return false;

  if(!Char->GetStack()->SortedItems(Char, &item::IsZappable) && !Char->EquipsSomething(&item::IsZappable))
    {
      ADD_MESSAGE("You have nothing to zap with, %s.", game::Insult());
      return false;
    }

  item* Item = Char->SelectFromPossessions(CONST_S("What do you want to zap with?"), &item::IsZappable);

  if(Item)
    {
      int Answer = game::DirectionQuestion(CONST_S("In what direction do you wish to zap?  [press a direction key or '.']"), false, true);

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

bool commandsystem::Rest(character* Char)
{
  if(Char->StateIsActivated(PANIC))
    {
      ADD_MESSAGE("You are too scared to rest.");
      return false;
    }

  bool Error = false;

  if(Char->GetHP() == Char->GetMaxHP())
    {
      ADD_MESSAGE("You HP is already at its maximum.");
      Error = true;
    }
  else if(!Char->CanHeal())
    {
      ADD_MESSAGE("You cannot heal.");
      Error = true;
    }

  if(Error)
    {
      long MinutesToRest = game::NumberQuestion(CONST_S("How many minutes to wait?"), vector2d(16, 6), WHITE);

      if(MinutesToRest > 0)
	{
	  oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

	  if(Terrain)
	    Terrain->ShowRestMessage(Char);

	  rest* Rest = new rest(Char);
	  Rest->SetMinToStop(game::GetTotalMinutes() + MinutesToRest);
	  Rest->SetGoalHP(0);
	  Char->SetAction(Rest);
	  return true;
	}
      else
	return false;
    }

  long HPToRest = game::ScrollBarQuestion(CONST_S("How many hit points you desire?"), vector2d(16, 6), Char->GetMaxHP(), 1, 0, Char->GetMaxHP(), 0, WHITE, LIGHT_GRAY, DARK_GRAY);

  if(HPToRest <= Char->GetHP())
    {
      ADD_MESSAGE("Your HP is already %d.", Char->GetHP());
      return false;
    }

  oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

  if(Terrain)
    Terrain->ShowRestMessage(Char);

  rest* Rest = new rest(Char);
  Rest->SetMinToStop(0);
  Rest->SetGoalHP(HPToRest);
  Char->SetAction(Rest);
  return true;
}

bool commandsystem::Sit(character* Char)
{
  lsquare* Square = Char->GetLSquareUnder();
  return (Square->GetOLTerrain() && Square->GetOLTerrain()->SitOn(Char)) || Square->GetGLTerrain()->SitOn(Char);
}

bool commandsystem::Go(character* Char)
{
  /*if(Char->StateIsActivated(CONFUSED))
    {
      ADD_MESSAGE("In this mental state you would forget your destination immediately.");
      return false;
    }*/

  int Dir = game::DirectionQuestion(CONST_S("In what direction do you want to go? [press a direction key]"), false);

  if(Dir == DIR_ERROR)
    return false;

  go* Go = new go(Char);
  Go->SetDirection(Dir);
  int OKDirectionsCounter = 0;

  for(int d = 0; d < Char->GetNeighbourSquares(); ++d)
    {
      lsquare* Square = Char->GetNeighbourLSquare(d);

      if(Square && Char->CanMoveOn(Square))
	++OKDirectionsCounter;
    }

  Go->SetIsWalkingInOpen(OKDirectionsCounter > 2);
  Char->SetAction(Go);
  Char->EditAP(Char->GetStateAPGain(100)); // gum solution
  Char->GoOn(Go, true);
  return !!Char->GetAction();
}

bool commandsystem::ShowConfigScreen(character*)
{
  ivanconfig::Show();
  return false;
}

bool commandsystem::AssignName(character* Char)
{
  game::PositionQuestion(CONST_S("What do you want to name? [press 'n' over a tame creature or ESC to exit]"), Char->GetPos(), 0, &game::NameKeyHandler);
  return false;
}

bool commandsystem::EquipmentScreen(character* Char)
{
  return Char->EquipmentScreen(Char->GetStack(), 0);
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
  felist List(CONST_S("Your experience in weapon categories"));
  List.AddDescription(CONST_S(""));
  List.AddDescription(CONST_S("Category name                 Level     Points    Needed    Battle bonus"));
  bool Something = false;
  festring Buffer;

  for(int c = 0; c < Char->GetAllowedWeaponSkillCategories(); ++c)
    {
      cweaponskill* Skill = Char->GetCWeaponSkill(c);

      if(Skill->GetHits() / 100)
	{
	  Buffer = Skill->GetName(c);
	  Buffer.Resize(30);
	  Buffer << Skill->GetLevel();
	  Buffer.Resize(40);
	  Buffer << Skill->GetHits() / 100;
	  Buffer.Resize(50);

	  if(Skill->GetLevel() != 20)
	    Buffer << (Skill->GetLevelMap(Skill->GetLevel() + 1) - Skill->GetHits()) / 100;
	  else
	    Buffer << '-';

	  Buffer.Resize(60);
	  Buffer << '+' << (Skill->GetBonus() - 1000) / 10;

	  if(Skill->GetBonus() % 10)
	    Buffer << '.' << Skill->GetBonus() % 10;

	  Buffer << '%';

	  if((PLAYER->GetMainWielded() && PLAYER->GetMainWielded()->GetWeaponCategory() == c)
	  || (PLAYER->GetSecondaryWielded() && PLAYER->GetSecondaryWielded()->GetWeaponCategory() == c))
	    List.AddEntry(Buffer, WHITE);
	  else
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

bool commandsystem::WieldInRightArm(character* Char)
{
  if(!Char->CanUseEquipment())
    ADD_MESSAGE("You cannot wield anything.");
  else if(Char->TryToChangeEquipment(Char->GetStack(), 0, RIGHT_WIELDED_INDEX))
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
  else if(Char->TryToChangeEquipment(Char->GetStack(), 0, LEFT_WIELDED_INDEX))
    {
      Char->DexterityAction(5);
      return true;
    }

  return false;
}

bool commandsystem::Search(character* Char)
{
  Char->Search(Char->GetAttribute(PERCEPTION) << 2);
  return true;
}

#ifdef WIZARD

bool commandsystem::WizardMode(character* Char)
{
  if(!game::WizardModeIsActive())
    {
      if(game::BoolQuestion(CONST_S("Do you want to cheat, cheater? This action cannot be undone. [y/N]")))
	{
	  game::ActivateWizardMode();
	  ADD_MESSAGE("Wizard mode activated.");

	  for(int c = 0; c < 5; ++c)
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

      for(int c = 0; c < 5; ++c)
	Char->GetStack()->AddItem(new scrollofwishing);
    }

  return false;
}

bool commandsystem::RaiseStats(character* Char)
{
  Char->EditAllAttributes(10);
  return false;
}

bool commandsystem::LowerStats(character* Char)
{
  Char->EditAllAttributes(-10);
  return false;
}

bool commandsystem::GainAllItems(character* Char)
{
  itemvector AllItems;
  protosystem::CreateEveryItem(AllItems);
  stack* Stack = game::IsInWilderness() ? Char->GetStack() : Char->GetStackUnder();

  for(uint c = 0; c < AllItems.size(); ++c)
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

bool commandsystem::RaiseGodRelations(character*)
{
  for(int c = 1; c <= GODS; ++c)
    game::GetGod(c)->AdjustRelation(50);

  return false;
}

bool commandsystem::LowerGodRelations(character*)
{
  for(int c = 1; c <= GODS; ++c)
    game::GetGod(c)->AdjustRelation(-50);

  return false;
}

bool commandsystem::GainDivineKnowledge(character*)
{
  for(int c = 1; c <= GODS; ++c)
    game::GetGod(c)->SetIsKnown(true);

  return false;
}

bool commandsystem::SecretKnowledge(character* Char)
{
  felist List(CONST_S("Knowledge of the ancients"));
  List.AddEntry(CONST_S("Character attributes"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Character attack info"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Character defence info"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Character danger values"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Item attack info"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Miscellaneous item info"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Material info"), LIGHT_GRAY);
  game::SetStandardListAttributes(List);
  List.AddFlags(SELECTABLE);
  uint c, Chosen = List.Draw();
  festring Entry;

  if(Chosen & FELIST_ERROR_BIT)
    return false;

  List.Empty();
  List.RemoveFlags(SELECTABLE);

  if(Chosen < 4)
    {
      charactervector& Character = game::GetCharacterDrawVector();
      protosystem::CreateEveryCharacter(Character);
      List.SetEntryDrawer(game::CharacterEntryDrawer);

      switch(Chosen)
	{
	case 0:
	  List.AddDescription(CONST_S("                                                AS LS DX AG EN PE IN WI CH MA"));

	  for(c = 0; c < Character.size(); ++c)
	    {
	      Entry.Empty();
	      Character[c]->AddName(Entry, UNARTICLED);
	      Character[c]->AddAttributeInfo(Entry);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, c);
	    }

	  List.SetPageLength(15);
	  break;
	case 1:
	  List.AddDescription(CONST_S("                                                  BD        THV       APC"));

	  for(c = 0; c < Character.size(); ++c)
	    {
	      Entry.Empty();
	      Character[c]->AddName(Entry, UNARTICLED);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, c);
	      Character[c]->AddAttackInfo(List);
	    }

	  List.SetPageLength(20);
	  break;
	case 2:
	  List.AddDescription(CONST_S("                                                  DV/BV     HP        AV/BS"));

	  for(c = 0; c < Character.size(); ++c)
	    {
	      Entry.Empty();
	      Character[c]->AddName(Entry, UNARTICLED);
	      Entry.Resize(47);
	      Entry << int(Character[c]->GetDodgeValue());
	      Entry.Resize(57);
	      Entry << Character[c]->GetMaxHP();
	      List.AddEntry(Entry, LIGHT_GRAY, 0, c);
	      Character[c]->AddDefenceInfo(List);
	    }

	  List.SetPageLength(25);
	  break;
	case 3:
	  List.AddDescription(CONST_S("                                                  Danger    NGM       EGM"));

	  for(c = 0; c < Character.size(); ++c)
	    {
	      Entry.Empty();
	      Character[c]->AddName(Entry, UNARTICLED);
	      Entry.Resize(47);
	      Entry << int(Character[c]->GetRelativeDanger(Char, true) * 1000);
	      Entry.Resize(57);
	      const dangerid& DI = game::GetDangerMap().find(configid(Character[c]->GetType(), Character[c]->GetConfig()))->second;
	      Entry << int(DI.NakedDanger * 1000);
	      Entry.Resize(67);
	      Entry << int(DI.EquippedDanger * 1000);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, c);
	    }

	  List.SetPageLength(15);
	  break;
	}

      List.Draw();

      for(c = 0; c < Character.size(); ++c)
	delete Character[c];

      game::ClearCharacterDrawVector();
    }
  else if(Chosen < 6)
    {
      itemvector& Item = game::GetItemDrawVector();
      protosystem::CreateEveryItem(Item);
      List.SetEntryDrawer(game::ItemEntryDrawer);
      List.SetPageLength(20);

      switch(Chosen)
	{
	case 4:
	  List.AddDescription(CONST_S("                                        Weight    Size      SR        DAM"));

	  for(c = 0; c < Item.size(); ++c)
	    {
	      Entry.Empty();
	      Item[c]->AddName(Entry, UNARTICLED);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, c, true);
	      Item[c]->AddAttackInfo(List);
	    }

	  break;
	case 5:
	  List.AddDescription(CONST_S("                                        \177              OV             NV"));

	  for(c = 0; c < Item.size(); ++c)
	    {
	      Entry.Empty();
	      Item[c]->AddName(Entry, UNARTICLED);
	      List.AddEntry(Entry, LIGHT_GRAY, 0, c, true);
	      Item[c]->AddMiscellaneousInfo(List);
	    }

	  break;
	}

      List.Draw();

      for(c = 0; c < Item.size(); ++c)
	delete Item[c];

      game::ClearItemDrawVector();
    }
  else if(Chosen < 7)
    {
      std::vector<material*> Material;
      protosystem::CreateEveryMaterial(Material);
      List.SetPageLength(30);
      List.AddDescription(CONST_S("                                        Strength       Flexibility   Density"));

      for(c = 0; c < Material.size(); ++c)
	{
	  Entry.Empty();
	  Material[c]->AddName(Entry, false, false);
	  Entry.Resize(40);
	  Entry << Material[c]->GetStrengthValue();
	  Entry.Resize(55);
	  Entry << int(Material[c]->GetFlexibility());
	  Entry.Resize(70);
	  Entry << int(Material[c]->GetDensity());
	  List.AddEntry(Entry, Material[c]->GetColor());
	}

      List.Draw();

      for(c = 0; c < Material.size(); ++c)
	delete Material[c];
    }

  List.PrintToFile(game::GetHomeDir() + "secret" + Chosen + ".txt");
  ADD_MESSAGE("Info written also to %ssecret%d.txt.", game::GetHomeDir().CStr(), Chosen);
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
    {
      festring Temp = game::DefaultQuestion(CONST_S("Summon which monster?"),
					    game::GetDefaultSummonMonster());
      Summoned = protosystem::CreateMonster(Temp);
    }

  Summoned->SetTeam(game::GetTeam(MONSTER_TEAM));
  Summoned->PutNear(Char->GetPos());
  return false;
}

bool commandsystem::LevelTeleport(character*)
{
  long Level = game::NumberQuestion(CONST_S("To which level?"), vector2d(16, 6), WHITE);

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

bool commandsystem::Possess(character* Char)
{
  int Dir = game::DirectionQuestion(CONST_S("Choose creature to possess. [press a direction key]"), false);

  if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
    return false;

  character* ToPossess = Char->GetNearLSquare(Char->GetPos() + game::GetMoveVector(Dir))->GetCharacter();

  if(ToPossess)
    {
      Char->SetIsPlayer(false);
      game::SetPlayer(ToPossess);
    }
  else
    ADD_MESSAGE("There's no one to possess, %s!", game::Insult());

  return true; // The old player's turn must end
}

#endif

bool commandsystem::ToggleRunning(character*)
{
  if(game::PlayerIsRunning()
  && PLAYER->StateIsActivated(PANIC)
  && PLAYER->GetTirednessState() != FAINTING)
    {
      ADD_MESSAGE("You are too scared to move at normal pace.");
      return false;
    }

  game::SetPlayerIsRunning(!game::PlayerIsRunning());
  return false;
}

bool commandsystem::IssueCommand(character* Char)
{
  if(!Char->CheckTalk())
    return false;

  return game::CommandQuestion();
}
