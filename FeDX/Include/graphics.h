#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#pragma warning(disable : 4786)

#define DOUBLEBUFFER	graphics::GetDoubleBuffer()
#define XRES		graphics::GetXRes()
#define YRES		graphics::GetYRes()
#define FONT		graphics::GetDefaultFont()

#define MAKE_RGB(Red, Green, Blue) ((Red) << 8 & 0xF800) | ((Green) << 3 & 0x7E0) | ((Blue) >> 3 & 0x1F)

#define GET_RED(Color)		((Color >> 8) & 0xF8)
#define GET_GREEN(Color)	((Color >> 3) & 0xFC)
#define GET_BLUE(Color)		((Color << 3) & 0xF8)

#define RED	MAKE_RGB(255, 0, 0)
#define GREEN	MAKE_RGB(0, 255, 0)
#define BLUE	MAKE_RGB(0, 0, 255)

#define YELLOW	MAKE_RGB(255, 255, 0)
#define PINK	MAKE_RGB(255, 0, 255)

#define WHITE	MAKE_RGB(255, 255, 255)
#define BLACK	MAKE_RGB(0, 0, 0)

#include <windows.h>
#include <list>

#include "typedef.h"

class bitmap;
class colorizablebitmap;
class CDisplay;

class graphics
{
public:
	friend class bitmap;
	static void Init();
	static void DeInit();
	static void SetMode(HINSTANCE, HWND*, const char*, ushort, ushort, uchar, bool);
	static void BlitDBToScreen();
	static ushort GetXRes() { return XRes; }
	static ushort GetYRes() { return YRes; }
	static bitmap* GetDoubleBuffer() { return DoubleBuffer; }
	//static CDisplay* GetDXDisplay() { return DXDisplay; }
	static void UpdateBounds();
	static void SwitchMode();
	static void LoadDefaultFont(std::string);
	static colorizablebitmap* GetDefaultFont() { return DefaultFont; }
	static bool GetFullScreen() { return FullScreen; }
private:
	static HWND hWnd;
	static bool FullScreen;
	static CDisplay* DXDisplay;
	static bitmap* DoubleBuffer;
	static ushort XRes;
	static ushort YRes;
	static uchar ColorDepth;
	//static std::list<bitmap*> BitmapContainer;
	static colorizablebitmap* DefaultFont;
};

#endif
