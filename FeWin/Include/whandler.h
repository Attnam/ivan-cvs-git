#ifndef __WHANDLER_H__
#define __WHANDLER_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define GETKEY globalwindowhandler::GetKey
#define READKEY globalwindowhandler::ReadKey

#ifdef __DJGPP__

#include <pc.h>

class globalwindowhandler
{
public:
	static int GetKey(bool = true, bool = false);
	static int ReadKey();
};

#else

#ifdef WIN32
#include <windows.h>
#endif

#ifdef SDL
#include "SDL.h"
#endif

#include "typedef.h"
#include "dynarray.h"

class bitmap;

class globalwindowhandler
{
public:
#ifdef WIN32
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void Init(HINSTANCE, HWND*, const char*, LPCTSTR);
	static void CheckMessages();
#endif
#ifdef SDL
	static void Init(const char*);
	static void ProcessMessage(SDL_Event);
#endif
	static int GetKey(bool = true, bool = false);
	static int ReadKey();
	static void ClearKeyBuffer() { KeyBuffer.Resize(0); }
	static void SetQuitMessageHandler(bool (*What)()) { QuitMessageHandler = What; }
	static void SetInitialized(bool What) { Initialized = What; }
	static bool KeyIsDown(int Key) { return KeyBuffer.Search(Key) != 0xFFFF; }
private:
	static dynarray<int> KeyBuffer;
	static bool Initialized;
	static bool (*QuitMessageHandler)();
#ifdef WIN32
	static bool Active;
	static char KeyboardLayoutName[KL_NAMELENGTH];
#endif
};

#endif

#endif



