#ifndef __STROVER_H__
#define __STROVER_H__

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

#endif
