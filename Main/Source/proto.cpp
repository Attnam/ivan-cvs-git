/* Compiled through dataset.cpp */

character* protosystem::BalancedCreateMonster()
{
  std::vector<std::pair<ushort, ushort> > Illegal;

  for(ushort c = 0;; ++c)
    {
      float Difficulty = game::Difficulty();

      for(ushort i = 0; i < 25; ++i)
	{
	  ushort ChosenType;
	  ushort ChosenConfig;
	  std::pair<ushort, ushort> Chosen;
	  const character::prototype* Proto;
	  const character::databasemap* Config;

	  if(c < 100 || Illegal.size() < 3)
	    {
	      Chosen.first = ChosenType = 1 + RAND() % (protocontainer<character>::GetProtoAmount() - 1);
	      Proto = protocontainer<character>::GetProto(ChosenType);
	      Config = &Proto->GetConfig();
	      Chosen.second = ChosenConfig = RAND() % Config->size();

	      if(c < 100 && std::find(Illegal.begin(), Illegal.end(), Chosen) != Illegal.end())
		continue;
	    }
	  else
	    {
	      Chosen = Illegal[RAND() % Illegal.size()];
	      ChosenType = Chosen.first;
	      ChosenConfig = Chosen.second;
	      Proto = protocontainer<character>::GetProto(ChosenType);
	      Config = &Proto->GetConfig();
	    }

	  for(character::databasemap::const_iterator i = Config->begin(); i != Config->end(); ++i)
	    if(!ChosenConfig--)
	      {
		if(!i->second.IsAbstract && i->second.CanBeGenerated && (i->second.Frequency == 10000 || i->second.Frequency > RAND() % 10000))
		  {
		    const dangerid& DangerId = game::GetDangerMap().find(configid(ChosenType, i->first))->second;

		    if(i->second.IsUnique && DangerId.HasBeenGenerated)
		      break;

		    if(c < 100 && !i->second.IgnoreDanger)
		      {
			float DangerModifier = DangerId.Danger * 100 / i->second.DangerModifier;

			if(DangerModifier > Difficulty * 5 || DangerModifier < Difficulty / 5)
			  break;
		      }

		    if(c < 100 && PLAYER->GetMaxHP() < i->second.HPRequirementForGeneration)
		      break;

		    if(c < 100 && !i->second.IsUnique && RAND() % 3)
		      {
			Illegal.push_back(Chosen);
			break;
		      }

		    character* Monster = Proto->Clone(i->first);

		     if(c >= 100 || (Monster->GetTimeToKill(PLAYER, true) > 10000 && (i->second.IsUnique || PLAYER->GetTimeToKill(Monster, true) < 100000)))
		      {
			game::SignalGeneration(ChosenType, i->first);
			Monster->SetTeam(game::GetTeam(MONSTER_TEAM));
			return Monster;
		      }
		    else
		      delete Monster;
		  }

		break;
	      }
	}
    }

  /* This line is never reached, but it prevents warnings given by some (stupid) compilers. */

  return 0;
}

item* protosystem::BalancedCreateItem(ulong MinPrice, ulong MaxPrice, ulong Category, bool Polymorph)
{
  ulong SumOfPossibilities = 0;

  for(ushort c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
    {
      const item::databasemap& Config = protocontainer<item>::GetProto(c)->GetConfig();

      for(item::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract && (!Category || Category & i->second.Category))
	  SumOfPossibilities += i->second.Possibility;
    }

  while(true)
    {
      for(ushort c1 = 0; c1 < 100; ++c1)
	{
	  ulong Counter = 0, RandomOne = 1 + RAND() % (SumOfPossibilities);

	  for(ushort c2 = 1; c2 < protocontainer<item>::GetProtoAmount(); ++c2)
	    {
	      const item::prototype* Proto = protocontainer<item>::GetProto(c2);
	      const item::databasemap& Config = Proto->GetConfig();

	      for(item::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
		if(!i->second.IsAbstract && (!Category || Category & i->second.Category))
		  {
		    Counter += i->second.Possibility;

		    if(Counter >= RandomOne)
		      {
			if(!Polymorph || i->second.IsPolymorphSpawnable)
			  {
			    item* Item = Proto->Clone(i->first);

			    if(MinPrice == 0 && MaxPrice == MAX_PRICE) // optimization, GetPrice() may be rather slow
			      return Item;

			    ulong Price = Item->GetPrice();

			    if(Item->HandleInPairs())
			      Price <<= 1;

			    if(Price >= MinPrice && Price <= MaxPrice)
			      return Item;
			    else
			      delete Item;
			  }

			break;
		      }
		  }

	      if(Counter >= RandomOne)
		break;
	    }
	}

      MinPrice = MinPrice * 3 >> 2;
      MaxPrice = MaxPrice * 5 >> 2;
    }
}

character* protosystem::CreateMonster(ushort MinDanger, ushort MaxDanger, ushort SpecialFlags)
{
  while(true)
    {
      for(ushort c = 0; c < 100; ++c)
	{
	  ushort Chosen = 1 + RAND() % (protocontainer<character>::GetProtoAmount() - 1);
	  const character::prototype* Proto = protocontainer<character>::GetProto(Chosen);
	  const character::databasemap& Config = Proto->GetConfig();
	  Chosen = RAND() % Config.size();

	  for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	    if(!Chosen--)
	      {
		if(!i->second.IsAbstract
		&& i->second.CanBeGenerated
		&& i->second.CanBeWished
		&& (i->second.Frequency == 10000 || i->second.Frequency > RAND() % 10000)
		&& (!i->second.IsUnique || !game::GetDangerMap().find(configid(Chosen, i->first))->second.HasBeenGenerated))
		  {
		    character* Monster = Proto->Clone(i->first, SpecialFlags);

		    if(MinDanger > 0 || MaxDanger < 10000)
		      {
			ushort Danger = ushort(Monster->GetRelativeDanger(PLAYER) * 100.0f);

			if(Danger < MinDanger || Danger > MaxDanger)
			  {
			    delete Monster;
			    break;
			  }
		      }

		    game::SignalGeneration(Chosen, i->first);
		    Monster->SetTeam(game::GetTeam(MONSTER_TEAM));
		    return Monster;
		  }

		break;
	      }
	}

      MinDanger = MinDanger * 3 >> 2;
      MaxDanger = MaxDanger * 5 >> 2;
    }
}

template <class type> std::pair<ushort, ushort> CountCorrectNameLetters(const typename type::database& DataBase, const std::string& Identifier)
{
  std::pair<ushort, ushort> Result(0, 0);

  if(!DataBase.NameSingular.empty())
    ++Result.second;

  if(festring::IgnoreCaseFind(Identifier, " " + DataBase.NameSingular + " ") != std::string::npos)
    Result.first += DataBase.NameSingular.length();

  if(!DataBase.Adjective.empty())
    ++Result.second;

  if(DataBase.Adjective.length() && festring::IgnoreCaseFind(Identifier, " " + DataBase.Adjective + " ") != std::string::npos)
    Result.first += DataBase.Adjective.length();

  if(!DataBase.PostFix.empty())
    ++Result.second;

  if(DataBase.PostFix.length() && festring::IgnoreCaseFind(Identifier, " " + DataBase.PostFix + " ") != std::string::npos)
    Result.first += DataBase.PostFix.length();

  for(ushort c = 0; c < DataBase.Alias.size(); ++c)
    if(festring::IgnoreCaseFind(Identifier, " " + DataBase.Alias[c] + " ") != std::string::npos)
      Result.first += DataBase.Alias[c].length();

  return Result;
}

template <class type> std::pair<const typename type::prototype*, ushort> SearchForProto(const std::string& What, bool Output)
{
  typedef typename type::prototype prototype;
  typedef typename type::databasemap databasemap;

  std::string Identifier;
  Identifier << " " << What << " ";
  bool Illegal = false, Conflict = false;
  std::pair<const prototype*, ushort> Id(0, 0);
  std::pair<ushort, ushort> Best(0, 0);

  for(ushort c = 1; c < protocontainer<type>::GetProtoAmount(); ++c)
    {
      const prototype* Proto = protocontainer<type>::GetProto(c);
      const databasemap& Config = Proto->GetConfig();

      for(typename databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract)
	  {
	    std::pair<ushort, ushort> Correct = CountCorrectNameLetters<type>(i->second, Identifier);

	    if(Correct == Best)
	      Conflict = true;
	    else if(Correct.first > Best.first || (Correct.first == Best.first && Correct.second < Best.second))
	      if(i->second.CanBeWished || game::WizardModeIsActive())
		{
		  Id.first = Proto;
		  Id.second = i->first;
		  Best = Correct;
		  Conflict = false;
		}
	      else
		Illegal = true;
	  }
    }

  if(Output)
    {
      if(!Best.first)
	{
	  if(Illegal)
	    ADD_MESSAGE("You hear a booming voice: \"No, mortal! This will not be done!\"");
	  else
	    ADD_MESSAGE("What a strange wish!");
	}
      else if(Conflict)
	{
	  ADD_MESSAGE("Be more precise!");
	  return std::pair<const prototype*, ushort>(0, 0);
	}
    }

  return Id;
}

character* protosystem::CreateMonster(const std::string& What, ushort SpecialFlags, bool Output)
{
  std::pair<const character::prototype*, ushort> Id = SearchForProto<character>(What, Output);

  if(Id.first)
    return Id.first->Clone(Id.second, SpecialFlags);
  else
    return 0;
}

item* protosystem::CreateItem(const std::string& What, bool Output)
{
  std::pair<const item::prototype*, ushort> Id = SearchForProto<item>(What, Output);

  if(Id.first)
    return Id.first->Clone(Id.second);
  else
    return 0;
}

material* protosystem::CreateMaterial(const std::string& What, ulong Volume, bool Output)
{
  for(ushort c = 1; c < protocontainer<material>::GetProtoAmount(); ++c)
    {
      const material::prototype* Proto = protocontainer<material>::GetProto(c);
      const material::databasemap& Config = Proto->GetConfig();

      for(material::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(i->first && i->second.NameStem == What)
	  if(i->second.CanBeWished || game::WizardModeIsActive())
	    return Proto->Clone(i->first, Volume);
	  else if(Output)
	    {
	      ADD_MESSAGE("You hear a booming voice: \"No, mortal! This will not be done!\"");
	      return 0;
	    }
    }
		
  if(Output)
    ADD_MESSAGE("There is no such material.");

  return 0;
}

void protosystem::CreateEveryCharacter(std::vector<character*>& Character)
{
  for(ushort c = 1; c < protocontainer<character>::GetProtoAmount(); ++c)
    {
      const character::prototype* Proto = protocontainer<character>::GetProto(c);
      const character::databasemap& Config = Proto->GetConfig();

      for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract)
	  Character.push_back(Proto->Clone(i->first));
    }
}

void protosystem::CreateEveryItem(std::vector<item*>& Item)
{
  for(ushort c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
    {
      const item::prototype* Proto = protocontainer<item>::GetProto(c);
      const item::databasemap& Config = Proto->GetConfig();

      for(item::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract && i->second.IsAutoInitializable)
	  Item.push_back(Proto->Clone(i->first));
    }
}
