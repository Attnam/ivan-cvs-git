#ifndef __ERROR_H__
#define __ERROR_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define ABORT globalerrorhandler::Abort

#ifdef WIN32
#include <windows.h>
#endif

#include "typedef.h"

class globalerrorhandler
{
public:
	static void Install();
	static void DeInstall();
	static void Abort(const char*, ...);
#ifdef WIN32
	static void SetWindow(HWND* NewhWnd) { hWnd = NewhWnd; }
#endif
private:
#ifdef VC
	static int NewHandler(size_t);
	static int (*OldNewHandler)(size_t);
#else
	static void NewHandler(void);
	static void (*OldNewHandler)(void);
#endif
#ifdef WIN32
	static HWND* hWnd;
#endif
};

#endif



