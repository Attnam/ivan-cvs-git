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

#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <cstdlib>

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
  int GetManhattanLength() const	      { return Max(abs(X), abs(Y)); }
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
const vector2d ABORT_VECTOR(-0x7FFF, -0x7FFF);

#endif
