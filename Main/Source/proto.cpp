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

template <class type> ushort protocontainer<type>::Add(prototype* Proto)
{
  static bool Initialized = false;

  if(!Initialized)
    {
      ProtoData.resize(1, 0);
      Initialized = true;
    }

  ProtoData.push_back(Proto);//insert(ProtoData.end() - 1, Proto);
  //CodeNameMap[Proto->ClassName()] = ProtoData.size() - 2;
  return ProtoData.size() - 1;
}

template ushort protocontainer<action>::Add(action::prototype*);
template ushort protocontainer<character>::Add(character::prototype*);
template ushort protocontainer<god>::Add(god::prototype*);
template ushort protocontainer<item>::Add(item::prototype*);
template ushort protocontainer<room>::Add(room::prototype*);
template ushort protocontainer<olterrain>::Add(olterrain::prototype*);
template ushort protocontainer<glterrain>::Add(glterrain::prototype*);
template ushort protocontainer<material>::Add(material::prototype*);
template ushort protocontainer<owterrain>::Add(owterrain::prototype*);
template ushort protocontainer<gwterrain>::Add(gwterrain::prototype*);

template <class type> ushort protocontainer<type>::SearchCodeName(const std::string& Name)
{
  valuemap::iterator I = CodeNameMap.find(Name);

  if(I != CodeNameMap.end())
    return I->second;
  else
    return 0;
}

template ushort protocontainer<action>::SearchCodeName(const std::string&);
template ushort protocontainer<character>::SearchCodeName(const std::string&);
template ushort protocontainer<god>::SearchCodeName(const std::string&);
template ushort protocontainer<item>::SearchCodeName(const std::string&);
template ushort protocontainer<room>::SearchCodeName(const std::string&);
template ushort protocontainer<olterrain>::SearchCodeName(const std::string&);
template ushort protocontainer<glterrain>::SearchCodeName(const std::string&);
template ushort protocontainer<material>::SearchCodeName(const std::string&);
template ushort protocontainer<owterrain>::SearchCodeName(const std::string&);
template ushort protocontainer<gwterrain>::SearchCodeName(const std::string&);

template <class type> void protocontainer<type>::GenerateCodeNameMap()
{
  for(ushort c = 1; c < GetProtoAmount(); ++c)
    CodeNameMap[GetProto(c)->ClassName()] = c;
}

template <class type> outputfile& operator<<(outputfile& SaveFile, type* Class)
{
  if(Class)
    Class->Save(SaveFile);
  else
    SaveFile << ushort(0);

  return SaveFile;
}

template outputfile& operator<<(outputfile&, action*);
template outputfile& operator<<(outputfile&, character*);
template outputfile& operator<<(outputfile&, god*);
template outputfile& operator<<(outputfile&, item*);
template outputfile& operator<<(outputfile&, room*);
template outputfile& operator<<(outputfile&, olterrain*);
template outputfile& operator<<(outputfile&, glterrain*);
template outputfile& operator<<(outputfile&, material*);
template outputfile& operator<<(outputfile&, owterrain*);
template outputfile& operator<<(outputfile&, gwterrain*);

template <class type> inputfile& operator>>(inputfile& SaveFile, type*& Class)
{
  ushort Type;
  SaveFile >> Type;

  if(Type)
    Class = protocontainer<type>::GetProto(Type)->CloneAndLoad(SaveFile);
  else
    Class = 0;

  return SaveFile;
}

template inputfile& operator>>(inputfile&, action*&);
template inputfile& operator>>(inputfile&, character*&);
template inputfile& operator>>(inputfile&, god*&);
template inputfile& operator>>(inputfile&, item*&);
template inputfile& operator>>(inputfile&, room*&);
template inputfile& operator>>(inputfile&, olterrain*&);
template inputfile& operator>>(inputfile&, glterrain*&);
template inputfile& operator>>(inputfile&, material*&);
template inputfile& operator>>(inputfile&, owterrain*&);
template inputfile& operator>>(inputfile&, gwterrain*&);

character* protosystem::BalancedCreateMonster(float Multiplier, bool CreateItems)
{
  for(ushort c = 0;; ++c)
    {
      float Difficulty = game::Difficulty();

      for(ushort i = 0; i < 10; ++i)
	{
	  ushort Chosen = 1 + RAND() % (protocontainer<character>::GetProtoAmount() - 1);
	  const character::prototype* const Proto = protocontainer<character>::GetProto(Chosen);

	  if(Proto->CanBeGenerated() && Proto->GetFrequency() > RAND() % 10000)
	    {
	      character* Monster = Proto->Clone(true, CreateItems);
	      float Danger = Monster->MaxDanger();

	      if(c >= 99 || (Danger < Difficulty * Multiplier * 2 && Danger > Difficulty * Multiplier / 2))
		{
		  Monster->SetTeam(game::GetTeam(1));
		  return Monster;
		}
	      else
		delete Monster;
	    }
	}
    }
}

item* protosystem::BalancedCreateItem(bool Polymorph)
{
  while(true)
    {
      ushort SumOfPossibilities = 0, Counter = 0, RandomOne;

      ushort c;

      for(c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
	SumOfPossibilities += protocontainer<item>::GetProto(c)->GetPossibility();
			
      RandomOne = 1 + RAND() % (SumOfPossibilities);
		
      for(c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
	{
	  Counter += protocontainer<item>::GetProto(c)->GetPossibility();

	  if(Counter >= RandomOne)
	    if(!Polymorph || protocontainer<item>::GetProto(c)->IsPolymorphSpawnable())
	      return protocontainer<item>::GetProto(c)->Clone();
	    else
	      break;
	}
    }
}

character* protosystem::CreateMonster(bool CreateItems)
{
  while(true)
    {
      ushort Chosen = 1 + RAND() % (protocontainer<character>::GetProtoAmount() - 1);

      if(protocontainer<character>::GetProto(Chosen)->CanBeGenerated())
	{
	  character* Monster = protocontainer<character>::GetProto(Chosen)->Clone(true, CreateItems);
	  Monster->SetTeam(game::GetTeam(1));
	  return Monster;
	}
    }
}

item* protosystem::CreateItem(const std::string& What, bool Output)
{
  for(ushort c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
    {
      /* "Temporary" gum solution! */

      item* Temp = protocontainer<item>::GetProto(c)->Clone(false);

      if(Temp->NameSingular() == What)
	if(protocontainer<item>::GetProto(c)->CanBeWished() || game::GetWizardMode())
	  //return protocontainer<item>::GetProto(c)->CreateWishedItem();
	  return protocontainer<item>::GetProto(c)->Clone();
	else if(Output)
	  {
	    ADD_MESSAGE("You hear a booming voice: \"No, mortal! This will not be done!\"");
	    return 0;
	  }
    }

  if(Output)
    ADD_MESSAGE("There is no such item.");

  return 0;
}

material* protosystem::CreateMaterial(const std::string& What, ulong Volume, bool Output)
{
  for(ushort c = 1; c < protocontainer<material>::GetProtoAmount(); ++c)
    {
      /* "Temporary" gum solution! */

      material* Temp = protocontainer<material>::GetProto(c)->Clone();

      if(Temp->Name(UNARTICLED) == What)
	if(protocontainer<material>::GetProto(c)->CanBeWished())
	  return protocontainer<material>::GetProto(c)->Clone(Volume);
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

material* protosystem::CreateRandomSolidMaterial(ulong Volume)
{
  for(ushort c = 1 + RAND() % (protocontainer<material>::GetProtoAmount() - 1);; c = 1 + RAND() % (protocontainer<material>::GetProtoAmount() - 1))
    if(protocontainer<material>::GetProto(c)->IsSolid())
      return protocontainer<material>::GetProto(c)->Clone(Volume);
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
