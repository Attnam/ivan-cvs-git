#ifndef __ERROR_H__
#define __ERROR_H__

#define ABORT globalerrorhandler::Abort

#include <windows.h>

#include "typedef.h"

class globalerrorhandler
{
public:
	static void Install();
	static void DeInstall();
	static void Abort(const char*, ...);
	static void SetWindow(HWND* NewhWnd) { hWnd = NewhWnd; }
private:
	static void NewHandler();
	static void (*OldNewHandler)();
	static HWND* hWnd;
};

#endif
