#ifndef __WHANDLER_H__
#define __WHANDLER_H__

#pragma warning(disable : 4786)

#define GETKEY globalwindowhandler::GetKey
#define READKEY globalwindowhandler::ReadKey

#include <windows.h>

#include "typedef.h"
#include "dynarray.h"

class bitmap;

class globalwindowhandler
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static int GetKey(bool = true, bool = false);
	static void Init(HINSTANCE, HWND*, const char*, LPCTSTR);
	static int ReadKey();
	static void ClearKeyBuffer() { KeyBuffer.Resize(0); }
	static void SetQuitMessageHandler(bool (*What)()) { QuitMessageHandler = What; }
	static void SetInitialized(bool What) { Initialized = What; }
	static void CheckMessages();
	static bool KeyIsDown(int Key) { return KeyBuffer.Search(Key) != 0xFFFF; }
private:
	static dynarray<int> KeyBuffer;
	static char KeyboardLayoutName[KL_NAMELENGTH];
	static bool Initialized;
	static bool (*QuitMessageHandler)();
	static bool Active;
};

#endif
