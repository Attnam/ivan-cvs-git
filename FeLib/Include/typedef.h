#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <map>

typedef signed   char  schar;
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;

typedef std::map<std::string, long> valuemap;

#endif
