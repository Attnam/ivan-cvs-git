#ifndef __ALLOCATE_H__
#define __ALLOCATE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

template <class Type> inline void Alloc2D(Type**& Map, ulong XSize, ulong YSize)
{
  ulong MapXSize = sizeof(Type*) * XSize;
  Map = (Type**)(new uchar[MapXSize + sizeof(Type) * XSize * YSize]);
  ulong XPointer = ulong(Map) + MapXSize, TrueYSize = YSize * sizeof(Type);

  for(ulong x = 0; x < XSize; ++x, XPointer += TrueYSize)
    Map[x] = (Type*)XPointer;
}

template <class Type> inline Type** Alloc2D(ulong XSize, ulong YSize)
{
  Type** Map;
  Alloc2D(Map, XSize, YSize);
  return Map;
}

template <class Type> inline void Alloc2D(Type**& Map, ulong XSize, ulong YSize, const Type& Initializer)
{
  ulong MapXSize = sizeof(Type*) * XSize;
  Map = (Type**)(new uchar[MapXSize + sizeof(Type) * XSize * YSize]);
  ulong XPointer = ulong(Map) + MapXSize, TrueYSize = YSize * sizeof(Type);

  for(ulong x = 0; x < XSize; ++x, XPointer += TrueYSize)
  {
    Map[x] = (Type*)XPointer;

    for(ulong y = 0; y < YSize; ++y)
      Map[x][y] = Initializer;
  }
}

template <class Type> inline Type** Alloc2D(ulong XSize, ulong YSize, const Type& Initializer)
{
  Type** Map;
  Alloc2D(Map, XSize, YSize, Initializer);
  return Map;
}

template <class Type> inline void Fill2D(Type** Map, ulong CornerXPos, ulong CornerYPos, ulong XSize, ulong YSize, const Type& Value)
{
  for(ulong x = CornerXPos; x < CornerXPos + XSize; ++x)
    for(ulong y = CornerYPos; y < CornerYPos + YSize; ++y)
      Map[x][y] = Value;
}

template <class Type> inline void Alloc3D(Type***& Map, ulong XSize, ulong YSize, ulong ZSize)
{
  ulong MapXSize = sizeof(Type**) * XSize;
  ulong MapYSize = sizeof(Type*) * XSize * YSize;
  Map = (Type***)(new uchar[MapXSize + MapYSize + sizeof(Type) * XSize * YSize * ZSize]);
  ulong XPointer = ulong(Map) + MapXSize, YPointer = XPointer + MapYSize, TrueYSize = YSize * sizeof(Type*), TrueZSize = ZSize * sizeof(Type);

  for(ulong x = 0; x < XSize; ++x, XPointer += TrueYSize)
  {
    Map[x] = (Type**)XPointer;

    for(ulong y = 0; y < YSize; ++y, YPointer += TrueZSize)
      Map[x][y] = (Type*)YPointer;
  }
}

template <class Type> inline Type*** Alloc3D(ulong XSize, ulong YSize, ulong ZSize)
{
  Type*** Map;
  Alloc3D(Map, XSize, YSize, ZSize);
  return Map;
}

template <class Type> inline void Alloc3D(Type***& Map, ulong XSize, ulong YSize, ulong ZSize, const Type& Initializer)
{
  ulong MapXSize = sizeof(Type**) * XSize;
  ulong MapYSize = sizeof(Type*) * XSize * YSize;
  Map = (Type***)(new uchar[MapXSize + MapYSize + sizeof(Type) * XSize * YSize * ZSize]);
  ulong XPointer = ulong(Map) + MapXSize, YPointer = XPointer + MapYSize, TrueYSize = YSize * sizeof(Type*), TrueZSize = ZSize * sizeof(Type);

  for(ulong x = 0; x < XSize; ++x, XPointer += TrueYSize)
  {
    Map[x] = (Type**)XPointer;

    for(ulong y = 0; y < YSize; ++y, YPointer += TrueZSize)
    {
      Map[x][y] = (Type*)YPointer;

      for(ulong z = 0; z < ZSize; ++z)
        Map[x][y][z] = Initializer;
    }
  }
}

template <class Type> inline Type*** Alloc3D(ulong XSize, ulong YSize, ulong ZSize, const Type& Initializer)
{
  Type*** Map;
  Alloc3D(Map, XSize, YSize, ZSize, Initializer);
  return Map;
}

template <class Type> inline void Fill3D(Type*** Map, ulong CornerXPos, ulong CornerYPos, ulong CornerZPos, ulong XSize, ulong YSize, ulong ZSize, const Type& Value)
{
  for(ulong x = CornerXPos; x < CornerXPos + XSize; ++x)
    for(ulong y = CornerYPos; y < CornerYPos + YSize; ++y)
      for(ulong z = CornerZPos; z < CornerZPos + ZSize; ++z)
        Map[x][y] = Value;
}

#endif
