/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __ERROR_H__
#define __ERROR_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

#define ABORT globalerrorhandler::Abort

#ifdef __DJGPP__
#define SIGNALS 8
#endif

class globalerrorhandler
{
 public:
  static void Install();
  static void DeInstall();
  static void Abort(const char*, ...);
  static const char* GetBugMsg() { return BugMsg; }
 private:
  static const char* BugMsg;
#ifdef VC
  static int NewHandler(size_t);
  static int (*OldNewHandler)(size_t);
#else
  static void NewHandler();
  static void (*OldNewHandler)();
#endif
#ifdef __DJGPP__
  static void SignalHandler(int);
  static void (*OldSignal[SIGNALS])(int);
  static int Signal[SIGNALS];
#endif
};

#endif
