#ifndef __STROVER_H__
#define __STROVER_H__

#pragma warning(disable : 4786)

#include <cstdio>
#include <string>

#include "typedef.h"

inline std::string operator+ (std::string CppString, const int& Int)
{
	char Buffer[12];

	sprintf(Buffer, "%d", Int);

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

inline std::string operator+ (const char* CString, std::string CppString)
{
	return std::string(CString) + CppString;
}

inline std::string& operator+ (std::string& CppString, const unsigned long& Int)
{
	char Buffer[12];

	sprintf(Buffer, "%d", Int);

	CppString += Buffer;

	return CppString;
}

#endif
