#ifndef __FEIO_H__
#define __FEIO_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"
#include "festring.h"

class bitmap;

typedef bool (*stringkeyhandler)(int, festring&);

class iosystem
{
 public:
  static festring ContinueMenu(color16, color16, const festring&);
  static int StringQuestion(festring&, const festring&, vector2d, color16, festring::sizetype, festring::sizetype, bool, bool, stringkeyhandler = 0);
  static long NumberQuestion(const festring&, vector2d, color16, bool);
  static long ScrollBarQuestion(const festring&, vector2d, long, long, long, long, long, color16, color16, color16, int, int, bool, void (*)(long) = 0);
  static int Menu(const bitmap*, vector2d, const festring&, const festring&, color16, const festring& = CONST_S(""), const festring& = CONST_S(""));
  static void TextScreen(const festring&, color16 = 0xFFFF, bool = true, void (*)(bitmap*) = 0);
  static bool iosystem::IsAcceptableForStringQuestion(char);
};

#endif
