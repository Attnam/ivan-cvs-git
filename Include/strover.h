#ifndef __STROVER_H__
#define __STROVER_H__

#include <cstdio>
#include <fstream>
#include <string>

#include "typedef.h"

inline std::string operator+ (std::string String, const int& Int)
{
	char Buffer[12];

	sprintf(Buffer, "%d", Int);

	String += Buffer;

	return String;
}

inline std::string& operator+= (std::string& String, const int& Int)
{
	char Buffer[12];

	sprintf(Buffer, "%d", Int);

	String += Buffer;

	return String;
}

inline std::ofstream& operator<<(std::ofstream& SaveFile, std::string String)
{
	uchar Length = String.length();

	SaveFile.put(Length);

	if(Length)
		SaveFile.write(String.c_str(), Length);

	return SaveFile;
}

inline std::ifstream& operator>>(std::ifstream& SaveFile, std::string& String)
{
	char Buffer[256];

	uchar Length;

	Length = SaveFile.get();

	if(Length)
	{
		SaveFile.read(Buffer, Length);
		Buffer[Length] = 0;
		String = Buffer;
	}
	else
		String = "";

	return SaveFile;
}

#endif

