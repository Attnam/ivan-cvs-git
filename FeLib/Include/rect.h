#ifndef __RECT_H__
#define __RECT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"

struct rect
{
  rect() { }
  rect(short X1, short Y1, short X2, short Y2) : X1(X1), Y1(Y1), X2(X2), Y2(Y2) { }
  rect(vector2d V1, vector2d V2) : X1(V1.X), Y1(V1.Y), X2(V2.X), Y2(V2.Y) { }
  rect operator+(vector2d V) const { return rect(X1 + V.X, Y1 + V.Y, X2 + V.X, Y2 + V.Y); }
  short X1, Y1, X2, Y2;
};

#endif
