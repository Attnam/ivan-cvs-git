#define __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__ // for prototyping system

#include "proto.h"

std::vector<typeable*> prototypesystem::ProtoData(2, 0);

#include "wterrain.h"
#include "char.h"
#include "material.h"
#include "item.h"
#include "lterrain.h"

#include "error.h"

ushort prototypesystem::Add(typeable* Proto)
{
	ProtoData.insert(ProtoData.end() - 1, Proto);

	return ProtoData.size() - 2;
}

character* prototypesystem::BalancedCreateMonster(float Multiplier)
{
	ushort Types = character::GetProtoIndexEnd() - character::GetProtoIndexBegin();

	for(ushort c = 0; ; c++)
	{
		ushort Chosen = character::GetProtoIndexBegin() + rand() % Types;

		if(!GetProtoType<character>(Chosen)->Possibility())
			continue;

		character* Monster = GetProtoType<character>(Chosen)->Clone(); //GGG

		if(c == 99 || (Monster->Danger() < game::Difficulty() * Multiplier * 1.5f && Monster->Danger() > game::Difficulty() * 0.5f))
			return Monster;
		else
			delete Monster;
	}
}

item* prototypesystem::BalancedCreateItem(void)
{
	ushort SumOfPossibilities = 0, Counter = 0, RandomOne;

	{
	for(ushort c = item::GetProtoIndexBegin(); c < item::GetProtoIndexEnd(); c++)
	{
		SumOfPossibilities += GetProtoType<item>(c)->Possibility();
	}
	}
		
	RandomOne = 1 + rand() % (SumOfPossibilities);
	
	for(ushort c = item::GetProtoIndexBegin(); c < item::GetProtoIndexEnd(); c++)
	{
		Counter += GetProtoType<item>(c)->Possibility();

		if(Counter >= RandomOne)
		{
			return CreateItem(c);
		}
	}

	ABORT("Balanced Create Item kaatuuu");

	return 0;
}

character* prototypesystem::CreateMonster(ushort Index)
{
	return GetProtoType<character>(Index)->Clone();
}

item* prototypesystem::CreateItem(ushort Index)
{
	return GetProtoType<item>(Index)->Clone();
}

item* prototypesystem::CreateItem(std::string What)
{
	for(ushort c = item::GetProtoIndexBegin(); c < item::GetProtoIndexEnd(); c++)
		if(GetProtoType<item>(c)->CanBeWished() && GetProtoType<item>(c)->GetNameSingular() == What)
			return GetProtoType<item>(c)->CreateWishedItem();

	return 0;
}

material* prototypesystem::CreateRandomSolidMaterial(ulong Volume)
{
	ushort Materials = material::GetProtoIndexEnd() - material::GetProtoIndexBegin();

	for(ushort c = material::GetProtoIndexBegin() + rand() % Materials;; c = material::GetProtoIndexBegin() + rand() % Materials)
		if(GetProtoType<material>(c)->IsSolid())
			return GetProtoType<material>(c)->Clone(Volume);
}

material* prototypesystem::CreateMaterial(ushort Index, ulong Volume)
{
	return GetProtoType<material>(Index)->Clone(Volume);
}
