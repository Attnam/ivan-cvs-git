#ifndef __PROTO_H__
#define __PROTO_H__

#include <fstream>
#include <vector>

#include "typedef.h"

class material;
class character;
class item;
class groundlevelterrain;
class overlevelterrain;
class groundworldmapterrain;
class overworldmapterrain;

template <class ProtoType> class prototypecontainer
{
public:
	prototypecontainer(void) { ProtoData.push_back(0); ProtoData.push_back(0); }
	ushort Add(ProtoType*);
	const ProtoType* const Access(ushort Index) const { return ProtoData[Index]; }
	const ProtoType* const operator [] (ushort Index) const { return ProtoData[Index]; }
private:
	std::vector<ProtoType*> ProtoData;
};

template <class ProtoType> inline ushort prototypecontainer<ProtoType>::Add(ProtoType* Proto)
{
	ProtoData.insert(ProtoData.end() - 1, Proto);

	return ProtoData.size() - 2;
}

class prototypesystem
{
public:
	static character*		BalancedCreateMonster(void);
	static item*			BalancedCreateItem(void);
	static character*		CreateMonster(ushort);
	static item*			CreateItem(ushort);
	static item*			CreateItem(std::string);
	static material*		CreateRandomSolidMaterial(ulong);
	static material*		CreateMaterial(ushort, ulong);

	static material*		LoadMaterial(std::ifstream*);
	static character*		LoadCharacter(std::ifstream*);
	static item*			LoadItem(std::ifstream*);
	static groundlevelterrain*	LoadGroundLevelTerrain(std::ifstream*);
	static overlevelterrain*	LoadOverLevelTerrain(std::ifstream*);
	static groundworldmapterrain*	LoadGroundWorldMapTerrain(std::ifstream*);
	static overworldmapterrain*	LoadOverWorldMapTerrain(std::ifstream*);

	static const material*			const	GetMaterialPrototype(ushort Index)		{ return MaterialPrototype[Index]; }
	static const character*			const	GetCharacterPrototype(ushort Index)		{ return CharacterPrototype[Index]; }
	static const item*			const	GetItemPrototype(ushort Index)			{ return ItemPrototype[Index]; }
	static const groundlevelterrain*	const	GetGroundLevelTerrainPrototype(ushort Index)	{ return GroundLevelTerrainPrototype[Index]; }
	static const overlevelterrain*		const	GetOverLevelTerrainPrototype(ushort Index)	{ return OverLevelTerrainPrototype[Index]; }
	static const groundworldmapterrain*	const	GetGroundWorldMapTerrainPrototype(ushort Index)	{ return GroundWorldMapTerrainPrototype[Index]; }
	static const overworldmapterrain*	const	GetOverWorldMapTerrainPrototype(ushort Index)	{ return OverWorldMapTerrainPrototype[Index]; }

	static ushort AddProtoType(material*);
	static ushort AddProtoType(character*);
	static ushort AddProtoType(item*);
	static ushort AddProtoType(groundlevelterrain*);
	static ushort AddProtoType(overlevelterrain*);
	static ushort AddProtoType(groundworldmapterrain*);
	static ushort AddProtoType(overworldmapterrain*);
private:
	static prototypecontainer<material> MaterialPrototype;
	static prototypecontainer<character> CharacterPrototype;
	static prototypecontainer<item> ItemPrototype;
	static prototypecontainer<groundlevelterrain> GroundLevelTerrainPrototype;
	static prototypecontainer<overlevelterrain> OverLevelTerrainPrototype;
	static prototypecontainer<groundworldmapterrain> GroundWorldMapTerrainPrototype;
	static prototypecontainer<overworldmapterrain> OverWorldMapTerrainPrototype;
};

#endif
