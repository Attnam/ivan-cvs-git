#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

struct donothing { };

typedef signed   char  schar;
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;

#ifdef VC
typedef __int64 longlong;
typedef unsigned __int64 ulonglong;
#endif

#ifdef GCC
typedef long long longlong;
typedef unsigned long long ulonglong;
#endif

#endif
