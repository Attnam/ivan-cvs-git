#ifndef __PROTO_H__
#define __PROTO_H__

#pragma warning(disable : 4786)

#include <vector>
#include <map>
#include <string>

#include "typedef.h"

class material;
class character;
class item;
class groundlevelterrain;
class overlevelterrain;
class groundworldmapterrain;
class overworldmapterrain;
class outputfile;
class inputfile;

template <class type> class protocontainer
{
public:
	static ushort Add(type*);
	static const type* const GetProto(ushort Index) { return ProtoData[Index]; }
	static ushort GetProtoAmount() { return ProtoData.size() - 2; }
	static ushort SearchCodeName(std::string);
private:
	static std::vector<type*> ProtoData;
	static std::map<std::string, ushort> CodeNameMap;
};

template <class type> ushort protocontainer<type>::Add(type* Proto)
{
	ProtoData.insert(ProtoData.end() - 1, Proto);
	CodeNameMap[Proto->ClassName()] = ProtoData.size() - 2;
	return ProtoData.size() - 2;
}

template <class type> ushort protocontainer<type>::SearchCodeName(std::string Name)
{
	std::map<std::string, ushort>::iterator I = CodeNameMap.find(Name);

	if(I != CodeNameMap.end())
		return (*I)->GetType();
	else
		return 0;
}

class protosystem
{
public:
	static character*		BalancedCreateMonster(float = 1);
	static item*			BalancedCreateItem();
	static character*		CreateMonster(ushort);
	static item*			CreateItem(ushort);
	static item*			CreateItem(std::string);
	static material*		CreateRandomSolidMaterial(ulong);
	static material*		CreateMaterial(ushort, ulong);
};

template <class type> inline outputfile& operator<<(outputfile& SaveFile, type* Class)
{
	if(Class)
		Class->Save(SaveFile);
	else
		SaveFile << ushort(0);

	return SaveFile;
}

template <class type> inline inputfile& operator>>(inputfile& SaveFile, type*& Class)
{
	ushort Type;

	SaveFile >> Type;

	if(Type)
		Class = dynamic_cast<type*>(protocontainer<type>::GetProto(Type)->CloneAndLoad(SaveFile));

	return SaveFile;
}

#endif
