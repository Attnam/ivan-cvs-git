#ifndef __FELIBDEF_H__
#define __FELIBDEF_H__

/*
 * Global defines for the project FeLib.
 * This file is created to decrease the need of including headers in other headers just for the sake of some silly macros,
 * because it decreases compilation efficiency and may even cause cross-including
 *
 * List of macros that should be gathered here:
 * 1. all numeric defines used in multiple source files
 * 2. all functional macros used in multiple source files and independent enough (do not require other headers)
 * 3. class construction macros used in multiple header files
 *
 * DO NOT INCLUDE ANY FILES IN THIS HEADER.
 */

/* Blit flags */

#define NONE 0
#define MIRROR 1
#define FLIP 2
#define ROTATE_90 4

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif

#define GET_RED(Color) (((Color) >> 8) & 0xF8)
#define GET_GREEN(Color) (((Color) >> 3) & 0xFC)
#define GET_BLUE(Color) (((Color) << 3) & 0xF8)

#define MAKE_RGB(Red, Green, Blue) ((Red) << 8 & 0xF800) | ((Green) << 3 & 0x7E0) | ((Blue) >> 3 & 0x1F)
#define MAKE_SHADE_COL(Color) MAKE_RGB(GET_RED(Color) / 3, GET_GREEN(Color) / 3, GET_BLUE(Color) / 3)

#define RED MAKE_RGB(255, 0, 0)
#define GREEN MAKE_RGB(0, 255, 0)
#define BLUE MAKE_RGB(0, 0, 255)

#define YELLOW MAKE_RGB(255, 255, 0)
#define PINK MAKE_RGB(255, 0, 255)
#define DEFAULT_TRANSPARENT 0xF81F

#define WHITE MAKE_RGB(255, 255, 255)
#define LIGHTGRAY MAKE_RGB(180, 180, 180)
#define DARKGRAY MAKE_RGB(80, 80, 80)
#define BLACK MAKE_RGB(0, 0, 0)

#define FK_BACKSPACE 0x08
#define FK_ESC 0x1b
#define FK_ENTER 0x0d

#endif
