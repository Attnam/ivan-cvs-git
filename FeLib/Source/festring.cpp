#include "festring.h"
#include "allocate.h"

namespace festring
{
  char** IntegerMap, IntegerBuffer[12];

  void InstallIntegerMap()
  {
    static bool AlreadyInstalled = false;

    if(!AlreadyInstalled)
      {
	AlreadyInstalled = true;
	IntegerMap = Alloc2D<char>(1000, 3);
	char Ones = '0', Tens = '0', Hundreds = '0';

	for(ushort c = 0; c < 1000; ++c)
	  {
	    IntegerMap[c][0] = Hundreds;
	    IntegerMap[c][1] = Tens;
	    IntegerMap[c][2] = Ones;

	    if(++Ones > '9')
	      {
		Ones = '0';

		if(++Tens > '9')
		  {
		    Tens = '0';
		    ++Hundreds;
		  }
	      }
	  }

	atexit(DeInstallIntegerMap);
      }
  }

  void DeInstallIntegerMap()
  {
    delete [] IntegerMap;
  }

  /*
   * Displays numbers in the range [-2147483647, 2147483647].
   * Much faster than sprintf and (nonstandard) itoa,
   * but note that it uses a global buffer, so you must
   * process the result of the last call before the next use.
   * And you must call InstallIntegerMap() before the first use.
   */

  char* IntegerToChar(long Integer)
  {
    char* BufferPtr = IntegerBuffer;
    bool Negative = false;

    if(Integer < 0)
      {
	if(Integer < -2147483647)
	  return strcpy(BufferPtr, "-Inf");

	*BufferPtr++ = '0';
	Integer = -Integer;
	Negative = true;
      }
    else if(Integer > 2147483647)
      return strcpy(BufferPtr, "Inf");

    if(Integer >= 2000000000)
      {
	*BufferPtr++ = '2';
	Integer -= 2000000000;
      }
    else if(Integer >= 1000000000)
      {
	*BufferPtr++ = '1';
	Integer -= 1000000000;
      }

    if(Integer >= 1000000)
      {
	ushort Temp = Integer / 1000000;
	*BufferPtr++ = IntegerMap[Temp][0];
	*BufferPtr++ = IntegerMap[Temp][1];
	*BufferPtr++ = IntegerMap[Temp][2];
	Integer -= Temp * 1000000;
      }

    if(Integer >= 1000)
      {
	ushort Temp = Integer / 1000;
	*BufferPtr++ = IntegerMap[Temp][0];
	*BufferPtr++ = IntegerMap[Temp][1];
	*BufferPtr++ = IntegerMap[Temp][2];
	Integer -= Temp * 1000;
      }

    *BufferPtr++ = IntegerMap[Integer][0];
    *BufferPtr++ = IntegerMap[Integer][1];
    *BufferPtr++ = IntegerMap[Integer][2];
    *BufferPtr = 0;

    for(BufferPtr = IntegerBuffer; *BufferPtr == '0'; ++BufferPtr);

    if(Negative)
      *--BufferPtr = '-';
    else if(!*BufferPtr)
      --BufferPtr;

    return BufferPtr;
  }
}
