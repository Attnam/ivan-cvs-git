/* Compiled through roomset.cpp */

void shop::Enter(character* Customer)
{
  if(Customer->IsPlayer())
    if(MasterIsActive())
      {
	if(GetMaster()->GetRelation(Customer) != HOSTILE && Customer->CanBeSeenBy(GetMaster()))
	  if(GetMaster()->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s welcomes you warmly to the shop.", GetMaster()->CHAR_NAME(DEFINITE));
	  else
	    ADD_MESSAGE("Something welcomes you warmly to the shop.");
      }
    else
      ADD_MESSAGE("The shop appears to be deserted.");
}

/* item* ForSale can also be in chest or other container, so don't assume anything else in this function */

bool shop::PickupItem(character* Customer, item* ForSale, ushort Amount)
{
  if(!MasterIsActive() || Customer == GetMaster() || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  ulong Price = ForSale->GetTruePrice() * Amount * 100 / (100 + Customer->GetAttribute(CHARISMA));

  if(!Customer->IsPlayer())
    if(Customer->CanBeSeenByPlayer() && Customer->GetMoney() >= Price)
      {
	ADD_MESSAGE("%s buys %s.", Customer->CHAR_NAME(DEFINITE), ForSale->GetName(INDEFINITE, Amount).CStr());
	Customer->EditMoney(-Price);
	GetMaster()->EditMoney(Price);
	Customer->EditExperience(CHARISMA, Price);
	return true;
      }
    else
      return false;

  if(Customer->CanBeSeenBy(GetMaster()))
    {
      if(ForSale->IsHeadOfElpuri() || ForSale->IsGoldenEagleShirt() || ForSale->IsPetrussNut() || ForSale->IsTheAvatar() || ForSale->IsEncryptedScroll())
	{
	  ADD_MESSAGE("\"I think it is yours. Take it.\"");
	  return true;
	}

      if(!Price)
	{
	  ADD_MESSAGE("\"Thank you for cleaning that junk out of my floor.\"");
	  return true;
	}

      if(Customer->GetMoney() >= Price)
	{
	  if(Amount == 1)
	    ADD_MESSAGE("\"Ah! That %s costs %d gold pieces. No haggling, please.\"", ForSale->CHAR_NAME(UNARTICLED), Price);
	  else
	    ADD_MESSAGE("\"Ah! Those %d %s cost %d gold pieces. No haggling, please.\"", Amount, ForSale->CHAR_NAME(PLURAL), Price);

	  if(game::BoolQuestion(CONST_S("Do you accept this deal? [y/N]")))
	    {
	      Customer->EditMoney(-Price);
	      GetMaster()->EditMoney(+Price);
	      Customer->EditExperience(CHARISMA, Price);
	      return true;
	    }
	  else
	    return false;
	}
      else
	{
	  if(Amount == 1)
	    ADD_MESSAGE("\"Don't touch that %s, beggar! It is worth %d gold pieces!\"", ForSale->CHAR_NAME(UNARTICLED), Price);
	  else
	    ADD_MESSAGE("\"Don't touch those %s, beggar! They are worth %d gold pieces!\"", ForSale->CHAR_NAME(PLURAL), Price);

	  return false;
	}
    }
  else
    if(game::BoolQuestion(CONST_S("Are you sure you want to commit this thievery? [y/N]")))
      {
	Customer->Hostility(GetMaster());
	return true;
      }
    else
      return false;
}

bool shop::DropItem(character* Customer, item* ForSale, ushort Amount)
{
  if(!MasterIsActive() || Customer == GetMaster() || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  ulong Price = ForSale->GetTruePrice() * Amount * (100 + Customer->GetAttribute(CHARISMA)) / 400;

  if(!Customer->IsPlayer())
    if(Price && Customer->CanBeSeenByPlayer() && GetMaster()->GetMoney() >= Price)
      {
	ADD_MESSAGE("%s sells %s.", Customer->CHAR_NAME(DEFINITE), ForSale->GetName(INDEFINITE, Amount).CStr());
	Customer->EditMoney(Price);
	GetMaster()->EditMoney(-Price);
	Customer->EditExperience(CHARISMA, Price);
	return true;
      }
    else
      return false;

  if(Customer->CanBeSeenBy(GetMaster()))
    {
      if(ForSale->IsHeadOfElpuri() || ForSale->IsGoldenEagleShirt() || ForSale->IsPetrussNut() || ForSale->IsTheAvatar() || ForSale->IsEncryptedScroll())
	{
	  ADD_MESSAGE("\"Oh no! You need it far more than I!\"");
	  return false;
	}

      if(!Price)
	{
	  ADD_MESSAGE("\"Hah! I wouldn't take %s even if you paid me for it!\"", Amount == 1 ? "that" : "those");
	  return false;
	}

      if(GetMaster()->GetMoney() != 0)
	{
	  if(GetMaster()->GetMoney() < Price)
	    Price = GetMaster()->GetMoney();

	  if(Amount == 1)
	    ADD_MESSAGE("\"What a fine %s. I'll pay %d gold pieces for it.\"", ForSale->CHAR_NAME(UNARTICLED), Price);
	  else
	    ADD_MESSAGE("\"What a fine pile of %d %s. I'll pay %d gold pieces for them.\"", Amount, ForSale->CHAR_NAME(PLURAL), Price);

	  if(game::BoolQuestion(CONST_S("Do you accept this deal? [y/N]")))
	    {
	      Customer->SetMoney(Customer->GetMoney() + Price);
	      GetMaster()->SetMoney(GetMaster()->GetMoney() - Price);
	      Customer->EditExperience(CHARISMA, Price);
	      return true;
	    }
	  else
	    return false;
	}
      else
	{
	  ADD_MESSAGE("\"I would pay you %d gold pieces for %s, but I'm temporary short of cash. Sorry.\"", Price, Amount == 1 ? "it" : "them");
	  return false;
	}
    }
  else
    {
      ADD_MESSAGE("The shopkeeper doesn't see you, so you cannot trade with him.");
      return game::BoolQuestion(CONST_S("Still drop ") + (Amount == 1 ? "this item" : "these items") + "? [y/N]");
    }
}

void shop::KickSquare(character* Infidel, lsquare* Square)
{
  if(!AllowKick(Infidel, Square))
    {
      ADD_MESSAGE("\"You infidel!\"");
      Infidel->Hostility(GetMaster());
    }
}

bool shop::ConsumeItem(character* Customer, item*, ushort)
{
  if(!MasterIsActive() || Customer == GetMaster() || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  if(!Customer->IsPlayer())
    return false;

  if(Customer->CanBeSeenBy(GetMaster()))
    {
      ADD_MESSAGE("\"Buy that first, please.\"");
      return false;
    }
  else
    if(game::BoolQuestion(CONST_S("It's illegal to eat property of others. Are you sure you sure? [y/N]")))
      {
	Customer->Hostility(GetMaster());
	return true;
      }
    else
      return false;
}

void cathedral::Enter(character* Visitor)
{
  if(Visitor->IsPlayer() && !Entered)
    {
      ADD_MESSAGE("The majestetic Cathedral of Valpurus looms before you. You watch it with utter respect.");
      Entered = true;
    }
}

/* Item can also be in a chest, so don't assume anything else... */

bool cathedral::PickupItem(character* Visitor, item* Item, ushort)
{
  if(game::GetStoryState() == 2 || game::GetTeam(ATTNAM_TEAM)->GetRelation(Visitor->GetTeam()) == HOSTILE)
    return true;

  if(Visitor->IsPlayer())
    {
      if(Item->IsHeadOfElpuri() || Item->IsGoldenEagleShirt() || Item->IsPetrussNut() || !Item->GetTruePrice() || Item->IsEncryptedScroll())
	return true;

      ADD_MESSAGE("Picking up property of the Cathedral is prohibited.");

      if(game::BoolQuestion(CONST_S("Do you still want to do this? [y/N]")))
	{
	  Visitor->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
	  return true;
	}
    }

  return false;
}

bool cathedral::DropItem(character* Visitor, item* Item, ushort)
{
  if(game::GetStoryState() == 2 || game::GetTeam(ATTNAM_TEAM)->GetRelation(Visitor->GetTeam()) == HOSTILE)
    return true;

  if(Visitor->IsPlayer())
    {
      if(Item->IsHeadOfElpuri() || Item->IsGoldenEagleShirt() || Item->IsPetrussNut() || Item->IsTheAvatar() || Item->IsEncryptedScroll())
	{
	  ADD_MESSAGE("Donating this to the Cathedral wouldn't be wise. You may still need it.");
	  return false;
	}

      if(game::BoolQuestion(CONST_S("Do you wish to donate this item to the Cathedral? [y/N]")))
	return true;
    }

  return false;
}

void cathedral::KickSquare(character* Kicker, lsquare* Square)
{
  if(!AllowKick(Kicker, Square) && Kicker->IsPlayer() && game::GetStoryState() != 2 && game::GetTeam(ATTNAM_TEAM)->GetRelation(Kicker->GetTeam()) != HOSTILE)
    {
      ADD_MESSAGE("You have harmed the property of the Cathedral!");
      Kicker->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
    }
}

bool cathedral::ConsumeItem(character* HungryMan, item*, ushort)
{
  if(game::GetStoryState() == 2 || game::GetTeam(ATTNAM_TEAM)->GetRelation(HungryMan->GetTeam()) == HOSTILE)
    return true;

  if(HungryMan->IsPlayer())
    {
      ADD_MESSAGE("Eating the property of the Cathedral is forbidden.");

      if(game::BoolQuestion(CONST_S("Do you still want to do this? [y/N]")))
	{
	  HungryMan->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
	  return true;
	}
    }

  return false;
}

void cathedral::Save(outputfile& SaveFile) const
{
  room::Save(SaveFile);
  SaveFile << Entered;
}

void cathedral::Load(inputfile& SaveFile)
{
  room::Load(SaveFile);
  SaveFile >> Entered;
}

bool cathedral::Drink(character* Thirsty) const
{
  if(game::GetStoryState() == 2 || game::GetTeam(ATTNAM_TEAM)->GetRelation(Thirsty->GetTeam()) == HOSTILE)
    return game::BoolQuestion(CONST_S("Do you want to drink? [y/N]"));

  if(Thirsty->IsPlayer())
    {
      ADD_MESSAGE("Drinking property of the Cathedral is prohibited.");

      if(game::BoolQuestion(CONST_S("Do you still want to do this? [y/N]")))
	{
	  Thirsty->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
	  return true;
	}
    }

  return false;
}

void shop::TeleportSquare(character* Infidel, lsquare* Square)
{
  if(Square->GetStack()->GetItems() && MasterIsActive() && Infidel && Infidel != GetMaster() && GetMaster()->GetRelation(Infidel) != HOSTILE && Square->CanBeSeenBy(GetMaster()))
    {
      ADD_MESSAGE("\"You infidel!\"");
      Infidel->Hostility(GetMaster());
    }
}

void cathedral::TeleportSquare(character* Teleporter, lsquare* Square)
{
  if(game::GetStoryState() == 2 || !Teleporter || game::GetTeam(ATTNAM_TEAM)->GetRelation(Teleporter->GetTeam()) == HOSTILE)
    return;

  if(Teleporter->IsPlayer() && Square->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have done unnatural things to the property of the Cathedral!");
      Teleporter->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
    }
}

bool cathedral::Dip(character* Thirsty) const
{
  if(game::GetStoryState() == 2 || game::GetTeam(ATTNAM_TEAM)->GetRelation(Thirsty->GetTeam()) == HOSTILE)
    return true;

  if(Thirsty->IsPlayer())
    {
      /* What if it's not water? */

      ADD_MESSAGE("Stealing the precious water of the Cathedral is prohibited.");

      if(game::BoolQuestion(CONST_S("Are you sure you want to dip? [y/N]")))
	{
	  Thirsty->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
	  return true;
	}
    }

  return false;
}

void cathedral::VirtualConstructor(bool)
{
  SetEntered(false);
}

void library::Enter(character* Customer)
{
  if(Customer->IsPlayer())
    if(MasterIsActive())
      {
	if(GetMaster()->GetRelation(Customer) != HOSTILE && Customer->CanBeSeenBy(GetMaster()))
	  if(GetMaster()->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s looks at you suspiciously. \"Feel free to open the shelves, but be quiet in the library!\" %s whispers.", GetMaster()->CHAR_NAME(DEFINITE), GetMaster()->GetPersonalPronoun().CStr());
	  else
	    ADD_MESSAGE("You feel somebody staring at you.");
      }
    else
      ADD_MESSAGE("The library appears to be deserted.");
}

bool library::PickupItem(character* Customer, item* ForSale, ushort Amount)
{
  if(!MasterIsActive() || Customer == GetMaster() || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  ulong Price = ForSale->GetTruePrice() * Amount * 100 / (100 + Customer->GetAttribute(CHARISMA));

  if(!Customer->IsPlayer())
    {
      if(Customer->CanBeSeenByPlayer() && Customer->GetMoney() >= Price)
	{
	  ADD_MESSAGE("%s buys %s.", Customer->CHAR_NAME(DEFINITE), ForSale->GetName(INDEFINITE, Amount).CStr());
	  Customer->EditMoney(-Price);
	  GetMaster()->EditMoney(Price);
	  Customer->EditExperience(CHARISMA, Price);
	  return true;
	}
      else
	return false;
    }

  if(Customer->CanBeSeenBy(GetMaster()))
    {
      if(ForSale->IsHeadOfElpuri() || ForSale->IsGoldenEagleShirt() || ForSale->IsPetrussNut() || ForSale->IsTheAvatar() || ForSale->IsEncryptedScroll())
	{
	  ADD_MESSAGE("\"I think it is yours. Take it.\"");
	  return true;
	}

      if(!Price || !ForSale->CanBeSoldInLibrary(GetMaster()))
	{
	  ADD_MESSAGE("\"Thank you for cleaning that junk out of my floor.\"");
	  return true;
	}

      if(Customer->GetMoney() >= Price)
	{
	  if(Amount == 1)
	    ADD_MESSAGE("\"Ah! That %s costs %d gold pieces. No haggling, please.\"", ForSale->CHAR_NAME(UNARTICLED), Price);
	  else
	    ADD_MESSAGE("\"Ah! Those %d %s cost %d gold pieces. No haggling, please.\"", Amount, ForSale->CHAR_NAME(PLURAL), Price);

	  if(game::BoolQuestion(CONST_S("Do you accept this deal? [y/N]")))
	    {
	      Customer->EditMoney(-Price);
	      GetMaster()->EditMoney(Price);
	      Customer->EditExperience(CHARISMA, Price);
	      return true;
	    }
	  else
	    return false;
	}
      else
	{
	  if(Amount == 1)
	    ADD_MESSAGE("\"Don't touch that %s, beggar! It is worth %d gold pieces!\"", ForSale->CHAR_NAME(UNARTICLED), Price);
	  else
	    ADD_MESSAGE("\"Don't touch those %s, beggar! They are worth %d gold pieces!\"", ForSale->CHAR_NAME(PLURAL), Price);

	  return false;
	}
    }
  else
    if(game::BoolQuestion(CONST_S("Are you sure you want to commit this thievery? [y/N]")))
      {
	Customer->Hostility(GetMaster());
	return true;
      }
    else
      return false;
}

bool library::DropItem(character* Customer, item* ForSale, ushort Amount)
{
  if(!MasterIsActive() || Customer == GetMaster() || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  ulong Price = ForSale->GetTruePrice() * Amount * (100 + Customer->GetAttribute(CHARISMA)) / 400;

  if(!Customer->IsPlayer())
    if(Price && Customer->CanBeSeenByPlayer() && GetMaster()->GetMoney() >= Price)
      {
	ADD_MESSAGE("%s sells %s.", Customer->CHAR_NAME(DEFINITE), ForSale->GetName(INDEFINITE, Amount).CStr());
	Customer->SetMoney(Customer->GetMoney() + Price);
	GetMaster()->SetMoney(GetMaster()->GetMoney() - Price);
	Customer->EditExperience(CHARISMA, Price);
	return true;
      }
    else
      return false;

  if(Customer->CanBeSeenBy(GetMaster()))
    {
      if(ForSale->IsHeadOfElpuri() || ForSale->IsGoldenEagleShirt() || ForSale->IsPetrussNut() || ForSale->IsTheAvatar() || ForSale->IsEncryptedScroll())
	{
	  ADD_MESSAGE("\"Oh no! You need it far more than I!\"");
	  return false;
	}

      if(!Price || !ForSale->CanBeSoldInLibrary(GetMaster()))
	{
	  ADD_MESSAGE("\"Sorry, but I don't think %s into my collection.\"", Amount == 1 ? "that fits" : "those fit");
	  return false;
	}
      
      if(GetMaster()->GetMoney() != 0)
	{
	  if(GetMaster()->GetMoney() < Price)
	    Price = GetMaster()->GetMoney();

	  if(Amount == 1)
	    ADD_MESSAGE("\"What an interesting %s. I'll pay %d gold pieces for it.\"", ForSale->CHAR_NAME(UNARTICLED), Price);
	  else
	    ADD_MESSAGE("\"What an interesting collection of %d %s. I'll pay %d gold pieces for it.\"", Amount, ForSale->CHAR_NAME(PLURAL), Price);

	  if(game::BoolQuestion(CONST_S("Do you want to sell ") + (Amount == 1 ? "this item" : "these items") + "? [y/N]"))
	    {
	      Customer->EditMoney(Price);
	      GetMaster()->EditMoney(-Price);
	      Customer->EditExperience(CHARISMA, Price);
	      return true;
	    }
	  else
	    return false;
	}
      else
	{
	  ADD_MESSAGE("\"I would pay you %d gold pieces for %s, but I'm temporary short of cash. Sorry.\"", Price, Amount == 1 ? "it" : "them");
	  return false;
	}
    }
  else
    {
      ADD_MESSAGE("The librarian doesn't see you, so you cannot trade with him.");
      return game::BoolQuestion(CONST_S("Still drop ") +  + (Amount == 1 ? "this item" : "these items") + "? [y/N]");
    }
}

void library::KickSquare(character* Infidel, lsquare* Square)
{
  if(!AllowKick(Infidel, Square))
    {
      ADD_MESSAGE("\"You book vandal!\"");
      Infidel->Hostility(GetMaster());
    }
}

bool library::ConsumeItem(character*, item*, ushort)
{
  return true;
}

void library::TeleportSquare(character* Infidel, lsquare* Square)
{
  if(Square->GetStack()->GetItems() && MasterIsActive() && Infidel && Infidel != GetMaster() && GetMaster()->GetRelation(Infidel) != HOSTILE && Square->CanBeSeenBy(GetMaster()))
    {
      ADD_MESSAGE("\"You book hater!\"");
      Infidel->Hostility(GetMaster());
    }
}

bool landingsite::PickupItem(character* Hungry, item* Item, ushort)
{
  if(game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Hungry->GetTeam()) == HOSTILE)
    return true;

  if(Hungry->IsPlayer())
    {
      if(!Item->IsBanana())
	return true;

      ADD_MESSAGE("That would be stealing.");

      if(game::BoolQuestion(CONST_S("Do you still want to do this? [y/N]")))
	{
	  Hungry->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
	  return true;
	}
    }

  return false;
}

bool landingsite::DropItem(character* Dropper, item* Item, ushort)
{
  return game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Dropper->GetTeam()) == HOSTILE || (Dropper->IsPlayer() && (!Item->IsBanana() || game::BoolQuestion(CONST_S("Do you wish to donate this item to the town? [y/N]"))));
}

void landingsite::KickSquare(character* Kicker, lsquare* Square)
{
  if(AllowKick(Kicker, Square) && Kicker->IsPlayer() && game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Kicker->GetTeam()) != HOSTILE)
    {
      for(stackiterator i = Square->GetStack()->GetBottom(); i.HasItem(); ++i)
	if(i->IsBanana())
	  {
	    ADD_MESSAGE("You have harmed the property of the town!");
	    Kicker->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
	    return;
	  }
    }
}

bool landingsite::ConsumeItem(character* HungryMan, item* Item, ushort)
{
  if(game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(HungryMan->GetTeam()) == HOSTILE)
    return true;

  if(HungryMan->IsPlayer())
    {
      if(Item->IsBanana())
	ADD_MESSAGE("Eating this is forbidden.");

      if(game::BoolQuestion(CONST_S("Do you still want to do this? [y/N]")))
	{
	  HungryMan->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
	  return true;
	}
    }

  return false;
}

void landingsite::TeleportSquare(character* Infidel, lsquare* Square)
{
  if(!Infidel || game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Infidel->GetTeam()) == HOSTILE)
    return;

  for(stackiterator i = Square->GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsBanana())
      {
	Infidel->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
	return;
      }
}

bool shop::AllowSpoil(const item* Item) const
{
  character* Master = game::SearchCharacter(MasterID);
  return !Master || !Master->IsEnabled() || !Item->GetTruePrice();
}

bool shop::AllowKick(const character* Char, const lsquare* LSquare) const // gum solution
{
  return !LSquare->GetStack()->GetItems() || !MasterIsActive() || Char == GetMaster() || GetMaster()->GetRelation(Char) == HOSTILE || !LSquare->CanBeSeenBy(GetMaster());
}

bool cathedral::AllowKick(const character* Char, const lsquare* LSquare) const
{
  return game::GetTeam(ATTNAM_TEAM)->GetRelation(Char->GetTeam()) == HOSTILE || !LSquare->GetStack()->GetItems();
}

bool library::AllowKick(const character* Char, const lsquare* LSquare) const
{
  return !LSquare->GetStack()->GetItems() || MasterIsActive() || Char == GetMaster() || GetMaster()->GetRelation(Char) == HOSTILE || LSquare->CanBeSeenBy(GetMaster());
}

bool landingsite::AllowKick(const character* Char, const lsquare*) const
{
 return !Char->IsPlayer() || (game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Char->GetTeam()) == HOSTILE);
}

void shop::HostileAction(character* Guilty) const
{
  if(MasterIsActive() && Guilty && Guilty != GetMaster() && GetMaster()->GetRelation(Guilty) != HOSTILE && Guilty->CanBeSeenBy(GetMaster()))
    {
      ADD_MESSAGE("\"You infidel!\"");
      Guilty->Hostility(GetMaster());
    }
}

void cathedral::HostileAction(character* Guilty) const
{
  if(game::GetStoryState() != 2 && Guilty)
    Guilty->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
}

void library::HostileAction(character* Guilty) const
{
  if(MasterIsActive() && Guilty && Guilty != GetMaster() && GetMaster()->GetRelation(Guilty) != HOSTILE && Guilty->CanBeSeenBy(GetMaster()))
    {
      ADD_MESSAGE("\"You infidel!\"");
      Guilty->Hostility(GetMaster());
    }
}
