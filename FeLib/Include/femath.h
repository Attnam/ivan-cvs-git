#ifndef __FEMATH_H__
#define __FEMATH_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "typedef.h"
#include "vector2d.h"
#include "felibdef.h"

#define RAND femath::Rand
#define RAND_N femath::RandN
#define RAND_2 long(femath::Rand() & 1)
#define RAND_4 long(femath::Rand() & 3)
#define RAND_8 long(femath::Rand() & 7)
#define RAND_16 long(femath::Rand() & 15)
#define RAND_32 long(femath::Rand() & 31)
#define RAND_64 long(femath::Rand() & 63)
#define RAND_128 long(femath::Rand() & 127)
#define RAND_256 long(femath::Rand() & 255)

template <class type> inline type HypotSquare(const type& X, const type& Y) { return X * X + Y * Y; }
template <class type> inline const type& Max(const type& X, const type& Y) { return X >= Y ? X : Y; }
template <class type> inline const type& Max(const type& X, const type& Y, const type& Z) { return Max(Max(X, Y), Z); }
template <class type> inline const type& Min(const type& X, const type& Y) { return X <= Y ? X : Y; }
template <class type> inline const type& Min(const type& X, const type& Y, const type& Z) { return Min(Min(X, Y), Z); }
template <class type> inline const type& Limit(const type& Value, const type& Minimum, const type& Maximum) { return Min(Max(Value, Minimum), Maximum); }
template <class f, class type> inline type Recurse(f F, type Value, ushort Amount) { return Amount ? Recurse(F, F(Value), Amount - 1) : Value; }

/* This allows ordering of POD objects whose structure members are not aligned */

template <class type> inline bool CompareBits(const type& V1, const type& V2)
{
  const char* Ptr1 = reinterpret_cast<const char*>(&V1);
  const char* Ptr2 = reinterpret_cast<const char*>(&V2);

  for(ushort c = 0; c < sizeof(type); ++c, ++Ptr1, ++Ptr2)
    if(*Ptr1 != *Ptr2)
      return *Ptr1 < *Ptr2;

  return false;
}

struct rect
{
  rect() { }
  rect(short X1, short Y1, short X2, short Y2) : X1(X1), Y1(Y1), X2(X2), Y2(Y2) { }
  rect operator+(const vector2d& Vector) { return rect(X1 + Vector.X, Y1 + Vector.Y, X2 + Vector.X, Y2 + Vector.Y); }
  short X1, Y1, X2, Y2;
};

class femath
{
public: 
  static ulong Rand() { return (Seed = Seed * RAND_MULTIPLIER + 1) >> 21; }
  static long RandN(long N) { return Rand() % N; }
  static ulonglong GetSeed() { return Seed; }
  static void SetSeed(ulonglong What) { Seed = What; }
  static bool DoLine(long, long, long, long, bool (*Proc)(long, long));
  static ushort WeightedRand(const std::vector<long>&);
  static float CalculateAngle(vector2d);
  static void CalculateEnvironmentRectangle(rect&, const rect&, vector2d, ushort);
  static bool Clip(ushort&, ushort&, ushort&, ushort&, ushort&, ushort&, ushort, ushort, ushort, ushort);
protected:
  static ulonglong Seed;
};

#endif
