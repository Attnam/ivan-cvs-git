#include <iostream>
#include <new.h>
#include <cstdlib>
#include <cstdio>

#include "error.h"

int (*globalerrorhandler::OldNewHandler)(size_t) = 0;
HWND* globalerrorhandler::hWnd;

void globalerrorhandler::Install()
{
	static bool AlreadyInstalled = false;

	if(!AlreadyInstalled)
	{
		AlreadyInstalled = true;

		OldNewHandler = _set_new_handler(NewHandler);

		atexit(globalerrorhandler::DeInstall);
	}
}

void globalerrorhandler::DeInstall()
{
	_set_new_handler(OldNewHandler);
}

void globalerrorhandler::Abort(const char* Format, ...)
{
	char Buffer[256];

	va_list AP;
	va_start(AP, Format);
	vsprintf(Buffer, Format, AP);
	va_end(AP);

	ShowWindow(*hWnd, SW_HIDE);

	MessageBox(NULL, Buffer, "Program aborted!", MB_OK|MB_ICONEXCLAMATION);

	exit(4);
}

int globalerrorhandler::NewHandler(size_t)
{
	ShowWindow(*hWnd, SW_HIDE);

	MessageBox(NULL, "Fatal Error: Memory depleted. Check that you have enough free RAM and hard disk space.", "Program aborted!", MB_OK|MB_ICONEXCLAMATION);

	exit(1);

	return 0;
}
