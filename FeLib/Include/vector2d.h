#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

struct vector2d
{
  vector2d()				      { }
  vector2d(short X, short Y) : X(X), Y(Y)     { }
  vector2d  operator +   (vector2d H) const   { return vector2d(X + H.X, Y + H.Y); }
  vector2d& operator +=  (vector2d H)	      { X += H.X; Y += H.Y; return *this; }
  vector2d  operator -   (vector2d H) const   { return vector2d(X - H.X, Y - H.Y); }
  vector2d& operator -=  (vector2d H)	      { X -= H.X; Y -= H.Y; return *this; }
  vector2d  operator -   () const	      { return vector2d(-X, -Y); }
  vector2d  operator *   (short H) const      { return vector2d(X * H, Y * H); }
  vector2d& operator *=  (short H)	      { X *= H; Y *= H; return *this; }
  vector2d  operator /   (short H) const      { return vector2d(X / H, Y / H); }
  vector2d& operator /=  (short H)	      { X /= H; Y /= H; return *this; }
  vector2d  operator *   (float H) const      { return vector2d(short(X * H), short(Y * H)); }
  vector2d& operator *=  (float H)	      { X = short(X * H); Y = short(Y * H); return *this; }
  vector2d  operator /   (float H) const      { return vector2d(short(X / H), short(Y / H)); }
  vector2d& operator /=  (float H)	      { X = short(X / H); Y = short(Y / H); return *this; }
  bool	    operator ==  (vector2d H) const   { return X == H.X && Y == H.Y; }
  bool	    operator !=  (vector2d H) const   { return X != H.X || Y != H.Y; }
  vector2d  operator <<  (uchar Shift) const  { return vector2d(X << Shift, Y << Shift); }
  vector2d& operator <<= (uchar Shift)	      { X <<= Shift; Y <<= Shift; return *this; }
  vector2d  operator >>  (uchar Shift) const  { return vector2d(X >> Shift, Y >> Shift); }
  vector2d& operator >>= (uchar Shift)	      { X >>= Shift; Y >>= Shift; return *this; }
  ulong GetLengthSquare() const		      { return long(X) * long(X) + long(Y) * long(Y); }
  bool IsAdjacent(vector2d TPos) const { return TPos.X >= X - 1 && TPos.X <= X + 1 && TPos.Y <= Y + 1 && TPos.Y >= Y - 1; } /* Also returns true if TPos == this */
  short X, Y;
};

#endif
