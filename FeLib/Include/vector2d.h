#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "felibdef.h"

struct vector2d;

struct packedvector2d
{
  packedvector2d& operator=(vector2d);
  short X, Y;
};

/* Standard structure for representing positions */

struct vector2d
{
  vector2d()				      { }
  vector2d(int X, int Y) : X(X), Y(Y)	      { }
  vector2d(packedvector2d V) : X(V.X), Y(V.Y) { }
  vector2d  operator +   (vector2d V) const   { return vector2d(X + V.X, Y + V.Y); }
  vector2d& operator +=  (vector2d V)	      { X += V.X; Y += V.Y; return *this; }
  vector2d  operator -   (vector2d V) const   { return vector2d(X - V.X, Y - V.Y); }
  vector2d& operator -=  (vector2d V)	      { X -= V.X; Y -= V.Y; return *this; }
  vector2d  operator -   () const	      { return vector2d(-X, -Y); }
  vector2d  operator *   (int I) const	      { return vector2d(X * I, Y * I); }
  vector2d& operator *=  (int I)	      { X *= I; Y *= I; return *this; }
  vector2d  operator /   (int I) const	      { return vector2d(X / I, Y / I); }
  vector2d& operator /=  (int I)	      { X /= I; Y /= I; return *this; }
  vector2d  operator *   (double D) const     { return vector2d(int(X * D), int(Y * D)); }
  vector2d& operator *=  (double D)	      { X = int(X * D); Y = int(Y * D); return *this; }
  vector2d  operator /   (double D) const     { return vector2d(int(X / D), int(Y / D)); }
  vector2d& operator /=  (double D)	      { X = int(X / D); Y = int(Y / D); return *this; }
  bool	    operator ==  (vector2d V) const   { return X == V.X && Y == V.Y; }
  bool	    operator !=  (vector2d V) const   { return X != V.X || Y != V.Y; }
  vector2d  operator <<  (int S) const	      { return vector2d(X << S, Y << S); }
  vector2d& operator <<= (int S)	      { X <<= S; Y <<= S; return *this; }
  vector2d  operator >>  (int S) const	      { return vector2d(X >> S, Y >> S); }
  vector2d& operator >>= (int S)	      { X >>= S; Y >>= S; return *this; }
  bool	    operator <	 (vector2d V) const   { return X < V.X || (X == V.X && Y < V.Y); }
  int GetLengthSquare() const		      { return X * X + Y * Y; }
  /* Also returns true if V == *this */
  bool IsAdjacent(vector2d V) const	      { return V.X >= X - 1 && V.X <= X + 1 && V.Y <= Y + 1 && V.Y >= Y - 1; }
  /* Pack a position of a 16x16 map to a byte and vice versa */
  /*static vbyte PackToByte(int X, int Y)	      { return (Y << 4) | (X & 0xF); }
  static vbyte PackToByte(vector2d V)	      { return (V.Y << 4) | (V.X & 0xF); }
  static vector2d UnpackByte(vbyte B)	      { return vector2d(B & 0xF, B >> 4); }*/
  int X, Y;
};

inline packedvector2d& packedvector2d::operator=(vector2d V)
{
  X = V.X; Y = V.Y;
  return *this;
}

/* Rotates a position Vect of a square map of size Size x Size according to
   Flags (see felibdef.h) */

inline void Rotate(vector2d& Vect, int Size, int Flags)
{
  const int Limit = Size - 1;

  if(Flags & ROTATE)
    {
      const int T = Vect.X;
      Vect.X = Limit - Vect.Y;
      Vect.Y = T;
    }

  if(Flags & MIRROR)
    Vect.X = Limit - Vect.X;

  if(Flags & FLIP)
    Vect.Y = Limit - Vect.Y;
}

const vector2d ERROR_VECTOR(-0x8000, -0x8000);

#endif
