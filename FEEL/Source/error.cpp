#include <iostream>
#include <new.h>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

#ifdef __DJGPP__
#include <conio.h>
#include <csignal>
#endif

#include "error.h"

#ifdef VC
int (*globalerrorhandler::OldNewHandler)(size_t) = 0;
#else
void (*globalerrorhandler::OldNewHandler)() = 0;
#endif

#ifdef __DJGPP__
void (*globalerrorhandler::OldSignal[SIGNALS])(int)	= {0, 0, 0, 0, 0, 0, 0, 0};
int    globalerrorhandler::Signal[SIGNALS]		= {SIGABRT, SIGFPE, SIGILL, SIGSEGV, SIGTERM, SIGINT, SIGKILL, SIGQUIT};
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

#ifdef __DJGPP__
		for(ushort c = 0; c < SIGNALS; c++)
			OldSignal[c] = signal(Signal[c], SignalHandler);
#endif

		atexit(globalerrorhandler::DeInstall);
	}
}

void globalerrorhandler::DeInstall()
{
#ifdef __DJGPP__
	for(ushort c = 0; c < SIGNALS; c++)
		signal(Signal[c], OldSignal[c]);
#endif

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
#endif
#ifdef SDL
	std::cout << Buffer << std::endl;
#endif
#ifdef __DJGPP__
	textmode(0x3);
	std::cout << Buffer << std::endl;
#endif

	exit(4);
}

#ifdef VC
int globalerrorhandler::NewHandler(size_t)
#else
void globalerrorhandler::NewHandler()
#endif
{
#ifdef WIN32
	ShowWindow(*hWnd, SW_HIDE);
	MessageBox(NULL, "Fatal Error: Memory depleted. Check that you have enough free RAM and hard disk space.", "Program aborted!", MB_OK|MB_ICONEXCLAMATION);	
#endif
#ifdef SDL
	std::cout << "Fatal Error: Memory depleted. Check that you have enough free RAM and hard disk space." << std::endl;
#endif
#ifdef __DJGPP__
	textmode(0x3);
	std::cout << "Fatal Error: Memory depleted. Check that you have enough free RAM and hard disk space." << std::endl;
#endif

	exit(1);

#ifdef VC
	return 0;
#endif
}

#ifdef __DJGPP__

void globalerrorhandler::SignalHandler(int Signal)
{
	static bool AlreadySignalled = false;

	if(!AlreadySignalled)
	{
		AlreadySignalled = true;

		textmode(0x3);

		std::cout << "Fatal Error: ";

		switch (Signal)
		{
		case SIGABRT:
			std::cout << "Abort";
			break;

		case SIGFPE:
			std::cout << "Divide by zero";
			break;

		case SIGILL:
			std::cout << "Invalid/unknown";
			break;

		case SIGSEGV:
			std::cout << "Segmentation violation";
			break;

		case SIGTERM:
			std::cout << "Termination request";
			break;

		case SIGINT:
			std::cout << "Break interrupt";
			break;

		case SIGKILL:
			std::cout << "Kill";
			break;

		case SIGQUIT:
			std::cout << "Quit";
			break;

		default:
			std::cout << "Unknown";
		}

		std::cout << " exception signalled." << std::endl;
	}

	exit(2);
}

#endif
