#ifndef __FESTRING_H__
#define __FESTRING_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

namespace festring
{
  extern char Buffer[];

  inline std::string& Capitalize(std::string& String)
  {
    if(String[0] > 0x60 && String[0] < 0x7B)
      String[0] &= ~0x20;

    return String;
  }

  inline std::string CapitalizeCopy(std::string String) { return Capitalize(String); }
}

inline std::string operator+(std::string String, short Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string operator+(std::string String, ushort Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string operator+(std::string String, int Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string operator+(std::string String, long Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string operator+(std::string String, ulong Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string& operator+=(std::string& String, short Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string& operator+=(std::string& String, ushort Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string& operator+=(std::string& String, int Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string& operator+=(std::string& String, long Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string& operator+=(std::string& String, ulong Int) { return String.append(itoa(Int, festring::Buffer, 10)); }
inline std::string operator+(const char* const CString, std::string CppString) { return CppString.insert(0, CString); }
template <class type> inline std::string& operator<<(std::string& String, const type& Type) { return String += Type; }

#endif
