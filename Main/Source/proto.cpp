#include "proto.h"
#include "charba.h"
#include "itemba.h"
#include "materba.h"
#include "femath.h"
#include "message.h"
#include "actionba.h"
#include "godba.h"
#include "roomba.h"
#include "wterraba.h"
#include "game.h"
#include "lterraba.h"
#include "save.h"
#include "stdover.h"

template <class type> void protocontainer<type>::GenerateCodeNameMap()
{
  for(ushort c = 1; c < GetProtoAmount(); ++c)
    CodeNameMap[GetProto(c)->GetClassId()] = c;
}

character* protosystem::BalancedCreateMonster(float Multiplier, bool CreateItems)
{
  for(ushort c = 0;; ++c)
    {
      float Difficulty = game::Difficulty() * Multiplier;

      for(ushort i = 0; i < 10; ++i)
	{
	  ushort ChosenType = 1 + RAND() % (protocontainer<character>::GetProtoAmount() - 1);
	  const character::prototype* Proto = protocontainer<character>::GetProto(ChosenType);
	  const character::databasemap& Config = Proto->GetConfig();

	  ushort ChosenConfig = RAND() % Config.size();
	  character* Monster = 0;

	  for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	    if(!ChosenConfig--)
	      {
		if(!i->second.IsAbstract && i->second.CanBeGenerated && (i->second.Frequency == 10000 || i->second.Frequency > RAND() % 10000))
		  {
		    float DangerModifier = game::GetDangerMap().find(configid(ChosenType, i->first))->second;

		    if(c >= 99 || (DangerModifier < Difficulty * 3 && DangerModifier > Difficulty / 3))
		      {
			character* Monster = Proto->Clone(i->first, CreateItems);
			Monster->SetTeam(game::GetTeam(1));
			return Monster;
		      }
		  }

		break;
	      }
	}
    }
}

item* protosystem::BalancedCreateItem(bool Polymorph)
{
  ulong c, SumOfPossibilities = 0;

  for(c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
    {
      const item::databasemap& Config = protocontainer<item>::GetProto(c)->GetConfig();

      for(item::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract)
	  SumOfPossibilities += i->second.Possibility;
    }

  while(true)
    {
      ulong Counter = 0, RandomOne = 1 + RAND() % (SumOfPossibilities);

      for(c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
	{
	  const item::prototype* Proto = protocontainer<item>::GetProto(c);
	  const item::databasemap& Config = Proto->GetConfig();

	  for(item::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	    if(!i->second.IsAbstract)
	      {
		Counter += i->second.Possibility;

		if(Counter >= RandomOne)
		  if(!Polymorph || i->second.IsPolymorphSpawnable)
		    return Proto->Clone(i->first);
		  else
		    break;
	      }
	}
    }
}

character* protosystem::CreateMonster(bool CreateItems)
{
  while(true)
    {
      ushort Chosen = 1 + RAND() % (protocontainer<character>::GetProtoAmount() - 1);
      const character::prototype* Proto = protocontainer<character>::GetProto(Chosen);
      const character::databasemap& Config = Proto->GetConfig();

      Chosen = RAND() % Config.size();

      for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!Chosen--)
	  {
	    if(!i->second.IsAbstract && i->second.CanBeGenerated && (i->second.Frequency == 10000 || i->second.Frequency > RAND() % 10000))
	      {
		character* Monster = Proto->Clone(i->first, CreateItems);
		Monster->SetTeam(game::GetTeam(1));
		return Monster;
	      }

	    break;
	  }
    }
}

template <class type> ushort CountCorrectNameLetters(const typename type::database& DataBase, const std::string& Identifier)
{
  ushort Counter = 0;

  if(Identifier.find(" " + DataBase.NameSingular + " ") != ulong(-1))
    Counter += DataBase.NameSingular.length();

  if(DataBase.Adjective.length() && Identifier.find(" " + DataBase.Adjective + " ") != ulong(-1))
    Counter += DataBase.Adjective.length();

  if(DataBase.PostFix.length() && Identifier.find(" " + DataBase.PostFix + " ") != ulong(-1))
    Counter += DataBase.PostFix.length();

  for(ushort c = 0; c < DataBase.Alias.size(); ++c)
    if(Identifier.find(" " + DataBase.Alias[c] + " ") != ulong(-1))
      Counter += DataBase.Alias[c].length();

  return Counter;
}

template <class type> std::pair<const typename type::prototype*, ushort> SearchForProto(const std::string& What, bool Output)
{
  typedef typename type::prototype prototype;
  typedef typename type::databasemap databasemap;

  std::string Identifier;
  Identifier << " " << What << " ";
  bool Illegal = false;
  std::pair<const prototype*, ushort> Id(0, 0);
  ushort Best = 0;

  for(ushort c = 1; c < protocontainer<type>::GetProtoAmount(); ++c)
    {
      const prototype* Proto = protocontainer<type>::GetProto(c);
      const databasemap& Config = Proto->GetConfig();

      /*ushort Correct = CountCorrectNameLetters<type>(*Proto->GetDataBase(), Identifier);

      if(!Proto->IsAbstract() && Correct > Best)
	if(Proto->CanBeWished() || game::WizardModeActivated())
	  {
	    Id.first = Proto;
	    Id.second = 0;
	    Best = Correct;
	  }
	else
	  Illegal = true;*/

      for(typename databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract)
	  {
	    ushort Correct = CountCorrectNameLetters<type>(i->second, Identifier);

	    if(Correct > Best)
	      if(i->second.CanBeWished || game::WizardModeActivated())
		{
		  Id.first = Proto;
		  Id.second = i->first;
		  Best = Correct;
		}
	      else
		Illegal = true;
	  }
    }

  if(!Best && Output)
    {
      if(Illegal)
	ADD_MESSAGE("You hear a booming voice: \"No, mortal! This will not be done!\"");
      else
	ADD_MESSAGE("What a strange wish!");
    }

  return Id;
}

character* protosystem::CreateMonster(const std::string& What, bool CreateEquipment, bool Output)
{
  std::pair<const character::prototype*, ushort> Id = SearchForProto<character>(What, Output);

  if(Id.first)
    return Id.first->Clone(Id.second, CreateEquipment);
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
	  if(i->second.CanBeWished || game::WizardModeActivated())
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

void protosystem::GenerateCodeNameMaps()
{
  protocontainer<action>::GenerateCodeNameMap();
  protocontainer<character>::GenerateCodeNameMap();
  protocontainer<god>::GenerateCodeNameMap();
  protocontainer<item>::GenerateCodeNameMap();
  protocontainer<room>::GenerateCodeNameMap();
  protocontainer<olterrain>::GenerateCodeNameMap();
  protocontainer<glterrain>::GenerateCodeNameMap();
  protocontainer<material>::GenerateCodeNameMap();
  protocontainer<owterrain>::GenerateCodeNameMap();
  protocontainer<gwterrain>::GenerateCodeNameMap();
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
