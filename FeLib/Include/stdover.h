#ifndef __STDOVER_H__
#define __STDOVER_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <cstdio>
#include <string>
#include <vector>

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

inline std::string operator+ (const short& Int, const std::string& CppString)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  return Buffer + CppString;
}

inline std::string operator+ (const ushort& Int, const std::string& CppString)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  return Buffer + CppString;
}

inline std::string operator+ (const int& Int, const std::string& CppString)
{
  char Buffer[12];
  sprintf(Buffer, "%d", Int);
  return Buffer + CppString;
}

inline std::string operator+ (const long& Int, const std::string& CppString)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  return Buffer + CppString;
}

inline std::string operator+ (const ulong& Int, const std::string& CppString)
{
  char Buffer[12];
  sprintf(Buffer, "%d", int(Int));
  return Buffer + CppString;
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

template <class type> std::vector<type>& operator<<(std::vector<type>& Vector, const type& Element)
{
  Vector.push_back(Element);
  return Vector;
}

inline std::string CapitalizeCopy(std::string String)
{
  if(String[0] > 0x60 && String[0] < 0x7B)
    String[0] &= ~0x20;

  return String;
}

inline void Capitalize(std::string& String)
{
  if(String[0] > 0x60 && String[0] < 0x7B)
    String[0] &= ~0x20;
}

inline std::string& operator<<(std::string& CppString, const char* CString)
{
  return CppString += CString;
}

inline std::string& operator<<(std::string& CppString1, const std::string& CppString2)
{
  return CppString1 += CppString2;
}

inline std::string& operator<< (std::string& CppString, const short& Int)
{
  return CppString += Int;
}

inline std::string& operator<< (std::string& CppString, const ushort& Int)
{
  return CppString += Int;
}

inline std::string& operator<< (std::string& CppString, const int& Int)
{
  return CppString += Int;
}

inline std::string& operator<< (std::string& CppString, const long& Int)
{
  return CppString += Int;
}

inline std::string& operator<< (std::string& CppString, const ulong& Int)
{
  return CppString += Int;
}

#endif
