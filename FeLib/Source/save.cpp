#include <ctype.h>

#include "save.h"
#include "festring.h"
#include "error.h"
#include "femath.h"
#include "graphics.h"
#include "felibdef.h"

outputfile::outputfile(const std::string& FileName, bool AbortOnErr) : Buffer(fopen(FileName.c_str(), "wb"))
{
  if(AbortOnErr && !IsOpen())
    ABORT("Can't open %s for output!", FileName.c_str());
}

inputfile::inputfile(const std::string& FileName, bool AbortOnErr) : Buffer(fopen(FileName.c_str(), "rb")), FileName(FileName)
{
  if(AbortOnErr && !IsOpen())
    ABORT("File %s not found!", FileName.c_str());
}

/* This function is a gum solution. */

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

std::string inputfile::ReadWord(bool AbortOnEOF)
{
  std::string ToReturn;
  ReadWord(ToReturn, AbortOnEOF);
  return ToReturn;
}

void inputfile::ReadWord(std::string& Buffer, bool AbortOnEOF)
{
  uchar Mode = 0;
  Buffer.resize(0);

  for(;;)
    {
      if(Eof())
	{
	  if(AbortOnEOF)
	    ABORT("Unexpected end of file %s!", FileName.c_str());

	  return;
	}

      int Char = Peek();

      if(isalpha(Char) || Char == '_')
	{
	  if(!Mode)
	    Mode = 1;
	  else
	    if(Mode == 2)
	      return;

	  Buffer += char(Char);
	}

      if(Char == ' ' && Mode)
	return;

      if(isdigit(Char))
	{
	  if(!Mode)
	    Mode = 2;
	  else
	    if(Mode == 1)
	      return;

	  Buffer += char(Char);
	}

      if(ispunct(Char) && Char != '_')
	{
	  if(Char == '/')
	    {
	      Get();

	      if(!Eof())
		if(Peek() == '*')
		  {
		    long StartPos = TellPos();

		    for(;;)
		      {
			Char = Get();

			if(Eof())
			  ABORT("Unterminated comment in file %s, beginning at line %d!", FileName.c_str(), TellLineOfPos(StartPos));

			if(Char == '*' && Peek() == '/')
			  {
			    Get();

			    if(Mode == 2)
			      return;
			    else
			      break;
			  }		
		      }

		    continue;
		  }

	      if(Mode)
		return;

	      Buffer += char(Char);
	      return;
	    }

	  if(Mode)
	    return;

	  if(Char == '"')
	    {
	      long StartPos = TellPos();
	      Get();

	      if(Eof())
		ABORT("Unterminated string in file %s, beginning at line %d!", FileName.c_str(), TellLineOfPos(StartPos));

	      if(Peek() == '"')
		{
		  Get();
		  return;
		}

	      for(;;)
		{
		  Char = Get();

		  if(Eof())
		    ABORT("Unterminated string in file %s, beginning at line %d!", FileName.c_str(), TellLineOfPos(StartPos));

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

	      return;
	    }

	  Buffer += char(Char);
	  Get();
	  return;
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
	    ABORT("Unexpected end of file %s!", FileName.c_str());

	  return 0;
	}

      int Char = Get();

      if(isalpha(Char) || isdigit(Char))
	return Char;

      if(ispunct(Char))
	{
	  if(Char == '/')
	    {
	      if(!Eof())
		if(Peek() == '*')
		  {
		    long StartPos = TellPos();

		    for(;;)
		      {
			Char = Get();

			if(Eof())
			  ABORT("Unterminated comment in file %s, beginning at line %d!", FileName.c_str(), TellLineOfPos(StartPos));

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

/*
 * Reads a number or a formula from inputfile. Valid values could be for instance "3", "5 * 4+5", "2+rand%4" etc.
 * ValueMap contains keyword pairs that attach a certain numeric value to a word.
 */

long inputfile::ReadNumber(const valuemap& ValueMap, uchar CallLevel)
{
  long Value = 0;
  std::string Word;

  for(;;)
    {
      ReadWord(Word);

      if(isdigit(Word[0]))
	{
	  Value = atoi(Word.c_str());
	  continue;
	}

      if(Word == ";" || Word == "," || Word == ")")
	{
	  if(CallLevel != 0xFF && (Word != ")" || CallLevel != 4))
	    SeekPosCur(-1);

	  return Value;
	}

      /* Convert this into an inline function! */

      #define CHECK_OP(op, cl)\
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

      CHECK_OP(&, 1); CHECK_OP(|, 1); CHECK_OP(^, 1);
      CHECK_OP(*, 2); CHECK_OP(/, 2); CHECK_OP(%, 2);
      CHECK_OP(+, 3); CHECK_OP(-, 3);

      if(Word == "(")
	{
	  Value = ReadNumber(ValueMap, 4);
	  continue;
	}

      if(Word == "rand")
	{
	  Value = RAND();
	  continue;
	}

      if(Word == "rgb")
	{
	  ushort Red = ReadNumber(ValueMap);
	  ushort Green = ReadNumber(ValueMap);
	  ushort Blue = ReadNumber(ValueMap);
	  Value = MakeRGB(Red, Green, Blue);
	  continue;
	}

      valuemap::const_iterator Iterator = ValueMap.find(Word);

      if(Iterator != ValueMap.end())
	{
	  Value = Iterator->second;
	  continue;
	}

      if(Word == "=" && CallLevel == 0xFF)
	continue;

      if(Word == "#") // for #defines
	{
	  SeekPosCur(-1);
	  return Value;
	}

      ABORT("Odd numeric value \"%s\" encountered in file %s, line %d!", Word.c_str(), FileName.c_str(), TellLine());
    }
}

vector2d inputfile::ReadVector2d(const valuemap& ValueMap)
{
  vector2d Vector;
  Vector.X = ReadNumber(ValueMap);
  Vector.Y = ReadNumber(ValueMap);
  return Vector;
}

rect inputfile::ReadRect(const valuemap& ValueMap)
{
  rect Rect;
  Rect.X1 = ReadNumber(ValueMap);
  Rect.Y1 = ReadNumber(ValueMap);
  Rect.X2 = ReadNumber(ValueMap);
  Rect.Y2 = ReadNumber(ValueMap);
  return Rect;
}

bool inputfile::ReadBool()
{
  std::string Word;
  ReadWord(Word);

  if(Word == "=")
    ReadWord(Word);

  if(ReadWord() != ";")
    ABORT("Bool value terminated incorrectly in file %s, line %d!", FileName.c_str(), TellLine());

  if(Word == "true" || Word == "1")
    return true;

  if(Word == "false" || Word == "0")
    return false;

  ABORT("Odd bool value \"%s\" encountered in file %s, line %d!", Word.c_str(), FileName.c_str(), TellLine());
  return false;
}

outputfile& operator<<(outputfile& SaveFile, const std::string& String)
{
  uchar Length = String.length();
  SaveFile << Length;

  if(Length)
    SaveFile.Write(String.c_str(), Length);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, std::string& String)
{
  char* RealBuffer, StackBuffer[1024];
  uchar Length;
  SaveFile >> Length;

  RealBuffer = Length < 1024 ? StackBuffer : new char[Length + 1];

  if(Length)
    {
      SaveFile.Read(RealBuffer, Length);
      RealBuffer[Length] = 0;
      String = RealBuffer;
    }
  else
    String.resize(0);

  if(Length >= 1024)
    delete [] RealBuffer;

  return SaveFile;
}

/* Little easier-to-call version of the ReadNumber routine */

long inputfile::ReadNumber()
{
  return ReadNumber(valuemap());
}

void ReadData(std::string& String, inputfile& SaveFile, const valuemap&)
{
  SaveFile.ReadWord(String);

  if(String == "=")
    SaveFile.ReadWord(String);

  SaveFile.ReadWord();
}

void ReadData(std::vector<long>& Vector, inputfile& SaveFile, const valuemap& ValueMap)
{
  Vector.clear();
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    SaveFile.ReadWord(Word);

  if(Word == "=")
    {
      Vector.push_back(SaveFile.ReadNumber(ValueMap));
      return;
    }

  if(Word != "{")
    ABORT("Array syntax error \"%s\" found in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());

  ushort Size = SaveFile.ReadNumber(ValueMap);

  for(ushort c = 0; c < Size; ++c)
    Vector.push_back(SaveFile.ReadNumber(ValueMap));

  if(SaveFile.ReadWord() != "}")
    ABORT("Illegal array terminator \"%s\" encountered in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
}

void ReadData(std::vector<std::string>& Vector, inputfile& SaveFile, const valuemap& ValueMap)
{
  Vector.clear();
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    SaveFile.ReadWord(Word);

  if(Word == "=")
    {
      Vector.push_back(SaveFile.ReadWord());

      if(SaveFile.ReadWord() != ";")
	ABORT("Array syntax error \"%s\" found in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());

      return;
    }

  if(Word != "{")
    ABORT("Array syntax error \"%s\" found in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());

  ushort Size = SaveFile.ReadNumber(ValueMap);

  for(ushort c = 0; c < Size; ++c)
    {
      Vector.push_back(SaveFile.ReadWord());
      SaveFile.ReadWord(Word);

      if(Word != "," && Word != ";")
	ABORT("Array syntax error \"%s\" found in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
    }

  if(SaveFile.ReadWord() != "}")
    ABORT("Illegal array terminator \"%s\" encountered in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
}

ulong inputfile::TellLineOfPos(long Pos)
{
  ulong Line = 1;
  long BackupPos = TellPos();
  SeekPosBeg(0);

  while(TellPos() != Pos)
    if(Get() == '\n')
      ++Line;

  if(TellPos() != BackupPos)
    SeekPosBeg(BackupPos);

  return Line;
}

