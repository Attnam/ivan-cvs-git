#ifndef __STROVER_H__

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

inline std::ofstream& operator+= (std::ofstream& File, const std::string& String)
{
	ulong Length = String.length();

	File.write((char*)&Length, sizeof(Length));

	if(Length)
		File.write(String.c_str(), Length);

	return File;
}

inline std::string& operator-= (std::ifstream& File, std::string& String)
{
	ulong Length;

	File.read((char*)&Length, sizeof(Length));

	if(Length)
	{
		char* Buffer = new char[Length + 1];
		File.read((char*)Buffer, Length);
		Buffer[Length] = 0;
		String = Buffer;
		delete [] Buffer;
	}
	else
		String = "";

	return String;
}

#endif


