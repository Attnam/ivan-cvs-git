#ifndef __ALLOCATE_H__
#define __ALLOCATE_H__

#include "typedef.h"

template <class Type> inline Type** Alloc2D(ulong XSize, ulong YSize)
{
	ulong MapSize = sizeof(Type*) * XSize;

	Type** Map = (Type**)(new uchar[MapSize + sizeof(Type) * XSize * YSize]);

	ulong Pointer = ulong(Map) + MapSize, TrueYSize = YSize * sizeof(Type);

	for(ulong c = 0; c < XSize; c++, Pointer += TrueYSize)
		Map[c] = (Type*)Pointer;

	return Map;
}

#endif
