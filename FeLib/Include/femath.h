#ifndef __FEMATH_H__
#define __FEMATH_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "vector2d.h"
#include "rect.h"

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

class outputfile;
class inputfile;

class femath
{
public:
  static long Rand();
  static void SetSeed(ulong);
  static long RandN(long N) { return Rand() % N; }
  static bool DoLine(long, long, long, long, bool (*Proc)(long, long));
  static ushort WeightedRand(long*, ushort);
  static ushort WeightedRand(const std::vector<long>&);
  static float CalculateAngle(vector2d);
  static void CalculateEnvironmentRectangle(rect&, const rect&, vector2d, ushort);
  static bool Clip(ushort&, ushort&, ushort&, ushort&, ushort&, ushort&, ushort, ushort, ushort, ushort);
  static void SaveSeed();
  static void LoadSeed();
protected:
  static ulong mt[];
  static long mti;
  static ulong mtb[];
  static long mtib;
};

struct interval
{
  long Randomize() const { return Min < Max ? Min + RAND() % (Max - Min + 1) : Min; }
  long Min;
  long Max;
};

struct region
{
  vector2d Randomize() const { return vector2d(X.Randomize(), Y.Randomize()); }
  interval X;
  interval Y;
};

void ReadData(interval&, inputfile&);
void ReadData(region&, inputfile&);

outputfile& operator<<(outputfile&, const interval&);
inputfile& operator>>(inputfile&, interval&);
outputfile& operator<<(outputfile&, const region&);
inputfile& operator>>(inputfile&, region&);

#endif
