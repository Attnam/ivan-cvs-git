#include "proto.h"
#include "charba.h"
#include "itemba.h"
#include "materba.h"
#include "femath.h"
#include "message.h"

character* protosystem::BalancedCreateMonster(float Multiplier, bool CreateItems)
{
  for(ushort c = 0;; ++c)
    {
      float Difficulty = game::Difficulty();

      for(ushort i = 0; i < 10; ++i)
	{
	  ushort Chosen = 1 + RAND() % protocontainer<character>::GetProtoAmount();

	  const character::prototype* const Proto = protocontainer<character>::GetProto(Chosen);

	  if(Proto->CanBeGenerated() && Proto->Frequency() > RAND() % 10000)
	    {
	      character* Monster = Proto->Clone(true, true, CreateItems);

	      float Danger = Monster->MaxDanger();

	      if(c == 99 || (Danger < Difficulty * Multiplier * 2 && Danger > Difficulty * Multiplier / 2))
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

      for(c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
	SumOfPossibilities += protocontainer<item>::GetProto(c)->Possibility();
			
      RandomOne = 1 + RAND() % (SumOfPossibilities);
		
      for(c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
	{
	  Counter += protocontainer<item>::GetProto(c)->Possibility();

	  if(Counter >= RandomOne)
	    if(!Polymorph || protocontainer<item>::GetProto(c)->PolymorphSpawnable())
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
      ushort Chosen = 1 + RAND() % protocontainer<character>::GetProtoAmount();

      if(protocontainer<character>::GetProto(Chosen)->CanBeGenerated())
	{
	  character* Monster = protocontainer<character>::GetProto(Chosen)->Clone(true, true, CreateItems);

	  Monster->SetTeam(game::GetTeam(1));

	  return Monster;
	}
    }
}

item* protosystem::CreateItem(std::string What, bool Output)
{
  for(ushort c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
    {
      /* "Temporary" gum solution! */

      item* Temp = protocontainer<item>::GetProto(c)->Clone(false, false);

      if(Temp->NameSingular() == What)
	if(protocontainer<item>::GetProto(c)->CanBeWished() || game::GetWizardMode())
	  return protocontainer<item>::GetProto(c)->CreateWishedItem();
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

material* protosystem::CreateMaterial(std::string What, ulong Volume, bool Output)
{
  for(ushort c = 1; c <= protocontainer<material>::GetProtoAmount(); ++c)
    {
      /* "Temporary" gum solution! */

      material* Temp = protocontainer<material>::GetProto(c)->Clone();

      if(Temp->Name(UNARTICLED) == What)
	if(protocontainer<material>::GetProto(c)->CanBeWished())
	  return protocontainer<material>::GetProto(c)->CreateWishedMaterial(Volume);
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
  for(ushort c = 1 + RAND() % protocontainer<material>::GetProtoAmount();; c = 1 + RAND() % protocontainer<material>::GetProtoAmount())
    if(protocontainer<material>::GetProto(c)->IsSolid())
      return protocontainer<material>::GetProto(c)->Clone(Volume);
}

material* protosystem::CreateMaterial(ushort Index, ulong Volume)
{
  return protocontainer<material>::GetProto(Index)->Clone(Volume);
}
