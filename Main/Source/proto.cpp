#define __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

#include "proto.h"

std::vector<material*>			protocontainer<material>::ProtoData;
std::vector<item*>			protocontainer<item>::ProtoData;
std::vector<character*>			protocontainer<character>::ProtoData;
std::vector<groundlevelterrain*>	protocontainer<groundlevelterrain>::ProtoData;
std::vector<overlevelterrain*>		protocontainer<overlevelterrain>::ProtoData;
std::vector<groundworldmapterrain*>	protocontainer<groundworldmapterrain>::ProtoData;
std::vector<overworldmapterrain*>	protocontainer<overworldmapterrain>::ProtoData;
std::map<std::string, ushort>		protocontainer<material>::CodeNameMap;
std::map<std::string, ushort>		protocontainer<item>::CodeNameMap;
std::map<std::string, ushort>		protocontainer<character>::CodeNameMap;
std::map<std::string, ushort>		protocontainer<groundlevelterrain>::CodeNameMap;
std::map<std::string, ushort>		protocontainer<overlevelterrain>::CodeNameMap;
std::map<std::string, ushort>		protocontainer<groundworldmapterrain>::CodeNameMap;
std::map<std::string, ushort>		protocontainer<overworldmapterrain>::CodeNameMap;

#include "materde.h"
#include "itemde.h"
#include "charde.h"
#include "lterrade.h"
#include "wterrade.h"

#include "error.h"

character* protosystem::BalancedCreateMonster(float Multiplier)
{
	for(ushort c = 0; ; ++c)
	{
		ushort Chosen = 1 + rand() % protocontainer<character>::GetProtoAmount();

		if(!protocontainer<character>::GetProto(Chosen)->Possibility())
			continue;

		character* Monster = protocontainer<character>::GetProto(Chosen)->Clone();

		if(c == 99 || (Monster->Danger() < game::Difficulty() * Multiplier * 1.5f && Monster->Danger() > game::Difficulty() * 0.5f))
		{
			Monster->SetTeam(game::GetTeam(1));
			return Monster;
		}
		else
			delete Monster;
	}
}

item* protosystem::BalancedCreateItem()
{
	ushort SumOfPossibilities = 0, Counter = 0, RandomOne;

	{
	for(ushort c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
	{
		SumOfPossibilities += protocontainer<item>::GetProto(c)->Possibility();
	}
	}
		
	RandomOne = 1 + rand() % (SumOfPossibilities);
	
	for(ushort c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
	{
		Counter += protocontainer<item>::GetProto(c)->Possibility();

		if(Counter >= RandomOne)
		{
			return CreateItem(c);
		}
	}

	ABORT("Balanced Create Item kaatuuu");

	return 0;
}

character* protosystem::CreateMonster(ushort Index)
{
	return protocontainer<character>::GetProto(Index)->Clone();
}

item* protosystem::CreateItem(ushort Index)
{
	return protocontainer<item>::GetProto(Index)->Clone();
}

item* protosystem::CreateItem(std::string What)
{
	for(ushort c = 1; c <= protocontainer<item>::GetProtoAmount(); ++c)
		if(protocontainer<item>::GetProto(c)->CanBeWished() && protocontainer<item>::GetProto(c)->GetNameSingular() == What)
			return protocontainer<item>::GetProto(c)->CreateWishedItem();

	return 0;
}

material* protosystem::CreateMaterial(std::string What, ulong Volume)
{
	for(ushort c = 1; c <= protocontainer<material>::GetProtoAmount(); ++c)
		if(protocontainer<material>::GetProto(c)->CanBeWished() && protocontainer<material>::GetProto(c)->Name() == What)
			return protocontainer<material>::GetProto(c)->CreateWishedMaterial(Volume);

	return 0;
}
material* protosystem::CreateRandomSolidMaterial(ulong Volume)
{
	for(ushort c = 1 + rand() % protocontainer<material>::GetProtoAmount();; c = 1 + rand() % protocontainer<material>::GetProtoAmount())
		if(protocontainer<material>::GetProto(c)->IsSolid())
			return protocontainer<material>::GetProto(c)->Clone(Volume);
}

material* protosystem::CreateMaterial(ushort Index, ulong Volume)
{
	return protocontainer<material>::GetProto(Index)->Clone(Volume);
}
