#ifndef __FEMATH_H__
#define __FEMATH_H__

#pragma warning(disable : 4786)

#define PI 3.1415926535897932384626433832795f

template <class type> inline type GetHypotSquare(type X, type Y) { return X * X + Y * Y; }
template <class type> inline type Max(type X, type Y) { return X > Y ? X : Y; }
template <class type> inline type Min(type X, type Y) { return X < Y ? X : Y; }

#endif
