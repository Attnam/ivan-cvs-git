#include "whandler.h"

#ifdef __DJGPP__

#include <unistd.h>

ulong globalwindowhandler::Tick = 0;
void (*globalwindowhandler::ControlLoop)() = 0;

int globalwindowhandler::GetKey(bool EmptyBuffer, bool)
{
  if(EmptyBuffer)
    while(kbhit()) getkey();

  while(!kbhit())
    if(ControlLoop)
      {
	ControlLoop();
	++Tick;
	usleep(50000);
      }

  return getkey();			
}

int globalwindowhandler::ReadKey()
{
  if(kbhit())
    return getkey();
  else
    return 0;
}

#else

#include "graphics.h"
#include "error.h"
#include "bitmap.h"

dynarray<int> globalwindowhandler::KeyBuffer;
bool globalwindowhandler::Initialized = false;
bool (*globalwindowhandler::QuitMessageHandler)() = 0;
void (*globalwindowhandler::ControlLoop)() = 0;
ulong globalwindowhandler::Tick = 0;

#ifdef WIN32
char globalwindowhandler::KeyboardLayoutName[KL_NAMELENGTH];
bool globalwindowhandler::Active = false;

#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include "ddutil.h"

LRESULT CALLBACK globalwindowhandler::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
    {
    case WM_SIZE:
      if(wParam == SIZE_MAXHIDE || wParam == SIZE_MINIMIZED)
	Active = false;
      else
	{
	  Active = true;

	  if(wParam == SIZE_MAXIMIZED)
	    {
	      graphics::SwitchMode();
	      break;
	    }
	}

    case WM_MOVE:
      {
	graphics::UpdateBounds();

	if(Initialized && Active)
	  graphics::BlitDBToScreen();

	break;
      }

    case WM_PAINT:
      {
	if(Initialized && Active)
	  graphics::BlitDBToScreen();

	break;
      }

    case WM_SYSCOMMAND:
      {
	switch (wParam)
	  {
	  case SC_SCREENSAVE:
	  case SC_MONITORPOWER:
	    return 0;
	  }

	break;
      }

    case WM_CLOSE:
      {
	if(!QuitMessageHandler || QuitMessageHandler())
	  PostQuitMessage(0);

	return 0;
      }

    case WM_KEYDOWN:
      {
	int Key = 0;

	if(wParam == VK_LEFT || wParam == VK_NUMPAD4) Key = 0x14B;
	if(wParam == VK_HOME || wParam == VK_NUMPAD7) Key = 0x147;
	if(wParam == VK_UP || wParam == VK_NUMPAD8) Key = 0x148;
	if(wParam == VK_PRIOR || wParam == VK_NUMPAD9) Key = 0x149;	// page up! Believe it, or not!
	if(wParam == VK_RIGHT || wParam == VK_NUMPAD6) Key = 0x14D;
	if(wParam == VK_NEXT || wParam == VK_NUMPAD3) Key = 0x151;	// page down! Believe it, or not!
	if(wParam == VK_DOWN || wParam == VK_NUMPAD2) Key = 0x150;
	if(wParam == VK_END || wParam == VK_NUMPAD1) Key = 0x14F;
	if(wParam == VK_NUMPAD5) Key = '.';

	if(Key)
	  {
	    ushort Index = KeyBuffer.Search(Key);

	    if(Index == 0xFFFF)
	      KeyBuffer.Add(Key);
	  }

	return 0;
      }

    case WM_CHAR:
      {
	ushort Index = KeyBuffer.Search(wParam);

	if(Index == 0xFFFF)
	  KeyBuffer.Add(wParam);

	return 0;
      }

    case WM_KEYUP:
      {
	if(wParam == VK_SNAPSHOT)
	  DOUBLEBUFFER->Save("Scrshot.bmp");

	return 0;
      }

    case WM_SYSKEYUP:
      if(wParam == VK_RETURN)
	{
	  graphics::SwitchMode();
	  return 0;
	}
				
    }

  return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int globalwindowhandler::GetKey(bool EmptyBuffer)
{
  MSG msg;

  if(EmptyBuffer)
    {
      CheckMessages();

      while(KeyBuffer.Length())
	KeyBuffer.Remove(0);
    }

  while(true)
    if(Active && KeyBuffer.Length())
      return KeyBuffer.Remove(0);
    else
      if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	if(msg.message == WM_QUIT)
	  exit(0);
	else
	  {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	  }
      else
	{
	  if(ControlLoop)
	    {
	      ControlLoop();
	      ++Tick;
	      Sleep(50);
	    }
	  else
	    WaitMessage();
	}
}

void globalwindowhandler::Init(HINSTANCE hInst, HWND* phWnd, const char* Title, LPCTSTR IconName)
{
  WNDCLASS wc;
  HWND hWnd;
  wc.lpszClassName = Title;
  wc.lpfnWndProc   = (WNDPROC) globalwindowhandler::WndProc;
  wc.style         = CS_OWNDC;
  wc.hInstance     = hInst;
  wc.hIcon         = LoadIcon(hInst, IconName);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
  wc.lpszMenuName  = NULL;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;

  if(!RegisterClass(&wc))
    ABORT("No Window register.");

  hWnd = CreateWindowEx(0, Title, Title, WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInst, 0);

  if(!hWnd)
    ABORT("No Window.");

  ShowWindow(hWnd, SW_SHOW);
  SetFocus(hWnd);
  UpdateWindow(hWnd);
  *phWnd = hWnd;
  GetKeyboardLayoutName(KeyboardLayoutName);
}

int globalwindowhandler::ReadKey()
{
  while(true)
    {
      CheckMessages();

      if(Active) // Active true if the window is not iconified
	break;
      else
	WaitMessage();
    }

  if(KeyBuffer.Length())
    return GetKey(false);
  else
    return 0;
}

void globalwindowhandler::CheckMessages()
{
  MSG msg;

  while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    if(msg.message == WM_QUIT)
      exit(0);
    else
      {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
      }
}

#endif

#ifdef USE_SDL

void globalwindowhandler::Init(const char* Title)
{
  SDL_WM_SetCaption(Title, 0);
  SDL_EnableUNICODE(1);
}

int globalwindowhandler::GetKey(bool EmptyBuffer)
{
  SDL_Event event;
  if(EmptyBuffer)
    {
      while(SDL_PollEvent(&event))
	ProcessMessage(event);	     

      while(KeyBuffer.Length())
	KeyBuffer.Remove(0);
    }

  while(true)
    if(KeyBuffer.Length())
      {
	int Key =  KeyBuffer.Remove(0);

	if(Key > 0xE000) 
	  return Key - 0xE000;

	if(Key != 0 && Key < 0x81)
	  return Key;
      }
    else
      {
	if(SDL_PollEvent(&event))
	  ProcessMessage(event);
	else
	  {
	    if(ControlLoop)
	      {
		ControlLoop();
		++Tick;
		SDL_Delay(50);
	      }
	    else
	      {
		SDL_WaitEvent(&event);
		ProcessMessage(event);
	      }
	  }
      }
}

int globalwindowhandler::ReadKey()
{
  SDL_Event event;

  if(!(SDL_GetAppState() & SDL_APPACTIVE))
    {
      SDL_WaitEvent(&event);
      ProcessMessage(event);
    }
  else
    {
      while(SDL_PollEvent(&event))
	ProcessMessage(event);
    }

  if(KeyBuffer.Length())
    return GetKey(false);
  else
    return 0;
}

void globalwindowhandler::ProcessMessage(SDL_Event event)
{
  ushort Index, KeyPressed;
 
  switch(event.type)
    {
    case SDL_QUIT:
      if(!QuitMessageHandler || QuitMessageHandler())
	exit(0);	
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
	{
	case SDLK_RETURN:
	  if(event.key.keysym.mod & KMOD_ALT)
	     graphics::ToggleFullScreen();
	  else
	     KeyPressed = event.key.keysym.unicode;
	  break;
	case SDLK_DOWN:
	case SDLK_KP2:
	  KeyPressed = 0x150 + 0xE000;
	  break;
	case SDLK_UP:
	case SDLK_KP8:
	  KeyPressed = 0x148 + 0xE000;
	  break;
	case SDLK_RIGHT:
	case SDLK_KP6:
	  KeyPressed = 0x14D + 0xE000;
	  break;
	case SDLK_LEFT:
	case SDLK_KP4:
	  KeyPressed = 0x14B + 0xE000;
	  break;
	case SDLK_HOME:
	case SDLK_KP7:
	  KeyPressed = 0x147 + 0xE000;
	  break;
	case SDLK_END:
	case SDLK_KP1:
	  KeyPressed = 0x14F + 0xE000;
	  break;
	case SDLK_PAGEUP:
	case SDLK_KP9:
	  KeyPressed = 0x149 + 0xE000;
	  break;
	case SDLK_KP3:
	case SDLK_PAGEDOWN:
	  KeyPressed = 0x151 + 0xE000;
	  break;
	case SDLK_PRINT:
	  DOUBLEBUFFER->Save(std::string(getenv("HOME")) + "/Scrshot.bmp");
	  return;
	default:
	  KeyPressed = event.key.keysym.unicode;
	}

      Index = KeyBuffer.Search(KeyPressed);

      if(Index == 0xFFFF)
	KeyBuffer.Add(KeyPressed);	  
      break;

    case SDL_KEYUP:
      break;

    default:
      break;
    }
}

#endif

#endif
