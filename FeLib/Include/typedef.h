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

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

struct donothing { };

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int color16;
typedef ushort packedcolor16;
typedef ulong color24;
typedef int alpha;
typedef uchar packedalpha;
typedef int priority;
typedef uchar packedpriority;
typedef uchar paletteindex;

#endif
