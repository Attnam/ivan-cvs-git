#ifndef __RECT_H__
#define __RECT_H__
#include "vector2d2d.h"

class rectangle
{
 public:
  long Top, Left, Right, Bottom;
  rectangle operator = (rectangle& What) { Top = What.Top; Left = What.Left; Right = What.Right; Bottom = What.Bottom; return *this;}
  rectangle operator = (rectangle* What) { Top = What->Top; Left = What->Left; Right = What->Right; Bottom = What->Bottom; return *this;}
  rectangle(long Top, long Left, long Right, long Bottom) : Top(Top), Left(Left), Right(Right), Bottom(Bottom) {}
  rectangle() {}
  bool IsIn(vector2d2d2d What)
  { if(What.X > Left && What.X < Right && What.Y > Top && What.Y < Bottom) return true; else return false; }
};

#endif



