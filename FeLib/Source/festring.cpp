#include <cstdarg>

#include "festring.h"
#include "allocate.h"
#include "error.h"

char** festring::IntegerMap = 0;
char festring::IntegerBuffer[12];

/*
 * Creates map of char representations of numbers 0-999 used by IntegerToChar.
 * Due to automatization, you don't need to explicitly call it.
 */

void festring::InstallIntegerMap()
{
  if(!IntegerMap) // check if already installed
    {
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

/*
 * Deletes the integer map used by IntegerToChar.
 * Due to automatization, you don't need to explicitly call it.
 */

void festring::DeInstallIntegerMap()
{
  delete [] IntegerMap;
  IntegerMap = 0;
}

/*
 * Displays numbers in the range [-2147483647, 2147483647].
 * Much faster than sprintf and (nonstandard) itoa,
 * but note that it uses a global buffer, so you must
 * process the result of the last call before the next use.
 */

char* festring::IntegerToChar(long Integer)
{
  if(!IntegerMap)
    InstallIntegerMap();

  char* BufferPtr = IntegerBuffer;
  bool Negative = false;

  if(Integer < 0)
    {
      if(Integer < -2147483647)
	return strncpy(BufferPtr, "-Inf", 5);

      *BufferPtr++ = '0';
      Integer = -Integer;
      Negative = true;
    }
  else if(Integer > 2147483647)
    return strncpy(BufferPtr, "Inf", 4);

  bool ForceZeros = false;

  if(Integer >= 2000000000)
    {
      *BufferPtr++ = '2';
      Integer -= 2000000000;
      ForceZeros = true;
    }
  else if(Integer >= 1000000000)
    {
      *BufferPtr++ = '1';
      Integer -= 1000000000;
      ForceZeros = true;
    }

  if(ForceZeros || Integer >= 1000000)
    {
      ushort Temp = Integer / 1000000;
      *BufferPtr++ = IntegerMap[Temp][0];
      *BufferPtr++ = IntegerMap[Temp][1];
      *BufferPtr++ = IntegerMap[Temp][2];
      Integer -= Temp * 1000000;
      ForceZeros = true;
    }

  if(ForceZeros || Integer >= 1000)
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
  else if(!*BufferPtr) // check if the original Integer was zero
    --BufferPtr;

  return BufferPtr;
}

/*
 * The Result string receives up to Length characters from source,
 * but words are left uncut if possible.
 */

void festring::SplitString(std::string& Source, std::string& Result, strsize Length)
{
  if(Source.length() <= Length)
    {
      Result += Source;
      Source.resize(0);
      return;
    }

  strsize Pos = Source.find_last_of(' ', Length);

  if(Pos != std::string::npos)
    {
      Result.append(Source, 0, Pos);
      Source.erase(0, Pos + 1);
    }
  else
    {
      Result.append(Source, 0, Length);
      Source.erase(0, Length);
    }
}

/*
 * Divides Source into lines of size up to Length without cutting words
 * and stores them one by one to StringVector. You can also specify a Marginal,
 * in which case a number of spaces is inserted in the beginning of each line
 * except the first. It returns the number of created lines.
 */

ushort festring::SplitString(const std::string& Source, std::vector<std::string>& StringVector, strsize Length, strsize Marginal)
{
  if(!Length)
    ABORT("Illegal Length 0 passed to festring::SplitString()!");

  if(Marginal >= Length)
    ABORT("Illegal festring::SplitString() call: Marginal must be less than Length!");

  std::string CopyOfSource(Source);

  if(StringVector.empty())
    StringVector.push_back(std::string());
  else
    StringVector[0].resize(0);

  SplitString(CopyOfSource, StringVector[0], Length);
  ushort Size = 1;

  while(CopyOfSource.length())
    {
      if(StringVector.size() <= Size)
	StringVector.push_back(std::string());

      std::string& String = StringVector[Size++];
      String.assign(Marginal, ' ');
      SplitString(CopyOfSource, String, Length - Marginal);
    }

  return Size;
}

/*
 * Returns the position of the first occurance of What in Where
 * starting at Begin or after it, ignoring the case of letters.
 * If the search fails, std::string::npos is returned instead.
 */

strsize festring::IgnoreCaseFind(const std::string& Where, const std::string& What, strsize Begin)
{
  if(What.empty())
   return Begin;

  for(; Where.size() >= What.size() + Begin; ++Begin)
    if(CapitalizeCopy(Where[Begin]) == CapitalizeCopy(What[0]))
      {
	bool Equal = true;

	for(strsize c = 1; c < What.size(); ++c)
	  if(CapitalizeCopy(Where[Begin + c]) != CapitalizeCopy(What[c]))
	    {
	      Equal = false;
	      break;
	    }

	if(Equal)
	  return Begin;
      }

  return std::string::npos;
}
