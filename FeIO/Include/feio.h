#ifndef __FEIO_H__
#define __FEIO_H__

#include <string>

#include "typedef.h"
#include "vector2d.h"

class bitmap;

class iosystem
{
public:
	static std::string WhatToLoadMenu(bitmap*, bitmap*);
	static std::string StringQuestion(bitmap*, std::string, vector2d, ushort, ushort);
	static long NumberQuestion(bitmap*, std::string, vector2d);
	static int Menu(bitmap*, bitmap*, std::string);
	static void TextScreen(bitmap*, std::string, bool = true);
private:
	static unsigned int CountChars(char, std::string);
};

#endif
