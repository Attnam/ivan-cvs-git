#include <cmath>

#include "femath.h"
#include "error.h"

ulonglong femath::Seed;

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

ulong femath::Rand() { return (Seed = Seed * RAND_MULTIPLIER + 1) >> 21; }
