#ifndef __STROVER_H__
#define __STROVER_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <cstdio>

#include "typedef.h"

inline std::string operator+ (const std::string& CppString, const short& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  return CppString + Buffer;
}

inline std::string operator+ (const std::string& CppString, const ushort& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  return CppString + Buffer;
}

inline std::string operator+ (const std::string& CppString, const int& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", Int);
  return CppString + Buffer;
}

inline std::string operator+ (const std::string& CppString, const long& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  return CppString + Buffer;
}

inline std::string operator+ (const std::string& CppString, const ulong& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  return CppString + Buffer;
}

inline std::string& operator+= (std::string& CppString, const short& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  CppString += Buffer;
  return CppString;
}

inline std::string& operator+= (std::string& CppString, const ushort& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  CppString += Buffer;
  return CppString;
}

inline std::string& operator+= (std::string& CppString, const int& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", Int);
  CppString += Buffer;
  return CppString;
}

inline std::string& operator+= (std::string& CppString, const long& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  CppString += Buffer;
  return CppString;
}

inline std::string& operator+= (std::string& CppString, const ulong& Int)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  CppString += Buffer;
  return CppString;
}

inline std::string operator+ (const char* CString, std::string CppString)
{
  return std::string(CString) + CppString;
}

#endif
