#include <cctype>
#include <string>

#include "save.h"
#include "strover.h"
#include "error.h"

std::string inputfile::ReadWord(bool AbortOnEOF)
{
	uchar Mode = 0;

	std::string Buffer;

	for(;;)
	{
		if(GetBuffer().eof())
		{
			if(AbortOnEOF)
				ABORT("Unexpected end of script file!");

			for(short c = Buffer.length() - 1; Buffer[c] == ' '; c--)
				Buffer.resize(Buffer.length() - 1);

			return Buffer;
		}

		int Char = GetBuffer().peek();

		if(isalpha(Char))
		{
			if(!Mode)
				Mode = 1;
			else
				if(Mode == 2)
					return Buffer;

			Buffer += char(Char);
		}

		if(Char == ' ')
		{
			if(Mode == 1)
				Buffer += char(Char);

			if(Mode == 2)
				return Buffer;
		}

		if(isdigit(Char))
		{
			if(!Mode)
				Mode = 2;
			else
				if(Mode == 1)
				{
					for(short c = Buffer.length() - 1; Buffer[c] == ' '; c--)
						Buffer.resize(Buffer.length() - 1);

					return Buffer;
				}

			Buffer += char(Char);
		}

		if(ispunct(Char))
		{
			if(Char == '/')
			{
				GetBuffer().get();

				if(!GetBuffer().eof())
					if(GetBuffer().peek() == '*')
					{
						for(;;)
						{
							Char = GetBuffer().get();

							if(GetBuffer().eof())
								ABORT("Script error: Unterminated comment!");

							if(Char == '*' && GetBuffer().peek() == '/')
							{
								GetBuffer().get();

								if(Mode == 2)
									return Buffer;
								else
									break;
							}		
						}

						continue;
					}

				if(Mode == 1)
				{
					for(short c = Buffer.length() - 1; Buffer[c] == ' '; c--)
						Buffer.resize(Buffer.length() - 1);

					return Buffer;
				}

				if(Mode == 2)
					return Buffer;

				Buffer += char(Char);
				return Buffer;
			}

			if(Mode == 2)
				return Buffer;

			if(Char == '"')
			{
				Mode = 1;

				GetBuffer().get();

				if(GetBuffer().eof())
					ABORT("Script error: Unterminated comment");

				for(;;)
				{
					Char = GetBuffer().get();

					if(GetBuffer().eof())
						ABORT("Script error: Unterminated comment");

					if(GetBuffer().peek() == '"')
						if(Char == '\\')
						{
							Buffer += '"';
							GetBuffer().get();

							if(GetBuffer().peek() == '"')
							{
								GetBuffer().get();
								break;
							}
						}
						else
						{
							Buffer += char(Char);
							GetBuffer().get();
							break;
						}
					else
						Buffer += char(Char);
				}

				continue;
			}

			if(Mode == 1)
			{
				for(short c = Buffer.length() - 1; Buffer[c] == ' '; c--)
					Buffer.resize(Buffer.length() - 1);

				return Buffer;
			}

			Buffer += char(Char);
			GetBuffer().get();
			return Buffer;
		}

		GetBuffer().get();
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
				GetBuffer().seekg(-1, std::ios::cur);

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
				GetBuffer().seekg(-1, std::ios::cur);\
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
			Value = rand();
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

	return rand() % 2 ? true : false;
}

outputfile& operator<<(outputfile& SaveFile, std::string String)
{
	uchar Length = String.length();

	SaveFile << Length;

	if(Length)
		SaveFile.GetBuffer().write(String.c_str(), Length);

	return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, std::string& String)
{
	char Buffer[256];

	uchar Length;

	SaveFile >> Length;

	if(Length)
	{
		SaveFile.GetBuffer().read(Buffer, Length);
		Buffer[Length] = 0;
		String = Buffer;
	}
	else
		String = "";

	return SaveFile;
}
