#ifndef __FEIO_H__
#define __FEIO_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"
#include "festring.h"

class bitmap;

class iosystem
{
 public:
  static festring ContinueMenu(ushort, ushort, const festring&);
  static festring StringQuestion(const festring&, vector2d, ushort, ushort, ushort, bool, bool);
  static long NumberQuestion(const festring&, vector2d, ushort, bool);
  static long ScrollBarQuestion(const festring&, vector2d, long, long, long, long, long, ushort, ushort, ushort, ushort, ushort, bool, void (*)(long) = 0);
  static int Menu(bitmap*, vector2d, const festring&, const festring&, ushort, const festring& = CONST_S(""), const festring& = CONST_S(""));
  static void TextScreen(const festring&, ushort = 0xFFFF, bool = true, void (*)(bitmap*) = 0);
 private:
  static ulong CountChars(char, const festring&);
};

#endif
