#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef USE_SDL
#include "SDL.h"
#endif

#include <list>
#include <string>

#include "typedef.h"
#include "vector2d.h"

#ifdef __DJGPP__
#define PACKED __attribute__ ((packed))
#endif

#define DOUBLEBUFFER graphics::GetDoubleBuffer()
#define RES graphics::GetRes()
#define FONT graphics::GetDefaultFont()

class bitmap;
class colorizablebitmap;

#ifdef WIN32
class CDisplay;
#endif

class graphics
{
 public:
  friend class bitmap;
  static void Init();
  static void DeInit();
#ifdef WIN32 
  static void SetMode(HINSTANCE, HWND*, const char*, vector2d, uchar, bool, LPCTSTR);
  static void SwitchMode();
  static void SetSwitchModeHandler(void (*What)()) { SwitchModeHandler = What; }
  static bool GetFullScreen() { return FullScreen; }
#endif
#ifdef USE_SDL
  static void SetMode(const char*, vector2d, uchar);
  static void ToggleFullScreen(void);
#endif
#ifdef __DJGPP__
  static void SetMode(ushort);
#endif
  static void BlitDBToScreen();
  static vector2d GetRes() { return Res; }
  static bitmap* GetDoubleBuffer() { return DoubleBuffer; }
  static void UpdateBounds();
  static void LoadDefaultFont(const std::string&);
  static colorizablebitmap* GetDefaultFont() { return DefaultFont; }
 private:
#ifdef WIN32
  static HWND hWnd;
  static bool FullScreen;
  static CDisplay* DXDisplay;
  static void (*SwitchModeHandler)();
#endif
#ifdef USE_SDL
  static SDL_Surface* screen;
#endif
#ifdef __DJGPP__
  static ulong BufferSize;
  static ushort ScreenSelector;
  static struct vesainfo
  {
    void Retrieve();
    bool   CheckSupport() { return Version == 0x0200; }
    ulong Signature		PACKED;
    ushort Version		PACKED;
    ulong OEMString		PACKED;
    ulong Capabilities	PACKED;
    ulong ModeList		PACKED;
    ushort Memory		PACKED;
    uchar  Shit[493]		PACKED;
  } VesaInfo;
  static struct modeinfo
  {
    void Retrieve(ushort);
    bool   CheckSupport() { return Attribs1 & 1 != 0; }
    ushort Attribs1		PACKED;
    uchar  AWindowAttribs	PACKED;
    uchar  BWindowAttribs	PACKED;
    ushort Granularity		PACKED;
    ushort WindowSize		PACKED;
    ushort WindowASegment	PACKED;
    ushort WindowBSegment	PACKED;
    ulong WindowMoveFunction	PACKED;
    ushort BytesPerLine	PACKED;
    ushort Width		PACKED;
    ushort Height		PACKED;
    uchar  CharWidth		PACKED;
    uchar  CharHeight		PACKED;
    uchar  Planes		PACKED;
    uchar  BitsPerPixel	PACKED;
    uchar  Banks		PACKED;
    uchar  MemoryModel		PACKED;
    uchar  BankSize		PACKED;
    uchar  ImagePages		PACKED;
    uchar  Reserved1		PACKED;
    uchar  RedBits		PACKED;
    uchar  RedShift		PACKED;
    uchar  GreenBits		PACKED;
    uchar  GreenShift		PACKED;
    uchar  BlueBits		PACKED;
    uchar  BlueShift		PACKED;
    uchar  ResBits		PACKED;
    uchar  ResShift		PACKED;
    uchar  Attribs2		PACKED;
    ulong PhysicalLFBAddress	PACKED;
    ulong OffScreenMem	PACKED;
    ushort OffScreenMemSize	PACKED;
    uchar  Reserved2[206]	PACKED;
  } ModeInfo;
#endif
  static bitmap* DoubleBuffer;
  static vector2d Res;
  static uchar ColorDepth;
  static colorizablebitmap* DefaultFont;
};

#endif

