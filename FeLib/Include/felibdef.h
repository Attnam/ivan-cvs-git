#ifndef __FELIBDEF_H__
#define __FELIBDEF_H__

/*
 * Global defines for the project FeLib.
 * This file is created to decrease the need of including headers in other headers just for the sake of some silly macros,
 * because it decreases compilation efficiency and may even cause cross-including
 *
 * List of macros that should be gathered here:
 * 1. all numeric defines used in multiple source files
 * 2. all inline functions used in multiple source files and independent enough (do not require other headers)
 * 3. class construction macros used in multiple header files
 *
 * DO NOT INCLUDE ANY FILES IN THIS HEADER.
 */

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif

inline ushort GetRed16(ushort Color) { return Color >> 8 & 0xF8; }
inline ushort GetGreen16(ushort Color) { return Color >> 3 & 0xFC; }
inline ushort GetBlue16(ushort Color) { return Color << 3 & 0xF8; }
inline ushort MakeRGB16(ushort Red, ushort Green, ushort Blue) { return (Red << 8 & 0xF800) | (Green << 3 & 0x7E0) | (Blue >> 3 & 0x1F); }
inline ushort MakeShadeColor(ushort Color) { return MakeRGB16(GetRed16(Color) / 3, GetGreen16(Color) / 3, GetBlue16(Color) / 3); }

inline ulong GetRed24(ulong Color) { return Color >> 16 & 0xFF; }
inline ulong GetGreen24(ulong Color) { return Color >> 8 & 0xFF; }
inline ulong GetBlue24(ulong Color) { return Color & 0xFF; }
inline ulong MakeRGB24(ulong Red, ulong Green, ulong Blue) { return (Red << 16 & 0xFF0000) | (Green << 8 & 0xFF00) | (Blue & 0xFF); }

#define NONE 0
#define MIRROR 1
#define FLIP 2
#define ROTATE 4

#define DEFAULTTRANSPARENT 0xF81F

#define RED MakeRGB16(255, 0, 0)
#define GREEN MakeRGB16(0, 255, 0)
#define BLUE MakeRGB16(0, 0, 255)

#define YELLOW MakeRGB16(255, 255, 0)
#define PINK MakeRGB16(255, 0, 255)

#define WHITE MakeRGB16(255, 255, 255)
#define LIGHTGRAY MakeRGB16(180, 180, 180)
#define DARKGRAY MakeRGB16(80, 80, 80)
#define BLACK MakeRGB16(0, 0, 0)

#define KEYBACKSPACE 0x08
#define KEYESC 0x1B
#define KEYENTER 0x0D
#define KEYUP 0x148
#define KEYDOWN 0x150
#define KEYRIGHT 0x14D
#define KEYLEFT 0x14B
#define KEYHOME 0x147
#define KEYEND 0x14F
#define KEYPAGEDOWN 0x151
#define KEYPAGEUP 0x149

#define NONE 0
#define MIRROR 1
#define FLIP 2
#define ROTATE_90 4

#define NOFLAME 0xFFFF

#define BITMAP_ERROR_VECTOR vector2d(-666, -666)

#ifdef GCC
#define PACKED __attribute__ ((packed))
#else
#define PACKED
#endif

#define SELECTABLE 1
#define INVERSE_MODE 2
#define BLIT_AFTERWARDS 4
#define DRAW_BACKGROUND_AFTERWARDS 8
#define FADE 16

#endif

