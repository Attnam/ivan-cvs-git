#include "whandler.h"
#include "graphics.h"
#include "error.h"

bool (*globalwindowhandler::ControlLoop[MAX_CONTROLS])();
ushort globalwindowhandler::Controls = 0;
ulong globalwindowhandler::Tick = 0;

void globalwindowhandler::InstallControlLoop(bool (*What)())
{
  if(Controls == MAX_CONTROLS)
    ABORT("Animation control frenzy!");

  ControlLoop[Controls++] = What;
}

void globalwindowhandler::DeInstallControlLoop(bool (*What)())
{
  ushort c;

  for(c = 0; c < Controls; ++c)
    if(ControlLoop[c] == What)
      break;

  if(c != Controls)
    {
      --Controls;

      for(; c < Controls; ++c)
	ControlLoop[c] = ControlLoop[c + 1];
    }
}

#ifdef __DJGPP__

#include <pc.h>

int globalwindowhandler::GetKey(bool EmptyBuffer)
{
  if(EmptyBuffer)
    while(kbhit())
      getkey();

  while(!kbhit())
    if(Controls)
      {
	static ulong LastTick = 0;
	UpdateTick();

	if(LastTick != Tick)
	  {
	    LastTick = Tick;
	    bool Draw = false;

	    for(ushort c = 0; c < Controls; ++c)
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

#endif

#ifdef USE_SDL

#include <algorithm>

#include "bitmap.h"

std::vector<int> globalwindowhandler::KeyBuffer;
bool (*globalwindowhandler::QuitMessageHandler)() = 0;

void globalwindowhandler::Init()
{
  SDL_EnableUNICODE(1);
  SDL_EnableKeyRepeat(500, 30);
}

int globalwindowhandler::GetKey(bool EmptyBuffer)
{
  SDL_Event Event;

  if(EmptyBuffer)
    {
      while(SDL_PollEvent(&Event))
	ProcessMessage(&Event);	     

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
	if(SDL_PollEvent(&Event))
	  ProcessMessage(&Event);
	else
	  {
	    if(SDL_GetAppState() & SDL_APPACTIVE && Controls)
	      {
		static ulong LastTick = 0;
		UpdateTick();

		if(LastTick != Tick)
		  {
		    LastTick = Tick;
		    bool Draw = false;

		    for(ushort c = 0; c < Controls; ++c)
		      if(ControlLoop[c]())
			Draw = true;

		    if(Draw)
		      graphics::BlitDBToScreen();
		  }

		SDL_Delay(10);
	      }
	    else
	      {
		SDL_WaitEvent(&Event);
		ProcessMessage(&Event);
	      }
	  }
      }
}

int globalwindowhandler::ReadKey()
{
  SDL_Event Event;

  if(SDL_GetAppState() & SDL_APPACTIVE)
    {
      ProcessMessage(&Event);

      while(SDL_PollEvent(&Event))
	ProcessMessage(&Event);
    }
  else
    {
      SDL_WaitEvent(&Event);
      ProcessMessage(&Event);
    }

  if(KeyBuffer.size())
    return GetKey(false);
  else
    return 0;
}

void globalwindowhandler::ProcessMessage(SDL_Event* Event)
{
  ushort KeyPressed;
 
  switch(Event->active.type)
    {
    case SDL_VIDEOEXPOSE:
      graphics::BlitDBToScreen();
      break;
    case SDL_QUIT:
      if(!QuitMessageHandler || QuitMessageHandler())
	exit(0);	
      return;
    case SDL_KEYDOWN:
      switch(Event->key.keysym.sym)
	{
	case SDLK_RETURN:
	  if(Event->key.keysym.mod & KMOD_ALT)
	    {
	      graphics::SwitchMode();
	      return;
	    }
	  else
	    KeyPressed = Event->key.keysym.unicode;
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
	case SDLK_KP5:
	  KeyPressed = KEY_NUMPAD_5;
	  break;
	case SDLK_PRINT:
	  DOUBLE_BUFFER->Save(std::string(getenv("HOME")) + "/Scrshot.bmp");
	  return;

	default:
	  KeyPressed = Event->key.keysym.unicode;

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
