#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define DOUBLEBUFFER	graphics::GetDoubleBuffer()
#define XRES		graphics::CXRes()
#define YRES		graphics::CYRes()

#include <windows.h>
#include <vector>

#include "typedef.h"

class bitmap;
class CDisplay;

class graphics
{
public:
	friend class bitmap;
	static void Init(void);
	static void DeInit(void);
	static void SetMode(HINSTANCE, HWND*, const char*, ushort, ushort, uchar, bool);
	static void BlitDBToScreen(void);
	static void ClearDBToColor(ushort = 0);
	static void ClearDBToColor(ushort, ushort, ushort, ushort, ushort = 0);
	static ushort CXRes(void) { return XRes; }
	static ushort CYRes(void) { return YRes; }
	static bitmap* GetDoubleBuffer(void) { return DoubleBuffer; }
	static CDisplay* GetDXDisplay(void) { return DXDisplay; }
	static void UpdateBounds(void);
	static void SwitchMode(void);
private:
	static HWND hWnd;
	static bool FullScreen;
	static CDisplay* DXDisplay;
	static bitmap* DoubleBuffer;
	static ushort XRes;
	static ushort YRes;
	static uchar ColorDepth;
	static std::vector<bitmap*> BitmapContainer;
};

#endif
