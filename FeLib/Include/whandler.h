#ifndef __WHANDLER_H__
#define __WHANDLER_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#ifdef USE_SDL
#include <vector>
#include "SDL.h"
#endif

#ifdef __DJGPP__
#include <ctime>
#endif

#include "typedef.h"
#include "felibdef.h"

#define GET_KEY globalwindowhandler::GetKey
#define READ_KEY globalwindowhandler::ReadKey

class globalwindowhandler
{
 public:
  static int GetKey(bool = true);
  static int ReadKey();
  static void InstallControlLoop(bool (*)());
  static void DeInstallControlLoop(bool (*)());
  static void SetTick(ulong What) { Tick = What; }
  static ulong GetTick() { return Tick; }
  static bool ControlLoopsInstalled() { return Controls != 0; }
  static void EnableControlLoops() { ControlLoopsEnabled = true; }
  static void DisableControlLoops() { ControlLoopsEnabled = false; }
#ifdef USE_SDL
  static void Init();
  static void SetQuitMessageHandler(bool (*What)()) { QuitMessageHandler = What; }
  static void UpdateTick() { Tick = SDL_GetTicks() / 40; }
#endif
#ifdef __DJGPP__
  static void Init() { }
  static void SetQuitMessageHandler(bool (*)()) { }
  static void UpdateTick() { Tick = uclock() * 25 / UCLOCKS_PER_SEC; }
#endif
 private:
#ifdef USE_SDL
  static void ProcessMessage(SDL_Event*);
  static std::vector<int> KeyBuffer;
  static bool (*QuitMessageHandler)();
#endif
  static bool (*ControlLoop[MAX_CONTROLS])();
  static ushort Controls;
  static ulong Tick;
  static bool ControlLoopsEnabled;
};

#endif
