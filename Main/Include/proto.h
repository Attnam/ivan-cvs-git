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

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define BEGIN_PROTOTYPING(protoclass) ushort protoclass::ProtoIndexBegin = prototypesystem::GetNextProtoIndex();
	#define FINISH_PROTOTYPING(protoclass) ushort protoclass::ProtoIndexEnd = prototypesystem::GetNextProtoIndex();

#else

	#define BEGIN_PROTOTYPING(protoclass)
	#define FINISH_PROTOTYPING(protoclass)

#endif

class typeable;

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

	static ushort Add(typeable*);
	static const typeable* const Access(ushort Index) { return ProtoData[Index]; }
	static ushort GetNextProtoIndex(void) { return ProtoData.size() - 1; }
private:
	static std::vector<typeable*> ProtoData;
};

template <class ProtoType> const ProtoType* const GetProtoType(ushort Index)
{
	return dynamic_cast<const ProtoType* const>(prototypesystem::Access(Index));
}

template <class Class> inline std::ofstream& operator<<(std::ofstream& SaveFile, Class* ClassPtr)
{
	if(ClassPtr)
		ClassPtr->Save(SaveFile);
	else
	{
		SaveFile.put(0);
		SaveFile.put(0);
	}

	return SaveFile;
}

template <class Class> inline std::ifstream& operator>>(std::ifstream& SaveFile, Class*& ClassPtr)
{
	ushort Type;

	Type = SaveFile.get() | SaveFile.get() << 8;

	if(Type)
		ClassPtr = dynamic_cast<Class*>(prototypesystem::Access(Type)->CloneAndLoad(SaveFile));

	return SaveFile;
}

#endif
