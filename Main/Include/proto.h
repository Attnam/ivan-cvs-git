#ifndef __PROTO_H__
#define __PROTO_H__

#include <vector>

template <class ProtoType> class prototypecontainer
{
public:
	prototypecontainer(void) { ProtoData.push_back(0); }
	void Add(const ushort, ProtoType*);
	const ProtoType* const Access(ushort Index) const { return ProtoData[Index]; }
	const ProtoType* const operator [] (ushort Index) const { return ProtoData[Index]; }
private:
	std::vector<ProtoType*> ProtoData;
};

template <class ProtoType> inline void prototypecontainer<ProtoType>::Add(const ushort Index, ProtoType* Proto)
{
	if(ProtoData.size() < Index + 2)
	{
		ProtoData.resize(Index + 2, 0);
		ProtoData[Index] = Proto;
	}
	else
		ProtoData[Index] = Proto;
}

#endif
