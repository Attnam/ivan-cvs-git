#ifndef __FEIO_H__
#define __FEIO_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "vector2d.h"

class bitmap;
class colorizablebitmap;

class iosystem
{
 public:
  static std::string WhatToLoadMenu(ushort, ushort, const std::string&);
  static std::string StringQuestion(const std::string&, vector2d, ushort, ushort, ushort, bool, bool);
  static long NumberQuestion(const std::string&, vector2d, ushort, bool);
  static int Menu(bitmap*, const std::string&, const std::string&, ushort, ushort);
  static void TextScreen(const std::string&, ushort = 0xFFFF, bool = true, void (*)(bitmap*) = 0);
 private:
  static ulong CountChars(char, const std::string&);
};

#endif
