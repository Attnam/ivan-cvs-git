#define __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__ // for prototyping system

#include "proto.h"

prototypecontainer<material>			prototypesystem::MaterialPrototype;
prototypecontainer<character>			prototypesystem::CharacterPrototype;
prototypecontainer<item>			prototypesystem::ItemPrototype;
prototypecontainer<groundlevelterrain>		prototypesystem::GroundLevelTerrainPrototype;
prototypecontainer<overlevelterrain>		prototypesystem::OverLevelTerrainPrototype;
prototypecontainer<groundworldmapterrain>	prototypesystem::GroundWorldMapTerrainPrototype;
prototypecontainer<overworldmapterrain>		prototypesystem::OverWorldMapTerrainPrototype;

#include "material.h"
#include "char.h"
#include "item.h"
#include "lterrain.h"
#include "wterrain.h"

#include "error.h"

ushort prototypesystem::AddProtoType(material* What)			{ return MaterialPrototype.Add(What); }
ushort prototypesystem::AddProtoType(character* What)			{ return CharacterPrototype.Add(What); }
ushort prototypesystem::AddProtoType(item* What)			{ return ItemPrototype.Add(What); }
ushort prototypesystem::AddProtoType(overlevelterrain* What)		{ return OverLevelTerrainPrototype.Add(What); }
ushort prototypesystem::AddProtoType(groundlevelterrain* What)		{ return GroundLevelTerrainPrototype.Add(What); }
ushort prototypesystem::AddProtoType(overworldmapterrain* What)		{ return OverWorldMapTerrainPrototype.Add(What); }
ushort prototypesystem::AddProtoType(groundworldmapterrain* What)	{ return GroundWorldMapTerrainPrototype.Add(What); }

character* prototypesystem::BalancedCreateMonster(void)
{
	ushort Types = 0;

	{
	for(ushort c = 1; CharacterPrototype[c]; c++)
		Types++;
	}

	for(ushort c = 0; ; c++)
	{
		ushort Chosen = 1 + rand() % Types;

		if(!CharacterPrototype[Chosen]->Possibility())
			continue;

		character* Monster = CharacterPrototype[Chosen]->Clone();

		if(c == 99 || (Monster->Danger() < game::Difficulty() * 1.5f && Monster->Danger() > game::Difficulty() * 0.5f))
			return Monster;
		else
			delete Monster;
	}
}

item* prototypesystem::BalancedCreateItem(void)
{
	ushort SumOfPossibilities = 0, Counter = 0, RandomOne;

	{
	for(ushort c = 1; ItemPrototype[c]; c++)
	{
		SumOfPossibilities += ItemPrototype[c]->Possibility();
	}
	}
		
	RandomOne = 1+ rand() % (SumOfPossibilities);
	
	for(ushort c = 1; ItemPrototype[c]; c++)
	{
		Counter += ItemPrototype[c]->Possibility();
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
	return CharacterPrototype[Index]->Clone();
}

item* prototypesystem::CreateItem(ushort Index)
{
	return ItemPrototype[Index]->Clone();
}

item* prototypesystem::CreateItem(std::string What)
{
	for(ushort x = 1; ItemPrototype[x]; x++)
		if(ItemPrototype[x]->CanBeWished() && ItemPrototype[x]->GetNameSingular() == What)
			return ItemPrototype[x]->CreateWishedItem();

	return 0;
}

material* prototypesystem::CreateRandomSolidMaterial(ulong Volume)
{
	for(ushort c = 1, Materials = 0; GetMaterialPrototype(c++); Materials++);

	for(c = 1 + rand() % Materials;; c = 1 + rand() % Materials)
		if(GetMaterialPrototype(c)->IsSolid())
			return GetMaterialPrototype(c)->Clone(Volume);
}

material* prototypesystem::CreateMaterial(ushort Index, ulong Volume)
{
	return MaterialPrototype[Index]->Clone(Volume);
}

material* prototypesystem::LoadMaterial(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	if(Type)
	{
		material* Material = MaterialPrototype[Type]->Clone();
		Material->Load(SaveFile);
		return Material;
	}
	else
		return 0;
}

character* prototypesystem::LoadCharacter(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	if(Type)
	{
		character* Character = CharacterPrototype[Type]->Clone(false, false, false);
		Character->Load(SaveFile);
		return Character;
	}
	else
		return 0;
}

item* prototypesystem::LoadItem(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	if(Type)
	{
		item* Item = ItemPrototype[Type]->Clone(false, false);
		Item->Load(SaveFile);
		return Item;
	}
	else
		return 0;
}

groundlevelterrain* prototypesystem::LoadGroundLevelTerrain(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	if(Type)
	{
		groundlevelterrain* GroundLevelTerrain = GroundLevelTerrainPrototype[Type]->Clone(false, false);
		GroundLevelTerrain->Load(SaveFile);
		return GroundLevelTerrain;
	}
	else
		return 0;
}

overlevelterrain* prototypesystem::LoadOverLevelTerrain(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	if(Type)
	{
		overlevelterrain* OverLevelTerrain = OverLevelTerrainPrototype[Type]->Clone(false, false);
		OverLevelTerrain->Load(SaveFile);
		return OverLevelTerrain;
	}
	else
		return 0;
}

groundworldmapterrain* prototypesystem::LoadGroundWorldMapTerrain(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	if(Type)
	{
		groundworldmapterrain* GroundWorldMapTerrain = GroundWorldMapTerrainPrototype[Type]->Clone(false);
		GroundWorldMapTerrain->Load(SaveFile);
		return GroundWorldMapTerrain;
	}
	else
		return 0;
}

overworldmapterrain* prototypesystem::LoadOverWorldMapTerrain(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	if(Type)
	{
		overworldmapterrain* OverWorldMapTerrain = OverWorldMapTerrainPrototype[Type]->Clone(false);
		OverWorldMapTerrain->Load(SaveFile);
		return OverWorldMapTerrain;
	}
	else
		return 0;
}
