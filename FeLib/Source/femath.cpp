#include <cmath>

#include "femath.h"
#include "error.h"
#include "save.h"

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

/* backups */

ulong femath::mtb[N1];
long femath::mtib;

void femath::SetSeed(ulong Seed)
{
  /* setting initial seeds to mt[N] using         */
  /* the generator Line 25 of Table 1 in          */
  /* [KNUTH 1981, The Art of Computer Programming */
  /*    Vol. 2 (2nd Ed.), pp102]                  */

  mt[0] = Seed & 0xffffffff;

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

ushort femath::WeightedRand(long* Possibility, ushort Size)
{
  ushort c;
  ulong TotalPossibility = 0;

  for(c = 0; c < Size; ++c)
    TotalPossibility += Possibility[c];

  ulong Chosen = RAND() % TotalPossibility;
  TotalPossibility = 0;

  for(c = 0; c < Size; ++c)
    {
      TotalPossibility += Possibility[c];

      if(TotalPossibility > Chosen)
	return c;
    }

  ABORT("WeightedRand bugs severely!");
  return 0x0666;
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
    return atan(float(Direction.Y) / Direction.X) + FPI;
  else if(Direction.X > 0)
    {
      if(Direction.Y < 0)
	return atan(float(Direction.Y) / Direction.X) + 2 * FPI;
      else
	return atan(float(Direction.Y) / Direction.X);
    }
  else
    {
      if(Direction.Y < 0)
	return 3 * FPI / 2;
      else if(Direction.Y > 0)
	return FPI / 2;
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

bool femath::Clip(ushort& SourceX, ushort& SourceY, ushort& DestX, ushort& DestY, ushort& Width, ushort& Height, ushort XSize, ushort YSize, ushort DestXSize, ushort DestYSize)
{
  if(short(SourceX) < 0)
    {
      Width += SourceX;
      DestX -= SourceX;
      SourceX = 0;
    }

  if(short(SourceY) < 0)
    {
      Height += SourceY;
      DestY -= SourceY;
      SourceY = 0;
    }

  if(short(DestX) < 0)
    {
      Width += DestX;
      SourceX -= DestX;
      DestX = 0;
    }

  if(short(DestY) < 0)
    {
      Height += DestY;
      SourceY -= DestY;
      DestY = 0;
    }

  if(short(SourceX) + Width > XSize)
    Width = XSize - SourceX;

  if(short(SourceY) + Height > YSize)
    Height = YSize - SourceY;

  if(short(DestX) + Width > DestXSize)
    Width = DestXSize - DestX;

  if(short(DestY) + Height > DestYSize)
    Height = DestYSize - DestY;

  return short(Width) > 0 && short(Height) > 0;
}

/* This allows ordering of POD objects whose structure members are not aligned */

bool femath::CompareBits(const void* V1, const void* V2, ushort Size)
{
  const char* Ptr1 = reinterpret_cast<const char*>(V1);
  const char* Ptr2 = reinterpret_cast<const char*>(V2);

  for(ushort c = 0; c < Size; ++c, ++Ptr1, ++Ptr2)
    if(*Ptr1 != *Ptr2)
      return *Ptr1 < *Ptr2;

  return false;
}

void femath::SaveSeed()
{
  mtib = mti;

  for(ushort c = 0; c < N1; ++c)
    mtb[c] = mt[c];
}

void femath::LoadSeed()
{
  mti = mtib;

  for(ushort c = 0; c < N1; ++c)
    mt[c] = mtb[c];
}

void ReadData(interval& I, inputfile& SaveFile)
{
  I.Min = SaveFile.ReadNumber(HIGHEST, true);
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == ";" || Word == ",")
    I.Max = I.Min;
  else if(Word == ":")
    I.Max = Max(SaveFile.ReadNumber(), I.Min);
  else
    ABORT("Odd interval terminator %s detected, file %s line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
}

void ReadData(region& R, inputfile& SaveFile)
{
  ReadData(R.X, SaveFile);
  ReadData(R.Y, SaveFile);
}

outputfile& operator<<(outputfile& SaveFile, const interval& I)
{
  SaveFile.Write(reinterpret_cast<const char*>(&I), sizeof(I));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, interval& I)
{
  SaveFile.Read(reinterpret_cast<char*>(&I), sizeof(I));
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, const region& R)
{
  SaveFile.Write(reinterpret_cast<const char*>(&R), sizeof(R));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, region& R)
{
  SaveFile.Read(reinterpret_cast<char*>(&R), sizeof(R));
  return SaveFile;
}
