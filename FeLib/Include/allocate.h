/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __ALLOCATE_H__
#define __ALLOCATE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

template <class type> inline void Alloc2D(type**& Map, int XSize, int YSize)
{
  Map = reinterpret_cast<type**>(new char[XSize * (sizeof(type*) + YSize * sizeof(type))]);
  type* XPointer = reinterpret_cast<type*>(Map + XSize);

  for(int x = 0; x < XSize; ++x, XPointer += YSize)
    Map[x] = XPointer;
}

template <class type> inline void Alloc2D(type**& Map, int XSize, int YSize, const type& Initializer)
{
  Map = reinterpret_cast<type**>(new char[XSize * (sizeof(type*) + YSize * sizeof(type))]);
  type* XPointer = reinterpret_cast<type*>(Map + XSize);

  for(int x = 0; x < XSize; ++x, XPointer += YSize)
    {
      Map[x] = XPointer;

      for(int y = 0; y < YSize; ++y)
	Map[x][y] = Initializer;
    }
}

template <class type> inline void Alloc3D(type***& Map, int XSize, int YSize, int ZSize)
{
  Map = reinterpret_cast<type***>(new char[XSize * (sizeof(type**) + YSize * (sizeof(type*) + ZSize * sizeof(type)))]);
  type** XPointer = reinterpret_cast<type**>(Map + XSize);
  type* YPointer = reinterpret_cast<type*>(XPointer + XSize * YSize);

  for(int x = 0; x < XSize; ++x, XPointer += YSize)
    {
      Map[x] = XPointer;

      for(int y = 0; y < YSize; ++y, YPointer += ZSize)
	Map[x][y] = YPointer;
    }
}

template <class type> inline void Alloc3D(type***& Map, int XSize, int YSize, int ZSize, const type& Initializer)
{
  Map = reinterpret_cast<type***>(new char[XSize * (sizeof(type**) + YSize * (sizeof(type*) + ZSize * sizeof(type)))]);
  type** XPointer = reinterpret_cast<type**>(Map + XSize);
  type* YPointer = reinterpret_cast<type*>(XPointer + XSize * YSize);

  for(int x = 0; x < XSize; ++x, XPointer += YSize)
    {
      Map[x] = XPointer;

      for(int y = 0; y < YSize; ++y, YPointer += ZSize)
	{
	  Map[x][y] = YPointer;

	  for(int z = 0; z < ZSize; ++z)
	    Map[x][y][z] = Initializer;
	}
    }
}

#endif
