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

template <class type> void protocontainer<type>::GenerateCodeNameMap()
{
  for(ushort c = 1; c < GetProtoAmount(); ++c)
    CodeNameMap[GetProto(c)->ClassName()] = c;
}

character* protosystem::BalancedCreateMonster(float Multiplier, bool CreateItems)
{
  for(ushort c = 0;; ++c)
    {
      float Difficulty = game::Difficulty();

      for(ushort i = 0; i < 10; ++i)
	{
	  ushort Chosen = 1 + RAND() % (protocontainer<character>::GetProtoAmount() - 1);
	  const character::prototype* const Proto = protocontainer<character>::GetProto(Chosen);

	  if(Proto->IsConcrete() && Proto->CanBeGenerated() && Proto->GetFrequency() > RAND() % 10000)
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
	if(protocontainer<item>::GetProto(c)->IsConcrete())
	  SumOfPossibilities += protocontainer<item>::GetProto(c)->GetPossibility();
			
      RandomOne = 1 + RAND() % (SumOfPossibilities);
		
      for(c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
	if(protocontainer<item>::GetProto(c)->IsConcrete())
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

      const character::prototype* const Proto = protocontainer<character>::GetProto(Chosen);

      if(Proto->IsConcrete() && Proto->CanBeGenerated())
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
    if(protocontainer<item>::GetProto(c)->IsConcrete())
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
    if(protocontainer<material>::GetProto(c)->IsConcrete())
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
    if(protocontainer<material>::GetProto(c)->IsSolid() && protocontainer<material>::GetProto(c)->IsConcrete())
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
