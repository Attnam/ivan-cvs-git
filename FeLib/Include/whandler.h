#ifndef __WHANDLER_H__
#define __WHANDLER_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#define GETKEY globalwindowhandler::GetKey
#define READKEY globalwindowhandler::ReadKey

typedef std::vector<bool (*)()> controlvector;

#ifdef __DJGPP__

#include <pc.h>

#include "typedef.h"

class globalwindowhandler
{
 public:
  static int GetKey(bool = true, bool = false);
  static int ReadKey();
  static ulong GetTick() { return Tick; }
  static void InstallControlLoop(bool (*)());
  static void DeInstallControlLoop(bool (*)());
  static bool ControlLoopsInstalled() { return ControlLoop.size() != 0; }
  static void UpdateTick();
 protected:
  static controlvector ControlLoop;
  static ulong Tick;
};

#else

#ifdef WIN32
#include <windows.h>
#endif

#ifdef USE_SDL
#include "SDL.h"
#endif

#include "typedef.h"

class bitmap;

class globalwindowhandler
{
 public:
#ifdef WIN32
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  static void Init(HINSTANCE, HWND*, const char*, LPCTSTR);
  static void CheckMessages();
#endif
#ifdef USE_SDL
  static void Init(const char*);
  static void ProcessMessage(SDL_Event*);
#endif
  static int GetKey(bool = true);
  static int ReadKey();
  static void SetQuitMessageHandler(bool (*What)()) { QuitMessageHandler = What; }
  static void InstallControlLoop(bool (*)());
  static void DeInstallControlLoop(bool (*)());
  static void SetInitialized(bool What) { Initialized = What; }
  static ulong GetTick() { return Tick; }
  static bool ControlLoopsInstalled() { return ControlLoop.size() != 0; }
  static void UpdateTick();
 private:
  static std::vector<int> KeyBuffer;
  static bool Initialized;
  static bool (*QuitMessageHandler)();
  static controlvector ControlLoop;
  static ulong Tick;
  static bool Active;
#ifdef WIN32
  static char KeyboardLayoutName[KL_NAMELENGTH];
#endif
};

#endif

#endif
