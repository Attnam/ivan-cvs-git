#ifndef __FEMATH_H__
#define __FEMATH_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif

#define RAND femath::Rand

#define N1 624

#define DO_FOR_SQUARES_AROUND(X, Y, BorderX, BorderY, Then)\
{\
  while(true)\
  {\
    ushort DoX, DoY, DoIndex = 0;\
    \
    if(X && Y)\
    {\
      DoX = X - 1; DoY = Y - 1;\
      \
      Then;\
    }\
    \
    ++DoIndex;\
    \
    if(Y)\
    {\
      DoX = X; DoY = Y - 1;\
      \
      Then;\
    }\
    \
    ++DoIndex;\
    \
    if(X < BorderX - 1 && Y)\
    {\
      DoX = X + 1; DoY = Y - 1;\
      \
      Then;\
    }\
    \
    ++DoIndex;\
    \
    if(X)\
    {\
      DoX = X - 1; DoY = Y;\
      \
      Then;\
    }\
    \
    ++DoIndex;\
    \
    if(X < BorderX - 1)\
    {\
      DoX = X + 1; DoY = Y;\
      \
      Then;\
    }\
    \
    ++DoIndex;\
    \
    if(X && Y < BorderY - 1)\
    {\
      DoX = X - 1; DoY = Y + 1;\
      \
      Then;\
    }\
    \
    ++DoIndex;\
    \
    if(Y < BorderY - 1)\
    {\
      DoX = X; DoY = Y + 1;\
      \
      Then;\
    }\
    \
    ++DoIndex;\
    \
    if(X < BorderX - 1 && Y < BorderY - 1)\
    {\
      DoX = X + 1; DoY = Y + 1;\
      \
      Then;\
    }\
    \
    break;\
  }\
}

#define DO_FOR_SQUARES_AROUND_IN_TWO_PARTS(X, Y, BorderX, BorderY, FirstCommand, SecondCommand, CommonCommand)\
{\
  while(true)\
  {\
    ushort DoX, DoY, DoIndex = 0;\
    \
    if(X && Y)\
    {\
      DoX = X - 1; DoY = Y - 1;\
      \
      FirstCommand;\
      CommonCommand;\
    }\
    \
    ++DoIndex;\
    \
    if(Y)\
    {\
      DoX = X; DoY = Y - 1;\
      \
      FirstCommand;\
      CommonCommand;\
    }\
    \
    ++DoIndex;\
    \
    if(X < BorderX - 1 && Y)\
    {\
      DoX = X + 1; DoY = Y - 1;\
      \
      FirstCommand;\
      CommonCommand;\
    }\
    \
    ++DoIndex;\
    \
    if(X)\
    {\
      DoX = X - 1; DoY = Y;\
      \
      FirstCommand;\
      CommonCommand;\
    }\
    \
    ++DoIndex;\
    \
    if(X < BorderX - 1)\
    {\
      DoX = X + 1; DoY = Y;\
      \
      SecondCommand;\
      CommonCommand;\
    }\
    \
    ++DoIndex;\
    \
    if(X && Y < BorderY - 1)\
    {\
      DoX = X - 1; DoY = Y + 1;\
      \
      SecondCommand;\
      CommonCommand;\
    }\
    \
    ++DoIndex;\
    \
    if(Y < BorderY - 1)\
    {\
      DoX = X; DoY = Y + 1;\
      \
      SecondCommand;\
      CommonCommand;\
    }\
    \
    ++DoIndex;\
    \
    if(X < BorderX - 1 && Y < BorderY - 1)\
    {\
      DoX = X + 1; DoY = Y + 1;\
      \
      SecondCommand;\
      CommonCommand;\
    }\
    \
    break;\
  }\
}

#define DO_FILLED_RECTANGLE(CenterX, CenterY, ClipLeft, ClipTop, ClipRigth, ClipBottom, Radius, DoWhat)\
{\
  long	Left    = (CenterX) - (Radius),\
	Top     = (CenterY) - (Radius),\
	Rigth   = (CenterX) + (Radius),\
	Bottom  = (CenterY) + (Radius);\
  \
  if(Left   < (ClipLeft))   Left   = (ClipLeft);\
  if(Top    < (ClipTop))    Top    = (ClipTop);\
  if(Rigth  > (ClipRigth))  Rigth  = (ClipRigth);\
  if(Bottom > (ClipBottom)) Bottom = (ClipBottom);\
  \
  if(Left <= (ClipRigth) && Top <= (ClipBottom) && Rigth >= (ClipLeft) && Bottom >= (ClipTop))\
  {\
    ushort XPointer, YPointer;\
    \
    for(XPointer = Left; XPointer <= Rigth; ++XPointer)\
      for(YPointer = Top; YPointer <= Bottom; ++YPointer)\
      {\
        DoWhat;\
      }\
  }\
}

template <class type> inline type GetHypotSquare(type X, type Y) { return X * X + Y * Y; }
template <class type> inline type Max(type X, type Y) { return X > Y ? X : Y; }
template <class type> inline type Min(type X, type Y) { return X < Y ? X : Y; }

#include "typedef.h"
#include "vector2d.h"

class femath
{
public: 
  static long Rand();
  static void SetSeed(ulong);
  static bool DoLine(long, long, long, long, ulong, bool (*Proc)(vector2d, vector2d));
  static ushort WeightedRand(ushort, ushort*);
protected:
  static ulong mt[N1];
  static long mti;
};

#endif
