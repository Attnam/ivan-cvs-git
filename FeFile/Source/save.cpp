#include <ctype.h>
#include <string>

#include "save.h"
#include "strover.h"
#include "error.h"
#include "femath.h"

//#ifdef __SAVE_H__

outputfile::outputfile(std::string FileName, bool AbortOnErr) : Buffer(fopen(FileName.c_str(), "wb"))
{
  if(AbortOnErr && !IsOpen())
    ABORT("Can't open %s for output!", FileName.c_str());
}

inputfile::inputfile(std::string FileName, bool AbortOnErr) : Buffer(fopen(FileName.c_str(), "rb"))
{
  if(AbortOnErr && !IsOpen())
    ABORT("File %s not found!", FileName.c_str());
}

bool inputfile::Eof()
{
  int Char = fgetc(Buffer);

  if(feof(Buffer) != 0)
    return true;
  else
    {
      ungetc(Char, Buffer);
      return false;
    }
}

int inputfile::Peek()
{
  int Char = fgetc(Buffer);
  ungetc(Char, Buffer);
  return Char;
}

/*#else

outputfile::outputfile(std::string FileName, bool AbortOnErr) : Buffer(FileName.c_str(), std::ios::out | std::ios::binary)
{
	if(AbortOnErr && !IsOpen())
		ABORT("Can't open %s for output!", FileName.c_str());
}

inputfile::inputfile(std::string FileName, bool AbortOnErr) : Buffer(FileName.c_str(), std::ios::in | std::ios::binary)
{
	if(AbortOnErr && !IsOpen())
		ABORT("File %s not found!", FileName.c_str());
}

#endif*/

std::string inputfile::ReadWord(bool AbortOnEOF)
{
  uchar Mode = 0;

  std::string Buffer;

  for(;;)
    {
      if(Eof())
	{
	  if(AbortOnEOF)
	    ABORT("Unexpected end of script file!");

	  return Buffer;
	}

      int Char = Peek();

      if(isalpha(Char))
	{
	  if(!Mode)
	    Mode = 1;
	  else
	    if(Mode == 2)
	      return Buffer;

	  Buffer += char(Char);
	}

      if(Char == ' ' && Mode)
	return Buffer;

      if(isdigit(Char))
	{
	  if(!Mode)
	    Mode = 2;
	  else
	    if(Mode == 1)
	      return Buffer;

	  Buffer += char(Char);
	}

      if(ispunct(Char))
	{
	  if(Char == '/')
	    {
	      Get();

	      if(!Eof())
		if(Peek() == '*')
		  {
		    for(;;)
		      {
			Char = Get();

			if(Eof())
			  ABORT("Script error: Unterminated comment!");

			if(Char == '*' && Peek() == '/')
			  {
			    Get();

			    if(Mode == 2)
			      return Buffer;
			    else
			      break;
			  }		
		      }

		    continue;
		  }

	      if(Mode)
		return Buffer;

	      Buffer += char(Char);
	      return Buffer;
	    }

	  if(Mode)
	    return Buffer;

	  if(Char == '"')
	    {
	      Get();

	      if(Eof())
		ABORT("Script error: Unterminated comment");

	      if(Peek() == '"')
		{
		  Get();
		  return Buffer;
		}

	      for(;;)
		{
		  Char = Get();

		  if(Eof())
		    ABORT("Script error: Unterminated comment");

		  if(Peek() == '"')
		    if(Char == '\\')
		      {
			Buffer += '"';
			Get();

			if(Peek() == '"')
			  {
			    Get();
			    break;
			  }
		      }
		    else
		      {
			Buffer += char(Char);
			Get();
			break;
		      }
		  else
		    Buffer += char(Char);
		}

	      return Buffer;
	    }

	  Buffer += char(Char);
	  Get();
	  return Buffer;
	}

      Get();
    }
}

char inputfile::ReadLetter(bool AbortOnEOF)
{
  for(;;)
    {
      if(Eof())
	{
	  if(AbortOnEOF)
	    ABORT("Unexpected end of script file!");

	  return 0;
	}

      int Char = Get();

      if(isalpha(Char) || isdigit(Char))
	{
	  return Char;
	}

      if(ispunct(Char))
	{
	  if(Char == '/')
	    {
	      if(!Eof())
		if(Peek() == '*')
		  {
		    for(;;)
		      {
			Char = Get();

			if(Eof())
			  ABORT("Script error: Unterminated comment!");

			if(Char == '*' && Peek() == '/')
			  {
			    Get();
			    break;
			  }		
		      }

		    continue;
		  }
		else
		  return Char;
	    }
	  else
	    return Char;
	}
    }
}

long inputfile::ReadNumber(std::map<std::string, long> ValueMap, uchar CallLevel)
{
  long Value = 0;

  for(;;)
    {
      std::string Word = ReadWord();

      if(isdigit(Word[0]))
	{
	  Value = atoi(Word.c_str());
	  continue;
	}

      if(Word == ";" || Word == ",")
	{
	  if(CallLevel != 0xFF)
	    SeekPosCur(-1);

	  return Value;
	}

#define CHECK_OP(op, cl)\
		\
		if(Word == #op)\
			if(cl < CallLevel)\
			{\
				Value op##= ReadNumber(ValueMap, cl);\
				continue;\
			}\
			else\
			{\
				SeekPosCur(-1);\
				return Value;\
			}

      CHECK_OP(&, 1)	CHECK_OP(|, 1)	CHECK_OP(^, 1)
	CHECK_OP(*, 2)	CHECK_OP(/, 2)	CHECK_OP(%, 2)
	CHECK_OP(+, 3)	CHECK_OP(-, 3)

	if(Word == "(")
	  {
	    Value = ReadNumber(ValueMap, 4);
	    continue;
	  }

      if(Word == ")")
	return Value;

      if(Word == "rand")
	{
	  Value = RAND();
	  continue;
	}

      std::map<std::string, long>::iterator Iterator = ValueMap.find(Word);

      if(Iterator != ValueMap.end())
	{
	  Value = Iterator->second;
	  continue;
	}

      if(Word == "=" && CallLevel == 0xFF)
	continue;

      ABORT("Odd script value \"%s\" encountered!", Word.c_str());
    }
}

vector2d inputfile::ReadVector2d(std::map<std::string, long> ValueMap)
{
  vector2d Vector;

  Vector.X = ReadNumber(ValueMap);
  Vector.Y = ReadNumber(ValueMap);

  return Vector;
}

bool inputfile::ReadBool()
{
  std::string Word = ReadWord();

  if(Word == "=")
    Word = ReadWord();

  if(ReadWord() != ";")
    ABORT("Bool value terminated incorrectly!");

  if(Word == "true" || Word == "1")
    return true;

  if(Word == "false" || Word == "0")
    return false;

  ABORT("Odd bool value \"%s\" encountered!", Word.c_str());

  return RAND() % 2 ? true : false;
}

outputfile& operator<<(outputfile& SaveFile, std::string String)
{
  uchar Length = String.length();

  SaveFile << Length;

  if(Length)
    SaveFile.Write(String.c_str(), Length);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, std::string& String)
{
  char Buffer[256];

  uchar Length;

  SaveFile >> Length;

  if(Length)
    {
      SaveFile.Read(Buffer, Length);
      Buffer[Length] = 0;
      String = Buffer;
    }
  else
    String = "";

  return SaveFile;
}
