#include <cstdio>
#include <ctime>
#include <cmath>

#include "femath.h"
#include "error.h"
#include "felibdef.h"

/* A C-program for MT19937: Integer     version                   */
/*  genrand() generates one pseudorandom unsigned integer (32bit) */
/* which is uniformly distributed among 0 to 2^32-1  for each     */
/* call. sgenrand(seed) set initial values to the working area    */
/* of 624 words. Before genrand(), sgenrand(seed) must be         */
/* called once. (seed is any 32-bit integer except for 0).        */
/*   Coded by Takuji Nishimura, considering the suggestions by    */
/* Topher Cooper and Marc Rieffel in July-Aug. 1997.              */

/* This library is free software; you can redistribute it and/or   */
/* modify it under the terms of the GNU Library General Public     */
/* License as published by the Free Software Foundation; either    */
/* version 2 of the License, or (at your option) any later         */
/* version.                                                        */
/* This library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of  */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.            */
/* See the GNU Library General Public License for more details.    */
/* You should have received a copy of the GNU Library General      */
/* Public License along with this library; if not, write to the    */
/* Free Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   */ 
/* 02111-1307  USA                                                 */

/* Copyright (C) 1997 Makoto Matsumoto and Takuji Nishimura.       */
/* Any feedback is very welcome. For any question, comments,       */
/* see http://www.math.keio.ac.jp/matumoto/emt.html or email       */
/* matumoto@math.keio.ac.jp                                        */

/* Period parameters */
#define N1 624
#define M 397
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */   
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y) (y >> 11)
#define TEMPERING_SHIFT_S(y) (y << 7)
#define TEMPERING_SHIFT_T(y) (y << 15)
#define TEMPERING_SHIFT_L(y) (y >> 18)

ulong femath::mt[N1]; /* the array for the state vector  */
long femath::mti = N1+1; /* mti==N+1 means mt[N] is not initialized */

void femath::SetSeed(ulong seed)
{
  /* setting initial seeds to mt[N] using         */
  /* the generator Line 25 of Table 1 in          */
  /* [KNUTH 1981, The Art of Computer Programming */
  /*    Vol. 2 (2nd Ed.), pp102]                  */
  mt[0]= seed & 0xffffffff;
  for (mti=1; mti<N1; mti++)
    mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;
}

long femath::Rand()
{
  ulong y;
  static ulong mag01[2]={0x0, MATRIX_A};
  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if (mti >= N1) { /* generate N words at one time */
    int kk;

    if (mti == N1+1) /* if sgenrand() has not been called, */
      SetSeed(4357); /* a default initial seed is used   */

    for (kk=0;kk<N1-M;kk++) {
      y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
      mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
    }
    for (;kk<N1-1;kk++) {
      y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
      mt[kk] = mt[kk+(M-N1)] ^ (y >> 1) ^ mag01[y & 0x1];
    }
    y = (mt[N1-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
    mt[N1-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

    mti = 0;
  }
  
  y = mt[mti++];
  y ^= TEMPERING_SHIFT_U(y);
  y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
  y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
  y ^= TEMPERING_SHIFT_L(y);

  return y & 0x7FFFFFFF;
}

/* Why the f*ck these _insane_ warnings didn't appear in game.cpp??? */

#ifdef VC
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)
#endif

bool femath::DoLine(long X1, long Y1, long X2, long Y2, bool (*Proc)(long, long))
{
  long DX = X2 - X1, DY = Y2 - Y1, I1, I2, X, Y, DD;

  #define DO_LINE(PriSign, PriC, PriCond, SecSign, SecC, SecCond)\
    {\
      if(!D##PriC)\
	{\
	  Proc(X1, Y1);\
	  return true;\
	}\
      \
      I1 = D##SecC << 1;\
      DD = I1 - (SecSign (PriSign D##PriC));\
      I2 = DD - (SecSign (PriSign D##PriC));\
      \
      X = X1;\
      Y = Y1;\
      \
      Proc(X, Y);\
      \
      if(DD SecCond 0)\
	{\
	  SecC SecSign##= 1;\
	  DD += I2;\
	}\
      else\
	DD += I1;\
      \
      PriC PriSign##= 1;\
      \
      while(PriC PriCond PriC##2)\
	{\
	  if(!Proc(X, Y))\
	    return false;\
	  \
	  if(DD SecCond 0)\
	    {\
	      SecC SecSign##= 1;\
	      DD += I2;\
	    }\
	  else\
	    DD += I1;\
	  \
	  PriC PriSign##= 1;\
	}\
    }

  if(DX >= 0)
    {
      if(DY >= 0)
	{
	  if(DX >= DY)
	    DO_LINE(+, X, <=, +, Y, >=)
	  else
	    DO_LINE(+, Y, <=, +, X, >=)
	}
      else
	{
	  if(DX >= -DY)
	    DO_LINE(+, X, <=, -, Y, <=)
	  else
	    DO_LINE(-, Y, >=, +, X, >=)
	}
    }
  else
    {
      if(DY >= 0)
	{
	  if(-DX >= DY)
	    DO_LINE(-, X, >=, +, Y, >=)
	  else
	    DO_LINE(+, Y, <=, -, X, <=)
	}
      else
	{
	  if(-DX >= -DY)
	    DO_LINE(-, X, >=, -, Y, <=)
	  else
	    DO_LINE(-, Y, >=, -, X, <=)
	}
    }

  return true;
}

ushort femath::WeightedRand(const std::vector<long>& Possibility)
{
  ushort c;
  ulong TotalPossibility = 0;

  for(c = 0; c < Possibility.size(); ++c)
    TotalPossibility += Possibility[c];

  ulong Chosen = RAND() % TotalPossibility;
  TotalPossibility = 0;

  for(c = 0; c < Possibility.size(); ++c)
    {
      TotalPossibility += Possibility[c];

      if(TotalPossibility > Chosen)
	return c;
    }

  ABORT("WeightedRand bugs severely!");
  return 0x0666;
}

float femath::CalculateAngle(vector2d Direction)
{
  if(Direction.X < 0)
    return atan(float(Direction.Y) / Direction.X) + PI;
  else if(Direction.X > 0)
    {
      if(Direction.Y < 0)
	return atan(float(Direction.Y) / Direction.X) + 2 * PI;
      else
	return atan(float(Direction.Y) / Direction.X);
    }
  else
    {
      if(Direction.Y < 0)
	return 3 * PI / 2;
      else if(Direction.Y > 0)
	return PI / 2;
      else
	{
	  ABORT("Illegal direction (0, 0) passed to femath::CalculateAngle()!");
	  return 0;
	}
    }
}

void femath::CalculateEnvironmentRectangle(rect& Rect, const rect& MotherRect, vector2d Origo, ushort Radius)
{
  Rect.X1 = Origo.X - Radius;
  Rect.Y1 = Origo.Y - Radius;
  Rect.X2 = Origo.X + Radius;
  Rect.Y2 = Origo.Y + Radius;

  if(Rect.X1 < MotherRect.X1)
    Rect.X1 = MotherRect.X1;

  if(Rect.Y1 < MotherRect.Y1)
    Rect.Y1 = MotherRect.Y1;

  if(Rect.X2 > MotherRect.X2)
    Rect.X2 = MotherRect.X2;

  if(Rect.Y2 > MotherRect.Y2)
    Rect.Y2 = MotherRect.Y2;
}
