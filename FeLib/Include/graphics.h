#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#ifdef USE_SDL
#include "SDL.h"
#endif

#include <list>
#include <string>

#include "typedef.h"
#include "vector2d.h"
#include "felibdef.h"

#define DOUBLE_BUFFER graphics::GetDoubleBuffer()
#define RES graphics::GetRes()
#define FONT graphics::GetDefaultFont()

class bitmap;
class colorizablebitmap;

class graphics
{
 public:
  friend class bitmap;
  static void Init();
  static void DeInit();
  static void SwitchMode();
#ifdef USE_SDL
  static void SetMode(const char*, const char*, vector2d, uchar, bool);
#endif
#ifdef __DJGPP__
  static void SetMode(ushort);
#endif
  static void BlitDBToScreen();
  static vector2d GetRes() { return Res; }
  static bitmap* GetDoubleBuffer() { return DoubleBuffer; }
  static void LoadDefaultFont(const std::string&);
  static colorizablebitmap* GetDefaultFont() { return DefaultFont; }
  static void SetSwitchModeHandler(void (*What)()) { SwitchModeHandler = What; }
 private:
  static void (*SwitchModeHandler)();
#ifdef USE_SDL
  static SDL_Surface* Screen;
#endif
#ifdef __DJGPP__
  static ulong BufferSize;
  static ushort ScreenSelector;
  static struct vesainfo
  {
    void Retrieve();
    bool CheckSupport() { return Version == 0x0200; }
    ulong Signature NO_ALIGNMENT;
    ushort Version NO_ALIGNMENT;
    ulong OEMString NO_ALIGNMENT;
    ulong Capabilities NO_ALIGNMENT;
    ulong ModeList NO_ALIGNMENT;
    ushort Memory NO_ALIGNMENT;
    uchar Shit[493] NO_ALIGNMENT;
  } VesaInfo;
  static struct modeinfo
  {
    void Retrieve(ushort);
    bool CheckSupport() { return Attribs1 & 1 != 0; }
    ushort Attribs1 NO_ALIGNMENT;
    uchar AWindowAttribs NO_ALIGNMENT;
    uchar BWindowAttribs NO_ALIGNMENT;
    ushort Granularity NO_ALIGNMENT;
    ushort WindowSize NO_ALIGNMENT;
    ushort WindowASegment NO_ALIGNMENT;
    ushort WindowBSegment NO_ALIGNMENT;
    ulong WindowMoveFunction NO_ALIGNMENT;
    ushort BytesPerLine NO_ALIGNMENT;
    ushort Width NO_ALIGNMENT;
    ushort Height NO_ALIGNMENT;
    uchar CharWidth NO_ALIGNMENT;
    uchar CharHeight NO_ALIGNMENT;
    uchar Planes NO_ALIGNMENT;
    uchar BitsPerPixel NO_ALIGNMENT;
    uchar Banks NO_ALIGNMENT;
    uchar MemoryModel NO_ALIGNMENT;
    uchar BankSize NO_ALIGNMENT;
    uchar ImagePages NO_ALIGNMENT;
    uchar Reserved1 NO_ALIGNMENT;
    uchar RedBits NO_ALIGNMENT;
    uchar RedShift NO_ALIGNMENT;
    uchar GreenBits NO_ALIGNMENT;
    uchar GreenShift NO_ALIGNMENT;
    uchar BlueBits NO_ALIGNMENT;
    uchar BlueShift NO_ALIGNMENT;
    uchar ResBits NO_ALIGNMENT;
    uchar ResShift NO_ALIGNMENT;
    uchar Attribs2 NO_ALIGNMENT;
    ulong PhysicalLFBAddress NO_ALIGNMENT;
    ulong OffScreenMem NO_ALIGNMENT;
    ushort OffScreenMemSize NO_ALIGNMENT;
    uchar Reserved2[206] NO_ALIGNMENT;
  } ModeInfo;
#endif
  static bitmap* DoubleBuffer;
  static vector2d Res;
  static uchar ColorDepth;
  static colorizablebitmap* DefaultFont;
};

#endif
