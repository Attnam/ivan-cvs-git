#ifndef __PROTO_H__
#define __PROTO_H__

#include <vector>

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

#endif
