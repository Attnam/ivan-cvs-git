#ifndef __ALLOCATE_H__
#define __ALLOCATE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

template <class type> inline void Alloc2D(type**& Map, ulong XSize, ulong YSize)
{
  Map = reinterpret_cast<type**>(new uchar[XSize * (sizeof(type*) + YSize * sizeof(type))]);
  type* XPointer = reinterpret_cast<type*>(Map + XSize);

  for(ulong x = 0; x < XSize; ++x, XPointer += YSize)
    Map[x] = XPointer;
}

template <class type> inline void Alloc2D(type**& Map, ulong XSize, ulong YSize, const type& Initializer)
{
  Map = reinterpret_cast<type**>(new uchar[XSize * (sizeof(type*) + YSize * sizeof(type))]);
  type* XPointer = reinterpret_cast<type*>(Map + XSize);

  for(ulong x = 0; x < XSize; ++x, XPointer += YSize)
    {
      Map[x] = XPointer;

      for(ulong y = 0; y < YSize; ++y)
	Map[x][y] = Initializer;
    }
}

template <class type> inline void Alloc3D(type***& Map, ulong XSize, ulong YSize, ulong ZSize)
{
  Map = reinterpret_cast<type***>(new uchar[XSize * (sizeof(type**) + YSize * (sizeof(type*) + ZSize * sizeof(type)))]);
  type** XPointer = reinterpret_cast<type**>(Map + XSize);
  type* YPointer = reinterpret_cast<type*>(XPointer + XSize * YSize);

  for(ulong x = 0; x < XSize; ++x, XPointer += YSize)
    {
      Map[x] = XPointer;

      for(ulong y = 0; y < YSize; ++y, YPointer += ZSize)
	Map[x][y] = YPointer;
    }
}

template <class type> inline void Alloc3D(type***& Map, ulong XSize, ulong YSize, ulong ZSize, const type& Initializer)
{
  Map = reinterpret_cast<type***>(new uchar[XSize * (sizeof(type**) + YSize * (sizeof(type*) + ZSize * sizeof(type)))]);
  type** XPointer = reinterpret_cast<type**>(Map + XSize);
  type* YPointer = reinterpret_cast<type*>(XPointer + XSize * YSize);

  for(ulong x = 0; x < XSize; ++x, XPointer += YSize)
    {
      Map[x] = XPointer;

      for(ulong y = 0; y < YSize; ++y, YPointer += ZSize)
	{
	  Map[x][y] = YPointer;

	  for(ulong z = 0; z < ZSize; ++z)
	    Map[x][y][z] = Initializer;
	}
    }
}

#endif
