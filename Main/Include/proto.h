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

/*#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define BEGIN_PROTOTYPING(protoclass) ushort protoclass::ProtoIndexBegin = prototypesystem::GetNextProtoIndex();
	#define FINISH_PROTOTYPING(protoclass) ushort protoclass::ProtoIndexEnd = prototypesystem::GetNextProtoIndex();

#else

	#define BEGIN_PROTOTYPING(protoclass)
	#define FINISH_PROTOTYPING(protoclass)

#endif*/

template <class type> class protocontainer
{
public:
	static ushort Add(type*);
	static const type* const GetProto(ushort Index) { return ProtoData[Index]; }
	static ushort GetProtoAmount(void) { return ProtoData.size() - 2; }
	//static void Load(ivanifstream&, ProtoType*&);
private:
	static std::vector<type*> ProtoData;
};

template <class type> ushort protocontainer<type>::Add(type* Proto)
{
	ProtoData.insert(ProtoData.end() - 1, Proto);
	return ProtoData.size() - 2;
}

//class typeable;

class protosystem
{
public:
	static character*		BalancedCreateMonster(float = 1);
	static item*			BalancedCreateItem(void);
	static character*		CreateMonster(ushort);
	static item*			CreateItem(ushort);
	static item*			CreateItem(std::string);
	static material*		CreateRandomSolidMaterial(ulong);
	static material*		CreateMaterial(ushort, ulong);

	//template <class type> static ushort Add(type* Proto) { return protocontainer<type>::Add(Proto); }
	//template <class type> static const type* const GetProto(ushort Index) { return protocontainer<type>::GetProto(Index); }
	//template <class type> static ushort GetProtoAmount(void) { return protocontainer<type>::GetProtoAmount(); }

	/*static ushort Add(typeable*);
	static const typeable* const Access(ushort Index) { return ProtoData[Index]; }
	static ushort GetNextProtoIndex(void) { return ProtoData.size() - 1; }*/
private:
	//static std::vector<typeable*> ProtoData;
	/*static std::vector<item*> 
	static std::vector<character*>
	static std::vector<material*>
	static std::vector<groundlevelterrain*>
	static std::vector<overlevelterrain*>
	static std::vector<groundworldmapterrain*>
	static std::vector<overworldmapterrain*>*/
};

/*template <class ProtoType> const ProtoType* const GetProtoType(ushort Index)
{
	return dynamic_cast<const ProtoType* const>(prototypesystem::Access(Index));
}*/

template <class type> inline std::ofstream& operator<<(std::ofstream& SaveFile, type* Class)
{
	if(Class)
		Class->Save(SaveFile);
	else
	{
		SaveFile.put(0);
		SaveFile.put(0);
	}

	return SaveFile;
}

template <class type> inline std::ifstream& operator>>(std::ifstream& SaveFile, type*& Class)
{
	ushort Type;

	Type = SaveFile.get() | SaveFile.get() << 8;

	if(Type)
		Class = dynamic_cast<type*>(protocontainer<type>::GetProto(Type)->CloneAndLoad(SaveFile));

	return SaveFile;
}

#endif

