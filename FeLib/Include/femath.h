#ifndef __FEMATH_H__
#define __FEMATH_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "typedef.h"
#include "vector2d.h"

#define RAND femath::Rand

template <class type> inline type GetHypotSquare(const type& X, const type& Y) { return X * X + Y * Y; }
template <class type> inline const type& Max(const type& X, const type& Y) { return X > Y ? X : Y; }
template <class type> inline const type& Min(const type& X, const type& Y) { return X < Y ? X : Y; }
template <class type> inline const type& Limit(const type& Value, const type& Minimum, const type& Maximum) { return Min(Max(Value, Minimum), Maximum); }

struct rect
{
  rect() { }
  rect(short X1, short Y1, short X2, short Y2) : X1(X1), Y1(Y1), X2(X2), Y2(Y2) { }
  short X1, Y1, X2, Y2;
};

class femath
{
public: 
  static long Rand();
  static void SetSeed(ulong);
  static bool DoLine(long, long, long, long, bool (*Proc)(long, long));
  static ushort WeightedRand(const std::vector<long>&);
  static float CalculateAngle(vector2d);
  static void CalculateEnvironmentRectangle(rect&, const rect&, vector2d, ushort);
  static bool Clip(ushort&, ushort&, ushort&, ushort&, ushort&, ushort&, ushort, ushort, ushort, ushort);
protected:
  static ulong mt[];
  static long mti;
};

#endif

