#ifndef __ERROR_H__
#define __ERROR_H__

#include <windows.h>

#include "typedef.h"

class globalerrorhandler
{
public:
	static void Install(void);
	static void DeInstall(void);
	static void Abort(const char*, ...);
	static void SetWindow(HWND* NewhWnd) { hWnd = NewhWnd; }
private:
	static int NewHandler(size_t);
	static int (*OldNewHandler)(size_t);
	static HWND* hWnd;
};

#define ABORT globalerrorhandler::Abort

#endif
