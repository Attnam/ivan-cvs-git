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

inputfile::inputfile(const std::string& FileName, const valuemap* ValueMap, bool AbortOnErr) : Buffer(fopen(FileName.c_str(), "rb")), FileName(FileName), ValueMap(ValueMap)
{
  if(AbortOnErr && !IsOpen())
    ABORT("File %s not found!", FileName.c_str());
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

      int Char = Get();

      if(isalpha(Char) || Char == '_')
	{
	  if(!Mode)
	    Mode = 1;
	  else if(Mode == 2)
	    {
	      UnGet(Char);
	      return;
	    }

	  Buffer += char(Char);
	  continue;
	}

      if(isdigit(Char))
	{
	  if(!Mode)
	    Mode = 2;
	  else if(Mode == 1)
	    {
	      UnGet(Char);
	      return;
	    }

	  Buffer += char(Char);
	  continue;
	}

      if((Char == ' ' || Char == '\n') && Mode)
	return;

      if(ispunct(Char))
	{
	  if(Char == '/')
	    {
	      if(!Eof())
		{
		  Char = Get();

		  if(Char == '*')
		    {
		      long StartPos = TellPos();
		      int OldChar = 0;
		      ushort CommentLevel = 1;

		      for(;;)
			{
			  if(Eof())
			    ABORT("Unterminated comment in file %s, beginning at line %d!", FileName.c_str(), TellLineOfPos(StartPos));

			  Char = Get();

			  if(OldChar != '*' || Char != '/')
			    {
			      if(OldChar != '/' || Char != '*')
				OldChar = Char;
			      else
				{
				  ++CommentLevel;
				  OldChar = 0;
				}
			    }
			  else
			    {
			      if(!--CommentLevel)
				break;
			      else
				OldChar = 0;
			    }
			}

		      continue;
		    }
		  else
		    UnGet(Char);
		}

	      if(Mode)
		UnGet('/');
	      else
		Buffer += '/';

	      return;
	    }

	  if(Mode)
	    {
	      UnGet(Char);
	      return;
	    }

	  if(Char == '"')
	    {
	      long StartPos = TellPos();
	      int OldChar = 0;

	      for(;;)
		{
		  if(Eof())
		    ABORT("Unterminated string in file %s, beginning at line %d!", FileName.c_str(), TellLineOfPos(StartPos));

		  Char = Get();

		  if(Char != '"')
		    {
		      Buffer += char(Char);
		      OldChar = Char;
		    }
		  else if(OldChar == '\\')
		    {
		      Buffer[Buffer.length() - 1] = '"';
		      OldChar = 0;
		    }
		  else
		    return;
		}
	    }

	  Buffer += char(Char);
	  return;
	}
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
		{
		  Char = Get();

		  if(Char == '*')
		    {
		      long StartPos = TellPos();
		      int OldChar = 0;
		      ushort CommentLevel = 1;

		      for(;;)
			{
			  if(Eof())
			    ABORT("Unterminated comment in file %s, beginning at line %d!", FileName.c_str(), TellLineOfPos(StartPos));

			  Char = Get();

			  if(OldChar != '*' || Char != '/')
			    {
			      if(OldChar != '/' || Char != '*')
				OldChar = Char;
			      else
				{
				  ++CommentLevel;
				  OldChar = 0;
				}
			    }
			  else
			    {
			      if(!--CommentLevel)
				break;
			      else
				OldChar = 0;
			    }
			}

		      continue;
		    }
		  else
		    UnGet(Char);
		}

	      return '/';
	    }

	  return Char;
	}
    }
}

/*
 * Reads a number or a formula from inputfile. Valid values could be for instance "3", "5 * 4+5", "2+rand%4" etc.
 */

long inputfile::ReadNumber(uchar CallLevel)
{
  long Value = 0;
  std::string Word;
  bool NumberCorrect = false;

  for(;;)
    {
      ReadWord(Word);

      if(isdigit(Word[0]))
	{
	  Value = atoi(Word.c_str());
	  NumberCorrect = true;
	  continue;
	}

      if(Word.length() == 1)
	{
	  if(Word[0] == ';' || Word[0] == ',' || Word[0] == ')')
	    {
	      if(CallLevel != 0xFF && (Word[0] != ')' || CallLevel != 4))
		SeekPosCurrent(-1);

	      return Value;
	    }

	  /* Convert this into an inline function! */

	  #define CHECK_OP(op, cl)\
	  if(Word[0] == #op[0])\
	    if(cl < CallLevel)\
	      {\
		Value op##= ReadNumber(cl);\
		NumberCorrect = true;\
		continue;\
	      }\
	    else\
	      {\
		SeekPosCurrent(-1);\
		return Value;\
	      }

	  CHECK_OP(&, 1); CHECK_OP(|, 1); CHECK_OP(^, 1);
	  CHECK_OP(*, 2); CHECK_OP(/, 2); CHECK_OP(%, 2);
	  CHECK_OP(+, 3); CHECK_OP(-, 3);

	  if(Word[0] == '(')
	    if(NumberCorrect)
	      {
		SeekPosCurrent(-1);
		return Value;
	      }
	    else
	      {
		Value = ReadNumber(4);
		NumberCorrect = false;
		continue;
	      }

	  if(Word[0] == '=' && CallLevel == 0xFF)
	    continue;

	  if(Word[0] == '#') // for #defines
	    {
	      SeekPosCurrent(-1);
	      return Value;
	    }
	}

      if(Word == "rand")
	{
	  Value = RAND() & 0x7FFFFFFF;
	  NumberCorrect = true;
	  continue;
	}

      if(Word == "rgb")
	{
	  ushort Bits = ReadNumber();

	  if(Bits == 16)
	    {
	      ushort Red = ReadNumber();
	      ushort Green = ReadNumber();
	      ushort Blue = ReadNumber();
	      Value = MakeRGB16(Red, Green, Blue);
	    }
	  else if(Bits == 24)
	    {
	      ulong Red = ReadNumber();
	      ulong Green = ReadNumber();
	      ulong Blue = ReadNumber();
	      Value = MakeRGB24(Red, Green, Blue);
	    }
	  else
	    ABORT("Illegal RGB bit size %d in file %s, line %d!", Bits, FileName.c_str(), TellLine());

	  NumberCorrect = true;
	  continue;
	}

      if(ValueMap)
	{
	  valuemap::const_iterator Iterator = ValueMap->find(Word);

	  if(Iterator != ValueMap->end())
	    {
	      Value = Iterator->second;
	      NumberCorrect = true;
	      continue;
	    }
	}

      ABORT("Odd numeric value \"%s\" encountered in file %s, line %d!", Word.c_str(), FileName.c_str(), TellLine());
    }
}

vector2d inputfile::ReadVector2d()
{
  vector2d Vector;
  Vector.X = ReadNumber();
  Vector.Y = ReadNumber();
  return Vector;
}

rect inputfile::ReadRect()
{
  rect Rect;
  Rect.X1 = ReadNumber();
  Rect.Y1 = ReadNumber();
  Rect.X2 = ReadNumber();
  Rect.Y2 = ReadNumber();
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

void ReadData(std::string& String, inputfile& SaveFile)
{
  SaveFile.ReadWord(String);

  if(String == "=")
    SaveFile.ReadWord(String);

  SaveFile.ReadWord();
}

void ReadData(std::vector<long>& Vector, inputfile& SaveFile)
{
  Vector.clear();
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    SaveFile.ReadWord(Word);

  if(Word == "=")
    {
      Vector.push_back(SaveFile.ReadNumber());
      return;
    }

  if(Word != "{")
    ABORT("Array syntax error \"%s\" found in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());

  ushort Size = SaveFile.ReadNumber();

  for(ushort c = 0; c < Size; ++c)
    Vector.push_back(SaveFile.ReadNumber());

  if(SaveFile.ReadWord() != "}")
    ABORT("Illegal array terminator \"%s\" encountered in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
}

void ReadData(std::vector<std::string>& Vector, inputfile& SaveFile)
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

  ushort Size = SaveFile.ReadNumber();

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
  SeekPosBegin(0);

  while(TellPos() != Pos)
    if(Get() == '\n')
      ++Line;

  if(TellPos() != BackupPos)
    SeekPosBegin(BackupPos);

  return Line;
}
