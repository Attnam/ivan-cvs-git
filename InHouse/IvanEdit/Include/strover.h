#ifndef __STROVER_H__
#define __STROVER_H__

#include <fstream>
#include <cstdio>
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

inline ofstream& operator<<(ofstream& SaveFile, std::string String)
{
	uchar Length = String.length();

	SaveFile.put(Length);

	if(Length)
		SaveFile.write(String.c_str(), Length);

	return SaveFile;
}

inline ifstream& operator>>(ifstream& SaveFile, std::string& String)
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

inline std::string operator+ (std::string String, const char& Int)
{
        char Buffer[2];
	sprintf(Buffer, "%c", Int);

	String += Buffer;

	return String;
}

#endif

