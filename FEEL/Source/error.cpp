#include <iostream>
#include <new.h>
#include <cstdlib>
#include <cstdio>

#include "error.h"

#ifdef VC
int (*globalerrorhandler::OldNewHandler)(size_t) = 0;
#else
void (*globalerrorhandler::OldNewHandler)(void) = 0;
#endif

#ifdef WIN32
HWND* globalerrorhandler::hWnd;
#endif

#ifdef VC
#define set_new_handler _set_new_handler
#endif

void globalerrorhandler::Install()
{
	static bool AlreadyInstalled = false;

	if(!AlreadyInstalled)
	{
		AlreadyInstalled = true;

		OldNewHandler = set_new_handler(NewHandler);

		atexit(globalerrorhandler::DeInstall);
	}
}

void globalerrorhandler::DeInstall()
{
	set_new_handler(OldNewHandler);
}

void globalerrorhandler::Abort(const char* Format, ...)
{
	char Buffer[256];

	va_list AP;
	va_start(AP, Format);
	vsprintf(Buffer, Format, AP);
	va_end(AP);

#ifdef WIN32
	ShowWindow(*hWnd, SW_HIDE);
	MessageBox(NULL, Buffer, "Program aborted!", MB_OK|MB_ICONEXCLAMATION);
#else
	cout << Buffer << endl;
#endif

	exit(4);
}

#ifdef VC
int globalerrorhandler::NewHandler(size_t)
#else
void globalerrorhandler::NewHandler(void)
#endif
{
#ifdef WIN32
	ShowWindow(*hWnd, SW_HIDE);
	MessageBox(NULL, "Fatal Error: Memory depleted. Check that you have enough free RAM and hard disk space.", "Program aborted!", MB_OK|MB_ICONEXCLAMATION);
#else
	cout << "Fatal Error: Memory depleted. Check that you have enough free RAM and hard disk space." << endl;
#endif

	exit(1);

#ifdef VC
	return 0;
#endif
}
