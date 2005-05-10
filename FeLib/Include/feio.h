/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

#ifndef __FEIO_H__
#define __FEIO_H__

#include "v2.h"
#include "festring.h"

class bitmap;

typedef truth (*stringkeyhandler)(int, festring&);

class iosystem
{
 public:
  static festring ContinueMenu(col16, col16, cfestring&);
  static int StringQuestion(festring&, cfestring&, v2, col16,
			    festring::sizetype, festring::sizetype,
			    truth, truth, stringkeyhandler = 0);
  static long NumberQuestion(cfestring&, v2, col16,
			     truth, truth = false);
  static long ScrollBarQuestion(cfestring&, v2, long, long, long,
				long, long, col16, col16, col16, int,
				int, truth, void (*)(long) = 0);
  static int Menu(cbitmap*, v2, cfestring&,
		  cfestring&, col16,
		  cfestring& = CONST_S(""),
		  cfestring& = CONST_S(""));
  static void TextScreen(cfestring&, col16 = 0xFFFF,
			 truth = true, void (*)(bitmap*) = 0);
  static truth IsAcceptableForStringQuestion(char);
};

#endif
