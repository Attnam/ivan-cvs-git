/* Compiled through dataset.cpp */

character* protosystem::BalancedCreateMonster()
{
  for(ushort c = 0;; ++c)
    {
      float MinDifficulty = game::GetMinDifficulty(), MaxDifficulty = MinDifficulty * 25;
      std::vector<configid> Possible;

      for(ushort Type = 1; Type < protocontainer<character>::GetProtoAmount(); ++Type)
	{
	  const character::prototype* Proto = protocontainer<character>::GetProto(Type);
	  const character::databasemap& Config = Proto->GetConfig();

	  for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	    if(!i->second.IsAbstract && i->second.CanBeGenerated)
	      {
		configid ConfigID(Type, i->first);
		const dangerid& DangerId = game::GetDangerMap().find(ConfigID)->second;

		if(i->second.IsUnique && DangerId.HasBeenGenerated)
		  continue;

		if(c >= 100)
		  {
		    Possible.push_back(ConfigID);
		    continue;
		  }

		if(!i->second.IgnoreDanger)
		  {
		    float DangerModifier = i->second.DangerModifier == 100 ? DangerId.EquippedDanger : DangerId.EquippedDanger * 100 / i->second.DangerModifier;

		    if(DangerModifier < MinDifficulty || DangerModifier > MaxDifficulty)
		      continue;
		  }

		if(PLAYER->GetMaxHP() < i->second.HPRequirementForGeneration)
		  continue;

		Possible.push_back(ConfigID);
	      }
	}

      if(Possible.empty())
	continue;

      for(ushort i = 0; i < 25; ++i)
	{
	  configid Chosen = Possible[RAND() % Possible.size()];
	  const character::prototype* Proto = protocontainer<character>::GetProto(Chosen.Type);
	  character* Monster = Proto->Clone(Chosen.Config);

	   if(c >= 100 || ((Monster->GetFrequency() == 10000 || Monster->GetFrequency() > RAND() % 10000) && Monster->GetTimeToKill(PLAYER, true) > 5000 && (Monster->IsUnique() || PLAYER->GetTimeToKill(Monster, true) < 75000)))
	    {
	      game::SignalGeneration(Chosen);
	      Monster->SetTeam(game::GetTeam(MONSTER_TEAM));
	      return Monster;
	    }
	  else
	    delete Monster;
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

			    ulong Price = Item->GetTruePrice();

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
  std::vector<configid> Possible;
  character* Monster = 0;

  for(ushort c = 0; !Monster; ++c)
    {
      for(ushort Type = 1; Type < protocontainer<character>::GetProtoAmount(); ++Type)
	{
	  const character::prototype* Proto = protocontainer<character>::GetProto(Type);
	  const character::databasemap& Config = Proto->GetConfig();

	  for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	    if(!i->second.IsAbstract && i->second.CanBeGenerated && i->second.CanBeWished && !i->second.IsUnique && (i->second.Frequency == 10000 || i->second.Frequency > RAND() % 10000))
	      {
		configid ConfigID(Type, i->first);

		if((MinDanger > 0 || MaxDanger < 10000) && c < 25)
		  {
		    const dangerid& DangerId = game::GetDangerMap().find(ConfigID)->second;
		    float RawDanger = SpecialFlags & NO_EQUIPMENT ? DangerId.NakedDanger : DangerId.EquippedDanger;
		    ushort Danger = ushort(i->second.DangerModifier == 100 ? RawDanger * 100 : RawDanger * 10000 / i->second.DangerModifier);

		    if(Danger < MinDanger || Danger > MaxDanger)
		      continue;
		  }

		Possible.push_back(ConfigID);
	      }
	}

      if(Possible.empty())
	{
	  MinDanger = MinDanger > 0 ? Max<ushort>(MinDanger * 3 >> 2, 1) : 0;
	  MaxDanger = MaxDanger < 10000 ? Min<ushort>(MaxDanger * 5 >> 2, 9999) : 10000;
	  continue;
	}

      configid Chosen = Possible[RAND() % Possible.size()];
      Monster = protocontainer<character>::GetProto(Chosen.Type)->Clone(Chosen.Config, SpecialFlags);
      game::SignalGeneration(Chosen);
      Monster->SetTeam(game::GetTeam(MONSTER_TEAM));
    }

  return Monster;
}

template <class type> std::pair<ushort, ushort> CountCorrectNameLetters(const typename type::database& DataBase, const festring& Identifier)
{
  std::pair<ushort, ushort> Result(0, 0);

  if(!DataBase.NameSingular.IsEmpty())
    ++Result.second;

  if(festring::IgnoreCaseFind(Identifier, " " + DataBase.NameSingular + ' ') != festring::NPos)
    Result.first += DataBase.NameSingular.GetSize();

  if(!DataBase.Adjective.IsEmpty())
    ++Result.second;

  if(DataBase.Adjective.GetSize() && festring::IgnoreCaseFind(Identifier, " " + DataBase.Adjective + ' ') != festring::NPos)
    Result.first += DataBase.Adjective.GetSize();

  if(!DataBase.PostFix.IsEmpty())
    ++Result.second;

  if(DataBase.PostFix.GetSize() && festring::IgnoreCaseFind(Identifier, " " + DataBase.PostFix + ' ') != festring::NPos)
    Result.first += DataBase.PostFix.GetSize();

  for(ushort c = 0; c < DataBase.Alias.size(); ++c)
    if(festring::IgnoreCaseFind(Identifier, " " + DataBase.Alias[c] + ' ') != festring::NPos)
      Result.first += DataBase.Alias[c].GetSize();

  return Result;
}

template <class type> std::pair<const typename type::prototype*, ushort> SearchForProto(const festring& What, bool Output)
{
  typedef typename type::prototype prototype;
  typedef typename type::databasemap databasemap;

  festring Identifier;
  Identifier << ' ' << What << ' ';
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
	    bool BrokenRequested = festring::IgnoreCaseFind(Identifier, " broken ") != festring::NPos;

	    if(BrokenRequested != ((i->first & BROKEN) != 0))
	      continue;

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

character* protosystem::CreateMonster(const festring& What, ushort SpecialFlags, bool Output)
{
  std::pair<const character::prototype*, ushort> Id = SearchForProto<character>(What, Output);

  if(Id.first)
    return Id.first->Clone(Id.second, SpecialFlags);
  else
    return 0;
}

item* protosystem::CreateItem(const festring& What, bool Output)
{
  std::pair<const item::prototype*, ushort> Id = SearchForProto<item>(What, Output);

  if(Id.first)
    return Id.first->Clone(Id.second);
  else
    return 0;
}

material* protosystem::CreateMaterial(const festring& What, ulong Volume, bool Output)
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

#ifdef WIZARD

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

void protosystem::CreateEveryItem(itemvector& Item)
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

void protosystem::CreateEveryMaterial(std::vector<material*>& Material)
{
  for(ushort c = 1; c < protocontainer<material>::GetProtoAmount(); ++c)
    {
      const material::prototype* Proto = protocontainer<material>::GetProto(c);
      const material::databasemap& Config = Proto->GetConfig();

      for(material::databasemap::const_iterator i = ++Config.begin(); i != Config.end(); ++i)
	Material.push_back(Proto->Clone(i->first));
    }
}

#endif
