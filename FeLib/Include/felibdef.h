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

inline ushort GetRed(ushort Color) { return Color >> 8 & 0xF8; }
inline ushort GetGreen(ushort Color) { return Color >> 3 & 0xFC; }
inline ushort GetBlue(ushort Color) { return Color << 3 & 0xF8; }
inline ushort MakeRGB(ushort Red, ushort Green, ushort Blue) { return (Red << 8 & 0xF800) | (Green << 3 & 0x7E0) | (Blue >> 3 & 0x1F); }
inline ushort MakeShadeColor(ushort Color) { return MakeRGB(GetRed(Color) / 3, GetGreen(Color) / 3, GetBlue(Color) / 3); }

#define NONE 0
#define MIRROR 1
#define FLIP 2
#define ROTATE 4

#define DEFAULTTRANSPARENT 0xF81F

#define RED MakeRGB(255, 0, 0)
#define GREEN MakeRGB(0, 255, 0)
#define BLUE MakeRGB(0, 0, 255)

#define YELLOW MakeRGB(255, 255, 0)
#define PINK MakeRGB(255, 0, 255)

#define WHITE MakeRGB(255, 255, 255)
#define LIGHTGRAY MakeRGB(180, 180, 180)
#define DARKGRAY MakeRGB(80, 80, 80)
#define BLACK MakeRGB(0, 0, 0)

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

#endif

