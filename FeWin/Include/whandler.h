#ifndef __WHANDLER_H__
#define __WHANDLER_H__

#define GETKEY globalwindowhandler::GetKey

#include <windows.h>

#include "typedef.h"
#include "dynarray.h"

class bitmap;

class globalwindowhandler
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static int GetKey(void);
	static bool CActive(void) { return Active; }
	static bool CKeyPressed(void) { return KeyPressed; }
	static char* CKeyboardLayoutName(void) { return KeyboardLayoutName; }
	static void Init(HINSTANCE, HWND*, const char*);
private:
	static dynarray<int> KeyBuffer;
	static bool Active, KeyPressed;
	static char KeyboardLayoutName[KL_NAMELENGTH];
	static bool InGetKey;
};

#endif
