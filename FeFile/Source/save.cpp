#include <cctype>
#include <string>

#include "save.h"
#include "strover.h"
#include "error.h"

std::string inputfile::ReadWord()
{
	uchar Mode = 0;

	std::string Buffer;

	for(;;)
	{
		if(GetBuffer().eof())
			return Buffer;

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
					return Buffer;

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

							if(Char == '*')
							{
								if(GetBuffer().peek() == '/')
								{
									GetBuffer().get();

									if(Mode == 2)
										return Buffer;
									else
										break;
								}
							}		
						}

						continue;
					}

				if(Mode == 1 || Mode == 2)
					return Buffer;

				Buffer += char(Char);
			}
			else
			{
				if(Mode == 1 || Mode == 2)
					return Buffer;

				Buffer += char(Char);
				GetBuffer().get();
			}

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

		if(Word == ";")
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

		ABORT("Odd script value \"%s\" encountered!", Word.c_str());
	}
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
