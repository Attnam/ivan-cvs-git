#ifndef __ALLOCATE_H__
#define __ALLOCATE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

template <class type> inline void Alloc2D(type**& Map, ulong XSize, ulong YSize)
{
  ulong MapXSize = sizeof(type*) * XSize;
  Map = reinterpret_cast<type**>(new uchar[MapXSize + sizeof(type) * XSize * YSize]);
  ulong XPointer = ulong(Map) + MapXSize, TrueYSize = YSize * sizeof(type);

  for(ulong x = 0; x < XSize; ++x, XPointer += TrueYSize)
    Map[x] = reinterpret_cast<type*>(XPointer);
}

template <class type> inline type** Alloc2D(ulong XSize, ulong YSize)
{
  type** Map;
  Alloc2D(Map, XSize, YSize);
  return Map;
}

template <class type> inline void Alloc2D(type**& Map, ulong XSize, ulong YSize, const type& Initializer)
{
  ulong MapXSize = sizeof(type*) * XSize;
  Map = reinterpret_cast<type**>(new uchar[MapXSize + sizeof(type) * XSize * YSize]);
  ulong XPointer = ulong(Map) + MapXSize, TrueYSize = YSize * sizeof(type);

  for(ulong x = 0; x < XSize; ++x, XPointer += TrueYSize)
    {
      Map[x] = reinterpret_cast<type*>(XPointer);

      for(ulong y = 0; y < YSize; ++y)
	Map[x][y] = Initializer;
    }
}

template <class type> inline type** Alloc2D(ulong XSize, ulong YSize, const type& Initializer)
{
  type** Map;
  Alloc2D(Map, XSize, YSize, Initializer);
  return Map;
}

template <class type> inline void Fill2D(type** Map, ulong CornerXPos, ulong CornerYPos, ulong XSize, ulong YSize, const type& Value)
{
  for(ulong x = CornerXPos; x < CornerXPos + XSize; ++x)
    for(ulong y = CornerYPos; y < CornerYPos + YSize; ++y)
      Map[x][y] = Value;
}

template <class type> inline void Alloc3D(type***& Map, ulong XSize, ulong YSize, ulong ZSize)
{
  ulong MapXSize = sizeof(type**) * XSize;
  ulong MapYSize = sizeof(type*) * XSize * YSize;
  Map = reinterpret_cast<type***>(new uchar[MapXSize + MapYSize + sizeof(type) * XSize * YSize * ZSize]);
  ulong XPointer = ulong(Map) + MapXSize, YPointer = XPointer + MapYSize, TrueYSize = YSize * sizeof(type*), TrueZSize = ZSize * sizeof(type);

  for(ulong x = 0; x < XSize; ++x, XPointer += TrueYSize)
    {
      Map[x] = reinterpret_cast<type**>(XPointer);

      for(ulong y = 0; y < YSize; ++y, YPointer += TrueZSize)
	Map[x][y] = reinterpret_cast<type*>(YPointer);
    }
}

template <class type> inline type*** Alloc3D(ulong XSize, ulong YSize, ulong ZSize)
{
  type*** Map;
  Alloc3D(Map, XSize, YSize, ZSize);
  return Map;
}

template <class type> inline void Alloc3D(type***& Map, ulong XSize, ulong YSize, ulong ZSize, const type& Initializer)
{
  ulong MapXSize = sizeof(type**) * XSize;
  ulong MapYSize = sizeof(type*) * XSize * YSize;
  Map = reinterpret_cast<type***>(new uchar[MapXSize + MapYSize + sizeof(type) * XSize * YSize * ZSize]);
  ulong XPointer = ulong(Map) + MapXSize, YPointer = XPointer + MapYSize, TrueYSize = YSize * sizeof(type*), TrueZSize = ZSize * sizeof(type);

  for(ulong x = 0; x < XSize; ++x, XPointer += TrueYSize)
    {
      Map[x] = reinterpret_cast<type**>(XPointer);

      for(ulong y = 0; y < YSize; ++y, YPointer += TrueZSize)
	{
	  Map[x][y] = reinterpret_cast<type*>(YPointer);

	  for(ulong z = 0; z < ZSize; ++z)
	    Map[x][y][z] = Initializer;
	}
    }
}

template <class type> inline type*** Alloc3D(ulong XSize, ulong YSize, ulong ZSize, const type& Initializer)
{
  type*** Map;
  Alloc3D(Map, XSize, YSize, ZSize, Initializer);
  return Map;
}

template <class type> inline void Fill3D(type*** Map, ulong CornerXPos, ulong CornerYPos, ulong CornerZPos, ulong XSize, ulong YSize, ulong ZSize, const type& Value)
{
  for(ulong x = CornerXPos; x < CornerXPos + XSize; ++x)
    for(ulong y = CornerYPos; y < CornerYPos + YSize; ++y)
      for(ulong z = CornerZPos; z < CornerZPos + ZSize; ++z)
        Map[x][y] = Value;
}

#endif

