#include <iostream>
#include <new.h>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

#ifdef __DJGPP__
#include <conio.h>
#include <csignal>
#include "graphics.h"
#endif

#include "error.h"

/* Shouldn't be initialized here! */

const char* globalerrorhandler::BugMsg = "\n\nPlease send bug report to ivan-users@sourceforge.net\n"
					 "including a brief description of what you did, what version\n"
					 "you are running and which kind of system you are using.";

#ifdef VC
int (*globalerrorhandler::OldNewHandler)(size_t) = 0;
#else
void (*globalerrorhandler::OldNewHandler)() = 0;
#endif

#ifdef __DJGPP__
void (*globalerrorhandler::OldSignal[SIGNALS])(int) = {0, 0, 0, 0, 0, 0, 0, 0};
int globalerrorhandler::Signal[SIGNALS] = {SIGABRT, SIGFPE, SIGILL, SIGSEGV, SIGTERM, SIGINT, SIGKILL, SIGQUIT};
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
      for(ushort c = 0; c < SIGNALS; ++c)
	OldSignal[c] = signal(Signal[c], SignalHandler);
#endif

      atexit(globalerrorhandler::DeInstall);
    }
}

void globalerrorhandler::DeInstall()
{
#ifdef __DJGPP__
  for(ushort c = 0; c < SIGNALS; ++c)
    signal(Signal[c], OldSignal[c]);
#endif

  set_new_handler(OldNewHandler);
}

void globalerrorhandler::Abort(const char* Format, ...)
{
  char Buffer[512];

  va_list AP;
  va_start(AP, Format);
  vsprintf(Buffer, Format, AP);
  va_end(AP);

  strcat(Buffer, BugMsg);

#ifdef WIN32
  ShowWindow(*hWnd, SW_HIDE);
  MessageBox(NULL, Buffer, "Program aborted!", MB_OK|MB_ICONEXCLAMATION);
#endif
#ifdef USE_SDL
  std::cout << Buffer << std::endl;
#endif
#ifdef __DJGPP__
  graphics::DeInit();
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
#ifdef USE_SDL
  std::cout << "Fatal Error: Memory depleted. Check that you have enough free RAM and hard disk space." << std::endl;
#endif
#ifdef __DJGPP__
  graphics::DeInit();
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
      graphics::DeInit();
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

      std::cout << " exception signalled.";

      if(Signal != SIGINT)
	std::cout << BugMsg;

      std::cout << std::endl;
    }

  exit(2);
}

#endif
