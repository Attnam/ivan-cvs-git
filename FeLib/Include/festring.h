#ifndef __FESTRING_H__
#define __FESTRING_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "typedef.h"

typedef std::string::size_type strsize;

/* Class for containing various std::string handling functions */

class festring
{
public:
  static void InstallIntegerMap();
  static void DeInstallIntegerMap();
  static char* IntegerToChar(long);
  static void SplitString(std::string&, std::string&, strsize);
  static void SplitString(const std::string&, std::vector<std::string>&, strsize, strsize = 0);
  static char& Capitalize(char& Char) { return Char > 0x60 && Char < 0x7B ? Char ^= 0x20 : Char; }
  static char CapitalizeCopy(char Char) { return Capitalize(Char); }
  static std::string& Capitalize(std::string& String) { Capitalize(String[0]); return String; }
  static std::string CapitalizeCopy(std::string String) { Capitalize(String[0]); return String; }
  static strsize IgnoreCaseFind(const std::string&, const std::string&, strsize = 0);
  static void SearchAndReplace(std::string&, const std::string&, const std::string&, strsize = 0);
private:
  static char** IntegerMap;
  static char IntegerBuffer[12];
};

/* Replaces all occurances of What in Where after Begin with With */

inline void festring::SearchAndReplace(std::string& Where, const std::string& What, const std::string& With, strsize Begin)
{
  for(strsize Pos = Where.find(What, Begin); Pos != std::string::npos; Pos = Where.find(What, Pos))
    {
      Where.erase(Pos, What.length());
      Where.insert(Pos, With);
    }
}

/* This macro doesn't evaluate with if what is not found so it's often faster */

#define SEARCH_N_REPLACE(what, with) if(Msg.find(what, 0) != std::string::npos) festring::SearchAndReplace(Msg, what, with);

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
