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
  rect operator+(vector2d Vector) { return rect(X1 + Vector.X, Y1 + Vector.Y, X2 + Vector.X, Y2 + Vector.Y); }
  short X1, Y1, X2, Y2;
};

#endif
