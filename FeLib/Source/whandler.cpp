#include <ctime>

#include "whandler.h"
#include "graphics.h"

controlvector globalwindowhandler::ControlLoop;
ulong globalwindowhandler::Tick = 0;

void globalwindowhandler::InstallControlLoop(bool (*What)())
{
  ControlLoop.push_back(What);
}

void globalwindowhandler::DeInstallControlLoop(bool (*What)())
{
  for(controlvector::iterator i = ControlLoop.begin(); i != ControlLoop.end(); ++i)
    if(*i == What)
      {
	ControlLoop.erase(i);
	return;
      }
}

ulong globalwindowhandler::UpdateTick()
{
#ifdef USE_SDL
  Tick = SDL_GetTicks() / 40;
#else
  Tick = clock() * 25 / CLOCKS_PER_SEC;
#endif
  return Tick;
}

#ifdef __DJGPP__

int globalwindowhandler::GetKey(bool EmptyBuffer, bool)
{
  if(EmptyBuffer)
    while(kbhit()) getkey();

  while(!kbhit())
    if(ControlLoop.size())
      {
	static ulong LastTick = 0;
	UpdateTick();

	if(LastTick != Tick)
	  {
	    LastTick = Tick;
	    bool Draw = false;

	    for(ushort c = 0; c < ControlLoop.size(); ++c)
	      if(ControlLoop[c]())
		Draw = true;

	    if(Draw)
	      graphics::BlitDBToScreen();
	  }
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

#include <algorithm>

#include "error.h"
#include "bitmap.h"

std::vector<int> globalwindowhandler::KeyBuffer;
bool globalwindowhandler::Initialized = false;
bool (*globalwindowhandler::QuitMessageHandler)() = 0;

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

	if(wParam == VK_LEFT || wParam == VK_NUMPAD4) Key = KEY_LEFT;
	if(wParam == VK_HOME || wParam == VK_NUMPAD7) Key = KEY_HOME;
	if(wParam == VK_UP || wParam == VK_NUMPAD8) Key = KEY_UP;
	if(wParam == VK_PRIOR || wParam == VK_NUMPAD9) Key = KEY_PAGE_UP;
	if(wParam == VK_RIGHT || wParam == VK_NUMPAD6) Key = KEY_RIGHT;
	if(wParam == VK_NEXT || wParam == VK_NUMPAD3) Key = KEY_PAGE_DOWN;
	if(wParam == VK_DOWN || wParam == VK_NUMPAD2) Key = KEY_DOWN;
	if(wParam == VK_END || wParam == VK_NUMPAD1) Key = KEY_END;
	if(wParam == VK_NUMPAD5) Key = '.';

	if(Key && std::find(KeyBuffer.begin(), KeyBuffer.end(), Key) == KeyBuffer.end())
	  KeyBuffer.push_back(Key);

	return 0;
      }

    case WM_CHAR:
      {
	if(std::find(KeyBuffer.begin(), KeyBuffer.end(), int(wParam)) == KeyBuffer.end())
	  KeyBuffer.push_back(wParam);

	return 0;
      }

    case WM_KEYUP:
      {
	if(wParam == VK_SNAPSHOT)
	  DOUBLE_BUFFER->Save("Scrshot.bmp");

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
      KeyBuffer.clear();
    }

  while(true)
    if(Active && KeyBuffer.size())
      {
	int Key = KeyBuffer[0];
	KeyBuffer.erase(KeyBuffer.begin());
	return Key;
      }
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
	  if(Active && ControlLoop.size())
	    {
	      static ulong LastTick = 0;
	      UpdateTick();

	      if(LastTick != Tick)
		{
		  LastTick = Tick;
		  bool Draw = false;

		  for(ushort c = 0; c < ControlLoop.size(); ++c)
		    if(ControlLoop[c]())
		      Draw = true;

		  if(Draw)
		    graphics::BlitDBToScreen();
		}

	      Sleep(30);
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

  if(KeyBuffer.size())
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
  SDL_EnableKeyRepeat(500, 30);
}

int globalwindowhandler::GetKey(bool EmptyBuffer)
{
  SDL_Event event;
  if(EmptyBuffer)
    {
      while(SDL_PollEvent(&event))
	ProcessMessage(&event);	     

      KeyBuffer.clear();
    }

  while(true)
    if(!KeyBuffer.empty())
      {
	int Key = KeyBuffer[0];
	KeyBuffer.erase(KeyBuffer.begin());

	if(Key > 0xE000) 
	  return Key - 0xE000;

	if(Key != 0 && Key < 0x81)
	  return Key;
      }
    else
      {
	if(SDL_PollEvent(&event))
	  ProcessMessage(&event);
	else
	  {
	    if((SDL_GetAppState() & SDL_APPACTIVE)  && ControlLoop.size())
	      {
		static ulong LastTick = 0;
		UpdateTick();

		if(LastTick != Tick)
		  {
		    LastTick = Tick;
		    bool Draw = false;

		    for(ushort c = 0; c < ControlLoop.size(); ++c)
		      if(ControlLoop[c]())
			Draw = true;

		    if(Draw)
		      graphics::BlitDBToScreen();
		  }
		SDL_Delay(10);
	      }
	    else
	      {
		SDL_WaitEvent(&event);
		ProcessMessage(&event);
	      }
	  }
      }
}

int globalwindowhandler::ReadKey()
{
  SDL_Event event;

  if(SDL_GetAppState() & SDL_APPACTIVE)
    {
      ProcessMessage(&event);
      while(SDL_PollEvent(&event))
	{
	  ProcessMessage(&event);
	}
    }
  else
    {
      SDL_WaitEvent(&event);
      ProcessMessage(&event);
    }
  if(KeyBuffer.size())
    return GetKey(false);
  else
    return 0;
}

void globalwindowhandler::ProcessMessage(SDL_Event* event)
{
  ushort Index, KeyPressed;
 
  switch(event->active.type)
    {
    case SDL_QUIT:
      if(!QuitMessageHandler || QuitMessageHandler())
	exit(0);	
      return;
    case SDL_KEYDOWN:
      switch(event->key.keysym.sym)
	{
	case SDLK_RETURN:
	  if(event->key.keysym.mod & KMOD_ALT)
	     graphics::SwitchMode();
	  else
	     KeyPressed = event->key.keysym.unicode;
	  break;

	case SDLK_DOWN:
	case SDLK_KP2:
	  KeyPressed = KEY_DOWN + 0xE000;
	  break;
	case SDLK_UP:
	case SDLK_KP8:
	  KeyPressed = KEY_UP + 0xE000;
	  break;
	case SDLK_RIGHT:
	case SDLK_KP6:
	  KeyPressed = KEY_RIGHT + 0xE000;
	  break;
	case SDLK_LEFT:
	case SDLK_KP4:
	  KeyPressed = KEY_LEFT + 0xE000;
	  break;
	case SDLK_HOME:
	case SDLK_KP7:
	  KeyPressed = KEY_HOME + 0xE000;
	  break;
	case SDLK_END:
	case SDLK_KP1:
	  KeyPressed = KEY_END + 0xE000;
	  break;
	case SDLK_PAGEUP:
	case SDLK_KP9:
	  KeyPressed = KEY_PAGE_UP + 0xE000;
	  break;
	case SDLK_KP3:
	case SDLK_PAGEDOWN:
	  KeyPressed = KEY_PAGE_DOWN + 0xE000;
	  break;
	case SDLK_PRINT:
	  DOUBLE_BUFFER->Save(std::string(getenv("HOME")) + "/Scrshot.bmp");
	  return;
	default:

	  KeyPressed = event->key.keysym.unicode;
	  if(KeyPressed == 0)
	    return;
	}

      if(std::find(KeyBuffer.begin(), KeyBuffer.end(), KeyPressed) == KeyBuffer.end())
	KeyBuffer.push_back(KeyPressed);
	  
      break;

    case SDL_KEYUP:
      break;

    default:
      break;
    }
}

#endif

#endif
