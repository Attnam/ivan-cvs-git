/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __FELIBDEF_H__
#define __FELIBDEF_H__

/* Global defines for the project FeLib.
   This file is created to decrease the need of including headers in other
   headers just for the sake of some silly macros, because it decreases
   compilation efficiency and may cause cross-including

   List of macros that should be gathered here:
   1. all numeric defines used in multiple .cpp or .h files
   2. all inline functions used in multiple .cpp or .h files and independent
      enough (do not require other headers)
   3. class construction macros used in multiple .h files */

#include "typedef.h"

const int MapMoveX[9] = { -1, 0, 1, -1, 1, -1, 0, 1, 0 };
const int MapMoveY[9] = { -1, -1, -1, 0, 0, 1, 1, 1, 0 };

const ulong SquarePartTickMask[4] = { 0xFF, 0xFF00, 0xFF0000, 0xFF000000 };

#define FPI 3.1415926535897932384626433832795

template <class type>
inline type Max(type X, type Y) { return X >= Y ? X : Y; }

template <class type> inline type Max(type X, type Y, type Z)
{
  return X >= Y ? (X >= Z ? X : Z) : (Y >= Z ? Y : Z);
}

template <class type>
inline type Min(type X, type Y) { return X <= Y ? X : Y; }

template <class type> inline type Min(type X, type Y, type Z)
{
  return X <= Y ? (X <= Z ? X : Z) : (Y <= Z ? Y : Z);
}

template <class type>
inline type HypotSquare(type X, type Y) { return X * X + Y * Y; }

template <class type>
inline type Limit(type Value, type Minimum, type Maximum)
{
  return Value >= Minimum ? Value <= Maximum ? Value : Maximum : Minimum;
}

template <class type>
inline void LimitRef(type& Value, type Minimum, type Maximum)
{
  if(Value <= Minimum)
    Value = Minimum;
  else if(Value >= Maximum)
    Value = Maximum;
}

template <class type> inline void Swap(type& X, type& Y)
{
  const type T = X;
  X = Y;
  Y = T;
}

inline color16 GetRed16(color16 Color) { return Color >> 8 & 0xF8; }
inline color16 GetGreen16(color16 Color) { return Color >> 3 & 0xFC; }
inline color16 GetBlue16(color16 Color) { return Color << 3 & 0xF8; }

inline color16 MakeRGB16(int Red, int Green, int Blue)
{
  return (Red << 8 & 0xF800) | (Green << 3 & 0x7E0) | (Blue >> 3 & 0x1F);
}

inline color16 MakeShadeColor(color16 Color)
{
  return MakeRGB16(GetRed16(Color) / 3,
		   GetGreen16(Color) / 3,
		   GetBlue16(Color) / 3);
}

inline color24 GetRed24(color24 Color) { return Color >> 16 & 0xFF; }
inline color24 GetGreen24(color24 Color) { return Color >> 8 & 0xFF; }
inline color24 GetBlue24(color24 Color) { return Color & 0xFF; }

inline color24 MakeRGB24(int Red, int Green, int Blue)
{
  return (Red << 16 & 0xFF0000) | (Green << 8 & 0xFF00) | (Blue & 0xFF);
}

inline int GetMaxColor24(color24 Color)
{
  return Max(GetRed24(Color), GetGreen24(Color), GetBlue24(Color));
}

inline int GetMinColor24(color24 Color)
{
  return Min(GetRed24(Color), GetGreen24(Color), GetBlue24(Color));
}

#define NONE 0
#define MIRROR 1
#define FLIP 2
#define ROTATE 4

#define TRANSPARENT_COLOR 0xF81F

#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F

#define YELLOW 0xFFE0
#define PINK 0xF01E

#define WHITE 0xFFFF
#define LIGHT_GRAY 0x94B2
#define DARK_GRAY 0x528A
#define BLACK 0x0000

#define NORMAL_LUMINANCE 0x808080

#define KEY_BACK_SPACE 0x08
#define KEY_ESC 0x1B
#define KEY_ENTER 0x0D
#define KEY_UP 0x148
#define KEY_DOWN 0x150
#define KEY_RIGHT 0x14D
#define KEY_LEFT 0x14B
#define KEY_HOME 0x147
#define KEY_END 0x14F
#define KEY_PAGE_DOWN 0x151
#define KEY_PAGE_UP 0x149
#define KEY_SPACE ' '
#define KEY_NUMPAD_5 2

#define NO_FLAME 0xFFFF

#ifdef GCC
#define NO_ALIGNMENT __attribute__ ((packed))
#else
#define NO_ALIGNMENT
#endif

#define SELECTABLE 1
#define INVERSE_MODE 2
#define BLIT_AFTERWARDS 4
#define DRAW_BACKGROUND_AFTERWARDS 8
#define FADE 16

/* felist errors */

#define FELIST_ERROR_BIT 0x8000
#define LIST_WAS_EMPTY 0xFFFF
#define ESCAPED 0xFFFE
#define NOTHING_SELECTED 0xFFFD

#define NO_LIMIT 0xFFFF

#define MAX_CONTROLS 0x10

#define HIGHEST 0xFF

#define NORMAL_EXIT 0
#define ABORTED 1

#define MAX_CONFIG_OPTIONS 0x100

#define FLY_PRIORITY ((10 << 4) + 10)
#define SPARKLE_PRIORITY ((12 << 4) + 12)
#define LIGHTNING_PRIORITY ((14 << 4) + 14)
#define AVERAGE_PRIORITY ((8 << 4) + 8)

#define NO_IMAGE 0xFFFF

#define ZERO_POOLS 1
#define RAND_ALLOC 2

#define REFS(ptr) reinterpret_cast<ulong*>(ptr)[-1]

#define SKIP_FIRST 1
#define ALLOW_END_FAILURE 2

#define MAX_RAND 0x7FFFFFFF

#define TRANSPARENT_PALETTE_INDEX 191

#define MAX_HIGHSCORES 100

#endif
