#ifndef __FESTRING_H__
#define __FESTRING_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "typedef.h"

/* Class for containing various std::string handling functions */

class festring
{
public:
  static void InstallIntegerMap();
  static void DeInstallIntegerMap();
  static char* IntegerToChar(long);
  static void SplitString(std::string&, std::string&, std::string::size_type);
  static void SplitString(const std::string&, std::vector<std::string>&, std::string::size_type, std::string::size_type = 0);
  static std::string& Capitalize(std::string&);
  static std::string CapitalizeCopy(std::string String) { return Capitalize(String); }
private:
  static char** IntegerMap;
  static char IntegerBuffer[12];
};

/* Overloaded operators that make std::string use much more convenient */

inline std::string operator+(std::string String, short Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string operator+(std::string String, ushort Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string operator+(std::string String, int Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string operator+(std::string String, long Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string operator+(std::string String, ulong Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string& operator+=(std::string& String, short Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string& operator+=(std::string& String, ushort Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string& operator+=(std::string& String, int Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string& operator+=(std::string& String, long Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string& operator+=(std::string& String, ulong Int) { return String.append(festring::IntegerToChar(Int)); }
inline std::string operator+(const char* CString, std::string CppString) { return CppString.insert(0, CString); }
template <class type> inline std::string& operator<<(std::string& String, const type& Type) { return String += Type; }

#endif

