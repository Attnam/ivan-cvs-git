#ifndef __WHANDLER_H__
#define __WHANDLER_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "felibdef.h"

#define GET_KEY globalwindowhandler::GetKey
#define READ_KEY globalwindowhandler::ReadKey

#ifdef __DJGPP__

#include <pc.h>

class globalwindowhandler
{
 public:
  static int GetKey(bool = true, bool = false);
  static int ReadKey();
  static ulong GetTick() { return Tick; }
  static void InstallControlLoop(bool (*)());
  static void DeInstallControlLoop(bool (*)());
  static bool ControlLoopsInstalled() { return Controls != 0; }
  static ulong UpdateTick();
 protected:
  static bool (*ControlLoop[MAX_CONTROLS])();
  static ushort Controls;
  static ulong Tick;
};

#else

#include <vector>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef USE_SDL
#include "SDL.h"
#endif

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
  static bool ControlLoopsInstalled() { return Controls != 0; }
  static ulong UpdateTick();
 private:
  static std::vector<int> KeyBuffer;
  static bool Initialized;
  static bool (*QuitMessageHandler)();
  static bool (*ControlLoop[MAX_CONTROLS])();
  static ushort Controls;
  static ulong Tick;
#ifdef WIN32
  static bool Active;
  static char KeyboardLayoutName[KL_NAMELENGTH];
#endif
};

#endif

#endif
